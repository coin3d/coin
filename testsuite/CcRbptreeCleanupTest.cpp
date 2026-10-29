#include <Inventor/SoDB.h>
#include <Inventor/C/base/rbptree.h>
#include <Inventor/C/tidbits.h>

#include <cstdio>

static cc_rbptree trees[5];
static int values[8];
static int cleanupcalls;
static bool valid = true;

static void
countEntry(void *, void *, void * closure)
{
  ++*static_cast<int *>(closure);
}

static void
cleanTrees(void)
{
  ++cleanupcalls;
  for (int i = 0; i < 5; ++i) {
    // Static-data cleanup runs after the normal-priority rbptree cleanup.
    // Cleaning an existing tree must not register a new shutdown callback.
    cc_rbptree_clean(&trees[i]);
    cc_rbptree_clean(&trees[i]);
    int count = 0;
    cc_rbptree_traverse(&trees[i], countEntry, &count);
    valid = valid && cc_rbptree_size(&trees[i]) == 0 && count == 0;
  }
}

int
main(void)
{
  SoDB::init();
  const int lengths[5] = { 0, 1, 2, 3, 8 };
  for (int i = 0; i < 5; ++i) {
    cc_rbptree_init(&trees[i]);
    for (int j = 0; j < lengths[i]; ++j) {
      cc_rbptree_insert(&trees[i], &values[j], &values[j]);
    }
  }
  cc_coin_atexit_static_internal(cleanTrees);
  SoDB::finish();
  if (cleanupcalls != 1 || !valid) {
    std::fprintf(stderr, "rbptree cleanup failed during Coin shutdown\n");
    return 1;
  }
  return 0;
}
