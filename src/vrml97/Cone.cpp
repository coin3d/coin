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

/*!
  \class SoVRMLCone SoVRMLCone.h Inventor/VRMLnodes/SoVRMLCone.h
  \brief The SoVRMLCone class is used to represent a Cone object.
  \ingroup VRMLnodes

  \WEB3DCOPYRIGHT

  \verbatim
  Cone {
    field     SFFloat   bottomRadius 1        # (0, inf)
    field     SFFloat   height       2        # (0, inf)
    field     SFBool    side         TRUE
    field     SFBool    bottom       TRUE
  }
  \endverbatim

  The Cone node specifies a cone which is centred in the local
  coordinate system and whose central axis is aligned with the local
  Y-axis. The bottomRadius field specifies the radius of the cone's
  base, and the height field specifies the height of the cone from the
  centre of the base to the apex.  By default, the cone has a radius
  of 1.0 at the bottom and a height of 2.0, with its apex at y =
  height/2 and its bottom at y = -height/2.  Both bottomRadius and
  height shall be greater than zero. Figure 6.3 illustrates the Cone
  node.

  <center>
  <img src="http://www.web3d.org/technicalinfo/specifications/vrml97/Images/cone.gif">
  Figure 6.3
  </center>

  The side field specifies whether sides of the cone are created and
  the bottom field specifies whether the bottom cap of the cone is
  created. A value of TRUE specifies that this part of the cone
  exists, while a value of FALSE specifies that this part does not
  exist (not rendered or eligible for collision or sensor intersection
  tests).  When a texture is applied to the sides of the cone, the
  texture wraps counterclockwise (from above) starting at the back of
  the cone. The texture has a vertical seam at the back in the X=0
  plane, from the apex (0, height/2, 0) to the point (0, -height/2, -
  bottomRadius). For the bottom cap, a circle is cut out of the
  texture square centred at (0, -height/2, 0) with dimensions (2 ×
  bottomRadius) by (2 × bottomRadius).  The bottom cap texture appears
  right side up when the top of the cone is rotated towards the
  -Z-axis. SoVRMLTextureTransform affects the texture coordinates of
  the Cone.

  The Cone geometry requires outside faces only. When viewed from the
  inside the results are undefined.

*/

/*!
  \var SoSFFloat SoVRMLCone::bottomRadius
  The cone bottom radius. Default value is 1.0.
*/

/*!
  \var SoSFFloat SoVRMLCone::height
  The cone height. Default value is 2.0.
*/

/*!
  \var SoSFBool SoVRMLCone::side
  Enable/disable the cone side wall. Default value is TRUE.
*/

/*!
  \var SoSFBool SoVRMLCone::bottom
  Enable/disable the cone bottom. Default value is TRUE.
*/

#include <Inventor/VRMLnodes/SoVRMLCone.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>

#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/bundles/SoMaterialBundle.h>
#include <Inventor/details/SoConeDetail.h>
#include <Inventor/misc/SoGL.h>
#include <Inventor/misc/SoGenerate.h>
#include <Inventor/misc/SoPick.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/elements/SoGLShapeHintsElement.h>
#include <Inventor/elements/SoGLTextureEnabledElement.h>
#include <Inventor/elements/SoGLTexture3EnabledElement.h>

SO_NODE_SOURCE(SoVRMLCone);

#define CONE_SIDE_NUMTRIS 40.0f

// Doc in parent
void
SoVRMLCone::initClass(void) // static
{
  SO_NODE_INTERNAL_INIT_CLASS(SoVRMLCone, SO_VRML97_NODE_TYPE);
}

/*!
  Constructor.
*/
SoVRMLCone::SoVRMLCone(void)
{
  SO_VRMLNODE_INTERNAL_CONSTRUCTOR(SoVRMLCone);

  SO_VRMLNODE_ADD_FIELD(bottomRadius, (1.0f));
  SO_VRMLNODE_ADD_FIELD(height, (2.0f));

  SO_VRMLNODE_ADD_FIELD(side, (TRUE));
  SO_VRMLNODE_ADD_FIELD(bottom, (TRUE));
}

/*!
  Destructor.
*/
SoVRMLCone::~SoVRMLCone()
{
}

// Doc in parent
void
SoVRMLCone::GLRender(SoGLRenderAction * action)
{
  if (!shouldGLRender(action)) return;

  SoState * state = action->getState();
  
  SbBool doTextures = SoGLTextureEnabledElement::get(state) ||
    SoGLTexture3EnabledElement::get(state);

  SoMaterialBundle mb(action);
  mb.sendFirst();

  SbBool sendNormals = !mb.isColorOnly();

  unsigned int flags = 0;
  if (doTextures) flags |= SOGL_NEED_TEXCOORDS;
  if (sendNormals) flags |= SOGL_NEED_NORMALS;

  if (this->side.getValue()) flags |= SOGL_RENDER_SIDE;
  if (this->bottom.getValue()) flags |= SOGL_RENDER_BOTTOM;

  float complexity = this->getComplexityValue(action);

  // enable back face culling
  SoGLShapeHintsElement::forceSend(state, TRUE, TRUE);

  sogl_render_cone(this->bottomRadius.getValue(),
                   this->height.getValue(),
                   (int)(CONE_SIDE_NUMTRIS * complexity),
                   &mb,
                   flags);
}

// Doc in parent
void
SoVRMLCone::rayPick(SoRayPickAction * action)
{
  if (!this->shouldRayPick(action)) return;

  unsigned int flags = 0;
  if (this->side.getValue()) flags |= SOPICK_SIDES;
  if (this->bottom.getValue()) flags |= SOPICK_BOTTOM;

  sopick_pick_cone(this->bottomRadius.getValue(),
                   this->height.getValue(),
                   flags, this, action);
}

// Doc in parent
void
SoVRMLCone::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  if (!this->shouldPrimitiveCount(action)) return;

  float complexity = this->getComplexityValue(action);
  int numtris = (int)(complexity * CONE_SIDE_NUMTRIS);

  if (this->bottom.getValue()) {
    action->addNumTriangles(numtris-2);
  }
  if (this->side.getValue()) {
    action->addNumTriangles(numtris);
  }
}

// Doc in parent
void
SoVRMLCone::generatePrimitives(SoAction * action)
{
  unsigned int flags = 0;
  if (this->side.getValue()) flags |= SOGEN_GENERATE_SIDE;
  if (this->bottom.getValue()) flags |= SOGEN_GENERATE_BOTTOM;

  float complexity = this->getComplexityValue(action);

  sogen_generate_cone(this->bottomRadius.getValue(),
                      this->height.getValue(),
                      (int)(CONE_SIDE_NUMTRIS * complexity),
                      flags,
                      this,
                      action);
}

// Doc in parent
void
SoVRMLCone::computeBBox(SoAction * action,
                        SbBox3f & box,
                        SbVec3f & center)
{
  float r = this->bottomRadius.getValue();
  float h = this->height.getValue();

  // Allow negative values.
  if (h < 0.0f) h = -h;
  if (r < 0.0f) r = -r;

  float half_height = h * 0.5f;

  // The SIDES are present, so just find the middle point and enclose
  // everything.
  if (this->side.getValue()) {
    center.setValue(0.0f, 0.0f, 0.0f);
    box.setBounds(SbVec3f(-r, -half_height, -r), SbVec3f(r, half_height, r));
  }
  // ..no SIDES, but we've still got the bottom (NB: OIV misses this case).
  else if (this->bottom.getValue()) {
    center.setValue(0.0f, -half_height, 0.0f);
    box.setBounds(SbVec3f(-r, -half_height, -r), SbVec3f(r, -half_height, r));
  }
  // ..no parts present. My confidence is shot -- I feel very small.
  else {
    center.setValue(0.0f, 0.0f, 0.0f);
    box.setBounds(SbVec3f(0.0f, 0.0f, 0.0f), SbVec3f(0.0f, 0.0f, 0.0f));
  }
}

#undef CONE_SIDE_NUMTRIS
