/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

/*!
  \class SoClipPlaneElement Inventor/elements/SoClipPlaneElement.h
  \brief The SoClipPlaneElement class is yet to be documented.
*/

#include <Inventor/elements/SoClipPlaneElement.h>

#include <Inventor/elements/SoModelMatrixElement.h>

#include <assert.h>



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

  FIXME: write doc.
*/

/*!
  \fn SoClipPlaneElement::num

  FIXME: write doc.
*/

/*!
  \fn SoClipPlaneElement::startIndex

  FIXME: write doc.
*/

SO_ELEMENT_SOURCE(SoClipPlaneElement);

/*!
  This static method initializes static data for the
  SoClipPlaneElement class.
*/

void
SoClipPlaneElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoClipPlaneElement, inherited);
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoClipPlaneElement::createInstance()
*/

SoClipPlaneElement::SoClipPlaneElement()
  : startIndex(0),
    planes(1)
{
  setTypeId(SoClipPlaneElement::classTypeId);
  setStackIndex(SoClipPlaneElement::classStackIndex);
}

/*!
  The destructor.
*/

SoClipPlaneElement::~SoClipPlaneElement()
{
}

//! FIXME: write doc.

void
SoClipPlaneElement::add(SoState * const state,
                        SoNode * const /* node */,
                        const SbPlane & plane)
{
  SoClipPlaneElement *element =
    (SoClipPlaneElement*) SoElement::getElement(state, classStackIndex);

  element->addToElt(plane, SoModelMatrixElement::get(state));
}

//! FIXME: write doc.

const SoClipPlaneElement *
SoClipPlaneElement::getInstance(SoState * const state)
{
  return (const SoClipPlaneElement*)
    SoElement::getConstElement(state, classStackIndex);
}

//! FIXME: write doc.

//$ EXPORT INLINE
int
SoClipPlaneElement::getNum() const
{
  return this->planes.getLength();
}

//! FIXME: write doc.

const SbPlane &
SoClipPlaneElement::get(const int index,
                        const SbBool inWorldSpace) const
{
  // Dynamically allocated to avoid problems on systems which doesn't
  // handle static constructors. (Yes, I know we only use the
  // operator=() in this method, but better safe than sorry -- if the
  // code changes, we've got a hard to find bug on our hands).
  static SbPlane * staticplane = new SbPlane; // FIXME: should deallocate on exit. 20000406 mortene.

  assert(index >= 0 && index < this->planes.getLength());
  const so_plane_data &data = this->planes[index];
  if (inWorldSpace) *staticplane = data.wcPlane;
  else *staticplane = data.plane;

  return *staticplane;
}

//! FIXME: write doc.

void
SoClipPlaneElement::print(FILE *file) const
{
  fprintf(file, "SoClipPlaneElement[%p]: num = %d, start = %d\n", this,
          this->planes.getLength(), this->startIndex);
}

/*!
  This method adds the clipping plane, \a plane, to an instance.
  \a modelMatrix is the current model matrix.
*/

void
SoClipPlaneElement::addToElt(const SbPlane &plane,
                             const SbMatrix &modelMatrix)
{
  so_plane_data data(plane, modelMatrix);
  this->planes.append(data);
}

//! FIXME: write doc.

void
SoClipPlaneElement::init(SoState * state)
{
  inherited::init(state);
  this->startIndex = 0;
  this->planes.truncate(0);
}

//! FIXME: write doc.

void
SoClipPlaneElement::push(SoState * state)
{
  inherited::push(state);

  SoClipPlaneElement * const element =
    (SoClipPlaneElement *)(this->next);

  element->planes.truncate(0);
  for (int i = 0; i < this->planes.getLength(); i++) {
    element->planes.append(this->planes[i]);
  }
  element->startIndex = this->planes.getLength();
}

//! FIXME: write doc.

void
SoClipPlaneElement::pop(SoState * state,
                        const SoElement * prevTopElement)
{
  inherited::pop(state, prevTopElement);
}
