#include <Inventor/SoDB.h>
#include "fonts/fontlib_wrapper.h"
#include "fonts/common.h"
#include <cstdio>
#include <cstring>
#include <vector>
#include <cmath>
int main(){
 SoDB::init();int failures=0;
 int reference=cc_flw_get_font_id_and_ref("DejaVu Sans",12,0,.4f);
 int mixed=cc_flw_get_font_id_and_ref("DejaVu Sans",12,0,.6f);
 unsigned rb=cc_flw_get_glyph(reference,'B'),mb=cc_flw_get_glyph(mixed,'B'),ma=cc_flw_get_glyph(mixed,'A');
 auto *before=cc_flw_get_bitmap(reference,rb);
 auto *vector=cc_flw_get_vector_glyph(mixed,ma);
 if(!before || !vector){std::puts("native vector backend unavailable");return 77;}
 int width=before->width,rows=before->rows,pitch=before->pitch,advance=before->advanceX;
 std::vector<unsigned char> pixels(before->buffer,before->buffer+size_t(rows)*pitch);
 float ax,ay,bx,by;cc_flw_get_vector_advance(mixed,ma,&ax,&ay);
 auto *after=cc_flw_get_bitmap(mixed,mb);
 if(!after || after->width!=width || after->rows!=rows || after->pitch!=pitch || after->advanceX!=advance || std::memcmp(after->buffer,pixels.data(),pixels.size())){
  ++failures;std::fprintf(stderr,"FAIL bitmap after vector: expected %dx%d advance%d, got %dx%d advance%d\n",width,rows,advance,after?after->width:0,after?after->rows:0,after?after->advanceX:0);
 }
 cc_flw_get_vector_advance(mixed,ma,&bx,&by);
 if(std::fabs(ax-bx)>1e-6 || std::fabs(ay-by)>1e-6){++failures;std::fprintf(stderr,"FAIL vector advance after bitmap %f -> %f\n",ax,bx);}
 cc_flw_done_glyph(reference,rb);cc_flw_done_glyph(mixed,mb);cc_flw_done_glyph(mixed,ma);
 cc_flw_unref_font(reference);cc_flw_unref_font(mixed);SoDB::finish();
 std::printf("Mixed font sizes: 2 checks, %d failures\n",failures);return failures?1:0;
}
