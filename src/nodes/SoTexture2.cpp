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
  \brief The SoTexture2 class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoTexture2.h>
#include <coindefs.h> // COIN_STUB()
#include <Inventor/SoInput.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLTextureEnabledElement.h>
#include <Inventor/elements/SoGLTextureImageElement.h>
#include <Inventor/elements/SoTextureImageElement.h>
#include <Inventor/elements/SoTextureQualityElement.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG
#include <Inventor/errors/SoReadError.h>
#include <Inventor/misc/SoGLImage.h>
#include <Inventor/misc/SoImageInterface.h>

#include <assert.h>

/*!
  \enum SoTexture2::Model
  FIXME: write documentation for enum
*/
/*!
  \var SoTexture2::Model SoTexture2::MODULATE
  FIXME: write documentation for enum definition
*/
/*!
  \var SoTexture2::Model SoTexture2::DECAL
  FIXME: write documentation for enum definition
*/
/*!
  \var SoTexture2::Model SoTexture2::BLEND
  FIXME: write documentation for enum definition
*/

/*!
  \enum SoTexture2::Wrap
  FIXME: write documentation for enum
*/
/*!
  \var SoTexture2::Wrap SoTexture2::REPEAT
  FIXME: write documentation for enum definition
*/
/*!
  \var SoTexture2::Wrap SoTexture2::CLAMP
  FIXME: write documentation for enum definition
*/


/*!
  \var SoSFString SoTexture2::filename
  FIXME: write documentation for field
*/
/*!
  \var SoSFImage SoTexture2::image
  FIXME: write documentation for field
*/
/*!
  \var SoSFEnum SoTexture2::wrapS
  FIXME: write documentation for field
*/
/*!
  \var SoSFEnum SoTexture2::wrapT
  FIXME: write documentation for field
*/
/*!
  \var SoSFEnum SoTexture2::model
  FIXME: write documentation for field
*/
/*!
  \var SoSFColor SoTexture2::blendColor
  FIXME: write documentation for field
*/

// *************************************************************************

SO_NODE_SOURCE(SoTexture2);

/*!
  Constructor.
*/
SoTexture2::SoTexture2()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoTexture2);

  SO_NODE_ADD_FIELD(filename,(""));
  SO_NODE_ADD_FIELD(image,(SbVec2s(0,0), 0, NULL));
  SO_NODE_ADD_FIELD(wrapS,(REPEAT));
  SO_NODE_ADD_FIELD(wrapT,(REPEAT));
  SO_NODE_ADD_FIELD(model,(MODULATE));
  SO_NODE_ADD_FIELD(blendColor,(0.0f, 0.0f, 0.0f));

  SO_NODE_DEFINE_ENUM_VALUE(Wrap, REPEAT);
  SO_NODE_DEFINE_ENUM_VALUE(Wrap, CLAMP);

  SO_NODE_SET_SF_ENUM_TYPE(wrapS, Wrap);
  SO_NODE_SET_SF_ENUM_TYPE(wrapT, Wrap);

  SO_NODE_DEFINE_ENUM_VALUE(Model, MODULATE);
  SO_NODE_DEFINE_ENUM_VALUE(Model, DECAL);
  SO_NODE_DEFINE_ENUM_VALUE(Model, BLEND);
  SO_NODE_SET_SF_ENUM_TYPE(model, Model);

  this->imageData = NULL;
  this->glImage = NULL;
}

/*!
  Destructor.
*/
SoTexture2::~SoTexture2()
{
  if (this->imageData) this->imageData->unref();
  if (this->glImage) this->glImage->unref();
}

/*!
  Does initialization common for all objects of the
  SoTexture2 class. This includes setting up the
  type system, among other things.
*/
void
SoTexture2::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoTexture2);

  SO_ENABLE(SoGLRenderAction, SoGLTextureImageElement);
  SO_ENABLE(SoGLRenderAction, SoGLTextureEnabledElement);

  SO_ENABLE(SoCallbackAction, SoTextureImageElement);
}


/*!
  FIXME: write function documentation
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

/*!
  FIXME: write function documentation
*/
SbBool
SoTexture2::readImage(void)
{
  this->getImage();
  if (this->imageData) return this->imageData->load();
  return FALSE;
}

/*!
  FIXME: write function documentation
*/
void
SoTexture2::GLRender(SoGLRenderAction * action)
{
  // FIXME: consider context, pederb
  SoState *state = action->getState();
  this->getImage();

  if (this->imageData) {
    float quality = SoTextureQualityElement::get(state);
    SbBool clamps = this->wrapS.getValue() == SoTexture2::CLAMP;
    SbBool clampt = this->wrapT.getValue() == SoTexture2::CLAMP;

    if (this->glImage && !this->glImage->matches(clamps, clampt, quality)) {
      this->imageData->ref();
      this->glImage->unref();
      this->glImage = NULL;
    }

    if (this->glImage == NULL) {
      this->glImage =
        SoGLImage::findOrCreateGLImage(this->imageData,
                                       clamps, clampt, quality, NULL);

    }
  }

  SoGLTextureImageElement::set(state, this,
                               this->glImage,
                               (SoTextureImageElement::Model) model.getValue(),
                               this->blendColor.getValue());
  SoGLTextureEnabledElement::set(state,
                                 this, this->glImage != NULL);
}

/*!
  FIXME: write doc
 */
void
SoTexture2::doAction(SoAction *action)
{
  this->getImage();
  if (this->imageData) {
    SoTextureImageElement::set(action->getState(), this,
                               imageData->getSize(),
                               imageData->getNumComponents(),
                               imageData->getDataPtr(),
                               (int)this->wrapT.getValue(),
                               (int)this->wrapS.getValue(),
                               (SoTextureImageElement::Model) model.getValue(),
                               this->blendColor.getValue());
  }
  else {
    SoTextureImageElement::set(action->getState(), this,
                               SbVec2s(0,0),
                               0,
                               NULL,
                               (int)this->wrapT.getValue(),
                               (int)this->wrapS.getValue(),
                               (SoTextureImageElement::Model) model.getValue(),
                               this->blendColor.getValue());
  }
}

/*!
  FIXME: write doc
 */
void
SoTexture2::callback(SoCallbackAction *action)
{
  SoTexture2::doAction(action);
}

/*!
  FIXME: write doc
 */
SbBool
SoTexture2::readImage(const SbString & /* fname */,
                      int & /* w */, int & /* h */, int & /* nc */,
                      unsigned char *& /* bytes */)
{
  COIN_STUB();
  return TRUE;
}

/*!
  FIXME: write doc
 */
int
SoTexture2::getReadStatus(void)
{
  COIN_STUB();
  return 0;
}

/*!
  FIXME: write doc
 */
void
SoTexture2::setReadStatus(int /* s */)
{
  COIN_STUB();
}

//
// Private method that creates an SoImageInterface object. This
// provides a common interface, whether the texture is loaded from a
// file or the image data is supplied inside the node.
//
void
SoTexture2::getImage(void)
{
  if (this->imageData) return;

  SbVec2s size;
  int nc;
  const unsigned char * bytes = this->image.getValue(size, nc);

  if (bytes && size[0] > 0 && size[1] > 0 && nc > 0) {
    this->imageData = new SoImageInterface(size, nc, bytes);
    this->imageData->ref();
  }
  else {
    if (this->filename.getValue().getLength()) {
      const SbStringList & dirlist = SoInput::getDirectories();
      const char * texname = this->filename.getValue().getString();
      this->imageData = SoImageInterface::findOrCreateImage(texname, dirlist);
    }
  }
}
