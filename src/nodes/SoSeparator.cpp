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
  \class SoSeparator SoSeparator.h Inventor/nodes/SoSeparator.h
  \brief The SoSeparator class ...
  \ingroup nodes

  FIXME: write class doc
*/


#include <Inventor/misc/SoChildList.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <coindefs.h> // COIN_STUB()
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/caches/SoBoundingBoxCache.h>

#include <Inventor/misc/SoState.h>

#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoHandleEventAction.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoGLRenderAction.h>

#include <Inventor/elements/SoCacheElement.h>
#include <Inventor/elements/SoLocalBBoxMatrixElement.h>


/*!
  \enum SoSeparator::CacheEnabled
  FIXME: write documentation for enum
*/
/*!
  \var SoSeparator::CacheEnabled SoSeparator::OFF
  FIXME: write documentation for enum definition
*/
/*!
  \var SoSeparator::CacheEnabled SoSeparator::ON
  FIXME: write documentation for enum definition
*/
/*!
  \var SoSeparator::CacheEnabled SoSeparator::AUTO
  FIXME: write documentation for enum definition
*/


/*!
  \var SoSFEnum SoSeparator::renderCaching
  FIXME: write documentation for field
*/
/*!
  \var SoSFEnum SoSeparator::boundingBoxCaching
  FIXME: write documentation for field
*/
/*!
  \var SoSFEnum SoSeparator::renderCulling
  FIXME: write documentation for field
*/
/*!
  \var SoSFEnum SoSeparator::pickCulling
  FIXME: write documentation for field
*/


// *************************************************************************

SO_NODE_SOURCE(SoSeparator);

/*!
  Constructor.
*/
SoSeparator::SoSeparator()
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

  this->bboxCache = NULL;
}

/*!
  Destructor.
*/
SoSeparator::~SoSeparator()
{
  delete this->bboxCache;
}

/*!
  Does initialization common for all objects of the
  SoSeparator class. This includes setting up the
  type system, among other things.
*/
void
SoSeparator::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoSeparator);

  SO_ENABLE(SoGetBoundingBoxAction, SoCacheElement);

  SO_ENABLE(SoGLRenderAction, SoCacheElement);
}

/*!
  FIXME: write function documentation
*/
void
SoSeparator::doAction(SoAction * action)
{
  action->getState()->push();
  inherited::doAction(action);
  action->getState()->pop();
}

/*!
  FIXME: write function documentation
*/
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
  SbBool validcache = this->bboxCache && this->bboxCache->isValid(state);

  // FIXME: there needs to be some extra magic here to make caching
  // work correctly when the bounding box is calculated in camera
  // space. NB: getting this code in general to cooperate with an
  // SoGetBoundingBoxAction with the isInCameraSpace() flag set is
  // non-trivial. 19990513 mortene.
  //  assert (!action->isInCameraSpace());

  if (iscaching && validcache) {
    childrenbbox = this->bboxCache->getBox();
    childrencenterset = this->bboxCache->isCenterSet();
    childrencenter = this->bboxCache->getCenter();
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
      delete this->bboxCache;
      this->bboxCache = new SoBoundingBoxCache(state);
      this->bboxCache->set(childrenbbox, childrencenterset, childrencenter);
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

/*!
  FIXME: write function documentation
*/
void
SoSeparator::callback(SoCallbackAction * action)
{
  action->getState()->push();
  SoGroup::callback(action);
  action->getState()->pop();
}

/*!
  FIXME: write function documentation
*/
void
SoSeparator::GLRender(SoGLRenderAction * action)
{
  action->getState()->push();
  this->children->traverse(action);
  action->getState()->pop();
}

/*!
  FIXME: write doc
 */
void
SoSeparator::GLRenderBelowPath(SoGLRenderAction * action)
{
  action->getState()->push();
  this->children->traverse(action);
  action->getState()->pop();
}

/*!
  FIXME: write doc
 */
void
SoSeparator::GLRenderInPath(SoGLRenderAction * action)
{
  assert(action->getCurPathCode() == SoAction::IN_PATH);

  int numIndices;
  const int *indices;
  action->getPathCode(numIndices, indices);

  action->getState()->push();
  this->children->traverse(action, 0, indices[numIndices-1]);
  action->getState()->pop();
}

/*!
  FIXME: write doc
 */
void
SoSeparator::GLRenderOffPath(SoGLRenderAction *)
{
  // do nothing, since all state changes will be reset by the separator
}

/*!
  FIXME: write function documentation
*/
void
SoSeparator::handleEvent(SoHandleEventAction * action)
{
  SoSeparator::doAction(action);
}

/*!
  FIXME: write function documentation
*/
void
SoSeparator::rayPick(SoRayPickAction * action)
{
#if 0 // debug
  SoDebugError::postInfo("SoSeparator::rayPick",
                         "");
#endif // debug

  SoSeparator::doAction(action);
}

/*!
  FIXME: write function documentation
*/
void
SoSeparator::search(SoSearchAction * action)
{
  // Include this node in the search. (_Don't_ use the
  // SoGroup::search(), as it will also traverse all children.)
  SoNode::search(action);

  if (action->isFound()) return;

  SoSeparator::doAction(action);
}

/*!
  FIXME: write function documentation
*/
void
SoSeparator::getMatrix(SoGetMatrixAction * action)
{
  if (action->getCurPathCode() == SoAction::IN_PATH)
    inherited::getMatrix(action);

  // FIXME: ..and? Is this all? 20000302 mortene.
}

/*!
  FIXME: write function documentation
*/
SoSeparator::SoSeparator(const int /* nChildren */)
{
  COIN_STUB();
}

/*!
  FIXME: write function documentation
*/
void
SoSeparator::setNumRenderCaches(const int /* howMany */)
{
  COIN_STUB();
}

/*!
  FIXME: write function documentation
*/
int
SoSeparator::getNumRenderCaches()
{
  COIN_STUB();
  return 0;
}

/*!
  FIXME: write function documentation
*/
SbBool
SoSeparator::affectsState(void) const
{
  return FALSE;
}


/*!
  FIXME: write doc
 */
void
SoSeparator::getPrimitiveCount(SoGetPrimitiveCountAction *action)
{
  SoSeparator::doAction((SoAction*)action);
}

/*!
  FIXME: write doc
 */
void
SoSeparator::notify(SoNotList * list)
{
  // FIXME: flag caches as dirty. 19990612 mortene.
  inherited::notify(list);

  if (this->bboxCache) {
    this->bboxCache->invalidate();
  }
}

/*!
  FIXME: write doc
 */
SbBool
SoSeparator::cullTest(SoGLRenderAction * /* action */, int & /* cullResults */)
{
  COIN_STUB();
  return FALSE;
}

/*!
  FIXME: write doc
 */
SbBool
SoSeparator::readInstance(SoInput * in, unsigned short flags)
{
  // FIXME: anything missing here?
  return inherited::readInstance(in, flags);
}
