/**************************************************************************\
 * 
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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

#include <Inventor/SbName.h>
#include <Inventor/elements/SoModelMatrixElement.h>

#include <assert.h>

class plane_data
{
public:
  plane_data(const SbPlane &plane, const SbMatrix &matrix) {
    this->plane = this->wcPlane = plane;
    this->wcPlane.transform(matrix);
  }
public: // yeah, yeah
  SbPlane plane;
  SbPlane wcPlane;
}; // class plane_data

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

//$ BEGIN TEMPLATE ElementSource(SoClipPlaneElement)

/*!
  \var SoClipPlaneElement::classTypeId

  This is the static class type identifier for the
  SoClipPlaneElement class.
*/

SoType SoClipPlaneElement::classTypeId = SoType::badType();

/*!
  This method returns the SoType object for the element class of
  the instance.
*/
SoType
SoClipPlaneElement::getClassTypeId(void)
{
  return SoClipPlaneElement::classTypeId;
}

/*!
  This static method creates an object instance of the SoClipPlaneElement class.
*/
void *
SoClipPlaneElement::createInstance(void)
{
  return (void *) new SoClipPlaneElement;
}

/*!
  \var SoClipPlaneElement::classStackIndex

  This is the static state stack index for the
  SoClipPlaneElement class.
*/
int SoClipPlaneElement::classStackIndex;

/*!
  This static method returns the state stack index for the SoClipPlaneElement class.
*/
int
SoClipPlaneElement::getClassStackIndex(void)
{
  return SoClipPlaneElement::classStackIndex;
}
//$ END TEMPLATE ElementSource

/*!
  This static method initializes static data for the
  SoClipPlaneElement class.
*/

void
SoClipPlaneElement::initClass(void)
{
//$ BEGIN TEMPLATE InitElementSource(SoClipPlaneElement)
  assert(SoClipPlaneElement::classTypeId == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoClipPlaneElement::classTypeId =
    SoType::createType(inherited::getClassTypeId(),
                       "SoClipPlaneElement",
                       &SoClipPlaneElement::createInstance);

  if (inherited::classStackIndex < 0) {
    SoClipPlaneElement::classStackIndex =
      createStackIndex(SoClipPlaneElement::classTypeId);
  }
  else {
    SoClipPlaneElement::classStackIndex = inherited::classStackIndex;
  }
//$ END TEMPLATE InitElementSource
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoClipPlaneElement::createInstance()
*/

SoClipPlaneElement::SoClipPlaneElement()
  : startIndex(0)
{
  this->num = 0;
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
  //  return this->planes.getLength();
  return this->num;
}

//! FIXME: write doc.

const SbPlane &
SoClipPlaneElement::get(const int /* index */,
			const SbBool /* inWorldSpace */) const
{
  static SbPlane dummy;

//    assert(index >= 0 && index < this->planes.getLength());
//    plane_data *data = (plane_data*) this->planes[index]; 
//    if (inWorldSpace) return data->plane;
//    else return data->wcPlane;

  return dummy;
}

//! FIXME: write doc.

void
SoClipPlaneElement::print(FILE * /* file */) const
{
//    fprintf(file, "SoClipPlaneElement[%p]: num = %d, start = %d\n", this,
//  	  this->planes.getLength(), this->startIndex);
}

/*!
  This method adds the clipping plane, \a plane, to an instance.
  \a modelMatrix is the current model matrix.
*/

void
SoClipPlaneElement::addToElt(const SbPlane &plane,
			     const SbMatrix &modelMatrix)
{
  plane_data *data = new plane_data(plane, modelMatrix);
  //  this->planes.append((void*)data);
  planes[num++] = data;
}

//! FIXME: write doc.

void
SoClipPlaneElement::init(SoState * state)
{
  inherited::init(state);
  this->startIndex = 0;
  this->num = 0;
}

//! FIXME: write doc.

void
SoClipPlaneElement::push(SoState * state)
{
  inherited::push(state);

  SoClipPlaneElement * const element =
    (SoClipPlaneElement *)(this->next);
//    element->planes.copy(this->planes); // copy pointers
//    element->startIndex = this->planes.getLength();

  for (int i = 0; i < num; i++) {
    element->planes[i] = planes[i];
  } 
  element->num = num;
  element->startIndex = num;
}

//! FIXME: write doc.

void
SoClipPlaneElement::pop(SoState * state,
			const SoElement * prevTopElement)
{
//    for (int i = this->planes.getLength()-1; i >= this->startIndex; i--) { 
//      delete (plane_data*) this->planes[i];
//      //    this->planes.remove(i);
//    }

  for (int i = this->num-1; i >= this->startIndex; i--) { 
    delete (plane_data*) this->planes[i];
    //    this->planes.remove(i);
  }
  inherited::pop(state, prevTopElement);
}

