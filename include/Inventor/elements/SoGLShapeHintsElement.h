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

#ifndef __SOGLSHAPEHINTSELEMENT_H__
#define __SOGLSHAPEHINTSELEMENT_H__

#include <Inventor/elements/SoShapeHintsElement.h>


class SoGLShapeHintsElement : public SoShapeHintsElement {
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

#endif // !__SOGLSHAPEHINTSELEMENT_H__
