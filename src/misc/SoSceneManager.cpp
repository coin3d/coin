/**************************************************************************\
 *
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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
  \class SoSceneManager SoSceneManager.h Inventor/SoSceneManager.h
  \brief The SoSceneManager class is the interface between the scene graph
  and the window system dependent classes.
  \ingroup general

  TODO: more doc
*/

#include <Inventor/SoSceneManager.h>
#include <Inventor/sensors/SoNodeSensor.h>
#include <Inventor/sensors/SoOneShotSensor.h>
#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/misc/SoGL.h>

#ifdef _WIN32
#include <windows.h>
#endif // _WIN32

#include <GL/gl.h>
#include <GL/glu.h> // For gluErrorString - not absolutely necessary.
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
#if !defined(COIN_EXCLUDE_SOHANDLEEVENTACTION)
#include <Inventor/actions/SoHandleEventAction.h>
#endif // !COIN_EXCLUDE_SOHANDLEEVENTACTION
#include <Inventor/nodes/SoNode.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG
#include <assert.h>

/*
  \typedef void SoSceneManagerRenderCB(void * userData, SoSceneManager * mgr)

  FIXME: write doc
*/


/*!
  Constructor.
 */
SoSceneManager::SoSceneManager()
{
#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  this->glAction = new SoGLRenderAction(SbViewportRegion(400,400));
  this->deleteGLAction = TRUE;
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SOHANDLEEVENTACTION)
  this->handleeventaction = new SoHandleEventAction(SbViewportRegion(400,400));
  this->deletehandleeventaction = TRUE;
#endif // !COIN_EXCLUDE_SOHANDLEEVENTACTION

  this->scene = NULL;
  this->rootsensor = NULL;
  this->redrawshot = NULL;

  this->backgroundIndex = 0;
  this->backgroundcolor.setValue(0.0f, 0.0f, 0.0f);
  this->rgbMode = TRUE;
  this->active = FALSE;
  this->redrawPri = SoSceneManager::getDefaultRedrawPriority();

  this->renderCB = NULL;
  this->renderCBdata = NULL;
}

/*!
  Destructor.
 */
SoSceneManager::~SoSceneManager()
{
#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  if (this->deleteGLAction) delete this->glAction;
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
#if !defined(COIN_EXCLUDE_SOHANDLEEVENTACTION)
  if (this->deletehandleeventaction) delete this->handleeventaction;
#endif // !COIN_EXCLUDE_SOHANDLEEVENTACTION

  delete this->rootsensor;
  delete this->redrawshot;
}

/*!
  Render the scene graph.
 */
void
SoSceneManager::render(const SbBool clearWindow,
                       const SbBool clearZbuffer)
{
#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  // FIXME: this probably only needs to be done at the first
  // invocation, since the viewport will later always be
  // up-to-date. (?)  The division of initialization work between
  // SoSceneManager::render() and SoGLRenderAction::beginTraversal()
  // is a mess right now, and need to be sorted out. Should the init
  // code be moved to elements? 19990215 mortene.

  SbViewportRegion vp = this->glAction->getViewportRegion();
  SbVec2s origin = vp.getViewportOriginPixels();
  SbVec2s size = vp.getViewportSizePixels();
  glViewport(origin[0], origin[1], size[0], size[1]);


  GLbitfield mask = 0;
  if (clearWindow) mask |= GL_COLOR_BUFFER_BIT;
  if (clearZbuffer) mask |= GL_DEPTH_BUFFER_BIT;

  glClearColor(this->backgroundcolor[0],
               this->backgroundcolor[1],
               this->backgroundcolor[2],
               0.0f);
  glClear(mask);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
#endif // !COIN_EXCLUDE_SOGLRENDERACTION


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

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  // Apply the GLRenderAction to the scenegraph root.
  this->glAction->apply(this->scene);
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
}

/*!
  Process the given event by applying an SoHandleEventAction on the scene
  graph.
 */
SbBool
SoSceneManager::processEvent(const SoEvent * const event)
{
#if !defined(COIN_EXCLUDE_SOHANDLEEVENTACTION)
  assert(this->handleeventaction);

  this->handleeventaction->setEvent(event);
  // FIXME: other initialization which must be done? 19990214 mortene.

  this->handleeventaction->apply(this->scene);
  return this->handleeventaction->isHandled();
#else // COIN_EXCLUDE_SOHANDLEEVENTACTION
#if COIN_DEBUG
  SoDebugError::postInfo("SoSceneManager::processEvent",
                         "SoHandleEventAction support was not compiled in");
#endif // COIN_DEBUG
  return FALSE;
#endif // COIN_EXCLUDE_SOHANDLEEVENTACTION
}

/*!
  FIXME: write doc
 */
void
SoSceneManager::reinitialize(void)
{
  // FIXME: implement
  assert(FALSE);
}

/*!
  FIXME: write doc
 */
void
SoSceneManager::scheduleRedraw(void)
{
  if (this->active && this->renderCB) {
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
  FIXME: write doc
 */
int
SoSceneManager::isActive(void) const
{
  // FIXME: implement
  assert(0);
  return 0;
}

/*!
  FIXME: write doc
 */
void
SoSceneManager::redraw(void)
{
  if (this->renderCB) this->renderCB(this->renderCBdata, this);
}

void
SoSceneManager::redrawshotTriggeredCB(void * data, SoSensor * /* sensor */)
{
  ((SoSceneManager *)data)->redraw();
}

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
  Set the node which is top of the scene graph we're managing.
  The \a newScene node reference count will be increased by 1, and
  any previously set scene graph top node will have it's reference
  count decreased by 1.

  \sa getSceneGraph()
 */
void
SoSceneManager::setSceneGraph(SoNode * const newScene)
{
  if (this->rootsensor) this->rootsensor->detach();
  if (this->scene) this->scene->unref();

  this->scene = newScene;
  this->scene->ref();

  if (!this->rootsensor)
    this->rootsensor = new SoNodeSensor(SoSceneManager::nodesensorCB, this);

  this->rootsensor->attach(newScene);
}

/*!
  FIXME: write doc
 */
SoNode *
SoSceneManager::getSceneGraph(void) const
{
  return this->scene;
}

/*!
  FIXME: write doc
 */
void
SoSceneManager::setWindowSize(const SbVec2s &newSize)
{
#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  SbViewportRegion region = this->glAction->getViewportRegion();
  region.setWindowSize(newSize[0], newSize[1]);
  this->glAction->setViewportRegion(region);
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SOHANDLEEVENTACTION)
  region = this->handleeventaction->getViewportRegion();
  region.setWindowSize(newSize[0], newSize[1]);
  this->handleeventaction->setViewportRegion(region);
#endif // !COIN_EXCLUDE_SOHANDLEEVENTACTION
}

/*!
  FIXME: write doc
 */
const SbVec2s &
SoSceneManager::getWindowSize(void) const
{
#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  return this->glAction->getViewportRegion().getWindowSize();
#elif !defined(COIN_EXCLUDE_SOHANDLEEVENTACTION)
  return this->handleventaction->getViewportRegion().getWindowSize();
#else // COIN_EXCLUDE_SOGLRENDERACTION && COIN_EXCLUDE_SOHANDLEEVENTACTION
  return SbVec2s(100, 100);
#endif // COIN_EXCLUDE_SOGLRENDERACTION && COIN_EXCLUDE_SOHANDLEEVENTACTION
}

/*!
  FIXME: write doc
 */
void
SoSceneManager::setSize(const SbVec2s &newSize)
{
#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  SbViewportRegion region = this->glAction->getViewportRegion();
  SbVec2s origin = region.getViewportOriginPixels();
  region.setViewportPixels(origin, newSize);
  this->glAction->setViewportRegion(region);
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SOHANDLEEVENTACTION)
  region = this->handleeventaction->getViewportRegion();
  origin = region.getViewportOriginPixels();
  region.setViewportPixels(origin, newSize);
  this->handleeventaction->setViewportRegion(region);
#endif // !COIN_EXCLUDE_SOHANDLEEVENTACTION
}

/*!
  FIXME: write doc
 */
const SbVec2s &
SoSceneManager::getSize(void) const
{
#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  return this->glAction->getViewportRegion().getViewportSizePixels();
#elif !defined(COIN_EXCLUDE_SOHANDLEEVENTACTION)
  return this->handleventaction->getViewportRegion().getViewportSizePixels();
#else // COIN_EXCLUDE_SOGLRENDERACTION && COIN_EXCLUDE_SOHANDLEEVENTACTION
  return SbVec2s(100, 100);
#endif // COIN_EXCLUDE_SOGLRENDERACTION && COIN_EXCLUDE_SOHANDLEEVENTACTION
}

/*!
  FIXME: write doc
 */
void
SoSceneManager::setOrigin(const SbVec2s &newOrigin)
{
#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  SbViewportRegion region = this->glAction->getViewportRegion();
  SbVec2s size = region.getViewportSizePixels();
  region.setViewportPixels(newOrigin, size);
  this->glAction->setViewportRegion(region);
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SOHANDLEEVENTACTION)
  region = this->handleeventaction->getViewportRegion();
  size = region.getViewportSizePixels();
  region.setViewportPixels(newOrigin, size);
  this->handleeventaction->setViewportRegion(region);
#endif // !COIN_EXCLUDE_SOHANDLEEVENTACTION
}

/*!
  FIXME: write doc
 */
const SbVec2s &
SoSceneManager::getOrigin(void) const
{
#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  return this->glAction->getViewportRegion().getViewportOriginPixels();
#elif !defined(COIN_EXCLUDE_SOHANDLEEVENTACTION)
  return this->handleventaction->getViewportRegion().getViewportOriginPixels();
#else // COIN_EXCLUDE_SOGLRENDERACTION && COIN_EXCLUDE_SOHANDLEEVENTACTION
  return SbVec2s(0, 0);
#endif // COIN_EXCLUDE_SOGLRENDERACTION && COIN_EXCLUDE_SOHANDLEEVENTACTION
}

/*!
  FIXME: write doc
 */
void
SoSceneManager::setViewportRegion(const SbViewportRegion & /* newRegion */)
{
  // FIXME: implement
  assert(0);
}

/*!
  FIXME: write doc
 */
const SbViewportRegion &
SoSceneManager::getViewportRegion(void) const
{
  // FIXME: implement
  assert(0);
  static SbViewportRegion vpr;
  return vpr;
}

/*!
  FIXME: write doc
 */
void
SoSceneManager::setBackgroundColor(const SbColor &color)
{
  this->backgroundcolor = color;
}

/*!
  FIXME: write doc
 */
const SbColor &
SoSceneManager::getBackgroundColor(void) const
{
  return this->backgroundcolor;
}

/*!
  FIXME: write doc
 */
void
SoSceneManager::setBackgroundIndex(const int index)
{
  this->backgroundIndex = index;
}

/*!
  FIXME: write doc
 */
int
SoSceneManager::getBackgroundIndex(void) const
{
  return this->backgroundIndex;
}

/*!
  FIXME: write doc
 */
void
SoSceneManager::setRGBMode(const SbBool onOrOff)
{
  assert(onOrOff == TRUE);
  this->rgbMode = onOrOff;
}

/*!
  FIXME: write doc
 */
SbBool
SoSceneManager::isRGBMode(void) const
{
  return this->rgbMode;
}

/*!
  FIXME: write doc
 */
void
SoSceneManager::activate(void)
{
  this->active = TRUE;
}

/*!
  FIXME: write doc
 */
void
SoSceneManager::deactivate(void)
{
  this->active = FALSE;
}

/*!
  FIXME: write doc
 */
void
SoSceneManager::setRenderCallback(SoSceneManagerRenderCB *f,
                                  void * const userData)
{
  this->renderCB = f;
  this->renderCBdata = userData;
}

/*!
  FIXME: write doc
 */
SbBool
SoSceneManager::isAutoRedraw(void) const
{
  return this->renderCB != NULL;
}

/*!
  FIXME: write doc
 */
void
SoSceneManager::setRedrawPriority(const uint32_t priority)
{
  this->redrawPri = priority;

  if (this->redrawshot) this->redrawshot->setPriority(priority);
}

/*!
  FIXME: write doc
 */
uint32_t
SoSceneManager::getRedrawPriority(void) const
{
  return this->redrawPri;
}

/*!
  FIXME: write doc
 */
void
SoSceneManager::setAntialiasing(const SbBool smoothing, const int numPasses)
{
#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  this->glAction->setSmoothing(smoothing);
  this->glAction->setNumPasses(numPasses);
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
}

/*!
  FIXME: write doc
 */
void
SoSceneManager::getAntialiasing(SbBool &smoothing, int &numPasses) const
{
#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  smoothing = this->glAction->isSmoothing();
  numPasses = this->glAction->getNumPasses();
#else // COIN_EXCLUDE_SOGLRENDERACTION
  smoothing = FALSE;
  numPasses = 0;
#endif // COIN_EXCLUDE_SOGLRENDERACTION
}

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write doc
 */
void
SoSceneManager::setGLRenderAction(SoGLRenderAction * const action)
{
  if (this->deleteGLAction) delete this->glAction;
  this->glAction = action;
  this->deleteGLAction = FALSE;
}

/*!
  FIXME: write doc
 */
SoGLRenderAction *
SoSceneManager::getGLRenderAction(void) const
{
  return this->glAction;
}
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SOHANDLEEVENTACTION)
/*!
  FIXME: write doc
 */
void
SoSceneManager::setHandleEventAction(SoHandleEventAction * hea)
{
  if (this->deletehandleeventaction) delete this->handleeventaction;
  this->handleeventaction = hea;
  this->deletehandleeventaction = FALSE;
}

/*!
  FIXME: write doc
 */
SoHandleEventAction *
SoSceneManager::getHandleEventAction(void) const
{
  return this->handleeventaction;
}
#endif // !COIN_EXCLUDE_SOHANDLEEVENTACTION

/*!
  FIXME: write doc
 */
uint32_t
SoSceneManager::getDefaultRedrawPriority(void)
{
  return 10000;
}

/*!
  FIXME: write doc
 */
void
SoSceneManager::enableRealTimeUpdate(const SbBool /* flag */)
{
  // FIXME: implement -- check SoDB::enableRealTimeUpdate()
  assert(0);
}

/*!
  FIXME: write doc
 */
SbBool
SoSceneManager::isRealTimeUpdateEnabled(void)
{
  // FIXME: implement
  assert(0);
  return FALSE;
}
