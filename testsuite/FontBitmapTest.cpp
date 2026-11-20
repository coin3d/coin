#include "fonts/bitmap.h"
#include <cstdio>
#include <vector>
#include <algorithm>

static int cases = 0, failures = 0;
static void check(const char *name, bool ok) {
  ++cases;
  if (!ok) { ++failures; std::fprintf(stderr, "FAIL %s\n", name); }
}
static int allocations;
static void *reject(size_t) { ++allocations; return NULL; }
int main() {
  const unsigned char topdown[] = {1,2,9, 3,4,8, 5,6,7};
  const unsigned char bottomup[] = {5,6,7, 3,4,8, 1,2,9};
  for (int sign : {1,-1}) {
    unsigned char *buffer = NULL;
    unsigned int stride = 0;
    const unsigned char *source = sign > 0 ? topdown : bottomup;
    const bool copied = coin_font_bitmap_copy(3,2,sign*3,source,FALSE,buffer,stride);
    check("signed pitch copy", copied && stride == 3 && buffer && std::memcmp(buffer,topdown,9)==0);
    cc_font_bitmap bitmap = {};
    bitmap.buffer=buffer; bitmap.rows=3; bitmap.width=2; bitmap.pitch=stride;
    check("bottom-up flip", copied && coin_font_bitmap_flip(bitmap) && std::memcmp(buffer,bottomup,9)==0);
    check("flip involution", copied && coin_font_bitmap_flip(bitmap) && std::memcmp(buffer,topdown,9)==0);
    std::free(buffer);
  }
  unsigned char seed = 0;
  unsigned char *buffer = &seed;
  unsigned int stride = 91;
  check("allocation failure", !coin_font_bitmap_copy(3,2,3,topdown,FALSE,buffer,stride,reject)
        && buffer == &seed && stride == 91 && allocations == 1);
  check("empty needs no allocation", coin_font_bitmap_copy(0,0,0,NULL,FALSE,buffer,stride,reject)
        && buffer == NULL && stride == 0 && allocations == 1);
  check("zero width needs no allocation", coin_font_bitmap_copy(5,0,0,NULL,FALSE,buffer,stride,reject)
        && buffer == NULL && allocations == 1);
  size_t bytes = 19;
  check("gray stride too small", !coin_font_bitmap_size(3,4,3,FALSE,bytes));
  check("mono stride too small", !coin_font_bitmap_size(3,9,1,TRUE,bytes));
  check("zero stride", !coin_font_bitmap_size(3,1,0,FALSE,bytes));
  check("height range", !coin_font_bitmap_size(UINT_MAX,1,1,FALSE,bytes));
  check("width range", !coin_font_bitmap_size(1,UINT_MAX,UINT_MAX,FALSE,bytes));
  check("padded mono width range", !coin_font_bitmap_size(1,1,unsigned(INT_MAX)/8+1,TRUE,bytes));
  check("minimum pitch", !coin_font_bitmap_copy(1,1,INT_MIN,topdown,FALSE,buffer,stride,reject));
  check("missing data", !coin_font_bitmap_copy(3,2,3,NULL,FALSE,buffer,stride,reject));
  cc_font_bitmap bitmap = {};
  bitmap.rows=3; bitmap.width=2; bitmap.pitch=3;
  check("missing data flip", !coin_font_bitmap_flip(bitmap));
  bitmap.rows=0;
  check("empty flip", coin_font_bitmap_flip(bitmap));
  const unsigned char mono[] = {0x80,0x80, 0x40,0x40};
  check("mono padding preserved", coin_font_bitmap_copy(2,9,2,mono,TRUE,buffer,stride)
        && stride == 2 && std::memcmp(buffer,mono,4)==0);
  std::free(buffer);
  for (bool tall : {false,true}) {
    std::vector<unsigned char> pixels(70000,42);
    check("dimensions above 16 bits", coin_font_bitmap_copy(tall ? 70000 : 1,tall ? 1 : 70000,
          tall ? 1 : 70000,pixels.data(),FALSE,buffer,stride) && buffer &&
          std::memcmp(buffer,pixels.data(),pixels.size())==0);
    std::free(buffer);
  }
  // On 32-bit targets this product must be rejected before allocation; on
  // 64-bit targets inject allocation failure instead of requesting gigabytes.
  check("large product", !coin_font_bitmap_copy(INT_MAX,INT_MAX,INT_MAX,topdown,FALSE,buffer,stride,reject));
  std::printf("Font bitmap: %d cases, %d failures\n", cases, failures);
  return failures ? 1 : 0;
}
