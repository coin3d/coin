#ifndef COIN_SOFONTLIB_H
#define COIN_SOFONTLIB_H

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

#include <Inventor/SbName.h>
#include <Inventor/SbVec2s.h>

#include <Inventor/C/glue/fontlib_wrapper.h>

class SoFontLibP;

class SoFontLib {
public:
  static void initialize(void);
  static void exit(void);
  static SbString createFont(const SbName & fontname, const SbName & stylename, const SbVec2s & size);
  static int getFont(const SbName & fontname, const SbVec2s & size);
  static void doneFont(const int font);
  static int getNumCharmaps(const int font);
  static SbName getCharmapName(const int font, const int charmap);
  static void setCharmap(const int font, const int charmap);
  static void setCharSize(const int font, const SbVec2s & size);
  static void setFontRotation(const int font, const float angle);
  static int getGlyph(const int font, const int charidx);
  static SbVec2s getAdvance(const int font, const int glyph);
  static SbVec2s getKerning(const int font, const int leftglyph, const int rightglyph);
  static void doneGlyph(const int font, const int glyph);
  static unsigned char * getBitmap(const int font, const int glyph, SbVec2s & size, SbVec2s & pos, const SbBool antialiased);
  static int getOutline(const int font, const int glyph);

protected:
  SoFontLib(void);
  ~SoFontLib();

private:
  SoFontLibP * pimpl;
};

#endif // !COIN_SOFONTLIB_H
