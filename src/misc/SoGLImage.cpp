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
  \class SoGLImage include/Inventor/misc/SoGLImage.h
  \brief The SoGLImage class is used to control OpenGL textures.
*/

#include <Inventor/misc/SoGLImage.h>
#include <Inventor/SoImageInterface.h>
#include <Inventor/misc/SoGL.h>
#include <Inventor/errors/SoDebugError.h>
#include <stdio.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#endif // _WIN32
#include <GL/gl.h>
#include <Inventor/lists/SbPList.h>

#define MAXPATHLEN 4096 // FIXME: get this properly. 19981024 mortene.

//
// will probably rip out this stupid class later
//
class SoTexHandler {
public:
  static void SetNotfoundImage(SoImageInterface * const image);
  static SoImageInterface *GetNotfoundImage();
  static SoGLImage *GetTexture(const char *texname, void *context);
  static void UnrefTexture(SoGLImage *image);
  static void RegisterModelFilename(const char *filename);
  static void GetTextureImages(SbPList & images, void *context);
  static SbBool ReplaceTexture(const char * const orgname,
                               const char * const newpath,
                               void *context);

  static SoImageInterface *GetImage(const char * const orgname,
                                        void *context);

private:
  static char modelPath[MAXPATHLEN+1];
  static SbPList texArray;
  static SoImageInterface *notFoundImage;

  static char *SearchForTexture(const char *orgname, char *namebuf);
  static void SearchForReuse(SoGLImage *&teximage, SoImageInterface *&image,
                             const char *realname, void *context);
};



//
// private constructor
//
SoGLImage::SoGLImage(SoImageInterface * const img,
                     const char * const fname,
                     const char * const org_name,
                     void * const ctx)
{
  this->image = img;
  this->handle = 0;
  this->alpha = 0;
  this->clampS = 0;
  this->clampT = 0;
  this->refCount = 0;
  this->context = ctx;
  this->filename = NULL;
  this->orgname = NULL;
  if (this->image) this->image->ref();
  this->filename = fname; // SbString will handle NULL pointers
  this->orgname = org_name;
}

/*!
  A constuctor. This constructor should be used if the image data
  is present in memory (not loaded from file).
*/
SoGLImage::SoGLImage(const SbVec2s size,
                     const int numComponents,
                     const unsigned char * bytes)
{
  SoImageInterface *img =
    new SoImageInterface(size, numComponents, bytes);
  this->image = img;
  this->image->ref(); // ref to enable destructor to work correctly
  this->handle = 0;
  this->alpha = 0;
  this->clampS = 0;
  this->clampT = 0;
  this->refCount = -1;  // mark this as not reuseable
  this->context = NULL;
  this->filename = NULL;
  this->orgname = NULL;
}

//
// private destructor
//
SoGLImage::~SoGLImage()
{
  if (this->handle) sogl_free_texture(this->handle);
  if (this->image) this->image->unref();
}

/*!
  This method is used to replace the image data for this GLImage.
  Make sure you have called makeCurrent() for the GL context before
  calling this function.
*/
void
SoGLImage::replaceImage(SoImageInterface *const newimage,
                        const char * const fname,
                        const char * const oname)
{
  if (this->handle) sogl_free_texture(this->handle);
  this->handle = 0;
  if (this->image) this->image->unref();
  this->image = NULL;
  this->filename = fname;
  this->orgname = oname;
  this->image = newimage;
  if (this->image) this->image->ref();
}

/*!
  Initializes the GL image. \a clamps and \a clampt specifies
  whether texture coordininates should be clamped outside
  0 and 1. Returns \e TRUE on success.

  The OpenGL context that is going to use this texture must
  be the current GL context.
*/
SbBool
SoGLImage::init(const SbBool clamps, const SbBool clampt)
{
  if (this->handle) return TRUE;
  if (this->image && this->image->load()) {
    checkResize(); // resize if necessary
    SbVec2s size = image->getSize();
    int format = image->getNumComponents();

    this->handle = sogl_create_texture(clamps, clampt,
                                       this->image->getDataPtr(),
                                       format, size[0], size[1]);
    if (format == 2 || format == 4) this->alpha = TRUE;
    else this->alpha = FALSE;
    return TRUE;
  }
  return FALSE;
}

/*!
  Returns \e TRUE if the GL image has been initialized.
*/
SbBool
SoGLImage::isInitialized() const
{
  return this->handle != 0;
}

//
// some helpful functions
//
static int
cnt_bits(unsigned long val, int &highbit)
{
  int cnt = 0;
  highbit = 0;
  while (val) {
    if (val & 1) cnt++;
    val>>=1;
    highbit++;
  }
  return cnt;
}

static unsigned long
nearest_binary(unsigned long val)
{
  int highbit;
  if (cnt_bits(val, highbit) > 1) {
    return 1<<highbit;
  }
  return val;
}

//
// private method that tests the size of the image, and
// performs an resize if the size is not "binary".
//
void
SoGLImage::checkResize()
{
  SbVec2s size = this->image->getSize();
  int xsize = size[0];
  int ysize = size[1];
  unsigned long newx = nearest_binary(xsize);
  unsigned long newy = nearest_binary(ysize);

  // if >= 256, don't scale up unless size is close to an above binary
  // this saves a lot of texture memory
  if (newx >= 256) {
    if ((newx - xsize) > (newx>>3)) newx >>= 1;
  }
  if (newy >= 256) {
    if ((newy - ysize) > (newy>>3)) newy >>= 1;
  }

  // downscale until legal GL size (implementation dependant)
  // e.g. old 3dfx hardware only allows up to 256x256 textures.

  unsigned long maxsize = sogl_max_texture_size();
  while (newx > maxsize) newx >>= 1;
  while (newy > maxsize) newy >>= 1;

  if (newx != (unsigned long) xsize || newy != (unsigned long) ysize) {
    this->image->resize(SbVec2s(newx, newy));
  }
}

/*!
  Unreferences the texture. Make sure the OpenGL context using
  this texture is the current GL context.

  An object using a GL image should call this method when the
  object is not going to use the texture ant more. The reference
  counting will make sure a GL image is not freed until all
  objects using the image has called this method.
*/
void
SoGLImage::unref()
{
  // not reusable textures are not stored in the texhandler
  // check for this and delete if so
  if (this->refCount < 0) delete this;
  else SoTexHandler::UnrefTexture(this);
}

/*!
  Makes this texture the current OpenGL texture.
*/
void
SoGLImage::apply() const
{
  //  assert(this->handle);
  sogl_apply_texture(this->handle);
}

/*!
  Returns the OpenGL handle for this texture. An OpenGL handle
  will either be an OpenGL texture object or a displat list
  for old OpenGL implementations.
*/
int
SoGLImage::getHandle() const
{
  return this->handle;
}

/*!
  Returns \e TRUE if this texture has an alpha component.
*/
SbBool
SoGLImage::hasAlphaComponent() const
{
  return this->alpha == 1;
}

/*!
  Returns \e TRUE if texture coordinates should be clamped in
  the s-direction.
*/
SbBool
SoGLImage::shouldClampS() const
{
  return this->clampS == 1;
}

/*!
  Returns \e TRUE if texture coordinates should be clamped in
  the t-direction.
*/
SbBool
SoGLImage::shouldClampT() const
{
  return this->clampT == 1;
}

/*!
  Returns the image data for this OpenGL texture.
*/
const SoImageInterface *
SoGLImage::getImage() const
{
  return this->image;
}

/*!
  Searches the texture database and returns a texture object
  that matches \a texname and \a context. If no such texture
  is found, a new texture object is created and returned.

  It is currently not possible to share textures between
  contexts, but this will be implemented at a later stage.
*/
SoGLImage *
SoGLImage::getGLImage(const char * const texname,
                      void * const context)
{
  return SoTexHandler::GetTexture(texname, context);
}


//
// SoTexHandler is a convenience class used by SoGLImage.
//


SbPList SoTexHandler::texArray;
char SoTexHandler::modelPath[MAXPATHLEN+1];
SoImageInterface *SoTexHandler::notFoundImage = NULL;


SoGLImage *
SoTexHandler::GetTexture(const char *texname, void *context)
{
  char realname[MAXPATHLEN*2+1];
  SoImageInterface *image;
  SoGLImage *teximage;

  SearchForReuse(teximage, image, texname, context);
  if (image && !teximage) { //found file, but not in same context
#if COIN_DEBUG
    SoDebugError::post("SoTexHandler::GetTexture",
                       "reusing texture file: %s, not GL texture\n",
                       texname);
#endif
    teximage = new SoGLImage(image, realname, texname, context);
    teximage->refCount++;
    texArray.append(teximage);
    return teximage;
  }
  else if (teximage) {
#if COIN_DEBUG
    if (!image) SoDebugError::post("SoTexHandler::GetTexture",
                                   "texture '%s' still does not exist\n",
                                   texname);
    else SoDebugError::post("SoTexHandler::GetTexture",
                            "reusing texture %s, _and_ GL texture!\n",
                            texname);
#endif
    teximage->refCount++;
    return teximage;
  }
  else {
    // search for texture on disk(s)
    SearchForTexture(texname, realname);
    image = new SoImageInterface(realname);

    if (image) {
      teximage = new SoGLImage(image, realname, texname, context);
    }
    else {
      image = notFoundImage;
      teximage = new SoGLImage(image, NULL, texname, context);
    }
    teximage->refCount++;
    texArray.append(teximage);
    return teximage;
  }
}

void
SoTexHandler::UnrefTexture(SoGLImage *image)
{
  if (image) {
    image->refCount--;
    if (image->refCount == 0) {
      int i, n = texArray.getLength();
      for (i = 0; i < n; i++) {
        if (image == texArray[i]) break;
      }
      if (i < n) texArray.removeFast(i);
      delete image;
    }
  }
}

void
SoTexHandler::RegisterModelFilename(const char * /* filename */)
{
  // FIXME: "refix" without using any common/XPlatform.cpp
  // code. 19981024 mortene.
#if 0
  XP_getpath(filename, modelPath);
#else
  strcpy(modelPath, "");
#endif
}


#define TEST_FILE(x) \
   SoDebugError::postInfo("TEST_FILE", "texture search: %s\n", x); \
   fp = fopen(x, "rb"); \
   if (fp != NULL) { \
     fclose(fp); \
     return x;}


char *
SoTexHandler::SearchForTexture(const char *orgname, char *namebuf)
{
#if defined(_WIN32)
  char dirsplit = '\\';
#else
  char dirsplit = '/';
#endif

  FILE *fp;
  strcpy(namebuf, orgname);
  TEST_FILE(namebuf);

  char basename[MAXPATHLEN+1];
  // FIXME: "refix" without using any common/XPlatform.cpp
  // code. 19981024 mortene.
#if 0
  XP_getbasename(orgname, basename);
#else
  strcpy(basename, orgname);
#endif

#ifndef _WIN32
  // quick fix to handle m$-doze-filenames under UNIX
  if (strcmp(basename, orgname) == 0) {
    char *ptr = strrchr(orgname, '\\');
    if (ptr) { // this is a m$-dos filename
      strcpy(basename, ptr+1);
    }
  }
#endif
  strcpy(namebuf, basename);
  TEST_FILE(namebuf);

  sprintf(namebuf,"textures%c%s", dirsplit, basename);
  TEST_FILE(namebuf);

  sprintf(namebuf,"%s%s", modelPath, basename);
  TEST_FILE(namebuf);

  sprintf(namebuf,"%stextures%c%s", modelPath, dirsplit, basename);
  TEST_FILE(namebuf);

  sprintf(namebuf,"%s..%cmaps%c%s", modelPath, dirsplit, dirsplit, basename);
  TEST_FILE(namebuf);

  sprintf(namebuf,"%s..%ctextures%c%s", modelPath, dirsplit, dirsplit, basename);
  TEST_FILE(namebuf);

#ifdef _WIN32_
  sprintf(namebuf, "\\3dsmax\\maps\\%s", basename);
  TEST_FILE(namebuf);

  sprintf(namebuf, "\\3dsmax2\\maps\\%s", basename);
  TEST_FILE(namebuf);

  sprintf(namebuf, "\\3ds\\materials\\%s", basename);
  TEST_FILE(namebuf);
#endif
  return namebuf;
}

#undef TEST_FILE


void
SoTexHandler::SearchForReuse(SoGLImage *&teximage,
                             SoImageInterface *&image,
                             const char *texname, void *context)
{
  teximage = NULL;
  image = NULL;

  SoGLImage *t = NULL;

  int i, n = texArray.getLength();
  for (i = 0; i < n; i++) {
    t = (SoGLImage*)texArray[i];
    if (!strcmp(t->orgname.getString(), texname)) {
      image = t->image;
      if (t->context == context) {
        teximage = t;
        return; // found one, just return
      }
    }
  }
}

void
SoTexHandler::GetTextureImages(SbPList &images, void *context)
{
  SoGLImage *t = NULL;
  int i, n = texArray.getLength();
  for (i = 0; i < n; i++) {
    t = (SoGLImage*)texArray[i];
    if (t->context == context)
      images.append(t);
  }
}

SoImageInterface *
SoTexHandler::GetImage(const char * const orgname, void *context)
{
  SoGLImage *t = NULL;
  int i, n = texArray.getLength();
  for (i = 0; i < n; i++) {
    t = (SoGLImage*)texArray[i];
    if (t->context == context && !strcmp(t->orgname.getString(), orgname))
      return t->image;
  }
  return NULL;
}


SbBool
SoTexHandler::ReplaceTexture(const char * const orgname,
                             const char * const newpath,
                             void *context)
{
  SoGLImage *t = NULL;
  int i, n = texArray.getLength();
  for (i = 0; i < n; i++) {
    t = (SoGLImage*)texArray[i];
    if (t->context == context && !strcmp(t->orgname.getString(), orgname))
      break;
  }
  assert(i < n);

  SoGLImage *dummy = NULL;
  SoImageInterface *image = NULL;
  SearchForReuse(dummy, image, newpath, context);
  if (image == NULL) {
    char realname[MAXPATHLEN*2+1];
    SearchForTexture(newpath, realname);
    image = new SoImageInterface(realname);
  }
  if (image) {
    t->replaceImage(image, image->getFilename(), newpath);
    return TRUE;
  }
  return FALSE;
}

void
SoTexHandler::SetNotfoundImage(SoImageInterface * const image)
{
  notFoundImage = image;
}


SoImageInterface *
SoTexHandler::GetNotfoundImage()
{
  return notFoundImage;
}
