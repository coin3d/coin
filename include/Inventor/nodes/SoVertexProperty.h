#ifndef COIN_SOVERTEXPROPERTY_H
#define COIN_SOVERTEXPROPERTY_H

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

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoMFUInt32.h>
#include <Inventor/fields/SoMFVec3f.h>
#include <Inventor/fields/SoMFVec2f.h>
#include <Inventor/fields/SoSFEnum.h>

class COIN_DLL_API SoVertexProperty : public SoNode {
  typedef SoNode inherited;

  SO_NODE_HEADER(SoVertexProperty);

public:
  static void initClass(void);
  SoVertexProperty(void);

  // These must match 100% with the enum in SoMaterialBindingElement.
  enum Binding {
    OVERALL = 2,
    PER_PART,
    PER_PART_INDEXED,
    PER_FACE,
    PER_FACE_INDEXED,
    PER_VERTEX,
    PER_VERTEX_INDEXED
  };

  SoMFVec3f vertex;
  SoMFVec2f texCoord;
  SoMFVec3f normal;
  SoSFEnum normalBinding;
  SoMFUInt32 orderedRGBA;
  SoSFEnum materialBinding;

  virtual void doAction(SoAction * action);
  virtual void GLRender(SoGLRenderAction * action);
  virtual void getBoundingBox(SoGetBoundingBoxAction * action);
  virtual void callback(SoCallbackAction * action);
  virtual void pick(SoPickAction * action);
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);

protected:
  virtual ~SoVertexProperty();
  virtual void notify(SoNotList *list);

private:
  SbBool transparent;
  SbBool checktransparent;
};

#endif // !COIN_SOVERTEXPROPERTY_H
