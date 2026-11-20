#include <Inventor/SoDB.h>
#include "fonts/fontlib_wrapper.h"
#include "fonts/freetype.h"
#include "glue/freetype.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
static bool bitmapfail,vectorfail;
static int bitmapcalls,vectorcalls;
extern "C" cc_font_bitmap *cc_flwft_get_bitmap(void *,unsigned int) {
 ++bitmapcalls;if(bitmapfail)return NULL;
 auto *b=static_cast<cc_font_bitmap *>(std::calloc(1,sizeof(cc_font_bitmap)));
 b->rows=2;b->width=2;b->pitch=2;b->advanceX=2;b->advanceY=0;
 b->buffer=static_cast<unsigned char *>(std::malloc(4));std::memset(b->buffer,42,4);return b;
}
extern "C" cc_font_vector_glyph *cc_flwft_get_vector_glyph(void *,unsigned int,float) {
 ++vectorcalls;if(vectorfail)return NULL;
 auto *v=static_cast<cc_font_vector_glyph *>(std::calloc(1,sizeof(cc_font_vector_glyph)));
 v->vertices=static_cast<float *>(std::malloc(6*sizeof(float)));
 v->faceindices=static_cast<int *>(std::malloc(4*sizeof(int)));
 v->edgeindices=static_cast<int *>(std::malloc(4*sizeof(int)));
 const float points[]={0,0,1,0,0,1};const int indices[]={0,1,2,-1};
 std::memcpy(v->vertices,points,sizeof(points));std::memcpy(v->faceindices,indices,sizeof(indices));std::memcpy(v->edgeindices,indices,sizeof(indices));return v;
}
int main() {
 SoDB::init();if(!cc_ftglue_available()){SoDB::finish();return 77;}
 int failures=0;
 bitmapfail=vectorfail=false;bitmapcalls=0;
 int probe=cc_flw_get_font_id_and_ref("DejaVu Sans",23,0,0.5f);
 auto pg=cc_flw_get_glyph(probe,'A');cc_flw_get_bitmap(probe,pg);
 const bool native=bitmapcalls>0;
 cc_flw_done_glyph(probe,pg);cc_flw_unref_font(probe);
 if(!native){SoDB::finish();return 77;}
 for(int order=0;order<2;++order){
  int font=cc_flw_get_font_id_and_ref("DejaVu Sans",24+order,0,0.5f);
  unsigned int glyph=cc_flw_get_glyph(font,'A');
  bitmapfail=order==1;vectorfail=order==0;bitmapcalls=vectorcalls=0;
  if(order==0){
   if(cc_flw_get_vector_glyph(font,glyph))++failures;
   auto *b=cc_flw_get_bitmap(font,glyph);
   if(bitmapcalls!=1)++failures;
   if(!b||b->mono||b->buffer[0]!=42)++failures;
   if(cc_flw_get_vector_glyph(font,glyph)||vectorcalls!=1)++failures;
   int x,y;cc_flw_get_bitmap_advance(font,glyph,&x,&y);if(x!=2||y!=0)++failures;
  } else {
   auto *b=cc_flw_get_bitmap(font,glyph);if(!b||!b->mono)++failures;
   if(!cc_flw_get_vector_glyph(font,glyph)||vectorcalls!=1)++failures;
  }
  cc_flw_done_glyph(font,glyph);cc_flw_unref_font(font);
 }
 bitmapfail=vectorfail=false;
 int font=cc_flw_get_font_id_and_ref("DejaVu Sans",26,0,0.5f);
 auto a=cc_flw_get_glyph(font,'A'),b=cc_flw_get_glyph(font,'A');
 const auto *first=cc_flw_get_bitmap(font,a);cc_flw_done_glyph(font,a);
 const auto *second=cc_flw_get_bitmap(font,b);
 if(first!=second||!second||second->buffer[0]!=42)++failures;
 cc_flw_done_glyph(font,b);cc_flw_unref_font(font);
 SoDB::finish();std::printf("Font representations: 2 failure orders + shared glyph lifetime, %d failures\n",failures);return failures?1:0;
}
