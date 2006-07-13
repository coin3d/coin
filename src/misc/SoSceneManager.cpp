/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2005 by Systems in Motion.  All rights reserved.
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
 *  See <URL:http://www.coin3d.org/> for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
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

// *************************************************************************

#include <Inventor/SoSceneManager.h>

#include <assert.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/C/tidbits.h>
#include <Inventor/C/tidbitsp.h>
#include <Inventor/SoDB.h>
#include <Inventor/actions/SoAudioRenderAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoHandleEventAction.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/fields/SoSFTime.h>
#include <Inventor/misc/SoAudioDevice.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/sensors/SoNodeSensor.h>
#include <Inventor/sensors/SoOneShotSensor.h>
#include <Inventor/system/gl.h>
#include "AudioTools.h"
#ifdef COIN_THREADSAFE
#include <Inventor/threads/SbMutex.h>
#endif // COIN_THREADSAFE

// *************************************************************************

class SoSceneManagerP {
public:
  enum {
    FLAG_RGBMODE = 0x0001,
    FLAG_ACTIVE = 0x0002
  };

  SbBool isActive(void) const { return this->flags & SoSceneManagerP::FLAG_ACTIVE; }

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

  static SbBool cleanupfunctionset;
  static void cleanup(void);

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
SbBool SoSceneManagerP::cleanupfunctionset = FALSE;

#define PRIVATE(p) (p->pimpl)

// *************************************************************************

// This class inherits SoNodeSensor and overrides its notify() method
// to provide a means of debugging notifications on the root node.
//
// Good for debugging cases when there are continuous redraws due to
// scene graph changes we have no clue as to the source of.
//
// A sensor of this class is only made if the below debugging envvar
// is set. Otherwise, and ordinary SoNodeSensor is used instead.

class SoSceneManagerRootSensor : public SoNodeSensor {
  typedef SoNodeSensor inherited;

public:
  SoSceneManagerRootSensor(SoSensorCB * func, void * data) : inherited(func, data) { }
  virtual ~SoSceneManagerRootSensor() { }

  virtual void notify(SoNotList * l);

  static SbBool debug(void);

private:
  static int debugrootnotifications;
};

int SoSceneManagerRootSensor::debugrootnotifications = -1;

void
SoSceneManagerRootSensor::notify(SoNotList * l)
{
  l->print();
  (void)fprintf(stdout, "end\n");

  inherited::notify(l);
}

SbBool
SoSceneManagerRootSensor::debug(void)
{
  if (SoSceneManagerRootSensor::debugrootnotifications == -1) {
    const char * env = coin_getenv("COIN_DEBUG_ROOT_NOTIFICATIONS");
    SoSceneManagerRootSensor::debugrootnotifications = env && (atoi(env) > 0);
  }
  return SoSceneManagerRootSensor::debugrootnotifications ? TRUE : FALSE;
}

// *************************************************************************

/*!
  \typedef SoSceneManagerRenderCB(void * userdata, SoSceneManager * mgr)

  Render callback function must have this signature.
*/

// *************************************************************************

/*!
  Constructor. Sets up default SoGLRenderAction and
  SoHandleEventAction instances.
 */
SoSceneManager::SoSceneManager(void)
{
  assert(SoDB::isInitialized() && "SoDB::init() has not been invoked");

  PRIVATE(this) = new SoSceneManagerP;

  PRIVATE(this)->glaction = new SoGLRenderAction(SbViewportRegion(400, 400));
  PRIVATE(this)->deleteglaction = TRUE;

  PRIVATE(this)->audiorenderaction = new SoAudioRenderAction();
  PRIVATE(this)->deleteaudiorenderaction = TRUE;

  PRIVATE(this)->handleeventaction =
    new SoHandleEventAction(SbViewportRegion(400, 400));
  PRIVATE(this)->deletehandleeventaction = TRUE;

  PRIVATE(this)->scene = NULL;
  PRIVATE(this)->rootsensor = NULL;
  PRIVATE(this)->redrawshot = NULL;

  PRIVATE(this)->backgroundindex = 0;
  PRIVATE(this)->backgroundcolor.setValue(0.0f, 0.0f, 0.0f);
  // rgbmode by default
  PRIVATE(this)->flags = SoSceneManagerP::FLAG_RGBMODE;
  PRIVATE(this)->redrawpri = SoSceneManager::getDefaultRedrawPriority();

  PRIVATE(this)->rendercb = NULL;
  PRIVATE(this)->rendercbdata = NULL;
}

/*!
  Destructor.
 */
SoSceneManager::~SoSceneManager()
{
  if (PRIVATE(this)->deleteglaction) delete PRIVATE(this)->glaction;
  if (PRIVATE(this)->deleteaudiorenderaction) delete PRIVATE(this)->audiorenderaction;
  if (PRIVATE(this)->deletehandleeventaction) delete PRIVATE(this)->handleeventaction;

  this->setSceneGraph(NULL);
  delete PRIVATE(this)->rootsensor;
  delete PRIVATE(this)->redrawshot;

  delete PRIVATE(this);
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
  // FIXME: according to a user, TGS Inventor seems to disable the
  // redraw SoOneShotSensor while the scene graph is being rendered,
  // which Coin does not do. SGI Inventor probably has the same
  // behavior as TGS Inventor. (Should investigate this.)
  //
  // pederb suggests keeping the current behavior in Coin, even though
  // this may cause trouble for code being ported from SGI / TGS
  // Inventor, as it is convenient to "touch()" a node for triggering
  // continuous animation. Besides, making Coin compatible with SGI
  // (?) / TGS Inventor now may cause problems for existing Coin
  // client code.
  //
  // I'm however not too happy with this fairly large incompatibility.
  // Any usable suggestions for a resolution of this problem would be
  // welcome.
  //
  // 20050809 mortene.

  if (PRIVATE(this)->scene && 
      // Order is important below, because we don't want to call
      // SoAudioDevice::instance() unless we need to -- as it triggers
      // loading the OpenAL library, which should only be loaded on
      // demand.
      coin_sound_should_traverse() &&
      SoAudioDevice::instance()->haveSound() &&
      SoAudioDevice::instance()->isEnabled())
    PRIVATE(this)->audiorenderaction->apply(PRIVATE(this)->scene);

  this->render(PRIVATE(this)->glaction, TRUE, clearwindow, clearzbuffer);
}

/*!
  Render method used for thread safe rendering.

  Since only one thread can use an SoGLRenderAction, this method
  enables you to supply your own thread-specific SoGLRenderAction to
  be used for rendering the scene.

  If \a initmatrices is \c TRUE, the OpenGL model and projection
  matrices will be initialized to identity matrices before applying
  the action.

  If \a clearwindow is \c TRUE, clear the rendering buffer before
  drawing. If \a clearzbuffer is \c TRUE, clear the depth buffer
  values before rendering.

  \COIN_FUNCTION_EXTENSION

  \since Coin 2.0
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

  // this is the default depth function in SGI Inventor
  glDepthFunc(GL_LEQUAL);

  if (mask) {
    if (PRIVATE(this)->flags & SoSceneManagerP::FLAG_RGBMODE) {
      glClearColor(PRIVATE(this)->backgroundcolor[0],
                   PRIVATE(this)->backgroundcolor[1],
                   PRIVATE(this)->backgroundcolor[2],
                   0.0f);
    }
    else {
      glClearIndex((GLfloat) PRIVATE(this)->backgroundindex);
    }
    // Registering a callback is needed since the correct GL viewport
    // is set by SoGLRenderAction before rendering. It might not be
    // correct when we get here.
    // This callback is removed again in the prerendercb function
    action->addPreRenderCallback(PRIVATE(this)->prerendercb, 
                                 (void*) (uintptr_t) mask);
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
  PRIVATE(this)->lock();
  if (PRIVATE(this)->rootsensor && PRIVATE(this)->rootsensor->isScheduled()) {
#if COIN_DEBUG && 0 // debug
    SoDebugError::postInfo("SoSceneManager::render",
                           "rootsensor unschedule");
#endif // debug
    PRIVATE(this)->rootsensor->unschedule();
  }
  PRIVATE(this)->unlock();
  // Apply the SoGLRenderAction to the scenegraph root.
  if (PRIVATE(this)->scene) action->apply(PRIVATE(this)->scene);


  // Automatically re-triggers rendering if any animation stuff is
  // connected to the realTime field.
  if (SoSceneManagerP::touchtimer) {
    // FIXME: it would be more elegant to use a private field class
    // inheriting SoSFTime ("SFRealTime") which could just be
    // touch()'ed, and which would do lazy reading of time-of-day on
    // demand. 20000316 mortene.
    SoField * realtime = SoDB::getGlobalField("realTime");
    if (realtime && (realtime->getTypeId() == SoSFTime::getClassTypeId())) {
      // Note that this should not get in the way of a
      // app-programmer controlled realTime field, as
      // enableRealTimeUpdate(FALSE) should then have been called.
      ((SoSFTime *)realtime)->setValue(SbTime::getTimeOfDay());
    }
  }
}

/*!
  Process the given event by applying an SoHandleEventAction on the
  scene graph.
 */
SbBool
SoSceneManager::processEvent(const SoEvent * const event)
{
  assert(PRIVATE(this)->handleeventaction);

  SbBool handled = FALSE;
  if ( PRIVATE(this)->handleeventaction->getState() != NULL &&
       PRIVATE(this)->handleeventaction->getState()->getDepth() != 0 ) {
    // recursive invocation - action currently in use
#if COIN_DEBUG
    SoDebugError::post("SoSceneManager::processEvent",
                       "Recursive invocation detected. Delay processing event "
                       "until the current event is finished processing.");
#endif // COIN_DEBUG
  } else if ( PRIVATE(this)->scene == NULL ) {
    // nothing
  } else {
    PRIVATE(this)->handleeventaction->setEvent(event);
    PRIVATE(this)->handleeventaction->apply(PRIVATE(this)->scene);
    handled = PRIVATE(this)->handleeventaction->isHandled();
  }
  return handled;
}

/*!
  Reinitialize after parameters affecting the OpenGL context has
  changed.
*/
void
SoSceneManager::reinitialize(void)
{
  PRIVATE(this)->glaction->invalidateState();
}

/*!
  Redraw at first opportunity as system becomes idle.

  Multiple calls to this method before an actual redraw has taken
  place will only result in a single redraw of the scene.
*/
void
SoSceneManager::scheduleRedraw(void)
{
  PRIVATE(this)->lock();
  if (PRIVATE(this)->isActive() && PRIVATE(this)->rendercb) {
    if (!PRIVATE(this)->redrawshot) {
      PRIVATE(this)->redrawshot =
        new SoOneShotSensor(SoSceneManagerP::redrawshotTriggeredCB, this);
      PRIVATE(this)->redrawshot->setPriority(this->getRedrawPriority());
    }

#if COIN_DEBUG && 0 // debug
    SoDebugError::postInfo("SoSceneManager::scheduleRedraw",
                           "scheduling redrawshot (oneshotsensor) %p",
                           PRIVATE(this)->redrawshot);
#endif // debug
    PRIVATE(this)->redrawshot->schedule();
  }
  PRIVATE(this)->unlock();
}

/*!
  Returns the \e active flag.
 */
int
SoSceneManager::isActive(void) const
{
  return PRIVATE(this)->isActive();
}

/*!
  Do an immediate redraw by calling the redraw callback function.
 */
void
SoSceneManager::redraw(void)
{
  if (PRIVATE(this)->rendercb) {
    PRIVATE(this)->rendercb(PRIVATE(this)->rendercbdata, this);
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
  if (PRIVATE(this)->rootsensor) PRIVATE(this)->rootsensor->detach();
  // Don't unref() until after we've set up the new root, in case the
  // old root == the new sceneroot. (Just to be that bit more robust.)
  SoNode * oldroot = PRIVATE(this)->scene;

  PRIVATE(this)->scene = sceneroot;

  if (PRIVATE(this)->scene) {
    PRIVATE(this)->scene->ref();

    if (!PRIVATE(this)->rootsensor) {
      SoNodeSensor * n;
      if (SoSceneManagerRootSensor::debug()) {
        n = new SoSceneManagerRootSensor(SoSceneManagerP::nodesensorCB, this);
      }
      else {
        n = new SoNodeSensor(SoSceneManagerP::nodesensorCB, this);
      }
      PRIVATE(this)->rootsensor = n;
    }

    PRIVATE(this)->rootsensor->attach(sceneroot);
  }

  if (oldroot) oldroot->unref();
}

/*!
  Returns pointer to root of scene graph.
 */
SoNode *
SoSceneManager::getSceneGraph(void) const
{
  return PRIVATE(this)->scene;
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

  SbViewportRegion region = PRIVATE(this)->glaction->getViewportRegion();
  region.setWindowSize(newsize[0], newsize[1]);
  PRIVATE(this)->glaction->setViewportRegion(region);

  region = PRIVATE(this)->handleeventaction->getViewportRegion();
  region.setWindowSize(newsize[0], newsize[1]);
  PRIVATE(this)->handleeventaction->setViewportRegion(region);
}

/*!
  Returns the current render action window size.

  \sa setWindowSize()
*/
const SbVec2s &
SoSceneManager::getWindowSize(void) const
{
  return PRIVATE(this)->glaction->getViewportRegion().getWindowSize();
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

  SbViewportRegion region = PRIVATE(this)->glaction->getViewportRegion();
  SbVec2s origin = region.getViewportOriginPixels();
  region.setViewportPixels(origin, newsize);
  PRIVATE(this)->glaction->setViewportRegion(region);

  region = PRIVATE(this)->handleeventaction->getViewportRegion();
  origin = region.getViewportOriginPixels();
  region.setViewportPixels(origin, newsize);
  PRIVATE(this)->handleeventaction->setViewportRegion(region);
}

/*!
  Returns size of render area.
 */
const SbVec2s &
SoSceneManager::getSize(void) const
{
  return PRIVATE(this)->glaction->getViewportRegion().getViewportSizePixels();
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

  SbViewportRegion region = PRIVATE(this)->glaction->getViewportRegion();
  SbVec2s size = region.getViewportSizePixels();
  region.setViewportPixels(newOrigin, size);
  PRIVATE(this)->glaction->setViewportRegion(region);

  region = PRIVATE(this)->handleeventaction->getViewportRegion();
  size = region.getViewportSizePixels();
  region.setViewportPixels(newOrigin, size);
  PRIVATE(this)->handleeventaction->setViewportRegion(region);
}

/*!
  Returns origin of rendering area viewport.

  \sa setOrigin()
*/
const SbVec2s &
SoSceneManager::getOrigin(void) const
{
  return PRIVATE(this)->glaction->getViewportRegion().getViewportOriginPixels();
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

  PRIVATE(this)->glaction->setViewportRegion(newregion);
  PRIVATE(this)->handleeventaction->setViewportRegion(newregion);
}

/*!
  Returns current viewport region used by the renderaction and the
  event handling.

  \sa setViewportRegion()
*/
const SbViewportRegion &
SoSceneManager::getViewportRegion(void) const
{
  return PRIVATE(this)->glaction->getViewportRegion();
}

/*!
  Sets color of rendering canvas.
 */
void
SoSceneManager::setBackgroundColor(const SbColor & color)
{
  PRIVATE(this)->backgroundcolor = color;
}

/*!
  Returns color used for clearing the rendering area before rendering
  the scene.
 */
const SbColor &
SoSceneManager::getBackgroundColor(void) const
{
  return PRIVATE(this)->backgroundcolor;
}

/*!
  Set index of background color in the color lookup table if rendering
  in colorindex mode.

  Note: colorindex mode is not supported yet in Coin.
 */
void
SoSceneManager::setBackgroundIndex(const int index)
{
  PRIVATE(this)->backgroundindex = index;
}

/*!
  Returns index of colormap for background filling.

  \sa setBackgroundIndex()
 */
int
SoSceneManager::getBackgroundIndex(void) const
{
  return PRIVATE(this)->backgroundindex;
}

/*!
  Turn RGB truecolor mode on or off. If you turn truecolor mode off,
  colorindex mode will be used instead.
*/
void
SoSceneManager::setRGBMode(const SbBool flag)
{
  if (flag) {
    PRIVATE(this)->flags |= SoSceneManagerP::FLAG_RGBMODE;
  }
  else {
    PRIVATE(this)->flags &= ~SoSceneManagerP::FLAG_RGBMODE;
  }
}

/*!
  Returns the "truecolor or colorindex" mode flag.
 */
SbBool
SoSceneManager::isRGBMode(void) const
{
  return (PRIVATE(this)->flags & SoSceneManagerP::FLAG_RGBMODE) != 0;
}

/*!
  Activate rendering and event handling. Default is \c off.
 */
void
SoSceneManager::activate(void)
{
  PRIVATE(this)->flags |= SoSceneManagerP::FLAG_ACTIVE;
}

/*!
  Deactive rendering and event handling.
 */
void
SoSceneManager::deactivate(void)
{
  PRIVATE(this)->flags &= ~SoSceneManagerP::FLAG_ACTIVE;
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
  PRIVATE(this)->rendercb = f;
  PRIVATE(this)->rendercbdata = userdata;
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
  return PRIVATE(this)->rendercb != NULL;
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
  PRIVATE(this)->redrawpri = priority;

  if (PRIVATE(this)->redrawshot) PRIVATE(this)->redrawshot->setPriority(priority);
}

/*!
  Returns value of priority on the redraw sensor.
 */
uint32_t
SoSceneManager::getRedrawPriority(void) const
{
  return PRIVATE(this)->redrawpri;
}

/*!
  Turn antialiased rendering on or off.

  See documentation for SoGLRenderAction::setSmoothing() and
  SoGLRenderAction::setNumPasses().
 */
void
SoSceneManager::setAntialiasing(const SbBool smoothing, const int numpasses)
{
  PRIVATE(this)->glaction->setSmoothing(smoothing);
  PRIVATE(this)->glaction->setNumPasses(numpasses);
}

/*!
  Returns rendering pass information.

  \sa setAntialiasing()
 */
void
SoSceneManager::getAntialiasing(SbBool & smoothing, int & numpasses) const
{
  smoothing = PRIVATE(this)->glaction->isSmoothing();
  numpasses = PRIVATE(this)->glaction->getNumPasses();
}

/*!
  Set the \a action to use for rendering. Overrides the default action
  made in the constructor.
 */
void
SoSceneManager::setGLRenderAction(SoGLRenderAction * const action)
{
  if (PRIVATE(this)->deleteglaction) {
    delete PRIVATE(this)->glaction;
    PRIVATE(this)->glaction = NULL;
  }

  // If action change, we need to invalidate state to enable lazy GL
  // elements to be evaluated correctly.
  //
  // Note that the SGI and TGS Inventor implementations doesn't do
  // this -- which smells of a bug.
  if (action && action != PRIVATE(this)->glaction) action->invalidateState();
  PRIVATE(this)->glaction = action;
  PRIVATE(this)->deleteglaction = FALSE;
}

/*!
  Returns pointer to render action.
 */
SoGLRenderAction *
SoSceneManager::getGLRenderAction(void) const
{
  return PRIVATE(this)->glaction;
}

/*!
  Set the \a action to use for rendering audio. Overrides the default action
  made in the constructor.
 */
void
SoSceneManager::setAudioRenderAction(SoAudioRenderAction * const action)
{
  if (PRIVATE(this)->deleteaudiorenderaction) {
    delete PRIVATE(this)->audiorenderaction;
    PRIVATE(this)->audiorenderaction = NULL;
  }

  // If action change, we need to invalidate state to enable lazy GL
  // elements to be evaluated correctly.
  //
  if (action && action != PRIVATE(this)->audiorenderaction) action->invalidateState();
  PRIVATE(this)->audiorenderaction = action;
  PRIVATE(this)->deleteaudiorenderaction = FALSE;
}

/*!
  Returns pointer to audio render action.
 */
SoAudioRenderAction *
SoSceneManager::getAudioRenderAction(void) const
{
  return PRIVATE(this)->audiorenderaction;
}

/*!
  Set the \a action to use for event handling. Overrides the default
  action made in the constructor.
 */
void
SoSceneManager::setHandleEventAction(SoHandleEventAction * hea)
{
  if (PRIVATE(this)->deletehandleeventaction) delete PRIVATE(this)->handleeventaction;
  PRIVATE(this)->handleeventaction = hea;
  PRIVATE(this)->deletehandleeventaction = FALSE;
}

/*!
  Returns pointer to event handler action.
 */
SoHandleEventAction *
SoSceneManager::getHandleEventAction(void) const
{
  return PRIVATE(this)->handleeventaction;
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
  if (!SoSceneManagerP::cleanupfunctionset) {
    coin_atexit((coin_atexit_f*) SoSceneManagerP::cleanup, CC_ATEXIT_NORMAL);
    SoSceneManagerP::cleanupfunctionset = TRUE;
  }
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

// *************************************************************************

// Internal callback.
void
SoSceneManagerP::redrawshotTriggeredCB(void * data, SoSensor * /* sensor */)
{
#if COIN_DEBUG && 0 // debug
  SoDebugError::postInfo("SoSceneManager::redrawshotTriggeredCB", "start");
#endif // debug

  SoSceneManager * sm = (SoSceneManager *)data;

  // Need to recheck the "active" flag, as it could have changed since
  // it was tested in the SoSceneManager::scheduleRedraw() call.
  if (PRIVATE(sm)->isActive()) { sm->redraw(); }

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
  // MSVC7 on 64-bit Window wants it to go through this cast.
  const uintptr_t bitfield = (uintptr_t)userdata;
  GLbitfield mask = (GLbitfield)bitfield;

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

void
SoSceneManagerP::cleanup(void)
{
  SoSceneManagerP::touchtimer = TRUE;
  SoSceneManagerP::cleanupfunctionset = FALSE;
}
#undef PRIVATE
