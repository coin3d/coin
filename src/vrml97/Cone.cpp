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
  \class SoVRMLCone SoVRMLCone.h Inventor/VRMLnodes/SoVRMLCone.h
  \brief The SoVRMLCone class is used to represent a Cone object.
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
  SO_NODE_INTERNAL_CONSTRUCTOR(SoVRMLCone);

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

  unsigned int flags = SOGL_NEED_NORMALS | SOGL_NEED_TEXCOORDS;
  if (this->side.getValue()) flags |= SOGL_RENDER_SIDE;
  if (this->bottom.getValue()) flags |= SOGL_RENDER_BOTTOM;

  SoMaterialBundle mb(action);
  mb.sendFirst();

  float complexity = this->getComplexityValue(action);

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

#undef CONE_SIDE_NUMTRIS 40.0f
