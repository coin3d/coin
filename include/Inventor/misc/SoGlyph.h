#ifndef COIN_SOGLYPH_H
#define COIN_SOGLYPH_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#include <Inventor/SbBasic.h>
#include <Inventor/SbBox2f.h>

class SbVec2f;
class SbName;

class COIN_DLL_API SoGlyph {
public:

  static const SoGlyph *getGlyph(const char character, const SbName &font);
  void unref() const;

  const SbVec2f * getCoords(void) const;
  const int * getFaceIndices(void) const;
  const int * getEdgeIndices(void) const;
  const int * getNextCWEdge(const int edgeidx) const;
  const int * getNextCCWEdge(const int edgeidx) const;

  float getWidth(void) const;
  const SbBox2f & getBoundingBox(void) const;

protected:
  SoGlyph();
  ~SoGlyph();

  void setCoords(SbVec2f * coords, int numcoords = -1);
  void setFaceIndices(int * indices, int numindices = -1);
  void setEdgeIndices(int * indices, int numindices = -1);

private:

  static SoGlyph *createSystemGlyph(const char character, const SbName & font);
  static void unrefGlyph(SoGlyph * glyph);

  SbVec2f * coords;
  SbBox2f bbox;
  int * faceidx;
  int * edgeidx;
  int refcount;
  float ymin, ymax;

  struct {
    unsigned int didalloccoords : 1;
    unsigned int didallocfaceidx : 1;
    unsigned int didallocedgeidx : 1;
    unsigned int didcalcbbox : 1;
  } flags;
};

#endif // !COIN_SOGLYPH_H
