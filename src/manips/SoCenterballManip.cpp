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
  \class SoCenterballManip SoCenterballManip.h Inventor/manips/SoCenterballManip.h
  \brief The SoCenterballManip wraps an SoCenterballDragger for convenience.
  \ingroup manips

  <center>
  <img src="http://doc.coin3d.org/images/Coin/draggers/centerball.png">
  </center>

  The manipulator class takes care of wrapping up the
  SoCenterballDragger in a simple and convenient API for the
  application programmer, making it automatically surround the
  geometry it influences and taking care of the book-keeping routines
  for it's interaction with the relevant fields of an SoTransformation
  node.
*/

#include <Inventor/manips/SoCenterballManip.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/draggers/SoCenterballDragger.h>
#include <Inventor/nodes/SoSurroundScale.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <coindefs.h>

SO_NODE_SOURCE(SoCenterballManip);


// doc in super
void
SoCenterballManip::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoCenterballManip, SO_FROM_INVENTOR_1);
}

/*!
  Default constructor. Allocates an SoCenterballDragger and an
  SoSurroundScale node to surround the geometry within our part of the
  scenegraph.
*/
SoCenterballManip::SoCenterballManip(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoCenterballManip);

  SoCenterballDragger * dragger = new SoCenterballDragger;
  this->setDragger(dragger);

  SoSurroundScale * ss = (SoSurroundScale*) dragger->getPart("surroundScale", TRUE);
  ss->numNodesUpToContainer = 4;
  ss->numNodesUpToReset = 3;

  // we don't need to use specific callbacks for this manipulator.
  // it's ok to use the functionality provided by the superclass:
  // SoTransformManip.  we'll keep the callback to be API-compatible,
  // and to make it possible to add new code if we at some point find
  // find that it's still needed; without breaking the ABI.
}


/*!
  Destructor.
*/
SoCenterballManip::~SoCenterballManip()
{
}

// Doc in superclass.
void
SoCenterballManip::setDragger(SoDragger * newDragger)
{
  inherited::setDragger(newDragger);
}

// Doc in superclass.
void
SoCenterballManip::fieldSensorCB(void * f, SoSensor * s)
{
  // this function should never be called, but just-in-case
  // we call the superclass method.
  inherited::fieldSensorCB(f, s);
}

// Doc in superclass.
void
SoCenterballManip::valueChangedCB(void * f, SoDragger * d)
{
  // this function should never be called, but just-in-case
  // we call the superclass method.
  inherited::valueChangedCB(f, d);
}
