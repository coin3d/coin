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
  \class SoGroup SoGroup.h Inventor/nodes/SoGroup.h
  \brief The SoGroup class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoGroup.h>
#include <assert.h>

#include <Inventor/SbName.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/misc/SoChildList.h>
#if !defined(COIN_EXCLUDE_SOACTION)
#include <Inventor/actions/SoAction.h>
#endif // !COIN_EXCLUDE_SOACTION
#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION
#if !defined(COIN_EXCLUDE_SOGETMATRIXACTION)
#include <Inventor/actions/SoGetMatrixAction.h>
#endif // !COIN_EXCLUDE_SOGETMATRIXACTION
#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
#include <Inventor/actions/SoGLRenderAction.h>
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
#if !defined(COIN_EXCLUDE_SOSEARCHACTION)
#include <Inventor/actions/SoSearchAction.h>
#endif // !COIN_EXCLUDE_SOSEARCHACTION
#if !defined(COIN_EXCLUDE_SOWRITEACTION)
#include <Inventor/actions/SoWriteAction.h>
#endif // !COIN_EXCLUDE_SOWRITEACTION
#include <Inventor/errors/SoReadError.h>

static const char * const DEFINITION_KEYWORD = "DEF";
static const char * const REFERENCE_KEYWORD  = "USE";

/*!
  \var SoChildList * SoGroup::children
  FIXME: write doc
*/

// *************************************************************************

//$ BEGIN TEMPLATE NodeSource(SoGroup)
SoType SoGroup::classTypeId = SoType::badType();

/*!
  Returns a new instance of the SoGroup node class.
*/
void *
SoGroup::createInstance(void)
{
  return new SoGroup;
}

/*!
  Returns the unique type identifier for the SoGroup class.
*/
SoType
SoGroup::getClassTypeId(void)
{
  return SoGroup::classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoGroup::getTypeId(void) const
{
  return SoGroup::classTypeId;
}
//$ END TEMPLATE NodeSource

/*!
  Constructor.
*/
SoGroup::SoGroup()
{
//$ BEGIN TEMPLATE NodeConstructor(SoGroup)
  // Make sure the class has been initialized.
  assert(SoGroup::classTypeId != SoType::badType());
//$ END TEMPLATE NodeConstructor

  this->children = new SoChildList(this);
}

/*!
  FIXME: write function documentation
*/
SoGroup::SoGroup(int nchildren)
{
  assert(0 && "FIXME: not implemented");
}

/*!
  Destructor.
*/
SoGroup::~SoGroup()
{
  delete this->children;
}

/*!
  FIXME: write function documentation
*/
SoNode *
SoGroup::getChild(const int index) const
{
  assert((index >= 0) && (index < getNumChildren()));

  return (*this->children)[index];
}

/*!
  FIXME: write function documentation
*/
int
SoGroup::getNumChildren() const
{
  return this->children->getLength();
}

/*!
  FIXME: write function documentation
*/
SbBool
SoGroup::readInstance(SoInput * in, unsigned short flags)
{
  SbName typeString;
  // FIXME: cast-hack to compile. 19980915 mortene.
  SoFieldData * fdata = (SoFieldData *)this->getFieldData();

  if(in->read(typeString, TRUE)) {
    if(typeString == "fields") {
      if(!fdata->readFieldTypes(in, this)) {
	SoReadError::post(in, "Bad field specifications for node");
	return FALSE;
      }
    }
    else {
      in->putBack(typeString.getString());
    }
  }

  SbBool notbuiltin;
  return (fdata->read(in, this, FALSE, notbuiltin) && readChildren(in));
}

/*!
  FIXME: write function documentation
*/
SbBool
SoGroup::readChildren(SoInput * in)
{
  SoBase * child;
  SbBool ret = TRUE;
 
  while(TRUE) {
    child = NULL;
    if (this->read(in, child, SoNode::getClassTypeId())) {
      if (child == (SoBase *)-1); // ROUTE
      else if (child != NULL) this->addChild((SoNode *)child);
      else break;

#if 0 // FIXME: progress aware code tmp disabled. 19980923 mortene
      if(!SoDB::progress(in)) {
	ret = FALSE;
	break;
      }
#endif // disabled
    }
    else {
      if(child && (child != (SoBase *)-1)) {
	child->ref();
	child->unref();
      }
      ret = FALSE;
      break;
    }
  }
 
  return ret;
}

/*!
  FIXME: write function documentation
*/
SoNode *
SoGroup::copy(void)
{
  SoGroup * newnode = (SoGroup *)SoNode::copy();

  for(int i=0;i<getNumChildren();i++) {
    SoNode * newchild = getChild(i)->copy();
    newnode->addChild(newchild);
  }

  return newnode;
}

/*!
  FIXME: write function documentation
*/
void
SoGroup::addChild(SoNode * const node)
{
  assert(node != NULL);
  this->children->append(node);

  node->addAuditor(this, SoNotRec::PARENT);
}

/*!
  FIXME: write function documentation
*/
void
SoGroup::insertChild(SoNode * const child, const int newChildIndex)
{
  this->children->insert(child, newChildIndex);

  child->addAuditor(this, SoNotRec::PARENT);
}

/*!
  FIXME: write function documentation
*/
void
SoGroup::removeChild(const int childIndex)
{
  (*this->children)[childIndex]->removeAuditor(this, SoNotRec::PARENT);

  this->children->remove(childIndex);
}

/*!
  FIXME: write function documentation
*/
int
SoGroup::findChild(const SoNode * const node) const
{
  return this->children->find((SoNode *) node);
}

/*!
  Does initialization common for all objects of the
  SoGroup class. This includes setting up the
  type system, among other things.
*/
void
SoGroup::initClass()
{
//$ BEGIN TEMPLATE InitNodeSource(Group)
  // Make sure we only initialize once.
  assert(SoGroup::classTypeId == SoType::badType());
  // Make sure superclass get initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoGroup::classTypeId =
    SoType::createType(inherited::getClassTypeId(), "Group",
                       &SoGroup::createInstance,
                       SoNode::nextActionMethodIndex++);
//$ END TEMPLATE InitNodeSource
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoGroup::cleanClass()
{
}


#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write function documentation
*/
void 
SoGroup::doAction(SoAction * action)
{
  int numIndices;
  const int * indices;
  switch(action->getPathCode(numIndices, indices)) {
  case SoAction::IN_PATH: 
    // FIXME: not necessary to traverse children which do no
    // affect state and is not in indices[] ?
    // But, traversal will stop pretty soon anyway, so it might
    // be slower to include a check here. pederb, 990618
    this->children->traverse(action, 0, indices[numIndices - 1]);
    break;
    
  case SoAction::NO_PATH:
  case SoAction::BELOW_PATH:
    this->children->traverse(action); // traverse all children
    break;
  
  case SoAction::OFF_PATH:
    {
      int n = this->getNumChildren();
      for (int i = 0; i < n; i++) {
	if (this->getChild(i)->affectsState())
	  this->children->traverse(action, i);
      }
      break;
    }
  default:
    assert(0 && "Unknown path code");
    break;
  }
}
#endif // !COIN_EXCLUDE_SOACTION

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
/*!
  FIXME: write function documentation
*/
void
SoGroup::getBoundingBox(SoGetBoundingBoxAction * action)
{
  int numIndices;    
  const int * indices;     
  int lastChildIndex;
  
  if (action->getPathCode(numIndices, indices) == SoAction::IN_PATH)
    lastChildIndex = indices[numIndices-1];
  else 
    lastChildIndex = getNumChildren() - 1;
  
  // Initialize accumulation variables.
  SbVec3f acccenter(0.0f, 0.0f, 0.0f);
  int numCenters = 0;
  
  for (int i = 0; i <= lastChildIndex; i++) {
    this->children->traverse(action, i);
    
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
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write function documentation
*/
void 
SoGroup::GLRender(SoGLRenderAction * action)
{
  SoGroup::doAction(action);
}
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
/*!
  FIXME: write function documentation
*/
void 
SoGroup::callback(SoCallbackAction * action)
{
  SoGroup::doAction((SoAction *)action);
}
#endif // COIN_EXCLUDE_SOCALLBACKACTION

#if !defined(COIN_EXCLUDE_SOGETMATRIXACTION)
/*!
  FIXME: write function documentation
*/
void 
SoGroup::getMatrix(SoGetMatrixAction * action)
{
  SoGroup::doAction((SoAction *)action);
}
#endif // !COIN_EXCLUDE_SOGETMATRIXACTION

#if !defined(COIN_EXCLUDE_SOPICKACTION)
/*!
  FIXME: write function documentation
*/
void 
SoGroup::pick(SoPickAction * action)
{
  SoGroup::doAction((SoAction *)action);
}
#endif // !COIN_EXCLUDE_SOPICKACTION

#if !defined(COIN_EXCLUDE_SOHANDLEEVENTACTION)
/*!
  FIXME: write function documentation
*/
void 
SoGroup::handleEvent(SoHandleEventAction * action)
{
  SoGroup::doAction((SoAction *)action);
}
#endif // !COIN_EXCLUDE_SOHANDLEEVENTACTION

#if !defined(COIN_EXCLUDE_SOWRITEACTION)
/*!
  FIXME: write function documentation
*/
void 
SoGroup::write(SoWriteAction * action)
{
  // FIXME: is this code correct? 19990629 mortene.

  SoOutput * out = action->getOutput();
  if (this->writeHeader(out, TRUE, FALSE)) return;
  SoGroup::doAction((SoAction *)action);
  this->writeFooter(out);
}
#endif // !COIN_EXCLUDE_SOWRITEACTION

#if !defined(COIN_EXCLUDE_SOSEARCHACTION)
/*!
  FIXME: write function documentation
*/
void 
SoGroup::search(SoSearchAction * action)
{
  // Include this node in the search.
  inherited::search(action);
  if (action->isFound()) return;

  // If we're not the one being sought after, try child subgraphs.
  SoGroup::doAction((SoAction *)action);
}
#endif // !COIN_EXCLUDE_SOSEARCHACTION

/*!
  FIXME: write function documentation
*/
SoChildList *
SoGroup::getChildren() const
{
  return ((SoGroup *)this)->children;
}

/*!
  FIXME: write function documentation
*/
void 
SoGroup::removeChild(SoNode * const child)
{
  int idx = findChild(child);
  if (idx >= 0) {
    removeChild(idx);
  }
  else {
    // FIXME: write some debug info
  }
}

/*!
  FIXME: write function documentation
*/
void 
SoGroup::removeAllChildren()
{
  int n = getNumChildren();
  while (n) {
    n--;
    removeChild(n);
  }
}

/*!
  FIXME: write function documentation
*/
void 
SoGroup::replaceChild(const int index, SoNode * const newChild)
{
  removeChild(index);
  insertChild(newChild, index);
}

/*!
  FIXME: write function documentation
*/
void 
SoGroup::replaceChild(SoNode * const oldChild, SoNode * const newChild)
{
  replaceChild(findChild(oldChild), newChild);
}


#if !defined(COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION)
/*!
  FIXME: write doc
*/
void
SoGroup::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION
