// Exercise assignment rollback when either a node list or an auditor list grows.
#include <Inventor/SoDB.h>
#include <Inventor/SoPath.h>
#include <Inventor/misc/SoTempPath.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoSeparator.h>

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <new>

static int failafter = -1;

void *
operator new[](std::size_t bytes)
{
  if (failafter == 0) {
    failafter = -1;
    throw std::bad_alloc();
  }
  if (failafter > 0) failafter--;
  void * memory = std::malloc(bytes);
  if (memory == NULL) throw std::bad_alloc();
  return memory;
}

void
operator delete[](void * memory) noexcept
{
  std::free(memory);
}

static bool
node_copy_failure()
{
  SoSeparator * oldroot = new SoSeparator;
  oldroot->ref();
  SoGroup * oldchild = new SoGroup;
  oldroot->addChild(oldchild);
  SoPath * destination = new SoPath(oldroot);
  destination->ref();
  destination->append(oldchild);

  SoSeparator * root = new SoSeparator;
  root->ref();
  SoPath * source = new SoPath(root);
  source->ref();
  SoGroup * parent = root;
  for (int i = 0; i < 5; i++) {
    SoGroup * child = new SoGroup;
    parent->addChild(child);
    source->append(child);
    parent = child;
  }

  bool ok = true;
  for (int stage = 0; stage < 2; stage++) {
    // Fail first while growing nodes, then while reserving indices.
    failafter = stage;
    bool caught = false;
    try { *destination = *source; }
    catch (const std::bad_alloc &) { caught = true; }
    failafter = -1;
    ok = ok && caught && destination->getLength() == 2 &&
         destination->getHead() == oldroot &&
         destination->getNode(1) == oldchild &&
         destination->getIndex(1) == stage;
    oldroot->insertChild(new SoGroup, 0);
    ok = ok && destination->getIndex(1) == stage + 1;
  }
  *destination = *source; // A later assignment still succeeds.
  ok = ok && destination->getHead() == root && destination->getLength() == 6;
  oldroot->insertChild(new SoGroup, 0);
  ok = ok && destination->getHead() == root; // Old auditor was removed.

  destination->unref();
  source->unref();
  oldroot->unref();
  root->unref();
  return ok;
}

static bool
auditor_registration_failure()
{
  SoSeparator * root = new SoSeparator;
  root->ref();
  SoGroup * oldchild = new SoGroup;
  SoGroup * newchild = new SoGroup;
  root->addChild(oldchild);
  root->addChild(newchild);
  SoPath * destination = new SoPath(root);
  destination->ref();
  destination->append(oldchild);
  SoPath * source = new SoPath(root);
  source->ref();
  source->append(newchild);
  SoPath * others[3];
  for (int i = 0; i < 3; i++) {
    others[i] = new SoPath(newchild);
    others[i]->ref();
  }

  // Source plus three other paths fill the new child's four inline auditor
  // slots. Registering on root succeeds; registering on newchild fails.
  // Root is shared with the old path, exercising duplicate rollback.
  failafter = 0;
  bool caught = false;
  try { *destination = *source; }
  catch (const std::bad_alloc &) { caught = true; }
  failafter = -1;

  bool ok = caught && destination->getLength() == 2 &&
            destination->getNode(1) == oldchild &&
            destination->getIndex(1) == 0;
  root->insertChild(new SoGroup, 0);
  ok = ok && destination->getIndex(1) == 1; // Old auditor survived.
  *destination = *source; // Shared-root registration also succeeds later.
  ok = ok && destination->getNode(1) == newchild &&
       destination->getIndex(1) == source->getIndex(1);
  root->insertChild(new SoGroup, 0);
  ok = ok && destination->getIndex(1) == source->getIndex(1) &&
       destination->getIndex(1) == 3; // Exactly one registration remains.
  destination->unref();
  root->insertChild(new SoGroup, 0);
  newchild->insertChild(new SoGroup, 0); // No stale registration.
  for (int i = 0; i < 3; i++) others[i]->unref();
  source->unref();
  root->unref();
  return ok;
}



static bool
late_auditor_registration_failure()
{
  SoSeparator * root = new SoSeparator;
  root->ref();
  SoGroup * oldchild = new SoGroup;
  SoGroup * middle = new SoGroup;
  SoGroup * leaf = new SoGroup;
  root->addChild(oldchild);
  root->addChild(middle);
  middle->addChild(leaf);

  SoPath * destination = new SoPath(root);
  destination->ref();
  destination->append(oldchild);
  SoPath * source = new SoPath(root);
  source->ref();
  source->append(middle);
  source->append(leaf);
  SoPath * others[3];
  for (int i = 0; i < 3; i++) {
    others[i] = new SoPath(leaf);
    others[i]->ref();
  }

  // Root and middle accept the new auditor; the full leaf list rejects it.
  // Rollback must remove both registrations, including the shared root.
  failafter = 0;
  bool caught = false;
  try { *destination = *source; }
  catch (const std::bad_alloc &) { caught = true; }
  failafter = -1;
  bool ok = caught && destination->getLength() == 2 &&
            destination->getNode(1) == oldchild;
  root->insertChild(new SoGroup, 0);
  middle->insertChild(new SoGroup, 0);
  ok = ok && destination->getIndex(1) == 1 &&
       source->getIndex(1) == 2 && source->getIndex(2) == 1;

  *destination = *source;
  root->insertChild(new SoGroup, 0);
  middle->insertChild(new SoGroup, 0);
  ok = ok && destination->getLength() == 3 &&
       destination->getIndex(1) == 3 && destination->getIndex(2) == 2 &&
       destination->getIndex(1) == source->getIndex(1) &&
       destination->getIndex(2) == source->getIndex(2);

  destination->unref();
  root->insertChild(new SoGroup, 0);
  middle->insertChild(new SoGroup, 0);
  leaf->insertChild(new SoGroup, 0); // No stale destination auditor.
  for (int i = 0; i < 3; i++) others[i]->unref();
  source->unref();
  root->unref();
  return ok;
}

static bool
copy_construction_failure()
{
  SoSeparator * root = new SoSeparator;
  root->ref();
  SoGroup * child = new SoGroup;
  root->addChild(child);
  SoPath * source = new SoPath(root);
  source->ref();
  source->append(child);
  SoPath * others[3];
  for (int i = 0; i < 3; i++) {
    others[i] = new SoPath(child);
    others[i]->ref();
  }

  // Copy construction registers on root, then fails when child's auditor
  // list grows. The partially constructed path must leave no auditor behind.
  failafter = 0;
  bool caught = false;
  try {
    SoPath * copy = new SoPath(*source);
    copy->ref();
    copy->unref();
  }
  catch (const std::bad_alloc &) { caught = true; }
  failafter = -1;
  root->insertChild(new SoGroup, 0);
  const bool ok = caught && source->getIndex(1) == 1;
  for (int i = 0; i < 3; i++) others[i]->unref();
  source->unref();
  root->insertChild(new SoGroup, 0);
  child->insertChild(new SoGroup, 0);
  root->unref();
  return ok;
}

static bool
empty_and_self_assignment()
{
  SoSeparator * root = new SoSeparator;
  root->ref();
  SoGroup * child = new SoGroup;
  root->addChild(child);
  SoPath * path = new SoPath(root);
  path->ref();
  path->append(child);
  SoPath * empty = new SoPath;
  empty->ref();

  failafter = 0;
  *path = *path;
  bool ok = failafter == 0 && path->getLength() == 2;
  *empty = *empty;
  ok = ok && failafter == 0 && empty->getLength() == 0;
  failafter = -1;

  *path = *empty;
  ok = ok && path->getLength() == 0;
  root->insertChild(new SoGroup, 0); // Old path registration is gone.
  SoPath * replacement = new SoPath(root);
  replacement->ref();
  *path = *replacement; // Reuse the emptied destination.
  ok = ok && path->getHead() == root && path->getLength() == 1;

  replacement->unref();
  path->unref();
  empty->unref();
  root->unref();
  return ok;
}



static bool
shorter_shared_root_assignment()
{
  SoSeparator * root = new SoSeparator;
  root->ref();
  SoGroup * middle = new SoGroup;
  SoGroup * leaf = new SoGroup;
  root->addChild(middle);
  middle->addChild(leaf);
  SoPath * destination = new SoPath(root);
  destination->ref();
  destination->append(middle);
  destination->append(leaf);
  SoPath * shortpath = new SoPath(root);
  shortpath->ref();

  *destination = *shortpath;
  bool ok = destination->getLength() == 1;
  middle->insertChild(new SoGroup, 0); // Old middle auditor is gone.
  root->insertChild(new SoGroup, 0);
  ok = ok && destination->getLength() == 1;

  SoPath * longpath = new SoPath(root);
  longpath->ref();
  longpath->append(middle);
  longpath->append(leaf);
  *destination = *longpath;
  middle->insertChild(new SoGroup, 0);
  root->insertChild(new SoGroup, 0);
  ok = ok && destination->getLength() == 3 &&
       destination->getIndex(1) == 2 && destination->getIndex(2) == 2 &&
       destination->getIndex(1) == longpath->getIndex(1) &&
       destination->getIndex(2) == longpath->getIndex(2);

  destination->unref();
  middle->insertChild(new SoGroup, 0); // No stale destination auditor.
  longpath->unref();
  shortpath->unref();
  root->unref();
  return ok;
}

static bool
temporary_path_assignment()
{
  SoSeparator * root = new SoSeparator;
  root->ref();
  SoGroup * child = new SoGroup;
  root->addChild(child);
  SoPath * destination = new SoPath(root);
  destination->ref();
  destination->append(child);

  SoTempPath temporary(2);
  temporary.simpleAppend(root, -1);
  temporary.simpleAppend(child, 0);
  *destination = temporary;
  root->insertChild(new SoGroup, 0);
  bool ok = destination->getIndex(1) == 0; // No auditor on temporary path.

  SoPath * audited = new SoPath(root);
  audited->ref();
  audited->append(child);
  *destination = *audited;
  root->insertChild(new SoGroup, 0);
  ok = ok && destination->getIndex(1) == 2 &&
       destination->getIndex(1) == audited->getIndex(1);

  destination->unref();
  audited->unref();
  root->unref();
  return ok;
}

int
main()
{
  SoDB::init();
  bool ok = true;
  if (!node_copy_failure()) {
    std::fprintf(stderr, "node or index growth rollback failed\n");
    ok = false;
  }
  if (!auditor_registration_failure()) {
    std::fprintf(stderr, "auditor registration rollback failed\n");
    ok = false;
  }
  if (!late_auditor_registration_failure()) {
    std::fprintf(stderr, "late auditor registration rollback failed\n");
    ok = false;
  }
  if (!copy_construction_failure()) {
    std::fprintf(stderr, "copy construction rollback failed\n");
    ok = false;
  }
  if (!empty_and_self_assignment()) {
    std::fprintf(stderr, "empty or self assignment failed\n");
    ok = false;
  }
  if (!shorter_shared_root_assignment()) {
    std::fprintf(stderr, "shorter shared-root assignment failed\n");
    ok = false;
  }
  if (!temporary_path_assignment()) {
    std::fprintf(stderr, "temporary path assignment failed\n");
    ok = false;
  }
  SoDB::finish();
  return ok ? 0 : 1;
}
