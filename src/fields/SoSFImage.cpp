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
  \class SoSFImage SoSFImage.h Inventor/fields/SoSFImage.h
  \brief The SoSFImage class is used to store pixel images.
  \ingroup fields

  The SoSFImage class provides storage for inline 2D image
  maps. Images in Coin are mainly used for texture mapping support.

  SoSFImage instances can be exported and imported as any other field
  class in Coin.

  The components of an SoSFImage is: its image dimensions (width and
  height), the number of bytes used for describing each pixel and an
  associated pixel buffer. The size of the pixel buffer will be
  width * height * bytesperpixel.

  For texture maps, the bytes per pixel setting translates to: 1 byte
  means a grayscale imagemap, 2 bytes is grayscale + translucency, 3
  bytes is 8-bit red + 8-bit green + 8-bit blue (aka RGB) and 4 bytes
  per pixel means 3 bytes for RGB + 1 byte translucency value.

  \sa SoTexture2
*/

/*¡
  Wouldn't it make it better for the code in SoSFImage if there existed
  an ``SbImage'' class? SbImage could probably also be used other places,
  and could perhaps be the sole interface against the imagelib?

  Discuss with pederb.

  19990620 mortene.
 */

#include <Inventor/fields/SoSFImage.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/errors/SoReadError.h>



PRIVATE_TYPEID_SOURCE(SoSFImage);
PRIVATE_EQUALITY_SOURCE(SoSFImage);


/*!
  Constructor, initialized field to be empty.
*/
SoSFImage::SoSFImage(void)
{
  this->imgdim[0] = this->imgdim[1] = 0;
  this->bytedepth = 0;
  this->pixblock = NULL;
}

/*!
  Destructor, free resources associated with the image.
*/
SoSFImage::~SoSFImage()
{
  delete[] this->pixblock;
}

/*!
  Copy the image of \a field into this field.
*/
const SoSFImage &
SoSFImage::operator=(const SoSFImage & field)
{
  this->setValue(field.imgdim, field.bytedepth, field.pixblock);
  return *this;
}

// Override from parent class.
void
SoSFImage::initClass(void)
{
  SO_SFIELD_INTERNAL_INIT_CLASS(SoSFImage);
}

SbBool
SoSFImage::readValue(SoInput * in)
{
  if (!in->read(this->imgdim[0]) || !in->read(this->imgdim[1]) ||
      !in->read(this->bytedepth)) {
    SoReadError::post(in, "Premature end of file");
    return FALSE;
  }

  // Note: empty images (dimensions 0x0x0) are allowed.

  if (this->imgdim[0] < 0 || this->imgdim[1] < 0 ||
      this->bytedepth < 0 || this->bytedepth > 4) {
    SoReadError::post(in, "Invalid image specification %dx%dx%d",
                      this->imgdim[0], this->imgdim[1], this->bytedepth);
    return FALSE;
  }

  int buffersize = this->imgdim[0] * this->imgdim[1] * this->bytedepth;

  if (buffersize == 0 &&
      (this->imgdim[0] != 0 || this->imgdim[1] != 0 || this->bytedepth != 0)) {
    SoReadError::post(in, "Invalid image specification %dx%dx%d",
                      this->imgdim[0], this->imgdim[1], this->bytedepth);
    return FALSE;
  }


#if COIN_DEBUG && 0 // debug
  SoDebugError::postInfo("SoSFImage::readValue", "image dimensions: %dx%dx%d",
                         this->imgdim[0], this->imgdim[1], this->bytedepth);
#endif // debug

  delete [] this->pixblock;
  this->pixblock = NULL;
  if (!buffersize) {
    this->valueChanged();
    return TRUE;
  }

  // Align data -- since this is what is done during import of binary
  // files with version number 2.0 (and older?).
  buffersize = ((buffersize + 3) / 4) * 4;

  this->pixblock = new unsigned char[buffersize];

  // The binary image format of 2.1 and later tries to be less
  // wasteful when storing images.
  if (in->isBinary() && in->getIVVersion() >= 2.1f) {
    if (!in->readBinaryArray(this->pixblock, buffersize)) {
      SoReadError::post(in, "Premature end of file");
      return FALSE;
    }
  }
  else {
    int byte = 0;
    for (int i = 0; i < this->imgdim[0] * this->imgdim[1]; i++) {
      uint32_t l;
      if (!in->read(l)) {
        SoReadError::post(in, "Premature end of file");
        return FALSE;
      }
      for (int j = 0; j < this->bytedepth; j++) {
        this->pixblock[byte++] =
          (unsigned char) ((l >> (8 * (this->bytedepth-j-1))) & 0xFF);
      }
    }
  }

  this->valueChanged();
  return TRUE;
}

void
SoSFImage::writeValue(SoOutput * out) const
{
  out->write(this->imgdim[0]);
  if (!out->isBinary()) out->write(' ');
  out->write(this->imgdim[1]);
  if (!out->isBinary()) out->write(' ');
  out->write(this->bytedepth);

  if (out->isBinary()) {
    int buffersize = this->imgdim[0] * this->imgdim[1] * this->bytedepth;
    if (buffersize) { // in case of an empty image
      out->writeBinaryArray(this->pixblock, buffersize);
      int padsize = ((buffersize + 3) / 4) * 4 - buffersize;
      if (padsize) {
        unsigned char pads[3] = {'\0','\0','\0'};
        out->writeBinaryArray(this->pixblock, padsize);
      }
    }
  }
  else {
    out->write('\n');
    out->indent();

    for (int i=0; i < this->imgdim[0] * this->imgdim[1]; i++) {
      uint32_t data = 0;
      for (int j=0; j < this->bytedepth; j++) {
        if (j) data <<= 8;
        data |= (uint32_t)(this->pixblock[i * this->bytedepth + j]);
      }
      out->write(data);
      if (((i+1)%8 == 0) && (i+1 != this->imgdim[0] * this->imgdim[1])) {
        out->write('\n');
        out->indent();
      }
      else {
        out->write(' ');
      }
    }
  }
}


/*!
  \fn int SoSFImage::operator!=(const SoSFImage & field) const
  Compare image of \a field with the image in this field and
  return \c FALSE if they are equal.
*/

/*!
  Compare image of \a field with the image in this field and
  return \c TRUE if they are equal.
*/
int
SoSFImage::operator==(const SoSFImage & field) const
{
  if (this->imgdim[0] != field.imgdim[0]) return FALSE;
  if (this->imgdim[1] != field.imgdim[1]) return FALSE;
  if (this->bytedepth != field.bytedepth) return FALSE;

  int bytesize = this->imgdim[0] * this->imgdim[1] * this->bytedepth;
  for (int i=0; i < bytesize; i++) {
    if (this->pixblock[i] != field.pixblock[i]) return FALSE;
  }
  return TRUE;
}


/*!
  Return pixel buffer, set \a size to contain the image dimensions and
  \a nc to the number of components in the image.
*/
const unsigned char *
SoSFImage::getValue(SbVec2s & size, int & nc) const
{
  size = this->imgdim;
  nc = this->bytedepth;
  return this->pixblock;
}

/*!
  Copy the image data from \a bytes with \a size and \a nc
  number of components into this field.
*/
void
SoSFImage::setValue(const SbVec2s & size, const int nc,
                    const unsigned char * bytes)
{
  delete[] this->pixblock;
  this->pixblock = NULL;

  int buffersize = size[0] * size[1] * nc;

  if (buffersize) { // images can be empty

    // Align buffers because the binary file format has the data aligned
    // (simplifies export code).
    buffersize = ((buffersize + 3) / 4) * 4;
    unsigned char * newblock = new unsigned char[buffersize];
    this->pixblock = newblock;

    this->imgdim = size;
    this->bytedepth = nc;
    (void)memcpy(this->pixblock, bytes,
                 this->imgdim[0] * this->imgdim[1] * this->bytedepth);
  }

  this->valueChanged();
}

/*!
  Return pixel buffer, set \a size to contain the image dimensions and
  \a nc to the number of components in the image.

  The field's container will not be notified about the changes
  until you call finishEditing().
*/
unsigned char *
SoSFImage::startEditing(SbVec2s & size, int & nc)
{
  size = this->imgdim;
  nc = this->bytedepth;
  return this->pixblock;
}

/*!
  Notify the field's auditors that the image data has been
  modified.
*/
void
SoSFImage::finishEditing(void)
{
  this->valueChanged();
}
