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
  \class SoVRMLShape SoVRMLShape.h Inventor/VRMLnodes/SoVRMLShape.h
  \brief The SoVRMLShape class holds geometry and geometry appearance nodes.
  \ingroup VRMLnodes

  \WEB3DCOPYRIGHT

  \verbatim
  Shape {
    exposedField SFNode appearance NULL
    exposedField SFNode geometry   NULL
  }
  \endverbatim

  The Shape node has two fields, \e appearance and \e geometry, which
  are used to create rendered objects in the world. The \e appearance
  field contains an SoVRMLAppearance node that specifies the visual
  attributes (e.g., material and texture) to be applied to the
  geometry. The geometry field contains a geometry node. The specified
  geometry node is rendered with the specified appearance nodes
  applied. See 4.6.3, Shapes and geometry
  (<http://www.web3d.org/technicalinfo/specifications/vrml97/part1/concepts.html#4.6.3>),
  and SoVRMLAppearance, for more information.

  4.14, Lighting model
  (<http://www.web3d.org/technicalinfo/specifications/vrml97/part1/concepts.html#4.14>),
  contains details of the VRML lighting model and the interaction
  between Appearance nodes and geometry nodes.  If the geometry field
  is NULL, the object is not drawn.

*/

/*!
  \enum SoVRMLShape::CacheEnabled
  Used to enumerate cache strategies.
*/

/*!
  \var SoVRMLShape::CacheEnabled SoVRMLShape::OFF
  Never cache
*/

/*!
  \var SoVRMLShape::CacheEnabled SoVRMLShape::ON
  Always cache
*/

/*!
  \var SoVRMLShape::CacheEnabled SoVRMLShape::AUTO
  Use heuristics to try to figure out the optimal caching policy.
*/

/*!
  \var SoSFNode SoVRMLShape::appearance
  Can store an SoVRMLAppearance node, or NULL.
*/

/*!
  \var SoSFNode SoVRMLShape::geometry
  Can store any SoVRMLGeometry subclass, or NULL.
*/

/*!
  \var SoSFEnum SoVRMLShape::renderCaching
  Render caching strategy. Default value is AUTO.
*/

/*!
  \var SoSFEnum SoVRMLShape::boundingBoxCaching
  Bounding box caching strategy. Default value is AUTO.
*/




#include <Inventor/VRMLnodes/SoVRMLShape.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/VRMLnodes/SoVRMLAppearance.h>
#include <Inventor/VRMLnodes/SoVRMLMaterial.h>
#include <Inventor/VRMLnodes/SoVRMLParent.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/misc/SoChildList.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/elements/SoCacheElement.h>
#include <Inventor/elements/SoLocalBBoxMatrixElement.h>
#include <Inventor/elements/SoCullElement.h>
#include <Inventor/elements/SoComplexityTypeElement.h>
#include <Inventor/caches/SoBoundingBoxCache.h>
#include <Inventor/caches/SoGLCacheList.h>
#include <Inventor/elements/SoGLLazyElement.h>
#include <Inventor/caches/SoBoundingBoxCache.h>
#include <Inventor/caches/SoGLCacheList.h>
#include <Inventor/misc/SoGL.h>
#include <Inventor/elements/SoGLShapeHintsElement.h>
#include <Inventor/elements/SoGLTextureEnabledElement.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H
#include <Inventor/system/gl.h>

#ifdef COIN_THREADSAFE
#include <Inventor/threads/SbMutex.h>
#endif // COIN_THREADSAFE

#include <stddef.h>

#ifndef DOXYGEN_SKIP_THIS

class SoVRMLShapeP {
public:
  SoBoundingBoxCache * bboxcache;
  SoGLCacheList * cachelist;
  SoChildList * childlist;
  SbBool childlistvalid;
#ifdef COIN_THREADSAFE
  SbMutex bboxmutex;
  SbMutex childlistmutex;
#endif // COIN_THREADSAFE
  void lockChildList(void) {
#ifdef COIN_THREADSAFE
    this->childlistmutex.lock();
#endif // COIN_THREADSAFE
  }
  void unlockChildList(void) {
#ifdef COIN_THREADSAFE
    this->childlistmutex.unlock();
#endif // COIN_THREADSAFE
  }
};

#endif // DOXYGEN_SKIP_THIS


SO_NODE_SOURCE(SoVRMLShape);

int SoVRMLShape::numrendercaches;

void
SoVRMLShape::initClass(void) // static
{
  SO_NODE_INTERNAL_INIT_CLASS(SoVRMLShape, SO_VRML97_NODE_TYPE);

  SoType type = SoVRMLShape::getClassTypeId();
  SoRayPickAction::addMethod(type, SoNode::rayPickS);
}

#define PRIVATE(thisp) ((thisp)->pimpl)

#ifdef COIN_THREADSAFE
#define TRY_LOCK_BBOX(_thisp_) (_thisp_)->pimpl->bboxmutex.tryLock()
#define LOCK_BBOX(_thisp_) (_thisp_)->pimpl->bboxmutex.lock()
#define UNLOCK_BBOX(_thisp_) (_thisp_)->pimpl->bboxmutex.unlock()
#else // COIN_THREADSAFE
#define TRY_LOCK_BBOX(_thisp_) TRUE
#define LOCK_BBOX(_thisp_)
#define UNLOCK_BBOX(_thisp_)
#endif // COIN_THREADSAFE

SoVRMLShape::SoVRMLShape(void)
{
  PRIVATE(this) = new SoVRMLShapeP;

  SO_VRMLNODE_INTERNAL_CONSTRUCTOR(SoVRMLShape);

  SO_VRMLNODE_ADD_EXPOSED_FIELD(appearance, (NULL));
  SO_VRMLNODE_ADD_EXPOSED_FIELD(geometry, (NULL));

  SO_NODE_ADD_FIELD(renderCaching, (AUTO));
  SO_NODE_ADD_FIELD(boundingBoxCaching, (AUTO));

  SO_NODE_DEFINE_ENUM_VALUE(CacheEnabled, ON);
  SO_NODE_DEFINE_ENUM_VALUE(CacheEnabled, OFF);
  SO_NODE_DEFINE_ENUM_VALUE(CacheEnabled, AUTO);

  SO_NODE_SET_SF_ENUM_TYPE(renderCaching, CacheEnabled);
  SO_NODE_SET_SF_ENUM_TYPE(boundingBoxCaching, CacheEnabled);

  PRIVATE(this)->childlist = new SoChildList(this);
  PRIVATE(this)->childlistvalid = FALSE;
  PRIVATE(this)->bboxcache = NULL;
  PRIVATE(this)->cachelist = NULL;
}

SoVRMLShape::~SoVRMLShape()
{
  delete PRIVATE(this)->childlist;
  if (PRIVATE(this)->bboxcache) PRIVATE(this)->bboxcache->unref();
  delete PRIVATE(this)->cachelist;
  delete PRIVATE(this);
}

void
SoVRMLShape::setNumRenderCaches(int num)
{
  SoVRMLShape::numrendercaches = num;
}

int
SoVRMLShape::getNumRenderCaches(void)
{
  return SoVRMLShape::numrendercaches;
}

SbBool
SoVRMLShape::affectsState(void) const
{
  return FALSE;
}

void
SoVRMLShape::doAction(SoAction * action)
{
  SoState * state = action->getState();

  if (state->isElementEnabled(SoLazyElement::getClassStackIndex())) {
    if ((this->appearance.getValue() == NULL) ||
        (((SoVRMLAppearance*)this->appearance.getValue())->material.getValue() == NULL)) {
      SoLazyElement::setLightModel(state, SoLazyElement::BASE_COLOR);
    }
  }

  state->push();
  int numindices;
  const int * indices;
  if (action->getPathCode(numindices, indices) == SoAction::IN_PATH) {
    this->getChildren()->traverseInPath(action, numindices, indices);
  }
  else {
    this->getChildren()->traverse(action); // traverse all children
  }
  state->pop();
}

void
SoVRMLShape::callback(SoCallbackAction * action)
{
  SoVRMLShape::doAction((SoAction*) action);
}

void
SoVRMLShape::GLRender(SoGLRenderAction * action)
{
  SoState * state = action->getState();

  LOCK_BBOX(this);

  if (SoComplexityTypeElement::get(state) ==
      SoComplexityTypeElement::BOUNDING_BOX) {
    
    SbBool validcache = PRIVATE(this)->bboxcache && PRIVATE(this)->bboxcache->isValid(state);
    if (validcache) {
      state->push();
      SbBox3f box = PRIVATE(this)->bboxcache->getProjectedBox();      
      SbVec3f center = (box.getMin() + box.getMax()) * 0.5f;
      SbVec3f size = box.getMax()  - box.getMin();
      
      UNLOCK_BBOX(this);

      if (SoGLTextureEnabledElement::get(state)) {
        SoGLTextureEnabledElement::set(state, FALSE);
      }
      SoGLShapeHintsElement::forceSend(state, TRUE, FALSE, FALSE);
      SoGLLazyElement::sendLightModel(state, SoLazyElement::BASE_COLOR);
  
      SoVRMLAppearance * app = (SoVRMLAppearance*) this->appearance.getValue();
      
      SbColor color(0.8f, 0.8f, 0.8f);

      if (app && app->material.getValue()) {
        SoVRMLMaterial * mat = (SoVRMLMaterial*) app->material.getValue();
        color = mat->diffuseColor.getValue();
      }
      SoGLLazyElement::sendPackedDiffuse(state, color.getPackedValue());

      glPushMatrix();
      glTranslatef(center[0], center[1], center[2]);
      sogl_render_cube(size[0], size[1], size[2], NULL, 
                       SOGL_NEED_NORMALS | SOGL_NEED_TEXCOORDS);
      glPopMatrix();
      state->pop();
      return;
    }
  }

  // if we have a valid bbox cache, do a view volume cull test here.
  if (!state->isCacheOpen() && PRIVATE(this)->bboxcache &&
      PRIVATE(this)->bboxcache->isValid(state)) {
    if (!SoCullElement::completelyInside(state)) {
      if (SoCullElement::cullTest(state, PRIVATE(this)->bboxcache->getProjectedBox())) {
        UNLOCK_BBOX(this);
        return;
      }
    }
  }

  UNLOCK_BBOX(this);

  state->push();

  if ((this->appearance.getValue() == NULL) ||
      (((SoVRMLAppearance*)this->appearance.getValue())->material.getValue() == NULL)) {
    SoLazyElement::setLightModel(state, SoLazyElement::BASE_COLOR);
  }

  int numindices;
  const int * indices;
  SoAction::PathCode pathcode = action->getPathCode(numindices, indices);

  SoNode ** childarray = (SoNode**) this->getChildren()->getArrayPtr();

  if (pathcode == SoAction::IN_PATH) {
    int lastchild = indices[numindices - 1];
    for (int i = 0; i <= lastchild && !action->hasTerminated(); i++) {
      SoNode * child = childarray[i];
      action->pushCurPath(i, child);
      if (action->getCurPathCode() != SoAction::OFF_PATH ||
          child->affectsState()) {
        if (!action->abortNow()) {
          child->GLRender(action);
        }
        else {
          SoCacheElement::invalidate(state);
        }
      }
      action->popCurPath(pathcode);
    }
  }
  else {
    action->pushCurPath();
    int n = this->getChildren()->getLength();
    for (int i = 0; i < n && !action->hasTerminated(); i++) {
      if (action->abortNow()) {
        // only cache if we do a full traversal
        SoCacheElement::invalidate(state);
        break;
      }
      action->popPushCurPath(i, childarray[i]);
      childarray[i]->GLRender(action);
    }
    action->popCurPath();
  }
  state->pop();
}

void
SoVRMLShape::getBoundingBox(SoGetBoundingBoxAction * action)
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
      // if we get here, we know bbox cache is not created or is invalid
      if (PRIVATE(this)->bboxcache) PRIVATE(this)->bboxcache->unref();
      PRIVATE(this)->bboxcache = new SoBoundingBoxCache(state);
      PRIVATE(this)->bboxcache->ref();
      // set active cache to record cache dependencies
      SoCacheElement::set(state, PRIVATE(this)->bboxcache);
    }

    SoLocalBBoxMatrixElement::makeIdentity(state);
    action->getXfBoundingBox().makeEmpty();

    SoNode * shape = this->geometry.getValue();
    if (shape) shape->getBoundingBox(action);

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

// compute object space ray and test for intersection
static SbBool
vrmlshape_ray_intersect(SoRayPickAction * action, const SbBox3f & box)
{
  if (box.isEmpty()) return FALSE;
  action->setObjectSpace();
  return action->intersect(box, TRUE);
}


void
SoVRMLShape::rayPick(SoRayPickAction * action)
{
  LOCK_BBOX(this);
  if (!PRIVATE(this)->bboxcache || !PRIVATE(this)->bboxcache->isValid(action->getState()) ||
      !action->hasWorldSpaceRay() ||
      vrmlshape_ray_intersect(action, PRIVATE(this)->bboxcache->getProjectedBox())) {
    UNLOCK_BBOX(this);
    SoVRMLShape::doAction(action);
  }
  else {
    UNLOCK_BBOX(this);
  }
}

// Doc in parent
void
SoVRMLShape::write(SoWriteAction * action)
{
  // do not call inherited::write() or SoGroup::write()
  this->boundingBoxCaching.setDefault(TRUE);
  this->renderCaching.setDefault(TRUE);
  inherited::write(action);
}

void
SoVRMLShape::search(SoSearchAction * action)
{
  // Include this node in the search.
  SoNode::search(action);
  if (action->isFound()) return;

  SoVRMLShape::doAction(action);
}

void
SoVRMLShape::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  SoVRMLShape::doAction((SoAction*) action);
}

SoChildList *
SoVRMLShape::getChildren(void) const
{
  if (!PRIVATE(this)->childlistvalid) {
    // this is not 100% thread safe. The assumption is that no nodes
    // will be added or removed while a scene graph is being
    // traversed. For Coin, this is an ok assumption.
    PRIVATE(this)->lockChildList();
    // test again after we've locked
    if (!PRIVATE(this)->childlistvalid) {
      SoVRMLShape * thisp = (SoVRMLShape*) this;
      SoVRMLParent::updateChildList(thisp, *(PRIVATE(thisp)->childlist));
      PRIVATE(thisp)->childlistvalid = TRUE;
    }
    PRIVATE(this)->unlockChildList();
  }
  return PRIVATE(this)->childlist;
}

void
SoVRMLShape::notify(SoNotList * list)
{
  SoField * f = list->getLastField();
  if (f && f->getTypeId() == SoSFNode::getClassTypeId()) {
    PRIVATE(this)->childlistvalid = FALSE;
  }
  if (PRIVATE(this)->bboxcache) PRIVATE(this)->bboxcache->invalidate();
  inherited::notify(list);
}

void
SoVRMLShape::copyContents(const SoFieldContainer * from,
                          SbBool copyConn)
{
  inherited::copyContents(from, copyConn);
  PRIVATE(this)->childlistvalid = FALSE;
  PRIVATE(this)->childlist->truncate(0);
}

#undef PRIVATE

