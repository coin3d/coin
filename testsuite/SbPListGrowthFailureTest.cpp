#include <Inventor/lists/SbPList.h>

#include <cstddef>
#include <cstdlib>
#include <new>

static bool failarrayallocation;

void *
operator new[](std::size_t bytes)
{
  if (failarrayallocation) {
    failarrayallocation = false;
    throw std::bad_alloc();
  }
  void * memory = std::malloc(bytes);
  if (memory == NULL) throw std::bad_alloc();
  return memory;
}

void
operator delete[](void * memory) noexcept
{
  std::free(memory);
}

class SbPListTestAccess : public SbPList {
public:
  int capacity(void) const { return this->getArraySize(); }
};

int
main(void)
{
  int values[5] = { 0, 1, 2, 3, 4 };
  SbPListTestAccess list;
  for (int i = 0; i < 4; i++) list.append(&values[i]);

  void ** array = list.getArrayPtr();
  failarrayallocation = true;
  bool caught = false;
  try {
    list.append(&values[4]);
  }
  catch (const std::bad_alloc &) {
    caught = true;
  }

  if (!caught || list.getLength() != 4 || list.capacity() != 4 ||
      list.getArrayPtr() != array) return 1;
  for (int i = 0; i < 4; i++) {
    if (list.get(i) != &values[i]) return 1;
  }

  list.append(&values[4]);
  if (list.getLength() != 5 || list.capacity() != 8 ||
      list.get(4) != &values[4]) return 1;
  return 0;
}
