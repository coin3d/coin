#ifndef COIN_SONORMALBINDING_H
#define COIN_SONORMALBINDING_H

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

#include <Inventor/elements/SoNormalBindingElement.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/nodes/SoSubNode.h>

class COIN_DLL_API SoNormalBinding : public SoNode {
  typedef SoNode inherited;

  SO_NODE_HEADER(SoNormalBinding);

public:
  static void initClass(void);
  SoNormalBinding(void);

  enum Binding {
    OVERALL = SoNormalBindingElement::OVERALL,
    PER_PART = SoNormalBindingElement::PER_PART,
    PER_PART_INDEXED = SoNormalBindingElement::PER_PART_INDEXED,
    PER_FACE = SoNormalBindingElement::PER_FACE,
    PER_FACE_INDEXED = SoNormalBindingElement::PER_FACE_INDEXED,
    PER_VERTEX = SoNormalBindingElement::PER_VERTEX,
    PER_VERTEX_INDEXED = SoNormalBindingElement::PER_VERTEX_INDEXED,

    // DEFAULT and NONE are obsolete, but included for compatibility
    // with old Open Inventor applications.
    DEFAULT = PER_VERTEX_INDEXED,
    NONE = PER_VERTEX_INDEXED
  };

  SoSFEnum value;

  virtual void doAction(SoAction * action);
  virtual void GLRender(SoGLRenderAction * action);
  virtual void callback(SoCallbackAction * action);
  virtual void pick(SoPickAction * action);
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);

protected:
  virtual ~SoNormalBinding();

  virtual SbBool readInstance(SoInput * in, unsigned short flags);
};

#endif // !COIN_SONORMALBINDING_H
