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

#include <Inventor/nodes/SoUnits.h>
#include <Inventor/nodes/SoSubNodeP.h>

#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>

#include <Inventor/elements/SoModelMatrixElement.h>
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
  0.0000000001f, // ANGSTROMS
  1000.0f, // KILOMETERS
  0.3048f, // FEET
  0.0254f, // INCHES
  3.52777737e-4f, // POINTS
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
  SO_NODE_INTERNAL_INIT_CLASS(SoUnits, SO_FROM_INVENTOR_1);

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
  SoUnits::doAction((SoAction*)action);
}

// Doc from superclass.
void
SoUnits::GLRender(SoGLRenderAction * action)
{
  SoUnits::doAction((SoAction*)action);
}

// Doc from superclass.
void
SoUnits::doAction(SoAction * action)
{
  if (this->units.isIgnored()) return;
  SoState * state = action->getState();

  SoUnitsElement::Units currentunit = SoUnitsElement::get(state);

  if (currentunit != (SoUnitsElement::Units)units.getValue()) {
    SoUnitsElement::set(state,
                        (SoUnitsElement::Units)units.getValue());

    float scale = factors[units.getValue()] / factors[currentunit];
    SoModelMatrixElement::scaleBy(state, this,
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
  if (this->units.isIgnored()) return;

  SoState * state = action->getState();
  SoUnitsElement::Units currentunit = SoUnitsElement::get(state);
  if (currentunit != (SoUnitsElement::Units) this->units.getValue()) {
    SoUnitsElement::set(state,
                        (SoUnitsElement::Units)units.getValue());

    float scale = factors[(int)this->units.getValue()] / factors[(int) currentunit];
    float inv = 1.0f / scale;
    
    SbMatrix m;
    m.setScale(SbVec3f(scale, scale, scale));
    action->getMatrix().multLeft(m);
    m.setScale(SbVec3f(inv, inv, inv));
    action->getInverse().multRight(m);
  }
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
