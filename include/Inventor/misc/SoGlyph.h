#ifndef COIN_SOGLYPH_H
#define COIN_SOGLYPH_H

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

#include <Inventor/SbBasic.h>
#include <Inventor/SbBox2f.h>
#include <Inventor/SbVec2s.h>
#include <Inventor/misc/SoState.h>

class SbName;
class SoGlyphP;

class COIN_DLL_API SoGlyph {
public:

  enum Fonttype {
    FONT2D = 1,
    FONT3D = 2
  };
  
  void unref(void) const;
  
  static const SoGlyph * getGlyph(const char character, const SbName & font);
  
  const SbVec2f * getCoords(void) const;
  const int * getFaceIndices(void) const;
  const int * getEdgeIndices(void) const;
  const int * getNextCWEdge(const int edgeidx) const;
  const int * getNextCCWEdge(const int edgeidx) const;

  float getWidth(void) const;
  const SbBox2f & getBoundingBox(void) const;
  
  static const SoGlyph * getGlyph(SoState * state,
                                  const unsigned int character, 
                                  const SbVec2s & size,
                                  const float angle);
  SbVec2s getAdvance(void) const;
  SbVec2s getKerning(const SoGlyph & rightglyph) const;
  unsigned char * getBitmap(SbVec2s & size, SbVec2s & pos, const SbBool antialiased) const;

protected:
  SoGlyph(void);
  ~SoGlyph();

#if (COIN_MAJOR_VERSION == 2)
#error Reminder: when copying this file over to Coin-2, next 3 functions
#error must be changed back to take a non-const first argument -- to keep
#error ABI-compatibility. Do not change any other interfaces because of this,
#error but handle by doing work-around casting internally in SoGlyph.cpp.
#endif
  void setCoords(const SbVec2f * coords, int numcoords = -1);
  void setFaceIndices(const int * indices, int numindices = -1);
  void setEdgeIndices(const int * indices, int numindices = -1);

private:
  static void unrefGlyph(SoGlyph * glyph);
  void setFontType(Fonttype type) const;


  friend class SoGlyphP;
  SoGlyphP * pimpl;
};

#endif // !COIN_SOGLYPH_H
