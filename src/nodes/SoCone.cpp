/**************************************************************************\
 * 
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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
  \class SoCone SoCone.h Inventor/nodes/SoCone.h
  \brief The SoCone class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/SbName.h>
#include <Inventor/bundles/SoMaterialBundle.h>
#include <Inventor/misc/SoState.h>
#include <assert.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/misc/SoGL.h>
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION

#if !defined(COIN_EXCLUDE_SOGLSHAPEHINTSELEMENT)
#include <Inventor/elements/SoGLShapeHintsElement.h>
#endif // !COIN_EXCLUDE_SOGLSHAPEHINTSELEMENY

#if !defined(COIN_EXCLUDE_SOGLSHADEMODELELEMENT)
#include <Inventor/elements/SoGLShadeModelElement.h>
#endif // !COIN_EXCLUDE_SOGLSHADEMODELELEMENT


#if !defined(COIN_EXCLUDE_SOMATERIALBINDINGELEMENT)
#include <Inventor/elements/SoMaterialBindingElement.h>
#endif

#if !defined(COIN_EXCLUDE_SOCOMPLEXITYTYPEELEMENT)
#include <Inventor/elements/SoComplexityTypeElement.h>
#endif

#if !defined(COIN_EXCLUDE_SOCOMPLEXITYELEMENT)
#include <Inventor/elements/SoComplexityElement.h>
#endif

/*!
  \enum SoCone::Part
  FIXME: write documentation for enum
*/
/*!
  \var SoCone::Part SoCone::SIDES
  FIXME: write documentation for enum definition
*/
/*!
  \var SoCone::Part SoCone::BOTTOM
  FIXME: write documentation for enum definition
*/
/*!
  \var SoCone::Part SoCone::ALL
  FIXME: write documentation for enum definition
*/


/*!
  \var SoSFBitMask SoCone::parts
  FIXME: write documentation for field
*/
/*!
  \var SoSFFloat SoCone::bottomRadius
  FIXME: write documentation for field
*/
/*!
  \var SoSFFloat SoCone::height
  FIXME: write documentation for field
*/


// *************************************************************************

//$ BEGIN TEMPLATE NodeSource(SoCone)
SoType SoCone::classTypeId = SoType::badType();

/*!
  Returns a new instance of the SoCone node class.
*/
void *
SoCone::createInstance(void)
{
  return new SoCone;
}

/*!
  Returns the unique type identifier for the SoCone class.
*/
SoType
SoCone::getClassTypeId(void)
{
  return SoCone::classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoCone::getTypeId(void) const
{
  return SoCone::classTypeId;
}
//$ END TEMPLATE NodeSource


// parameterized equation for a point on a line:
//     Q = P + t*D,  P is a known line point, D is the line direction
//
// equation for a point on a cone:
// 
//  |Q - L| = r
//
//     h - y 
// r = ----- * bottomr,  L = y-axis
//       h 
//
// |P + t*D - L| = r
//
// some algebra (phew) leads to:
// 
// h * ((dx^2 + dy^2 + dz^2 + 1) * t^2 +
// (2dx - 2y0^2 + 3y0 + 2dz) * t  +
// x0^2 + 2y0^2 + z0^2 - h)            =  0
//                                   
// using
//
// t = -b +/- sqrt(b^2 - 4ac)/2a
//
// a = h(dx^2 + dy^2 +dz^2 + 1)
// b = h(2dx - 2y0^2 + 3y0 + 2dz)
// c = h(x0^2 + 2y0^2 + z0^2 - h)
//
// we get zero, one or two t-values.
//
// After backsubstitution in the line equation, 
// we have to check if 0 <= y <= h.
//
static SbBool 
intersect_cone_line(const float bottomr,
		    const float h,
		    const SbLine &line,
		    SbVec3f &enter,
		    SbVec3f &exit)
{
#if 0
  const SbVec3f &d = line.getDirection();
  const SbVec3f &p = line.getPosition();
  float a = h*(d.dot(d) + 1.0f);
  float b = h*(d[0] - 2.0f*p[1]*p[1] + 3.0f*p[1] + 2.0f*d[2]);
  float c = h*(p[0]*p[0] + 2.0f*p[1]*p[1] + p[2]*p[2] - h);

  float root = b*b - 4.0f*a*c;
  if (root < 0.0f) return FALSE; // no solutions
  
  root = sqrt(root);
  
  float t0 = (-b + root) / (2.0f*a);
  float t1 = (-b - root) / (2.0f*a);
#endif

}


/*!
  Constructor.
*/
SoCone::SoCone()
{
//$ BEGIN TEMPLATE NodeConstructor(SoCone)
  // Make sure the class has been initialized.
  assert(SoCone::classTypeId != SoType::badType());
//$ END TEMPLATE NodeConstructor

  SO_NODE_ADD_FIELD(bottomRadius,(1.0f));
  SO_NODE_ADD_FIELD(height,(2.0f));
  SO_NODE_ADD_FIELD(parts,(ALL));

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

/*!
  Does initialization common for all objects of the
  SoCone class. This includes setting up the
  type system, among other things.
*/
void
SoCone::initClass(void)
{
//$ BEGIN TEMPLATE InitNodeSource(Cone)
  // Make sure we only initialize once.
  assert(SoCone::classTypeId == SoType::badType());
  // Make sure superclass get initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoCone::classTypeId =
    SoType::createType(inherited::getClassTypeId(), "Cone",
                       &SoCone::createInstance,
                       SoNode::nextActionMethodIndex++);
//$ END TEMPLATE InitNodeSource
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoCone::cleanClass(void)
{
}

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
/*!
  FIXME: write function documentation
*/
void
SoCone::computeBBox(SoAction * /* action */, SbBox3f & box, SbVec3f & center)
{
  float r = this->bottomRadius.getValue();
  float h = this->height.getValue();

  // The SIDES are present, so just find the middle point and enclose
  // everything.
  if (this->parts.getValue() & SoCone::SIDES) {
    center.setValue(0.0f, 0.0f, 0.0f);
    box.setBounds(SbVec3f(-r, -h/2.0f, -r), SbVec3f(r, h/2.0f, r));
  }
  // ..no SIDES, but we've still got the bottom (NB: OIV misses this case).
  else if (this->parts.getValue() & SoCone::BOTTOM) {
    center.setValue(0.0f, -h/2.0f, 0.0f);
    box.setBounds(SbVec3f(-r, -h/2.0f, -r), SbVec3f(r, -h/2.0f, r));
  }
  // ..no parts present. My confidence is shot -- I feel very small.
  else {
    center.setValue(0.0f, 0.0f, 0.0f);
    box.setBounds(SbVec3f(0.0f, 0.0f, 0.0f), SbVec3f(0.0f, 0.0f, 0.0f));
  }
}
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write function documentation
*/
void
SoCone::GLRender(SoGLRenderAction * action)
{
  if (!shouldGLRender(action)) return;

  SoState *state = action->getState();

  SoCone::Part p = (SoCone::Part) this->parts.getValue();
  unsigned int flags = SOGL_NEED_NORMALS | SOGL_NEED_TEXCOORDS;
  if (p & SoCone::SIDES) flags |= SOGL_RENDER_SIDE;
  if (p & SoCone::BOTTOM) flags |= SOGL_RENDER_BOTTOM;

#if !defined(COIN_EXCLUDE_SOMATERIALBINDINGELEMENT)
  SoMaterialBindingElement::Binding bind =
    SoMaterialBindingElement::get(state);
  if (bind == SoMaterialBindingElement::PER_PART ||
      bind == SoMaterialBindingElement::PER_PART_INDEXED)
    flags |= SOGL_MATERIAL_PER_PART;
#endif
  

  SoMaterialBundle mb(action);
  mb.sendFirst();

#if !defined(COIN_EXCLUDE_SOGLSHAPEHINTSELEMENT)
  const SoGLShapeHintsElement *sh = (SoGLShapeHintsElement*)
    state->getConstElement(SoGLShapeHintsElement::getClassStackIndex());

  if (p == ALL) sh->forceSend(TRUE, TRUE);
  else sh->forceSend(TRUE, FALSE, TRUE);
#endif

  float complexity = 1.0f;

#if !defined(COIN_EXCLUDE_SOCOMPLEXITYELEMENT)
#if !defined(COIN_EXCLUDE_SOCOMPLEXITYTYPEELEMENT)

  // FIXME: check for OBJECT_SPACE or SCREEN_SPACE
  if (SoComplexityTypeElement::get(state) !=
      SoComplexityTypeElement::BOUNDING_BOX) {
    complexity = SoComplexityElement::get(state);
  }

#endif
#endif
  
#if !defined(COIN_EXCLUDE_SOGLSHADEMODELELEMENT)
  const SoGLShadeModelElement * sm = (SoGLShadeModelElement *)
    state->getConstElement(SoGLShadeModelElement::getClassStackIndex());
  if (!(p & SIDES)) 
    sm->forceSend(TRUE); // flatshading
  else
    sm->forceSend(FALSE); // smooth
  
#endif // ! COIN_EXCLUDE_SOGLSHADEMODELELEMENT

  sogl_render_cone(this->bottomRadius.getValue(),
		   this->height.getValue(),
		   (int)(40.0f*complexity),
		   &mb,
		   flags);
}

/*!
  FIXME: write function documentation
*/
SbBool 
SoCone::willSetShapeHints(void) const
{
  return TRUE;
}

/*!
  FIXME: write function documentation
*/
SbBool 
SoCone::willSetShadeModel(void) const
{
  return TRUE;
}

#endif // !COIN_EXCLUDE_SOGLRENDERACTION


/*!
  Add a part to the cone.

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
  Remove a part from the cone.

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
  Returns \a TRUE if rendering of the given part is currently turned on.

  \sa addPart(), removePart()
*/
SbBool
SoCone::hasPart(SoCone::Part part) const
{
  return (this->parts.getValue() & part) ? TRUE : FALSE;
}

#if !defined(COIN_EXCLUDE_SORAYPICKACTION)
/*!
  FIXME: write doc
*/
void
SoCone::rayPick(SoRayPickAction * /* action */)
{
#if 0
  action->setObjectSpace();
  const SbLine &line = action->getLine();
#endif
}
#endif // !COIN_EXCLUDE_SORAYPICKACTION

#if !defined(COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION)
/*!
  FIXME: write doc
*/
void
SoCone::getPrimitiveCount(SoGetPrimitiveCountAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION

#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write doc
*/
void
SoCone::generatePrimitives(SoAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOACTION
