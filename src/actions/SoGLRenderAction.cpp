/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
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

// Metadon doc:
/*¡
  FIXME. This is missing from the implementation:
  <ul>

  <li> the abort callback function is never called -- it _should_ be
       called for every node during traversal.

  <li> the pass callback function is never called.

  <li> the cachecontext setting for sharing OpenGL display lists is
       not used.

  <li> the UpdateArea setting isn't heeded.

  <li> the Smoothing flag does not influence rendering.

  <li> the PassUpdate flag is not used.

  <li> antialiased rendering is not done if number of rendering passes
       is larger than 1.

  </ul>
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H
#ifdef HAVE_WINDOWS_H
#include <windows.h>
#endif // HAVE_WINDOWS_H
#include <GL/gl.h>

// *************************************************************************

/*!
  \enum SoGLRenderAction::TransparencyType

  Various settings for how to do rendering of transparent objects in
  the scene. Some of the settings will provide faster rendering, while
  others gives you better quality rendering.
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

/*!
  Constructor. Sets up the render action for rendering within the
  given \a viewportregion.
*/
SoGLRenderAction::SoGLRenderAction(const SbViewportRegion & viewportregion)
{
  SO_ACTION_CONSTRUCTOR(SoGLRenderAction);

  SO_ACTION_ADD_METHOD_INTERNAL(SoNode, SoNode::GLRenderS);

  // Can't just push this on the SoViewportRegionElement stack, as the
  // state hasn't been made yet.
  this->viewport = viewportregion;

  this->passcallback = NULL;
  this->passcallbackdata = NULL;
  this->smoothing = FALSE;
  this->numpasses = 1;
  this->transparencytype = SoGLRenderAction::SCREEN_DOOR;
  this->delayedrender = FALSE;
  this->sortrender = FALSE;
  this->isblendenabled = FALSE;
  this->passupdate = FALSE;
  this->bboxaction = NULL;
  this->updateorigin.setValue(0.0f, 0.0f);
  this->updatesize.setValue(1.0f, 1.0f);
  this->renderingremote = FALSE;
  this->abortcallback = NULL;
  this->cachecontext = 0;
  this->needglinit = TRUE;
}

/*!
  Destructor, frees up all internal resources for action instance.
*/
SoGLRenderAction::~SoGLRenderAction()
{
  delete this->bboxaction;
}

/*!
  Sets the viewport region for rendering. This will then override the
  region passed in with the constructor.
*/
void
SoGLRenderAction::setViewportRegion(const SbViewportRegion & newregion)
{
  this->viewport = newregion;

  // The SoViewportRegionElement is not set here, as it is always
  // initialized before redraw in beginTraversal().
}

/*!
  Returns the viewport region for the rendering action.
*/
const SbViewportRegion &
SoGLRenderAction::getViewportRegion(void) const
{
  return this->viewport;
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
  this->updateorigin = origin;
  this->updatesize = size;
}

/*!
  Returns information about the area of the rendering context window
  to be updated.
*/
void
SoGLRenderAction::getUpdateArea(SbVec2f & origin, SbVec2f & size) const
{
  origin = this->updateorigin;
  size = this->updatesize;
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
  this->abortcallback = func;
  this->abortcallbackdata = userdata;
}

/*!
  Sets the transparency rendering method for transparent objects in
  the scene graph.

  \sa SoGLRenderAction::TransparencyType
*/
void
SoGLRenderAction::setTransparencyType(const TransparencyType type)
{
  if (this->transparencytype != type) {
    this->transparencytype = type;
    this->needglinit = TRUE;
  }
}

/*!
  Returns the transparency rendering type.
*/
SoGLRenderAction::TransparencyType
SoGLRenderAction::getTransparencyType(void) const
{
  return this->transparencytype;
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
  if (smooth != this->smoothing) {
    this->smoothing = smooth;
    this->needglinit = TRUE;
  }
}

/*!
  Returns whether smoothing is set or not.
*/
SbBool
SoGLRenderAction::isSmoothing(void) const
{
  return this->smoothing;
}

/*!
  Sets the number of rendering passes.  Default is 1, anything greater
  will enable antialiasing.
*/
void
SoGLRenderAction::setNumPasses(const int num)
{
  this->numpasses = num;
}

/*!
  Returns the number of rendering passes done on updates.
*/
int
SoGLRenderAction::getNumPasses(void) const
{
  return this->numpasses;
}

/*!
  Sets whether each pass should render to screen or not.
*/
void
SoGLRenderAction::setPassUpdate(const SbBool flag)
{
  this->passupdate = flag;
}

/*!
  Returns the value of the "show intermediate updates" flag.

  \sa setPassUpdate()
*/
SbBool
SoGLRenderAction::isPassUpdate(void) const
{
  return this->passupdate;
}

/*!
  Sets the pass callback.  The callback is called between each
  rendering pass.
*/
void
SoGLRenderAction::setPassCallback(SoGLRenderPassCB * const func,
                                  void * const userdata)
{
  this->passcallback = func;
  this->passcallbackdata = userdata;
}

/*!
  Sets the OpenGL cache context key, which is used for deciding when
  to share OpenGL display lists.
*/
void
SoGLRenderAction::setCacheContext(const uint32_t context)
{
  if (context != this->cachecontext) {
    this->cachecontext = context;
    this->invalidateState();
  }
}

/*!
  Returns the cache context key for this rendering action instance.
*/
uint32_t
SoGLRenderAction::getCacheContext(void) const
{
  return this->cachecontext;
}

/*!
  Overloaded from parent class to initialize the OpenGL state.
*/
void
SoGLRenderAction::beginTraversal(SoNode * node)
{
  if (this->sortrender || this->delayedrender) {
    inherited::beginTraversal(node);
    return;
  }
  if (this->needglinit) {
    this->needglinit = FALSE;

    // we are always using GL_COLOR_MATERIAL in Coin
    glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    // force blending to off in case GL state is invalid
    this->disableBlend(TRUE);

    switch(this->transparencytype) {
    case BLEND:
    case DELAYED_BLEND:
    case SORTED_OBJECT_BLEND:
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      break;
    case ADD:
    case DELAYED_ADD:
    case SORTED_OBJECT_ADD:
      glBlendFunc(GL_SRC_ALPHA, GL_ONE);
      break;
    case SCREEN_DOOR:
      // needed for line smoothing to work properly
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      break;
    }
    if (this->smoothing) {
      glEnable(GL_POINT_SMOOTH);
      glEnable(GL_LINE_SMOOTH);
    }
    else {
      glDisable(GL_POINT_SMOOTH);
      glDisable(GL_LINE_SMOOTH);
    }
  }
  this->currentpass = 0;
  this->didhavetransparent = FALSE;

  if (this->transparencytype == SCREEN_DOOR && this->smoothing) {
    this->enableBlend(); // needed for line smoothing
  }
  else {
    this->disableBlend();
  }

  this->getState()->push();

  SoShapeStyleElement::setTransparencyType(this->getState(),
                                           this->transparencytype);

  SoViewportRegionElement::set(this->getState(), this->viewport);
  SoLazyElement::setTransparencyType(this->getState(),
                                     (int32_t)this->transparencytype);
  SoLazyElement::setBlending(this->getState(), FALSE);
  SoLazyElement::setColorMaterial(this->getState(), TRUE);

  // FIXME: is this the correct place to set these elements? 19990314 mortene.
  SoDecimationPercentageElement::set(this->getState(), 1.0f);
  SoDecimationTypeElement::set(this->getState(),
                               SoDecimationTypeElement::AUTOMATIC);

  // FIXME: use these as they're supposed to be used. 19990314 mortene.
  SoGLRenderPassElement::set(this->getState(), 0);
  SoGLUpdateAreaElement::set(this->getState(),
                             SbVec2f(0.0f, 0.0f), SbVec2f(1.0f, 1.0f));

  SoGLCacheContextElement::set(this->getState(), this->cachecontext,
                               FALSE, this->renderingremote);

  inherited::beginTraversal(node);

  if (this->didhavetransparent && !this->sortrender) {
    if (this->transparencytype == DELAYED_BLEND ||
        this->transparencytype == DELAYED_ADD) {
      this->currentpass = 1;
      SoGLRenderPassElement::set(this->getState(), 1);
      SoGLCacheContextElement::set(this->getState(), this->cachecontext,
                                   TRUE, this->renderingremote);
      this->enableBlend();
      inherited::beginTraversal(node);
      this->disableBlend();
    }
    else if (this->transparencytype == SORTED_OBJECT_BLEND ||
             this->transparencytype == SORTED_OBJECT_ADD) {
      SoGLCacheContextElement::set(this->getState(), this->cachecontext,
                                   TRUE, this->renderingremote);
      this->sortrender = TRUE;
      this->doPathSort();
      this->enableBlend();
      this->apply(this->transpobjpaths, TRUE);
      this->disableBlend();
    }
  }

  this->disableBlend();

  if (this->delayedpaths.getLength()) {
    if (!this->delayedrender) {
      this->delayedrender = TRUE;
      this->apply(this->delayedpaths, TRUE);
    }
  }
  this->getState()->pop();
}

/*!
  Overloaded from parent class to clean up the lists of objects which
  were included in the delayed rendering.
 */
void
SoGLRenderAction::endTraversal(SoNode *)
{
  if (this->delayedpaths.getLength() && this->delayedrender) {
    this->delayedrender = FALSE;
    this->delayedpaths.truncate(0);
  }

  if (this->transpobjpaths.getLength() && this->sortrender) {
    this->sortrender = FALSE;
    this->transpobjpaths.truncate(0);
    this->transpobjdistances.truncate(0);
  }
}

/*!
  Used by shape nodes or others which need to know whether or not they
  should immediately render themselves or if they should wait until
  the next pass.
*/
SbBool
SoGLRenderAction::handleTransparency(SbBool istransparent)
{
  if (this->delayedrender) { // special case when rendering delayed paths
    if (!istransparent) {
      this->disableBlend();
    }
    else {
      if (this->transparencytype != SCREEN_DOOR) this->enableBlend();
      else this->disableBlend();
    }
    return FALSE; // always render
  }

  // normal case
  if (istransparent) this->didhavetransparent = TRUE;
  if (this->transparencytype == DELAYED_ADD ||
      this->transparencytype == DELAYED_BLEND) {
    if (this->currentpass == 0) return istransparent;
    else return !istransparent;
  }
  else if (this->transparencytype == SORTED_OBJECT_ADD ||
           this->transparencytype == SORTED_OBJECT_BLEND) {
    if (this->sortrender || !istransparent) return FALSE;
    this->addTransPath(this->getCurPath()->copy());
    return TRUE;
  }
  else if (this->transparencytype == ADD || this->transparencytype == BLEND) {
    if (istransparent) this->enableBlend();
    else this->disableBlend();
    return FALSE; // node should always render
  }
  else
    return FALSE; // polygon stipple used to render
}

/*!
  Returns the number of the current rendering pass.
*/

int
SoGLRenderAction::getCurPass(void) const
{
  return this->currentpass;
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
  if (this->abortcallback) {
    switch (this->abortcallback(this->abortcallbackdata)) {
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
      this->addDelayedPath((SoPath*)this->getCurPath());
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
  this->renderingremote = isremote;
}

/*!
  Returns whether or not the application is running remotely.

  \sa setRenderingIsRemote()
 */
SbBool
SoGLRenderAction::getRenderingIsRemote(void) const
{
  return this->renderingremote;
}

/*!
  Adds a path to the list of paths to render after the current pass.
 */
void
SoGLRenderAction::addDelayedPath(SoPath * path)
{
  assert(!this->delayedrender);
  SoPath * copy = path->copy();
  this->delayedpaths.append(copy);
}

/*!
  Returns a flag indicating whether or not we are currently rendering
  from the list of delayed paths of the scene graph.
 */
SbBool
SoGLRenderAction::isRenderingDelayedPaths(void) const
{
  return this->delayedrender;
}

// Remember a path containing a transparent object for later
// rendering.
void
SoGLRenderAction::addTransPath(SoPath * path)
{
  // Do this first to increase the reference count (we want to avoid a
  // zero refcount for the bboxaction apply()).
  this->transpobjpaths.append(path);

  if (this->bboxaction == NULL) {
    this->bboxaction =
      new SoGetBoundingBoxAction(SoViewportRegionElement::get(this->state));
  }
  this->bboxaction->setViewportRegion(SoViewportRegionElement::get(this->state));
  this->bboxaction->apply(path);
  SbVec3f center = this->bboxaction->getBoundingBox().getCenter();
  SoModelMatrixElement::get(this->state).multVecMatrix(center, center);
  float dist = SoViewVolumeElement::get(this->state).getPlane(0.0f).getDistance(center);
  this->transpobjdistances.append(dist);
}

// Disable OpenGL blending.
void
SoGLRenderAction::disableBlend(const SbBool force)
{
  if (force || this->isblendenabled) {
    glDisable(GL_BLEND);
    if (!this->delayedrender && this->transparencytype != SCREEN_DOOR) glDepthMask(GL_TRUE);
    this->isblendenabled = FALSE;
  }
}

/*!
  Overloaded to reinitialize GL state on next apply.
*/
void
SoGLRenderAction::invalidateState(void)
{
  inherited::invalidateState();
  this->needglinit = TRUE;
}

// Enable OpenGL blending.
void
SoGLRenderAction::enableBlend(const SbBool force)
{
  if (force || !this->isblendenabled) {
    glEnable(GL_BLEND);
    if (!this->delayedrender && this->transparencytype != SCREEN_DOOR) glDepthMask(GL_FALSE);
    this->isblendenabled = TRUE;
  }
}

// Sort paths with transparent objects before rendering.
void
SoGLRenderAction::doPathSort(void)
{
  // need to cast to SbPList to avoid ref/unref problems
  SbPList * plist = (SbPList *)&this->transpobjpaths;
  float * darray = (float *)this->transpobjdistances.getArrayPtr();

  int i, j, distance, n = this->transpobjdistances.getLength();
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
