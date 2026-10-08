#include <Inventor/C/base/rbptree.h>

#include <atomic>
#include <cstdio>
#include <thread>

static void
exercise(cc_rbptree * tree, int * keys, std::atomic<bool> * start,
         std::atomic<bool> * ok)
{
  while (!start->load(std::memory_order_acquire)) {}
  for (int i = 0; i < 4000; ++i) {
    for (int j = 0; j < 3; ++j) cc_rbptree_insert(tree, &keys[j], NULL);
    for (int j = 2; j >= 0; --j) {
      if (!cc_rbptree_remove(tree, &keys[j])) ok->store(false);
    }
    if (cc_rbptree_size(tree) != 0) ok->store(false);
  }
}

int
main()
{
  cc_rbptree first, second;
  cc_rbptree_init(&first);
  cc_rbptree_init(&second);
  int firstkeys[3], secondkeys[3];
  std::atomic<bool> start(false), ok(true);
  std::thread one(exercise, &first, firstkeys, &start, &ok);
  std::thread two(exercise, &second, secondkeys, &start, &ok);
  start.store(true, std::memory_order_release);
  one.join();
  two.join();
  const bool passed = ok.load() &&
    cc_rbptree_size(&first) == 0 && cc_rbptree_size(&second) == 0;
  cc_rbptree_clean(&first);
  cc_rbptree_clean(&second);
  if (!passed) std::fprintf(stderr, "parallel trees lost entries\n");
  return passed ? 0 : 1;
}
