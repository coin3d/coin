/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
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
  \class SoCone SoCone.h Inventor/nodes/SoCone.h
  \brief The SoCone class is for rendering cone shapes.
  \ingroup nodes
*/

#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoSubNodeP.h>

#include <Inventor/SbLine.h>
#include <Inventor/SbPlane.h>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/bundles/SoMaterialBundle.h>
#include <Inventor/details/SoConeDetail.h>
#include <Inventor/elements/SoMaterialBindingElement.h>
#include <Inventor/misc/SoGL.h>
#include <Inventor/misc/SoGenerate.h>
#include <Inventor/misc/SoState.h>
#include <assert.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG


/*!
  \enum SoCone::Part
  Enumerates the various parts of the cone, for setting inclusion or
  exclusion from the shape.
*/

/*!
  \var SoSFBitMask SoCone::parts
  The parts to use for the cone shape. Defaults to SoCone::ALL.
*/
/*!
  \var SoSFFloat SoCone::bottomRadius
  Radius of the cone's bottom disc. Default value is 1.0.
*/
/*!
  \var SoSFFloat SoCone::height
  Height of cone. Default value is 2.0.
*/

#define CONE_SIDE_NUMTRIS 40.0f

// *************************************************************************

SO_NODE_SOURCE(SoCone);

/*!
  Constructor.
*/
SoCone::SoCone(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoCone);

  SO_NODE_ADD_FIELD(bottomRadius, (1.0f));
  SO_NODE_ADD_FIELD(height, (2.0f));
  SO_NODE_ADD_FIELD(parts, (ALL));

  SO_NODE_DEFINE_ENUM_VALUE(Part, SIDES);
  SO_NODE_DEFINE_ENUM_VALUE(Part, BOTTOM);
  SO_NODE_DEFINE_ENUM_VALUE(Part, ALL);
  SO_NODE_SET_SF_ENUM_TYPE(parts, Part);
}

/*!
  Destructor.
*/
SoCone::~SoCone()
{
}


//
// this was actually much easier than I first though since the Cone
// is aligned with the y-axis.
//
// A point on a SoCone can be expressed by:
//
// x^2 + z^2 = r^2, where r = ((h/2)-y)*br/h
//
// Substituting x, y and z with the parametric line equations, and we
// can find zero, one or two solutions for t. We have to check the y-value
// afterwards to see if it's between +/- (h/2)
//

static int
intersect_cone_line(const float br,
                    const float h,
                    const SbLine & line,
                    SbVec3f & enter,
                    SbVec3f & exit)
{
  float h2 = h * 0.5f;
  SbVec3f d = line.getDirection();
  SbVec3f p = line.getPosition();

  float tmp = (br * br)/(h * h);

  float a = d[0]*d[0] + d[2]*d[2] - d[1]*d[1]*tmp;
  float b = 2.0f*d[0]*p[0] + 2.0f*d[2]*p[2] + (2.0f*h2*d[1] - 2.0f*p[1]*d[1]) * tmp;
  float c = p[0]*p[0] + p[2]*p[2] + (2.0f*p[1]*h2 - h2*h2 - p[1]*p[1])*tmp;

  float root = b*b - 4.0f*a*c;

  if (root < 0) return 0;

  root = (float) sqrt(root);

  float t0 = (-b - root) / (2.0f*a);
  float t1 = (-b + root) / (2.0f*a);

  if (t1 < t0) SbSwap(t0, t1);

  enter = p + t0*d;
  exit = p + t1*d;

  int numisect = 0;
  if (fabs(enter[1]) <= h2) numisect++;
  if (fabs(exit[1]) <= h2 && t0 != t1) {
    numisect++;
    if (numisect == 1) enter = exit;
  }
  return numisect;
}


// Doc from parent.
void
SoCone::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoCone);
}

// Doc from parent.
void
SoCone::computeBBox(SoAction * action, SbBox3f & box, SbVec3f & center)
{
  float r = this->bottomRadius.getValue();
  float h = this->height.getValue();

  // Allow negative values.
  if (h < 0.0f) h = -h;
  if (r < 0.0f) r = -r;

  float half_height = h/2.0f;

  // The SIDES are present, so just find the middle point and enclose
  // everything.
  if (this->parts.getValue() & SoCone::SIDES) {
    center.setValue(0.0f, 0.0f, 0.0f);
    box.setBounds(SbVec3f(-r, -half_height, -r), SbVec3f(r, half_height, r));
  }
  // ..no SIDES, but we've still got the bottom (NB: OIV misses this case).
  else if (this->parts.getValue() & SoCone::BOTTOM) {
    center.setValue(0.0f, -half_height, 0.0f);
    box.setBounds(SbVec3f(-r, -half_height, -r), SbVec3f(r, -half_height, r));
  }
  // ..no parts present. My confidence is shot -- I feel very small.
  else {
    center.setValue(0.0f, 0.0f, 0.0f);
    box.setBounds(SbVec3f(0.0f, 0.0f, 0.0f), SbVec3f(0.0f, 0.0f, 0.0f));
  }
}

// Doc from parent.
void
SoCone::GLRender(SoGLRenderAction * action)
{
#if COIN_DEBUG && 0 // debug
  SoDebugError::postInfo("SoCone::GLRender", "start");
#endif // debug

  if (!shouldGLRender(action)) return;

  SoState * state = action->getState();

  SoCone::Part p = (SoCone::Part) this->parts.getValue();
  unsigned int flags = SOGL_NEED_NORMALS | SOGL_NEED_TEXCOORDS;
  if (p & SoCone::SIDES) flags |= SOGL_RENDER_SIDE;
  if (p & SoCone::BOTTOM) flags |= SOGL_RENDER_BOTTOM;

  SoMaterialBindingElement::Binding bind =
    SoMaterialBindingElement::get(state);
  if (bind == SoMaterialBindingElement::PER_PART ||
      bind == SoMaterialBindingElement::PER_PART_INDEXED)
    flags |= SOGL_MATERIAL_PER_PART;

  SoMaterialBundle mb(action);
  mb.sendFirst();

  float complexity = this->getComplexityValue(action);

  sogl_render_cone(this->bottomRadius.getValue(),
                   this->height.getValue(),
                   (int)(CONE_SIDE_NUMTRIS * complexity),
                   &mb,
                   flags);

#if COIN_DEBUG && 0 // debug
  SoDebugError::postInfo("SoCone::GLRender", "end");
#endif // debug
}

/*!
  Add a \a part to the cone.

  \sa removePart(), hasPart()
*/
void
SoCone::addPart(SoCone::Part part)
{
  if (this->hasPart(part)) {
#if COIN_DEBUG
    SoDebugError::postWarning("SoCone::addPart", "part already set");
#endif // COIN_DEBUG
    return;
  }

  this->parts.setValue(this->parts.getValue() | part);
}

/*!
  Remove a \a part from the cone.

  \sa addPart(), hasPart()
*/
void
SoCone::removePart(SoCone::Part part)
{
  if (!this->hasPart(part)) {
#if COIN_DEBUG
    SoDebugError::postWarning("SoCone::removePart", "part was not set");
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
SoCone::hasPart(SoCone::Part part) const
{
  return (this->parts.getValue() & part) ? TRUE : FALSE;
}

// Doc from parent.
void
SoCone::rayPick(SoRayPickAction * action)
{
  if (!this->shouldRayPick(action)) return;

  action->setObjectSpace();
  const SbLine & line = action->getLine();

  int numisect = 0;
  SbVec3f isect[2];

  if (this->parts.getValue() & SoCone::SIDES) {
    numisect = intersect_cone_line(this->bottomRadius.getValue(),
                                   this->height.getValue(),
                                   line,
                                   isect[0],
                                   isect[1]);

    for (int i = 0; i < numisect; i++) {
      if (action->isBetweenPlanes(isect[i])) {
        SoPickedPoint * pp = action->addIntersection(isect[i]);
        if (pp) {
          SoConeDetail * detail = new SoConeDetail;
          detail->setPart((int)SoCone::SIDES);
          pp->setDetail(detail, this);
        }
      }
    }
  }

  if ((numisect < 2) && (this->parts.getValue() & SoCone::BOTTOM)) {
    SbPlane bottom(SbVec3f(0, 1, 0), -this->height.getValue()* 0.5f);
    SbVec3f bpt;
    float r2 = this->bottomRadius.getValue();
    r2 *= r2;
    if (bottom.intersect(line, bpt)) {
      if (((bpt[0] * bpt[0] + bpt[2] * bpt[2]) <= r2) &&
          (action->isBetweenPlanes(bpt))) {
        SoPickedPoint * pp = action->addIntersection(bpt);
        if (pp) {
          SoConeDetail * detail = new SoConeDetail();
          detail->setPart((int)SoCone::BOTTOM);
          pp->setDetail(detail, this);
        }
      }
    }
  }
}

// Doc from parent.
void
SoCone::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  if (!this->shouldPrimitiveCount(action)) return;

  float complexity = this->getComplexityValue(action);
  int numtris = (int)(complexity * CONE_SIDE_NUMTRIS);

  if (this->parts.getValue() & SoCone::BOTTOM) {
    action->addNumTriangles(numtris-2);
  }
  if (this->parts.getValue() & SoCone::SIDES) {
    action->addNumTriangles(numtris);
  }
}

// Doc from parent.
void
SoCone::generatePrimitives(SoAction * action)
{
  SoCone::Part p = (SoCone::Part) this->parts.getValue();
  unsigned int flags = 0;
  if (p & SoCone::SIDES) flags |= SOGEN_GENERATE_SIDE;
  if (p & SoCone::BOTTOM) flags |= SOGEN_GENERATE_BOTTOM;

  SoMaterialBindingElement::Binding bind =
    SoMaterialBindingElement::get(action->getState());
  if (bind == SoMaterialBindingElement::PER_PART ||
      bind == SoMaterialBindingElement::PER_PART_INDEXED)
    flags |= SOGL_MATERIAL_PER_PART;

  float complexity = this->getComplexityValue(action);

  sogen_generate_cone(this->bottomRadius.getValue(),
                      this->height.getValue(),
                      (int)(CONE_SIDE_NUMTRIS * complexity),
                      flags,
                      this,
                      action);
}
