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
  \class SoVRMLAppearance SoVRMLAppearance.h Inventor/VRMLnodes/SoVRMLAppearance.h
  \brief The SoVRMLAppearance class specifies visual properties for shapes.
  \ingroup VRMLnodes
  
  \WEB3DCOPYRIGHT

  \verbatim

  Appearance {
    exposedField SFNode material          NULL
    exposedField SFNode texture           NULL
    exposedField SFNode textureTransform  NULL
  }
  \endverbatim

  The Appearance node specifies the visual properties of geometry. The
  value for each of the fields in this node may be NULL. However, if
  the field is non-NULL, it shall contain one node of the appropriate
  type.  The material field, if specified, shall contain a VRMLMaterial
  node. If the material field is NULL or unspecified, lighting is off
  (all lights are ignored during rendering of the object that
  references this Appearance) and the unlit object colour is (1, 1,
  1). Details of the VRML lighting model are in 4.14, Lighting model
  (<http://www.web3d.org/technicalinfo/specifications/vrml97/part1/concepts.html#4.14>).

  The texture field, if specified, shall contain one of the various
  types of texture nodes (VRMLImageTexture, VRMLMovieTexture, or
  VRMLPixelTexture).  If the texture node is NULL or the texture field
  is unspecified, the object that references this Appearance is not
  textured.  The textureTransform field, if specified, shall contain a
  VRMLTextureTransform node. If the textureTransform is NULL or
  unspecified, the textureTransform field has no effect.

*/

/*!
  \var SoSFNode SoVRMLAppearance::material
  Can contain an SoVRMLMaterial node. Is NULL by default.
*/

/*!
  \var SoSFNode SoVRMLAppearance::texture
  Can contain a texture node. Is NULL by default.
*/

/*!
  \var SoSFNode SoVRMLAppearance::textureTransform
  Can contain an SoVRMLTextureTransform node. Is NULL by default.
*/

#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/VRMLnodes/SoVRMLAppearance.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/VRMLnodes/SoVRMLParent.h>
#include <Inventor/misc/SoChildList.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/actions/SoActions.h>
#include <Inventor/elements/SoCacheElement.h>
#include <Inventor/elements/SoLazyElement.h>
#include <Inventor/elements/SoTextureQualityElement.h>
#include <stddef.h>

SO_NODE_SOURCE(SoVRMLAppearance);

// doc in parent
void
SoVRMLAppearance::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoVRMLAppearance, SO_VRML97_NODE_TYPE);
}

/*!
  Constructor.
*/
SoVRMLAppearance::SoVRMLAppearance(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoVRMLAppearance);

  SO_VRMLNODE_ADD_EXPOSED_FIELD(material, (NULL));
  SO_VRMLNODE_ADD_EXPOSED_FIELD(texture, (NULL));
  SO_VRMLNODE_ADD_EXPOSED_FIELD(textureTransform, (NULL));
  this->childlist = new SoChildList(this);
  this->childlistvalid = FALSE;
}

/*!
  Destructor.
*/
SoVRMLAppearance::~SoVRMLAppearance()
{
  delete this->childlist;
}

// doc in parent
void
SoVRMLAppearance::doAction(SoAction * action)
{
  int numindices;
  const int * indices;
  if (action->getPathCode(numindices, indices) == SoAction::IN_PATH) {
    this->getChildren()->traverseInPath(action, numindices, indices);
  }
  else {
    this->getChildren()->traverse(action); // traverse all children
  }
}

// doc in parent
void
SoVRMLAppearance::callback(SoCallbackAction * action)
{
  SoVRMLAppearance::doAction(action);
}

// doc in parent
void
SoVRMLAppearance::GLRender(SoGLRenderAction * action)
{
  SoState * state = action->getState();
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

  if (this->texture.getValue() && SoTextureQualityElement::get(state) > 0.0f) {
    static uint32_t white = 0xffffffff;
    SoLazyElement::setPacked(state, this, 1, &white);
  }
}

// doc in parent
void
SoVRMLAppearance::search(SoSearchAction * action)
{
  SoNode::search(action);
  if (action->isFound()) return;
  SoVRMLAppearance::doAction(action);
}

// doc in parent
SoChildList *
SoVRMLAppearance::getChildren(void) const
{
  if (!this->childlistvalid) {
    SoVRMLAppearance * thisp = (SoVRMLAppearance*) this;
    SoVRMLParent::updateChildList(thisp, *thisp->childlist);
    thisp->childlistvalid = TRUE;
  }
  return this->childlist;
}

// doc in parent
void
SoVRMLAppearance::notify(SoNotList * list)
{
  SoField * f = list->getLastField();
  if (f && f->getTypeId() == SoSFNode::getClassTypeId()) {
    this->childlistvalid = FALSE;
  }
  inherited::notify(list);
}

// doc in parent
void
SoVRMLAppearance::copyContents(const SoFieldContainer * from,
                               SbBool copyConn)
{
  inherited::copyContents(from, copyConn);
  this->childlistvalid = FALSE;
  this->childlist->truncate(0);
}
