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
  \class SoOrthographicCamera SoOrthographicCamera.h Inventor/nodes/SoOrthographicCamera.h
  \brief The SoOrthographicCamera class defines a camera node with orthographic rendering.
  \ingroup nodes

  Orthographic rendering will not give a particularly realistic
  impression of the scene, but is for various reasons widely used in
  applications for Computer Aided Design.
*/

#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/SbSphere.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

/*!
  \var SoSFFloat SoOrthographicCamera::height
  Height of viewport in world-space scale. Defaults to 2.0 units.
*/

// *************************************************************************

SO_NODE_SOURCE(SoOrthographicCamera);

/*!
  Constructor.
*/
SoOrthographicCamera::SoOrthographicCamera()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoOrthographicCamera);

  SO_NODE_ADD_FIELD(height, (2.0f));
}

/*!
  Destructor.
*/
SoOrthographicCamera::~SoOrthographicCamera()
{
}

// Doc in superclass.
void
SoOrthographicCamera::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoOrthographicCamera, SO_FROM_INVENTOR_1|SoNode::VRML1);
}

/*!
  Scale SoOrthographicCamera::height by multiplying with \a
  scalefactor.
*/
void
SoOrthographicCamera::scaleHeight(float scalefactor)
{
  this->height.setValue(this->height.getValue() * scalefactor);
}

// Doc in superclass.
SbViewVolume
SoOrthographicCamera::getViewVolume(float useaspectratio) const
{
  SbViewVolume volume;

  if (useaspectratio == 0.0f) useaspectratio = this->aspectRatio.getValue();
  float halfheight = this->height.getValue() * 0.5f;
  float halfwidth = halfheight * useaspectratio;
  volume.ortho(-halfwidth, halfwidth,
               -halfheight, halfheight,
               this->nearDistance.getValue(), this->farDistance.getValue());
  
  volume.rotateCamera(this->orientation.getValue());
  volume.translateCamera(this->position.getValue());
  return volume;
}

// Doc in superclass.
void
SoOrthographicCamera::viewBoundingBox(const SbBox3f & box,
                                      float aspect, float slack)
{
#if COIN_DEBUG
  if (box.isEmpty()) {
    SoDebugError::postWarning("SoOrthographicCamera::viewBoundingBox",
                              "bounding box empty");
    return;
  }
#endif // COIN_DEBUG

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
