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
#include <Inventor/actions/SoAudioRenderAction.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/fields/SoSFTime.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/sensors/SoNodeSensor.h>
#include <Inventor/sensors/SoOneShotSensor.h>
#include <Inventor/misc/SoAudioDevice.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H
#include <Inventor/system/gl.h>
#include <assert.h>

#ifdef COIN_THREADSAFE
#include <Inventor/threads/SbMutex.h>
#endif // COIN_THREADSAFE

#ifndef DOXYGEN_SKIP_THIS

// defines for the flags member
#define FLAG_RGBMODE 0x0001
#define FLAG_ACTIVE  0x0002

class SoSceneManagerP {
public:
  static void nodesensorCB(void * data, SoSensor * sensor);
  static void redrawshotTriggeredCB(void * data, SoSensor * sensor);

  SoSceneManagerRenderCB * rendercb;
  void * rendercbdata;

  SoGLRenderAction * glaction;
  SbBool deleteglaction;
  SoAudioRenderAction *audiorenderaction;
  SbBool deleteaudiorenderaction;
  SoHandleEventAction * handleeventaction;
  SbBool deletehandleeventaction;

  SoNode * scene;
  SoNodeSensor * rootsensor;
  SoOneShotSensor * redrawshot;

  SbColor backgroundcolor;
  int backgroundindex;
  uint32_t flags;
  static SbBool touchtimer;

  uint32_t redrawpri;
  static void prerendercb(void * userdata, SoGLRenderAction * action);

#ifdef COIN_THREADSAFE
  SbMutex mutex;
#endif // COIN_THREADSAFE
  void lock(void) {
#ifdef COIN_THREADSAFE
    this->mutex.lock();
#endif // COIN_THREADSAFE
  }
  void unlock(void) {
#ifdef COIN_THREADSAFE
    this->mutex.unlock();
#endif // COIN_THREADSAFE
  }
};

SbBool SoSceneManagerP::touchtimer = TRUE;

#endif // DOXYGEN_SKIP_THIS

/*!
  \typedef SoSceneManagerRenderCB(void * userdata, SoSceneManager * mgr)

  Render callback function must have this signature.
*/

#undef THIS
#define THIS this->pimpl

/*!
  Constructor. Sets up default SoGLRenderAction and
  SoHandleEventAction instances.
 */
SoSceneManager::SoSceneManager(void)
{
  THIS = new SoSceneManagerP;

  THIS->glaction = new SoGLRenderAction(SbViewportRegion(400, 400));
  THIS->deleteglaction = TRUE;

  THIS->audiorenderaction = new SoAudioRenderAction();
  THIS->deleteaudiorenderaction = TRUE;

  THIS->handleeventaction =
    new SoHandleEventAction(SbViewportRegion(400, 400));
  THIS->deletehandleeventaction = TRUE;

  THIS->scene = NULL;
  THIS->rootsensor = NULL;
  THIS->redrawshot = NULL;

  THIS->backgroundindex = 0;
  THIS->backgroundcolor.setValue(0.0f, 0.0f, 0.0f);
  // rgbmode by default
  THIS->flags = FLAG_RGBMODE;
  THIS->redrawpri = SoSceneManager::getDefaultRedrawPriority();

  THIS->rendercb = NULL;
  THIS->rendercbdata = NULL;
}

/*!
  Destructor.
 */
SoSceneManager::~SoSceneManager()
{
  if (THIS->deleteglaction) delete THIS->glaction;
  if (THIS->deleteaudiorenderaction) delete THIS->audiorenderaction;
  if (THIS->deletehandleeventaction) delete THIS->handleeventaction;

  this->setSceneGraph(NULL);
  delete THIS->rootsensor;
  delete THIS->redrawshot;

  delete THIS;
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
#ifdef HAVE_SOUND
  if ( THIS->scene && SoAudioDevice::instance()->haveSound() &&
       SoAudioDevice::instance()->isEnabled())
    THIS->audiorenderaction->apply(THIS->scene);
#endif

  this->render(THIS->glaction, TRUE, clearwindow, clearzbuffer);
}

/*!
  Render method needed for thread safe rendering. Since only one
  thread can use an SoGLRenderAction, this method enables you to
  supply your own thread-specific SoGLRenderAction to be used for
  rendering the scene. If \a initmatrices is \c TRUE, the OpenGL model
  and projection matrices will be initialize to identity before
  applying the action. If \a clearwindow is \c TRUE, clear the
  rendering buffer before drawing. If \a clearzbuffer is \c TRUE,
  clear the depth buffer values before rendering

  \since 2002-10-19
 */
void
SoSceneManager::render(SoGLRenderAction * action,
                       const SbBool initmatrices,
                       const SbBool clearwindow,
                       const SbBool clearzbuffer)
{
  GLbitfield mask = 0;
  if (clearwindow) mask |= GL_COLOR_BUFFER_BIT;
  if (clearzbuffer) mask |= GL_DEPTH_BUFFER_BIT;

  if (mask) {
    if (THIS->flags & FLAG_RGBMODE) {
      glClearColor(THIS->backgroundcolor[0],
                   THIS->backgroundcolor[1],
                   THIS->backgroundcolor[2],
                   0.0f);
    }
    else {
      glClearIndex(THIS->backgroundindex);
    }
    // Registering a callback is needed since the correct GL viewport
    // is set by SoGLRenderAction before rendering.  It might not be
    // correct when we get here.
    // This callback is removed again in the prerendercb function
    action->addPreRenderCallback(THIS->prerendercb, (void*) mask);
  }

  if (initmatrices) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
  }

  // If there has been changes in the scene graph leading to a node
  // sensor detect and schedule before we've gotten around to serving
  // the current redraw -- remove it. This will prevent infinite loops
  // in the case of scenegraph modifications between a nodesensor
  // trigger and SoSceneManager::render() actually being called. It
  // will also help us avoid "double redraws" at expose events.
  THIS->lock();
  if (THIS->rootsensor && THIS->rootsensor->isScheduled()) {
#if COIN_DEBUG && 0 // debug
    SoDebugError::postInfo("SoSceneManager::render",
                           "rootsensor unschedule");
#endif // debug
    THIS->rootsensor->unschedule();
  }
  THIS->unlock();
  // Apply the SoGLRenderAction to the scenegraph root.
  if (THIS->scene) action->apply(THIS->scene);
}

/*!
  Process the given event by applying an SoHandleEventAction on the
  scene graph.
 */
SbBool
SoSceneManager::processEvent(const SoEvent * const event)
{
  assert(THIS->handleeventaction);

  THIS->handleeventaction->setEvent(event);
  if (THIS->scene) THIS->handleeventaction->apply(THIS->scene);
  return THIS->handleeventaction->isHandled();
}

/*!
  Reinitialize after parameters affecting the OpenGL context has
  changed.
*/
void
SoSceneManager::reinitialize(void)
{
  THIS->glaction->invalidateState();
}

/*!
  Redraw at first opportunity as system becomes idle.

  Multiple calls to this method before an actual redraw has taken
  place will only result in a single redraw of the scene.
*/
void
SoSceneManager::scheduleRedraw(void)
{
  THIS->lock();
  if ((THIS->flags & FLAG_ACTIVE) && THIS->rendercb) {
    if (!THIS->redrawshot) {
      THIS->redrawshot =
        new SoOneShotSensor(SoSceneManagerP::redrawshotTriggeredCB, this);
      THIS->redrawshot->setPriority(this->getRedrawPriority());
    }

#if COIN_DEBUG && 0 // debug
    SoDebugError::postInfo("SoSceneManager::scheduleRedraw",
                           "scheduling redrawshot (oneshotsensor) %p",
                           THIS->redrawshot);
#endif // debug
    THIS->redrawshot->schedule();
  }
  THIS->unlock();
}

/*!
  Returns the \e active flag.
 */
int
SoSceneManager::isActive(void) const
{
  return (THIS->flags & FLAG_ACTIVE) != 0;
}

/*!
  Do an immediate redraw by calling the redraw callback function.
 */
void
SoSceneManager::redraw(void)
{
  if (THIS->rendercb) {
    THIS->rendercb(THIS->rendercbdata, this);

    // Automatically re-triggers rendering if any animation stuff is
    // connected to the realTime field.
    if (SoSceneManagerP::touchtimer) {
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
  if (THIS->rootsensor) THIS->rootsensor->detach();
  // Don't unref() until after we've set up the new root, in case the
  // old root == the new sceneroot. (Just to be that bit more robust.)
  SoNode * oldroot = THIS->scene;

  THIS->scene = sceneroot;

  if (THIS->scene) {
    THIS->scene->ref();

    if (!THIS->rootsensor)
      THIS->rootsensor = new SoNodeSensor(SoSceneManagerP::nodesensorCB, this);

    THIS->rootsensor->attach(sceneroot);
  }

  if (oldroot) oldroot->unref();
}

/*!
  Returns pointer to root of scene graph.
 */
SoNode *
SoSceneManager::getSceneGraph(void) const
{
  return THIS->scene;
}

/*!
  Update window size of our SoGLRenderAction's viewport settings.

  Note that this will \e only change the information about window
  dimensions, the actual viewport size and origin (ie the rectangle
  which redraws are confined to) will stay the same.

  \sa setViewportRegion()
*/
void
SoSceneManager::setWindowSize(const SbVec2s & newsize)
{
#if COIN_DEBUG && 0 // debug
  SoDebugError::postInfo("SoSceneManager::setWindowSize",
                         "(%d, %d)", newsize[0], newsize[1]);
#endif // debug

  SbViewportRegion region = THIS->glaction->getViewportRegion();
  region.setWindowSize(newsize[0], newsize[1]);
  THIS->glaction->setViewportRegion(region);

  region = THIS->handleeventaction->getViewportRegion();
  region.setWindowSize(newsize[0], newsize[1]);
  THIS->handleeventaction->setViewportRegion(region);
}

/*!
  Returns the current render action window size.

  \sa setWindowSize()
*/
const SbVec2s &
SoSceneManager::getWindowSize(void) const
{
  return THIS->glaction->getViewportRegion().getWindowSize();
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

  SbViewportRegion region = THIS->glaction->getViewportRegion();
  SbVec2s origin = region.getViewportOriginPixels();
  region.setViewportPixels(origin, newsize);
  THIS->glaction->setViewportRegion(region);

  region = THIS->handleeventaction->getViewportRegion();
  origin = region.getViewportOriginPixels();
  region.setViewportPixels(origin, newsize);
  THIS->handleeventaction->setViewportRegion(region);
}

/*!
  Returns size of render area.
 */
const SbVec2s &
SoSceneManager::getSize(void) const
{
  return THIS->glaction->getViewportRegion().getViewportSizePixels();
}

/*!
  Set \e only the origin of the viewport region within the rendering
  window.

  \sa setViewportRegion(), setWindowSize()
*/
void
SoSceneManager::setOrigin(const SbVec2s & newOrigin)
{
#if COIN_DEBUG && 0 // debug
  SoDebugError::postInfo("SoSceneManager::setOrigin",
                         "(%d, %d)", newOrigin[0], newOrigin[1]);
#endif // debug

  SbViewportRegion region = THIS->glaction->getViewportRegion();
  SbVec2s size = region.getViewportSizePixels();
  region.setViewportPixels(newOrigin, size);
  THIS->glaction->setViewportRegion(region);

  region = THIS->handleeventaction->getViewportRegion();
  size = region.getViewportSizePixels();
  region.setViewportPixels(newOrigin, size);
  THIS->handleeventaction->setViewportRegion(region);
}

/*!
  Returns origin of rendering area viewport.

  \sa setOrigin()
*/
const SbVec2s &
SoSceneManager::getOrigin(void) const
{
  return THIS->glaction->getViewportRegion().getViewportOriginPixels();
}

/*!
  Update our SoGLRenderAction's viewport settings.

  This will change \e both the information about window dimensions and
  the actual viewport size and origin.

  \sa setWindowSize()
*/
void
SoSceneManager::setViewportRegion(const SbViewportRegion & newregion)
{
#if COIN_DEBUG && 0 // debug
  const SbVec2s & ws = newregion.getWindowSize();
  const SbVec2s & vpop = newregion.getViewportOriginPixels();
  const SbVec2s & vpsp = newregion.getViewportSizePixels();
  SoDebugError::postInfo("SoSceneManager::setViewportRegion",
                         "windowsize=(%d, %d) "
                         "viewportorigin=(%d, %d) "
                         "viewportsize=(%d, %d) ",
                         ws[0], ws[1],
                         vpop[0], vpop[1],
                         vpsp[0], vpsp[1]);
#endif // debug

  THIS->glaction->setViewportRegion(newregion);
  THIS->handleeventaction->setViewportRegion(newregion);
}

/*!
  Returns current viewport region used by the renderaction and the
  event handling.

  \sa setViewportRegion()
*/
const SbViewportRegion &
SoSceneManager::getViewportRegion(void) const
{
  return THIS->glaction->getViewportRegion();
}

/*!
  Sets color of rendering canvas.
 */
void
SoSceneManager::setBackgroundColor(const SbColor & color)
{
  THIS->backgroundcolor = color;
}

/*!
  Returns color used for clearing the rendering area before rendering
  the scene.
 */
const SbColor &
SoSceneManager::getBackgroundColor(void) const
{
  return THIS->backgroundcolor;
}

/*!
  Set index of background color in the color lookup table if rendering
  in colorindex mode.

  Note: colorindex mode is not supported yet in Coin.
 */
void
SoSceneManager::setBackgroundIndex(const int index)
{
  THIS->backgroundindex = index;
}

/*!
  Returns index of colormap for background filling.

  \sa setBackgroundIndex()
 */
int
SoSceneManager::getBackgroundIndex(void) const
{
  return THIS->backgroundindex;
}

/*!
  Turn RGB truecolor mode on or off. If you turn truecolor mode off,
  colorindex mode will be used instead.
*/
void
SoSceneManager::setRGBMode(const SbBool flag)
{
  if (flag) {
    THIS->flags |= FLAG_RGBMODE;
  }
  else {
    THIS->flags &= ~FLAG_RGBMODE;
  }
}

/*!
  Returns the "truecolor or colorindex" mode flag.
 */
SbBool
SoSceneManager::isRGBMode(void) const
{
  return (THIS->flags & FLAG_RGBMODE) != 0;
}

/*!
  Activate rendering and event handling. Default is \c on.
 */
void
SoSceneManager::activate(void)
{
  THIS->flags |= FLAG_ACTIVE;
}

/*!
  Deactive rendering and event handling.
 */
void
SoSceneManager::deactivate(void)
{
  THIS->flags &= ~FLAG_ACTIVE;
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
  THIS->rendercb = f;
  THIS->rendercbdata = userdata;
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
  return THIS->rendercb != NULL;
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
  THIS->redrawpri = priority;

  if (THIS->redrawshot) THIS->redrawshot->setPriority(priority);
}

/*!
  Returns value of priority on the redraw sensor.
 */
uint32_t
SoSceneManager::getRedrawPriority(void) const
{
  return THIS->redrawpri;
}

/*!
  Turn antialiased rendering on or off.

  See documentation for SoGLRenderAction::setSmoothing() and
  SoGLRenderAction::setNumPasses().
 */
void
SoSceneManager::setAntialiasing(const SbBool smoothing, const int numpasses)
{
  THIS->glaction->setSmoothing(smoothing);
  THIS->glaction->setNumPasses(numpasses);
}

/*!
  Returns rendering pass information.

  \sa setAntialiasing()
 */
void
SoSceneManager::getAntialiasing(SbBool & smoothing, int & numpasses) const
{
  smoothing = THIS->glaction->isSmoothing();
  numpasses = THIS->glaction->getNumPasses();
}

/*!
  Set the \a action to use for rendering. Overrides the default action
  made in the constructor.
 */
void
SoSceneManager::setGLRenderAction(SoGLRenderAction * const action)
{
  if (THIS->deleteglaction) {
    delete THIS->glaction;
    THIS->glaction = NULL;
  }

  // If action change, we need to invalidate state to enable lazy GL
  // elements to be evaluated correctly.
  //
  // Note that the SGI and TGS Inventor implementations doesn't do
  // this -- which smells of a bug.
  if (action && action != THIS->glaction) action->invalidateState();
  THIS->glaction = action;
  THIS->deleteglaction = FALSE;
}

/*!
  Returns pointer to render action.
 */
SoGLRenderAction *
SoSceneManager::getGLRenderAction(void) const
{
  return THIS->glaction;
}

/*!
  Set the \a action to use for rendering audio. Overrides the default action
  made in the constructor.
 */
void
SoSceneManager::setAudioRenderAction(SoAudioRenderAction * const action)
{
  if (THIS->deleteaudiorenderaction) {
    delete THIS->audiorenderaction;
    THIS->audiorenderaction = NULL;
  }

  // If action change, we need to invalidate state to enable lazy GL
  // elements to be evaluated correctly.
  //
  if (action && action != THIS->audiorenderaction) action->invalidateState();
  THIS->audiorenderaction = action;
  THIS->deleteaudiorenderaction = FALSE;
}

/*!
  Returns pointer to audio render action.
 */
SoAudioRenderAction *
SoSceneManager::getAudioRenderAction(void) const
{
  return THIS->audiorenderaction;
}

/*!
  Set the \a action to use for event handling. Overrides the default
  action made in the constructor.
 */
void
SoSceneManager::setHandleEventAction(SoHandleEventAction * hea)
{
  if (THIS->deletehandleeventaction) delete THIS->handleeventaction;
  THIS->handleeventaction = hea;
  THIS->deletehandleeventaction = FALSE;
}

/*!
  Returns pointer to event handler action.
 */
SoHandleEventAction *
SoSceneManager::getHandleEventAction(void) const
{
  return THIS->handleeventaction;
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
  SoSceneManagerP::touchtimer = flag;
}

/*!
  Returns whether or not we automatically notifies everything
  connected to the \c realTime field after a redraw.
 */
SbBool
SoSceneManager::isRealTimeUpdateEnabled(void)
{
  return SoSceneManagerP::touchtimer;
}

#undef THIS
#undef FLAG_RGBMODE
#undef FLAG_ACTIVE

#ifndef DOXYGEN_SKIP_THIS

// Internal callback.
void
SoSceneManagerP::redrawshotTriggeredCB(void * data, SoSensor * /* sensor */)
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
SoSceneManagerP::nodesensorCB(void * data, SoSensor * /* sensor */)
{
#if COIN_DEBUG && 0 // debug
  SoDebugError::postInfo("SoSceneManager::nodesensorCB",
                         "detected change in scene graph");
#endif // debug
  ((SoSceneManager *)data)->scheduleRedraw();
}

void
SoSceneManagerP::prerendercb(void * userdata, SoGLRenderAction * action)
{
  // remove callback again
  action->removePreRenderCallback(prerendercb, userdata);
  GLbitfield mask = (GLbitfield) userdata;

#if COIN_DEBUG && 0 // debug
  GLint view[4];
  glGetIntegerv(GL_VIEWPORT, view);
  SoDebugError::postInfo("SoSceneManagerP::prerendercb",
                         "GL_VIEWPORT=<%d, %d, %d, %d>",
                         view[0], view[1], view[2], view[3]);
#endif // debug
  
  // clear the viewport
  glClear(mask);
}

#endif // DOXYGEN_SKIP_THIS
