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
  \class SoGLTextureEnabledElement Inventor/elements/SoGLTextureEnabledElement.h
  \brief The SoGLTextureEnabledElement class is yet to be documented.

  FIXME: write doc.
*/

//
// FIXME: this should be made a lazy element (pederb, 990624)
//

#include <Inventor/elements/SoGLTextureEnabledElement.h>

#if !defined(COIN_EXCLUDE_SOSHAPESTYLEELEMENT)
#include <Inventor/elements/SoShapeStyleElement.h>
#endif // ! COIN_EXCLUDE_SOSHAPESTYLEELEMENT

#include <Inventor/SbName.h>

#ifdef _WIN32
#include <windows.h>
#endif // !_WIN32

#include <GL/gl.h>
#include <assert.h>

//$ BEGIN TEMPLATE ElementSource( SoGLTextureEnabledElement )

/*!
  \var SoGLTextureEnabledElement::classTypeId

  This is the static class type identifier for the
  SoGLTextureEnabledElement class.
*/

SoType SoGLTextureEnabledElement::classTypeId = SoType::badType();

/*!
  This method returns the SoType object for the element class of
  the instance.
*/
SoType
SoGLTextureEnabledElement::getClassTypeId(void)
{
  return SoGLTextureEnabledElement::classTypeId;
}

/*!
  This static method creates an object instance of the SoGLTextureEnabledElement class.
*/
void *
SoGLTextureEnabledElement::createInstance(void)
{
  return (void *) new SoGLTextureEnabledElement;
}

/*!
  \var SoGLTextureEnabledElement::classStackIndex

  This is the static state stack index for the
  SoGLTextureEnabledElement class.
*/
int SoGLTextureEnabledElement::classStackIndex;

/*!
  This static method returns the state stack index for the SoGLTextureEnabledElement class.
*/
int
SoGLTextureEnabledElement::getClassStackIndex(void)
{
  return SoGLTextureEnabledElement::classStackIndex;
}
//$ END TEMPLATE ElementSource

/*!
  This static method initializes static data for the
  SoGLTextureEnabledElement class.
*/

void
SoGLTextureEnabledElement::initClass(
    void )
{
//$ BEGIN TEMPLATE InitElementSource( SoGLTextureEnabledElement )
  assert(SoGLTextureEnabledElement::classTypeId == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoGLTextureEnabledElement::classTypeId = SoType::createType(
    inherited::getClassTypeId(),
    "SoGLTextureEnabledElement",
    &SoGLTextureEnabledElement::createInstance);

  if (inherited::classStackIndex < 0) {
    SoGLTextureEnabledElement::classStackIndex =
      createStackIndex( SoGLTextureEnabledElement::classTypeId );
  } else {
    SoGLTextureEnabledElement::classStackIndex =
      inherited::classStackIndex;
  }
//$ END TEMPLATE InitElementSource
}

/*!
  This static method cleans up static data for the
  SoGLTextureEnabledElement class.
*/

void
SoGLTextureEnabledElement::cleanClass(
    void )
{
//$ BEGIN TEMPLATE CleanElementSource( SoGLTextureEnabledElement )
//$ END TEMPLATE CleanElementSource
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoGLTextureEnabledElement::createInstance( void )
*/

SoGLTextureEnabledElement::SoGLTextureEnabledElement(
    void )
{
    setTypeId( SoGLTextureEnabledElement::classTypeId );
    setStackIndex( SoGLTextureEnabledElement::classStackIndex );
    this->data = getDefault();
}

/*!
  The destructor.
*/

SoGLTextureEnabledElement::~SoGLTextureEnabledElement(
    void )
{
}

//! FIXME: write doc.

void
SoGLTextureEnabledElement::set(SoState * const state,
			       SoNode * const node,
			       const SbBool enabled)
{
  SoInt32Element::set(classStackIndex, state, node, (int32_t) enabled);
#if !defined(COIN_EXCLUDE_SOSHAPESTYLEELEMENT)
  SoShapeStyleElement::setTextureEnabled(state, enabled);
#endif // ! COIN_EXCLUDE_SOSHAPESTYLEELEMENT
}

//! FIXME: write doc.

void
SoGLTextureEnabledElement::init(SoState * state)
{
  inherited::init(state);
  this->data = getDefault();
  updategl();
}

//! FIXME: write doc.

void
SoGLTextureEnabledElement::push(SoState * state)
{
  inherited::push(state);
  ((SoGLTextureEnabledElement*)this->next)->data = this->data;
}

//! FIXME: write doc.

void
SoGLTextureEnabledElement::pop(SoState * state,
			       const SoElement * prevTopElement)
{
  if (this->data != ((SoGLTextureEnabledElement*)prevTopElement)->data) 
    ((SoGLTextureEnabledElement*)prevTopElement)->updategl();
  inherited::pop(state, prevTopElement);
}

//! FIXME: write doc.

//$ EXPORT INLINE
void 
SoGLTextureEnabledElement::set(SoState * const state, const SbBool enabled)
{
  set(state, NULL, enabled); 
}

//! FIXME: write doc.

//$ EXPORT INLINE
SbBool 
SoGLTextureEnabledElement::get(SoState * const state)
{ 
  return (SbBool) SoInt32Element::get( classStackIndex, state ); 
}

//! FIXME: write doc.

//$ EXPORT INLINE
SbBool 
SoGLTextureEnabledElement::getDefault()
{ 
  return FALSE; 
}

//! FIXME: write doc.

void 
SoGLTextureEnabledElement::setElt(int32_t value)
{
  if (this->data != value) {
    inherited::setElt(value);
    this->data = value;
    updategl();
  }
}

//! FIXME: write doc.

void
SoGLTextureEnabledElement::updategl()
{
  if (this->data) glEnable(GL_TEXTURE_2D);
  else glDisable(GL_TEXTURE_2D);
}

