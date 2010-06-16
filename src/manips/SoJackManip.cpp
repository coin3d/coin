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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#ifdef HAVE_MANIPULATORS

/*!
  \class SoJackManip SoJackManip.h Inventor/manips/SoJackManip.h
  \brief The SoJackManip wraps an SoJackDragger for convenience.
  \ingroup manips

  <center>
  <img src="http://doc.coin3d.org/images/Coin/draggers/jack.png">
  </center>

  The manipulator class takes care of wrapping up the SoJackDragger in
  a simple and convenient API for the application programmer, making
  it automatically surround the geometry it influences and taking care
  of the book-keeping routines for it's interaction with the relevant
  fields of an SoTransformation node.
*/

#include <Inventor/manips/SoJackManip.h>

#include <Inventor/nodes/SoSurroundScale.h>
#include <Inventor/draggers/SoJackDragger.h>

#include "nodes/SoSubNodeP.h"

class SoJackManipP {
public:
};

SO_NODE_SOURCE(SoJackManip);


// doc in super
void
SoJackManip::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoJackManip, SO_FROM_INVENTOR_1);
}

/*!
  Default constructor. Allocates an SoCenterballDragger and an
  SoSurroundScale node to surround the geometry within our part of the
  scenegraph.
*/
SoJackManip::SoJackManip(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoJackManip);

  SoJackDragger *dragger = new SoJackDragger;
  this->setDragger(dragger);

  SoSurroundScale *ss = (SoSurroundScale*) dragger->getPart("surroundScale", TRUE);
  assert(ss);
  ss->numNodesUpToContainer = 4;
  ss->numNodesUpToReset = 3;
}


/*!
  Destructor.
*/
SoJackManip::~SoJackManip()
{
}

#endif // HAVE_MANIPULATORS
