#ifndef COIN_FLWFREETYPE_H
#define COIN_FLWFREETYPE_H

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

#include <Inventor/C/glue/fontlib_wrapper.h>
#include <Inventor/C/basic.h>
#include <Inventor/C/base/string.h>

#ifdef __cplusplus
extern "C" {
#endif
  
  /*
    Here is the glue between the FLW and the FreeType font lib.

    See http://www.freetype.org for more information about
    the FreeType font library.
  */
  
  SbBool cc_flwft_initialize(void);
  void cc_flwft_exit(void);

  void * cc_flwft_get_font(const char * fontname);
  cc_string * cc_flwft_get_font_name(void * font);
  cc_string * cc_flwft_get_font_style(void * font);
  void cc_flwft_done_font(void * font);

  int cc_flwft_get_num_charmaps(void * font);
  const char * cc_flwft_get_charmap_name(void * font, int charmap);
  int cc_flwft_set_charmap(void * font, int charmap);

  int cc_flwft_set_char_size(void * font, int width, int height);
  int cc_flwft_set_font_rotation(void * font, float angle);
  
  int cc_flwft_get_glyph(void * font, unsigned int charidx);
  int cc_flwft_get_advance(void * font, int glyph, float * x, float * y);
  int cc_flwft_get_kerning(void * font, int glyph1, int glyph2, float * x, float * y);
  void cc_flwft_done_glyph(void * font, int glyph);
  
  struct cc_FLWbitmap * cc_flwft_get_bitmap(void * font, int glyph);
  int cc_flwft_get_outline(void * font, int glyph);
  
#ifdef __cplusplus
}
#endif

#endif /* !COIN_FLWFREETYPE_H */
