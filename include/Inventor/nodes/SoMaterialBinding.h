/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

#ifndef COIN_SOMATERIALBINDING_H
#define COIN_SOMATERIALBINDING_H

#include <Inventor/elements/SoMaterialBindingElement.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/nodes/SoSubNode.h>


class COIN_DLL_API SoMaterialBinding : public SoNode {
  typedef SoNode inherited;

  SO_NODE_HEADER(SoMaterialBinding);

public:
  static void initClass(void);
  SoMaterialBinding(void);

  enum Binding {
    OVERALL = SoMaterialBindingElement::OVERALL,
    PER_PART = SoMaterialBindingElement::PER_PART,
    PER_PART_INDEXED = SoMaterialBindingElement::PER_PART_INDEXED,
    PER_FACE = SoMaterialBindingElement::PER_FACE,
    PER_FACE_INDEXED = SoMaterialBindingElement::PER_FACE_INDEXED,
    PER_VERTEX = SoMaterialBindingElement::PER_VERTEX,
    PER_VERTEX_INDEXED = SoMaterialBindingElement::PER_VERTEX_INDEXED,

    // Note: DEFAULT and NONE bindings are obsolete, but we include them
    // to be compatible with old Open Inventor code.
    DEFAULT = OVERALL,
    NONE = OVERALL
  };

  SoSFEnum value;

  virtual void doAction(SoAction * action);
  virtual void GLRender(SoGLRenderAction * action);
  virtual void callback(SoCallbackAction * action);
  virtual void pick(SoPickAction * action);
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);

protected:
  virtual ~SoMaterialBinding();
};

#endif // !COIN_SOMATERIALBINDING_H
