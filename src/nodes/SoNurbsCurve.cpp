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
  \class SoNurbsCurve SoNurbsCurve.h Inventor/nodes/SoNurbsCurve.h
  \brief The SoNurbsCurve class is a node for representing smooth curves.
  \ingroup nodes

  Explaining NURBS is beyond the scope of this documentation. If you
  are unfamiliar with the principles of representing smooth curves and
  surfaces when doing 3D visualization, we recommend finding a good
  book on the subject.
*/
// FIXME: recommend the book pederb bought about the subject? If so,
// also add to class doc for SoNurbsSurface (and others?). Plus, add a
// usage exaple. 20010909 mortene.

#include <Inventor/nodes/SoNurbsCurve.h>
#include <Inventor/nodes/SoSubNodeP.h>

#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/bundles/SoMaterialBundle.h>
#include <Inventor/caches/SoBoundingBoxCache.h>
#include <Inventor/elements/SoCoordinateElement.h>
#include <Inventor/elements/SoDrawStyleElement.h>
#include <Inventor/elements/SoGLTextureEnabledElement.h>
#include <Inventor/elements/SoPickStyleElement.h>
#include <Inventor/elements/SoLazyElement.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/misc/SoGL.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/elements/SoComplexityTypeElement.h>

#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H
#include <coindefs.h> // COIN_OBSOLETED()
#include <Inventor/system/gl.h>
#include <../misc/GLUWrapper.h>

/*!
  \var SoSFInt32 SoNurbsCurve::numControlPoints
  Number of control points to use in this NURBS curve.
*/
/*!
  \var SoMFFloat SoNurbsCurve::knotVector
  The knot vector.
*/

// *************************************************************************

class SoNurbsCurveP {
public:
  SoNurbsCurveP(SoNurbsCurve * m)
  {
    this->owner = m;
    this->nurbsrenderer = NULL;
  }

  ~SoNurbsCurveP()
  {
    if (this->nurbsrenderer) {
      GLUWrapper()->gluDeleteNurbsRenderer(this->nurbsrenderer);
    }
  }

  void * nurbsrenderer;
  void doNurbs(SoAction * action, const SbBool glrender, const SbBool drawaspoints);

  static void APIENTRY tessBegin(int type, void * data);
  static void APIENTRY tessTexCoord(float * texcoord, void * data);
  static void APIENTRY tessNormal(float * normal, void * data);
  static void APIENTRY tessVertex(float * vertex, void * data);
  static void APIENTRY tessEnd(void * data);

private:
  SoNurbsCurve * owner;
};

#undef PRIVATE
#define PRIVATE(p) (p->pimpl)
#undef PUBLIC
#define PUBLIC(p) (p->owner)

// *************************************************************************

SO_NODE_SOURCE(SoNurbsCurve);

/*!
  Constructor.
*/
SoNurbsCurve::SoNurbsCurve(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoNurbsCurve);

  SO_NODE_ADD_FIELD(numControlPoints, (0));
  SO_NODE_ADD_FIELD(knotVector, (0));

  PRIVATE(this) = new SoNurbsCurveP(this);
}

/*!
  Destructor.
*/
SoNurbsCurve::~SoNurbsCurve()
{
  delete PRIVATE(this);
}

// Doc from parent class.
void
SoNurbsCurve::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoNurbsCurve, SO_FROM_INVENTOR_1);
}

// Doc from parent class.
void
SoNurbsCurve::GLRender(SoGLRenderAction * action)
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

/*!
  Calculates the bounding box of all control points, and sets the
  center to the average of these points.
*/
void
SoNurbsCurve::computeBBox(SoAction * action, SbBox3f & box, SbVec3f & center)
{
  SoState * state = action->getState();
  const SoCoordinateElement * coordelem =
    SoCoordinateElement::getInstance(state);

  int numCoords = coordelem->getNum();
  int num = this->numControlPoints.getValue();

  assert(num <= numCoords);

  SbVec3f acccenter(0.0f, 0.0f, 0.0f);
  box.makeEmpty();
  if (coordelem->is3D()) {
    const SbVec3f * coords = coordelem->getArrayPtr3();
    assert(coords);
    for (int i = 0; i < num; i++) {
      box.extendBy(coords[i]);
      acccenter += coords[i];
    }
  }
  else {
    const SbVec4f * coords = coordelem->getArrayPtr4();
    assert(coords);
    for (int i = 0; i < num; i++) {
      SbVec4f tmp = coords[i];
      float mul = (tmp[3] != 0.0f) ? 1.0f / tmp[3] : 1.0f;
      SbVec3f tmp3D(tmp[0]*mul, tmp[1]*mul, tmp[2]*mul);
      box.extendBy(tmp3D);
      acccenter += tmp3D;
    }
  }
  if (num) center = acccenter / float(num);
}

// Doc from parent class.
void
SoNurbsCurve::rayPick(SoRayPickAction * action)
{
  if (GLUWrapper()->versionMatchesAtLeast(1, 3, 0)) {
    SoShape::rayPick(action); // do normal generatePrimitives() pick
  }
  else {
    if (!this->shouldRayPick(action)) return;
    static SbBool firstpick = TRUE;
    if (firstpick) {
      firstpick = FALSE;
      SoDebugError::postWarning("SoNurbsCurve::rayPick",
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

// Doc from parent class.
void
SoNurbsCurve::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  // for now, just generate primitives to count. Very slow, of course.
  SoShape::getPrimitiveCount(action);
}

/*!
  Redefined to notify open caches that this shape contains lines.
*/
void
SoNurbsCurve::getBoundingBox(SoGetBoundingBoxAction * action)
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
SoNurbsCurve::sendPrimitive(SoAction * a, SoPrimitiveVertex * p)
{
  COIN_OBSOLETED();
}

// Doc from parent class.
void
SoNurbsCurve::generatePrimitives(SoAction * action)
{
  PRIVATE(this)->doNurbs(action, FALSE, FALSE);
}

// Documented in superclass.
SoDetail *
SoNurbsCurve::createLineSegmentDetail(SoRayPickAction * action,
                                      const SoPrimitiveVertex * v1,
                                      const SoPrimitiveVertex * v2,
                                      SoPickedPoint * pp)
{
  return NULL;
}

//
// used only for GLU callbacks
//
typedef struct {
  SoAction * action;
  SoNurbsCurve * thisp;
  SoPrimitiveVertex vertex;
} coin_nc_cbdata;

void
SoNurbsCurveP::doNurbs(SoAction * action,
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
      GLUWrapper()->gluNurbsCallback(this->nurbsrenderer, (GLenum) GLU_NURBS_BEGIN_DATA, (gluNurbsCallback_cb_t)tessBegin);
      GLUWrapper()->gluNurbsCallback(this->nurbsrenderer, (GLenum) GLU_NURBS_TEXTURE_COORD_DATA, (gluNurbsCallback_cb_t)tessTexCoord);
      GLUWrapper()->gluNurbsCallback(this->nurbsrenderer, (GLenum) GLU_NURBS_NORMAL_DATA, (gluNurbsCallback_cb_t)tessNormal);
      GLUWrapper()->gluNurbsCallback(this->nurbsrenderer, (GLenum) GLU_NURBS_VERTEX_DATA, (gluNurbsCallback_cb_t)tessVertex);
      GLUWrapper()->gluNurbsCallback(this->nurbsrenderer, (GLenum) GLU_NURBS_END_DATA, (gluNurbsCallback_cb_t)tessEnd);
    }
  }

  // NB, don't move this structure inside the if-statement. It needs
  // to be here so that the callbacks from sogl_render_nurbs_curve()
  // have a valid pointer to the structure.
  coin_nc_cbdata cbdata;

  if (GLUWrapper()->versionMatchesAtLeast(1, 3, 0)) {
    if (glrender) {
      GLUWrapper()->gluNurbsCallbackData(this->nurbsrenderer, NULL);
    }
    else {
      cbdata.action = action;
      cbdata.thisp = PUBLIC(this);
      cbdata.vertex.setNormal(SbVec3f(0.0f, 0.0f, 1.0f));
      cbdata.vertex.setMaterialIndex(0);
      cbdata.vertex.setTextureCoords(SbVec4f(0.0f, 0.0f, 0.0f, 1.0f));
      cbdata.vertex.setPoint(SbVec3f(0.0f, 0.0f, 0.0f));
      cbdata.vertex.setDetail(NULL);
      GLUWrapper()->gluNurbsCallbackData(this->nurbsrenderer, &cbdata);
    }
  }

  sogl_render_nurbs_curve(action, PUBLIC(this), this->nurbsrenderer,
                          PUBLIC(this)->numControlPoints.getValue(),
                          PUBLIC(this)->knotVector.getValues(0),
                          PUBLIC(this)->knotVector.getNum(),
                          glrender,
                          drawaspoints);
}

void APIENTRY
SoNurbsCurveP::tessBegin(int type, void * data)
{
  coin_nc_cbdata * cbdata = (coin_nc_cbdata*) data;
  SoNurbsCurve::TriangleShape shapetype;
  switch (type) {
  case GL_LINES:
    shapetype = SoShape::LINES;
    break;
  case GL_LINE_STRIP:
    shapetype = SoShape::LINE_STRIP;
    break;
  case GL_LINE_LOOP:
    shapetype = SoShape::LINE_STRIP; // will not be closed...
    // FIXME: implement this functionality. 20010909 mortene.

    // FIXME: un-commenting the following line exposes a _weird_ HP-UX
    // aCC bug -- should investigate closer.  (Detected with aCC
    // version B3910B A.03.25).  If possible, try to re-write the
    // COIN_STUB() function with a work-around for the aCC bug.
    // 20010917 mortene.

    //COIN_STUB();

#if COIN_DEBUG
    SoDebugError::postWarning("SoNurbsCurveP::tessBegin",
                              "LINE_LOOP is not supported yet");
#endif // COIN_DEBUG
    break;
  case GL_POINTS:
    shapetype = SoShape::POINTS;
    break;
  default:
    shapetype = SoShape::POLYGON; // illegal value
    // FIXME: should this be an assert, or does it represent
    // something which is out of our control, like a possible future
    // feature of the GLU tessellator?  20010909 mortene.
#if COIN_DEBUG && 1 // debug
    SoDebugError::postWarning("SoNurbsCurveP::tessBegin",
                              "unsupported GL enum: 0x%x", type);
#endif // debug
    break;
  }
  if (shapetype != SoShape::POINTS) {
    cbdata->thisp->beginShape(cbdata->action, shapetype, NULL);
  }
}

void APIENTRY
SoNurbsCurveP::tessTexCoord(float * texcoord, void * data)
{
  coin_nc_cbdata * cbdata = (coin_nc_cbdata*) data;
  cbdata->vertex.setTextureCoords(SbVec4f(texcoord[0], texcoord[1], texcoord[2], texcoord[3]));
}

void APIENTRY
SoNurbsCurveP::tessNormal(float * normal, void * data)
{
  coin_nc_cbdata * cbdata = (coin_nc_cbdata*) data;
  cbdata->vertex.setNormal(SbVec3f(normal[0], normal[1], normal[2]));
}

void APIENTRY
SoNurbsCurveP::tessVertex(float * vertex, void * data)
{
  coin_nc_cbdata * cbdata = (coin_nc_cbdata*) data;
  cbdata->vertex.setPoint(SbVec3f(vertex[0], vertex[1], vertex[2]));
  cbdata->thisp->shapeVertex(&cbdata->vertex);
}

void APIENTRY
SoNurbsCurveP::tessEnd(void * data)
{
  coin_nc_cbdata * cbdata = (coin_nc_cbdata*) data;
  cbdata->thisp->endShape();
}
