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

// Note: the class documentation for the basic primitive shapes
// SoSphere, SoCylinder, SoCone and SoCube have many common, or at
// least close to common, paragraphs. If you make any changes, check
// those other shapes too, to see if your updates / fixes should be
// migrated. <mortene@sim.no>.
/*!
  \class SoCylinder SoCylinder.h Inventor/nodes/SoCylinder.h
  \brief The SoCylinder class is for rendering cylinder shapes.
  \ingroup nodes

  Insert a cylinder shape into the scenegraph. The cylinder is
  rendered with the current material, texture and drawstyle settings
  (if any, otherwise the default settings are used).

  The SoCylinder node class is provided as a convenient abstraction
  for the application programmer to use "complex" shapes of this type
  without having to do the tessellation to polygons and other low-level
  programming herself.

  A cylinder is visualized by the underlying rendering system by first
  tessellating the conceptual cylinder into a set of polygons. To
  control the trade-off between an as much as possible correct visual
  appearance of the cylinder versus fast rendering, use an
  SoComplexity node to influence the number of polygons generated from
  the tessellation process. (The higher the complexity value, the more
  polygons will be generated, the more \e rounded the sides of the
  cylinder will look.) Set the SoComplexity::value field to what you
  believe would be a good trade-off between correctness and speed for
  your particular application.

  \sa SoCone, SoSphere, SoCube
*/

#include <Inventor/nodes/SoCylinder.h>
#include <Inventor/nodes/SoSubNodeP.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#include <Inventor/SbCylinder.h>
#include <Inventor/SbPlane.h>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/bundles/SoMaterialBundle.h>
#include <Inventor/details/SoCylinderDetail.h>
#include <Inventor/elements/SoComplexityTypeElement.h>
#include <Inventor/elements/SoMaterialBindingElement.h>
#include <Inventor/elements/SoGLTextureEnabledElement.h>
#include <Inventor/elements/SoGLTexture3EnabledElement.h>
#include <Inventor/elements/SoTextureCoordinateElement.h>
#include <Inventor/misc/SoGL.h>
#include <Inventor/misc/SoGenerate.h>
#include <Inventor/misc/SoPick.h>
#include <Inventor/misc/SoState.h>
#include <math.h>

#define CYL_SIDE_NUMTRIS 40.0f

/*!
  \enum SoCylinder::Part
  The parts of a cylinder shape.
*/


/*!
  \var SoSFFloat SoCylinder::radius
  Radius of cylinder. Default value 1.0.
*/
/*!
  \var SoSFFloat SoCylinder::height
  Height of cylinder. Default is 2.0.
*/
/*!
  \var SoSFBitMask SoCylinder::parts
  Which parts to use for rendering, picking, etc.  Defaults to
  SoCylinder::ALL.
*/


// *************************************************************************

SO_NODE_SOURCE(SoCylinder);

/*!
  Constructor.
*/
SoCylinder::SoCylinder(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoCylinder);

  SO_NODE_ADD_FIELD(radius, (1.0f));
  SO_NODE_ADD_FIELD(height, (2.0f));
  SO_NODE_ADD_FIELD(parts, (SoCylinder::ALL));

  SO_NODE_DEFINE_ENUM_VALUE(Part, SIDES);
  SO_NODE_DEFINE_ENUM_VALUE(Part, TOP);
  SO_NODE_DEFINE_ENUM_VALUE(Part, BOTTOM);
  SO_NODE_DEFINE_ENUM_VALUE(Part, ALL);
  SO_NODE_SET_SF_ENUM_TYPE(parts, Part);
}

/*!
  Destructor.
*/
SoCylinder::~SoCylinder()
{
}

// Doc in parent.
void
SoCylinder::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoCylinder, SO_FROM_INVENTOR_1|SoNode::VRML1);
}

// Doc in parent.
void
SoCylinder::computeBBox(SoAction * action, SbBox3f & box, SbVec3f & center)
{

  float r = this->radius.getValue();
  float h = this->height.getValue();

  // Allow negative values.
  if (r < 0.0f) r = -r;
  if (h < 0.0f) h = -h;

  // Either the SIDES are present, or we've at least got both the TOP
  // and BOTTOM caps -- so just find the middle point and enclose
  // everything.
  if (this->parts.getValue() & SoCylinder::SIDES ||
      (this->parts.getValue() & SoCylinder::BOTTOM &&
       this->parts.getValue() & SoCylinder::TOP)) {
    center.setValue(0.0f, 0.0f, 0.0f);
    box.setBounds(SbVec3f(-r, -h/2.0f, -r), SbVec3f(r, h/2.0f, r));
  }
  // ..not a "full" cylinder, but we've got the BOTTOM cap.
  else if (this->parts.getValue() & SoCylinder::BOTTOM) {
    center.setValue(0.0f, -h/2.0f, 0.0f);
    box.setBounds(SbVec3f(-r, -h/2.0f, -r), SbVec3f(r, -h/2.0f, r));
  }
  // ..not a "full" cylinder, but we've got the TOP cap.
  else if (this->parts.getValue() & SoCylinder::TOP) {
    center.setValue(0.0f, h/2.0f, 0.0f);
    box.setBounds(SbVec3f(-r, h/2.0f, -r), SbVec3f(r, h/2.0f, r));
  }
  // ..no parts present. My confidence is shot -- I feel very small.
  else {
    center.setValue(0.0f, 0.0f, 0.0f);
    box.setBounds(SbVec3f(0.0f, 0.0f, 0.0f), SbVec3f(0.0f, 0.0f, 0.0f));
  }
}

// Doc in parent.
void
SoCylinder::GLRender(SoGLRenderAction * action)
{
  if (!shouldGLRender(action)) return;

  SoState * state = action->getState();

  SoCylinder::Part p = (SoCylinder::Part) this->parts.getValue();
  SoMaterialBundle mb(action);
  mb.sendFirst();

  unsigned int flags = 0;
  if (!mb.isColorOnly())
    flags |= SOGL_NEED_NORMALS;
  if (SoGLTextureEnabledElement::get(state))
    flags |= SOGL_NEED_TEXCOORDS;
  else if (SoGLTexture3EnabledElement::get(state))
    flags |= SOGL_NEED_3DTEXCOORDS;
  
  if (p & SIDES) flags |= SOGL_RENDER_SIDE;
  if (p & TOP) flags |= SOGL_RENDER_TOP;
  if (p & BOTTOM) flags |= SOGL_RENDER_BOTTOM;

  SoMaterialBindingElement::Binding bind =
    SoMaterialBindingElement::get(state);
  if (bind == SoMaterialBindingElement::PER_PART ||
      bind == SoMaterialBindingElement::PER_PART_INDEXED)
    flags |= SOGL_MATERIAL_PER_PART;

  float complexity = this->getComplexityValue(action);

  sogl_render_cylinder(this->radius.getValue(),
                       this->height.getValue(),
                       (int)(CYL_SIDE_NUMTRIS * complexity),
                       &mb,
                       flags);
}

/*!
  Add a \a part to the cylinder.

  \sa removePart(), hasPart()
*/
void
SoCylinder::addPart(SoCylinder::Part part)
{
  if (this->hasPart(part)) {
#if COIN_DEBUG
    SoDebugError::postWarning("SoCylinder::addPart", "part already set");
#endif // COIN_DEBUG
    return;
  }

  this->parts.setValue(this->parts.getValue() | part);
}

/*!
  Remove a \a part from the cylinder.

  \sa addPart(), hasPart()
*/
void
SoCylinder::removePart(SoCylinder::Part part)
{
  if (!this->hasPart(part)) {
#if COIN_DEBUG
    SoDebugError::postWarning("SoCylinder::removePart", "part was not set");
#endif // COIN_DEBUG
    return;
  }

  this->parts.setValue(this->parts.getValue() & ~part);
}

/*!
  Returns \c TRUE if rendering of the given \a part is currently
  turned on.

  \sa addPart(), removePart()
*/
SbBool
SoCylinder::hasPart(SoCylinder::Part part) const
{
  return (this->parts.getValue() & part) ? TRUE : FALSE;
}

// Doc in parent.
void
SoCylinder::rayPick(SoRayPickAction * action)
{
  if (!shouldRayPick(action)) return;


  unsigned int flags = 0;
  SoCylinder::Part p = (SoCylinder::Part) this->parts.getValue();
  if (p & SIDES) flags |= SOPICK_SIDES;
  if (p & TOP) flags |= SOPICK_TOP;
  if (p & BOTTOM) flags |= SOPICK_BOTTOM;

  SoMaterialBindingElement::Binding bind =
    SoMaterialBindingElement::get(action->getState());
  if (bind == SoMaterialBindingElement::PER_PART ||
      bind == SoMaterialBindingElement::PER_PART_INDEXED)
    flags |= SOPICK_MATERIAL_PER_PART;
  
  sopick_pick_cylinder(this->radius.getValue(),
                       this->height.getValue(),
                       flags,
                       this, action);
}

// Doc in parent.
void
SoCylinder::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  if (!this->shouldPrimitiveCount(action)) return;

  float complexity = this->getComplexityValue(action);
  int numtris = (int)(complexity * CYL_SIDE_NUMTRIS);

  if (this->parts.getValue() & SoCylinder::BOTTOM) {
    action->addNumTriangles(numtris-2);
  }
  if (this->parts.getValue() & SoCylinder::TOP) {
    action->addNumTriangles(numtris-2);
  }
  if (this->parts.getValue() & SoCylinder::SIDES) {
    action->addNumTriangles(numtris * 2);
  }
}

// Doc in parent.
void
SoCylinder::generatePrimitives(SoAction * action)
{
  SoCylinder::Part p = (SoCylinder::Part) this->parts.getValue();
  unsigned int flags = 0;
  if (p & SoCylinder::SIDES) flags |= SOGEN_GENERATE_SIDE;
  if (p & SoCylinder::BOTTOM) flags |= SOGEN_GENERATE_BOTTOM;
  if (p & SoCylinder::TOP) flags |= SOGEN_GENERATE_TOP;

  SoMaterialBindingElement::Binding bind =
    SoMaterialBindingElement::get(action->getState());
  if (bind == SoMaterialBindingElement::PER_PART ||
      bind == SoMaterialBindingElement::PER_PART_INDEXED)
    flags |= SOGL_MATERIAL_PER_PART;

  float complexity = this->getComplexityValue(action);

  sogen_generate_cylinder(this->radius.getValue(),
                          this->height.getValue(),
                          (int)(CYL_SIDE_NUMTRIS * complexity),
                          flags,
                          this,
                          action);
}
