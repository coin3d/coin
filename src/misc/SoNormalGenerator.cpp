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
  \class SoNormalGenerator include/Inventor/misc/SoNormalGenerator.h
  \brief The SoNormalGenerator class is used to generate normals.
  \ingroup general

  FIXME: document properly
*/

#include <Inventor/misc/SoNormalGenerator.h>
#include <coindefs.h> // COIN_OBSOLETED()
#include <stdio.h>

/*!
  Constructor with \a isccw indicating if polygons are specified
  in counter clockwise order. The \a approxVertices can be used
  to optimize normal generation.
*/
SoNormalGenerator::SoNormalGenerator(const SbBool isccw,
                                     const int approxVertices)
  : bsp(approxVertices),
    vertexList(approxVertices),
    vertexFace(approxVertices),
    faceNormals(approxVertices / 4),
    vertexNormals(approxVertices),
    ccw(isccw),
    perVertex(TRUE)
{
}

/*!
  Destructor.
*/
SoNormalGenerator::~SoNormalGenerator()
{
}

/*!
  Resets the normal generator, making it possible to reuse it without
  allocating a new one.

  This method was not part of the Inventor v2.1 API, and is an
  extension specific to Coin.

  \since 2001-10-12
*/
void
SoNormalGenerator::reset(const SbBool ccw)
{
  this->ccw = ccw;
  this->bsp.clear();
  this->vertexList.truncate(0);
  this->vertexFace.truncate(0);
  this->faceNormals.truncate(0);
  this->vertexNormals.truncate(0);
}

/*!
  Signals the start of a new polygon.

  \sa SoNormalGenerator::polygonVertex()
  \sa SoNormalGenerator::endPolygon()
*/
void
SoNormalGenerator::beginPolygon(void)
{
  this->currFaceStart = this->vertexList.getLength();
}

/*!
  Adds a vertex to the current polygon.
  \sa SoNormalGenerator::beginPolygon()
  \sa SoNormalGenerator::endPolygon()
*/
void
SoNormalGenerator::polygonVertex(const SbVec3f &v)
{
  this->vertexList.append(this->bsp.addPoint(v));
  this->vertexFace.append(this->faceNormals.getLength());
}

/*!
  Signals the end of a polygon.
  \sa SoNormalGenerator::beginPolygon()
  \sa SoNormalGenerator::polygonVertex()
*/
void
SoNormalGenerator::endPolygon(void)
{
  SbVec3f n = this->calcFaceNormal();
  this->faceNormals.append(n);
}

/*!
  Convenience method for adding a triangle.
*/
void
SoNormalGenerator::triangle(const SbVec3f &v0,
                            const SbVec3f &v1,
                            const SbVec3f &v2)
{
  this->beginPolygon();
  this->polygonVertex(v0);
  this->polygonVertex(v1);
  this->polygonVertex(v2);
  this->endPolygon();
}

/*!
  Convenience method for adding a quad
*/
void
SoNormalGenerator::quad(const SbVec3f &v0,
                        const SbVec3f &v1,
                        const SbVec3f &v2,
                        const SbVec3f &v3)
{
  this->beginPolygon();
  this->polygonVertex(v0);
  this->polygonVertex(v1);
  this->polygonVertex(v2);
  this->polygonVertex(v3);
  this->endPolygon();
}

//
// calculates the normal vector for a vertex, based on the
// normal vectors of all incident faces
//
static void
calc_normal_vec(const SbVec3f *facenormals, const int facenum,
                SbList <int32_t> &faceArray, const float threshold,
                SbVec3f &vertnormal)
{
  // start with face normal vector
  const SbVec3f * facenormal = &facenormals[facenum];
  vertnormal = *facenormal;

  int n = faceArray.getLength();
  int currface;

  for (int i = 0; i < n; i++) {
    currface = faceArray[i];
    if (currface != facenum) { // check all but this face
      const SbVec3f &normal = facenormals[currface];
      if ((normal.dot(*facenormal)) > threshold) {
        // smooth towards this face
        vertnormal += normal;
      }
    }
  }
}

/*!
  Triggers the normal generation. Normals are generated using
  \a creaseAngle to find which edges should be flat-shaded
  and which should be smooth-shaded.

  If normals are generated for triangle strips, the \a striplens and
  \a numstrips must be supplied. See src/nodes/SoTriangleStripSet.cpp
  (generateDefaultNormals()) for an example on how you send triangle
  strip information to this generator. It's not trivial, since you
  have to know how OpenGL/Coin generate triangles from triangle
  strips.

*/
void
SoNormalGenerator::generate(const float creaseAngle,
                            const int32_t *striplens,
                            const int numstrips)
{
  int i;

  // for each vertex, store all faceindices the vertex is a part of
  SbList <int32_t> * vertexFaceArray = new SbList<int32_t>[bsp.numPoints()];

  int numvi = this->vertexList.getLength();

  for (i = 0; i < numvi; i++) {
    vertexFaceArray[vertexList[i]].append(this->vertexFace[i]);
  }

  float threshold = (float)cos(creaseAngle);

  if (striplens) {
    i = 0;
    for (int j = 0; j < numstrips; j++) {
      assert(i+2 < numvi);
      SbVec3f tmpvec;
      calc_normal_vec(this->faceNormals.getArrayPtr(),
                      this->vertexFace[i],
                      vertexFaceArray[vertexList[i]],
                      threshold, tmpvec);
      tmpvec.normalize();
      this->vertexNormals.append(tmpvec);
      calc_normal_vec(this->faceNormals.getArrayPtr(),
                      this->vertexFace[i+1],
                      vertexFaceArray[vertexList[i+1]],
                      threshold, tmpvec);
      tmpvec.normalize();
      this->vertexNormals.append(tmpvec);

      int num = striplens[j] - 2;

      while (num--) {
        i += 2;
        assert(i < numvi);
        calc_normal_vec(this->faceNormals.getArrayPtr(),
                        this->vertexFace[i],
                        vertexFaceArray[vertexList[i]],
                        threshold, tmpvec);
        tmpvec.normalize();
        this->vertexNormals.append(tmpvec);
        i++;
      }
    }
  }
  else {
    for (i = 0; i < numvi; i++) {
      SbVec3f tmpvec;
      calc_normal_vec(this->faceNormals.getArrayPtr(),
                      this->vertexFace[i],
                      vertexFaceArray[vertexList[i]],
                      threshold, tmpvec);
      tmpvec.normalize();
      this->vertexNormals.append(tmpvec);
    }
  }
  delete [] vertexFaceArray;
  this->vertexFace.truncate(0, TRUE);
  this->vertexList.truncate(0, TRUE);
  this->faceNormals.truncate(0, TRUE);
  this->bsp.clear();
  this->vertexNormals.fit();

  // return vertex normals
  this->perVertex = TRUE;
}

/*!
  Generates one normal per strip by averaging face normals.
*/
void
SoNormalGenerator::generatePerStrip(const int32_t * striplens,
                                    const int numstrips)
{
  int cnt = 0;
  for (int i = 0; i < numstrips; i++) {
    int n = striplens[i] - 2;
    SbVec3f acc(0.0f, 0.0f, 0.0f);
    while (n > 0) {
      acc += this->faceNormals[cnt++];
      n--;
    }
    acc.normalize();
    // use face normal array to store strip normals
    this->faceNormals[i] = acc;
  }
  // strip normals can now be found in faceNormals array
  this->faceNormals.truncate(numstrips, TRUE);
  this->perVertex = FALSE;
}

/*!
  Generates the normals per face. Use this when PER_FACE normal
  binding is needed. This method is not part of the OIV API.
*/
void
SoNormalGenerator::generatePerFace(void)
{
  // face normals have already been generated. Just set flag.
  this->perVertex = FALSE;
  this->faceNormals.fit();
}

/*!
  Generates one overall normal by averaging all face
  normals. Use when normal binding is OVERALL. This method
  is not part of the OIV API.
*/
void
SoNormalGenerator::generateOverall(void)
{
  const int n = this->faceNormals.getLength();
  const SbVec3f * normals = this->faceNormals.getArrayPtr();
  SbVec3f acc(0.0f, 0.0f, 0.0f);
  for (int i = 0; i < n; i++) acc += normals[i];
  acc.normalize();
  this->faceNormals.truncate(0, TRUE);
  this->faceNormals.append(acc);

  // normals are not per vertex
  this->perVertex = FALSE;
}

/*!
  Returns the number of normals generated.
*/
int
SoNormalGenerator::getNumNormals(void) const
{
  if (!this->perVertex) {
    return this->faceNormals.getLength();
  }
  return this->vertexNormals.getLength();
}

/*!
  Sets the number of generated normals. This method is not supported
  in Coin, and is provided for API compatibility only.
*/
void
SoNormalGenerator::setNumNormals(const int /* num */)
{
  COIN_OBSOLETED();
}

/*!
  Returns a pointer to the generated normals.
*/
const SbVec3f *
SoNormalGenerator::getNormals(void) const
{
  if (!this->perVertex) {
    if (this->faceNormals.getLength()) return this->faceNormals.getArrayPtr();
    return NULL;
  }
  if (this->vertexNormals.getLength())
    return this->vertexNormals.getArrayPtr();
  return NULL;
}

/*!
  Returns the normal at index \a i.
  \sa SoNormalGenerator::getNumNormals()
*/
const SbVec3f &
SoNormalGenerator::getNormal(const int32_t i) const
{
  assert(i >= 0 && i < this->getNumNormals());
  return this->getNormals()[i];
}

/*!
  Sets the normal at index \a index to \a normal. This method
  is not supported in Coin, and is provided for API compatibility
  only.
*/
void
SoNormalGenerator::setNormal(const int32_t /* index */,
                             const SbVec3f & /* normal */)
{
  COIN_OBSOLETED();
}

//
// Calculates the face normal to the current face.
//
SbVec3f
SoNormalGenerator::calcFaceNormal()
{
  int num = this->vertexList.getLength() - this->currFaceStart;
  assert(num >= 3);
  const int * cind = (const int *) this->vertexList.getArrayPtr() + this->currFaceStart;
  const SbVec3f * coords = this->bsp.getPointsArrayPtr();
  SbVec3f ret;

  if (num == 3) { // triangle
    if (!ccw)
      ret =
        (coords[cind[0]] -
         coords[cind[1]]).cross(coords[cind[2]] -
                                coords[cind[1]]);
    else
      ret =
        (coords[cind[2]] -
         coords[cind[1]]).cross(coords[cind[0]] -
                                coords[cind[1]]);
  }
  else {
    const SbVec3f *vert1, *vert2;
    ret.setValue(0.0f, 0.0f, 0.0f);
    vert2 = coords + cind[num-1];
    for (int i = 0; i < num; i++) {
      vert1 = vert2;
      vert2 = coords + cind[i];
      ret[0] += ((*vert1)[1] - (*vert2)[1]) * ((*vert1)[2] + (*vert2)[2]);
      ret[1] += ((*vert1)[2] - (*vert2)[2]) * ((*vert1)[0] + (*vert2)[0]);
      ret[2] += ((*vert1)[0] - (*vert2)[0]) * ((*vert1)[1] + (*vert2)[1]);
    }
    if (!ccw) ret = -ret;
  }
  ret.normalize();
  return ret;
}
