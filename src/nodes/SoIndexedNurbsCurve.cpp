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
  \class SoIndexedNurbsCurve SoIndexedNurbsCurve.h Inventor/nodes/SoIndexedNurbsCurve.h
  \brief The SoIndexedNurbsCurve class is a node for representing smooth curves.
  \ingroup nodes

  Explaining NURBS is beyond the scope of this documentation. If you
  are unfamiliar with the principles of representing smooth curves and
  surfaces when doing 3D visualization, we recommend finding a good
  book on the subject.
*/
// FIXME: Usage example! Also give a reference to a decent
// book on NURBS. 20011220 mortene.

#include <Inventor/nodes/SoIndexedNurbsCurve.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/bundles/SoMaterialBundle.h>
#include <Inventor/caches/SoBoundingBoxCache.h>
#include <Inventor/elements/SoCoordinateElement.h>
#include <Inventor/elements/SoPickStyleElement.h>
#include <Inventor/elements/SoGLTextureEnabledElement.h>
#include <Inventor/elements/SoDrawStyleElement.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/elements/SoLazyElement.h>
#include <Inventor/misc/SoGL.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/elements/SoComplexityTypeElement.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <coindefs.h> // COIN_OBSOLETED()
#include <Inventor/system/gl.h>
#include <../glue/GLUWrapper.h>


/*!
  \var SoSFInt32 SoIndexedNurbsCurve::numControlPoints
  Number of control points for this curve.
*/
/*!
  \var SoMFInt32 SoIndexedNurbsCurve::coordIndex
  The control point indices. Supply at least numControlPoint indices.
*/
/*!
  \var SoMFFloat SoIndexedNurbsCurve::knotVector
  The knot vector.
*/


// *************************************************************************

class SoIndexedNurbsCurveP {
public:
  SoIndexedNurbsCurveP(SoIndexedNurbsCurve * m)
  {
    this->owner = m;
    this->nurbsrenderer = NULL;
  }

  ~SoIndexedNurbsCurveP()
  {
    if (this->nurbsrenderer) {
      GLUWrapper()->gluDeleteNurbsRenderer(this->nurbsrenderer);
    }
  }

  void * nurbsrenderer;
  void doNurbsWrapper(SoAction * action, const SbBool drawaspoints);
  void doNurbs(SoAction * action, const SbBool glrender, const SbBool drawaspoints);

  static void APIENTRY tessBegin(int , void * data);
  static void APIENTRY tessTexCoord(float * texcoord, void * data);
  static void APIENTRY tessNormal(float * normal, void * data);
  static void APIENTRY tessVertex(float * vertex, void * data);
  static void APIENTRY tessEnd(void * data);

private:
  SoIndexedNurbsCurve * owner;
};

#undef PRIVATE
#define PRIVATE(p) (p->pimpl)
#undef PUBLIC
#define PUBLIC(p) (p->owner)

// *************************************************************************

SO_NODE_SOURCE(SoIndexedNurbsCurve);

/*!
  Constructor.
*/
SoIndexedNurbsCurve::SoIndexedNurbsCurve()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoIndexedNurbsCurve);

  SO_NODE_ADD_FIELD(numControlPoints, (0));
  SO_NODE_ADD_FIELD(coordIndex, (0));
  SO_NODE_ADD_FIELD(knotVector, (0));

  PRIVATE(this) = new SoIndexedNurbsCurveP(this);
}

/*!
  Destructor.
*/
SoIndexedNurbsCurve::~SoIndexedNurbsCurve()
{
  delete PRIVATE(this);
}

// doc from parent
void
SoIndexedNurbsCurve::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoIndexedNurbsCurve, SO_FROM_INVENTOR_1);
}

/*!
  Calculates the bounding box of all control points, and sets the
  center to the average of these points.
*/
void
SoIndexedNurbsCurve::computeBBox(SoAction * action,
                                 SbBox3f & box, SbVec3f & center)
{
  SoState * state = action->getState();
  const SoCoordinateElement * coordelem =
    SoCoordinateElement::getInstance(state);

  int num = this->numControlPoints.getValue();

  SbVec3f acccenter(0.0f, 0.0f, 0.0f);
  box.makeEmpty();
  SbVec3f tmp3D;
  if (coordelem->is3D()) {
    const SbVec3f * coords = coordelem->getArrayPtr3();
    assert(coords);
    for (int i = 0; i < num; i++) {
      tmp3D = coords[this->coordIndex[i]];
      box.extendBy(tmp3D);
      acccenter += tmp3D;
    }
  }
  else {
    const SbVec4f * coords = coordelem->getArrayPtr4();
    assert(coords);
    for (int i = 0; i < num; i++) {
      SbVec4f tmp = coords[this->coordIndex[i]];
      float mul = (tmp[3] != 0.0f) ? 1.0f / tmp[3] : 1.0f;
      tmp3D.setValue(tmp[0]*mul, tmp[1]*mul, tmp[2]*mul);
      box.extendBy(tmp3D);
      acccenter += tmp3D;
    }
  }
  if (num) center = acccenter / float(num);
}

// doc from parent
void
SoIndexedNurbsCurve::GLRender(SoGLRenderAction * action)
{
  if (!this->shouldGLRender(action)) return;

  SoState * state = action->getState();
  state->push();

  // disable lighting
  SoLazyElement::setLightModel(state, SoLazyElement::BASE_COLOR);

  // initialize current material
  SoMaterialBundle mb(action);
  mb.sendFirst();

  // disable texturing
  if (SoGLTextureEnabledElement::get(state)) {
    SoGLTextureEnabledElement::set(state, FALSE);
  }

  // Create lazy element for GL_AUTO_NORMAL ?
  glEnable(GL_AUTO_NORMAL);
  PRIVATE(this)->doNurbs(action, TRUE, SoDrawStyleElement::get(action->getState()) == SoDrawStyleElement::POINTS);
  glDisable(GL_AUTO_NORMAL);

  state->pop();
  if (SoComplexityTypeElement::get(state) == SoComplexityTypeElement::OBJECT_SPACE) {
    SoGLCacheContextElement::shouldAutoCache(state,
                                             SoGLCacheContextElement::DO_AUTO_CACHE);
  }
}

// doc from parent
void
SoIndexedNurbsCurve::rayPick(SoRayPickAction * action)
{
  if (GLUWrapper()->versionMatchesAtLeast(1, 3, 0)) {
    SoShape::rayPick(action); // do normal generatePrimitives() pick
  }
  else {
    if (!this->shouldRayPick(action)) return;
    static SbBool firstpick = TRUE;
    if (firstpick) {
      firstpick = FALSE;
      SoDebugError::postWarning("SoIndexedNurbsCurve::rayPick",
                                "Proper NURBS picking requires\n"
                                "GLU version 1.3. Picking will be done on bounding box.");
    }
    SoState * state = action->getState();
    state->push();
    SoPickStyleElement::set(state, this, SoPickStyleElement::BOUNDING_BOX);
    (void)this->shouldRayPick(action); // this will cause a pick on bbox
    state->pop();
  }
}

// doc from parent
void
SoIndexedNurbsCurve::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  // for now, just generate primitives to count. Very slow, of course.
  SoShape::getPrimitiveCount(action);
}

/*!
  Redefined to notify open caches that this shape contains lines.
*/
void
SoIndexedNurbsCurve::getBoundingBox(SoGetBoundingBoxAction * action)
{
  inherited::getBoundingBox(action);
  SoBoundingBoxCache::setHasLinesOrPoints(action->getState());
}

/*!
  This method is part of the original SGI Inventor API, but not
  implemented in Coin, as it looks like a method that should probably
  have been private in Open Inventor.
*/
void
SoIndexedNurbsCurve::sendPrimitive(SoAction * ,  SoPrimitiveVertex *)
{
  COIN_OBSOLETED();
}

// doc from parent
void
SoIndexedNurbsCurve::generatePrimitives(SoAction * action)
{
  if (GLUWrapper()->versionMatchesAtLeast(1, 3, 0)) {
    // HACK WARNING: pederb, 2002-01-30
    //
    // call doNurbsWrapper() instead of doNurbs(). doNurbsWrapper()
    // will use sogl_offscreencontext_callback() to call
    // doNurbs(). This is needed since it seems like the GLU NURBS
    // renderer makes some OpenGL calls even when in tessellate
    // mode. sogl_offscreencontext_callback() will set up an offscreen
    // context so that we are guaranteed to have a valid GL context
    // before making the GLU calls.
    PRIVATE(this)->doNurbsWrapper(action, FALSE);
  }
}

// Documented in superclass.
SoDetail *
SoIndexedNurbsCurve::createLineSegmentDetail(SoRayPickAction * /* action */,
                                             const SoPrimitiveVertex * /* v1 */,
                                             const SoPrimitiveVertex * /* v2 */,
                                             SoPickedPoint * /* pp */)
{
  return NULL;
}

//
// used only for GLU callbacks
//
typedef struct {
  SoAction * action;
  SoIndexedNurbsCurve * thisp;
  SoPrimitiveVertex vertex;
} coin_inc_cbdata;

void
SoIndexedNurbsCurveP::doNurbs(SoAction * action,
                              const SbBool glrender, const SbBool drawaspoints)
{
  if (GLUWrapper()->available == 0 || !GLUWrapper()->gluNewNurbsRenderer) {
#if COIN_DEBUG
    static int first = 1;
    if (first) {
      SoDebugError::postInfo("SoIndexedNurbsCurve::doNurbs",
                             "Looks like your GLU library doesn't have NURBS "
                             "functionality");
      first = 0;
    }
#endif // COIN_DEBUG
    return;
  }

  if (this->nurbsrenderer == NULL) {
    this->nurbsrenderer = GLUWrapper()->gluNewNurbsRenderer();

    if (GLUWrapper()->versionMatchesAtLeast(1, 3, 0)) {
      GLUWrapper()->gluNurbsCallback(this->nurbsrenderer, (GLenum) GLU_NURBS_BEGIN_DATA, (gluNurbsCallback_cb_t)SoIndexedNurbsCurveP::tessBegin);
      GLUWrapper()->gluNurbsCallback(this->nurbsrenderer, (GLenum) GLU_NURBS_TEXTURE_COORD_DATA, (gluNurbsCallback_cb_t)SoIndexedNurbsCurveP::tessTexCoord);
      GLUWrapper()->gluNurbsCallback(this->nurbsrenderer, (GLenum) GLU_NURBS_NORMAL_DATA, (gluNurbsCallback_cb_t)SoIndexedNurbsCurveP::tessNormal);
      GLUWrapper()->gluNurbsCallback(this->nurbsrenderer, (GLenum) GLU_NURBS_VERTEX_DATA, (gluNurbsCallback_cb_t)SoIndexedNurbsCurveP::tessVertex);
      GLUWrapper()->gluNurbsCallback(this->nurbsrenderer, (GLenum) GLU_NURBS_END_DATA, (gluNurbsCallback_cb_t)SoIndexedNurbsCurveP::tessEnd);
    }
  }

  // NB, don't move this structure inside the if-statement. It needs
  // to be here so that the callbacks from sogl_render_nurbs_curve()
  // have a valid pointer to the structure.
  coin_inc_cbdata cbdata;

  if (GLUWrapper()->versionMatchesAtLeast(1, 3, 0)) {
    if (!glrender) {
      GLUWrapper()->gluNurbsCallbackData(this->nurbsrenderer, &cbdata);
      cbdata.action = action;
      cbdata.thisp = PUBLIC(this);
      cbdata.vertex.setNormal(SbVec3f(0.0f, 0.0f, 1.0f));
      cbdata.vertex.setMaterialIndex(0);
      cbdata.vertex.setTextureCoords(SbVec4f(0.0f, 0.0f, 0.0f, 1.0f));
      cbdata.vertex.setPoint(SbVec3f(0.0f, 0.0f, 0.0f));
      cbdata.vertex.setDetail(NULL);
    }
  }

  sogl_render_nurbs_curve(action, PUBLIC(this), this->nurbsrenderer,
                          PUBLIC(this)->numControlPoints.getValue(),
                          PUBLIC(this)->knotVector.getValues(0),
                          PUBLIC(this)->knotVector.getNum(),
                          glrender,
                          drawaspoints,
                          PUBLIC(this)->coordIndex.getNum(),
                          PUBLIC(this)->coordIndex.getValues(0));
}

void APIENTRY
SoIndexedNurbsCurveP::tessBegin(int type, void * data)
{
  coin_inc_cbdata * cbdata = (coin_inc_cbdata*) data;
  SoIndexedNurbsCurve::TriangleShape shapetype;
  switch ((int)type) {
  case GL_LINES:
    shapetype = SoShape::LINES;
    break;
  case GL_LINE_STRIP:
    shapetype = SoShape::LINE_STRIP;
    break;
  case GL_LINE_LOOP:
    shapetype = SoShape::LINE_STRIP; // will not be closed...
#if COIN_DEBUG && 1 // debug
    SoDebugError::postInfo("SoIndexedNurbsCurve::tessBegin",
                           "LINE_LOOP is not supported yet");
#endif // debug
    break;
  case GL_POINTS:
    shapetype = SoShape::POINTS;
    break;
  default:
    shapetype = SoShape::POLYGON; // illegal value
#if COIN_DEBUG && 1 // debug
    SoDebugError::postInfo("SoIndexedNurbsCurve::tessBegin",
                           "unsupported GL enum: 0x%x", type);
#endif // debug
    break;
  }
  if (shapetype != SoShape::POINTS) {
    cbdata->thisp->beginShape(cbdata->action, shapetype, NULL);
  }
}

void APIENTRY
SoIndexedNurbsCurveP::tessTexCoord(float * texcoord, void * data)
{
  coin_inc_cbdata * cbdata = (coin_inc_cbdata*) data;
  cbdata->vertex.setTextureCoords(SbVec4f(texcoord[0], texcoord[1], texcoord[2], texcoord[3]));
}

void APIENTRY
SoIndexedNurbsCurveP::tessNormal(float * normal, void * data)
{
  coin_inc_cbdata * cbdata = (coin_inc_cbdata*) data;
  cbdata->vertex.setNormal(SbVec3f(normal[0], normal[1], normal[2]));
}

void APIENTRY
SoIndexedNurbsCurveP::tessVertex(float * vertex, void * data)
{
  coin_inc_cbdata * cbdata = (coin_inc_cbdata*) data;
  cbdata->vertex.setPoint(SbVec3f(vertex[0], vertex[1], vertex[2]));
  cbdata->thisp->shapeVertex(&cbdata->vertex);
}

void APIENTRY
SoIndexedNurbsCurveP::tessEnd(void * data)
{
  coin_inc_cbdata * cbdata = (coin_inc_cbdata*) data;
  cbdata->thisp->endShape();
}

typedef struct {
  SoIndexedNurbsCurveP * thisp;
  SoAction * action;
  SbBool drawaspoints;
} soidxnurbscurve_call_donurbs_data;

static void call_donurbs(void * userdata, SoAction * action)
{
  soidxnurbscurve_call_donurbs_data * data = 
    (soidxnurbscurve_call_donurbs_data*) userdata;
  data->thisp->doNurbs(data->action, FALSE, data->drawaspoints);
}

void 
SoIndexedNurbsCurveP::doNurbsWrapper(SoAction * action, const SbBool drawaspoints)
{
  soidxnurbscurve_call_donurbs_data data;
  data.thisp = this;
  data.action = action;
  data.drawaspoints = drawaspoints;
  sogl_offscreencontext_callback(call_donurbs, &data);
}
