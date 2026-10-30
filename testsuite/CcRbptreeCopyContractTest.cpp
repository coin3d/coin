#include <Inventor/C/base/rbptree.h>

#include <type_traits>

static_assert(std::is_standard_layout<cc_rbptree>::value,
              "cc_rbptree must retain its C-compatible layout");
static_assert(std::is_trivial<cc_rbptree>::value,
              "cc_rbptree must retain its historical C/C++ ABI classification");
static_assert(std::is_trivially_copyable<cc_rbptree>::value,
              "cc_rbptree must retain its historical C++ ABI classification");

int
main()
{
  cc_rbptree tree = {};
  cc_rbptree_init(&tree);
  cc_rbptree_clean(&tree);
  return 0;
}
