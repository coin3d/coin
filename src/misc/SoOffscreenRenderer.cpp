/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoOffscreenRenderer SoOffscreenRenderer.h Inventor/SoOffscreenRenderer.h
  \brief The SoOffscreenRenderer class is used for rendering scenes in offscreen buffers.
  \ingroup general

  If you want to render to a memory buffer instead of an on-screen
  OpenGL context, use this class. Rendering to a memory buffer can be
  used to generate texture maps on-the-fly, or for saving snapshots of
  the scene to disk files (as pixel bitmaps or as Postscript files for
  sending to a Postscript-capable printer).

  Here's a dead simple usage example:

  \code
  SoOffscreenRenderer * myRenderer = new SoOffscreenRenderer(vpregion);
  SoNode * root = myViewer->getSceneManager()->getSceneGraph();
  SbBool ok = myRenderer->render(root);
  // [then use image buffer in a texture, or write it to file, or whatever]
  \endcode


  Offscreen rendering is internally done with either GLX (i.e. OpenGL
  on X11) or WGL (i.e. OpenGL on Win32) or AGL (i.e. OpenGL on the Mac
  OS). The pixeldata is fetched from the OpenGL buffer with
  glReadPixels(), with the format and type arguments set to GL_RGBA
  and GL_UNSIGNED_BYTE, respectively. This means that the maximum
  resolution is 32 bits, 8 bits for each of the R/G/B/A components.


  One particular usage of the SoOffscreenRenderer is to make it render
  frames to be used for the construction of movies. The general
  technique for doing this is to iterate over the following actions:

  <ul>
  <li>move camera to correct position for frame</li>
  <li>update the \c realTime global field (see explanation below)</li>
  <li>invoke the SoOffscreenRenderer</li>
  <li>dump rendered scene to file</li>
  </ul>

  ..then you use some external tool or library to construct the movie
  file, for instance in MPEG format, from the set of files dumped to
  disk from the iterative process above.

  The code would go something like the following (pseudo-code
  style). First we need to stop the Coin library itself from doing any
  automatic updating of the \c realTime field, so your application
  initialization for Coin should look something like:


  \code
   [...] = SoQt::init([...]); // or SoWin::init() or SoDB::init()
   // ..and then immediately:

   // Control realTime field ourselves, so animations within the scene
   // follows "movie-time" and not "wallclock-time".
   SoDB::enableRealTimeSensor(FALSE);
   SoSceneManager::enableRealTimeUpdate(FALSE);
   SoSFTime * realtime = SoDB::getGlobalField("realTime");
   realtime->setValue(0.0);
  \endcode

  Note that it is important that the \c realTime field is initialized
  to \e your start-time \e before setting up any engines or other
  entities in the system that uses the \c realTime field.

  Then for the rendering loop, something like:

  \code
   for (int i=0; i < NRFRAMES; i++) {
     // [...reposition camera here, if necessary...]

     // render
     offscreenrend->render(root);

     // dump to file
     SbString framefile;
     framefile.sprintf("frame%06d.rgb", i);
     offscreenrend->writeToRGB(framefile);

     // advance "current time" by the frames-per-second value, which
     // is 24 fps in this example
     realtime->setValue(realtime.getValue() + 1/24.0);
   }
  \endcode

  When making movies you need to write your application control code
  to take care of moving the camera along the correct trajectory
  yourself, and to explicitly control the global \c realTime field.
  The latter is so you're able to "step" with appropriate time units
  for each render operation (e.g. if you want a movie that has a 24
  FPS refresh rate, first render with \c realTime=0.0, then add 1/24s
  to the \c realTime field, render again to a new frame, add another
  1/24s to the \c realTime field, render, and so on).

  For further information about how to control the \c realTime field,
  see documentation of SoDB::getGlobalField(),
  SoDB::enableRealTimeSensor(), and
  SoSceneManager::enableRealTimeUpdate().
*/

// As first mentioned to me by Kyrah, the functionality of this class
// should really have been outside the core Coin library, seeing how
// it makes heavy use of window-system specifics. To be SGI Inventor
// compatible we need it to be part of the Coin API, though.
//
// mortene.

// FIXME: we don't set up and render to RGBA-capable OpenGL-contexts,
// even when the requested format from the app-programmer is
// RGBA.
//
// I think this is what we should do:
//
//        1) first, try to get hold of a p-buffer with destination
//        alpha (p-buffers are faster to render into, as they can take
//        advantage of hardware acceleration)
// 
//        2) failing that, try to make WGL (or GLX or AGL on
//        non-MSWindows platforms) set up a buffer with destination
//        alpha
// 
//        3) failing that, get hold of either a p-buffer or a straight
//        WGL buffer with only RGB (no destination alpha -- this
//        should never fail), and do post-processing on the rendered
//        scene pixel-by-pixel to convert it into an RGBA texture
//
// 20020604 mortene.

#include <Inventor/SoOffscreenRenderer.h>
#include <Inventor/SoPath.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/SbVec2f.h>
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H
#include <Inventor/system/gl.h>
#include <assert.h>
#include <string.h> // memset(), memcpy()
#include <coindefs.h> // COIN_STUB()
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <../glue/simage_wrapper.h>
#include <math.h> // for ceil()

#include <Inventor/nodes/SoCamera.h> 
#include <Inventor/nodes/SoCallback.h> 
#include <Inventor/SbMatrix.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/elements/SoViewportRegionElement.h>
#include <Inventor/elements/SoProjectionMatrixElement.h>
#include <Inventor/elements/SoViewVolumeElement.h>
#include <Inventor/elements/SoViewingMatrixElement.h>
#include <Inventor/elements/SoCullElement.h>
#include <Inventor/C/glue/gl.h>
#include <Inventor/C/tidbits.h>

/*!
  \enum SoOffscreenRenderer::Components

  Enumerated values for the available image formats.

  \sa setComponents()
*/

#define PRIVATE(p) (p->pimpl)
#define PUBLIC(p) (p->master)

class SoOffscreenRendererP {
public:
  SoOffscreenRendererP(SoOffscreenRenderer * master) {
    this->master = master;
  };

  SoOffscreenRenderer * master;

  static SoGLRenderAction::AbortCode subscreenAbortCallback(void *userData);
  SbBool renderFromBase(SoBase * base);
  void convertBuffer(void);
  void convertSubscreenBuffer(void);
  void setSubscreenCameraPosition(int renderpass, SoCamera *cam);
  void setupSubscreens(SbVec2s totalSize);
  void pasteSubscreen(int renderpass);

  SbViewportRegion viewport;
  SbViewportRegion subviewport;
  SbColor backgroundcolor;
  SoOffscreenRenderer::Components components;
  SoGLRenderAction * renderaction;
  SbBool didallocaction;
  class SoOffscreenInternalData * internaldata;
  unsigned char * buffer;

  int numsubscreens[2];
  SbVec2s requestedsize;
  unsigned char * subscreen;
  int currentsubscreen;
  SbBool mustusesubscreens;
  SbBool lastnodewasacamera;
  SoCamera * visitedcamera;

  SbVec2s lastsubscreensize;
  SbVec2s maxres;

};


class SoOffscreenInternalData {
public:
  SoOffscreenInternalData(void) {
    this->buffersize = SbVec2s(0, 0);
  }
  virtual ~SoOffscreenInternalData() {
  }

  SoOffscreenRendererP * master;

  // Return FALSE if the necessary resource for rendering are not
  // available.
  virtual SbBool makeContextCurrent(uint32_t contextid) = 0;

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

  // Note: should _really_ be overridden by subclasses.
  virtual SbVec2s getMax(void) { 
    SbVec2s maxres = SoOffscreenRenderer::getMaximumResolution();
    return(maxres);
  }
};


#ifdef HAVE_AGL
#include "SoOffscreenAGLData.icc"
#endif // HAVE_AGL

#ifdef HAVE_GLX
#include "SoOffscreenGLXData.icc"
#endif // HAVE_GLX

#ifdef HAVE_WGL
#include "SoOffscreenWGLData.icc"
#endif // HAVE_WGL


/*!
  Constructor. Argument is the \a viewportregion we should use when
  rendering. An internal SoGLRenderAction will be constructed.
*/
SoOffscreenRenderer::SoOffscreenRenderer(const SbViewportRegion & viewportregion)
{
  PRIVATE(this) = new SoOffscreenRendererP(this);

  PRIVATE(this)->mustusesubscreens = FALSE;
  PRIVATE(this)->currentsubscreen = 0;
  PRIVATE(this)->backgroundcolor.setValue(0,0,0);
  PRIVATE(this)->components = RGB;
  PRIVATE(this)->didallocaction = TRUE;
  PRIVATE(this)->buffer = NULL;
  PRIVATE(this)->renderaction = new SoGLRenderAction(viewportregion);

  PRIVATE(this)->lastnodewasacamera = FALSE;

  PRIVATE(this)->internaldata = NULL;
#ifdef HAVE_GLX
  PRIVATE(this)->internaldata = new SoOffscreenGLXData();
#elif defined(HAVE_WGL)
  PRIVATE(this)->internaldata = new SoOffscreenWGLData();
#elif defined(HAVE_AGL)
  PRIVATE(this)->internaldata = new SoOffscreenAGLData();
#endif // HAVE_AGL

  PRIVATE(this)->internaldata->master = PRIVATE(this);
  PRIVATE(this)->renderaction->setCacheContext(SoGLCacheContextElement::getUniqueCacheContext());
  this->setViewportRegion(viewportregion);

}



/*!
  Constructor. Argument is the \a action we should apply to the
  scene graph when rendering the scene. Information about the
  viewport is extracted from the \a action.
*/
SoOffscreenRenderer::SoOffscreenRenderer(SoGLRenderAction * action)
{
  PRIVATE(this) = new SoOffscreenRendererP(this);

  PRIVATE(this)->mustusesubscreens = FALSE;
  PRIVATE(this)->currentsubscreen = 0;
  PRIVATE(this)->viewport.setWindowSize(256,256);
  PRIVATE(this)->backgroundcolor.setValue(0,0,0);
  PRIVATE(this)->components = RGB;
  PRIVATE(this)->didallocaction = TRUE;
  PRIVATE(this)->buffer = NULL;
  PRIVATE(this)->renderaction = action;
  PRIVATE(this)->lastnodewasacamera = FALSE;

  PRIVATE(this)->internaldata = NULL;
#ifdef HAVE_GLX
  PRIVATE(this)->internaldata = new SoOffscreenGLXData();
#elif defined(HAVE_WGL)
  PRIVATE(this)->internaldata = new SoOffscreenWGLData();
#elif defined(HAVE_AGL)
  PRIVATE(this)->internaldata = new SoOffscreenAGLData();
#endif // HAVE_AGL
  assert(action);

  PRIVATE(this)->internaldata->master = PRIVATE(this);

  this->setViewportRegion(action->getViewportRegion());

}



/*!
  Destructor.
*/
SoOffscreenRenderer::~SoOffscreenRenderer()
{
  delete [] PRIVATE(this)->buffer;
  if(PRIVATE(this)->mustusesubscreens) delete [] PRIVATE(this)->subscreen;
  delete PRIVATE(this)->internaldata;
  if(PRIVATE(this)->didallocaction) delete PRIVATE(this)->renderaction;
  delete PRIVATE(this);
}



/*!
  Returns the screen pixels per inch resolution of your monitor.
*/
float
SoOffscreenRenderer::getScreenPixelsPerInch(void)
{
  SbVec2f pixmmres(72.0f / 25.4f, 72.0f / 25.4f);
#ifdef HAVE_GLX
  pixmmres = SoOffscreenGLXData::getResolution();
#elif defined(HAVE_WGL)
  pixmmres = SoOffscreenWGLData::getResolution();
#elif defined(HAVE_AGL)
  pixmmres = SoOffscreenAGLData::getResolution();
#endif // HAVE_AGL

  // The API-signature of this method is not what it should be: it
  // assumes the same resolution in the vertical and horizontal
  // directions.
  float pixprmm = (pixmmres[0] + pixmmres[1]) / 2.0f; // find average

  return pixprmm * 25.4f; // an inch is 25.4 mm.
}


static void getMaxCB(void *ptr, SoAction *action)
{

  GLint *size = (GLint *) ptr; 
  glGetIntegerv(GL_MAX_VIEWPORT_DIMS, size);

  
  const char * vendor = (const char *)glGetString(GL_VENDOR);
  if (strcmp(vendor, "NVIDIA Corporation") == 0) {

    // FIXME: Here we need to add a version check if NVIDIA fixes this
    // bug in the future (20021023 handegar)
    
    // FIXME: Due to a possible NVIDIA bug, max render size is limited
    // by desktop resolution, not the texturesize returned by
    // OpenGL. This is fixed temporarily by limiting max size to the
    // lowend resolution for desktop monitors. (20021023 handegar)
    
    size[0] = 800;
    size[1] = 600;
    
  }

}


/*!
  Get maximum dimensions (width, height) of the offscreen buffer.
*/
SbVec2s
SoOffscreenRenderer::getMaximumResolution(void)
{

  static SbBool isvisited = FALSE;
  static short dims[2] = {128, 128};

  if (!isvisited){
    isvisited = TRUE;
    SoOffscreenRenderer * osr = new SoOffscreenRenderer(SbViewportRegion(128,128));
    SoCallback *cb = new SoCallback();
    cb->ref();
    
    GLint temp[2];
    cb->setCallback(getMaxCB,temp);
    
    osr->render(cb);
    
    delete osr;
    cb->unref();
    
    dims[0] = (short) temp[0];
    dims[1] = (short) temp[1];
  } 
  return SbVec2s(dims[0], dims[1]);

}



/*!
  Sets the component format of the offscreen buffer.

  If set to \c LUMINANCE, a grayscale image is rendered, \c
  LUMINANCE_TRANSPARENCY gives us a grayscale image with transparency,
  \c RGB will give us a 24-bit image with 8 bits each for the red,
  green and blue component, and \c RGB_TRANSPARENCY yields a 32-bit
  image (\c RGB plus transparency).

  The default format to render to is \c RGB.

  This will invalidate the current buffer, if any. The buffer will not
  contain valid data until another call to
  SoOffscreenRenderer::render() happens.
*/
void
SoOffscreenRenderer::setComponents(const Components components)
{
  PRIVATE(this)->components = components;
  SbVec2s dims = this->getViewportRegion().getViewportSizePixels();
}



/*!
  Returns the component format of the offscreen buffer.

  \sa setComponents()
 */
SoOffscreenRenderer::Components
SoOffscreenRenderer::getComponents(void) const
{
  return PRIVATE(this)->components;

}



/*!
  Sets the viewport region.

  This will invalidate the current buffer, if any. The buffer will not
  contain valid data until another call to
  SoOffscreenRenderer::render() happens.
*/
void
SoOffscreenRenderer::setViewportRegion(const SbViewportRegion & region)
{
  // As the current context is destructed and a new one is set up
  // below, it can be a major optimization to just return when there
  // is no real change.
  if(PRIVATE(this)->viewport == region) { return; }  
  PRIVATE(this)->viewport = region;
  

  // Adjust rendering canvas size if needed.
  SbVec2s size = PRIVATE(this)->viewport.getViewportSizePixels();
  SbVec2s maxsize = this->getMaximumResolution();
  
  PRIVATE(this)->requestedsize = PRIVATE(this)->viewport.getViewportSizePixels();  
  PRIVATE(this)->mustusesubscreens = (size[0] > maxsize[0]) || (size[1] > maxsize[1]);

  if (PRIVATE(this)->mustusesubscreens) {
    PRIVATE(this)->setupSubscreens(size);

    PRIVATE(this)->currentsubscreen = 0;
    PRIVATE(this)->subviewport = SbViewportRegion(maxsize);
  }
    

  if(!PRIVATE(this)->mustusesubscreens){

    if(PRIVATE(this)->renderaction) 
      PRIVATE(this)->renderaction->setViewportRegion(PRIVATE(this)->viewport);
    PRIVATE(this)->internaldata->setBufferSize(size);

  } else {
    
    if(PRIVATE(this)->renderaction) 
      PRIVATE(this)->renderaction->setViewportRegion(PRIVATE(this)->subviewport);
    PRIVATE(this)->internaldata->setBufferSize(maxsize);

  }


  if(!PRIVATE(this)->buffer)
    delete [] PRIVATE(this)->buffer;

}



/*!
  Returns the viewerport region.
*/
const SbViewportRegion &
SoOffscreenRenderer::getViewportRegion(void) const
{
  return PRIVATE(this)->viewport;
}



/*!
  Sets the background color. The buffer is cleared to this color
  before rendering.
*/
void
SoOffscreenRenderer::setBackgroundColor(const SbColor & color)
{
  PRIVATE(this)->backgroundcolor = color;
}



/*!
  Returns the background color.
*/
const SbColor &
SoOffscreenRenderer::getBackgroundColor(void) const
{
  return PRIVATE(this)->backgroundcolor;
}



/*!
  Sets the render action. Use this if you have special rendering needs.
*/
void
SoOffscreenRenderer::setGLRenderAction(SoGLRenderAction * action)
{
  if (PRIVATE(this)->didallocaction) delete PRIVATE(this)->renderaction;
  PRIVATE(this)->renderaction = action;
  PRIVATE(this)->didallocaction = FALSE;
}



/*!
  Returns the rendering action currently used.
*/
SoGLRenderAction *
SoOffscreenRenderer::getGLRenderAction(void) const
{
  return PRIVATE(this)->renderaction;
}



static void
pre_render_cb(void * userdata, SoGLRenderAction * action)
{
  glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
  action->removePreRenderCallback(pre_render_cb, userdata);

  action->setRenderingIsRemote(FALSE);
}




// Callback when rendering scenegraph to subscreens. Searching for cameras.
SoGLRenderAction::AbortCode 
SoOffscreenRendererP::subscreenAbortCallback(void *userData)
{

  SoOffscreenRendererP * thisp = (SoOffscreenRendererP *) userData;
  const SoFullPath * path = (const SoFullPath*) thisp->renderaction->getCurPath();
  SoNode * node = path->getTail();

  if(thisp->lastnodewasacamera){
    thisp->setSubscreenCameraPosition(thisp->currentsubscreen, thisp->visitedcamera);
    thisp->lastnodewasacamera=FALSE;
    return SoGLRenderAction::CONTINUE;
  }
 
  if(node != NULL){
    if(node->isOfType(SoCamera::getClassTypeId())){
      thisp->visitedcamera = (SoCamera *) node;
      thisp->lastnodewasacamera=TRUE;
    }
  }
 
  return SoGLRenderAction::CONTINUE;
}




// Collects common code from the two render() functions.
SbBool
SoOffscreenRendererP::renderFromBase(SoBase * base)
{

  if (!this->internaldata) {
    static SbBool first = TRUE;
    if (first) {
      SoDebugError::post("SoOffscreenRenderer::renderFromBase",
                         "SoOffscreenRenderer not compiled against any "
                         "window-system binding, it is defunct for this build.");
      first = FALSE;
    }
    return FALSE;
  }

  uint32_t oldcontext = this->renderaction->getCacheContext();

  if (!this->internaldata->makeContextCurrent(oldcontext)) {
    SoDebugError::postWarning("SoOffscreenRenderer::renderFromBase",
                              "could not set up a current OpenGL context.");
    return FALSE;
  }

#if COIN_DEBUG && 0 // debug, enable to check offscreen canvas properties
  GLint colbits[4];
  glGetIntegerv(GL_RED_BITS, &colbits[0]);
  glGetIntegerv(GL_GREEN_BITS, &colbits[1]);
  glGetIntegerv(GL_BLUE_BITS, &colbits[2]);
  glGetIntegerv(GL_ALPHA_BITS, &colbits[3]);
  SoDebugError::postInfo("SoOffscreenRenderer::renderFromBase",
                         "GL context GL_[RED|GREEN|BLUE|ALPHA]_BITS=="
                         "[%d, %d, %d, %d]",
                         colbits[0], colbits[1], colbits[2], colbits[3]);
#endif // debug
  
  glEnable(GL_DEPTH_TEST);
  glClearColor(this->backgroundcolor[0],
               this->backgroundcolor[1],
               this->backgroundcolor[2],
               0.0f);
  
  if (!this->didallocaction) {
    this->renderaction->setCacheContext(SoGLCacheContextElement::getUniqueCacheContext());
  }
 
  // Allocate target buffer
  this->buffer = new unsigned char[this->requestedsize[0] * this->requestedsize[1] * master->getComponents()];

  // Shall we use subscreen rendering or regular one-screen renderer?
  if(this->mustusesubscreens){

    //Allocate memory for subscreen
    subscreen = new unsigned char [(maxres[0]*maxres[1]*this->master->getComponents())];

    // We have to grab cameras using this callback during rendering
    this->renderaction->setAbortCallback(&subscreenAbortCallback,this);    
    this->currentsubscreen = 0;
    
    // Render entire scenegraph for each subscreen.
    for(int i=0;i<(this->numsubscreens[0]*this->numsubscreens[1]);++i){

      if (!this->internaldata->makeContextCurrent(oldcontext)) {
        SoDebugError::postWarning("SoOffscreenRenderer::renderFromBase",
                                  "could not set up a current OpenGL context.");
        return FALSE;
      }
   
      this->renderaction->addPreRenderCallback(pre_render_cb, NULL);

      if(base->isOfType(SoNode::getClassTypeId()))
        this->renderaction->apply((SoNode *)base);
      else if (base->isOfType(SoPath::getClassTypeId()))
        this->renderaction->apply((SoPath *)base);
      else assert(FALSE && "impossible");
      
      this->internaldata->postRender();
      this->convertSubscreenBuffer();
      this->pasteSubscreen(this->currentsubscreen);

      ++this->currentsubscreen;
    }

    this->renderaction->setAbortCallback(NULL, this);


  } else { // -- Regular rendering
    // needed to clear viewport after glViewport is called
    this->renderaction->addPreRenderCallback(pre_render_cb, NULL);

    if (base->isOfType(SoNode::getClassTypeId()))
      this->renderaction->apply((SoNode *)base);
    else if (base->isOfType(SoPath::getClassTypeId()))
      this->renderaction->apply((SoPath *)base);
    else assert(FALSE && "impossible");

    this->internaldata->postRender();
    this->convertBuffer();
 
  }
  
  if (!this->didallocaction) {
    this->renderaction->setCacheContext(oldcontext);
  }
  

  return TRUE;
}



// Convert from RGBA format to the application programmer's requested
// format.
void
SoOffscreenRendererP::convertSubscreenBuffer(void)
{
  int pixels = maxres[0]*maxres[1];
  int depth = PUBLIC(this)->getComponents();

  unsigned char * local = this->subscreen;
  unsigned char * native = this->internaldata->getBuffer();

  switch (depth) {
  case SoOffscreenRenderer::RGB_TRANSPARENCY:
    memcpy(local, native, pixels * depth);
    break;

  case SoOffscreenRenderer::RGB:
    {
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
      for (int i=0; i < pixels; i++) {
        int val = (int(*native++) + int(*native++) + int(*native++)) / 3;
        *local++ = (unsigned char)val;
        *local++ = *native++;
      }
    }
    break;

  case SoOffscreenRenderer::LUMINANCE:
    {
      for (int i=0; i < pixels; i++) {
        uint32_t val = 76*native[0]+155*native[1]+26*native[2];
        *local++ = (unsigned char)(val>>8);
        native += 4;
      }
    }
    break;

  default:
    assert(FALSE && "unknown buffer format"); break;
  }
}



// Convert from RGBA format to the application programmer's requested
// format.
void
SoOffscreenRendererP::convertBuffer(void)
{

  SbVec2s dims = PUBLIC(this)->getViewportRegion().getViewportSizePixels();
  int pixels = dims[0] * dims[1];
  int depth = PUBLIC(this)->getComponents();

  unsigned char * native = this->internaldata->getBuffer();
  unsigned char * local = this->buffer;
 
  switch (depth) {
  case SoOffscreenRenderer::RGB_TRANSPARENCY:
    memcpy(local, native, pixels * depth);
    break;

  case SoOffscreenRenderer::RGB:
    {
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
      for (int i=0; i < pixels; i++) {
        int val = (int(*native++) + int(*native++) + int(*native++)) / 3;
        *local++ = (unsigned char)val;
        *local++ = *native++;
      }
    }
    break;

  case SoOffscreenRenderer::LUMINANCE:
    {
      for (int i=0; i < pixels; i++) {
        uint32_t val = 76*native[0]+155*native[1]+26*native[2];
        *local++ = (unsigned char)(val>>8);
        native += 4;
      }
    }
    break;

  default:
    assert(FALSE && "unknown buffer format"); break;
  }
}



/*!
  Render the scenegraph rooted at \a scene into our internal pixel
  buffer.


  Important note: make sure you pass in a \a scene node pointer which
  has both a camera and at least one lightsource below it -- otherwise
  you are likely to end up with just a blank or black image buffer.

  This mistake is easily made if you use an SoOffscreenRenderer on a
  scenegraph from one of the standard viewer components, as you will
  often just leave the addition of a camera and a headlight
  lightsource to the viewer to set up. This camera and lightsource are
  then part of the viewer's private "super-graph" outside of the scope
  of the scenegraph passed in by the application programmer. To make
  sure the complete scenegraph (including the viewer's "private parts"
  (*snicker*)) are passed to this method, you can get the scenegraph
  root from the viewer's internal SoSceneManager instance instead of
  from the viewer's own getSceneGraph() method, like this:

  \code
  SoOffscreenRenderer * myRenderer = new SoOffscreenRenderer(vpregion);
  SoNode * root = myViewer->getSceneManager()->getSceneGraph();
  SbBool ok = myRenderer->render(root);
  // [then use image buffer in a texture, or write it to file, or whatever]
  \endcode

  If you do this and still get a blank buffer, another common problem
  is to have a camera which is not actually pointing at the scene
  geometry you want a snapshot of. If you suspect that could be the
  cause of problems on your end, take a look at SoCamera::pointAt()
  and SoCamera::viewAll() to see how you can make a camera node
  guaranteed to be directed at the scene geometry.

  \sa writeToRGB()
*/
SbBool
SoOffscreenRenderer::render(SoNode * scene)
{
  return PRIVATE(this)->renderFromBase(scene);
}



/*!
  Render the \a scene path into our internal memory buffer.
*/
SbBool
SoOffscreenRenderer::render(SoPath * scene)
{
  return PRIVATE(this)->renderFromBase(scene);
}



/*!
  Returns the offscreen memory buffer.
*/
unsigned char *
SoOffscreenRenderer::getBuffer(void) const
{
  return PRIVATE(this)->buffer;
}



//
// avoid endian problems (little endian sucks, right? :)
//
static int
write_short(FILE * fp, unsigned short val)
{
  unsigned char tmp[2];
  tmp[0] = (unsigned char)(val >> 8);
  tmp[1] = (unsigned char)(val & 0xff);
  return fwrite(&tmp, 2, 1, fp);
}



/*!
  Writes the buffer in SGI RGB format by appending it to the already
  open file. Returns \c FALSE if writing fails.

  Important note: do \e not use this method when the Coin library has
  been compiled as an MSWindows DLL, as passing FILE* instances back
  or forth to DLLs is dangerous and will most likely cause a
  crash. This is an intrinsic limitation for MSWindows DLLs.
*/
SbBool
SoOffscreenRenderer::writeToRGB(FILE * fp) const
{
  // FIXME: add code to rle rows, pederb 2000-01-10
  // FIXME: errorchecking when writing! 20010625 mortene.

  if (PRIVATE(this)->internaldata) {
    
    SbVec2s size = PRIVATE(this)->internaldata->getSize();
    if(PRIVATE(this)->mustusesubscreens){
      size[0] = PRIVATE(this)->requestedsize[0];
      size[1] = PRIVATE(this)->requestedsize[1];
    }

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

    unsigned char * ptr = PRIVATE(this)->buffer;
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
  Opens a file with the given name and writes the offscreen buffer in
  SGI RGB format to the new file. If the file already exists, it will
  be overwritten (if permitted by the filesystem).

  Returns \c TRUE if all went ok, otherwise \c FALSE.
*/
SbBool
SoOffscreenRenderer::writeToRGB(const char * filename) const
{
  FILE * rgbfp = fopen(filename, "wb");
  if (!rgbfp) {
    SoDebugError::postWarning("SoOffscreenRenderer::writeToRGB",
                              "couldn't open file '%s'", filename);
    return FALSE;
  }
  SbBool result = this->writeToRGB(rgbfp);
  (void)fclose(rgbfp);
  return result;
}



static int
encode_ascii85(const unsigned char * in, unsigned char * out)
{
  uint32_t data =
    (uint32_t(in[0])<<24) |
    (uint32_t(in[1])<<16) |
    (uint32_t(in[2])<< 8) |
    (uint32_t(in[3]));

  if (data == 0) {
    out[0] = 'z';
    return 1;
  }
  out[4] = (unsigned char) (data%85 + '!');
  data /= 85;
  out[3] = (unsigned char) (data%85 + '!');
  data /= 85;
  out[2] = (unsigned char) (data%85 + '!');
  data /= 85;
  out[1] = (unsigned char) (data%85 + '!');
  data /= 85;
  out[0] = (unsigned char) (data%85 + '!');
  return 5;
}



static void
output_ascii85(FILE * fp,
               const unsigned char val,
               unsigned char * tuple,
               unsigned char * linebuf,
               int & tuplecnt, int & linecnt,
               const int rowlen,
               const SbBool flush = FALSE)
{
  int i;
  if (flush) {
    // fill up tuple
    for (i = tuplecnt; i < 4; i++) tuple[i] = 0;
  }
  else {
    tuple[tuplecnt++] = val;
  }
  if (flush || tuplecnt == 4) {
    if (tuplecnt) {
      int add = encode_ascii85(tuple, linebuf + linecnt);
      if (flush) {
        if (add == 1) {
          for (i = 0; i < 5; i++) linebuf[linecnt+i] = '!';
        }
        linecnt += tuplecnt + 1;
      }
      else linecnt += add;
      tuplecnt = 0;
    }
    if (linecnt >= rowlen) {
      unsigned char store = linebuf[rowlen];
      linebuf[rowlen] = 0;
      fprintf(fp, "%s\n", linebuf);
      linebuf[rowlen] = store;
      for (i = rowlen; i < linecnt; i++) {
        linebuf[i-rowlen] = linebuf[i];
      }
      linecnt -= rowlen;
    }
    if (flush && linecnt) {
      linebuf[linecnt] = 0;
      fprintf(fp, "%s\n", linebuf);
    }
  }
}



static void
flush_ascii85(FILE * fp,
              unsigned char * tuple,
              unsigned char * linebuf,
              int & tuplecnt, int & linecnt,
              const int rowlen)
{
  output_ascii85(fp, 0, tuple, linebuf, tuplecnt, linecnt, rowlen, TRUE);
}



/*!
  Writes the buffer in Postscript format by appending it to the
  already open file. Returns \c FALSE if writing fails.

  Important note: do \e not use this method when the Coin library has
  been compiled as an MSWindows DLL, as passing FILE* instances back
  or forth to DLLs is dangerous and will most likely cause a
  crash. This is an intrinsic limitation for MSWindows DLLs.
*/
SbBool
SoOffscreenRenderer::writeToPostScript(FILE * fp) const
{
  // just choose a page size of 8.5 x 11 inches (A4)
  return this->writeToPostScript(fp, SbVec2f(8.5f, 11.0f));
}



/*!
  Opens a file with the given name and writes the offscreen buffer in
  Postscript format to the new file. If the file already exists, it
  will be overwritten (if permitted by the filesystem).

  Returns \c TRUE if all went ok, otherwise \c FALSE.
*/
SbBool
SoOffscreenRenderer::writeToPostScript(const char * filename) const
{
  FILE * psfp = fopen(filename, "wb");
  if (!psfp) {
    SoDebugError::postWarning("SoOffscreenRenderer::writeToPostScript",
                              "couldn't open file '%s'", filename);
    return FALSE;
  }
  SbBool result = this->writeToPostScript(psfp);
  (void)fclose(psfp);
  return result;
}



/*!
  Writes the buffer to a file in Postscript format, with \a printsize
  dimensions.

  Important note: do \e not use this method when the Coin library has
  been compiled as an MSWindows DLL, as passing FILE* instances back
  or forth to DLLs is dangerous and will most likely cause a
  crash. This is an intrinsic limitation for MSWindows DLLs.
*/
SbBool
SoOffscreenRenderer::writeToPostScript(FILE * fp,
                                       const SbVec2f & printsize) const
{
  if (PRIVATE(this)->internaldata) {
    const SbVec2s size = PRIVATE(this)->requestedsize;
    const int nc = this->getComponents();
    const float defaultdpi = 72.0f; // we scale against this value
    const float dpi = this->getScreenPixelsPerInch();
    const SbVec2s pixelsize((short)(printsize[0]*defaultdpi),
                            (short)(printsize[1]*defaultdpi));

    const unsigned char * src = PRIVATE(this)->buffer;
    const int chan = nc <= 2 ? 1 : 3;
    const SbVec2s scaledsize((short) ceil(size[0]*defaultdpi/dpi),
                             (short) ceil(size[1]*defaultdpi/dpi));

    fprintf(fp, "%%!PS-Adobe-2.0 EPSF-1.2\n");
    fprintf(fp, "%%%%Pages: 1\n");
    fprintf(fp, "%%%%PageOrder: Ascend\n");
    fprintf(fp, "%%%%BoundingBox: 0 %d %d %d\n",
            pixelsize[1]-scaledsize[1],
            scaledsize[0],
            pixelsize[1]);
    fprintf(fp, "%%%%Creator: Coin <http://www.coin3d.org>\n");
    fprintf(fp, "%%%%EndComments\n");

    fprintf(fp, "\n");
    fprintf(fp, "/origstate save def\n");
    fprintf(fp, "\n");
    fprintf(fp, "%% workaround for bug in some PS interpreters\n");
    fprintf(fp, "%% which doesn't skip the ASCII85 EOD marker.\n");
    fprintf(fp, "/~ {currentfile read pop pop} def\n\n");
    fprintf(fp, "/image_wd %d def\n", size[0]);
    fprintf(fp, "/image_ht %d def\n", size[1]);
    fprintf(fp, "/pos_wd %d def\n", size[0]);
    fprintf(fp, "/pos_ht %d def\n", size[1]);
    fprintf(fp, "/image_dpi %g def\n", dpi);
    fprintf(fp, "/image_scale %g image_dpi div def\n", defaultdpi);
    fprintf(fp, "/image_chan %d def\n", chan);
    fprintf(fp, "/xpos_offset 0 image_scale mul def\n");
    fprintf(fp, "/ypos_offset 0 image_scale mul def\n");
    fprintf(fp, "/pix_buf_size %d def\n\n", size[0]*chan);
    fprintf(fp, "/page_ht %g %g mul def\n", printsize[1], defaultdpi);
    fprintf(fp, "/page_wd %g %g mul def\n", printsize[0], defaultdpi);
    fprintf(fp, "/image_xpos 0 def\n");
    fprintf(fp, "/image_ypos page_ht pos_ht image_scale mul sub def\n");
    fprintf(fp, "image_xpos xpos_offset add image_ypos ypos_offset add translate\n");
    fprintf(fp, "\n");
    fprintf(fp, "/pix pix_buf_size string def\n");
    fprintf(fp, "image_wd image_scale mul image_ht image_scale mul scale\n");
    fprintf(fp, "\n");
    fprintf(fp, "image_wd image_ht 8\n");
    fprintf(fp, "[image_wd 0 0 image_ht 0 0]\n");
    fprintf(fp, "currentfile\n");
    fprintf(fp, "/ASCII85Decode filter\n");
    // fprintf(fp, "/RunLengthDecode filter\n"); // FIXME: add later
    if (chan == 3) fprintf(fp, "false 3\ncolorimage\n");
    else fprintf(fp,"image\n");

    const int rowlen = 72;
    int num = size[0] * size[1];
    unsigned char tuple[4];
    unsigned char linebuf[rowlen+5];
    int tuplecnt = 0;
    int linecnt = 0;
    int cnt = 0;
    while (cnt < num) {
      switch (nc) {
      default: // avoid warning
      case 1:
        output_ascii85(fp, src[cnt], tuple, linebuf, tuplecnt, linecnt, rowlen);
        break;
      case 2:
        output_ascii85(fp, src[cnt*2], tuple, linebuf, tuplecnt, linecnt, rowlen);
        break;
      case 3:
        output_ascii85(fp, src[cnt*3], tuple, linebuf, tuplecnt, linecnt, rowlen);
        output_ascii85(fp, src[cnt*3+1], tuple, linebuf, tuplecnt, linecnt, rowlen);
        output_ascii85(fp, src[cnt*3+2], tuple, linebuf, tuplecnt, linecnt, rowlen);
        break;
      case 4:
        output_ascii85(fp, src[cnt*4], tuple, linebuf, tuplecnt, linecnt, rowlen);
        output_ascii85(fp, src[cnt*4+1], tuple, linebuf, tuplecnt, linecnt,rowlen);
        output_ascii85(fp, src[cnt*4+2], tuple, linebuf, tuplecnt, linecnt, rowlen);
        break;
      }
      cnt++;
    }

    // flush data in ascii85 encoder
    flush_ascii85(fp, tuple, linebuf, tuplecnt, linecnt, rowlen);

    fprintf(fp, "~>\n\n"); // ASCII85 EOD marker
    fprintf(fp, "origstate restore\n");
    fprintf(fp, "\n");
    fprintf(fp, "%%%%Trailer\n");
    fprintf(fp, "\n");
    fprintf(fp, "%%%%EOF\n");
    return (SbBool) (ferror(fp) == 0);
  }
  return FALSE;
}



/*!
  Opens a file with the given name and writes the offscreen buffer in
  Postscript format with \a printsize dimensions to the new file. If
  the file already exists, it will be overwritten (if permitted by the
  filesystem).

  Returns \c TRUE if all went ok, otherwise \c FALSE.
*/
SbBool
SoOffscreenRenderer::writeToPostScript(const char * filename,
                                       const SbVec2f & printsize) const
{
  FILE * psfp = fopen(filename, "wb");
  if (!psfp) {
    SoDebugError::postWarning("SoOffscreenRenderer::writeToPostScript",
                              "couldn't open file '%s'", filename);
    return FALSE;
  }
  SbBool result = this->writeToPostScript(psfp, printsize);
  (void)fclose(psfp);
  return result;
}



// FIXME: the file format support checking could have been done
// better, for instance by using MIME types. 20020206 mortene.

/*!
  Returns \c TRUE if the buffer can be saved as a file of type \a
  filetypeextension, using SoOffscreenRenderer::writeToFile().  This
  function needs simage v1.1 or newer.

  Examples of possibly supported extensions are: "jpg", "png", "tiff",
  "gif", "bmp", etc. The extension match is not case sensitive.

  Which formats are \e actually supported depends on the capabilities
  of Coin's support library for handling import and export of
  pixel-data files: the simage library. If the simage library is not
  installed on your system, no extension output formats will be
  supported.

  Also, note that it is possible to build and install a simage library
  that lacks support for most or all of the file formats it is \e
  capable of supporting. This is so because the simage library depends
  on other, external 3rd party libraries -- in the same manner as Coin
  depends on the simage library for added file format support.

  The two built-in formats that are supported through the
  SoOffscreenRenderer::writeToRGB() and
  SoOffscreenRenderer::writeToPostScript() methods (for SGI RGB format
  and for Adobe Postscript files, respectively) are \e not considered
  by this method, as those two formats are guaranteed to \e always be
  supported through those functions.

  So if you want to be guaranteed to be able to export a screenshot in
  your wanted format, you will have to use either one of the above
  mentioned method for writing SGI RGB or Adobe Postscript directly,
  or make sure the Coin library has been built and is running on top
  of a version of the simage library (that you have preferably built
  yourself) with the file format(s) you want support for.


  This method is an extension versus the original SGI Open Inventor
  API.

  \sa  getNumWriteFiletypes(), getWriteFiletypeInfo(), writeToFile()
*/
SbBool
SoOffscreenRenderer::isWriteSupported(const SbName & filetypeextension) const
{
  if (!simage_wrapper()->versionMatchesAtLeast(1,1,0)) {
#if COIN_DEBUG
    SoDebugError::postInfo("SoOffscreenRenderer::isWriteSupported",
                           "You need simage v1.1 for this functionality.");
#endif // COIN_DEBUG
    return FALSE;
  }
  int ret = simage_wrapper()->simage_check_save_supported(filetypeextension.getString());
  return ret ? TRUE : FALSE;
}



/*!
  Returns the number of available exporters. Detailed information
  about the exporters can then be found using getWriteFiletypeInfo().

  See SoOffscreenRenderer::isWriteSupported() for information about
  which file formats you can expect to be present.

  Note that the two built-in export formats, SGI RGB and Adobe
  Postscript, are not counted.

  This method is an extension versus the original SGI Open Inventor
  API.

  \sa getWriteFiletypeInfo()
*/
int
SoOffscreenRenderer::getNumWriteFiletypes(void) const
{
  if (!simage_wrapper()->versionMatchesAtLeast(1,1,0)) {
#if COIN_DEBUG
    SoDebugError::postInfo("SoOffscreenRenderer::getNumWriteFiletypes",
                           "You need simage v1.1 for this functionality.");
#endif // COIN_DEBUG
    return 0;
  }
  return simage_wrapper()->simage_get_num_savers();
}



/*!
  Returns information about an image exporter. \a extlist is a list of
  filenameextensions for a file format. E.g. for JPEG it is legal to
  use both jpg and jpeg. \a fullname is the full name of the image
  format. \a description is an optional string with more information
  about the file format.

  See SoOffscreenRenderer::isWriteSupported() for information about
  which file formats you can expect to be present.

  This method is an extension versus the original SGI Open Inventor
  API.

  Here is a stand-alone, complete code example that shows how you can
  check exactly which output formats are supported:

  \code
  #include <Inventor/SoDB.h>
  #include <Inventor/SoOffscreenRenderer.h>

  int
  main(int argc, char **argv)
  {
    SoDB::init();
    SoOffscreenRenderer * r = new SoOffscreenRenderer(*(new SbViewportRegion));
    int num = r->getNumWriteFiletypes();

    if (num == 0) {
      (void)fprintf(stdout,
                    "No image formats supported by the "
                    "SoOffscreenRenderer except SGI RGB and Postscript.\n");
    }
    else {
      for (int i=0; i < num; i++) {
        SbList<SbName> extlist;
        SbString fullname, description;
        r->getWriteFiletypeInfo(i, extlist, fullname, description);
        (void)fprintf(stdout, "%s: %s (extension%s: ",
                      fullname.getString(), description.getString(),
                      extlist.getLength() > 1 ? "s" : "");
        for (int j=0; j < extlist.getLength(); j++) {
          (void)fprintf(stdout, "%s%s", j>0 ? ", " : "", extlist[j].getString());
        }
        (void)fprintf(stdout, ")\n");
      }
    }

    delete r;
    return 0;
  }
  \endcode

  \sa getNumWriteFiletypes(), writeToFile()
*/
void
SoOffscreenRenderer::getWriteFiletypeInfo(const int idx,
                                          SbList <SbName> & extlist,
                                          SbString & fullname,
                                          SbString & description)
{
  if (!simage_wrapper()->versionMatchesAtLeast(1,1,0)) {
#if COIN_DEBUG
    SoDebugError::postInfo("SoOffscreenRenderer::getNumWriteFiletypes",
                           "You need simage v1.1 for this functionality.");
#endif // COIN_DEBUG
    return;
  }
  extlist.truncate(0);
  assert(idx >= 0 && idx < this->getNumWriteFiletypes());
  void * saver = simage_wrapper()->simage_get_saver_handle(idx);
  SbString allext(simage_wrapper()->simage_get_saver_extensions(saver));
  const char * start = allext.getString();
  const char * curr = start;
  const char * end = strchr(curr, ',');
  while (end) {
    SbString ext = allext.getSubString(curr-start, end-start-1);
    extlist.append(SbName(ext.getString()));
    curr = end+1;
    end = strchr(curr, ',');
  }
  SbString ext = allext.getSubString(curr-start);
  extlist.append(SbName(ext.getString()));
  const char * fullname_s = simage_wrapper()->simage_get_saver_fullname(saver);
  const char * desc_s = simage_wrapper()->simage_get_saver_description(saver);
  fullname = fullname_s ? SbString(fullname_s) : SbString("");
  description = desc_s ? SbString(desc_s) : SbString("");
}


/*!
  Saves the buffer to \a filename, in the filetype specified by \a
  filetypeextensions.

  Note that you must still specify the \e full \a filename for the
  first argument, i.e. the second argument will not automatically be
  attached to the filename -- it is only used to decide the filetype.

  This method is an extension versus the orignal SGI Open Inventor
  API.

  \sa isWriteSupported()
*/
SbBool
SoOffscreenRenderer::writeToFile(const SbString & filename, const SbName & filetypeextension) const
{
  if (!simage_wrapper()->versionMatchesAtLeast(1,1,0)) {
    return FALSE;
  }
  if (PRIVATE(this)->internaldata) {
    SbVec2s size = PRIVATE(this)->requestedsize;
    int comp = (int) this->getComponents();
    unsigned char * bytes = PRIVATE(this)->buffer;
    int ret = simage_wrapper()->simage_save_image(filename.getString(),
                                                  bytes,
                                                  int(size[0]), int(size[1]), comp,
                                                  filetypeextension.getString());
    return ret ? TRUE : FALSE;
  }
  return FALSE;
}


void
SoOffscreenRendererP::setupSubscreens(SbVec2s totalsize)
{

  this->maxres = this->master->getMaximumResolution();

  numsubscreens[0] = 0;
  numsubscreens[1] = 0;
 
  // Was this really called for?
  if((maxres[0] > totalsize[0]) &&
     (maxres[1] > totalsize[1])){
    this->mustusesubscreens = FALSE;  // False alarm, DONT use subscreens...
    return;
  }
  
  // Find number of horizontal subscreens
  short size = totalsize[0];
  while(size > 0){
    size -= maxres[0];
    ++numsubscreens[0];
  }

  // Find number of vertical subscreens
  size = totalsize[1];
  while(size > 0){
    size -= maxres[1];
    ++numsubscreens[1];
  }

  lastsubscreensize[0] = 0;
  lastsubscreensize[1] = 0;
  if(maxres[0] < totalsize[0])
    lastsubscreensize[0] = maxres[0] - ((maxres[0]*numsubscreens[0]) - totalsize[0]);
  if(maxres[1] < totalsize[1])
    lastsubscreensize[1] = maxres[1] - ((maxres[1]*numsubscreens[1]) - totalsize[1]);

  // Last check... 
  if(lastsubscreensize[0] == maxres[0])
    lastsubscreensize[0] = 0;
  if(lastsubscreensize[1] == maxres[1])
    lastsubscreensize[1] = 0;

  return;
}


void 
SoOffscreenRendererP::setSubscreenCameraPosition(int renderpass, SoCamera * cam)
{
 
  int subscreenposx = 0;
  int subscreenposy = 0;

  if(renderpass >= numsubscreens[0]*numsubscreens[1]){
    subscreenposx = numsubscreens[0]-1;
    subscreenposy = numsubscreens[1]-1;
    SoDebugError::post("setSubscreenCameraPosition",
                       "ERROR: Internal error. Not enough subscreens allocated for offscreenrendering.\n"
                       "Rendering to last allocated subscreen.");
  } else {
    subscreenposy = renderpass / this->numsubscreens[0]; // Exploiting integer division loss
    subscreenposx = renderpass - subscreenposy*this->numsubscreens[0];
  }

  SoState * state = (this->master->getGLRenderAction())->getState();
  const SbViewportRegion & vp = SoViewportRegionElement::get(state);

  // A small trick to change the aspect ratio without changing the scenegraph camera
  SbViewVolume vv;
  int vpm = cam->viewportMapping.getValue();
  float aspectratio = this->viewport.getViewportAspectRatio();

  switch(vpm) {
  case SoCamera::CROP_VIEWPORT_FILL_FRAME:
  case SoCamera::CROP_VIEWPORT_LINE_FRAME:
  case SoCamera::CROP_VIEWPORT_NO_FRAME:
    vv = cam->getViewVolume(0.0f);
    break;
  case SoCamera::ADJUST_CAMERA:
    vv = cam->getViewVolume(aspectratio);
    if (aspectratio < 1.0f) vv.scale(1.0f / aspectratio);
    break;
  case SoCamera::LEAVE_ALONE:
    vv = cam->getViewVolume(0.0f);
    break;
  default:
    assert(0 && "unknown viewport mapping");
    break;
  }

  float left =   ((float) (subscreenposx*this->maxres[0])) / (float)requestedsize[0];
  float bottom = ((float) ((numsubscreens[1]-subscreenposy-1)*this->maxres[1])) / (float)requestedsize[1];
  float right =  ((float) ((subscreenposx + 1)*this->maxres[0])) / (float)requestedsize[0];
  float top =    ((float) ((numsubscreens[1] - subscreenposy)*this->maxres[1])) / (float)requestedsize[1];

  // Reshape view volume
  vv = vv.narrow(left, bottom, right, top);

  SbMatrix proj, affine;  
  vv.getMatrices(affine, proj);
  
  // Support antialiasing if renderpasses > 1
  if(renderaction->getNumPasses() > 1){
    SbVec3f jittervec;
    SbMatrix m;
    const int vpsize[2] = { maxres[0], maxres[1] };
    coin_viewvolume_jitter(renderaction->getNumPasses(), renderaction->getCurPass(), 
                           vpsize, (float *)jittervec.getValue());
    m.setTranslate(jittervec);
    proj.multRight(m);
  }
  
  SoCullElement::setViewVolume(state, vv);
  SoViewVolumeElement::set(state, cam, vv);
  SoProjectionMatrixElement::set(state, cam, proj);
  SoViewingMatrixElement::set(state, cam, affine);
    
}


void 
SoOffscreenRendererP::pasteSubscreen(int renderpass)
{
  
  int depth = this->master->getComponents();
  int subscreenposy = renderpass / this->numsubscreens[0]; // Exploiting integer division loss
  int subscreenposx = renderpass - subscreenposy*this->numsubscreens[0];

  int suboffset = 0;
  int suboffsetinc = this->maxres[0]*depth;
  
  int offset = ((this->numsubscreens[1] - subscreenposy - 1)*this->requestedsize[0]*maxres[1] + subscreenposx*maxres[0])*depth;
  int offsetinc = this->requestedsize[0]*depth;

  int linelen = this->maxres[0]*depth;
  if(subscreenposx == numsubscreens[0]-1){
    if(this->lastsubscreensize[0] != 0)
      linelen = this->lastsubscreensize[0]*depth;
  }

  int lines = this->maxres[1];
  
  if(subscreenposy == 0){
    if(this->lastsubscreensize[1] != 0){
      lines = this->lastsubscreensize[1];
      offset = (this->requestedsize[0]*(this->requestedsize[1] - lines) + subscreenposx*maxres[0])*depth;
    }
  }
   
  // Do the pasting
  for(int j=0;j<lines;++j){ 
    memcpy((unsigned char *) this->buffer+offset,(unsigned char *) this->subscreen + suboffset, linelen);        
    offset += offsetinc;
    suboffset += suboffsetinc;
  }     
 
}
