/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
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
  \class SoPointSet SoPointSet.h Inventor/nodes/SoPointSet.h
  \brief The SoPointSet class is used to handle a set of points.
  \ingroup nodes

  This node uses the coordinates currently on the state (or in the
  vertexProperty field) in order. The numPoints field specifies the
  number of points in the set.
*/

#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/nodes/SoSubNodeP.h>

#include <Inventor/misc/SoState.h>
#include <Inventor/bundles/SoTextureCoordinateBundle.h>
#include <Inventor/SoPrimitiveVertex.h>

#include <Inventor/actions/SoGLRenderAction.h>
#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H
#ifdef HAVE_WINDOWS_H
#include <windows.h> // Needed to include gl.h.
#endif // HAVE_WINDOWS_H
#include <GL/gl.h>

#include <Inventor/nodes/SoVertexProperty.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/elements/SoGLCoordinateElement.h>
#include <Inventor/elements/SoNormalBindingElement.h>
#include <Inventor/elements/SoMaterialBindingElement.h>
#include <Inventor/bundles/SoMaterialBundle.h>
#include <Inventor/elements/SoGLLightModelElement.h>
#include <Inventor/caches/SoNormalCache.h>
#include <Inventor/details/SoPointDetail.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

/*!
  \var SoSFInt32 SoPointSet::numPoints
  Used to specify number of points in the point set. If this field is set
  to -1 (the default value) all coordinates currently on the state will be
  drawn/handled. Please note that PointSet inherits the field
  SoNonIndexedShape::startIndex, which spedifies the start index for
  points in this point set. This field is obsolete (do no use it!), but
  is provided for compatibility. However, if startIndex != 0, the point
  set will start handling coordinates from that index on.
*/

SO_NODE_SOURCE(SoPointSet);

/*!
  Constructor.
*/
SoPointSet::SoPointSet()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoPointSet);

  SO_NODE_ADD_FIELD(numPoints, (-1));
}

/*!
  Destructor.
*/
SoPointSet::~SoPointSet()
{
}

// doc from parent
void
SoPointSet::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoPointSet);
}

// doc from parent
void
SoPointSet::computeBBox(SoAction * action, SbBox3f & box, SbVec3f & center)
{
  inherited::computeCoordBBox(action, this->numPoints.getValue(), box, center);
}

// Internal method which translates the current material binding
// found on the state to a material binding for this node.
// PER_PART, PER_FACE, PER_VERTEX and their indexed counterparts
// are translated to PER_VERTEX binding. OVERALL means overall
// binding for point set also, of course. The default material
// binding is OVERALL.
SoPointSet::Binding
SoPointSet::findMaterialBinding(SoState * const state) const
{
  Binding binding = OVERALL;
  if (SoMaterialBindingElement::get(state) !=
      SoMaterialBindingElement::OVERALL) binding = PER_VERTEX;
  return binding;
}

//  Internal method which translates the current normal binding
//  found on the state to a normal binding for this node.
//  PER_PART, PER_FACE, PER_VERTEX and their indexed counterparts
//  are translated to PER_VERTEX binding. OVERALL means overall
//  binding for point set also, of course. The default normal
//  binding is PER_VERTEX.
SoPointSet::Binding
SoPointSet::findNormalBinding(SoState * const state) const

{
  Binding binding = PER_VERTEX;

  if (SoNormalBindingElement::get(state) ==
      SoNormalBindingElement::OVERALL) binding = OVERALL;
  return binding;
}


// here we include the 8 variations directly...

void sogl_render_pointset_m0n0t0(const SoGLCoordinateElement * coords,
				 const SbVec3f *normals,
				 SoMaterialBundle * mb,
				 const SoTextureCoordinateBundle * tb,
				 int32_t numpts,
				 int32_t idx)
{
  int i;
  const int unroll = numpts >> 2;
  const int rest = numpts & 3;

  glBegin(GL_POINTS);
  for (i = 0; i < unroll; i++) {
    coords->send(idx++);
    coords->send(idx++);
    coords->send(idx++);
    coords->send(idx++);
  }
  for (i = 0; i < rest; i++) {
    coords->send(idx++);
  }
  glEnd();
}

void sogl_render_pointset_m0n0t1(const SoGLCoordinateElement * coords,
				 const SbVec3f *normals,
				 SoMaterialBundle * mb,
				 const SoTextureCoordinateBundle * tb,
				 int32_t numpts,
				 int32_t idx)
{
  int texnr = 0;
  const SbVec3f currnormal(0.0f,0.0f,1.0f);

  glBegin(GL_POINTS);
  for (int i = 0; i < numpts; i++) {
    tb->send(texnr++, coords->get3(idx), currnormal);
    coords->send(idx++);
  }
  glEnd();
}

void sogl_render_pointset_m0n1t0(const SoGLCoordinateElement * coords,
				 const SbVec3f *normals,
				 SoMaterialBundle * mb,
				 const SoTextureCoordinateBundle * tb,
				 int32_t numpts,
				 int32_t idx)
{
  glBegin(GL_POINTS);
  for (int i = 0; i < numpts; i++) {
    glNormal3fv((const GLfloat*)normals++);
    coords->send(idx++);
  }
  glEnd();
}

void sogl_render_pointset_m0n1t1(const SoGLCoordinateElement * coords,
				 const SbVec3f *normals,
				 SoMaterialBundle * mb,
				 const SoTextureCoordinateBundle * tb,
				 int32_t numpts,
				 int32_t idx)
{
  int texnr = 0;
  const SbVec3f currnormal(0.0f,0.0f,1.0f);

  glBegin(GL_POINTS);
  for (int i = 0; i < numpts; i++) {
    glNormal3fv((const GLfloat*)normals++);
    tb->send(texnr++, coords->get3(idx), currnormal);
    coords->send(idx++);
  }
  glEnd();
}

void sogl_render_pointset_m1n0t0(const SoGLCoordinateElement * coords,
				 const SbVec3f *normals,
				 SoMaterialBundle * mb,
				 const SoTextureCoordinateBundle * tb,
				 int32_t numpts,
				 int32_t idx)
{
  int i;
  int matnr = 0;
  const int unroll = numpts >> 2;
  const int rest = numpts & 3;

  glBegin(GL_POINTS);
  for (i = 0; i < unroll; i++) {
    mb->send(matnr++, TRUE);
    coords->send(idx++);
    mb->send(matnr++, TRUE);
    coords->send(idx++);
    mb->send(matnr++, TRUE);
    coords->send(idx++);
    mb->send(matnr++, TRUE);
    coords->send(idx++);
  }
  for (i = 0; i < rest; i++) {
    mb->send(matnr++, TRUE);
    coords->send(idx++);
  }
  glEnd();
}

void sogl_render_pointset_m1n0t1(const SoGLCoordinateElement * coords,
				 const SbVec3f *normals,
				 SoMaterialBundle * mb,
				 const SoTextureCoordinateBundle * tb,
				 int32_t numpts,
				 int32_t idx)
{
  int matnr = 0;
  int texnr = 0;
  const SbVec3f currnormal(0.0f,0.0f,1.0f);

  glBegin(GL_POINTS);
  for (int i = 0; i < numpts; i++) {
    mb->send(matnr++, TRUE);
    tb->send(texnr++, coords->get3(idx), currnormal);
    coords->send(idx++);
  }
  glEnd();
}

void sogl_render_pointset_m1n1t0(const SoGLCoordinateElement * coords,
				 const SbVec3f *normals,
				 SoMaterialBundle * mb,
				 const SoTextureCoordinateBundle * tb,
				 int32_t numpts,
				 int32_t idx)
{
  int matnr = 0;

  glBegin(GL_POINTS);
  for (int i = 0; i < numpts; i++) {
    mb->send(matnr++, TRUE);
    glNormal3fv((const GLfloat*)normals++);
    coords->send(idx++);
  }
  glEnd();
}

void sogl_render_pointset_m1n1t1(const SoGLCoordinateElement * coords,
				 const SbVec3f *normals,
				 SoMaterialBundle * mb,
				 const SoTextureCoordinateBundle * tb,
				 int32_t numpts,
				 int32_t idx)
{
  int matnr = 0;
  int texnr = 0;
  const SbVec3f currnormal(0.0f,0.0f,1.0f);

  glBegin(GL_POINTS);
  for (int i = 0; i < numpts; i++) {
    glNormal3fv((const GLfloat*)normals++);
    tb->send(texnr++, coords->get3(idx), currnormal);
    coords->send(idx++);
  }
  glEnd();
}

// ---

typedef void sogl_render_pointset_func(const SoGLCoordinateElement * coords,
					 const SbVec3f *normals,
					 SoMaterialBundle * mb,
					 const SoTextureCoordinateBundle * tb,
					 int32_t numpts,
					 int32_t idx);

static sogl_render_pointset_func *sogl_render_pointset_funcs[8];

// doc from parent
void
SoPointSet::GLRender(SoGLRenderAction * action)
{
  static int first = 1;
  if (first) {
    first = 0;
    sogl_render_pointset_funcs[0] = sogl_render_pointset_m0n0t0;
    sogl_render_pointset_funcs[1] = sogl_render_pointset_m0n0t1;
    sogl_render_pointset_funcs[2] = sogl_render_pointset_m0n1t0;
    sogl_render_pointset_funcs[3] = sogl_render_pointset_m0n1t1;
    sogl_render_pointset_funcs[4] = sogl_render_pointset_m1n0t0;
    sogl_render_pointset_funcs[5] = sogl_render_pointset_m1n0t1;
    sogl_render_pointset_funcs[6] = sogl_render_pointset_m1n1t0;
    sogl_render_pointset_funcs[7] = sogl_render_pointset_m1n1t1;
  }

  SoState * state = action->getState();

  SbBool didpush = FALSE;
  if (this->vertexProperty.getValue()) {
    state->push();
    didpush = TRUE;
    this->vertexProperty.getValue()->GLRender(action);
  }

  const SoCoordinateElement * tmp;
  const SbVec3f * normals;
  SbBool needNormals =
    (SoLightModelElement::get(state) !=
     SoLightModelElement::BASE_COLOR);

  SoVertexShape::getVertexData(state, tmp, normals,
                               needNormals);

  if (normals == NULL && needNormals) {
    needNormals = FALSE;
    if (!didpush) {
      state->push();
      didpush = TRUE;
    }
    SoLightModelElement::set(state, SoLightModelElement::BASE_COLOR);
  }

  if (!this->shouldGLRender(action)) {
    if (didpush)
      state->pop();
    return;
  }

  SbBool doTextures;

  const SoGLCoordinateElement * coords = (SoGLCoordinateElement *)tmp;

  SoTextureCoordinateBundle tb(action, TRUE, FALSE);
  doTextures = tb.needCoordinates();

  Binding mbind = this->findMaterialBinding(action->getState());

  Binding nbind = OVERALL;
  if (needNormals) nbind = this->findNormalBinding(action->getState());

  if (nbind == OVERALL && needNormals) {
    if (normals) glNormal3fv((const GLfloat *)normals);
    else glNormal3f(0.0f, 0.0f, 1.0f);
  }

  SoMaterialBundle mb(action);
  mb.sendFirst(); // make sure we have the correct material

  int32_t idx = this->startIndex.getValue();
  int32_t numpts = this->numPoints.getValue();
  if (numpts < 0) numpts = coords->getNum() - idx;

  sogl_render_pointset_funcs[ (mbind << 2) | (nbind << 1) | doTextures ]
    ( coords,
      normals,
      &mb,
      &tb,
      numpts,
      idx);

#if 0 // obsoleted 2001-1-19, skei

  glBegin(GL_POINTS);
  for (int i = 0; i < numpts; i++) {
    if (nbind == PER_VERTEX) {
      currnormal = normals++;
      glNormal3fv((const GLfloat *)currnormal);
    }
    if (mbind == PER_VERTEX) mb.send(matnr++, TRUE);
    if (doTextures) tb.send(texnr++, coords->get3(idx), *currnormal);
    coords->send(idx++);
  }
  glEnd();

#endif // obsoleted

  if (didpush)
    state->pop();
}

/*!
  Overloaded to clear normal cache.
*/
SbBool
SoPointSet::generateDefaultNormals(SoState *, SoNormalCache * nc)
{
  // not possible to generate normals for PointSet
  nc->set(0, NULL);
  return TRUE;
}

/*!
  Overloaded to avoid compiler warnings.
*/
SbBool
SoPointSet::generateDefaultNormals(SoState * state,
                                   SoNormalBundle * bundle)
{
  return FALSE;
}

// doc from parent
void
SoPointSet::getBoundingBox(SoGetBoundingBoxAction *action)
{
  inherited::getBoundingBox(action);
}

// doc from parent
void
SoPointSet::getPrimitiveCount(SoGetPrimitiveCountAction *action)
{
  if (!this->shouldPrimitiveCount(action)) return;
  int num = this->numPoints.getValue();
  if (num < 0) {
    SoVertexProperty *vp = (SoVertexProperty*) this->vertexProperty.getValue();
    if (vp && vp->vertex.getNum()) {
      num = vp->vertex.getNum() - this->startIndex.getValue();
    }
    else {
      const SoCoordinateElement *coordelem =
        SoCoordinateElement::getInstance(action->getState());
      num = coordelem->getNum() - this->startIndex.getValue();
    }
  }
  action->addNumPoints(num);
}

// doc from parent
void
SoPointSet::generatePrimitives(SoAction *action)
{
  SoState * state = action->getState();

  if (this->vertexProperty.getValue()) {
    state->push();
    this->vertexProperty.getValue()->doAction(action);
  }

  const SoCoordinateElement *coords;
  const SbVec3f * normals;
  SbBool doTextures;
  SbBool needNormals = TRUE;

  SoVertexShape::getVertexData(action->getState(), coords, normals,
                               needNormals);

  if (normals == NULL) needNormals = FALSE;

  SoTextureCoordinateBundle tb(action, FALSE, FALSE);
  doTextures = tb.needCoordinates();

  Binding mbind = this->findMaterialBinding(action->getState());
  Binding nbind = this->findNormalBinding(action->getState());

  if (!needNormals) nbind = OVERALL;

  SoPrimitiveVertex vertex;
  SoPointDetail pointDetail;
  vertex.setDetail(&pointDetail);

  SbVec3f dummynormal(0.0f, 0.0f, 1.0f);
  const SbVec3f * currnormal = &dummynormal;
  if (normals) currnormal = normals;
  if (nbind == OVERALL && needNormals)
    vertex.setNormal(*currnormal);

  int32_t idx = this->startIndex.getValue();
  int32_t numpts = this->numPoints.getValue();
  if (numpts < 0) numpts = coords->getNum() - idx;

  int matnr = 0;
  int texnr = 0;
  int normnr = 0;

  this->beginShape(action, SoShape::POINTS);
  for (int i = 0; i < numpts; i++) {
    if (nbind == PER_VERTEX) {
      pointDetail.setNormalIndex(normnr);
      currnormal = &normals[normnr++];
      vertex.setNormal(*currnormal);
    }
    if (mbind == PER_VERTEX) {
      pointDetail.setMaterialIndex(matnr);
      vertex.setMaterialIndex(matnr++);
    }
    if (doTextures) {
      if (tb.isFunction()) {
        vertex.setTextureCoords(tb.get(coords->get3(idx), *currnormal));
      }
      else {
        pointDetail.setTextureCoordIndex(texnr);
        vertex.setTextureCoords(tb.get(texnr++));
      }
    }
    pointDetail.setCoordinateIndex(idx);
    vertex.setPoint(coords->get3(idx++));
    this->shapeVertex(&vertex);
  }
  this->endShape();

  if (this->vertexProperty.getValue())
    state->pop();
}
