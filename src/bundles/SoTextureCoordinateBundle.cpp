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
  \class SoTextureCoordinateBundle include/Inventor/bundles/SoTextureCoordinateBundle.h
  \brief The SoTextureCoordinateBundle class simplifies texture coordinate handling.
  bla, bla, bla, FIXME: doc
*/

#include <Inventor/bundles/SoTextureCoordinateBundle.h>

#if !defined(COIN_EXCLUDE_SOSTATE)
#include <Inventor/misc/SoState.h>
#endif // !COIN_EXCLUDE_SOSTATE

#if !defined(COIN_EXCLUDE_SOTEXTURECOORDINATEELEMENT)
#include <Inventor/elements/SoTextureCoordinateElement.h>
#endif

#if !defined(COIN_EXCLUDE_SOGLTEXTURECOORDINATEELEMENT)
#include <Inventor/elements/SoGLTextureCoordinateElement.h>
#endif

#if !defined(COIN_EXCLUDE_SOGLTEXTUREENABLEDELEMENT)
#include <Inventor/elements/SoGLTextureEnabledElement.h>
#endif

#ifdef _WIN32
#include <windows.h>
#endif // _WIN32

#include <GL/gl.h>
#include <assert.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG


#define FLAG_FOR_RENDERING 0x1
#define FLAG_FUNCTION      0x4
#define FLAG_NEEDCOORDS    0x8

/*!
  Constructor with \a action being the action applied to the node.
  The \a forRendering parameter must be \e TRUE if the bundle is to
  be used for sending texture coordinates to GL during rendering.
  The \a setUpDefault must be \e TRUE if default texture coordinates
  should be generated.
*/
SoTextureCoordinateBundle::
SoTextureCoordinateBundle(SoAction * const action, 
			  const SbBool forRendering,
			  const SbBool setUpDefault)
  : SoBundle(action)
{
  this->flags = 0;

#if !defined(COIN_EXCLUDE_SOTEXTURECOORDINATEELEMENT)
  coordElt = SoTextureCoordinateElement::getInstance(state);
  switch (coordElt->getType()) {
  case SoTextureCoordinateElement::DEFAULT:
    assert(0 && "FIXME: not implemented");
    break;
  case SoTextureCoordinateElement::EXPLICIT:
    if (coordElt->getNum() > 0) {
      this->flags |= FLAG_NEEDCOORDS;
    }
    break;
  case SoTextureCoordinateElement::FUNCTION:
    this->flags |= FLAG_FUNCTION; 
    this->flags |= FLAG_NEEDCOORDS; // not automatically generated
    break;

  case SoTextureCoordinateElement::TEXGEN:
    // texcoord won't be needed. This will only happen during SoGLRenderAction,
    // when GL generates texture coorinates. Therefore, we will not set
    // the FLAG_NEEDCOORDS here.
    this->flags |= FLAG_FUNCTION;
    break;
  default:
    assert(0 && "FIXME: unknown CoordType");
    break;
  }

#endif
  glElt = NULL;
#if !defined(COIN_EXCLUDE_SOGLTEXTUREENABLEDELEMENT)
  if (forRendering && SoGLTextureEnabledElement::get(state)) {
    glElt = (SoGLTextureCoordinateElement*) coordElt;
  }
#endif // !COIN_EXCLUDE_SOGLTEXTUREENABLEDELEMENT
  if (setUpDefault) {
    assert(0 && "FIXME: implement default texture coordinates (pederb)");
  }
  
  if (forRendering) this->flags |= FLAG_FOR_RENDERING;
}
  
/*!
  Destructor.
*/
SoTextureCoordinateBundle::~SoTextureCoordinateBundle()
{
}

/*!
  Returns \e TRUE if texture coordinates is needed dureing rendering.
*/
SbBool 
SoTextureCoordinateBundle::needCoordinates() const
{
  return (this->flags & FLAG_NEEDCOORDS) != 0;
}

/*!
  Returns \e TRUE if a texture coordinate function should be used.
*/
SbBool 
SoTextureCoordinateBundle::isFunction() const
{
  return (this->flags & FLAG_FUNCTION) != 0;
}

/*!
  Returns the texture coordinates based on \a point and \a normal.
  Should only be used if SoTextureCoordinateBundle::isFunction() is \a TRUE.
*/
const SbVec4f & 
SoTextureCoordinateBundle::get(const SbVec3f &point, const SbVec3f &normal)
{
  assert(coordElt != NULL && (this->flags & FLAG_FUNCTION));
  return coordElt->get(point, normal);
}

/*!
  Returns the texture coordinates at index \a index.
  Should only be used if SoTextureCoordinateBundle::isFunction() is \a FALSE.
*/
const SbVec4f &
SoTextureCoordinateBundle::get(const int index)
{
  assert(coordElt && !(this->flags & FLAG_FUNCTION));
  return coordElt->get4(index);
}


/*!
  \fn void SoTextureCoordinateBundle::send(const int index) const
  Send texture coordinates to GL. Should only be used if 
  SoTextureCoordinateBundle::isFunction() is \a FALSE.
*/

/*!
  \fn void SoTextureCoordinateBundle::send(const int index, const SbVec3f &point, const SbVec3f &normal) const
  Convenience function that will make it transparent to the rendering
  code if ordinary texture coordinates or function texture coordinates
  are used.
*/

