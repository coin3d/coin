/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#ifndef COIN_SOGLYPH_H
#define COIN_SOGLYPH_H

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


#endif // COIN_SOGLYPH_H
