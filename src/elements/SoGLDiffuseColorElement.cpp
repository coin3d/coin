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
  \class SoGLDiffuseColorElement Inventor/elements/SoGLDiffuseColorElement.h
  \brief The SoGLDiffuseColorElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoGLDiffuseColorElement.h>

#include <Inventor/SbName.h>
#include <Inventor/SbColor.h>
#include <GL/gl.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#include <assert.h>

#define DIV255 0.0039215686f

//$ BEGIN TEMPLATE ElementSource( SoGLDiffuseColorElement )

/*!
  \var SoGLDiffuseColorElement::classTypeId

  This is the static class type identifier for the
  SoGLDiffuseColorElement class.
*/

SoType SoGLDiffuseColorElement::classTypeId = SoType::badType();

/*!
  This method returns the SoType object for the element class of
  the instance.
*/
SoType
SoGLDiffuseColorElement::getClassTypeId(void)
{
  return SoGLDiffuseColorElement::classTypeId;
}

/*!
  This static method creates an object instance of the SoGLDiffuseColorElement class.
*/
void *
SoGLDiffuseColorElement::createInstance(void)
{
  return (void *) new SoGLDiffuseColorElement;
}

/*!
  \var SoGLDiffuseColorElement::classStackIndex

  This is the static state stack index for the
  SoGLDiffuseColorElement class.
*/
int SoGLDiffuseColorElement::classStackIndex;

/*!
  This static method returns the state stack index for the SoGLDiffuseColorElement class.
*/
int
SoGLDiffuseColorElement::getClassStackIndex(void)
{
  return SoGLDiffuseColorElement::classStackIndex;
}
//$ END TEMPLATE ElementSource

/*!
  This static method initializes static data for the
  SoGLDiffuseColorElement class.
*/

void
SoGLDiffuseColorElement::initClass()
{
//$ BEGIN TEMPLATE InitElementSource( SoGLDiffuseColorElement )
  assert(SoGLDiffuseColorElement::classTypeId == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoGLDiffuseColorElement::classTypeId = SoType::createType(
    inherited::getClassTypeId(),
    "SoGLDiffuseColorElement",
    &SoGLDiffuseColorElement::createInstance);

  if (inherited::classStackIndex < 0) {
    SoGLDiffuseColorElement::classStackIndex =
      createStackIndex( SoGLDiffuseColorElement::classTypeId );
  } else {
    SoGLDiffuseColorElement::classStackIndex =
      inherited::classStackIndex;
  }
//$ END TEMPLATE InitElementSource
} // initClass()

/*!
  This static method cleans up static data for the
  SoGLDiffuseColorElement class.
*/

void 
SoGLDiffuseColorElement::cleanClass()
{
//$ BEGIN TEMPLATE CleanElementSource( SoGLDiffuseColorElement )
//$ END TEMPLATE CleanElementSource
} // cleanClass()

/*!
  A constructor.  Can't be used directly.

  \sa void * SoGLDiffuseColorElement::createInstance( void )
*/

SoGLDiffuseColorElement::SoGLDiffuseColorElement()
{
  setTypeId(classTypeId);
  setStackIndex(classStackIndex);
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
		    currentPacked);
      this->updategl();
    }
  }
  else if (packedColors && packedColors[realindex] != this->currentPacked) {
    this->currentPacked = packedColors[realindex];
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
  send(index, this->current[3]);
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

