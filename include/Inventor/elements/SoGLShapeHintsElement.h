/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to aquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#ifndef COIN_SOGLSHAPEHINTSELEMENT_H
#define COIN_SOGLSHAPEHINTSELEMENT_H

#include <Inventor/elements/SoShapeHintsElement.h>


class COIN_DLL_EXPORT SoGLShapeHintsElement : public SoShapeHintsElement {
  typedef SoShapeHintsElement inherited;

  SO_ELEMENT_HEADER(SoGLShapeHintsElement);
public:
  static void initClass(void);
protected:
  virtual ~SoGLShapeHintsElement();

public:
  virtual void init(SoState * state);

  virtual void push(SoState * state);
  virtual void pop(SoState * state,
                   const SoElement * prevTopElement);

  void evaluate() const;

  void forceSend(const SbBool twoside) const;
  void forceSend(const SbBool ccw, const SbBool cull) const;
  void forceSend(const SbBool ccw, const SbBool cull,
                 const SbBool twoside) const;


protected:
  virtual void setElt(VertexOrdering vertexOrdering,
                      ShapeType shapeType, FaceType faceType);
private:
  unsigned int glflags;
  void updategl(const unsigned int flags);

};

#endif // !COIN_SOGLSHAPEHINTSELEMENT_H
