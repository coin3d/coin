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
