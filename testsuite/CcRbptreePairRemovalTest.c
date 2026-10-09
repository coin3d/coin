#include <Inventor/C/base/rbptree.h>
#include <stdio.h>

struct pair { void * key; void * data; };
struct observed { struct pair entries[64]; int count; };
static void collect(void * key, void * data, void * closure)
{
  struct observed * out = (struct observed *) closure;
  if (out->count < 64) {
    out->entries[out->count].key = key;
    out->entries[out->count].data = data;
  }
  ++out->count;
}
static int check_tree(const cc_rbptree * tree, const struct pair * expected, int count)
{
  struct observed out;
  int i, j;
  out.count = 0;
  cc_rbptree_traverse(tree, collect, &out);
  if (out.count != count || cc_rbptree_size(tree) != (unsigned int) count) return 0;
  for (i = 0; i < count; ++i) {
    int actual = 0, wanted = 0;
    for (j = 0; j < count; ++j) {
      actual += out.entries[j].key == expected[i].key && out.entries[j].data == expected[i].data;
      wanted += expected[j].key == expected[i].key && expected[j].data == expected[i].data;
    }
    if (actual != wanted) return 0;
  }
  return 1;
}
int main(void)
{
  cc_rbptree tree;
  struct pair expected[48];
  int keys[5] = {0}, values[7] = {0};
  int count = 0, i, j, ok = 1;
  cc_rbptree_init(&tree);
  ok &= !cc_rbptree_remove_with_data(&tree, &keys[0], NULL);
  for (i = 0; i < 48; ++i) {
    expected[count].key = &keys[i % 5];
    expected[count].data = i % 7 == 0 ? NULL : &values[i % 7];
    cc_rbptree_insert(&tree, expected[count].key, expected[count].data);
    ++count;
    ok &= check_tree(&tree, expected, count);
  }
  ok &= !cc_rbptree_remove_with_data(&tree, &keys[0], &values[0]);
  ok &= check_tree(&tree, expected, count);
  while (count > 0) {
    int at = ((count * 7 + 3) % 11) % count;
    ok &= cc_rbptree_remove_with_data(&tree, expected[at].key, expected[at].data);
    for (j = at; j + 1 < count; ++j) expected[j] = expected[j + 1];
    --count;
    ok &= check_tree(&tree, expected, count);
  }
  ok &= !cc_rbptree_remove_with_data(&tree, &keys[0], NULL);
  cc_rbptree_clean(&tree);
  if (!ok) fprintf(stderr, "typed removal changed the wrong pointer/data occurrence\n");
  return ok ? 0 : 1;
}
