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
  \class SoGroup SoGroup.h Inventor/nodes/SoGroup.h
  \brief The SoGroup class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoGroup.h>
#include <assert.h>


#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/misc/SoChildList.h>
#include <Inventor/actions/SoAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/errors/SoReadError.h>
#include <Inventor/actions/SoCallbackAction.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

static const char * const DEFINITION_KEYWORD = "DEF";
static const char * const REFERENCE_KEYWORD  = "USE";

/*!
  \var SoChildList * SoGroup::children
  FIXME: write doc
*/

// *************************************************************************

SO_NODE_SOURCE(SoGroup);

/*!
  Constructor.
*/
SoGroup::SoGroup()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoGroup);

  this->children = new SoChildList(this);
}

/*!
  FIXME: write function documentation
*/
SoGroup::SoGroup(int /* nchildren */)
{
  COIN_STUB();
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
SoGroup::readInstance(SoInput * in, unsigned short /* flags */)
{
  SbName typeString;
  // FIXME: cast-hack to compile. 19980915 mortene.
  SoFieldData * fdata = (SoFieldData *)this->getFieldData();

  // FIXME: tmp disabled reading of not built in node types, as I need
  // to find out how to do this properly for both ascii and binary
  // formats (.iv and .wrl). 19990708 mortene.
#if 0
  if (in->read(typeString, TRUE)) {
    if (typeString == "fields") {
      if (!fdata->readFieldTypes(in, this)) {
        SoReadError::post(in, "Bad field specifications for node");
        return FALSE;
      }
    }
    else {
      in->putBack(typeString.getString());
    }
  }
#endif

  SbBool notbuiltin;
  return (fdata->read(in, this, FALSE, notbuiltin) && this->readChildren(in));
}

/*!
  Read all children of this node from \a in and attach them below this group.
  Returns \a FALSE upon read error.
*/
SbBool
SoGroup::readChildren(SoInput * in)
{
  SbBool ret = TRUE;

  if (in->isBinary()) {
    unsigned int numchildren;
    ret = in->read(numchildren);

    for (unsigned int i=0; (i < numchildren) && ret; i++) {
      SoBase * child = NULL;
      if ((ret = SoBase::read(in, child, SoNode::getClassTypeId()))) {
        if (child == NULL) {
          if (in->eof())
            SoReadError::post(in, "File corrupt, premature end of file");
          else
            SoReadError::post(in, "``NULL'' keyword misplaced");
          ret = FALSE;
        }
        else {
          this->addChild((SoNode *)child);
        }
      }
    }
  }
  else {
#if 0 // new code
    SbBool done = FALSE;
    while (!done) {
      SoBase * child = NULL;
      if ((ret = SoBase::read(in, child, SoNode::getClassTypeId())) && child) {
        this->addChild((SoNode *)child);
      }
      else {
        if (ret && !child && !in->eof()) {
          SoReadError::post(in, "``NULL'' keyword misplaced");
          ret = FALSE;
        }
        done = TRUE;
      }
    }
#else // old code
    while (ret) {
      SoBase * child = NULL;

      if ((ret = SoBase::read(in, child, SoNode::getClassTypeId()))) {
        if (child != NULL) this->addChild((SoNode *)child);
        else break;
      }
    }
#endif
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

  for (int i=0;i<getNumChildren();i++) {
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
}

/*!
  FIXME: write function documentation
*/
void
SoGroup::insertChild(SoNode * const child, const int newChildIndex)
{
  this->children->insert(child, newChildIndex);
}

/*!
  FIXME: write function documentation
*/
void
SoGroup::removeChild(const int childIndex)
{
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
  SO_NODE_INTERNAL_INIT_CLASS(SoGroup);
}


/*!
  FIXME: write function documentation
*/
void
SoGroup::doAction(SoAction * action)
{
  int numIndices;
  const int * indices;
  switch (action->getPathCode(numIndices, indices)) {
  case SoAction::IN_PATH:
    // FIXME: not necessary to traverse children which do not
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
    action->setCenter(acccenter / float(numCenters), FALSE);
}

/*!
  FIXME: write function documentation
*/
void
SoGroup::GLRender(SoGLRenderAction * action)
{
  SoGroup::doAction(action);
}

/*!
  FIXME: write function documentation
*/
void
SoGroup::callback(SoCallbackAction * action)
{
  action->invokePreCallbacks(this);
  if (action->getCurrentResponse() == SoCallbackAction::CONTINUE) {
    SoGroup::doAction((SoAction *)action);
    action->invokePostCallbacks(this);
  }
}

/*!
  FIXME: write function documentation
*/
void
SoGroup::getMatrix(SoGetMatrixAction * action)
{
  switch (action->getCurPathCode()) {
  case SoAction::NO_PATH:
  case SoAction::BELOW_PATH:
    break;
  case SoAction::OFF_PATH:
  case SoAction::IN_PATH:
    SoGroup::doAction((SoAction *)action);
    break;
  }
}

/*!
  FIXME: write function documentation
*/
void
SoGroup::pick(SoPickAction * action)
{
  SoGroup::doAction((SoAction *)action);
}

/*!
  FIXME: write function documentation
*/
void
SoGroup::handleEvent(SoHandleEventAction * action)
{
  SoGroup::doAction((SoAction *)action);
}

/*!
  Write action method is overloaded from SoNode to call
  SoBase::addWriteReference() on the children of the group.
*/
void
SoGroup::write(SoWriteAction * action)
{
  SoOutput * out = action->getOutput();
  if (out->getStage() == SoOutput::COUNT_REFS) {
#if 0 // OBSOLETED: this code is plain wrong, I believe. 19991112 mortene.
    int n = this->getNumChildren();
    for (int i = 0; i < n; i++)
      this->getChild(i)->addWriteReference(out, FALSE);
#endif // OBSOLETED
    inherited::write(action);
    // Only increase number of writereferences to the top level node
    // in a tree which is used multiple times.
    if (!this->hasMultipleWriteRefs()) SoGroup::doAction((SoAction *)action);
  }
  else if (out->getStage() == SoOutput::WRITE) {
    if (this->writeHeader(out, TRUE, FALSE)) return;
    this->writeInstance(out);
    if (out->isBinary()) out->write(this->getNumChildren());
    SoGroup::doAction((SoAction *)action);
    this->writeFooter(out);
  }
  else assert(0 && "unknown stage");
}

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

/*!
  FIXME: write function documentation
*/
SoChildList *
SoGroup::getChildren(void) const
{
  return ((SoGroup *)this)->children;
}

/*!
  FIXME: write function documentation
*/
void
SoGroup::removeChild(SoNode * const child)
{
  int idx = this->findChild(child);
  if (idx >= 0) {
    this->removeChild(idx);
  }
  else {
    // FIXME: write some debug info
  }
}

/*!
  FIXME: write function documentation
*/
void
SoGroup::removeAllChildren(void)
{
  int n = this->getNumChildren();
  while (n) {
    n--;
    this->removeChild(n);
  }
}

/*!
  FIXME: write function documentation
*/
void
SoGroup::replaceChild(const int index, SoNode * const newChild)
{
  this->removeChild(index);
  this->insertChild(newChild, index);
}

/*!
  FIXME: write function documentation
*/
void
SoGroup::replaceChild(SoNode * const oldChild, SoNode * const newChild)
{
  this->replaceChild(this->findChild(oldChild), newChild);
}

/*!
  FIXME: write doc
*/
void
SoGroup::getPrimitiveCount(SoGetPrimitiveCountAction *action)
{
  SoGroup::doAction((SoAction*)action);
}
