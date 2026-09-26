#include "TestAdapter.h"
#include <cstring>
#include <thread>
#include <atomic>

namespace {
enum Failure { NONE, SAVE, GENERATE, ZERO_NAME, BIND, UPLOAD, LENGTH, ZERO_LENGTH, RESTORE };
void (*warningCallback)() = NULL;
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
const GLubyte * testGetString(GLenum) { return (const GLubyte *) "injected upload failure"; }
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
}
int main() {
  SoDB::init();
  failure_cases(); deferred_cases(); stale_initializer_case(); lifetime_cases(); diffuse_and_context_cases();
  error_handler_cases(); concurrent_cases();
  CHECK(CoinBumpTestRenderer::ProgramCache::registry().entries.empty());
  CHECK(BumpTestCacheContext::queue().empty());
  SoDB::finish();
  // Coin reinitialization must reset cleanup registration, not reuse tokens.
  SoDB::init(); reset();
  { CoinBumpTestRenderer r; CoinBumpTestRenderer::spec_programidx p;
    CHECK(r.ensurePrograms(testGlueInstance(1), NULL, p)); }
  BumpTestCacheContext::flush(1);
  CHECK(CoinBumpTestRenderer::ProgramCache::registry().entries.empty());
  SoDB::finish();
  std::puts("Bump program failure, cache, deferred initialization and lifetime tests passed.");
}
