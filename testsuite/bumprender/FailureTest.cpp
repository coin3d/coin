#include "AllocationFailure.h"
#include "TestAdapter.h"
#include <Inventor/SbTime.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/sensors/SoNodeSensor.h>
#include <Inventor/sensors/SoSensorManager.h>
#include <cstring>
#include <thread>
#include <atomic>

namespace {
enum Failure { NONE, SAVE, GENERATE, ZERO_NAME, BIND, UPLOAD, LENGTH, ZERO_LENGTH, RESTORE };
void (*warningCallback)() = NULL;
const char * programErrorString = "injected upload failure";
struct MockGL {
  MockGL() : failure(NONE), failupload(1), error(GL_NO_ERROR), list(0), next(100),
             generated(0), uploads(0), deleted(0), warnings(0) { }
  std::mutex mutex;
  Failure failure;
  int failupload;
  GLenum error;
  GLint list;
  GLuint next;
  int generated, uploads, deleted, warnings;
  std::map<std::pair<uint32_t, GLenum>, GLuint> bindings;
  std::map<std::pair<uint32_t, GLuint>, GLint> lengths;
  std::vector<std::pair<uint32_t, GLuint> > deletions;
} mock;
uint32_t context() { return BumpTestCacheContext::current(); }
GLenum testGetError() {
  std::lock_guard<std::mutex> lock(mock.mutex);
  const GLenum error = mock.error; mock.error = GL_NO_ERROR; return error;
}
void testGetIntegerv(GLenum pname, GLint * value) {
  std::lock_guard<std::mutex> lock(mock.mutex);
  *value = pname == GL_LIST_INDEX ? mock.list : -1;
}
const GLubyte * testGetString(GLenum) { return (const GLubyte *) programErrorString; }
const cc_glglue * testGlueInstance(int) { return (const cc_glglue *) 1; }
void testGetProgramiv(const cc_glglue *, GLenum target, GLenum pname, GLint * value) {
  std::lock_guard<std::mutex> lock(mock.mutex);
  if (pname == GL_PROGRAM_BINDING_ARB) {
    if (mock.failure == SAVE) { mock.error = GL_INVALID_OPERATION; return; }
    *value = (GLint) mock.bindings[std::make_pair(context(), target)];
  }
  else {
    if (mock.failure == LENGTH) { mock.error = GL_INVALID_OPERATION; return; }
    *value = mock.failure == ZERO_LENGTH ? 0 :
      mock.lengths[std::make_pair(context(), mock.bindings[std::make_pair(context(), target)])];
  }
}
void testGenPrograms(const cc_glglue *, GLsizei n, GLuint * ids) {
  std::lock_guard<std::mutex> lock(mock.mutex);
  if (mock.failure == GENERATE) { mock.error = GL_OUT_OF_MEMORY; return; }
  if (mock.failure == ZERO_NAME) return;
  while (n--) { *ids++ = ++mock.next; ++mock.generated; }
}
void testBindProgram(const cc_glglue *, GLenum target, GLuint id) {
  std::lock_guard<std::mutex> lock(mock.mutex);
  if ((mock.failure == BIND && id > 100) || (mock.failure == RESTORE && id <= 100)) {
    mock.error = GL_INVALID_OPERATION; return;
  }
  mock.bindings[std::make_pair(context(), target)] = id;
}
void testProgramString(const cc_glglue *, GLenum target, GLenum, GLsizei length, const GLvoid *) {
  std::lock_guard<std::mutex> lock(mock.mutex);
  ++mock.uploads;
  if (mock.failure == UPLOAD && mock.uploads == mock.failupload) {
    mock.error = GL_INVALID_OPERATION; return;
  }
  mock.lengths[std::make_pair(context(), mock.bindings[std::make_pair(context(), target)])] = length;
}
void testDeletePrograms(const cc_glglue *, GLsizei n, const GLuint * ids) {
  std::lock_guard<std::mutex> lock(mock.mutex);
  while (n--) {
    const std::pair<uint32_t, GLuint> key(context(), *ids++);
    CHECK(mock.lengths.erase(key) != 0 || mock.failure != NONE);
    mock.deletions.push_back(key); ++mock.deleted;
    for (std::map<std::pair<uint32_t, GLenum>, GLuint>::iterator it = mock.bindings.begin();
         it != mock.bindings.end(); ++it) {
      if (it->first.first == key.first && it->second == key.second) it->second = 0;
    }
  }
}
struct TestDebugError {
  template<class... Args> static void postWarning(Args...) {
    { std::lock_guard<std::mutex> lock(mock.mutex); ++mock.warnings; }
    if (warningCallback) warningCallback();
  }
};
}

#define glGetError testGetError
#define glGetIntegerv testGetIntegerv
#define glGetString testGetString
#define cc_glglue_instance testGlueInstance
#define cc_glglue_glGetProgramiv testGetProgramiv
#define cc_glglue_glGenPrograms testGenPrograms
#define cc_glglue_glBindProgram testBindProgram
#define cc_glglue_glProgramString testProgramString
#define cc_glglue_glDeletePrograms testDeletePrograms
#define SoDebugError TestDebugError
#include "shapenodes/soshape_bumprender.cpp"

namespace {
void reset(Failure failure = NONE) {
  CHECK(CoinBumpTestRenderer::ProgramCache::registry().entries.empty());
  mock.failure = failure; mock.failupload = 1; mock.error = GL_NO_ERROR; mock.list = 0;
  mock.next = 100; mock.generated = mock.uploads = mock.deleted = mock.warnings = 0;
  mock.bindings.clear(); mock.lengths.clear(); mock.deletions.clear();
  warningCallback = NULL;
  programErrorString = "injected upload failure";
  BumpTestCacheContext::failNextScheduling() = false;
  BumpTestCacheContext::current() = 1;
  mock.bindings[std::make_pair(1, GL_FRAGMENT_PROGRAM_ARB)] = 91;
  mock.bindings[std::make_pair(1, GL_VERTEX_PROGRAM_ARB)] = 92;
  mock.lengths[std::make_pair(1, 91)] = 77;
  mock.lengths[std::make_pair(1, 92)] = 88;
  BumpTestCacheContext::flush(1);
}
void failure_cases() {
  const cc_glglue * glue = testGlueInstance(1);
  for (int value = SAVE; value <= RESTORE; ++value) {
    const Failure failure = (Failure) value;
    reset(failure);
    {
      CoinBumpTestRenderer renderer;
      CoinBumpTestRenderer::spec_programidx programs;
      CHECK(!renderer.ensurePrograms(glue, NULL, programs));
      const int uploads = mock.uploads, generated = mock.generated;
      CHECK(!renderer.ensurePrograms(glue, NULL, programs));
      CHECK(mock.uploads == uploads && mock.generated == generated && mock.warnings == 1);
      CHECK(mock.lengths[std::make_pair(1, 91)] == 77);
      CHECK(mock.lengths[std::make_pair(1, 92)] == 88);
      if (failure <= BIND) CHECK(mock.uploads == 0);
      if (failure != RESTORE) {
        CHECK(mock.bindings[std::make_pair(1, GL_FRAGMENT_PROGRAM_ARB)] == 91);
      }
      // Destruction clears a failed sentinel, allowing a recycled context id.
      SoContextHandler::destructingContext(1);
      mock.failure = NONE;
      CHECK(renderer.ensurePrograms(glue, NULL, programs));
    }
    BumpTestCacheContext::flush(1);
  }
  for (int failat = 1; failat <= 3; ++failat) {
    reset(UPLOAD); mock.failupload = failat;
    { CoinBumpTestRenderer r; CoinBumpTestRenderer::spec_programidx p;
      CHECK(!r.ensurePrograms(glue, NULL, p)); CHECK(mock.deleted == failat); }
  }
}
void deferred_cases() {
  reset(); mock.list = 5;
  CoinBumpTestRenderer * r = new CoinBumpTestRenderer;
  CoinBumpTestRenderer::spec_programidx p;
  CHECK(!r->ensurePrograms(testGlueInstance(1), NULL, p));
  CHECK(!r->ensurePrograms(testGlueInstance(1), NULL, p));
  CHECK(mock.generated == 0 && mock.uploads == 0 && BumpTestCacheContext::queue().size() == 1);
  // A current-context callback inside a user list must not upload or deadlock.
  BumpTestCacheContext::flush(1); CHECK(mock.uploads == 0);
  CHECK(!r->ensurePrograms(testGlueInstance(1), NULL, p));
  mock.list = 0; BumpTestCacheContext::flush(1);
  CHECK(r->ensurePrograms(testGlueInstance(1), NULL, p)); CHECK(mock.uploads == 3);
  CHECK(r->ensurePrograms(testGlueInstance(1), NULL, p)); CHECK(mock.uploads == 3);
  delete r; BumpTestCacheContext::flush(1); CHECK(mock.deleted == 3);

  reset(); mock.list = 9; r = new CoinBumpTestRenderer;
  CHECK(!r->ensurePrograms(testGlueInstance(1), NULL, p));
  delete r; mock.list = 0; BumpTestCacheContext::flush(1); CHECK(mock.generated == 0);

  reset(); mock.list = 9; r = new CoinBumpTestRenderer;
  CHECK(!r->ensurePrograms(testGlueInstance(1), NULL, p));
  SoContextHandler::destructingContext(1); mock.list = 0;
  BumpTestCacheContext::flush(1); CHECK(mock.generated == 0);
  delete r;
}
void stale_initializer_case() {
  reset(); mock.list = 5;
  CoinBumpTestRenderer * old = new CoinBumpTestRenderer;
  CoinBumpTestRenderer::spec_programidx p;
  CHECK(!old->ensurePrograms(testGlueInstance(1), NULL, p));
  delete old; mock.list = 0;
  { CoinBumpTestRenderer fresh;
    CHECK(fresh.ensurePrograms(testGlueInstance(1), NULL, p));
    BumpTestCacheContext::flush(1);
    CHECK(mock.uploads == 3 && mock.deleted == 0); }
  BumpTestCacheContext::flush(1); CHECK(mock.deleted == 3);
}
struct RedrawProbe {
  RedrawProbe() : renderer(NULL), notifications(0), destroyRenderer(false) { }
  CoinBumpTestRenderer * renderer;
  int notifications;
  bool destroyRenderer;
};
void observe_redraw(void * closure, SoSensor *) {
  RedrawProbe * probe = (RedrawProbe *) closure;
  ++probe->notifications;
  if (probe->destroyRenderer) {
    CoinBumpTestRenderer * renderer = probe->renderer;
    probe->renderer = NULL;
    delete renderer;
  }
}
void redraw_lifetime_cases() {
  reset(); mock.list = 5;
  SoSeparator * root = new SoSeparator; root->ref();
  RedrawProbe probe;
  SoNodeSensor monitor(observe_redraw, &probe); monitor.setPriority(0); monitor.attach(root);
  probe.renderer = new CoinBumpTestRenderer;
  CoinBumpTestRenderer::spec_programidx p;
  CHECK(!probe.renderer->ensurePrograms(testGlueInstance(1), NULL, p));
  const uintptr_t token = probe.renderer->programcache->token;
  std::weak_ptr<CoinBumpTestRenderer::ProgramCache> lifetime = probe.renderer->programcache;
  probe.renderer->scheduleRedraw(NULL, root);
  probe.renderer->scheduleRedraw(NULL, root); // One redraw for repeated PENDING requests.
  CHECK(probe.notifications == 0 && root->getRefCount() == 1);
  CHECK(probe.renderer->programcache->redraws[root]->isScheduled());
  probe.destroyRenderer = true;
  SoDB::getSensorManager()->processDelayQueue(TRUE);
  // The immediate observer destroys the renderer from inside redraw touch().
  // No strong test reference may mask the sensor's post-callback lifetime.
  CHECK(probe.renderer == NULL && probe.notifications == 1);
  CHECK(lifetime.expired());
  CHECK(CoinBumpTestRenderer::ProgramCache::registry().entries.count(token) == 0);
  CHECK(root->getRefCount() == 1);
  SoDB::getSensorManager()->processDelayQueue(TRUE); CHECK(probe.notifications == 1);
  mock.list = 0; BumpTestCacheContext::flush(1); CHECK(mock.generated == 0);
  monitor.detach(); root->unref();

  // Destruction before queue processing must cancel redraw and initialization.
  reset(); mock.list = 5;
  root = new SoSeparator; root->ref();
  probe.destroyRenderer = false; probe.notifications = 0;
  monitor.attach(root); probe.renderer = new CoinBumpTestRenderer;
  CHECK(!probe.renderer->ensurePrograms(testGlueInstance(1), NULL, p));
  probe.renderer->scheduleRedraw(NULL, root);
  lifetime = probe.renderer->programcache;
  delete probe.renderer; probe.renderer = NULL;
  CHECK(lifetime.expired());
  SoDB::getSensorManager()->processDelayQueue(TRUE); CHECK(probe.notifications == 0);
  mock.list = 0; BumpTestCacheContext::flush(1); CHECK(mock.generated == 0);
  monitor.detach(); root->unref();

  // A queued sensor does not retain its root and must tolerate root deletion.
  reset(); mock.list = 5;
  root = new SoSeparator; root->ref();
  monitor.attach(root); probe.renderer = new CoinBumpTestRenderer;
  CHECK(!probe.renderer->ensurePrograms(testGlueInstance(1), NULL, p));
  probe.renderer->scheduleRedraw(NULL, root);
  CHECK(root->getRefCount() == 1);
  root->unref();
  CHECK(monitor.getAttachedNode() == NULL);
  CHECK(probe.renderer->programcache->redraws[root]->getAttachedNode() == NULL);
  SoDB::getSensorManager()->processDelayQueue(TRUE); CHECK(probe.notifications == 0);
  CHECK(!probe.renderer->programcache->redraws[root]->isScheduled());
  delete probe.renderer; probe.renderer = NULL;
  mock.list = 0; BumpTestCacheContext::flush(1); CHECK(mock.generated == 0);
}
void multiple_root_redraw_case() {
  reset(); mock.list = 5;
  SoSeparator * a = new SoSeparator; a->ref();
  SoSeparator * b = new SoSeparator; b->ref();
  RedrawProbe aprobe, bprobe;
  SoNodeSensor amonitor(observe_redraw, &aprobe), bmonitor(observe_redraw, &bprobe);
  amonitor.setPriority(0); amonitor.attach(a);
  bmonitor.setPriority(0); bmonitor.attach(b);
  {
    CoinBumpTestRenderer renderer; CoinBumpTestRenderer::spec_programidx p;
    CHECK(!renderer.ensurePrograms(testGlueInstance(1), NULL, p));
    renderer.scheduleRedraw(NULL, a); renderer.scheduleRedraw(NULL, b);
    renderer.scheduleRedraw(NULL, a); renderer.scheduleRedraw(NULL, b);
    CHECK(renderer.programcache->redraws.size() == 2);
    CHECK(aprobe.notifications == 0 && bprobe.notifications == 0);
    CHECK(a->getRefCount() == 1 && b->getRefCount() == 1);
    SoDB::getSensorManager()->processDelayQueue(TRUE);
    CHECK(aprobe.notifications == 1 && bprobe.notifications == 1);
    SoDB::getSensorManager()->processDelayQueue(TRUE);
    CHECK(aprobe.notifications == 1 && bprobe.notifications == 1);
  }
  mock.list = 0; BumpTestCacheContext::flush(1); CHECK(mock.generated == 0);
  amonitor.detach(); bmonitor.detach(); a->unref(); b->unref();
}
struct QueueChangeProbe {
  QueueChangeProbe(CoinBumpTestRenderer ** owner, bool destroy)
    : renderer(owner), calls(0), destroyRenderer(destroy) { }
  CoinBumpTestRenderer ** renderer;
  int calls;
  bool destroyRenderer;
};
void change_queue_reentrantly(void * closure) {
  QueueChangeProbe * probe = (QueueChangeProbe *) closure;
  ++probe->calls;
  // Unscheduling also changes the queue. Disarm first to avoid nested deletes.
  SoDB::getSensorManager()->setChangedCallback(NULL, NULL);
  if (probe->destroyRenderer) {
    CoinBumpTestRenderer * renderer = *probe->renderer;
    *probe->renderer = NULL;
    delete renderer;
  }
  else SoContextHandler::destructingContext(1);
}
void queue_changed_lifetime_cases() {
  SoSensorManager * manager = SoDB::getSensorManager();
  const SbTime timeout = manager->getDelaySensorTimeout();
  // Test the delay queue insertion itself, rather than the timeout timer.
  manager->setDelaySensorTimeout(SbTime::zero());
  for (int destroy = 0; destroy < 2; ++destroy) {
    reset(); mock.list = 5;
    SoSeparator * root = new SoSeparator; root->ref();
    RedrawProbe observer;
    SoNodeSensor monitor(observe_redraw, &observer); monitor.setPriority(0); monitor.attach(root);
    CoinBumpTestRenderer * renderer = new CoinBumpTestRenderer;
    CoinBumpTestRenderer::spec_programidx p;
    CHECK(!renderer->ensurePrograms(testGlueInstance(1), NULL, p));
    const uintptr_t token = renderer->programcache->token;
    std::weak_ptr<CoinBumpTestRenderer::ProgramCache> lifetime = renderer->programcache;
    QueueChangeProbe probe(&renderer, destroy != 0);
    manager->setChangedCallback(change_queue_reentrantly, &probe);
    renderer->scheduleRedraw(NULL, root);
    CHECK(probe.calls == 1 && observer.notifications == 0);
    if (destroy) {
      CHECK(renderer == NULL && lifetime.expired());
      CHECK(CoinBumpTestRenderer::ProgramCache::registry().entries.count(token) == 0);
    }
    else CHECK(renderer->programcache->contexts.empty());
    manager->processDelayQueue(TRUE);
    CHECK(observer.notifications == (destroy ? 0 : 1));
    delete renderer;
    mock.list = 0; BumpTestCacheContext::flush(1); CHECK(mock.generated == 0);
    monitor.detach(); root->unref();
  }
  manager->setChangedCallback(NULL, NULL);
  manager->setDelaySensorTimeout(timeout);
}
void delete_renderer(uint32_t, void * closure) { delete (CoinBumpTestRenderer *) closure; }
void lifetime_cases() {
  reset(); CoinBumpTestRenderer * r = new CoinBumpTestRenderer;
  CoinBumpTestRenderer::spec_programidx p;
  CHECK(r->ensurePrograms(testGlueInstance(1), NULL, p));
  SoContextHandler::addContextDestructionCallback(delete_renderer, r);
  SoContextHandler::destructingContext(1);
  SoContextHandler::removeContextDestructionCallback(delete_renderer, r);
  CHECK(mock.deleted == 3);
  BumpTestCacheContext::flush(1); CHECK(mock.deleted == 3);
  CHECK(CoinBumpTestRenderer::ProgramCache::registry().entries.empty());

  // The copied callback must also be safe when there are no GL resources.
  r = new CoinBumpTestRenderer;
  SoContextHandler::addContextDestructionCallback(delete_renderer, r);
  SoContextHandler::destructingContext(1);
  SoContextHandler::removeContextDestructionCallback(delete_renderer, r);
}
void diffuse_and_context_cases() {
  reset(); CoinBumpTestRenderer r;
  CoinBumpTestRenderer::spec_programidx a, b;
  CoinBumpTestRenderer::diffuse_programidx diffuse;
  CHECK(r.ensurePrograms(testGlueInstance(1), NULL, a));
  CHECK(r.ensureDiffusePrograms(testGlueInstance(1), NULL, diffuse));
  CHECK(diffuse.pointlight == 0 && diffuse.dirlight != 0 && diffuse.normalrendering != 0);
  BumpTestCacheContext::current() = 2;
  CHECK(r.ensurePrograms(testGlueInstance(2), NULL, b)); CHECK(a.fragment != b.fragment);
  BumpTestCacheContext::current() = 1;
  SoContextHandler::destructingContext(1); CHECK(mock.deleted == 5);
  BumpTestCacheContext::current() = 2;
  const int count = mock.uploads;
  CHECK(r.ensurePrograms(testGlueInstance(2), NULL, b)); CHECK(mock.uploads == count);
  SoContextHandler::destructingContext(2); CHECK(mock.deleted == 8);
}
void destroy_other_context_on_warning() {
  BumpTestCacheContext::current() = 2;
  SoContextHandler::destructingContext(2);
  BumpTestCacheContext::current() = 1;
}
void error_handler_cases() {
  reset();
  {
    CoinBumpTestRenderer r; CoinBumpTestRenderer::spec_programidx p;
    BumpTestCacheContext::current() = 2;
    CHECK(r.ensurePrograms(testGlueInstance(2), NULL, p));
    BumpTestCacheContext::current() = 1;
    mock.failure = SAVE; warningCallback = destroy_other_context_on_warning;
    CHECK(!r.ensurePrograms(testGlueInstance(1), NULL, p));
    CHECK(mock.warnings == 1 && mock.deleted == 3);
    warningCallback = NULL;
  }
  reset();
  {
    CoinBumpTestRenderer r; CoinBumpTestRenderer::spec_programidx p;
    mock.list = 5;
    CHECK(!r.ensurePrograms(testGlueInstance(1), NULL, p));
    mock.list = 0; mock.failure = UPLOAD; BumpTestCacheContext::flush(1);
    CHECK(mock.warnings == 0); // Never call a user handler from the scheduler.
    CHECK(!r.ensurePrograms(testGlueInstance(1), NULL, p)); CHECK(mock.warnings == 1);
    CHECK(!r.ensurePrograms(testGlueInstance(1), NULL, p)); CHECK(mock.warnings == 1);
  }
}
void concurrent_cases() {
  reset(); CoinBumpTestRenderer r; std::atomic<bool> start(false);
  std::thread destroy([&] {
    BumpTestCacheContext::current() = 2;
    while (!start.load()) { }
    for (int i = 0; i < 1000; ++i) {
      CoinBumpTestRenderer::spec_programidx p;
      CHECK(r.ensurePrograms(testGlueInstance(2), NULL, p));
      CoinBumpTestRenderer::context_destruction_cb(2, (void *) r.programcache->token);
    }
  });
  start.store(true);
  for (int i = 0; i < 1000; ++i) {
    CoinBumpTestRenderer::spec_programidx p;
    CHECK(r.ensurePrograms(testGlueInstance(1), NULL, p));
  }
  destroy.join();
  SoContextHandler::destructingContext(1);
}

void allocation_failure_cases() {
  typedef CoinBumpTestRenderer Renderer;
  typedef Renderer::ProgramCache Cache;
  // Long driver text is bounded and copied without allocating host memory.
  char text[soshape_bump_program_error::MESSAGE_CAPACITY + 32];
  std::memset(text, 'x', sizeof(text)); text[sizeof(text) - 1] = '\0';
  {
    BumpTestAllocation::Scope failure;
    soshape_bump_program_error error;
    error.setMessage(NULL); CHECK(error.message[0] == '\0');
    error.setMessage(text);
    CHECK(std::strlen(error.message) == soshape_bump_program_error::MESSAGE_CAPACITY - 1);
    CHECK(std::strcmp(error.message + soshape_bump_program_error::MESSAGE_CAPACITY - 4, "...") == 0);
    text[soshape_bump_program_error::MESSAGE_CAPACITY - 1] = '\0';
    error.setMessage(text);
    CHECK(error.message[soshape_bump_program_error::MESSAGE_CAPACITY - 2] == 'x');
    error.setMessage(""); CHECK(error.message[0] == '\0');
    CHECK(failure.untouched());
  }
  // Diagnostic capture cannot interrupt rollback after an upload error.
  for (int longtext = 0; longtext < 2; ++longtext) {
    reset(UPLOAD); mock.deletions.reserve(16);
    if (longtext) {
      text[soshape_bump_program_error::MESSAGE_CAPACITY - 1] = 'x';
      programErrorString = text;
    }
    Renderer r; r.programcache->contexts[1];
    Renderer::spec_programidx p;
    BumpTestAllocation::Scope failure;
    CHECK(!r.ensurePrograms(testGlueInstance(1), NULL, p));
    CHECK(mock.generated == 1 && mock.deleted == 1 && mock.warnings == 1);
    CHECK(r.programcache->contexts[1].specstatus == Cache::FAILED);
    CHECK(r.programcache->contexts[1].spec.fragment == 0);
    CHECK(mock.bindings[std::make_pair(1, GL_FRAGMENT_PROGRAM_ARB)] == 91);
    CHECK(failure.untouched());
  }
  // Context cleanup must not allocate while copying an unreported diagnostic.
  reset(); mock.deletions.reserve(16);
  {
    Renderer r; Renderer::spec_programidx spec; Renderer::diffuse_programidx diffuse;
    CHECK(r.ensurePrograms(testGlueInstance(1), NULL, spec));
    mock.list = 5; CHECK(!r.ensureDiffusePrograms(testGlueInstance(1), NULL, diffuse));
    mock.list = 0; mock.failure = UPLOAD; mock.failupload = mock.uploads + 1;
    BumpTestCacheContext::flush(1);
    CHECK(r.programcache->contexts[1].diffusestatus == Cache::FAILED && mock.warnings == 0);
    CHECK(std::strlen(r.programcache->contexts[1].diffuseerror.message) > 15);
    mock.failure = NONE;
    BumpTestAllocation::Scope failure;
    Renderer::context_destruction_cb(1, (void *) r.programcache->token);
    CHECK(r.programcache->contexts.empty() && mock.deleted == 4);
    CHECK(failure.untouched());
  }
  // No allocation for destructor metadata, including zero and maximal IDs.
  reset();
  BumpTestCacheContext::queue().reserve(8);
  Renderer * renderer = new Renderer;
  const uint32_t ids[] = { 0, 1, std::numeric_limits<uint32_t>::max() };
  for (size_t i = 0; i < 3; ++i) {
    BumpTestCacheContext::current() = ids[i];
    Renderer::spec_programidx p;
    CHECK(renderer->ensurePrograms(testGlueInstance((int) ids[i]), NULL, p));
  }
  {
    BumpTestAllocation::Scope failure;
    delete renderer;
    CHECK(BumpTestCacheContext::queue().size() == 3 && failure.untouched());
  }
  for (size_t i = 0; i < 3; ++i) {
    BumpTestCacheContext::current() = ids[i];
    BumpTestCacheContext::flush(ids[i]);
  }
  CHECK(mock.deleted == 9 && Cache::registry().entries.empty());

  // If deletion cannot be queued, the registry retains ownership until the
  // context's existing destruction callback releases the names, without throw.
  reset(); renderer = new Renderer;
  Renderer::spec_programidx p;
  CHECK(renderer->ensurePrograms(testGlueInstance(1), NULL, p));
  std::weak_ptr<Cache> lifetime = renderer->programcache;
  BumpTestCacheContext::failNextScheduling() = true;
  {
    BumpTestAllocation::Scope failure;
    delete renderer;
    CHECK(failure.untouched());
  }
  CHECK(BumpTestCacheContext::queue().empty() && !lifetime.expired());
  CHECK(mock.deleted == 0 && Cache::registry().entries.size() == 1);
  SoContextHandler::destructingContext(1);
  CHECK(mock.deleted == 3 && lifetime.expired() && Cache::registry().entries.empty());

  // A real allocation failure in the adapted queue must not leave PENDING
  // without a callback, and must stay isolated to its own program family.
  for (int diffuse = 0; diffuse < 2; ++diffuse) {
    reset();
    { std::vector<BumpTestCacheContext::Callback> empty;
      empty.swap(BumpTestCacheContext::queue()); }
    {
      Renderer r; r.programcache->contexts[1];
      Renderer::spec_programidx spec; Renderer::diffuse_programidx diff;
      mock.list = 5;
      {
        BumpTestAllocation::Scope failure;
        CHECK(!(diffuse ? r.ensureDiffusePrograms(testGlueInstance(1), NULL, diff) :
                          r.ensurePrograms(testGlueInstance(1), NULL, spec)));
        CHECK(!failure.untouched());
      }
      const Cache::Context & ctx = r.programcache->contexts[1];
      CHECK((diffuse ? ctx.diffusestatus : ctx.specstatus) == Cache::FAILED);
      CHECK((diffuse ? ctx.specstatus : ctx.diffusestatus) == Cache::EMPTY);
      CHECK((diffuse ? ctx.diffuseerror : ctx.specerror).error == GL_OUT_OF_MEMORY);
      CHECK(BumpTestCacheContext::queue().empty() && mock.generated == 0 && mock.warnings == 1);
      mock.list = 0;
      CHECK(!(diffuse ? r.ensureDiffusePrograms(testGlueInstance(1), NULL, diff) :
                        r.ensurePrograms(testGlueInstance(1), NULL, spec)));
      CHECK(mock.warnings == 1 && mock.generated == 0);
      SoContextHandler::destructingContext(1);
      CHECK(diffuse ? r.ensureDiffusePrograms(testGlueInstance(1), NULL, diff) :
                      r.ensurePrograms(testGlueInstance(1), NULL, spec));
    }
    BumpTestCacheContext::flush(1);
    CHECK(mock.deleted == (diffuse ? 2 : 3));
  }
}

void owned_allocation_cases() {
  typedef CoinBumpTestRenderer Renderer;
  typedef Renderer::ProgramCache Cache;
  // Cache object, shared ownership and registry node each fail independently.
  // A failed factory never publishes an entry or rewinds a consumed token.
  for (int stage = 0; stage < 3; ++stage) {
    reset();
    const uintptr_t previous = Cache::registry().nexttoken;
    {
      BumpTestAllocation::Scope failure(stage);
      Renderer unavailable;
      CHECK(!failure.untouched() && !unavailable.programcache);
      Renderer::spec_programidx spec;
      Renderer::diffuse_programidx diffuse;
      spec.fragment = spec.dirlight = spec.pointlight = 7;
      diffuse.dirlight = diffuse.pointlight = diffuse.normalrendering = 8;
      CHECK(!unavailable.ensurePrograms(NULL, NULL, spec));
      CHECK(!unavailable.ensureDiffusePrograms(NULL, NULL, diffuse));
      unavailable.scheduleRedraw(NULL, NULL);
      CHECK(spec.fragment == 7 && spec.dirlight == 7 && spec.pointlight == 7);
      CHECK(diffuse.dirlight == 8 && diffuse.pointlight == 8 && diffuse.normalrendering == 8);
      CHECK(Cache::registry().entries.empty());
      CHECK(Cache::registry().nexttoken == previous + (stage == 2 ? 1 : 0));
      CHECK(mock.generated == 0 && BumpTestCacheContext::queue().empty());
    }
    Renderer recovered;
    CHECK(recovered.programcache && recovered.programcache->token > previous);
  }
  // Failed insertion precedes all GL/queue operations. A scalar latch rejects
  // new contexts, not ready resources in an existing context, and warns once.
  for (int diffuse = 0; diffuse < 2; ++diffuse) {
    for (int inlist = 0; inlist < 2; ++inlist) {
      reset(); mock.deletions.reserve(16);
      Renderer r; Renderer::spec_programidx spec; Renderer::diffuse_programidx diff;
      CHECK(r.ensurePrograms(testGlueInstance(1), NULL, spec));
      CHECK(r.ensureDiffusePrograms(testGlueInstance(1), NULL, diff));
      const int generated = mock.generated;
      spec.fragment = spec.dirlight = spec.pointlight = 7;
      diff.dirlight = diff.pointlight = diff.normalrendering = 8;
      BumpTestCacheContext::current() = 2; mock.list = inlist ? 5 : 0;
      static Renderer * warningOwner;
      warningOwner = &r;
      warningCallback = [] {
        Renderer::spec_programidx ignored;
        // Reentry proves that diagnostics run outside our mutex and the latch
        // is visible before invoking the external handler.
        CHECK(!warningOwner->ensurePrograms(NULL, NULL, ignored));
      };
      {
        BumpTestAllocation::Scope failure;
        CHECK(!(diffuse ? r.ensureDiffusePrograms(testGlueInstance(2), NULL, diff) :
                          r.ensurePrograms(testGlueInstance(2), NULL, spec)));
        CHECK(!failure.untouched());
      }
      warningCallback = NULL;
      CHECK(r.programcache->contextallocationfailed && r.programcache->contexts.size() == 1);
      CHECK(mock.generated == generated && mock.warnings == 1);
      CHECK(BumpTestCacheContext::queue().empty());
      CHECK(spec.fragment == 7 && spec.dirlight == 7 && spec.pointlight == 7);
      CHECK(diff.dirlight == 8 && diff.pointlight == 8 && diff.normalrendering == 8);
      {
        BumpTestAllocation::Scope failure;
        BumpTestCacheContext::current() = 3;
        CHECK(!r.ensurePrograms(NULL, NULL, spec));
        CHECK(!r.ensureDiffusePrograms(NULL, NULL, diff));
        BumpTestCacheContext::current() = 1;
        CHECK(r.ensurePrograms(NULL, NULL, spec));
        CHECK(r.ensureDiffusePrograms(NULL, NULL, diff));
        CHECK(failure.untouched() && mock.warnings == 1 && mock.generated == generated);
        Renderer::context_destruction_cb(2, (void *) r.programcache->token);
        CHECK(failure.untouched() && !r.programcache->contextallocationfailed);
      }
      BumpTestCacheContext::current() = 2;
      if (inlist) {
        CHECK(!(diffuse ? r.ensureDiffusePrograms(testGlueInstance(2), NULL, diff) :
                          r.ensurePrograms(testGlueInstance(2), NULL, spec)));
        CHECK(BumpTestCacheContext::queue().size() == 1);
        mock.list = 0; BumpTestCacheContext::flush(2);
      }
      CHECK(diffuse ? r.ensureDiffusePrograms(testGlueInstance(2), NULL, diff) :
                      r.ensurePrograms(testGlueInstance(2), NULL, spec));
      CHECK(mock.generated == generated + (diffuse ? 2 : 3));
      SoContextHandler::destructingContext(2);
      BumpTestCacheContext::current() = 1;
      SoContextHandler::destructingContext(1);
      CHECK(mock.deleted == mock.generated);
    }
  }
  // Sensor object, shared ownership and redraw-map node. The other viewer's
  // existing sensor survives, and there is never an empty map entry to crash
  // pruning/destruction. A later traversal can retry the failed viewer.
  for (int stage = 0; stage < 3; ++stage) {
    reset(); mock.list = 5;
    SoSeparator * a = new SoSeparator; a->ref();
    SoSeparator * b = new SoSeparator; b->ref();
    RedrawProbe aprobe, bprobe;
    SoNodeSensor amonitor(observe_redraw, &aprobe), bmonitor(observe_redraw, &bprobe);
    amonitor.setPriority(0); amonitor.attach(a);
    bmonitor.setPriority(0); bmonitor.attach(b);
    {
      Renderer r; Renderer::spec_programidx spec;
      CHECK(!r.ensurePrograms(testGlueInstance(1), NULL, spec));
      r.scheduleRedraw(NULL, a);
      const Cache::RedrawPtr existing = r.programcache->redraws.find(a)->second;
      {
        BumpTestAllocation::Scope failure(stage);
        r.scheduleRedraw(NULL, b);
        CHECK(!failure.untouched());
      }
      CHECK(r.programcache->redraws.size() == 1 && r.programcache->redraws.count(b) == 0);
      CHECK(existing->isScheduled() && existing->getAttachedNode() == a);
      CHECK(existing->self.lock() == existing);
      CHECK(a->getRefCount() == 1 && b->getRefCount() == 1);
      CHECK(mock.generated == 0 && BumpTestCacheContext::queue().size() == 1);
      CHECK(r.programcache->contexts[1].specstatus == Cache::PENDING);
      r.scheduleRedraw(NULL, b);
      CHECK(r.programcache->redraws.size() == 2);
      SoDB::getSensorManager()->processDelayQueue(TRUE);
      CHECK(aprobe.notifications == 1 && bprobe.notifications == 1);
      mock.list = 0; BumpTestCacheContext::flush(1);
      CHECK(r.ensurePrograms(testGlueInstance(1), NULL, spec));
      SoContextHandler::destructingContext(1);
    }
    amonitor.detach(); bmonitor.detach(); a->unref(); b->unref();
  }
}

void dependency_failure_boundary_cases() {
  typedef CoinBumpTestRenderer Renderer;
  typedef Renderer::ProgramCache Cache;
  // Constructor rollback removes only our entry. Provider exceptions, even
  // bad_alloc, propagate instead of claiming the provider recovered its state.
  for (int value = BumpTestContextHandler::ALLOCATION;
       value <= BumpTestContextHandler::FOREIGN; ++value) {
    reset();
    BumpTestContextHandler::failNextRegistration() = (BumpTestContextHandler::Failure) value;
    bool caught = false;
    try { Renderer r; }
    catch (const std::bad_alloc &) { CHECK(value == BumpTestContextHandler::ALLOCATION); caught = true; }
    catch (const BumpTestForeignFailure &) { CHECK(value == BumpTestContextHandler::FOREIGN); caught = true; }
    CHECK(caught && Cache::registry().entries.empty());
    const uintptr_t failed = BumpTestContextHandler::lastToken();
    Renderer::context_destruction_cb(1, (void *) failed);
    CHECK(mock.generated == 0 && BumpTestCacheContext::queue().empty());
    Renderer recovered;
    CHECK(recovered.programcache->token > failed);
  }
  // A user diagnostic handler is not an internal allocation site. Its throw
  // must remain distinguishable, with our state committed and mutex released.
  reset();
  Renderer r; Renderer::spec_programidx spec;
  warningCallback = [] { throw std::bad_alloc(); };
  bool caught = false;
  {
    BumpTestAllocation::Scope failure;
    try { (void) r.ensurePrograms(NULL, NULL, spec); }
    catch (const std::bad_alloc &) { caught = true; }
    CHECK(!failure.untouched());
  }
  warningCallback = NULL;
  CHECK(caught && r.programcache->contextallocationfailed);
  CHECK(!r.ensurePrograms(NULL, NULL, spec));
  CHECK(r.programcache->contexts.empty() && mock.warnings == 1 && mock.generated == 0);
  Renderer::context_destruction_cb(1, (void *) r.programcache->token);
  CHECK(r.ensurePrograms(testGlueInstance(1), NULL, spec));
  SoContextHandler::destructingContext(1);
}

// Run last: exhausted tokens must never be reset, even by this test.
void token_exhaustion_cases() {
  reset();
  typedef CoinBumpTestRenderer::ProgramCache Cache;
  const uintptr_t limit = std::numeric_limits<uintptr_t>::max() >> 1;
  CHECK(Cache::registry().nexttoken < limit - 1);
  Cache::registry().nexttoken = limit - 1;
  std::vector<BumpTestCacheContext::Callback> stale;
  {
    CoinBumpTestRenderer valid;
    CHECK(valid.programcache && valid.programcache->token == limit);
    CoinBumpTestRenderer::spec_programidx spec;
    CoinBumpTestRenderer::diffuse_programidx diffuse;
    mock.list = 5;
    CHECK(!valid.ensurePrograms(testGlueInstance(1), NULL, spec));
    CHECK(!valid.ensureDiffusePrograms(testGlueInstance(1), NULL, diffuse));
    CHECK(BumpTestCacheContext::queue().size() == 2);
    stale = BumpTestCacheContext::queue();

    // The diagnostic may reenter the registry, but cannot run under its lock.
    warningCallback = [] {
      CHECK(Cache::lookup(std::numeric_limits<uintptr_t>::max() >> 1));
      CHECK(!Cache::create()); // No recursive warning or registry lock held.
    };
    {
      CoinBumpTestRenderer unavailable;
      CHECK(!unavailable.programcache && mock.warnings == 1);
      CHECK(Cache::registry().nexttoken == limit && Cache::registry().entries.size() == 1);
      spec.dirlight = spec.pointlight = spec.fragment = 7;
      diffuse.dirlight = diffuse.pointlight = diffuse.normalrendering = 8;
      for (int i = 0; i < 2; ++i) {
        CHECK(!unavailable.ensurePrograms(NULL, NULL, spec));
        CHECK(!unavailable.ensureDiffusePrograms(NULL, NULL, diffuse));
        unavailable.initPrograms(NULL, NULL);
        unavailable.initDiffusePrograms(NULL, NULL);
        unavailable.scheduleRedraw(NULL, NULL);
      }
      CHECK(spec.dirlight == 7 && spec.pointlight == 7 && spec.fragment == 7);
      CHECK(diffuse.dirlight == 8 && diffuse.pointlight == 8 && diffuse.normalrendering == 8);
      CHECK(mock.warnings == 1 && mock.generated == 0 && mock.uploads == 0 && mock.deleted == 0);
      CHECK(BumpTestCacheContext::queue().size() == 2);
      // A second failure cannot advance the counter or replace the live cache.
      CoinBumpTestRenderer another;
      CHECK(!another.programcache && mock.warnings == 1);
      CHECK(Cache::registry().nexttoken == limit && Cache::registry().entries.size() == 1);
    }
    warningCallback = NULL;
    CHECK(Cache::lookup(limit) == valid.programcache);
    mock.list = 0;
    BumpTestCacheContext::flush(1);
    CHECK(valid.ensurePrograms(testGlueInstance(1), NULL, spec));
    CHECK(valid.ensureDiffusePrograms(testGlueInstance(1), NULL, diffuse));
    CHECK(mock.generated == 5 && mock.uploads == 5);
  }
  BumpTestCacheContext::flush(1);
  CHECK(mock.deleted == 5 && Cache::registry().entries.empty());
  for (size_t i = 0; i < stale.size(); ++i) stale[i].func(stale[i].closure, stale[i].id);
  CoinBumpTestRenderer::context_destruction_cb(1, (void *) limit);
  CHECK(mock.generated == 5 && mock.uploads == 5 && mock.deleted == 5);
  CHECK(BumpTestCacheContext::queue().empty());

  // SoDB reinitialization must not make an exhausted identity available again.
  SoDB::finish();
  SoDB::init();
  reset();
  {
    CoinBumpTestRenderer unavailable;
    CHECK(!unavailable.programcache && mock.warnings == 0);
    CHECK(Cache::registry().nexttoken == limit && Cache::registry().entries.empty());
    unavailable.scheduleRedraw(NULL, NULL);
  }
  SoContextHandler::destructingContext(1);
  CHECK(mock.generated == 0 && mock.uploads == 0 && mock.deleted == 0);
  CHECK(BumpTestCacheContext::queue().empty());
}
}
int main() {
  SoDB::init();
  failure_cases(); deferred_cases(); stale_initializer_case(); lifetime_cases(); diffuse_and_context_cases();
  error_handler_cases(); redraw_lifetime_cases(); multiple_root_redraw_case();
  queue_changed_lifetime_cases(); concurrent_cases();
  allocation_failure_cases();
  owned_allocation_cases(); dependency_failure_boundary_cases();
  CHECK(CoinBumpTestRenderer::ProgramCache::registry().entries.empty());
  CHECK(BumpTestCacheContext::queue().empty());
  SoDB::finish();
  // Coin reinitialization must reset cleanup registration, not reuse tokens.
  SoDB::init(); reset();
  { CoinBumpTestRenderer r; CoinBumpTestRenderer::spec_programidx p;
    CHECK(r.ensurePrograms(testGlueInstance(1), NULL, p)); }
  BumpTestCacheContext::flush(1);
  CHECK(CoinBumpTestRenderer::ProgramCache::registry().entries.empty());
  token_exhaustion_cases();
  SoDB::finish();
  std::puts("Bump program failure, allocation, cache, deferred initialization and lifetime tests passed.");
}
