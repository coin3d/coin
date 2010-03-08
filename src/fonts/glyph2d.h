#ifndef COIN_CC_GLYPH2D_H
#define COIN_CC_GLYPH2D_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#ifndef COIN_INTERNAL
#error this is a private header file
#endif /* ! COIN_INTERNAL */

/* ********************************************************************** */

#include "fontspec.h"

#ifdef __cplusplus
extern "C" {
#endif

  typedef struct cc_glyph2d cc_glyph2d;

  cc_glyph2d * cc_glyph2d_ref(uint32_t character, const cc_font_specification * spec, float angle);
  void cc_glyph2d_unref(cc_glyph2d * glyph);

  void cc_glyph2d_getadvance(const cc_glyph2d * g, int * x, int * y);
  void cc_glyph2d_getkerning(const cc_glyph2d * left, const cc_glyph2d * right, int * x, int * y);

  unsigned int cc_glyph2d_getwidth(const cc_glyph2d * g);
  const unsigned char * cc_glyph2d_getbitmap(const cc_glyph2d * g, int * size, int * offset);
  SbBool cc_glyph2d_getmono(const cc_glyph2d * g);

#ifdef __cplusplus
}
#endif

#endif /* !COIN_CC_GLYPH2D_H */
