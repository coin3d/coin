#include <Inventor/SoDB.h>
#include "fonts/fontspec.h"
#include "fonts/glyph2d.h"
#include <limits>
#include <initializer_list>
#include <cstdio>
int main() {
 SoDB::init(); cc_font_specification spec;
 cc_fontspec_construct(&spec,"DejaVu Sans",24,0.5f);
 auto *zero=cc_glyph2d_ref('A',&spec,0);
 auto *rotated=cc_glyph2d_ref('A',&spec,0.7f);
 auto *again=cc_glyph2d_ref('A',&spec,0.7f);
 auto *turn=cc_glyph2d_ref('A',&spec,6.283185307179586f);
 auto *negative=cc_glyph2d_ref('A',&spec,-0.7f);
 auto *alias=cc_glyph2d_ref('A',&spec,6.283185307179586f-0.7f);
 auto *nan=cc_glyph2d_ref('A',&spec,std::numeric_limits<float>::quiet_NaN());
 const bool ok=zero!=rotated && rotated==again && zero==turn && negative==alias && nan==zero;
 for(auto *glyph:{zero,rotated,again,turn,negative,alias,nan}) cc_glyph2d_unref(glyph);
 cc_fontspec_clean(&spec); SoDB::finish();
 std::printf("Font angle: 5 cases, %d failures\n",ok?0:1); return ok?0:1;
}
