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
  \class SoTabBoxManip SoTabBoxManip.h Inventor/manips/SoTabBoxManip.h
  \brief The SoTabBoxManip class wraps an SoTabBoxDragger.
  \ingroup manips

  The SoTabBoxManip provides a convenient mechanism for the
  application programmer for setting up an SoTabBoxDragger in the
  scene connected to the relevant fields of an SoTransform node.

  The interaction from the end-user with the manipulator will then
  automatically influence the transformation matrix for the geometry
  following it in the scenegraph.
*/

#include <Inventor/manips/SoTabBoxManip.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/nodes/SoSurroundScale.h>
#include <Inventor/draggers/SoTabBoxDragger.h>

SO_NODE_SOURCE(SoTabBoxManip);

// Doc in superclass.
void
SoTabBoxManip::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoTabBoxManip, SO_FROM_INVENTOR_2_1);
}

/*!
  Constructor sets us up with an SoTabBoxDragger for manipulating a
  transformation.
*/
SoTabBoxManip::SoTabBoxManip(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoTabBoxManip);

  SoTabBoxDragger *dragger = new SoTabBoxDragger;
  this->setDragger(dragger);

  SoSurroundScale * ss =
    (SoSurroundScale *)dragger->getPart("surroundScale", TRUE);
  ss->numNodesUpToContainer = 4;
  ss->numNodesUpToReset = 3;
}


/*!
  Protected destructor. (SoHandleBoxManip is automatically destructed
  when it's reference count goes to 0.)
 */
SoTabBoxManip::~SoTabBoxManip()
{
}
