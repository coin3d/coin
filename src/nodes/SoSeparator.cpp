/**************************************************************************\
 * 
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/caches/SoBoundingBoxCache.h>

#if !defined(COIN_EXCLUDE_SOSTATE)
#include <Inventor/misc/SoState.h>
#endif // !COIN_EXCLUDE_SOSTATE

#if !defined(COIN_EXCLUDE_SOACTION)
#include <Inventor/actions/SoAction.h>
#endif // !COIN_EXCLUDE_SOACTION
#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION
#if !defined(COIN_EXCLUDE_SOHANDLEEVENTACTION)
#include <Inventor/actions/SoHandleEventAction.h>
#endif // !COIN_EXCLUDE_SOHANDLEEVENTACTION
#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
#include <Inventor/actions/SoCallbackAction.h>
#endif // !COIN_EXCLUDE_SOCALLBACKACTION
#if !defined(COIN_EXCLUDE_SOGETMATRIXACTION)
#include <Inventor/actions/SoGetMatrixAction.h>
#endif // !COIN_EXCLUDE_SOGETMATRIXACTION
#if !defined(COIN_EXCLUDE_SORAYPICKACTION)
#include <Inventor/actions/SoRayPickAction.h>
#endif // !COIN_EXCLUDE_SORAYPICKACTION
#if !defined(COIN_EXCLUDE_SOSEARCHACTION)
#include <Inventor/actions/SoSearchAction.h>
#endif // !COIN_EXCLUDE_SOSEARCHACTION
#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
#include <Inventor/actions/SoGLRenderAction.h>
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SOCACHEELEMENT)
#include <Inventor/elements/SoCacheElement.h>
#endif // !COIN_EXCLUDE_SOCACHEELEMENT
#if !defined(COIN_EXCLUDE_SOLOCALBBOXMATRIXELEMENT)
#include <Inventor/elements/SoLocalBBoxMatrixElement.h>
#endif // !COIN_EXCLUDE_SOLOCALBBOXMATRIXELEMENT

#include <Inventor/SbMatrix.h>

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

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
  SO_ENABLE(SoGetBoundingBoxAction, SoCacheElement);
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  SO_ENABLE(SoGLRenderAction, SoCacheElement);
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
}

#if !defined(COIN_EXCLUDE_SOACTION)
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
#endif // !COIN_EXCLUDE_SOACTION

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
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
  assert (!action->isInCameraSpace());

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
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION

#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
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
#endif // !COIN_EXCLUDE_SOCALLBACKACTION

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
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

#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SOHANDLEEVENTACTION)
/*!
  FIXME: write function documentation
*/
void 
SoSeparator::handleEvent(SoHandleEventAction * action)
{
  SoSeparator::doAction(action);
}
#endif // !COIN_EXCLUDE_SOHANDLEEVENTACTION

#if !defined(COIN_EXCLUDE_SORAYPICKACTION)
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
#endif // !COIN_EXCLUDE_SORAYPICKACTION

#if !defined(COIN_EXCLUDE_SOSEARCHACTION)
/*!
  FIXME: write function documentation
*/
void 
SoSeparator::search(SoSearchAction * action)
{
  SoSeparator::doAction(action);
}
#endif // !COIN_EXCLUDE_SOSEARCHACTION

#if !defined(COIN_EXCLUDE_SOGETMATRIXACTION)
/*!
  FIXME: write function documentation
*/
void 
SoSeparator::getMatrix(SoGetMatrixAction * action)
{
  if (action->getCurPathCode() == SoAction::IN_PATH)
    inherited::getMatrix(action);

}
#endif // !COIN_EXCLUDE_SOGETMATRIXACTION

/*!
  FIXME: write function documentation
*/
SoSeparator::SoSeparator(const int /* nChildren */)
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
void 
SoSeparator::setNumRenderCaches(const int /* howMany */)
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
int 
SoSeparator::getNumRenderCaches()
{
  assert(0 && "FIXME: not implemented yet");
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


#if !defined(COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION)
/*!
  FIXME: write doc
 */
void
SoSeparator::getPrimitiveCount(SoGetPrimitiveCountAction *action)
{
  SoSeparator::doAction((SoAction*)action);
}
#endif // !COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION

/*!
  FIXME: write doc
 */
void
SoSeparator::notify(SoNotList * list)
{
  // FIXME: flag caches as dirty. 19990612 mortene.
  inherited::notify(list);
}

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write doc
 */
SbBool
SoSeparator::cullTest(SoGLRenderAction * /* action */, int & /* cullResults */)
{
  assert(0 && "FIXME: not implemented");
  return FALSE;
}
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

/*!
  FIXME: write doc
 */
SbBool
SoSeparator::readInstance(SoInput * in, unsigned short flags)
{
  // FIXME: what else?
  return inherited::readInstance(in, flags);
}
