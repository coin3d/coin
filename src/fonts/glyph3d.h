#ifndef COIN_CC_GLYPH3D_H
#define COIN_CC_GLYPH3D_H

#ifdef __cplusplus
extern "C" {
#endif

#include "fontspec.h"
  
  typedef struct cc_glyph3d cc_glyph3d;
  
  cc_glyph3d * cc_glyph3d_getglyph(uint32_t character, 
                                   const cc_font_specification * spec);
  
  const float * cc_glyph3d_getcoords(const cc_glyph3d * g);
  const int * cc_glyph3d_getfaceindices(const cc_glyph3d * g);
  const int * cc_glyph3d_getedgeindices(const cc_glyph3d * g);
  const int * cc_glyph3d_getnextcwedge(const cc_glyph3d * g, int edgeidx);
  const int * cc_glyph3d_getnextccwedge(const cc_glyph3d * g, int edgeidx);

  float cc_glyph3d_getwidth(const cc_glyph3d * g);
  /* FIXME: the interface of the getboundingbox() function should be
     fixed, either by passing in the float array, or by making a
     datatype abstraction for a bbox. 20030916 mortene. */
  const float * cc_glyph3d_getboundingbox(const cc_glyph3d * g);

  void cc_glyph3d_getadvance(const cc_glyph3d * g, float * x, float * y);
  void cc_glyph3d_getkerning(const cc_glyph3d * left, const cc_glyph3d * right, 
                             float * x, float * y);

#ifdef __cplusplus
}
#endif
#endif /* !COIN_CC_GLYPH3D_H */
