/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

/*!
  \class SoHandleBoxManip SoHandleBoxManip.h Inventor/manips/SoHandleBoxManip.h
  \brief The SoHandleBoxManip class wraps an SoHandleBoxDragger for manipulating a transformation.
  \ingroup manips
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
  SO_NODE_INTERNAL_INIT_CLASS(SoHandleBoxManip);
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
