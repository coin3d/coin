#ifndef COIN_CC_GLYPH2D_H
#define COIN_CC_GLYPH2D_H

#ifdef __cplusplus
extern "C" {
#endif

#include "fontspec.h"
 
  typedef struct cc_glyph2d cc_glyph2d;

  cc_glyph2d * cc_glyph2d_getglyph(uint32_t character, const cc_font_specification * spec, float angle);
  
  void cc_glyph2d_getadvance(const cc_glyph2d * g, int * x, int * y);
  void cc_glyph2d_getkerning(const cc_glyph2d * left, const cc_glyph2d * right, int * x, int * y);
  
  unsigned int cc_glyph2d_getwidth(const cc_glyph2d * g);
  const unsigned char * cc_glyph2d_getbitmap(const cc_glyph2d * g, int * size, int * offset);
  
   
#ifdef __cplusplus
}
#endif

#endif /* !COIN_CC_GLYPH2D_H */
