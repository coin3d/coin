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
  \class SoGLTextureCoordinateElement Inventor/elements/SoGLTextureCoordinateElement.h
  \brief The SoGLTextureCoordinateElement class stores the current gltexture
  coordinates.
*/

#include <Inventor/elements/SoGLTextureCoordinateElement.h>

#if !defined(COIN_EXCLUDE_SOSHAPESTYLEELEMENT)
#include <Inventor/elements/SoShapeStyleElement.h>
#endif // ! COIN_EXCLUDE_SOSHAPESTYLEELEMENT

#include <Inventor/SbName.h>

#ifdef _WIN32
#include <windows.h>
#endif // !_WIN32

#include <GL/gl.h>
#include <assert.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

//$ BEGIN TEMPLATE ElementSource(SoGLTextureCoordinateElement)

/*!
  \var SoGLTextureCoordinateElement::classTypeId

  This is the static class type identifier for the
  SoGLTextureCoordinateElement class.
*/

SoType SoGLTextureCoordinateElement::classTypeId = SoType::badType();

/*!
  This method returns the SoType object for the element class of
  the instance.
*/
SoType
SoGLTextureCoordinateElement::getClassTypeId(void)
{
  return SoGLTextureCoordinateElement::classTypeId;
}

/*!
  This static method creates an object instance of the SoGLTextureCoordinateElement class.
*/
void *
SoGLTextureCoordinateElement::createInstance(void)
{
  return (void *) new SoGLTextureCoordinateElement;
}

/*!
  \var SoGLTextureCoordinateElement::classStackIndex

  This is the static state stack index for the
  SoGLTextureCoordinateElement class.
*/
int SoGLTextureCoordinateElement::classStackIndex;

/*!
  This static method returns the state stack index for the SoGLTextureCoordinateElement class.
*/
int
SoGLTextureCoordinateElement::getClassStackIndex(void)
{
  return SoGLTextureCoordinateElement::classStackIndex;
}
//$ END TEMPLATE ElementSource

/*!
  This static method initializes static data for the
  SoGLTextureCoordinateElement class.
*/

void
SoGLTextureCoordinateElement::initClass(void)
{
//$ BEGIN TEMPLATE InitElementSource(SoGLTextureCoordinateElement)
  assert(SoGLTextureCoordinateElement::classTypeId == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoGLTextureCoordinateElement::classTypeId =
    SoType::createType(inherited::getClassTypeId(),
                       "SoGLTextureCoordinateElement",
                       &SoGLTextureCoordinateElement::createInstance);

  if (inherited::classStackIndex < 0) {
    SoGLTextureCoordinateElement::classStackIndex =
      createStackIndex(SoGLTextureCoordinateElement::classTypeId);
  }
  else {
    SoGLTextureCoordinateElement::classStackIndex = inherited::classStackIndex;
  }
//$ END TEMPLATE InitElementSource
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoGLTextureCoordinateElement::createInstance(void)
*/

SoGLTextureCoordinateElement::SoGLTextureCoordinateElement()
{
    setTypeId(SoGLTextureCoordinateElement::classTypeId);
    setStackIndex(SoGLTextureCoordinateElement::classStackIndex);
}

/*!
  The destructor.
*/

SoGLTextureCoordinateElement::~SoGLTextureCoordinateElement()
{
}

//!  FIXME: write doc.

void
SoGLTextureCoordinateElement::init(SoState * state)
{
  inherited::init(state);
  this->texgenCB = NULL;
  this->texgenData = NULL;
}

//!  FIXME: write doc.

void
SoGLTextureCoordinateElement::push(SoState * state)
{
  inherited::push(state);
//   SoGLTextureCoordinateElement *elem = (SoGLTextureCoordinateElement*)
//     this->next;
}

//!  FIXME: write doc.

void
SoGLTextureCoordinateElement::pop(SoState * state,
                                  const SoElement * prevTopElement)
{
  inherited::pop(state, prevTopElement);
  ((SoGLTextureCoordinateElement*)prevTopElement)->doCallback();
}

//!  FIXME: write doc.

void
SoGLTextureCoordinateElement::setTexGen(SoState * const state,
                                        SoNode * const node,
                                        SoTexCoordTexgenCB * const texgenFunc,
                                        void * const texgenData,
                                        SoTextureCoordinateFunctionCB * const func,
                                        void * const funcData)
{
#if !defined(COIN_EXCLUDE_SOSHAPESTYLEELEMENT)
  SoShapeStyleElement::setTextureFunction(state, texgenFunc && !func);
#endif // ! COIN_EXCLUDE_SOSHAPESTYLEELEMENT

  SoTextureCoordinateElement::setFunction(state, node, func, funcData);

  SoGLTextureCoordinateElement *element = (SoGLTextureCoordinateElement *)
    SoReplacedElement::getElement(state, classStackIndex, node);
  element->setElt(texgenFunc, texgenData);
  element->doCallback();
}

//!  FIXME: write doc.

SoGLTextureCoordinateElement::CoordType
SoGLTextureCoordinateElement::getType() const
{
  if (this->texgenCB) return NONE;
  return inherited::getType();
}

//!  FIXME: write doc.

const SoGLTextureCoordinateElement *
SoGLTextureCoordinateElement::getInstance(SoState * const state)
{
  return (SoGLTextureCoordinateElement*)
    SoElement::getConstElement(state, classStackIndex);
}

//!  FIXME: write doc.

void
SoGLTextureCoordinateElement::send(const int index) const
{
  assert(this->whatKind == EXPLICIT);
  assert(index < this->numCoords);
  if (this->coordsAre2D)
    glTexCoord2fv(coords2[index].getValue());
  else
    glTexCoord4fv(coords4[index].getValue());
}

//!  FIXME: write doc.

void
SoGLTextureCoordinateElement::send(const int index,
                                   const SbVec3f & /* c */,
                                   const SbVec3f & /* n */) const
{
  if (index < 0) return;
  if (index >= this->numCoords) return;

  // FIXME: handle texcoord functions
  assert(this->whatKind == EXPLICIT);
  assert(index < this->numCoords);
  if (this->coordsAre2D)
    glTexCoord2fv(coords2[index].getValue());
  else
    glTexCoord4fv(coords4[index].getValue());
}

//!  FIXME: write doc.

void
SoGLTextureCoordinateElement::setElt(SoTexCoordTexgenCB * func,
                                     void *data)
{
  this->texgenCB = func;
  this->texgenData = data;
}

void
SoGLTextureCoordinateElement::doCallback() const
{
  if (this->texgenCB) {
    this->texgenCB(this->texgenData);
  }
  else {
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
  }
}
