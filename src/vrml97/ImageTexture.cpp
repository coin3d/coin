/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 2001 by Systems in Motion. All rights reserved.
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
  \class SoVRMLImageTexture SoVRMLImageTexture.h Inventor/VRMLnodes/SoVRMLImageTexture.h
  \brief The SoVRMLImageTexture class is used for mapping a texture file onto geometry.
  \ingroup VRMLnodes

  \WEB3DCOPYRIGHT

  \verbatim
  ImageTexture {
    exposedField MFString url     []
    field        SFBool   repeatS TRUE
    field        SFBool   repeatT TRUE
  }
  \endverbatim

  The ImageTexture node defines a texture map by specifying an image
  file and general parameters for mapping to geometry. Texture maps
  are defined in a 2D coordinate system (s, t) that ranges from [0.0,
  1.0] in both directions. The bottom edge of the image corresponds to
  the S-axis of the texture map, and left edge of the image
  corresponds to the T-axis of the texture map. The lower-left pixel
  of the image corresponds to s=0, t=0, and the top-right pixel of the
  image corresponds to s=1, t=1. These relationships are depicted in
  Figure 6.9.

  <center>
  <img src="http://www.web3d.org/technicalinfo/specifications/vrml97/Images/ImageTexture.gif">
  Figure 6.9
  </center>

  The texture is read from the URL specified by the url field. When
  the url field contains no values ([]), texturing is
  disabled. Browsers shall support the JPEG and PNG image file
  formats. In addition, browsers may support other image formats
  (e.g. CGM) which can be rendered into a 2D image. Support for the
  GIF format is also recommended (including transparency).

  Details on the url field can be found in 4.5, VRML and the World
  Wide Web.

  See 4.6.11, Texture maps
  (http://www.web3d.org/technicalinfo/specifications/vrml97/part1/concepts.html#4.6.11),
  for a general description of texture maps.

  See 4.14, Lighting model
  (http://www.web3d.org/technicalinfo/specifications/vrml97/part1/concepts.html#4.14),
  for a description of lighting equations and the interaction between
  textures, materials, and geometry appearance.

  The repeatS and repeatT fields specify how the texture wraps in the
  S and T directions. If repeatS is TRUE (the default), the texture
  map is repeated outside the [0.0, 1.0] texture coordinate range in
  the S direction so that it fills the shape. If repeatS is FALSE, the
  texture coordinates are clamped in the S direction to lie within the
  [0.0, 1.0] range. The repeatT field is analogous to the repeatS
  field.

*/

/*!
  SoMFString SoVRMLImageTexture::url
  The texture file URL.
*/

#include <Inventor/VRMLnodes/SoVRMLImageTexture.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/nodes/SoSubNodeP.h>

#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLTextureEnabledElement.h>
#include <Inventor/elements/SoGLTextureImageElement.h>
#include <Inventor/elements/SoTextureQualityElement.h>
#include <Inventor/elements/SoTextureOverrideElement.h>
#include <Inventor/elements/SoTextureScalePolicyElement.h>
#include <Inventor/errors/SoReadError.h>
#include <Inventor/misc/SoGLBigImage.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/lists/SbStringList.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/SbImage.h>
#include <Inventor/SoInput.h>
#include "../tidbits.h" // coin_atexit()
#include <assert.h>
#include "../misc/simage_wrapper.h"

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

static int imagedata_maxage;

#ifdef HAVE_THREADS

#include <Inventor/C/threads/sched.h>
#include <Inventor/threads/SbMutex.h>
#include <Inventor/C/base/debug.h>

static cc_sched * imagetexture_scheduler = NULL;
static SbBool imagetexture_is_exiting = FALSE;

#endif // HAVE_THREADS

#ifndef DOXYGEN_SKIP_THIS

class SoVRMLImageTextureP {
public:
  int readstatus;
  class SoGLImage * glimage;
  SbBool glimagevalid;
  SbImage image;
  SoFieldSensor * urlsensor;

  void readimage_cleanup(void);
#ifdef HAVE_THREADS
  SbString scheduledfilename;
  SbMutex readimagemutex;
  SbBool isdestructing;
#endif // HAVE_THREADS
};

#endif // DOXYGEN_SKIP_THIS

SO_NODE_SOURCE(SoVRMLImageTexture);

static void
imagetexture_cleanup(void)
{
#ifdef HAVE_THREADS
  imagetexture_is_exiting = TRUE;
  if (imagetexture_scheduler) {
    cc_sched_destruct(imagetexture_scheduler);
  }
#endif // HAVE_THREADS
}

// Doc in parent
void
SoVRMLImageTexture::initClass(void) // static
{
  SO_NODE_INTERNAL_INIT_CLASS(SoVRMLImageTexture, SO_VRML97_NODE_TYPE);
  imagedata_maxage = 30;

#ifdef HAVE_THREADS
  imagetexture_scheduler = cc_sched_construct(2);
  coin_atexit((coin_atexit_f*) imagetexture_cleanup);
#endif // HAVE_THREADS
}

#undef THIS
#define THIS this->pimpl

#undef THISP
#define THISP thisp->pimpl

/*!
  Constructor.
*/
SoVRMLImageTexture::SoVRMLImageTexture(void)
{
  THIS = new SoVRMLImageTextureP;

  SO_NODE_INTERNAL_CONSTRUCTOR(SoVRMLImageTexture);
  SO_VRMLNODE_ADD_EMPTY_EXPOSED_MFIELD(url);

  THIS->glimage = NULL;
  THIS->glimagevalid = FALSE;
  THIS->readstatus = 1;
  
  // use field sensor for url since we will load an image if
  // filename changes. This is a time-consuming task which should
  // not be done in notify().
  THIS->urlsensor = new SoFieldSensor(urlSensorCB, this);
  THIS->urlsensor->setPriority(0);
  THIS->urlsensor->attach(&this->url);

#ifdef HAVE_THREADS
  THIS->isdestructing = FALSE;
#endif // HAVE_THREADS
}

/*!
  Destructor.
*/
SoVRMLImageTexture::~SoVRMLImageTexture()
{
#ifdef HAVE_THREADS
  // just wait for all threads to finish reading
  if (imagetexture_scheduler) {
    THIS->isdestructing = TRUE; // signal thread that we are destructing
    cc_sched_wait_all(imagetexture_scheduler);
  }
#endif // HAVE_THREADS  

  if (THIS->glimage) THIS->glimage->unref(NULL);
  delete THIS->urlsensor;
  delete THIS;
}

/*!
  Sets the prequalify callback for ImageTexture nodes.
*/
void
SoVRMLImageTexture::setPrequalifyFileCallBack(VRMLPrequalifyFileCallback * cb,
                                              void * closure)
{
}

/*!
  Enable prequalify file loading.
*/
void
SoVRMLImageTexture::allowPrequalifyFile(SbBool enable)
{
}

// Doc in parent
void
SoVRMLImageTexture::doAction(SoAction * action)
{
}

static SoGLImage::Wrap
translateWrap(const SbBool repeat)
{
  if (repeat) return SoGLImage::REPEAT;
  return SoGLImage::CLAMP_TO_EDGE;
}

// Doc in parent
void
SoVRMLImageTexture::GLRender(SoGLRenderAction * action)
{
  SoState * state = action->getState();

  if (SoTextureOverrideElement::getImageOverride(state))
    return;

  float quality = SoTextureQualityElement::get(state);
  if (!THIS->glimagevalid) {
    SbBool needbig =
      SoTextureScalePolicyElement::get(state) ==
      SoTextureScalePolicyElement::DONT_SCALE;

    if (needbig &&
        (THIS->glimage == NULL ||
         THIS->glimage->getTypeId() != SoGLBigImage::getClassTypeId())) {
      if (THIS->glimage) {
        THIS->glimage->setEndFrameCallback(NULL, NULL);
        THIS->glimage->unref(state);
      }
      THIS->glimage = new SoGLBigImage();
    }
    else if (!needbig &&
             (THIS->glimage == NULL ||
              THIS->glimage->getTypeId() != SoGLImage::getClassTypeId())) {
      if (THIS->glimage) {
        THIS->glimage->setEndFrameCallback(NULL, NULL);
        THIS->glimage->unref(state);
      }
      THIS->glimage = new SoGLImage();
    }

    THIS->glimagevalid = TRUE;
    THIS->glimage->setData(&THIS->image,
                           translateWrap(this->repeatS.getValue()),
                           translateWrap(this->repeatT.getValue()),
                           quality);
  }

  SoGLTextureImageElement::set(state, this,
                               THIS->glimagevalid ? THIS->glimage : NULL,
                               SoTextureImageElement::MODULATE,
                               SbColor(1.0f, 1.0f, 1.0f));

  SbBool enable = THIS->glimagevalid &&
    quality > 0.0f &&
    THIS->glimage->getImage() &&
    THIS->glimage->getImage()->hasData();

  SoGLTextureEnabledElement::set(state,
                                 this,
                                 enable);

  if (this->isOverride()) {
    SoTextureOverrideElement::setImageOverride(state, TRUE);
  }
}

// Doc in parent
void
SoVRMLImageTexture::callback(SoCallbackAction * action)
{
}

// Doc in parent
SbBool
SoVRMLImageTexture::readInstance(SoInput * in,
                                 unsigned short flags)
{
  THIS->urlsensor->detach();
  SbBool ret = inherited::readInstance(in, flags);
  this->setReadStatus((int) ret);
  if (ret) {
    if (!this->loadUrl()) {
      SoReadError::post(in, "Could not read texture file: %s",
                        url[0].getString());
      this->setReadStatus(FALSE);
    }
  }
  THIS->urlsensor->attach(&this->url);
  return ret;
}

/*!
  Returns the read status.
*/
int
SoVRMLImageTexture::getReadStatus(void) const
{
  return THIS->readstatus;
}

/*!
  Sets the read status.
*/
void
SoVRMLImageTexture::setReadStatus(int status)
{
  THIS->readstatus = status;
}

//
// Called from readInstance() or when user changes the
// filename field.
//
SbBool
SoVRMLImageTexture::loadUrl(void)
{
  THIS->glimagevalid = FALSE; // recreate GL image in next GLRender()

  SbBool retval = FALSE;
  if (this->url.getNum() && this->url[0].getLength()) {
    const SbStringList & sl = SoInput::getDirectories();
#ifdef HAVE_THREADS
    // instruct SbImage to call image_read_cb the first time the image
    // data is requested (typically when some shape using the texture
    // is inside the view frustum).
    if (THIS->image.scheduleReadFile(image_read_cb, this,
                                     this->url[0],
                                     sl.getArrayPtr(), sl.getLength())) {
      retval = TRUE;
    }
#else // HAVE_THREADS
    // if we don't have threads, read the image file immedidately
    if (THIS->image.readFile(this->url[0],
                             sl.getArrayPtr(), sl.getLength())) {
      retval = TRUE;
    }
#endif // ! HAVE_THREADS
  }
  else {
    retval = TRUE;
  }
  return retval;
}

//
// used for multithread loading.
//
void
SoVRMLImageTexture::glimage_callback(void * closure)
{
#ifdef HAVE_THREADS
  SoVRMLImageTexture * thisp = (SoVRMLImageTexture*) closure;
  if (THISP->glimage) {
    int age = THISP->glimage->getNumFramesSinceUsed();
    if (age > imagedata_maxage) {
      THISP->glimagevalid = FALSE;
      assert(THISP->glimage);
      THISP->glimage->setEndFrameCallback(NULL, NULL);
      THISP->image.setValue(SbVec2s(0,0), 0, NULL);
      (void) thisp->loadUrl();
    }
  }
#endif // HAVE_THREADS
}

//
// multithread loading thread
//
void
SoVRMLImageTexture::read_thread(void * closure)
{
#ifdef HAVE_THREADS
  SoVRMLImageTexture * thisp = (SoVRMLImageTexture*) closure;
  
  if (!imagetexture_is_exiting && !THISP->isdestructing) {
    
    (void) THISP->image.readFile(THISP->scheduledfilename);
    
    assert(THISP->glimage);
    THISP->glimage->setEndFrameCallback(glimage_callback, thisp);
    THISP->glimagevalid = FALSE;
#ifdef COIN_THREADSAFE
    thisp->touch(); // schedule redraw
#endif // COIN_THREADSAFE
  }  
  THISP->readimagemutex.unlock(); // unlock to enable new images to be read
#endif // HAVE_THREADS
}

//
// called (from SbImage) when image data is needed.
//
SbBool
SoVRMLImageTexture::image_read_cb(const SbString & filename, SbImage * image, void * closure)
{
#ifdef HAVE_THREADS
  SoVRMLImageTexture * thisp = (SoVRMLImageTexture*) closure;
  assert(&THISP->image == image);
  assert(THISP->glimage);
  if (!THISP->glimage) {
    return FALSE;
  }
  // lock mutex to avoid another thread overwriting our data.
  (void) THISP->readimagemutex.lock();

  THISP->scheduledfilename = filename;

  cc_sched_schedule(imagetexture_scheduler,
                    read_thread, thisp, 0);
  return TRUE;
#endif // HAVE_THREADS
  return TRUE;
}

//
// called when filename changes
//
void
SoVRMLImageTexture::urlSensorCB(void * data, SoSensor *)
{
  SoVRMLImageTexture * thisp = (SoVRMLImageTexture*) data;

  thisp->setReadStatus(1);
  if (thisp->url.getNum() && thisp->url[0].getLength() &&
      !thisp->loadUrl()) {
    SoDebugError::postWarning("SoVRMLImageTexture::urlSensorCB",
                              "Image file could not be read: %s",
                              thisp->url[0].getString());
    thisp->setReadStatus(0);
  }
}

/*!
  Returns the texture image.
*/
const SbImage *
SoVRMLImageTexture::getImage(void) const
{
  return &THIS->image;
}

/*!
  \internal
*/
void
SoVRMLImageTexture::setImageDataMaxAge(const uint32_t maxage)
{
  imagedata_maxage = maxage;
}
