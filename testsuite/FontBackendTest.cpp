// Linux symbol interposition substitutes only the FreeType glue calls. The
// production FreeType adapter, font wrapper and glyph cache remain unchanged.
#include <Inventor/SoDB.h>
#include "glue/freetype.h"
#include "fonts/freetype.h"
#include "fonts/glyph2d.h"
#include "fonts/fontspec.h"
#include "fonts/common.h"
#include <cstdio>
#include <climits>
#include <cstdlib>
#include <cstring>
#include <vector>

static FT_BitmapGlyphRec_ fixture;
static int released, loaderror, geterror, conversionerror, sizeset;
static long requestedheight;
extern "C" FT_Error cc_ftglue_FT_Set_Char_Size(FT_Face, long, long height, unsigned int, unsigned int) {
  ++sizeset; requestedheight=height; return 0;
}
extern "C" FT_Error cc_ftglue_FT_Load_Glyph(FT_Face, unsigned int, int32_t) { return loaderror; }
extern "C" FT_Error cc_ftglue_FT_Get_Glyph(void *, FT_Glyph *glyph) {
  if (geterror) return geterror;
  auto *copy = new FT_BitmapGlyphRec_(fixture);
  *glyph = reinterpret_cast<FT_Glyph>(copy);
  return 0;
}
extern "C" FT_Error cc_ftglue_FT_Glyph_To_Bitmap(FT_Glyph *, int, FT_Vector *, int) { return conversionerror; }
extern "C" void cc_ftglue_FT_Done_Glyph(FT_Glyph glyph) {
  ++released;
  delete reinterpret_cast<FT_BitmapGlyphRec_ *>(glyph);
}
static int cases, failures;
static void check(const char *name, bool ok) {
  ++cases;
  if (!ok) { ++failures; std::fprintf(stderr,"FAIL %s\n",name); }
}
static void destroy(cc_font_bitmap *bitmap) {
  if (bitmap) { std::free(bitmap->buffer); std::free(bitmap); }
}
int main(int argc, char **) {
  const bool metricsonly = argc > 1;
  SoDB::init();
  if (!cc_ftglue_available()) { SoDB::finish(); return 77; }
  FT_FaceRec_ face = {};
  FT_GlyphSlotRec_ slot = {};
  slot.advance.x=12*64;
  face.glyph=&slot;
  fixture.root.format=0x62697473u /* bits */;
  fixture.left=-40000; fixture.top=40000;
  const unsigned char topdown[]={1,2,9, 3,4,8, 5,6,7};
  unsigned char bottomup[]={5,6,7, 3,4,8, 1,2,9};
  fixture.bitmap.rows=3; fixture.bitmap.width=2; fixture.bitmap.pitch=-3;
  fixture.bitmap.buffer=bottomup; fixture.bitmap.pixel_mode=FT_PIXEL_MODE_GRAY; fixture.bitmap.num_grays=256;
  if (!metricsonly) {
  cc_flwft_set_char_size(&face,24);
  check("normal 26.6 size",sizeset==1 && requestedheight==24*64);
  cc_flwft_set_char_size(&face,67108864);
  check("large 26.6 size",sizeof(long)>4 ? sizeset==2 && requestedheight==4294967296L : sizeset==1);
  const int sizebefore=sizeset;
  cc_flwft_set_char_size(&face,-1);
  check("negative size rejected",sizeset==sizebefore);
  cc_font_bitmap *bitmap=cc_flwft_get_bitmap(&face,1);
  check("negative pitch adapter",bitmap && bitmap->pitch==3 && !bitmap->mono &&
        bitmap->bearingX==-40000 && bitmap->bearingY==40000 && bitmap->advanceX==12 &&
        std::memcmp(bitmap->buffer,topdown,9)==0 && released==1);
  destroy(bitmap);
  fixture.bitmap.pixel_mode=FT_PIXEL_MODE_MONO;
  bitmap=cc_flwft_get_bitmap(&face,1);
  check("mono mode",bitmap && bitmap->mono); destroy(bitmap);
  fixture.bitmap.pixel_mode=7; // BGRA: unsupported by SoText2's gray/mono consumers.
  int before=released;
  check("unsupported mode releases glyph",!cc_flwft_get_bitmap(&face,1) && released==before+1);
  fixture.bitmap.pixel_mode=FT_PIXEL_MODE_GRAY;
  fixture.bitmap.pitch=1;
  before=released;
  check("invalid stride releases glyph",!cc_flwft_get_bitmap(&face,1) && released==before+1);
  fixture.root.format=0x6f75746cu /* outl */;
  conversionerror=1;
  before=released;
  check("conversion error releases glyph",!cc_flwft_get_bitmap(&face,1) && released==before+1);
  conversionerror=0; fixture.root.format=0x62697473u /* bits */;
  loaderror=1;
  before=released;
  check("load error",!cc_flwft_get_bitmap(&face,1) && released==before);
  loaderror=0; geterror=1;
  check("get error",!cc_flwft_get_bitmap(&face,1) && released==before);
  geterror=0;
  }

  // Send synthetic large metrics through the real wrapper/cache. Allocations
  // are only 70 KB. The old unsigned-short/short storage truncated these.
  std::vector<unsigned char> pixels(70000,42);
  fixture.bitmap.buffer=pixels.data();
  for (int tall=0;tall<2;++tall) {
    fixture.bitmap.width=tall?1:70000;
    fixture.bitmap.rows=tall?70000:1;
    fixture.bitmap.pitch=tall?1:70000;
    cc_font_specification spec;
    cc_fontspec_construct(&spec,"DejaVu Sans",24+tall,0.5f);
    cc_glyph2d *glyph=cc_glyph2d_ref('M',&spec,0);
    int size[2]={},offset[2]={};
    const unsigned char *data=cc_glyph2d_getbitmap(glyph,size,offset);
    check("cache retains large dimensions and bearings",data && size[0]==(tall?1:70000) &&
          size[1]==(tall?70000:1) && offset[0]==-40000 && offset[1]==40000 &&
          cc_glyph2d_getwidth(glyph)==unsigned(tall?1:70000) && data[69999]==42);
    cc_glyph2d_unref(glyph);
    cc_fontspec_clean(&spec);
  }
  // The wrapper must reject a malformed native bitmap and return its valid
  // default glyph. No unbounded allocation is performed by this fixture.
  fixture.bitmap.width=2; fixture.bitmap.rows=3; fixture.bitmap.pitch=1;
  cc_font_specification spec;
  cc_fontspec_construct(&spec,"DejaVu Sans",26,0.5f);
  cc_glyph2d *glyph=cc_glyph2d_ref('M',&spec,0);
  int size[2],offset[2];
  const unsigned char *data=cc_glyph2d_getbitmap(glyph,size,offset);
  check("invalid bitmap fallback",data && cc_glyph2d_getmono(glyph) && size[0]>0 && size[1]>0 && offset[0]==0);
  cc_glyph2d_unref(glyph); cc_fontspec_clean(&spec);
  SoDB::finish();
  std::printf("Font backend: %d cases, %d failures\n",cases,failures);
  return failures?1:0;
}
