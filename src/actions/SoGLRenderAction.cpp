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
  \class SoGLRenderAction SoGLRenderAction.h Inventor/actions/SoGLRenderAction.h
  \brief The SoGLRenderAction class renders the scene graph with OpenGL calls.
  \ingroup actions

  Applying this method at a root node for a scene graph, path or
  pathlist will render all geometry contained within that instance to
  the current OpenGL context.
 */

/*!
  \typedef void SoGLRenderPassCB(void * userdata)

  Callback functions for the setPassCallback() method need to be of
  this type.

  \a userdata is a void pointer to any data the application need to
  know of in the callback function (like for instance a \e this
  pointer).

  \sa setPassCallback()
 */

#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/SbColor.h>
#include <Inventor/SbPlane.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoSubActionP.h>
#include <Inventor/elements/SoDecimationPercentageElement.h>
#include <Inventor/elements/SoDecimationTypeElement.h>
#include <Inventor/elements/SoGLLightIdElement.h>
#include <Inventor/elements/SoGLNormalizeElement.h>
#include <Inventor/elements/SoGLRenderPassElement.h>
#include <Inventor/elements/SoGLShadeModelElement.h>
#include <Inventor/elements/SoGLUpdateAreaElement.h>
#include <Inventor/elements/SoGLViewportRegionElement.h>
#include <Inventor/elements/SoLazyElement.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/elements/SoOverrideElement.h>
#include <Inventor/elements/SoShapeStyleElement.h>
#include <Inventor/elements/SoTextureOverrideElement.h>
#include <Inventor/elements/SoViewVolumeElement.h>
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/elements/SoWindowElement.h>
#include <Inventor/lists/SoEnabledElementsList.h>
#include <Inventor/misc/SoGL.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/nodes/SoShape.h>
#include <Inventor/lists/SoCallbackList.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H
#include <Inventor/system/gl.h>

// *************************************************************************

/*!
  \enum SoGLRenderAction::TransparencyType

  Various settings for how to do rendering of transparent objects in
  the scene. Some of the settings will provide faster rendering, while
  others gives you better quality rendering.
*/

/*!
  \var SoGLRenderAction::TransparencyType SoGLRenderAction::SCREEN_DOOR
  Transparent triangles are rendered with a dither pattern. This is
  a fast (on most GFX cards) but not-so-high-quality transparency mode.
*/

/*!

  \var SoGLRenderAction::TransparencyType SoGLRenderAction::ADD

  Transparent objects are rendered using additive alpha blending.
  Additive blending is probably mostly used to create special
  transparency effects. The new pixel color is calculated as the
  current pixel color plus the source pixel color multiplied with the
  source pixel alpha value.

*/

/*!
  \var SoGLRenderAction::TransparencyType SoGLRenderAction::DELAYED_ADD

  SoGLRenderAction::DELAYED_ADD Transparent objects are rendered using
  additive alpha blending, in a second rendering pass with depth
  buffer updates disabled.

*/

/*!
  \var SoGLRenderAction::TransparencyType SoGLRenderAction::SORTED_OBJECT_ADD

  SoGLRenderAction::SORTED_OBJECT_ADD Transparent objects are rendered
  using additive alpha blending.  Opaque objects are rendered first,
  and transparent objects are rendered back to front with z-buffer
  updates disabled.

*/

/*!
  \var SoGLRenderAction::TransparencyType SoGLRenderAction::BLEND

  Transparent objects are rendered using multiplicative alpha blending.

  Multiplicative alpha blending is the blending type that is most
  often used to render transparent objects. The new pixel value is
  calculated as the old pixel color multiplied with one minus the
  source alpha value, plus the source pixel color multiplied with the
  source alpha value.

  We recommend that you use this transparency mode if you have only
  one transparent object in your scene, and you know that it will be
  rendered after the opaque objects.

*/

/*!
  \var SoGLRenderAction::TransparencyType SoGLRenderAction::DELAYED_BLEND

  Transparent objects are rendered using multiplicative alpha
  blending, in a second rendering pass with depth buffer updates
  disabled.

  Use this transparency type when you have one transparent object, or
  several transparent object that you know will never overlap (when
  projected to screen). Since the transparent objects are rendered
  after opaque ones, you'll not have to worry about putting the
  transparent objects at the end of your scene graph. It will not be
  as fast as the BLEND transparency type, of course, since the scene
  graph is traversed twice.

*/

/*!
  \var SoGLRenderAction::TransparencyType SoGLRenderAction::SORTED_OBJECT_BLEND

  Transparent objects are rendered using multiplicative alpha
  blending, Opaque objects are rendered first, and transparent objects
  are rendered back to front with z-buffer updates disabled.

  Use this transparency mode when you have several transparent object
  that you know might overlap (when projected to screen). This method
  will require 1 + num_transparent_objects rendering passes. Path
  traversal is used when rendering transparent objects, of course, but
  it might still be slow if you have lots of state changes before your
  transparent object. When using this mode, we recommend placing the
  transparent objects as early as possible in the scene graph to
  minimize traversal overhead.
*/

/*!
  \var SoGLRenderAction::TransparencyType SoGLRenderAction::SORTED_OBJECT_SORTED_TRIANGLE_ADD

  This transparency type is a Coin extension versus the Open Inventor
  API.

  Transparent objects are rendered using additive alpha blending,
  Transparent objects are rendered back to front, and triangles in
  each object are sorted back to front before rendering.

  See description for SORTED_OBJECT_SORTED_TRIANGLE_BLEND for more
  information about this transparency type.

*/

/*!
  \var SoGLRenderAction::TransparencyType SoGLRenderAction::SORTED_OBJECT_SORTED_TRIANGLE_BLEND

  This transparency type is a Coin extension versus the Open Inventor
  API.

  Transparent objects are rendered using multiplicative alpha
  blending, Transparent objects are rendered back to front, and
  triangles in each object are sorted back to front before
  rendering.

  Use this transparency type when you have one (or more) transparent
  object(s) where you know triangles might overlap inside the object.
  This transparency type might be very slow if you have an object with
  lots of triangles, since all triangles have to be sorted before
  rendering, and an unoptimized rendering loop is used when rendering.
  Lines and points are not sorted before rendering. They are rendered
  as in the normal SORTED_OBJECT_BLEND transparency type.

  Please note that this transparency mode does not guarantee
  "correct" transparency rendering. It is almost impossible to find an
  algorithm that will sort triangles correctly in all cases, and
  intersecting triangles are not handled. Also, since each object
  is handled separately, two intersecting object will lead to
  incorrect transparency.
*/

/*!
  \var SoGLRenderAction::TransparencyType SoGLRenderAction::NONE

  This transparency type is a Coin extension versus the Open Inventor
  API.

  Turns off transparency for objects, even if transparency is set using
  an SoMaterial node.
*/

/*!
  \enum SoGLRenderAction::AbortCode

  The return codes which an SoGLRenderAbortCB callback function should
  use.
*/
/*!
  \var SoGLRenderAction::AbortCode SoGLRenderAction::CONTINUE
  Continue rendering as usual.
*/
/*!
  \var SoGLRenderAction::AbortCode SoGLRenderAction::ABORT
  Abort the rendering action immediately.
*/
/*!
  \var SoGLRenderAction::AbortCode SoGLRenderAction::PRUNE
  Do not render the current node or any of its children, but continue
  the rendering traversal.
*/
/*!
  \var SoGLRenderAction::AbortCode SoGLRenderAction::DELAY
  Delay rendering of the current node (and its children) until the
  next rendering pass.
*/

/*!
  \typedef typedef SoGLRenderAction::SoGLRenderAbortCB(void * userdata)
  Abort callbacks should be of this type.
  \sa setAbortCallback()
*/


#ifndef DOXYGEN_SKIP_THIS

class SoGLRenderActionP {
public:
  SoGLRenderActionP(SoGLRenderAction * action)
    : action(action) { }

  SoGLRenderAction * action;
  SbViewportRegion viewport;
  int numpasses;
  SoGLRenderAction::TransparencyType transparencytype;
  SbBool smoothing;
  SbBool passupdate;
  SoGLRenderPassCB * passcallback;
  void * passcallbackdata;
  SoGLRenderAction::SoGLRenderAbortCB * abortcallback;
  void * abortcallbackdata;
  uint32_t cachecontext;
  int currentpass;
  SbBool isblendenabled;
  SoPathList delayedpaths;
  SbBool delayedpathrender;
  SbBool transparencyrender;
  SoPathList transpobjpaths;
  SbList<float> transpobjdistances;

  SoGetBoundingBoxAction * bboxaction;
  SbVec2f updateorigin, updatesize;
  SbBool renderingremote;
  SbBool needglinit;
  SoGLRenderAction::TransparencyType gltransptype;
  SbBool isrendering;
  SoCallbackList precblist;

public:
  void setupTransparency(const SoGLRenderAction::TransparencyType newtype);

  void disableBlend(const SbBool force = FALSE);
  void enableBlend(const SbBool force = FALSE);

  void render(SoNode * node);
  void renderMulti(SoNode * node);
  void renderSingle(SoNode * node);
};

#endif // DOXYGEN_SKIP_THIS

SO_ACTION_SOURCE(SoGLRenderAction);

// Override from parent class.
void
SoGLRenderAction::initClass(void)
{
  SO_ACTION_INIT_CLASS(SoGLRenderAction, SoAction);

  SO_ENABLE(SoGLRenderAction, SoDecimationPercentageElement);
  SO_ENABLE(SoGLRenderAction, SoDecimationTypeElement);
  SO_ENABLE(SoGLRenderAction, SoGLLightIdElement);
  SO_ENABLE(SoGLRenderAction, SoGLNormalizeElement);
  SO_ENABLE(SoGLRenderAction, SoGLRenderPassElement);
  SO_ENABLE(SoGLRenderAction, SoGLShadeModelElement);
  SO_ENABLE(SoGLRenderAction, SoGLUpdateAreaElement);
  SO_ENABLE(SoGLRenderAction, SoLazyElement);
  SO_ENABLE(SoGLRenderAction, SoOverrideElement);
  SO_ENABLE(SoGLRenderAction, SoTextureOverrideElement);
  SO_ENABLE(SoGLRenderAction, SoWindowElement);
  SO_ENABLE(SoGLRenderAction, SoGLViewportRegionElement);
  SO_ENABLE(SoGLRenderAction, SoGLCacheContextElement);
}

// *************************************************************************

#undef THIS
#define THIS this->pimpl

/*!
  Constructor. Sets up the render action for rendering within the
  given \a viewportregion.
*/
SoGLRenderAction::SoGLRenderAction(const SbViewportRegion & viewportregion)
{
  SO_ACTION_CONSTRUCTOR(SoGLRenderAction);

  THIS = new SoGLRenderActionP(this);
  // Can't just push this on the SoViewportRegionElement stack, as the
  // state hasn't been made yet.
  THIS->viewport = viewportregion;

  THIS->passcallback = NULL;
  THIS->passcallbackdata = NULL;
  THIS->smoothing = FALSE;
  THIS->numpasses = 1;
  THIS->transparencytype = SoGLRenderAction::SCREEN_DOOR;
  THIS->delayedpathrender = FALSE;
  THIS->transparencyrender = FALSE;
  THIS->isrendering = FALSE;
  THIS->isblendenabled = FALSE;
  THIS->passupdate = FALSE;
  THIS->bboxaction = NULL;
  THIS->updateorigin.setValue(0.0f, 0.0f);
  THIS->updatesize.setValue(1.0f, 1.0f);
  THIS->renderingremote = FALSE;
  THIS->abortcallback = NULL;
  THIS->cachecontext = 0;
  THIS->needglinit = TRUE;
  THIS->gltransptype = SoGLRenderAction::NONE;
}

/*!
  Destructor, frees up all internal resources for action instance.
*/
SoGLRenderAction::~SoGLRenderAction()
{
  delete THIS->bboxaction;
  delete THIS;
}

/*!
  Sets the viewport region for rendering. This will then override the
  region passed in with the constructor.
*/
void
SoGLRenderAction::setViewportRegion(const SbViewportRegion & newregion)
{
  THIS->viewport = newregion;

  // The SoViewportRegionElement is not set here, as it is always
  // initialized before redraw in beginTraversal().
}

/*!
  Returns the viewport region for the rendering action.
*/
const SbViewportRegion &
SoGLRenderAction::getViewportRegion(void) const
{
  return THIS->viewport;
}

/*!
  Sets the area of the OpenGL context canvas we should render into.

  The coordinates for \a origin and \a size should be normalized to be
  within [0.0, 1.0]. The default settings are <0.0, 0.0> for the \a
  origin and <1.0, 1.0> for the \a size, using the full size of the
  rendering canvas.
*/
void
SoGLRenderAction::setUpdateArea(const SbVec2f & origin, const SbVec2f & size)
{
  THIS->updateorigin = origin;
  THIS->updatesize = size;
}

/*!
  Returns information about the area of the rendering context window
  to be updated.
*/
void
SoGLRenderAction::getUpdateArea(SbVec2f & origin, SbVec2f & size) const
{
  origin = THIS->updateorigin;
  size = THIS->updatesize;
}

/*!
  Sets the abort callback.  The abort callback is called by the action
  for each node during traversal to check for abort conditions.

  \sa SoGLRenderAction::AbortCode
*/
void
SoGLRenderAction::setAbortCallback(SoGLRenderAbortCB * const func,
                                   void * const userdata)
{
  THIS->abortcallback = func;
  THIS->abortcallbackdata = userdata;
}

/*!
  Sets the transparency rendering method for transparent objects in
  the scene graph.

  \sa SoGLRenderAction::TransparencyType
*/
void
SoGLRenderAction::setTransparencyType(const TransparencyType type)
{
  if (THIS->transparencytype != type) {
    THIS->transparencytype = type;
    THIS->needglinit = TRUE;
  }
}

/*!
  Returns the transparency rendering type.
*/
SoGLRenderAction::TransparencyType
SoGLRenderAction::getTransparencyType(void) const
{
  return THIS->transparencytype;
}

/*!
  Sets (or unsets) smoothing. If the smoothing flag is \c on, Coin
  will try to use built-in features from the OpenGL implementation to
  smooth the appearance of otherwise jagged borders.

  This is a simple (and computationally non-intensive) way of doing
  anti-aliasing.

  Default value for this flag is to be \c off.
*/
void
SoGLRenderAction::setSmoothing(const SbBool smooth)
{
  if (smooth != THIS->smoothing) {
    THIS->smoothing = smooth;
    THIS->needglinit = TRUE;
  }
}

/*!
  Returns whether smoothing is set or not.
*/
SbBool
SoGLRenderAction::isSmoothing(void) const
{
  return THIS->smoothing;
}

/*!
  Sets the number of rendering passes.  Default is 1, anything greater
  will enable antialiasing.
*/
void
SoGLRenderAction::setNumPasses(const int num)
{
  THIS->numpasses = num;
}

/*!
  Returns the number of rendering passes done on updates.
*/
int
SoGLRenderAction::getNumPasses(void) const
{
  return THIS->numpasses;
}

/*!
  Sets whether each pass should render to screen or not.
*/
void
SoGLRenderAction::setPassUpdate(const SbBool flag)
{
  THIS->passupdate = flag;
}

/*!
  Returns the value of the "show intermediate updates" flag.

  \sa setPassUpdate()
*/
SbBool
SoGLRenderAction::isPassUpdate(void) const
{
  return THIS->passupdate;
}

/*!
  Sets the pass callback.  The callback is called between each
  rendering pass.
*/
void
SoGLRenderAction::setPassCallback(SoGLRenderPassCB * const func,
                                  void * const userdata)
{
  THIS->passcallback = func;
  THIS->passcallbackdata = userdata;
}

/*!
  Sets the OpenGL cache context key, which is used for deciding when
  to share OpenGL display lists.

  \sa SoGLCacheContextElement::getUniqueCacheContext()
*/
void
SoGLRenderAction::setCacheContext(const uint32_t context)
{
  if (context != THIS->cachecontext) {
    THIS->cachecontext = context;
    this->invalidateState();
  }
}

/*!
  Returns the cache context key for this rendering action instance.
*/
uint32_t
SoGLRenderAction::getCacheContext(void) const
{
  return THIS->cachecontext;
}

// Documented in superclass. Overridden from parent class to
// initialize the OpenGL state.
void
SoGLRenderAction::beginTraversal(SoNode * node)
{
  if (THIS->isrendering) {
    inherited::beginTraversal(node);
    return;
  }
  if (THIS->needglinit) {
    THIS->needglinit = FALSE;

    // we are always using GL_COLOR_MATERIAL in Coin
    glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    // force blending to off in case GL state is invalid
    THIS->disableBlend(TRUE);
    THIS->gltransptype = SoGLRenderAction::NONE;

    if (THIS->smoothing) {
      glEnable(GL_POINT_SMOOTH);
      glEnable(GL_LINE_SMOOTH);
    }
    else {
      glDisable(GL_POINT_SMOOTH);
      glDisable(GL_LINE_SMOOTH);
    }
  }
  THIS->render(node);
}

/*!
  Overloaded from parent class to clean up the lists of objects which
  were included in the delayed rendering.
 */
void
SoGLRenderAction::endTraversal(SoNode * node)
{
  inherited::endTraversal(node);
}

/*!
  Used by shape nodes or others which need to know whether or not they
  should immediately render themselves or if they should wait until
  the next pass.
*/
SbBool
SoGLRenderAction::handleTransparency(SbBool istransparent)
{
  SoGLRenderAction::TransparencyType transptype = 
    (SoGLRenderAction::TransparencyType)
    SoShapeStyleElement::getTransparencyType(this->getState());
  
  if (transptype == SoGLRenderAction::SCREEN_DOOR) {
    if (THIS->smoothing) {
      THIS->setupTransparency(transptype);
      THIS->enableBlend();
    }
    else THIS->disableBlend();
    return FALSE;
  }

  // check common case first
  if (!istransparent || transptype == SoGLRenderAction::NONE) {
    THIS->disableBlend();
    return FALSE;
  }
  // for the transparency render pass(es) we should always render when
  // we get here. blending will be enabled.
  if (THIS->transparencyrender) {
    THIS->setupTransparency(transptype);
    return FALSE;
  }

  // check for special case when rendering delayed paths.  we don't
  // want to add these objects to the list of transparent objects, but
  // render right away.
  if (THIS->delayedpathrender) {
    THIS->setupTransparency(transptype);
    if (transptype != SoGLRenderAction::SCREEN_DOOR || THIS->smoothing) THIS->enableBlend();
    else THIS->disableBlend();
    return FALSE; // always render
  }
  switch (transptype) {
  case SoGLRenderAction::ADD:
  case SoGLRenderAction::BLEND:
    THIS->setupTransparency(transptype);
    THIS->enableBlend();
    return FALSE; // render now
  case SoGLRenderAction::DELAYED_ADD:
  case SoGLRenderAction::DELAYED_BLEND:
    this->addTransPath(this->getCurPath()->copy());
    return TRUE;
  case SoGLRenderAction::SORTED_OBJECT_ADD:
  case SoGLRenderAction::SORTED_OBJECT_BLEND:
  case SoGLRenderAction::SORTED_OBJECT_SORTED_TRIANGLE_ADD:
  case SoGLRenderAction::SORTED_OBJECT_SORTED_TRIANGLE_BLEND:
    this->addTransPath(this->getCurPath()->copy());
    return TRUE; // delay render
  case SoGLRenderAction::NONE:
    assert(0 && "should not get here");
    break;
  default:
    assert(0 && "unknown transparency mode.");
  }
  return FALSE;
}

/*!
  Returns the number of the current rendering pass.
*/

int
SoGLRenderAction::getCurPass(void) const
{
  return THIS->currentpass;
}

/*!
  Returns \c TRUE if the render action should abort now based on user
  callback.

  \sa setAbortCallback()
*/
SbBool
SoGLRenderAction::abortNow(void)
{
  if (this->hasTerminated()) return TRUE;
  SbBool abort = FALSE;
  if (THIS->abortcallback) {
    switch (THIS->abortcallback(THIS->abortcallbackdata)) {
    case CONTINUE:
      break;
    case ABORT:
      this->setTerminated(TRUE);
      abort = TRUE;
      break;
    case PRUNE:
      // abort this node, but do not abort rendering
      abort = TRUE;
      break;
    case DELAY:
      this->addDelayedPath(this->getCurPath()->copy());
      // prune this node
      abort = TRUE;
      break;
    }
  }
  return abort;
}

/*!
  Let SoGLRenderAction instance know if application is running on the
  local machine or if the rendering instructions are sent over the
  network.

  This call has no effect in Coin for now, but eventually it will be
  used to optimize rendering (the displaylist caching strategy should
  be influenced by this flag to be more aggressive with the caching
  when rendering instructions are passed over the network).

  \sa getRenderingIsRemote()
 */
void
SoGLRenderAction::setRenderingIsRemote(SbBool isremote)
{
  THIS->renderingremote = isremote;
}

/*!
  Returns whether or not the application is running remotely.

  \sa setRenderingIsRemote()
 */
SbBool
SoGLRenderAction::getRenderingIsRemote(void) const
{
  return THIS->renderingremote;
}

/*!
  Adds a path to the list of paths to render after the current pass.
 */
void
SoGLRenderAction::addDelayedPath(SoPath * path)
{
  assert(!THIS->delayedpathrender);
  THIS->delayedpaths.append(path);
}

/*!
  Returns a flag indicating whether or not we are currently rendering
  from the list of delayed paths of the scene graph.
 */
SbBool
SoGLRenderAction::isRenderingDelayedPaths(void) const
{
  return THIS->delayedpathrender;
}

// Remember a path containing a transparent object for later
// rendering. We know path == this->getCurPath() when we get here.
void
SoGLRenderAction::addTransPath(SoPath * path)
{
  THIS->transpobjpaths.append(path);

  // if we're not going to sort the paths we don't need to calculate
  // distance
  if (THIS->transparencytype == BLEND ||
      THIS->transparencytype == ADD) return;

  SoNode * tail = ((SoFullPath*)path)->getTail();
  float dist;

  // test if we can calculate bbox using SoShape::computeBBox. This is
  // the common case, and quite a lot faster than using an
  // SoGetBoundingBoxAction. We only do this if no cache is currently
  // open, to avoid cache dependencies on model matrix and view
  // volume, which would be very bad for cache performance.
  if (!this->state->isCacheOpen() &&
      tail->isOfType(SoShape::getClassTypeId())) { // common case
    SbBox3f dummy;
    SbVec3f center;
    ((SoShape*)tail)->computeBBox(this, dummy, center);
    SoModelMatrixElement::get(this->state).multVecMatrix(center, center);
    dist = SoViewVolumeElement::get(this->state).getPlane(0.0f).getDistance(center);
  }
  else {
    if (THIS->bboxaction == NULL) {
      THIS->bboxaction =
        new SoGetBoundingBoxAction(SoViewportRegionElement::get(this->state));
    }
    THIS->bboxaction->setViewportRegion(SoViewportRegionElement::get(this->state));
    THIS->bboxaction->apply(path);
    SbVec3f center = THIS->bboxaction->getBoundingBox().getCenter();
    dist = SoViewVolumeElement::get(this->state).getPlane(0.0f).getDistance(center);
  }
  THIS->transpobjdistances.append(dist);
}

/*!
  Overloaded to reinitialize GL state on next apply.
*/
void
SoGLRenderAction::invalidateState(void)
{
  inherited::invalidateState();
  THIS->needglinit = TRUE;
}

// Sort paths with transparent objects before rendering.
void
SoGLRenderAction::doPathSort(void)
{
  // need to cast to SbPList to avoid ref/unref problems
  SbPList * plist = (SbPList *)&THIS->transpobjpaths;
  float * darray = (float *)THIS->transpobjdistances.getArrayPtr();

  int i, j, distance, n = THIS->transpobjdistances.getLength();
  void * ptmp;
  float dtmp;

  // shell sort algorithm (O(nlog(n))
  for (distance = 1; distance <= n/9; distance = 3*distance + 1);
  for (; distance > 0; distance /= 3) {
    for (i = distance; i < n; i++) {
      dtmp = darray[i];
      ptmp = plist->get(i);
      j = i;
      while (j >= distance && darray[j-distance] > dtmp) {
        darray[j] = darray[j-distance];
        plist->set(j, plist->get(j-distance));
        j -= distance;
      }
      darray[j] = dtmp;
      plist->set(j, ptmp);
    }
  }
}

/*!
  Adds a callback which is invoked right before the scene graph traversal
  starts. All necessary GL initialization is then done (e.g. the viewport
  is correctly set), and this callback can be useful to, for instance,
  clear the viewport before rendering, or draw a bitmap in the background
  before rendering etc.

  The callback is only invoked once (before the first rendering pass)
  when multi pass rendering is enabled.

  Please note that SoSceneManager usually adds a callback to clear the
  GL buffers in SoSceneManager::render(). So, if you plan to for
  instance draw an image in the color buffer using this callback, you
  should make sure that the scene manager doesn't clear the buffer.
  This can be done either by calling SoSceneManager::render() with
  both arguments FALSE, or, if you're using one of our GUI toolkits
  (SoXt/SoQt/SoGtk/SoWin), call setClearBeforeRender() on the viewer.

  This method is an extension versus the Open Inventor API.

  \sa removePreRenderCallback().  
*/
void
SoGLRenderAction::addPreRenderCallback(SoGLPreRenderCB * func, void * userdata)
{
  THIS->precblist.addCallback((SoCallbackListCB*) func, userdata);
}

/*!
  Removed a callback added with the addPreRenderCallback() method.

  This method is an extension versus the Open Inventor API.

  \sa addPreRenderCallback()
*/
void
SoGLRenderAction::removePreRenderCallback(SoGLPreRenderCB * func, void * userdata)
{
  THIS->precblist.removeCallback((SoCallbackListCB*) func, userdata);
}

#undef THIS

// methods in SoGLRenderActionP
#ifndef DOXYGEN_SKIP_THIS

// Enable OpenGL blending.
void
SoGLRenderActionP::enableBlend(const SbBool force)
{
  if (force || !this->isblendenabled) {
    glEnable(GL_BLEND);
    if (!this->delayedpathrender &&
        this->transparencytype != SoGLRenderAction::SCREEN_DOOR) {
      glDepthMask(GL_FALSE);
    }
    this->isblendenabled = TRUE;
  }
}

// Disable OpenGL blending.
void
SoGLRenderActionP::disableBlend(const SbBool force)
{
  if (force || this->isblendenabled) {
    glDisable(GL_BLEND);
    if (!this->delayedpathrender &&
        this->transparencytype != SoGLRenderAction::SCREEN_DOOR) {
      glDepthMask(GL_TRUE);
    }
    this->isblendenabled = FALSE;
  }
}

//
// render the scene. Called from beginTraversal()
//
void
SoGLRenderActionP::render(SoNode * node)
{
  this->isrendering = TRUE;

  this->currentpass = 0;
  
  SoState * state = this->action->getState();
  state->push();

  SoShapeStyleElement::setTransparencyType(state,
                                           this->transparencytype);
                                 
  SoViewportRegionElement::set(state, this->viewport);
  SoLazyElement::setTransparencyType(state,
                                     (int32_t)this->transparencytype);
  SoLazyElement::setBlending(state, FALSE);
  SoLazyElement::setColorMaterial(state, TRUE);

  SoGLUpdateAreaElement::set(state,
                             this->updateorigin, this->updatesize);

  SoGLCacheContextElement::set(state, this->cachecontext,
                               FALSE, this->renderingremote);
  SoGLRenderPassElement::set(state, 0);

  this->precblist.invokeCallbacks((void*) this->action);

  if (this->action->getNumPasses() > 1) {
    this->renderMulti(node);
  }
  else {
    this->renderSingle(node);
  }

  state->pop();
  this->isrendering = FALSE;
}

//
// render multiple passes (antialiasing)
//
void
SoGLRenderActionP::renderMulti(SoNode * node)
{
  assert(this->numpasses > 1);
  float fraction = 1.0f / float(this->numpasses);

  this->currentpass = 0;
  this->renderSingle(node);
  if (this->action->hasTerminated()) return;

  glAccum(GL_LOAD, fraction);

  for (int i = 1; i < this->numpasses; i++) {
    if (this->passupdate) {
      glAccum(GL_RETURN, float(this->numpasses) / float(i));
    }
    if (this->passcallback) this->passcallback(this->passcallbackdata);
    else glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    this->currentpass = i;
    this->renderSingle(node);

    if (this->action->hasTerminated()) return;
    glAccum(GL_ACCUM, fraction);
  }
  glAccum(GL_RETURN, 1.0f);
}

//
// render a single pass. Might start a transparency or delayed pass
// though.
//
void
SoGLRenderActionP::renderSingle(SoNode * node)
{
  SoState * state = this->action->getState();

  SoGLRenderPassElement::set(state, this->currentpass);
  SoGLCacheContextElement::set(state, this->cachecontext,
                               FALSE, this->renderingremote);

  assert(this->delayedpathrender == FALSE);
  assert(this->transparencyrender == FALSE);

  // truncate just in case
  this->transpobjpaths.truncate(0);
  this->transpobjdistances.truncate(0);
  this->delayedpaths.truncate(0);

  this->action->beginTraversal(node);

  if (this->transpobjpaths.getLength() && !this->action->hasTerminated()) {
    this->transparencyrender = TRUE;
    SoGLCacheContextElement::set(state, this->cachecontext,
                                 TRUE, this->renderingremote);
    this->enableBlend();

    // test if paths should rendered back-to-front
    if (this->transparencytype == SoGLRenderAction::SORTED_OBJECT_BLEND ||
        this->transparencytype == SoGLRenderAction::SORTED_OBJECT_ADD ||
        this->transparencytype == SoGLRenderAction::SORTED_OBJECT_SORTED_TRIANGLE_BLEND ||
        this->transparencytype == SoGLRenderAction::SORTED_OBJECT_SORTED_TRIANGLE_BLEND) {
      this->action->doPathSort();
      int n = this->transpobjpaths.getLength();
      for (int i  = 0; i < n; i++) {
        this->action->apply(this->transpobjpaths[i]);
      }
    }
    else {
      this->action->apply(this->transpobjpaths, TRUE);
    }
    this->disableBlend();
    this->transparencyrender = FALSE;
  }

  this->disableBlend();

  if (this->delayedpaths.getLength() && !this->action->hasTerminated()) {
    this->delayedpathrender = TRUE;
    this->action->apply(this->delayedpaths, TRUE);
    this->delayedpathrender = FALSE;
  }

  // truncate lists to unref paths.
  this->transpobjpaths.truncate(0);
  this->transpobjdistances.truncate(0);
  this->delayedpaths.truncate(0);
}

void 
SoGLRenderActionP::setupTransparency(const SoGLRenderAction::TransparencyType newtype)
{
  if (newtype != this->gltransptype) {
    this->gltransptype = newtype;
    switch(newtype) {
    case SoGLRenderAction::BLEND:
    case SoGLRenderAction::DELAYED_BLEND:
    case SoGLRenderAction::SORTED_OBJECT_BLEND:
    case SoGLRenderAction::SORTED_OBJECT_SORTED_TRIANGLE_BLEND:
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      break;
    case SoGLRenderAction::ADD:
    case SoGLRenderAction::DELAYED_ADD:
    case SoGLRenderAction::SORTED_OBJECT_ADD:
    case SoGLRenderAction::SORTED_OBJECT_SORTED_TRIANGLE_ADD:
      glBlendFunc(GL_SRC_ALPHA, GL_ONE);
      break;
    case SoGLRenderAction::SCREEN_DOOR:
      // needed for line smoothing to work properly
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      break;
    case SoGLRenderAction::NONE:
      this->disableBlend();
      break;
    }
  }
}

#endif // DOXYGEN_SKIP_THIS
