/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
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
  \class SoUnits SoUnits.h Inventor/nodes/SoUnits.h
  \brief The SoUnits class is a node for setting unit types.
  \ingroup nodes

  Even though Coin doesn't care what units you are using in your scene
  graph \e per \e se, there's an advantage to using SoUnits nodes: you
  have a way to split your scene graph into different "conceptual"
  parts.

  When encountering SoUnit nodes, the traversal actions methods makes
  sure the following geometry is scaled accordingly.
*/

// Metadon doc:
/*¡
  FIXME: doesn't work correctly for unit settings ANGSTROMS or
  POINTS. 20000321 mortene.
 */

#include <Inventor/nodes/SoUnits.h>
#include <Inventor/nodes/SoSubNodeP.h>

#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/elements/SoBBoxModelMatrixElement.h>
#include <Inventor/elements/SoGLNormalizeElement.h>
#include <Inventor/elements/SoUnitsElement.h>

/*!
  \enum SoUnits::Units

  Enumerates the available unit settings.
*/


/*!
  \var SoSFEnum SoUnits::units

  The units which will be used for nodes following this node in the
  traversal (until the next SoUnit node, if any). Default value of the
  field is SoUnits::METERS.
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
SoUnits::SoUnits(void)
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

// Doc from superclass.
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

// Doc from superclass.
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

// Doc from superclass.
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

// Doc from superclass.
void
SoUnits::doAction(SoAction * action)
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

// Doc from superclass.
void
SoUnits::callback(SoCallbackAction * action)
{
  SoUnits::doAction((SoAction *)action);
}

// Doc from superclass.
void
SoUnits::getMatrix(SoGetMatrixAction * action)
{
  SoUnits::doAction((SoAction *)action);
}

// Doc from superclass.
void
SoUnits::pick(SoPickAction * action)
{
  SoUnits::doAction((SoAction *)action);
}

// Doc from superclass.
void
SoUnits::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  SoUnits::doAction((SoAction *)action);
}
