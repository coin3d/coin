/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoComputeBoundingBox SoComputeBoundingBox.h Inventor/engines/SoComputeBoundingBox.h
  \brief The SoComputeBoundingBox class is used to calculate a bounding box.
  \ingroup engines

  This engine is simply a wrapper around the SoGetBoundingBoxAction,
  for a convenient way of having automatic updating of some data in
  the scene graph which is dependent on the bounding box of some other
  part of the scene.
*/

// FIXME: wouldn't it be better design to keep the SbViewportRegion of
// the SoGetBoundingBoxAction (and other data of the action) as
// inputs, so they are written and copied automatically? 20000922 mortene.

#include <Inventor/engines/SoComputeBoundingBox.h>
#include <Inventor/lists/SoEngineOutputList.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/engines/SoSubEngineP.h>

SO_ENGINE_SOURCE(SoComputeBoundingBox);

/*!
  \var SoSFNode SoComputeBoundingBox::node

  If this input field is set and SoComputeBoundingBox::path is \c
  NULL, the resultant bounding box will be the box encompassing the
  scene graph rooted at this node pointer.
*/
/*!
  \var SoSFPath SoComputeBoundingBox::path

  If this input field is not \c NULL, the bounding box values of the
  given path will be set on the outputs.
*/
/*!
  \var SoEngineOutput SoComputeBoundingBox::min
  (SoSFVec3f) Corner coordinates of the bounding box.
*/
/*!
  \var SoEngineOutput SoComputeBoundingBox::max
  (SoSFVec3f) Corner coordinates of the bounding box.
*/
/*!
  \var SoEngineOutput SoComputeBoundingBox::boxCenter
  (SoSFVec3f) Geometric center point of the bounding box.
*/
/*!
  \var SoEngineOutput SoComputeBoundingBox::objectCenter

  (SoSFVec3f) Object center point for the bounding box. See
  SoGetBoundingBoxAction::getCenter() for an explanation for how this
  can differ from the geometric center point of the bounding box.
*/


/*!
  Default constructor. Sets up the internal SoGetBoundingBoxAction
  instance.
*/
SoComputeBoundingBox::SoComputeBoundingBox(void)
{
  SO_ENGINE_INTERNAL_CONSTRUCTOR(SoComputeBoundingBox);

  SO_ENGINE_ADD_INPUT(node,(NULL));
  SO_ENGINE_ADD_INPUT(path,(NULL));

  SO_ENGINE_ADD_OUTPUT(min, SoSFVec3f);
  SO_ENGINE_ADD_OUTPUT(max, SoSFVec3f);
  SO_ENGINE_ADD_OUTPUT(boxCenter, SoSFVec3f);
  SO_ENGINE_ADD_OUTPUT(objectCenter, SoSFVec3f);

  // Start with a default viewportregion.
  this->bboxaction = new SoGetBoundingBoxAction(SbViewportRegion());
}

// doc in parent
void
SoComputeBoundingBox::initClass(void)
{
  SO_ENGINE_INTERNAL_INIT_CLASS(SoComputeBoundingBox);
}

/*!
  Destructor. Deallocate the SoGetBoundingBoxAction instance.
*/
SoComputeBoundingBox::~SoComputeBoundingBox()
{
  delete this->bboxaction;
}

// doc in parent
void
SoComputeBoundingBox::evaluate(void)
{
  SoPath * bboxpath = this->path.getValue();
  SoNode * bboxnode = this->node.getValue();

  if (!bboxpath && !bboxnode) {
    // Nothing to see, move along.. (we "un-dirty" the fields to avoid
    // them being re-evaluated again and again while the engine inputs
    // are NULL).
    SO_ENGINE_OUTPUT(min, SoSFVec3f, setDirty(FALSE));
    SO_ENGINE_OUTPUT(max, SoSFVec3f, setDirty(FALSE));
    SO_ENGINE_OUTPUT(boxCenter, SoSFVec3f, setDirty(FALSE));
    SO_ENGINE_OUTPUT(objectCenter, SoSFVec3f, setDirty(FALSE));
    return;
  }

  if (bboxpath) this->bboxaction->apply(bboxpath);
  else this->bboxaction->apply(bboxnode);

  SbBox3f box = this->bboxaction->getBoundingBox();
  SO_ENGINE_OUTPUT(min, SoSFVec3f, setValue(box.getMin()));
  SO_ENGINE_OUTPUT(max, SoSFVec3f, setValue(box.getMax()));
  SO_ENGINE_OUTPUT(boxCenter, SoSFVec3f, setValue(box.getCenter()));

  const SbVec3f & center = this->bboxaction->getCenter();
  SO_ENGINE_OUTPUT(objectCenter, SoSFVec3f, setValue(center));
}

/*!
  Set viewport region for the SoGetBoundingBoxAction instance we're
  using for calculating bounding boxes.

  The default setting is to use an SbViewportRegion with only default
  values.
 */
void
SoComputeBoundingBox::setViewportRegion(const SbViewportRegion & vpr)
{
  this->bboxaction->setViewportRegion(vpr);
}

/*!
  Returns viewport region used by the internal SoGetBoundingBoxAction
  instance.
 */
const SbViewportRegion &
SoComputeBoundingBox::getViewportRegion(void) const
{
  return this->bboxaction->getViewportRegion();
}
