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

#include <Inventor/misc/SoImageInterface.h>

#include <Inventor/SoInput.h>
#include <Inventor/lists/SbStringList.h>
#include <config.h> // HAVE_LIBSIMAGE define
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#if HAVE_LIBSIMAGE
#include <simage.h>
#endif // HAVE_LIBSIMAGE

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

//
// TODO: write support for other image libraries
//

/*!
  \class SoImageInterface include/Inventor/SoImageInterface.h
  \brief The SoImageInterface class organizes an image file.

  It is a simple class which accepts four types of images.
  The number of components specyfies the image type.
  <li> 1 - Grayscale image, no alpha.</li>
  <li> 2 - Grayscale image with alpha component.</li>
  <li> 3 - RGB image, no alpha.</li>
  <li> 4 - RGBA image.</li>

  This image class is organized using reference counting in
  a similar way to Coin nodes. This way it can be shared
  between, for instance, several nodes.

  This class uses the simage library for loading images.
*/


/*!
  Constructor with \a file_name specifying the path and name of the
  image file. This constructor is private. Use the
  SoImageInterface::findOrCreateImage() method to interface against
  the "fetch data from external source" mechanism.
*/
SoImageInterface::SoImageInterface(const char * const file_name)
  : filename(file_name),
    orgSize(0,0),
    orgNumComponents(0),
    size(0,0),
    numComponents(0),
    dataPtr(NULL),
    refCount(0),
    hasTried(FALSE),
    didAlloc(FALSE),
    transparency(FALSE),
    isReuseable(TRUE)
{
}

/*!
  Constructor with \a size specifying the size of the image,
  \a numComponents specifying the number of components
  per pixel, and \a data specyfying the image data.

  \a data will not be freed when the instance is deleted.
*/
SoImageInterface::SoImageInterface(const SbVec2s size,
                                   const int numComponents,
                                   const unsigned char * data)
{
  this->orgSize = this->size = size;
  this->orgNumComponents = this->numComponents = numComponents;
  this->dataPtr = (unsigned char*)data;
  this->didAlloc = FALSE;  // do not delete this data!
  this->hasTried = TRUE;   // data is loaded, kind of...
  this->checkTransparency();
  this->refCount = 0;
  this->isReuseable = FALSE;
}

/*!
  Destructor which frees the image data if it was allocated by
  this class.
*/
SoImageInterface::~SoImageInterface()
{
  if (this->didAlloc && this->dataPtr) free(this->dataPtr);
}

/*!
  Increases the reference count for this image.

  \sa SoImageInterface::unref()
*/
void
SoImageInterface::ref()
{
  refCount++;
}

/*!
  Decreases the reference count for this image. If the reference
  count equals zero after decreasing, the image is destructed.
  \sa SoImageInterface::ref()
*/
void
SoImageInterface::unref()
{
  if (this->isReuseable)
    SoImageInterface::unrefImage(this);
  else {
    if (--refCount == 0) delete this;
  }
}

/*!
  A very lame resize function. No bilinear interpolation when
  scaling up, and no square sum when scaling down. FIXME
*/
SbBool
SoImageInterface::resize(const SbVec2s newsize)
{
  if (newsize != size) {
#if COIN_DEBUG && 0 // debug
    SoDebugError::postInfo("SoImageInterface::resize",
                           "(%d): %d %d --> %d %d",
                           numComponents,
                           size[0], size[1], newsize[0], newsize[1]);
#endif // debug
    int num_comp = this->numComponents;
    unsigned char *dest =
      (unsigned char *)malloc(newsize[0]*newsize[1]*num_comp);

    float sx, sy, dx, dy;
    dx = ((float)this->size[0])/((float)newsize[0]);
    dy = ((float)this->size[1])/((float)newsize[1]);
    int src_bpr = this->size[0] * num_comp;
    int dest_bpr = newsize[0] * num_comp;

    unsigned char *src = this->dataPtr; // use local variable for speed

    sy = 0.0f;
    int ystop = newsize[1] * dest_bpr;
    int xstop = newsize[0] * num_comp;
    for (int y = 0; y < ystop; y += dest_bpr) {
      sx = 0.0f;
      for (int x = 0; x < xstop; x += num_comp) {
        int offset = ((int)sy)*src_bpr + ((int)sx)*num_comp;
        for (int i = 0; i < num_comp; i++) dest[x+y+i] = src[offset+i];
        sx += dx;
      }
      sy += dy;
    }

    this->size = newsize;
    if (this->didAlloc && this->dataPtr) free(this->dataPtr);
    this->dataPtr = dest;
    this->didAlloc = TRUE; // we did alloc this data
  }

  return TRUE;
}

/*!
  If the file isn't already loaded, this method will attempt
  to load the file specified in the constructor.
  If \a forceTry is \e TRUE, it will try to load the file
  even though the file could not be loaded the last the
  this method was called.

  \e TRUE is returned if the image file is loaded, \e FALSE
  otherwise.
*/
SbBool
SoImageInterface::load(const SbBool forceTry)
{
  if (this->dataPtr) return TRUE;
  if (forceTry || !this->hasTried) {
    this->hasTried = TRUE;
    this->dataPtr = NULL;

    //
    // TODO: code to read images using other image libraries
    // should be inserted here.
    //

#if HAVE_LIBSIMAGE
    int w, h, nc;
    this->dataPtr = simage_read_image(this->filename.getString(),
                                      &w, &h, &nc);
    if (this->dataPtr) {
      this->orgSize = this->size = SbVec2s(w, h);
      this->orgNumComponents = this->numComponents = nc;
      this->checkTransparency();
      return TRUE;
    }
#endif // HAVE_LIBSIMAGE
  }
  return FALSE;
}

/*!
  Returns \e TRUE if the image contains data
  (is loaded or was constructed with image data).
*/
SbBool
SoImageInterface::isLoaded() const
{
  return (this->dataPtr != NULL);
}

/*!
  Returns a pointer to the image data.
*/
const unsigned char *
SoImageInterface::getDataPtr() const
{
  return this->dataPtr;
}

/*!
  Returns the number of components in the image.
*/
int
SoImageInterface::getNumComponents() const
{
  return this->numComponents;
}

/*!
  Returns the size of the image.
*/
SbVec2s
SoImageInterface::getSize() const
{
  return this->size;
}

/*!
  Returns the filename for this image.
*/
const char *
SoImageInterface::getFilename() const
{
  return this->filename.getString();
}

/*!
  Returns the original number of components of this image.
  Currently it is not possible to change the number of components,
  but this might change in the future.
*/
int
SoImageInterface::getOrgNumComponents() const
{
  return this->orgNumComponents;
}

/*!
  Returns the original size of the image, before any resize.
*/
SbVec2s
SoImageInterface::getOriginalSize() const
{
  return this->orgSize;
}

/*!
  Returns TRUE if at least one pixel has alpha != 255. This value
  is cached, of course.
*/
SbBool
SoImageInterface::hasTransparency() const
{
  return this->transparency;
}

/*!
  Convenience method that creates an SoImageInterface which is a copy
  of this image. Only the image data will be copied, not attributes
  like filename.

  You should call ref() on the copied image after receiving it, and
  unref() to delete it.
*/
SoImageInterface *
SoImageInterface::imageCopy() const
{
  SbVec2s size = this->getSize();
  int nc = this->getNumComponents();
  const unsigned char *src = this->getDataPtr();
  assert(src != NULL);
  unsigned char *dest = (unsigned char *) malloc(size[0]*size[1]*nc);
  memcpy(dest, src, size[0]*size[1]*nc);
  SoImageInterface *copy = new SoImageInterface(size, nc, dest);
  copy->didAlloc = TRUE;
  return copy;
}

//
// private method that checks data for transparency
//
void
SoImageInterface::checkTransparency()
{
  if (this->numComponents == 2 || this->numComponents == 4) {

    int n = this->size[0] * this->size[1];
    int nc = this->numComponents;
    unsigned char *ptr = (unsigned char *) this->dataPtr + nc - 1;

    while (n) {
      if (*ptr != 255) break;
      ptr += nc;
      n--;
    }
    this->transparency = n > 0;
  }
  else this->transparency = FALSE;
}


class so_image_data {
public:
  so_image_data(const char * const orgname = NULL,
                SoImageInterface *image = NULL) {
    this->orgname = NULL;
    if (orgname) {
      this->orgname = new char[strlen(orgname)+1];
      strcpy(this->orgname, orgname);
    }
    this->image = image;
  }
  ~so_image_data() {
    delete [] this->orgname;
  }
public:
  char *orgname;
  SoImageInterface *image;
};

/****** static methods used to enable image reuse **************/


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


// FIXME: I'm not too fond of the API design for this function;
// passing character buffers of unknown size seldom did anyone any
// good.. why not replace them with SbString arguments instead?
// 19991215 mortene.

static SbString
search_for_image(const char * const orgname, const SbStringList & dirlist)
{
  // FIXME: use a configure check. Or remove completely, I believe
  // MSWin can grok filenames with '/' path delimiters. 19991215 mortene.
#if defined(_WIN32)
  const char dirsplit = '\\';
#else
  const char dirsplit = '/';
#endif

  SbString fullname = orgname;
  TRY_FILE(fullname);

  SbString basename = SoInput::getBasename(orgname);

  for (int i = 0; i < dirlist.getLength(); i++) {
    fullname.sprintf("%s%c%s", dirlist[i]->getString(), dirsplit, basename.getString());
    TRY_FILE(fullname);

    fullname.sprintf("%s%ctexture%c%s", dirlist[i]->getString(), dirsplit, dirsplit, basename.getString());
    TRY_FILE(fullname);

    fullname.sprintf("%s%ctextures%c%s", dirlist[i]->getString(), dirsplit, dirsplit, basename.getString());
    TRY_FILE(fullname);
  }

  return fullname; // not found, return org name
}

#undef TRY_FILE_DEBUG
#undef TRY_FILE

// Dynamically allocated to avoid problems on systems which doesn't
// handle static constructors.
static SbList<so_image_data *> * loadedfiles = NULL;

/*!
  Should be called by a image holder to enable reusage of images.
  The image will be ref'ed before returned so you should not
  call ref() after you have received an image from this method.
*/
SoImageInterface *
SoImageInterface::findOrCreateImage(const char * const filename,
                                    const SbStringList & dirlist)
{
 // FIXME: deallocate on exit. 20000406 mortene.
  if (loadedfiles) loadedfiles = new SbList<so_image_data *>;

  int n = loadedfiles->getLength();
  for (int i = 0; i < n; i++) {
    if ((*loadedfiles)[i]->image->filename == filename) {
      (*loadedfiles)[i]->image->ref();
      return (*loadedfiles)[i]->image;
    }
  }

  SbString fullname = search_for_image(filename, dirlist);
  if (fullname.getLength()) {
    SoImageInterface *image = new SoImageInterface(fullname.getString());
    loadedfiles->append(new so_image_data(filename, image));
    image->ref();
    return image;
  }
  return NULL;
}

void
SoImageInterface::unrefImage(SoImageInterface * const image)
{
  assert(loadedfiles != NULL);

  int i, n = loadedfiles->getLength();
  for (i = 0; i < n; i++) {
    if ((*loadedfiles)[i]->image == image) break;
  }
  assert(i < n);
  if (image->refCount == 1) {
    delete image;
    delete (*loadedfiles)[i];
    loadedfiles->removeFast(i);
  }
  else image->refCount--;
}
