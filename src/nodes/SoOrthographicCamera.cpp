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
 *  http://www.sim.no/ sales@sim.no Voice: +47 73540378 Fax: +47 73943861
 *
\**************************************************************************/

/*!
  \class SoOrthographicCamera SoOrthographicCamera.h Inventor/nodes/SoOrthographicCamera.h
  \brief The SoOrthographicCamera class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/SbName.h>
#include <Inventor/SbSphere.h>

#include <math.h>

/*!
  \var SoSFFloat SoOrthographicCamera::height
  FIXME: write documentation for field
*/

// *************************************************************************

//$ BEGIN TEMPLATE NodeSource(SoOrthographicCamera)
SoType SoOrthographicCamera::classTypeId = SoType::badType();

/*!
  Returns a new instance of the SoOrthographicCamera node class.
*/
void *
SoOrthographicCamera::createInstance(void)
{
  return new SoOrthographicCamera;
}

/*!
  Returns the unique type identifier for the SoOrthographicCamera class.
*/
SoType
SoOrthographicCamera::getClassTypeId(void)
{
  return SoOrthographicCamera::classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoOrthographicCamera::getTypeId(void) const
{
  return SoOrthographicCamera::classTypeId;
}
//$ END TEMPLATE NodeSource

/*!
  Constructor.
*/
SoOrthographicCamera::SoOrthographicCamera()
{
//$ BEGIN TEMPLATE NodeConstructor(SoOrthographicCamera)
  // Make sure the class has been initialized.
  assert(SoOrthographicCamera::classTypeId != SoType::badType());
//$ END TEMPLATE NodeConstructor

  SO_NODE_ADD_FIELD(height, (2.0f));
}

/*!
  Destructor.
*/
SoOrthographicCamera::~SoOrthographicCamera()
{
}

/*!
  Does initialization common for all objects of the
  SoOrthographicCamera class. This includes setting up the
  type system, among other things.
*/
void
SoOrthographicCamera::initClass(void)
{
//$ BEGIN TEMPLATE InitNodeSource(OrthographicCamera)
  // Make sure we only initialize once.
  assert(SoOrthographicCamera::classTypeId == SoType::badType());
  // Make sure superclass get initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoOrthographicCamera::classTypeId =
    SoType::createType(inherited::getClassTypeId(), "OrthographicCamera",
                       &SoOrthographicCamera::createInstance,
                       SoNode::nextActionMethodIndex++);
//$ END TEMPLATE InitNodeSource
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoOrthographicCamera::cleanClass(void)
{
}

/*!
  FIXME: write function documentation
*/
void 
SoOrthographicCamera::scaleHeight(float scaleFactor)
{
  height.setValue(height.getValue() * scaleFactor);
}

/*!
  FIXME: write function documentation
*/
SbViewVolume 
SoOrthographicCamera::getViewVolume(float useAspectRatio) const
{
  SbViewVolume volume;

  if (useAspectRatio == 0.0f) useAspectRatio = aspectRatio.getValue();

  // FIXME: this is pro'ly not correct (?). 19981024 mortene.
  float width = height.getValue() * useAspectRatio;
  volume.ortho(-width/2.0f, width/2.0f,
	       -height.getValue()/2.0f, height.getValue()/2.0f,
	       nearDistance.getValue(), farDistance.getValue());

  volume.rotateCamera(orientation.getValue());
  volume.translateCamera(position.getValue()); 
  return volume;
}

/*!
  FIXME: write function documentation
*/
void 
SoOrthographicCamera::viewBoundingBox(const SbBox3f & box,
				      float aspect, float slack)
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
  if (aspect < 1.0f)
    this->height = 2 * radius / aspect;
  else
    this->height = 2 * radius;


  // Move the camera to the edge of the bounding sphere, while still
  // pointing at the scene.
  SbVec3f direction = this->position.getValue() - box.getCenter();
  direction.normalize();
  this->position.setValue(box.getCenter() + direction * radius);


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
