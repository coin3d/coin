/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
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
  means a grayscale imagemap, 2 bytes is grayscale + opacity
  (i.e. alpha value), 3 bytes is 8-bit red + 8-bit green + 8-bit blue
  (aka RGB) and 4 bytes per pixel means 3 bytes for RGB + 1 byte
  opacity value (aka RGBA).

  \sa SoTexture2
*/

#include <Inventor/fields/SoSFImage.h>
#include <Inventor/fields/SoSubFieldP.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/errors/SoReadError.h>
#include <Inventor/SbImage.h>


PRIVATE_TYPEID_SOURCE(SoSFImage);
PRIVATE_EQUALITY_SOURCE(SoSFImage);


// (Declarations hidden in macro in SoSFImage.h, so don't use Doxygen
// commenting.)
#ifndef DOXYGEN_SKIP_THIS

/* Constructor, initializes fields to represent an empty image. */
SoSFImage::SoSFImage(void)
  : image(new SbImage)
{
}

/* Free all resources associated with the image. */
SoSFImage::~SoSFImage()
{
  delete this->image;
}

/* Copy the image of \a field into this field. */
const SoSFImage &
SoSFImage::operator=(const SoSFImage & field)
{
  int nc = 0;
  SbVec2s size(0,0);
  unsigned char * bytes = field.image->getValue(size, nc);

  this->setValue(size, nc, bytes);
  return *this;
}

#endif // DOXYGEN_SKIP_THIS


// Override from parent class.
void
SoSFImage::initClass(void)
{
  SO_SFIELD_INTERNAL_INIT_CLASS(SoSFImage);
}

SbBool
SoSFImage::readValue(SoInput * in)
{
  SbVec2s size;
  int nc;
  if (!in->read(size[0]) || !in->read(size[1]) ||
      !in->read(nc)) {
    SoReadError::post(in, "Premature end of file");
    return FALSE;
  }

  // Note: empty images (dimensions 0x0x0) are allowed.

  if (size[0] < 0 || size[1] < 0 || nc < 0 || nc > 4) {
    SoReadError::post(in, "Invalid image specification %dx%dx%d",
                      size[0], size[1], nc);
    return FALSE;
  }

  int buffersize = int(size[0]) * int(size[1]) * nc;

  if (buffersize == 0 &&
      (size[0] != 0 || size[1] != 0 || nc != 0)) {
    SoReadError::post(in, "Invalid image specification %dx%dx%d",
                      size[0], size[1], nc);
    return FALSE;
  }

#if COIN_DEBUG && 0 // debug
  SoDebugError::postInfo("SoSFImage::readValue", "image dimensions: %dx%dx%d",
                         size[0], size[1], nc);
#endif // debug

  if (!buffersize) {
    this->image->setValue(SbVec2s(0,0), 0, NULL);
    this->valueChanged();
    return TRUE;
  }

  // allocate image data and get new pointer back
  this->image->setValue(size, nc, NULL);
  unsigned char * pixblock = this->image->getValue(size, nc);

  // The binary image format of 2.1 and later tries to be less
  // wasteful when storing images.
  if (in->isBinary() && in->getIVVersion() >= 2.1f) {
    if (!in->readBinaryArray(pixblock, buffersize)) {
      SoReadError::post(in, "Premature end of file");
      return FALSE;
    }
  }
  else {
    int byte = 0;
    int numpixels = int(size[0]) * int(size[1]);
    for (int i = 0; i < numpixels; i++) {
      uint32_t l;
      if (!in->read(l)) {
        SoReadError::post(in, "Premature end of file");
        return FALSE;
      }
      for (int j = 0; j < nc; j++) {
        pixblock[byte++] =
          (unsigned char) ((l >> (8 * (nc-j-1))) & 0xFF);
      }
    }
  }

  this->valueChanged();
  return TRUE;
}

void
SoSFImage::writeValue(SoOutput * out) const
{
  int nc;
  SbVec2s size;
  unsigned char * pixblock = this->image->getValue(size, nc);

  out->write(size[0]);
  if (!out->isBinary()) out->write(' ');
  out->write(size[1]);
  if (!out->isBinary()) out->write(' ');
  out->write(nc);

  if (out->isBinary()) {
    int buffersize = int(size[0]) * int(size[1]) * nc;
    if (buffersize) { // in case of an empty image
      out->writeBinaryArray(pixblock, buffersize);
      int padsize = ((buffersize + 3) / 4) * 4 - buffersize;
      if (padsize) {
        unsigned char pads[3] = {'\0','\0','\0'};
        out->writeBinaryArray(pads, padsize);
      }
    }
  }
  else {
    out->write('\n');
    out->indent();

    int numpixels = int(size[0]) * int(size[1]);
    for (int i = 0; i < numpixels; i++) {
      uint32_t data = 0;
      for (int j = 0; j < nc; j++) {
        if (j) data <<= 8;
        data |= (uint32_t)(pixblock[i * nc + j]);
      }
      out->write(data);
      if (((i+1)%8 == 0) && (i+1 != numpixels)) {
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
  return (*this->image) == (*field.image);
}


/*!
  Return pixel buffer, set \a size to contain the image dimensions and
  \a nc to the number of components in the image.
*/
const unsigned char *
SoSFImage::getValue(SbVec2s & size, int & nc) const
{
  return this->image->getValue(size, nc);
}

/*!
  Initialize this field to \a size and \a nc.

  If \a bytes is not \c NULL, the image data is copied from \a bytes
  into this field.  If \a bytes is \c NULL, the image data is cleared
  by setting all bytes to 0 (note that the behavior on passing a \c
  NULL pointer is specific for Coin, Open Inventor will crash if you
  try it).
*/
void
SoSFImage::setValue(const SbVec2s & size, const int nc,
                    const unsigned char * bytes)
{
  this->image->setValue(size, nc, bytes);
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
  return this->image->getValue(size, nc);
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
