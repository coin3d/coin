/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

/*!
  \class SoQuadMesh SoQuadMesh.h Inventor/nodes/SoQuadMesh.h
  \brief The SoQuadMesh class is used to render and optimize a quadrilateral mesh.
  \ingroup nodes

  This node uses the coordinates in order, either from the state or
  from the SoQuadMesh::vertexProperty node, to construct a
  quadrilateral mesh.

  The quads are generated in row major order, using the two fields
  SoQuadMesh::verticesPerColumn and SoQuadMesh::verticesPerRow to
  specify the mesh. E.g. if SoQuadMesh::verticesPerColumn is 3 and
  SoQuadMesh::verticesPerRow is 2, two quads will be generated with
  the first one using (in order) coordinates 0, 1, 3 and 2, the second
  one using coordinates 2, 3, 5 and 4 (you get three rows of vertices;
  the first row uses vertices 0 and 1, the second row 2 and 3, and the
  third row 4 and 5).

  Here's a quick and simple usage example code snippet:

  \code
  // Vertices for the Quad mesh.
  static float vertices[25][3] = {
    // Row 1
    {-11, 0, 1}, {0, 11, 1}, {11, 0, 1}, {0, -11, 1}, {-11, 0, 1},
    // Row 2
    {-9, 0, 1}, {0, 9, 1}, {9, 0, 1}, {0, -9, 1}, {-9, 0, 1},
    // Row 3
    {-9, 0, -1}, {0, 9, -1}, {9, 0, -1}, {0, -9, -1}, {-9, 0, -1},
    // Row 4
    {-11, 0, -1}, {0, 11, -1}, {11, 0, -1}, {0, -11, -1}, {-11, 0, -1},
    // Row 5
    {-11, 0, 1}, {0, 11, 1}, {11, 0, 1}, {0, -11, 1}, {-11, 0, 1}
  };
  
  // This function generate an object by using the SoQuadMesh node
  // Return:
  //  SoSeparator *
  static SoSeparator *
  quadMesh(void) 
  {
    SoSeparator * qm = new SoSeparator;
    
    // Define coordinates
    SoCoordinate3 * coords = new SoCoordinate3;
    coords->point.setValues(0, 30, vertices);
    qm->addChild(coords);
    
    // QuadMesh
    SoQuadMesh * mesh = new SoQuadMesh;
    mesh->verticesPerRow = 5;
    mesh->verticesPerColumn = 5;
    qm->addChild(mesh);
  
    return qm;
  }
  \endcode

  The quadmesh geometry resulting from this code looks like this:<br>

  <center>
  <img src="http://doc.coin3d.org/images/Coin/nodes/quadmesh.png">
  </center>


  For SoQuadMesh, normals and materials can be bound PER_PART (per
  row), PER_FACE, PER_VERTEX and OVERALL. The default material binding
  is OVERALL. The default normal binding is PER_VERTEX.

  \sa SoTriangleStripSet SoIndexedTriangleStripSet
*/

#include <Inventor/nodes/SoQuadMesh.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/elements/SoGLShadeModelElement.h>

#include <Inventor/actions/SoGLRenderAction.h>
#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H
#include <Inventor/system/gl.h>

#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/elements/SoLightModelElement.h>
#include <Inventor/elements/SoGLCoordinateElement.h>
#include <Inventor/elements/SoNormalBindingElement.h>
#include <Inventor/elements/SoMaterialBindingElement.h>
#include <Inventor/bundles/SoMaterialBundle.h>
#include <Inventor/elements/SoShapeHintsElement.h>
#include <Inventor/caches/SoNormalCache.h>
#include <Inventor/bundles/SoTextureCoordinateBundle.h>

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

// Documented in superclass.
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
#include "../misc/SoGLqmeshTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_ROW
#define NBINDING OVERALL
#define TEXTURES FALSE
static void sogl_qmesh_m1_n0_t0
#include "../misc/SoGLqmeshTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_FACE
#define NBINDING OVERALL
#define TEXTURES FALSE
static void sogl_qmesh_m2_n0_t0
#include "../misc/SoGLqmeshTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX
#define NBINDING OVERALL
#define TEXTURES FALSE
static void sogl_qmesh_m3_n0_t0
#include "../misc/SoGLqmeshTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

// -----

// n1

#define MBINDING OVERALL
#define NBINDING PER_ROW
#define TEXTURES FALSE
static void sogl_qmesh_m0_n1_t0
#include "../misc/SoGLqmeshTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_ROW
#define NBINDING PER_ROW
#define TEXTURES FALSE
static void sogl_qmesh_m1_n1_t0
#include "../misc/SoGLqmeshTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_FACE
#define NBINDING PER_ROW
#define TEXTURES FALSE
static void sogl_qmesh_m2_n1_t0
#include "../misc/SoGLqmeshTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX
#define NBINDING PER_ROW
#define TEXTURES FALSE
static void sogl_qmesh_m3_n1_t0
#include "../misc/SoGLqmeshTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

// -----

// n2

#define MBINDING OVERALL
#define NBINDING PER_FACE
#define TEXTURES FALSE
static void sogl_qmesh_m0_n2_t0
#include "../misc/SoGLqmeshTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_ROW
#define NBINDING PER_FACE
#define TEXTURES FALSE
static void sogl_qmesh_m1_n2_t0
#include "../misc/SoGLqmeshTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_FACE
#define NBINDING PER_FACE
#define TEXTURES FALSE
static void sogl_qmesh_m2_n2_t0
#include "../misc/SoGLqmeshTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX
#define NBINDING PER_FACE
#define TEXTURES FALSE
static void sogl_qmesh_m3_n2_t0
#include "../misc/SoGLqmeshTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

// -----

// n3

#define MBINDING OVERALL
#define NBINDING PER_VERTEX
#define TEXTURES FALSE
static void sogl_qmesh_m0_n3_t0
#include "../misc/SoGLqmeshTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_ROW
#define NBINDING PER_VERTEX
#define TEXTURES FALSE
static void sogl_qmesh_m1_n3_t0
#include "../misc/SoGLqmeshTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_FACE
#define NBINDING PER_VERTEX
#define TEXTURES FALSE
static void sogl_qmesh_m2_n3_t0
#include "../misc/SoGLqmeshTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX
#define NBINDING PER_VERTEX
#define TEXTURES FALSE
static void sogl_qmesh_m3_n3_t0
#include "../misc/SoGLqmeshTemplate.icc"
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
#include "../misc/SoGLqmeshTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_ROW
#define NBINDING OVERALL
#define TEXTURES TRUE
static void sogl_qmesh_m1_n0_t1
#include "../misc/SoGLqmeshTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_FACE
#define NBINDING OVERALL
#define TEXTURES TRUE
static void sogl_qmesh_m2_n0_t1
#include "../misc/SoGLqmeshTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX
#define NBINDING OVERALL
#define TEXTURES TRUE
static void sogl_qmesh_m3_n0_t1
#include "../misc/SoGLqmeshTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

// -----

// n1

#define MBINDING OVERALL
#define NBINDING PER_ROW
#define TEXTURES TRUE
static void sogl_qmesh_m0_n1_t1
#include "../misc/SoGLqmeshTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_ROW
#define NBINDING PER_ROW
#define TEXTURES TRUE
static void sogl_qmesh_m1_n1_t1
#include "../misc/SoGLqmeshTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_FACE
#define NBINDING PER_ROW
#define TEXTURES TRUE
static void sogl_qmesh_m2_n1_t1
#include "../misc/SoGLqmeshTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX
#define NBINDING PER_ROW
#define TEXTURES TRUE
static void sogl_qmesh_m3_n1_t1
#include "../misc/SoGLqmeshTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

// -----

// n2

#define MBINDING OVERALL
#define NBINDING PER_FACE
#define TEXTURES TRUE
static void sogl_qmesh_m0_n2_t1
#include "../misc/SoGLqmeshTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_ROW
#define NBINDING PER_FACE
#define TEXTURES TRUE
static void sogl_qmesh_m1_n2_t1
#include "../misc/SoGLqmeshTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_FACE
#define NBINDING PER_FACE
#define TEXTURES TRUE
static void sogl_qmesh_m2_n2_t1
#include "../misc/SoGLqmeshTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX
#define NBINDING PER_FACE
#define TEXTURES TRUE
static void sogl_qmesh_m3_n2_t1
#include "../misc/SoGLqmeshTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

// -----

// n3

#define MBINDING OVERALL
#define NBINDING PER_VERTEX
#define TEXTURES TRUE
static void sogl_qmesh_m0_n3_t1
#include "../misc/SoGLqmeshTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_ROW
#define NBINDING PER_VERTEX
#define TEXTURES TRUE
static void sogl_qmesh_m1_n3_t1
#include "../misc/SoGLqmeshTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_FACE
#define NBINDING PER_VERTEX
#define TEXTURES TRUE
static void sogl_qmesh_m2_n3_t1
#include "../misc/SoGLqmeshTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

#define MBINDING PER_VERTEX
#define NBINDING PER_VERTEX
#define TEXTURES TRUE
static void sogl_qmesh_m3_n3_t1
#include "../misc/SoGLqmeshTemplate.icc"
#undef MBINDING
#undef NBINDING
#undef TEXTURES

// -----

#undef IDX

#undef OVERALL
#undef PER_ROW
#undef PER_FACE
#undef PER_VERTEX


// Documented in superclass.
void
SoQuadMesh::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoQuadMesh, SO_FROM_INVENTOR_1);

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

// -----

// Documented in superclass.
void
SoQuadMesh::GLRender(SoGLRenderAction * action)
{
  SoState * state = action->getState();

  SbBool didpush = FALSE;

  if (this->vertexProperty.getValue()) {
    state->push();
    didpush = TRUE;
    this->vertexProperty.getValue()->GLRender(action);
  }
  
  if (!this->shouldGLRender(action)) {
    if (didpush) state->pop();
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

  int start = this->startIndex.getValue();

  Binding mbind = findMaterialBinding(action->getState());
  Binding nbind = findNormalBinding(action->getState());
  if (!needNormals) nbind = OVERALL;

  if ((nbind == PER_FACE) || (mbind == PER_FACE))
    SoGLShadeModelElement::forceSend(state, TRUE);

  SoNormalCache * nc = NULL;

  if (needNormals && normals == NULL) {
    nc = this->generateAndReadLockNormalCache(state);
    normals = nc->getNormals();
  }

  SoMaterialBundle mb(action);
  mb.sendFirst(); // make sure we have the correct material

  soquadmesh_ni_render_funcs[ (mbind << 3) | (nbind << 1) | doTextures ]
    (coords,
     normals,
     &mb,
     &tb,
     needNormals,
     rowsize,
     colsize,
     start);
  
  if (nc) {
    this->readUnlockNormalCache();
  }

  if (didpush) state->pop();
}

// Documented in superclass.
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

// Documented in superclass.
void
SoQuadMesh::getPrimitiveCount(SoGetPrimitiveCountAction *action)
{
  if (!this->shouldPrimitiveCount(action)) return;

  action->addNumTriangles(2 * this->verticesPerRow.getValue() *
                          this->verticesPerColumn.getValue());
}

// Documented in superclass. Overridden to return FALSE. Normals are
// generated in normal cache.
SbBool
SoQuadMesh::generateDefaultNormals(SoState * /* state */, SoNormalBundle * /* nb */)
{
  return FALSE;
}

// Documented in superclass.
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

  SoNormalCache * nc = NULL;

  if (needNormals && normals == NULL) {
    nc = this->generateAndReadLockNormalCache(state);
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

  if (nc) {
    this->readUnlockNormalCache();
  }

  if (this->vertexProperty.getValue())
    state->pop();

}
