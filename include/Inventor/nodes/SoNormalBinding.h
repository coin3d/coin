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

#ifndef __SONORMALBINDING_H__
#define __SONORMALBINDING_H__

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/nodes/SoNode.h>


class SoNormalBinding : public SoNode {
  typedef SoNode inherited;

  SO_NODE_HEADER(SoNormalBinding);

public:
  static void initClass(void);
  SoNormalBinding(void);

  // Definitions must match 100% with the enum in
  // SoNormalBindingElement. DEFAULT and NONE are obsolete, but
  // included for compatibility with old code using Open Inventor.
  enum Binding {
    OVERALL = 2,
    PER_PART,
    PER_PART_INDEXED,
    PER_FACE,
    PER_FACE_INDEXED,
    PER_VERTEX,
    PER_VERTEX_INDEXED,
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

#endif // !__SONORMALBINDING_H__
