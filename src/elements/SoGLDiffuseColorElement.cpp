/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
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
  \class SoGLDiffuseColorElement Inventor/elements/SoGLDiffuseColorElement.h
  \brief The SoGLDiffuseColorElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoGLDiffuseColorElement.h>

#include <Inventor/SbColor.h>

#ifdef _WIN32
#include <windows.h>
#endif // !_WIN32

#include <GL/gl.h>

#include <assert.h>

#define DIV255 0.0039215686f

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

//! FIXME: write doc.

void
SoGLDiffuseColorElement::init(SoState *state)
{
  inherited::init(state);
  this->currentPacked = 0xccccccff;
  this->current.setValue(0.8f, 0.8f, 0.8f, 1.0f);
  this->updategl();
}

//! FIXME: write doc.

void
SoGLDiffuseColorElement::push(SoState *state)
{
  inherited::push(state);
  SoGLDiffuseColorElement * elem =
    (SoGLDiffuseColorElement *)this->next;
  elem->current = this->current;
  elem->currentPacked = this->currentPacked;
}

//! FIXME: write doc.

void
SoGLDiffuseColorElement::pop(SoState *state, const SoElement *prevTopElement)
{
  inherited::pop(state, prevTopElement);
  SoGLDiffuseColorElement *prev =
    (SoGLDiffuseColorElement*)prevTopElement;

  prev->current = this->current;
  prev->currentPacked = this->currentPacked;
}

//! FIXME: write doc.

static void
create_packed(unsigned int r, unsigned int g, unsigned int b,
              unsigned int a, uint32_t &packed)
{
  packed = (r<<24)|(g<<16)|(b<<8)|a;
}

//! FIXME: write doc.

static void
convert_packed(uint32_t packed, SbVec4f &v)
{
  v[3] = float(packed&0xff)*DIV255;
  packed >>= 8;
  v[2] = float(packed & 0xff)*DIV255;
  packed >>= 8;
  v[1] = float(packed & 0xff)*DIV255;
  v[0] = float(packed >> 8)*DIV255;
}

//! FIXME: write doc.

void
SoGLDiffuseColorElement::send(const int index, const float alpha)
{
  assert(this->numColors > 0);
  int realindex = index < this->numColors ? index : this->numColors - 1;
  if (realindex < 0) realindex = 0;

  if (this->colors) {
    const SbVec3f &c = this->colors[realindex];
    SbVec4f col(c[0], c[1], c[2], alpha);
    if (col != this->current) { // FIXME: is it worth it?
      this->current = col;
      create_packed((unsigned int)(col[0]*255.0f),
                    (unsigned int)(col[1]*255.0f),
                    (unsigned int)(col[2]*255.0f),
                    (unsigned int)(col[3]*255.0f),
                    this->currentPacked);
      this->updategl();
    }
  }
  else if (this->packedColors &&
           this->packedColors[realindex] != this->currentPacked) {
    this->currentPacked = this->packedColors[realindex];
    convert_packed(this->currentPacked, current);
    this->updategl();
  }
}

/*!
  FIXME: write doc.
*/
void
SoGLDiffuseColorElement::send(const int index)
{
  this->send(index, this->current[3]);
}

/*!
  Do not use. Provided only to support SoGLLazyElement.
*/
void
SoGLDiffuseColorElement::sendOnePacked(const uint32_t packedcol)
{
  SoGLDiffuseColorElement *thisp = (SoGLDiffuseColorElement*)this;
  thisp->currentPacked = packedcol;
  convert_packed(packedcol, thisp->current);
  thisp->updategl();
}

//! FIXME: write doc.

void
SoGLDiffuseColorElement::updategl()
{
#if 1 // if using GL_COLOR_MATERIAL
  glColor4fv((const GLfloat*)&current);
#else
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, col);
#endif
}
