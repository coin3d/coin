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
  \class SoGLNormalizeElement Inventor/elements/SoGLNormalizeElement.h
  \brief The SoGLNormalizeElement class is for GL optimization only.
  
  It keeps track of the GL_NORMALIZE flag and enables/disables this
  GL feature depending on the GL matrix and supplied normals. 
  It is very important that all transformation nodes calls 
  SoGLNormalizeElement::setMatrixState(state, FALSE) if it contains
  a scale operation. Also, geometry nodes should let this
  element know if it is going to use unit-length normals
  (by overloading SoShape::willSendUnitLengthNormals()).
  When the GL matrix does not contain any scale operations, and the normals
  are unit length, GL_NORMALIZE can be disabled.

  This is a lazy element which is evaluated by SoShape.
*/

#include <Inventor/elements/SoGLNormalizeElement.h>

#include <Inventor/SbName.h>

#ifdef _WIN32
#include <windows.h>
#endif // !_WIN32

#include <GL/gl.h>
#include <assert.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

//$ BEGIN TEMPLATE ElementSource(SoGLNormalizeElement)

/*!
  \var SoGLNormalizeElement::classTypeId

  This is the static class type identifier for the
  SoGLNormalizeElement class.
*/

SoType SoGLNormalizeElement::classTypeId = SoType::badType();

/*!
  This method returns the SoType object for the element class of
  the instance.
*/
SoType
SoGLNormalizeElement::getClassTypeId(void)
{
  return SoGLNormalizeElement::classTypeId;
}

/*!
  This static method creates an object instance of the SoGLNormalizeElement class.
*/
void *
SoGLNormalizeElement::createInstance(void)
{
  return (void *) new SoGLNormalizeElement;
}

/*!
  \var SoGLNormalizeElement::classStackIndex

  This is the static state stack index for the
  SoGLNormalizeElement class.
*/
int SoGLNormalizeElement::classStackIndex;

/*!
  This static method returns the state stack index for the SoGLNormalizeElement class.
*/
int
SoGLNormalizeElement::getClassStackIndex(void)
{
  return SoGLNormalizeElement::classStackIndex;
}
//$ END TEMPLATE ElementSource

/*!
  This static method initializes static data for the
  SoGLNormalizeElement class.
*/

void
SoGLNormalizeElement::initClass()
{
//$ BEGIN TEMPLATE InitElementSource(SoGLNormalizeElement)
  assert(SoGLNormalizeElement::classTypeId == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoGLNormalizeElement::classTypeId =
    SoType::createType(inherited::getClassTypeId(),
                       "SoGLNormalizeElement",
                       &SoGLNormalizeElement::createInstance);

  if (inherited::classStackIndex < 0) {
    SoGLNormalizeElement::classStackIndex =
      createStackIndex(SoGLNormalizeElement::classTypeId);
  }
  else {
    SoGLNormalizeElement::classStackIndex = inherited::classStackIndex;
  }
//$ END TEMPLATE InitElementSource
}

/*!
  This static method cleans up static data for the
  SoGLNormalizeElement class.
*/

void
SoGLNormalizeElement::cleanClass()
{
//$ BEGIN TEMPLATE CleanElementSource(SoGLNormalizeElement)
//$ END TEMPLATE CleanElementSource
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoGLNormalizeElement::createInstance(void)
*/

SoGLNormalizeElement::SoGLNormalizeElement()
{
  setTypeId(SoGLNormalizeElement::classTypeId);
  setStackIndex(SoGLNormalizeElement::classStackIndex);
}

/*!
  The destructor.
*/

SoGLNormalizeElement::~SoGLNormalizeElement()
{
}

//! FIXME: write doc.

void
SoGLNormalizeElement::init(SoState * state)
{
  inherited::init(state);
  this->unitNormals = FALSE; /* we know nothing about the normals yet */
  this->okMatrix = TRUE;     /* first matrix is identity */

  /* force a glEnable(GL_NORMALIZE) */ 
  this->glnormalize = FALSE; 
  this->updategl(TRUE);
}

//! FIXME: write doc.

void
SoGLNormalizeElement::push(SoState * state)
{
  inherited::push(state);
  SoGLNormalizeElement *elem = (SoGLNormalizeElement*)this->next;
  elem->glnormalize = this->glnormalize;
  elem->unitNormals = this->unitNormals;
  elem->okMatrix = this->okMatrix;
}

//! FIXME: write doc.

void
SoGLNormalizeElement::pop(SoState * state,
			   const SoElement * prevTopElement)
{
  ((SoGLNormalizeElement*)prevTopElement)->glnormalize = this->glnormalize;
  inherited::pop(state, prevTopElement);
}

//! FIXME: write doc.

SbBool
SoGLNormalizeElement::matches(const SoElement * /* element */) const
{
  assert(0 && "FIXME: not implemented");
  return FALSE;
}

//! FIXME: write doc.

SoElement *
SoGLNormalizeElement::copyMatchInfo() const
{
  assert(0 && "FIXME: not implemented");
  return NULL;
}

/*!
  Should be called only when the node's matrix contains a
  scale transformation (or some other transformation which will create 
  non-unit length normals). It should not be called to tell the element 
  that the node has a transformation which is ok. The exception
  is when a node causes a new matrix to be loaded, and it
  is certain that this new matrix contains no scaling.
*/
void 
SoGLNormalizeElement::setMatrixState(SoState * const state,
				     const SbBool valid)
{
  SoGLNormalizeElement * e = (SoGLNormalizeElement *)
    inherited::getElement(state, SoGLNormalizeElement::classStackIndex);
  e->okMatrix = valid;

#if 0 // debug
  SoDebugError::postInfo("SoGLNormalizeElement::setMatrixState",
			 "%d", valid);
#endif // debug

}

/*!
  Normal and geometry nodes should call this to let the element know if
  the normals are unit length or not.
*/
void 
SoGLNormalizeElement::setUnitNormals(SoState * const state,
				     const SbBool unitNormals)
{
  SoGLNormalizeElement * e = (SoGLNormalizeElement *)
    inherited::getElement(state, SoGLNormalizeElement::classStackIndex);
  e->unitNormals = unitNormals;

#if 0 // debug
  SoDebugError::postInfo("SoGLNormalizeElement::setUnitNormals",
			 "%d", unitNormals);
#endif // debug

}


/*
  Lazy update
*/
void
SoGLNormalizeElement::evaluate() const
{
  SbBool normalize = !(this->unitNormals && this->okMatrix);

#if 0 // debug
  SoDebugError::postInfo("SoGLNormalizeElement::evaluate",
			 "%d %d %d", unitNormals, okMatrix, normalize);
#endif // debug

  ((SoGLNormalizeElement*)this)->updategl(normalize);
}

/*!
  Turns on/off normalizing without affecting the state. This
  method should be used by shapes supplying their own
  normals. If it is known that the normals are unit length,
  this method should be used to optimize rendering. 
  Normalizing is not disabled unless matrix is valid though.
*/
void
SoGLNormalizeElement::forceSend(const SbBool unit) const
{
#if 0 // debug
  SoDebugError::postInfo("SoGLNormalizeElement::forceSend",
			 "%d", unit);
#endif // debug
  ((SoGLNormalizeElement*)this)->updategl(!(unit && this->okMatrix));
}

//! FIXME: write doc.

void
SoGLNormalizeElement::updategl(const SbBool normalize)
{
#if 0 // debug
  SoDebugError::postInfo("SoGLNormalizeElement::updategl",
			 "norm: %d %d", normalize, glnormalize);
#endif // debug
  
#if 0 // to disable this optimizing feature 
  if (!glnormalize) {
    glnormalize = TRUE;
    glEnable(GL_NORMALIZE);
  }
#else // optimized
  if (normalize != this->glnormalize) {
    this->glnormalize = normalize;
    if (normalize) glEnable(GL_NORMALIZE);
    else glDisable(GL_NORMALIZE); /* yeah, 10% speed increase :-) */
  }
#endif // optimized
}



