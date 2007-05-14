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
    }
  \endcode
*/

// *************************************************************************

#include <Inventor/nodes/SoGeoOrigin.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/elements/SoModelMatrixElement.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

// *************************************************************************

/*!
  \var SoSFString SoGeoOrigin::geoCoords
*/

/*!
  \var SoMFString SoGeoOrigin::geoSystem
*/


// *************************************************************************

SO_NODE_SOURCE(SoGeoOrigin);

/*!
  Constructor.
*/
SoGeoOrigin::SoGeoOrigin(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoGeoOrigin);

  SO_NODE_ADD_FIELD(geoCoords, (""));
  SO_NODE_ADD_FIELD(geoSystem, (""));
  SO_NODE_ADD_FIELD(rotateYUp, (FALSE));

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
}

// Doc from superclass.
void
SoGeoOrigin::doAction(SoAction * action)
{
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
  SoGeoOrigin::doAction((SoAction *)action);
}
