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

#ifndef __SOIMAGE_H__
#define __SOIMAGE_H__

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoShape.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFImage.h>
#include <Inventor/fields/SoSFString.h>


class SoImage : public SoShape {
  typedef SoShape inherited;

  SO_NODE_HEADER(SoImage);

public:
  static void initClass(void);
  SoImage(void);

  enum VertAlignment {
    BOTTOM,
    HALF,
    TOP
  };

  enum HorAlignment {
    LEFT,
    CENTER,
    RIGHT
  };

  SoSFInt32 width;
  SoSFInt32 height;
  SoSFEnum vertAlignment;
  SoSFEnum horAlignment;
  SoSFImage image;
  SoSFString filename;

  virtual void GLRender(SoGLRenderAction * action);
  virtual void rayPick(SoRayPickAction * action);
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);

protected:
  virtual ~SoImage();

  virtual void generatePrimitives(SoAction * action);
  virtual void computeBBox(SoAction * action, SbBox3f & box, SbVec3f & center);

  virtual SbBool readInstance(SoInput * in, unsigned short flags);
  int getReadStatus(void);
  void setReadStatus(SbBool flag);

protected:
  SbVec2s getSize() const;
  static SbVec3f getNilpoint(SoState *state);
  class SoImageInterface *imageData;
  class SoImageInterface *orgImageData;
  SbBool getImage();
  void getQuad(SoState *state, SbVec3f &v0, SbVec3f &v1,
               SbVec3f &v2, SbVec3f &v3);
};

#endif // !__SOIMAGE_H__
