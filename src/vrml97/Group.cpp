/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 2001 by Systems in Motion. All rights reserved.
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
  \class SoVRMLGroup SoVRMLGroup.h Inventor/VRMLnodes/SoVRMLGroup.h
  \brief The SoVRMLGroup class is used for grouping VRML nodes.
  \ingroup VRMLnodes

  \WEB3DCOPYRIGHT

  \verbatim
  Group {
    eventIn      MFNode  addChildren
    eventIn      MFNode  removeChildren
    exposedField MFNode  children      []
    field        SFVec3f bboxCenter    0 0 0     # (-inf,inf)
    field        SFVec3f bboxSize      -1 -1 -1  # (0,inf) or -1,-1,-1
  }
  \endverbatim

  A Group node contains children nodes without introducing a new
  transformation.  It is equivalent to a Transform node containing an
  identity transform.  More details on the children, addChildren, and
  removeChildren fields and eventIns can be found in 4.6.5, Grouping
  and children nodes
  (http://www.web3d.org/technicalinfo/specifications/vrml97/part1/concepts.html#4.6.5).

  The bboxCenter and bboxSize fields specify a bounding box that
  encloses the Group node's children. This is a hint that may be used
  for optimization purposes. The results are undefined if the
  specified bounding box is smaller than the actual bounding box of
  the children at any time.  A default bboxSize value, (-1, -1, -1),
  implies that the bounding box is not specified and, if needed, is
  calculated by the browser. A description of the bboxCenter and
  bboxSize fields is contained in 4.6.4, Bounding boxes
  (http://www.web3d.org/technicalinfo/specifications/vrml97/part1/concepts.html#4.6.4).


*/

/*!
  SoSFEnum SoVRMLGroup::renderCaching
  Render caching policy. Default value is AUTO.
*/

/*!
  SoSFEnum SoVRMLGroup::boundingBoxCaching
  Bounding box cache policy. Default value is AUTO.
*/

/*!
  SoSFEnum SoVRMLGroup::renderCulling
  Render culling policy. Default value is AUTO.
*/

/*!
  SoSFEnum SoVRMLGroup::pickCulling
  Pick culling policy. Default value is AUTO.
*/

/*!
  SoSFVec3f SoVRMLGroup::bboxCenter
  Can be used to optimize bounding box calculations.
*/

/*!
  SoSFVec3f SoVRMLGroup::bboxSize
  Can be used to optimize bounding box calculations.
*/

#include <Inventor/VRMLnodes/SoVRMLGroup.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/actions/SoActions.h>
#include <Inventor/elements/SoCacheElement.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/misc/SoChildList.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/caches/SoBoundingBoxCache.h>
#include <Inventor/caches/SoGLCacheList.h>
#include <Inventor/bundles/SoMaterialBundle.h>
#include <Inventor/elements/SoCacheElement.h>
#include <Inventor/elements/SoCullElement.h>
#include <Inventor/elements/SoLocalBBoxMatrixElement.h>
#include <Inventor/misc/SoGL.h>

#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/system/gl.h>

#ifdef COIN_THREADSAFE
#include <Inventor/threads/SbMutex.h>
#endif // COIN_THREADSAFE

int SoVRMLGroup::numRenderCaches = 2;

#ifndef DOXYGEN_SKIP_THIS

class SoVRMLGroupP {
public:
  SoBoundingBoxCache * bboxcache;
  SoGLCacheList * glcachelist;
#ifdef COIN_THREADSAFE
  SbMutex bboxmutex;
#endif // COIN_THREADSAFE
};

#endif // DOXYGEN_SKIP_THIS

SO_NODE_SOURCE(SoVRMLGroup);

// Doc in parent
void
SoVRMLGroup::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoVRMLGroup, SO_VRML97_NODE_TYPE);

  SoType type = SoVRMLGroup::getClassTypeId();
  SoRayPickAction::addMethod(type, SoNode::rayPickS);
}

#undef THIS
#define THIS this->pimpl

#ifdef COIN_THREADSAFE
#define TRY_LOCK_BBOX(_thisp_) (_thisp_)->pimpl->bboxmutex.tryLock()
#define LOCK_BBOX(_thisp_) (_thisp_)->pimpl->bboxmutex.lock()
#define UNLOCK_BBOX(_thisp_) (_thisp_)->pimpl->bboxmutex.unlock()
#else // COIN_THREADSAFE
#define TRY_LOCK_BBOX(_thisp_) TRUE
#define LOCK_BBOX(_thisp_)
#define UNLOCK_BBOX(_thisp_)
#endif // COIN_THREADSAFE

/*!
  Constructor.
*/
SoVRMLGroup::SoVRMLGroup(void)
{
  this->commonConstructor();
}

/*!
  Constructor. \a numchildren is the expected number of children.
*/
SoVRMLGroup::SoVRMLGroup(int numchildren)
  : SoVRMLParent(numchildren)
{
  this->commonConstructor();
}

void
SoVRMLGroup::commonConstructor(void)
{
  THIS = new SoVRMLGroupP;
  THIS->bboxcache = NULL;

  SO_NODE_INTERNAL_CONSTRUCTOR(SoVRMLGroup);

  SO_VRMLNODE_ADD_FIELD(bboxCenter, (0.0f, 0.0f, 0.0f));
  SO_VRMLNODE_ADD_FIELD(bboxSize, (-1.0f, -1.0f, -1.0f));

  SO_VRMLNODE_ADD_FIELD(renderCaching, (AUTO));
  SO_VRMLNODE_ADD_FIELD(boundingBoxCaching, (AUTO));
  SO_VRMLNODE_ADD_FIELD(renderCulling, (AUTO));
  SO_VRMLNODE_ADD_FIELD(pickCulling, (AUTO));

  SO_NODE_DEFINE_ENUM_VALUE(CacheEnabled, ON);
  SO_NODE_DEFINE_ENUM_VALUE(CacheEnabled, OFF);
  SO_NODE_DEFINE_ENUM_VALUE(CacheEnabled, AUTO);

  SO_NODE_SET_SF_ENUM_TYPE(renderCaching, CacheEnabled);
  SO_NODE_SET_SF_ENUM_TYPE(boundingBoxCaching, CacheEnabled);
  SO_NODE_SET_SF_ENUM_TYPE(renderCulling, CacheEnabled);
  SO_NODE_SET_SF_ENUM_TYPE(pickCulling, CacheEnabled);

  THIS->glcachelist = NULL;
#ifdef COIN_THREADSAFE
  // SoGLCacheList should be thread safe, but we need to create the
  // instance in the constructor to avoid a race condition when
  // creating the cache list
  THIS->glcachelist = new SoGLCacheList;
#endif // COIN_THREADSAFE
}

/*!
  Destructor.
*/
SoVRMLGroup::~SoVRMLGroup()
{
  if (THIS->bboxcache) THIS->bboxcache->unref();
  delete THIS->glcachelist;
  delete THIS;
}

/*!
  Set the maximum number of render caches per group node.
*/
void
SoVRMLGroup::setNumRenderCaches(int num )
{
  SoVRMLGroup::numRenderCaches = num;
}

/*!
  Returns the maximum number of render caches per group node.
*/
int
SoVRMLGroup::getNumRenderCaches(void)
{
  return SoVRMLGroup::numRenderCaches;
}

// Doc in parent
void
SoVRMLGroup::doAction(SoAction * action)
{
  SoState * state = action->getState();
  state->push();
  inherited::doAction(action);
  state->pop();
}

// Doc in parent
void
SoVRMLGroup::callback(SoCallbackAction * action)
{
  SoState * state = action->getState();
  state->push();
  // culling planes should normally not be set, but can be set
  // manually by the application programmer to optimize callback
  // action traversal.
  if (!this->cullTest(state)) { inherited::callback(action); }
  state->pop();
}

// Doc in parent
void
SoVRMLGroup::GLRender(SoGLRenderAction * action )
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

// Doc in parent
void
SoVRMLGroup::getBoundingBox(SoGetBoundingBoxAction * action)
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

  if (iscaching) iscaching = TRY_LOCK_BBOX(this);

  SbBool validcache = iscaching && THIS->bboxcache && THIS->bboxcache->isValid(state);

  if (iscaching && validcache) {
    SoCacheElement::addCacheDependency(state, THIS->bboxcache);
    childrenbbox = THIS->bboxcache->getBox();
    childrencenterset = THIS->bboxcache->isCenterSet();
    childrencenter = THIS->bboxcache->getCenter();
    if (THIS->bboxcache->hasLinesOrPoints()) {
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
      // if we get here, we know bbox cache is not created or is invalid
      if (THIS->bboxcache) THIS->bboxcache->unref();
      THIS->bboxcache = new SoBoundingBoxCache(state);
      THIS->bboxcache->ref();
      // set active cache to record cache dependencies
      SoCacheElement::set(state, THIS->bboxcache);
    }

    SoLocalBBoxMatrixElement::makeIdentity(state);
    action->getXfBoundingBox().makeEmpty();
    action->getXfBoundingBox().setTransform(SbMatrix::identity());
    inherited::getBoundingBox(action);

    childrenbbox = action->getXfBoundingBox();
    childrencenterset = action->isCenterSet();
    if (childrencenterset) childrencenter = action->getCenter();

    action->getXfBoundingBox() = abox; // reset action bbox

    if (iscaching) {
      THIS->bboxcache->set(childrenbbox, childrencenterset, childrencenter);
    }
    state->pop();
    if (iscaching) SoCacheElement::setInvalid(storedinvalid);
  }

  if (iscaching) UNLOCK_BBOX(this);

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

// Doc in parent
void
SoVRMLGroup::getMatrix(SoGetMatrixAction * action)
{
  int numindices;
  const int * indices;
  if (action->getPathCode(numindices, indices) == SoAction::IN_PATH) {
    this->getChildren()->traverseInPath(action, numindices, indices);
  }
}

// compute object space ray and test for intersection
static SbBool
ray_intersect(SoRayPickAction * action, const SbBox3f & box)
{
  if (box.isEmpty()) return FALSE;
  action->setObjectSpace();
  return action->intersect(box, TRUE);
}

// Doc in parent
void
SoVRMLGroup::rayPick(SoRayPickAction * action)
{
  if (this->pickCulling.getValue() == OFF ||
      !THIS->bboxcache || !THIS->bboxcache->isValid(action->getState()) ||
      !action->hasWorldSpaceRay() ||
      ray_intersect(action, THIS->bboxcache->getProjectedBox())) {
    SoVRMLGroup::doAction(action);
  }
}

// Doc in parent
void
SoVRMLGroup::search(SoSearchAction * action)
{
  SoNode::search(action);
  if (action->isFound()) return;

  SoState * state = action->getState();
  state->push();
  inherited::search(action);
  state->pop();
}

// Doc in parent
void
SoVRMLGroup::write(SoWriteAction * action)
{
  this->boundingBoxCaching.setDefault(TRUE);
  this->renderCaching.setDefault(TRUE);
  inherited::write(action);
}

// Doc in parent
void
SoVRMLGroup::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  SoState * state = action->getState();
  state->push();
  inherited::getPrimitiveCount(action);
  state->pop();
}

// Doc in parent
void
SoVRMLGroup::GLRenderBelowPath(SoGLRenderAction * action)
{
  // FIXME: for now we just cache if the renderCaching field is
  // ON. We'll develop the heuristics for automatically deciding when
  // to cache or not upon renderCaching==AUTO later.  pederb, 20001005
  SbBool didlazyeval = FALSE;
  SoState * state = action->getState();
  SoGLCacheList * createcache = NULL;
  if (this->renderCaching.getValue() == ON) {
    // test if bbox is outside view-volume
    if (this->cullTestNoPush(state)) {
      return;
    }

    if (!THIS->glcachelist) {
      THIS->glcachelist = new SoGLCacheList(SoVRMLGroup::getNumRenderCaches());
    }
    else {
      SoMaterialBundle mb(action);
      mb.sendFirst();

      // update lazy elements
      state->lazyEvaluate();

      if (THIS->glcachelist->call(action, GL_ALL_ATTRIB_BITS)) {
#if GLCACHE_DEBUG && 1 // debug
        SoDebugError::postInfo("SoSeparator::GLRenderBelowPath",
                               "Executing GL cache: %p", this);
#endif // debug
        return;
      }
    }
    if (!SoCacheElement::anyOpen(state)) { // nested GL caches not supported yet
#if GLCACHE_DEBUG // debug
      SoDebugError::postInfo("SoSeparator::GLRenderBelowPath",
                             "Creating GL cache: %p", this);
#endif // debug
      createcache = THIS->glcachelist;
    }
  }

  state->push();
  if (createcache) {
    if (!didlazyeval) {
      state->lazyEvaluate();
      SoMaterialBundle mb(action);
      mb.sendFirst();
    }
    createcache->open(action);
  }

  SbBool outsidefrustum = this->cullTest(state);

  if (createcache || !outsidefrustum) {
    int n = this->getChildren()->getLength();
    SoNode ** childarray = (SoNode**) this->getChildren()->getArrayPtr();
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
        int err = glGetError();
        if (err != GL_NO_ERROR) {
          SoDebugError::postInfo("SoVRMLGroup::GLRenderBelowPath",
                                 "GL error: %s, nodetype: %s",
                                 sogl_glerror_string(err).getString(),
                                 (*this->getChildren())[i]->getTypeId().getName().getString());
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

// Doc in parent
void
SoVRMLGroup::GLRenderInPath(SoGLRenderAction * action)
{
  int numindices;
  const int * indices;
  if (action->getPathCode(numindices, indices) == SoAction::IN_PATH) {
    SoState * state = action->getState();
    SoNode ** childarray = (SoNode**) this->getChildren()->getArrayPtr();
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

// Doc in parent
void
SoVRMLGroup::GLRenderOffPath(SoGLRenderAction * action)
{
  // do nothing
}

// Doc in parent
void
SoVRMLGroup::notify(SoNotList * list)
{
  inherited::notify(list);

  if (THIS->bboxcache) THIS->bboxcache->invalidate();
  if (THIS->glcachelist) {
#if GLCACHE_DEBUG && 0 // debug
    SoDebugError::postInfo("SoVRMLGroup::notify",
                           "Invalidating GL cache: %p", this);
#endif // debug
    THIS->glcachelist->invalidateAll();
  }
}

/*!
  Returns TRUE if children can be culled.
*/
SbBool
SoVRMLGroup::cullTest(SoState * state)
{
  if (this->renderCulling.getValue() == SoVRMLGroup::OFF) return FALSE;
  if (SoCullElement::completelyInside(state)) return FALSE;
  
  SbBool outside = FALSE;
  LOCK_BBOX(this);
  if (THIS->bboxcache &&
      THIS->bboxcache->isValid(state)) {
    const SbBox3f & bbox = THIS->bboxcache->getProjectedBox();
    if (!bbox.isEmpty()) {
      outside = SoCullElement::cullBox(state, bbox);
    }
  }
  UNLOCK_BBOX(this);
  return outside;
}

//
// no-push culltest
//
SbBool
SoVRMLGroup::cullTestNoPush(SoState * state)
{
  if (this->renderCulling.getValue() == SoVRMLGroup::OFF) return FALSE;
  if (SoCullElement::completelyInside(state)) return FALSE;

  SbBool outside = FALSE;
  LOCK_BBOX(this);
  if (THIS->bboxcache &&
      THIS->bboxcache->isValid(state)) {
    const SbBox3f & bbox = THIS->bboxcache->getProjectedBox();
    if (!bbox.isEmpty()) {
      outside = SoCullElement::cullTest(state, bbox);
    }
  }
  UNLOCK_BBOX(this);
  return outside;
}

#undef THIS
#undef LOCK_BBOX
#undef UNLOCK_BBOX
