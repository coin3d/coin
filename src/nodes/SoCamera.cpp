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
  (SoXt, SoQt, SoBe, etc) will automatically insert a camera into a
  scene graph is none has been defined.
*/

// Metadon doc:
/*¡
  FIXME:
  <ul>
  <li>antialiasing by "jittering" the camera when doing multipass
      rendering has not been implemented yet</li>
  </ul>
 */

#include <Inventor/nodes/SoCamera.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <coindefs.h> // COIN_STUB()
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
#include <Inventor/elements/SoGLPolygonStippleElement.h>
#include <Inventor/elements/SoCullElement.h>
#include <Inventor/misc/SoState.h>

#ifdef _WIN32
#include <windows.h>
#endif // _WIN32

#include <GL/gl.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

// FIXME: should document the enum values. 20000310 mortene.
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

  Distance from camera position to the near clipping plane. Defaults
  to 1.0.

  The default behavior of the GUI viewer components is to
  automatically update this value for the scene camera according to
  the scene bounding box. Ditto for the far clipping plane.
*/
/*!
  \var SoSFFloat SoCamera::farDistance

  Distance from camera position to the far clipping plane. Defaults to
  10.0.

  See also documentation of SoCamera::nearDistance.
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
  SO_NODE_INTERNAL_INIT_ABSTRACT_CLASS(SoCamera);

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
  Reorients the camera so the direction vector goes through \a
  targetpoint.
*/
void
SoCamera::pointAt(const SbVec3f & targetpoint)
{
  // FIXME: is this really correct? What if camera is not positioned
  // in or near origo and targetpoint is behind us, for instance?
  // 19990228 mortene.

  SbVec3f dir = targetpoint - this->position.getValue();
  SbRotation rot(SbVec3f(0.0f, 0.0f, -1.0f), dir);
  this->orientation.setValue(rot);
}

/*!
  Position the camera so all geometry of the scene from \a sceneroot
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
#if 0 // debug
  SoDebugError::postInfo("SoCamera::viewAll",
                         "bbox: <%f %f %f>, <%f %f %f>\n",
                         box.getMin()[0], box.getMin()[1], box.getMin()[2],
                         box.getMax()[0], box.getMax()[1], box.getMax()[2]);
#endif // debug
  this->viewBoundingBox(box.isEmpty() ? SbBox3f(.0f, .0f, .0f, .0f, .0f, .0f) : box,
                        this->aspectRatio.getValue(), slack);
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
  this->viewBoundingBox(box.isEmpty() ? SbBox3f(.0f, .0f, .0f, .0f, .0f, .0f) : box,
                        this->aspectRatio.getValue(), slack);
}

/*!
  Based in the SoCamera::viewportMapping setting, convert the values
  of \a region to the viewport region we will actually render into.
*/
SbViewportRegion
SoCamera::getViewportBounds(const SbViewportRegion & region) const
{
  COIN_STUB(); // FIXME
  return SbViewportRegion();
}

// Doc in superclass.
void
SoCamera::GLRender(SoGLRenderAction * action)
{
  SoCamera::doAction(action);
  SoState * state = action->getState();
  const SbViewVolume &vv = SoViewVolumeElement::get(state);
  SbPlane plane[6];
  vv.getViewVolumePlanes(plane);
  SoCullElement::addPlanes(state, plane, 6);
}

// Doc in superclass.
void
SoCamera::getBoundingBox(SoGetBoundingBoxAction * action)
{
#if 0 // FIXME: experimental code, not enabled, pederb, 2000-01-20
  if (action->isInCameraSpace()) {
    SoState * state = action->getState();
    SbMatrix modelmatrix = SoModelMatrixElement::get(state);
    SbVec3f camerapos(0.0f, 0.0f, 0.0f);
    SbVec3f cameradir(0.0f, 0.0f, -1.0f);
    modelmatrix.multVecMatrix(camerapos, camerapos);
    modelmatrix.multDirMatrix(cameradir, cameradir);
    cameradir.normalize();
    SoModelMatrixElement::translateBy(state, this, -camerapos - SbVec3f(0, 0, 1));
    SoModelMatrixElement::rotateBy(state, this, SbRotation(cameradir, SbVec3f(0, 0, -1)));
    SoModelMatrixElement::makeIdentity(action->getState(), this);
  }
#endif // experimental code
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
  // FIXME: viewportMapping field is not accounted for. 19990315
  // mortene.
  SoState * state = action->getState();

  SbViewportRegion vp = SoViewportRegionElement::get(state);
  float aspectratio = vp.getViewportAspectRatio();
  int vpm = this->viewportMapping.getValue();

  SbViewVolume vv =
    this->getViewVolume(vpm == ADJUST_CAMERA ? aspectratio : 0.0f);

  SbBool adjustvp = FALSE;

  switch (vpm) {
  case CROP_VIEWPORT_FILL_FRAME:
  case CROP_VIEWPORT_LINE_FRAME:
  case CROP_VIEWPORT_NO_FRAME:
    adjustvp = TRUE;
    break;
  case ADJUST_CAMERA:
  case LEAVE_ALONE:
    break;
  default:
    assert(0 && "unknown viewport mapping");
    break;
  }

  if (adjustvp) {
    float cameraratio = this->aspectRatio.getValue();
    if (aspectratio != cameraratio) {
      if (aspectratio < cameraratio) {
        vp.scaleHeight(aspectratio/cameraratio);
      }
      else {
        vp.scaleWidth(cameraratio/aspectratio);
      }
      SoViewportRegionElement::set(action->getState(), vp);
    }
  }
  SoViewVolumeElement::set(state, this, vv);
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
  COIN_STUB(); // FIXME
}

/*!
  Overloaded to set up the viewing and projection matrices.
 */
void
SoCamera::doAction(SoAction * action)
{
  SoState * state = action->getState();

  SbViewportRegion vp = SoViewportRegionElement::get(state);
  float aspectratio = vp.getViewportAspectRatio();
  int vpm = this->viewportMapping.getValue();

  SbViewVolume vv =
    this->getViewVolume(vpm == ADJUST_CAMERA ? aspectratio : 0.0f);

  SbBool adjustvp = FALSE;

  switch (vpm) {
  case CROP_VIEWPORT_FILL_FRAME:
  case CROP_VIEWPORT_LINE_FRAME:
  case CROP_VIEWPORT_NO_FRAME:
    adjustvp = TRUE;
    break;
  case ADJUST_CAMERA:
  case LEAVE_ALONE:
    break;
  default:
    assert(0 && "unknown viewport mapping");
    break;
  }

  if (adjustvp) {
    float cameraratio = this->aspectRatio.getValue();
    if (aspectratio != cameraratio) {
      SbViewportRegion newvp = vp;

      if (aspectratio < cameraratio) {
        newvp.scaleHeight(aspectratio/cameraratio);
      }
      else {
        newvp.scaleWidth(cameraratio/aspectratio);
      }
      if (action->isOfType(SoGLRenderAction::getClassTypeId())) {
        this->drawCroppedFrame((SoGLRenderAction*)action, vpm, vp, newvp);
      }
      SoViewportRegionElement::set(action->getState(), newvp);
    }
  }

  SoViewVolumeElement::set(state, this, vv);

  SbMatrix affine, proj;
  if (vv.getDepth() == 0.0f || vv.getWidth() == 0.0f || vv.getHeight() == 0.0f) {
    // Handle empty scenes.
    affine = proj = SbMatrix::identity();
  }
  else {
    vv.getMatrices(affine, proj);
  }

  affine.multRight(SoModelMatrixElement::get(state).inverse());
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

/*!
  Overloaded to calculate the coordinates of the ray within the
  current camera settings.
 */
void
SoCamera::rayPick(SoRayPickAction * action)
{
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

/*!
  The number of primitives used to render a shape can change according
  to the shape's distance to the camera, so we need to overload this
  method from the superclass to modify the traversal state settings
  for the camera view.
 */
void
SoCamera::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  SoCamera::doAction(action);
}

void
SoCamera::drawCroppedFrame(SoGLRenderAction *action,
                           const int viewportmapping,
                           const SbViewportRegion & oldvp,
                           const SbViewportRegion & newvp)
{
  if (viewportmapping == SoCamera::CROP_VIEWPORT_NO_FRAME) return;

  if (action->handleTransparency(FALSE))
    return;

  SoState *state = action->getState();
  state->push();

  if (viewportmapping == SoCamera::CROP_VIEWPORT_LINE_FRAME) {
    SoLineWidthElement::set(state, this, 1.0f);
    const SoGLLineWidthElement * lw = (SoGLLineWidthElement *)
      state->getConstElement(SoGLLineWidthElement::getClassStackIndex());
    lw->evaluate();
  }
  else { // FILL
    SoDrawStyleElement::set(state, this, SoDrawStyleElement::FILLED);
    const SoGLShapeHintsElement * sh = (SoGLShapeHintsElement *)
      state->getConstElement(SoGLShapeHintsElement::getClassStackIndex());
    sh->forceSend(TRUE, FALSE); // turn off backface culling
    SoGLPolygonStippleElement::set(state, FALSE);
    const SoGLPolygonStippleElement * ps = (SoGLPolygonStippleElement *)
      state->getConstElement(SoGLPolygonStippleElement::getClassStackIndex());
    ps->evaluate(); // lazy element, force evaluate
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
               GL_TEXTURE_BIT);

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  glDisable(GL_LIGHTING);
  glDisable(GL_TEXTURE_2D);
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
