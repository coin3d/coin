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
#include <../misc/simage_wrapper.h>
#include <Inventor/SoInput.h> // for SoInput::searchForFile()
#include <Inventor/lists/SbStringList.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#ifdef HAVE_THREADS
#include <Inventor/threads/SbRWMutex.h>
#endif // HAVE_THREADS

#ifndef DOXYGEN_SKIP_THIS

class SbImageP {
public:
  SbImageP(void)
    : bytes(NULL),
      didalloc(FALSE),
      size(0,0),
      bpp(0),
      schedulecb(NULL)
#ifdef HAVE_THREADS
    , rwmutex(SbRWMutex::READ_PRECEDENCE)
#endif // HAVE_THREADS
  { }
  unsigned char * bytes;
  SbBool didalloc;
  SbVec2s size;
  int bpp;
  SbString schedulename;
  SbImageScheduleReadCB * schedulecb;
  void * scheduleclosure;

#ifdef HAVE_THREADS
  SbRWMutex rwmutex;
  void readLock(void) {
    //    fprintf(stderr,"readlock: %p\n", this);
    this->rwmutex.readLock();
    //fprintf(stderr,"readlock achieved: %p\n", this);
  }
  void readUnlock(void) {
    //fprintf(stderr,"readUnlock: %p\n", this);
    this->rwmutex.readUnlock();
  }
  void writeLock(void) {
    //fprintf(stderr,"writelock: %p\n", this);
    this->rwmutex.writeLock();
    //fprintf(stderr,"writelock achived: %p\n", this);
  }
  void writeUnlock(void) {
    //fprintf(stderr,"writeUnlock: %p\n", this);
    this->rwmutex.writeUnlock();
  }
#else // HAVE_THREADS
  void readLock(void) { }
  void readUnlock(void) { }
  void writeLock(void) { }
  void writeUnlock(void) { }
#endif // ! HAVE_THREADS
};

#endif // DOXYGEN_SKIP_THIS

//////////////////////////////////////////////////////////////////////////


// FIXME HACK: We use bytes to store the SbImageP pointer to avoid
// breaking the ABI compatibility. pederb, 2001-11-06
#undef THIS
#define THIS ((SbImageP*) (this->bytes_obsoleted))
#define PRIVATE(image) ((SbImageP*)((image)->bytes_obsoleted))

/*!
  Default constructor.
*/
SbImage::SbImage(void)
{
  this->bytes_obsoleted = (unsigned char*) new SbImageP;
}

/*!
  Constructor which sets the data using setValue().
  \sa setValue()
*/
SbImage::SbImage(const unsigned char * bytes,
                 const SbVec2s & size, const int bytesperpixel)
{
  this->bytes_obsoleted = (unsigned char*) new SbImageP;
  this->setValue(size, bytesperpixel, bytes);
}

/*!
  Destructor.
*/
SbImage::~SbImage(void)
{
  if (THIS->didalloc) {
    delete[] THIS->bytes;
  }
  delete THIS;
}

/*!
  Apply a read lock on this image. This will make it impossible for
  other threads to change the image while this lock is active. Other
  threads can do read-only operations on this image, of course.

  For the single thread version of Coin, this method does nothing.

  \sa readUnlock()

  \since 2001-11-06
*/
void
SbImage::readLock(void) const
{
  THIS->readLock();
}

/*!
  Release a read lock on this image.

  For the single thread version of Coin, this method does nothing.

  \ sa readLock()

  \since 2001-11-06
*/
void
SbImage::readUnlock(void) const
{
  THIS->readUnlock();
}

/*!
  Sets the image data without copying the data. \a bytes will be used
  directly, and the data will not be freed when the image instance is
  destructed.

  \sa setValue()

  \since 2001-11-06
*/
void
SbImage::setValuePtr(const SbVec2s & size, const int bytesperpixel,
                     const unsigned char * bytes)
{
  THIS->writeLock();
  THIS->schedulename = "";
  THIS->schedulecb = NULL;
  if (THIS->bytes && THIS->didalloc) delete[] THIS->bytes;
  THIS->bytes = (unsigned char *) bytes;
  THIS->didalloc = FALSE;
  THIS->size = size;
  THIS->bpp = bytesperpixel;
  THIS->writeUnlock();
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
  THIS->writeLock();
  THIS->schedulename = "";
  THIS->schedulecb = NULL;
  if (THIS->bytes && THIS->didalloc) {
    // check for special case where we don't have to reallocate
    if (bytes && (size == THIS->size) && (bytesperpixel == THIS->bpp)) {
      memcpy(THIS->bytes, bytes, int(size[0])*int(size[1])*bytesperpixel);
      THIS->writeUnlock();
      return;
    }
    delete[] THIS->bytes;
  }
  THIS->bytes = NULL;
  THIS->size = size;
  THIS->bpp = bytesperpixel;
  int buffersize = int(size[0]) * int(size[1]) * bytesperpixel;
  if (buffersize) {
    // Align buffers because the binary file format has the data aligned
    // (simplifies export code in SoSFImage).
    buffersize = ((buffersize + 3) / 4) * 4;
    THIS->bytes = new unsigned char[buffersize];
    THIS->didalloc = TRUE;

    if (bytes) {
      // Important: don't copy buffersize num bytes here!
      (void)memcpy(THIS->bytes, bytes,
                   int(size[0]) * int(size[1]) * bytesperpixel);
    }
  }
  THIS->writeUnlock();
}

/*!
  Returns the image data.
*/
unsigned char *
SbImage::getValue(SbVec2s & size, int & bytesperpixel) const
{
  THIS->readLock();
  if (THIS->schedulecb) {
    SbImage * thisp = (SbImage*) this;
    // start a thread to read the image.
    SbBool scheduled = THIS->schedulecb(THIS->schedulename, thisp, THIS->scheduleclosure);
    if (scheduled) {
      THIS->schedulecb = NULL;
    }
  }
  size = THIS->size;
  bytesperpixel = THIS->bpp;
  unsigned char * bytes = THIS->bytes;
  THIS->readUnlock();
  return bytes;

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
        SoDebugError::post("SbImage::readFile", "(%s) %s",
                           filename.getString(),
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
  this->readLock();
  int ret = 0;
  if (!THIS->schedulecb && !PRIVATE(&image)->schedulecb) {
    if (THIS->size != PRIVATE(&image)->size) return FALSE;
    if (THIS->bpp != PRIVATE(&image)->bpp) return FALSE;
    if (THIS->bytes == NULL || PRIVATE(&image)->bytes == NULL) {
      return (THIS->bytes == PRIVATE(&image)->bytes);
    }
    SbBool ret = memcmp(THIS->bytes, PRIVATE(&image)->bytes,
                        int(THIS->size[0])*int(THIS->size[1])*THIS->bpp) == 0;
  }
  this->readUnlock();
  return ret;
}

/*!
  Schedule a file for reading. \a cb will be called the first time
  getValue() is called for this image, and the callback should then
  start a thread to read the image. Do not read the image in the
  callback, as this will lock up the application.

  \sa readFile()
*/
SbBool
SbImage::scheduleReadFile(SbImageScheduleReadCB * cb,
                          void * closure,
                          const SbString & filename,
                          const SbString * const * searchdirectories,
                          const int numdirectories)
{
  this->setValue(SbVec2s(0,0), 0, NULL);
  THIS->writeLock();
  THIS->schedulecb = NULL;
  THIS->schedulename =
    this->searchForFile(filename, searchdirectories, numdirectories);
  int len = THIS->schedulename.getLength();
  if (len > 0) {
    THIS->schedulecb = cb;
    THIS->scheduleclosure = closure;
  }
  THIS->writeUnlock();
  return len > 0;
}

/*!
  Returns \a TRUE if the image is not empty. This can be useful, since
  getValue() will start loading the image if scheduleReadFile() has
  been used to set the image data.

  \since 2001-11-08
*/
SbBool 
SbImage::hasData(void) const
{
  SbBool ret;
  this->readLock();
  ret = THIS->bytes != NULL;
  this->readUnlock();
  return ret;
}


#undef THIS
#undef PRIVATE
