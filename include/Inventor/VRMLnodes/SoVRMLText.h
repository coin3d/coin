/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 2001 by Systems in Motion. All rights reserved.
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

#ifndef COIN_SOVRMLTEXT_H
#define COIN_SOVRMLTEXT_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/VRMLnodes/SoVRMLGeometry.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/fields/SoSFNode.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoMFFloat.h>

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

  virtual void GLRender(SoAction * action);
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);

  virtual void notify(SoNotList * list);
  virtual SoChildList * getChildren(void) const;

protected:
  virtual ~SoVRMLText();

  virtual void computeBBox(SoAction * action,
                            SbBox3f & box, SbVec3f & center);
  virtual void generatePrimitives(SoAction * action);
  SoChildList * children;

}; // class SoVRMLText

#endif // ! COIN_SOVRMLTEXT_H
