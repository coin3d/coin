/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoTexture2 SoTexture2.h Inventor/nodes/SoTexture2.h
  \brief The SoTexture2 class is used to map a 2D texture onto subsequent geometry in the scenegraph.
  \ingroup nodes

  Shape nodes within the scope of SoTexture2 nodes in the scenegraph
  (ie below the same SoSeparator and to the righthand side of the
  SoTexture2) will have the texture applied according to each shape
  type's individual characteristics.  See the documentation of the
  various shape types (SoFaceSet, SoCube, SoSphere, etc etc) for
  information about the specifics of how the textures will be applied.

  For a simple usage example, see the class documentation for SoSFImage.


  When working with Inventor files, one often wants to embed external
  texture image files into the Inventor files themselves. Here's a
  minimal, stand-alone example which shows how that can be
  accomplished by calling SoField::touch() on the SoTexture2::image
  fields before scenegraph export:

  \code
  #include <Inventor/SoDB.h>
  #include <Inventor/nodes/SoSeparator.h>
  #include <Inventor/nodes/SoTexture2.h>
  #include <Inventor/actions/SoSearchAction.h>
  #include <Inventor/actions/SoWriteAction.h>

  int
  main(void)
  {
    SoDB::init();

    SoInput in;
    SoSeparator * root = SoDB::readAll(&in);
    if (!root) { exit(1); }

    root->ref();

    SoSearchAction searchaction;
    searchaction.setType(SoTexture2::getClassTypeId());
    searchaction.setSearchingAll(TRUE);
    searchaction.setInterest(SoSearchAction::ALL);

    searchaction.apply(root);

    const SoPathList & pl = searchaction.getPaths();
    for (int i=0; i < pl.getLength(); i++) {
      SoFullPath * fp = (SoFullPath *)pl[i];
      SoTexture2 * tex = (SoTexture2 *)fp->getTail();
      assert(tex->getTypeId() == SoTexture2::getClassTypeId());
      tex->image.touch();
    }

    SoWriteAction wa;
    wa.apply(root);

    root->unref();

    return 0;
  }
  \endcode

  Run the example by piping the iv-file you want do texture-embedding
  on from stdin, e.g. like this:

  \verbatim
  $ ./test < input.iv
  \endverbatim
*/

#include <coindefs.h> // COIN_OBSOLETED()
#include <Inventor/SoInput.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/elements/SoGLTextureEnabledElement.h>
#include <Inventor/elements/SoGLTexture3EnabledElement.h>
#include <Inventor/elements/SoGLTextureImageElement.h>
#include <Inventor/elements/SoTextureQualityElement.h>
#include <Inventor/elements/SoTextureOverrideElement.h>
#include <Inventor/elements/SoTextureScalePolicyElement.h>
#include <Inventor/elements/SoGLLazyElement.h>
#include <Inventor/errors/SoReadError.h>
#include <Inventor/misc/SoGLBigImage.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/lists/SbStringList.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/SbImage.h>
#include <assert.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#ifdef COIN_THREADSAFE
#include <Inventor/threads/SbMutex.h>
#endif // COIN_THREADSAFE

/*!
  \enum SoTexture2::Model

  Texture mapping model, for deciding how to "merge" the texturemap
  with the object it is mapped unto.
*/
/*!
  \var SoTexture2::Model SoTexture2::MODULATE

  Texture image is modulated with polygon shading.
*/
/*!
  \var SoTexture2::Model SoTexture2::DECAL

  Texture image overwrites polygon colors.
*/
/*!
  \var SoTexture2::Model SoTexture2::BLEND

  Blend texturemap image on polygons using the color stored in the
  SoTexture2::blendColor field.
*/

/*!
  \enum SoTexture2::Wrap

  Enumeration of wrapping strategies which can be used when the
  texturemap doesn't cover the full extent of the geometry.
*/
/*!
  \var SoTexture2::Wrap SoTexture2::REPEAT
  Repeat texture when coordinate is not between 0 and 1.
*/
/*!
  \var SoTexture2::Wrap SoTexture2::CLAMP
  Clamp coordinate between 0 and 1.
*/


/*!
  \var SoSFString SoTexture2::filename

  Texture filename, referring to a file on disk in a supported image
  bitmap format.

  By default contains an empty string, which means the texture will be
  fetched from SoTexture2::image and not from disk. (Specify either
  this field or use SoTexture2::image, not both.)

  For reading texture image files from disk, Coin uses the "simage"
  library from Systems in Motion. This library must be installed for
  it to be possible to load any image files at all.

  The set of image formats you can actually read with the simage
  library depends on how the simage library was built, as simage is
  again dependent on a few other low-level file import/export
  libraries (for instance for JPEG, GIF, TIFF and PNG support).  To
  make sure all wanted formats are supported, you should build the
  simage library yourself.

  The set of image formats guaranteed to be supported by simage is
  Targa (.tga), PIC (.pic), SGI RGB (.rgb, .bw) and XWD (.xwd).

  For more information about the simage library, including download
  and build instructions, see the <a href="http://www.coin3d.org">Coin
  www-pages</a>.
*/
/*!
  \var SoSFImage SoTexture2::image

  Inline image data. Defaults to contain an empty image.

  See documentation of the SoSFImage class for a very detailed
  description of how the format specification for the image data is
  layed out, and what different image formats for color textures,
  semi-transparent textures, grayscale textures, etc etc, are
  supported.
*/
/*!
  \var SoSFEnum SoTexture2::wrapS

  Wrapping strategy for the S coordinate when the texturemap is
  narrower than the object to map unto.

  Default value is SoTexture2::REPEAT.
*/
/*!
  \var SoSFEnum SoTexture2::wrapT

  Wrapping strategy for the T coordinate when the texturemap is
  shorter than the object to map unto.

  Default value is SoTexture2::REPEAT.
*/
/*!
  \var SoSFEnum SoTexture2::model

  Texturemapping model for how the texturemap is "merged" with the
  polygon primitives it is applied to. Default value is
  SoTexture2::MODULATE.
*/
/*!
  \var SoSFColor SoTexture2::blendColor

  Blend color. Used when SoTexture2::model is SoTexture2::BLEND.

  Default color value is [0, 0, 0], black, which means no contribution
  to the blending is made.
*/

// *************************************************************************

#ifndef DOXYGEN_SKIP_THIS
class SoTexture2P {
public:
  int readstatus;
  SoGLImage * glimage;
  SbBool glimagevalid;
  SoFieldSensor * filenamesensor;
#ifdef COIN_THREADSAFE
  SbMutex mutex;
#endif // COIN_THREADSAFE
};
#endif // DOXYGEN_SKIP_THIS


#undef THIS
#define THIS this->pimpl

#ifdef COIN_THREADSAFE
#define LOCK_GLIMAGE(_thisp_) (_thisp_)->pimpl->mutex.lock()
#define UNLOCK_GLIMAGE(_thisp_) (_thisp_)->pimpl->mutex.unlock()
#else // COIN_THREADSAFE
#define LOCK_GLIMAGE(_thisp_)
#define UNLOCK_GLIMAGE(_thisp_)
#endif // COIN_THREADSAFE


SO_NODE_SOURCE(SoTexture2);

/*!
  Constructor.
*/
SoTexture2::SoTexture2(void)
{
  THIS = new SoTexture2P;

  SO_NODE_INTERNAL_CONSTRUCTOR(SoTexture2);

  SO_NODE_ADD_FIELD(filename, (""));
  SO_NODE_ADD_FIELD(image, (SbVec2s(0, 0), 0, NULL));
  SO_NODE_ADD_FIELD(wrapS, (REPEAT));
  SO_NODE_ADD_FIELD(wrapT, (REPEAT));
  SO_NODE_ADD_FIELD(model, (MODULATE));
  SO_NODE_ADD_FIELD(blendColor, (0.0f, 0.0f, 0.0f));

  SO_NODE_DEFINE_ENUM_VALUE(Wrap, REPEAT);
  SO_NODE_DEFINE_ENUM_VALUE(Wrap, CLAMP);

  SO_NODE_SET_SF_ENUM_TYPE(wrapS, Wrap);
  SO_NODE_SET_SF_ENUM_TYPE(wrapT, Wrap);

  SO_NODE_DEFINE_ENUM_VALUE(Model, MODULATE);
  SO_NODE_DEFINE_ENUM_VALUE(Model, DECAL);
  SO_NODE_DEFINE_ENUM_VALUE(Model, BLEND);
  SO_NODE_SET_SF_ENUM_TYPE(model, Model);

  THIS->glimage = NULL;
  THIS->glimagevalid = FALSE;
  THIS->readstatus = 1;

  // use field sensor for filename since we will load an image if
  // filename changes. This is a time-consuming task which should
  // not be done in notify().
  THIS->filenamesensor = new SoFieldSensor(filenameSensorCB, this);
  THIS->filenamesensor->setPriority(0);
  THIS->filenamesensor->attach(&this->filename);
}

/*!
  Destructor. Frees up internal resources used to store texture image
  data.
*/
SoTexture2::~SoTexture2()
{
  if (THIS->glimage) THIS->glimage->unref(NULL);
  delete THIS->filenamesensor;
  delete THIS;
}

// Documented in superclass.
void
SoTexture2::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoTexture2, SO_FROM_INVENTOR_1|SoNode::VRML1);

  SO_ENABLE(SoGLRenderAction, SoGLTextureImageElement);
  SO_ENABLE(SoGLRenderAction, SoGLTextureEnabledElement);
  SO_ENABLE(SoGLRenderAction, SoGLTexture3EnabledElement);

  SO_ENABLE(SoCallbackAction, SoTextureImageElement);
  SO_ENABLE(SoCallbackAction, SoTextureEnabledElement);
  SO_ENABLE(SoCallbackAction, SoTexture3EnabledElement);

  SO_ENABLE(SoRayPickAction, SoTextureImageElement);
  SO_ENABLE(SoRayPickAction, SoTextureEnabledElement);
  SO_ENABLE(SoRayPickAction, SoTexture3EnabledElement);

}


// Documented in superclass. Overridden to check if texture file (if
// any) can be found and loaded.
SbBool
SoTexture2::readInstance(SoInput * in, unsigned short flags)
{
  THIS->filenamesensor->detach();
  SbBool readOK = inherited::readInstance(in, flags);
  this->setReadStatus((int) readOK);
  if (readOK && !filename.isDefault() && filename.getValue() != "") {
    if (!this->loadFilename()) {
      SoReadError::post(in, "Could not read texture file '%s'",
                        filename.getValue().getString());
      this->setReadStatus(FALSE);
    }
  }
  THIS->filenamesensor->attach(&this->filename);
  return readOK;
}

static SoGLImage::Wrap
translateWrap(const SoTexture2::Wrap wrap)
{
  if (wrap == SoTexture2::REPEAT) return SoGLImage::REPEAT;
  return SoGLImage::CLAMP;
}

// Documented in superclass.
void
SoTexture2::GLRender(SoGLRenderAction * action)
{
  // FIXME: consider sharing textures among contexts, pederb
  SoState * state = action->getState();

  if (SoTextureOverrideElement::getImageOverride(state))
    return;

  float quality = SoTextureQualityElement::get(state);

  LOCK_GLIMAGE(this);

  if (!THIS->glimagevalid) {
    int nc;
    SbVec2s size;
    const unsigned char * bytes =
      this->image.getValue(size, nc);
    SbBool needbig =
      SoTextureScalePolicyElement::get(state) ==
      SoTextureScalePolicyElement::DONT_SCALE;

    if (needbig &&
        (THIS->glimage == NULL ||
         THIS->glimage->getTypeId() != SoGLBigImage::getClassTypeId())) {
      if (THIS->glimage) THIS->glimage->unref(state);
      THIS->glimage = new SoGLBigImage();
    }
    else if (!needbig &&
             (THIS->glimage == NULL ||
              THIS->glimage->getTypeId() != SoGLImage::getClassTypeId())) {
      if (THIS->glimage) THIS->glimage->unref(state);
      THIS->glimage = new SoGLImage();
    }

    if (bytes && size != SbVec2s(0,0)) {
      THIS->glimage->setData(bytes, size, nc,
                             translateWrap((Wrap)this->wrapS.getValue()),
                             translateWrap((Wrap)this->wrapT.getValue()),
                             quality);
      THIS->glimagevalid = TRUE;
    }
  }

  UNLOCK_GLIMAGE(this);

  SoGLTextureImageElement::set(state, this,
                               THIS->glimagevalid ? THIS->glimage : NULL,
                               (SoTextureImageElement::Model) model.getValue(),
                               this->blendColor.getValue());

  SoGLTexture3EnabledElement::set(state, this, FALSE);
  SoGLTextureEnabledElement::set(state,
                                 this, THIS->glimagevalid &&
                                 quality > 0.0f);

  if (this->isOverride()) {
    SoTextureOverrideElement::setImageOverride(state, TRUE);
  }
}

// Documented in superclass.
void
SoTexture2::doAction(SoAction * action)
{
  SoState * state = action->getState();

  if (SoTextureOverrideElement::getImageOverride(state))
    return;

  int nc;
  SbVec2s size;
  const unsigned char * bytes = this->image.getValue(size, nc);

  SoTexture3EnabledElement::set(state, this, FALSE);

  if (size != SbVec2s(0,0)) {
    SoTextureImageElement::set(state, this,
                               size, nc, bytes,
                               (int)this->wrapT.getValue(),
                               (int)this->wrapS.getValue(),
                               (SoTextureImageElement::Model) model.getValue(),
                               this->blendColor.getValue());
    SoTextureEnabledElement::set(state, this, TRUE);
  }
  // if a filename has been set, but the file has not been loaded, supply
  // a dummy texture image to make sure texture coordinates are generated.
  else if (this->image.isDefault() && this->filename.getValue().getLength()) {
    static const unsigned char dummytex[] = {0xff,0xff,0xff,0xff};
    SoTextureImageElement::set(state, this,
                               SbVec2s(2,2), 1, dummytex,
                               (int)this->wrapT.getValue(),
                               (int)this->wrapS.getValue(),
                               (SoTextureImageElement::Model) model.getValue(),
                               this->blendColor.getValue());
    SoTextureEnabledElement::set(state, this, TRUE);
  }
  else {
    SoTextureImageElement::setDefault(state, this);
    SoTextureEnabledElement::set(state, this, FALSE);
  }
  if (this->isOverride()) {
    SoTextureOverrideElement::setImageOverride(state, TRUE);
  }
}

// doc from parent
void
SoTexture2::callback(SoCallbackAction * action)
{
  SoTexture2::doAction(action);
}

// doc from parent
void 
SoTexture2::rayPick(SoRayPickAction * action)
{
  SoTexture2::doAction(action);
}

/*!
  Not implemented in Coin; should probably not have been public in the
  original SGI Open Inventor API.  We'll consider to implement it if
  requested.
*/
SbBool
SoTexture2::readImage(const SbString & fname, int & w, int & h, int & nc,
                      unsigned char *& bytes)
{
  COIN_OBSOLETED();
  return FALSE;
}

/*!
  Returns read status. 1 for success, 0 for failure.
*/
int
SoTexture2::getReadStatus(void)
{
  return THIS->readstatus;
}

/*!
  Sets read status.
  \sa getReadStatus()
 */
void
SoTexture2::setReadStatus(int s)
{
  THIS->readstatus = s;
}

// Documented in superclass. Overridden to detect when fields change.
void
SoTexture2::notify(SoNotList * l)
{
  SoField * f = l->getLastField();
  if (f == &this->image) {
    THIS->glimagevalid = FALSE;

    // write image, not filename
    this->filename.setDefault(TRUE);
    this->image.setDefault(FALSE);
  }
  else if (f == &this->wrapS || f == &this->wrapT) {
    THIS->glimagevalid = FALSE;
  }
  inherited::notify(l);
}

//
// Called from readInstance() or when user changes the
// filename field.
//
SbBool
SoTexture2::loadFilename(void)
{
  SbBool retval = FALSE;
  if (this->filename.getValue().getLength()) {
    SbImage tmpimage;
    const SbStringList & sl = SoInput::getDirectories();
    if (tmpimage.readFile(this->filename.getValue(),
                          sl.getArrayPtr(), sl.getLength())) {
      int nc;
      SbVec2s size;
      unsigned char * bytes = tmpimage.getValue(size, nc);
      // disable notification on image while setting data from filename
      // as a notify will cause a filename.setDefault(TRUE).
      SbBool oldnotify = this->image.enableNotify(FALSE);
      this->image.setValue(size, nc, bytes);
      this->image.enableNotify(oldnotify);
      THIS->glimagevalid = FALSE; // recreate GL image in next GLRender()
      retval = TRUE;
    }
  }
  this->image.setDefault(TRUE); // write filename, not image
  return retval;
}

//
// called when filename changes
//
void
SoTexture2::filenameSensorCB(void * data, SoSensor *)
{
  SoTexture2 * thisp = (SoTexture2*) data;

  thisp->setReadStatus(1);
  if (thisp->filename.getValue().getLength() &&
      !thisp->loadFilename()) {
    SoDebugError::postWarning("SoTexture2::filenameSensorCB",
                              "Image file '%s' could not be read",
                              thisp->filename.getValue().getString());
    thisp->setReadStatus(0);
  }
}

#undef LOCK_GLIMAGE
#undef UNLOCK_GLIMAGE

