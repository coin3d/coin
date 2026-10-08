#include <Inventor/C/base/rbptree.h>

#include <type_traits>

static_assert(std::is_standard_layout<cc_rbptree>::value,
              "cc_rbptree must retain its C-compatible layout");
static_assert(std::is_default_constructible<cc_rbptree>::value,
              "cc_rbptree must remain default constructible");
static_assert(!std::is_copy_constructible<cc_rbptree>::value,
              "cc_rbptree must not share owned nodes through a copy");
static_assert(!std::is_copy_assignable<cc_rbptree>::value,
              "cc_rbptree must not share owned nodes through assignment");
static_assert(!std::is_move_constructible<cc_rbptree>::value,
              "cc_rbptree has no ownership-transferring move operation");
static_assert(!std::is_move_assignable<cc_rbptree>::value,
              "cc_rbptree has no ownership-transferring move assignment");

int
main()
{
  cc_rbptree tree;
  cc_rbptree_init(&tree);
  cc_rbptree_clean(&tree);
  return 0;
}
