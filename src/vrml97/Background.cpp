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
  \class SoVRMLBackground SoVRMLBackground.h Inventor/VRMLnodes/SoVRMLBackground.h
  \brief The SoVRMLBackground class is used for specifying a viewer panorama.
  \ingroup VRMLnodes

  \WEB3DCOPYRIGHT

  \verbatim
  Background {
    eventIn      SFBool   set_bind
    exposedField MFFloat  groundAngle  []         # [0,pi/2]
    exposedField MFColor  groundColor  []         # [0,1]
    exposedField MFString backUrl      []
    exposedField MFString bottomUrl    []
    exposedField MFString frontUrl     []
    exposedField MFString leftUrl      []
    exposedField MFString rightUrl     []
    exposedField MFString topUrl       []
    exposedField MFFloat  skyAngle     []         # [0,pi]
    exposedField MFColor  skyColor     0 0 0      # [0,1]
    eventOut     SFBool   isBound
  }
  \endverbatim

  The Background node is used to specify a colour backdrop that
  simulates ground and sky, as well as a background texture, or
  panorama, that is placed behind all geometry in the scene and in
  front of the ground and sky. Background nodes are specified in the
  local coordinate system and are affected by the accumulated rotation
  of their ancestors as described below.  Background nodes are
  bindable nodes as described in 4.6.10, Bindable children nodes
  FIXME
  (<http://www.web3d.org/technicalinfo/specifications/vrml97/part1/concepts.html#4.6.10>).
  There exists a Background stack, in which the top-most Background on
  the stack is the currently active Background. To move a Background
  to the top of the stack, a TRUE value is sent to the set_bind
  eventIn.  Once active, the Background is then bound to the browsers
  view. A FALSE value sent to set_bind removes the Background from the
  stack and unbinds it from the browser's view. More detail on the
  bind stack is described in 4.6.10, Bindable children nodes
  (<http://www.web3d.org/technicalinfo/specifications/vrml97/part1/concepts.html#4.6.10>).

  The backdrop is conceptually a partial sphere (the ground) enclosed
  inside of a full sphere (the sky) in the local coordinate system
  with the viewer placed at the centre of the spheres. Both spheres
  have infinite radius and each is painted with concentric circles of
  interpolated colour perpendicular to the local Y-axis of the
  sphere. The Background node is subject to the accumulated rotations
  of its ancestors' transformations. Scaling and translation
  transformations are ignored. The sky sphere is always slightly
  farther away from the viewer than the ground partial sphere causing
  the ground to appear in front of the sky where they overlap.  The
  skyColor field specifies the colour of the sky at various angles on
  the sky sphere. The first value of the skyColor field specifies the
  colour of the sky at 0.0 radians representing the zenith (i.e.,
  straight up from the viewer). The skyAngle field specifies the
  angles from the zenith in which concentric circles of colour
  appear. The zenith of the sphere is implicitly defined to be 0.0
  radians, the natural horizon is at pi/2 radians, and the nadir
  (i.e., straight down from the viewer) is at pi radians. skyAngle is
  restricted to non-decreasing values in the range [0.0, pi]. There
  shall be one more skyColor value than there are skyAngle values. The
  first colour value is the colour at the zenith, which is not
  specified in the skyAngle field.  If the last skyAngle is less than
  pi, then the colour band between the last skyAngle and the nadir is
  clamped to the last skyColor.  The sky colour is linearly
  interpolated between the specified skyColor values.

  The groundColor field specifies the colour of the ground at the
  various angles on the ground partial sphere. The first value of the
  groundColor field specifies the colour of the ground at 0.0 radians
  representing the nadir (i.e., straight down from the user). The
  groundAngle field specifies the angles from the nadir that the
  concentric circles of colour appear. The nadir of the sphere is
  implicitly defined at 0.0 radians. groundAngle is restricted to
  non-decreasing values in the range [0.0, pi/2]. There shall be one
  more groundColor value than there are groundAngle values. The first
  colour value is for the nadir which is not specified in the
  groundAngle field. If the last groundAngle is less than pi/2, the
  region between the last groundAngle and the equator is
  non-existant. The ground colour is linearly interpolated between the
  specified groundColor values.

  The backUrl, bottomUrl, frontUrl, leftUrl, rightUrl, and topUrl
  fields specify a set of images that define a background panorama
  between the ground/sky backdrop and the scene's geometry.  The
  panorama consists of six images, each of which is mapped onto a face
  of an infinitely large cube contained within the backdrop spheres
  and centred in the local coordinate system. The images are applied
  individually to each face of the cube. On the front, back, right,
  and left faces of the cube, when viewed from the origin looking down
  the negative Z-axis with the Y-axis as the view up direction, each
  image is mapped onto the corresponding face with the same
  orientation as if the image were displayed normally in 2D (backUrl
  to back face, frontUrl to front face, leftUrl to left face, and
  rightUrl to right face). On the top face of the cube, when viewed
  from the origin looking along the +Y-axis with the +Z-axis as the
  view up direction, the topUrl image is mapped onto the face with the
  same orientation as if the image were displayed normally in 2D. On
  the bottom face of the box, when viewed from the origin along the
  negative Y-axis with the negative Z-axis as the view up direction,
  the bottomUrl image is mapped onto the face with the same
  orientation as if the image were displayed normally in 2D.

  <center>
  <img src="http://www.web3d.org/technicalinfo/specifications/vrml97/Images/background.gif">
  Figure 6.1
  </center>

  Figure 6.1 illustrates the Background node backdrop and background
  textures.  Alpha values in the panorama images (i.e., two or four
  component images) specify that the panorama is semi-transparent or
  transparent in regions, allowing the groundColor and skyColor to be
  visible.  See 4.6.11, Texture maps, for a general description of
  texture maps.  Often, the bottomUrl and topUrl images will not be
  specified, to allow sky and ground to show. The other four images
  may depict surrounding mountains or other distant scenery. Browsers
  shall support the JPEG (see 2.[JPEG]) and PNG (see 2.[PNG]) image
  file formats, and in addition, may support any other image format
  (e.g., CGM) that can be rendered into a 2D image. Support for the
  GIF (see E.[GIF]) format is recommended (including transparency).
  More detail on the url fields can be found in 4.5, VRML and the
  World Wide Web
  (<http://www.web3d.org/technicalinfo/specifications/vrml97/part1/concepts.html#4.5>).


*/

/*!
  SoMFFloat SoVRMLBackground::groundAngle

  The ground angles where different colors should be used.
*/

/*!
  SoMFColor SoVRMLBackground::groundColor

  The color for each groundAngle.
*/

/*!
  SoMFFloat SoVRMLBackground::skyAngle

  The sky angles where different colors should be used.
*/

/*!
  SoMFColor SoVRMLBackground::skyColor

  The color for each skyAngle.
*/

/*!
  SoMFString SoVRMLBackground::backUrl

  URL for the background image.
*/

/*!
  SoMFString SoVRMLBackground::bottomUrl

  URL for the bottom image.
*/

/*!
  SoMFString SoVRMLBackground::frontUrl

  URL for the front image.
*/

/*!
  SoMFString SoVRMLBackground::leftUrl

  URL for the left image.
*/

/*!
  SoMFString SoVRMLBackground::rightUrl

  URL for the right image.
*/

/*!
  SoMFString SoVRMLBackground::topUrl

  URL for the top image.
*/

/*!
  SoSFBool SoVRMLBackground::set_bind
  An eventIn which is triggered when the node is bound.
*/

/*!
  SoSFBool SoVRMLBackground::isBound
  An eventOut that is sent after the node has been bound/unbound.
*/


#include <Inventor/VRMLnodes/SoVRMLBackground.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>

SO_NODE_SOURCE(SoVRMLBackground);

// Doc in parent
void
SoVRMLBackground::initClass(void) // static
{
  SO_NODE_INTERNAL_INIT_CLASS(SoVRMLBackground, SO_VRML97_NODE_TYPE);
}

/*!
  Constructor.
*/
SoVRMLBackground::SoVRMLBackground(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoVRMLBackground);

  SO_VRMLNODE_ADD_EMPTY_EXPOSED_MFIELD(groundColor);
  SO_VRMLNODE_ADD_EMPTY_EXPOSED_MFIELD(skyColor);
  SO_VRMLNODE_ADD_EMPTY_EXPOSED_MFIELD(groundAngle);
  SO_VRMLNODE_ADD_EMPTY_EXPOSED_MFIELD(skyAngle);
  SO_VRMLNODE_ADD_EMPTY_EXPOSED_MFIELD(backUrl);
  SO_VRMLNODE_ADD_EMPTY_EXPOSED_MFIELD(bottomUrl);
  SO_VRMLNODE_ADD_EMPTY_EXPOSED_MFIELD(frontUrl);
  SO_VRMLNODE_ADD_EMPTY_EXPOSED_MFIELD(leftUrl);
  SO_VRMLNODE_ADD_EMPTY_EXPOSED_MFIELD(rightUrl);
  SO_VRMLNODE_ADD_EMPTY_EXPOSED_MFIELD(topUrl);

  SO_VRMLNODE_ADD_EVENT_IN(set_bind);
  SO_VRMLNODE_ADD_EVENT_OUT(isBound);
}

/*!
  Destructor.
*/
SoVRMLBackground::~SoVRMLBackground()
{
}

// Doc in parent
void
SoVRMLBackground::GLRender(SoGLRenderAction * action)
{
}
