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
  \class SoNormalCache include/Inventor/caches/SoNormalCache.h
  The SoNormalCache class is used to hold cached normals.
  As an OIV extension, it is also possible to generate normals
  using this class. It is more powerful and easier to use than the
  SoNormalGenerator class. It is possible to generate normal
  per vertex with indices (uses much less memory than plain
  per vertex normals), and it contains special methods to generate
  normals for triangle strips and quads.
*/

#include <Inventor/caches/SoNormalCache.h>
#include <Inventor/misc/SoNormalGenerator.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG


//
// fixme: add test to shrink normalArray.
//

#define NORMAL_EPSILON 0.001f

/*!
  Contructor with \a state being the current state.
*/
SoNormalCache::SoNormalCache(SoState * const state)
  : SoCache(state)
{
  this->normalData.normals = NULL;
  this->numNormals = 0;
}

/*!
  Destructor
*/
SoNormalCache::~SoNormalCache()
{
  this->clearGenerator();
}

/*!
  Sets an array of normals for this cache. The normals will not
  be deleted when the instance is deleted.
*/
void
SoNormalCache::set(const int num, const SbVec3f * const normals)
{
  this->clearGenerator();
  this->numNormals = num;
  this->normalData.normals = normals;
  this->indices.truncate(0, TRUE);
  this->normalArray.truncate(0, TRUE);
}

/*!
  Uses a normal generator in this cache. The normal generator will
  be deleted when the cache is deleted or reset.
*/
void
SoNormalCache::set(SoNormalGenerator *generator)
{
  this->clearGenerator();
  this->indices.truncate(0, TRUE);
  this->normalArray.truncate(0, TRUE);
  this->numNormals = 0;
  this->normalData.generator = generator;
}

/*!
  Returns the number of normals in the cache.
*/
int
SoNormalCache::getNum() const
{
  if (this->numNormals == 0 && this->normalData.generator) {
    return this->normalData.generator->getNumNormals();
  }
  return this->numNormals;
}

/*!
  Return a pointer to the normals in this cache.
*/
const SbVec3f *
SoNormalCache::getNormals() const
{
  if (this->numNormals == 0 && this->normalData.generator) {
    return this->normalData.generator->getNormals();
  }
  return this->normalData.normals;
}

/*!
  Returns the number of indices in this cache. Normals are
  generated with PER_VERTEX_INDEXED binding.
*/
int
SoNormalCache::getNumIndices() const
{
  return this->indices.getLength();
}

/*!
  Returns the normal indices.
*/
const int32_t *
SoNormalCache::getIndices() const
{
  if (this->indices.getLength()) return this->indices.getArrayPtr();
  return NULL;
}

static int
count_faces(const int32_t *ptr,
            const int numindices)
{
  const int32_t *endptr = ptr + numindices;
  int cnt = 0;
  while (ptr < endptr) {
    if (*ptr++ < 0) cnt++;
  }
  return cnt;
}

static int
count_strip_faces(const int32_t *ptr,
                  const int numindices)
{
  const int32_t *endptr = ptr + numindices;
  int cnt = 0;
  while (ptr < endptr) {
    ptr+=3; // skip first triangle
    cnt++;
    while (*ptr++ >= 0) cnt++;
  }
  return cnt;

}

static int
count_strips(const int32_t *ptr,
             const int numindices)
{
  const int32_t *endptr = ptr + numindices;
  int cnt = 0;
  while (ptr < endptr) {
    ptr+=3; // skip first triangle
    while (*ptr++ >= 0);
    cnt++;
  }
  return cnt;
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
  Generates normals for each vertex for each face. It is possible
  to specify face normals if these have been calculated somewhere
  else, otherwise the face normals will be calculated before
  the vertex normals are calculated. \a triStrip should be
  \e TRUE if the geometry consists of triangle strips.
*/
void
SoNormalCache::generatePerVertex(const SbVec3f * const coords,
                                 const int32_t *vindex,
                                 const int numvi,
                                 const float crease_angle,
                                 const SbVec3f *facenormals,
                                 const SbBool ccw,
                                 const int numFaces,
                                 const SbBool tristrip)
{
  this->clearGenerator();
  this->indices.truncate(0);
  this->normalArray.truncate(0);

  SoNormalCache tempcache(NULL);
  const SbVec3f *facenorm = (SbVec3f*) facenormals;
  if (facenorm == NULL) {
    // use a SoNormalCache to store temporary data
    if (tristrip) {
      tempcache.generatePerFaceStrip(coords,
                                     vindex,
                                     numvi,
                                     ccw,
                                     numFaces);
    }
    else {
      tempcache.generatePerFace(coords,
                                vindex,
                                numvi,
                                ccw,
                                numFaces);
    }
    facenorm = tempcache.getNormals();
  }

  // find biggest vertex index
  int i;
  int maxi = 0;
  int temp;
  for (i = 0; i < numvi; i++) {
    temp = vindex[i]; // don't care about -1's
    if (temp > maxi) maxi = temp;
  }

  // for each vertex, store all faceindices the vertex is a part of
  SbList<int32_t> *vertexFaceArray = new SbList<int32_t>[maxi+1]; // [0, maxi]

  // for each vertex, store all normals that have been calculated
  SbList <int32_t>*vertexNormalArray = new SbList<int32_t>[maxi+1]; // [0, maxi]

  int numfaces = 0;

  if (tristrip) {
    i = 0;
    while (i < numvi) {
      temp = vindex[i++];
      assert(temp >= 0);
      vertexFaceArray[temp].append(numfaces);
      temp = vindex[i++];
      assert(temp >= 0);
      vertexFaceArray[temp].append(numfaces);
      temp = vindex[i++];
      assert(temp >= 0);
      vertexFaceArray[temp].append(numfaces);

      numfaces++;
      temp = vindex[i++];

      while (temp >= 0) {
        vertexFaceArray[temp].append(numfaces++);
        temp = vindex[i++];
      }
    }
  }
  else { // !tristrip
    for (i = 0; i < numvi; i++) {
      temp = vindex[i];
      if (temp >= 0)
        vertexFaceArray[temp].append(numfaces);
      else numfaces++;
    }
  }

  float threshold = (float)cos(crease_angle);
  SbBool found;
  int currindex = 0; // current normal index
  int nindex = 0;
  int j,n ;
  int facenum = 0;
  int stripcnt = 0;

  for (i = 0; i < numvi; i++) {
    currindex = vindex[i];
    if (currindex >= 0) {
      if (tristrip) {
        if (++stripcnt > 3) facenum++; // next face
      }
      // calc normal for this vertex
      SbVec3f tmpvec;
      calc_normal_vec(facenorm, facenum, vertexFaceArray[currindex],
                      threshold, tmpvec);
      tmpvec.normalize();

      if (normalArray.getLength() <= nindex)
        normalArray.append(tmpvec);
      else
        normalArray[nindex] = tmpvec;

      // try to find equal normal (total smoothing)
      SbList <int32_t> &array = vertexNormalArray[currindex];
      found = FALSE;
      n = array.getLength();
      int same_normal = -1;
      for (j = 0; j < n && !found; j++) {
        same_normal = array[j];
        found = normalArray[same_normal].equals(normalArray[nindex],
                                                NORMAL_EPSILON);
      }
      if (found)
        this->indices.append(same_normal);
      // might be equal to the previous normal (when all normals for a face are eq)
      else if ((nindex > 0) &&
               normalArray[nindex].equals(normalArray[nindex-1],
                                          NORMAL_EPSILON)) {
        this->indices.append(nindex-1);
      }
      else {
        this->indices.append(nindex);
        array.append(nindex);
        nindex++;
      }
    }
    else { // new face
      facenum++;
      stripcnt = 0;
      this->indices.append(-1); // add a -1 for PER_VERTEX_INDEXED binding
    }
  }
  if (this->normalArray.getLength()) {
    this->normalData.normals = this->normalArray.getArrayPtr();
    this->numNormals = this->normalArray.getLength();
  }
#if 0 && COIN_DEBUG
  SoDebugError::post(("SoNormalCache::generatePerVertex",
                      "generated normals per vertex: %p %d %d\n",
                      normals, numNormals, this->indices.getLength());
#endif
  delete [] vertexFaceArray;
  delete [] vertexNormalArray;
}

/*!
  Generates face normals for the faceset defined by \a coords
  and \a cind. Assumes indices are correct (all faces has at least
  three vertices, index array ends with a -1).
*/
void
SoNormalCache::generatePerFace(const SbVec3f * const coords,
                               const int32_t *cind,
                               const int nv,
                               const SbBool ccw,
                               const int numFaces)
{
  this->clearGenerator();
  // FIXME: make this code robust. 19990405 mortene.
  this->indices.truncate(0);
  this->normalArray.truncate(0, TRUE);

  const int numalloc = numFaces > 0 ? numFaces : count_faces(cind, nv);
  const int32_t *endptr = cind + nv;

  SbVec3f tmpvec;

  while(cind < endptr) {
    int v0 = cind[0];
    int v1 = cind[1];
    int v2 = cind[2];

    if (cind[3] < 0) { // triangle
      if (!ccw)
        tmpvec = (coords[v0] - coords[v1]).cross(coords[v2] - coords[v1]);
      else
        tmpvec = (coords[v2] - coords[v1]).cross(coords[v0] - coords[v1]);

      // Be robust when it comes to erroneously specified triangles.
      float len = tmpvec.length();
#if COIN_DEBUG
      if(len <= 0.0f) {
        static uint32_t normgenerrors_tri = 0;
        if (normgenerrors_tri < 1) {
          SoDebugError::postWarning("SoNormalCache::generatePerFace",
                                    "Erroneous triangle specification in model "
                                    "(indices= [%d, %d, %d], "
                                    "coords=<%f, %f, %f>, <%f, %f, %f>, <%f, %f, %f>) "
                                    "(this warning will be printed only once, "
                                    "but there might be more errors).",
                                    v0, v1, v2,
                                    coords[v0][0], coords[v0][1], coords[v0][2],
                                    coords[v1][0], coords[v1][1], coords[v1][2],
                                    coords[v2][0], coords[v2][1], coords[v2][2]);
        }
        normgenerrors_tri++;
      }
#endif // !COIN_DEBUG
      if (len > 0.0f) tmpvec.normalize();
      else tmpvec.setValue(1.0f, 0.0f, 0.0f); // dummy value
      this->normalArray.append(tmpvec);
      cind += 4; // goto next triangle/polygon
    }
    else { // more than 3 vertices
      // use Newell's method to calculate normal vector
      const SbVec3f *vert1, *vert2;
      tmpvec.setValue(0.0f, 0.0f, 0.0f);
      vert2 = coords + v0;
      cind++; // v0 is already read
      while (*cind >= 0) {
        vert1 = vert2;
        vert2 = coords + *cind++;
        tmpvec[0] += ((*vert1)[1] - (*vert2)[1]) * ((*vert1)[2] + (*vert2)[2]);
        tmpvec[1] += ((*vert1)[2] - (*vert2)[2]) * ((*vert1)[0] + (*vert2)[0]);
        tmpvec[2] += ((*vert1)[0] - (*vert2)[0]) * ((*vert1)[1] + (*vert2)[1]);
      }
      vert1 = vert2;  // last edge (back to v0)
      vert2 = coords + v0;
      tmpvec[0] += ((*vert1)[1] - (*vert2)[1]) * ((*vert1)[2] + (*vert2)[2]);
      tmpvec[1] += ((*vert1)[2] - (*vert2)[2]) * ((*vert1)[0] + (*vert2)[0]);
      tmpvec[2] += ((*vert1)[0] - (*vert2)[0]) * ((*vert1)[1] + (*vert2)[1]);

      // Be robust when it comes to erroneously specified polygons.
      float len = tmpvec.length();
#if COIN_DEBUG
      if(len <= 0.0f) {
        static uint32_t normgenerrors_poly = 0;
        if (normgenerrors_poly < 1) {
          SoDebugError::postWarning("SoNormalCache::generatePerFace",
                                    "Erroneous polygon specification in model "
                                    "(this warning will be printed only once, "
                                    "but there might be more errors).");
        }
        normgenerrors_poly++;
      }
#endif // !COIN_DEBUG
      if (len > 0.0f) tmpvec.normalize();
      else tmpvec.setValue(1.0f, 0.0f, 0.0f); // dummy value
      this->normalArray.append(ccw ? tmpvec : -tmpvec);
      cind++; // skip the -1
    }
  }

  if (this->normalArray.getLength()) {
    this->normalData.normals = this->normalArray.getArrayPtr();
    this->numNormals = this->normalArray.getLength();
  }

#if 0 // debug
  SoDebugError::postInfo("SoNormalCache::generatePerFace",
                         "generated normals per face: %p %d",
                         normals, numNormals);
#endif // debug
}

/*!
  Generates face normals for triangle strips.
*/
void
SoNormalCache::generatePerFaceStrip(const SbVec3f * const coords,
                                    const int32_t *cind,
                                    const int nv,
                                    const SbBool ccw,
                                    const int numFaces)
{
  this->clearGenerator();
  this->indices.truncate(0);
  this->normalArray.truncate(0, TRUE);

  const int numalloc = numFaces > 0 ? numFaces : count_strip_faces(cind, nv);
  const int32_t *endptr = cind + nv;

  const SbVec3f *c0, *c1, *c2;
  SbVec3f n;

  SbBool flip = ccw;

  while (cind < endptr) {
    flip = ccw;
    c0 = &coords[*cind++];
    c1 = &coords[*cind++];
    c2 = &coords[*cind++];

    if (!flip)
      n = (*c0 - *c1).cross(*c2 - *c1);
    else
      n = (*c2 - *c1).cross(*c0 - *c1);
    n.normalize();
    this->normalArray.append(n);

    int idx = *cind++;
    while (idx >= 0) {
      c0 = c1;
      c1 = c2;
      c2 = &coords[idx];
      flip = !flip;
      if (!flip)
        n = (*c0 - *c1).cross(*c2 - *c1);
      else
        n = (*c2 - *c1).cross(*c0 - *c1);
      n.normalize();
      this->normalArray.append(n);
      idx = *cind++;
    }
  }

  if (this->normalArray.getLength()) {
    this->normalData.normals = this->normalArray.getArrayPtr();
    this->numNormals = this->normalArray.getLength();
  }

#if 0 && COIN_DEBUG
  SoDebugError::post("SoNormalCache::generatePerFaceStrip",
                     "generated tristrip normals per face: %p %d",
                     normals, numNormals);
#endif // debug

}

/*!
  Generates one normal per triangle strips (avarages all triangle normals).
*/
void
SoNormalCache::generatePerStrip(const SbVec3f * const coords,
                                const int32_t *cind,
                                const int nv,
                                const SbBool ccw,
                                const int numStrips)
{
  this->clearGenerator();
  this->indices.truncate(0);
  this->normalArray.truncate(0, TRUE);

  const int numalloc = numStrips > 0 ? numStrips : count_strips(cind, nv);
  const int32_t *endptr = cind + nv;

  const SbVec3f *c0, *c1, *c2;
  SbVec3f n;

  SbBool flip = ccw;

  while (cind < endptr) {
    flip = ccw;
    c0 = &coords[*cind++];
    c1 = &coords[*cind++];
    c2 = &coords[*cind++];

    if (!flip)
      n = (*c0 - *c1).cross(*c2 - *c1);
    else
      n = (*c2 - *c1).cross(*c0 - *c1);

    int idx = *cind++;
    while (idx >= 0) {
      c0 = c1;
      c1 = c2;
      c2 = &coords[idx];
      flip = !flip;
      if (!flip)
        n += (*c0 - *c1).cross(*c2 - *c1);
      else
        n += (*c2 - *c1).cross(*c0 - *c1);
      idx = *cind++;
    }
    n.normalize();
    this->normalArray.append(n);
  }
  if (this->normalArray.getLength()) {
    this->normalData.normals = this->normalArray.getArrayPtr();
    this->numNormals = this->normalArray.getLength();
  }
}

/*!
  Generates PER_VERTEX normals for quad data.
*/
void
SoNormalCache::generatePerVertexQuad(const SbVec3f * const coords,
                                     const int vPerRow,
                                     const int vPerColumn,
                                     const SbBool ccw)
{
  this->clearGenerator();
  this->normalArray.truncate(0, TRUE);

  SoNormalCache tempcache(NULL);
  tempcache.generatePerFaceQuad(coords, vPerRow, vPerColumn, ccw);
  const SbVec3f *facenormals = tempcache.getNormals();

#define IDX(r,c) ((r)*(vPerRow-1)+(c))

  for (int i = 0; i < vPerColumn; i++) {
    for (int j = 0; j < vPerRow; j++) {
      SbVec3f n(0,0,0);
      if (i < vPerColumn-1 && j < vPerRow-1) n += facenormals[IDX(i,j)];
      if (i > 0 && j < vPerRow-1) n += facenormals[IDX(i-1,j)];
      if (j > 0 && i > 0) n += facenormals[IDX(i-1, j-1)];
      if (j > 0 && i < vPerColumn-1) n += facenormals[IDX(i, j-1)];
      n.normalize();
      this->normalArray.append(ccw ? -n : n);
    }
  }

#undef IDX

  this->normalData.normals = this->normalArray.getArrayPtr();
  this->numNormals = this->normalArray.getLength();
}

/*!
  Generates per face normals for quad data.
*/
void
SoNormalCache::generatePerFaceQuad(const SbVec3f * const coords,
                                   const int vPerRow,
                                   const int vPerColumn,
                                   const SbBool ccw)
{
  this->clearGenerator();
  this->normalArray.truncate(0, TRUE);

#define IDX(r,c) ((r)*(vPerRow)+(c))

  for (int i = 0; i < vPerColumn-1; i++) {
    for (int j = 0; j < vPerRow-1; j++) {
      int idx = IDX(i,j);
      SbVec3f n = (coords[IDX(i+1,j)]-coords[idx]).cross(coords[IDX(i,j+1)]-coords[idx]);
      n.normalize();
      this->normalArray.append(ccw ? -n : n);
    }
  }

#undef IDX

  if (this->normalArray.getLength()) {
    this->normalData.normals = this->normalArray.getArrayPtr();
    this->numNormals = this->normalArray.getLength();
  }
}

/*!
  Generates per row normals for quad data.
*/
void
SoNormalCache::generatePerRowQuad(const SbVec3f * const coords,
                                  const int vPerRow,
                                  const int vPerColumn,
                                  const SbBool ccw)
{
  this->clearGenerator();
  this->normalArray.truncate(0, TRUE);
  SbVec3f n;

#define IDX(r,c) ((r)*(vPerRow)+(c))

  for (int i = 0; i < vPerColumn-1; i++) {
    n.setValue(0.0f, 0.0f, 0.0f);
    for (int j = 0; j < vPerRow-1; j++) {
      int idx = IDX(i,j);
      n += (coords[IDX(i+1,j)]-coords[idx]).cross(coords[IDX(i,j+1)]-coords[idx]);
    }
    n.normalize();
    this->normalArray.append(ccw ? -n : n);
  }

#undef IDX

  if (this->normalArray.getLength()) {
    this->normalData.normals = this->normalArray.getArrayPtr();
    this->numNormals = this->normalArray.getLength();
  }
}

#undef IDX

//
// frees generator and resets normal data.
//
void
SoNormalCache::clearGenerator()
{
  if (this->numNormals == 0 && this->normalData.generator) {
    delete this->normalData.generator;
  }
  this->normalData.normals = NULL;
  this->numNormals = 0;
}
