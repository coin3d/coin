/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 2001 by Systems in Motion. All rights reserved.
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

/*!
  \class SoVRMLBox SoVRMLBox.h Inventor/VRMLnodes/SoVRMLBox.h
  \brief The SoVRMLBox class is used for representing a 3D box.
*/

/*!
  \var SoSFVec3f size
  
  Box size vector. Default value is (2,2,2).
*/

#include <Inventor/VRMLnodes/SoVRMLBox.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/nodes/SoSubNodeP.h>

#include <Inventor/elements/SoGLNormalizeElement.h>
#include <Inventor/elements/SoGLTextureEnabledElement.h>
#include <Inventor/elements/SoLightModelElement.h>

#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/bundles/SoMaterialBundle.h>
#include <Inventor/misc/SoGL.h>
#include <Inventor/misc/SoGenerate.h>
#include <Inventor/misc/SoPick.h>
#include <Inventor/misc/SoState.h>

SO_NODE_SOURCE(SoVRMLBox);

// Doc in parent
void
SoVRMLBox::initClass(void) // static
{
  SO_NODE_INTERNAL_INIT_CLASS(SoVRMLBox, SO_VRML97_NODE_TYPE);
}

/*!
  Constructor.
*/
SoVRMLBox::SoVRMLBox(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoVRMLBox);

  SO_VRMLNODE_ADD_FIELD(size, (2.0f, 2.0f, 2.0f));
}

/*!
  Destructor.
*/
SoVRMLBox::~SoVRMLBox()
{
}

// Doc in parent
void
SoVRMLBox::GLRender(SoGLRenderAction * action)
{
 if (!this->shouldGLRender(action)) return;
  SoState * state = action->getState();

  SbBool doTextures = SoGLTextureEnabledElement::get(state);

  SbBool sendNormals =
    (SoLightModelElement::get(state) !=
     SoLightModelElement::BASE_COLOR);

  SoMaterialBundle mb(action);
  mb.sendFirst();

  if (sendNormals) {
    SoGLNormalizeElement::forceSend(state, TRUE);
  }

  unsigned int flags = 0;
  if (doTextures) flags |= SOGL_NEED_TEXCOORDS;
  if (sendNormals) flags |= SOGL_NEED_NORMALS;

  SbVec3f s = this->size.getValue();

  sogl_render_cube(s[0],
                   s[1],
                   s[2],
                   &mb,
                   flags);
}

// Doc in parent
void
SoVRMLBox::rayPick(SoRayPickAction * action)
{
  if (!shouldRayPick(action)) return;

  SbVec3f s = this->size.getValue();
  sopick_pick_cube(s[0],
                   s[1],
                   s[2],
                   0,
                   this, action);
}

// Doc in parent
void
SoVRMLBox::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  if (!this->shouldPrimitiveCount(action)) return;
  action->addNumTriangles(12);
}

// Doc in parent
void
SoVRMLBox::generatePrimitives(SoAction * action)
{
  SbVec3f s = this->size.getValue();
  sogen_generate_cube(s[0],
                      s[1],
                      s[2],
                      0,
                      this,
                      action);
}

// Doc in parent
void
SoVRMLBox::computeBBox(SoAction * action,
                       SbBox3f & box,
                       SbVec3f & center)
{
  center.setValue(0.0f, 0.0f, 0.0f);
  SbVec3f s = this->size.getValue();
  float w = s[0] * 0.5f;
  float h = s[1] * 0.5f;
  float d = s[2] * 0.5f;

  // Allow negative values.
  if (w < 0.0f) w = -w;
  if (h < 0.0f) h = -h;
  if (d < 0.0f) d = -d;

  box.setBounds(-w, -h, -d, w, h, d);
}
