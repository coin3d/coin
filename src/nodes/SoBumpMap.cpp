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
  \class SoBumpMap SoBumpMap.h Inventor/nodes/SoBumpMap.h
  \brief The SoBumpMap class is used to map a bump map onto subsequent shapes.
  \ingroup nodes

  Currently only normal maps are supported. This means that the image
  must have three components, where the red image component equals the
  X normal component, green equals Y, and blue is Z. See
  http://www.paulsprojects.net/tutorials/simplebump/simplebump.html
  for a nice introduction about bump mapping and normal maps.
  
  Grayscale images will be treated as height maps, and automatically
  converted to normal maps.

  For bump mapping to work with extension nodes for Coin, the
  SoShape::generatePrimitives() method must be correctly implemented
  for the shape. This is needed since tangent space coordinates needs
  to be calculated for each vertex in the shape. All shape nodes which
  are a standard part of Coin meets this criteria.

  \since Coin 2.2
*/

#include <assert.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/nodes/SoBumpMap.h>
#include <coindefs.h> // COIN_OBSOLETED()
#include <Inventor/SoInput.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/elements/SoBumpMapElement.h>
#include <Inventor/errors/SoReadError.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/lists/SbStringList.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/SbImage.h>
#include <Inventor/SbVec3f.h>
#include <Inventor/misc/SoGLImage.h>
#include <Inventor/C/glue/gl.h>


/*!
  \enum SoBumpMap::Wrap

  Enumeration of wrapping strategies which can be used when the
  bump map doesn't cover the full extent of the geometry.
*/
/*!
  \var SoBumpMap::Wrap SoBumpMap::REPEAT
  Repeat bump map  when coordinate is not between 0 and 1.
*/
/*!
  \var SoBumpMap::Wrap SoBumpMap::CLAMP
  Clamp coordinate between 0 and 1.
*/

/*!
  \var SoSFString SoBumpMap::filename

  Bump map (or normal map) filename, referring to a file on disk in a
  supported image bitmap format. See SoBumpMap::filename for more
  information.
*/

/*!
  \var SoSFImage SoBumpMap::image

  Inline image data. Defaults to contain an empty bump map.

*/

/*!
  \var SoSFEnum SoBumpMap::wrapS

  Wrapping strategy for the S coordinate when the bump map is
  narrower than the object to map unto.

  Default value is SoBumpMap::REPEAT.
*/
/*!
  \var SoSFEnum SoBumpMap::wrapT

  Wrapping strategy for the T coordinate when the bump map is
  shorter than the object to map unto.

  Default value is SoBumpMap::REPEAT.
*/

// *************************************************************************

class SoBumpMapP {
public:
  SoFieldSensor * filenamesensor;
  SoGLImage * glimage;
  SbBool glimagevalid;
  SbImage convertedheightmap;
  SbBool didconvert;
};

#undef PRIVATE
#define PRIVATE(p) (p->pimpl)

static void 
convert_heightmap_to_normalmap(const unsigned char * srcptr, 
                               const SbVec2s size,
                               const int nc,
                               SbImage & dst)
{
  float dx, dy;
  int width = size[0];
  int height = size[1];
  unsigned char * dstptr = new unsigned char[width*height*3];
  unsigned char * dststore = dstptr;
  unsigned char red;
  SbVec3f n;

#define GET_PIXEL_RED(x_, y_) \
  srcptr[(y_)*width*nc + (x_)*nc]
  
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {      
      // do Y Sobel filter
      red = GET_PIXEL_RED((x-1+width) % width, (y+1) % height);
      dy  = ((float) red) / 255.0f * -1.0f;
      
      red = GET_PIXEL_RED(x % width, (y+1) % height);
      dy += ((float) red) / 255.0f * -2.0f;
      
      red = GET_PIXEL_RED((x+1) % width, (y+1) % height);
      dy += ((float) red) / 255.0f * -1.0f;
      
      red = GET_PIXEL_RED((x-1+width) % width, (y-1+height) % height);
      dy += ((float) red) / 255.0f *  1.0f;
      
      red = GET_PIXEL_RED(x % width, (y-1+height) % height);
      dy += ((float) red) / 255.0f *  2.0f;
      
      red = GET_PIXEL_RED((x+1) % width, (y-1+height) % height);
      dy += ((float) red) / 255.0f *  1.0f;
      
      // Do X Sobel filter
      red = GET_PIXEL_RED((x-1+width) % width, (y-1+height) % height);
      dx  = ((float) red) / 255.0f * -1.0f;
      
      red = GET_PIXEL_RED((x-1+width) % width,   y % height);
      dx += ((float) red) / 255.0f * -2.0f;
      
      red = GET_PIXEL_RED((x-1+width) % width, (y+1) % height);
      dx += ((float) red) / 255.0f * -1.0f;
            
      red = GET_PIXEL_RED((x+1) % width, (y-1+height) % height);
      dx += ((float) red) / 255.0f *  1.0f;
      
      red = GET_PIXEL_RED((x+1) % width,   y % height);
      dx += ((float) red) / 255.0f *  2.0f;
      
      red = GET_PIXEL_RED((x+1) % width, (y+1) % height);
      dx += ((float) red) / 255.0f *  1.0f;
            
      n[0] = -dx;
      n[1] = -dy;
      n[2] = 1.0f;
      n.normalize();
            
      *dstptr++ = (unsigned char) ((n[0]+1.0f) * 128.0f);
      *dstptr++ = (unsigned char) ((n[1]+1.0f) * 128.0f);
      *dstptr++ = (unsigned char) ((n[2]+1.0f) * 128.0f);
    }
  }      
#undef GET_PIXEL_RED
  dst.setValue(size, 3, dststore);
  delete[] dststore;
}


SO_NODE_SOURCE(SoBumpMap);

/*!
  Constructor.
*/
SoBumpMap::SoBumpMap(void)
{
  PRIVATE(this) = new SoBumpMapP;
  PRIVATE(this)->glimage = new SoGLImage;
  PRIVATE(this)->glimagevalid = FALSE;
  PRIVATE(this)->didconvert = FALSE;

  SO_NODE_INTERNAL_CONSTRUCTOR(SoBumpMap);

  SO_NODE_ADD_FIELD(filename, (""));
  SO_NODE_ADD_FIELD(image, (SbVec2s(0, 0), 0, NULL));
  SO_NODE_ADD_FIELD(wrapS, (REPEAT));
  SO_NODE_ADD_FIELD(wrapT, (REPEAT));

  SO_NODE_DEFINE_ENUM_VALUE(Wrap, REPEAT);
  SO_NODE_DEFINE_ENUM_VALUE(Wrap, CLAMP);

  SO_NODE_SET_SF_ENUM_TYPE(wrapS, Wrap);
  SO_NODE_SET_SF_ENUM_TYPE(wrapT, Wrap);

  // use field sensor for filename since we will load an image if
  // filename changes. This is a time-consuming task which should
  // not be done in notify().
  PRIVATE(this)->filenamesensor = new SoFieldSensor(filenameSensorCB, this);
  PRIVATE(this)->filenamesensor->setPriority(0);
  PRIVATE(this)->filenamesensor->attach(&this->filename);
}

/*!
  Destructor. Frees up internal resources used to store texture image
  data.
*/
SoBumpMap::~SoBumpMap()
{
  PRIVATE(this)->glimage->unref(NULL);
  delete PRIVATE(this)->filenamesensor;
  delete PRIVATE(this);
}

// Documented in superclass.
void
SoBumpMap::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoBumpMap, SO_FROM_COIN_2_2);

  SO_ENABLE(SoGLRenderAction, SoBumpMapElement);
}


// Documented in superclass. Overridden to check if texture file (if
// any) can be found and loaded.
SbBool
SoBumpMap::readInstance(SoInput * in, unsigned short flags)
{
  PRIVATE(this)->filenamesensor->detach();
  SbBool readOK = inherited::readInstance(in, flags);
  if (readOK && !filename.isDefault() && filename.getValue() != "") {
    if (!this->loadFilename()) {
      SoReadError::post(in, "Could not read texture file '%s'",
                        filename.getValue().getString());
    }
  }
  PRIVATE(this)->filenamesensor->attach(&this->filename);
  PRIVATE(this)->glimagevalid = FALSE;
  return readOK;
}

static SoGLImage::Wrap
bumpmap_translateWrap(const SoBumpMap::Wrap wrap)
{
  if (wrap == SoBumpMap::REPEAT) return SoGLImage::REPEAT;
  return SoGLImage::CLAMP;
}


// Documented in superclass.
void
SoBumpMap::GLRender(SoGLRenderAction * action)
{
  SoState * state = action->getState();

  const cc_glglue * glue = cc_glglue_instance(action->getCacheContext());

  if (cc_glglue_can_do_bumpmapping(glue)) {
    int nc;
    SbVec2s size;
    const unsigned char * bytes = this->image.getValue(size, nc);

    if (bytes && size != SbVec2s(0,0)) {
      if (!PRIVATE(this)->glimagevalid) {
        if (nc < 3) {
          if (!PRIVATE(this)->didconvert) {
            convert_heightmap_to_normalmap(bytes, size, nc, PRIVATE(this)->convertedheightmap);
            PRIVATE(this)->didconvert = TRUE;
          }
          bytes = PRIVATE(this)->convertedheightmap.getValue(size, nc);
        }
        PRIVATE(this)->glimage->setData(bytes, size, nc,
                                        bumpmap_translateWrap((Wrap)this->wrapS.getValue()),
                                        bumpmap_translateWrap((Wrap)this->wrapT.getValue()),
                                        1.0f); // max quality for bumpmaps
        PRIVATE(this)->glimagevalid = TRUE;
      }
      SoBumpMapElement::set(state, this, PRIVATE(this)->glimage);
    }
    else {
      SoBumpMapElement::set(state, this, NULL);
    }
  }
  else {
    static int didwarn = 0;
    if (!didwarn) {
      // FIXME: add link to bumpmapping doc on doc.coin3d.org. pederb, 2003-11-18
      SoDebugError::postWarning("SoBumpMap::GLRender",
                                "Your OpenGL driver does not support the "
                                "required extensions to do bumpmapping.");
      didwarn = 1;
    }
  }
}

// Documented in superclass.
void
SoBumpMap::doAction(SoAction * action)
{
  // will be implemented when (if) more actions support SoBumpMap.
}

// doc from parent
void
SoBumpMap::callback(SoCallbackAction * action)
{
  // not supported for SoCallbackAction yet
  // SoBumpMap::doAction(action);
}

// doc from parent
void
SoBumpMap::rayPick(SoRayPickAction * action)
{
  // not supported for SoRayPickAction yet
  // SoBumpMap::doAction(action);
}

// Documented in superclass. Overridden to detect when fields change.
void
SoBumpMap::notify(SoNotList * l)
{
  SoField * f = l->getLastField();
  if (f == &this->image) {
    // write image, not filename
    this->filename.setDefault(TRUE);
    this->image.setDefault(FALSE);
    PRIVATE(this)->didconvert = FALSE;
  }
  PRIVATE(this)->glimagevalid = FALSE;
  inherited::notify(l);
}

//
// Called from readInstance() or when user changes the
// filename field.
//
SbBool
SoBumpMap::loadFilename(void)
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
      PRIVATE(this)->didconvert = FALSE;
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
SoBumpMap::filenameSensorCB(void * data, SoSensor *)
{
  SoBumpMap * thisp = (SoBumpMap*) data;

  if (thisp->filename.getValue().getLength() &&
      !thisp->loadFilename()) {
    SoDebugError::postWarning("SoBumpMap::filenameSensorCB",
                              "Image file '%s' could not be read",
                              thisp->filename.getValue().getString());
  }
}

#undef PRIVATE
