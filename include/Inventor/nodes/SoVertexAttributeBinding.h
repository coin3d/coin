#ifndef COIN_VERTEXATTRIBUTEBINDING_H
#define COIN_VERTEXATTRIBUTEBINDING_H

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

#include <Inventor/elements/SoVertexAttributeBindingElement.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/nodes/SoSubNode.h>

class COIN_DLL_API SoVertexAttributeBinding : public SoNode {
  typedef SoNode inherited;

  SO_NODE_HEADER(SoVertexAttributeBinding);

public:
  static void initClass(void);
  SoVertexAttributeBinding(void);

  enum Binding {
    OVERALL = SoVertexAttributeBindingElement::OVERALL,
    PER_VERTEX = SoVertexAttributeBindingElement::PER_VERTEX,
    PER_VERTEX_INDEXED = SoVertexAttributeBindingElement::PER_VERTEX_INDEXED,
    DEFAULT = PER_VERTEX_INDEXED
  };

  SoSFEnum value;

  virtual void doAction(SoAction * action);
  virtual void GLRender(SoGLRenderAction * action);
  virtual void callback(SoCallbackAction * action);
  virtual void pick(SoPickAction * action);
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);

protected:
  virtual ~SoVertexAttributeBinding();
};

#endif // !COIN_VERTEXATTRIBUTEBINDING_H
