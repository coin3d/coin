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
  \class SoIndexedNurbsSurface SoIndexedNurbsSurface.h Inventor/nodes/SoIndexedNurbsSurface.h
  \brief The SoIndexedNurbsSurface class can be used to render NURBS surfaces.
  \ingroup nodes

  It is very similar to the SoNurbsSurface class, but controlpoints
  can be specified using indices.
*/
// FIXME: more class doc. Usage example! 20011220 mortene.

#include <Inventor/nodes/SoIndexedNurbsSurface.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/elements/SoCoordinateElement.h>
#include <Inventor/elements/SoDrawStyleElement.h>
#include <Inventor/bundles/SoMaterialBundle.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/elements/SoPickStyleElement.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/misc/SoGL.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/elements/SoComplexityTypeElement.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/system/gl.h>
#include <../glue/GLUWrapper.h>

#include <coindefs.h> // COIN_OBSOLETED()

/*!
  \var SoSFInt32 SoIndexedNurbsSurface::numUControlPoints
  Number of control points in the U direction.
*/
/*!
  \var SoSFInt32 SoIndexedNurbsSurface::numVControlPoints
  Number of control points in the V direction.
*/
/*!
  \var SoSFInt32 SoIndexedNurbsSurface::numSControlPoints
  Number of control points in the S direction.
*/
/*!
  \var SoSFInt32 SoIndexedNurbsSurface::numTControlPoints
  Number of control points in the T direction.
*/
/*!
  \var SoMFFloat SoIndexedNurbsSurface::uKnotVector
  The Bezier knot vector for the U direction.
*/
/*!
  \var SoMFFloat SoIndexedNurbsSurface::vKnotVector
  The Bezier knot vector for the V direction.
*/
/*!
  \var SoMFFloat SoIndexedNurbsSurface::sKnotVector
  The Bezier knot vector for the S direction.
*/
/*!
  \var SoMFFloat SoIndexedNurbsSurface::tKnotVector
  The Bezier knot vector for the T direction.
*/

/*!
  \var SoMFInt32 SoIndexedNurbsSurface::coordIndex
  The coordinate control point indices.
*/
/*!
  \var SoMFInt32 SoIndexedNurbsSurface::textureCoordIndex
  The texture coordinate control point indices.
*/

// *************************************************************************

class SoIndexedNurbsSurfaceP {
public: 
  SoIndexedNurbsSurfaceP(SoIndexedNurbsSurface * m)
  {
    this->owner = m;
    this->nurbsrenderer = NULL;
  }

  ~SoIndexedNurbsSurfaceP()
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
  SoIndexedNurbsSurface * owner;
};

#undef PRIVATE
#define PRIVATE(p) (p->pimpl)
#undef PUBLIC
#define PUBLIC(p) (p->owner)

// *************************************************************************

SO_NODE_SOURCE(SoIndexedNurbsSurface);

/*!
  Constructor.
*/
SoIndexedNurbsSurface::SoIndexedNurbsSurface()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoIndexedNurbsSurface);

  SO_NODE_ADD_FIELD(numUControlPoints, (0));
  SO_NODE_ADD_FIELD(numVControlPoints, (0));
  SO_NODE_ADD_FIELD(coordIndex, (0));
  SO_NODE_ADD_FIELD(uKnotVector, (0));
  SO_NODE_ADD_FIELD(vKnotVector, (0));
  SO_NODE_ADD_FIELD(numSControlPoints, (0));
  SO_NODE_ADD_FIELD(numTControlPoints, (0));
  SO_NODE_ADD_FIELD(textureCoordIndex, (-1));
  SO_NODE_ADD_FIELD(sKnotVector, (0));
  SO_NODE_ADD_FIELD(tKnotVector, (0));

  PRIVATE(this) = new SoIndexedNurbsSurfaceP(this);
}

/*!
  Destructor.
*/
SoIndexedNurbsSurface::~SoIndexedNurbsSurface()
{
  delete PRIVATE(this);
}

// doc from parent
void
SoIndexedNurbsSurface::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoIndexedNurbsSurface, SO_FROM_INVENTOR_1);
}

/*!
  Calculates the bounding box of all control points and sets the center
  to the average of these points.
*/
void
SoIndexedNurbsSurface::computeBBox(SoAction * action,
                                   SbBox3f & box, SbVec3f & center)
{
  SoState * state = action->getState();
  const SoCoordinateElement * coordelem =
    SoCoordinateElement::getInstance(state);

  const int num = this->coordIndex.getNum();
  const int32_t * idxptr = this->coordIndex.getValues(0);

  box.makeEmpty();
  SbVec3f acccenter(0.0f, 0.0f, 0.0f);
  SbVec3f tmp3D;

  if (coordelem->is3D()) {
    const SbVec3f * coords = coordelem->getArrayPtr3();
    assert(coords);
    for (int i = 0; i < num; i++) {
      tmp3D = coords[idxptr[i]];
      box.extendBy(tmp3D);
      acccenter += tmp3D;
    }
  }
  else {
    const SbVec4f * coords = coordelem->getArrayPtr4();
    assert(coords);
    for (int i = 0; i< num; i++) {
      coords[idxptr[i]].getReal(tmp3D);
      box.extendBy(tmp3D);
      acccenter += tmp3D;
    }
  }
  if (num) center = acccenter / float(num);
}

// Doc in superclass.
void
SoIndexedNurbsSurface::GLRender(SoGLRenderAction * action)
{
  if (!this->shouldGLRender(action)) return;

  // initialize current material
  SoMaterialBundle mb(action);
  mb.sendFirst();

  glEnable(GL_AUTO_NORMAL);
  PRIVATE(this)->doNurbs(action, TRUE);
  glDisable(GL_AUTO_NORMAL);

  SoState * state = action->getState();
  if (SoComplexityTypeElement::get(state) == SoComplexityTypeElement::OBJECT_SPACE) {
    SoGLCacheContextElement::shouldAutoCache(state,
                                             SoGLCacheContextElement::DO_AUTO_CACHE);
  }
}

// Doc in superclass.
void
SoIndexedNurbsSurface::rayPick(SoRayPickAction * action)
{
  if (GLUWrapper()->versionMatchesAtLeast(1, 3, 0)) {
    SoShape::rayPick(action); // do normal generatePrimitives() pick
  }
  else {
    if (!this->shouldRayPick(action)) return;
    static SbBool firstpick = TRUE;
    if (firstpick) {
      firstpick = FALSE;
      SoDebugError::postWarning("SoIndexedNurbsSurface::rayPick",
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

//
// used only for GLU callbacks
//
typedef struct {
  SoAction * action;
  SoIndexedNurbsSurface * thisp;
  SoPrimitiveVertex vertex;
} coin_ins_cbdata;


// Documented in superclass.
void
SoIndexedNurbsSurface::getPrimitiveCount(SoGetPrimitiveCountAction * action)
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
SoIndexedNurbsSurface::sendPrimitive(SoAction * ,  SoPrimitiveVertex *)
{
  COIN_OBSOLETED();
}

// Documented in superclass.
void
SoIndexedNurbsSurface::generatePrimitives(SoAction * action)
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
    PRIVATE(this)->doNurbsWrapper(action);
  }
}

// Documented in superclass.
SoDetail *
SoIndexedNurbsSurface::createTriangleDetail(SoRayPickAction * /* action */,
                                            const SoPrimitiveVertex * /*v1*/,
                                            const SoPrimitiveVertex * /*v2*/,
                                            const SoPrimitiveVertex * /*v3*/,
                                            SoPickedPoint * /* pp */)
{
  return NULL;
}


void
SoIndexedNurbsSurfaceP::doNurbs(SoAction * action, const SbBool glrender)
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

  if (!PUBLIC(this)->coordIndex.getNum()) return;

  if (this->nurbsrenderer == NULL) {
    this->nurbsrenderer = GLUWrapper()->gluNewNurbsRenderer();

    if (GLUWrapper()->versionMatchesAtLeast(1, 3, 0)) {
      GLUWrapper()->gluNurbsCallback(this->nurbsrenderer, (GLenum) GLU_NURBS_BEGIN_DATA, (gluNurbsCallback_cb_t)SoIndexedNurbsSurfaceP::tessBegin);
      GLUWrapper()->gluNurbsCallback(this->nurbsrenderer, (GLenum) GLU_NURBS_TEXTURE_COORD_DATA, (gluNurbsCallback_cb_t)SoIndexedNurbsSurfaceP::tessTexCoord);
      GLUWrapper()->gluNurbsCallback(this->nurbsrenderer, (GLenum) GLU_NURBS_NORMAL_DATA, (gluNurbsCallback_cb_t)SoIndexedNurbsSurfaceP::tessNormal);
      GLUWrapper()->gluNurbsCallback(this->nurbsrenderer, (GLenum) GLU_NURBS_VERTEX_DATA, (gluNurbsCallback_cb_t)SoIndexedNurbsSurfaceP::tessVertex);
      GLUWrapper()->gluNurbsCallback(this->nurbsrenderer, (GLenum) GLU_NURBS_END_DATA, (gluNurbsCallback_cb_t)SoIndexedNurbsSurfaceP::tessEnd);
    }
  }

  // NB, don't move this structure inside the if-statement. It needs
  // to be here so that the callbacks from sogl_render_nurbs_surface()
  // have a valid pointer to the structure.
  coin_ins_cbdata cbdata;

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

  SbBool texindex =
    PUBLIC(this)->textureCoordIndex.getNum() &&
    PUBLIC(this)->textureCoordIndex[0] >= 0;

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
                            glrender,
                            PUBLIC(this)->coordIndex.getNum(),
                            PUBLIC(this)->coordIndex.getValues(0),
                            texindex ? PUBLIC(this)->textureCoordIndex.getNum() : 0,
                            texindex ? PUBLIC(this)->textureCoordIndex.getValues(0) : NULL);
}

void APIENTRY
SoIndexedNurbsSurfaceP::tessBegin(int type, void * data)
{
  coin_ins_cbdata * cbdata = (coin_ins_cbdata*) data;
  SoIndexedNurbsSurface::TriangleShape shapetype;
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
#if COIN_DEBUG
    SoDebugError::postInfo("SoIndexedNurbsSurfaceP::tessBegin",
                           "unsupported GL enum: 0x%x", type);
#endif // COIN_DEBUG
    break;
  }
  if (shapetype != SoShape::POINTS) {
    cbdata->thisp->beginShape(cbdata->action, shapetype, NULL);
  }
}

void APIENTRY
SoIndexedNurbsSurfaceP::tessTexCoord(float * texcoord, void * data)
{
  coin_ins_cbdata * cbdata = (coin_ins_cbdata*) data;
  cbdata->vertex.setTextureCoords(SbVec2f(texcoord[0], texcoord[1]));
}

void APIENTRY
SoIndexedNurbsSurfaceP::tessNormal(float * normal, void * data)
{
  coin_ins_cbdata * cbdata = (coin_ins_cbdata*) data;
  cbdata->vertex.setNormal(SbVec3f(normal[0], normal[1], normal[2]));
}

void APIENTRY
SoIndexedNurbsSurfaceP::tessVertex(float * vertex, void * data)
{
  coin_ins_cbdata * cbdata = (coin_ins_cbdata*) data;
  cbdata->vertex.setPoint(SbVec3f(vertex[0], vertex[1], vertex[2]));
  cbdata->thisp->shapeVertex(&cbdata->vertex);
}

void APIENTRY
SoIndexedNurbsSurfaceP::tessEnd(void * data)
{
  coin_ins_cbdata * cbdata = (coin_ins_cbdata*) data;
  cbdata->thisp->endShape();
}

typedef struct {
  SoIndexedNurbsSurfaceP * thisp;
  SoAction * action;
} soidxnurbssurface_call_donurbs_data;

static void call_donurbs(void * userdata, SoAction * action)
{
  soidxnurbssurface_call_donurbs_data * data = 
    (soidxnurbssurface_call_donurbs_data*) userdata;
  data->thisp->doNurbs(data->action, FALSE);
}

void 
SoIndexedNurbsSurfaceP::doNurbsWrapper(SoAction * action)
{
  soidxnurbssurface_call_donurbs_data data;
  data.thisp = this;
  data.action = action;
  sogl_offscreencontext_callback(call_donurbs, &data);
}
