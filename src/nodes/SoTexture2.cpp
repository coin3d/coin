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

/*!
  \class SoTexture2 SoTexture2.h Inventor/nodes/SoTexture2.h
  \brief The SoTexture2 class is used to map a 2D texture onto geometry.
  \ingroup nodes

*/

#include <coindefs.h> // COIN_STUB()
#include <Inventor/SoInput.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLTextureEnabledElement.h>
#include <Inventor/elements/SoGLTextureImageElement.h>
#include <Inventor/elements/SoTextureQualityElement.h>
#include <Inventor/elements/SoTextureOverrideElement.h>
#include <Inventor/errors/SoReadError.h>
#include <Inventor/misc/SoGLImage.h>
#include <Inventor/misc/SoImageInterface.h>

#include <assert.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

/*!
  \enum SoTexture2::Model
  Texture mapping model.
*/
/*!
  \var SoTexture2::Model SoTexture2::MODULATE
  Texture image is modulated with polygon.
*/
/*!
  \var SoTexture2::Model SoTexture2::DECAL
  Texture image overwrites polygon color.
*/
/*!
  \var SoTexture2::Model SoTexture2::BLEND
  Blend image using blendColor.
*/

/*!
  \enum SoTexture2::Wrap
  Enum used to specify wrapping strategy.
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
  Texture filename. Specify either this or use image, not both.
*/
/*!
  \var SoSFImage SoTexture2::image
  Inline image data.
*/
/*!
  \var SoSFEnum SoTexture2::wrapS
  Wrapping strategy for the S coordinate.
*/
/*!
  \var SoSFEnum SoTexture2::wrapT
  Wrapping strategy for the T coordinate.
*/
/*!
  \var SoSFEnum SoTexture2::model
  Texture model.
*/
/*!
  \var SoSFColor SoTexture2::blendColor
  Blend color. Used when model is BLEND.
*/

// *************************************************************************

SO_NODE_SOURCE(SoTexture2);

/*!
  Constructor.
*/
SoTexture2::SoTexture2()
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

  this->imagedata = NULL;
  this->glimage = NULL;
  this->imagedatavalid = FALSE;
  this->glimagevalid = FALSE;
  this->readstatus = 1;
}

/*!
  Destructor.
*/
SoTexture2::~SoTexture2()
{
  if (this->imagedata) this->imagedata->unref();
  if (this->glimage) this->glimage->unref();
}

// doc from parent
void
SoTexture2::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoTexture2);

  SO_ENABLE(SoGLRenderAction, SoGLTextureImageElement);
  SO_ENABLE(SoGLRenderAction, SoGLTextureEnabledElement);

  SO_ENABLE(SoCallbackAction, SoTextureImageElement);
}


/*!
  Overloaded to check if texture file (if any) can be found
  and loaded.
*/
SbBool
SoTexture2::readInstance(SoInput * in, unsigned short flags)
{
  SbBool readOK = inherited::readInstance(in, flags);

  if (readOK && !filename.isDefault()) {
    if (!this->readImage()) {
      SoReadError::post(in, "Could not read texture file %s",
                        filename.getValue().getString());
    }
  }
  return readOK;
}

// doc from parent
void
SoTexture2::GLRender(SoGLRenderAction * action)
{
  // FIXME: consider sharing textures among contexts, pederb
  SoState * state = action->getState();

  if (SoTextureOverrideElement::getImageOverride(state))
    return;

  if (!this->getImage()) return;

  float quality = SoTextureQualityElement::get(state);

  if (this->imagedata && this->imagedata->load()) {
    SbBool clamps = this->wrapS.getValue() == SoTexture2::CLAMP;
    SbBool clampt = this->wrapT.getValue() == SoTexture2::CLAMP;

    if (this->glimage && (!this->glimagevalid ||
                          !this->glimage->matches(clamps, clampt))) {
      this->glimage->unref();
      this->glimage = NULL;
    }

    if (this->glimage == NULL) {
      this->glimage =
        SoGLImage::findOrCreateGLImage(this->imagedata,
                                       clamps, clampt, quality, NULL);
      this->glimagevalid = TRUE;
    }
  }
  SoGLTextureImageElement::set(state, this,
                               this->glimage,
                               (SoTextureImageElement::Model) model.getValue(),
                               this->blendColor.getValue());
  SoGLTextureEnabledElement::set(state,
                                 this, this->glimage != NULL && quality > 0.0f);

  if (this->isOverride()) {
    SoTextureOverrideElement::setImageOverride(state, TRUE);
  }
}

// doc from parent
void
SoTexture2::doAction(SoAction * action)
{
  SoState * state = action->getState();

  if (SoTextureOverrideElement::getImageOverride(state))
    return;

  if (!this->getImage()) return;

  if (this->imagedata) {
    SoTextureImageElement::set(state, this,
                               this->imagedata->getSize(),
                               this->imagedata->getNumComponents(),
                               this->imagedata->getDataPtr(),
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
  Not implemented. We have a different strategy for loading images,
  using SoImageInterface. Let us know if anybody needs this function.
*/
SbBool
SoTexture2::readImage(const SbString & /* fname */,
                      int & /* w */, int & /* h */, int & /* nc */,
                      unsigned char *& /* bytes */)
{
  COIN_STUB();
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

//
// Private method that creates an SoImageInterface object. This
// provides a common interface, whether the texture is loaded from a
// file or the image data is supplied inside the node.
//
// returns TRUE if texture image element should be updated,
// FALSE if not.
//
SbBool
SoTexture2::getImage(void)
{
  if (this->filename.isIgnored() && this->image.isIgnored())
    return FALSE;

  if (this->imagedatavalid) return TRUE; // common case

  SbVec2s size;
  int nc;
  const unsigned char * bytes = this->image.getValue(size, nc);
  SbBool validinline =
    (!this->image.isIgnored()) &&
    (bytes != NULL) &&
    (size[0] > 0) &&
    (size[1] > 0) &&
    (nc > 0 && nc <= 4);

  if (this->filename.isIgnored() && !validinline) return FALSE;

  if (this->imagedata) {
    // if we get here, we'll have to "reload" image
    this->imagedata->unref();
    this->imagedata = NULL;
  }

  if (validinline) {
    this->imagedata = new SoImageInterface(size, nc, bytes);
    this->imagedata->ref();
  }
  else {
    if (this->filename.getValue().getLength()) {
      const SbStringList & dirlist = SoInput::getDirectories();
      const char * texname = this->filename.getValue().getString();
      this->imagedata = SoImageInterface::findOrCreateImage(texname, dirlist);
      if (this->imagedata == NULL) {
#if COIN_DEBUG
        SoDebugError::postWarning("SoTexture2::getImage",
                                  "Image not found: %s", texname);
#endif
      }
    }
  }
  this->imagedatavalid = TRUE; // imagedata should be valid (or NULL) now
  return TRUE;
}

//
// read image
//
SbBool
SoTexture2::readImage(void)
{
  this->getImage();
  this->readstatus = this->imagedata && this->imagedata->load();
  return this->readstatus;
}

/*!
  Overloaded to detect when filename or image changes.
*/
void
SoTexture2::notify(SoNotList *list)
{
  SoField *f = list->getLastField();
  if (f == &this->image || f == &this->filename) {
    this->imagedatavalid = FALSE;
    this->glimagevalid = FALSE;
  }
  SoNode::notify(list);
}
