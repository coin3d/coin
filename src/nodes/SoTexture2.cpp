/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
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
*/

#include <coindefs.h> // COIN_OBSOLETED()
#include <Inventor/SoInput.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLTextureEnabledElement.h>
#include <Inventor/elements/SoGLTexture3EnabledElement.h>
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
#include <assert.h>

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

SO_NODE_SOURCE(SoTexture2);

/*!
  Constructor.
*/
SoTexture2::SoTexture2(void)
{
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

  this->glimage = NULL;
  this->glimagevalid = FALSE;
  this->readstatus = 1;

  // use field sensor for filename since we will load an image if
  // filename changes. This is a time-consuming task which should
  // not be done in notify().
  this->filenamesensor = new SoFieldSensor(filenameSensorCB, this);
  this->filenamesensor->setPriority(0);
  this->filenamesensor->attach(&this->filename);
}

/*!
  Destructor. Frees up internal resources used to store texture image
  data.
*/
SoTexture2::~SoTexture2()
{
  if (this->glimage) this->glimage->unref(NULL);
  delete this->filenamesensor;
}

// Documented in superclass.
void
SoTexture2::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoTexture2, SO_FROM_INVENTOR_2_1);

  SO_ENABLE(SoGLRenderAction, SoGLTextureImageElement);
  SO_ENABLE(SoGLRenderAction, SoGLTextureEnabledElement);
  SO_ENABLE(SoGLRenderAction, SoGLTexture3EnabledElement);

  SO_ENABLE(SoCallbackAction, SoTextureImageElement);
}


// Documented in superclass. Overridden to check if texture file (if
// any) can be found and loaded.
SbBool
SoTexture2::readInstance(SoInput * in, unsigned short flags)
{
  this->filenamesensor->detach();
  SbBool readOK = inherited::readInstance(in, flags);
  this->setReadStatus((int) readOK);
  if (readOK && !filename.isDefault() && filename.getValue() != "") {
    if (!this->loadFilename()) {
      SoReadError::post(in, "Could not read texture file %s",
                        filename.getValue().getString());
      this->setReadStatus(FALSE);
    }
  }
  this->filenamesensor->attach(&this->filename);
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
  if (!this->glimagevalid) {
    int nc;
    SbVec2s size;
    const unsigned char * bytes =
      this->image.getValue(size, nc);
    SbBool needbig =
      SoTextureScalePolicyElement::get(state) ==
      SoTextureScalePolicyElement::DONT_SCALE;

    if (needbig &&
        (this->glimage == NULL ||
         this->glimage->getTypeId() != SoGLBigImage::getClassTypeId())) {
      if (this->glimage) this->glimage->unref(state);
      this->glimage = new SoGLBigImage();
    }
    else if (!needbig &&
             (this->glimage == NULL ||
              this->glimage->getTypeId() != SoGLImage::getClassTypeId())) {
      if (this->glimage) this->glimage->unref(state);
      this->glimage = new SoGLImage();
    }

    if (bytes && size != SbVec2s(0,0)) {
      this->glimage->setData(bytes, size, nc,
                             translateWrap((Wrap)this->wrapS.getValue()),
                             translateWrap((Wrap)this->wrapT.getValue()),
                             quality);
      this->glimagevalid = TRUE;
    }
  }

  SoGLTextureImageElement::set(state, this,
                               this->glimagevalid ? this->glimage : NULL,
                               (SoTextureImageElement::Model) model.getValue(),
                               this->blendColor.getValue());

  SoGLTexture3EnabledElement::set(state, this, FALSE);
  SoGLTextureEnabledElement::set(state,
                                 this, this->glimagevalid &&
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

  if (size != SbVec2s(0,0)) {
    SoTextureImageElement::set(state, this,
                               size, nc, bytes,
                               (int)this->wrapT.getValue(),
                               (int)this->wrapS.getValue(),
                               (SoTextureImageElement::Model) model.getValue(),
                               this->blendColor.getValue());
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
  }
  else {
    SoTextureImageElement::setDefault(state, this);
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
  return this->readstatus;
}

/*!
  Sets read status.
  \sa getReadStatus()
 */
void
SoTexture2::setReadStatus(int s)
{
  this->readstatus = s;
}

// Documented in superclass. Overridden to detect when fields change.
void
SoTexture2::notify(SoNotList * l)
{
  SoField * f = l->getLastField();
  if (f == &this->image) {
    this->glimagevalid = FALSE;
    this->filename.setDefault(TRUE); // write image, not filename
  }
  else if (f == &this->wrapS || f == &this->wrapT) {
    this->glimagevalid = FALSE;
  }
  SoNode::notify(l);
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
      this->glimagevalid = FALSE; // recreate GL image in next GLRender()
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
                              "Image file could not be read: %s",
                              thisp->filename.getValue().getString());
    thisp->setReadStatus(0);
  }
}
