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

#ifndef COIN_SONORMALBINDING_H
#define COIN_SONORMALBINDING_H

#include <Inventor/elements/SoNormalBindingElement.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/nodes/SoSubNode.h>


class SoNormalBinding : public SoNode {
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
