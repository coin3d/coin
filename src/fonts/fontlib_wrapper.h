#ifndef COIN_FLW_H
#define COIN_FLW_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2004 by Systems in Motion.  All rights reserved.
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
 *  See <URL:http://www.coin3d.org/> for more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif
  
#include <Inventor/C/tidbits.h>
#include <Inventor/C/base/string.h>
#include <Inventor/C/base/list.h>
  
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
  
  
  typedef struct cc_flw_bitmap {
    int bearingX; /* left side of bitmap relative to pen */
    int bearingY; /* top of bitmap relative to pen */
    unsigned int rows; /* height of bitmap */
    unsigned int width; /* width of bitmap */
    unsigned int pitch; /* number of bytes occupied by each row (rows are padded to nearest byte) */
    int advanceX; /* where to position pen for next glyph */
    int advanceY;
    unsigned char * buffer; /* bitmap data */
  } cc_flw_bitmap;
  
  typedef struct cc_flw_vector_glyph {
    float * vertices;
    int * faceindices;
    int * edgeindices;
  } cc_flw_vector_glyph;


  int cc_flw_get_font(const char * fontname, const unsigned int sizex, const unsigned int sizey, const float angle);
  int cc_flw_find_font(const char * fontname, const unsigned int sizex, const unsigned int ysizey, const float angle);
  void cc_flw_done_font(unsigned int font);
  const char * cc_flw_get_font_name(unsigned int font);

  unsigned int cc_flw_get_glyph(unsigned int font, unsigned int charidx);
  void cc_flw_get_bitmap_advance(unsigned int font, unsigned int glyph, int * x, int * y);
  void cc_flw_get_vector_advance(unsigned int font, unsigned int glyph, float * x, float * y);
  void cc_flw_get_bitmap_kerning(unsigned int font, unsigned int glyph1, unsigned int glyph2, int * x, int * y);
  void cc_flw_get_vector_kerning(unsigned int font, unsigned int glyph1, unsigned int glyph2, float * x, float * y);
  void cc_flw_done_glyph(unsigned int font, unsigned int glyph);

  struct cc_flw_bitmap * cc_flw_get_bitmap(unsigned int font, unsigned int glyph);
  struct cc_flw_vector_glyph * cc_flw_get_vector_glyph(unsigned int font, unsigned int glyph, float complexity);

  const float * cc_flw_get_vector_glyph_coords(struct cc_flw_vector_glyph * vecglyph);
  const int * cc_flw_get_vector_glyph_faceidx(struct cc_flw_vector_glyph * vecglyph);
  const int * cc_flw_get_vector_glyph_edgeidx(struct cc_flw_vector_glyph * vecglyph);

  SbBool cc_flw_debug(void);       

#ifdef __cplusplus
}
#endif

#endif /* !COIN_FLW_H */
