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
  \class SoExtSelection SoExtSelection.h Inventor/nodes/SoExtSelection.h
  \brief The SoExtSelection class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoExtSelection.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <coindefs.h> // COIN_STUB()

#include <Inventor/events/SoEvent.h>
#include <Inventor/actions/SoHandleEventAction.h>
#include <Inventor/events/SoMouseButtonEvent.h>
#include <Inventor/events/SoKeyboardEvent.h>
#include <Inventor/events/SoLocation2Event.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/SbTime.h>

#include <Inventor/sensors/SoTimerSensor.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/nodes/SoShape.h>

#include <Inventor/SbBox3f.h>
#include <Inventor/SbBox2s.h>
#include <Inventor/SbVec3f.h>
#include <Inventor/SbVec2s.h>
#include <Inventor/SbViewVolume.h>
#include <Inventor/SbMatrix.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/elements/SoViewportRegionElement.h>
#include <Inventor/elements/SoProjectionMatrixElement.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/elements/SoViewingMatrixElement.h>
#include <Inventor/lists/SoCallbackList.h>

#include <Inventor/SbMatrix.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_WINDOWS_H
#include <windows.h>
#endif

#include <GL/gl.h>
#include <float.h>


/*!
  \enum SoExtSelection::LassoType
  FIXME: write documentation for enum
*/
/*!
  \var SoExtSelection::LassoType SoExtSelection::NOLASSO
  FIXME: write documentation for enum definition
*/
/*!
  \var SoExtSelection::LassoType SoExtSelection::LASSO
  FIXME: write documentation for enum definition
*/
/*!
  \var SoExtSelection::LassoType SoExtSelection::RECTANGLE
  FIXME: write documentation for enum definition
*/

/*!
  \enum SoExtSelection::LassoPolicy
  FIXME: write documentation for enum
*/
/*!
  \var SoExtSelection::LassoPolicy SoExtSelection::FULL_BBOX
  FIXME: write documentation for enum definition
*/
/*!
  \var SoExtSelection::LassoPolicy SoExtSelection::PART_BBOX
  FIXME: write documentation for enum definition
*/
/*!
  \var SoExtSelection::LassoPolicy SoExtSelection::FULL
  FIXME: write documentation for enum definition
*/
/*!
  \var SoExtSelection::LassoPolicy SoExtSelection::PART
q  FIXME: write documentation for enum definition
*/


/*!
  \var SoSFEnum SoExtSelection::lassoType
  FIXME: write documentation for field
*/
/*!
  \var SoSFEnum SoExtSelection::lassoPolicy
  FIXME: write documentation for field
*/

// *************************************************************************


class SoExtSelectionP {
public:
  SoExtSelectionP(SoExtSelection * master) {
    this->master = master;
  }

  SbColor lassocolor;
  float lassowidth;
  SbBool lassopatternanimate;
  unsigned short lassopattern;

  enum SelectionState {
    NONE,
    RECTANGLE,
    LASSO
  };

  SelectionState selectionstate;
  SbBool isDragging;  // 0=no, 1=currently dragging a new point (mouse = last pos)

  SbList <SbVec2s> coords;
  SoTimerSensor * timersensor;
  SoCallbackAction * cbaction;

  const SbList <SbVec2s> & getCoords(void) const {
    return coords;
  }

  SoExtSelection * master;
  SbViewportRegion curvp;

  static void timercallback(void *data, SoSensor *sensor);
  static SoCallbackAction::Response myCallback(void *data,
                                               SoCallbackAction *action,
                                               const SoNode *node);

  SoCallbackAction::Response testShape(SoCallbackAction * action, const SoShape * shape);

  SoCallbackAction::Response testBBox(SoCallbackAction * action,
                                      const SbMatrix & projmatrix,
                                      const SoShape * shape,
                                      const SbBox2s & lassorect,
                                      const SbBool full);

  SoCallbackAction::Response testPrimitives(SoCallbackAction * action,
                                            const SbMatrix & projmatrix,
                                            const SoShape * shape,
                                            const SbBox2s & lassorect,
                                            const SbBool full);

};

void
SoExtSelectionP::timercallback(void *data, SoSensor *sensor)
{
  SoExtSelection *ext = (SoExtSelection *)data;
  if (ext == NULL) return;
  if (ext->isOverlayLassoAnimated()) {
    int pat = ext->getOverlayLassoPattern();
    int pat2 = pat << 1;
    if ((pat & 0x8000) != 0) pat2 |= 1;
    ext->setOverlayLassoPattern(pat2 & 0xffff);
    ext->touch();
  }
}

// ----------

    // The following code is by Randolph Franklin,
    // it returns 1 for interior points and 0 for exterior points.
    // http://astronomy.swin.edu.au/pbourke/geometry/insidepoly/
    /*
      int
      pnpoly(int npol, float *xp, float *yp, float x, float y)
      {
        int i, j, c = 0;
        for (i = 0, j = npol-1; i < npol; j = i++) {
          if ((((yp[i] <= y) && (y < yp[j])) ||
            ((yp[j] <= y) && (y < yp[i]))) &&
            (x < (xp[j] - xp[i]) * (y - yp[i]) / (yp[j] - yp[i]) + xp[i]))
          c = !c;
        }
        return c;
      }
    */

static SbBool
pointinpoly(const SbList <SbVec2s> & coords, const SbVec2s & point)
{
  int i, j;
  SbBool c = FALSE;
  int npol = coords.getLength();
  float x = (float) point[0];
  float y = (float) point[1];
  SbVec2f pi, pj;

  for (i = 0, j = npol-1; i < npol; j = i++) {

    pi[0] = (float) coords[i][0];
    pi[1] = (float) coords[i][1];
    pj[0] = (float) coords[j][0];
    pj[1] = (float) coords[j][1];
    
    if ((((pi[1] <= y) && (y < pj[1])) ||
	 ((pj[1] <= y) && (y < pi[1]))) &&
	(x < (pj[0] - pi[0]) * (y - pi[1]) / (pj[1] - pi[1]) + pi[0]))
      c = !c;
  }
  return c;
}

// ----------

SoCallbackAction::Response
SoExtSelectionP::myCallback(void *data, SoCallbackAction *action, const SoNode *node)
{
  SoExtSelection * ext = (SoExtSelection*)data;
  assert(node->isOfType(SoShape::getClassTypeId()));
  return ext->pimpl->testShape(action, (const SoShape*) node);
}

#undef THIS
#define THIS this->pimpl

SO_NODE_SOURCE(SoExtSelection);

/*!
  Constructor.
*/
SoExtSelection::SoExtSelection(void)
{
  THIS = new SoExtSelectionP(this);

  SO_NODE_INTERNAL_CONSTRUCTOR(SoExtSelection);

  SO_NODE_ADD_FIELD(lassoType, (SoExtSelection::NOLASSO));
  SO_NODE_ADD_FIELD(lassoPolicy, (SoExtSelection::FULL_BBOX));

  SO_NODE_DEFINE_ENUM_VALUE(LassoType, NOLASSO);
  SO_NODE_DEFINE_ENUM_VALUE(LassoType, LASSO);
  SO_NODE_DEFINE_ENUM_VALUE(LassoType, RECTANGLE);
  SO_NODE_SET_SF_ENUM_TYPE(lassoType, LassoType);

  SO_NODE_DEFINE_ENUM_VALUE(LassoPolicy, FULL_BBOX);
  SO_NODE_DEFINE_ENUM_VALUE(LassoPolicy, PART_BBOX);
  SO_NODE_DEFINE_ENUM_VALUE(LassoPolicy, FULL);
  SO_NODE_DEFINE_ENUM_VALUE(LassoPolicy, PART);
  SO_NODE_SET_SF_ENUM_TYPE(lassoPolicy, LassoPolicy);

  // setup timer
  THIS->timersensor = new SoTimerSensor(&SoExtSelectionP::timercallback,
                                        (void *)this);
  THIS->timersensor->setBaseTime(SbTime(0.0));
  THIS->timersensor->setInterval(SbTime(0.3));

  THIS->cbaction = new SoCallbackAction();
  THIS->cbaction->addPreCallback(SoShape::getClassTypeId(), SoExtSelectionP::myCallback,
                                 (void *) this);

  // some init (just to be sure?)
  THIS->lassocolor = SbColor(1.0f, 1.0f, 1.0f);
  THIS->lassowidth = 1.0f;
  THIS->lassopatternanimate = TRUE;
  THIS->lassopattern = 0xf0f0;

  THIS->selectionstate = SoExtSelectionP::NONE;
  THIS->isDragging = FALSE;
  THIS->coords.truncate(0);
}

/*!
  Destructor.
*/
SoExtSelection::~SoExtSelection()
{
  if (THIS->timersensor->isScheduled()) THIS->timersensor->unschedule();
  delete THIS->timersensor;
  delete THIS->cbaction;
  delete THIS;
}

/*!
  Does initialization common for all objects of the
  SoExtSelection class. This includes setting up the
  type system, among other things.
*/
void
SoExtSelection::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoExtSelection);
}


/*!
  FIXME: write doc
 */
void
SoExtSelection::useOverlay(const SbBool /* overlay */)
{
  COIN_STUB();
}

/*!
  FIXME: write doc
 */
SbBool
SoExtSelection::isUsingOverlay(void)
{
  COIN_STUB();
  return FALSE;
}

/*!
  FIXME: write doc
 */
SoSeparator *
SoExtSelection::getOverlaySceneGraph(void)
{
  COIN_STUB();
  return NULL;
}

/*!
  FIXME: write doc
 */
void
SoExtSelection::setOverlayLassoColorIndex(const int /* index */)
{
  COIN_STUB();
}

/*!
  FIXME: write doc
 */
int
SoExtSelection::getOverlayLassoColorIndex(void)
{
  COIN_STUB();
  return 0;
}

/*!
  FIXME: write doc
 */
void
SoExtSelection::setLassoColor(const SbColor & color)
{
  THIS->lassocolor = color;
}

/*!
  FIXME: write doc
 */
const SbColor &
SoExtSelection::getLassoColor(void)
{
  return THIS->lassocolor;
}

/*!
  FIXME: write doc
 */
void
SoExtSelection::setLassoWidth(const float width)
{
  THIS->lassowidth = width;
}

/*!
  FIXME: write doc
 */
float
SoExtSelection::getLassoWidth(void)
{
  return THIS->lassowidth;
}

/*!
  FIXME: write doc
 */
void
SoExtSelection::setOverlayLassoPattern(const unsigned short pattern)
{
  THIS->lassopattern = pattern;
}

/*!
  FIXME: write doc
 */
unsigned short
SoExtSelection::getOverlayLassoPattern(void)
{
  return THIS->lassopattern;
}

/*!
  FIXME: write doc
 */
void
SoExtSelection::animateOverlayLasso(const SbBool animate)
{
  THIS->lassopatternanimate = animate;
}

/*!
  FIXME: write doc
 */
SbBool
SoExtSelection::isOverlayLassoAnimated(void)
{
  return THIS->lassopatternanimate;
}

void
SoExtSelection::handleEvent(SoHandleEventAction * action)
{
  // do not call SoSelction::handleEvent()
  SoSeparator::handleEvent(action);
  if (action->isHandled()) return;

  const SoEvent *event = action->getEvent();
  const SbVec2s mousecoords = event->getPosition();

  switch (this->lassoType.getValue()) {

    // ---------- NO LASSO ----------

  case SoExtSelection::NOLASSO:
    // nothing to do here..
    break;

    // ---------- RECTANGLE ----------

  case SoExtSelection::RECTANGLE:
    // mouse click
    if (SO_MOUSE_PRESS_EVENT(event,BUTTON1)) {
      THIS->isDragging = TRUE;
      THIS->selectionstate = SoExtSelectionP::RECTANGLE;
      THIS->coords.truncate(0);
      THIS->coords.append(mousecoords);
      THIS->coords.append(mousecoords);
      if (!THIS->timersensor->isScheduled()) THIS->timersensor->schedule();
    }
    // mouse release
    else if (SO_MOUSE_RELEASE_EVENT(event,BUTTON1)) {
      THIS->timersensor->unschedule();
      THIS->isDragging = FALSE;
      THIS->selectionstate = SoExtSelectionP::NONE;
      THIS->curvp = action->getViewportRegion();
      THIS->cbaction->setViewportRegion(THIS->curvp);

      this->deselectAll();
      THIS->cbaction->apply(action->getCurPath()->getHead());
      this->touch();
    }
    // mouse move
    else if ((event->isOfType( SoLocation2Event::getClassTypeId()))) {
      if (THIS->isDragging == TRUE) {
	assert(THIS->coords.getLength() >= 2);
	THIS->coords[1] = mousecoords;
	this->touch();
      }
    }

    break;

    // ---------- LASSO ----------

  case SoExtSelection::LASSO:
    // mouse click
    if (SO_MOUSE_PRESS_EVENT(event,BUTTON1)) {
      if (event->wasShiftDown()) {
	if (THIS->selectionstate == SoExtSelectionP::NONE) {
	  THIS->coords.truncate(0);
	  THIS->coords.append(mousecoords);
	  THIS->selectionstate = SoExtSelectionP::LASSO;
	}
	THIS->isDragging = TRUE;
	THIS->coords.append(mousecoords);
        if (!THIS->timersensor->isScheduled()) THIS->timersensor->schedule();
	this->touch();
      }
    }
    // mouse release
    else if (SO_MOUSE_RELEASE_EVENT(event,BUTTON1)) {
      THIS->isDragging = FALSE;
    }
    // mouse move
    else if ( ( event->isOfType( SoLocation2Event::getClassTypeId() ) ) ) {
      if (THIS->isDragging == TRUE) {
	assert(THIS->coords.getLength());
	THIS->coords[THIS->coords.getLength()-1] = mousecoords;
	this->touch();
      }
    }
    // SHIFT press
    else if (SO_KEY_PRESS_EVENT(event,LEFT_SHIFT)) {
    }
    // SHIFT release
    else if (SO_KEY_RELEASE_EVENT(event,LEFT_SHIFT)) {
      THIS->timersensor->unschedule();
      if (THIS->selectionstate == SoExtSelectionP::LASSO) {
	THIS->curvp = action->getViewportRegion();
	THIS->cbaction->setViewportRegion(THIS->curvp);

        this->deselectAll();
	THIS->cbaction->apply(action->getCurPath()->getHead());
	this->touch();
      }
      THIS->isDragging = FALSE;
      THIS->selectionstate = SoExtSelectionP::NONE;
      THIS->coords.truncate(0);
    }
    break;
  }
}

// internal method for drawing lasso
void
SoExtSelection::draw(SoGLRenderAction *action)
{
  SbViewportRegion vp = action->getViewportRegion();
  SbVec2s vpo = vp.getViewportOriginPixels();
  SbVec2s vps = vp.getViewportSizePixels();

  // matrices
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(vpo[0], vpo[0]+vps[0]-1,
	  vpo[1], vpo[0]+vps[1]-1,
	  -1, 1);

  // attributes
  glPushAttrib(GL_LIGHTING_BIT|
	       GL_FOG_BIT|
	       GL_DEPTH_BUFFER_BIT|
	       GL_TEXTURE_BIT|
	       GL_LINE_BIT|
	       GL_CURRENT_BIT);
  glDisable(GL_LIGHTING);
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_FOG);
  glDisable(GL_DEPTH_TEST);

  // line color & width
  glColor3f(THIS->lassocolor[0],THIS->lassocolor[1],THIS->lassocolor[2]);
  glLineWidth(THIS->lassowidth);

  // stipple pattern
  glEnable(GL_LINE_STIPPLE);
  glLineStipple(1, THIS->lassopattern);

  // --- RECTANGLE ---

  if (THIS->selectionstate == SoExtSelectionP::RECTANGLE) {
    assert(THIS->coords.getLength() >= 2);
    SbVec2s c1 = THIS->coords[0];
    SbVec2s c2 = THIS->coords[1];
    glBegin(GL_LINE_LOOP);
    glVertex2s(c1[0], c1[1]);
    glVertex2s(c2[0], c1[1]);
    glVertex2s(c2[0], c2[1]);
    glVertex2s(c1[0], c2[1]);
    glEnd();
  }

  // --- LASSO ---

  else if (THIS->selectionstate == SoExtSelectionP::LASSO) {
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < THIS->coords.getLength(); i++) {
      SbVec2s temp = THIS->coords[i];
      glVertex2s(temp[0],temp[1]);
    }
    glEnd();
  }

  // finish - restore state
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  glPopAttrib();
}

// doc in parent
void
SoExtSelection::GLRenderBelowPath(SoGLRenderAction * action)
{
  inherited::GLRenderBelowPath(action);
  SoState *state = action->getState();
  state->push();

  if (action->isRenderingDelayedPaths()) {

    SbViewportRegion vp = SoViewportRegionElement::get(state);
    SbVec2s vpo = vp.getViewportOriginPixels();
    SbVec2s vps = vp.getViewportSizePixels();
    this->draw(action);
  }
  // render this path after all other (delayed)
  else {
    if (THIS->selectionstate != SoExtSelectionP::NONE) {
      action->addDelayedPath(action->getCurPath()->copy());
    }
  }
  state->pop();
}

#undef THIS

#ifndef DOXYGEN_SKIP_THIS

SoCallbackAction::Response
SoExtSelectionP::testShape(SoCallbackAction * action, const SoShape * shape)
{
  int i;
  SoState * state = action->getState();

  SbBox2s rectbbox;
  for (i = 0; i < this->coords.getLength(); i++) {
    rectbbox.extendBy(this->coords[i]);
  }

  SbMatrix projmatrix;
  projmatrix = (SoModelMatrixElement::get(state) *
                SoViewingMatrixElement::get(state) *
                SoProjectionMatrixElement::get(state));

  SbBool full = FALSE;
  switch (this->master->lassoPolicy.getValue()) {
  case SoExtSelection::FULL_BBOX:
    full = TRUE;
  case SoExtSelection::PART_BBOX:
    return testBBox(action, projmatrix, shape, rectbbox, full);
  case SoExtSelection::FULL:
  case SoExtSelection::PART:
    return testPrimitives(action, projmatrix, shape, rectbbox, full);
  default:
    assert(0 && "unknown lasso policy");
    break;
  }
  return SoCallbackAction::CONTINUE;
}


SoCallbackAction::Response
SoExtSelectionP::testBBox(SoCallbackAction * action,
                          const SbMatrix & projmatrix,
                          const SoShape * shape,
                          const SbBox2s & lassorect,
                          const SbBool full)
{
  SbBox3f bbox;
  SbVec3f center;
  ((SoShape*)shape)->computeBBox(action, bbox, center);

  SbVec3f mincorner = bbox.getMin();
  SbVec3f maxcorner = bbox.getMax();

  SbBox2s shapebbox;

  SbVec2s vppt;
  SbVec3f normpt;
  SbVec2s vpo = this->curvp.getViewportOriginPixels();
  SbVec2s vps = this->curvp.getViewportSizePixels();

  SbVec2s projpts[8];


  for (int i = 0; i < 8; i++) {
    SbVec3f corner(i & 1 ? mincorner[0] : maxcorner[0],
                   i & 2 ? mincorner[1] : maxcorner[1],
                   i & 4 ? mincorner[2] : maxcorner[2]);
    projmatrix.multVecMatrix(corner, normpt);
    normpt[0] += 1.0f;
    normpt[1] += 1.0f;
    normpt[0] *= 0.5f;
    normpt[1] *= 0.5f;

    normpt[0] *= (float) vps[0];
    normpt[1] *= (float) vps[1];
    normpt[0] += (float)vpo[0];
    normpt[1] += (float)vpo[1];

    vppt[0] = (short) SbClamp(normpt[0], -32768.0f, 32767.0f);
    vppt[1] = (short) SbClamp(normpt[1], -32768.0f, 32767.0f);
    projpts[i] = vppt;
    shapebbox.extendBy(vppt);
  }
  if (lassorect.intersect(shapebbox)) { // quick reject
    int i;
    int hit = 0;
    switch (this->master->lassoType.getValue()) {
    case SoExtSelection::LASSO:
      if (full) {
        for (i = 0; i < 8; i++) {
          if (!pointinpoly(this->coords, projpts[i])) break;
        }
        if (i == 8) hit = TRUE;
      }
      else {
        // not 100% correct since we're testing against the 
        // projected shape bbox, but should be good enough
        for (i = 0; i < this->coords.getLength(); i++) {
          if (shapebbox.intersect(this->coords[i])) { hit = TRUE; break; }
        }
        if (!hit) {
          for (i = 0; i < 8; i++) {
            if (pointinpoly(this->coords, projpts[i])) { hit = TRUE; break; }
          }
        }
      }
      break;
    case SoExtSelection::RECTANGLE:
      if (full) {
        for (i = 0; i < 8; i++) {
          if (!lassorect.intersect(projpts[i])) break;
        }
        if (i == 8) hit = TRUE;
      }
      else {
        for (i = 0; i < 8; i++) {
          if (lassorect.intersect(projpts[i])) { hit = TRUE; break; }
        }
      }
      break;
    default:
      break;
    }
    if (hit) {
      this->master->startCBList->invokeCallbacks(this->master);
      this->master->invokeSelectionPolicy((SoPath*) action->getCurPath(), TRUE);
      this->master->finishCBList->invokeCallbacks(this->master);
    }
  }
  return SoCallbackAction::CONTINUE;
}

SoCallbackAction::Response
SoExtSelectionP::testPrimitives(SoCallbackAction * action,
                                const SbMatrix & projmatrix,
                                const SoShape * shape,
                                const SbBox2s & lassorect,
                                const SbBool full)
{
  COIN_STUB();
  return SoCallbackAction::CONTINUE;
}

#endif // DOXYGEN_SKIP_THIS
