/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

/*!
  \class SoGLDiffuseColorElement Inventor/elements/SoGLDiffuseColorElement.h
  \brief The SoGLDiffuseColorElement class is yet to be documented.
  \ingroup elements

  FIXME: write doc.
*/

#include <Inventor/elements/SoGLDiffuseColorElement.h>

#include <Inventor/SbColor4f.h>

#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/system/gl.h>

#include <assert.h>

SO_ELEMENT_SOURCE(SoGLDiffuseColorElement);

/*!
  This static method initializes static data for the
  SoGLDiffuseColorElement class.
*/

void
SoGLDiffuseColorElement::initClass()
{
  SO_ELEMENT_INIT_CLASS(SoGLDiffuseColorElement, inherited);
}

/*!
  The destructor.
*/

SoGLDiffuseColorElement::~SoGLDiffuseColorElement()
{
}

inline void
SoGLDiffuseColorElement::updategl(const uint32_t col)
{
  glColor4ub((col>>24)&0xff, (col>>16)&0xff, (col>>8)&0xff, col&0xff);
}

//! FIXME: write doc.

void
SoGLDiffuseColorElement::init(SoState *state)
{
  inherited::init(state);
  this->packedRGBA = 0xccccccff;
  this->updategl(this->packedRGBA);
}

//! FIXME: write doc.

void
SoGLDiffuseColorElement::push(SoState * state)
{
  inherited::push(state);
  SoGLDiffuseColorElement * const prev =
    (SoGLDiffuseColorElement *)this->getNextInStack();
  this->packedRGBA = prev->packedRGBA;
}

//! FIXME: write doc.

void
SoGLDiffuseColorElement::pop(SoState *state, const SoElement * prevTopElement)
{
  inherited::pop(state, prevTopElement);
  SoGLDiffuseColorElement * const prev =
    (SoGLDiffuseColorElement *)prevTopElement;
  this->packedRGBA = prev->packedRGBA;
}

//! FIXME: write doc.

void
SoGLDiffuseColorElement::send(const int index, const float alpha)
{
  int realindex = SbClamp(index, 0, this->numColors - 1);
  // FIXME: shouldn't there be a warning here? 20020119 mortene.
  
  uint32_t packed = 0;

  if (this->colors) {
    SbColor4f rgba(this->colors[realindex], alpha);
    packed = rgba.getPackedValue();
  }
  else if (this->packedColors) {
    packed = this->packedColors[realindex];
  }
  // FIXME: "else {"? Warning? Assert? 20020119 mortene.


  if (this->packedRGBA != packed) {
    this->updategl(packed);
    this->packedRGBA = packed;
  }
}

/*!
  FIXME: write doc.
*/
void
SoGLDiffuseColorElement::send(const int index)
{
  this->send(index, (this->packedRGBA & 0xff) / 255.0f);
}

/*!
  Can be used to send a color to GL. If you plan to write your own
  GL-code where you send colors to GL, it might be a good idea to
  use this method to enable the element to keep track of the current
  OpenGL color.

  Colors represented in the SbColor or SbColor4f types are easily
  converted to packed format by using either SbColor::getPackedValue()
  or SbColor4f::getPackedValue().

  This method was not part of the original SGI Inventor v2.1 API, and
  is an extension specific to Coin.
*/
void
SoGLDiffuseColorElement::sendOnePacked(const uint32_t packedcol)
{
  if (this->packedRGBA != packedcol) {
    this->updategl(packedcol);
    this->packedRGBA = packedcol;
  }
}
