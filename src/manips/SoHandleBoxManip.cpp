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
  \class SoHandleBoxManip SoHandleBoxManip.h Inventor/manips/SoHandleBoxManip.h
  \brief The SoHandleBoxManip class wraps an SoHandleBoxDragger for manipulating a transformation.
  \ingroup manips

  <center>
  <img src="http://doc.coin3d.org/images/Coin/draggers/handlebox.png">
  </center>

  \sa SoHandleBoxDragger, SoDragger
*/
// FIXME: more class documentation? 20010909 mortene.

#include <Inventor/manips/SoHandleBoxManip.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/nodes/SoSurroundScale.h>
#include <Inventor/draggers/SoHandleBoxDragger.h>

SO_NODE_SOURCE(SoHandleBoxManip);

// Doc in super.
void
SoHandleBoxManip::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoHandleBoxManip, SO_FROM_INVENTOR_1);
}

/*!
  Constructor sets us up with an SoHandleBoxDragger for manipulating a
  transformation.
 */
SoHandleBoxManip::SoHandleBoxManip(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoHandleBoxManip);

  SoHandleBoxDragger * dragger = new SoHandleBoxDragger;
  this->setDragger(dragger);

  SoSurroundScale * ss =
    (SoSurroundScale *)dragger->getPart("surroundScale", TRUE);
  // FIXME: be robust here in case user supplied a faulty graph for
  // the dragger? Or is that taken care of by the import code in the
  // dragger? 20010909 mortene.

  ss->numNodesUpToContainer = 4;
  ss->numNodesUpToReset = 3;
}

/*!
  Protected destructor. (SoHandleBoxManip is automatically destructed
  when it's reference count goes to 0.)
*/
SoHandleBoxManip::~SoHandleBoxManip()
{
}
