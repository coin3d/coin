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

/*!
  \class SoCylinder SoCylinder.h Inventor/nodes/SoCylinder.h
  \brief The SoCylinder class is for rendering cylinder shapes.
  \ingroup nodes
*/

#include <Inventor/nodes/SoCylinder.h>
#include <Inventor/nodes/SoSubNodeP.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#include <Inventor/SbCylinder.h>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/bundles/SoMaterialBundle.h>
#include <Inventor/details/SoCylinderDetail.h>
#include <Inventor/elements/SoComplexityTypeElement.h>
#include <Inventor/elements/SoMaterialBindingElement.h>
#include <Inventor/misc/SoGL.h>
#include <Inventor/misc/SoGenerate.h>
#include <Inventor/misc/SoState.h>

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
  SO_NODE_INTERNAL_INIT_CLASS(SoCylinder);
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

  // FIXME: test if we really need normals and texcoords
  unsigned int flags = SOGL_NEED_NORMALS | SOGL_NEED_TEXCOORDS;
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

  action->setObjectSpace();
  const SbLine & line = action->getLine();
  float r = this->radius.getValue();
  float h = this->height.getValue() * 0.5f;

  // FIXME: possible to simplify cylinder test, since this cylinder
  // is aligned with the y-axis. 19991110 pederb.
  int numPicked = 0; // will never be > 2
  SbVec3f enter, exit;

  if (this->parts.getValue() & SoCylinder::SIDES) {
#if 0
    // The following line of code doesn't compile with GCC 2.95, as
    // reported by Petter Reinholdtsen (pere@hungry.com) on
    // coin-discuss.
    //
    // Update: it doesn't work with GCC 2.95.2 either, which is now
    // the current official release of GCC. And I can't find any
    // mention of a bug like this being fixed from the CVS ChangeLog,
    // neither in the gcc/egcs head branch nor the release-2.95
    // branch.  20000103 mortene.
    //
    // FIXME: should a) make sure this is known to the GCC
    // maintainers, b) have an autoconf check to test for this exact
    // bug. 19991230 mortene.
    SbCylinder cyl(SbLine(SbVec3f(0, 0, 0), SbVec3f(0, 1, 0)), r);
#else // GCC 2.95 work-around.
    SbVec3f v0(0.0f, 0.0f, 0.0f);
    SbVec3f v1(0.0f, 1.0f, 0.0f);
    SbLine l(v0, v1);
    SbCylinder cyl(l, r);
#endif // GCC 2.95 work-around.
    if (cyl.intersect(line, enter, exit)) {
      if ((fabs(enter[1]) <= h) && action->isBetweenPlanes(enter)) {
        SoPickedPoint * pp = action->addIntersection(enter);
        if (pp) {
          SoCylinderDetail * detail = new SoCylinderDetail();
          detail->setPart((int)SoCylinder::SIDES);
          pp->setDetail(detail, this);
          numPicked++;
        }
      }
      if ((fabs(exit[1]) <= h) && (enter != exit) && action->isBetweenPlanes(exit)) {
        SoPickedPoint * pp = action->addIntersection(exit);
        if (pp) {
          SoCylinderDetail * detail = new SoCylinderDetail();
          detail->setPart((int)SoCylinder::SIDES);
          pp->setDetail(detail, this);
          numPicked++;
        }
      }
    }
  }

  float r2 = r * r;

  if ((numPicked < 2) && (this->parts.getValue() & SoCylinder::TOP)) {
    SbPlane top(SbVec3f(0, 1, 0), h);
    if (top.intersect(line, enter)) {
      if (((enter[0] * enter[0] + enter[2] * enter[2]) <= r2) &&
          (action->isBetweenPlanes(enter))) {
        SoPickedPoint * pp = action->addIntersection(enter);
        if (pp) {
          SoCylinderDetail * detail = new SoCylinderDetail();
          detail->setPart((int)SoCylinder::TOP);
          pp->setDetail(detail, this);
          numPicked++;
        }
      }
    }
  }

  if ((numPicked < 2) && (this->parts.getValue() & SoCylinder::BOTTOM)) {
    SbPlane bottom(SbVec3f(0, 1, 0), -h);
    if (bottom.intersect(line, enter)) {
      if (((enter[0] * enter[0] + enter[2] * enter[2]) <= r2) &&
          (action->isBetweenPlanes(enter))) {
        SoPickedPoint * pp = action->addIntersection(enter);
        if (pp) {
          SoCylinderDetail * detail = new SoCylinderDetail();
          detail->setPart((int)SoCylinder::BOTTOM);
          pp->setDetail(detail, this);
        }
      }
    }
  }
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
