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
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/lists/SbStringList.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/SbImage.h>
#include <assert.h>

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

  this->glimage = new SoGLImage();
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
  Destructor.
*/
SoTexture2::~SoTexture2()
{
  delete this->glimage;
  delete this->filenamesensor;
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
  this->filenamesensor->detach();
  SbBool readOK = inherited::readInstance(in, flags);
  this->setReadStatus((int) readOK);
  if (readOK && !filename.isDefault()) {
    if (!this->loadFilename()) {
      SoReadError::post(in, "Could not read texture file %s",
                        filename.getValue().getString());
      this->setReadStatus(FALSE);
    }
  }
  this->filenamesensor->attach(&this->filename);
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

  float quality = SoTextureQualityElement::get(state);
  if (!this->glimagevalid) {
    SbBool clamps = this->wrapS.getValue() == SoTexture2::CLAMP;
    SbBool clampt = this->wrapT.getValue() == SoTexture2::CLAMP;
    int nc;
    SbVec2s size;
  
    const unsigned char * bytes =
      this->image.getValue(size, nc);
    if (bytes && size != SbVec2s(0,0)) {
      this->glimage->setData(bytes, size, nc, clamps, clampt,
                             quality, NULL);
      this->glimagevalid = TRUE;
    }
  }

  SoGLTextureImageElement::set(state, this,
                               this->glimagevalid ? this->glimage : NULL,
                               (SoTextureImageElement::Model) model.getValue(),
                               this->blendColor.getValue());

  SoGLTextureEnabledElement::set(state,
                                 this, this->glimagevalid &&
                                 quality > 0.0f);

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
  Not implemented in Coin; should probably have been private in OIV.
  Let us know if you need this method and we'll implement it.
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

/*!
  Overloaded to detect when fields change.
*/
void
SoTexture2::notify(SoNotList *list)
{
  SoField *f = list->getLastField();
  if (f == &this->image) {
    this->glimagevalid = FALSE;
    this->filename.setDefault(TRUE); // write image, not filename
  }
  else if (f == &this->wrapS || f == &this->wrapT) {
    this->glimagevalid = FALSE;
  }
  SoNode::notify(list);
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
      this->image.setValue(size, nc, bytes);
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
  if (!thisp->loadFilename()) {
    SoDebugError::postWarning("SoTexture2::filenameSensorCB",
                              "Image file could not be read: %s",
                              thisp->filename.getValue().getString());
    thisp->setReadStatus(0);
  }
}
