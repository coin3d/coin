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

#include <Inventor/actions/SoGLRenderAction.h>

#include <GL/gl.h>
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
#include <Inventor/elements/SoWindowElement.h>
#include <Inventor/lists/SoEnabledElementsList.h>
#include <Inventor/misc/SoGL.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/nodes/SoNode.h>
#include <coindefs.h>

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

/*!
  \typedef typedef SoGLRenderAction::SoGLRenderPassCB(void * userdata)
  Renderpass callbacks should be of this type.
  \sa setPassCallback()
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

  SoGLRenderAction::methods->setUp();

  // Can't just push this on the SoViewportRegionElement stack, as the
  // state hasn't been made yet.
  this->viewport = viewportregion;

  this->passcallback = NULL;
  this->passcallbackdata = NULL;
  this->smoothing = FALSE;
  this->numpasses = 1;
  this->transparencytype = SoGLRenderAction::SCREEN_DOOR;
  this->firstrender = TRUE;
  this->delayedrender = FALSE;
  this->sortrender = FALSE;
  this->isblendenabled = FALSE;
  this->passupdate = FALSE;
  this->bboxaction = NULL;
  this->updateorigin.setValue(0.0f, 0.0f);
  this->updatesize.setValue(1.0f, 1.0f);
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
  this->transparencytype = type;
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
  this->smoothing = smooth;
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
  this->cachecontext = context;
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

  static SbBool first = TRUE;
  if (first) {
    first = FALSE;
    // Check and remember various GL implementation details and
    // limits.
    sogl_global_init();
  }

  if (this->firstrender) {
    this->firstrender = FALSE;
    glDisable(GL_FOG);
    glDisable(GL_TEXTURE_1D);
    glDisable(GL_LINE_STIPPLE);
    glDisable(GL_POLYGON_STIPPLE);
    glDisable(GL_CULL_FACE);
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
    glDisable(GL_LOGIC_OP);
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_CLIP_PLANE0);
    glDisable(GL_CLIP_PLANE1);
    glDisable(GL_CLIP_PLANE2);
    glDisable(GL_CLIP_PLANE3);
    glDisable(GL_CLIP_PLANE4);
    glDisable(GL_CLIP_PLANE5);
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHT1);
    glDisable(GL_LIGHT2);
    glDisable(GL_LIGHT3);
    glDisable(GL_LIGHT4);
    glDisable(GL_LIGHT5);
    glDisable(GL_LIGHT6);
    glDisable(GL_LIGHT7);
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_GEN_R);
    glDisable(GL_TEXTURE_GEN_Q);
    glDisable(GL_MAP1_VERTEX_3);
    glDisable(GL_MAP1_VERTEX_4);
    glDisable(GL_MAP1_COLOR_4);
    glDisable(GL_MAP1_INDEX);
    glDisable(GL_MAP1_NORMAL);
    glDisable(GL_MAP1_TEXTURE_COORD_1);
    glDisable(GL_MAP1_TEXTURE_COORD_2);
    glDisable(GL_MAP1_TEXTURE_COORD_3);
    glDisable(GL_MAP1_TEXTURE_COORD_4);
    glDisable(GL_MAP2_VERTEX_3);
    glDisable(GL_MAP2_VERTEX_4);
    glDisable(GL_MAP2_COLOR_4);
    glDisable(GL_MAP2_INDEX);
    glDisable(GL_MAP2_NORMAL);
    glDisable(GL_MAP2_TEXTURE_COORD_1);
    glDisable(GL_MAP2_TEXTURE_COORD_2);
    glDisable(GL_MAP2_TEXTURE_COORD_3);
    glDisable(GL_MAP2_TEXTURE_COORD_4);
    glDisable(GL_POINT_SMOOTH);
    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_SCISSOR_TEST);
    glDisable(GL_COLOR_MATERIAL);
    glDisable(GL_AUTO_NORMAL);

    // FIXME (pederb) (fix what? -- 20000305 mortene)
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // light model
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE); // GL_TRUE for higher quality

    // no texturing unless textures in model
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_NORMALIZE);

    glDisable(GL_CULL_FACE);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
    glFrontFace(GL_CCW);
    glShadeModel(GL_SMOOTH);

    // only useful while using GL_COLOR_MATERIAL
    glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    this->disableBlend(TRUE);
  }

  this->currentpass = 0;
  this->didhavetransparent = FALSE;
  this->disableBlend(); // just in case

  if (this->transparencytype == BLEND ||
      this->transparencytype == DELAYED_BLEND ||
      this->transparencytype == SORTED_OBJECT_BLEND) {
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  }
  else if (this->transparencytype == ADD ||
           this->transparencytype == DELAYED_ADD ||
           this->transparencytype == SORTED_OBJECT_ADD) {
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
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

  inherited::beginTraversal(node);

  if (this->didhavetransparent && !this->sortrender) {
    if (this->transparencytype == DELAYED_BLEND ||
        this->transparencytype == DELAYED_ADD) {
      this->currentpass = 1;
      SoGLRenderPassElement::set(this->getState(), 1);
      this->enableBlend();
      inherited::beginTraversal(node);
      this->disableBlend();
    }
    else if (this->transparencytype == SORTED_OBJECT_BLEND ||
             this->transparencytype == SORTED_OBJECT_ADD) {
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
      SbBool usedepthbuffer = glIsEnabled(GL_DEPTH_TEST);
      if (usedepthbuffer) glDisable(GL_DEPTH_TEST);
      this->delayedrender = TRUE;
      this->apply(this->delayedpaths, TRUE);
      if (usedepthbuffer) glEnable(GL_DEPTH_TEST);
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
      this->disableBlend();
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
  Returns \c TRUE if the render action should abort now.  It always
  returns \c FALSE at this moment.
*/
SbBool
SoGLRenderAction::abortNow(void)
{
  COIN_STUB(); // FIXME
  return FALSE;
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
SoGLRenderAction::addTransPath(SoPath *path)
{
  if (this->bboxaction == NULL) {
    this->bboxaction =
      new SoGetBoundingBoxAction(SoViewportRegionElement::get(this->state));
  }
  this->bboxaction->setViewportRegion(SoViewportRegionElement::get(this->state));
  this->bboxaction->apply(path);
  SbVec3f center = this->bboxaction->getBoundingBox().getCenter();
  SoModelMatrixElement::get(this->state).multVecMatrix(center, center);
  float dist = SoViewVolumeElement::get(this->state).getPlane(0.0f).getDistance(center);
  this->transpobjpaths.append(path);
  this->transpobjdistances.append(dist);
}

// Disable OpenGL blending.
void
SoGLRenderAction::disableBlend(const SbBool force)
{
  if (force || this->isblendenabled) {
    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
    this->isblendenabled = FALSE;
  }
}

// Enable OpenGL blending.
void
SoGLRenderAction::enableBlend(const SbBool force)
{
  if (force || !this->isblendenabled) {
    glEnable(GL_BLEND);
    glDepthMask(GL_FALSE);
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
