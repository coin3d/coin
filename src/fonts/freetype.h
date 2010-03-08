#ifndef COIN_FLWFREETYPE_H
#define COIN_FLWFREETYPE_H

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
  Here is the glue between the FLW and the FreeType font lib.

  See http://www.freetype.org for more information about
  the FreeType font library.
*/

/* ********************************************************************** */

#ifndef COIN_INTERNAL
#error this is a private header file
#endif /* ! COIN_INTERNAL */

/* ********************************************************************** */

#include <Inventor/C/basic.h>
#include <Inventor/C/base/string.h>

#ifdef __cplusplus
extern "C" {
#endif

  SbBool cc_flwft_initialize(void);
  void cc_flwft_exit(void);

  void * cc_flwft_get_font(const char * fontname, const unsigned int pixelsize);
  void cc_flwft_get_font_name(void * font, cc_string * str);
  void cc_flwft_done_font(void * font);

  int cc_flwft_get_num_charmaps(void * font);
  const char * cc_flwft_get_charmap_name(void * font, int charmap);
  void cc_flwft_set_charmap(void * font, int charmap);

  void cc_flwft_set_char_size(void * font, int height);
  void cc_flwft_set_font_rotation(void * font, float angle);

  int cc_flwft_get_glyph(void * font, unsigned int charidx);
  void cc_flwft_get_vector_advance(void * font, int glyph, float * x, float * y);
  void cc_flwft_get_bitmap_kerning(void * font, int glyph1, int glyph2, int * x, int * y);
  void cc_flwft_get_vector_kerning(void * font, int glyph1, int glyph2, float * x, float * y);
  void cc_flwft_done_glyph(void * font, int glyph);

  struct cc_font_bitmap * cc_flwft_get_bitmap(void * font, unsigned int glyph);
  struct cc_font_vector_glyph * cc_flwft_get_vector_glyph(void * font, unsigned int glyph, float complexity);

#ifdef __cplusplus
}
#endif

#endif /* !COIN_FLWFREETYPE_H */
