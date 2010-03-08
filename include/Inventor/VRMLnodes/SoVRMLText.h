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

#ifndef COIN_SOVRMLTEXT_H
#define COIN_SOVRMLTEXT_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/VRMLnodes/SoVRMLGeometry.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/fields/SoSFNode.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoMFFloat.h>

class SoVRMLTextP;

class COIN_DLL_API SoVRMLText : public SoVRMLGeometry
{
  typedef SoVRMLGeometry inherited;
  SO_NODE_HEADER(SoVRMLText);

public:
  static void initClass(void);
  SoVRMLText(void);

  SoMFString string;
  SoSFNode fontStyle;
  SoSFFloat maxExtent;
  SoMFFloat length;

  enum Justification {
    BEGIN   = 0x01,
    END     = 0x02,
    MIDDLE  = 0x03
  };

  virtual void GLRender(SoGLRenderAction * action);
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);

  virtual void notify(SoNotList * list);
  virtual SoChildList * getChildren(void) const;

protected:
  virtual ~SoVRMLText();

  virtual void computeBBox(SoAction * action,
                            SbBox3f & box, SbVec3f & center);
  virtual void generatePrimitives(SoAction * action);
  SoChildList * children;

private:
  SoVRMLTextP * pimpl;
  friend class SoVRMLTextP;

};

#endif // ! COIN_SOVRMLTEXT_H
