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
  \class SoGLTextureImageElement Inventor/elements/SoGLTextureImageElement.h
  \brief The SoGLTextureImageElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoGLTextureImageElement.h>

#include <Inventor/SbName.h>
#include <Inventor/misc/SoGLImage.h>
#include <Inventor/SoImageInterface.h>

#ifdef _WIN32
#include <windows.h>
#endif // !_WIN32

#include <GL/gl.h>
#include <assert.h>

//$ BEGIN TEMPLATE ElementSource( SoGLTextureImageElement )

/*!
  \var SoGLTextureImageElement::classTypeId

  This is the static class type identifier for the
  SoGLTextureImageElement class.
*/

SoType SoGLTextureImageElement::classTypeId = SoType::badType();

/*!
  This method returns the SoType object for the element class of
  the instance.
*/
SoType
SoGLTextureImageElement::getClassTypeId(void)
{
  return SoGLTextureImageElement::classTypeId;
}

/*!
  This static method creates an object instance of the SoGLTextureImageElement class.
*/
void *
SoGLTextureImageElement::createInstance(void)
{
  return (void *) new SoGLTextureImageElement;
}

/*!
  \var SoGLTextureImageElement::classStackIndex

  This is the static state stack index for the
  SoGLTextureImageElement class.
*/
int SoGLTextureImageElement::classStackIndex;

/*!
  This static method returns the state stack index for the SoGLTextureImageElement class.
*/
int
SoGLTextureImageElement::getClassStackIndex(void)
{
  return SoGLTextureImageElement::classStackIndex;
}
//$ END TEMPLATE ElementSource

/*!
  This static method initializes static data in the 
  SoGLTextureImageElement class.
*/

void
SoGLTextureImageElement::initClass( // static
    void )
{
//$ BEGIN TEMPLATE InitElementSource( SoGLTextureImageElement )
  assert(SoGLTextureImageElement::classTypeId == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoGLTextureImageElement::classTypeId = SoType::createType(
    inherited::getClassTypeId(),
    "SoGLTextureImageElement",
    &SoGLTextureImageElement::createInstance);

  if (inherited::classStackIndex < 0) {
    SoGLTextureImageElement::classStackIndex =
      createStackIndex( SoGLTextureImageElement::classTypeId );
  } else {
    SoGLTextureImageElement::classStackIndex =
      inherited::classStackIndex;
  }
//$ END TEMPLATE InitElementSource
} // initClass()

/*!
  This static method cleans up static data for the
  SoGLTextureImageElement class.
*/

void
SoGLTextureImageElement::cleanClass( // static
    void )
{
//$ BEGIN TEMPLATE CleanElementSource( SoGLTextureImageElement )
//$ END TEMPLATE CleanElementSource
} // cleanClass()

/*!
  A constructor.  Can't be used directly.

  \sa void * SoGLTextureImageElement::createInstance( void )
*/

SoGLTextureImageElement::SoGLTextureImageElement()
{
  setTypeId( SoGLTextureImageElement::classTypeId );
  setStackIndex( SoGLTextureImageElement::classStackIndex );
} // SoGLTextureImageElement()

/*!
  The destructor.
*/

SoGLTextureImageElement::~SoGLTextureImageElement( // virtual
    void )
{
} // ~SoGLTextureImageElement()

//! FIXME: write doc.

void
SoGLTextureImageElement::init(SoState * state)
{
  inherited::init(state);
  this->glimage = NULL;
} // init()

//! FIXME: write doc.

void
SoGLTextureImageElement::pop(SoState * state,
			     const SoElement * prevTopElement)
{
  inherited::pop(state, prevTopElement);
  SoGLTextureImageElement *prev = (SoGLTextureImageElement*)
    prevTopElement;

  if (prev->glimage != NULL)
    prev->glimage->apply();
} // pop()

//! FIXME: write doc.

void 
SoGLTextureImageElement::set(SoState * const state, SoNode * const node,
			     SoGLImage *glimage, const Model model,
			     const SbColor &blendColor)
{
  SoGLTextureImageElement *elem = (SoGLTextureImageElement*)
    SoReplacedElement::getElement(state, classStackIndex, node);
  if (glimage) {
    // enable GL texturing if not enabled
    if (elem->glimage == NULL) glEnable(GL_TEXTURE_2D);
    const SoImageInterface *image = glimage->getImage();
    // keep SoTextureImageElement "up-to-date"
    inherited::set(state, node,
		   image->getSize(),
		   image->getNumComponents(),
		   image->getDataPtr(),
		   glimage->shouldClampS() ? 
		   SoTextureImageElement::CLAMP : 
		   SoTextureImageElement::REPEAT,
		   glimage->shouldClampT() ?
		   SoTextureImageElement::CLAMP :
		   SoTextureImageElement::REPEAT,
		   model,
		   blendColor);

    // got to check this in case textures are shared between nodes.
    // yes, we think people (or programs) who create .wrl|iv files 
    // are stupid :)
    if (elem->glimage != glimage) {
      elem->glimage = glimage;
      glimage->apply();
    }
  }
  else {
    // not much to do, is there...
  }
}

