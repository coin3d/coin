/* FIXME: just a mock-up implementation yet. handegar is working on
   it. 20030901 mortene. */

#ifndef COIN_CC_GLYPH3D_H
#define COIN_CC_GLYPH3D_H

#ifdef __cplusplus
extern "C" {
#endif

#include "fontspec.h"
  
  typedef struct cc_glyph3d cc_glyph3d;
  
  cc_glyph3d * cc_glyph3d_getglyph(uint32_t character, 
                                   const cc_font_specification * spec);
  
  float * cc_glyph3d_getcoords(const cc_glyph3d * g);
  int * cc_glyph3d_getfaceindices(const cc_glyph3d * g);
  int * cc_glyph3d_getedgeindices(const cc_glyph3d * g);
  int * cc_glyph3d_getnextcwedge(const cc_glyph3d * g, int edgeidx);
  int * cc_glyph3d_getnextccwedge(const cc_glyph3d * g, int edgeidx);
  float cc_glyph3d_getwidth(const cc_glyph3d * g);
  float * cc_glyph3d_getboundingbox(const cc_glyph3d * g);

  void cc_glyph3d_getadvance(const cc_glyph3d * g, float * x, float * y);
  void cc_glyph3d_getkerning(const cc_glyph3d * left, const cc_glyph3d * right, 
                             float * x, float * y);

#ifdef __cplusplus
}
#endif
#endif /* !COIN_CC_GLYPH3D_H */
