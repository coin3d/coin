#ifndef COIN_FLW_H
#define COIN_FLW_H

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

/*
  FLW is a Font Library Wrapper abstraction designed to allow any
  number of underlying font libraries to be used through the same API.

  Functions and datatypes are loosely modeled on the FreeType font
  library.

  Which underlying font library to use is currently determined at
  compile time.
*/

/* ********************************************************************** */

#ifndef COIN_INTERNAL
#error this is a private header file
#endif /* ! COIN_INTERNAL */

/* ********************************************************************** */

#include <Inventor/C/tidbits.h>
#include <Inventor/C/base/string.h>
#include <Inventor/C/base/list.h>
#include "fontspec.h"

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

  void cc_flw_ref_font(int fontid);
  void cc_flw_unref_font(int fontid);

  int cc_flw_get_font_id(const char * fontname, const unsigned int sizey,
                         const float angle, const float complexity);

  unsigned int cc_flw_get_glyph(int font, unsigned int charidx);
  void cc_flw_done_glyph(int font, unsigned int glyphidx);

  struct cc_font_bitmap * cc_flw_get_bitmap(int font, unsigned int glyph);
  struct cc_font_vector_glyph * cc_flw_get_vector_glyph(int font, unsigned int glyph);

  const float * cc_flw_get_vector_glyph_coords(struct cc_font_vector_glyph * vecglyph);
  const int * cc_flw_get_vector_glyph_faceidx(struct cc_font_vector_glyph * vecglyph);
  const int * cc_flw_get_vector_glyph_edgeidx(struct cc_font_vector_glyph * vecglyph);

  void cc_flw_get_bitmap_advance(int font, unsigned int glyph, int * x, int * y);
  void cc_flw_get_bitmap_kerning(int font, unsigned int glyph1, unsigned int glyph2, int * x, int * y);

  void cc_flw_get_vector_advance(int font, unsigned int glyph, float * x, float * y);
  void cc_flw_get_vector_kerning(int font, unsigned int glyph1, unsigned int glyph2, float * x, float * y);

#ifdef __cplusplus
}
#endif

#endif /* !COIN_FLW_H */
