/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
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
  height), the number of bytes used for describing each pixel (number
  of components) and an associated pixel buffer. The size of the pixel
  buffer will be width*height*components.

  For texture maps, the components / bytes-per-pixel setting
  translates as follows: use 1 for a grayscale imagemap, 2 for
  grayscale + opacity (i.e. alpha value), 3 for RGB (1 byte each for
  red, green and blue) and 4 components means 3 bytes for RGB + 1 byte
  opacity value (aka RGBA).

  This field is serializable into the Inventor / Coin file format in
  the following manner:

  \code
  FIELDNAME X Y C 0xRRGGBBAA 0xRRGGBBAA ...
  \endcode

  "X" and "Y" are the image dimensions along the given axes, "C" is
  the number of components in the image. The number of 0xRRGGBBAA
  pixel color specifications needs to equal the exact number of
  pixels, which of course is given by X*Y. Each part of the pixel
  color value is in the range 0x00 to 0xff (hexadecimal, 0 to 255
  decimal).

  For 3-component images, the pixel-format is 0xXXRRGGBB, where the
  byte in the pixel color value marked as "XX" is ignored and can be
  left out.

  For 2-component images, the pixel-format is 0xXXXXGGAA, where the
  bytes in the pixel color values marked as "XX" are ignored and can
  be left out. "GG" is the part which gives a grayscale value and "AA"
  is for opacity.
  
  For 1-component images, the pixel-format is 0xXXXXXXGG, where the
  bytes in the pixel color values marked as "XX" are ignored and can
  be left out.

  The pixels are read as being ordered in rows along X (width) and
  columns along Y (height, bottom to top).

  Here's a simple example of the file format serialization, for a 2x2
  RGB-image inside an SoTexture2 node, as mapped unto an SoCube:

  \code
  Texture2 {
     image 2 2 3

     0xffffff 0x00ff00   # white   green
     0xff0000 0xffff00   #  red    yellow
  }

  Complexity { textureQuality 0.1 }   # set low to avoid smoothing
  Cube { }
  \endcode

  The mini-scenegraph above results in the following mapping on the
  cube:<br>

  <center>
  <img src="http://doc.coin3d.org/images/Coin/fields/sosfimage.png">
  </center>

  The cube has only been \e slightly rotated, so as you can see from
  the snapshot, the Y-rows are mapped from bottom to top, while the
  X-column pixels are mapped unto the cube from left to right.

  \sa SoTexture2, SoSFImage3
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

  The image dimensions is given by the \a size argument, and the \a nc
  argument specifies the number of bytes-pr-pixel. A 24-bit RGB image
  would for instance have an \a nc equal to 3.
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
