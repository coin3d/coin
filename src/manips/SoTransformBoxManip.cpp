/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
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
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

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
*/

#include <Inventor/manips/SoTransformBoxManip.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/nodes/SoSurroundScale.h>
#include <Inventor/draggers/SoTransformBoxDragger.h>

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
