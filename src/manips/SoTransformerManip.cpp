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
  \class SoTransformerManip SoTransformerManip.h Inventor/manips/SoTransformerManip.h
  \brief The SoTransformerManip wraps an SoTransformerDragger for convenience.
  \ingroup manips

  The manipulator class takes care of wrapping up the
  SoTransformerDragger in a simple and convenient API for the
  application programmer, making it automatically surround the
  geometry it influences and taking care of the book-keeping routines
  for it's interaction with the relevant fields of an SoTransformation
  node.
*/

#include <Inventor/manips/SoTransformerManip.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/nodes/SoSurroundScale.h>
#include <Inventor/draggers/SoTransformerDragger.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

SO_NODE_SOURCE(SoTransformerManip);


// doc in super
void
SoTransformerManip::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoTransformerManip, SO_FROM_INVENTOR_2_1);
}

/*!
  Default constructor. Allocates an SoTransformerDragger and an
  SoSurroundScale node to surround the geometry within our part of the
  scenegraph.
*/
SoTransformerManip::SoTransformerManip(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoTransformerManip);

  SoTransformerDragger *dragger = new SoTransformerDragger;
  this->setDragger(dragger);

  SoSurroundScale *ss = (SoSurroundScale*) dragger->getPart("surroundScale", TRUE);
  ss->numNodesUpToContainer = 4;
  ss->numNodesUpToReset = 3;
}

/*!
  Destructor.
*/
SoTransformerManip::~SoTransformerManip()
{
}

/*!
  Convenience function to use the
  SoTransformerDragger::isLocateHighlighting() method of the embedded
  dragger. See documentation of that method.
*/
SbBool
SoTransformerManip::isLocateHighlighting(void)
{
  SoDragger *dragger = this->getDragger();
  if (dragger && dragger->isOfType(SoTransformerDragger::getClassTypeId())) {
    return ((SoTransformerDragger*)dragger)->isLocateHighlighting();
  }
#if COIN_DEBUG
  SoDebugError::postWarning("SoTransformerManip::isLocateHighlighting",
                            "Not a valid dragger in manipulator");
#endif // debug
  return FALSE;
}

/*!
  Convenience function to use the
  SoTransformerDragger::setLocateHighlighting() method of the embedded
  dragger. See documentation of that method.
*/
void
SoTransformerManip::setLocateHighlighting(SbBool onoff)
{
  SoDragger *dragger = this->getDragger();
  if (dragger && dragger->isOfType(SoTransformerDragger::getClassTypeId())) {
    ((SoTransformerDragger*)dragger)->setLocateHighlighting(onoff);
  }
  else {
#if COIN_DEBUG
    SoDebugError::postWarning("SoTransformerManip::setLocateHighlighting",
                              "Not a valid dragger in manipulator");
#endif // debug
  }
}

/*!
  Convenience function to use the SoTransformerDragger::unsquishKnobs()
  method of the embedded dragger. See documentation of that method.
*/
void
SoTransformerManip::unsquishKnobs(void)
{
  SoDragger *dragger = this->getDragger();
  if (dragger && dragger->isOfType(SoTransformerDragger::getClassTypeId())) {
    ((SoTransformerDragger*)dragger)->unsquishKnobs();
  }
  else {
#if COIN_DEBUG
    SoDebugError::postWarning("SoTransformerManip::setLocateHighlighting",
                              "Not a valid dragger in manipulator");
#endif // debug
  }
}
