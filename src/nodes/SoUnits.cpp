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
  \class SoUnits SoUnits.h Inventor/nodes/SoUnits.h
  \brief The SoUnits class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoUnits.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/SbName.h>

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION
#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
#include <Inventor/actions/SoGLRenderAction.h>
#if !defined(COIN_EXCLUDE_SOGLNORMALIZEELEMENT)
#include <Inventor/elements/SoGLNormalizeElement.h>
#endif // ! COIN_EXCLUDE_SOGLNORMALIZEELEMENT
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SOUNITSELEMENT)
#include <Inventor/elements/SoUnitsElement.h>
#endif // !COIN_EXCLUDE_SOUNITSELEMENT
#if !defined(COIN_EXCLUDE_SOBBOXMODELMATRIXELEMENT)
#include <Inventor/elements/SoBBoxModelMatrixElement.h>
#endif // !COIN_EXCLUDE_SOBBOXMODELMATRIXELEMENT
#if !defined(COIN_EXCLUDE_SOMODELMATRIXELEMENT)
#include <Inventor/elements/SoModelMatrixElement.h>
#endif // !COIN_EXCLUDE_SOMODELMATRIXELEMENT

/*!
  \enum SoUnits::Units
  FIXME: write documentation for enum
*/
/*!
  \var SoUnits::Units SoUnits::METERS
  FIXME: write documentation for enum definition
*/
/*!
  \var SoUnits::Units SoUnits::CENTIMETERS
  FIXME: write documentation for enum definition
*/
/*!
  \var SoUnits::Units SoUnits::MILLIMETERS
  FIXME: write documentation for enum definition
*/
/*!
  \var SoUnits::Units SoUnits::MICROMETERS
  FIXME: write documentation for enum definition
*/
/*!
  \var SoUnits::Units SoUnits::MICRONS
  FIXME: write documentation for enum definition
*/
/*!
  \var SoUnits::Units SoUnits::NANOMETERS
  FIXME: write documentation for enum definition
*/
/*!
  \var SoUnits::Units SoUnits::ANGSTROMS
  FIXME: write documentation for enum definition
*/
/*!
  \var SoUnits::Units SoUnits::KILOMETERS
  FIXME: write documentation for enum definition
*/
/*!
  \var SoUnits::Units SoUnits::FEET
  FIXME: write documentation for enum definition
*/
/*!
  \var SoUnits::Units SoUnits::INCHES
  FIXME: write documentation for enum definition
*/
/*!
  \var SoUnits::Units SoUnits::POINTS
  FIXME: write documentation for enum definition
*/
/*!
  \var SoUnits::Units SoUnits::YARDS
  FIXME: write documentation for enum definition
*/
/*!
  \var SoUnits::Units SoUnits::MILES
  FIXME: write documentation for enum definition
*/
/*!
  \var SoUnits::Units SoUnits::NAUTICAL_MILES
  FIXME: write documentation for enum definition
*/


/*!
  \var SoSFEnum SoUnits::units
  FIXME: write documentation for field
*/


// *************************************************************************

static const float factors[] = {
  1.0f, // METERS
  0.01f, // CENTIMETERS
  0.001f, // MILLIMETERS
  0.000001f, // MICROMETERS
  0.000001f, // MICRONS
  0.000000001f, // NANOMETERS
  // FIXME: find correct value. 19990315 mortene.
  1.0f, // ANGSTROMS
  1000.0f, // KILOMETERS
  0.3048f, // FEET
  0.0254f, // INCHES
  // FIXME: find correct value. 19990315 mortene.
  1.0f, // POINTS
  0.9144f, // YARDS
  1609.3f, // MILES
  1852.0f, // NAUTICAL
};

// *************************************************************************

//$ BEGIN TEMPLATE NodeSource(SoUnits)
SoType SoUnits::classTypeId = SoType::badType();

/*!
  Returns a new instance of the SoUnits node class.
*/
void *
SoUnits::createInstance(void)
{
  return new SoUnits;
}

/*!
  Returns the unique type identifier for the SoUnits class.
*/
SoType
SoUnits::getClassTypeId(void)
{
  return SoUnits::classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoUnits::getTypeId(void) const
{
  return SoUnits::classTypeId;
}
//$ END TEMPLATE NodeSource

/*!
  Constructor.
*/
SoUnits::SoUnits()
{
//$ BEGIN TEMPLATE NodeConstructor(SoUnits)
  // Make sure the class has been initialized.
  assert(SoUnits::classTypeId != SoType::badType());
//$ END TEMPLATE NodeConstructor

  SO_NODE_ADD_FIELD(units, (SoUnits::METERS));

  SO_NODE_DEFINE_ENUM_VALUE(Units, METERS);
  SO_NODE_DEFINE_ENUM_VALUE(Units, CENTIMETERS);
  SO_NODE_DEFINE_ENUM_VALUE(Units, MILLIMETERS);
  SO_NODE_DEFINE_ENUM_VALUE(Units, MICROMETERS);
  SO_NODE_DEFINE_ENUM_VALUE(Units, MICRONS);
  SO_NODE_DEFINE_ENUM_VALUE(Units, NANOMETERS);
  SO_NODE_DEFINE_ENUM_VALUE(Units, ANGSTROMS);
  SO_NODE_DEFINE_ENUM_VALUE(Units, KILOMETERS);
  SO_NODE_DEFINE_ENUM_VALUE(Units, FEET);
  SO_NODE_DEFINE_ENUM_VALUE(Units, INCHES);
  SO_NODE_DEFINE_ENUM_VALUE(Units, POINTS);
  SO_NODE_DEFINE_ENUM_VALUE(Units, YARDS);
  SO_NODE_DEFINE_ENUM_VALUE(Units, MILES);
  SO_NODE_DEFINE_ENUM_VALUE(Units, NAUTICAL_MILES);
  SO_NODE_SET_SF_ENUM_TYPE(units, Units);
}

/*!
  Destructor.
*/
SoUnits::~SoUnits()
{
}

/*!
  Does initialization common for all objects of the
  SoUnits class. This includes setting up the
  type system, among other things.
*/
void
SoUnits::initClass(void)
{
//$ BEGIN TEMPLATE InitNodeSource(Units)
  // Make sure we only initialize once.
  assert(SoUnits::classTypeId == SoType::badType());
  // Make sure superclass get initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoUnits::classTypeId =
    SoType::createType(inherited::getClassTypeId(), "Units",
                       &SoUnits::createInstance,
                       SoNode::nextActionMethodIndex++);
//$ END TEMPLATE InitNodeSource

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
  SO_ENABLE(SoGetBoundingBoxAction, SoUnitsElement);
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION
#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  SO_ENABLE(SoGLRenderAction, SoUnitsElement);
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoUnits::cleanClass(void)
{
}

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
/*!
  FIXME: write function documentation
*/
void
SoUnits::getBoundingBox(SoGetBoundingBoxAction * action)
{
  SoUnitsElement::Units currentunit = SoUnitsElement::get(action->getState());

  if (currentunit != (SoUnitsElement::Units)units.getValue()) {
    SoUnitsElement::set(action->getState(),
			(SoUnitsElement::Units)units.getValue());

    float scale = factors[units.getValue()] / factors[currentunit];
    SoBBoxModelMatrixElement::scaleBy(action->getState(), this,
				      SbVec3f(scale, scale, scale));
  }
}
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write function documentation
*/
void
SoUnits::GLRender(SoGLRenderAction * action)
{
  SoUnitsElement::Units currentunit = SoUnitsElement::get(action->getState());

  if (currentunit != (SoUnitsElement::Units)units.getValue()) {
    SoUnitsElement::set(action->getState(),
			(SoUnitsElement::Units)units.getValue());

    float scale = factors[units.getValue()] / factors[currentunit];
    SoModelMatrixElement::scaleBy(action->getState(), this,
				  SbVec3f(scale, scale, scale));

#if !defined(COIN_EXCLUDE_SOGLNORMALIZEELEMENT)
    if (scale != 1.0f) {
      SoGLNormalizeElement::setMatrixState(action->getState(), FALSE);
    }
#endif // ! COIN_EXCLUDE_SOGLNORMALIZEELEMENT  
    
  }
}
#endif // !COIN_EXCLUDE_SOGLRENDERACTION



#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write doc
 */
void
SoUnits::doAction(SoAction *action)
{
  SoUnitsElement::Units currentunit = SoUnitsElement::get(action->getState());
  
  if (currentunit != (SoUnitsElement::Units)units.getValue()) {
    SoUnitsElement::set(action->getState(),
			(SoUnitsElement::Units)units.getValue());
    
    float scale = factors[units.getValue()] / factors[currentunit];
    SoModelMatrixElement::scaleBy(action->getState(), this,
				  SbVec3f(scale, scale, scale));    
  }
}
#endif // !COIN_EXCLUDE_SOACTION

#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
/*!
  FIXME: write doc
 */
void
SoUnits::callback(SoCallbackAction *action)
{
  SoUnits::doAction((SoAction*)action);
}
#endif // !COIN_EXCLUDE_SOCALLBACKACTION

#if !defined(COIN_EXCLUDE_SOGETMATRIXACTION)
/*!
  FIXME: write doc
 */
void
SoUnits::getMatrix(SoGetMatrixAction *action)
{
  SoUnits::doAction((SoAction*)action);
}
#endif // !COIN_EXCLUDE_SOGETMATRIXACTION

#if !defined(COIN_EXCLUDE_SOPICKACTION)
/*!
  FIXME: write doc
 */
void
SoUnits::pick(SoPickAction *action)
{
  SoUnits::doAction((SoAction*)action);
}
#endif // !COIN_EXCLUDE_SOPICKACTION

#if !defined(COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION)
/*!
  FIXME: write doc
 */
void
SoUnits::getPrimitiveCount(SoGetPrimitiveCountAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION
