#ifndef COIN_SOCLIPPLANEELEMENT_H
#define COIN_SOCLIPPLANEELEMENT_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#include <Inventor/elements/SoAccumulatedElement.h>
#include <Inventor/lists/SbList.h>
#include <Inventor/SbPlane.h>

class so_plane_data // internal
{
public:
  so_plane_data() {}
  so_plane_data(const SbPlane &plane, const SbMatrix &matrix);
public:
  SbPlane plane;
  SbPlane wcPlane;
  // Needed by AIX compiler
  SbBool operator==(const so_plane_data &spd) const {
    return spd.plane == plane && spd.wcPlane == wcPlane;
  }
  SbBool operator!=(const so_plane_data &spd) const {
    return spd.plane != plane || spd.wcPlane != wcPlane;
  }
};

class COIN_DLL_API SoClipPlaneElement : public SoAccumulatedElement {
  typedef SoAccumulatedElement inherited;

  SO_ELEMENT_HEADER(SoClipPlaneElement);
public:
  static void initClass(void);
protected:
  virtual ~SoClipPlaneElement();

public:
  virtual void init(SoState * state);
  virtual void push(SoState * state);

  static  void add(SoState * const state, SoNode * const node,
                   const SbPlane & plane);
  static  const SoClipPlaneElement * getInstance(SoState * const state);
  int getNum(void) const;
  const SbPlane & get(const int index, const SbBool inworldspace = TRUE) const;

protected:
  SbList <so_plane_data> planes;
  int startIndex;

  virtual void addToElt(const SbPlane & plane, const SbMatrix & modelmatrix);
};

#endif // !COIN_SOCLIPPLANEELEMENT_H
