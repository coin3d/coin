/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoNurbsSurface SoNurbsSurface.h Inventor/nodes/SoNurbsSurface.h
  \brief The SoNurbsSurface class is used to render smooth surfaces.
  \ingroup nodes

  Explaining NURBS is beyond the scope of this documentation. If you
  are unfamiliar with the principles of representing smooth curves and
  surfaces when doing 3D visualization, we recommend finding a good
  book on the subject.
*/
// FIXME: recommend the book pederb bought about the subject? If so,
// also add to class doc for SoNurbsSurface (and others?). Plus, add a
// usage exaple. 20010909 mortene.

#include <Inventor/nodes/SoNurbsSurface.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <coindefs.h> // COIN_OBSOLETED()
#include <Inventor/bundles/SoMaterialBundle.h>
#include <Inventor/elements/SoCoordinateElement.h>
#include <Inventor/elements/SoPickStyleElement.h>
#include <Inventor/elements/SoDrawStyleElement.h>

#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/misc/SoGL.h>
#include <Inventor/SoOffscreenRenderer.h>
#include <Inventor/nodes/SoCallback.h>

#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/elements/SoComplexityTypeElement.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/system/gl.h>
#include <Inventor/C/glue/GLUWrapper.h>



/*!
  \var SoSFInt32 SoNurbsSurface::numUControlPoints
  Number of control points in the U direction.
*/
/*!
  \var SoSFInt32 SoNurbsSurface::numVControlPoints
  Number of control points in the V direction.
*/
/*!
  \var SoSFInt32 SoNurbsSurface::numSControlPoints
  Number of control points in the S direction.
*/
/*!
  \var SoSFInt32 SoNurbsSurface::numTControlPoints
  Number of control points in the T direction.
*/
/*!
  \var SoMFFloat SoNurbsSurface::uKnotVector
  The Bezier knot vector for the U direction.
*/
/*!
  \var SoMFFloat SoNurbsSurface::vKnotVector
  The Bezier knot vector for the V direction.
*/
/*!
  \var SoMFFloat SoNurbsSurface::sKnotVector
  The Bezier knot vector for the S direction.
*/
/*!
  \var SoMFFloat SoNurbsSurface::tKnotVector
  The Bezier knot vector for the T direction.
*/

// *************************************************************************

class SoNurbsSurfaceP {
public: 
  SoNurbsSurfaceP(SoNurbsSurface * m)
  {
    this->owner = m;
    this->nurbsrenderer = NULL;
  }

  ~SoNurbsSurfaceP()
  {
    if (this->nurbsrenderer) {
      GLUWrapper()->gluDeleteNurbsRenderer(this->nurbsrenderer);
    }
  }

  void * nurbsrenderer;
  void doNurbsWrapper(SoAction * action);
  void doNurbs(SoAction * action, const SbBool glrender);

  static void APIENTRY tessBegin(int , void * data);
  static void APIENTRY tessTexCoord(float * texcoord, void * data);
  static void APIENTRY tessNormal(float * normal, void * data);
  static void APIENTRY tessVertex(float * vertex, void * data);
  static void APIENTRY tessEnd(void * data);

private:
  SoNurbsSurface * owner;
};

#undef PRIVATE
#define PRIVATE(p) (p->pimpl)
#undef PUBLIC
#define PUBLIC(p) (p->owner)

// *************************************************************************

SO_NODE_SOURCE(SoNurbsSurface);

/*!
  Constructor.
*/
SoNurbsSurface::SoNurbsSurface()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoNurbsSurface);

  SO_NODE_ADD_FIELD(numUControlPoints, (0));
  SO_NODE_ADD_FIELD(numVControlPoints, (0));
  SO_NODE_ADD_FIELD(numSControlPoints, (0));
  SO_NODE_ADD_FIELD(numTControlPoints, (0));
  SO_NODE_ADD_FIELD(uKnotVector, (0));
  SO_NODE_ADD_FIELD(vKnotVector, (0));
  SO_NODE_ADD_FIELD(sKnotVector, (0));
  SO_NODE_ADD_FIELD(tKnotVector, (0));

  PRIVATE(this) = new SoNurbsSurfaceP(this);
}

/*!
  Destructor.
*/
SoNurbsSurface::~SoNurbsSurface()
{
  delete PRIVATE(this);
}

// Documented in superclass.
void
SoNurbsSurface::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoNurbsSurface, SO_FROM_INVENTOR_1);
}

// Documented in superclass.
void
SoNurbsSurface::GLRender(SoGLRenderAction * action)
{
  if (!this->shouldGLRender(action)) return;

  // initialize current material
  SoMaterialBundle mb(action);
  mb.sendFirst();

  // Create lazy element for GL_AUTO_NORMAL ?
  glEnable(GL_AUTO_NORMAL);
  PRIVATE(this)->doNurbs(action, TRUE);
  glDisable(GL_AUTO_NORMAL);

  SoState * state = action->getState();
  if (SoComplexityTypeElement::get(state) == SoComplexityTypeElement::OBJECT_SPACE) {
    SoGLCacheContextElement::shouldAutoCache(state,
                                             SoGLCacheContextElement::DO_AUTO_CACHE);
  }
}

/*!
  Calculates the bounding box of all control points, and sets the center to
  the average of these points.
*/
void
SoNurbsSurface::computeBBox(SoAction * action, SbBox3f & box, SbVec3f & center)
{
  SoState * state = action->getState();
  const SoCoordinateElement * coordelem =
    SoCoordinateElement::getInstance(state);

  int num =
    this->numUControlPoints.getValue() * this->numVControlPoints.getValue();
  assert(num <= coordelem->getNum());

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
    for (int i = 0; i< num; i++) {
      SbVec4f tmp = coords[i];
      if (tmp[3] != 0.0f) {
        float mul = 1.0f / tmp[3];
        tmp[0] *= mul;
        tmp[1] *= mul;
        tmp[2] *= mul;
      }
      acccenter += SbVec3f(tmp[0], tmp[1], tmp[2]);
      box.extendBy(SbVec3f(tmp[0], tmp[1], tmp[2]));
    }
  }
  if (num > 0) center = acccenter / float(num);
}

// Documented in superclass.
void
SoNurbsSurface::rayPick(SoRayPickAction * action)
{
  if (GLUWrapper()->versionMatchesAtLeast(1, 3, 0)) {
    SoShape::rayPick(action); // do normal generatePrimitives() pick
  }
  else {
    if (!this->shouldRayPick(action)) return;
    static SbBool firstpick = TRUE;
    if (firstpick) {
      firstpick = FALSE;
      SoDebugError::postWarning("SoNurbsSurface::rayPick",
                                "Proper NURBS picking requires\n"
                                "GLU version 1.3. Picking is done on bounding box.");
    }
    SoState * state = action->getState();
    state->push();
    SoPickStyleElement::set(state, this, SoPickStyleElement::BOUNDING_BOX);
    (void)this->shouldRayPick(action); // this will cause a pick on bbox
    state->pop();
  }
}

// Doc in superclass.
void
SoNurbsSurface::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  // for now, just generate primitives to count. Very slow, of course.
  SoShape::getPrimitiveCount(action);
}

/*!
  This method is part of the original SGI Inventor API, but not
  implemented in Coin, as it looks like a method that should probably
  have been private in Open Inventor.
*/
void
SoNurbsSurface::sendPrimitive(SoAction *,  SoPrimitiveVertex *)
{
  COIN_OBSOLETED();
}

// Documented in superclass.
void
SoNurbsSurface::generatePrimitives(SoAction * action)
{
  if (GLUWrapper()->versionMatchesAtLeast(1, 3, 0)) {
    // HACK WARNING: pederb, 2003-01-30
    //
    // call doNurbsWrapper() instead of doNurbs(). doNurbsWrapper()
    // will use sogl_offscreencontext_callback() to call
    // doNurbs(). This is needed since it seems like the GLU NURBS
    // renderer makes some OpenGL calls even when in tessellate
    // mode. sogl_offscreencontext_callback() will set up an offscreen
    // context so that we are guaranteed to have a valid GL context
    // before making the GLU calls.
    PRIVATE(this)->doNurbsWrapper(action);
  }
}

// Documented in superclass.
SoDetail *
SoNurbsSurface::createTriangleDetail(SoRayPickAction * /* action */,
                                     const SoPrimitiveVertex * /*v1*/,
                                     const SoPrimitiveVertex * /*v2*/,
                                     const SoPrimitiveVertex * /*v3*/,
                                     SoPickedPoint * /* pp */)
{
  return NULL;
}

//
// used only for GLU callbacks
//
typedef struct {
  SoAction * action;
  SoNurbsSurface * thisp;
  SoPrimitiveVertex vertex;
} coin_ns_cbdata;

//
// render or generate the NURBS surface
//
void
SoNurbsSurfaceP::doNurbs(SoAction * action, const SbBool glrender)
{
  if (GLUWrapper()->available == 0 || !GLUWrapper()->gluNewNurbsRenderer) {
#if COIN_DEBUG
    static int first = 1;
    if (first) {
      SoDebugError::postInfo("SoIndexedNurbsCurveP::doNurbs",
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
      GLUWrapper()->gluNurbsCallback(this->nurbsrenderer, (GLenum) GLU_NURBS_BEGIN_DATA, (gluNurbsCallback_cb_t)SoNurbsSurfaceP::tessBegin);
      GLUWrapper()->gluNurbsCallback(this->nurbsrenderer, (GLenum) GLU_NURBS_TEXTURE_COORD_DATA, (gluNurbsCallback_cb_t)SoNurbsSurfaceP::tessTexCoord);
      GLUWrapper()->gluNurbsCallback(this->nurbsrenderer, (GLenum) GLU_NURBS_NORMAL_DATA,  (gluNurbsCallback_cb_t)SoNurbsSurfaceP::tessNormal);
      GLUWrapper()->gluNurbsCallback(this->nurbsrenderer, (GLenum) GLU_NURBS_VERTEX_DATA,  (gluNurbsCallback_cb_t)SoNurbsSurfaceP::tessVertex);
      GLUWrapper()->gluNurbsCallback(this->nurbsrenderer, (GLenum) GLU_NURBS_END_DATA,  (gluNurbsCallback_cb_t)SoNurbsSurfaceP::tessEnd);
    }
  }

  // NB, don't move this structure inside the if-statement. It needs
  // to be here so that the callbacks from sogl_render_nurbs_surface()
  // have a valid pointer to the structure.
  coin_ns_cbdata cbdata;

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

  int displaymode = (int) GLU_FILL;
  if (glrender) {
    switch (SoDrawStyleElement::get(action->getState())) {
    case SoDrawStyleElement::LINES:
      displaymode = (int) GLU_OUTLINE_POLYGON;
      break;
    case SoDrawStyleElement::POINTS:
      // not possible to draw NURBS as points using GLU...
      displaymode = (int) GLU_OUTLINE_PATCH;
      break;
    default:
      break;
    }
  }
  GLUWrapper()->gluNurbsProperty(this->nurbsrenderer, (GLenum) GLU_DISPLAY_MODE, displaymode);

  sogl_render_nurbs_surface(action, PUBLIC(this), this->nurbsrenderer,
                            PUBLIC(this)->numUControlPoints.getValue(),
                            PUBLIC(this)->numVControlPoints.getValue(),
                            PUBLIC(this)->uKnotVector.getValues(0),
                            PUBLIC(this)->vKnotVector.getValues(0),
                            PUBLIC(this)->uKnotVector.getNum(),
                            PUBLIC(this)->vKnotVector.getNum(),
                            PUBLIC(this)->numSControlPoints.getValue(),
                            PUBLIC(this)->numTControlPoints.getValue(),
                            PUBLIC(this)->sKnotVector.getValues(0),
                            PUBLIC(this)->tKnotVector.getValues(0),
                            PUBLIC(this)->sKnotVector.getNum(),
                            PUBLIC(this)->tKnotVector.getNum(),
                            glrender);
}

void APIENTRY
SoNurbsSurfaceP::tessBegin(int type, void * data)
{
  coin_ns_cbdata * cbdata = (coin_ns_cbdata*) data;
  SoNurbsSurface::TriangleShape shapetype;
  switch ((int)type) {
  case GL_TRIANGLES:
    shapetype = SoShape::TRIANGLES;
    break;
  case GL_TRIANGLE_STRIP:
    shapetype = SoShape::TRIANGLE_STRIP;
    break;
  case GL_TRIANGLE_FAN:
    shapetype = SoShape::TRIANGLE_FAN;
    break;
  case GL_QUADS:
    shapetype = SoShape::QUADS;
    break;
  case GL_QUAD_STRIP:
    shapetype = SoShape::QUAD_STRIP;
    break;
  case GL_POLYGON:
    shapetype = SoShape::POLYGON;
    break;
  default:
    shapetype = SoShape::POINTS; // illegal value
#if COIN_DEBUG && 1 // debug
    SoDebugError::postInfo("SoNurbsSurface::tessBegin",
                           "unsupported GL enum: 0x%x", type);
#endif // debug
    break;
  }
  if (shapetype != SoShape::POINTS) {
    cbdata->thisp->beginShape(cbdata->action, shapetype, NULL);
  }
}

void APIENTRY
SoNurbsSurfaceP::tessTexCoord(float * texcoord, void * data)
{
  coin_ns_cbdata * cbdata = (coin_ns_cbdata*) data;
  cbdata->vertex.setTextureCoords(SbVec2f(texcoord[0], texcoord[1]));
}

void APIENTRY
SoNurbsSurfaceP::tessNormal(float * normal, void * data)
{
  coin_ns_cbdata * cbdata = (coin_ns_cbdata*) data;
  cbdata->vertex.setNormal(SbVec3f(normal[0], normal[1], normal[2]));
}

void APIENTRY
SoNurbsSurfaceP::tessVertex(float * vertex, void * data)
{
  coin_ns_cbdata * cbdata = (coin_ns_cbdata*) data;
  cbdata->vertex.setPoint(SbVec3f(vertex[0], vertex[1], vertex[2]));
  cbdata->thisp->shapeVertex(&cbdata->vertex);
}

void APIENTRY
SoNurbsSurfaceP::tessEnd(void * data)
{
  coin_ns_cbdata * cbdata = (coin_ns_cbdata*) data;
  cbdata->thisp->endShape();
}

typedef struct {
  SoNurbsSurfaceP * thisp;
  SoAction * action;
} sonurbssurface_call_donurbs_data;

static void call_donurbs(void * userdata, SoAction * action)
{
  sonurbssurface_call_donurbs_data * data = 
    (sonurbssurface_call_donurbs_data*) userdata;
  data->thisp->doNurbs(data->action, FALSE);
}

void 
SoNurbsSurfaceP::doNurbsWrapper(SoAction * action)
{
  sonurbssurface_call_donurbs_data data;
  data.thisp = this;
  data.action = action;
  sogl_offscreencontext_callback(call_donurbs, &data);
}

