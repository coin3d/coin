/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

/*!
  \class SoClipPlaneElement Inventor/elements/SoClipPlaneElement.h
  \brief The SoClipPlaneElement class is used to manage the clip plane stack.
  \ingroup elements
*/

#include <Inventor/elements/SoClipPlaneElement.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/nodes/SoNode.h>

//
// constructor for the internal class
//
so_plane_data::so_plane_data(const SbPlane &plane, const SbMatrix &matrix)
{
  this->plane = this->wcPlane = plane;
  this->wcPlane.transform(matrix);
}

/*!
  \fn SoClipPlaneElement::planes
  List of currently active planes.
*/

/*!
  \fn SoClipPlaneElement::startIndex
  Index of first clip plane in this element. Used to disable clip planes
  in SoGLClipPlaneElement::pop().
*/

SO_ELEMENT_SOURCE(SoClipPlaneElement);

// doc from parent
void
SoClipPlaneElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoClipPlaneElement, inherited);
}

/*!
  The destructor.
*/
SoClipPlaneElement::~SoClipPlaneElement()
{
}

/*!
  Adds \a plane as an active plane. Calls addToElt() to do the job.
*/
void
SoClipPlaneElement::add(SoState * const state,
                        SoNode * const node,
                        const SbPlane & plane)
{
  SoClipPlaneElement *element =
    (SoClipPlaneElement*) SoElement::getElement(state, classStackIndex);
  if (element) {
    element->addToElt(plane, SoModelMatrixElement::get(state));
    if (node) element->addNodeId(node);
  }
}

/*!
  Returns the current (top-of-stack) element.
*/
const SoClipPlaneElement *
SoClipPlaneElement::getInstance(SoState * const state)
{
  return (const SoClipPlaneElement*)
    SoElement::getConstElement(state, classStackIndex);
}

/*!
  Returns the current number of active clipping planes.
*/
int
SoClipPlaneElement::getNum() const
{
  return this->planes.getLength();
}

/*!
  Returns the \index'th plane.
*/
const SbPlane &
SoClipPlaneElement::get(const int index,
                        const SbBool inworldspace) const
{
  assert(index >= 0 && index < this->planes.getLength());
  if (inworldspace) return this->planes.getArrayPtr()[index].wcPlane;
  return this->planes.getArrayPtr()[index].plane;
}

/*!
  This method adds the clipping plane, \a plane, to an instance.
  \a modelmatrix is the current model matrix.
*/
void
SoClipPlaneElement::addToElt(const SbPlane &plane,
                             const SbMatrix &modelMatrix)
{
  so_plane_data data(plane, modelMatrix);
  this->planes.append(data);
}

// doc from parent
void
SoClipPlaneElement::init(SoState * state)
{
  inherited::init(state);
  this->planes.truncate(0);
  this->startIndex = 0;
}

// Documented in superclass. Overridden to copy planes into the new
// top of stack, since planes are accumulated. Also copies accumulated
// node ids.
void
SoClipPlaneElement::push(SoState * state)
{
  inherited::push(state);
  
  SoClipPlaneElement * const prev =
    (SoClipPlaneElement *)this->getNextInStack();
  
  this->planes.truncate(0);
  for (int i = 0; i < prev->planes.getLength(); i++) {
    this->planes.append(prev->planes[i]);
  }
  this->startIndex = prev->planes.getLength();
  this->copyNodeIds(prev);
}
