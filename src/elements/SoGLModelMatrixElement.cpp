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
 *  http://www.sim.no/ sales@sim.no Voice: +47 73540378 Fax: +47 73943861
 *
\**************************************************************************/

/*!
  \class SoGLModelMatrixElement Inventor/elements/SoGLModelMatrixElement.h
  \brief The SoGLModelMatrixElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoGLModelMatrixElement.h>

#include <Inventor/elements/SoViewingMatrixElement.h>
#include <Inventor/SbName.h>
#include <Inventor/SbRotation.h>
#include <Inventor/SbVec3f.h>
#include <Inventor/errors/SoDebugError.h>

#include <GL/gl.h>
#include <math.h>

//$ BEGIN TEMPLATE ElementSource( SoGLModelMatrixElement )

/*!
  \var SoGLModelMatrixElement::classTypeId

  This is the static class type identifier for the
  SoGLModelMatrixElement class.
*/

SoType SoGLModelMatrixElement::classTypeId = SoType::badType();

/*!
  This method returns the SoType object for the element class of
  the instance.
*/
SoType
SoGLModelMatrixElement::getClassTypeId(void)
{
  return SoGLModelMatrixElement::classTypeId;
}

/*!
  This static method creates an object instance of the SoGLModelMatrixElement class.
*/
void *
SoGLModelMatrixElement::createInstance(void)
{
  return (void *) new SoGLModelMatrixElement;
}

/*!
  \var SoGLModelMatrixElement::classStackIndex

  This is the static state stack index for the
  SoGLModelMatrixElement class.
*/
int SoGLModelMatrixElement::classStackIndex;

/*!
  This static method returns the state stack index for the SoGLModelMatrixElement class.
*/
int
SoGLModelMatrixElement::getClassStackIndex(void)
{
  return SoGLModelMatrixElement::classStackIndex;
}
//$ END TEMPLATE ElementSource

/*!
  This static method initializes static data for the
  SoGLModelMatrixElement class.
*/

void
SoGLModelMatrixElement::initClass( // static
    void )
{
//$ BEGIN TEMPLATE InitElementSource( SoGLModelMatrixElement )
  assert(SoGLModelMatrixElement::classTypeId == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoGLModelMatrixElement::classTypeId = SoType::createType(
    inherited::getClassTypeId(),
    "SoGLModelMatrixElement",
    &SoGLModelMatrixElement::createInstance);

  if (inherited::classStackIndex < 0) {
    SoGLModelMatrixElement::classStackIndex =
      createStackIndex( SoGLModelMatrixElement::classTypeId );
  } else {
    SoGLModelMatrixElement::classStackIndex =
      inherited::classStackIndex;
  }
//$ END TEMPLATE InitElementSource
} // initClass()

/*!
  This static method cleans up static data for the
  SoGLModelMatrixElement class.
*/

void
SoGLModelMatrixElement::cleanClass( // static
    void )
{
//$ BEGIN TEMPLATE CleanElementSource( SoGLModelMatrixElement )
//$ END TEMPLATE CleanElementSource
} // cleanClass()

/*!
  A constructor.  Can't be used directly.

  \sa void * SoGLModelMatrixElement::createInstance( void )
*/

SoGLModelMatrixElement::SoGLModelMatrixElement( // protected
    void )
  : state( NULL ), viewEltNodeId( 0 )
{
    setTypeId( SoGLModelMatrixElement::classTypeId );
    setStackIndex( SoGLModelMatrixElement::classStackIndex );
} // SoGLModelMatrixElement()

/*!
  The destructor.
*/

SoGLModelMatrixElement::~SoGLModelMatrixElement( // virtual
    void )
{
} // ~SoGLModelMatrixElement()

//! FIXME: write doc.

void
SoGLModelMatrixElement::init(SoState * state)
{
#if 0 // too much debug output.. 981021 mortene.
  SoDebugError::postInfo("SoGLModelMatrixElement::init", "");
#endif // 0
  this->state = state;
  inherited::init(state);
} // init()

//! FIXME: write doc.

void
SoGLModelMatrixElement::push(SoState * state)
{
#if 0 // too much debug output.. 981021 mortene.
  SoDebugError::postInfo("SoGLModelMatrixElement::push", "");
#endif // 0
  glPushMatrix();
  inherited::push(state);
  SoGLModelMatrixElement *elem = (SoGLModelMatrixElement*)
    this->next;
  elem->state = state;
} // push()

//! FIXME: write doc.

void
SoGLModelMatrixElement::pop(SoState * state,
			    const SoElement * prevTopElement)
{
#if 0 // too much debug output.. 981021 mortene.
  SoDebugError::postInfo("SoGLModelMatrixElement::pop", "");
#endif // 0
  inherited::pop(state, prevTopElement);
  glPopMatrix();
} // pop()

//! FIXME: write doc.

void
SoGLModelMatrixElement::makeEltIdentity()
{
  const SbMatrix &mat = SoViewingMatrixElement::get(this->state); 
  glLoadMatrixf(mat[0]);
  inherited::makeEltIdentity();
} // makeEltIdentity()

//! FIXME: write doc.

void
SoGLModelMatrixElement::setElt(const SbMatrix &matrix)
{
  inherited::setElt(matrix);
  SbMatrix mat = SoViewingMatrixElement::get(this->state);
  mat.multRight(matrix);
  glLoadMatrixf(mat[0]);
} // setElt()

//! FIXME: write doc.

void
SoGLModelMatrixElement::multElt(const SbMatrix &matrix)
{
#if 0 // debug
  SbMatrix m = this->modelMatrix;
  SoDebugError::post("SoGLModelMatrixElement::multElt",
		     "\n\tmodelmatrix before:"
		     "\n\t\t[ %f %f %f %f"
		     "\n\t\t  %f %f %f %f"
		     "\n\t\t  %f %f %f %f"
		     "\n\t\t  %f %f %f %f ]",
		     m[0][0], m[0][1], m[0][2], m[0][3],
		     m[1][0], m[1][1], m[1][2], m[1][3],
		     m[2][0], m[2][1], m[2][2], m[2][3],
		     m[3][0], m[3][1], m[3][2], m[3][3]);

  SoDebugError::post("SoGLModelMatrixElement::multElt",
		     "\n\tmultiply with:"
		     "\n\t\t[ %f %f %f %f"
		     "\n\t\t  %f %f %f %f"
		     "\n\t\t  %f %f %f %f"
		     "\n\t\t  %f %f %f %f ]",
		     matrix[0][0], matrix[0][1], matrix[0][2], matrix[0][3],
		     matrix[1][0], matrix[1][1], matrix[1][2], matrix[1][3],
		     matrix[2][0], matrix[2][1], matrix[2][2], matrix[2][3],
		     matrix[3][0], matrix[3][1], matrix[3][2], matrix[3][3]);
#endif // debug

  glMultMatrixf(matrix[0]);
  inherited::multElt(matrix);

#if 0 // debug
  m = this->modelMatrix;
  SoDebugError::post("SoGLModelMatrixElement::multElt",
		     "\n\tmodelmatrix after:"
		     "\n\t\t[ %f %f %f %f"
		     "\n\t\t  %f %f %f %f"
		     "\n\t\t  %f %f %f %f"
		     "\n\t\t  %f %f %f %f ]",
		     m[0][0], m[0][1], m[0][2], m[0][3],
		     m[1][0], m[1][1], m[1][2], m[1][3],
		     m[2][0], m[2][1], m[2][2], m[2][3],
		     m[3][0], m[3][1], m[3][2], m[3][3]);
#endif // debug
} // multElt()

//! FIXME: write doc.

void
SoGLModelMatrixElement::translateEltBy(const SbVec3f &translation)
{
  glTranslatef(translation[0], translation[1], translation[2]);
  inherited::translateEltBy(translation);
} // translateEltBy()

//! FIXME: write doc.

void
SoGLModelMatrixElement::rotateEltBy(const SbRotation &rotation)
{
  SbVec3f axis;
  float angle;
  rotation.getValue(axis, angle);
  glRotatef(angle*180.0f/M_PI, axis[0], axis[1], axis[2]);
  inherited::rotateEltBy(rotation);
} // rotateEltBy()

//! FIXME: write doc.

void
SoGLModelMatrixElement::scaleEltBy(const SbVec3f &scaleFactor)
{
  glScalef(scaleFactor[0], scaleFactor[1], scaleFactor[2]);
  inherited::scaleEltBy(scaleFactor);
} // scaleEltBy()

//! FIXME: write doc.

SbMatrix
SoGLModelMatrixElement::pushMatrixElt()
{
  SbMatrix matrix;
  glGetFloatv(GL_MODELVIEW_MATRIX, matrix[0]);
  return matrix;
} // pushMatrixElt()

//! FIXME: write doc.

void
SoGLModelMatrixElement::popMatrixElt(const SbMatrix &matrix)
{
  glLoadMatrixf(matrix[0]);
} // popMatrixElt()

