/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

#ifndef COIN_SOGLYPH_H
#define COIN_SOGLYPH_H

#include <Inventor/SbBasic.h>

class SbVec2f;
class SbName;

class COIN_DLL_EXPORT SoGlyph {
public:

  static const SoGlyph *getGlyph(const char character, const SbName &font);
  void unref() const;

  const SbVec2f *getCoords(void) const;
  const int *getFaceIndices(void) const;
  const int *getEdgeIndices(void) const;
  float getWidth(void) const;

protected:
  SoGlyph();
  ~SoGlyph();

  void setCoords(SbVec2f *coords, int numcoords = -1);
  void setFaceIndices(int *indices, int numindices = -1);
  void setEdgeIndices(int *indices, int numindices = -1);

private:

  static SoGlyph *createSystemGlyph(const char character, const SbName &font);
  static void unrefGlyph(SoGlyph *glyph);

  SbVec2f *coords;
  int *faceidx;
  int *edgeidx;
  int refcount;
  float width;

  struct {
    unsigned int didalloccoords : 1;
    unsigned int didallocfaceidx : 1;
    unsigned int didallocedgeidx : 1;
  } flags;
};


#endif // COIN_SOGLYPH_H
