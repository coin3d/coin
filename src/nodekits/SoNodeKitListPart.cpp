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
  \class SoNodeKitListPart SoNodeKitListPart.h Inventor/nodekits/SoNodeKitListPart.h
  \brief The SoNodeKitListPart class is a container node.
  \ingroup nodekits

  This node is basically just a container node with these differences versus
  the other group nodes:

  1. It is possible to make a list of which node class types are
  allowed as children.

  2. You can freely select which kind of group node behaviour you want
  this container to have. Default is to act like an SoGroup node, but
  by using SoNodeKitListPart::setContainerType(), you can change the
  behaviour to be like a separator, a switch or whatever else you have
  of node types inheriting SoGroup.

  \sa SoGroup, SoSeparator, SoSwitch
 */

/*¡
  Import, export and copy functionality is missing.
 */

#include <Inventor/nodekits/SoNodeKitListPart.h>
#include <Inventor/SbName.h>
#include <Inventor/SoType.h>
#include <Inventor/lists/SoTypeList.h>
#include <Inventor/misc/SoChildList.h>
#include <Inventor/nodes/SoGroup.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

SO_NODE_SOURCE(SoNodeKitListPart);


/*!
  Constructor.
*/
SoNodeKitListPart::SoNodeKitListPart()
{
  SO_NODE_CONSTRUCTOR(SoNodeKitListPart);

  this->root = new SoGroup;
  this->root->ref();

  this->typelistlocked = FALSE;
}

/*!
  Destructor.
*/
SoNodeKitListPart::~SoNodeKitListPart()
{
  this->root->unref();
}

/*!
  Does initialization common for all objects of the
  SoNodeKitListPart class. This includes setting up the
  type system, among other things.
*/
void
SoNodeKitListPart::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoNodeKitListPart);
}

/*!
  Return type id for the list container. Default is SoGroup.
*/
SoType
SoNodeKitListPart::getContainerType(void) const
{
  return this->root->getTypeId();
}

/*!
  Change the behaviour of this container node. \a newContainerType
  must be derived from SoGroup.
*/
void
SoNodeKitListPart::setContainerType(SoType newContainerType)
{
  if (this->typelistlocked) {
#if COIN_DEBUG
    SoDebugError::post("SoNodeKitListPart::setContainerType",
                       "type list has been locked");
#endif // COIN_DEBUG
    return;
  }

#if COIN_DEBUG
  if (!newContainerType.isDerivedFrom(SoGroup::getClassTypeId())) {
    SoDebugError::postInfo("SoNodeKitListPart::setContainerType",
                           "'%s' is not a group node class type",
                           newContainerType.getName().getString());
    return;
  }
#endif // debug

  SoGroup * newroot = (SoGroup *) newContainerType.createInstance();
  newroot->ref();

  SoChildList * children = this->getChildren();
  for (int i=0; i < children->getLength(); i++) {
    newroot->addChild((*children)[i]);
  }

  this->root->unref();
  this->root = newroot;
}

/*!
  Return a list of all types which are allowed as children of this node.
  Default is to allow all nodetypes.
*/
const SoTypeList &
SoNodeKitListPart::getChildTypes(void) const
{
  if (this->allowedtypes.getLength()) return this->allowedtypes;

  static SoTypeList deflist;
  if (deflist.getLength() == 0) deflist.append(SoNode::getClassTypeId());
  return deflist;
}

/*!
  Add one more node class type which should be allowed to be used in the
  node list.

  Note: the default single SoNode type in the list will be removed
  upon the first call to this method.
*/
void
SoNodeKitListPart::addChildType(SoType typeToAdd)
{
  if (this->typelistlocked) {
#if COIN_DEBUG
    SoDebugError::post("SoNodeKitListPart::addChildType",
                       "type list has been locked");
#endif // COIN_DEBUG
    return;
  }

  for (int i=0; i < this->allowedtypes.getLength(); i++) {
    if (this->allowedtypes[i] == typeToAdd) {
#if COIN_DEBUG
      SoDebugError::post("SoNodeKitListPart::addChildType",
                         "type '%s' already present in the list",
                         typeToAdd.getName().getString());
#endif // COIN_DEBUG
      return;
    }
  }
  this->allowedtypes.append(typeToAdd);
}

/*!
  Return \c TRUE if nodes of the \a typeToCheck class type can be added
  to the list.
*/
SbBool
SoNodeKitListPart::isTypePermitted(SoType typeToCheck) const
{
  int numtypes = this->allowedtypes.getLength();

  if ((numtypes == 0) && typeToCheck.isDerivedFrom(SoNode::getClassTypeId()))
    return TRUE;

  for (int i=0; i < numtypes; i++) {
    if (typeToCheck.isDerivedFrom(this->allowedtypes[i])) return TRUE;
  }
  return FALSE;
}

/*!
  Return \c TRUE if \a child has a class type which is permitted to be
  in the list.
*/
SbBool
SoNodeKitListPart::isChildPermitted(const SoNode * child) const
{
  return this->isTypePermitted(child->getTypeId());
}

/*!
  Calls set() on the container node with \a fieldDataString.
*/
void
SoNodeKitListPart::containerSet(const char * fieldDataString)
{
  this->root->set(fieldDataString);
}

/*!
  Lock type list so no more node class types can be added by using
  addChildType(), and locks container type so it no longer can be
  changed by setContainerType().
*/
void
SoNodeKitListPart::lockTypes(void)
{
  this->typelistlocked = TRUE;
}

/*!
  Returns \c TRUE if the list of allowable node types and the container
  type have both been locked.
*/
SbBool
SoNodeKitListPart::isTypeLocked(void) const
{
  return this->typelistlocked;
}

/*!
  Adds \a child to the internal list of children, if it is of a
  type permitted to be added.
*/
void
SoNodeKitListPart::addChild(SoNode * child)
{
  if (!this->isChildPermitted(child)) {
#if COIN_DEBUG
    SoDebugError::post("SoNodeKitListPart::addChild",
                       "can't add child of type '%s'",
                       child->getTypeId().getName().getString());
#endif // COIN_DEBUG
    return;
  }

  this->root->addChild(child);
}

/*!
  Inserts \a child in the internal list of children at position
  \c childIndex, if it is of a type permitted to be added.
*/
void
SoNodeKitListPart::insertChild(SoNode * child, int childIndex)
{
  if (!this->isChildPermitted(child)) {
#if COIN_DEBUG
    SoDebugError::post("SoNodeKitListPart::insertChild",
                       "can't insert child of type '%s'",
                       child->getTypeId().getName().getString());
#endif // COIN_DEBUG
    return;
  }

  this->root->insertChild(child, childIndex);
}

/*!
  Return child node at position \a index.
*/
SoNode *
SoNodeKitListPart::getChild(int index) const
{
  return this->root->getChild(index);
}

/*!
  Return position index of \a child in list of children.
*/
int
SoNodeKitListPart::findChild(SoNode * child) const
{
  return this->root->findChild(child);
}

/*!
  Return number of children.
*/
int
SoNodeKitListPart::getNumChildren(void) const
{
  return this->root->getNumChildren();
}

/*!
  Remove child at position \a index.
*/
void
SoNodeKitListPart::removeChild(int index)
{
  this->root->removeChild(index);
}

/*!
  Remove \a child from list of children.
*/
void
SoNodeKitListPart::removeChild(SoNode * child)
{
  this->root->removeChild(child);
}

/*!
  Replace child at \a index with \a newChild, if \a newChild is of a
  permitted type.
*/
void
SoNodeKitListPart::replaceChild(int index, SoNode * newChild)
{
  if (!this->isChildPermitted(newChild)) {
#if COIN_DEBUG
    SoDebugError::post("SoNodeKitListPart::replaceChild",
                       "can't replace with child of type '%s'",
                       newChild->getTypeId().getName().getString());
#endif // COIN_DEBUG
    return;
  }

  this->root->replaceChild(index, newChild);
}

/*!
  Replace \a oldChild with \a newChild, if \a newChild is of a
  permitted type.
*/
void
SoNodeKitListPart::replaceChild(SoNode * oldChild, SoNode * newChild)
{
  if (!this->isChildPermitted(newChild)) {
#if COIN_DEBUG
    SoDebugError::post("SoNodeKitListPart::replaceChild",
                       "can't replace with child of type '%s'",
                       newChild->getTypeId().getName().getString());
#endif // COIN_DEBUG
    return;
  }

  this->root->replaceChild(oldChild, newChild);
}

/*!
  This just "forwards" the call to the same method at the container node.
*/
SbBool
SoNodeKitListPart::affectsState(void) const
{
  return this->root->affectsState();
}

/*!
  This just "forwards" the call to the same method at the container node.
*/
void
SoNodeKitListPart::doAction(SoAction * action)
{
  this->root->doAction(action);
}

/*!
  This just "forwards" the call to the same method at the container node.
*/
void
SoNodeKitListPart::callback(SoCallbackAction * action)
{
  this->root->callback(action);
}

/*!
  This just "forwards" the call to the same method at the container node.
*/
void
SoNodeKitListPart::GLRender(SoGLRenderAction * action)
{
  this->root->GLRender(action);
}

/*!
  This just "forwards" the call to the same method at the container node.
*/
void
SoNodeKitListPart::getBoundingBox(SoGetBoundingBoxAction * action)
{
  this->root->getBoundingBox(action);
}

/*!
  This just "forwards" the call to the same method at the container node.
*/
void
SoNodeKitListPart::getMatrix(SoGetMatrixAction * action)
{
  this->root->getMatrix(action);
}

/*!
  This just "forwards" the call to the same method at the container node.
*/
void
SoNodeKitListPart::handleEvent(SoHandleEventAction * action)
{
  this->root->handleEvent(action);
}

/*!
  This just "forwards" the call to the same method at the container node.
*/
void
SoNodeKitListPart::pick(SoPickAction * action)
{
  this->root->pick(action);
}

/*!
  This just "forwards" the call to the same method at the container node.
*/
void
SoNodeKitListPart::search(SoSearchAction * action)
{
  this->root->search(action);
}

/*!
  This just "forwards" the call to the same method at the container node.
*/
void
SoNodeKitListPart::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  this->root->getPrimitiveCount(action);
}

/*!
  This just "forwards" the call to the same method at the container node.
*/
SoChildList *
SoNodeKitListPart::getChildren(void) const
{
  return this->root->getChildren();
}

/*!
  Return the SoGroup container which is the parent of all the children
  which has been added.
*/
SoGroup *
SoNodeKitListPart::getContainerNode(void)
{
  return this->root;
}

/*!
  FIXME: write function documentation
*/
SbBool
SoNodeKitListPart::readInstance(SoInput * /*in*/, unsigned short /*flags*/)
{
  assert(0 && "FIXME: not implemented yet");
  return FALSE;
}

/*!
  FIXME: write function documentation
*/
void
SoNodeKitListPart::copyContents(const SoFieldContainer * /*fromFC*/,
                                SbBool /*copyConnections*/)
{
  assert(0 && "FIXME: not implemented yet");
}
