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
  \class SoUnits SoUnits.h Inventor/nodes/SoUnits.h
  \brief The SoUnits class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoUnits.h>



#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLNormalizeElement.h>

#include <Inventor/actions/SoPickAction.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>

#include <Inventor/elements/SoUnitsElement.h>
#include <Inventor/elements/SoBBoxModelMatrixElement.h>

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

SO_NODE_SOURCE(SoUnits);

/*!
  Constructor.
*/
SoUnits::SoUnits()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoUnits);

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
  SO_NODE_INTERNAL_INIT_CLASS(SoUnits);

  SO_ENABLE(SoGetBoundingBoxAction, SoUnitsElement);
  SO_ENABLE(SoGLRenderAction, SoUnitsElement);
  SO_ENABLE(SoPickAction, SoUnitsElement);
  SO_ENABLE(SoCallbackAction, SoUnitsElement);
  SO_ENABLE(SoGetPrimitiveCountAction, SoUnitsElement);
}

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

    if (scale != 1.0f) {
      SoGLNormalizeElement::setMatrixState(action->getState(), FALSE);
    }
  }
}

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

/*!
  FIXME: write doc
 */
void
SoUnits::callback(SoCallbackAction *action)
{
  SoUnits::doAction((SoAction*)action);
}

/*!
  FIXME: write doc
 */
void
SoUnits::getMatrix(SoGetMatrixAction *action)
{
  SoUnits::doAction((SoAction*)action);
}

/*!
  FIXME: write doc
 */
void
SoUnits::pick(SoPickAction *action)
{
  SoUnits::doAction((SoAction*)action);
}

/*!
  FIXME: write doc
 */
void
SoUnits::getPrimitiveCount(SoGetPrimitiveCountAction *action)
{
  SoUnits::doAction((SoAction*)action);
}
