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
  \ingroup VRMLnodes

  \WEB3DCOPYRIGHT

  \verbatim
  Cylinder {
    field    SFBool    bottom  TRUE
    field    SFFloat   height  2         # (0,)
    field    SFFloat   radius  1         # (0,)
    field    SFBool    side    TRUE
    field    SFBool    top     TRUE
  }
  \endverbatim

  The Cylinder node specifies a capped cylinder centred at (0,0,0) in
  the local coordinate system and with a central axis oriented along
  the local Y-axis. By default, the cylinder is sized at "-1" to "+1"
  in all three dimensions. The radius field specifies the radius of
  the cylinder and the height field specifies the height of the
  cylinder along the central axis. Both radius and height shall be
  greater than zero. Figure 6.4 illustrates the Cylinder node.

  The cylinder has three parts: the side, the top (Y = +height/2) and
  the bottom (Y = -height/2).  Each part has an associated SFBool
  field that indicates whether the part exists (TRUE) or does not
  exist (FALSE). Parts which do not exist are not rendered and not
  eligible for intersection tests (e.g., collision detection or sensor
  activation).

  <center>
  <img src="http://www.web3d.org/technicalinfo/specifications/vrml97/Images/cylinder.gif">
  Figure 6.4
  </center>


  When a texture is applied to a cylinder, it is applied differently
  to the sides, top, and bottom. On the sides, the texture wraps
  counterclockwise (from above) starting at the back of the
  cylinder. The texture has a vertical seam at the back, intersecting
  the X=0 plane. For the top and bottom caps, a circle is cut out of
  the unit texture squares centred at (0, +/- height/2, 0) with
  dimensions 2 × radius by 2 × radius.  The top texture appears right
  side up when the top of the cylinder is tilted toward the +Z-axis,
  and the bottom texture appears right side up when the top of the
  cylinder is tilted toward the -Z-axis. SoVRMLTextureTransform
  affects the texture coordinates of the Cylinder node.  The Cylinder
  node's geometry requires outside faces only. When viewed from the
  inside the results are undefined.

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
#include <Inventor/elements/SoGLTextureEnabledElement.h>
#include <Inventor/elements/SoGLTexture3EnabledElement.h>
#include <Inventor/elements/SoTextureCoordinateElement.h>
#include <Inventor/misc/SoGL.h>
#include <Inventor/misc/SoGenerate.h>
#include <Inventor/misc/SoPick.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/elements/SoGLShapeHintsElement.h>
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
  
  if (!mb.isColorOnly())
    flags |= SOGL_NEED_NORMALS;
  if ((SoGLTextureEnabledElement::get(state) || SoGLTexture3EnabledElement::get(state)) &&
      SoTextureCoordinateElement::getType(state) != SoTextureCoordinateElement::TEXGEN)
    flags |= SOGL_NEED_TEXCOORDS;
  if (this->side.getValue()) flags |= SOGL_RENDER_SIDE;
  if (this->top.getValue()) flags |= SOGL_RENDER_TOP;
  if (this->bottom.getValue()) flags |= SOGL_RENDER_BOTTOM;

  float complexity = this->getComplexityValue(action);

  // enable back face culling
  SoGLShapeHintsElement::forceSend(state, TRUE, TRUE);

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
