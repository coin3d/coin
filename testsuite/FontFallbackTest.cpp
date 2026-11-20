#include <Inventor/SoDB.h>
#include "fonts/fontspec.h"
#include "fonts/glyph2d.h"
#include "fonts/glyph3d.h"
#include <cstdio>
#include <initializer_list>
int main() {
 SoDB::init(); cc_font_specification spec;
 cc_fontspec_construct(&spec,"defaultFont",12,0.5f);
 auto *a=cc_glyph2d_ref('A',&spec,0);
 auto *q=cc_glyph2d_ref('?',&spec,0);
 auto *q3=cc_glyph3d_ref('?',&spec);
 int size[2],offset[2],failures=0,cases=0;
 const auto *ap=cc_glyph2d_getbitmap(a,size,offset);
 const auto *qp=cc_glyph2d_getbitmap(q,size,offset);
 for (uint32_t value:{0x141u,0x1041u,0x1f641u,0x10ffffu}) {
  auto *glyph=cc_glyph2d_ref(value,&spec,0);
  auto *glyph3=cc_glyph3d_ref(value,&spec);
  const auto *pixels=cc_glyph2d_getbitmap(glyph,size,offset);
  ++cases;
  if(pixels==ap || pixels!=qp) {++failures;std::fprintf(stderr,"FAIL 2D replacement U+%x\n",value);}
  ++cases;
  if(cc_glyph3d_getcoords(glyph3)!=cc_glyph3d_getcoords(q3)) {++failures;std::fprintf(stderr,"FAIL 3D replacement U+%x\n",value);}
  cc_glyph2d_unref(glyph);cc_glyph3d_unref(glyph3);
 }
 auto *latin=cc_glyph2d_ref(0xe9,&spec,0);
 ++cases;if(cc_glyph2d_getbitmap(latin,size,offset)==qp)++failures;
 cc_glyph2d_unref(latin); cc_glyph2d_unref(a);cc_glyph2d_unref(q);cc_glyph3d_unref(q3);
 cc_fontspec_clean(&spec);SoDB::finish();
 std::printf("Font fallback: %d cases, %d failures\n",cases,failures);return failures?1:0;
}
