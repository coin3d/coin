/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
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
  \class SoSceneManager SoSceneManager.h Inventor/SoSceneManager.h
  \brief The SoSceneManager class provides the main interface between the scene graph and the GUI toolkit.
  \ingroup general

  The renderarea class from the GUI toolkit you are using uses this
  class as the interface against the scene graph. Event handling and
  providing "hooks" to do rendering are the main functions of the
  class.

  A Coin library instance within an application will typically contain
  a single SoSceneManager object. The pointer for this object is
  stored in the GUI renderarea class.
*/

#include <Inventor/SoSceneManager.h>
#include <Inventor/SoDB.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoHandleEventAction.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/fields/SoSFTime.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/sensors/SoNodeSensor.h>
#include <Inventor/sensors/SoOneShotSensor.h>

#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H
#ifdef HAVE_WINDOWS_H
#include <windows.h>
#endif // HAVE_WINDOWS_H
#include <GL/gl.h>
#include <assert.h>

// defines for the flags member
#define FLAG_RGBMODE 0x0001
#define FLAG_ACTIVE  0x0002
#define FLAG_SENDVP  0x0004

/*!
  \typedef SoSceneManagerRenderCB(void * userdata, SoSceneManager * mgr)

  Render callback function must have this signature.
*/


SbBool SoSceneManager::touchtimer = TRUE;


/*!
  Constructor. Sets up default SoGLRenderAction and
  SoHandleEventAction instances.
 */
SoSceneManager::SoSceneManager(void)
{
  this->glaction = new SoGLRenderAction(SbViewportRegion(400, 400));
  this->deleteglaction = TRUE;

  this->handleeventaction =
    new SoHandleEventAction(SbViewportRegion(400, 400));
  this->deletehandleeventaction = TRUE;

  this->scene = NULL;
  this->rootsensor = NULL;
  this->redrawshot = NULL;

  this->backgroundindex = 0;
  this->backgroundcolor.setValue(0.0f, 0.0f, 0.0f);
  // rgbmode by default, and send vp to GL on first render
  this->flags = FLAG_RGBMODE|FLAG_SENDVP;
  this->redrawpri = SoSceneManager::getDefaultRedrawPriority();

  this->rendercb = NULL;
  this->rendercbdata = NULL;
}

/*!
  Destructor.
 */
SoSceneManager::~SoSceneManager()
{
  if (this->deleteglaction) delete this->glaction;
  if (this->deletehandleeventaction) delete this->handleeventaction;

  this->setSceneGraph(NULL);
  delete this->rootsensor;
  delete this->redrawshot;
}

/*!
  Render the scene graph.

  If \a clearwindow is \c TRUE, clear the rendering buffer before
  drawing. If \a clearzbuffer is \c TRUE, clear the depth buffer
  values before rendering. Both of these arguments should normally be
  \c TRUE, but they can be set to \c FALSE to optimize for special
  cases (e.g. when doing wireframe rendering one doesn't need a depth
  buffer).
 */
void
SoSceneManager::render(const SbBool clearwindow, const SbBool clearzbuffer)
{
  GLbitfield mask = 0;
  if (clearwindow) mask |= GL_COLOR_BUFFER_BIT;
  if (clearzbuffer) mask |= GL_DEPTH_BUFFER_BIT;

  if (mask) {
    if (this->flags & FLAG_RGBMODE) {
      glClearColor(this->backgroundcolor[0],
                   this->backgroundcolor[1],
                   this->backgroundcolor[2],
                   0.0f);
    }
    else {
      glClearIndex(backgroundindex);
    }
    // some (most) GL drivers need to have the viewport set correctly
    // before clearing. If the window size changes, the
    // SoGLViewportRegionElement will not set the GL viewport until the
    // glaction is applied, which is too late.  FIXME: bad design
    if (this->flags & FLAG_SENDVP) {
      this->flags &= ~FLAG_SENDVP;
      const SbViewportRegion & vp = this->getViewportRegion();
      SbVec2s origin = vp.getViewportOriginPixels();
      SbVec2s size = vp.getViewportSizePixels();
      glViewport((GLint) origin[0], (GLint) origin[1],
                 (GLint) size[0], (GLint) size[1]);
    }
    glClear(mask);
  }


  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // If there has been changes in the scene graph leading to a node
  // sensor detect and schedule before we've gotten around to serving
  // the current redraw -- remove it. This will prevent infinite loops
  // in the case of scenegraph modifications between a nodesensor
  // trigger and SoSceneManager::render() actually being called. It
  // will also help us avoid "double redraws" at expose events.
#if 0 // debug
  SoDebugError::postInfo("SoSceneManager::render",
                         "rootsensor %p isScheduled: %d",
                         this->rootsensor,
                         this->rootsensor ?
                         this->rootsensor->isScheduled() : -1);
#endif // debug
  if (this->rootsensor && this->rootsensor->isScheduled()) {
#if 0 // debug
    SoDebugError::postInfo("SoSceneManager::render",
                           "rootsensor unschedule");
#endif // debug
    this->rootsensor->unschedule();
  }


  // FIXME: this does the same thing for the actual redraw delay
  // sensor as for the nodesensor above. I don't think this is
  // necessary, though. If we never see the debug msg below during
  // development, we can probably safely remove this snippet of
  // code. 19990226 mortene.
  if (this->redrawshot && this->redrawshot->isScheduled()) {
#if 0 // debug
    SoDebugError::postInfo("SoSceneManager::render",
                           "redrawshot unschedule");
#endif // debug
    this->redrawshot->unschedule();
  }

#if 0 // debug
  SoDebugError::postInfo("SoSceneManager::render",
                         "applying the GLRenderAction "
                         "to the scenegraph _now_");
#endif // debug

#if COIN_DEBUG && 0 // debug
  SoDebugError::postInfo("SoSceneManager::render", "before glrender->apply()");
#endif // debug

  // Apply the SoGLRenderAction to the scenegraph root.
  if (this->scene) this->glaction->apply(this->scene);

#if COIN_DEBUG && 0 // debug
  SoDebugError::postInfo("SoSceneManager::render", "after glrender->apply()");
#endif // debug
}

/*!
  Process the given event by applying an SoHandleEventAction on the
  scene graph.
 */
SbBool
SoSceneManager::processEvent(const SoEvent * const event)
{
  assert(this->handleeventaction);

  this->handleeventaction->setEvent(event);
  if (this->scene) this->handleeventaction->apply(this->scene);
  return this->handleeventaction->isHandled();
}

/*!
  Reinitialize after parameters affecting the OpenGL context has
  changed.
*/
void
SoSceneManager::reinitialize(void)
{
  this->glaction->invalidateState();
}

/*!
  Redraw at first opportunity as system becomes idle.

  Multiple calls to this method before an actual redraw has taken
  place will only result in a single redraw of the scene.
*/
void
SoSceneManager::scheduleRedraw(void)
{
  if ((this->flags & FLAG_ACTIVE) && this->rendercb) {
    if (!this->redrawshot) {
      this->redrawshot =
        new SoOneShotSensor(SoSceneManager::redrawshotTriggeredCB, this);
      this->redrawshot->setPriority(this->getRedrawPriority());
    }

#if 0 // debug
    SoDebugError::postInfo("SoSceneManager::scheduleRedraw",
                           "scheduling redrawshot (oneshotsensor) %p",
                           this->redrawshot);
#endif // debug
    this->redrawshot->schedule();
  }
}

/*!
  Returns the \e active flag.
 */
int
SoSceneManager::isActive(void) const
{
  return (this->flags & FLAG_ACTIVE) != 0;
}

/*!
  Do an immediate redraw by calling the redraw callback function.
 */
void
SoSceneManager::redraw(void)
{
  if (this->rendercb) {
    this->rendercb(this->rendercbdata, this);

    // Automatically re-triggers rendering if any animation stuff is
    // connected to the realTime field.
    if (this->touchtimer) {
      // FIXME: it would be more elegant to use a private field class
      // inheriting SoSFTime ("SFRealTime") which could just be
      // touch()'ed, and which would do lazy reading of time-of-day on
      // demand. 20000316 mortene.
      SoField * realtime = SoDB::getGlobalField("realTime");
      if (realtime && (realtime->getTypeId() == SoSFTime::getClassTypeId())) {
        ((SoSFTime *)realtime)->setValue(SbTime::getTimeOfDay());
      }
    }
  }
}

// Internal callback.
void
SoSceneManager::redrawshotTriggeredCB(void * data, SoSensor * /* sensor */)
{
#if COIN_DEBUG && 0 // debug
  SoDebugError::postInfo("SoSceneManager::redrawshotTriggeredCB", "start");
#endif // debug

  ((SoSceneManager *)data)->redraw();

#if COIN_DEBUG && 0 // debug
  SoDebugError::postInfo("SoSceneManager::redrawshotTriggeredCB", "done\n\n");
#endif // debug
}

// Internal callback.
void
SoSceneManager::nodesensorCB(void * data, SoSensor * /* sensor */)
{
#if 0 // debug
  SoDebugError::postInfo("SoSceneManager::nodesensorCB",
                         "detected change in scene graph");
#endif // debug
  ((SoSceneManager *)data)->scheduleRedraw();
}

/*!
  Set the node which is top of the scene graph we're managing.  The \a
  sceneroot node reference count will be increased by 1, and any
  previously set scene graph top node will have it's reference count
  decreased by 1.

  \sa getSceneGraph()
*/
void
SoSceneManager::setSceneGraph(SoNode * const sceneroot)
{
  if (this->rootsensor) this->rootsensor->detach();
  // Don't unref() until after we've set up the new root, in case the
  // old root == the new sceneroot. (Just to be that bit more robust.)
  SoNode * oldroot = this->scene;

  this->scene = sceneroot;

  if (this->scene) {
    this->scene->ref();

    if (!this->rootsensor)
      this->rootsensor = new SoNodeSensor(SoSceneManager::nodesensorCB, this);

    this->rootsensor->attach(sceneroot);
  }

  if (oldroot) oldroot->unref();
}

/*!
  Returns pointer to root of scene graph.
 */
SoNode *
SoSceneManager::getSceneGraph(void) const
{
  return this->scene;
}

/*!
  Update window size. Typically used by the GUI renderarea instance
  upon user interaction.
 */
void
SoSceneManager::setWindowSize(const SbVec2s & newsize)
{
#if COIN_DEBUG && 0 // debug
  SoDebugError::postInfo("SoSceneManager::setWindowSize",
                         "(%d, %d)", newsize[0], newsize[1]);
#endif // debug

  SbViewportRegion region = this->glaction->getViewportRegion();
  region.setWindowSize(newsize[0], newsize[1]);
  this->glaction->setViewportRegion(region);

  region = this->handleeventaction->getViewportRegion();
  region.setWindowSize(newsize[0], newsize[1]);
  this->handleeventaction->setViewportRegion(region);
  this->flags |= FLAG_SENDVP;
}

/*!
  Returns the current render action window size.
 */
const SbVec2s &
SoSceneManager::getWindowSize(void) const
{
  return this->glaction->getViewportRegion().getWindowSize();
}

/*!
  Set size of rendering area for the viewport within the current
  window.
 */
void
SoSceneManager::setSize(const SbVec2s & newsize)
{
#if COIN_DEBUG && 0 // debug
  SoDebugError::postInfo("SoSceneManager::setSize",
                         "(%d, %d)", newsize[0], newsize[1]);
#endif // debug

  SbViewportRegion region = this->glaction->getViewportRegion();
  SbVec2s origin = region.getViewportOriginPixels();
  region.setViewportPixels(origin, newsize);
  this->glaction->setViewportRegion(region);

  region = this->handleeventaction->getViewportRegion();
  origin = region.getViewportOriginPixels();
  region.setViewportPixels(origin, newsize);
  this->handleeventaction->setViewportRegion(region);
  this->flags |= FLAG_SENDVP;
}

/*!
  Returns size of render area.
 */
const SbVec2s &
SoSceneManager::getSize(void) const
{
  return this->glaction->getViewportRegion().getViewportSizePixels();
}

/*!
  Set origin of render area of the viewport region within the
  rendering window.
 */
void
SoSceneManager::setOrigin(const SbVec2s & newOrigin)
{
  SbViewportRegion region = this->glaction->getViewportRegion();
  SbVec2s size = region.getViewportSizePixels();
  region.setViewportPixels(newOrigin, size);
  this->glaction->setViewportRegion(region);

  region = this->handleeventaction->getViewportRegion();
  size = region.getViewportSizePixels();
  region.setViewportPixels(newOrigin, size);
  this->handleeventaction->setViewportRegion(region);
  this->flags |= FLAG_SENDVP;
}

/*!
  Returns origin of rendering area.
 */
const SbVec2s &
SoSceneManager::getOrigin(void) const
{
  return this->glaction->getViewportRegion().getViewportOriginPixels();
}

/*!
  Change viewport region.
 */
void
SoSceneManager::setViewportRegion(const SbViewportRegion & newregion)
{
  this->glaction->setViewportRegion(newregion);
  this->handleeventaction->setViewportRegion(newregion);
  this->flags |= FLAG_SENDVP;
}

/*!
  Returns current viewport region used by the renderaction and the
  event handling.
 */
const SbViewportRegion &
SoSceneManager::getViewportRegion(void) const
{
  return this->glaction->getViewportRegion();
}

/*!
  Sets color of rendering canvas.
 */
void
SoSceneManager::setBackgroundColor(const SbColor & color)
{
  this->backgroundcolor = color;
}

/*!
  Returns color used for clearing the rendering area before rendering
  the scene.
 */
const SbColor &
SoSceneManager::getBackgroundColor(void) const
{
  return this->backgroundcolor;
}

/*!
  Set index of background color in the color lookup table if rendering
  in colorindex mode.

  Note: colorindex mode is not supported yet in Coin.
 */
void
SoSceneManager::setBackgroundIndex(const int index)
{
  this->backgroundindex = index;
}

/*!
  Returns index of colormap for background filling.

  \sa setBackgroundIndex()
 */
int
SoSceneManager::getBackgroundIndex(void) const
{
  return this->backgroundindex;
}

/*!
  Turn RGB truecolor mode on or off. If you turn truecolor mode off,
  colorindex mode will be used instead.
*/
void
SoSceneManager::setRGBMode(const SbBool flag)
{
  if (flag) {
    this->flags |= FLAG_RGBMODE;
  }
  else {
    this->flags &= ~FLAG_RGBMODE;
  }
}

/*!
  Returns the "truecolor or colorindex" mode flag.
 */
SbBool
SoSceneManager::isRGBMode(void) const
{
  return (this->flags & FLAG_RGBMODE) != 0;
}

/*!
  Activate rendering and event handling. Default is \c on.
 */
void
SoSceneManager::activate(void)
{
  this->flags |= FLAG_ACTIVE;
}

/*!
  Deactive rendering and event handling.
 */
void
SoSceneManager::deactivate(void)
{
  this->flags &= ~FLAG_ACTIVE;
}

/*!
  Set the callback function \a f to invoke when rendering the
  scene. \a userdata will be passed as the first argument of the
  function.
 */
void
SoSceneManager::setRenderCallback(SoSceneManagerRenderCB * f,
                                  void * const userdata)
{
  this->rendercb = f;
  this->rendercbdata = userdata;
}

/*!
  Returns \c TRUE if the SoSceneManager automatically redraws the
  scene upon detecting changes in the scene graph.

  The automatic redraw is turned on and off by setting either a valid
  callback function with setRenderCallback(), or by passing \c NULL.
 */
SbBool
SoSceneManager::isAutoRedraw(void) const
{
  return this->rendercb != NULL;
}

/*!
  Set up the redraw \a priority for the SoOneShotSensor used to
  trigger redraws. By setting this lower than for your own sensors,
  you can make sure some code is always run before redraw happens.

  \sa SoDelayQueueSensor
 */
void
SoSceneManager::setRedrawPriority(const uint32_t priority)
{
  this->redrawpri = priority;

  if (this->redrawshot) this->redrawshot->setPriority(priority);
}

/*!
  Returns value of priority on the redraw sensor.
 */
uint32_t
SoSceneManager::getRedrawPriority(void) const
{
  return this->redrawpri;
}

/*!
  Turn antialiased rendering on or off.

  See documentation for SoGLRenderAction::setSmoothing() and
  SoGLRenderAction::setNumPasses().
 */
void
SoSceneManager::setAntialiasing(const SbBool smoothing, const int numpasses)
{
  this->glaction->setSmoothing(smoothing);
  this->glaction->setNumPasses(numpasses);
}

/*!
  Returns rendering pass information.

  \sa setAntialiasing()
 */
void
SoSceneManager::getAntialiasing(SbBool & smoothing, int & numpasses) const
{
  smoothing = this->glaction->isSmoothing();
  numpasses = this->glaction->getNumPasses();
}

/*!
  Set the \a action to use for rendering. Overrides the default action
  made in the constructor.
 */
void
SoSceneManager::setGLRenderAction(SoGLRenderAction * const action)
{
  if (this->deleteglaction) {
    delete this->glaction;
    this->glaction = NULL;
  }

  // If action change, we need to invalidate state to enable lazy GL
  // elements to be evaluated correctly.
  //
  // Note that the SGI and TGS Inventor implementations doesn't do
  // this -- which smells of a bug.
  if (action && action != this->glaction) action->invalidateState();
  this->glaction = action;
  this->deleteglaction = FALSE;
}

/*!
  Returns pointer to render action.
 */
SoGLRenderAction *
SoSceneManager::getGLRenderAction(void) const
{
  return this->glaction;
}

/*!
  Set the \a action to use for event handling. Overrides the default
  action made in the constructor.
 */
void
SoSceneManager::setHandleEventAction(SoHandleEventAction * hea)
{
  if (this->deletehandleeventaction) delete this->handleeventaction;
  this->handleeventaction = hea;
  this->deletehandleeventaction = FALSE;
}

/*!
  Returns pointer to event handler action.
 */
SoHandleEventAction *
SoSceneManager::getHandleEventAction(void) const
{
  return this->handleeventaction;
}

/*!
  Returns the default priority of the redraw sensor.

  \sa SoDelayQueueSensor, setRedrawPriority()
 */
uint32_t
SoSceneManager::getDefaultRedrawPriority(void)
{
  return 10000;
}

/*!
  Set whether or not for SoSceneManager instances to "touch" the
  global \c realTime field after a redraw. If this is not done,
  redrawing when animating the scene will only happen as fast as the
  \c realTime interval goes (which defaults to 12 times a second).

  \sa SoDB::setRealTimeInterval()
 */
void
SoSceneManager::enableRealTimeUpdate(const SbBool flag)
{
  SoSceneManager::touchtimer = flag;
}

/*!
  Returns whether or not we automatically notifies everything
  connected to the \c realTime field after a redraw.
 */
SbBool
SoSceneManager::isRealTimeUpdateEnabled(void)
{
  return SoSceneManager::touchtimer;
}

#undef FLAG_RGBMODE
#undef FLAG_ACTIVE
#undef FLAG_SENDVP

