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

#include <Inventor/SbImage.h>
#include <Inventor/SbVec2s.h>
#include <Inventor/SbString.h>
#include <string.h>
#include <stdlib.h>
#if HAVE_CONFIG_H
#include <config.h> // HAVE_LIBSIMAGE, SIMAGE_RUNTIME_LINKING etc
#endif // HAVE_CONFIG_H
#if HAVE_LIBSIMAGE
#include <simage.h>
#endif // HAVE_LIBSIMAGE
#if HAVE_DLFCN_H
#include <dlfcn.h>
#endif // HAVE_DLFCN_H

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG


/*!
  \class SbImage SbImage.h Inventor/SbImage.h
  \brief The SbImage class is used to handle a 2D image.
  \ingroup base

  In the future this class will probably be used to handle image
  reusage, since it's quite common that the same image is used several
  times in a scene and for different contexts. The API will stay the
  same though.

  This class is an extension to the OIV API.
*/

// Data for libsimage interface.
static SbBool SIMAGE_failed_to_load = FALSE;
static void * SIMAGE_libhandle = NULL;
//  typedef SIMAGE_read_image_t unsigned char * (*)(const char *, int *, int *, int *);
static unsigned char * (*SIMAGE_read_image)(const char *, int *, int *, int *) = NULL;



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
    if ((size == this->size) && (bytesperpixel == this->bpp)) {
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

//
// The TRY_FILE macro is used to immediately return from the
// search_for_file function when a file can be opened.
//

#if COIN_DEBUG && 0 // flip 1<->0 to turn texture search trace on or off
#define TRY_FILE_DEBUG(x, result) \
  SoDebugError::postInfo("TRY_FILE", "texture search: %s (%s)", (x), (result))
#else // !COIN_DEBUG
#define TRY_FILE_DEBUG(x, result)
#endif // !COIN_DEBUG

#define TRY_FILE(x) \
  do { \
    FILE * fp = fopen(x.getString(), "rb"); \
    TRY_FILE_DEBUG(x.getString(), fp ? "hit!" : "miss"); \
    if (fp != NULL) { \
      fclose(fp); \
      return x; \
    } \
  } while (0)

//
// used internally to search for a file in different paths
// and subdirectories.
//
static SbString
search_for_file(const SbString & orgname,
                const SbString * const * dirlist,
                const int numdirs)
{
  int i;

  TRY_FILE(orgname);

  SbString fullname = orgname;

  SbBool trypath = TRUE;
  const char * strptr = orgname.getString();
  const char * lastunixdelim = strrchr(strptr, '/');
  const char * lastdosdelim = strrchr(strptr, '\\');
  if (!lastdosdelim) {
    trypath = FALSE;
    lastdosdelim = strrchr(strptr, ':');
  }
  const char * lastdelim = SbMax(lastunixdelim, lastdosdelim);

  if (lastdelim && trypath) {
    SbString tmpstring;
    for (i = 0; i < numdirs; i++) {
      SbString dirname = *(dirlist[i]);
      int dirlen = dirname.getLength();

      if (dirlen > 0 &&
          dirname[dirlen-1] != '/' &&
          dirname[dirlen-1] != '\\' &&
          dirname[dirlen-1] != ':') {
        dirname += "/";
      }

      tmpstring.sprintf("%s%s", dirname.getString(),
                        fullname.getString());
      TRY_FILE(tmpstring);
    }
  }

  SbString basename = lastdelim ?
    orgname.getSubString(lastdelim-strptr + 1, -1) :
    orgname;

  for (i = 0; i < numdirs; i++) {
    SbString dirname = *(dirlist[i]);
    int dirlen = dirname.getLength();

    if (dirlen > 0 &&
        dirname[dirlen-1] != '/' &&
        dirname[dirlen-1] != '\\' &&
        dirname[dirlen-1] != ':') {
      dirname += "/";
    }

    fullname.sprintf("%s%s", dirname.getString(),
                     basename.getString());
    TRY_FILE(fullname);

    // also try come common texture/picture subdirectories
    fullname.sprintf("%stexture/%s", dirname.getString(),
                     basename.getString());
    TRY_FILE(fullname);

    fullname.sprintf("%stextures/%s",
                     dirname.getString(),
                     basename.getString());
    TRY_FILE(fullname);

    fullname.sprintf("%simages/%s",
                     dirname.getString(),
                     basename.getString());
    TRY_FILE(fullname);

    fullname.sprintf("%spics/%s",
                     dirname.getString(),
                     basename.getString());
    TRY_FILE(fullname);

    fullname.sprintf("%spictures/%s",
                     dirname.getString(),
                     basename.getString());
    TRY_FILE(fullname);
  }

  // none found
  return SbString("");
}

#undef TRY_FILE_DEBUG
#undef TRY_FILE


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
  SbString finalname = search_for_file(filename, searchdirectories,
                                       numdirectories);
  if (finalname.getLength()) {
    int w, h, nc;
    unsigned char * simagedata = NULL;

#if HAVE_LIBSIMAGE
    // Function symbol simage_read_image() from libsimage is already
    // loaded.
    if (!SIMAGE_read_image) SIMAGE_read_image = simage_read_image;
#endif // HAVE_LIBSIMAGE

#if SIMAGE_RUNTIME_LINKING
    if (!SIMAGE_read_image && !SIMAGE_failed_to_load) {
      SIMAGE_libhandle = dlopen("libsimage.so", RTLD_NOW);
      if (!SIMAGE_libhandle) {
        SIMAGE_failed_to_load = TRUE;
#if COIN_DEBUG
        SoDebugError::post("SbImage::readFile",
                           "couldn't open libsimage.so; '%s'",
                           dlerror());
#endif // COIN_DEBUG
      }
      else {
        // FIXME: setup atexit() with dlclose. 20000927 mortene.
        SIMAGE_read_image = (unsigned char *(*)(const char *, int *, int *, int *))
          dlsym(SIMAGE_libhandle, "simage_read_image");
        if (!SIMAGE_read_image) {
          SIMAGE_failed_to_load = TRUE;
#if COIN_DEBUG
          SoDebugError::post("SbImage::readFile",
                             "couldn't find function pointer "
                             "simage_read_image; '%s'",
                             dlerror());
#endif // COIN_DEBUG
        }
      }
    }
#endif // SIMAGE_RUNTIME_LINKING

    if (SIMAGE_read_image) {
      simagedata = SIMAGE_read_image(finalname.getString(), &w, &h, &nc);
    }
    if (simagedata) {
      this->setValue(SbVec2s((short)w, (short)h), nc, simagedata);
      free(simagedata);
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
