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
  \class SoQuadMesh SoQuadMesh.h Inventor/nodes/SoQuadMesh.h
  \brief The SoQuadMesh class is used to render and optimize a quadrilateral mesh.
  \ingroup nodes

  This node uses the coordinates in order, either from the state or
  from the vertexProperty node, to construct a quadrilateral mesh. The
  quads are generated in row major order, using the two field
  verticesPerColumn and verticesPerRow to specify the mesh. E.g. if
  verticesPerColumn is 3 and verticesPerRow is 2, two quads will be
  generated, the first one using (in order) coordinates 0, 1, 3 and 2,
  the second one using coordinates 2, 3, 5 and 4 (you get three rows
  of vertices; the first row uses vertices 0 and 1, the second row 2
  and 3, and the third row 4 and 5).

  Normals and materials can be bound PER_PART (per row), PER_FACE,
  PER_VERTEX and OVERALL. The default material binding is OVERALL. The
  default normal binding is PER_VERTEX.
*/

#include <Inventor/nodes/SoQuadMesh.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/misc/SoState.h>

#include <Inventor/actions/SoGLRenderAction.h>
#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H
#ifdef HAVE_WINDOWS_H
#include <windows.h> // Needed to include gl.h.
#endif // HAVE_WINDOWS_H
#include <GL/gl.h>

#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/elements/SoLightModelElement.h>
#include <Inventor/elements/SoGLCoordinateElement.h>
#include <Inventor/elements/SoNormalBindingElement.h>
#include <Inventor/elements/SoMaterialBindingElement.h>
#include <Inventor/bundles/SoMaterialBundle.h>
#include <Inventor/elements/SoShapeHintsElement.h>
#include <Inventor/caches/SoNormalCache.h>
#include <Inventor/bundles/SoTextureCoordinateBundle.h>
#include <GL/gl.h>

#include <Inventor/details/SoFaceDetail.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif

/*!
  \var SoSFInt32 SoQuadMesh::verticesPerColumn
  Specifies to number of vertices in each column.
*/
/*!
  \var SoSFInt32 SoQuadMesh::verticesPerRow
  Specifies the number of vertices in each row.
*/

SO_NODE_SOURCE(SoQuadMesh);

/*!
  Constructor.
*/
SoQuadMesh::SoQuadMesh()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoQuadMesh);

  SO_NODE_ADD_FIELD(verticesPerColumn, (1));
  SO_NODE_ADD_FIELD(verticesPerRow, (1));
}

/*!
  Destructor.
*/
SoQuadMesh::~SoQuadMesh()
{
}

// doc from parent
void
SoQuadMesh::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoQuadMesh);
}

// doc from parent
void
SoQuadMesh::computeBBox(SoAction * action, SbBox3f & box, SbVec3f & center)
{
  inherited::computeCoordBBox(action,
                              this->verticesPerRow.getValue() *
                              this->verticesPerColumn.getValue(),
                              box, center);
}

//
// translates current material binding to the internal Binding enum.
//
SoQuadMesh::Binding
SoQuadMesh::findMaterialBinding(SoState * const state) const
{
  SoMaterialBindingElement::Binding matbind =
    SoMaterialBindingElement::get(state);

  Binding binding;
  switch (matbind) {
  case SoMaterialBindingElement::OVERALL:
    binding = OVERALL;
    break;
  case SoMaterialBindingElement::PER_VERTEX:
  case SoMaterialBindingElement::PER_VERTEX_INDEXED:
    binding = PER_VERTEX;
    break;
  case SoMaterialBindingElement::PER_PART:
  case SoMaterialBindingElement::PER_PART_INDEXED:
    binding = PER_ROW;
    break;
  case SoMaterialBindingElement::PER_FACE:
  case SoMaterialBindingElement::PER_FACE_INDEXED:
    binding = PER_FACE;
    break;
  default:
    binding = OVERALL;
#if COIN_DEBUG
    SoDebugError::postWarning("SoQuadMesh::findMaterialBinding",
                              "unknown material binding setting");
#endif // COIN_DEBUG
    break;
  }
  return binding;
}


//
// translates current normal binding to the internal Binding enum.
//
SoQuadMesh::Binding
SoQuadMesh::findNormalBinding(SoState * const state) const
{
  SoNormalBindingElement::Binding normbind =
    SoNormalBindingElement::get(state);

  Binding binding;
  switch (normbind) {
  case SoMaterialBindingElement::OVERALL:
    binding = OVERALL;
    break;
  case SoMaterialBindingElement::PER_VERTEX:
  case SoMaterialBindingElement::PER_VERTEX_INDEXED:
    binding = PER_VERTEX;
    break;
  case SoMaterialBindingElement::PER_PART:
  case SoMaterialBindingElement::PER_PART_INDEXED:
    binding = PER_ROW;
    break;
  case SoMaterialBindingElement::PER_FACE:
  case SoMaterialBindingElement::PER_FACE_INDEXED:
    binding = PER_FACE;
    break;
  default:
    binding = PER_VERTEX;
#if COIN_DEBUG
    SoDebugError::postWarning("SoQuadMesh::findNormalBinding",
                              "unknown normal binding setting");
#endif // COIN_DEBUG
    break;
  }
  return binding;
}

typedef void sogl_render_qmesh_func( const SoGLCoordinateElement * coords,
    const SbVec3f *normals,
    SoMaterialBundle * mb,
    const SoTextureCoordinateBundle * tb,
    SbBool needNormals,
    int rowsize,
    int colsize,
    int start );

static sogl_render_qmesh_func *soquadmesh_ni_render_funcs[ 32 ];

#define OVERALL       0
#define PER_ROW       1
#define PER_FACE      2
#define PER_VERTEX    3

#define IDX(r,c) ((r)*rowsize+(c))

// -----

// n0

#define MBINDING OVERALL
#define NBINDING OVERALL
#define TEXTURES FALSE
static void sogl_qmesh_m0_n0_t0
#include "../misc/SoGLqmeshTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_ROW
#define NBINDING OVERALL
#define TEXTURES FALSE
static void sogl_qmesh_m1_n0_t0
#include "../misc/SoGLqmeshTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_FACE
#define NBINDING OVERALL
#define TEXTURES FALSE
static void sogl_qmesh_m2_n0_t0
#include "../misc/SoGLqmeshTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX
#define NBINDING OVERALL
#define TEXTURES FALSE
static void sogl_qmesh_m3_n0_t0
#include "../misc/SoGLqmeshTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

// -----

// n1

#define MBINDING OVERALL
#define NBINDING PER_ROW
#define TEXTURES FALSE
static void sogl_qmesh_m0_n1_t0
#include "../misc/SoGLqmeshTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_ROW
#define NBINDING PER_ROW
#define TEXTURES FALSE
static void sogl_qmesh_m1_n1_t0
#include "../misc/SoGLqmeshTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_FACE
#define NBINDING PER_ROW
#define TEXTURES FALSE
static void sogl_qmesh_m2_n1_t0
#include "../misc/SoGLqmeshTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX
#define NBINDING PER_ROW
#define TEXTURES FALSE
static void sogl_qmesh_m3_n1_t0
#include "../misc/SoGLqmeshTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

// -----

// n2

#define MBINDING OVERALL
#define NBINDING PER_FACE
#define TEXTURES FALSE
static void sogl_qmesh_m0_n2_t0
#include "../misc/SoGLqmeshTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_ROW
#define NBINDING PER_FACE
#define TEXTURES FALSE
static void sogl_qmesh_m1_n2_t0
#include "../misc/SoGLqmeshTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_FACE
#define NBINDING PER_FACE
#define TEXTURES FALSE
static void sogl_qmesh_m2_n2_t0
#include "../misc/SoGLqmeshTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX
#define NBINDING PER_FACE
#define TEXTURES FALSE
static void sogl_qmesh_m3_n2_t0
#include "../misc/SoGLqmeshTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

// -----

// n3

#define MBINDING OVERALL
#define NBINDING PER_VERTEX
#define TEXTURES FALSE
static void sogl_qmesh_m0_n3_t0
#include "../misc/SoGLqmeshTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_ROW
#define NBINDING PER_VERTEX
#define TEXTURES FALSE
static void sogl_qmesh_m1_n3_t0
#include "../misc/SoGLqmeshTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_FACE
#define NBINDING PER_VERTEX
#define TEXTURES FALSE
static void sogl_qmesh_m2_n3_t0
#include "../misc/SoGLqmeshTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX
#define NBINDING PER_VERTEX
#define TEXTURES FALSE
static void sogl_qmesh_m3_n3_t0
#include "../misc/SoGLqmeshTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

// og de samme, med tekstur

// -----

// n0

#define MBINDING OVERALL
#define NBINDING OVERALL
#define TEXTURES TRUE
static void sogl_qmesh_m0_n0_t1
#include "../misc/SoGLqmeshTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_ROW
#define NBINDING OVERALL
#define TEXTURES TRUE
static void sogl_qmesh_m1_n0_t1
#include "../misc/SoGLqmeshTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_FACE
#define NBINDING OVERALL
#define TEXTURES TRUE
static void sogl_qmesh_m2_n0_t1
#include "../misc/SoGLqmeshTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX
#define NBINDING OVERALL
#define TEXTURES TRUE
static void sogl_qmesh_m3_n0_t1
#include "../misc/SoGLqmeshTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

// -----

// n1

#define MBINDING OVERALL
#define NBINDING PER_ROW
#define TEXTURES TRUE
static void sogl_qmesh_m0_n1_t1
#include "../misc/SoGLqmeshTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_ROW
#define NBINDING PER_ROW
#define TEXTURES TRUE
static void sogl_qmesh_m1_n1_t1
#include "../misc/SoGLqmeshTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_FACE
#define NBINDING PER_ROW
#define TEXTURES TRUE
static void sogl_qmesh_m2_n1_t1
#include "../misc/SoGLqmeshTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX
#define NBINDING PER_ROW
#define TEXTURES TRUE
static void sogl_qmesh_m3_n1_t1
#include "../misc/SoGLqmeshTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

// -----

// n2

#define MBINDING OVERALL
#define NBINDING PER_FACE
#define TEXTURES TRUE
static void sogl_qmesh_m0_n2_t1
#include "../misc/SoGLqmeshTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_ROW
#define NBINDING PER_FACE
#define TEXTURES TRUE
static void sogl_qmesh_m1_n2_t1
#include "../misc/SoGLqmeshTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_FACE
#define NBINDING PER_FACE
#define TEXTURES TRUE
static void sogl_qmesh_m2_n2_t1
#include "../misc/SoGLqmeshTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX
#define NBINDING PER_FACE
#define TEXTURES TRUE
static void sogl_qmesh_m3_n2_t1
#include "../misc/SoGLqmeshTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

// -----

// n3

#define MBINDING OVERALL
#define NBINDING PER_VERTEX
#define TEXTURES TRUE
static void sogl_qmesh_m0_n3_t1
#include "../misc/SoGLqmeshTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_ROW
#define NBINDING PER_VERTEX
#define TEXTURES TRUE
static void sogl_qmesh_m1_n3_t1
#include "../misc/SoGLqmeshTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_FACE
#define NBINDING PER_VERTEX
#define TEXTURES TRUE
static void sogl_qmesh_m2_n3_t1
#include "../misc/SoGLqmeshTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX
#define NBINDING PER_VERTEX
#define TEXTURES TRUE
static void sogl_qmesh_m3_n3_t1
#include "../misc/SoGLqmeshTemplate.cpp"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

// -----

#undef IDX

#undef OVERALL
#undef PER_ROW
#undef PER_FACE
#undef PER_VERTEX

// -----

// doc from parent
void
SoQuadMesh::GLRender(SoGLRenderAction * action)
{
  static int first = 1;
  if (first) {
    first = 0;
    soquadmesh_ni_render_funcs[ 0] = sogl_qmesh_m0_n0_t0;
    soquadmesh_ni_render_funcs[ 1] = sogl_qmesh_m0_n0_t1;
    soquadmesh_ni_render_funcs[ 2] = sogl_qmesh_m0_n1_t0;
    soquadmesh_ni_render_funcs[ 3] = sogl_qmesh_m0_n1_t1;
    soquadmesh_ni_render_funcs[ 4] = sogl_qmesh_m0_n2_t0;
    soquadmesh_ni_render_funcs[ 5] = sogl_qmesh_m0_n2_t1;
    soquadmesh_ni_render_funcs[ 6] = sogl_qmesh_m0_n3_t0;
    soquadmesh_ni_render_funcs[ 7] = sogl_qmesh_m0_n3_t1;

    soquadmesh_ni_render_funcs[ 8] = sogl_qmesh_m1_n0_t0;
    soquadmesh_ni_render_funcs[ 9] = sogl_qmesh_m1_n0_t1;
    soquadmesh_ni_render_funcs[10] = sogl_qmesh_m1_n1_t0;
    soquadmesh_ni_render_funcs[11] = sogl_qmesh_m1_n1_t1;
    soquadmesh_ni_render_funcs[12] = sogl_qmesh_m1_n2_t0;
    soquadmesh_ni_render_funcs[13] = sogl_qmesh_m1_n2_t1;
    soquadmesh_ni_render_funcs[14] = sogl_qmesh_m1_n3_t0;
    soquadmesh_ni_render_funcs[15] = sogl_qmesh_m1_n3_t1;

    soquadmesh_ni_render_funcs[16] = sogl_qmesh_m2_n0_t0;
    soquadmesh_ni_render_funcs[17] = sogl_qmesh_m2_n0_t1;
    soquadmesh_ni_render_funcs[18] = sogl_qmesh_m2_n1_t0;
    soquadmesh_ni_render_funcs[19] = sogl_qmesh_m2_n1_t1;
    soquadmesh_ni_render_funcs[20] = sogl_qmesh_m2_n2_t0;
    soquadmesh_ni_render_funcs[21] = sogl_qmesh_m2_n2_t1;
    soquadmesh_ni_render_funcs[22] = sogl_qmesh_m2_n3_t0;
    soquadmesh_ni_render_funcs[23] = sogl_qmesh_m2_n3_t1;

    soquadmesh_ni_render_funcs[24] = sogl_qmesh_m3_n0_t0;
    soquadmesh_ni_render_funcs[25] = sogl_qmesh_m3_n0_t1;
    soquadmesh_ni_render_funcs[26] = sogl_qmesh_m3_n1_t0;
    soquadmesh_ni_render_funcs[27] = sogl_qmesh_m3_n1_t1;
    soquadmesh_ni_render_funcs[28] = sogl_qmesh_m3_n2_t0;
    soquadmesh_ni_render_funcs[29] = sogl_qmesh_m3_n2_t1;
    soquadmesh_ni_render_funcs[30] = sogl_qmesh_m3_n3_t0;
    soquadmesh_ni_render_funcs[31] = sogl_qmesh_m3_n3_t1;

  }

  SoState * state = action->getState();

  if (this->vertexProperty.getValue()) {
    state->push();
    this->vertexProperty.getValue()->GLRender(action);
  }

  if (!this->shouldGLRender(action)) {
    if (this->vertexProperty.getValue())
      state->pop();
    return;
  }

  const int rowsize = this->verticesPerRow.getValue();
  const int colsize = this->verticesPerColumn.getValue();

  if (rowsize < 2 || colsize < 2) {
    if (this->vertexProperty.getValue()) state->pop();
    return; // nothing to draw
  }
  const SoCoordinateElement * tmp;
  const SbVec3f * normals;
  SbBool doTextures;
  SbBool needNormals =
    (SoLightModelElement::get(state) !=
     SoLightModelElement::BASE_COLOR);

  SoVertexShape::getVertexData(action->getState(), tmp, normals,
                               needNormals);

  const SoGLCoordinateElement * coords = (SoGLCoordinateElement *)tmp;

  SoTextureCoordinateBundle tb(action, TRUE, FALSE);
  doTextures = tb.needCoordinates();

  //
  // FIXME: should I test for texture coordinate binding other
  // than PER_VERTEX_INDEXED. The normal and material bindings
  // are equal for indexed and nonindex, this is probably the
  // case for texture coordinate binding too... (pederb, 990701)
  //

  int start = this->startIndex.getValue();

  Binding mbind = findMaterialBinding(action->getState());
  Binding nbind = findNormalBinding(action->getState());

  if (!needNormals) nbind = OVERALL;

  if (needNormals && normals == NULL) {
    normals = getNormalCache()->getNormals();
  }

  SoMaterialBundle mb(action);
  mb.sendFirst(); // make sure we have the correct material

  soquadmesh_ni_render_funcs[ (mbind << 3) | (nbind << 1) | doTextures ]
    ( coords,
      normals,
      &mb,
      &tb,
      needNormals,
      rowsize,
      colsize,
      start);


#if 0 // obsoleted 2001-01-08, skei

#define IDX(r,c) ((r)*rowsize+(c))

  int midx = 0;
  for (int i = 0; i < colsize-1; i++) {
    int j = 0;
    glBegin(GL_QUAD_STRIP);
    if (nbind == PER_ROW) {
      currnormal = normals++;
      glNormal3fv((const GLfloat *)currnormal);
    }
    if (mbind == PER_ROW) mb.send(midx++,TRUE);

    for (j = 0; j < rowsize; j++) {
      curridx = IDX(i,j);
      if (nbind == PER_VERTEX) {
        currnormal = &normals[curridx];
        glNormal3fv((const GLfloat *)currnormal);
      }
      else if (nbind == PER_FACE) {
        currnormal = normals++;
        glNormal3fv((const GLfloat *)currnormal);
      }
      if (mbind == PER_VERTEX) mb.send(curridx, TRUE);
      else if (mbind == PER_FACE) mb.send(midx++, TRUE);

      if (doTextures) {
        tb.send(curridx, coords->get3(start + curridx),
                *currnormal);
      }
      coords->send(start + curridx);
      curridx = IDX(i+1,j);
      if (nbind == PER_VERTEX) {
        currnormal = &normals[curridx];
        glNormal3fv((const GLfloat *)currnormal);
      }
      if (mbind == PER_VERTEX) mb.send(curridx, TRUE);
      if (doTextures) {
        tb.send(curridx, coords->get3(start + curridx),
                *currnormal);
      }
      coords->send(start + curridx);
    }
    glEnd(); // end of strip/row
  }
#undef IDX

#endif // obsoleted

  if (this->vertexProperty.getValue())
    state->pop();
}

// doc from parent
SbBool
SoQuadMesh::generateDefaultNormals(SoState * state, SoNormalCache * nc)
{
  // FIXME: consider creaseAngle? pederb, 20000809

  if (verticesPerRow.getValue() < 2 || verticesPerColumn.getValue() < 2)
    return TRUE; // nothing to generate

  SbBool ccw = TRUE;
  if (SoShapeHintsElement::getVertexOrdering(state) ==
      SoShapeHintsElement::CLOCKWISE) ccw = FALSE;

  const SbVec3f * coords = SoCoordinateElement::getInstance(state)->getArrayPtr3();
  assert(coords);

  Binding binding = findNormalBinding(state);

  switch (binding) {
  case PER_VERTEX:
    nc->generatePerVertexQuad(coords + startIndex.getValue(),
                              verticesPerRow.getValue(),
                              verticesPerColumn.getValue(),
                              ccw);
    break;
  case PER_FACE:
    nc->generatePerFaceQuad(coords + startIndex.getValue(),
                            verticesPerRow.getValue(),
                            verticesPerColumn.getValue(),
                            ccw);
    break;
  case PER_ROW:
    nc->generatePerRowQuad(coords + startIndex.getValue(),
                           verticesPerRow.getValue(),
                           verticesPerColumn.getValue(),
                           ccw);
    break;
  case OVERALL:
    break;
  default:
    assert(0);
    return FALSE;
    break;
  }
  return TRUE;
}

// doc from parent
void
SoQuadMesh::getPrimitiveCount(SoGetPrimitiveCountAction *action)
{
  if (!this->shouldPrimitiveCount(action)) return;

  action->addNumTriangles(2 * this->verticesPerRow.getValue() *
                          this->verticesPerColumn.getValue());
}

/*!
  Overloaded to return FALSE. Normals are genereted in normal cache.
 */
SbBool
SoQuadMesh::generateDefaultNormals(SoState * /* state */, SoNormalBundle * /* nb */)
{
  return FALSE;
}

// doc from parent
void
SoQuadMesh::generatePrimitives(SoAction *action)
{
  SoState * state = action->getState();

  if (this->vertexProperty.getValue()) {
    state->push();
    this->vertexProperty.getValue()->doAction(action);
  }

  const int rowsize = this->verticesPerRow.getValue();
  const int colsize = this->verticesPerColumn.getValue();

  if (rowsize < 2 || colsize < 2) {
    if (vertexProperty.getValue()) state->pop();
    return;
  }
  const SoCoordinateElement *coords;
  const SbVec3f * normals;
  SbBool doTextures;
  SbBool needNormals = TRUE;

  SoVertexShape::getVertexData(action->getState(), coords, normals,
                               needNormals);

  SoTextureCoordinateBundle tb(action, FALSE, FALSE);
  doTextures = tb.needCoordinates();

  int start = this->startIndex.getValue();

  Binding mbind = findMaterialBinding(action->getState());
  Binding nbind = findNormalBinding(action->getState());

  if (needNormals && normals == NULL) {
    SoNormalCache * nc = this->getNormalCache();
    if (nc == NULL || !nc->isValid(state)) {
      this->generateNormals(state);
      nc = this->getNormalCache();
    }
    normals = nc->getNormals();
  }

  SbVec3f dummynormal(0.0f, 0.0f, 1.0f);
  const SbVec3f * currnormal = &dummynormal;
  if (normals) currnormal = normals;

  SoPrimitiveVertex vertex;
  SoFaceDetail faceDetail;
  SoPointDetail pointDetail;
  vertex.setDetail(&pointDetail);

  if (nbind == OVERALL && needNormals) {
    vertex.setNormal(*currnormal);
  }

  int curridx; // for optimization only

#define IDX(r,c) ((r)*rowsize+(c))

  int normnr = 0;
  int midx = 0;
  for (int i = 0; i < colsize-1; i++) {
    int j = 0;
    this->beginShape(action, QUAD_STRIP, &faceDetail);
    if (nbind == PER_ROW) {
      pointDetail.setNormalIndex(normnr);
      currnormal = &normals[normnr++];
      vertex.setNormal(*currnormal);
    }
    if (mbind == PER_ROW) {
      pointDetail.setMaterialIndex(midx);
      vertex.setMaterialIndex(midx++);
    }
    SbBool first = TRUE;
    faceDetail.setFaceIndex(0);
    for (j = 0; j < rowsize; j++) {
      curridx = IDX(i,j);
      if (nbind == PER_VERTEX) {
        pointDetail.setNormalIndex(curridx);
        currnormal = &normals[curridx];
        vertex.setNormal(*currnormal);
      }
      else if (nbind == PER_FACE) {
        pointDetail.setNormalIndex(normnr);
        currnormal = &normals[normnr++];
        vertex.setNormal(*currnormal);
      }
      if (mbind == PER_VERTEX) {
        pointDetail.setMaterialIndex(curridx);
        vertex.setMaterialIndex(curridx);
      }
      else if (mbind == PER_FACE) {
        pointDetail.setMaterialIndex(midx);
        vertex.setMaterialIndex(midx++);
      }
      if (doTextures) {
        if (tb.isFunction())
          vertex.setTextureCoords(tb.get(coords->get3(start+curridx), *currnormal));
        else {
          pointDetail.setTextureCoordIndex(curridx);
          vertex.setTextureCoords(tb.get(curridx));
        }
      }
      pointDetail.setCoordinateIndex(start + curridx);
      vertex.setPoint(coords->get3(start + curridx));
      this->shapeVertex(&vertex);

      curridx = IDX(i+1,j);
      if (nbind == PER_VERTEX) {
        pointDetail.setNormalIndex(curridx);
        currnormal = &normals[curridx];
        vertex.setNormal(*currnormal);
      }
      if (mbind == PER_VERTEX) {
        pointDetail.setMaterialIndex(curridx);
        vertex.setMaterialIndex(curridx);
      }
      if (doTextures) {
        if (tb.isFunction())
          vertex.setTextureCoords(tb.get(coords->get3(start+curridx), *currnormal));
        else {
          pointDetail.setTextureCoordIndex(curridx);
          vertex.setTextureCoords(tb.get(curridx));
        }
      }
      pointDetail.setCoordinateIndex(start + curridx);
      vertex.setPoint(coords->get3(start + curridx));
      this->shapeVertex(&vertex);
      if (first) first = FALSE;
      else faceDetail.incFaceIndex();
    }
    this->endShape();
    faceDetail.incPartIndex();
  }
#undef IDX

  if (this->vertexProperty.getValue())
    state->pop();

}
