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
  \class SoSFImage SoSFImage.h Inventor/fields/SoSFImage.h
  \brief The SoSFImage class ...
  \ingroup fields

  FIXME: write class doc
*/

/*¡
  Wouldn't it make it better for the code in SoSFImage if there existed
  an ``SbImage'' class? SbImage could probably also be used other places,
  and could perhaps be a sole interface against the imagelib?

  Discuss with pederb.

  19990620 mortene.
 */

#include <Inventor/fields/SoSFImage.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/SbName.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

// *************************************************************************

//$ BEGIN TEMPLATE FieldId( SoSFImage )

SoType SoSFImage::classTypeId = SoType::badType();

/*!
  Virtual method which returns the type identifier for an object.

  \sa getClassTypeId()
*/
SoType
SoSFImage::getTypeId(void) const
{
  return SoSFImage::classTypeId;
}

/*!
  Returns a unique type identifier for the SoSFImage class.

  \sa getTypeId(), SoType
 */
SoType
SoSFImage::getClassTypeId(void)
{
  return SoSFImage::classTypeId;
}

/*!
  Constructs and returns a new instance of the SoSFImage class.
*/
void *
SoSFImage::createInstance(void)
{
  return new SoSFImage;
}
//$ END TEMPLATE FieldId
//$ BEGIN TEMPLATE FieldEqSame( SoSFImage )
/*!
  Copy all data from \a field into this object. \a field \e must
  be of the same type as the field we are copying into.
*/
void
SoSFImage::copyFrom(const SoField & field)
{
#if 0 // COIN_DEBUG
  // Calling field.getTypeId() here fails when "this" is connected to "field"
  // and "field" is destructed. The error message is "pure virtual method
  // called" with egcs 1.0.2 under Linux. 19990713 mortene.
  if (field.getTypeId() != this->getTypeId()) {
    SoDebugError::postWarning("SoSFImage::copyFrom",
                              "not of the same type: (this) '%s' (from) '%s'",
                              this->getTypeId().getName().getString(),
                              field.getTypeId().getName().getString());
    return;
  }
#endif // COIN_DEBUG

  this->operator=((const SoSFImage &)field);
}

/*!
  Tests \a field against this field for equality. Returns \a FALSE if they
  are not of the same type, or if they do not contain the same data.
*/
SbBool
SoSFImage::isSame(const SoField & field) const
{
  if (field.getTypeId() != this->getTypeId()) return FALSE;
  return this->operator==((const SoSFImage &) field);
}
//$ END TEMPLATE FieldEqSame

// *************************************************************************

/*!
  Constructor.
*/
SoSFImage::SoSFImage(void)
{
  this->imgdim[0] = this->imgdim[1] = 0;
  this->bytedepth = 0;
  this->pixblock = NULL;
}

/*!
  Destructor.
*/
SoSFImage::~SoSFImage()
{
  delete[] this->pixblock;
}

/*!
  FIXME: write function documentation
*/
const SoSFImage &
SoSFImage::operator = (const SoSFImage & field)
{
  this->setValue(field.imgdim, field.bytedepth, field.pixblock);
  return *this;
}

/*!
  Does initialization common for all objects of the
  SoSFImage class. This includes setting up the
  type system, among other things.
*/
void
SoSFImage::initClass(void)
{
//$ BEGIN TEMPLATE FieldInitClass( SFImage )
  // Make sure we only initialize once.
  assert(SoSFImage::classTypeId == SoType::badType());
  // Make sure superclass has been initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoSFImage::classTypeId =
    SoType::createType(SoSFImage::getClassTypeId(),
                       "SFImage", &SoSFImage::createInstance);
//$ END TEMPLATE FieldInitClass
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoSFImage::cleanClass(void)
{
}

SbBool
SoSFImage::readValue(SoInput * in)
{
  assert(!in->isBinary() && "FIXME: not implemented");

  if (! in->read(this->imgdim[0]) ||
      ! in->read(this->imgdim[1]) ||
      ! in->read(this->bytedepth))
    return FALSE;
    
  if (this->imgdim[0]<0 || this->imgdim[1]<0) return FALSE;
  if ((this->bytedepth<1 || this->bytedepth>4) &&
      (this->imgdim[0]>0 || this->imgdim[1]>0))
    return FALSE;

  delete [] this->pixblock;
  this->pixblock =
    new unsigned char[this->imgdim[0] * this->imgdim[1] * this->bytedepth];

  int byte = 0;
  for (int i = 0; i < this->imgdim[0] * this->imgdim[1]; i++) {
    uint32_t l;
    if (! in->read(l))
      return FALSE;
    for (int j = 0; j < this->bytedepth; j++)
      this->pixblock[byte++] =
	(unsigned char) ((l >> (8 * (this->bytedepth-j-1))) & 0xFF);
  }
  return TRUE;
}

/*!
  FIXME: write function documentation
*/
int
SoSFImage::operator == (const SoSFImage & field) const
{
  if(this->imgdim[0] != field.imgdim[0]) return FALSE;
  if(this->imgdim[1] != field.imgdim[1]) return FALSE;
  if(this->bytedepth != field.bytedepth) return FALSE;

  int bytesize = this->imgdim[0] * this->imgdim[1] * this->bytedepth;
  for(int i=0; i < bytesize; i++) {
    if(this->pixblock[i] != field.pixblock[i]) return FALSE;
  }
  return TRUE;
}


/*!
  FIXME: write function documentation
*/
const unsigned char *
SoSFImage::getValue(SbVec2s & size, int & nc) const
{
  size = this->imgdim;
  nc = this->bytedepth;
  return this->pixblock;
}

/*!
  FIXME: write function documentation
*/
void
SoSFImage::setValue(const SbVec2s & size, const int nc,
		    const unsigned char * const bytes)
{

  unsigned char * newblock =
    new unsigned char[size[0] * size[1] * nc];

  delete[] this->pixblock;
  this->pixblock = newblock;
  this->imgdim = size;
  this->bytedepth = nc;
  memcpy(this->pixblock, bytes,
	 this->imgdim[0] * this->imgdim[1] * this->bytedepth);

  this->valueChanged();
}

/*!
  FIXME: write function documentation
*/
unsigned char*
SoSFImage::startEditing(SbVec2s & size, int & nc)
{
  size = this->imgdim;
  nc = this->bytedepth;
  return this->pixblock;
}

/*!
  FIXME: write function documentation
*/
void
SoSFImage::finishEditing(void)
{
  this->valueChanged();
}

void
SoSFImage::writeValue(SoOutput * out) const
{
  assert(!out->isBinary() && "FIXME: not implemented");

  out->write(this->imgdim[0]);
  if(!out->isBinary()) out->write(' ');
  out->write(this->imgdim[1]);
  if(!out->isBinary()) out->write(' ');
  out->write(this->bytedepth);
  // FIXME: ok on mac/win32? 19980910 mortene.
  if(!out->isBinary()) out->write('\n');

  if(!out->isBinary()) out->indent();

  for(int i=0; i < this->imgdim[0] * this->imgdim[1]; i++) {
    uint32_t data = 0;
    for(int j=0; j < this->bytedepth; j++) {
      if(j) data <<= 8;
      data |= (uint32_t)(this->pixblock[i * this->bytedepth + j]);
    }
    out->write(data);
    if(((i+1)%8 == 0) && (i+1 != this->imgdim[0] * this->imgdim[1])) {
      out->write('\n'); // FIXME: ok on win32/mac? 19980910 mortene.
      out->indent();
    }
    else {
      if(!out->isBinary()) out->write(' ');
    }
  }
}
