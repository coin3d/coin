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
  \class SoVRMLParent SoVRMLParent.h Inventor/VRMLnodes/SoVRMLParent.h
  \brief The SoVRMLParent node is a superclass for all VRML grouping nodes.
*/

#include <Inventor/VRMLnodes/SoVRMLParent.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/misc/SoChildList.h>
#include <Inventor/fields/SoSFNode.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/fields/SoFieldData.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/SoOutput.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#ifdef COIN_THREADSAFE
#include <Inventor/threads/SbMutex.h>
#endif // COIN_THREADSAFE

/*!
  \var SoMFNode SoVRMLParent::children
  The children nodes.
*/

/*!
  \var SoMFNode SoVRMLParent::addChildren
  An event in that is used to add children to this node.
*/

/*!
  \var SoMFNode SoVRMLParent::removeChildren
  An event in that is used to remove children from this node.
*/

#ifndef DOXYGEN_SKIP_THIS

class SoVRMLParentP {
public:
  SbBool childlistvalid;
  SoFieldSensor * addsensor;
  SoFieldSensor * removesensor;

#ifdef COIN_THREADSAFE
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

#endif // DOXYGEN_SKIP_PRIVATE(this)


SO_NODE_ABSTRACT_SOURCE(SoVRMLParent);

// Doc in parent
void
SoVRMLParent::initClass(void)
{
  SO_NODE_INTERNAL_INIT_ABSTRACT_CLASS(SoVRMLParent, SO_VRML97_NODE_TYPE);
}

#define PRIVATE(thisp) ((thisp)->pimpl)

/*!
  Constructor.
*/
SoVRMLParent::SoVRMLParent(void)
{
  this->commonConstructor();
}

/*!
  Destructor. \a numchildren is the expected number of children.
*/
SoVRMLParent::SoVRMLParent(int numchildren)
  : SoGroup(numchildren)
{
  this->commonConstructor();
}

void
SoVRMLParent::commonConstructor(void)
{
  PRIVATE(this) = new SoVRMLParentP;
  PRIVATE(this)->childlistvalid = FALSE;

  SO_NODE_INTERNAL_CONSTRUCTOR(SoVRMLParent);

  SO_VRMLNODE_ADD_EMPTY_EXPOSED_MFIELD(children);
  SO_VRMLNODE_ADD_EVENT_IN(addChildren);
  SO_VRMLNODE_ADD_EVENT_IN(removeChildren);

  PRIVATE(this)->addsensor = new SoFieldSensor(field_sensor_cb, this);
  PRIVATE(this)->removesensor = new SoFieldSensor(field_sensor_cb, this);
  PRIVATE(this)->addsensor->attach(&this->addChildren);
  PRIVATE(this)->removesensor->attach(&this->removeChildren);
}

/*!
  Destructor.
*/
SoVRMLParent::~SoVRMLParent()
{
  PRIVATE(this)->addsensor->detach();
  PRIVATE(this)->removesensor->detach();
  delete PRIVATE(this)->addsensor;
  delete PRIVATE(this)->removesensor;
  delete PRIVATE(this);
}

// Doc in parent
SbBool
SoVRMLParent::affectsState(void) const
{
  return FALSE;
}

// Doc in parent
void
SoVRMLParent::addChild(SoNode * child)
{
  this->children.addNode(child);
  PRIVATE(this)->childlistvalid = FALSE;
}

// Doc in parent
void
SoVRMLParent::insertChild(SoNode * child, int idx)
{
  this->children.insertNode(child, idx);
  PRIVATE(this)->childlistvalid = FALSE;
}

// Doc in parent
SoNode *
SoVRMLParent::getChild(int idx) const
{
  return this->children.getNode(idx);
}

// Doc in parent
int
SoVRMLParent::findChild(const SoNode * child) const
{
  return this->children.findNode(child);
}

// Doc in parent
int
SoVRMLParent::getNumChildren(void) const
{
  return this->children.getNumNodes();
}

// Doc in parent
void
SoVRMLParent::removeChild(int idx)
{
  this->children.removeNode(idx);
  PRIVATE(this)->childlistvalid = FALSE;
}

// Doc in parent
void
SoVRMLParent::removeChild(SoNode * child)
{
  this->children.removeNode(child);
  PRIVATE(this)->childlistvalid = FALSE;
}

// Doc in parent
void
SoVRMLParent::removeAllChildren(void)
{
  this->children.removeAllNodes();
  SoGroup::children->truncate(0);
  PRIVATE(this)->childlistvalid = TRUE;
}

// Doc in parent
void
SoVRMLParent::replaceChild(int idx, SoNode * child)
{
  this->children.replaceNode(idx, child);
  PRIVATE(this)->childlistvalid = FALSE;
}

// Doc in parent
void
SoVRMLParent::replaceChild(SoNode * old,
                           SoNode * child)
{
  this->children.replaceNode(old, child);
  PRIVATE(this)->childlistvalid = FALSE;
}

// Doc in parent
SoChildList *
SoVRMLParent::getChildren(void) const
{
  if (!PRIVATE(this)->childlistvalid) {
    // this is not 100% thread safe. The assumption is that no nodes
    // will be added or removed while a scene graph is being
    // traversed. For Coin, this is an ok assumption.
    PRIVATE(this)->lockChildList();
    // test again after we've locked
    if (!PRIVATE(this)->childlistvalid) {
      
      SoVRMLParent::updateChildList(this->children.getValues(0),
                                    this->children.getNum(),
                                    *SoGroup::children);
      PRIVATE((SoVRMLParent*)this)->childlistvalid = TRUE;
    }
    PRIVATE(this)->unlockChildList();    
  }
  return SoGroup::children;
}

// Doc in parent
void
SoVRMLParent::doAction(SoAction * action)
{
  inherited::doAction(action);
}

// Doc in parent
void
SoVRMLParent::search(SoSearchAction * action)
{
  inherited::search(action);
}

void
SoVRMLParent::write(SoWriteAction * action)
{
  SoOutput * out = action->getOutput();

  if (out->getStage() == SoOutput::COUNT_REFS) {
    this->addWriteReference(out, FALSE);
  }
  else if (out->getStage() == SoOutput::WRITE) {
    if (this->writeHeader(out, FALSE, FALSE)) return;
    // don't use the standard SoFieldData, since we want the children
    // field to be written last.
    SoFieldData * fd = this->makeWriteData();
    fd->write(out, this);
    delete fd;
    this->writeFooter(out);
  }
  else assert(0 && "unknown stage");
}


// Doc in parent
void
SoVRMLParent::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  inherited::getPrimitiveCount(action);
}

// Doc in parent
void
SoVRMLParent::notify(SoNotList * list)
{
  SoField * f = list->getLastField();
  if (f == &this->children) {
    PRIVATE(this)->childlistvalid = FALSE;
  }
  inherited::notify(list);
}

// Doc in parent
SbBool
SoVRMLParent::readInstance(SoInput * in,
                           unsigned short flags)
{
  SbBool oldnot = this->children.enableNotify(FALSE);
  SbBool ret = inherited::readInstance(in, flags);
  if (oldnot) this->children.enableNotify(TRUE);
  PRIVATE(this)->childlistvalid = FALSE;
  return ret;
}

// Doc in parent
void
SoVRMLParent::copyContents(const SoFieldContainer * from,
                           SbBool copyConn)
{
  SoGroup::children->truncate(0);
  SoNode::copyContents(from, copyConn);
  PRIVATE(this)->childlistvalid = FALSE;
}

/*!

  A convenience method that can be used to update \a cl to match the
  list of nodes in \a nodes.

*/
void
SoVRMLParent::updateChildList(const SoNode * const * nodes,
                              const int numnodes,
                              SoChildList & cl)
{
  int i;
  SbBool needcopy = TRUE;
  if (numnodes == cl.getLength()) {
    const SoNode ** clarr = (const SoNode**) cl.getArrayPtr();
    for (i = 0; i < numnodes; i++) {
      if (clarr[i] != nodes[i]) break;
    }
    if (i == numnodes) needcopy = FALSE;
  }
  if (needcopy) {
    cl.truncate(0);
    for (i = 0; i < numnodes; i++) {
      if (nodes[i]) {
        cl.append((SoNode*) nodes[i]);
      }
    }
  }
}

/*!

  A convenience method that is used to sync the nodes in \a cl with
  all nodes in SoSFNode fields in \a nodewithsfnode.

*/
void
SoVRMLParent::updateChildList(SoNode * nodewithsfnode,
                              SoChildList & cl)
{
  SbList <SoNode*> nodelist;

  const SoFieldData * fd = nodewithsfnode->getFieldData();
  const int n = fd->getNumFields();

  SoType sosftype = SoSFNode::getClassTypeId();
  for (int i = 0; i < n; i++) {
    SoField * f = fd->getField(nodewithsfnode, i);
    if (f->getTypeId() == sosftype) {
      SoNode * node = ((SoSFNode*) f)->getValue();
      if (node) nodelist.append(node);
    }
  }
  SoVRMLParent::updateChildList(nodelist.getArrayPtr(),
                                nodelist.getLength(),
                                cl);
}

// add children in addChildren field
void
SoVRMLParent::processAddChildren(void)
{
  const int n = this->addChildren.getNum();
  const SoNode ** nodes = this->addChildren.getValues(0);
  for (int i = 0; i < n; i++) {
    const SoNode * node = nodes[i];
    if (this->findChild(node) < 0) {
      this->addChild((SoNode*)node);
    }
  }
}

// remove children in removeChildren field
void
SoVRMLParent::processRemoveChildren(void)
{
  const int n = this->removeChildren.getNum();
  const SoNode ** nodes = this->removeChildren.getValues(0);
  for (int i = 0; i < n; i++) {
    int idx = this->findChild(nodes[i]);
    if (idx >= 0) {
      this->removeChild(idx);
    }
  }
}

// callback from the addChildren/removeChildren sensors
void
SoVRMLParent::field_sensor_cb(void * data, SoSensor * sensor)
{
  SoVRMLParent * thisp = (SoVRMLParent*) data;
  if (sensor == PRIVATE(thisp)->addsensor) {
    thisp->processAddChildren();
  }
  else {
    assert(sensor == PRIVATE(thisp)->removesensor);
    thisp->processRemoveChildren();
  }
}

// Reorganize field order so that the "children" field is written
// last.
SoFieldData *
SoVRMLParent::makeWriteData(void)
{
  SoFieldData * newfd = new SoFieldData;
  const SoFieldData * fd = this->getFieldData();

  SoField * delay = &this->children;
  const int n = fd->getNumFields();

  for (int i = 0; i < n; i++) {
    SoField * f = fd->getField(this, i);
    if (f != delay) {
      newfd->addField(this, fd->getFieldName(i).getString(), f);
    }
  }
  newfd->addField(this, "children", delay);
  return newfd;
}

#undef PRIVATE
