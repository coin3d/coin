#ifndef COIN_FLW_H
#define COIN_FLW_H

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
  
#include <Inventor/C/tidbits.h>
#include <Inventor/C/base/string.h>
  
  /*
    FLW is a Font Library Wrapper designed to allow any number of
    underlying font libraries to be used through the same
    API. Functions and datatypes are modelled on the FreeType font
    library.

    Which underlying font library to use is currently determined at
    compile time.

    See http://www.freetype.org for more information about the
    FreeType font library.
  */
  
  
  struct cc_FLWbitmap {
    int bearingX; /* left side of bitmap relative to pen */
    int bearingY; /* top of bitmap relative to pen */
    unsigned int rows; /* height of bitmap */
    unsigned int width; /* width of bitmap */
    unsigned int pitch; /* number of bytes occupied by each row (rows are padded to nearest byte) */
    int advanceX; /* where to position pen for next glyph */
    int advanceY;
    unsigned char * buffer; /* bitmap data */
  };
  
  void cc_flw_initialize(void);
  void cc_flw_exit(void);
  
  int cc_flw_create_font(const char * fontname, const int sizex, const int sizey);
  int cc_flw_get_font(const char * fontname, const int sizex, const int ysizey);
  void cc_flw_done_font(int font);

  int cc_flw_get_num_charmaps(int font);
  const char * cc_flw_get_charmap_name(int font, int charmap);
  int cc_flw_set_charmap(int font, int charmap);

  int cc_flw_set_char_size(int font, int width, int height);
  const char * cc_flw_get_font_name(int font);
  int cc_flw_set_font_rotation(int font, float angle);

  int cc_flw_get_glyph(int font, int charidx);
  int cc_flw_get_advance(int font, int glyph, float *x, float *y);
  int cc_flw_get_kerning(int font, int glyph1, int glyph2, float *x, float *y);
  void cc_flw_done_glyph(int font, int glyph);

  struct cc_FLWbitmap * cc_flw_get_bitmap(int font, int glyph);
  void cc_flw_done_bitmap(struct cc_FLWbitmap * bitmap);
  
  int cc_flw_get_outline(int font, int glyph);
  
  SbBool cc_flw_debug(void);       

#ifdef __cplusplus
}
#endif

#endif /* !COIN_FLW_H */
