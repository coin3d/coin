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
  \class SoTexture2 SoTexture2.h Inventor/nodes/SoTexture2.h
  \brief The SoTexture2 class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoTexture2.h>



#include <Inventor/SoInput.h>
#include <Inventor/errors/SoDebugError.h>

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/misc/SoGLImage.h>
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SOGLTEXTUREIMAGEELEMENT)
#include <Inventor/elements/SoGLTextureImageElement.h>
#endif // !COIN_EXCLUDE_SOGLTEXTUREIMAGEELEMENT
#if !defined(COIN_EXCLUDE_SOGLTEXTUREENABLEDELEMENT)
#include <Inventor/elements/SoGLTextureEnabledElement.h>
#endif // !COIN_EXCLUDE_SOGLTEXTUREENABLEDELEMENT

#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/elements/SoTextureImageElement.h>
#include <Inventor/SoImageInterface.h>

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
#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  this->glImage = NULL;
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
}

/*!
  Destructor.
*/
SoTexture2::~SoTexture2()
{
  if (this->imageData) this->imageData->unref();
#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  if (this->glImage) this->glImage->unref();
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
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

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  SO_ENABLE(SoGLRenderAction, SoGLTextureImageElement);
  SO_ENABLE(SoGLRenderAction, SoGLTextureEnabledElement);
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

  SO_ENABLE(SoCallbackAction, SoTextureImageElement);
}


/*!
  FIXME: write function documentation
*/
SbBool
SoTexture2::readInstance(SoInput * in, unsigned short flags)
{
  assert(!in->isBinary() && "FIXME: not implemented yet");

  SbBool readOK = inherited::readInstance(in, flags);

  if (readOK && ! filename.isDefault()) readImage();
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

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write function documentation
*/
void
SoTexture2::GLRender(SoGLRenderAction * action)
{
  // FIXME: context check, pederb

  if (this->glImage == NULL && this->imageData) {
    this->glImage =
      SoGLImage::findOrCreateGLImage(this->imageData, NULL);

  }

  if (this->glImage && !this->glImage->isInitialized()) {
    this->glImage->init(this->wrapS.getValue() == CLAMP,
                        this->wrapT.getValue() == CLAMP);
  }
  SoGLTextureImageElement::set(action->getState(), this,
                               this->glImage,
                               (SoTextureImageElement::Model) model.getValue(),
                               this->blendColor.getValue());
  SoGLTextureEnabledElement::set(action->getState(),
                                 this, this->glImage != NULL);
}
#endif // !COIN_EXCLUDE_SOGLRENDERACTION


#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write doc
 */
void
SoTexture2::doAction(SoAction *action)
{
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
#endif // !COIN_EXCLUDE_SOACTION

#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
/*!
  FIXME: write doc
 */
void
SoTexture2::callback(SoCallbackAction *action)
{
  SoTexture2::doAction(action);
}
#endif // !COIN_EXCLUDE_SOCALLBACKACTION

/*!
  FIXME: write doc
 */
SbBool
SoTexture2::readImage(const SbString & /* fname */,
                      int & /* w */, int & /* h */, int & /* nc */,
                      unsigned char *& /* bytes */)
{
  //  assert(0 && "FIXME: not implemented");
  return TRUE;
}

/*!
  FIXME: write doc
 */
int
SoTexture2::getReadStatus(void)
{
  assert(0 && "FIXME: not implemented");
  return 0;
}

/*!
  FIXME: write doc
 */
void
SoTexture2::setReadStatus(int /* s */)
{
  assert(0 && "FIXME: not implemented");
}

void
SoTexture2::getImage()
{
  if (this->imageData) return;

  SbVec2s size;
  int nc;
  const unsigned char * bytes = this->image.getValue(size, nc);

  if (bytes && size[0] > 0 && size[1] > 0 && nc > 0) {
    this->imageData = new SoImageInterface(size, nc, bytes);
  }
  else {
    if (this->filename.getValue().getLength()) {
      this->imageData =
        SoImageInterface::findOrCreateImage(this->filename.
                                            getValue().getString());
    }
  }
}
