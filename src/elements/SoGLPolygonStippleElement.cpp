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
  \class SoGLPolygonStippleElement Inventor/elements/SoGLPolygonStippleElement.h
  \brief The SoGLPolygonStippleElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoGLPolygonStippleElement.h>

#include <Inventor/SbName.h>

#ifdef _WIN32
#include <windows.h>
#endif // !_WIN32

#include <GL/gl.h>
#include <assert.h>

unsigned char SoGLPolygonStippleElement::patterns[64 + 1][32 * 4];

//$ BEGIN TEMPLATE ElementSource( SoGLPolygonStippleElement )

/*!
  \var SoGLPolygonStippleElement::classTypeId

  This is the static class type identifier for the
  SoGLPolygonStippleElement class.
*/

SoType SoGLPolygonStippleElement::classTypeId = SoType::badType();

/*!
  This method returns the SoType object for the element class of
  the instance.
*/
SoType
SoGLPolygonStippleElement::getClassTypeId(void)
{
  return SoGLPolygonStippleElement::classTypeId;
}

/*!
  This static method creates an object instance of the SoGLPolygonStippleElement class.
*/
void *
SoGLPolygonStippleElement::createInstance(void)
{
  return (void *) new SoGLPolygonStippleElement;
}

/*!
  \var SoGLPolygonStippleElement::classStackIndex

  This is the static state stack index for the
  SoGLPolygonStippleElement class.
*/
int SoGLPolygonStippleElement::classStackIndex;

/*!
  This static method returns the state stack index for the SoGLPolygonStippleElement class.
*/
int
SoGLPolygonStippleElement::getClassStackIndex(void)
{
  return SoGLPolygonStippleElement::classStackIndex;
}
//$ END TEMPLATE ElementSource

// Some data and functions to create Bayer dither 
// matrices (used for screen door transparency)

static unsigned int two_by_two[] = {0, 2, 3, 1};

//
// Used to generate a matrix twice the size of the input   
//
static void generate_next_matrix(unsigned int *old, int oldsize, 
				 unsigned int *matrix)
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
// Warning: only binary sizes!!   
//
static void make_dither_matrix(uint32_t *ptr, int size)
{
  int currsize = 2;

  unsigned int *currmatrix = two_by_two;
  unsigned int *nextmatrix = NULL;
  int nextsize;

  while (currsize < size) {
    nextsize = currsize << 1;
    nextmatrix = new unsigned int[nextsize*nextsize];
    generate_next_matrix(currmatrix, currsize, nextmatrix);
    if (currmatrix != two_by_two) delete currmatrix;
    currmatrix = nextmatrix;
    currsize = nextsize;
  }
  // copy matrix
  int i;
  for (i = 0; i < size*size; i++)
    ptr[i] = currmatrix[i];
  
  if (currmatrix != two_by_two) delete currmatrix;
}

//
// Sets a bit bitnr bits from ptr   
//
static void set_bit(int bitnr, uint32_t *ptr)
{
  int byte = bitnr / 32;
  int bit = bitnr % 32;
  
  int mask = 0x80000000 >> bit;
  
  ptr[byte] |= mask;
}

//
// Create a bitmap from a 32x32 matrix
//
static void create_matrix_bitmap(int intensity, uint32_t *bitmap, 
				 uint32_t *matrix, int size)
{
  int cnt = 0;
  int i,j;
  for (i = 0; i < 32; i++) bitmap[i] = 0;
  for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      if ( matrix[i*size+j] > (unsigned int) intensity) {
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
//$ BEGIN TEMPLATE InitElementSource( SoGLPolygonStippleElement )
  assert(SoGLPolygonStippleElement::classTypeId == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoGLPolygonStippleElement::classTypeId = SoType::createType(
    inherited::getClassTypeId(),
    "SoGLPolygonStippleElement",
    &SoGLPolygonStippleElement::createInstance);

  if (inherited::classStackIndex < 0) {
    SoGLPolygonStippleElement::classStackIndex =
      createStackIndex( SoGLPolygonStippleElement::classTypeId );
  } else {
    SoGLPolygonStippleElement::classStackIndex =
      inherited::classStackIndex;
  }
//$ END TEMPLATE InitElementSource

  // create patterns
  int i;
  unsigned int matrix[32*32];
  make_dither_matrix(matrix, 32);
  for (i = 0; i <= 64; i++) {
    create_matrix_bitmap(((32*32)*i)/64, 
                         (uint32_t*)&patterns[i][0], matrix, 32);
  }
}

/*!
  This static method cleans up static data for the
  SoGLPolygonStippleElement class.
*/

void 
SoGLPolygonStippleElement::cleanClass()
{
//$ BEGIN TEMPLATE CleanElementSource( SoGLPolygonStippleElement )
//$ END TEMPLATE CleanElementSource
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoGLPolygonStippleElement::createInstance( void )
*/

SoGLPolygonStippleElement::SoGLPolygonStippleElement()
{
  setTypeId(classTypeId);
  setStackIndex(classStackIndex);
}

/*!
  The destructor.
*/

SoGLPolygonStippleElement::~SoGLPolygonStippleElement()
{
}

//! FIXME: write doc.

void 
SoGLPolygonStippleElement::init(SoState *state)
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

  elem->isEnabled = onoff;
}

//! FIXME: write doc.

void 
SoGLPolygonStippleElement::setTransparency(SoState * const state,
				    const float transparency)
{
  SoGLPolygonStippleElement *elem = (SoGLPolygonStippleElement*)
    SoElement::getElement(state, classStackIndex);
  
  int stipplenum = (int)(64.0f*(transparency)); //0-64
  
  // FIXME: print a warning?
  if (stipplenum < 0) stipplenum = 0;
  if (stipplenum > 64) stipplenum = 64;
  elem->pattern = stipplenum;
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
SoGLPolygonStippleElement::push(SoState *state)
{
  SoGLPolygonStippleElement *elem = (SoGLPolygonStippleElement*)
    this->next;
  elem->currentEnabled = this->currentEnabled;
  elem->currentPattern = this->currentPattern;
}

//! FIXME: write doc.

void 
SoGLPolygonStippleElement::pop(SoState *state, const SoElement *prevTopElement)
{
  SoGLPolygonStippleElement *prev = (SoGLPolygonStippleElement*)
    prevTopElement;

  prev->currentEnabled = this->currentEnabled;
  prev->currentPattern = this->currentPattern;
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
SoGLPolygonStippleElement::evaluate() const
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

