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
  \class SoVRMLCylinder SoVRMLCylinder.h Inventor/VRMLnodes/SoVRMLCylinder.h
  \brief The SoVRMLCylinder class is used to represent a cylinder object.
*/

/*!
  \var SoSFFloat SoVRMLCylinder::radius
  The cylinder radius. Default value is 1.0.
*/

/*!
  \var SoSFFloat SoVRMLCylinder::height
  The cylinder height. Default value is 2.0.
*/

/*!
  \var SoSFBool SoVRMLCylinder::side
  Enable/disable the cylinder side wall. Default value is TRUE.
*/

/*!
  \var SoSFBool SoVRMLCylinder::top
  Enable/disable the cylinder top. Default value is TRUE.
*/

/*!
  \var SoSFBool SoVRMLCylinder::bottom
  Enable/disable the cylinder bottom. Default value is TRUE.
*/


#include <Inventor/VRMLnodes/SoVRMLCylinder.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/bundles/SoMaterialBundle.h>
#include <Inventor/elements/SoComplexityTypeElement.h>
#include <Inventor/elements/SoLightModelElement.h>
#include <Inventor/elements/SoGLTextureEnabledElement.h>
#include <Inventor/elements/SoTextureCoordinateElement.h>
#include <Inventor/misc/SoGL.h>
#include <Inventor/misc/SoGenerate.h>
#include <Inventor/misc/SoPick.h>
#include <Inventor/misc/SoState.h>
#include <math.h>

#define CYL_SIDE_NUMTRIS 40.0f

SO_NODE_SOURCE(SoVRMLCylinder);

// Doc in parent
void
SoVRMLCylinder::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoVRMLCylinder, SO_VRML97_NODE_TYPE);
}

/*!
  Constructor.
*/
SoVRMLCylinder::SoVRMLCylinder(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoVRMLCylinder);

  SO_VRMLNODE_ADD_FIELD(radius, (1.0f));
  SO_VRMLNODE_ADD_FIELD(height, (2.0f));
  SO_VRMLNODE_ADD_FIELD(side, (TRUE));
  SO_VRMLNODE_ADD_FIELD(top, (TRUE));
  SO_VRMLNODE_ADD_FIELD(bottom, (TRUE));
}

/*!
  Destructor.
*/
SoVRMLCylinder::~SoVRMLCylinder()
{
}

// Doc in parent
void
SoVRMLCylinder::GLRender(SoGLRenderAction * action)
{
  if (!shouldGLRender(action)) return;

  SoState * state = action->getState();

  SoMaterialBundle mb(action);
  mb.sendFirst();

  unsigned int flags = 0;
  if (SoLightModelElement::get(state) != SoLightModelElement::BASE_COLOR)
    flags |= SOGL_NEED_NORMALS;
  if (SoGLTextureEnabledElement::get(state) &&
      SoTextureCoordinateElement::getType(state) != SoTextureCoordinateElement::TEXGEN)
    flags |= SOGL_NEED_TEXCOORDS;
  if (this->side.getValue()) flags |= SOGL_RENDER_SIDE;
  if (this->top.getValue()) flags |= SOGL_RENDER_TOP;
  if (this->bottom.getValue()) flags |= SOGL_RENDER_BOTTOM;

  float complexity = this->getComplexityValue(action);

  sogl_render_cylinder(this->radius.getValue(),
                       this->height.getValue(),
                       (int)(CYL_SIDE_NUMTRIS * complexity),
                       &mb,
                       flags);
}

// Doc in parent
void
SoVRMLCylinder::rayPick(SoRayPickAction * action)
{
  if (!shouldRayPick(action)) return;

  unsigned int flags = 0;
  if (this->side.getValue()) flags |= SOPICK_SIDES;
  if (this->top.getValue()) flags |= SOPICK_TOP;
  if (this->bottom.getValue()) flags |= SOPICK_BOTTOM;

  sopick_pick_cylinder(this->radius.getValue(),
                       this->height.getValue(),
                       flags,
                       this, action);
}

// Doc in parent
void
SoVRMLCylinder::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  if (!this->shouldPrimitiveCount(action)) return;

  float complexity = this->getComplexityValue(action);
  int numtris = (int)(complexity * CYL_SIDE_NUMTRIS);

  if (this->bottom.getValue()) {
    action->addNumTriangles(numtris-2);
  }
  if (this->top.getValue()) {
    action->addNumTriangles(numtris-2);
  }
  if (this->side.getValue()) {
    action->addNumTriangles(numtris * 2);
  }
}

// Doc in parent
void
SoVRMLCylinder::generatePrimitives(SoAction * action)
{
  unsigned int flags = 0;
  if (this->side.getValue()) flags |= SOGEN_GENERATE_SIDE;
  if (this->bottom.getValue()) flags |= SOGEN_GENERATE_BOTTOM;
  if (this->top.getValue()) flags |= SOGEN_GENERATE_TOP;

  float complexity = this->getComplexityValue(action);

  sogen_generate_cylinder(this->radius.getValue(),
                          this->height.getValue(),
                          (int)(CYL_SIDE_NUMTRIS * complexity),
                          flags,
                          this,
                          action);
}

// Doc in parent
void
SoVRMLCylinder::computeBBox(SoAction * action,
                            SbBox3f & box,
                            SbVec3f & center)
{
  float r = this->radius.getValue();
  float h = this->height.getValue();

  // Allow negative values.
  if (r < 0.0f) r = -r;
  if (h < 0.0f) h = -h;

  // Either the SIDES are present, or we've at least got both the TOP
  // and BOTTOM caps -- so just find the middle point and enclose
  // everything.
  if (this->side.getValue() ||
      (this->bottom.getValue() &&
       this->top.getValue())) {
    center.setValue(0.0f, 0.0f, 0.0f);
    box.setBounds(SbVec3f(-r, -h/2.0f, -r), SbVec3f(r, h/2.0f, r));
  }
  // ..not a "full" cylinder, but we've got the BOTTOM cap.
  else if (this->bottom.getValue()) {
    center.setValue(0.0f, -h/2.0f, 0.0f);
    box.setBounds(SbVec3f(-r, -h/2.0f, -r), SbVec3f(r, -h/2.0f, r));
  }
  // ..not a "full" cylinder, but we've got the TOP cap.
  else if (this->top.getValue()) {
    center.setValue(0.0f, h/2.0f, 0.0f);
    box.setBounds(SbVec3f(-r, h/2.0f, -r), SbVec3f(r, h/2.0f, r));
  }
  // ..no parts present. My confidence is shot -- I feel very small.
  else {
    center.setValue(0.0f, 0.0f, 0.0f);
    box.setBounds(SbVec3f(0.0f, 0.0f, 0.0f), SbVec3f(0.0f, 0.0f, 0.0f));
  }
}
