/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
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
  \class SoNormalGenerator include/Inventor/misc/SoNormalGenerator.h
  \brief The SoNormalGenerator class is used to generate normals.
*/

#include <Inventor/misc/SoNormalGenerator.h>
#include <Inventor/misc/SoBasic.h> // COIN_STUB()
#include <stdio.h>

/*!
  Constructor with \a isccw indicating if polygons are specified
  in counter clockwise order. The \a approxVertices can be used
  to optimize normal generation. The \a free flag specifies
  whether normals should be free when this instance is
  destructed.
*/
SoNormalGenerator::SoNormalGenerator(const SbBool isccw,
                                     const int approxVertices,
                                     const SbBool free)
  : bsp(approxVertices),
    vertexList(approxVertices),
    vertexFace(approxVertices),
    faceNormals(approxVertices / 4),
    vertexNormals(approxVertices),
    ccw(isccw),
    freeNormals(free)
{
}

/*!
  Destructor.
*/
SoNormalGenerator::~SoNormalGenerator()
{
  if (!freeNormals) {
    this->vertexNormals.stealPointer();
  }
}

/*!
  Signals the start of a new polygon.

  \sa SoNormalGenerator::polygonVertex()
  \sa SoNormalGenerator::endPolygon()
*/
void
SoNormalGenerator::beginPolygon()
{
  currFaceStart = vertexList.getLength();
}

/*!
  Adds a vertex to the current polygon.
  \sa SoNormalGenerator::beginPolygon()
  \sa SoNormalGenerator::endPolygon()
*/
void
SoNormalGenerator::polygonVertex(const SbVec3f &v)
{
  vertexList.append(bsp.addPoint(v));
  vertexFace.append(this->faceNormals.getLength());
}

/*!
  Signals the end of a polygon.
  \sa SoNormalGenerator::beginPolygon()
  \sa SoNormalGenerator::polygonVertex()
*/
void
SoNormalGenerator::endPolygon()
{
  SbVec3f n = calcFaceNormal();
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
  beginPolygon();
  polygonVertex(v0);
  polygonVertex(v1);
  polygonVertex(v2);
  endPolygon();
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
  beginPolygon();
  polygonVertex(v0);
  polygonVertex(v1);
  polygonVertex(v2);
  polygonVertex(v3);
  endPolygon();
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
  const SbVec3f *facenormal = &facenormals[facenum];
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

  If normals are generated for triangle strips, the
  \a striplens and \a numstrips must be supplied.
*/
void
SoNormalGenerator::generate(const float creaseAngle,
                            const int32_t *striplens,
                            const int numstrips)
{
  int i;

  // for each vertex, store all faceindices the vertex is a part of
  SbList <int32_t> *vertexFaceArray = new SbList<int32_t>[bsp.numPoints()];

  int numvi = this->vertexList.getLength();

  for (i = 0; i < numvi; i++) {
    vertexFaceArray[vertexList[i]].append(vertexFace[i]);
  }

  float threshold = (float)cos(creaseAngle);

  if (striplens) {
    i = 0;
    for (int j = 0; j < numstrips; j++) {
      assert(i+2 < numvi);
      SbVec3f tmpvec;
      calc_normal_vec(faceNormals.constArrayPointer(),
                      vertexFace[i],
                      vertexFaceArray[vertexList[i]],
                      threshold, tmpvec);
      tmpvec.normalize();
      vertexNormals.append(tmpvec);
      calc_normal_vec(faceNormals.constArrayPointer(),
                      vertexFace[i+1],
                      vertexFaceArray[vertexList[i+1]],
                      threshold, tmpvec);
      tmpvec.normalize();
      vertexNormals.append(tmpvec);

      int num = striplens[j] - 2;

      while (num--) {
        i += 2;
        assert(i < numvi);
        calc_normal_vec(faceNormals.constArrayPointer(),
                        vertexFace[i],
                        vertexFaceArray[vertexList[i]],
                        threshold, tmpvec);
        tmpvec.normalize();
        vertexNormals.append(tmpvec);
        i++;
      }
    }
  }
  else {
    for (i = 0; i < numvi; i++) {
      SbVec3f tmpvec;
      calc_normal_vec(faceNormals.constArrayPointer(),
                      vertexFace[i],
                      vertexFaceArray[vertexList[i]],
                      threshold, tmpvec);
      tmpvec.normalize();
      vertexNormals.append(tmpvec);
    }
  }
  delete [] vertexFaceArray;
  this->vertexFace.clear();
  this->vertexList.clear();
  this->faceNormals.clear();
  this->bsp.clear();
  this->vertexNormals.fit();
}

/*!
  Returns the number of normals generated.
*/
int
SoNormalGenerator::getNumNormals() const
{
  return this->vertexNormals.getLength();
}

/*!
  Sets the number of generated normals.
*/
void
SoNormalGenerator::setNumNormals(const int /* num */)
{
  COIN_STUB();
}

/*!
  Returns a pointer to the generated normals.
*/
const SbVec3f *
SoNormalGenerator::getNormals() const
{
  assert(this->vertexNormals.getLength());
  return this->vertexNormals.constArrayPointer();
}

/*!
  Returns the normal at index \a i.
  \sa SoNormalGenerator::getNumNormals()
*/
const SbVec3f &
SoNormalGenerator::getNormal(const int32_t i) const
{
  assert(i >= 0 && i < this->vertexNormals.getLength());
  return this->vertexNormals.constArrayPointer()[i];
}

/*!
  Sets the normal at index \a index to \a normal.
*/
void
SoNormalGenerator::setNormal(const int32_t /* index */,
                             const SbVec3f & /* normal */)
{
  COIN_STUB();
}

//
// Calculates the face normal to the current face.
//
SbVec3f
SoNormalGenerator::calcFaceNormal()
{
  int num = vertexList.getLength() - currFaceStart;
  assert(num >= 3);
  const int *cind = vertexList.constArrayPointer() + currFaceStart;
  const SbVec3f *coords = bsp.getPointsArrayPtr();
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
