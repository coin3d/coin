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

#ifdef __cplusplus
extern "C" {
#endif
  
  /*
    Here is the glue between the FLW and the FreeType font lib.

    See http://www.freetype.org for more information about
    the FreeType font library.
  */
  
  SbBool cc_flwftInitialize(void);
  void cc_flwftExit(void);

  FLWfont cc_flwftGetFont(const char * fontname);
  int cc_flwftGetFontName(FLWfont font, char * buffer, int bufsize);
  int cc_flwftGetFontStyle(FLWfont font, char * buffer, int bufsize);
  void cc_flwftDoneFont(FLWfont font);

  int cc_flwftGetNumCharmaps(FLWfont font);
  int cc_flwftGetCharmapName(FLWfont font, int charmap, char * buffer, int bufsize);
  int cc_flwftSetCharmap(FLWfont font, int charmap);

  int cc_flwftSetCharSize(FLWfont font, int width, int height);
  int cc_flwftSetFontRotation(FLWfont font, float angle);

  FLWglyph cc_flwftGetGlyph(FLWfont font, unsigned int charidx);
  int cc_flwftGetAdvance(FLWfont font, FLWglyph glyph, float *x, float *y);
  int cc_flwftGetKerning(FLWfont font, FLWglyph glyph1, FLWglyph glyph2, float *x, float *y);
  void cc_flwftDoneGlyph(FLWfont font, FLWglyph glyph);

  FLWbitmap * cc_flwftGetBitmap(FLWfont font, FLWglyph glyph);
  int cc_flwftGetOutline(FLWfont font, FLWglyph glyph);

#ifdef __cplusplus
}
#endif

#endif /* !COIN_FLWFREETYPE_H */
