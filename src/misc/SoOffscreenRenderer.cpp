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
  \class SoBase SoOffscreenRenderer.h Inventor/SoOffscreenRenderer.h
  \brief The SoOffscreenRenderer class is used for off screen rendering.
  \ingroup general

  If you want to render to a memory buffer instead of an on-screen
  OpenGL context, use this class. Rendering to a memory buffer can be
  used to generate texture maps on-the-fly, or for saving snapshots of
  the scene to disk files (as pixel bitmaps or as Postscript files for
  sending to a Postscript-capable printer).

  Currently offscreen rendering can only be done if Mesa is used as
  the OpenGL rendering library.

 */

#include <Inventor/SoOffscreenRenderer.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <GL/gl.h>
#include <assert.h>
#include <string.h> // memset()
#include <config.h>
#include <coindefs.h> // COIN_STUB()

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG


// FIXME: GLX offscreen rendering code below, ripped out from our good
// old KartSim project. Integrate in the class source. 20000417 mortene.

//      Display *dsp = XtDisplay(psGUI::glMap);
//      int attribs[] = {GLX_RGBA, None};

//      // Set up an off-screen GL pixmap for rendering the overview map w/
//      // pipelines.
//      XVisualInfo *visinfo = glXChooseVisual(dsp, DefaultScreen(dsp), attribs);
//      if(!visinfo) Debug::Info(dbMISC, dbFATAL, "Can't get visual info.");
//      Pixmap pixmap = XCreatePixmap(dsp, XtWindow(psGUI::glMap),
//  				  psGUI::mapwidth, psGUI::mapheight,
//  				  visinfo->depth);
//      if(!pixmap) Debug::Info(dbMISC, dbFATAL, "Can't get X pixmap.");
//      GLXPixmap glpixmap = glXCreateGLXPixmap(dsp, visinfo, pixmap);
//      if(!glpixmap) Debug::Info(dbMISC, dbFATAL, "Can't get GL pixmap.");
//      GLXContext context = glXCreateContext(dsp, visinfo, 0, GL_FALSE);
//      if(!context) Debug::Info(dbDATABASE, dbFATAL, "Found no usable context.");

//      // Set current GL context to the glpixmap.
//      glXMakeCurrent(dsp, glpixmap, context);

//      glMatrixMode(GL_PROJECTION);
//      glLoadIdentity();
//      glMatrixMode(GL_MODELVIEW);
//      glLoadIdentity();
//      glOrtho(0, psGUI::mapwidth, 0, psGUI::mapheight, -1, 1);

//      // Draw whole map.
//      glRasterPos2i(0, 0);
//      glDrawPixels(psGUI::mapwidth, psGUI::mapheight, GL_RGBA, GL_UNSIGNED_BYTE,
//  		 psGUI::mapbuffer);

//      // Draw lines to indicate pipes.
//      int r, g, b;
//      struct polypipes *polyrunner;
//      struct GUI_markerobject *h = psGUI::pipelist;
//      while(h) {
//        ((psPipes *)(h->obj))->psGetPipecolor(r, g, b);
//        glColor3b(r>>1, g>>1, b>>1);
//        glLineWidth(1.0);

//        psGUI::drawpipepieces(((psPipes *)(h->obj))->psGetPieceshead(), FALSE);

//        polyrunner = ((psPipes *)(h->obj))->psGetPolyhead();
//        while(polyrunner) {
//  	psGUI::drawpipepieces(polyrunner->first, TRUE);
//  	polyrunner = polyrunner->next;
//        }

//        h = h->next;
//      }

//      // Allocate the mapbuffer.
//      psGUI::mappipebuffer =
//        new unsigned char[psGUI::mapwidth*psGUI::mapheight*4];

//      glPixelStorei(GL_PACK_ALIGNMENT, 1);
//      glReadPixels(0, 0, psGUI::mapwidth, psGUI::mapheight,
//  		 GL_RGBA, GL_UNSIGNED_BYTE, psGUI::mappipebuffer);

//      // Use mappipebuffer from app start.
//      psGUI::drawbuffer = psGUI::mappipebuffer;

//      // Set current GL context to the 2D map drawing area's.
//      glXMakeCurrent(dsp, XtWindow(psGUI::glMap), glxcontext); 

//      glMatrixMode(GL_PROJECTION);
//      glLoadIdentity();
//      glMatrixMode(GL_MODELVIEW);
//      glLoadIdentity();
//      glOrtho(0, psGUI::mapwidth, 0, psGUI::mapheight, -1, 1);

//      // Draw map with pipes.
//      glRasterPos2i(0, 0);
//      glDrawPixels(psGUI::mapwidth, psGUI::mapheight, GL_RGBA, GL_UNSIGNED_BYTE,
//  		 psGUI::drawbuffer);

//      // Draw all 2D markers.
//      struct GUI_structlist *runner = psGUI::mo_head;
//      psGUI::setTransState();
//      while(runner) {
//        psGUI::drawmarker(runner->GUI_struct);
//        runner = runner->next;
//      }
//      psGUI::resetState();

//      // Reset the context to the one used by the 3D Performer window.
//      glXMakeCurrent(pfGetCurWSConnection(),
//  		   perf::shmem->xWinInfo.pw->getCurWSDrawable(),
//  		   perf::shmem->xWinInfo.pw->getGLCxt());

//      // Free resources used for the generation of map-w/pipes gfx.
//      glXDestroyContext(dsp, context);
//      glXDestroyGLXPixmap(dsp, glpixmap);
//      XFreePixmap(dsp, pixmap);



#ifndef DOXYGEN_SKIP_THIS // Don't document internal classes.

class SoOffscreenInternalData {
public:
  SoOffscreenInternalData(void) {
    this->buffersize = SbVec2s(0, 0);
  }
  virtual ~SoOffscreenInternalData() {
  }

  // must be overloaded
  virtual void makeContextCurrent(void) = 0;
  virtual unsigned char * getBuffer(void) = 0;

  virtual void setBufferSize(const SbVec2s & size) {
    SbVec2s maxsize = SoOffscreenRenderer::getMaximumResolution();

#if COIN_DEBUG
    if (size[0] > maxsize[0]) {
      SoDebugError::postWarning("SoOffscreenMesaData::setBufferSize",
                                "can't set width to %d, maximum is %d",
                                size[0], maxsize[0]);
    }
    else if (size[0] <= 0) {
      SoDebugError::postWarning("SoOffscreenMesaData::setBufferSize",
                                "can't set negative width %d", size[0]);
    }
    if (size[1] > maxsize[1]) {
      SoDebugError::postWarning("SoOffscreenMesaData::setBufferSize",
                                "can't set height to %d, maximum is %d",
                                size[1], maxsize[1]);
    }
    else if (size[1] <= 0) {
      SoDebugError::postWarning("SoOffscreenMesaData::setBufferSize",
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

protected:
  SbVec2s buffersize;
};

#if HAVE_OSMESACREATECONTEXT
#include <GL/osmesa.h>

class SoOffscreenMesaData : public SoOffscreenInternalData {
public:
  SoOffscreenMesaData(void) {
    this->context = OSMesaCreateContext(OSMESA_RGBA, NULL);
#if COIN_DEBUG
    if (!this->context) {
      SoDebugError::postWarning("SoOffscreenMesaData::SoOffscreenMesaData",
                                "couldn't create context");
    }
#endif // COIN_DEBUG
    this->buffer = NULL;
  }
  virtual ~SoOffscreenMesaData() {
    if (this->context) OSMesaDestroyContext(this->context);
    delete this->buffer;
  }

  virtual void setBufferSize(const SbVec2s & size) {
    SoOffscreenInternalData::setBufferSize(size);
    delete this->buffer;
    this->buffer =
      new unsigned char[this->buffersize[0] * this->buffersize[1] * 4];
  }

  virtual void makeContextCurrent(void) {
    assert(this->buffer);
    if (this->context) {
      OSMesaMakeCurrent(this->context, this->buffer,
                        GL_UNSIGNED_BYTE,
                        this->buffersize[0],
                        this->buffersize[1]);
    }
  }

  virtual unsigned char * getBuffer(void) {
    return this->buffer;
  }

private:
  unsigned char * buffer;
  OSMesaContext context;
};

#endif // HAVE_OSMESACREATECONTEXT

#endif // DOXYGEN_SKIP_THIS



/*!
  Constructor. Argument is the \a viewportregion we should use when
  rendering. An internal SoGLRenderAction will be constructed.
*/
SoOffscreenRenderer::SoOffscreenRenderer(const SbViewportRegion & viewportregion)
  : backgroundcolor(0.0f, 0.0f, 0.0f),
    components(RGB_TRANSPARENCY),
    renderaction(new SoGLRenderAction(viewportregion)),
    didallocaction(TRUE),
    internaldata(NULL)
{
#if HAVE_OSMESACREATECONTEXT
  this->internaldata = new SoOffscreenMesaData();
#endif // HAVE_OSMESACREATECONTEXT
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
    components(RGB_TRANSPARENCY),
    renderaction(action),
    didallocaction(FALSE),
    internaldata(NULL)
{
#if HAVE_OSMESACREATECONTEXT
  this->internaldata = new SoOffscreenMesaData();
#endif // HAVE_OSMESACREATECONTEXT
  assert(action);
  this->setViewportRegion(action->getViewportRegion());
}

/*!
  Destructor.
*/
SoOffscreenRenderer::~SoOffscreenRenderer()
{
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
#if HAVE_OSMESACREATECONTEXT
  // Create and set up a dummy context to be able to use
  // glGetIntegerv() below.
  OSMesaContext tmpctx = OSMesaCreateContext(OSMESA_RGBA, NULL);

  if (!tmpctx) {
#if COIN_DEBUG
    SoDebugError::postWarning("SoOffscreenRenderer::getMaximumResolution",
                              "couldn't create context");
#endif // COIN_DEBUG
    return SbVec2s(1, 1);
  }

  const int dummydim = 16;
  unsigned char buffer[4 * dummydim * dummydim];
  OSMesaMakeCurrent(tmpctx, buffer, GL_UNSIGNED_BYTE, dummydim, dummydim);

  GLint dims[2];
  glGetIntegerv(GL_MAX_VIEWPORT_DIMS, dims);

  OSMesaDestroyContext(tmpctx);

  // Avoid overflow.
  dims[0] = SbMin(dims[0], 32767);
  dims[1] = SbMin(dims[1], 32767);

  return SbVec2s((short)dims[0], (short)dims[1]);
#endif // HAVE_OSMESACREATECONTEXT

  return SbVec2s(1280, 1024);
}

/*!
  Sets the component format of the offscreen buffer. Currently only
  the RGB_TRANSPARENCY format is supported.
*/
void
SoOffscreenRenderer::setComponents(const Components components)
{
  // FIXME: create support for other image formats, pederb 20000109
  if (components != RGB_TRANSPARENCY) {
    COIN_STUB();
    return;
  }

  this->components = components;
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
  if (this->internaldata)
    this->internaldata->setBufferSize(region.getViewportSizePixels());
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

/*!
  Render \a scene into our internal memory buffer.
*/
SbBool
SoOffscreenRenderer::render(SoNode * scene)
{
  if (this->internaldata) {
    this->internaldata->makeContextCurrent();
    glClearColor(this->backgroundcolor[0],
                 this->backgroundcolor[1],
                 this->backgroundcolor[2],
                 0.0f);
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    this->renderaction->apply(scene);
    return TRUE;
  }
  return FALSE;
}

/*!
  Render the \a scene path into our internal memory buffer.
*/
SbBool
SoOffscreenRenderer::render(SoPath * scene)
{
  if (this->internaldata) {
    this->internaldata->makeContextCurrent();
    this->internaldata->makeContextCurrent();
    glClearColor(this->backgroundcolor[0],
                 this->backgroundcolor[1],
                 this->backgroundcolor[2],
                 0.0f);
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    this->renderaction->apply(scene);
    return TRUE;
  }
  return FALSE;
}

/*!
  Returns the offscreen memory buffer.
*/
unsigned char *
SoOffscreenRenderer::getBuffer(void) const
{
  return this->internaldata ? this->internaldata->getBuffer() : NULL;
}

//
// avoid endian problems (little endian sucks, right? :)
//
static int
write_short(FILE * fp, unsigned short val)
{
  // FIXME: does this code work on both little endian and big endian
  // platforms? 20000110 mortene.
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

    unsigned char * ptr = this->internaldata->getBuffer();
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
