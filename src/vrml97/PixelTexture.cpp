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
  \class SoVRMLPixelTexture SoVRMLPixelTexture.h Inventor/VRMLnodes/SoVRMLPixelTexture.h
  \brief The SoVRMLPixelTexture class is used for mapping a texture image onto geometry..
*/

/*!
  SoSFImage SoVRMLPixelTexture::image
  The image data.
*/

#include <Inventor/VRMLnodes/SoVRMLPixelTexture.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/SoInput.h>
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
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/SbImage.h>
#include <assert.h>
#include <stddef.h>

SO_NODE_SOURCE(SoVRMLPixelTexture);

// Doc in parent
void
SoVRMLPixelTexture::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoVRMLPixelTexture, SO_VRML97_NODE_TYPE);
}

/*!
  Constructor.
*/
SoVRMLPixelTexture::SoVRMLPixelTexture(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoVRMLPixelTexture);

  SO_VRMLNODE_ADD_EXPOSED_FIELD(image, (SbVec2s(0,0), 0, NULL));

  this->glimage = NULL;
  this->glimagevalid = FALSE;
  this->readstatus = 1;
}

/*!
  Destructor.
*/
SoVRMLPixelTexture::~SoVRMLPixelTexture()
{
  if (this->glimage) this->glimage->unref(NULL);
}

// Doc in parent
void
SoVRMLPixelTexture::doAction(SoAction * action)
{
  // FIXME: set image in SoTextureImageElement
}

static SoGLImage::Wrap
translateWrap(const SbBool repeat)
{
  if (repeat) return SoGLImage::REPEAT;
  return SoGLImage::CLAMP_TO_EDGE;
}

// Doc in parent
void
SoVRMLPixelTexture::GLRender(SoGLRenderAction * action)
{
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
                             translateWrap(this->repeatS.getValue()),
                             translateWrap(this->repeatT.getValue()),
                             quality);
      this->glimagevalid = TRUE;
    }
  }

  SoGLTextureImageElement::set(state, this,
                               this->glimagevalid ? this->glimage : NULL,
                               SoTextureImageElement::MODULATE,
                               SbColor(1.0f, 1.0f, 1.0f));

  SoGLTextureEnabledElement::set(state,
                                 this, this->glimagevalid &&
                                 quality > 0.0f);

  if (this->isOverride()) {
    SoTextureOverrideElement::setImageOverride(state, TRUE);
  }
}

// doc in parent
void
SoVRMLPixelTexture::callback(SoCallbackAction * action)
{
}

// doc in parent
SbBool
SoVRMLPixelTexture::readInstance(SoInput * in,
                                 unsigned short flags)
{
  this->glimagevalid = FALSE;
  return inherited::readInstance(in, flags);
}

/*!
  Overloaded to detect when fields change.
*/
void
SoVRMLPixelTexture::notify(SoNotList * list)
{
  this->glimagevalid = FALSE;
  SoNode::notify(list);
}


