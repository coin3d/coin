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
  \class SoSoOffscreenRenderer SoOffscreenRenderer.h Inventor/SoOffscreenRenderer.h
  \brief The SoOffscreenRenderer class is used for rendering scenes in offscreen buffers.
  \ingroup general

  If you want to render to a memory buffer instead of an on-screen
  OpenGL context, use this class. Rendering to a memory buffer can be
  used to generate texture maps on-the-fly, or for saving snapshots of
  the scene to disk files (as pixel bitmaps or as Postscript files for
  sending to a Postscript-capable printer).

  Currently offscreen rendering can only be done with GLX (i.e. OpenGL
  on X11).

 */

#include <Inventor/SoOffscreenRenderer.h>
#include <Inventor/SoPath.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/nodes/SoNode.h>
#ifdef _WIN32
#include <windows.h> // Need this to resolve everything for <GL/gl.h>.
#endif // _WIN32
#include <GL/gl.h>
#include <assert.h>
#include <string.h> // memset(), memcpy()
#include <config.h>
#include <coindefs.h> // COIN_STUB()
#include <Inventor/errors/SoDebugError.h>


#ifndef DOXYGEN_SKIP_THIS // Don't document internal classes.

class SoOffscreenInternalData {
public:
  SoOffscreenInternalData(void) {
    this->buffersize = SbVec2s(0, 0);
  }
  virtual ~SoOffscreenInternalData() {
  }

  // Return FALSE if the necessary resource for rendering are not
  // available.
  virtual SbBool makeContextCurrent(void) = 0;

  virtual unsigned char * getBuffer(void) = 0;

  virtual void setBufferSize(const SbVec2s & size) {
    SbVec2s maxsize = this->getMax();

#if COIN_DEBUG
    if (size[0] > maxsize[0]) {
      SoDebugError::postWarning("SoOffscreenInternalData::setBufferSize",
                                "can't set width to %d, maximum is %d",
                                size[0], maxsize[0]);
    }
    else if (size[0] <= 0) {
      SoDebugError::postWarning("SoOffscreenInternalData::setBufferSize",
                                "can't set negative width %d", size[0]);
    }
    if (size[1] > maxsize[1]) {
      SoDebugError::postWarning("SoOffscreenInternalData::setBufferSize",
                                "can't set height to %d, maximum is %d",
                                size[1], maxsize[1]);
    }
    else if (size[1] <= 0) {
      SoDebugError::postWarning("SoOffscreenInternalData::setBufferSize",
                                "can't set negative height %d", size[1]);
    }
#endif // COIN_DEBUG

    // The explicit casts are done to humour the HPUX aCC compiler,
    // which will otherwise say ``Template deduction failed to find a
    // match for the call to 'SbMin'''. mortene.
    this->buffersize[0] = SbMax((short)1,
				SbMin((short)size[0], (short)maxsize[0]));
    this->buffersize[1] = SbMax((short)1,
				SbMin((short)size[1], (short)maxsize[1]));
  }

  SbVec2s getSize(void) const {
    return this->buffersize;
  }

  // Will be called right after a render operation has taken
  // place. Default method does nothing.
  virtual void postRender(void) { }

protected:
  SbVec2s buffersize;

  // Note: should _really_ be overloaded by subclasses.
  virtual SbVec2s getMax(void) { return SbVec2s(32767, 32767); }
};


#if HAVE_GLX
#include <GL/glx.h>

class SoOffscreenGLXData : public SoOffscreenInternalData {
public:
  SoOffscreenGLXData(void)
    // set everything to NULL first to gracefully handle error conditions
    : display(NULL), visinfo(NULL), context(NULL), pixmap(0), glxpixmap(0)
  {
    this->buffer = NULL;

    this->display = XOpenDisplay(NULL);
    if (!this->display) {
      SoDebugError::postWarning("SoOffscreenGLXData::SoOffscreenGLXData",
                                "Couldn't connect to X11 DISPLAY.");
      return;
    }

    // FIXME: should fallback to less and less restrictive attribs
    // settings if the glXChooseVisual() below fails. 20000622 mortene.
    static int attribs[] = {
      GLX_RGBA,
      GLX_DEPTH_SIZE, 1,
      GLX_STENCIL_SIZE, 1,
      None
    };

    // FIXME: using glXChooseVisual() like this picks a 8-bit
    // PseudoColor visual on SGI IRIX X11 servers with default
    // configuration settings. Should instead use some logic based on
    // XMatchVisualInfo() to find the best (i.e. deepest)
    // visual. This is Bugzilla #129. 20000622 mortene.

    this->visinfo = glXChooseVisual(this->display,
                                    DefaultScreen(this->display),
                                    attribs);
    if (!this->visinfo) {
      SoDebugError::postWarning("SoOffscreenGLXData::SoOffscreenGLXData",
                                "Couldn't get RGBA X11 visual.");
      return;
    }

    this->context = glXCreateContext(this->display, this->visinfo,
                                     0, GL_FALSE);
    if (!this->context) {
      SoDebugError::postWarning("SoOffscreenGLXData::SoOffscreenGLXData",
                                "Couldn't create GLX context.");
    }
  }

  virtual ~SoOffscreenGLXData() {
    if (this->context) glXDestroyContext(this->display, this->context);
    if (this->glxpixmap) glXDestroyGLXPixmap(this->display, this->glxpixmap);
    if (this->pixmap) XFreePixmap(this->display, this->pixmap);
    if (this->visinfo) XFree(this->visinfo);
    // FIXME: there's a memory leak here, but leaving this in causes a
    // crash when running remotely from Mesa on RH Linux 6.2 to OpenGL
    // on IRIX6.5. Investigate further. 20000705 mortene.
//      if (this->display) XCloseDisplay(this->display);

    delete[] this->buffer;
  }

  virtual void setBufferSize(const SbVec2s & size) {
    SoOffscreenInternalData::setBufferSize(size);

    delete[] this->buffer;
    this->buffer =
      new unsigned char[this->buffersize[0] * this->buffersize[1] * 4];

    if (this->pixmap) XFreePixmap(this->display, this->pixmap);
    if (this->glxpixmap) glXDestroyGLXPixmap(this->display, this->glxpixmap);

    if (this->context) {
      this->pixmap = XCreatePixmap(this->display,
                                   DefaultRootWindow(this->display),
                                   size[0], size[1],
                                   this->visinfo->depth);
      if (!this->pixmap) {
        SoDebugError::postWarning("SoOffscreenGLXData::SoOffscreenGLXData",
                                  "Couldn't create %dx%dx%d Pixmap.");
      }
      else {
        this->glxpixmap = glXCreateGLXPixmap(this->display, this->visinfo,
                                             this->pixmap);
        if (!this->glxpixmap) {
          SoDebugError::postWarning("SoOffscreenGLXData::SoOffscreenGLXData",
                                    "Couldn't create GLX Pixmap.");
        }
      }

    }
  }

  virtual SbBool makeContextCurrent(void) {
    assert(this->buffer);
    if (this->context && this->glxpixmap) {
      glXMakeCurrent(this->display, this->glxpixmap, this->context);
      return TRUE;
    }
    return FALSE;
  }

  virtual unsigned char * getBuffer(void) {
    return this->buffer;
  }

  virtual void postRender(void) {
    SbVec2s size = this->getSize();

    if (this->context && this->buffer) {
      glPixelStorei(GL_PACK_ALIGNMENT, 1);
      glReadPixels(0, 0, size[0], size[1], GL_RGBA, GL_UNSIGNED_BYTE,
                   this->buffer);
    }
  }

  static SbVec2s getMaxDimensions(void) {
    // FIXME: where can we get hold of the _real_ max values for
    // Pixmap and/or GLXPixmap? 20000417 mortene.
    return SbVec2s(32767, 32767);
  }


private:
  virtual SbVec2s getMax(void) {
    return SoOffscreenGLXData::getMaxDimensions();
  }

  unsigned char * buffer;
  Display * display;
  XVisualInfo * visinfo;
  GLXContext context;
  Pixmap pixmap;
  GLXPixmap glxpixmap;
};

#endif // HAVE_GLX

#endif // DOXYGEN_SKIP_THIS



/*!
  Constructor. Argument is the \a viewportregion we should use when
  rendering. An internal SoGLRenderAction will be constructed.
*/
SoOffscreenRenderer::SoOffscreenRenderer(const SbViewportRegion & viewportregion)
  : backgroundcolor(0.0f, 0.0f, 0.0f),
    components(RGB),
    renderaction(new SoGLRenderAction(viewportregion)),
    didallocaction(TRUE),
    internaldata(NULL),
    buffer(NULL)
{
#if HAVE_GLX
  this->internaldata = new SoOffscreenGLXData();
#endif // HAVE_GLX
  this->setViewportRegion(viewportregion);
}

/*!
  Constructor. Argument is the \a action we should apply to the
  scene graph when rendering the scene. Information about the
  viewport is extracted from the \a action.
*/
SoOffscreenRenderer::SoOffscreenRenderer(SoGLRenderAction * action)
  : viewport(256, 256),
    backgroundcolor(0.0f, 0.0f, 0.0f),
    components(RGB),
    renderaction(action),
    didallocaction(FALSE),
    internaldata(NULL),
    buffer(NULL)
{
#if HAVE_GLX
  this->internaldata = new SoOffscreenGLXData();
#endif // HAVE_GLX
  assert(action);
  this->setViewportRegion(action->getViewportRegion());
}

/*!
  Destructor.
*/
SoOffscreenRenderer::~SoOffscreenRenderer()
{
  delete[] this->buffer;
  delete this->internaldata;
  if (this->didallocaction) delete this->renderaction;
}

/*!
  Returns the screen pixels per inch resolution of your monitor.
*/
float
SoOffscreenRenderer::getScreenPixelsPerInch(void)
{
  COIN_STUB();
  return 72.0f; // default value
}

/*!
  Get maximum dimensions (width, height) of the offscreen buffer.
*/
SbVec2s
SoOffscreenRenderer::getMaximumResolution(void)
{
#if HAVE_GLX
  SoOffscreenGLXData::getMaxDimensions();
#endif // HAVE_GLX
  return SbVec2s(0, 0);
}

/*!
  Sets the component format of the offscreen buffer. Currently only
  the RGB_TRANSPARENCY format is supported.
*/
void
SoOffscreenRenderer::setComponents(const Components components)
{
  this->components = components;

  SbVec2s dims = this->getViewportRegion().getViewportSizePixels();

  delete[] this->buffer;
  this->buffer = new unsigned char[dims[0] * dims[1] * this->components];
}

/*!
  Returns the component format of the offscreen buffer.

  \sa setComponents()
 */
SoOffscreenRenderer::Components
SoOffscreenRenderer::getComponents(void) const
{
  return this->components;
}

/*!
  Sets the viewport region.
*/
void
SoOffscreenRenderer::setViewportRegion(const SbViewportRegion & region)
{
  this->viewport = region;

  if (this->renderaction)
    this->renderaction->setViewportRegion(region);

  SbVec2s dims = region.getViewportSizePixels();
  if (this->internaldata) this->internaldata->setBufferSize(dims);

  delete[] this->buffer;
  this->buffer = new unsigned char[dims[0] * dims[1] * this->getComponents()];
}

/*!
  Returns the viewerport region.
*/
const SbViewportRegion &
SoOffscreenRenderer::getViewportRegion(void) const
{
  return this->viewport;
}

/*!
  Sets the background color. The buffer is cleared to this color
  before rendering.
*/
void
SoOffscreenRenderer::setBackgroundColor(const SbColor & color)
{
  this->backgroundcolor = color;
}

/*!
  Returns the background color.
*/
const SbColor &
SoOffscreenRenderer::getBackgroundColor(void) const
{
  return this->backgroundcolor;
}

/*!
  Sets the render action. Use this if you have special rendering needs.
*/
void
SoOffscreenRenderer::setGLRenderAction(SoGLRenderAction * action)
{
  if (this->didallocaction) delete this->renderaction;
  this->renderaction = action;
  this->didallocaction = FALSE;
}

/*!
  Returns the rendering action currently used.
*/
SoGLRenderAction *
SoOffscreenRenderer::getGLRenderAction(void) const
{
  return this->renderaction;
}

// Collects common code from the two render() functions.
SbBool
SoOffscreenRenderer::renderFromBase(SoBase * base)
{
  if (this->internaldata && this->internaldata->makeContextCurrent()) {
    glClearColor(this->backgroundcolor[0],
                 this->backgroundcolor[1],
                 this->backgroundcolor[2],
                 0.0f);
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

    if (base->isOfType(SoNode::getClassTypeId()))
      this->renderaction->apply((SoNode *)base);
    else if (base->isOfType(SoPath::getClassTypeId()))
      this->renderaction->apply((SoPath *)base);
    else assert(FALSE && "impossible");

    this->internaldata->postRender();
    this->convertBuffer();

    return TRUE;
  }
  return FALSE;
}

// Convert from RGBA format to the application programmer's requested
// format.
void
SoOffscreenRenderer::convertBuffer(void)
{
  SbVec2s dims = this->getViewportRegion().getViewportSizePixels();
  int pixels = dims[0] * dims[1];
  int depth = this->getComponents();
  unsigned char * nativebuffer = this->internaldata->getBuffer();

  switch (this->getComponents()) {
  case SoOffscreenRenderer::RGB_TRANSPARENCY:
    memcpy(this->buffer, nativebuffer, pixels * depth);
    break;

  case SoOffscreenRenderer::RGB:
    {
      unsigned char * native = nativebuffer;
      unsigned char * local = this->buffer;
      for (int i=0; i < pixels; i++) {
        *local++ = *native++;
        *local++ = *native++;
        *local++ = *native++;
        native++;
      }
    }
    break;

  case SoOffscreenRenderer::LUMINANCE_TRANSPARENCY:
    {
      unsigned char * native = nativebuffer;
      unsigned char * local = this->buffer;
      for (int i=0; i < pixels; i++) {
        int val = (int(*native++) + int(*native++) + int(*native++)) / 3;
        *local++ = (unsigned char)val;
        *local++ = *native++;
      }
    }
    break;

  case SoOffscreenRenderer::LUMINANCE:
    {
      unsigned char * native = nativebuffer;
      unsigned char * local = this->buffer;
      for (int i=0; i < pixels; i++) {
        int val = (int(*native++) + int(*native++) + int(*native++)) / 3;
        *local++ = (unsigned char)val;
        native++;
      }
    }
    break;

  default:
    assert(FALSE && "unknown buffer format"); break;
  }
}

/*!
  Render \a scene into our internal memory buffer.
*/
SbBool
SoOffscreenRenderer::render(SoNode * scene)
{
  return this->renderFromBase(scene);
}

/*!
  Render the \a scene path into our internal memory buffer.
*/
SbBool
SoOffscreenRenderer::render(SoPath * scene)
{
  return this->renderFromBase(scene);
}

/*!
  Returns the offscreen memory buffer.
*/
unsigned char *
SoOffscreenRenderer::getBuffer(void) const
{
  return this->buffer;
}

//
// avoid endian problems (little endian sucks, right? :)
//
static int
write_short(FILE * fp, unsigned short val)
{
  // FIXME: does this code really work on both little endian and big
  // endian platforms? 20000110 mortene.
  unsigned char tmp[2];
  tmp[0] = (unsigned char)(val >> 8);
  tmp[1] = (unsigned char)(val & 0xff);
  return fwrite(&tmp, 2, 1, fp);
}

/*!
  Writes the buffer to an SGI format RGB file.
*/
SbBool
SoOffscreenRenderer::writeToRGB(FILE * fp) const
{
  //
  // FIXME: add code to rle rows, pederb 2000-01-10
  //
  if (this->internaldata) {
    SbVec2s size = this->internaldata->getSize();
    write_short(fp, 0x01da); // imagic
    write_short(fp, 0x0001); // raw (no rle yet)

    unsigned short comp = (unsigned short)this->getComponents();
    if (comp == 1)
      write_short(fp, 0x0002); // 2 dimensions (heightmap)
    else
      write_short(fp, 0x0003); // 3 dimensions

    write_short(fp, (unsigned short) size[0]);
    write_short(fp, (unsigned short) size[1]);
    write_short(fp, (unsigned short) comp);

    unsigned char buf[500];
    memset(buf, 0, 500);
    buf[7] = 255; // set maximum pixel value to 255
    strcpy((char *)buf+8, "http://www.coin3d.org");
    fwrite(buf, 1, 500, fp);

    unsigned char * tmpbuf = new unsigned char[size[0]];

    unsigned char * ptr = this->buffer;
    for (int c = 0; c < comp; c++) {
      for (int y = 0; y < size[1]; y++) {
        for (int x = 0; x < size[0]; x++) {
          tmpbuf[x] = ptr[x * comp + y * comp * size[0] + c];
        }
        fwrite(tmpbuf, 1, size[0], fp);
      }
    }
    delete [] tmpbuf;
    return TRUE;
  }
  return FALSE;
}

/*!
  Writes the buffer to a file in Postscript format.
*/
SbBool
SoOffscreenRenderer::writeToPostScript(FILE * fp) const
{
  COIN_STUB();
  return FALSE;
}

/*!
  Writes the buffer to a file in Postscript format, with \a printsize
  dimensions.
*/
SbBool
SoOffscreenRenderer::writeToPostScript(FILE * fp,
                                       const SbVec2f & printsize) const
{
  COIN_STUB();
  return FALSE;
}
