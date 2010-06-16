/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

/*!
  \class SoGeoLocation SoGeoLocation.h Inventor/nodes/SoGeoLocation.h
  \brief The SoGeoLocation class is used to georeference the following nodes.
  \ingroup nodes

  <b>FILE FORMAT/DEFAULTS:</b>
  \code
    GeoLocation {
      geoSystem ["GD", "WE"]
      geoCoords 0 0 0
    }
  \endcode

  This node specifies an absolute geographic coordinate system for the
  following nodes. When rendering (or applying other actions), Coin
  will add a transformation which transforms the geometry into the
  SoGeoOrigin coordinate system. All objects will be rotated to make
  the local Z axis point up from the ground (at the specified
  geo-location), the Y axis will point towards the north pole, and the
  X axis is found using the right hand rule.


  \sa SoGeoOrigin
  \since Coin 2.5
*/

// *************************************************************************

#include <Inventor/nodes/SoGeoLocation.h>

#include <Inventor/nodes/SoGeoOrigin.h>
#include <Inventor/elements/SoGeoElement.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/errors/SoDebugError.h>

#include "nodes/SoSubNodeP.h"
#include "geo/SoGeo.h"


// *************************************************************************

/*!
  \var SoSFString SoGeoLocation::geoCoords

  Used for specifying the geographic coordinates.

  \sa SoGeoOrigin::geoCoords
*/

/*!
  \var SoMFString SoGeoLocation::geoSystem

  Used to specify a spatial reference frame.

  \sa SoGeoOrigin::geoSystem
*/


// *************************************************************************

SO_NODE_SOURCE(SoGeoLocation);

/*!
  Constructor.
*/
SoGeoLocation::SoGeoLocation(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoGeoLocation);

  SO_NODE_ADD_FIELD(geoCoords, (0.0, 0.0, 0.0));
  SO_NODE_ADD_FIELD(geoSystem, (""));

  this->geoSystem.setNum(2);
  this->geoSystem.set1Value(0, "GD");
  this->geoSystem.set1Value(1, "WE");
  this->geoSystem.setDefault(TRUE);
}

/*!
  Destructor.
*/
SoGeoLocation::~SoGeoLocation()
{
}

// Doc from superclass.
void
SoGeoLocation::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoGeoLocation, SO_FROM_INVENTOR_1|SoNode::VRML1);
}

// Doc from superclass.
void
SoGeoLocation::doAction(SoAction * action)
{
  SoState * state = action->getState();
  SbMatrix m = this->getTransform(state);

  SoModelMatrixElement::set(state, this, m);
}

// Doc from superclass.
void
SoGeoLocation::GLRender(SoGLRenderAction * action)
{
  SoGeoLocation::doAction((SoAction *)action);
}

// Doc from superclass.
void
SoGeoLocation::getBoundingBox(SoGetBoundingBoxAction * action)
{
  SoState * state = action->getState();
  SbMatrix m = this->getTransform(state);
  SoModelMatrixElement::mult(state,
                             this,
                             SoModelMatrixElement::get(state).inverse());
  SoModelMatrixElement::mult(state,
                             this,
                             m);
}

// Doc from superclass.
void
SoGeoLocation::getMatrix(SoGetMatrixAction * action)
{
  SbMatrix m = this->getTransform(action->getState());

  action->getMatrix().multLeft(m);
  action->getInverse().multRight(m.inverse());
}

// Doc from superclass.
void
SoGeoLocation::callback(SoCallbackAction * action)
{
  SoGeoLocation::doAction((SoAction *)action);
}

// Doc from superclass.
void
SoGeoLocation::pick(SoPickAction * action)
{
  SoGeoLocation::doAction((SoAction *)action);
}

// Doc from superclass.
void
SoGeoLocation::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  SoGeoLocation::doAction((SoAction *)action);
}

// *************************************************************************

SbMatrix
SoGeoLocation::getTransform(SoState * state) const
{
  SoGeoOrigin * origin = SoGeoElement::get(state);

  if (origin) {
    return SoGeo::calculateTransform(origin->geoSystem.getValues(0),
                                     origin->geoSystem.getNum(),
                                     origin->geoCoords.getValue(),

                                     this->geoSystem.getValues(0),
                                     this->geoSystem.getNum(),
                                     this->geoCoords.getValue());
  }
  SoDebugError::post("SoGeoLocation::getTransform",
                     "No SoGeoOrigin node found on stack.");
  return SbMatrix::identity();
}

#ifdef COIN_TEST_SUITE

BOOST_AUTO_TEST_CASE(initialized)
{
  SoGeoLocation * node = new SoGeoLocation;
  assert(node);
  node->ref();
  BOOST_CHECK_MESSAGE(node->getTypeId() != SoType::badType(),
                      "missing class initialization");
  node->unref();
}

#endif // COIN_TEST_SUITE
