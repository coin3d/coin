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

#ifndef COIN_SOTEXT3_H
#define COIN_SOTEXT3_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoShape.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFBitMask.h>
#include <Inventor/lists/SbList.h>

class SoSensor;
class SoFieldSensor;
class SoGlyph;

class SoText3 : public SoShape {
  typedef SoShape inherited;

  SO_NODE_HEADER(SoText3);

public:
  static void initClass(void);
  SoText3(void);

  enum Part {
    FRONT = 1,
    SIDES = 2,
    BACK =  4,
    ALL = FRONT|BACK|SIDES
  };

  enum Justification {
    LEFT = 1,
    RIGHT,
    CENTER
  };

  SoMFString string;
  SoSFFloat spacing;
  SoSFEnum justification;
  SoSFBitMask parts;

  SbBox3f getCharacterBounds(SoState * state, int stringindex, int charindex);

  virtual void GLRender(SoGLRenderAction * action);
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);

protected:
  virtual ~SoText3();

  virtual void generatePrimitives(SoAction *);
  virtual void computeBBox(SoAction * action, SbBox3f & box, SbVec3f & center);
  virtual SbBool willSetShapeHints(void) const;
  virtual SbBool willUpdateNormalizeElement(SoState *) const;
  virtual SoDetail * createTriangleDetail(SoRayPickAction * action,
                                         const SoPrimitiveVertex * v1,
                                         const SoPrimitiveVertex * v2,
                                         const SoPrimitiveVertex * v3,
                                         SoPickedPoint * pp);

private:
  SbList <const SoGlyph *> glyphs;
  SbList <float> widths;
  void setUpGlyphs(SoState * state);
  SbBool needsetup;

  SoFieldSensor * stringsensor;
  static void fieldSensorCB(void * d, SoSensor * s);

  void render(SoState * state, unsigned int part);
  void generate(SoAction * action, unsigned int part);
};

#endif // !COIN_SOTEXT3_H
