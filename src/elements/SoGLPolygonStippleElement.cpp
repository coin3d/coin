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
  \class SoGLPolygonStippleElement Inventor/elements/SoGLPolygonStippleElement.h
  \brief The SoGLPolygonStippleElement class is yet to be documented.
  \ingroup elements

  FIXME: write doc.
*/

#include <Inventor/elements/SoGLPolygonStippleElement.h>

#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/system/gl.h>

#include <assert.h>

unsigned char SoGLPolygonStippleElement::patterns[64 + 1][32 * 4];

SO_ELEMENT_SOURCE(SoGLPolygonStippleElement);

// Some data and functions to create Bayer dither
// matrices (used for screen door transparency)

static unsigned int two_by_two[] = {0, 2, 3, 1};

//
// Used to generate a matrix twice the size of the input
//
static void 
generate_next_matrix(unsigned int * old, int oldsize,
                     unsigned int * matrix)
{
  int i,j;
  int newsize = oldsize << 1;
  for (i = 0; i <  newsize; i++) {
    for (j = 0; j < newsize; j++) {
      matrix[i*newsize+j] = 4 * old[(i%oldsize)*oldsize + (j%oldsize)];
      matrix[i*newsize+j] += two_by_two[(i/oldsize)*2 + (j/oldsize)];
    }
  }
}

//
// Creates a matrix by starting with a 2x2 and doubling until size
// Warning: only 2^n sizes!!
//
static void 
make_dither_matrix(uint32_t * ptr, int size)
{
  int currsize = 2;

  unsigned int * currmatrix = two_by_two;
  unsigned int * nextmatrix = NULL;
  int nextsize;

  while (currsize < size) {
    nextsize = currsize << 1;
    nextmatrix = new unsigned int[nextsize*nextsize];
    generate_next_matrix(currmatrix, currsize, nextmatrix);
    if (currmatrix != two_by_two) delete[] currmatrix;
    currmatrix = nextmatrix;
    currsize = nextsize;
  }
  // copy matrix
  int i;
  for (i = 0; i < size*size; i++)
    ptr[i] = currmatrix[i];

  if (currmatrix != two_by_two) delete[] currmatrix;
}

//
// Sets a bit bitnr bits from ptr
//
static void 
set_bit(int bitnr, uint32_t * ptr)
{
  int byte = bitnr / 32;
  int bit = bitnr % 32;

  int mask = 0x80000000 >> bit;

  ptr[byte] |= mask;
}

//
// Create a bitmap from a 32x32 matrix
//
static void 
create_matrix_bitmap(int intensity, uint32_t *bitmap,
                     uint32_t *matrix, int size)
{
  int cnt = 0;
  int i,j;
  for (i = 0; i < 32; i++) bitmap[i] = 0;
  for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      if (matrix[i*size+j] > (unsigned int) intensity) {
        set_bit(i*32+j, bitmap);
        cnt++;
      }
    }
  }
}

/*!
  This static method initializes static data for the
  SoGLPolygonStippleElement class.
*/

void
SoGLPolygonStippleElement::initClass()
{
  SO_ELEMENT_INIT_CLASS(SoGLPolygonStippleElement, inherited);

  // create patterns
  int i;
  unsigned int matrix[32*32];
  make_dither_matrix(matrix, 32); 
  for (i = 0; i <= 64; i++) {
    int intensity = (32 * 32 * i) / 64 - 1;
    create_matrix_bitmap((intensity >= 0) ? intensity : 0,
                         (uint32_t*)&patterns[i][0], matrix, 32);
  }
}

/*!
  The destructor.
*/

SoGLPolygonStippleElement::~SoGLPolygonStippleElement()
{
}

//! FIXME: write doc.

void
SoGLPolygonStippleElement::init(SoState * /* state */)
{
  this->isEnabled = this->currentEnabled = FALSE;
  this->currentPattern = this->pattern = -1;
  this->updategl();
}

//! FIXME: write doc.

void
SoGLPolygonStippleElement::set(SoState * const state, const SbBool onoff)
{
  SoGLPolygonStippleElement *elem = (SoGLPolygonStippleElement*)
    SoElement::getElement(state, classStackIndex);

  if (elem) {
    elem->isEnabled = onoff;
  }
}

//! FIXME: write doc.

void
SoGLPolygonStippleElement::setTransparency(SoState * const state,
                                    const float transparency)
{
  SoGLPolygonStippleElement *elem = (SoGLPolygonStippleElement*)
    SoElement::getElement(state, classStackIndex);

  if (elem) {
    int stipplenum = (int)(64.0f*(transparency)); //0-64
    
    if (stipplenum < 0) stipplenum = 0;
    if (stipplenum > 64) stipplenum = 64;
    elem->pattern = stipplenum;
  }
}

//! FIXME: write doc.

SbBool
SoGLPolygonStippleElement::get(SoState * const state)
{
  SoGLPolygonStippleElement *elem = (SoGLPolygonStippleElement*)
    SoElement::getConstElement(state, classStackIndex);
  return elem->isEnabled;
}

//! FIXME: write doc.

void
SoGLPolygonStippleElement::push(SoState * state)
{
  SoGLPolygonStippleElement * prev = (SoGLPolygonStippleElement*)
    this->getNextInStack();
  this->currentEnabled = prev->currentEnabled;
  this->currentPattern = prev->currentPattern;
}

//! FIXME: write doc.

void
SoGLPolygonStippleElement::pop(SoState * state,
                               const SoElement * prevTopElement)
{
  SoGLPolygonStippleElement * prev = (SoGLPolygonStippleElement*)
    prevTopElement;

  this->currentEnabled = prev->currentEnabled;
  this->currentPattern = prev->currentPattern;
}

//! FIXME: write doc.

SbBool
SoGLPolygonStippleElement::matches(const SoElement *elt) const
{
  SoGLPolygonStippleElement *elem = (SoGLPolygonStippleElement*) elt;

  return (this->isEnabled == elem->isEnabled &&
          this->pattern == elem->pattern);
}

//! FIXME: write doc.

SoElement *
SoGLPolygonStippleElement::copyMatchInfo() const
{
  SoGLPolygonStippleElement * elem =
    (SoGLPolygonStippleElement *)(getTypeId().createInstance());

  elem->isEnabled = this->isEnabled;
  elem->pattern = this->pattern;
  return elem;
}

//! FIXME: write doc.

void
SoGLPolygonStippleElement::print(FILE *fp) const
{
  fprintf(fp, "SoGLPolygonStippleElement: %p\n", this);
}

//! FIXME: write doc.

void
SoGLPolygonStippleElement::lazyEvaluate(void) const
{
  if (this->currentEnabled != this->isEnabled ||
      this->currentPattern != this->pattern) {

    SoGLPolygonStippleElement *elem =
      (SoGLPolygonStippleElement*) this;
    elem->currentEnabled = this->isEnabled;
    elem->currentPattern = this->pattern;
    elem->updategl();
  }
}

// doc in parent
SbBool
SoGLPolygonStippleElement::isLazy(void) const
{
  return TRUE;
}

//! FIXME: write doc.

void
SoGLPolygonStippleElement::updategl()
{
  if (currentEnabled && currentPattern >= 0) {
    glEnable(GL_POLYGON_STIPPLE);
    glPolygonStipple(patterns[this->currentPattern]);
  }
  else glDisable(GL_POLYGON_STIPPLE);
}
