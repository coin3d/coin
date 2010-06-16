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

/*!
  \class SoGLMultiTextureMatrixElement Inventor/elements/SoGLMultiTextureMatrixElement.h
  \brief The SoGLMultiTextureMatrixElement class is used to update the OpenGL texture matrix.
  \ingroup elements

  Since (for some weird reason) most OpenGL implementations have a very
  small texture matrix stack, and since the matrix stack also is broken
  on many OpenGL implementations, the texture matrix is always loaded
  into OpenGL. We do not push() and pop() matrices.
*/

#include <Inventor/elements/SoGLMultiTextureMatrixElement.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/C/glue/gl.h>
#include <cassert>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/system/gl.h>

SO_ELEMENT_SOURCE(SoGLMultiTextureMatrixElement);


// doc from parent
void
SoGLMultiTextureMatrixElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoGLMultiTextureMatrixElement, inherited);
}

/*!
  The destructor.
*/

SoGLMultiTextureMatrixElement::~SoGLMultiTextureMatrixElement(void)
{
}

// doc from parent
void
SoGLMultiTextureMatrixElement::init(SoState * state)
{
  inherited::init(state);

  SoAction * action = state->getAction();
  assert(action->isOfType(SoGLRenderAction::getClassTypeId()));
  // fetch cache context from action since SoGLCacheContextElement
  // might not be initialized yet.
  SoGLRenderAction * glaction = (SoGLRenderAction*) action;
  this->cachecontext = glaction->getCacheContext();
}

void
SoGLMultiTextureMatrixElement::push(SoState * state)
{
  inherited::push(state);
  SoGLMultiTextureMatrixElement * prev = (SoGLMultiTextureMatrixElement*)
    this->getNextInStack();

  this->cachecontext = prev->cachecontext;

  // capture previous element since we might or might not change the
  // GL state in set/pop
  prev->capture(state);
}

// doc from parent
void
SoGLMultiTextureMatrixElement::pop(SoState * state,
                                   const SoElement * prevTopElement)
{
  inherited::pop(state, prevTopElement);

  const SoGLMultiTextureMatrixElement * prev = 
    static_cast<const SoGLMultiTextureMatrixElement *> (prevTopElement);
  
  SbMatrix identity = SbMatrix::identity();
  const int numunits = SbMax(this->getNumUnits(),
                             prev->getNumUnits());
  for (int i = 0; i < numunits; i++) {
    const SbMatrix & thism = 
      (i < this->getNumUnits()) ?
      this->getUnitData(i).textureMatrix : identity;
    
    const SbMatrix & prevm = 
      (i < prev->getNumUnits()) ? 
      prev->getUnitData(i).textureMatrix : identity;
    
    if (thism != prevm) {
      this->updategl(i);
    }
  }
}

// doc from parent
void
SoGLMultiTextureMatrixElement::multElt(const int unit, const SbMatrix & matrix)
{
  inherited::multElt(unit, matrix);
  this->updategl(unit);
}

void
SoGLMultiTextureMatrixElement::setElt(const int unit, const SbMatrix & matrix)
{
  inherited::setElt(unit, matrix);
  this->updategl(unit);
}


// updates GL state
void
SoGLMultiTextureMatrixElement::updategl(const int unit) const
{
  const cc_glglue * glue = cc_glglue_instance(this->cachecontext);
  if (unit != 0) {
    cc_glglue_glActiveTexture(glue, (GLenum) (int(GL_TEXTURE0) + unit));
  }
  glMatrixMode(GL_TEXTURE);
  if (unit < this->getNumUnits()) {
    glLoadMatrixf(this->getUnitData(unit).textureMatrix[0]);
  }
  else {
    glLoadIdentity();
  }
  glMatrixMode(GL_MODELVIEW);
  if (unit != 0) {
    cc_glglue_glActiveTexture(glue, (GLenum) GL_TEXTURE0);
  }
}

