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
  \class SoGLViewportRegionElement Inventor/elements/SoGLViewportRegionElement.h
  \brief The SoGLViewportRegionElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoGLViewportRegionElement.h>

#include <Inventor/SbName.h>

#ifdef _WIN32
#include <windows.h>
#endif // !_WIN32

#include <GL/gl.h>
#include <assert.h>

//$ BEGIN TEMPLATE ElementSource( SoGLViewportRegionElement )

/*!
  \var SoGLViewportRegionElement::classTypeId

  This is the static class type identifier for the
  SoGLViewportRegionElement class.
*/

SoType SoGLViewportRegionElement::classTypeId = SoType::badType();

/*!
  This method returns the SoType object for the element class of
  the instance.
*/
SoType
SoGLViewportRegionElement::getClassTypeId(void)
{
  return SoGLViewportRegionElement::classTypeId;
}

/*!
  This static method creates an object instance of the SoGLViewportRegionElement class.
*/
void *
SoGLViewportRegionElement::createInstance(void)
{
  return (void *) new SoGLViewportRegionElement;
}

/*!
  \var SoGLViewportRegionElement::classStackIndex

  This is the static state stack index for the
  SoGLViewportRegionElement class.
*/
int SoGLViewportRegionElement::classStackIndex;

/*!
  This static method returns the state stack index for the SoGLViewportRegionElement class.
*/
int
SoGLViewportRegionElement::getClassStackIndex(void)
{
  return SoGLViewportRegionElement::classStackIndex;
}
//$ END TEMPLATE ElementSource

/*!
  This static method initializes static data for the
  SoGLViewportRegionElement class.
*/

void
SoGLViewportRegionElement::initClass( // static
    void )
{
//$ BEGIN TEMPLATE InitElementSource( SoGLViewportRegionElement )
  assert(SoGLViewportRegionElement::classTypeId == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoGLViewportRegionElement::classTypeId = SoType::createType(
    inherited::getClassTypeId(),
    "SoGLViewportRegionElement",
    &SoGLViewportRegionElement::createInstance);

  if (inherited::classStackIndex < 0) {
    SoGLViewportRegionElement::classStackIndex =
      createStackIndex( SoGLViewportRegionElement::classTypeId );
  } else {
    SoGLViewportRegionElement::classStackIndex =
      inherited::classStackIndex;
  }
//$ END TEMPLATE InitElementSource
}

/*!
  This static method cleans up static data for the
  SoGLViewportRegionElement class.
*/

void
SoGLViewportRegionElement::cleanClass( // static
    void )
{
//$ BEGIN TEMPLATE CleanElementSource( SoGLViewportRegionElement )
//$ END TEMPLATE CleanElementSource
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoGLViewportRegionElement::createInstance( void )
*/

SoGLViewportRegionElement::SoGLViewportRegionElement()
{
  setTypeId( SoGLViewportRegionElement::classTypeId );
  setStackIndex( SoGLViewportRegionElement::classStackIndex );
}

/*!
  The destructor.
*/

SoGLViewportRegionElement::~SoGLViewportRegionElement()
{
}

//! FIXME: write doc.

void
SoGLViewportRegionElement::init(SoState * state)
{
  inherited::init(state);
  this->initialized = FALSE;
}

//! FIXME: write doc.

void
SoGLViewportRegionElement::push(SoState * state)
{
  inherited::push(state);
  SoGLViewportRegionElement *elem = (SoGLViewportRegionElement*)
    this->next;
  elem->viewportRegion = this->viewportRegion;
  elem->initialized = TRUE;
}

//! FIXME: write doc.

void
SoGLViewportRegionElement::pop(SoState * state,
			       const SoElement * prevTopElement)
{
  SoGLViewportRegionElement *prev = 
    (SoGLViewportRegionElement *)prevTopElement;
  if (!(this->viewportRegion == prev->viewportRegion))
    prev->updategl();
  inherited::pop(state, prevTopElement);
}

//! FIXME: write doc.

void
SoGLViewportRegionElement::setElt(const SbViewportRegion & viewportRegion)
{
  this->viewportRegion = viewportRegion;
  this->initialized = TRUE;
  this->updategl();
}

//! FIXME: write doc.

void
SoGLViewportRegionElement::updategl() const
{
  if (this->initialized) {
    SbVec2s origin = this->viewportRegion.getViewportOriginPixels();
    SbVec2s size = this->viewportRegion.getViewportSizePixels();
    glViewport(origin[0], origin[1], size[0], size[1]);
  }
#if 0 // too much debug output.. 981021 mortene.
  SoDebugError::postInfo("SoGLViewportRegionElement::updategl",
			 "glViewport: %d %d %d %d\n",
			 origin[0], origin[1], size[0], size[1]);
#endif // 0
}

