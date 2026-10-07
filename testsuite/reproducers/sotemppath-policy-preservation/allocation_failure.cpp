// Regression for exception safety when appending a path to an empty destination.
#include <Inventor/SoDB.h>
#include <Inventor/SoPath.h>
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
list_growth_failure()
{
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
    SoPath * destination = new SoPath;
    destination->ref();
    failafter = stage;
    bool caught = false;
    try { destination->append(source); }
    catch (const std::bad_alloc &) { caught = true; }
    failafter = -1;
    if (!caught || destination->getLength() != 0) {
      std::fprintf(stderr, "list growth failure left a partial path (stage %d)\n", stage);
      return false; // Avoid destroying a path with mismatched lists.
    }
    destination->unref();
  }

  source->unref();
  root->unref();
  return ok;
}

static bool
auditor_registration_failure()
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
  SoPath * destination = new SoPath;
  destination->ref();

  // Root registration succeeds, then the full child auditor list must grow.
  failafter = 0;
  bool caught = false;
  try { destination->append(source); }
  catch (const std::bad_alloc &) { caught = true; }
  failafter = -1;
  if (!caught || destination->getLength() != 0) {
    std::fprintf(stderr, "auditor failure left a partial path\n");
    return false; // Avoid destroying a path with an unregistered auditor.
  }

  root->insertChild(new SoGroup, 0);
  bool ok = source->getIndex(1) == 1;
  destination->append(source);
  root->insertChild(new SoGroup, 0);
  ok = ok && destination->getLength() == 2 &&
       destination->getIndex(1) == 2 &&
       destination->getIndex(1) == source->getIndex(1);
  destination->unref();
  root->insertChild(new SoGroup, 0); // No stale auditor remains.
  for (int i = 0; i < 3; i++) others[i]->unref();
  source->unref();
  root->unref();
  return ok;
}

int
main()
{
  SoDB::init();
  const bool ok = list_growth_failure() && auditor_registration_failure();
  SoDB::finish();
  return ok ? 0 : 1;
}
