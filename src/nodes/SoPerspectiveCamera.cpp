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
  \class SoPerspectiveCamera SoPerspectiveCamera.h Inventor/nodes/SoPerspectiveCamera.h
  \brief The SoPerspectiveCamera class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/SbName.h>
#include <Inventor/SbSphere.h>
#include <assert.h>
#include <math.h> // for atan()

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

/*!
  \var SoSFFloat SoPerspectiveCamera::heightAngle
  FIXME: write documentation for field
*/

// *************************************************************************

//$ BEGIN TEMPLATE NodeSource(SoPerspectiveCamera)
SoType SoPerspectiveCamera::classTypeId = SoType::badType();

/*!
  Returns a new instance of the SoPerspectiveCamera node class.
*/
void *
SoPerspectiveCamera::createInstance(void)
{
  return new SoPerspectiveCamera;
}

/*!
  Returns the unique type identifier for the SoPerspectiveCamera class.
*/
SoType
SoPerspectiveCamera::getClassTypeId(void)
{
  return SoPerspectiveCamera::classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoPerspectiveCamera::getTypeId(void) const
{
  return SoPerspectiveCamera::classTypeId;
}
//$ END TEMPLATE NodeSource

/*!
  Constructor.
*/
SoPerspectiveCamera::SoPerspectiveCamera()
{
//$ BEGIN TEMPLATE NodeConstructor(SoPerspectiveCamera)
  // Make sure the class has been initialized.
  assert(SoPerspectiveCamera::classTypeId != SoType::badType());
//$ END TEMPLATE NodeConstructor

  SO_NODE_ADD_FIELD(heightAngle, (SB_PI/4.0f));  // 45 degrees
}

/*!
  Destructor.
*/
SoPerspectiveCamera::~SoPerspectiveCamera()
{
}

/*!
  Does initialization common for all objects of the
  SoPerspectiveCamera class. This includes setting up the
  type system, among other things.
*/
void
SoPerspectiveCamera::initClass(void)
{
//$ BEGIN TEMPLATE InitNodeSource(PerspectiveCamera)
  // Make sure we only initialize once.
  assert(SoPerspectiveCamera::classTypeId == SoType::badType());
  // Make sure superclass get initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoPerspectiveCamera::classTypeId =
    SoType::createType(inherited::getClassTypeId(), "PerspectiveCamera",
                       &SoPerspectiveCamera::createInstance,
                       SoNode::nextActionMethodIndex++);
//$ END TEMPLATE InitNodeSource
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoPerspectiveCamera::cleanClass(void)
{
}

/*!
  FIXME: write function documentation
*/
void 
SoPerspectiveCamera::scaleHeight(float scaleFactor)
{
  float tmp = heightAngle.getValue();
  heightAngle.setValue(tmp * scaleFactor);
}

/*!
  FIXME: write function documentation
*/
SbViewVolume 
SoPerspectiveCamera::getViewVolume(float useAspectRatio) const
{
  if (useAspectRatio == 0.0f) useAspectRatio = aspectRatio.getValue();

#if 0 // debug
  // there's still a bug here somewhere. 19981029 mortene.
  SoDebugError::postInfo("SoPerspectiveCamera::getViewVolume",
			 "useAspectRatio: %f",
			 useAspectRatio);
#endif // 0  

  SbViewVolume volume;
  volume.perspective(heightAngle.getValue(), useAspectRatio, 
                     nearDistance.getValue(), farDistance.getValue());
  volume.rotateCamera(orientation.getValue());
  volume.translateCamera(position.getValue()); 
  return volume;
}

/*!
  FIXME: write function documentation
*/
void 
SoPerspectiveCamera::viewBoundingBox(const SbBox3f & box, float aspect,
				     float slack)
{
  // First, we want to move the camera in such a way that it is
  // pointing straight at the center of the scene bounding box -- but
  // without modifiying the rotation value (so we can't use
  // SoCamera::pointAt()).
  SbVec3f cameradirection;
  this->orientation.getValue().multVec(SbVec3f(0, 0, -1), cameradirection);
  this->position.setValue(box.getCenter() + -cameradirection);


  // Get the radius of the bounding sphere.
  SbSphere bs;
  bs.circumscribe(box);
  float radius = bs.getRadius();
  

  // Make sure that everything will still be inside the viewing volume
  // even if the aspect ratio "favorizes" width over height.
  float aspectradius = radius / (aspect < 1.0f ? aspect : 1.0f);

  // Move the camera to the edge of the bounding sphere, while still
  // pointing at the scene.
  SbVec3f direction = this->position.getValue() - box.getCenter();
  direction.normalize();
  float movelength =
    aspectradius + (aspectradius/atan(this->heightAngle.getValue()));
  this->position.setValue(box.getCenter() + direction * movelength);


  // Set up the clipping planes according to the slack value (a value
  // of 1.0 will yield clipping planes that are tangent to the
  // bounding sphere of the scene).
  float distance_to_midpoint =
    (this->position.getValue() - box.getCenter()).length();
  this->nearDistance = distance_to_midpoint - radius * slack;
  this->farDistance = distance_to_midpoint + radius * slack;


  // The focal distance is simply the distance from the camera to the
  // scene midpoint. This field is not used in rendering, its just
  // provided to make it easier for the user to do calculations based
  // on the distance between the camera and the scene.
  this->focalDistance = distance_to_midpoint;
}
