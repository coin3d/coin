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
  \class SoSeparator SoSeparator.h Inventor/nodes/SoSeparator.h
  \brief The SoSeparator class is a state-preserving group node.
  \ingroup nodes

  Subgraphs parented by SoSeparator nodes will not affect the state,
  as they push and pop the traversal state before and after traversal
  of its children.

  SoSeparator nodes also provides options for traversal optimalization
  through the use of caching.
*/

// Metadon doc:
/*¡
  <ul>
  <li>rendercaching is not implemented, neither is renderculling nor
      pickculling</li>
  </ul>
 */

#include <Inventor/misc/SoChildList.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <coindefs.h> // COIN_STUB()
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/caches/SoBoundingBoxCache.h>
#include <Inventor/caches/SoGLCacheList.h>

#include <Inventor/misc/SoState.h>

#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoHandleEventAction.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/bundles/SoMaterialBundle.h>

#include <Inventor/elements/SoCacheElement.h>
#include <Inventor/elements/SoLocalBBoxMatrixElement.h>
#include <Inventor/elements/SoCullElement.h>
#include <stdlib.h> // for getenv()

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#ifdef HAVE_WINDOWS_H
#include <windows.h> // needed for gl.h
#endif // HAVE_WINDOWS_H
#include <GL/gl.h>

#if COIN_DEBUG
#define GLCACHE_DEBUG 0 // set to 1 to debug caching
#else
#define GLCACHE_DEBUG 0
#endif

// environment variable
static int COIN_RENDER_CACHING = -1;

// Maximum number of caches available for allocation for the
// rendercaching (FIXME: which is not implemented yet.. 20000426 mortene).
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
  execution. This will typically use the OpenGL \e renderlists
  mechanism.

  Default value is SoSeparator::AUTO.

  If you know that some parts of your scene will never change,
  rendering might happen faster if you explicitly set this field to
  SoSeparator::ON. If you on the other hand know that parts of the
  scene will change a lot (like for every redraw), it will be
  beneficial to set this field to SoSeparator::OFF for the top-level
  separator node of this (sub)graph.

  For now, render caching is in the beta stage. You can enable
  render caching by setting the environment variable COIN_RENDER_CACHING to 1.
  Separators will then be cached if the renderCaching field is set to ON.
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
  do for the underlying immediate mode rendering library. This will
  often be faster than doing culling from within Coin, so be careful
  to monitor the change in execution speed if setting this field to
  SoSeparator::ON.

  See also documentation for SoSeparator::renderCaching.
*/
/*!
  \var SoSFEnum SoSeparator::pickCulling

  Policy for doing viewport culling during pick traversals. Default
  value is SoSeparator::AUTO.

  See documentation for SoSeparator::renderCulling.
*/


// *************************************************************************

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

  The \a nchildren argument is a hint to the separator group instance
  about how many children it is expected will be managed by this
  node. This makes it possible to do better resource allocation.
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

  this->bboxcache = NULL;
  this->glcachelist = NULL;
}

/*!
  Destructor. Frees resources used to implement caches.
*/
SoSeparator::~SoSeparator()
{
  if (this->bboxcache) this->bboxcache->unref();
  delete this->glcachelist;
}

// Doc from superclass.
void
SoSeparator::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoSeparator);

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

  // only update and use cache when we're traversing all children
  if (action->getCurPathCode() == SoAction::IN_PATH) iscaching = FALSE;

  SbBool validcache = this->bboxcache && this->bboxcache->isValid(state);

  // FIXME: there needs to be some extra magic here to make caching
  // work correctly when the bounding box is calculated in camera
  // space. NB: getting this code in general to cooperate with an
  // SoGetBoundingBoxAction with the isInCameraSpace() flag set is
  // non-trivial. 19990513 mortene.
  //  assert (!action->isInCameraSpace());

  if (iscaching && validcache) {
    childrenbbox = this->bboxcache->getBox();
    childrencenterset = this->bboxcache->isCenterSet();
    childrencenter = this->bboxcache->getCenter();
  }
  else {
    SbXfBox3f abox = action->getXfBoundingBox();

    state->push();
    SoLocalBBoxMatrixElement::makeIdentity(state);
    action->getXfBoundingBox().makeEmpty();
    inherited::getBoundingBox(action);
    state->pop();

    childrenbbox = action->getXfBoundingBox();
    childrencenterset = action->isCenterSet();
    if (childrencenterset) childrencenter = action->getCenter();

    action->getXfBoundingBox() = abox; // reset action bbox

    if (iscaching) {
      // FIXME: continuous new & delete during traversal is probably a
      // performance killer.. fix. 19990422 mortene.
      if (this->bboxcache) this->bboxcache->unref();
      this->bboxcache = new SoBoundingBoxCache(state);
      this->bboxcache->ref();
      this->bboxcache->set(childrenbbox, childrencenterset, childrencenter);
    }
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
  action->getState()->push();
  SoGroup::callback(action);
  action->getState()->pop();
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
  OIV 2.1 obsoleted support SoGLRenderAction::addMethod().
  Instead, GLRender() might be called directly, and to optimize
  traversal, the SoSeparator node calls GLRenderBelowPath whenever
  the path code is BELOW_PATH or NO_PATH (path code is guaranteed
  not to change).
  SoSeparator::GLRenderBelowPath() do not traverse its children using
  SoChildList::traverse(), but calls GLRenderBelowPath() directly
  for all its children.
*/
void
SoSeparator::GLRenderBelowPath(SoGLRenderAction * action)
{
  // Temporary caching code. For now we just cache if the
  // renderCaching field is ON. We'll develop an auto-caching scheme
  // for auto-caching later.    pederb, 20001005

  if (COIN_RENDER_CACHING < 0) {
    char * env = getenv("COIN_RENDER_CACHING");
    if (env) COIN_RENDER_CACHING = atoi(env);
    else COIN_RENDER_CACHING = 0;
  }

  SbBool didlazyeval = FALSE;
  SoState * state = action->getState();
  SoGLCacheList * createcache = NULL;
  if ((this->renderCaching.getValue() == ON) && COIN_RENDER_CACHING) {
    // test if bbox is outside view-volume
    if (this->cullTestNoPush(state)) return;

    if (!this->glcachelist) {
      this->glcachelist = new SoGLCacheList(SoSeparator::getNumRenderCaches());
    }
    else {
      SoMaterialBundle mb(action);
      mb.sendFirst();

      // update lazy elements
      state->lazyEvaluate();

      if (this->glcachelist->call(action, GL_ALL_ATTRIB_BITS)) {
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
      createcache = this->glcachelist;
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

  if (createcache || !this->cullTest(state)) {
    int n = this->children->getLength();
    SoAction::PathCode pathcode = action->getCurPathCode();
    SoNode ** childarray = (SoNode**) this->children->getArrayPtr();
    for (int i = 0; i < n; i++) {
      if (action->abortNow()) {
        // only cache if we do a full traversal
        SoCacheElement::invalidate(state);
        break;
      }
      action->pushCurPath(i, childarray[i]);
      childarray[i]->GLRenderBelowPath(action);
      // The GL error test is disabled for this optimized path.
      // If you get a GL error reporting an error in the Separator node,
      // enable this code to see exactly which node caused the error.
      //  pederb, 20000916

#if 0 // enable to debug GL errors
      int err = glGetError();
      if (err != GL_NO_ERROR) {
        const char * errorstring;
        switch (err) {
        case GL_INVALID_VALUE:
          errorstring = "GL_INVALID_VALUE";
          break;
        case GL_INVALID_ENUM:
          errorstring = "GL_INVALID_ENUM";
          break;
        case GL_INVALID_OPERATION:
          errorstring = "GL_INVALID_OPERATION";
          break;
        case GL_STACK_OVERFLOW:
          errorstring = "GL_STACK_OVERFLOW";
          break;
        case GL_STACK_UNDERFLOW:
          errorstring = "GL_STACK_UNDERFLOW";
          break;
        case GL_OUT_OF_MEMORY:
          errorstring = "GL_OUT_OF_MEMORY";
          break;
        default:
          errorstring = "Unknown GL error";
          break;
        }
        SoDebugError::postInfo("SoSeparator::GLRenderBelowPath",
                               "GL error: %s, nodetype: %s",
                               errorstring,
                               (*this->children)[i]->getTypeId().getName().getString());
      }
#endif // GL debug
      action->popCurPath(pathcode);
    }
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
  int numIndices;
  const int * indices;
  action->getPathCode(numIndices, indices);

  action->getState()->push();
  this->children->traverse(action, 0, indices[numIndices-1]);
  action->getState()->pop();
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
      !this->bboxcache || !this->bboxcache->isValid(action->getState()) ||
      !action->hasWorldSpaceRay() ||
      ray_intersect(action, this->bboxcache->getProjectedBox())) {
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
  if (action->getCurPathCode() == SoAction::IN_PATH)
    inherited::getMatrix(action);

  // FIXME: ..and? Is this all? 20000302 mortene.
}

/*!
  Set up number of caches SoSeparator nodes may allocate for render
  caching. More caches might give better performance, but will use
  more memory.

  Default value is 2.
*/
void
SoSeparator::setNumRenderCaches(const int howmany)
{
  SoSeparator::numrendercaches = howmany;
  // FIXME: not in use, as render caching has not been implemented
  // yet. 20000426 mortene.
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

  if (this->bboxcache) this->bboxcache->invalidate();
  if (this->glcachelist) {
#if GLCACHE_DEBUG && 0 // debug
    SoDebugError::postInfo("SoSeparator::notify",
                           "Invalidating GL cache: %p", this);
#endif // debug
    this->glcachelist->invalidateAll();
  }
  // FIXME: flag other caches (as they are implemented) as
  // dirty. 20000426 mortene.
}

/*!
  This is an internal Open Inventor method. We've implemented
  view frustum culling in a different manner. Let us know if
  you need this function, and we'll consider implementing it.
*/
SbBool
SoSeparator::cullTest(SoGLRenderAction * action, int & cullresults)
{
  // FIXME: not implemented, as support for render- and pickculling is
  // missing. 20000426 mortene.

  COIN_STUB();
  return FALSE;
}

// Doc from superclass.
SbBool
SoSeparator::readInstance(SoInput * in, unsigned short flags)
{
  // FIXME: anything missing here?
  return inherited::readInstance(in, flags);
}

/*!
  Internal method which do view frustum culling. For now, view
  frustum culling is performed if the renderCulling field is AUTO or
  ON, and the bounding box cache is valid. Returns TRUE if this
  separator is outside view frustum, FALSE if inside.
*/
SbBool
SoSeparator::cullTest(SoState * state)
{
  if (this->renderCulling.getValue() == SoSeparator::OFF) return FALSE;
  if (!this->bboxcache ||
      !this->bboxcache->isValid(state) ||
      this->bboxcache->getProjectedBox().isEmpty()) return FALSE;
  if (SoCullElement::completelyInside(state)) return FALSE;
  return SoCullElement::cullBox(state, this->bboxcache->getProjectedBox());
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
  if (!this->bboxcache ||
      !this->bboxcache->isValid(state) ||
      this->bboxcache->getProjectedBox().isEmpty()) return FALSE;
  if (SoCullElement::completelyInside(state)) return FALSE;
  return SoCullElement::cullBox(state, this->bboxcache->getProjectedBox());
}
