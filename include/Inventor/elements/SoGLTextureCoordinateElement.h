/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
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

#ifndef COIN_SOGLTEXTURECOORDINATEELEMENT_H
#define COIN_SOGLTEXTURECOORDINATEELEMENT_H

#include <Inventor/elements/SoTextureCoordinateElement.h>

typedef void SoTexCoordTexgenCB(void * data);


class COIN_DLL_API SoGLTextureCoordinateElement : public SoTextureCoordinateElement {
  typedef SoTextureCoordinateElement inherited;

  SO_ELEMENT_HEADER(SoGLTextureCoordinateElement);
public:
  static void initClass(void);
protected:
  virtual ~SoGLTextureCoordinateElement();

public:
  virtual void init(SoState * state);
  virtual void push(SoState * state);
  virtual void pop(SoState * state,
                   const SoElement * prevTopElement);

  static  void setTexGen(SoState * const state, SoNode * const node,
                         SoTexCoordTexgenCB * const texgenFunc,
                         void * const texgenData = NULL,
                         SoTextureCoordinateFunctionCB * const func = NULL,
                         void * const funcData = NULL);

  virtual CoordType getType() const;

  static const SoGLTextureCoordinateElement *
  getInstance(SoState * const state);

  void send(const int index) const;
  void send(const int index, const SbVec3f &c, const SbVec3f &n) const;

protected:
  virtual void setElt(SoTexCoordTexgenCB *func,
                      void *data = NULL);

private:
  SoTexCoordTexgenCB *texgenCB;
  void *texgenData;

  void doCallback() const;
};

#endif // !COIN_SOGLTEXTURECOORDINATEELEMENT_H
