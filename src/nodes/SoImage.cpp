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
  \class SoImage SoImage.h Inventor/nodes/SoImage.h
  \brief The SoImage class draws a 2D image on the viewport.
  \ingroup nodes

  An image can be specified either by using the image field, or by
  specifying a filename. If width and or height is specified, the
  image will be resized to match those values before it is displayed.

  The current modelview matrix will be used to find the viewport
  position, and the image is rendered in that position, with
  z-buffer testing activated.

  \since TGS Inventor 2.5
  \since Coin 1.0
*/

#include <Inventor/nodes/SoImage.h>
#include <Inventor/nodes/SoSubNodeP.h>

#include <Inventor/SbImage.h>
#include <Inventor/SbPlane.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/elements/SoTextureImageElement.h>
#include <Inventor/elements/SoViewVolumeElement.h>
#include <Inventor/elements/SoViewportRegionElement.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/errors/SoReadError.h>
#include <Inventor/lists/SbStringList.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/sensors/SoFieldSensor.h>

#include <../misc/simage_wrapper.h>

#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/system/gl.h>
#include <../misc/GLUWrapper.h>

/*!
  \enum SoImage::VertAlignment
  Vertical alignment for image.
*/
/*!
  \var SoImage::VertAlignment SoImage::BOTTOM
  Vertical alignment at bottom of image.
*/
/*!
  \var SoImage::VertAlignment SoImage::HALF
  Vertical alignment at center of image.
*/
/*!
  \var SoImage::VertAlignment SoImage::TOP
  Vertical alignment at top of image.
*/
/*!
  \enum SoImage::HorAlignment
  Horizontal alignment for image.
*/
/*!
  \var SoImage::HorAlignment SoImage::LEFT
  Horizontal alignment at left border.
*/
/*!
  \var SoImage::HorAlignment SoImage::CENTER
  Horizontal alignment at center of image.
*/
/*!
  \var SoImage::HorAlignment SoImage::RIGHT
  Horizontal alignment ar right border.
*/


/*!
  \var SoSFInt32 SoImage::width

  If bigger than 0, resize image to this width before rendering.
  Default value is -1 (ie "don't resize").
*/
/*!
  \var SoSFInt32 SoImage::height

  If bigger than 0, resize image to this height before rendering.
  Default value is -1 (ie "don't resize").
*/
/*!
  \var SoSFEnum SoImage::vertAlignment

  Vertical alignment. Default value is SoImage::BOTTOM.
*/
/*!
  \var SoSFEnum SoImage::horAlignment

  Horizontal alignment.  Default value is SoImage::LEFT.
*/
/*!
  \var SoSFImage SoImage::image

  Inline image data. Default empty.
*/
/*!
  \var SoSFString SoImage::filename

  Image filename. Default empty.
*/


// *************************************************************************

SO_NODE_SOURCE(SoImage);

/*!
  Constructor.
*/
SoImage::SoImage(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoImage);


  SO_NODE_ADD_FIELD(width, (-1));
  SO_NODE_ADD_FIELD(height, (-1));

  SO_NODE_ADD_FIELD(vertAlignment, (SoImage::BOTTOM));
  SO_NODE_ADD_FIELD(horAlignment, (SoImage::LEFT));
  SO_NODE_ADD_FIELD(image, (SbVec2s(0,0), 0, NULL));
  SO_NODE_ADD_FIELD(filename, (""));

  SO_NODE_DEFINE_ENUM_VALUE(VertAlignment, BOTTOM);
  SO_NODE_DEFINE_ENUM_VALUE(VertAlignment, HALF);
  SO_NODE_DEFINE_ENUM_VALUE(VertAlignment, TOP);
  SO_NODE_SET_SF_ENUM_TYPE(vertAlignment, VertAlignment);

  SO_NODE_DEFINE_ENUM_VALUE(HorAlignment, LEFT);
  SO_NODE_DEFINE_ENUM_VALUE(HorAlignment, CENTER);
  SO_NODE_DEFINE_ENUM_VALUE(HorAlignment, RIGHT);
  SO_NODE_SET_SF_ENUM_TYPE(horAlignment, HorAlignment);

  this->readstatus = TRUE;
  this->transparency = FALSE;
  this->testtransparency = FALSE;

  // use field sensor for filename since we will load an image if
  // filename changes. This is a time-consuming task which should
  // not be done in notify().
  this->filenamesensor = new SoFieldSensor(filenameSensorCB, this);
  this->filenamesensor->setPriority(0);
  this->filenamesensor->attach(&this->filename);
  this->resizedimage = new SbImage;
  this->resizedimagevalid = FALSE;
}

/*!
  Destructor.
*/
SoImage::~SoImage()
{
  delete this->resizedimage;
}

// doc from parent
void
SoImage::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoImage, SO_FROM_INVENTOR_2_5|SO_FROM_COIN_1_0);
}

// doc from parent
void
SoImage::computeBBox(SoAction * action,
                     SbBox3f & box, SbVec3f & center)
{
  // ignore if node is empty
  if (this->getSize() == SbVec2s(0,0)) return;

  SbVec3f v0, v1, v2, v3;
  // this will cause a cache dependency on the view volume,
  // model matrix and viewport.
  this->getQuad(action->getState(), v0, v1, v2, v3);

  box.makeEmpty();
  box.extendBy(v0);
  box.extendBy(v1);
  box.extendBy(v2);
  box.extendBy(v3);
  center = box.getCenter();
}

// doc from parent
void
SoImage::GLRender(SoGLRenderAction * action)
{
  SbVec2s size;
  int nc;
  const unsigned char * dataptr = this->getImage(size, nc);
  if (dataptr == NULL) return; // no image

  if (!this->shouldGLRender(action)) return;
  
  SoState *state = action->getState();
  this->testTransparency();
  if (action->handleTransparency(this->transparency)) return;

  const SbViewportRegion & vp = SoViewportRegionElement::get(state);
  SbVec2s vpsize = vp.getViewportSizePixels();

  SbVec3f nilpoint = SoImage::getNilpoint(state);

  int xpos = 0;
  switch (this->horAlignment.getValue()) {
  case SoImage::LEFT:
    xpos = (int)nilpoint[0];
    break;
  case SoImage::RIGHT:
    xpos = (int)nilpoint[0] - size[0];
    break;
  case SoImage::CENTER:
    xpos = (int)nilpoint[0] - (size[0]>>1);
    break;
#if COIN_DEBUG
  default:
    SoDebugError::post("SoImage::GLRender",
                       "value of horAlign field is invalid");
    break;
#endif // COIN_DEBUG
  }

  int ypos = 0;
  switch (this->vertAlignment.getValue()) {
  case SoImage::TOP:
    ypos = (int)nilpoint[1] - size[1];
    break;
  case SoImage::BOTTOM:
    ypos = (int)nilpoint[1];
    break;
  case SoImage::HALF:
    ypos = (int)nilpoint[1] - (size[1]>>1);
    break;
#if COIN_DEBUG
  default:
    SoDebugError::post("SoImage::GLRender",
                       "value of vertAlign field is invalid");
    break;
#endif // COIN_DEBUG
  }

  GLenum format = GL_LUMINANCE; // init unnecessary, but kills a compiler warning.
  switch (nc) {
  case 1:
    format = GL_LUMINANCE;
    break;
  case 2:
    format = GL_LUMINANCE_ALPHA;
    break;
  case 3:
    format = GL_RGB;
    break;
  case 4:
    format = GL_RGBA;
    break;
#if COIN_DEBUG
  default:
    assert(0 && "illegal numComponents");
    break;
#endif
  }

  int srcw = size[0];
  int srch = size[1];
  int skipx = 0;
  int skipy = 0;

  if (xpos >= vpsize[0]) return;
  else if (xpos < -size[0]) return;
  else if (xpos < 0) {
    srcw += xpos;
    skipx = -xpos;
    xpos = 0;
  }

  if (ypos > vpsize[1]) return;
  else if (ypos < -size[1]) return;
  else if (ypos < 0) {
    srch += ypos;
    skipy = -ypos;
    ypos = 0;
  }

  if (srcw > vpsize[0] - xpos) {
    srcw = vpsize[0]-xpos;
  }
  if (srch > vpsize[1] - ypos) {
    srch = vpsize[1]-ypos;
  }

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(0, vpsize[0], 0, vpsize[1], -1.0f, 1.0f);

  glRasterPos3f((float)xpos, (float)ypos, -nilpoint[2]);
  glPixelStorei(GL_UNPACK_ROW_LENGTH, size[0]);
  glPixelStorei(GL_UNPACK_SKIP_PIXELS, skipx);
  glPixelStorei(GL_UNPACK_SKIP_ROWS, skipy);
  glPixelStorei(GL_PACK_ROW_LENGTH, vpsize[0]);
  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  glDrawPixels(srcw, srch, format, GL_UNSIGNED_BYTE,
               (const GLvoid*) dataptr);
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();

  // we always restore pixel store values to default after use
  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
  glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
  glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
  glPixelStorei(GL_PACK_ROW_LENGTH, 0);
  glPixelStorei(GL_PACK_ALIGNMENT, 4);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
}

// doc from parent
void
SoImage::rayPick(SoRayPickAction * action)
{
  if (this->getSize() == SbVec2s(0,0)) return;

  if (this->shouldRayPick(action)) {
    this->computeObjectSpaceRay(action);

    SbVec3f intersection;
    SbVec3f barycentric;
    SbBool front;
    SbVec3f v0,v1,v2,v3;
    this->getQuad(action->getState(), v0, v1, v2, v3);

    if (action->intersect(v0, v1, v2,
                          intersection, barycentric, front)) {

      if (action->isBetweenPlanes(intersection))
        action->addIntersection(intersection);
    }
    else if (action->intersect(v0, v2, v3,
                               intersection, barycentric, front)) {

      if (action->isBetweenPlanes(intersection))
        action->addIntersection(intersection);
    }
  }
}

// doc from parent
void
SoImage::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  if (this->getSize() == SbVec2s(0,0)) return;

  if (this->shouldPrimitiveCount(action)) action->incNumImage();
}

/*!
  Will generate a textured quad, representing the image in 3D.
*/
void
SoImage::generatePrimitives(SoAction * action)
{
  if (this->getSize() == SbVec2s(0,0)) return;

  SoState *state = action->getState();
  state->push();

  // not quite sure if I should do this, but this will enable
  // SoCallbackAction to get all data it needs to render
  // this quad correctly. pederb 19991131

  // FIXME: We may need to explicitly turn on 2D texturing and turn off 
  // 3D texturing, but what is the correct way of doing that in this 
  // case? (kintel 20011118)

  SbVec2s size;
  int nc;
  const unsigned char * dataptr = this->getImage(size, nc);


  SoTextureImageElement::set(state, this,
                             size, nc, dataptr,
                             SoTextureImageElement::CLAMP,
                             SoTextureImageElement::CLAMP,
                             SoTextureImageElement::DECAL,
                             SbVec3f(0,0,0));
  SbVec3f v0, v1, v2, v3;
  this->getQuad(action->getState(), v0, v1, v2, v3);

  SbVec3f n = (v1-v0).cross(v2-v0);
  n.normalize();

  this->beginShape(action, SoShape::QUADS);
  SoPrimitiveVertex vertex;
  vertex.setNormal(n);

  vertex.setTextureCoords(SbVec2f(0,0));
  vertex.setPoint(v0);
  this->shapeVertex(&vertex);

  vertex.setTextureCoords(SbVec2f(1,0));
  vertex.setPoint(v1);
  this->shapeVertex(&vertex);

  vertex.setTextureCoords(SbVec2f(1,1));
  vertex.setPoint(v2);
  this->shapeVertex(&vertex);

  vertex.setTextureCoords(SbVec2f(0,1));
  vertex.setPoint(v3);
  this->shapeVertex(&vertex);

  this->endShape();

  state->pop();
}

// Documented in superclass.
SbBool
SoImage::readInstance(SoInput * in, unsigned short flags)
{
  // Overridden to load image file.

  this->filenamesensor->detach();
  SbBool readOK = inherited::readInstance(in, flags);
  this->setReadStatus(readOK);
  if (readOK && !filename.isDefault()) {
    if (!this->loadFilename()) {
      SoReadError::post(in, "Could not read image file '%s'",
                        filename.getValue().getString());
      this->setReadStatus(FALSE);
    }
  }
  this->filenamesensor->attach(&this->filename);
  return readOK;
}

// Documented in superclass.
void
SoImage::notify(SoNotList * list)
{
  SoField *f = list->getLastField();
  if (f == &this->image) {
    this->filename.setDefault(TRUE); // write image, not filename
    this->testtransparency = TRUE;
    this->resizedimagevalid = FALSE;
  }
  else if (f == &this->width || f == &this->height) {
    this->resizedimagevalid = FALSE;
  }
  SoNode::notify(list);
}

/*!
  Returns \e TRUE if node was read ok.
*/
int
SoImage::getReadStatus(void)
{
  return (int) this->readstatus;
}

/*!
  Set read status for this node.
*/
void
SoImage::setReadStatus(SbBool flag)
{
  this->readstatus = flag;
}

/*!
  Returns the screen coordinates for the point in (0,0,0) projected
  onto the screen. The z-value is stored in the third (z) coordinate.
*/
SbVec3f
SoImage::getNilpoint(SoState * state)
{
  SbVec3f nilpoint(0.0f, 0.0f, 0.0f);
  const SbMatrix & mat = SoModelMatrixElement::get(state);
  mat.multVecMatrix(nilpoint, nilpoint);

  const SbViewVolume &vv = SoViewVolumeElement::get(state);

  // this function will also modify the z-value of nilpoint
  // according to the view matrix
  vv.projectToScreen(nilpoint, nilpoint);

  const SbViewportRegion & vp = SoViewportRegionElement::get(state);
  SbVec2s vpsize = vp.getViewportSizePixels();
  nilpoint[0] = nilpoint[0] * float(vpsize[0]);
  nilpoint[1] = nilpoint[1] * float(vpsize[1]);
  // change z range from [0,1] to [-1,1]
  nilpoint[2] *= 2.0f;
  nilpoint[2] -= 1.0f;

  return nilpoint;
}

// Calculates the quad in 3D.
void
SoImage::getQuad(SoState * state, SbVec3f & v0, SbVec3f & v1,
                 SbVec3f & v2, SbVec3f & v3)
{
  SbVec3f nilpoint(0.0f, 0.0f, 0.0f);
  const SbMatrix & mat = SoModelMatrixElement::get(state);
  mat.multVecMatrix(nilpoint, nilpoint);

  const SbViewVolume &vv = SoViewVolumeElement::get(state);

  SbVec3f screenpoint;
  vv.projectToScreen(nilpoint, screenpoint);

  const SbViewportRegion & vp = SoViewportRegionElement::get(state);
  SbVec2s vpsize = vp.getViewportSizePixels();

  // find normalized width and height of image
  float nw = (float)this->getSize()[0];
  nw /= (float)vpsize[0];
  float nh = (float)this->getSize()[1];
  nh /= (float)vpsize[1];

  // need only half the width
  nw *= 0.5f;
  nh *= 0.5f;

  SbVec2f n0, n1, n2, n3;

  n0 = SbVec2f(screenpoint[0]-nw, screenpoint[1]-nh);
  n1 = SbVec2f(screenpoint[0]+nw, screenpoint[1]-nh);
  n2 = SbVec2f(screenpoint[0]+nw, screenpoint[1]+nh);
  n3 = SbVec2f(screenpoint[0]-nw, screenpoint[1]+nh);

  switch (this->horAlignment.getValue()) {
  case SoImage::LEFT:
    n0[0] += nw;
    n1[0] += nw;
    n2[0] += nw;
    n3[0] += nw;
    break;
  case SoImage::RIGHT:
    n0[0] -= nw;
    n1[0] -= nw;
    n2[0] -= nw;
    n3[0] -= nw;
    break;
  case SoImage::CENTER:
    break;
  default:
    assert(0 && "unknown alignment");
    break;
  }

  switch (this->vertAlignment.getValue()) {
  case SoImage::TOP:
    n0[1] -= nh;
    n1[1] -= nh;
    n2[1] -= nh;
    n3[1] -= nh;
    break;
  case SoImage::BOTTOM:
    n0[1] += nh;
    n1[1] += nh;
    n2[1] += nh;
    n3[1] += nh;
    break;
  case SoImage::HALF:
    break;
  default:
    assert(0 && "unknown alignment");
    break;
  }

  // get distance from nilpoint to camera plane
  float dist = -vv.getPlane(0.0f).getDistance(nilpoint);

  // find the four image points in the plane
  v0 = vv.getPlanePoint(dist, n0);
  v1 = vv.getPlanePoint(dist, n1);
  v2 = vv.getPlanePoint(dist, n2);
  v3 = vv.getPlanePoint(dist, n3);

  // transform back to object space
  SbMatrix inv = mat.inverse();
  inv.multVecMatrix(v0, v0);
  inv.multVecMatrix(v1, v1);
  inv.multVecMatrix(v2, v2);
  inv.multVecMatrix(v3, v3);
}

// Returns requested on-screen size.
SbVec2s
SoImage::getSize(void) const
{
  SbVec2s size;
  int nc;
  (void) this->image.getValue(size, nc);

  if (size[0] == 0 || size[1] == 0) return SbVec2s(0,0);

  if (this->width.getValue() > 0) {
    size[0] = this->width.getValue();
  }
  if (this->height.getValue() > 0) {
    size[1] = this->height.getValue();
  }
  return size;
}

const unsigned char *
SoImage::getImage(SbVec2s & size, int & nc)
{
  if (this->getSize() == SbVec2s(0,0)) {
    size = SbVec2s(0,0);
    nc = 0;
    return NULL;
  }

  if (this->width.getValue() >= 0 || this->height.getValue() >= 0) {
    if (!this->resizedimagevalid) {
      SbVec2s orgsize;
      const unsigned char * orgdata = this->image.getValue(orgsize, nc);
      SbVec2s newsize = this->getSize();   

      // simage version 1.1.1 has a pretty high quality resize
      // function. We prefer to use that to avoid using GLU, since
      // GLU might require a valid GL context for gluScale to work.
      // Also, there are lots of buggy GLU versions out there.
      if (simage_wrapper()->available &&
          simage_wrapper()->versionMatchesAtLeast(1,1,1) &&
          simage_wrapper()->simage_resize) {
        unsigned char * result = 
          simage_wrapper()->simage_resize((unsigned char*) orgdata, 
                                          int(orgsize[0]), int(orgsize[1]),
                                          nc, int(newsize[0]), int(newsize[1]));
        this->resizedimage->setValue(newsize, nc, result);
        simage_wrapper()->simage_free_image(result);
        this->resizedimagevalid = TRUE;
      }
      else if (GLUWrapper()->available) {
        this->resizedimage->setValue(newsize, nc, NULL);
        const unsigned char * rezdata = this->resizedimage->getValue(newsize, nc);
        GLenum format;
        switch (nc) {
        default: // avoid compiler warnings
        case 1: format = GL_LUMINANCE; break;
        case 2: format = GL_LUMINANCE_ALPHA; break;
        case 3: format = GL_RGB; break;
        case 4: format = GL_RGBA; break;
        }
        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
        glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
        glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
        glPixelStorei(GL_PACK_ROW_LENGTH, 0);
        glPixelStorei(GL_PACK_SKIP_PIXELS, 0);
        glPixelStorei(GL_PACK_SKIP_ROWS, 0);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        
        (void)GLUWrapper()->gluScaleImage(format,
                                          orgsize[0], orgsize[1],
                                          GL_UNSIGNED_BYTE, (void*) orgdata,
                                          newsize[0], newsize[1],
                                          GL_UNSIGNED_BYTE,
                                          (void*) rezdata);
        // restore to default
        glPixelStorei(GL_PACK_ALIGNMENT, 4);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
        this->resizedimagevalid = TRUE;
      }
#if COIN_DEBUG
      else {
        SoDebugError::postInfo("SoImage::getImage",
                               "No resize function found.");
      }
#endif // COIN_DEBUG
    }
    return this->resizedimage->getValue(size, nc);
  }
  return this->image.getValue(size, nc);
}

//
// check image data for transparency
//
void
SoImage::testTransparency(void)
{
  if (!this->testtransparency) return;
  this->testtransparency = FALSE;
  this->transparency = FALSE;
  SbVec2s size;
  int nc;
  const unsigned char * data = this->image.getValue(size, nc);

  if (nc == 2 || nc == 4) {
    int n = size[0] * size[1];
    const unsigned char * ptr = (unsigned char *) data + nc - 1;

    while (n) {
      if (*ptr != 255) break;
      ptr += nc;
      n--;
    }
    this->transparency = n > 0;
  }
}

//
// Called from readInstance() or when user changes the
// filename field.
//
SbBool
SoImage::loadFilename(void)
{
  SbBool retval = FALSE;
  if (this->filename.getValue().getLength()) {
    SbImage tmpimage;
    const SbStringList & sl = SoInput::getDirectories();
    if (tmpimage.readFile(this->filename.getValue(),
                          sl.getArrayPtr(), sl.getLength())) {
      int nc;
      SbVec2s size;
      const unsigned char * bytes = tmpimage.getValue(size, nc);
      // disable notification on image while setting data from filename
      // as a notify will cause a filename.setDefault(TRUE).
      SbBool oldnotify = this->image.enableNotify(FALSE);
      this->image.setValue(size, nc, bytes);
      this->image.enableNotify(oldnotify);
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
SoImage::filenameSensorCB(void * data, SoSensor *)
{
  SoImage * thisp = (SoImage*) data;
  thisp->setReadStatus(TRUE);
  if (thisp->filename.getValue().getLength() &&
      !thisp->loadFilename()) {
    SoDebugError::postWarning("SoImage::filenameSensorCB",
                              "could not read image file '%s'",
                              thisp->filename.getValue().getString());
    thisp->setReadStatus(FALSE);
  }
}
