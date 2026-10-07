#include <Inventor/SbDict.h>
#include <Inventor/lists/SbPList.h>
#include <cstdio>
#include <cstdlib>
#include <new>

static int arrays_before_failure = -1;
void * operator new[](std::size_t bytes) {
  if (arrays_before_failure == 0) { arrays_before_failure = -1; throw std::bad_alloc(); }
  if (arrays_before_failure > 0) --arrays_before_failure;
  void * memory = std::malloc(bytes ? bytes : 1);
  if (!memory) throw std::bad_alloc();
  return memory;
}
void operator delete[](void * memory) noexcept { std::free(memory); }
#if defined(__cpp_sized_deallocation)
void operator delete[](void * memory, std::size_t) noexcept { std::free(memory); }
#endif
#define CHECK(condition) do { if (!(condition)) { \
  std::fprintf(stderr, "line %d: %s\n", __LINE__, #condition); return 1; \
} } while (false)
int main() {
  int values[32];
  SbDict dictionary(17);
  for (unsigned int i = 0; i < 32; ++i) CHECK(dictionary.enter(i, &values[i]));
  for (int stage = 0; stage < 8; ++stage) {
    SbPList keys, mapped;
    for (int i = 0; i < 4; ++i) { keys.append(&values[i]); mapped.append(&values[i + 4]); }
    arrays_before_failure = stage;
    bool caught = false;
    try { dictionary.makePList(keys, mapped); }
    catch (const std::bad_alloc &) { caught = true; }
    arrays_before_failure = -1;
    CHECK(caught && keys.getLength() == 4 && mapped.getLength() == 4);
    for (int i = 0; i < 4; ++i) CHECK(keys[i] == &values[i] && mapped[i] == &values[i + 4]);
    for (unsigned int i = 0; i < 32; ++i) {
      void * found = NULL;
      CHECK(dictionary.find(i, found) && found == &values[i]);
    }
    dictionary.makePList(keys, mapped);
    CHECK(keys.getLength() == 36 && mapped.getLength() == 36);
    for (int i = 4; i < 36; ++i) {
      const SbDict::Key key = reinterpret_cast<SbDict::Key>(keys[i]);
      CHECK(key < 32 && mapped[i] == &values[key]);
    }
  }
  return 0;
}
