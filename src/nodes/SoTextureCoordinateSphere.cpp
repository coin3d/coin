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

#include <Inventor/nodes/SoTextureCoordinateSphere.h>
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
#include <Inventor/caches/SoBoundingBoxCache.h>

/*!
  \class SoTextureCoordinateSphere include/Inventor/nodes/SoTextureCoordinateSphere.h
  \brief The SoTextureCoordinateSphere class autogenerates spheremapped texture coordinated for shapes.
  \ingroup bundles
*/

// FIXME: Add a better class description (20040123 handegar)

SO_NODE_SOURCE(SoTextureCoordinateSphere);

class SoTextureCoordinateSphereP {

public:
  SoTextureCoordinateSphereP(SoTextureCoordinateSphere * texturenode) 
    : master(texturenode) { }

  SbVec3f origo;
  SbBox3f boundingbox;
  SoNode * currentshape;
  SoState * currentstate;
  
  SbVec4f calculateTextureCoordinate(SbVec3f point, SbVec3f n);
  
private:
  SoTextureCoordinateSphere * master;
};


static const SbVec4f & textureCoordinateSphereCallback(void * userdata, const SbVec3f & point, const SbVec3f & normal);

#undef PRIVATE
#undef PUBLIC
#define PRIVATE(p) (p->pimpl)
#define PUBLIC(p) (p->master)


/*!
  Constructor.
*/
SoTextureCoordinateSphere::SoTextureCoordinateSphere(void)
{

  PRIVATE(this) = new SoTextureCoordinateSphereP(this);

  SO_NODE_INTERNAL_CONSTRUCTOR(SoTextureCoordinateSphere);

  PRIVATE(this)->currentshape = NULL;
  PRIVATE(this)->currentstate = NULL;
  PRIVATE(this)->origo = SbVec3f(0,0,0);

}

/*!
  Destructor.
*/
SoTextureCoordinateSphere::~SoTextureCoordinateSphere()
{
}

// Documented in superclass.
void
SoTextureCoordinateSphere::initClass(void)
{
  // FIXME: This should actually be COIN_2_2_3 (20040122 handegar)
  SO_NODE_INTERNAL_INIT_CLASS(SoTextureCoordinateSphere, SO_FROM_COIN_2_2);
  
  SO_ENABLE(SoGLRenderAction, SoGLTextureCoordinateElement);
  SO_ENABLE(SoCallbackAction, SoTextureCoordinateElement);
  SO_ENABLE(SoPickAction, SoTextureCoordinateElement);

}

const SbVec4f &
textureCoordinateSphereCallback(void * userdata, 
                          const SbVec3f & point, 
                          const SbVec3f & normal)
{
  
  SoTextureCoordinateSphereP * pimpl = (SoTextureCoordinateSphereP *) userdata;
  
  SoState * state = pimpl->currentstate;
  SoFullPath * path = (SoFullPath *) state->getAction()->getCurPath();
  SoNode * node = path->getTail();


  if (!node->isOfType(SoShape::getClassTypeId())) {
    // FIXME: A better way to handle this? (20040122 handegar)
    assert(FALSE && "TextureCoordinateSphere callback called for a non-SoShape node.");
  }

  // Cast the node into a shape
  SoShape * shape = (SoShape *) node;

  if (shape != pimpl->currentshape) {  
    pimpl->boundingbox.makeEmpty();  
    const SoBoundingBoxCache * bboxcache = shape->getBoundingBoxCache();    
    if (bboxcache && bboxcache->isValid(state)) {
      pimpl->boundingbox = bboxcache->getProjectedBox();
      pimpl->origo = pimpl->boundingbox.getCenter();
    }
    else {
      shape->computeBBox(state->getAction(), pimpl->boundingbox, pimpl->origo);
      pimpl->origo = pimpl->boundingbox.getCenter();
    }
    pimpl->currentshape = shape;
  }

  const SbVec4f & ret = pimpl->calculateTextureCoordinate(point, normal);
  return ret;
  
}

SbVec4f
SoTextureCoordinateSphereP::calculateTextureCoordinate(SbVec3f point, SbVec3f n)
{

  // FIXME: This way of mapping will always lead to artifacts in the
  // change between 360 and 0 degrees around the Y-axis. This is
  // unavoidable as the callback cannot predict when the last vertex
  // will be received, and therefore be able to patch up the
  // transition. (20040127 handegar)
 
  SbVec4f tc((float) (atan2(point[0], point[2]) * (1.0/(2.0*M_PI)) + 0.5),
             (float) (atan2(point[1], sqrt(point[0]*point[0] + point[2]*point[2])) * (1.0/M_PI) + 0.5),
             0.0f, 1.0f);

  return tc;
 
}


// Documented in superclass.
void
SoTextureCoordinateSphere::doAction(SoAction * action)
{  
}

// Documented in superclass.
void
SoTextureCoordinateSphere::GLRender(SoGLRenderAction * action)
{
  
  PRIVATE(this)->currentstate = action->getState();    
  PRIVATE(this)->currentshape = NULL;  
  SoTextureCoordinateElement::setFunction(PRIVATE(this)->currentstate, 
                                          this, textureCoordinateSphereCallback, 
                                          PRIVATE(this));
}

// Documented in superclass.
void
SoTextureCoordinateSphere::callback(SoCallbackAction * action)
{
  SoTextureCoordinateSphere::doAction((SoAction *)action);
}

// Documented in superclass.
void
SoTextureCoordinateSphere::pick(SoPickAction * action)
{
  SoTextureCoordinateSphere::doAction((SoAction *)action);
}

