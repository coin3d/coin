/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
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
  \class SbViewportRegion SbViewportRegion.h Inventor/SbViewportRegion.h
  \brief The SbViewportRegion class is a mapping from normalized screen space coordinates to window coordinates.
  \ingroup base

  The SbViewportRegion class contains information to represent
  a subview within a desktop window. Available methods include inquiries
  and manipulation in both normalized coordinates and pixel coordinates.

  \sa SbViewVolume
*/


#include <assert.h>
#include <Inventor/SbViewportRegion.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

/*!
  The default SbViewportRegion constructor initializes the viewport to a
  (100, 100) window with 72 pixels per inch resolution.
 */
SbViewportRegion::SbViewportRegion(void)
  : winsize(100, 100),
    vporigin(0.0f, 0.0f),
    vpsize(1.0f, 1.0f)
{
  this->pixperinch = 72.0f;
}

/*!
  Construct and initialize an SbViewportRegion instance with the
  given pixel value window dimensions.
 */
SbViewportRegion::SbViewportRegion(short width, short height)
  : winsize(width, height),
    vporigin(0,0),
    vpsize(1.0f, 1.0f)
{
#if COIN_DEBUG
  if (width<0) {
    SoDebugError::postWarning("SbViewportRegion::SbViewportRegion",
                              "width (%d) should be >=0. Clamped to 0.",
                              width);
    winsize[0]=0;
  }
  if (height<0) {
    SoDebugError::postWarning("SbViewportRegion::SbViewportRegion",
                              "height (%d) should be >=0. Clamped to 0.",
                              height);
    winsize[1]=0;
  }
#endif // COIN_DEBUG

  this->pixperinch = 72.0f;
}

/*!
  Construct and initialize an SbViewportRegion instance with the
  given pixel value window dimensions.
 */
SbViewportRegion::SbViewportRegion(SbVec2s winsize)
  : winsize(winsize),
    vporigin(0,0),
    vpsize(1.0f, 1.0f)
{
#if COIN_DEBUG
  if (winsize[0]<0) {
    SoDebugError::postWarning("SbViewportRegion::SbViewportRegion",
                              "winsize[0] (%d) should be >=0. Clamped to 0.",
                              winsize[0]);
    this->winsize[0]=0;
  }
  if (winsize[1]<0) {
    SoDebugError::postWarning("SbViewportRegion::SbViewportRegion",
                              "winsize[1] (%d) should be >=0. Clamped to 0.",
                              winsize[1]);
    this->winsize[1]=0;
  }
#endif // COIN_DEBUG

  this->pixperinch = 72.0f;
}

/*!
  Copy constructor.
 */
SbViewportRegion::SbViewportRegion(const SbViewportRegion& vpReg)
{
  *this = vpReg;
}

/*!
  Set the viewport region window size in pixels.

  \sa getWindowSize().
 */
void
SbViewportRegion::setWindowSize(short width, short height)
{
#if COIN_DEBUG
  if (width<0) {
    SoDebugError::postWarning("SbViewportRegion::setWindowSize",
                              "width (%d) should be >=0. Clamped to 0.",width);
    width=0;
  }
  if (height<0) {
    SoDebugError::postWarning("SbViewportRegion::setWindowSize",
                              "height (%d) should be >=0. Clamped to 0.",
                              height);
    height=0;
  }
#endif // COIN_DEBUG

  this->winsize.setValue(width, height);
}

/*!
  Set the viewport region window size in pixels.

  \sa getWindowSize().
 */
void
SbViewportRegion::setWindowSize(SbVec2s winsize)
{
#if COIN_DEBUG
  if (winsize[0]<0) {
    SoDebugError::postWarning("SbViewportRegion::setWindowSize",
                              "winsize[0] (%d) should be >=0. Clamped to 0.",
                              winsize[0]);
    winsize[0]=0;
  }
  if (winsize[1]<0) {
    SoDebugError::postWarning("SbViewportRegion::setWindowSize",
                              "winsize[1] (%d) should be >=0. Clamped to 0.",
                              winsize[1]);
    winsize[1]=0;
  }
#endif // COIN_DEBUG

  this->setWindowSize(winsize[0], winsize[1]);
}

/*!
  Set up the origin and size of the viewport region in normalized
  coordinates.

  \sa getViewportOrigin(), getViewportSize(), setViewportPixels().
 */
void
SbViewportRegion::setViewport(float left, float bottom,
                              float width, float height)
{
#if COIN_DEBUG
  if (width<0) {
    SoDebugError::postWarning("SbViewportRegion::setViewport",
                              "width (%d) should be >=0. Clamped to 0.",width);
    width=0;
  }
  if (height<0) {
    SoDebugError::postWarning("SbViewportRegion::setViewport",
                              "height (%d) should be >=0. Clamped to 0.",
                              height);
    height=0;
  }
#endif // COIN_DEBUG

  this->vporigin.setValue(left, bottom);
  this->vpsize.setValue(width, height);
}

/*!
  Set up the origin and size of the viewport region in normalized
  coordinates.

  \sa getViewportOrigin(), getViewportSize(), setViewportPixels().
 */
void
SbViewportRegion::setViewport(SbVec2f origin, SbVec2f size)
{
#if COIN_DEBUG
  if (size[0]<0) {
    SoDebugError::postWarning("SbViewportRegion::setViewport",
                              "size[0] (%d) should be >=0. Clamped to 0.",
                              size[0]);
    size[0]=0;
  }
  if (size[1]<0) {
    SoDebugError::postWarning("SbViewportRegion::setViewport",
                              "size[1] (%d) should be >=0. Clamped to 0.",
                              size[1]);
    size[1]=0;
  }
#endif // COIN_DEBUG

  this->setViewport(origin[0], origin[1], size[0], size[1]);
}

/*!
  Set up the origin and size of the viewport region in pixel
  coordinates.

  \sa getViewportOriginPixels(), getViewportSizePixels(), setViewport().
 */
void
SbViewportRegion::setViewportPixels(short left, short bottom,
                                    short width, short height)
{
#if COIN_DEBUG
  if (width<0) {
    SoDebugError::postWarning("SbViewportRegion::setViewportPixels",
                              "width (%d) should be >=0. Clamped to 0.",width);
    width=0;
  }
  if (height<0) {
    SoDebugError::postWarning("SbViewportRegion::setViewportPixels",
                              "height (%d) should be >=0. Clamped to 0.",
                              height);
    height=0;
  }
#endif // COIN_DEBUG

  this->vporigin.setValue(((float)left)/((float)this->winsize[0]),
                          ((float)bottom)/((float)this->winsize[1]));
  this->vpsize.setValue(((float)width)/((float)this->winsize[0]),
                        ((float)height)/((float)this->winsize[1]));
}

/*!
  Set up the origin and size of the viewport region in pixel
  coordinates.

  \sa getViewportOriginPixels(), getViewportSizePixels(), setViewport().
 */
void
SbViewportRegion::setViewportPixels(SbVec2s origin, SbVec2s size)
{
#if COIN_DEBUG
  if (size[0]<0) {
    SoDebugError::postWarning("SbViewportRegion::setViewportPixels",
                              "size[0] (%d) should be >=0. Clamped to 0.",
                              size[0]);
    size[0]=0;
  }
  if (size[1]<0) {
    SoDebugError::postWarning("SbViewportRegion::setViewportPixels",
                              "size[1] (%d) should be >=0. Clamped to 0.",
                              size[1]);
    size[1]=0;
  }
#endif // COIN_DEBUG
  this->setViewportPixels(origin[0], origin[1], size[0], size[1]);
}

/*!
  Returns window dimensions (which are in absolute (i.e. pixel)
  coordinates).

  \sa setWindowSize().
 */
const SbVec2s&
SbViewportRegion::getWindowSize(void) const
{
  return this->winsize;
}

/*!
  Return normalized viewport origin coordinates.

  \sa setViewport(), getViewportOriginPixels().
 */
const SbVec2f &
SbViewportRegion::getViewportOrigin(void) const
{
  return this->vporigin;
}

/*!
  Return viewport origin coordinates in pixel values.

  \sa setViewportPixels(), getViewportOrigin().
 */
const SbVec2s &
SbViewportRegion::getViewportOriginPixels(void) const
{
  // Cast away constness. Ugly.
  SbViewportRegion * thisp = (SbViewportRegion *)this;
  thisp->vporigin_s.setValue((short)(this->winsize[0] * this->vporigin[0]),
                             (short)(this->winsize[1] * this->vporigin[1]));
  return this->vporigin_s;
}

/*!
  Returns the normalized viewport size.

  \sa setViewport(), getViewportSizePixels().
 */
const SbVec2f &
SbViewportRegion::getViewportSize(void) const
{
  return this->vpsize;
}

/*!
  Returns viewport size in pixel coordinates.

  \sa setViewportPixels(), getViewportSize().
 */
const SbVec2s &
SbViewportRegion::getViewportSizePixels(void) const
{
  // Cast away constness. Ugly.
  SbViewportRegion * thisp = (SbViewportRegion *)this;
  thisp->vpsize_s = SbVec2s((short)(this->winsize[0] * this->vpsize[0]),
                            (short)(this->winsize[1] * this->vpsize[1]));
  return this->vpsize_s;
}

/*!
  Returns the aspect ratio of the viewport region. The aspect ratio is
  calculated as pixelwidth divided on pixelheight.
 */
float
SbViewportRegion::getViewportAspectRatio(void) const
{
  // FIXME: what if height == 0? 19981021 mortene.

  return
    ((float)(this->getViewportSizePixels()[0]))/
    ((float)(this->getViewportSizePixels()[1]));
}

/*!
  Scale the width of the viewport region.

  The scale factor should not make the viewport larger than the
  window. If this happens, the viewport will be clamped.

  The scaling will be done around the viewport region center point, but
  if this causes the viewport origin to be moved below (0,0), the
  origin coordinates will be clamped.

  \sa scaleHeight().  */
void
SbViewportRegion::scaleWidth(float ratio)
{
#if COIN_DEBUG
  if (ratio<0.0f) {
    SoDebugError::postWarning("SbViewportRegion::scaleWidth",
                              "ratio (%f) should be >=0.0f. Clamped to 0.0f.",
                              ratio);
    ratio=0.0f;
  }
#endif // COIN_DEBUG

  // Yes, I know the error checking and correction in this method is
  // pretty lousy, but this is the way SGI OI2.1 behaves.
  //FIXME: Check this. 19980910 kintel.

  float oldw = this->vpsize[0];
  this->vpsize[0] *= ratio;
  this->vporigin[0] -= (this->vpsize[0] - oldw) / 2.0f;

  if(this->vpsize[0] > 1.0f) this->vpsize[0] = 1.0f;
  if(this->vporigin[0] < 0.0f) this->vporigin[0] = 0.0f;
}

/*!
  Scale the height of the viewport region.

  The scale factor should not make the viewport larger than the
  window. If this happens, the viewport will be clamped.

  The scaling will be done around the viewport region center point, but
  if this causes the viewport origin to be moved below (0,0), the
  origin coordinates will be clamped.

  \sa scaleWidth().  */
void
SbViewportRegion::scaleHeight(float ratio)
{
#if COIN_DEBUG
  if (ratio<0.0f) {
    SoDebugError::postWarning("SbViewportRegion::scaleheight",
                              "ratio (%f) should be >=0.0f. Clamped to 0.0f.",
                              ratio);
    ratio=0.0f;
  }
#endif // COIN_DEBUG

  // Yes, I know the error checking and correction in this method is
  // pretty lousy, but this is the way SGI OI2.1 behaves.
  //FIXME: Check this. 19980910 kintel.

  float oldh = this->vpsize[1];
  this->vpsize[1] *= ratio;
  this->vporigin[1] -= (this->vpsize[1] - oldh) / 2.0f;

  if(this->vpsize[1] > 1.0f) this->vpsize[1] = 1.0f;
  if(this->vporigin[1] < 0.0f) this->vporigin[1] = 0.0f;
}

/*!
  Set pixels per inch. Default value is 72.

  \sa getPixelsPerInch().
 */
void
SbViewportRegion::setPixelsPerInch(float ppi)
{
#if COIN_DEBUG
  if (ppi<0.0f) {
    SoDebugError::postWarning("SbViewportRegion::setPixelsPerInch",
                              "ppi value (%f) should be >=0.0f. "
                              "Clamped to 0.0f.",ppi);
    ppi=0.0f;
  }
#endif // COIN_DEBUG

  this->pixperinch = ppi;
}

/*!
  Get pixels per inch.

  \sa setPixelsPerInch().
 */
float
SbViewportRegion::getPixelsPerInch(void) const
{
  return this->pixperinch;
}

/*!
  Get pixels per point. A \e point is defined as something you can put
  72 of per inch...

  \sa setPixelsPerInch(), getPixelsPerInch().
 */
float
SbViewportRegion::getPixelsPerPoint(void) const
{
  return this->pixperinch / 72.0f;
}

/*!
  \relates SbViewportRegion

  Compares two SbViewportRegion instances for equality.
 */
int
operator ==(const SbViewportRegion& reg1, const SbViewportRegion& reg2)
{
  return
    reg1.winsize == reg2.winsize &&
    reg1.getViewportOriginPixels() == reg2.getViewportOriginPixels() &&
    reg1.getViewportSizePixels() == reg2.getViewportSizePixels() &&
    reg1.pixperinch == reg2.pixperinch;
}

/*!
  Dump the state of this object to the \a file stream. Only works in
  debug version of library, method does nothing in an optimized compile.
 */
void
SbViewportRegion::print(FILE * fp) const
{
#if COIN_DEBUG
  fprintf( fp, "  winsize:     " );
  this->getWindowSize().print(fp);
  fprintf( fp, "\n" );
  fprintf( fp, "  vporigin:    " );
  this->getViewportOrigin().print(fp);
  fprintf( fp, "\n" );
  fprintf( fp, "  vporiginpix: " );
  this->getViewportOriginPixels().print(fp);
  fprintf( fp, "\n" );
  fprintf( fp, "  vpsize:      " );
  this->getViewportSize().print(fp);
  fprintf( fp, "\n" );
  fprintf( fp, "  vpsizepix:   " );
  this->getViewportSizePixels().print(fp);
  fprintf( fp, "\n" );
  fprintf( fp, "  aspectratio: %f\n", this->getViewportAspectRatio() );
  fprintf( fp, "  ppi:         %f\n", this->getPixelsPerInch() );
  fprintf( fp, "  ppp:         %f\n", this->getPixelsPerPoint() );
#endif // COIN_DEBUG
}
