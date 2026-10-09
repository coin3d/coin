#include <Inventor/C/base/rbptree.h>
#include <Inventor/SoDB.h>
#include <atomic>
#include <cstdio>
#include <thread>

struct Contents { unsigned int count; bool valid; };
static void collect(void * pointer, void * data, void * closure) {
  Contents * contents = static_cast<Contents *>(closure);
  ++contents->count;
  contents->valid = contents->valid && pointer == data;
}
static void exercise(cc_rbptree * tree, int * keys, std::atomic<bool> * start,
                     std::atomic<bool> * ok) {
  while (!start->load(std::memory_order_acquire)) std::this_thread::yield();
  const int widths[] = {1, 2, 3, 8, 17, 32, 63};
  for (int i = 0; i < 600; ++i) {
    const int width = widths[i % 7];
    for (int j = 0; j < width; ++j) cc_rbptree_insert(tree, &keys[j], &keys[j]);
    Contents contents = {0, true};
    cc_rbptree_traverse(tree, collect, &contents);
    if (!contents.valid || contents.count != static_cast<unsigned int>(width)) ok->store(false);
    for (int j = 0; j < width; ++j) {
      const int index = i % 2 == 0 ? j : (31 * j) % width;
      if (!cc_rbptree_remove(tree, &keys[index]) ||
          cc_rbptree_size(tree) != static_cast<unsigned int>(width - j - 1)) ok->store(false);
    }
    if (cc_rbptree_remove(tree, &keys[63])) ok->store(false);
  }
}
int main() {
  SoDB::init();
  cc_rbptree first, second;
  cc_rbptree_init(&first); cc_rbptree_init(&second);
  int firstkeys[64] = {}, secondkeys[64] = {};
  std::atomic<bool> start(false), ok(true);
  std::thread one(exercise, &first, firstkeys, &start, &ok);
  std::thread two(exercise, &second, secondkeys, &start, &ok);
  start.store(true, std::memory_order_release);
  one.join(); two.join();
  const bool passed = ok.load() && cc_rbptree_size(&first) == 0 && cc_rbptree_size(&second) == 0;
  cc_rbptree_clean(&first); cc_rbptree_clean(&second);
  SoDB::finish();
  if (!passed) std::fprintf(stderr, "parallel trees lost entries or changed payloads\n");
  return passed ? 0 : 1;
}
