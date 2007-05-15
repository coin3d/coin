/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2007 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

/*!
  \class SoGeoCoordinate SoGeoCoordinate.h Inventor/nodes/SoGeoCoordinate.h
  \brief The SoGeoCoordinate class is used to specify...
  \ingroup nodes

  <b>FILE FORMAT/DEFAULTS:</b>
  \code
    GeoCoordinate {
    }
  \endcode
*/

// *************************************************************************

#include <Inventor/nodes/SoGeoCoordinate.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/elements/SoModelMatrixElement.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

// *************************************************************************

/*!
  \var SoSFString SoGeoCoordinate::geoCoords
*/

/*!
  \var SoMFString SoGeoCoordinate::geoSystem
*/


// *************************************************************************

SO_NODE_SOURCE(SoGeoCoordinate);

/*!
  Constructor.
*/
SoGeoCoordinate::SoGeoCoordinate(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoGeoCoordinate);

  SO_NODE_ADD_FIELD(point, (""));
  SO_NODE_ADD_FIELD(geoSystem, (""));

  this->geoSystem.setNum(2);
  this->geoSystem.set1Value(0, "GD");
  this->geoSystem.set1Value(1, "WE");
  this->geoSystem.setDefault(TRUE);
}

/*!
  Destructor.
*/
SoGeoCoordinate::~SoGeoCoordinate()
{
}

// Doc from superclass.
void
SoGeoCoordinate::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoGeoCoordinate, SO_FROM_INVENTOR_1|SoNode::VRML1);
}

// Doc from superclass.
void
SoGeoCoordinate::doAction(SoAction * action)
{
  SoState * state = action->getState();
  SbMatrix m = this->getTransform(state);
}

// Doc from superclass.
void
SoGeoCoordinate::GLRender(SoGLRenderAction * action)
{
  SoGeoCoordinate::doAction((SoAction *)action);
}

// Doc from superclass.
void
SoGeoCoordinate::getBoundingBox(SoGetBoundingBoxAction * action)
{
  SoGeoCoordinate::doAction((SoAction *)action);
}

// Doc from superclass.
void
SoGeoCoordinate::getMatrix(SoGetMatrixAction * action)
{
  SbMatrix m = this->getTransform(action->getState());

  action->getMatrix().multLeft(m);
  action->getInverse().multRight(m.inverse());
}

// Doc from superclass.
void
SoGeoCoordinate::callback(SoCallbackAction * action)
{
  SoGeoCoordinate::doAction((SoAction *)action);
}

// Doc from superclass.
void
SoGeoCoordinate::pick(SoPickAction * action)
{
  SoGeoCoordinate::doAction((SoAction *)action);
}

// Doc from superclass.
void
SoGeoCoordinate::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  SoGeoCoordinate::doAction((SoAction *)action);
}

// *************************************************************************

SbMatrix
SoGeoCoordinate::getTransform(SoState * state) const
{
  // FIXME: calculate based on GeoOrigin
  return SbMatrix::identity();
}
