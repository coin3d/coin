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
 *  http://www.sim.no/ sales@sim.no Voice: +47 73540378 Fax: +47 73943861
 *
\**************************************************************************/

/*!
  \class SoSwitch SoSwitch.h Inventor/nodes/SoSwitch.h
  \brief The SoSwitch class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/SbName.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/misc/SoChildList.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/errors/SoDebugError.h>

#if !defined(COIN_EXCLUDE_SOACTION)
#include <Inventor/actions/SoAction.h>
#endif // !COIN_EXCLUDE_SOACTION
#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION
#if !defined(COIN_EXCLUDE_SOSEARCHACTION)
#include <Inventor/actions/SoSearchAction.h>
#endif // !COIN_EXCLUDE_SOSEARCHACTION
#if !defined(COIN_EXCLUDE_SOGETMATRIXACTION)
#include <Inventor/actions/SoGetMatrixAction.h>
#endif // !COIN_EXCLUDE_SOGETMATRIXACTION
#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
#include <Inventor/actions/SoGLRenderAction.h>
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SOSWITCHELEMENT)
#include <Inventor/elements/SoSwitchElement.h>
#endif // !COIN_EXCLUDE_SOSWITCHELEMENT

/*!
  \var SoSFInt32 SoSwitch::whichChild
  FIXME: write documentation for field
*/

// *************************************************************************

//$ BEGIN TEMPLATE NodeSource(SoSwitch)
SoType SoSwitch::classTypeId = SoType::badType();

/*!
  Returns a new instance of the SoSwitch node class.
*/
void *
SoSwitch::createInstance(void)
{
  return new SoSwitch;
}

/*!
  Returns the unique type identifier for the SoSwitch class.
*/
SoType
SoSwitch::getClassTypeId(void)
{
  return SoSwitch::classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoSwitch::getTypeId(void) const
{
  return SoSwitch::classTypeId;
}
//$ END TEMPLATE NodeSource

/*!
  Constructor.
*/
SoSwitch::SoSwitch()
{
//$ BEGIN TEMPLATE NodeConstructor(SoSwitch)
  // Make sure the class has been initialized.
  assert(SoSwitch::classTypeId != SoType::badType());
//$ END TEMPLATE NodeConstructor

  SO_NODE_ADD_FIELD(whichChild, (SO_SWITCH_NONE));
}

/*!
  Destructor.
*/
SoSwitch::~SoSwitch()
{
}

/*!
  Does initialization common for all objects of the
  SoSwitch class. This includes setting up the
  type system, among other things.
*/
void
SoSwitch::initClass(void)
{
//$ BEGIN TEMPLATE InitNodeSource(Switch)
  // Make sure we only initialize once.
  assert(SoSwitch::classTypeId == SoType::badType());
  // Make sure superclass get initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoSwitch::classTypeId =
    SoType::createType(inherited::getClassTypeId(), "Switch",
                       &SoSwitch::createInstance,
                       SoNode::nextActionMethodIndex++);
//$ END TEMPLATE InitNodeSource

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
  SO_ENABLE(SoGetBoundingBoxAction, SoSwitchElement);
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION
#if !defined(COIN_EXCLUDE_SOSEARCHACTION)
  SO_ENABLE(SoSearchAction, SoSwitchElement);
#endif // !COIN_EXCLUDE_SOSEARCHACTION
#if !defined(COIN_EXCLUDE_SOGETMATRIXACTION)
  SO_ENABLE(SoGetMatrixAction, SoSwitchElement);
#endif // !COIN_EXCLUDE_SOGETMATRIXACTION
#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  SO_ENABLE(SoGLRenderAction, SoSwitchElement);
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
} // initClass()

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoSwitch::cleanClass(void)
{
}

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write function documentation
*/
void 
SoSwitch::GLRender(SoGLRenderAction * action)
{
  this->doAction(action);
}
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
/*!
  FIXME: write function documentation
*/
void 
SoSwitch::getBoundingBox(SoGetBoundingBoxAction * action)
{
  // FIXME: update this method to use a doAction() kind of
  // traversal. This traversal code is not correct i think.
  // pederb, 990618
  
  int numIndices;    
  const int * indices;     
  int idx;

  if (action->getPathCode(numIndices, indices) == SoAction::IN_PATH)
    idx = indices[numIndices-1];
  else 
    idx = whichChild.getValue();

  if (idx == SO_SWITCH_INHERIT) idx = SoSwitchElement::get(action->getState());
  else SoSwitchElement::set(action->getState(), idx);

  if (idx == SO_SWITCH_NONE) return;
  
  // FIXME: handle gracefully. 19990324 mortene.
  assert((idx < this->getNumChildren() && idx >= 0) 
	 || idx == SO_SWITCH_ALL);

  if (idx == SO_SWITCH_ALL) {
    // Initialize accumulation variables.
    SbVec3f acccenter(0.0f, 0.0f, 0.0f);
    int numCenters = 0;

    for (int i = 0; i < this->getNumChildren(); i++) {
      this->getChildren()->traverse(action, i);
    
      // If center point is set, accumulate.
      if (action->isCenterSet()) {
	acccenter += action->getCenter();
	numCenters++;
	action->resetCenter();
      }
    }

    if (numCenters != 0)
      action->setCenter(acccenter / numCenters, FALSE);
  }
  else {
    this->getChildren()->traverse(action, idx);
  }
}
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION

#if !defined(COIN_EXCLUDE_SOSEARCHACTION)
/*!
  FIXME: write function documentation
*/
void
SoSwitch::search(SoSearchAction * action)
{
  // Include this node in the search.
  SoNode::search(action);
  if (action->isFound()) return;

  // TRAVERSE
  if (action->isSearchingAll()) { // traverse all children no matter
    getChildren()->traverse(action);
  } else {
    doAction(action);
  }
}
#endif // !COIN_EXCLUDE_SOSEARCHACTION

#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write function documentation
*/
void 
SoSwitch::doAction(SoAction * action)
{
  SoState *state = action->getState();
  
  int numIndices;
  const int *indices;

  SbBool inheritIdx = FALSE;
  int idx = whichChild.getValue();
  if (idx == SO_SWITCH_INHERIT) {
    inheritIdx = TRUE;
    idx = SoSwitchElement::get(action->getState());
  }
  
  if (idx == SO_SWITCH_NONE) return;
  
  SbBool checkAffects = FALSE;
  int startIdx, endIdx;

  if (idx == SO_SWITCH_ALL) {
    startIdx = 0;
    endIdx = getNumChildren()-1;
  }
  else {
    startIdx = endIdx = idx;
  }
  
  switch (action->getPathCode(numIndices, indices)) {
  case SoAction::IN_PATH:
    // FIXME: not necessary to traverse children which
    // do not affect state and is not in indices[]
    
    if (idx == SO_SWITCH_ALL) {
      // only traverse until last child in paths
      endIdx = indices[numIndices-1];
    }
    else {
      // probably not possible with numIndices != 1 ?
      for (int i = 0; i < numIndices; i++) {
	if (!inheritIdx) SoSwitchElement::set(state, this, indices[i]);
	this->children->traverse(action, indices[i]);
      }
      endIdx = -1; // traversing is finished
    }
    break;

  case SoAction::OFF_PATH:
    checkAffects = TRUE;
    // otherwise normal behaviour
    break;
  }
  
  for (int i = startIdx; i <= endIdx; i++) {
    if (!checkAffects || this->getChild(i)->affectsState()) {
      if (!inheritIdx) SoSwitchElement::set(state, this, i);
      this->children->traverse(action, i);
    }
  }
}
#endif // !COIN_EXCLUDE_SOACTION



/*!
  FIXME: write doc
 */
SbBool
SoSwitch::affectsState(void) const
{
  int idx = whichChild.getValue();
  if (idx == SO_SWITCH_NONE) return FALSE;
  if (idx >= this->getNumChildren()) return FALSE;
  if (idx >= 0 && !this->getChild(idx)->affectsState()) return FALSE;
  
  // inherited or all
  return TRUE;
}

#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
/*!
  FIXME: write doc
 */
void
SoSwitch::callback(SoCallbackAction * action)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOCALLBACKACTION

#if !defined(COIN_EXCLUDE_SOPICKACTION)
/*!
  FIXME: write doc
 */
void
SoSwitch::pick(SoPickAction * action)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOPICKACTION

#if !defined(COIN_EXCLUDE_SOHANDLEEVENTACTION)
/*!
  FIXME: write doc
 */
void
SoSwitch::handleEvent(SoHandleEventAction * action)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOHANDLEEVENTACTION

#if !defined(COIN_EXCLUDE_SOGETMATRIXACTION)
/*!
  FIXME: write doc
 */
void
SoSwitch::getMatrix(SoGetMatrixAction * action)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOGETMATRIXACTION

#if !defined(COIN_EXCLUDE_SOWRITEACTION)
/*!
  FIXME: write doc
 */
void
SoSwitch::write(SoWriteAction * action)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOWRITEACTION

#if !defined(COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION)
/*!
  FIXME: write doc
 */
void
SoSwitch::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION

#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write doc
 */
void
SoSwitch::traverseChildren(SoAction * action)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOACTION
