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
  \class SoGLShapeHintsElement Inventor/elements/SoGLShapeHintsElement.h
  \brief The SoGLShapeHintsElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoGLShapeHintsElement.h>

#include <Inventor/SbName.h>

#include <GL/gl.h>
#include <assert.h>

// for the "flags" variable
#define SOSH_CCW      0x01
#define SOSH_TWOSIDE  0x02
#define SOSH_CULL     0x04

//$ BEGIN TEMPLATE ElementSource( SoGLShapeHintsElement )

/*!
  \var SoGLShapeHintsElement::classTypeId

  This is the static class type identifier for the
  SoGLShapeHintsElement class.
*/

SoType SoGLShapeHintsElement::classTypeId = SoType::badType();

/*!
  This method returns the SoType object for the element class of
  the instance.
*/
SoType
SoGLShapeHintsElement::getClassTypeId(void)
{
  return SoGLShapeHintsElement::classTypeId;
}

/*!
  This static method creates an object instance of the SoGLShapeHintsElement class.
*/
void *
SoGLShapeHintsElement::createInstance(void)
{
  return (void *) new SoGLShapeHintsElement;
}

/*!
  \var SoGLShapeHintsElement::classStackIndex

  This is the static state stack index for the
  SoGLShapeHintsElement class.
*/
int SoGLShapeHintsElement::classStackIndex;

/*!
  This static method returns the state stack index for the SoGLShapeHintsElement class.
*/
int
SoGLShapeHintsElement::getClassStackIndex(void)
{
  return SoGLShapeHintsElement::classStackIndex;
}
//$ END TEMPLATE ElementSource

/*!
  This static method initializes static data for the
  SoGLShapeHintsElement class.
*/

void
SoGLShapeHintsElement::initClass( // static
    void )
{
//$ BEGIN TEMPLATE InitElementSource( SoGLShapeHintsElement )
  assert(SoGLShapeHintsElement::classTypeId == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoGLShapeHintsElement::classTypeId = SoType::createType(
    inherited::getClassTypeId(),
    "SoGLShapeHintsElement",
    &SoGLShapeHintsElement::createInstance);

  if (inherited::classStackIndex < 0) {
    SoGLShapeHintsElement::classStackIndex =
      createStackIndex( SoGLShapeHintsElement::classTypeId );
  } else {
    SoGLShapeHintsElement::classStackIndex =
      inherited::classStackIndex;
  }
//$ END TEMPLATE InitElementSource
} // initClass()

/*!
  This static method cleans up static data for the SoGLShapeHintsElement
  class.
*/

void
SoGLShapeHintsElement::cleanClass( // static
    void )
{
//$ BEGIN TEMPLATE CleanElementSource( SoGLShapeHintsElement )
//$ END TEMPLATE CleanElementSource
} // cleanClass()

/*!
  A constructor.  Can't be used directly.

  \sa void * SoGLShapeHintsElement::createInstance( void )
*/

SoGLShapeHintsElement::SoGLShapeHintsElement()
{
    setTypeId( SoGLShapeHintsElement::classTypeId );
    setStackIndex( SoGLShapeHintsElement::classStackIndex );
} // SoGLShapeHintsElement()

/*!
  The destructor.
*/

SoGLShapeHintsElement::~SoGLShapeHintsElement( // virtual
    void )
{
} // ~SoGLShapeHintsElement()

//! FIXME: write doc.

void
SoGLShapeHintsElement::init(SoState * state)
{
  inherited::init(state);
  glCullFace(GL_BACK);
  this->glflags = SOSH_CULL; // fake this to turn off culling
  this->updategl(SOSH_CCW | SOSH_TWOSIDE);
} // init()

//! FIXME: write doc.

void
SoGLShapeHintsElement::push(SoState * state)
{
  inherited::push(state);
  SoGLShapeHintsElement * elem =
    (SoGLShapeHintsElement *)this->next;

  elem->glflags = this->glflags;
} // push()

//! FIXME: write doc.

void
SoGLShapeHintsElement::pop(SoState * state,
			   const SoElement * prevTopElement)
{
  inherited::pop(state, prevTopElement);

  SoGLShapeHintsElement *prev = (SoGLShapeHintsElement*) prevTopElement;
  prev->glflags = this->glflags;
} // pop()

//! FIXME: write doc.

void
SoGLShapeHintsElement::setElt(VertexOrdering vertexOrdering,
			      ShapeType shapeType,
			      FaceType faceType)
{
  inherited::setElt(vertexOrdering, shapeType, faceType);
  // do nothing since this is a lazy element
} // setElt()

//! FIXME: write doc.

void
SoGLShapeHintsElement::evaluate() const
{
  unsigned int flags = 0;
  
  if (vertexOrdering == UNKNOWN_ORDERING) {
    flags |= (this->glflags & SOSH_CCW) | SOSH_TWOSIDE;
  }
  else {
    if (vertexOrdering == COUNTERCLOCKWISE) flags |= SOSH_CCW;
    if (shapeType == SOLID) {
      flags |= SOSH_CULL;
    }
  }
  ((SoGLShapeHintsElement*)this)->updategl(flags);
}

/*!
  Update gl state. Use this is you only want to modify the
  twoside lighting feature.
*/

void 
SoGLShapeHintsElement::forceSend(const SbBool twoside) const
{
  SoGLShapeHintsElement *elem = (SoGLShapeHintsElement*)this;
  unsigned int flags = glflags & ~SOSH_TWOSIDE;
  if (twoside) flags |= SOSH_TWOSIDE;
  elem->updategl(flags);
}

/*!
  Update GL state. Use this if you don't care about the two-side
  lighting state.
*/

void
SoGLShapeHintsElement::forceSend(const SbBool ccw, const SbBool cull) const
{
  SoGLShapeHintsElement *elem = (SoGLShapeHintsElement*)this;
  unsigned int flags = glflags & SOSH_TWOSIDE;
  if (ccw) flags |= SOSH_CCW;
  if (cull) flags |= SOSH_CULL;
  elem->updategl(flags);
}

//! FIXME: write doc.

void
SoGLShapeHintsElement::forceSend(const SbBool ccw, const SbBool cull, 
				 const SbBool twoside) const
{
  SoGLShapeHintsElement *elem = (SoGLShapeHintsElement*)this;
  unsigned int flags = 0;
  if (ccw) flags |= SOSH_CCW;
  if (cull) flags |= SOSH_CULL;
  if (twoside) flags |= SOSH_TWOSIDE;
  elem->updategl(flags);
}

//! FIXME: write doc.

void 
SoGLShapeHintsElement::updategl(const unsigned int flags)
{
#if 0
  glFrontFace(GL_CCW);
  glDisable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

#else // ! 0
  unsigned int xor = flags ^ this->glflags;
  if (xor == 0) return; // quick return if equal
  
  this->glflags = flags;

  if (xor & SOSH_CCW) {
    flags & SOSH_CCW ? glFrontFace(GL_CCW) : glFrontFace(GL_CW);
  }
  if (xor & SOSH_TWOSIDE) {
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 
		  flags & SOSH_TWOSIDE ? GL_TRUE : GL_FALSE); 
  }
  if (xor & SOSH_CULL) {
    flags & SOSH_CULL ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
  }
#endif // ! 0
}

