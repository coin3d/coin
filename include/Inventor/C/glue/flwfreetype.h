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

#ifdef __cplusplus
extern "C" {
#endif
  
  /*
    Here is the glue between the FLW and the FreeType font lib.

    See http://www.freetype.org for more information about
    the FreeType font library.
  */
  
  int flwftInitialize(void);
  void flwftExit(void);

  FLWfont flwftGetFont(const char * fontname);
  int flwftGetFontName(FLWfont font, char * buffer, int bufsize);
  int flwftGetFontStyle(FLWfont font, char * buffer, int bufsize);
  void flwftDoneFont(FLWfont font);

  int flwftGetNumCharmaps(FLWfont font);
  int flwftGetCharmapName(FLWfont font, int charmap, char * buffer, int bufsize);
  int flwftSetCharmap(FLWfont font, int charmap);

  int flwftSetCharSize(FLWfont font, int width, int height);
  int flwftSetFontRotation(FLWfont font, float angle);

  FLWglyph flwftGetGlyph(FLWfont font, unsigned int charidx);
  int flwftGetAdvance(FLWfont font, FLWglyph glyph, float *x, float *y);
  int flwftGetKerning(FLWfont font, FLWglyph glyph1, FLWglyph glyph2, float *x, float *y);
  void flwftDoneGlyph(FLWfont font, FLWglyph glyph);

  FLWbitmap * flwftGetBitmap(FLWfont font, FLWglyph glyph);
  int flwftGetOutline(FLWfont font, FLWglyph glyph);

#ifdef __cplusplus
}
#endif

#endif /* !COIN_FLWFREETYPE_H */
