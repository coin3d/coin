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
  
  /*
    FLW is a Font Library Wrapper designed to allow any
    number of underlying font libraries to be used through
    the same API. Functions and datatypes are modelled on the
    FreeType font library, which is also the only library
    supported in this first version of FLW. 

    Which underlying font library to use is determined at
    compile time.

    See http://www.freetype.org for more information about
    the FreeType font library.
  */

  
  typedef void * FLWfont;
  typedef int FLWglyph;
  
  typedef struct FLWbitmap {
    int bearingX; /* left side of bitmap relative to pen */
    int bearingY; /* top of bitmap relative to pen */
    unsigned int rows; /* height of bitmap */
    unsigned int width; /* width of bitmap */
    unsigned int pitch; /* number of bytes occupied by each row (rows are padded to nearest byte) */
    int advanceX; /* where to position pen for next glyph */
    int advanceY;
    unsigned char * buffer; /* bitmap data */
  } FLWbitmap;

int flwInitialize(void);
void flwExit(void);

int flwCreateFont(const char * fontname, char * outname, const int outnamelen, const int sizex, const int sizey);
  int flwGetFont(const char * fontname, const int sizex, const int ysizey);
  void flwDoneFont(int font);

  int flwGetNumCharmaps(int font);
  int flwGetCharmapName(int font, int charmap, char * buffer, int bufsize);
  int flwSetCharmap(int font, int charmap);

  int flwSetCharSize(int font, int width, int height);
  int flwGetFontName(int font, char * buffer, int bufsize);
  int flwSetFontRotation(int font, float angle);

  int flwGetGlyph(int font, int charidx);
  int flwGetAdvance(int font, int glyph, float *x, float *y);
  int flwGetKerning(int font, int glyph1, int glyph2, float *x, float *y);
  void flwDoneGlyph(int font, int glyph);

  FLWbitmap * flwGetBitmap(int font, int glyph);
  void flwDoneBitmap(FLWbitmap * bitmap);

  int flwGetOutline(int font, int glyph);

#ifdef __cplusplus
}
#endif

#endif /* !COIN_FLW_H */
