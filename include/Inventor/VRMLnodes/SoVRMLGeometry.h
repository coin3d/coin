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

#ifndef COIN_SOVRMLGEOMETRY_H
#define COIN_SOVRMLGEOMETRY_H

#include <Inventor/nodes/SoShape.h>
#include <Inventor/nodes/SoSubNode.h>

class SoChildList;

class COIN_DLL_API SoVRMLGeometry : public SoShape
{
  typedef SoShape inherited;
  SO_NODE_ABSTRACT_HEADER(SoVRMLGeometry);

public:
  static void initClass(void);

  virtual void search(SoSearchAction * action);
  virtual void copyContents(const SoFieldContainer * from, SbBool copyConn);

protected:
  SoVRMLGeometry(void);
  virtual ~SoVRMLGeometry();

  void setupShapeHints(SoState * state, const SbBool ccw, const SbBool solid);
  virtual SbBool shouldGLRender(SoGLRenderAction * action);
  virtual SoChildList * getChildren(void) const;
  virtual void notify(SoNotList * list);

  SoChildList * childlist;
  SbBool childlistvalid;
};

#endif // ! COIN_SOVRMLGEOMETRY_H
