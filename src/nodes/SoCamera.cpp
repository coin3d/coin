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
  \class SoCamera SoCamera.h Inventor/nodes/SoCamera.h
  \brief The SoCamera class is the abstract base class for camera definition nodes.
  \ingroup nodes

  To be able to view a scene, one needs to have a camera in the scene
  graph. A camera node will set up the projection and viewing matrices
  for rendering of the geometry in the scene.

  This node just defines the abstract interface by collecting common
  fields all camera type nodes needs. Use of the non-abstract
  subclasses within our scene graph.

  Note that the viewer components of the GUI glue libraries of Coin
  (SoXt, SoQt, SoWin, etc) will automatically insert a camera into a
  scene graph is none has been defined.
*/

#include <Inventor/nodes/SoCamera.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/elements/SoCacheElement.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/actions/SoHandleEventAction.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/elements/SoFocalDistanceElement.h>
#include <Inventor/elements/SoGLProjectionMatrixElement.h>
#include <Inventor/elements/SoGLViewingMatrixElement.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/elements/SoViewVolumeElement.h>
#include <Inventor/elements/SoViewportRegionElement.h>
#include <Inventor/elements/SoDrawStyleElement.h>
#include <Inventor/elements/SoGLLineWidthElement.h>
#include <Inventor/elements/SoGLShapeHintsElement.h>
#include <Inventor/elements/SoCullElement.h>
#include <Inventor/elements/SoGLRenderPassElement.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/SbColor4f.h>
#include <Inventor/C/glue/gl.h>
#include <Inventor/C/glue/glp.h>
#include <float.h> // for FLT_EPSILON



#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG


/*!
  \enum SoCamera::ViewportMapping

  Enumerates the available possibilities for how the render frame
  should map the viewport.
*/

/*!
  \var SoSFEnum SoCamera::viewportMapping

  Set up how the render frame should map the viewport. The default is
  SoCamera::ADJUST_CAMERA.
*/
/*!
  \var SoSFVec3f SoCamera::position

  Camera position. Defaults to <0,0,1>.
*/
/*!
  \var SoSFRotation SoCamera::orientation

  Camera orientation specified as a rotation value from the default
  orientation where the camera is pointing along the negative z-axis,
  with "up" along the positive y-axis.
*/
/*!
  \var SoSFFloat SoCamera::aspectRatio

  Aspect ratio for the camera (i.e. width / height). Defaults to 1.0.
*/
/*!
  \var SoSFFloat SoCamera::nearDistance

  Distance from camera position to the near clipping plane in the
  camera's view volume.

  Default value is 1.0.  Value must be larger than 0.0, or it will not
  be possible to construct a valid viewing volume (for perspective
  rendering, at least).

  If you use one of the viewer components from the So[Xt|Qt|Win|Gtk]
  GUI libraries provided by Systems in Motion, they will automatically
  update this value for the scene camera according to the scene
  bounding box. Ditto for the far clipping plane.

  \sa SoCamera::farDistance
*/
/*!
  \var SoSFFloat SoCamera::farDistance

  Distance from camera position to the far clipping plane in the
  camera's view volume.

  Default value is 10.0.  Must be larger than the
  SoCamera::nearDistance value, or it will not be possible to
  construct a valid viewing volume.

  Note that the range [nearDistance, farDistance] decides the dynamic
  range of the Z-buffer in the underlying polygon-rendering
  rasterizer.  What this means is that if the near and far clipping
  planes of the camera are wide apart, the possibility of visual
  artifacts will increase. The artifacts will manifest themselves in
  the form of flickering of primitives close in depth.

  It is therefore a good idea to keep the near and far clipping planes
  of your camera(s) as closely fitted around the geometry of the
  scenegraph as possible.

  \sa SoCamera::nearDistance, SoPolygonOffset
*/
/*!
  \var SoSFFloat SoCamera::focalDistance

  Distance from camera position to center of scene.
*/


/*!
  \fn void SoCamera::scaleHeight(float scalefactor)

  Sets a \a scalefactor for the height of the camera viewport. What
  "viewport height" means exactly in this context depends on the
  camera model. See documentation in subclasses.
*/

/*!
  \fn SbViewVolume SoCamera::getViewVolume(float useaspectratio = 0.0f) const

  Returns total view volume covered by the camera under the current
  settings.

  This view volume is not adjusted to account for viewport mapping.
  If you want the same view volume as the one used during rendering,
  you should do something like this:

  \verbatim

  SbViewVolume vv;
  float aspectratio = myviewport.getViewportAspectRatio();

  switch (camera->viewportMapping.getValue()) {
  case SoCamera::CROP_VIEWPORT_FILL_FRAME:
  case SoCamera::CROP_VIEWPORT_LINE_FRAME:
  case SoCamera::CROP_VIEWPORT_NO_FRAME:
    vv = camera->getViewVolume(0.0f);
    break;
  case SoCamera::ADJUST_CAMERA:
    vv = camera->getViewVolume(aspectratio);
    if (aspectratio < 1.0f) vv.scale(1.0f / aspectratio);
    break;
  case SoCamera::LEAVE_ALONE:
    vv = camera->getViewVolume(0.0f);
    break;
  default:
    assert(0 && "unknown viewport mapping");
    break;
  }

  \endverbatim

  Also, for the CROPPED viewport mappings, the viewport might
  be changed if the viewport aspect ratio is not equal to the
  camera aspect ratio. See SoCamera::getView() to see how this
  is done.  
*/

/*!
  \fn void SoCamera::viewBoundingBox(const SbBox3f & box, float aspect, float slack)

  Convenience method for setting up the camera definition to cover the
  given bounding \a box with the given \a aspect ratio. Multiplies the
  exact dimensions with a \a slack factor to have some space between
  the rendered model and the borders of the rendering area.

  If you define your own camera node class, be aware that this method
  should \e not set the orientation field of the camera, only the
  position, focal distance and near and far clipping planes.
*/

/*!
  \enum SoCamera::StereoMode
  Enumerates the possible stereo modes.
*/

/*!
  \var SoCamera::MONOSCOPIC,
  No stereo.
*/

/*!
  \var SoCamera::LEFT_VIEW,
  Left view.
*/

/*!
  \var SoCamera::RIGHT_VIEW
  Right view.
*/


SO_NODE_ABSTRACT_SOURCE(SoCamera);

/*!
  Constructor.
*/
SoCamera::SoCamera()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoCamera);

  SO_NODE_ADD_FIELD(viewportMapping, (ADJUST_CAMERA));
  SO_NODE_ADD_FIELD(position, (0.0f, 0.0f, 1.0f));
  SO_NODE_ADD_FIELD(orientation, (SbRotation(SbVec3f(0.0f, 0.0f, 1.0f), 0.0f)));
  SO_NODE_ADD_FIELD(nearDistance, (1.0f));
  SO_NODE_ADD_FIELD(farDistance, (10.0f));
  SO_NODE_ADD_FIELD(aspectRatio, (1.0f));
  SO_NODE_ADD_FIELD(focalDistance, (5.0f));

  SO_NODE_DEFINE_ENUM_VALUE(ViewportMapping, CROP_VIEWPORT_FILL_FRAME);
  SO_NODE_DEFINE_ENUM_VALUE(ViewportMapping, CROP_VIEWPORT_LINE_FRAME);
  SO_NODE_DEFINE_ENUM_VALUE(ViewportMapping, CROP_VIEWPORT_NO_FRAME);
  SO_NODE_DEFINE_ENUM_VALUE(ViewportMapping, ADJUST_CAMERA);
  SO_NODE_DEFINE_ENUM_VALUE(ViewportMapping, LEAVE_ALONE);

  SO_NODE_SET_SF_ENUM_TYPE(viewportMapping, ViewportMapping);

  this->stereomode = MONOSCOPIC;
  this->stereoadjustment = 0.1f;
  this->balanceadjustment = 0.0f;
}

/*!
  Destructor.
*/
SoCamera::~SoCamera()
{
}

// Doc in superclass.
void
SoCamera::initClass(void)
{
  SO_NODE_INTERNAL_INIT_ABSTRACT_CLASS(SoCamera, SO_FROM_INVENTOR_1);

  SO_ENABLE(SoGLRenderAction, SoFocalDistanceElement);
  SO_ENABLE(SoGLRenderAction, SoGLProjectionMatrixElement);
  SO_ENABLE(SoGLRenderAction, SoViewVolumeElement);
  SO_ENABLE(SoGLRenderAction, SoGLViewingMatrixElement);
  SO_ENABLE(SoGLRenderAction, SoCullElement);

  SO_ENABLE(SoGetBoundingBoxAction, SoFocalDistanceElement);
  SO_ENABLE(SoGetBoundingBoxAction, SoProjectionMatrixElement);
  SO_ENABLE(SoGetBoundingBoxAction, SoViewVolumeElement);
  SO_ENABLE(SoGetBoundingBoxAction, SoViewingMatrixElement);

  SO_ENABLE(SoRayPickAction, SoFocalDistanceElement);
  SO_ENABLE(SoRayPickAction, SoProjectionMatrixElement);
  SO_ENABLE(SoRayPickAction, SoViewVolumeElement);
  SO_ENABLE(SoRayPickAction, SoViewingMatrixElement);

  SO_ENABLE(SoCallbackAction, SoFocalDistanceElement);
  SO_ENABLE(SoCallbackAction, SoProjectionMatrixElement);
  SO_ENABLE(SoCallbackAction, SoViewVolumeElement);
  SO_ENABLE(SoCallbackAction, SoViewingMatrixElement);

  SO_ENABLE(SoGetPrimitiveCountAction, SoFocalDistanceElement);
  SO_ENABLE(SoGetPrimitiveCountAction, SoProjectionMatrixElement);
  SO_ENABLE(SoGetPrimitiveCountAction, SoViewVolumeElement);
  SO_ENABLE(SoGetPrimitiveCountAction, SoViewingMatrixElement);
}

/*!
  Reorients the camera so that it points towards \a targetpoint.
  The positive y-axis is used as the up vector of the camera, unless
  the new camera direction is parallel to this axis, in which case the
  positive z-axis will be used instead.
*/
void
SoCamera::pointAt(const SbVec3f & targetpoint)
{
  SbVec3f dir = targetpoint - this->position.getValue();
  if (dir.normalize() == 0.0f) return;

  SbVec3f up(0.0f, 1.0f, 0.0f);

  // use 0,1,0 as the up vector unless direction and up vector are parallel
  if (SbAbs(dir.dot(up)) >= (1.0f - FLT_EPSILON)) up.setValue(0.0f, 0.0f, 1.0f);
  this->lookAt(dir, up);
}

/*!
  Reorients the camera so that it points towards \a targetpoint,
  using \a upvector as the camera up vector.

  This method is an extension versus the Open Inventor API.
*/
void
SoCamera::pointAt(const SbVec3f & targetpoint, const SbVec3f & upvector)
{
  SbVec3f dir = targetpoint - this->position.getValue();
  if (dir.normalize() == 0.0f) return;
  this->lookAt(dir, upvector);
}

// FIXME: should collect common code from the two viewAll() methods
// below. 20010824 mortene.

/*!
  Position the camera so that all geometry of the scene from \a sceneroot
  is contained in the view volume of the camera, while keeping the
  camera orientation constant.

  Finds the bounding box of the scene and calls
  SoCamera::viewBoundingBox().
*/
void
SoCamera::viewAll(SoNode * const sceneroot, const SbViewportRegion & vpregion,
                  const float slack)
{
  SoGetBoundingBoxAction action(vpregion);
  action.apply(sceneroot);
  SbBox3f box = action.getBoundingBox();
#if COIN_DEBUG && 0 // debug
  SoDebugError::postInfo("SoCamera::viewAll",
                         "bbox: <%f %f %f>, <%f %f %f>\n",
                         box.getMin()[0], box.getMin()[1], box.getMin()[2],
                         box.getMax()[0], box.getMax()[1], box.getMax()[2]);
#endif // debug

  // Only check for "flagged" emptiness and don't use
  // SbBox3f::hasVolume(), as we *can* handle flat boxes (in all
  // dimensions).
  if (box.isEmpty()) { return; }

  this->viewBoundingBox(box, this->aspectRatio.getValue(), slack);
}

/*!
  Position the camera so all geometry of the scene in \a path is
  contained in the view volume of the camera.

  Finds the bounding box of the scene and calls
  SoCamera::viewBoundingBox().
*/
void
SoCamera::viewAll(SoPath * const path, const SbViewportRegion & vpregion,
                  const float slack)
{
  SoGetBoundingBoxAction action(vpregion);
  action.apply(path);
  SbBox3f box = action.getBoundingBox();

  // Only check for "flagged" emptiness and don't use
  // SbBox3f::hasVolume(), as we *can* handle flat boxes (in all
  // dimensions).
  if (box.isEmpty()) { return; }

  this->viewBoundingBox(box, this->aspectRatio.getValue(), slack);
}

/*!
  Based in the SoCamera::viewportMapping setting, convert the values
  of \a region to the viewport region we will actually render into.
*/
SbViewportRegion
SoCamera::getViewportBounds(const SbViewportRegion & region) const
{
  SbViewportRegion vp = region;
  switch (this->viewportMapping.getValue()) {
  case CROP_VIEWPORT_FILL_FRAME:
  case CROP_VIEWPORT_LINE_FRAME:
  case CROP_VIEWPORT_NO_FRAME:
    {
      float vpaspect = region.getViewportAspectRatio();
      float camaspect = this->aspectRatio.getValue();
      if (vpaspect > camaspect) {
        vp.scaleWidth(camaspect / vpaspect);
        return vp;
      }
      else if (vpaspect < camaspect) {
        vp.scaleHeight(vpaspect / camaspect);
      }
    }
    break;
  default:
    // do nothing
    break;
  }
  return vp;
}

// Doc in superclass.
void
SoCamera::GLRender(SoGLRenderAction * action)
{
  SoState * state = action->getState();

  SbViewportRegion vp;
  SbViewVolume vv;
  this->getView(action, vv, vp, FALSE);

  SbMatrix affine, proj;
  if (vv.getDepth() == 0.0f || vv.getWidth() == 0.0f || vv.getHeight() == 0.0f) {
    // Handle empty scenes.
    affine = proj = SbMatrix::identity();
  }
  else {
    if (this->stereomode != MONOSCOPIC) {
      SbViewVolume copyvv = vv;
      SbMatrix dummy;
      float offset = this->stereoadjustment * 0.5f;
      if (this->stereomode == LEFT_VIEW) offset = -offset;
      SbVec3f r = vv.getProjectionDirection().cross(vv.getViewUp());
      r.normalize();
      r *= offset;
      vv.translateCamera(-(r * (vv.getNearDist() / this->focalDistance.getValue())));
      vv.getMatrices(dummy, proj);
      copyvv.translateCamera(r);
      copyvv.getMatrices(affine, dummy);
    }
    else {
      vv.getMatrices(affine, proj);
    }
    SbBool identity;
    const SbMatrix & mm = SoModelMatrixElement::get(state, identity);
    if (!identity) {
      affine.multRight(mm.inverse());
      vv.transform(SoModelMatrixElement::get(state));
    }
    SoCullElement::setViewVolume(state, vv);
  }

  SoViewVolumeElement::set(state, this, vv);
  if (action->getNumPasses() > 1) {
    SbVec3f jittervec;
    this->jitter(action->getNumPasses(), SoGLRenderPassElement::get(state),
                 vp, jittervec);
    SbMatrix m;
    m.setTranslate(jittervec);
    proj.multRight(m);
  }
  SoProjectionMatrixElement::set(state, this, proj);
  SoViewingMatrixElement::set(state, this, affine);
  SoFocalDistanceElement::set(state, this, this->focalDistance.getValue());
}

// Doc in superclass.
void
SoCamera::getBoundingBox(SoGetBoundingBoxAction * action)
{
  SoCacheElement::invalidate(action->getState());
  SoCamera::doAction(action);
}

/*!
  Picking actions can be triggered during handle event action
  traversal, and to do picking we need to know the camera state.

  \sa SoCamera::rayPick()
 */
void
SoCamera::handleEvent(SoHandleEventAction * action)
{
  SbViewportRegion vp;
  SbViewVolume vv;
  this->getView(action, vv, vp, FALSE);
  SoViewVolumeElement::set(action->getState(), this, vv);
}

/*!
  "Jitter" the camera according to the current rendering pass (\a
  curpass), to get an antialiased rendering of the scene when doing
  multipass rendering.
*/
void
SoCamera::jitter(int numpasses, int curpass, const SbViewportRegion & vpreg,
                 SbVec3f & jitteramount) const
{
  // jitter values from OpenGL Programming Guide
  static float jitter2[] = {
    0.25f, 0.77f,
    0.75f, 0.25f
  };
  static float jitter3[] = {
    0.5033922635f, 0.8317967229f,
    0.7806016275f, 0.2504380877f,
    0.2261828938f, 0.4131553612f
  };
  static float jitter4[] = {
    0.375f, 0.25f,
    0.125f, 0.75f,
    0.875f, 0.25f,
    0.625f, 0.75f
  };
  static float jitter5[] = {
    0.5f, 0.5f,
    0.3f, 0.1f,
    0.7f, 0.9f,
    0.9f, 0.3f,
    0.1f, 0.7f
  };
  static float jitter6[] = {
    0.4646464646f, 0.4646464646f,
    0.1313131313f, 0.7979797979f,
    0.5353535353f, 0.8686868686f,
    0.8686868686f, 0.5353535353f,
    0.7979797979f, 0.1313131313f,
    0.2020202020f, 0.2020202020f
  };
  static float jitter8[] = {
    0.5625f, 0.4375f,
    0.0625f, 0.9375f,
    0.3125f, 0.6875f,
    0.6875f, 0.8125f,
    0.8125f, 0.1875f,
    0.9375f, 0.5625f,
    0.4375f, 0.0625f,
    0.1875f, 0.3125f
  };
  static float jitter9[] = {
    0.5f, 0.5f,
    0.1666666666f, 0.9444444444f,
    0.5f, 0.1666666666f,
    0.5f, 0.8333333333f,
    0.1666666666f, 0.2777777777f,
    0.8333333333f, 0.3888888888f,
    0.1666666666f, 0.6111111111f,
    0.8333333333f, 0.7222222222f,
    0.8333333333f, 0.0555555555f
  };
  static float jitter12[] = {
    0.4166666666f, 0.625f,
    0.9166666666f, 0.875f,
    0.25f, 0.375f,
    0.4166666666f, 0.125f,
    0.75f, 0.125f,
    0.0833333333f, 0.125f,
    0.75f, 0.625f,
    0.25f, 0.875f,
    0.5833333333f, 0.375f,
    0.9166666666f, 0.375f,
    0.0833333333f, 0.625f,
    0.583333333f, 0.875f
  };
  static float jitter16[] = {
    0.375f, 0.4375f,
    0.625f, 0.0625f,
    0.875f, 0.1875f,
    0.125f, 0.0625f,
    0.375f, 0.6875f,
    0.875f, 0.4375f,
    0.625f, 0.5625f,
    0.375f, 0.9375f,
    0.625f, 0.3125f,
    0.125f, 0.5625f,
    0.125f, 0.8125f,
    0.375f, 0.1875f,
    0.875f, 0.9375f,
    0.875f, 0.6875f,
    0.125f, 0.3125f,
    0.625f, 0.8125f
  };

  static float * jittertab[] = {
    jitter2,
    jitter3,
    jitter4,
    jitter5,
    jitter6,
    jitter8,
    jitter8,
    jitter9,
    jitter12,
    jitter12,
    jitter12,
    jitter16,
    jitter16,
    jitter16,
    jitter16
  };

  // FIXME: support more rendering passes by generating jitter tables
  // using some clever algorithm. pederb, 2001-02-21
  if (numpasses > 16) numpasses = 16;
  if (curpass >= numpasses) curpass = numpasses - 1;

  float * jittab = jittertab[numpasses-2];

  SbVec2s vpsize = vpreg.getViewportSizePixels();
  jitteramount[0] = jittab[curpass*2] * 2.0f / float(vpsize[0]);
  jitteramount[1] = jittab[curpass*2+1] * 2.0f / float(vpsize[1]);
  jitteramount[2] = 0.0f;
}

// Documented in superclass. Overridden to set up the viewing and
// projection matrices.
void
SoCamera::doAction(SoAction * action)
{
  SoState * state = action->getState();

  SbViewportRegion vp;
  SbViewVolume vv;
  this->getView(action, vv, vp, FALSE);

  SbMatrix affine, proj;
  if (vv.getDepth() == 0.0f || vv.getWidth() == 0.0f || vv.getHeight() == 0.0f) {
    // Handle empty scenes.
    affine = proj = SbMatrix::identity();
  }
  else {
    vv.getMatrices(affine, proj);

    SbBool identity;
    const SbMatrix & mm = SoModelMatrixElement::get(state, identity);
    if (!identity) {
      vv.transform(mm);
      affine.multRight(mm.inverse());
    }
  }
  SoViewVolumeElement::set(state, this, vv);
  SoProjectionMatrixElement::set(state, this, proj);
  SoViewingMatrixElement::set(state, this, affine);
  SoFocalDistanceElement::set(state, this, this->focalDistance.getValue());
}

// Doc in superclass.
void
SoCamera::callback(SoCallbackAction * action)
{
  SoCamera::doAction(action);
}

// Documented in superclass.
void
SoCamera::rayPick(SoRayPickAction * action)
{
  // Overridden to calculate the coordinates of the ray within the
  // current camera settings.

  SoCamera::doAction(action);

  // We need to check for a non-empty view volume, as caused by scene
  // graphs with no geometry in them.
  SbViewVolume vv = this->getViewVolume(1.0f);
  if (vv.getDepth() != 0.0f &&
      vv.getWidth() != 0.0f &&
      vv.getHeight() != 0.0f) {
    action->computeWorldSpaceRay();
  }
}

// Documented in superclass.
void
SoCamera::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  // The number of primitives used to render a shape can change
  // according to the shape's distance to the camera, so we need to
  // override this method from the superclass to modify the traversal
  // state settings for the camera view.

  SoCamera::doAction(action);
}

//
// private method which calculates view volume, and calculates
// new viewport region if viewportMapping requires this.
// The state is updated with the new viewport, not with the
// new view volume.
//
void
SoCamera::getView(SoAction * action, SbViewVolume & resultvv, SbViewportRegion & resultvp,
                  const SbBool considermodelmatrix)
{
  SoState * state = action->getState();

  // need to test if vp element is enabled. SoGetPrimitiveCountAction
  // does not enable this element, although I think it should (to get
  // correct SCREEN_SPACE complexity handling).  pederb, 2001-10-31
  SbBool usevpelement =
    state->isElementEnabled(SoViewportRegionElement::getClassStackIndex());

  if (usevpelement) {
    resultvp = SoViewportRegionElement::get(state);
  }
  else {
    // just set it to some value. It's not important as the current
    // action does not support viewports.
    resultvp = SbViewportRegion(256, 256);
  }
  float aspectratio = resultvp.getViewportAspectRatio();
  int vpm = this->viewportMapping.getValue();

  SbBool adjustvp = FALSE;

  switch (vpm) {
  case CROP_VIEWPORT_FILL_FRAME:
  case CROP_VIEWPORT_LINE_FRAME:
  case CROP_VIEWPORT_NO_FRAME:
    resultvv = this->getViewVolume(0.0f);
    adjustvp = TRUE;
    break;
  case ADJUST_CAMERA:
    resultvv = this->getViewVolume(aspectratio);
    if (aspectratio < 1.0f) resultvv.scale(1.0f / aspectratio);
    break;
  case LEAVE_ALONE:
    resultvv = this->getViewVolume(0.0f);
    break;
  default:
    assert(0 && "unknown viewport mapping");
    break;
  }

  if (considermodelmatrix) {
    SbBool isidentity;
    const SbMatrix &mm = SoModelMatrixElement::get(state, isidentity);
    if (!isidentity) resultvv.transform(mm);
  }
  if (adjustvp) {
    float cameraratio = this->aspectRatio.getValue();
    if (aspectratio != cameraratio) {
      SbViewportRegion oldvp = resultvp;
      if (aspectratio < cameraratio) {
        resultvp.scaleHeight(aspectratio/cameraratio);
      }
      else {
        resultvp.scaleWidth(cameraratio/aspectratio);
      }
      // only draw if this is an SoGLRenderAction
      if (action->isOfType(SoGLRenderAction::getClassTypeId())) {
        this->drawCroppedFrame((SoGLRenderAction*)action, vpm, oldvp, resultvp);
      }
      if (usevpelement) {
        SoViewportRegionElement::set(action->getState(), resultvp);
      }
    }
  }
}

//
// private method that draws a cropped frame
//
void
SoCamera::drawCroppedFrame(SoGLRenderAction *action,
                           const int viewportmapping,
                           const SbViewportRegion & oldvp,
                           const SbViewportRegion & newvp)
{
  if (viewportmapping == SoCamera::CROP_VIEWPORT_NO_FRAME) return;

  if (action->handleTransparency(FALSE))
    return;
  
  const cc_glglue * glw = cc_glglue_instance(action->getCacheContext());

  SoState *state = action->getState();
  state->push();

  if (viewportmapping == SoCamera::CROP_VIEWPORT_LINE_FRAME) {
    SoLineWidthElement::set(state, this, 1.0f);
    const SoGLLineWidthElement * lw = (SoGLLineWidthElement *)
      state->getConstElement(SoGLLineWidthElement::getClassStackIndex());
    lw->lazyEvaluate();
  }
  else { // FILL
    SoDrawStyleElement::set(state, this, SoDrawStyleElement::FILLED);
    // turn off backface culling
    SoGLShapeHintsElement::forceSend(state, TRUE, FALSE);
  }

  SbVec2s oldorigin = oldvp.getViewportOriginPixels();
  SbVec2s oldsize = oldvp.getViewportSizePixels();
  glMatrixMode(GL_PROJECTION);
  // projection matrix will be set later, so don't push
  glOrtho(oldorigin[0], oldorigin[0]+oldsize[0]-1,
          oldorigin[1], oldorigin[1]+oldsize[1]-1,
          -1, 1);

  glPushAttrib(GL_LIGHTING_BIT|
               GL_FOG_BIT|
               GL_DEPTH_BUFFER_BIT|
               GL_TEXTURE_BIT|
               GL_CURRENT_BIT);

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  glDisable(GL_LIGHTING);
  glDisable(GL_TEXTURE_2D);
  if (glw->has3DTextures) glDisable(GL_TEXTURE_3D);
  glDisable(GL_FOG);
  glDisable(GL_DEPTH_TEST);
  
  glColor3f(0.8f, 0.8f, 0.8f);

  SbVec2s origin = newvp.getViewportOriginPixels();
  SbVec2s size = newvp.getViewportSizePixels();
  SbVec2s orgsize = oldvp.getViewportSizePixels();

  if (size[0] < orgsize[0]) {
    short minpos = origin[0] - 1;
    short maxpos = origin[0] + size[0];
    if (viewportmapping == SoCamera::CROP_VIEWPORT_LINE_FRAME) {
      glBegin(GL_LINES);
      glVertex2s(minpos, oldorigin[1]);
      glVertex2s(minpos, oldorigin[1]+oldsize[1]);
      glVertex2s(maxpos, oldorigin[1]);
      glVertex2s(maxpos, oldorigin[1]+oldsize[1]);
      glEnd();
    }
    else {
      glBegin(GL_QUADS);
      glVertex2s(oldorigin[0], oldorigin[1]);
      glVertex2s(oldorigin[0], oldorigin[1]+oldsize[1]-1);
      glVertex2s(minpos, oldorigin[1]+oldsize[1]);
      glVertex2s(minpos, oldorigin[1]);

      glVertex2s(maxpos, oldorigin[1]);
      glVertex2s(maxpos, oldorigin[1]+oldsize[1]-1);
      glVertex2s(oldorigin[0]+oldsize[0]-1, oldorigin[1]+oldsize[1]-1);
      glVertex2s(oldorigin[0]+oldsize[0]-1, oldorigin[1]);
      glEnd();
    }
  }
  else if (size[1] < orgsize[1]) {
    short minpos = origin[1] - 1;
    short maxpos = origin[1] + size[1];
    if (viewportmapping == SoCamera::CROP_VIEWPORT_LINE_FRAME) {
      glBegin(GL_LINES);
      glVertex2s(oldorigin[0], minpos);
      glVertex2s(oldorigin[0]+oldsize[0], minpos);
      glVertex2s(oldorigin[0], maxpos);
      glVertex2s(oldorigin[0]+oldsize[0], maxpos);
      glEnd();
    }
    else {
      glBegin(GL_QUADS);
      glVertex2s(oldorigin[0], minpos);
      glVertex2s(oldorigin[0]+oldsize[0]-1, minpos);
      glVertex2s(oldorigin[0]+oldsize[0]-1, oldorigin[1]);
      glVertex2s(oldorigin[0], oldorigin[1]);

      glVertex2s(oldorigin[0], maxpos);
      glVertex2s(oldorigin[0], oldorigin[1]+oldsize[1]-1);
      glVertex2s(oldorigin[0]+oldsize[0]-1, oldorigin[1]+oldsize[1]-1);
      glVertex2s(oldorigin[1]+oldsize[0]-1, maxpos);
      glEnd();
    }
  }

  glPopMatrix();
  glPopAttrib();

  state->pop();
}

/*!
  Sets the stereo mode.
*/
void
SoCamera::setStereoMode(StereoMode mode)
{
  this->stereomode = mode;
}

/*!
  Returns the stereo mode.
*/
SoCamera::StereoMode
SoCamera::getStereoMode(void) const
{
  return this->stereomode;
}

/*!
  Sets the stereo adjustment.
*/
void
SoCamera::setStereoAdjustment(float adjustment)
{
  this->stereoadjustment = adjustment;
}

/*!
  Returns the stereo adjustment.
*/
float
SoCamera::getStereoAdjustment(void) const
{
  return this->stereoadjustment;
}

/*!
  Sets the stereo balance adjustment.
*/
void
SoCamera::setBalanceAdjustment(float adjustment)
{
  this->balanceadjustment = adjustment;
}

/*!
  Returns the stereo balance adjustment.
*/
float
SoCamera::getBalanceAdjustment(void) const
{
  return this->balanceadjustment;
}

// Private method that calculates a new orientation based on camera
// direction and camera up vector. Vectors must be unit length.
void
SoCamera::lookAt(const SbVec3f & dir, const SbVec3f & up)
{
  SbVec3f z = -dir;
  SbVec3f y = up;
  SbVec3f x = y.cross(z);

  // recompute y to create a valid coordinate system
  y = z.cross(x);

  // normalize x and y to create an orthonormal coord system
  y.normalize();
  x.normalize();

  // create a rotation matrix
  SbMatrix rot = SbMatrix::identity();
  rot[0][0] = x[0];
  rot[0][1] = x[1];
  rot[0][2] = x[2];

  rot[1][0] = y[0];
  rot[1][1] = y[1];
  rot[1][2] = y[2];

  rot[2][0] = z[0];
  rot[2][1] = z[1];
  rot[2][2] = z[2];

  this->orientation.setValue(SbRotation(rot));
}
