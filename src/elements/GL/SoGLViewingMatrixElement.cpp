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
  \class SoGLViewingMatrixElement Inventor/elements/SoGLViewingMatrixElement.h
  \brief The SoGLViewingMatrixElement class is used to store the current viewing matrix.
  \ingroup elements

  The viewing matrix contains the inverse camera coordinate system
  matrix. The camera coordinate system is built from the field values
  in the current SoCamera (currently either SoPerspectiveCamera or
  SoOrthographicCamera) and any transformations prior to the camera in
  the scene graph.
*/

#include "coindefs.h"
#include <Inventor/elements/SoGLViewingMatrixElement.h>
#include <Inventor/actions/SoGLRenderAction.h>


#include <Inventor/elements/SoModelMatrixElement.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/system/gl.h>
#include "elements/GL/SoResetMatrixElement.h"

SO_ELEMENT_SOURCE(SoGLViewingMatrixElement);

// doc in parent
void
SoGLViewingMatrixElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoGLViewingMatrixElement, inherited);
  SoResetMatrixElement::initClass();
}

/*!
  The destructor.
*/
SoGLViewingMatrixElement::~SoGLViewingMatrixElement(void)
{
}

// doc in parent
void
SoGLViewingMatrixElement::init(SoState * stateptr)
{
  inherited::init(stateptr);
  this->state = stateptr;
  this->mmidentity = TRUE;
}

// doc in parent
void
SoGLViewingMatrixElement::push(SoState * stateptr)
{
  inherited::push(stateptr);
  this->state = stateptr;
}

// doc in parent
void
SoGLViewingMatrixElement::pop(SoState * stateptr,
                              const SoElement * COIN_UNUSED_ARG(prevTopElement))
{
  this->capture(stateptr);
  this->updategl();
}

/*!
  Returns the node id of the current camera node. This is used by
  SoGLModelMatrixElement to detect a change in the viewing matrix
  inside an SoTransformSeparator node.
*/
uint32_t
SoGLViewingMatrixElement::getNodeId(SoState * const state)
{
  const SoReplacedElement *elem = (const SoReplacedElement*)
    SoElement::getConstElement(state, classStackIndex);
  return elem->getNodeId();
}

/*!
  Sets the current viewing matrix.
*/
void
SoGLViewingMatrixElement::setElt(const SbMatrix & matrix)
{
  inherited::setElt(matrix);
  this->modelmatrix = SoModelMatrixElement::get(this->state, this->mmidentity);
  if (this->state->isElementEnabled(SoResetMatrixElement::getClassStackIndex())) {
    SbMatrix mat = this->viewingMatrix;
    if (!this->mmidentity) {
      mat.multRight(this->modelmatrix);
      mat.multLeft(this->modelmatrix.inverse());
    }
    SoResetMatrixElement::set(this->state, mat);
  }
  this->updategl();
}

// sends the current viewing matrix to GL. Eliminates the model matrix
// part of the matrix, since a GL matrix is always located in 0 0 0.
void
SoGLViewingMatrixElement::updategl(void)
{
  SbMatrix mat = this->viewingMatrix;
  if (!this->mmidentity) {
    mat.multRight(this->modelmatrix);
  }
  glLoadMatrixf((float*)mat);
}

/*!
  Returns the matrix that should be used by SoGLModelMatrixElement
  when the transformation should be reset during traversal (typically
  when encountering an SoResetTransform node). It is not sufficient to
  simply load the viewing matrix, since the transformations that were
  applied before the camera needs to be accounted for.

  \COIN_FUNCTION_EXTENSION

  \since Coin 2.0
*/
SbMatrix
SoGLViewingMatrixElement::getResetMatrix(SoState * state)
{
  if (state->isElementEnabled(SoResetMatrixElement::getClassStackIndex())) {
    return SoResetMatrixElement::get(state);
  }
  const SoGLViewingMatrixElement * element = (const SoGLViewingMatrixElement *)
    SoElement::getConstElement(state, classStackIndex);

  SbMatrix mat = element->viewingMatrix;
  if (!element->mmidentity) {
    // first eliminate model matrix part of matrix
    mat.multRight(element->modelmatrix);
    // then move geometry to account for the transformations prior to
    // the camera
    mat.multLeft(element->modelmatrix.inverse());
  }
  return mat;
}
