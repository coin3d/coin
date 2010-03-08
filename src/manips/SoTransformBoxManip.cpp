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
  \class SoTransformBoxManip SoTransformBoxManip.h Inventor/manips/SoTransformBoxManip.h
  \brief The SoTransformBoxManip wraps an SoTransformBoxDragger for convenience.
  \ingroup manips

  <center>
  <img src="http://doc.coin3d.org/images/Coin/draggers/transformbox.png">
  </center>

  The manipulator class takes care of wrapping up the
  SoTransformBoxDragger in a simple and convenient API for the
  application programmer, making it automatically surround the
  geometry it influences and taking care of the book-keeping routines
  for it's interaction with the relevant fields of an SoTransformation
  node.

  Here's a simple usage example, in the form of an Inventor format
  file:

  \verbatim
#Inventor V2.1 ascii

Separator {
   TransformBoxManip { }
   Group {
      Cone { }
   }
}

Translation { translation 2 4 0 }

Separator {
   TransformBoxManip { }
   Group {
      Sphere { }
   }
}
  \endverbatim
*/

#include <Inventor/manips/SoTransformBoxManip.h>

#include <Inventor/nodes/SoSurroundScale.h>
#include <Inventor/draggers/SoTransformBoxDragger.h>

#include "nodes/SoSubNodeP.h"

class SoTransformBoxManipP {
public:
};

SO_NODE_SOURCE(SoTransformBoxManip);


// doc in super
void
SoTransformBoxManip::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoTransformBoxManip, SO_FROM_INVENTOR_1);
}

/*!
  Default constructor. Allocates an SoTransformBoxDragger and an
  SoSurroundScale node to surround the geometry within our part of the
  scenegraph.
*/
SoTransformBoxManip::SoTransformBoxManip(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoTransformBoxManip);

  SoTransformBoxDragger *dragger = new SoTransformBoxDragger;
  this->setDragger(dragger);

  SoSurroundScale *ss = (SoSurroundScale*) dragger->getPart("surroundScale", TRUE);
  ss->numNodesUpToContainer = 4;
  ss->numNodesUpToReset = 3;
}

/*!
  Destructor.
*/
SoTransformBoxManip::~SoTransformBoxManip()
{
}

#endif // HAVE_MANIPULATORS
