// Exercise the actual fixture replacements, in their original translation unit.
// Each executable includes exactly one owner; its existing main is retained.
#if defined(COIN_HOOK_BUMP)
#include "bumprender/AllocationFailure.h"
#elif defined(COIN_HOOK_PATH)
#define main coin_original_allocation_failure_main
#include "reproducers/sopath-assignment-auditor-lifecycle/allocation_failure.cpp"
#undef main
#elif defined(COIN_HOOK_TEMP_PATH)
#define main coin_original_allocation_failure_main
#include "reproducers/sotemppath-policy-preservation/allocation_failure.cpp"
#undef main
#elif defined(COIN_HOOK_SENSOR)
#define main coin_original_allocation_failure_main
#include "SensorQueueAllocationFailureTest.cpp"
#undef main
#else
#error Select an allocation hook owner
#endif

#include <cstdio>
#include <cstring>
#if defined(COIN_HOOK_BUMP)
#include <atomic>
#include <thread>
#endif

static bool check(bool condition, const char * message)
{
  if (!condition) std::fprintf(stderr, "%s\n", message);
  return condition;
}

static void arm(int budget)
{
#if defined(COIN_HOOK_BUMP)
  BumpTestAllocation::remaining = budget;
#elif defined(COIN_HOOK_SENSOR)
  failnextarray = budget == 0;
#else
  failafter = budget;
#endif
}

static int budgetLeft()
{
#if defined(COIN_HOOK_BUMP)
  return BumpTestAllocation::remaining;
#elif defined(COIN_HOOK_SENSOR)
  return failnextarray ? 0 : -1;
#else
  return failafter;
#endif
}

// Volatile function pointers prevent allocation elision and force the actual
// replacement entry points, including when the compiler optimizes this test.
static void * allocate(bool array, bool nothrow, std::size_t bytes)
{
  if (nothrow) {
    typedef void * (*Function)(std::size_t, const std::nothrow_t &);
    Function volatile function = array ?
      static_cast<Function>(&::operator new[]) : static_cast<Function>(&::operator new);
    return function(bytes, std::nothrow);
  }
  typedef void * (*Function)(std::size_t);
  Function volatile function = array ?
    static_cast<Function>(&::operator new[]) : static_cast<Function>(&::operator new);
  return function(bytes);
}

static void release(void * memory, bool array, int form, std::size_t bytes)
{
  if (form == 1) {
    typedef void (*Function)(void *, const std::nothrow_t &);
    Function volatile function = array ?
      static_cast<Function>(&::operator delete[]) : static_cast<Function>(&::operator delete);
    function(memory, std::nothrow);
  }
#if defined(__cpp_sized_deallocation)
  else if (form == 2) {
    typedef void (*Function)(void *, std::size_t);
    Function volatile function = array ?
      static_cast<Function>(&::operator delete[]) : static_cast<Function>(&::operator delete);
    function(memory, bytes);
  }
#endif
  else {
    typedef void (*Function)(void *);
    Function volatile function = array ?
      static_cast<Function>(&::operator delete[]) : static_cast<Function>(&::operator delete);
    function(memory);
  }
  (void) bytes;
}

static bool familySuccess(bool array)
{
  bool ok = true;
  const std::size_t sizes[] = { 0, 1, 37, 4097 };
  for (unsigned int i = 0; i < sizeof(sizes) / sizeof(sizes[0]); ++i) {
    for (int nt = 0; nt < 2; ++nt) {
      for (int form = 0; form < 3; ++form) {
#if !defined(__cpp_sized_deallocation)
        if (form == 2) continue;
#endif
        arm(-1);
        void * memory = allocate(array, nt != 0, sizes[i]);
        ok &= check(memory != NULL, "successful allocation, including zero bytes, returned NULL");
        if (memory && sizes[i]) {
          std::memset(memory, 0x5a, sizes[i]);
          ok &= check(static_cast<unsigned char *>(memory)[sizes[i] - 1] == 0x5a,
                      "allocation does not provide the requested writable extent");
        }
        release(memory, array, form, sizes[i]);
        release(NULL, array, form, sizes[i]);
      }
    }
  }
  return ok;
}

static bool familyBudget(bool array)
{
  bool ok = true;
  for (int nt = 0; nt < 2; ++nt) {
#if defined(COIN_HOOK_SENSOR)
    const int maximum = 0;
#else
    const int maximum = 3;
#endif
    for (int budget = 0; budget <= maximum; ++budget) {
      arm(budget);
#if defined(COIN_HOOK_BUMP)
      BumpTestAllocation::Count count;
#endif
      for (int i = 0; i < budget; ++i) {
        void * memory = allocate(array, (i % 2) != 0, i == 0 ? 0 : 19);
        ok &= check(memory != NULL, "allocation failed before the requested budget");
        release(memory, array, 0, i == 0 ? 0 : 19);
        ok &= check(budgetLeft() == budget - i - 1, "allocation consumed the wrong budget");
      }
      void * memory = NULL;
      bool badalloc = false;
      bool other = false;
      try { memory = allocate(array, nt != 0, 0); }
      catch (const std::bad_alloc &) { badalloc = true; }
      catch (...) { other = true; }
      ok &= check(!other && (nt ? (!badalloc && memory == NULL) : (badalloc && memory == NULL)),
                  "injected failure violated throwing/nothrow contract");
      if (memory) release(memory, array, 0, 0);
      ok &= check(budgetLeft() == -1, "failure was not consumed exactly once");
      memory = allocate(array, nt == 0, 23);
      ok &= check(memory != NULL, "one-shot failure incorrectly affected the next allocation");
      release(memory, array, 0, 23);
#if defined(COIN_HOOK_BUMP)
      ok &= check(count.total() == static_cast<std::size_t>(budget + 2),
                  "counter must count successes and the failed attempt exactly once");
#endif
      arm(-1);
    }
  }
  return ok;
}

struct ConstructorFailure { };
struct ThrowingElement {
  static int attempts;
  static int live;
  static int failAt;
  ThrowingElement() {
    if (++attempts == failAt) throw ConstructorFailure();
    ++live;
  }
  ~ThrowingElement() { --live; }
};
int ThrowingElement::attempts = 0;
int ThrowingElement::live = 0;
int ThrowingElement::failAt = 0;

static bool constructorCleanup(bool array)
{
  // familyBudget() tests failed allocations through volatile function pointers.
  // A new-expression can omit a call to a replaceable global allocation
  // function, so only test constructor unwinding here.
  bool ok = true;
  for (int nt = 0; nt < 2; ++nt) {
    arm(-1);
    ThrowingElement::attempts = 0;
    ThrowingElement::failAt = array ? 2 : 1;
    bool caught = false;
    try {
      ThrowingElement * object = array ?
        (nt ? new(std::nothrow) ThrowingElement[3] : new ThrowingElement[3]) :
        (nt ? new(std::nothrow) ThrowingElement : new ThrowingElement);
      if (array) delete[] object;
      else delete object;
    }
    catch (const ConstructorFailure &) { caught = true; }
    catch (...) { }
    ok &= check(caught && ThrowingElement::attempts == ThrowingElement::failAt &&
                ThrowingElement::live == 0,
                "constructor exception must escape and destroy only constructed elements");
  }
  return ok;
}

#if defined(COIN_HOOK_BUMP)
static bool threadIsolation()
{
  std::atomic<int> phase(0);
  bool workerOK = false;
  std::thread worker([&]() {
    while (phase.load() != 1) std::this_thread::yield();
    bool localOK = check(budgetLeft() == -1, "worker inherited main-thread failure budget");
    BumpTestAllocation::Count count;
    void * scalar = allocate(false, true, 7);
    void * array = allocate(true, false, 11);
    localOK &= check(scalar && array && count.total() == 2,
                     "worker allocation counters are not local or count twice");
    release(scalar, false, 0, 7);
    release(array, true, 0, 11);
    arm(0);
    localOK &= check(allocate(true, true, 0) == NULL && budgetLeft() == -1,
                     "worker nothrow failure was not one-shot");
    workerOK = localOK;
    phase.store(2);
  });
  arm(0);
  BumpTestAllocation::Count count;
  phase.store(1);
  while (phase.load() != 2) std::this_thread::yield();
  bool ok = check(budgetLeft() == 0 && count.total() == 0,
                  "worker changed main-thread budget or allocation count");
  ok &= check(allocate(false, true, 9) == NULL && count.total() == 1,
              "main-thread armed failure was lost after worker allocations");
  arm(-1);
  worker.join();
  arm(1);
  BumpTestAllocation::Count mixedCount;
  void * first = allocate(true, true, 13);
  ok &= check(first != NULL && budgetLeft() == 0,
              "array nothrow allocation must consume the shared scalar/array budget");
  release(first, true, 0, 13);
  ok &= check(allocate(false, true, 17) == NULL && mixedCount.total() == 2,
              "scalar and array overloads must share one failure budget and counter");
  arm(-1);
  {
    BumpTestAllocation::Scope scope(0);
    ok &= check(scope.untouched(), "fresh scope should be untouched");
    ok &= check(allocate(true, true, 1) == NULL && !scope.untouched(),
                "nothrow failure must consume Scope in the same way as throwing failure");
  }
  ok &= check(budgetLeft() == -1, "Scope destructor did not disable failure injection");
  return ok && workerOK;
}
#else
static bool scalarIsolation()
{
  bool ok = true;
  arm(0);
  for (int nt = 0; nt < 2; ++nt) {
    void * memory = allocate(false, nt != 0, 29);
    ok &= check(memory != NULL && budgetLeft() == 0,
                "scalar allocation consumed an array-only failure budget");
    release(memory, false, 0, 29);
  }
  void * array = allocate(true, true, 29);
  ok &= check(array == NULL && budgetLeft() == -1,
              "array failure must survive scalar allocations and then be consumed");
  if (array) release(array, true, 0, 29);
  arm(-1);
  return ok;
}
#endif

int main(int argc, char ** argv)
{
  if (argc != 2) return 2;
  bool ok = true;
  if (std::strcmp(argv[1], "success") == 0) {
    ok &= familySuccess(true);
#if defined(COIN_HOOK_BUMP)
    ok &= familySuccess(false);
#endif
  }
  else if (std::strcmp(argv[1], "budget") == 0) {
    ok &= familyBudget(true);
#if defined(COIN_HOOK_BUMP)
    ok &= familyBudget(false);
#endif
  }
  else if (std::strcmp(argv[1], "constructor") == 0) {
    ok &= constructorCleanup(true);
#if defined(COIN_HOOK_BUMP)
    ok &= constructorCleanup(false);
#endif
  }
  else if (std::strcmp(argv[1], "isolation") == 0) {
#if defined(COIN_HOOK_BUMP)
    ok &= threadIsolation();
#else
    ok &= scalarIsolation();
#endif
  }
  else return 2;
  return ok ? 0 : 1;
}
