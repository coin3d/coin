/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2005 by Systems in Motion.  All rights reserved.
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
 *  See <URL:http://www.coin3d.org/> for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

/*!
  \class SoTextureCubeMap SoTextureCubeMap.h Inventor/nodes/SoTextureCubeMap.h
  \brief The SoTextureCubeMap class is used to map a cube map onto subsequent shape nodes.
  \ingroup nodes

  Shape nodes within the scope of SoTextureCubeMap nodes in the scenegraph
  (ie below the same SoSeparator and to the righthand side of the
  SoTextureCubeMap) will have the texture applied according to each shape
  type's individual characteristics.  See the documentation of the
  various shape types (SoFaceSet, SoCube, SoSphere, etc etc) for
  information about the specifics of how the textures will be applied.

  \since Coin 3.0
*/

#include <assert.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <coindefs.h> // COIN_OBSOLETED()
#include <Inventor/SoInput.h>
#include <Inventor/nodes/SoTextureCubeMap.h>
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
#include <Inventor/elements/SoCacheElement.h>
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/elements/SoTextureUnitElement.h>
#include <Inventor/elements/SoGLMultiTextureImageElement.h>
#include <Inventor/elements/SoGLMultiTextureEnabledElement.h>
#include <Inventor/errors/SoReadError.h>
#include <Inventor/misc/SoGLCubeMapImage.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/lists/SbStringList.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/SbImage.h>
#include <Inventor/C/glue/gl.h>

#ifdef COIN_THREADSAFE
#include <Inventor/threads/SbMutex.h>
#endif // COIN_THREADSAFE

/*!
  \enum SoTextureCubeMap::Model

  Texture mapping model, for deciding how to "merge" the texturemap
  with the object it is mapped unto.
*/
/*!
  \var SoTextureCubeMap::Model SoTextureCubeMap::MODULATE

  Texture color is multiplied by the polygon color. The result will
  be Phong shaded (if light model is PHONG).
*/
/*!
  \var SoTextureCubeMap::Model SoTextureCubeMap::DECAL

  Texture image overwrites polygon shading. Textured pixels will
  not be Phong shaded. Has undefined behaviour for grayscale and
  grayscale-alpha textures.
*/
/*!
  \var SoTextureCubeMap::Model SoTextureCubeMap::BLEND

  This model is normally used with monochrome textures (i.e. textures
  with one or two components). The first component, the intensity, is
  then used to blend between the shaded color of the polygon and the
  SoTextureCubeMap::blendColor.
*/
/*!
  \var SoTextureCubeMap::Model SoTextureCubeMap::REPLACE

  Texture image overwrites polygon shading. Textured pixels will not
  be Phong shaded. Supports grayscale and grayscale alpha
  textures. This feature requires OpenGL 1.1. MODULATE will be used if
  OpenGL version < 1.1 is detected.

  Please note that using this texture model will make your Inventor
  files incompatible with older versions of Coin and Inventor. You
  need Coin >= 2.2 or TGS Inventor 4.0 to load Inventor files that
  uses the REPLACE texture model.

*/ 

/*!
  \enum SoTextureCubeMap::Wrap

  Enumeration of wrapping strategies which can be used when the
  texturemap doesn't cover the full extent of the geometry.
*/
/*!
  \var SoTextureCubeMap::Wrap SoTextureCubeMap::REPEAT
  Repeat texture when coordinate is not between 0 and 1.
*/
/*!
  \var SoTextureCubeMap::Wrap SoTextureCubeMap::CLAMP
  Clamp coordinate between 0 and 1.
*/


/*!
  \var SoSFString SoTextureCubeMap::posXfilename

  The pos-x filename.
*/

/*!
  \var SoSFImage SoTextureCubeMap::posXimage

  The pos-x image.
*/

/*!
  \var SoSFString SoTextureCubeMap::negXfilename

  The neg-x filename.
*/

/*!
  \var SoSFImage SoTextureCubeMap::negXimage

  The neg-x image.
*/

/*!
  \var SoSFString SoTextureCubeMap::posYfilename

  The pos-y filename.
*/

/*!
  \var SoSFImage SoTextureCubeMap::posYimage

  The neg-y image.
*/

/*!
  \var SoSFString SoTextureCubeMap::negYfilename

  The neg-y filename.

*/

/*!
  \var SoSFImage SoTextureCubeMap::negYimage

  The neg-y image.
*/

/*!
  \var SoSFString SoTextureCubeMap::posZfilename

  The pos-z filename.
*/

/*!
  \var SoSFImage SoTextureCubeMap::posZimage
  
  The pos-z image.
*/

/*!
  \var SoSFString SoTextureCubeMap::negZfilename

  The neg-z filename.
*/

/*!
  \var SoSFImage SoTextureCubeMap::negZimage

  The neg-z image.
*/

/*!
  \var SoSFEnum SoTextureCubeMap::wrapS

  Wrapping strategy for the S coordinate when the texturemap is
  narrower than the object to map unto.

  Default value is SoTextureCubeMap::REPEAT.
*/
/*!
  \var SoSFEnum SoTextureCubeMap::wrapT

  Wrapping strategy for the T coordinate when the texturemap is
  shorter than the object to map unto.

  Default value is SoTextureCubeMap::REPEAT.
*/
/*!
  \var SoSFEnum SoTextureCubeMap::model

  Texturemapping model for how the texturemap is "merged" with the
  polygon primitives it is applied to. Default value is
  SoTextureCubeMap::MODULATE.
*/
/*!
  \var SoSFColor SoTextureCubeMap::blendColor

  Blend color. Used when SoTextureCubeMap::model is SoTextureCubeMap::BLEND.

  Default color value is [0, 0, 0], black, which means no contribution
  to the blending is made.
*/

// *************************************************************************

class SoTextureCubeMapP {
public:
  int readstatus;
  SoGLCubeMapImage * glimage;
  SbBool glimagevalid;
  SoFieldSensor * posx_sensor;
  SoFieldSensor * negx_sensor;
  SoFieldSensor * posy_sensor;
  SoFieldSensor * negy_sensor;
  SoFieldSensor * posz_sensor;
  SoFieldSensor * negz_sensor;
#ifdef COIN_THREADSAFE
  SbMutex mutex;
#endif // COIN_THREADSAFE
};


#undef PRIVATE
#define PRIVATE(p) (p->pimpl)

#ifdef COIN_THREADSAFE
#define LOCK_GLIMAGE(_thisp_) (PRIVATE(_thisp_)->mutex.lock())
#define UNLOCK_GLIMAGE(_thisp_) (PRIVATE(_thisp_)->mutex.unlock())
#else // COIN_THREADSAFE
#define LOCK_GLIMAGE(_thisp_)
#define UNLOCK_GLIMAGE(_thisp_)
#endif // COIN_THREADSAFE


SO_NODE_SOURCE(SoTextureCubeMap);

/*!
  Constructor.
*/
SoTextureCubeMap::SoTextureCubeMap(void)
{
  PRIVATE(this) = new SoTextureCubeMapP;

  SO_NODE_INTERNAL_CONSTRUCTOR(SoTextureCubeMap);

  SO_NODE_ADD_FIELD(posXfilename, (""));
  SO_NODE_ADD_FIELD(negXfilename, (""));
  SO_NODE_ADD_FIELD(posYfilename, (""));
  SO_NODE_ADD_FIELD(negYfilename, (""));
  SO_NODE_ADD_FIELD(posZfilename, (""));
  SO_NODE_ADD_FIELD(negZfilename, (""));
  SO_NODE_ADD_FIELD(posXimage, (SbVec2s(0, 0), 0, NULL));
  SO_NODE_ADD_FIELD(negXimage, (SbVec2s(0, 0), 0, NULL));
  SO_NODE_ADD_FIELD(posYimage, (SbVec2s(0, 0), 0, NULL));
  SO_NODE_ADD_FIELD(negYimage, (SbVec2s(0, 0), 0, NULL));
  SO_NODE_ADD_FIELD(posZimage, (SbVec2s(0, 0), 0, NULL));
  SO_NODE_ADD_FIELD(negZimage, (SbVec2s(0, 0), 0, NULL));
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
  SO_NODE_DEFINE_ENUM_VALUE(Model, REPLACE);
  SO_NODE_SET_SF_ENUM_TYPE(model, Model);

  PRIVATE(this)->glimage = NULL;
  PRIVATE(this)->glimagevalid = FALSE;
  PRIVATE(this)->readstatus = 1;

  // use field sensor for filename since we will load an image if
  // filename changes. This is a time-consuming task which should
  // not be done in notify().
  PRIVATE(this)->posx_sensor = new SoFieldSensor(filenameSensorCB, this);
  PRIVATE(this)->posx_sensor->setPriority(0);
  PRIVATE(this)->posx_sensor->attach(&this->posXfilename);

  PRIVATE(this)->negx_sensor = new SoFieldSensor(filenameSensorCB, this);
  PRIVATE(this)->negx_sensor->setPriority(0);
  PRIVATE(this)->negx_sensor->attach(&this->negXfilename);

  PRIVATE(this)->posy_sensor = new SoFieldSensor(filenameSensorCB, this);
  PRIVATE(this)->posy_sensor->setPriority(0);
  PRIVATE(this)->posy_sensor->attach(&this->posYfilename);

  PRIVATE(this)->negy_sensor = new SoFieldSensor(filenameSensorCB, this);
  PRIVATE(this)->negy_sensor->setPriority(0);
  PRIVATE(this)->negy_sensor->attach(&this->negYfilename);

  PRIVATE(this)->posz_sensor = new SoFieldSensor(filenameSensorCB, this);
  PRIVATE(this)->posz_sensor->setPriority(0);
  PRIVATE(this)->posz_sensor->attach(&this->posZfilename);

  PRIVATE(this)->negz_sensor = new SoFieldSensor(filenameSensorCB, this);
  PRIVATE(this)->negz_sensor->setPriority(0);
  PRIVATE(this)->negz_sensor->attach(&this->negZfilename);
}

/*!
  Destructor. Frees up internal resources used to store texture image
  data.
*/
SoTextureCubeMap::~SoTextureCubeMap()
{
  if (PRIVATE(this)->glimage) PRIVATE(this)->glimage->unref(NULL);
  delete PRIVATE(this)->posx_sensor;
  delete PRIVATE(this)->negx_sensor;
  delete PRIVATE(this)->posy_sensor;
  delete PRIVATE(this)->negy_sensor;
  delete PRIVATE(this)->posz_sensor;
  delete PRIVATE(this)->negz_sensor;
  delete PRIVATE(this);
}

// Documented in superclass.
void
SoTextureCubeMap::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoTextureCubeMap, SO_FROM_COIN_2_4);

  SO_ENABLE(SoGLRenderAction, SoGLTextureImageElement);
  SO_ENABLE(SoGLRenderAction, SoGLTextureEnabledElement);

  SO_ENABLE(SoCallbackAction, SoTextureImageElement);
  SO_ENABLE(SoCallbackAction, SoTextureEnabledElement);

  SO_ENABLE(SoRayPickAction, SoTextureImageElement);
  SO_ENABLE(SoRayPickAction, SoTextureEnabledElement);
}


// Documented in superclass. Overridden to check if texture file (if
// any) can be found and loaded.
SbBool
SoTextureCubeMap::readInstance(SoInput * in, unsigned short flags)
{
  PRIVATE(this)->posx_sensor->detach();
  PRIVATE(this)->posy_sensor->detach();
  PRIVATE(this)->posy_sensor->detach();
  PRIVATE(this)->negy_sensor->detach();
  PRIVATE(this)->posz_sensor->detach();
  PRIVATE(this)->negz_sensor->detach();

  SbBool readOK = inherited::readInstance(in, flags);
  this->setReadStatus((int) readOK);
  if (readOK) {
    for (int i = 0; i < 6; i++) {
      SoSFString * fn;
      SoSFImage * img;

      switch (i) {
      default:
      case 0: fn = &this->posXfilename; img = &this->posXimage; break;
      case 1: fn = &this->negXfilename; img = &this->negXimage; break;
      case 2: fn = &this->posYfilename; img = &this->posYimage; break;
      case 3: fn = &this->negYfilename; img = &this->negYimage; break;
      case 4: fn = &this->posZfilename; img = &this->posZimage; break;
      case 5: fn = &this->negZfilename; img = &this->negZimage; break;
      }
      if (!fn->isDefault() && fn->getValue() != "") {
        if (!this->loadFilename(fn, img)) {
          SoReadError::post(in, "Could not read texture file '%s'",
                            fn->getValue().getString());
          this->setReadStatus(FALSE);
        }
      }
    }
  }
  PRIVATE(this)->posx_sensor->attach(&this->posXfilename);
  PRIVATE(this)->negx_sensor->attach(&this->negXfilename);
  PRIVATE(this)->posy_sensor->attach(&this->posYfilename);
  PRIVATE(this)->negy_sensor->attach(&this->negYfilename);
  PRIVATE(this)->posz_sensor->attach(&this->posZfilename);
  PRIVATE(this)->negz_sensor->attach(&this->negZfilename);
  return readOK;
}

// Documented in superclass.
void
SoTextureCubeMap::GLRender(SoGLRenderAction * action)
{
  // FIXME: consider sharing textures among contexts, pederb
  SoState * state = action->getState();

  if (SoTextureOverrideElement::getImageOverride(state))
    return;

  float quality = SoTextureQualityElement::get(state);

  const cc_glglue * glue = cc_glglue_instance(SoGLCacheContextElement::get(state));

  LOCK_GLIMAGE(this);

  if (!PRIVATE(this)->glimagevalid) {    
    if (PRIVATE(this)->glimage) PRIVATE(this)->glimage->unref(state);
    PRIVATE(this)->glimage = new SoGLCubeMapImage();
    
    for (int i = 0; i < 6; i++) {
      SoSFImage * img;
      switch (i) {
      default:
      case 0: img = &this->posXimage; break;
      case 1: img = &this->negXimage; break;
      case 2: img = &this->posYimage; break;
      case 3: img = &this->negYimage; break;
      case 4: img = &this->posZimage; break;
      case 5: img = &this->negZimage; break;
      }

      SbVec2s size;
      int nc;
      const unsigned char * bytes = img->getValue(size, nc);
      
      if (bytes && size != SbVec2s(0,0)) {
        PRIVATE(this)->glimage->setCubeMapImage((SoGLCubeMapImage::Target)i, bytes, size, nc);
        // don't cache while creating a texture object
        SoCacheElement::setInvalid(TRUE);
        if (state->isCacheOpen()) {
          SoCacheElement::invalidate(state);
        }
      }
    }
    if (state->isCacheOpen()) {
      SoCacheElement::invalidate(state);
    }
    PRIVATE(this)->glimagevalid = TRUE;
  }
  
  UNLOCK_GLIMAGE(this);
  
  SoTextureImageElement::Model glmodel = (SoTextureImageElement::Model) 
    this->model.getValue();
  
  if (glmodel == SoTextureImageElement::REPLACE) {
    if (!cc_glglue_glversion_matches_at_least(glue, 1, 1, 0)) {
      static int didwarn = 0;
      if (!didwarn) {
        SoDebugError::postWarning("SoTextureCubeMap::GLRender",
                                  "Unable to use the GL_REPLACE texture model. "
                                  "Your OpenGL version is < 1.1. "
                                  "Using GL_MODULATE instead.");
        didwarn = 1;
      }
      // use MODULATE and not DECAL, since DECAL only works for RGB
      // and RGBA textures
      glmodel = SoTextureImageElement::MODULATE;
    }
  }
  
  int unit = SoTextureUnitElement::get(state);
  int maxunits = cc_glglue_max_texture_units(glue);
  if (unit == 0) {
    SoGLTextureImageElement::set(state, this,
                                 PRIVATE(this)->glimagevalid ? PRIVATE(this)->glimage : NULL,
                                 glmodel,
                                 this->blendColor.getValue());
    
    SoGLTexture3EnabledElement::set(state, this, FALSE);
    if (quality > 0.0f && PRIVATE(this)->glimagevalid) {
      SoGLTextureEnabledElement::enableCubeMap(state, this);
    }
    if (this->isOverride()) {
      SoTextureOverrideElement::setImageOverride(state, TRUE);
    }
  }
  else if (unit < maxunits) {
    SoGLMultiTextureImageElement::set(state, this, unit,
                                      PRIVATE(this)->glimagevalid ? PRIVATE(this)->glimage : NULL,
                                      glmodel,
                                      this->blendColor.getValue());
    if (quality > 0.0f && PRIVATE(this)->glimagevalid) {
      SoGLMultiTextureEnabledElement::enableCubeMap(state, this, unit);
      
    }

  }
  else {
    // we already warned in SoTextureUnit. I think it's best to just
    // ignore the texture here so that all texture for non-supported
    // units will be ignored. pederb, 2003-11-04
  }
}

// Documented in superclass.
void
SoTextureCubeMap::doAction(SoAction * action)
{
#if 0 // not implemented yet
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
#endif // not implemented
}

// doc from parent
void
SoTextureCubeMap::callback(SoCallbackAction * action)
{
  SoTextureCubeMap::doAction(action);
}

// doc from parent
void
SoTextureCubeMap::rayPick(SoRayPickAction * action)
{
  SoTextureCubeMap::doAction(action);
}

/*!
  Not implemented in Coin; should probably not have been public in the
  original SGI Open Inventor API.  We'll consider to implement it if
  requested.
*/
SbBool
SoTextureCubeMap::readImage(const SbString & fname, int & w, int & h, int & nc,
                      unsigned char *& bytes)
{
  COIN_OBSOLETED();
  return FALSE;
}

/*!
  Returns read status. 1 for success, 0 for failure.
*/
int
SoTextureCubeMap::getReadStatus(void)
{
  return PRIVATE(this)->readstatus;
}

/*!
  Sets read status.
  \sa getReadStatus()
 */
void
SoTextureCubeMap::setReadStatus(int s)
{
  PRIVATE(this)->readstatus = s;
}

// Documented in superclass. Overridden to detect when fields change.
void
SoTextureCubeMap::notify(SoNotList * l)
{
  SoField * f = l->getLastField();
  if (f == &this->posXimage) {
    PRIVATE(this)->glimagevalid = FALSE;
    
    // write image, not filename
    this->posXfilename.setDefault(TRUE);
    this->posXimage.setDefault(FALSE);
  }
  else if (f == &this->negXimage) {
    PRIVATE(this)->glimagevalid = FALSE;
    
    // write image, not filename
    this->negXfilename.setDefault(TRUE);
    this->negXimage.setDefault(FALSE);
  }

  else if (f == &this->posYimage) {
    PRIVATE(this)->glimagevalid = FALSE;
    
    // write image, not filename
    this->posYfilename.setDefault(TRUE);
    this->posYimage.setDefault(FALSE);
  }
  else if (f == &this->negYimage) {
    PRIVATE(this)->glimagevalid = FALSE;
    
    // write image, not filename
    this->negYfilename.setDefault(TRUE);
    this->negYimage.setDefault(FALSE);
  }

  else if (f == &this->posZimage) {
    PRIVATE(this)->glimagevalid = FALSE;
    
    // write image, not filename
    this->posZfilename.setDefault(TRUE);
    this->posZimage.setDefault(FALSE);
  }
  else if (f == &this->negZimage) {
    PRIVATE(this)->glimagevalid = FALSE;
    
    // write image, not filename
    this->negZfilename.setDefault(TRUE);
    this->negZimage.setDefault(FALSE);
  }

  else if (f == &this->wrapS || f == &this->wrapT) {
    PRIVATE(this)->glimagevalid = FALSE;
  }
  inherited::notify(l);
}

//
// Called from readInstance() or when user changes the
// filename field.
//
SbBool 
SoTextureCubeMap::loadFilename(SoSFString * filename, SoSFImage * image)
{
  SbBool retval = FALSE;
  if (filename->getValue().getLength()) {
    SbImage tmpimage;
    const SbStringList & sl = SoInput::getDirectories();
    if (tmpimage.readFile(filename->getValue(),
                          sl.getArrayPtr(), sl.getLength())) {
      int nc;
      SbVec2s size;
      unsigned char * bytes = tmpimage.getValue(size, nc);
      // disable notification on image while setting data from filename
      // as a notify will cause a filename.setDefault(TRUE).
      SbBool oldnotify = image->enableNotify(FALSE);
      image->setValue(size, nc, bytes);
      image->enableNotify(oldnotify);
      PRIVATE(this)->glimagevalid = FALSE; // recreate GL image in next GLRender()
      retval = TRUE;
    }
  }
  image->setDefault(TRUE); // write filename, not image
  return retval;
}

//
// called when filename changes
//
void
SoTextureCubeMap::filenameSensorCB(void * data, SoSensor * s)
{
  SoTextureCubeMap * thisp = (SoTextureCubeMap*) data;

  thisp->setReadStatus(1);

  SoSFString * filename;
  SoSFImage * image;

  if (s == PRIVATE(thisp)->posx_sensor) {
    filename = &thisp->posXfilename;
    image = &thisp->posXimage;
  }
  else if (s == PRIVATE(thisp)->negx_sensor) {
    filename = &thisp->negXfilename;
    image = &thisp->negXimage;

  }
  else if (s == PRIVATE(thisp)->posy_sensor) {
    filename = &thisp->posYfilename;
    image = &thisp->posYimage;

  }
  else if (s == PRIVATE(thisp)->negy_sensor) {
    filename = &thisp->negYfilename;
    image = &thisp->negYimage;

  }
  else if (s == PRIVATE(thisp)->posz_sensor) {
    filename = &thisp->posZfilename;
    image = &thisp->posZimage;

  }
  else {
    filename = &thisp->negZfilename;
    image = &thisp->negZimage;
  }

  if (filename->getValue().getLength() &&
      !thisp->loadFilename(filename, image)) {
    SoDebugError::postWarning("SoTextureCubeMap::filenameSensorCB",
                              "Image file '%s' could not be read",
                              filename->getValue().getString());
    thisp->setReadStatus(0);
  }
}

#undef LOCK_GLIMAGE
#undef UNLOCK_GLIMAGE

#undef PRIVATE
