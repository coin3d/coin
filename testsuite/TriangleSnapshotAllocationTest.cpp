#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <new>

namespace {
void * owned[4096] = {};
bool track = false;
int failAfter = -1;
int live = 0;
int failures = 0;
std::size_t failedSize = 0;
}

void * operator new(std::size_t size)
{
  if (failAfter == 0) {
    failAfter = -1;
    ++failures;
    failedSize = size;
    throw std::bad_alloc();
  }
  if (failAfter > 0) --failAfter;
  void * pointer = std::malloc(size ? size : 1);
  if (!pointer) throw std::bad_alloc();
  if (track) {
    for (auto & slot : owned) {
      if (!slot) { slot = pointer; ++live; return pointer; }
    }
    std::abort();
  }
  return pointer;
}
void operator delete(void * pointer) noexcept
{
  if (!pointer) return;
  for (auto & slot : owned) {
    if (slot == pointer) { slot = nullptr; --live; break; }
  }
  std::free(pointer);
}
void operator delete(void * pointer, std::size_t) noexcept { ::operator delete(pointer); }
void * operator new[](std::size_t size) { return ::operator new(size); }
void operator delete[](void * pointer) noexcept { ::operator delete(pointer); }
void operator delete[](void * pointer, std::size_t) noexcept { ::operator delete(pointer); }

// Keep nothrow allocations in the same allocator family as the throwing
// replacements. Otherwise ASan can supply new[] while this test supplies free.
void * operator new(std::size_t size, const std::nothrow_t &) noexcept
{
  try { return ::operator new(size); }
  catch (const std::bad_alloc &) { return nullptr; }
}
void * operator new[](std::size_t size, const std::nothrow_t &) noexcept
{
  try { return ::operator new[](size); }
  catch (const std::bad_alloc &) { return nullptr; }
}
void operator delete(void * pointer, const std::nothrow_t &) noexcept { ::operator delete(pointer); }
void operator delete[](void * pointer, const std::nothrow_t &) noexcept { ::operator delete[](pointer); }

#include "TriangleSnapshotTestDatabase.h"
#include <Inventor/SoTriangleSnapshot.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/actions/SoAction.h>
#include <Inventor/nodes/SoFaceSet.h>
#include <Inventor/nodes/SoCallback.h>
#include <Inventor/nodes/SoSeparator.h>
#include <cstring>
#include <limits>
#include <stdexcept>

namespace {
void armStack(void * closure, SoAction *)
{
  if (*static_cast<bool *>(closure)) failAfter = 0;
}

class AllocationShape : public SoFaceSet {
public:
  int injection = -1;
  bool completed = false;
  int producerResources = 0;
protected:
  void generatePrimitives(SoAction * action) override
  {
    ++producerResources;
    SoPrimitiveVertex vertices[3];
    for (int i = 0; i < 3; ++i) {
      vertices[i].setPoint(SbVec3f(float(i == 1),float(i == 2),0));
      vertices[i].setNormal(SbVec3f(0,0,1));
      vertices[i].setMaterialIndex(0);
    }
    // The next allocations belong to snapshot matrices and triangle storage.
    if (injection == 0 || injection == 1) failAfter = injection;
    this->invokeTriangleCallbacks(action,&vertices[0],&vertices[1],&vertices[2]);
    // The second triangle forces growth after partial capture.
    if (injection == 2) failAfter = 0;
    this->invokeTriangleCallbacks(action,&vertices[0],&vertices[1],&vertices[2]);
    --producerResources;
    completed = true;
  }
};
}

int main(int argc, char ** argv)
{
  if (argc != 2) return 10;
  const bool data = std::strcmp(argv[1],"data") == 0;
  const bool stack = std::strcmp(argv[1],"stack") == 0;
  const bool triangleLimit = std::strcmp(argv[1],"triangle-limit") == 0;
  const bool storageLimit = std::strcmp(argv[1],"storage-limit") == 0;
  const bool limited = triangleLimit || storageLimit;
  const int injection = data || stack || limited ? -1 : std::atoi(argv[1]);
  SnapshotTestDatabase database;
  auto * root = new SoSeparator;
  root->ref();
  bool arm = false;
  auto * callback = new SoCallback;
  callback->setCallback(armStack, &arm);
  root->addChild(callback);
  auto * shape = new AllocationShape;
  root->addChild(shape);
  // Initialize Coin's persistent lazy caches before accounting for a capture.
  std::size_t peak = 0;
  { SoTriangleSnapshot warm(*root, SbViewportRegion(64,64)); peak = warm.getPeakStorageBytes(); }
  shape->completed = false;
  shape->injection = injection;
  arm = stack;
  track = true;
  if (data) failAfter = 0;
  bool caught = false;
  const std::size_t unlimited = std::numeric_limits<std::size_t>::max();
  try {
    SoTriangleSnapshot snapshot(*root, SbViewportRegion(64,64),
      {triangleLimit ? 1 : unlimited, storageLimit ? peak-1 : unlimited});
  }
  catch (const std::bad_alloc &) { caught = !limited; }
  catch (const std::length_error &) { caught = limited; }
  failAfter = -1;
  track = false;
  arm = false;
  const int leaked = live;
  const std::size_t expectedSize = stack ? sizeof(std::size_t) :
    injection == 0 ? sizeof(SbMatrix) :
    injection == 1 ? sizeof(SoTriangleSnapshot::Triangle) :
                     2 * sizeof(SoTriangleSnapshot::Triangle);
  const bool site = data || limited || failedSize == expectedSize;
  const bool references = root->getRefCount() == 1 && shape->getRefCount() == 1;
  const bool completed = shape->completed;
  const int resources = shape->producerResources;
  shape->injection = -1;
  bool recovered = false;
  {
    track = true;
    SoTriangleSnapshot recovery(*root, SbViewportRegion(64,64));
    track = false;
    recovered = recovery.getNumTriangles() == 2 &&
                recovery.getNumOccurrences() == 1 &&
                recovery.getTriangle(1).vertices[1].point == SbVec3f(1,0,0);
  }
  root->unref();
  std::printf("caught=%d failures=%d failed_bytes=%zu capture_live=%d final_live=%d producer_resources=%d completed=%d recovered=%d\n",
              int(caught),failures,failedSize,leaked,live,resources,int(completed),int(recovered));
  return site && references && caught && failures == (limited ? 0 : 1) && leaked == 0 && live == 0 &&
         resources == 0 && (data || stack || completed) && recovered ? 0 : 1;
}
