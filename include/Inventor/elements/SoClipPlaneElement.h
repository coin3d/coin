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

#ifndef COIN_SOCLIPPLANEELEMENT_H
#define COIN_SOCLIPPLANEELEMENT_H

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

class COIN_DLL_EXPORT SoClipPlaneElement : public SoAccumulatedElement {
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
