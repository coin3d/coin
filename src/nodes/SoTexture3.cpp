/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoTexture3 SoTexture3.h Inventor/nodes/SoTexture3.h
  \brief The SoTexture3 class is used to map a 3D texture onto geometry.
  \ingroup nodes

  Shape nodes within the scope of SoTexture3 nodes in the scenegraph
  (ie below the same SoSeparator and to the righthand side of the
  SoTexture3) will have the texture applied according to each shape
  type's individual characteristics.  See the documentation of the
  various shape types (SoFaceSet, SoCube, SoSphere, etc etc) for
  information about the specifics of how the textures will be applied.
  An SoTexture3 node will override any previous encountered SoTexture2 nodes
  and vice versa. Mixing of SoTexture3 and SoTextureCoordinate2 (or the other
  way around) is legal, but the third texture coordinate component will
  be ignored (set to 0.0).

  <center>
  <img src="http://doc.coin3d.org/images/SoGuiExamples/nodes/texture3.png">
  </center>

  \since 2002-01-10
  \since TGS Inventor 2.6
*/

#include <Inventor/SoInput.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/nodes/SoTexture3.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
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
#include <string.h>

/*!
  \enum SoTexture3::Model
  Texture mapping model.
*/
/*!
  \var SoTexture3::Model SoTexture3::MODULATE
  Texture image is modulated with polygon.
*/
/*!
  \var SoTexture3::Model SoTexture3::DECAL
  Texture image overwrites polygon color.
*/
/*!
  \var SoTexture3::Model SoTexture3::BLEND
  Blend image using blendColor.
*/

/*!
  \enum SoTexture3::Wrap
  Enum used to specify wrapping strategy.
*/
/*!
  \var SoTexture3::Wrap SoTexture3::REPEAT
  Repeat texture when coordinate is not between 0 and 1.
*/
/*!
  \var SoTexture3::Wrap SoTexture3::CLAMP
  Clamp coordinate between 0 and 1.
*/


/*!
  \var SoMFString SoTexture3::filenames
  Texture filename(s). Specify either this or use SoTexture3::images, not both.
  The depth of the volume is specifies by the number of filenames specified.
  All images must have the same dimensions and number of components.
  NB! A field sensor is attached to this field internally and reloads all
  images when this field changes. You must therefore be careful when
  setting this field and either use startEditing()/finishEditing() or set
  all values with one function call; setValues().
*/
/*!
  \var SoSFImage3 SoTexture3::images
  Inline image data.
*/
/*!
  \var SoSFEnum SoTexture3::wrapR
  Wrapping strategy for the R coordinate (depth).
*/
/*!
  \var SoSFEnum SoTexture3::wrapS
  Wrapping strategy for the S coordinate.
*/
/*!
  \var SoSFEnum SoTexture3::wrapT
  Wrapping strategy for the T coordinate.
*/
/*!
  \var SoSFEnum SoTexture3::model
  Texture model.
*/
/*!
  \var SoSFColor SoTexture3::blendColor
  Blend color. Used when SoTexture3::model is SoTexture3::BLEND.
*/

// *************************************************************************

SO_NODE_SOURCE(SoTexture3);

/*!
  Constructor.
*/
SoTexture3::SoTexture3(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoTexture3);

  SO_NODE_ADD_FIELD(filenames, (""));
  SO_NODE_ADD_FIELD(images, (SbVec3s(0, 0, 0), 0, NULL));
  SO_NODE_ADD_FIELD(wrapR, (REPEAT));
  SO_NODE_ADD_FIELD(wrapS, (REPEAT));
  SO_NODE_ADD_FIELD(wrapT, (REPEAT));
  SO_NODE_ADD_FIELD(model, (MODULATE));
  SO_NODE_ADD_FIELD(blendColor, (0.0f, 0.0f, 0.0f));

  SO_NODE_DEFINE_ENUM_VALUE(Wrap, REPEAT);
  SO_NODE_DEFINE_ENUM_VALUE(Wrap, CLAMP);

  SO_NODE_SET_SF_ENUM_TYPE(wrapS, Wrap);
  SO_NODE_SET_SF_ENUM_TYPE(wrapT, Wrap);
  SO_NODE_SET_SF_ENUM_TYPE(wrapR, Wrap);

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
  this->filenamesensor->attach(&this->filenames);
}

/*!
  Destructor.
*/
SoTexture3::~SoTexture3()
{
  if (this->glimage) this->glimage->unref(NULL);
  delete this->filenamesensor;
}

// doc from parent
void
SoTexture3::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoTexture3, SO_FROM_INVENTOR_2_6|SO_FROM_COIN_2_0);

  SO_ENABLE(SoGLRenderAction, SoGLTextureImageElement);
  SO_ENABLE(SoGLRenderAction, SoGLTexture3EnabledElement);

  SO_ENABLE(SoCallbackAction, SoTextureImageElement);
}


// Documented in superclass.
SbBool
SoTexture3::readInstance(SoInput * in, unsigned short flags)
{
  // Overridden to check if texture files (if any) can be found and
  // loaded.

  this->filenamesensor->detach();
  SbBool readOK = inherited::readInstance(in, flags);
  this->setReadStatus((int) readOK);
  if (readOK && !filenames.isDefault() && filenames.getNum()>0) {
    if (!this->loadFilenames(in)) {
      this->setReadStatus(FALSE);
    }
  }
  this->filenamesensor->attach(&this->filenames);
  return readOK;
}

static SoGLImage::Wrap
translateWrap(const SoTexture3::Wrap wrap)
{
  if (wrap == SoTexture3::REPEAT) return SoGLImage::REPEAT;
  return SoGLImage::CLAMP;
}

// doc from parent
void
SoTexture3::GLRender(SoGLRenderAction * action)
{
  // FIXME: consider sharing textures among contexts, pederb
  SoState * state = action->getState();

  if (SoTextureOverrideElement::getImageOverride(state))
    return;

  float quality = SoTextureQualityElement::get(state);
  if (!this->glimagevalid) {
    int nc;
    SbVec3s size;
    const unsigned char *bytes = this->images.getValue(size, nc);
    //FIXME: 3D support in SoGLBigImage (kintel 20011113)
//      SbBool needbig =
//        SoTextureScalePolicyElement::get(state) ==
//        SoTextureScalePolicyElement::DONT_SCALE;

//      if (needbig &&
//          (this->glimage == NULL ||
//           this->glimage->getTypeId() != SoGLBigImage::getClassTypeId())) {
//        if (this->glimage) this->glimage->unref(state);
//        this->glimage = new SoGLBigImage();
//      }
//      else if (!needbig &&
//               (this->glimage == NULL ||
//                this->glimage->getTypeId() != SoGLImage::getClassTypeId())) {
//        if (this->glimage) this->glimage->unref(state);
//        this->glimage = new SoGLImage();
//      }
    if (this->glimage) this->glimage->unref(state);
    this->glimage = new SoGLImage();

    if (bytes && size != SbVec3s(0,0,0)) {
      this->glimage->setData(bytes, size, nc,
                             translateWrap((Wrap)this->wrapS.getValue()),
                             translateWrap((Wrap)this->wrapT.getValue()),
                             translateWrap((Wrap)this->wrapR.getValue()),
                             quality);
      this->glimagevalid = TRUE;
    }
  }

  SoGLTexture3EnabledElement::set(state,
                                  this, this->glimagevalid &&
                                  quality > 0.0f);
  SoGLTextureImageElement::set(state, this,
                               this->glimagevalid ? this->glimage : NULL,
                               (SoTextureImageElement::Model) model.getValue(),
                               this->blendColor.getValue());


  if (this->isOverride()) {
    SoTextureOverrideElement::setImageOverride(state, TRUE);
  }
}

// doc from parent
void
SoTexture3::doAction(SoAction *action)
{
  SoState *state = action->getState();

  if (SoTextureOverrideElement::getImageOverride(state))
    return;

  int nc;
  SbVec3s size;
  const unsigned char *bytes = this->images.getValue(size, nc);

  if (size != SbVec3s(0,0,0)) {
    SoTextureImageElement::set(state, this,
                               size, nc, bytes,
                               (int)this->wrapT.getValue(),
                               (int)this->wrapS.getValue(),
                               (int)this->wrapR.getValue(),
                               (SoTextureImageElement::Model) model.getValue(),
                               this->blendColor.getValue());
  }
  // if a filename has been set, but the file has not been loaded, supply
  // a dummy texture image to make sure texture coordinates are generated.
  else if (this->images.isDefault() && 
           this->filenames.getNum()>0 &&
           this->filenames[0].getLength()) {
    static const unsigned char dummytex[] = {0xff,0xff,0xff,0xff,
                                             0xff,0xff,0xff,0xff};
    SoTextureImageElement::set(state, this,
                               SbVec3s(2,2,2), 1, dummytex,
                               (int)this->wrapT.getValue(),
                               (int)this->wrapS.getValue(),
                               (int)this->wrapR.getValue(),
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
SoTexture3::callback(SoCallbackAction * action)
{
  SoTexture3::doAction(action);
}

/*!
  Returns read status. 1 for success, 0 for failure.
*/
int
SoTexture3::getReadStatus(void)
{
  return this->readstatus;
}

/*!
  Sets read status.
  \sa getReadStatus()
 */
void
SoTexture3::setReadStatus(int s)
{
  this->readstatus = s;
}

// Documented in superclass.
void
SoTexture3::notify(SoNotList * l)
{
  // Overridden to detect when fields change.

  SoField *f = l->getLastField();
  if (f == &this->images) {
    this->glimagevalid = FALSE;
    this->filenames.setDefault(TRUE); // write image, not filename
  }
  else if (f == &this->wrapS || f == &this->wrapT || f == &this->wrapR) {
    this->glimagevalid = FALSE;
  }
  inherited::notify(l);
}

//
// Called from readInstance() or when user changes the
// filenames field. \e in is set if this function is called
// while reading a scene graph.
//
//FIXME: Recalc so all images have same w, h and nc (kintel 20011201)
//FIXME: Rescale depth to be n^2 ? This might not work very well though
//       if someone decides to add one layer at the time (kintel 20011201)
SbBool
SoTexture3::loadFilenames(SoInput * in)
{
  SbBool retval = FALSE;
  SbVec3s volumeSize(0,0,0);
  int volumenc;
  int numImages = this->filenames.getNum();
  SbBool sizeError = FALSE;
  int i;

  // Fail on empty filenames
  for (i=0;i<numImages;i++) if (this->filenames[i].getLength()==0) break;

  if (i==numImages) { // All filenames valid
    for (int n=0 ; n<numImages && !sizeError ; n++) {
      SbString filename = this->filenames[n];
      SbImage tmpimage;
      const SbStringList &sl = SoInput::getDirectories();
      if (tmpimage.readFile(filename, sl.getArrayPtr(), sl.getLength())) {
        int nc;
        SbVec3s size;
        unsigned char *imgbytes = tmpimage.getValue(size, nc);
        if (size[2]==0) size[2]=1;
        if (this->images.isDefault()) { // First time => allocate memory
          volumeSize.setValue(size[0], 
                              size[1], 
                              size[2]*numImages);
          volumenc = nc;
          this->images.setValue(volumeSize, nc, NULL);
        }
        else { // Verify size & components
          if (size[0] != volumeSize[0] || 
              size[1] != volumeSize[1] || 
              //FIXME: always 1 or what? (kintel 20020110)
              size[2] != (volumeSize[2]/numImages) ||
              nc != volumenc) {
            sizeError = TRUE;
            retval = FALSE;
            
            SbString errstr;
            errstr.sprintf("Texture file #%d (%s) has wrong size:"
                           "Expected (%d,%d,%d,%d) got (%d,%d,%d,%d)\n",
                           n, filename.getString(), 
                           volumeSize[0],volumeSize[1],volumeSize[2],
                           volumenc,
                           size[0],size[1],size[2],nc);
            if (in) SoReadError::post(in, errstr.getString());
            else SoDebugError::postWarning("SoTexture3::loadFilenames()",
                                           errstr.getString());
          }
        }
        if (!sizeError) {
          // disable notification on images while setting data from the 
          // filenames as a notify will cause a filenames.setDefault(TRUE).
          SbBool oldnotify = this->images.enableNotify(FALSE);
          unsigned char *volbytes = this->images.startEditing(volumeSize, 
                                                              volumenc);
          memcpy(volbytes+int(size[0])*int(size[1])*int(size[2])*nc*n,
                 imgbytes, int(size[0])*int(size[1])*int(size[2])*nc);
          this->images.finishEditing();
          this->images.enableNotify(oldnotify);
          this->glimagevalid = FALSE; // recreate GL images in next GLRender()
          retval = TRUE;
        }
      }
      else {
        SbString errstr;
        errstr.sprintf("Could not read texture file #%d: %s",
                       n, filename.getString());
        if (in) SoReadError::post(in, errstr.getString());
        else SoDebugError::postWarning("SoTexture3::loadFilenames()",
                                       errstr.getString());
        retval = FALSE;
      }
    }
  }
  //FIXME: If sizeError, invalidate texture? (kintel 20011113)
  this->images.setDefault(TRUE); // write filenames, not images
  return retval;
}

//
// called when \e filenames changes
//
void
SoTexture3::filenameSensorCB(void * data, SoSensor *)
{
  SoTexture3 *thisp = (SoTexture3 *)data;

  thisp->setReadStatus(TRUE);
  if (thisp->filenames.getNum()<0 ||
      thisp->filenames[0].getLength() &&
      !thisp->loadFilenames()) {
    thisp->setReadStatus(FALSE);
  }
}
