/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

/*!
  \class SoComputeBoundingBox SoComputeBoundingBox.h Inventor/engines/SoComputeBoundingBox.h
  \brief The SoComputeBoundingBox class is used to calculate a bounding box.
  \ingroup engines

  FIXME: doc
*/

#include <Inventor/engines/SoComputeBoundingBox.h>
#include <Inventor/lists/SoEngineOutputList.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/engines/SoSubEngineP.h>

SO_ENGINE_SOURCE(SoComputeBoundingBox);

/*!
  Default constructor.
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

SoComputeBoundingBox::~SoComputeBoundingBox()
{
  delete this->bboxaction;
}

// doc in parent
void
SoComputeBoundingBox::evaluate(void)
{

  SoPath *mypath = this->path.getValue();
  if (mypath) {
    this->bboxaction->apply(mypath);
  }
  else {
    SoNode *mynode = this->node.getValue();
    if (mynode == NULL) return;
    this->bboxaction->apply(mynode);
  }
  SbBox3f box = this->bboxaction->getBoundingBox();
  const SbVec3f &center = this->bboxaction->getCenter();

  SO_ENGINE_OUTPUT(min, SoSFVec3f, setValue(box.getMin()));
  SO_ENGINE_OUTPUT(max, SoSFVec3f, setValue(box.getMax()));
  SO_ENGINE_OUTPUT(boxCenter, SoSFVec3f, setValue(box.getCenter()));
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
