/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
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
  \class SbImage SbImage.h Inventor/SbImage.h
  \brief The SbImage class is an abstract datatype for 2D images.
  \ingroup base

  This class is a Coin extension to the original Open Inventor API.
*/

// FIXME: this class could be used to handle image reusage, since it's
// quite common that the same image is used several times in a scene
// and for different contexts. The API should stay the same though.
// 20001026 mortene (original comment by pederb).


#include <Inventor/SbImage.h>
#include <Inventor/SbVec2s.h>
#include <Inventor/SbString.h>
#include <string.h>
#include <stdlib.h>
#include <src/misc/simage_wrapper.h>
#include <Inventor/SoInput.h> // for SoInput::searchForFile()
#include <Inventor/lists/SbStringList.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

//////////////////////////////////////////////////////////////////////////

/*!
  Default constructor.
*/
SbImage::SbImage(void)
  : bytes(NULL),
    size(0,0),
    bpp(0)
{
}

/*!
  Constructor which sets the data using setValue().
  \sa setValue()
*/
SbImage::SbImage(const unsigned char * bytes,
                 const SbVec2s & size, const int bytesperpixel)
  : bytes(NULL)
{
  this->setValue(size, bytesperpixel, bytes);
}

/*!
  Destructor.
*/
SbImage::~SbImage(void)
{
  delete [] this->bytes;
}

/*!
  Sets the image to \a size and \a bytesperpixel. If \a bytes !=
  NULL, data is copied from \a bytes into this class' image data. If
  \a bytes == NULL, the image data is left uninitialized.

  The image data will always be allocated in multiples of four. This
  means that if you set an image with size == (1,1) and bytesperpixel
  == 1, four bytes will be allocated to hold the data. This is mainly
  done to simplify the export code in SoSFImage and normally you'll
  not have to worry about this feature.
*/
void
SbImage::setValue(const SbVec2s & size, const int bytesperpixel,
                  const unsigned char * bytes)
{
  if (this->bytes) {
    // check for special case where we don't have to reallocate
    if (bytes && (size == this->size) && (bytesperpixel == this->bpp)) {
      memcpy(this->bytes, bytes, int(size[0])*int(size[1])*bytesperpixel);
      return;
    }
    delete [] this->bytes;
    this->bytes = NULL;
  }
  this->size = size;
  this->bpp = bytesperpixel;
  int buffersize = int(size[0]) * int(size[1]) * bytesperpixel;
  if (buffersize) {
    // Align buffers because the binary file format has the data aligned
    // (simplifies export code in SoSFImage).
    buffersize = ((buffersize + 3) / 4) * 4;
    this->bytes = new unsigned char[buffersize];

    if (bytes) {
      // Important: don't copy buffersize num bytes here!
      (void)memcpy(this->bytes, bytes,
                   int(size[0]) * int(size[1]) * bytesperpixel);
    }
  }
}

/*!
  Returns the image data.
*/
unsigned char *
SbImage::getValue(SbVec2s & size, int & bytesperpixel) const
{
  size = this->size;
  bytesperpixel = bpp;
  return this->bytes;
}

/*!
  Given a \a basename for a file and and array of directories to
  search (in \a dirlist, of length \a numdirs), returns the full name
  of the file found.

  In addition to looking at the root of each directory in \a dirlist,
  we also look into the subdirectories \e texture/, \e textures/, \e
  images/, \e pics/ and \e pictures/ of each \a dirlist directory.

  If no file matching \a basename could be found, returns an empty
  string.
*/
SbString
SbImage::searchForFile(const SbString & basename,
                       const SbString * const * dirlist, const int numdirs)
{
  int i;
  SbStringList directories;
  SbStringList subdirectories;
  
  for (i = 0; i < numdirs; i++) {
    directories.append((SbString*) dirlist[i]);
  }
  subdirectories.append(new SbString("texture"));
  subdirectories.append(new SbString("textures"));
  subdirectories.append(new SbString("images"));
  subdirectories.append(new SbString("pics"));
  subdirectories.append(new SbString("pictures"));
  
  SbString ret = SoInput::searchForFile(basename, directories, subdirectories);
  for (i = 0; i < subdirectories.getLength(); i++) {
    delete subdirectories[i];
  }
  return ret;
}

/*!
  Reads image data from \a filename. In Coin, simage is used to
  load image files, and several common file formats are supported.
  simage can be downloaded from our webpages.  If loading
  fails for some reason this method returns FALSE, and the instance
  is set to an empty image. If the file is successfully loaded, the
  file image data is copied into this class.

  If \a numdirectories > 0, this method will search for \a filename
  in all directories in \a searchdirectories.
*/
SbBool
SbImage::readFile(const SbString & filename,
                  const SbString * const * searchdirectories,
                  const int numdirectories)
{
  SbString finalname = SbImage::searchForFile(filename, searchdirectories,
                                              numdirectories);
  if (finalname.getLength()) {
    int w, h, nc;
    unsigned char * simagedata = NULL;

    if (simage_wrapper()->available && simage_wrapper()->simage_read_image) {
      simagedata = simage_wrapper()->simage_read_image(finalname.getString(), &w, &h, &nc);
#if COIN_DEBUG
      if (!simagedata) {
        SoDebugError::post("SbImage::readFile", "%s",
                           simage_wrapper()->simage_get_last_error ?
                           simage_wrapper()->simage_get_last_error() :
                           "Unknown error");
      }
#endif // COIN_DEBUG
    }

    if (simagedata) {
      this->setValue(SbVec2s((short)w, (short)h), nc, simagedata);
      if (simage_wrapper()->simage_free_image) {
        simage_wrapper()->simage_free_image(simagedata);
      }
#if COIN_DEBUG && 1 // debug
      else {
        SoDebugError::postInfo("SbImage::readFile",
                               "Couldn't free image.");
      }
#endif // debug
      return TRUE;
    }
  }

  this->setValue(SbVec2s(0,0), 0, NULL);
  return FALSE;
}

/*!
  \fn int SbImage::operator!=(const SbImage & image) const
  Compare image of \a image with the image in this class and
  return \c FALSE if they are equal.
*/


/*!
  Compare image of \a image with the image in this class and
  return \c TRUE if they are equal.
*/
int
SbImage::operator==(const SbImage & image) const
{
  if (this->size != image.size) return FALSE;
  if (this->bpp != image.bpp) return FALSE;
  if (this->bytes == NULL || image.bytes == NULL) {
    return (this->bytes == image.bytes);
  }
  return memcmp(this->bytes, image.bytes,
                int(this->size[0])*int(this->size[1])*this->bpp) == 0;
}
