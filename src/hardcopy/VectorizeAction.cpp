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
  \class SoVectorizeAction HardCopy/SoVectorizeAction.h
  \brief The SoVectorizeAction class is the base class for vectorizing Coin scene graphs.
  \ingroup hardcopy

  The main API for HardCopy support in Coin is the abstract class
  SoVectorizAction. SoVectorizeAction will extract geometry from an
  Inventor scene graph, and project the geometry onto a specified
  page.  Since postscript and other vector based file formats do not
  support z-buffer or depth clipping, all geometry is rendered using a
  simple painter's algorithm (geometry is sorted based on distance to
  camera).

  SoVectorizePSAction inherits SoVectorizeAction, and will output a
  Postscript file.

  SoVectorizeAction currently only has support for polygons, lines and
  text, but it should be pretty fast to add support for more geometry
  types such as images. Texture-mapped polygons is not supported,
  since this is not supported by the vector file formats, at least
  it's not supported in Postscript. Gouraud shading is not supported
  in the Postscript language (at least not for V2.0), but an
  approximation is implemeting using an algorithm that divides the
  triangle into several small (flat-shaded) triangles. The gouraud
  shading quality (the number of sub-triangles) is controlled by an
  epsilon value. The gouraud shading function is written by Frederic
  Delhoume (delhoume@ilog.fr), and is free (public domain)
  software. I've noticed some strange clipping problems when this
  function is used, so use with care.  It might be safest to just use
  flat shading.

  Typical use of SoVectorizePSAction is shown in the following piece
  of code:

  \code
  
  SoVectorizePSAction * ps = new SoVectorizePSAction;
  SoVectorOutput * out = ps->getOutput();
  
  if (!out->openFile("output.ps")) {
    return -1; // unable to open output file
  }
  
  // to enable gouraud shading. 0.1 is a nice epsilon value
  // ps->setGouraudThreshold(0.1f);

  // clear to white background. Not really necessary if you
  // want a white background
  ps->setBackgroundColor(TRUE, SbColor(1.0f, 1.0f, 1.0f));

  // select LANDSCAPE or PORTRAIT orientation
  ps->setOrientation(SoVectorizeAction::LANDSCAPE);

  // start creating a new page (A4 page, with 10mm border).  
  ps->beginPage(SbVec2f(10.0f, 10.0f), SbVec2f(190.0f, 277.0f));
  
  // There are also enums for A0-A10. Example:
  // ps->beginStandardPage(SoVectorizeAction::A4, 10.0f);

  // calibrate so that text and eventually images will have the
  // same size in the postscript file as on the monitor.
  ps->calibrate(viewer->getViewportRegion());  

  // apply action on the viewer scenegraph. Remember to use
  // SoSceneManager's scene graph so that the camera is included.
  ps->apply(viewer->getSceneManager()->getSceneGraph());

  // this will create the postscript file
  ps->endPage();

  // close file
  out->closeFile();

  delete ps;

  \endcode

  It is also possible to have several viewports and/or layers on a
  page. This is useful if your application has several layers of
  geometry, for instance some annotations in 2D on top of a 3D scene
  graph. To create several layers, the beginViewport() and
  endViewport() functions can be used. 

  FIXME: more doc.

*/

/*!
  \enum SoVectorizeAction::DimensionUnit

  The unit used by the scene graph.
*/

/*!
  \var SoVectorizeAction::DimensionUnit SoVectorizeAction::INCH
*/

/*!
  \var SoVectorizeAction::DimensionUnit SoVectorizeAction::MM
*/

/*!
  \var SoVectorizeAction::DimensionUnit SoVectorizeAction::METER
*/


/*!
  \enum SoVectorizeAction::Orientation 

  The drawing orientation.
*/

/*!
  \var SoVectorizeAction::Orientation SoVectorizeAction::PORTRAIT
*/
/*!
  \var SoVectorizeAction::Orientation SoVectorizeAction::LANDSCAPE
*/

/*!
  \enum SoVectorizeAction::PageSize

  The size of the destination page.
*/


/*!
  \var SoVectorizeAction::PageSize SoVectorizeAction::A0
  841 x 1189 mm.
*/

/*!
  \var SoVectorizeAction::PageSize SoVectorizeAction::A1
  594 x 841 mm.
*/

/*!
  \var SoVectorizeAction::PageSize SoVectorizeAction::A2
  420 x 594 mm.
*/

/*!
  \var SoVectorizeAction::PageSize SoVectorizeAction::A3
  297 x 420 mm.
*/

/*!
  \var SoVectorizeAction::PageSize SoVectorizeAction::A4
  210 x 297 mm.
*/

/*!
  \var SoVectorizeAction::PageSize SoVectorizeAction::A5
  148 x 210 mm.
*/

/*!
  \var SoVectorizeAction::PageSize SoVectorizeAction::A6
  105 x 148 mm.
*/

/*!
  \var SoVectorizeAction::PageSize SoVectorizeAction::A7
  74 x 105 mm.
*/

/*!
  \var SoVectorizeAction::PageSize SoVectorizeAction::A8
  52 x 74 mm.
*/

/*!
  \var SoVectorizeAction::PageSize SoVectorizeAction::A9
  37 x 52 mm.
*/

/*!
  \var SoVectorizeAction::PageSize SoVectorizeAction::A10
  26 x 37 mm.
*/

/*! 
  \enum SoVectorizeAction::PointStyle
  Enumerates point rendering styles.
*/

/*!
  \var SoVectorizeAction::PointStyle SoVectorizeAction::CIRCLE

  Render points as circles.
*/

/*!
  \var SoVectorizeAction::PointStyle SoVectorizeAction::SQUARE

  Render points as squares.
*/

/*!
  \fn void SoVectorizeAction::printHeader(void) const
  \COININTERNAL
*/

// *************************************************************************

#include <HardCopy/SoVectorizeAction.h>
#include "VectorizeActionP.h"

#include <Inventor/SbViewportRegion.h>

// *************************************************************************

#define PRIVATE(p) (p->pimpl)
#define PUBLIC(p) (p->publ)

// *************************************************************************

SO_ACTION_SOURCE(SoVectorizeAction);

// *************************************************************************

// Doc in parent 
void
SoVectorizeAction::initClass(void)
{
   SO_ACTION_INIT_CLASS(SoVectorizeAction, SoCallbackAction);
}

/*!
  Default constructor.
*/
SoVectorizeAction::SoVectorizeAction(void)
{
  PRIVATE(this) = new SoVectorizeActionP(this);
  SO_ACTION_CONSTRUCTOR(SoVectorizeAction);
}

/*!
  Destructor.
*/
SoVectorizeAction::~SoVectorizeAction()
{
  delete PRIVATE(this);
}

// *************************************************************************

/*!  
  Returns the SoVectorOutput class used by this action. The output
  is written to stdout by default, but you can change this by using
  SoVectorOutput::openFile().
*/
SoVectorOutput *
SoVectorizeAction::getOutput(void) const
{
  if (PRIVATE(this)->output == NULL) {
    PRIVATE(this)->output = new SoVectorOutput;
  }
  return PRIVATE(this)->output;
}

// doc in parent
void
SoVectorizeAction::apply(SoNode * node)
{
  inherited::apply(node);
}

// doc in parent
void
SoVectorizeAction::apply(SoPath * path)
{
  inherited::apply(path);
}

// doc in parent
void
SoVectorizeAction::apply(const SoPathList & pathlist, SbBool obeysrules)
{
  inherited::apply(pathlist, obeysrules);
}

/*!

  Begin writing a page. This will write file header information and
  print background (if enabled) and border.

*/
void
SoVectorizeAction::beginPage(const SbVec2f & startpagepos,
                             const SbVec2f & pagesize,
                             DimensionUnit u)
{
  PRIVATE(this)->page.startpos = to_mm(startpagepos, u);
  PRIVATE(this)->page.size = to_mm(pagesize, u);

  PRIVATE(this)->viewport.startpos = to_mm(startpagepos, u);
  PRIVATE(this)->viewport.size = to_mm(pagesize, u);

  // set up a viewport so that the aspect ratio will match the page
  SbVec2f s = this->getRotatedViewportSize();
  float m = SbMax(s[0], s[1]);
  
  SbVec2s ss;  
  ss[0] = (short) ((s[0] / m) * 32767.0f);
  ss[1] = (short) ((s[1] / m) * 32767.0f);
  this->setViewportRegion(SbViewportRegion(ss));

  // print header and page information
  this->printHeader();
  this->beginViewport(startpagepos, pagesize, u);
  if (PRIVATE(this)->background.on) this->printBackground();
}

/*!
  End page. This will write all remaining geometry, and write the file footer.
*/

void
SoVectorizeAction::endPage(void)
{
  this->endViewport();
  this->printFooter();
}

/*!
  Begin writing a viewport inside the current page.

  \sa beginPage(), endViewport()
*/
void 
SoVectorizeAction::beginViewport(const SbVec2f & start, const SbVec2f & size,
                                 DimensionUnit u)
{
  if (start[0] < 0.0f || start[1] < 0.0f) {
    PRIVATE(this)->viewport.startpos = PRIVATE(this)->page.startpos;
  }
  else {
    PRIVATE(this)->viewport.startpos = to_mm(start, u);
  }
  if (size[0] <= 0.0f || size[1] <= 0.0f) {
    PRIVATE(this)->viewport.size = PRIVATE(this)->page.size;
  }
  else {
    PRIVATE(this)->viewport.size = to_mm(size, u);
  }
  PRIVATE(this)->reset();

  // this will set up clipping (for postscript, at least)
  this->printViewport();

  // set up a SbViewportRegion (used by SoCallbackAction) so that the
  // aspect ratio will match the page
  SbVec2f s = this->getRotatedViewportSize();
  float m = SbMax(s[0], s[1]);
  
  SbVec2s ss;  
  ss[0] = (short) ((s[0] / m) * 32767.0f);
  ss[1] = (short) ((s[1] / m) * 32767.0f);
  this->setViewportRegion(SbViewportRegion(ss));
}

/*!
  End writing a viewport. This will flush all vector items.
*/
void 
SoVectorizeAction::endViewport(void)
{
  if (PRIVATE(this)->itemlist.getLength()) {
    PRIVATE(this)->outputItems();
    PRIVATE(this)->reset();
  }
}

/*!  
  Will calibrate pixel based attributes (font size, line width,
  points size, etc) so that it will match OpenGL rendering done in
  \a vp.

  \sa setPixelSize()
  \sa setNominalWidth()
*/
void 
SoVectorizeAction::calibrate(const SbViewportRegion & vp)
{
  SbVec2s vpsize = vp.getViewportSizePixels();  
  PRIVATE(this)->pixelimagesize = this->getPageSize()[1] / float(vpsize[1]);
  PRIVATE(this)->nominalwidth = this->getPageSize()[1] / float(vpsize[1]);
}

/*!
  Sets the orientation to \a o.
*/
void
SoVectorizeAction::setOrientation(Orientation o)
{
  PRIVATE(this)->orientation = o;
}

/*!
  Returns the current orientation.

  \sa setOrientation()
*/

SoVectorizeAction::Orientation
SoVectorizeAction::getOrientation(void) const
{
  return PRIVATE(this)->orientation;
}

/*!
  Sets the background color. If \a bg is \e FALSE, the background will
  not be cleared before rendering. If \a bg is \e TRUE, the background
  will be cleared to \a col before in beginPage().
*/
void
SoVectorizeAction::setBackgroundColor(SbBool bg, const SbColor & col)
{
  PRIVATE(this)->background.on = bg;
  PRIVATE(this)->background.color = col;
}

/*!
  Returns if the background will be cleared or not. When this function
  returns \e TRUE, \a col will be set to the background color.
*/
SbBool
SoVectorizeAction::getBackgroundColor(SbColor & col) const
{
  col = PRIVATE(this)->background.color;
  return PRIVATE(this)->background.on;
}

/*!  
  Sets how to convert pixel based attributes (line width and point
  size) to vector sizes. By default 1 pixel equals 0.35 mm.

  \sa calibrate()
*/
void
SoVectorizeAction::setNominalWidth(float w, DimensionUnit u)
{
  PRIVATE(this)->nominalwidth = to_mm(w, u);
}

float
SoVectorizeAction::getNominalWidth(DimensionUnit u) const
{
  return from_mm(PRIVATE(this)->nominalwidth, u);
}

/*!  
  Sets how the images and 2D fonts are converted. By default 1
  pixel equals 0.35 mm.


  \sa calibrate()
*/
void
SoVectorizeAction::setPixelImageSize(float w, DimensionUnit u)
{
  PRIVATE(this)->pixelimagesize = to_mm(w, u);
}

/*!
  Returns the pixel image size.

  \sa setPixelImageSize()
*/
float
SoVectorizeAction::getPixelImageSize(DimensionUnit u) const
{
  return from_mm(PRIVATE(this)->pixelimagesize, u);
}

/*!
  Sets the points rendering style. Default style is CIRCLE.
*/
void 
SoVectorizeAction::setPointStyle(const PointStyle & style)
{
  PRIVATE(this)->pointstyle = style;
}

/*!
  Returns the points rendering style.
*/
SoVectorizeAction::PointStyle 
SoVectorizeAction::getPointStyle(void) const
{
  return PRIVATE(this)->pointstyle;
}


/*!
  \COININTERNAL

  Should be overridden by subclasses to print file footer data.
 */
void
SoVectorizeAction::printFooter(void) const
{
}

/*!
  \COININTERNAL

  Should be overridden by subclasses to print background data.
*/
void
SoVectorizeAction::printBackground(void) const
{
}

/*!
  \COININTERNAL

  Should be overridden by subclasses to set up the current page viewport.
*/
void 
SoVectorizeAction::printViewport(void) const
{

}

/*!
  \COININTERNAL

  Should be overridden by subclasses to print an item.
*/
void
SoVectorizeAction::printItem(const SoVectorizeItem * item) const
{
}

/*!
  Convenience method for subclasses. Will return the viewport startpos,
  taking the orientation into account
*/
SbVec2f 
SoVectorizeAction::getRotatedViewportStartpos(void) const
{
  SbVec2f p = PRIVATE(this)->viewport.startpos;

  if (this->getOrientation() == LANDSCAPE) SbSwap(p[0], p[1]);
  return p;
}


/*!
  Convenience method for subclasses. Will return the viewport size,
  taking the orientation into account
*/
SbVec2f 
SoVectorizeAction::getRotatedViewportSize(void) const
{
  SbVec2f p = PRIVATE(this)->viewport.size;

  if (this->getOrientation() == LANDSCAPE) SbSwap(p[0], p[1]);
  return p;
}

/*!
  Should be used by subclasses to set the SoVectorOutput
  instance that should be used.
*/
void 
SoVectorizeAction::setOutput(SoVectorOutput * output)
{
  if (PRIVATE(this)->output) {
    delete PRIVATE(this)->output;
  }
  PRIVATE(this)->output = output;
}

/*!
  Converts pixels to normalized units.
 */
float 
SoVectorizeAction::pixelsToUnits(const int pixels)
{
  float mm = this->getPixelImageSize() * pixels;
  float ps = this->getPageSize()[1];
  return mm / ps;
}

/*!
  Returns the current page startpos.
*/
const SbVec2f &
SoVectorizeAction::getPageStartpos(void) const
{
  return PRIVATE(this)->page.startpos;
}

/*!
  Returns the current page size.
*/
const SbVec2f &
SoVectorizeAction::getPageSize(void) const
{
  return PRIVATE(this)->page.size;
}

/*!
  Returns the bps tree used to store triangle and line vertices.
*/
const SbBSPTree &
SoVectorizeAction::getBSPTree(void) const
{
  return PRIVATE(this)->bsp;
}

void 
SoVectorizeAction::beginStandardPage(const PageSize & pagesize, const float border)
{
  static int a_table[] = {
    // A0 - A10
    841, 1189,
    594, 841,
    420, 594,
    297, 420,
    210, 297,
    148, 210,
    105, 148,
    74, 105,
    52, 74,
    37, 52,
    26, 37,
    // B0 - B10 (might add later)
  };
  
  int idx = (int) pagesize;
  assert(idx >=0 && idx <= 10);
  
  float xdim = (float) a_table[idx*2];
  float ydim = (float) a_table[idx*2+1];
  
  this->beginPage(SbVec2f(border, border), SbVec2f(xdim-2.0f*border, ydim-2.0f*border), MM);
}

/*!
  Sets the drawing dimensions. You can use this and setStartPosition() instead
  of using beginViewport(). Provided for TGS OIV compatibility.

  \sa beginViewport()
*/
void 
SoVectorizeAction::setDrawingDimensions(const SbVec2f & d, DimensionUnit u)
{
  PRIVATE(this)->viewport.size = to_mm(d, u);
}

/*!
  Returns the current drawing dimensions.
*/
SbVec2f 
SoVectorizeAction::getDrawingDimensions(DimensionUnit u) const
{
  return from_mm(PRIVATE(this)->viewport.size, u);
}

/*!
  Sets the drawing staring position. You can use this and setDrawingDimensions() instead
  of using beginViewport(). Provided for TGS OIV compatibility.
*/
void 
SoVectorizeAction::setStartPosition(const SbVec2f & p, DimensionUnit u)
{
  PRIVATE(this)->viewport.startpos = to_mm(p, u);
  
}

/*!
  Returns the current drawing starting position.
*/
SbVec2f 
SoVectorizeAction::getStartPosition(DimensionUnit u) const
{
  return from_mm(PRIVATE(this)->viewport.startpos, u);
}

/*!
  Not implemented yet. Provided for TGS OIV compatibility.
*/
void 
SoVectorizeAction::setColorTranslationMethod(ColorTranslationMethod method)
{
}

/*!
  Not implemented yet. Provided for TGS OIV compatibility.
*/
SoVectorizeAction::ColorTranslationMethod 
SoVectorizeAction::getColorTranslationMethod(void) const
{
  return AS_IS;
}

/*!
  Not implemented yet. Provided for TGS OIV compatibility.
*/
void 
SoVectorizeAction::setLineEndStyle(EndLineStyle style)
{
}

/*!
  Not implemented yet. Provided for TGS OIV compatibility.
*/
SoVectorizeAction::EndLineStyle 
SoVectorizeAction::getLineEndStyle(void) const
{
  return BUTT_END;
}

/*!
  Not implemented yet. Provided for TGS OIV compatibility.
*/
void 
SoVectorizeAction::setLineJoinsStyle(JoinLineStyle style)
{
}

/*!
  Not implemented yet. Provided for TGS OIV compatibility.
*/
SoVectorizeAction::JoinLineStyle 
SoVectorizeAction::getLineJoinsStyle(void) const
{
  return NO_JOIN;
}

/*!
  Not implemented yet. Provided for TGS OIV compatibility.
*/
void 
SoVectorizeAction::setHLHSRMode(HLHSRMode mode)
{
}

/*!
  Not implemented yet. Provided for TGS OIV compatibility.
*/
SoVectorizeAction::HLHSRMode 
SoVectorizeAction::getHLHSRMode(void) const
{
  return HLHSR_PAINTER;
}
  
/*!
  Not implemented yet. Provided for TGS OIV compatibility.
*/
void 
SoVectorizeAction::setBorder(float width)
{
}

/*!
  Not implemented yet. Provided for TGS OIV compatibility.
*/
void 
SoVectorizeAction::setBorder (float width, SbColor color)
{
}
  
/*!
  Not implemented yet. Provided for TGS OIV compatibility.
*/
void 
SoVectorizeAction::setMiterLimit(float limit)
{
}

float 
SoVectorizeAction::getMiterLimit(void) const
{
  return 0.0f;
}

/*!
  Not implemented yet. Provided for TGS OIV compatibility.
*/
void 
SoVectorizeAction::setPenDescription(int num_pens, 
                                     const SbColor* colors, 
                                     const float * widths, 
                                     DimensionUnit u)
{
}

/*!
  Not implemented yet. Provided for TGS OIV compatibility.
*/
void 
SoVectorizeAction::getPenDescription(SbColor * colors, 
                                     float * widths, 
                                     DimensionUnit u) const
{
}

/*!
  Not implemented yet. Provided for TGS OIV compatibility.
*/
int 
SoVectorizeAction::getPenNum(void) const
{
  return 0;
}

/*!
  Not implemented yet. Provided for TGS OIV compatibility.
*/
void 
SoVectorizeAction::setColorPriority(SbBool priority)
{
}

/*!
  Not implemented yet. Provided for TGS OIV compatibility.
*/
SbBool 
SoVectorizeAction::getColorPriority(void) const
{
  return FALSE;
}

/*!
  Not implemented yet. Provided for TGS OIV compatibility.
*/
void 
SoVectorizeAction::enableLighting(SbBool flag)
{
}
/*!
  Not implemented yet. Provided for TGS OIV compatibility.
*/
SbBool 
SoVectorizeAction::isLightingEnabled(void) const
{
  return TRUE;
}


#undef PRIVATE


// *************************************************************************
