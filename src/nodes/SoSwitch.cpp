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
  \class SoSwitch SoSwitch.h Inventor/nodes/SoSwitch.h
  \brief The SoSwitch class is a group node which selects one child subgraph for traversal.
  \ingroup nodes

  Which child to traverse is controlled by the application programmer
  by using the SoSwitch::whichChild field. In addition to picking out
  a single child for traversal, it is also possible to flip all
  children on or off for traversal.

  This node is very useful for conditionally turning on or off parts
  of the scenegraph based on the current application processing mode,
  visualizing mode, or whatever else the application can do.
*/


#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/misc/SoChildList.h>
#include <coindefs.h> // COIN_OBSOLETED()

#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/actions/SoHandleEventAction.h>
#include <Inventor/actions/SoAudioRenderAction.h>
#include <Inventor/elements/SoSoundElement.h>
#include <Inventor/elements/SoSwitchElement.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/SoOutput.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

/*!
  \var SoSFInt32 SoSwitch::whichChild

  Selects which child to traverse during rendering (and some other)
  actions.

  The value should be either \c SO_SWITCH_NONE (for traversing no
  children, like it was an empty SoGroup node), \c SO_SWITCH_ALL (for
  traversing like if we were an SoGroup node), \c SO_SWITCH_INHERIT
  (for traversing the same child as the last SoSwitch node to parent
  this in the graph), or an index value for a child.

  When constructing ascii Inventor files, the integer values for the
  keywords must be used instead of their names.  They are -1 for
  \c SO_SWITCH_NONE, -2 for \c SO_SWITCH_INHERIT, and -3 for
  \c SO_SWITCH_ALL.

  Default value for the field is \c SO_SWITCH_NONE.
*/

// *************************************************************************

class SoSwitchP
{
public:
  SoSwitchP(SoSwitch * master) : master(master) {};
  SoSwitch *master;

  enum { YES, NO, MAYBE } hassoundchild;
  SbBool soundchildplaying;
  SbBool shoulddosoundtraversal;
};

#undef PRIVATE
#define PRIVATE(p) ((p)->pimpl)
#undef PUBLIC
#define PUBLIC(p) ((p)->master)

SO_NODE_SOURCE(SoSwitch);

/*!
  Default constructor.
*/
SoSwitch::SoSwitch(void)
{
  this->commonConstructor();
}

/*!
  Constructor.

  The argument should be the approximate number of children which is
  expected to be inserted below this node. The number need not be
  exact, as it is only used as a hint for better memory resource
  allocation.
*/
SoSwitch::SoSwitch(int numchildren)
  : inherited(numchildren)
{
  this->commonConstructor();
}

void
SoSwitch::commonConstructor(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoSwitch);

  SO_NODE_ADD_FIELD(whichChild, (SO_SWITCH_NONE));

  PRIVATE(this) = new SoSwitchP(this);
  PRIVATE(this)->hassoundchild = SoSwitchP::MAYBE;
  PRIVATE(this)->soundchildplaying = FALSE;
  PRIVATE(this)->shoulddosoundtraversal = TRUE;
}

/*!
  Destructor.
*/
SoSwitch::~SoSwitch()
{
  delete PRIVATE(this);
}

// doc in super
void
SoSwitch::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoSwitch, SO_FROM_INVENTOR_1|SoNode::VRML1);

  SO_ENABLE(SoGetBoundingBoxAction, SoSwitchElement);
  SO_ENABLE(SoSearchAction, SoSwitchElement);
  SO_ENABLE(SoGetMatrixAction, SoSwitchElement);
  SO_ENABLE(SoGLRenderAction, SoSwitchElement);
  SO_ENABLE(SoPickAction, SoSwitchElement);

  SO_ENABLE(SoCallbackAction, SoSwitchElement);
  SO_ENABLE(SoGetPrimitiveCountAction, SoSwitchElement);
  SO_ENABLE(SoHandleEventAction, SoSwitchElement);
}

// Documented in superclass.
void
SoSwitch::GLRender(SoGLRenderAction * action)
{
  this->doAction(action);
}

// Documented in superclass.
void
SoSwitch::getBoundingBox(SoGetBoundingBoxAction * action)
{
  SoSwitch::doAction(action);
}

// Documented in superclass.
void
SoSwitch::search(SoSearchAction * action)
{
  // This method must be overridden in SoSwitch nodes to take into
  // account if the search involves every single node, or just the
  // nodes involved in normal graph traversal.

  // Include this node in the search.
  SoNode::search(action);
  if (action->isFound()) return;

  if (action->isSearchingAll()) {
    this->children->traverse(action);
  }
  else {
    SoSwitch::doAction(action);
  }
}

// Documented in superclass.
void
SoSwitch::doAction(SoAction * action)
{
  SoState * state = action->getState();
  int idx = this->whichChild.isIgnored() ?
    SO_SWITCH_NONE : this->whichChild.getValue();
  if (idx == SO_SWITCH_INHERIT) {
    idx = SoSwitchElement::get(action->getState());
    // when we inherit, idx might be out of range. Use modulo.
    if (idx >= this->getNumChildren()) idx %= this->getNumChildren();
  }
  else {
    SoSwitchElement::set(state, idx);
  }

  int numindices;
  const int * indices;
  SoAction::PathCode pathcode = action->getPathCode(numindices, indices);

  if (idx == SO_SWITCH_ALL) {
    if (action->isOfType(SoGetBoundingBoxAction::getClassTypeId())) {
      // calculate center of bbox if bboxaction. This makes the
      // switch node behave exactly like a group node
      SoGetBoundingBoxAction * bbaction = (SoGetBoundingBoxAction*) action;
      // Initialize accumulation variables.
      SbVec3f acccenter(0.0f, 0.0f, 0.0f);
      int numcenters = 0;
      // only traverse nodes in path(s) for IN_PATH traversal
      int n = pathcode == SoAction::IN_PATH ? numindices : this->getNumChildren();

      for (int i = 0; i < n; i++) {
        this->children->traverse(bbaction,
                                 pathcode == SoAction::IN_PATH ? indices[i] : i);

        // If center point is set, accumulate.
        if (bbaction->isCenterSet()) {
          acccenter += bbaction->getCenter();
          numcenters++;
          bbaction->resetCenter();
        }
      }
      if (numcenters != 0) {
        bbaction->setCenter(acccenter / float(numcenters), FALSE);
      }
    }
    else { // not a getBoundingBoxAction
      if (pathcode == SoAction::IN_PATH) {
        this->children->traverseInPath(action, numindices, indices);
      }
      else {
        this->children->traverse(action);
      }
    }
  } else {
    SbBool dosoundtraversal = action->isOfType(SoAudioRenderAction::getClassTypeId()) &&
      (PRIVATE(this)->hassoundchild != SoSwitchP::NO);
      // do more testing before enabeling this && PRIVATE(this)->soundchildplaying;
    if (idx >= 0) { // should only traverse one child
      if (pathcode == SoAction::IN_PATH) {
        // traverse only if one path matches idx
        dosoundtraversal = FALSE;
        for (int i = 0; i < numindices; i++) {
          if (indices[i] == idx) {
            this->children->traverse(action, idx);
            break;
          }
        }
      }
      else { // off, below or no path traversal
        // be robust for index out of range
        if (idx >= this->getNumChildren()) {
#if COIN_DEBUG
          static SbBool first = TRUE;
          if (first) {
            first = FALSE;
            SbString s("(warning will be printed once, but there might be more cases of this problem).");
            int lastidx = this->getNumChildren()-1;
            if (lastidx >= 0) {
              SoDebugError::post("SoSwitch::doAction",
                                 "whichChild %d out of range [0, %d] %s",
                                 idx, lastidx, s.getString());
            }
            else {
              SoDebugError::post("SoSwitch::doAction",
                                 "whichChild %d out of range -- "
                                 "switch node has no children! %s",
                                 idx, s.getString());
            }
          }
#endif // COIN_DEBUG
        }
        else {
          this->children->traverse(action, idx);
        }
      }
    }
    if (dosoundtraversal) {
      // Note: If there is a playing SoVRMLSound node somewhere among
      // the non-active children sub-graphs, it must be informed that
      // it shouldn't be playing anymore. So we traverse all inactive
      // children. This could be optimized, as we only need to visit
      // the old active (now inactive) child. 2003-02-04 thammer.

      if (PRIVATE(this)->shoulddosoundtraversal) {
        SoSoundElement::setIsPartOfActiveSceneGraph(state, this, FALSE);
        int n = this->getNumChildren();
        for (int i=0; i<n; i++) {
          if (i != idx) {
            action->getState()->push();
            SoSoundElement::setIsPartOfActiveSceneGraph(state, this, FALSE);
            this->children->traverse(action, i);
            action->getState()->pop();
          }
        }
        PRIVATE(this)->shoulddosoundtraversal = FALSE;
      }
    }
  }
}

// Documented in superclass.
SbBool
SoSwitch::affectsState(void) const
{
  // Overridden because when this function is called we don't know
  // which "mode" the traversing action is in. If it's an
  // SoSearchAction with isSearchingAll() set to TRUE, we should
  // behave as if whichChild == SO_SWITCH_ALL, for instance.
  //
  // (To handle this exact case, SGI and TGS Inventor seems to use a
  // global static flag SoSearchAction::duringSearchAll. We find this
  // to be an utterly crap idea, though.)
  //
  // So to be safe, we _always_ behave as if whichChild is set to
  // traverse all children. The worst that can happen is that we get a
  // "false positive", ie TRUE when it should be FALSE. That means the
  // action needs to traverse one level further down onto one of our
  // children -- which will just take a miniscule amount of additional
  // processing time.

  int n = this->getNumChildren();
  for (int i=0; i < n; i++) {
    if (this->getChild(i)->affectsState()) { return TRUE; }
  }
  return FALSE;
}

// Documented in superclass.
void
SoSwitch::callback(SoCallbackAction *action)
{
  SoSwitch::doAction(action);
}

// Documented in superclass.
void
SoSwitch::audioRender(SoAudioRenderAction * action)
{
  SbBool lookforsoundnode = FALSE;
  SbBool oldhassound = FALSE, oldisplaying = FALSE; // initialize to avoid compiler warnings
  int numindices;
  const int * indices;
  SoState * state = action->getState();
  if ((action->getPathCode(numindices, indices) != SoAction::IN_PATH) &&
      (PRIVATE(this)->hassoundchild != SoSwitchP::NO) )
    lookforsoundnode = TRUE;
  if (lookforsoundnode) {
    oldhassound = SoSoundElement::setSceneGraphHasSoundNode(state, this, FALSE);
    oldisplaying = SoSoundElement::setSoundNodeIsPlaying(state, this, FALSE);
  }
  
  SoSwitch::doAction((SoAction*)action);

  if (lookforsoundnode) {
    SbBool soundnodefound = SoSoundElement::sceneGraphHasSoundNode(state);
    PRIVATE(this)->soundchildplaying = SoSoundElement::soundNodeIsPlaying(state);
    SoSoundElement::setSceneGraphHasSoundNode(state, this, oldhassound || soundnodefound);
    SoSoundElement::setSoundNodeIsPlaying(state, this,
                                          oldisplaying || PRIVATE(this)->soundchildplaying);
    PRIVATE(this)->hassoundchild = soundnodefound ? SoSwitchP::YES :
      SoSwitchP::NO;
  }
}

// Documented in superclass.
void
SoSwitch::pick(SoPickAction *action)
{
  SoSwitch::doAction((SoAction*)action);
}

// Documented in superclass.
void
SoSwitch::handleEvent(SoHandleEventAction *action)
{
  SoSwitch::doAction(action);
}

// Documented in superclass.
void
SoSwitch::getMatrix(SoGetMatrixAction *action)
{
  switch (action->getCurPathCode()) {
  case SoAction::OFF_PATH:
  case SoAction::IN_PATH:
    SoSwitch::doAction((SoAction*)action);
    break;
  default:
    break;
  }
}

// Documented in superclass.
void
SoSwitch::write(SoWriteAction * action)
{
  // to keep child numbering, always write out all children for a
  // switch

  SoOutput * out = action->getOutput();
  if (out->getStage() == SoOutput::COUNT_REFS) {
    this->addWriteReference(out, FALSE);
    // Only increase number of writereferences to the top level node
    // in a tree which is used multiple times.
    if (!this->hasMultipleWriteRefs()) this->getChildren()->traverse(action);
  }
  else if (out->getStage() == SoOutput::WRITE) {
    if (this->writeHeader(out, TRUE, FALSE)) return;
    this->getFieldData()->write(out, this);
    if (out->isBinary()) out->write(this->getNumChildren());
    this->getChildren()->traverse(action);
    this->writeFooter(out);
  }
  else assert(0 && "unknown stage");
}

// Documented in superclass.
void
SoSwitch::getPrimitiveCount(SoGetPrimitiveCountAction *action)
{
  SoSwitch::doAction((SoAction*)action);
}

/*!
  This function was part of the original SGI Inventor API, but it is
  not supported in Coin, as it looks like it should probably have been
  private in Inventor.
*/
void
SoSwitch::traverseChildren(SoAction * action)
{
  COIN_OBSOLETED();
}

// Doc from superclass.
void
SoSwitch::notify(SoNotList * nl)
{
  inherited::notify(nl);

  PRIVATE(this)->hassoundchild = SoSwitchP::MAYBE;
  PRIVATE(this)->soundchildplaying = FALSE;
  PRIVATE(this)->shoulddosoundtraversal = TRUE;
}
