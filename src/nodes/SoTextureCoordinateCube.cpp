/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2004 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#include <Inventor/nodes/SoTextureCoordinateCube.h>
#include <Inventor/nodes/SoSubNodeP.h>

#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLTextureCoordinateElement.h>
#include <Inventor/elements/SoGLMultiTextureCoordinateElement.h>
#include <Inventor/elements/SoTextureUnitElement.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/nodes/SoShape.h>
#include <Inventor/SbBox3f.h>
#include <Inventor/SoFullPath.h>

/*!
  \class SoTextureCoordinateCube include/Inventor/nodes/SoTextureCoordinateCube.h
  \brief The SoTextureCoordinateCube class autogenerates cubemapped texture coordinated for shapes.
  \ingroup bundles

  The cube used for reference when mapping is the boundingbox for the shape.
*/

// FIXME: Add a better class description (20040123 handegar)

SO_NODE_SOURCE(SoTextureCoordinateCube);

class SoTextureCoordinateCubeP {

public:
  SoTextureCoordinateCubeP(SoTextureCoordinateCube * texturenode) 
    : master(texturenode) { }

  SbVec3f origo;
  SbBox3f boundingbox;
  SoNode * currentshape;
  SoState * currentstate;
  
  SbVec4f calculateTextureCoordinate(SbVec3f point, SbVec3f n);
  
private:
  SoTextureCoordinateCube * master;
};


static const SbVec4f & textureCoordinateCubeCallback(void * userdata, const SbVec3f & point, const SbVec3f & normal);

#undef PRIVATE
#undef PUBLIC
#define PRIVATE(p) (p->pimpl)
#define PUBLIC(p) (p->master)


/*!
  Constructor.
*/
SoTextureCoordinateCube::SoTextureCoordinateCube(void)
{

  PRIVATE(this) = new SoTextureCoordinateCubeP(this);

  SO_NODE_INTERNAL_CONSTRUCTOR(SoTextureCoordinateCube);

  PRIVATE(this)->currentshape = NULL;
  PRIVATE(this)->currentstate = NULL;
  PRIVATE(this)->origo = SbVec3f(0,0,0);

}

/*!
  Destructor.
*/
SoTextureCoordinateCube::~SoTextureCoordinateCube()
{
}

// Documented in superclass.
void
SoTextureCoordinateCube::initClass(void)
{
  // FIXME: What should this one say?? (20040122 handegar)
  SO_NODE_INTERNAL_INIT_CLASS(SoTextureCoordinateCube, SO_FROM_INVENTOR_1|SoNode::VRML1);
  
  SO_ENABLE(SoGLRenderAction, SoGLTextureCoordinateElement);
  SO_ENABLE(SoCallbackAction, SoTextureCoordinateElement);
  SO_ENABLE(SoPickAction, SoTextureCoordinateElement);

}

const SbVec4f &
textureCoordinateCubeCallback(void * userdata, 
                          const SbVec3f & point, 
                          const SbVec3f & normal)
{
  
  SoTextureCoordinateCubeP * pimpl = (SoTextureCoordinateCubeP *) userdata;
  
  SoState * state = pimpl->currentstate;
  SoFullPath * path = (SoFullPath *) state->getAction()->getCurPath();
  SoNode * node = path->getTail();


  if (!node->isOfType(SoShape::getClassTypeId())) {
    // FIXME: A better way to handle this? (20040122 handegar)
    assert(FALSE && "TextureCoordinateCube callback called for a non-SoShape node.");
  }

  // Cast the node into a shape
  SoShape * shape = (SoShape *) node;

  if (shape != pimpl->currentshape) {       
    pimpl->boundingbox.makeEmpty();
    shape->computeBBox(state->getAction(), pimpl->boundingbox, pimpl->origo);
    SbVec3f c = pimpl->origo = pimpl->boundingbox.getCenter();
    pimpl->currentshape = shape;

    float sx, sy, sz;
    pimpl->boundingbox.getSize(sx, sy, sz);
    if (sy > sx) sx = sy;
    if (sz > sx) sx = sz;
    sx *= 0.5f;
    
    pimpl->boundingbox.setBounds(c[0] - sx, c[1] - sx, c[2] - sx,
                                 c[0] + sx, c[1] + sx, c[2] + sx);
  }

  const SbVec4f & ret = pimpl->calculateTextureCoordinate(point, normal);
  return ret;
  
}

SbVec4f
SoTextureCoordinateCubeP::calculateTextureCoordinate(SbVec3f point, SbVec3f n)
{

  double maxv = fabs(n[0]);
  int maxi = 0;
      
  if (fabs(n[1]) > maxv) { maxi = 1; maxv = fabs(n[1]); }
  if (fabs(n[2]) > maxv) { maxi = 2; }  
  
  int i0 = (maxi + 1) % 3;
  int i1 = (maxi + 2) % 3;

  const SbVec3f bmax = this->boundingbox.getMax();
  const SbVec3f bmin = this->boundingbox.getMin();
  float d0 = bmax[i0] - bmin[i0];
  float d1 = bmax[i1] - bmin[i1];

  if (d0 == 0.0f) d0 = 1.0f;
  if (d1 == 0.0f) d1 = 1.0f;

  float s = (point[i0] - bmin[i0]) / d0; 
  float t = (point[i1] - bmin[i1]) / d1;

  SbVec4f tc(s, t, 0.0f, 1.0f);
  switch (maxi) {
  case 0:
    tc[0] = 1.0f - t;
    tc[1] = s; 
    break;
  case 1:
    tc[0] = t;
    tc[1] = 1.0f - s; 
    break;
  }
  if (n[maxi] < 0.0f) {
    if (maxi == 1) {
      tc[1] = 1.0f - tc[1];
    }
    else {
      tc[0] = 1.0f - tc[0];
    }
  }

  return tc;
}


// Documented in superclass.
void
SoTextureCoordinateCube::doAction(SoAction * action)
{  
}

// Documented in superclass.
void
SoTextureCoordinateCube::GLRender(SoGLRenderAction * action)
{
  
  if (PRIVATE(this)->currentstate != action->getState()) 
    PRIVATE(this)->currentstate = action->getState();    

  PRIVATE(this)->currentshape = NULL;  

  SoTextureCoordinateElement::setFunction(PRIVATE(this)->currentstate, 
                                          this, textureCoordinateCubeCallback,
                                          PRIVATE(this));
}

// Documented in superclass.
void
SoTextureCoordinateCube::callback(SoCallbackAction * action)
{
  SoTextureCoordinateCube::doAction((SoAction *)action);
}

// Documented in superclass.
void
SoTextureCoordinateCube::pick(SoPickAction * action)
{
  SoTextureCoordinateCube::doAction((SoAction *)action);
}


