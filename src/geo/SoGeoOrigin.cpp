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
  \class SoGeoOrigin SoGeoOrigin.h Inventor/nodes/SoGeoOrigin.h
  \brief The SoGeoOrigin class is used to specify an absolute geographic location against which geometry is referenced.
  \ingroup nodes

  <b>FILE FORMAT/DEFAULTS:</b>
  \code
    GeoOrigin {
      geoSystem ["GD", "WE"] 
      geoCoords ""
    }
  \endcode

  A scene graph should only contain one GeoOrigin node, and all
  geometry in the scene graph will be rendered relative to this
  position. This means that the precision will be best if the
  GeoOrigin position is as close to actual camera position as
  possible.
  
  Coin needs a local Cartesian coordinate system when rendering,
  meaning that if the origin is specified as a Geodetic spatial
  reference frame (latitude/longitude), the most appropriate UTM zone
  will be chosen, and used as a projection when rendering.

*/

// *************************************************************************

#include <Inventor/nodes/SoGeoOrigin.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/actions/SoAudioRenderAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/elements/SoGeoElement.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

// *************************************************************************

/*!
  \var SoSFString SoGeoOrigin::geoCoords

  Used for specifying the geographic coordinates.
*/

/*!
  \var SoMFString SoGeoOrigin::geoSystem

  Used to specify a spatial reference frame.
*/


// *************************************************************************

SO_NODE_SOURCE(SoGeoOrigin);

/*!
  Constructor.
*/
SoGeoOrigin::SoGeoOrigin(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoGeoOrigin);
  
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
SoGeoOrigin::~SoGeoOrigin()
{
}

// Doc from superclass.
void
SoGeoOrigin::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoGeoOrigin, SO_FROM_INVENTOR_1|SoNode::VRML1);

  SO_ENABLE(SoGetBoundingBoxAction, SoGeoElement);
  SO_ENABLE(SoGLRenderAction, SoGeoElement);
  SO_ENABLE(SoGetMatrixAction, SoGeoElement);
  SO_ENABLE(SoGetPrimitiveCountAction, SoGeoElement);
  SO_ENABLE(SoPickAction, SoGeoElement);
  SO_ENABLE(SoCallbackAction, SoGeoElement);
  SO_ENABLE(SoGetPrimitiveCountAction, SoGeoElement);
  SO_ENABLE(SoAudioRenderAction, SoGeoElement);
}

// Doc from superclass.
void
SoGeoOrigin::doAction(SoAction * action)
{
  SoGeoElement::set(action->getState(), this);
}

// Doc from superclass.
void
SoGeoOrigin::GLRender(SoGLRenderAction * action)
{
  SoGeoOrigin::doAction((SoAction *)action);
}

// Doc from superclass.
void
SoGeoOrigin::getBoundingBox(SoGetBoundingBoxAction * action)
{
  SoGeoOrigin::doAction((SoAction *)action);
}

// Doc from superclass.
void
SoGeoOrigin::getMatrix(SoGetMatrixAction * action)
{
  SoGeoOrigin::doAction((SoAction*) action);
}

// Doc from superclass.
void
SoGeoOrigin::callback(SoCallbackAction * action)
{
  SoGeoOrigin::doAction((SoAction *)action);
}

// Doc from superclass.
void
SoGeoOrigin::pick(SoPickAction * action)
{
  SoGeoOrigin::doAction((SoAction *)action);
}

// Doc from superclass.
void
SoGeoOrigin::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  //  SoGeoOrigin::doAction((SoAction *)action);
}
