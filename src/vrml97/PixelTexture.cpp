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
  \ingroup VRMLnodes

  \WEB3DCOPYRIGHT

  \verbatim
  PixelTexture {
    exposedField SFImage  image      0 0 0    # see SoSFImage
    field        SFBool   repeatS    TRUE
    field        SFBool   repeatT    TRUE
  }
  \endverbatim

  The PixelTexture node defines a 2D image-based texture map as an
  explicit array of pixel values (image field) and parameters
  controlling tiling repetition of the texture onto geometry.  Texture
  maps are defined in a 2D coordinate system (s, t) that ranges from
  0.0 to 1.0 in both directions. The bottom edge of the pixel image
  corresponds to the S-axis of the texture map, and left edge of the
  pixel image corresponds to the T-axis of the texture map. The
  lower-left pixel of the pixel image corresponds to s=0.0, t=0.0, and
  the top-right pixel of the image corresponds to s = 1.0, t = 1.0.
  See 4.6.11, Texture maps
  (http://www.web3d.org/technicalinfo/specifications/vrml97/part1/concepts.html#4.6.11),
  for a general description of texture
  maps. Figure 6.13 depicts an example PixelTexture.

  <center>
  <img src="http://www.web3d.org/technicalinfo/specifications/vrml97/Images/PixelTexture.gif">
  Figure 6.13 -- PixelTexture node
  </center>

  See 4.14, Lighting model
  (http://www.web3d.org/technicalinfo/specifications/vrml97/part1/concepts.html#4.14),
  for a description of how the texture values interact with the
  appearance of the geometry.  SoSFImage, describes the
  specification of an image.  The repeatS and repeatT fields specify
  how the texture wraps in the S and T directions. If repeatS is TRUE
  (the default), the texture map is repeated outside the 0-to-1
  texture coordinate range in the S direction so that it fills the
  shape. If repeatS is FALSE, the texture coordinates are clamped in
  the S direction to lie within the 0.0 to 1.0 range. The repeatT
  field is analogous to the repeatS field.

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

#ifndef DOXYGEN_SKIP_THIS

class SoVRMLPixelTextureP {
public:
  SoGLImage * glimage;
  SbBool glimagevalid;
  int readstatus;
};

#endif // DOXYGEN_SKIP_THIS

SO_NODE_SOURCE(SoVRMLPixelTexture);

// Doc in parent
void
SoVRMLPixelTexture::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoVRMLPixelTexture, SO_VRML97_NODE_TYPE);
}

#undef THIS
#define THIS this->pimpl

/*!
  Constructor.
*/
SoVRMLPixelTexture::SoVRMLPixelTexture(void)
{
  THIS = new SoVRMLPixelTextureP;

  SO_NODE_INTERNAL_CONSTRUCTOR(SoVRMLPixelTexture);

  SO_VRMLNODE_ADD_EXPOSED_FIELD(image, (SbVec2s(0,0), 0, NULL));

  THIS->glimage = NULL;
  THIS->glimagevalid = FALSE;
  THIS->readstatus = 1;
}

/*!
  Destructor.
*/
SoVRMLPixelTexture::~SoVRMLPixelTexture()
{
  if (THIS->glimage) THIS->glimage->unref(NULL);
  delete THIS;
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
                             translateWrap(this->repeatS.getValue()),
                             translateWrap(this->repeatT.getValue()),
                             quality);
      THIS->glimagevalid = TRUE;
    }
  }

  SoGLTextureImageElement::set(state, this,
                               THIS->glimagevalid ? THIS->glimage : NULL,
                               SoTextureImageElement::MODULATE,
                               SbColor(1.0f, 1.0f, 1.0f));

  SoGLTextureEnabledElement::set(state,
                                 this, THIS->glimagevalid &&
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
  THIS->glimagevalid = FALSE;
  return inherited::readInstance(in, flags);
}

/*!
  Overloaded to detect when fields change.
*/
void
SoVRMLPixelTexture::notify(SoNotList * list)
{
  THIS->glimagevalid = FALSE;
  SoNode::notify(list);
}
