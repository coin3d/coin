/**************************************************************************\
 *
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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

#ifndef __SOCLIPPLANEELEMENT_H__
#define __SOCLIPPLANEELEMENT_H__

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
};

class SoClipPlaneElement : public SoAccumulatedElement {
  typedef SoAccumulatedElement inherited;

  SO_ELEMENT_HEADER(SoClipPlaneElement);
public:
  static void initClass(void);
protected:
  virtual ~SoClipPlaneElement();

public:
  virtual void init(SoState * state);

  virtual void push(SoState * state);
  virtual void pop(SoState * state, const SoElement *prevTopElement);

  static  void add(SoState * const state, SoNode * const node,
                   const SbPlane & plane);
  static  const SoClipPlaneElement *getInstance(SoState * const state);
  int getNum(void) const;
  const SbPlane & get(const int index, const SbBool inWorldSpace = TRUE) const;

  virtual void print(FILE * file) const;

protected:
  int startIndex;
  SbList <so_plane_data> planes;

  virtual void addToElt(const SbPlane & plane, const SbMatrix & modelMatrix);
};

#endif // !__SOCLIPPLANEELEMENT_H__
