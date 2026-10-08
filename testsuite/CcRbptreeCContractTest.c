#include <Inventor/C/base/rbptree.h>

int
main(void)
{
  cc_rbptree tree = { 0 };
  cc_rbptree_init(&tree);
  cc_rbptree_clean(&tree);
  return 0;
}
