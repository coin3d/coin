/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#ifdef HAVE_VRML97

/*!
  \class SoVRMLNormal SoVRMLNormal.h Inventor/VRMLnodes/SoVRMLNormal.h
  \brief The SoVRMLNormal class is used to bind normals to geometry.
  \ingroup VRMLnodes

  \WEB3DCOPYRIGHT

  \verbatim
  Normal {
    exposedField MFVec3f vector  []   # (-,)
  }
  \endverbatim

  This node defines a set of 3D surface normal vectors to be used in
  the vector field of some geometry nodes (e.g., SoVRMLIndexedFaceSet
  and SoVRMLElevationGrid). This node contains one multiple-valued
  field that contains the normal vectors. Normals shall be of unit
  length.
*/

/*!
  \var SoMFVec3f SoVRMLNormal::vector
  The normal vectors. Empty by default.
*/

#include <Inventor/VRMLnodes/SoVRMLNormal.h>

#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoNormalElement.h>
#include <Inventor/elements/SoGLVBOElement.h>

#include "nodes/SoSubNodeP.h"
#include "rendering/SoVBO.h"

SO_NODE_SOURCE(SoVRMLNormal);

class SoVRMLNormalP {
 public:
  SoVRMLNormalP() : vbo(NULL) { }
  ~SoVRMLNormalP() { delete this->vbo; }
  
  SoVBO * vbo;
};

#define PRIVATE(obj) obj->pimpl

// Doc in parent
void
SoVRMLNormal::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoVRMLNormal, SO_VRML97_NODE_TYPE);
}

/*!
  Constructor.
*/
SoVRMLNormal::SoVRMLNormal(void)
{
  PRIVATE(this) = new SoVRMLNormalP;
  SO_VRMLNODE_INTERNAL_CONSTRUCTOR(SoVRMLNormal);

  SO_VRMLNODE_ADD_EMPTY_EXPOSED_MFIELD(vector);
}

/*!
  Destructor.
*/
SoVRMLNormal::~SoVRMLNormal()
{
  delete PRIVATE(this);
}

// Doc in parent
void
SoVRMLNormal::doAction(SoAction * action)
{
  SoNormalElement::set(action->getState(), this,
                       this->vector.getNum(), this->vector.getValues(0));
}

// Doc in parent
void
SoVRMLNormal::GLRender(SoGLRenderAction * action)
{
  //
  // FIXME: code to test if all normals are unit length, and store
  // this in some cached variable.  should be passed on to
  // SoGLNormalizeElement to optimize rendering (pederb)
  //
  SoVRMLNormal::doAction((SoAction*) action);
  SoState * state = action->getState();
  
  SoBase::staticDataLock();
  SbBool setvbo = FALSE;
  const int num = this->vector.getNum();
  if (SoGLVBOElement::shouldCreateVBO(state, num)) {
    setvbo = TRUE;
    SbBool dirty = FALSE;
    if (PRIVATE(this)->vbo == NULL) {
      PRIVATE(this)->vbo = new SoVBO(GL_ARRAY_BUFFER, GL_STATIC_DRAW); 
      dirty =  TRUE;
    }
    else if (PRIVATE(this)->vbo->getBufferDataId() != this->getNodeId()) {
      dirty = TRUE;
    }
    if (dirty) {
      PRIVATE(this)->vbo->setBufferData(this->vector.getValues(0),
                                        num*sizeof(SbVec3f),
                                        this->getNodeId());
    }
  }
  else if (PRIVATE(this)->vbo && PRIVATE(this)->vbo->getBufferDataId()) {
    // clear buffers to deallocate VBO memory
    PRIVATE(this)->vbo->setBufferData(NULL, 0, 0);
  }
  SoBase::staticDataUnlock();
  if (setvbo) {
    SoGLVBOElement::setNormalVBO(state, PRIVATE(this)->vbo);
  }

}

// Doc in parent
void
SoVRMLNormal::callback(SoCallbackAction * action)
{
  SoVRMLNormal::doAction((SoAction*) action);
}

// Doc in parent
void
SoVRMLNormal::pick(SoPickAction * action)
{
  SoVRMLNormal::doAction((SoAction*) action);
}

// Doc in parent
void
SoVRMLNormal::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  SoVRMLNormal::doAction((SoAction*) action);
}

#undef PRIVATE
#endif // HAVE_VRML97
