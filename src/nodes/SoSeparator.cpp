/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2004 by Systems in Motion.  All rights reserved.
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
 *  See <URL:http://www.coin3d.org/> for more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

/*!
  \class SoSeparator SoSeparator.h Inventor/nodes/SoSeparator.h
  \brief The SoSeparator class is a state-preserving group node.
  \ingroup nodes

  Subgraphs parented by SoSeparator nodes will not affect the state,
  as they push and pop the traversal state before and after traversal
  of its children.

  SoSeparator nodes also provides options for traversal optimalization
  through the use of caching.

  \sa SoTransformSeparator
*/

#include <Inventor/nodes/SoSeparator.h>

#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/actions/SoHandleEventAction.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoAudioRenderAction.h>
#include <Inventor/caches/SoBoundingBoxCache.h>
#include <Inventor/caches/SoGLCacheList.h>
#include <Inventor/elements/SoCacheElement.h>
#include <Inventor/elements/SoCullElement.h>
#include <Inventor/elements/SoLocalBBoxMatrixElement.h>
#include <Inventor/elements/SoSoundElement.h>
#include <Inventor/misc/SoChildList.h>
#include <Inventor/misc/SoGL.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/errors/SoDebugError.h>

#include <Inventor/C/tidbits.h> // coin_getenv()
#include <stdlib.h> // strtol(), rand()
#include <limits.h> // LONG_MIN, LONG_MAX

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/system/gl.h>
#include <coindefs.h> // COIN_OBSOLETED()

#ifdef COIN_THREADSAFE
#include <Inventor/threads/SbMutex.h>
#include <Inventor/threads/SbStorage.h>
#endif // COIN_THREADSAFE

#if COIN_DEBUG
#define GLCACHE_DEBUG 0 // set to 1 to debug caching
#endif

// environment variable
static int COIN_RANDOMIZE_RENDER_CACHING = -1;

// Maximum number of caches available for allocation for the
// rendercaching.
int SoSeparator::numrendercaches = 2;


/*!
  \enum SoSeparator::CacheEnabled

  Enumeration of flags for the fields deciding which optimalizations
  to do in SoSeparator nodes. There are two types of settings
  available: caching policies or culling policies. See doumentation of
  fields.
*/
/*!
  \var SoSeparator::CacheEnabled SoSeparator::OFF
  No caching.
*/
/*!
  \var SoSeparator::CacheEnabled SoSeparator::ON
  Always try to cache state.
*/
/*!
  \var SoSeparator::CacheEnabled SoSeparator::AUTO
  Use heuristics to try to figure out the optimal caching policy.
*/


/*!
  \var SoSFEnum SoSeparator::renderCaching

  Policy for caching of rendering instructions for faster
  execution. This will typically use the OpenGL \e displaylist
  mechanism.

  Default value is SoSeparator::AUTO.

  If you know that some parts of your scene will never change,
  rendering might happen faster if you explicitly set this field to
  SoSeparator::ON. If you on the other hand know that parts of the
  scene will change a lot (like for every redraw), it will be
  beneficial to set this field to SoSeparator::OFF for the top-level
  separator node of this (sub)graph.

  Usually the default setting of \c AUTO will handle any scene very
  well. The advantages that \e can be had from setting
  SoSeparator::renderCaching to \c ON are:

  <ul>
   <li> If you positively know that the geometry under the SoSeparator is
     static, you get the cache set up right away.

     Otherwise, the code in Coin will do a bit of testing and decide
     by some heuristics whether or not to enable it. That will make
     the rendering be a tiny bit slower right after start-up than with
     renderCaching set to \c ON.

     (The slow-down should hardly be noticable, though, so we don't
     advice application programmers to do this.)
   </li>

   <li> For many of the shape nodes that can contain many basic
     primitives, like e.g. SoFaceSet, SoIndexedFaceSet, SoLineSet, etc
     etc, there is an internal threshold for how many primitives a
     node can contain before we don't do caching when
     SoSeparator::renderCaching is set to \c AUTO.

     The reason we do this is because OpenGL renderlists can
     potentially suck up a lot of memory resources on the graphics
     card.

     But if you know that it will be advantageous on your particular
     platform, you can override this by setting
     SoSeparator::renderCaching equal to \c ON.

     (We don't advice application programmers to do this either. A
     better solution in these cases would simply be to get in touch
     with SIM and describe the platform and the problem, and we could
     integrate a proper fix into Coin.)
   </li>
  </ul>

  There are good reasons for setting renderCaching to \c OFF, like
  when you know the geometry will be changing a lot. Still, Coin
  should work fairly well without even this optimization.  (If
  renderCaching is \c AUTO over a sub-graph with changing geometry or
  other cache smashing nodes, the caching heuristics will stop the
  SoSeparator node from trying to make caches -- at least after a few
  tries has been made and failed.)


  The short story about how auto-caching works is as follows:

  <ul>

    <li>For vertex-based shapes with fewer than 100 triangles and
        where the geometry is detected to be fairly static, caching is
        enabled.</li>

    <li>For shapes with more than 1000 trangles, it is disabled, to
        avoid spending too much of the on-board graphics card's memory
        resources.</li>

    <li>For shapes with between 100 and 1000 shapes, displaylist
        caching will be turned on if our heuristics decides that the
        geometry can be considered static.</li>

  </ul>

  The maximum threshold (of 1000) is higher when doing remote
  rendering (as when rendering from one X11-based system to another).

  Disabling the displaylist caching takes precedence over enabling, so
  if you have an SoSeparator with a shape with more than 1000
  triangles and a shape with fewer than 100 triangles, caching will be
  disabled for the SoSeparator.

  It's possible to tune the limits using some environment variables:

  <ul>
 
    <li>\c COIN_AUTOCACHE_LOCAL_MIN can be used to change the
        enable-caching limit, while \c COIN_AUTOCACHE_LOCAL_MAX
        controls the disable-caching limit.</li>

    <li>The corresponding variables for remote rendering are \c
        COIN_AUTOCACHE_REMOTE_MIN and \c
        COIN_AUTOCACHE_REMOTE_MAX.</li>

  </ul>
*/

/*!
  \var SoSFEnum SoSeparator::boundingBoxCaching

  Policy for caching bounding box calculations. Default value is
  SoSeparator::AUTO.

  See also documentation for SoSeparator::renderCaching.
*/
/*!
  \var SoSFEnum SoSeparator::renderCulling

  Policy for doing viewport culling during rendering
  traversals. Default value is SoSeparator::AUTO.

  When the render culling is turned off for Coin, it will be left to
  be done for the underlying immediate mode rendering library. This
  will often be faster than doing culling from within Coin, so be
  careful to monitor the change in execution speed if setting this
  field to SoSeparator::ON.

  See also documentation for SoSeparator::renderCaching.
*/
/*!
  \var SoSFEnum SoSeparator::pickCulling

  Policy for doing viewport culling during pick traversals. Default
  value is SoSeparator::AUTO.

  See documentation for SoSeparator::renderCulling.
*/

#ifdef COIN_THREADSAFE
// when doing threadsafe rendering, each thread needs its own
// glcachelist
typedef struct {
  SoGLCacheList * glcachelist;
} soseparator_storage;

static void
soseparator_storage_construct(void * data)
{
  soseparator_storage * ptr = (soseparator_storage*) data;
  ptr->glcachelist = NULL;
}

static void
soseparator_storage_destruct(void * data)
{
  soseparator_storage * ptr = (soseparator_storage*) data;
  delete ptr->glcachelist;
}
#endif // COIN_THREADSAFE

class SoSeparatorP {
public:
  // lots of ifdefs here but it can't be helped...
  SoSeparatorP(void) {
#ifdef COIN_THREADSAFE
    this->glcachestorage =
      new SbStorage(sizeof(soseparator_storage),
                    soseparator_storage_construct,
                    soseparator_storage_destruct);
#else // COIN_THREADSAFE
    this->glcachelist = NULL;
#endif // !COIN_THREADSAFE
  }
  ~SoSeparatorP() {
#ifdef COIN_THREADSAFE
    delete this->glcachestorage;
#else // COIN_THREADSAFE
    delete this->glcachelist;
#endif // COIN_THREADSAFE
  }
  SoBoundingBoxCache * bboxcache;
#ifdef COIN_THREADSAFE
  SbMutex mutex;
  SbStorage * glcachestorage;
  static void invalidate_gl_cache(void * tls, void *) {
    soseparator_storage * ptr = (soseparator_storage*) tls;
    if (ptr->glcachelist) {
      ptr->glcachelist->invalidateAll();
    }
  }
#else // COIN_THREADSAFE
  SoGLCacheList * glcachelist;
#endif // !COIN_THREADSAFE

  enum { YES, NO, MAYBE } hassoundchild;

public:
  SoGLCacheList * getGLCacheList(SbBool createifnull);

  void invalidateGLCaches(void) {
#ifdef COIN_THREADSAFE
    glcachestorage->applyToAll(invalidate_gl_cache, NULL);
#else // COIN_THREADSAFE
    if (this->glcachelist) {
      this->glcachelist->invalidateAll();
    }
#endif // !COIN_THREADSAFE
  }

  void lock(void) {
#ifdef COIN_THREADSAFE
    this->mutex.lock();
#endif // COIN_THREADSAFE
  }
  void unlock(void) {
#ifdef COIN_THREADSAFE
    this->mutex.unlock();
#endif // COIN_THREADSAFE
  }
};

#ifdef COIN_THREADSAFE
SoGLCacheList *
SoSeparatorP::getGLCacheList(SbBool createifnull)
{
  soseparator_storage * ptr =
    (soseparator_storage*) this->glcachestorage->get();
  if (createifnull && ptr->glcachelist == NULL) {
    ptr->glcachelist = new SoGLCacheList(SoSeparator::getNumRenderCaches());
  }
  return ptr->glcachelist;
}
#else // COIN_THREADSAFE
SoGLCacheList *
SoSeparatorP::getGLCacheList(SbBool createifnull)
{
  if (createifnull && this->glcachelist == NULL) {
    this->glcachelist = new SoGLCacheList(SoSeparator::getNumRenderCaches());
  }
  return this->glcachelist;
}
#endif // !COIN_THREADSAFE

// *************************************************************************

#define PRIVATE(obj) ((obj)->pimpl)

SO_NODE_SOURCE(SoSeparator);

/*!
  Default constructor.
*/
SoSeparator::SoSeparator(void)
{
  this->commonConstructor();
}

/*!
  Constructor.

  The argument should be the approximate number of children which is
  expected to be inserted below this node. The number need not be
  exact, as it is only used as a hint for better memory resource
  allocation.
*/
SoSeparator::SoSeparator(const int nchildren)
  : SoGroup(nchildren)
{
  this->commonConstructor();
}

// private common constructor helper function
void
SoSeparator::commonConstructor(void)
{
  PRIVATE(this) = new SoSeparatorP;

  SO_NODE_INTERNAL_CONSTRUCTOR(SoSeparator);

  SO_NODE_ADD_FIELD(renderCaching, (SoSeparator::AUTO));
  SO_NODE_ADD_FIELD(boundingBoxCaching, (SoSeparator::AUTO));
  SO_NODE_ADD_FIELD(renderCulling, (SoSeparator::AUTO));
  SO_NODE_ADD_FIELD(pickCulling, (SoSeparator::AUTO));

  SO_NODE_DEFINE_ENUM_VALUE(CacheEnabled, ON);
  SO_NODE_DEFINE_ENUM_VALUE(CacheEnabled, OFF);
  SO_NODE_DEFINE_ENUM_VALUE(CacheEnabled, AUTO);

  SO_NODE_SET_SF_ENUM_TYPE(renderCaching, CacheEnabled);
  SO_NODE_SET_SF_ENUM_TYPE(boundingBoxCaching, CacheEnabled);
  SO_NODE_SET_SF_ENUM_TYPE(renderCulling, CacheEnabled);
  SO_NODE_SET_SF_ENUM_TYPE(pickCulling, CacheEnabled);

  const char * maxcachesstr = coin_getenv("IV_SEPARATOR_MAX_CACHES");
  if (maxcachesstr) {
    long int maxcaches = strtol(maxcachesstr, NULL, 10);
    if ((maxcaches == LONG_MIN) || (maxcaches == LONG_MAX) || (maxcaches < 0)) {
      SoDebugError::post("SoSeparator::commonConstructor",
                         "Environment variable IV_SEPARATOR_MAX_CACHES "
                         "has invalid setting \"%s\"", maxcachesstr);
    }
    else {
      SoSeparator::setNumRenderCaches(maxcaches);
    }
  }

  PRIVATE(this)->bboxcache = NULL;

  // This environment variable used for local stability / robustness /
  // correctness testing of the render caching. If set >= 1,
  // renderCaching will be set to "ON" with a probability of 0.5 for
  // every SoSeparator instantiated.
  if (COIN_RANDOMIZE_RENDER_CACHING < 0) {
    const char * env = coin_getenv("COIN_RANDOMIZE_RENDER_CACHING");
    if (env) COIN_RANDOMIZE_RENDER_CACHING = atoi(env);
    else COIN_RANDOMIZE_RENDER_CACHING = 0;
  }
  if (COIN_RANDOMIZE_RENDER_CACHING > 0) {
    if (rand() > (RAND_MAX/2)) { this->renderCaching = SoSeparator::ON; }
  }

  PRIVATE(this)->hassoundchild = SoSeparatorP::MAYBE;
}

/*!
  Destructor. Frees resources used to implement caches.
*/
SoSeparator::~SoSeparator()
{
  if (PRIVATE(this)->bboxcache) PRIVATE(this)->bboxcache->unref();
  delete PRIVATE(this);
}

// Doc from superclass.
void
SoSeparator::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoSeparator, SO_FROM_INVENTOR_1|SoNode::VRML1);

  SO_ENABLE(SoGetBoundingBoxAction, SoCacheElement);
  SO_ENABLE(SoGLRenderAction, SoCacheElement);
}

// Doc from superclass.
void
SoSeparator::doAction(SoAction * action)
{
  action->getState()->push();
  inherited::doAction(action);
  action->getState()->pop();
}

// Doc from superclass.
void
SoSeparator::getBoundingBox(SoGetBoundingBoxAction * action)
{
  SoState * state = action->getState();

  SbXfBox3f childrenbbox;
  SbBool childrencenterset;
  SbVec3f childrencenter;

  // FIXME: AUTO is interpreted as ON for the boundingBoxCaching
  // field, but we should trigger some heuristics based on scene graph
  // "behavior" in the children subgraphs if the value is set to
  // AUTO. 19990513 mortene.
  SbBool iscaching = this->boundingBoxCaching.getValue() != OFF;

  switch (action->getCurPathCode()) {
  case SoAction::IN_PATH:
    // can't cache if we're not traversing all children
    iscaching = FALSE;
    break;
  case SoAction::OFF_PATH:
    return; // no need to do any more work
  case SoAction::BELOW_PATH:
  case SoAction::NO_PATH:
    // check if this is a normal traversal
    if (action->isInCameraSpace() || action->isResetPath()) iscaching = FALSE;
    break;
  default:
    iscaching = FALSE;
    assert(0 && "unknown path code");
    break;
  }

  SbBool validcache = iscaching && PRIVATE(this)->bboxcache && PRIVATE(this)->bboxcache->isValid(state);

  if (iscaching && validcache) {
    SoCacheElement::addCacheDependency(state, PRIVATE(this)->bboxcache);
    childrenbbox = PRIVATE(this)->bboxcache->getBox();
    childrencenterset = PRIVATE(this)->bboxcache->isCenterSet();
    childrencenter = PRIVATE(this)->bboxcache->getCenter();
    if (PRIVATE(this)->bboxcache->hasLinesOrPoints()) {
      SoBoundingBoxCache::setHasLinesOrPoints(state);
    }
  }
  else {
    SbXfBox3f abox = action->getXfBoundingBox();

    SbBool storedinvalid = FALSE;
    if (iscaching) {
      storedinvalid = SoCacheElement::setInvalid(FALSE);
    }
    state->push();

    if (iscaching) {
      // lock before changing the bboxcache pointer so that the notify()
      // function can be used by another thread.
      PRIVATE(this)->lock();
      // if we get here, we know bbox cache is not created or is invalid
      if (PRIVATE(this)->bboxcache) PRIVATE(this)->bboxcache->unref();
      PRIVATE(this)->bboxcache = new SoBoundingBoxCache(state);
      PRIVATE(this)->bboxcache->ref();
      PRIVATE(this)->unlock();
      // set active cache to record cache dependencies
      SoCacheElement::set(state, PRIVATE(this)->bboxcache);
    }

    SoLocalBBoxMatrixElement::makeIdentity(state);
    action->getXfBoundingBox().makeEmpty();
    inherited::getBoundingBox(action);

    childrenbbox = action->getXfBoundingBox();
    childrencenterset = action->isCenterSet();
    if (childrencenterset) childrencenter = action->getCenter();

    action->getXfBoundingBox() = abox; // reset action bbox

    if (iscaching) {
      PRIVATE(this)->bboxcache->set(childrenbbox, childrencenterset, childrencenter);
    }
    state->pop();
    if (iscaching) SoCacheElement::setInvalid(storedinvalid);
  }

  if (!childrenbbox.isEmpty()) {
    action->extendBy(childrenbbox);
    if (childrencenterset) {
      // FIXME: shouldn't this assert() hold up? Investigate. 19990422 mortene.
#if 0 // disabled
      assert(!action->isCenterSet());
#else
      action->resetCenter();
#endif
      action->setCenter(childrencenter, TRUE);
    }
  }
}

// Doc from superclass.
void
SoSeparator::callback(SoCallbackAction * action)
{
  SoState * state = action->getState();
  state->push();
  // culling planes should normally not be set, but can be set
  // manually by the application programmer to optimize callback
  // action traversal.

  if (!this->cullTest(state)) {
    SoGroup::callback(action);
  }
  state->pop();
}

// Doc from superclass.
void
SoSeparator::GLRender(SoGLRenderAction * action)
{
  switch (action->getCurPathCode()) {
  case SoAction::NO_PATH:
  case SoAction::BELOW_PATH:
    this->GLRenderBelowPath(action);
    break;
  case SoAction::OFF_PATH:
    // do nothing. Separator will reset state.
    break;
  case SoAction::IN_PATH:
    this->GLRenderInPath(action);
    break;
  }
}

/*!
  SGI Open Inventor v2.1 obsoleted support for
  SoGLRenderAction::addMethod().  Instead, GLRender() might be called
  directly, and to optimize traversal, the SoSeparator node calls
  GLRenderBelowPath whenever the path code is BELOW_PATH or NO_PATH
  (path code is guaranteed not to change). To be compatible with SGI's
  Inventor (and thereby also TGS') we have chosen to follow their
  implementation in this respect.

  SoSeparator::GLRenderBelowPath() do not traverse its children using
  SoChildList::traverse(), but calls GLRenderBelowPath() directly
  for all its children.
*/
void
SoSeparator::GLRenderBelowPath(SoGLRenderAction * action)
{
  SoState * state = action->getState();
  state->push();
  SbBool didcull = FALSE;

  SoGLCacheList * createcache = NULL;
  if ((this->renderCaching.getValue() != OFF) &&
      (SoSeparator::getNumRenderCaches() > 0)) {

    // test if bbox is outside view-volume
    if (!state->isCacheOpen()) {
      didcull = TRUE;
      if (this->cullTest(state)) {
        state->pop();
        return;
      }
    }
    PRIVATE(this)->lock();
    SoGLCacheList * glcachelist = PRIVATE(this)->getGLCacheList(TRUE);
    PRIVATE(this)->unlock();
    if (glcachelist->call(action)) {
#if GLCACHE_DEBUG && 1 // debug
      SoDebugError::postInfo("SoSeparator::GLRenderBelowPath",
                             "Executing GL cache: %p", this);
#endif // debug
      state->pop();
      return;
    }
    if (!SoCacheElement::anyOpen(state)) {
#if GLCACHE_DEBUG // debug
      SoDebugError::postInfo("SoSeparator::GLRenderBelowPath",
                             "Creating GL cache: %p", this);
#endif // debug
      createcache = glcachelist;
    }
  }

  if (createcache) createcache->open(action, this->renderCaching.getValue() == AUTO);

  SbBool outsidefrustum =
    (createcache || state->isCacheOpen() || didcull) ?
    FALSE : this->cullTest(state);
  if (createcache || !outsidefrustum) {
    int n = this->children->getLength();
    SoNode ** childarray = (SoNode**) this->children->getArrayPtr();
    action->pushCurPath();
    for (int i = 0; i < n && !action->hasTerminated(); i++) {
      action->popPushCurPath(i, childarray[i]);
      if (action->abortNow()) {
        // only cache if we do a full traversal
        SoCacheElement::invalidate(state);
        break;
      }
      childarray[i]->GLRenderBelowPath(action);

#if COIN_DEBUG
      // The GL error test is default disabled for this optimized
      // path.  If you get a GL error reporting an error in the
      // Separator node, enable this code by setting the environment
      // variable COIN_GLERROR_DEBUGGING to "1" to see exactly which
      // node caused the error.
      static SbBool chkglerr = sogl_glerror_debugging();
      if (chkglerr) {
        GLenum err = glGetError();
        if (err != GL_NO_ERROR) {
          SoDebugError::post("SoSeparator::GLRenderBelowPath",
                             "GL error: %s, nodetype: %s",
                             sogl_glerror_string(err).getString(),
                             (*this->children)[i]->getTypeId().getName().getString());
        }
      }
#endif // COIN_DEBUG
    }
    action->popCurPath();
  }
  state->pop();
  if (createcache) {
    createcache->close(action);
  }
}

// Doc from superclass.
void
SoSeparator::GLRenderInPath(SoGLRenderAction * action)
{
  int numindices;
  const int * indices;
  if (action->getPathCode(numindices, indices) == SoAction::IN_PATH) {
    SoState * state = action->getState();
    SoNode ** childarray = (SoNode**) this->children->getArrayPtr();
    state->push();
    int childidx = 0;
    for (int i = 0; i < numindices; i++) {
      SoAction::PathCode pathcode = action->getCurPathCode();
      for (; childidx < indices[i] && !action->hasTerminated(); childidx++) {
        SoNode * offpath = childarray[childidx];
        if (offpath->affectsState()) {
          action->pushCurPath(childidx, offpath);
          if (!action->abortNow()) {
            offpath->GLRenderOffPath(action);
          }
          else {
            SoCacheElement::invalidate(state);
          }
          action->popCurPath(pathcode);
        }
      }
      SoNode * inpath = childarray[childidx];
      action->pushCurPath(childidx, inpath);
      if (!action->abortNow()) {
        inpath->GLRenderInPath(action);
      }
      else {
        SoCacheElement::invalidate(state);
      }
      action->popCurPath(pathcode);
      childidx++;
    }
    state->pop();
  }
  else {
    // we got to the end of the path
    assert(action->getCurPathCode() == SoAction::BELOW_PATH);
    this->GLRenderBelowPath(action);
  }
}

// Doc from superclass.
void
SoSeparator::GLRenderOffPath(SoGLRenderAction *)
{
  // do nothing, since all state changes will be reset by the separator
}

// Doc from superclass.
void
SoSeparator::handleEvent(SoHandleEventAction * action)
{
  SoSeparator::doAction(action);
}

// Doc from superclass.
void
SoSeparator::audioRender(SoAudioRenderAction * action)
{
  // Note: This function is similar to SoVRMLGroup::audioRender().
  /* FIXME: how should we handle termination of an action? We should
     probably reset PRIVATE(this)->hassoundchild to MAYBE. Investigate
     2003-01-31 thammer. */

  int numindices;
  const int * indices;
  SoState * state = action->getState();
  if (PRIVATE(this)->hassoundchild != SoSeparatorP::NO) {
    if (action->getPathCode(numindices, indices) != SoAction::IN_PATH) {
      action->getState()->push();
      SoSoundElement::setSceneGraphHasSoundNode(state, this, FALSE);
      inherited::doAction(action);
      PRIVATE(this)->hassoundchild = SoSoundElement::sceneGraphHasSoundNode(state) ?
        SoSeparatorP::YES : SoSeparatorP::NO;
      action->getState()->pop();
    } else {
      SoSeparator::doAction((SoAction*)action);
    }
  }
}

// compute object space ray and test for intersection
static SbBool
ray_intersect(SoRayPickAction * action, const SbBox3f &box)
{
  if (box.isEmpty()) return FALSE;
  action->setObjectSpace();
  return action->intersect(box, TRUE);
}

// Doc from superclass.
void
SoSeparator::rayPick(SoRayPickAction * action)
{
  if (this->pickCulling.getValue() == OFF ||
      !PRIVATE(this)->bboxcache || !PRIVATE(this)->bboxcache->isValid(action->getState()) ||
      !action->hasWorldSpaceRay() ||
      ray_intersect(action, PRIVATE(this)->bboxcache->getProjectedBox())) {
    SoSeparator::doAction(action);
  }
}

// Doc from superclass.
void
SoSeparator::search(SoSearchAction * action)
{
  // Include this node in the search. (_Don't_ use the
  // SoGroup::search(), as it will also traverse all children.)
  SoNode::search(action);

  if (action->isFound()) return;

  SoSeparator::doAction(action);
}

// Doc from superclass.
void
SoSeparator::getMatrix(SoGetMatrixAction * action)
{
  int numindices;
  const int * indices;
  if (action->getPathCode(numindices, indices) == SoAction::IN_PATH) {
    this->children->traverseInPath(action, numindices, indices);
  }
}

/*!
  Set the maximum number of caches that SoSeparator nodes may allocate
  for render caching.

  This is a global value which will be used for all SoSeparator nodes,
  but the value indicate the maximum number \e per SoSeparator node.

  More caches might give better performance, but will use more memory.
  The built-in default value is 2.

  The value can also be changed globally by setting the host system's
  environment variable IV_SEPARATOR_MAX_CACHES to the wanted
  number. This is primarily meant as an aid during debugging, to make
  it easy to turn off rendercaching completely (by setting
  "IV_SEPARATOR_MAX_CACHES=0") without having to change any
  application code.
*/
void
SoSeparator::setNumRenderCaches(const int howmany)
{
  SoSeparator::numrendercaches = howmany;
}

/*!
  Returns maximum number of caches SoSeparator nodes are allowed to
  use for render caching.

  \sa setNumRenderCaches()
*/
int
SoSeparator::getNumRenderCaches(void)
{
  return SoSeparator::numrendercaches;
}

// Doc from superclass.
SbBool
SoSeparator::affectsState(void) const
{
  // Subgraphs parented by SoSeparator nodes will not affect the
  // state, as they push and pop the traversal state before and after
  // traversal of its children.
  return FALSE;
}

// Doc from superclass.
void
SoSeparator::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  SoSeparator::doAction((SoAction *)action);
}

// Doc from superclass.
void
SoSeparator::notify(SoNotList * nl)
{
  inherited::notify(nl);

  // lock before using the cache pointers so that we know the pointers
  // are valid while reading them
  PRIVATE(this)->lock();
  if (PRIVATE(this)->bboxcache) PRIVATE(this)->bboxcache->invalidate();
  PRIVATE(this)->invalidateGLCaches();
  PRIVATE(this)->hassoundchild = SoSeparatorP::MAYBE;
  PRIVATE(this)->unlock();
}

/*!
  This is an internal Open Inventor method. We've implemented
  view frustum culling in a different manner. Let us know if
  you need this function, and we'll consider implementing it.
*/
SbBool
SoSeparator::cullTest(SoGLRenderAction * action, int & cullresults)
{
  COIN_OBSOLETED();
  return FALSE;
}

// Doc from superclass.
SbBool
SoSeparator::readInstance(SoInput * in, unsigned short flags)
{
  return inherited::readInstance(in, flags);
}

/*!
  Internal method which do view frustum culling. For now, view frustum
  culling is performed if the renderCulling field is \c AUTO or \c ON,
  and the bounding box cache is valid.

  Returns \c TRUE if this separator is outside view frustum, \c FALSE
  if inside.
*/
SbBool
SoSeparator::cullTest(SoState * state)
{
  if (this->renderCulling.getValue() == SoSeparator::OFF) return FALSE;
  if (SoCullElement::completelyInside(state)) return FALSE;

  SbBool outside = FALSE;
  if (PRIVATE(this)->bboxcache &&
      PRIVATE(this)->bboxcache->isValid(state)) {
    const SbBox3f & bbox = PRIVATE(this)->bboxcache->getProjectedBox();
    if (!bbox.isEmpty()) {
      outside = SoCullElement::cullBox(state, bbox);
    }
  }
  return outside;
}

//
//  Performs a cull test on this node. This call will not update
//  the SoCullElement, so it can be called without calling
//  state->push() first.
//
SbBool
SoSeparator::cullTestNoPush(SoState * state)
{
  if (this->renderCulling.getValue() == SoSeparator::OFF) return FALSE;
  if (SoCullElement::completelyInside(state)) return FALSE;

  SbBool outside = FALSE;
  if (PRIVATE(this)->bboxcache &&
      PRIVATE(this)->bboxcache->isValid(state)) {
    const SbBox3f & bbox = PRIVATE(this)->bboxcache->getProjectedBox();
    if (!bbox.isEmpty()) {
      outside = SoCullElement::cullTest(state, bbox);
    }
  }
  return outside;
}

#undef PRIVATE
