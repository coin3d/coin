#ifndef COIN_SOTEXTURECOORDINATEBINDING_H
#define COIN_SOTEXTURECOORDINATEBINDING_H

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

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/elements/SoTextureCoordinateBindingElement.h>

class COIN_DLL_API SoTextureCoordinateBinding : public SoNode {
  typedef SoNode inherited;

  SO_NODE_HEADER(SoTextureCoordinateBinding);

public:
  static void initClass(void);
  SoTextureCoordinateBinding(void);

  enum Binding {
    PER_VERTEX = SoTextureCoordinateBindingElement::PER_VERTEX,
    PER_VERTEX_INDEXED = SoTextureCoordinateBindingElement::PER_VERTEX_INDEXED,

    // Note: this value is marked as obsolete in SGI Open Inventor
    // 2.1, but we need it for compatibility with old 3rd party Open
    // Inventor code.
    DEFAULT = PER_VERTEX_INDEXED
  };

  SoSFEnum value;

  virtual void doAction(SoAction * action);
  virtual void callback(SoCallbackAction * action);
  virtual void GLRender(SoGLRenderAction * action);
  virtual void pick(SoPickAction * action);

protected:
  virtual ~SoTextureCoordinateBinding();

  virtual SbBool readInstance(SoInput * in, unsigned short flags);
};

#endif // !COIN_SOTEXTURECOORDINATEBINDING_H
