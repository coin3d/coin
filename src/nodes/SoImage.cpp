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
  \class SoImage SoImage.h Inventor/nodes/SoImage.h
  \brief The SoImage class draws a 2D image on the viewport.
  \ingroup nodes

  An image can be specified either by using the image field, or by 
  specifying a filename. If width and or height is specified, the
  image will be resized to match those values before it is displayed.
  
  The current modelview matrix will be used to find the viewport
  position, and the image is rendered in that position, with 
  z-buffer testing activated.
*/

#include <Inventor/nodes/SoImage.h>
#include <Inventor/nodes/SoSubNodeP.h>

#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/misc/SoImageInterface.h>
#include <Inventor/SoInput.h>

#include <Inventor/elements/SoViewVolumeElement.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/elements/SoViewportRegionElement.h>
#include <Inventor/elements/SoTextureImageElement.h>

#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/SbBasic.h>

#ifdef _WIN32
#include <windows.h> // sigh...
#endif // !_WIN32
#include <GL/gl.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

// Metadon FIXME
/*¡
  What happens if the node contains an image and the user modifies
  the width and/or height and/or numcomponents values? 20000217 mortene.
 */

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
  If bigger than 0, resize image to this width.
*/
/*!
  \var SoSFInt32 SoImage::height
  if bigger than 0, resize image to this height.
*/
/*!
  \var SoSFEnum SoImage::vertAlignment
  Vertical alignment.
*/
/*!
  \var SoSFEnum SoImage::horAlignment
  Horizontal alignment.
*/
/*!
  \var SoSFImage SoImage::image
  Inline image data.
*/
/*!
  \var SoSFString SoImage::filename
  Image filename.
*/


// *************************************************************************

SO_NODE_SOURCE(SoImage);

/*!
  Constructor.
*/
SoImage::SoImage()
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

  this->imageData = NULL;
  this->orgImageData = NULL;
  this->readstatus = TRUE;
}

/*!
  Destructor.
*/
SoImage::~SoImage()
{
  if (this->imageData) this->imageData->unref();
  if (this->orgImageData) this->orgImageData->unref();
}

// doc from parent
void
SoImage::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoImage);
}

// doc from parent
void
SoImage::computeBBox(SoAction *action,
                     SbBox3f &box, SbVec3f &center)
{
  // FIXME: pederb, 20000509
  // need to invalidate the bbox-cache for this to work 100%,
  // since bbox changes position each time modelmatrix has changed.
  if (!this->getImage()) {
    box.setBounds(0,0,0,0,0,0);
    center = SbVec3f(0,0,0);
    return;
  }
  SbVec3f v0, v1, v2, v3;
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
SoImage::GLRender(SoGLRenderAction *action)
{
  if (!this->shouldGLRender(action)) return;

  SoState *state = action->getState();

  if (!this->getImage()) return;

  if (action->handleTransparency(this->imageData->hasTransparency())) return;

  const SbViewportRegion & vp = SoViewportRegionElement::get(state);
  SbVec2s vpsize = vp.getViewportSizePixels();

  SbVec3f nilpoint = SoImage::getNilpoint(state);
  SbVec2s size = this->imageData->getSize();

  float xpos = 0.0; // init unnecessary, but kills a compiler warning.
  switch (this->horAlignment.getValue()) {
  case SoImage::LEFT:
    xpos = nilpoint[0];
    break;
  case SoImage::RIGHT:
    xpos = nilpoint[0] - (float)size[0];
    break;
  case SoImage::CENTER:
    xpos = nilpoint[0] - (float)size[0]*0.5f;
    break;
#if COIN_DEBUG
  default:
    SoDebugError::post("SoImage::GLRender",
                       "value of horAlign field is invalid");
    break;
#endif // COIN_DEBUG
  }

  float ypos = 0.0; // init unnecessary, but kills a compiler warning.
  switch (this->vertAlignment.getValue()) {
  case SoImage::TOP:
    ypos = nilpoint[1] - (float)size[1];
    break;
  case SoImage::BOTTOM:
    ypos = nilpoint[1];
    break;
  case SoImage::HALF:
    ypos = nilpoint[1] - (float)size[1]*0.5f;
    break;
#if COIN_DEBUG
  default:
    SoDebugError::post("SoImage::GLRender",
                       "value of vertAlign field is invalid");
    break;
#endif // COIN_DEBUG
  }

  GLenum format = GL_LUMINANCE; // init unnecessary, but kills a compiler warning.
  switch (this->imageData->getNumComponents()) {
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

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();

  short srcw = size[0];
  short srch = size[1];
  
  if (xpos < 0.0f) {
    srcw += (short)xpos;
    xpos = 0;
  }
  if (ypos < 0.0f) {
    srch += (short)ypos;
    ypos = 0;
  }

  srcw = SbMin(srcw, (short)(vpsize[0]-(short)xpos));
  srch = SbMin(srch, (short)(vpsize[1]-(short)ypos));

  // FIXME: push raster state?
  glOrtho(0, vpsize[0], 0, vpsize[1], -1.0f, 1.0f);

  glRasterPos3f(xpos, ypos, -nilpoint[2]);
  glPixelStorei(GL_UNPACK_ROW_LENGTH, size[0]);
  glPixelStorei(GL_PACK_ROW_LENGTH, vpsize[0]); // needed?

  glDrawPixels(srcw, srch, format, GL_UNSIGNED_BYTE,
               (const GLvoid*)this->imageData->getDataPtr());

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
}

// doc from parent
void
SoImage::rayPick(SoRayPickAction * action)
{
  if (this->shouldRayPick(action)) {
    if (!this->getImage()) return;
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
SoImage::getPrimitiveCount(SoGetPrimitiveCountAction *action)
{
  if (this->shouldPrimitiveCount(action)) action->incNumImage();
}

/*!
  Will generate a textured quad, representing the image in 3D.
*/
void
SoImage::generatePrimitives(SoAction *action)
{
  if (!this->getImage()) return;

  SoState *state = action->getState();
  state->push();

  // not quite sure if I should do this, but this will enable
  // SoCallbackAction to get all data it needs to render
  // this quad correctly. pederb 19991131
  SoTextureImageElement::set(state, this,
                             this->imageData->getSize(),
                             this->imageData->getNumComponents(),
                             this->imageData->getDataPtr(),
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

/*!
  Overloaded to load image file.
*/
SbBool
SoImage::readInstance(SoInput * in, unsigned short flags)
{
  this->readstatus = FALSE;
  if (inherited::readInstance(in, flags)) {
    this->readstatus = this->getImage();
  }
  return this->readstatus;
}

/*!
  Returns \e TRUE if node was read ok.
*/
int
SoImage::getReadStatus(void)
{
  return this->readstatus;
}

/*!
  Set read status for this node.
*/
void
SoImage::setReadStatus(SbBool flag)
{
  this->readstatus = flag;
}

// loads and resizes image
SbBool
SoImage::getImage()
{
  if (this->imageData && (this->imageData->getSize() == this->getSize()))
    return TRUE;

  if (this->imageData) {
    this->imageData->unref();
    this->imageData = NULL;
  }

  if (this->orgImageData == NULL) { // load if not loaded
    SbVec2s size;
    int nc;
    const unsigned char * bytes = this->image.getValue(size, nc);

    if (bytes && size[0] > 0 && size[1] > 0 && nc > 0) {
      this->orgImageData = new SoImageInterface(size, nc, bytes);
      this->orgImageData->ref();
    }
    else if (this->filename.getValue().getLength()) {
      const SbStringList & dirlist = SoInput::getDirectories();
      const char * imgname = this->filename. getValue().getString();
      this->orgImageData =
        SoImageInterface::findOrCreateImage(imgname, dirlist);
      if (this->orgImageData) this->orgImageData->load();
    }
  }

  if (this->orgImageData && this->orgImageData->isLoaded()) {
    SbVec2s orgsize = this->orgImageData->getSize();
    SbVec2s size = this->getSize();

    if (orgsize != size) {
      this->imageData = this->orgImageData->imageCopy();
      this->imageData->ref();
      this->imageData->resize(size);
    }
    else {
      // safe to reuse image, don't forget to ref() though
      this->imageData = this->orgImageData;
      this->imageData->ref();
    }
  }
  return this->imageData != NULL;
}

/*!
  Returns the screen coordinates for the point in (0,0,0) projected
  onto the screen. The z-value is stored in the third (z) coordinate.
*/
SbVec3f
SoImage::getNilpoint(SoState *state)
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

  return nilpoint;
}

//
// calculates the quad in 3D
//
void
SoImage::getQuad(SoState *state, SbVec3f &v0, SbVec3f &v1,
                 SbVec3f &v2, SbVec3f &v3)
{
  assert(this->imageData);
  SbVec3f nilpoint(0.0f, 0.0f, 0.0f);
  const SbMatrix & mat = SoModelMatrixElement::get(state);
  mat.multVecMatrix(nilpoint, nilpoint);

  const SbViewVolume &vv = SoViewVolumeElement::get(state);

  SbVec3f screenpoint;
  vv.projectToScreen(nilpoint, screenpoint);

  const SbViewportRegion & vp = SoViewportRegionElement::get(state);
  SbVec2s vpsize = vp.getViewportSizePixels();

  // find normalized width and height of image
  float nw = (float)this->imageData->getSize()[0];
  nw /= (float)vpsize[0];
  float nh = (float)this->imageData->getSize()[1];
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
    n1[1] -= nw;
    n2[2] -= nw;
    n3[3] -= nw;
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

// returns requested on-screen size 
SbVec2s
SoImage::getSize() const
{
  assert(this->orgImageData);

  SbVec2s size = this->orgImageData->getSize();

  if (this->width.getValue() > 0) {
    size[0] = this->width.getValue();
  }
  if (this->height.getValue() > 0) {
    size[1] = this->height.getValue();
  }
  return size;
}
