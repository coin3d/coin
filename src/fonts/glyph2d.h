#ifndef COIN_CC_GLYPH2D_H
#define COIN_CC_GLYPH2D_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

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
