/**************************************************************************\
 * 
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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
  \class SoRayPickAction Inventor/include/SoRayPickAction.h
  \brief The SoRayPickAction class is for picking objects in a scene through
  sending rays into it.
*/

#include <Inventor/SbName.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/lists/SoEnabledElementsList.h>
#include <Inventor/lists/SoPickedPointList.h>

// *************************************************************************

//$ BEGIN TEMPLATE ActionSource(SoRayPickAction)

SoType SoRayPickAction::classTypeId = SoType::badType();

/*!
  Returns the unique type identifier for the classname class.
*/
SoType
SoRayPickAction::getClassTypeId(void)
{
  return classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoRayPickAction::getTypeId(void) const
{
  return classTypeId;
}

// static variables
SoEnabledElementsList * SoRayPickAction::enabledElements;
SoActionMethodList * SoRayPickAction::methods;

/*!
  \fn SoRayPickAction::enabledElements
  FIXME: write doc.
*/

/*!
  \fn SoRayPickAction::methods
  FIXME: write doc.
*/

/*!
  This method returns the list of enabled elements for the given action class.
*/
const SoEnabledElementsList &
SoRayPickAction::getEnabledElements(void) const
{
  return *enabledElements;
}

/*!
  This method adds a method to be perfomed by the action class on the given
  node type.
*/
void 
SoRayPickAction::addMethod(const SoType type, SoActionMethod method)
{
  methods->addMethod(type, method);
}

/*!
  This method enables an element in the state stack for the action class.
*/
void 
SoRayPickAction::enableElement(const SoType type, const int stackIndex)
{
  enabledElements->enable(type, stackIndex);
}
//$ END TEMPLATE ActionSource

// *************************************************************************

/*!
  This static method is for initializing static data for the
  SoRayPickAction class.
*/

void
SoRayPickAction::initClass(void)
{
//$ BEGIN TEMPLATE InitActionSource(SoRayPickAction)
  assert(SoRayPickAction::getClassTypeId() == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoRayPickAction::classTypeId = 
      SoType::createType(inherited::getClassTypeId(),
                         "SoRayPickAction");
  enabledElements = new SoEnabledElementsList(inherited::enabledElements);
  methods = new SoActionMethodList(inherited::methods);
//$ END TEMPLATE InitActionSource
}

/*!
  This static method is for cleaning up static data for the SoRayPickAction
  class.
*/

void
SoRayPickAction::cleanClass(void)
{
}

// *************************************************************************

/*!
  A constructor.
*/

SoRayPickAction::SoRayPickAction(const SbViewportRegion & viewportRegion)
  : inherited(viewportRegion)
{
  assert(0 && "FIXME: not implemented");
}

/*!
  The destructor.
*/

SoRayPickAction::~SoRayPickAction(void)
{
}

/*!
  This method sets the viewport-space point which the ray is sent through.
*/

void
SoRayPickAction::setPoint(const SbVec2s & /* viewportPoint */)
{
  assert(0 && "FIXME: not implemented");
}

/*!
  This method sets the viewport-space point which the ray is sent through.
  The coordinate is normalized, ranging from (0,0) to (1,1).
*/

void
SoRayPickAction::setNormalizedPoint(const SbVec2f & /* normPoint */)
{
  assert(0 && "FIXME: not implemented");
}

/*!
  This method sets the radius of the picking ray.
*/

void
SoRayPickAction::setRadius(float /* radiusInPixels */)
{
  assert(0 && "FIXME: not implemented");
}

/*!
  This method sets the ray in world-space coordinates.
*/

void
SoRayPickAction::setRay(const SbVec3f & /* start */,
			const SbVec3f & /* direction */,
			float /* nearDistance */, float /* farDistance */)
{
  assert(0 && "FIXME: not implemented");
}

/*!
  This method lets you decide wether only the closest object or all the objects
  the ray intersects with should be picked.
*/

void
SoRayPickAction::setPickAll(SbBool /* flag */)
{
  assert(0 && "FIXME: not implemented");
}

/*!
  This method returns wether only the closest object or all the objects the
  ray intersects with is picked.
*/

SbBool
SoRayPickAction::isPickAll(void) const
{
  assert(0 && "FIXME: not implemented");
  return FALSE;
}

/*!
  This method returns a list of the picked points.
*/

const SoPickedPointList &
SoRayPickAction::getPickedPointList(void) const
{
  assert(0 && "FIXME: not implemented");
  static SoPickedPointList pl;
  return pl;
}

/*!
  This method returns the picked point with index \a index in the list of
  picked points.  \a index starts at index 0.
*/

SoPickedPoint *
SoRayPickAction::getPickedPoint(int /* index */) const
{
  assert(0 && "FIXME: not implemented");
  return NULL;
}

