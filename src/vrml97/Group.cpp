/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 2001 by Systems in Motion. All rights reserved.
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
  \class SoVRMLGroup SoVRMLGroup.h Inventor/VRMLnodes/SoVRMLGroup.h
  \brief The SoVRMLGroup class is used for grouping VRML nodes.
*/

/*!
  SoSFEnum SoVRMLGroup::renderCaching
  Render caching policy. Default value is AUTO.
*/

/*!
  SoSFEnum SoVRMLGroup::boundingBoxCaching
  Bounding box cache policy. Default value is AUTO.
*/

/*!
  SoSFEnum SoVRMLGroup::renderCulling
  Render culling policy. Default value is AUTO.
*/

/*!
  SoSFEnum SoVRMLGroup::pickCulling
  Pick culling policy. Default value is AUTO.
*/

/*!
  SoSFVec3f SoVRMLGroup::bboxCenter
  Can be used to optimize bounding box calculations.
*/

/*!
  SoSFVec3f SoVRMLGroup::bboxSize
  Can be used to optimize bounding box calculations.
*/

#include <Inventor/VRMLnodes/SoVRMLGroup.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/actions/SoActions.h>
#include <Inventor/elements/SoCacheElement.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/misc/SoChildList.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/caches/SoBoundingBoxCache.h>
#include <Inventor/caches/SoGLCacheList.h>

#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/system/gl.h>

int SoVRMLGroup::numRenderCaches = 2;

#ifndef DOXYGEN_SKIP_THIS

class SoVRMLGroupP {
public:
  SoBoundingBoxCache * bboxcache;
  SoGLCacheList * cachelist;
};

#endif // DOXYGEN_SKIP_THIS 

SO_NODE_SOURCE(SoVRMLGroup);

// Doc in parent
void
SoVRMLGroup::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoVRMLGroup, SO_VRML97_NODE_TYPE);
}

#undef THIS
#define THIS this->pimpl
/*!
  Constructor.
*/
SoVRMLGroup::SoVRMLGroup(void)
{
  this->commonConstructor();
}

/*!
  Constructor. \a numchildren is the expected number of children.
*/
SoVRMLGroup::SoVRMLGroup(int numchildren)
  : SoVRMLParent(numchildren)
{
  this->commonConstructor();
}

void
SoVRMLGroup::commonConstructor(void)
{
  THIS = new SoVRMLGroupP;
  THIS->bboxcache = NULL;
  THIS->cachelist = NULL;

  SO_NODE_INTERNAL_CONSTRUCTOR(SoVRMLGroup);

  SO_VRMLNODE_ADD_FIELD(bboxCenter, (0.0f, 0.0f, 0.0f));
  SO_VRMLNODE_ADD_FIELD(bboxSize, (-1.0f, -1.0f, -1.0f));

  SO_VRMLNODE_ADD_FIELD(renderCaching, (AUTO));
  SO_VRMLNODE_ADD_FIELD(boundingBoxCaching, (AUTO));
  SO_VRMLNODE_ADD_FIELD(renderCulling, (AUTO));
  SO_VRMLNODE_ADD_FIELD(pickCulling, (AUTO));

  SO_NODE_DEFINE_ENUM_VALUE(CacheEnabled, ON);
  SO_NODE_DEFINE_ENUM_VALUE(CacheEnabled, OFF);
  SO_NODE_DEFINE_ENUM_VALUE(CacheEnabled, AUTO);

  SO_NODE_SET_SF_ENUM_TYPE(renderCaching, CacheEnabled);
  SO_NODE_SET_SF_ENUM_TYPE(boundingBoxCaching, CacheEnabled);
  SO_NODE_SET_SF_ENUM_TYPE(renderCulling, CacheEnabled);
  SO_NODE_SET_SF_ENUM_TYPE(pickCulling, CacheEnabled);
}

/*!
  Destructor.
*/
SoVRMLGroup::~SoVRMLGroup()
{
  delete THIS;
}

/*!
  Set the maximum number of render caches per group node.
*/ 
void
SoVRMLGroup::setNumRenderCaches(int num )
{
  SoVRMLGroup::numRenderCaches = num;
}

/*!
  Returns the maximum number of render caches per group node.
*/
int
SoVRMLGroup::getNumRenderCaches(void)
{
  return SoVRMLGroup::numRenderCaches;
}

// Doc in parent
void
SoVRMLGroup::doAction(SoAction * action)
{
  SoState * state = action->getState();
  state->push();
  inherited::doAction(action);
  state->pop();
}

// Doc in parent
void
SoVRMLGroup::callback(SoCallbackAction * action)
{
  SoState * state = action->getState();
  state->push();
  inherited::callback(action);
  state->pop();
}

// Doc in parent
void
SoVRMLGroup::GLRender(SoGLRenderAction * action )
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

// Doc in parent
void
SoVRMLGroup::getBoundingBox(SoGetBoundingBoxAction * action)
{
  SoState * state = action->getState();
  state->push();
  SoGroup::getBoundingBox(action);
  state->pop();
}

// Doc in parent
void
SoVRMLGroup::getMatrix(SoGetMatrixAction * action)
{
  SoGroup::getMatrix(action);
}

// Doc in parent
void
SoVRMLGroup::pick(SoPickAction * action)
{
  SoState * state = action->getState();
  state->push();
  SoGroup::pick(action);
  state->pop();
}

// Doc in parent
void
SoVRMLGroup::search(SoSearchAction * action)
{
  SoNode::search(action);
  if (action->isFound()) return;

  SoState * state = action->getState();
  state->push();
  inherited::search(action);
  state->pop();
}

// Doc in parent
void
SoVRMLGroup::write(SoWriteAction * action)
{
  SoState * state = action->getState();
  state->push();
  // do not call inherited::write() or SoGroup::write()
  SoNode::write(action);
  state->pop();
}

// Doc in parent
void
SoVRMLGroup::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  SoState * state = action->getState();
  state->push();
  inherited::getPrimitiveCount(action);
  state->pop();
}

// Doc in parent
void
SoVRMLGroup::GLRenderBelowPath(SoGLRenderAction * action)
{
  SoState * state = action->getState();
  state->push();

  int n = this->getChildren()->getLength();
  SoNode ** childarray = (SoNode**) this->getChildren()->getArrayPtr();

  action->pushCurPath();
  for (int i = 0; i < n && !action->hasTerminated(); i++) {
    if (action->abortNow()) {
      // only cache if we do a full traversal
      SoCacheElement::invalidate(state);
      break;
    }
    action->popPushCurPath(i, childarray[i]);
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
  }
  action->popCurPath();
  state->pop();
}

// Doc in parent
void
SoVRMLGroup::GLRenderInPath(SoGLRenderAction * action)
{
  int numindices;
  const int * indices;
  if (action->getPathCode(numindices, indices) == SoAction::IN_PATH) {
    SoState * state = action->getState();
    SoNode ** childarray = (SoNode**) this->getChildren()->getArrayPtr();
    state->push();
    int childidx = 0;
    for (int i = 0; i < numindices; i++) {
      SoAction::PathCode pathcode = action->getCurPathCode();
      for (; childidx < indices[i] && !action->hasTerminated(); childidx++) {
        SoNode * offpath = childarray[childidx];
        if (offpath->affectsState()) {
          action->pushCurPath(childidx, offpath);
          if (!action->abortNow()) {
            offpath->GLRenderOffPath(action);
          }
          else {
            SoCacheElement::invalidate(state);
          }
          action->popCurPath(pathcode);
        }
      }
      SoNode * inpath = childarray[childidx];
      action->pushCurPath(childidx, inpath);
      if (!action->abortNow()) {
        inpath->GLRenderInPath(action);
      }
      else {
        SoCacheElement::invalidate(state);
      }
      action->popCurPath(pathcode);
      childidx++;
    }
    state->pop();
  }
  else {
    // we got to the end of the path
    assert(action->getCurPathCode() == SoAction::BELOW_PATH);
    this->GLRenderBelowPath(action);
  }
}

// Doc in parent
void
SoVRMLGroup::GLRenderOffPath(SoGLRenderAction * action)
{
  // do nothing
}

// Doc in parent
void
SoVRMLGroup::notify(SoNotList * list)
{
  inherited::notify(list);
}

/*!
  Returns TRUE if children can be culled.
*/
SbBool
SoVRMLGroup::cullTest(SoState * state)
{
  return FALSE;
}

//
// no-push culltest
//  
SbBool
SoVRMLGroup::cullTestNoPush(SoState * state)
{
  return FALSE;
}

#undef THIS
