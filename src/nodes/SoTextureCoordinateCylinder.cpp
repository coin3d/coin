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

#include <Inventor/nodes/SoTextureCoordinateCylinder.h>
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
  \class SoTextureCoordinateCylinder include/Inventor/nodes/SoTextureCoordinateCylinder.h
  \brief The SoTextureCoordinateCylinder class autogenerates cylinder mapped texture coordinated for shapes.
  \ingroup bundles
*/

// FIXME: Add a better class description (20040123 handegar)

SO_NODE_SOURCE(SoTextureCoordinateCylinder);

class SoTextureCoordinateCylinderP {

public:
  SoTextureCoordinateCylinderP(SoTextureCoordinateCylinder * texturenode) 
    : master(texturenode) { }

  SbVec3f origo;
  SbBox3f boundingbox;
  SoNode * currentshape;
  SoState * currentstate;
  
  SbVec4f calculateTextureCoordinate(SbVec3f point, SbVec3f n);
  
private:
  SoTextureCoordinateCylinder * master;
};


static const SbVec4f & textureCoordinateCylinderCallback(void * userdata, const SbVec3f & point, const SbVec3f & normal);

#undef PRIVATE
#undef PUBLIC
#define PRIVATE(p) (p->pimpl)
#define PUBLIC(p) (p->master)


/*!
  Constructor.
*/
SoTextureCoordinateCylinder::SoTextureCoordinateCylinder(void)
{

  PRIVATE(this) = new SoTextureCoordinateCylinderP(this);

  SO_NODE_INTERNAL_CONSTRUCTOR(SoTextureCoordinateCylinder);

  PRIVATE(this)->currentshape = NULL;
  PRIVATE(this)->currentstate = NULL;
  PRIVATE(this)->origo = SbVec3f(0,0,0);

}

/*!
  Destructor.
*/
SoTextureCoordinateCylinder::~SoTextureCoordinateCylinder()
{
}

// Documented in superclass.
void
SoTextureCoordinateCylinder::initClass(void)
{
  // FIXME: What should this one say?? (20040122 handegar)
  SO_NODE_INTERNAL_INIT_CLASS(SoTextureCoordinateCylinder, SO_FROM_INVENTOR_1|SoNode::VRML1);
  
  SO_ENABLE(SoGLRenderAction, SoGLTextureCoordinateElement);
  SO_ENABLE(SoCallbackAction, SoTextureCoordinateElement);
  SO_ENABLE(SoPickAction, SoTextureCoordinateElement);

}

const SbVec4f &
textureCoordinateCylinderCallback(void * userdata, 
                          const SbVec3f & point, 
                          const SbVec3f & normal)
{
  
  SoTextureCoordinateCylinderP * pimpl = (SoTextureCoordinateCylinderP *) userdata;
  
  SoState * state = pimpl->currentstate;
  SoFullPath * path = (SoFullPath *) state->getAction()->getCurPath();
  SoNode * node = path->getTail();


  if (!node->isOfType(SoShape::getClassTypeId())) {
    // FIXME: A better way to handle this? (20040122 handegar)
    assert(FALSE && "TextureCoordinateCylinder callback called for a non-SoShape node.");
  }

  // Cast the node into a shape
  SoShape * shape = (SoShape *) node;

  if (shape != pimpl->currentshape) {       
    pimpl->boundingbox.makeEmpty();
    shape->computeBBox(state->getAction(), pimpl->boundingbox, pimpl->origo);
    pimpl->origo = pimpl->boundingbox.getCenter();
    pimpl->currentshape = shape;
  }


  const SbVec4f & ret = pimpl->calculateTextureCoordinate(point, normal);
  return ret;
  
}

SbVec4f
SoTextureCoordinateCylinderP::calculateTextureCoordinate(SbVec3f point, SbVec3f n)
{
 
  // FIXME: This way of mapping will always lead to artifacts in the
  // change between 360 and 0 degrees around the Y-axis. This is
  // unavoidable as the callback cannot predict when the last vertex
  // will be received, and therefore be able to patch up the
  // transition. (20040127 handegar)
 
  SbVec4f tc;
 
  const SbVec3f bmax = this->boundingbox.getMax();
  const SbVec3f bmin = this->boundingbox.getMin();

  double maxv = fabs(n[0]);
  int maxi = 0;
      
  if (fabs(n[1]) > maxv) { maxi = 1; maxv = fabs(n[1]); }
  if (fabs(n[2]) > maxv) { maxi = 2; }  

  if (maxi == 1) { // Cylinder top or bottom?

    // FIXME: A better solution might be to calculate the angle before
    // deciding whether this is the cylinder top/bottom or
    // not. (20040127 handegar) 
    float d0 = bmax[2] - bmin[2];
    float d1 = bmax[0] - bmin[0];    
    if (d0 == 0.0f) d0 = 1.0f;
    if (d1 == 0.0f) d1 = 1.0f;
    tc = SbVec4f((point[0] - bmin[0]) / d1,
                 (point[2] - bmin[2]) / d0, 
                 0.0f, 1.0f);
    if (n[maxi] > 0.0f) tc[1] = 1.0f - tc[1];        
  }
  else {
    float d = bmax[1] - bmin[1];
    if (d == 0.0f) d = 1.0f;
    tc = SbVec4f((float) (atan2(point[0], point[2]) * (1.0/(2.0*M_PI)) + 0.5), 
                 (point[1] - bmin[1]) / d, 
                 0.0f, 1.0f);
  }


  return tc;

}


// Documented in superclass.
void
SoTextureCoordinateCylinder::doAction(SoAction * action)
{  
}

// Documented in superclass.
void
SoTextureCoordinateCylinder::GLRender(SoGLRenderAction * action)
{
  
  if (PRIVATE(this)->currentstate != action->getState()) 
    PRIVATE(this)->currentstate = action->getState();    

  PRIVATE(this)->currentshape = NULL;  

  SoTextureCoordinateElement::setFunction(PRIVATE(this)->currentstate, 
                                          this, textureCoordinateCylinderCallback, 
                                          PRIVATE(this));
}

// Documented in superclass.
void
SoTextureCoordinateCylinder::callback(SoCallbackAction * action)
{
  SoTextureCoordinateCylinder::doAction((SoAction *)action);
}

// Documented in superclass.
void
SoTextureCoordinateCylinder::pick(SoPickAction * action)
{
  SoTextureCoordinateCylinder::doAction((SoAction *)action);
}

