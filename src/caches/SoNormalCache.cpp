/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2005 by Systems in Motion.  All rights reserved.
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
 *  See <URL:http://www.coin3d.org/> for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

/*!
  \class SoNormalCache SoNormalCache.h include/Inventor/caches/SoNormalCache.h
  \brief The SoNormalCache class is used to hold cached normals.
  \ingroup caches

  As an extension to the original SGI Open Inventor v2.1 API, it is
  also possible to generate normals using this class.

  It is more powerful and easier to use than the SoNormalGenerator
  class. It is possible to generate normals per vertex with indices
  (using much less memory than plain per vertex normals), and it
  contains special methods to generate normals for triangle strips and
  quads.
*/

// *************************************************************************

#include <float.h> // FLT_EPSILON

#include <Inventor/caches/SoNormalCache.h>

#include <Inventor/misc/SoNormalGenerator.h>
#include <Inventor/lists/SbList.h>
#include <Inventor/errors/SoDebugError.h>

// *************************************************************************

#ifndef DOXYGEN_SKIP_THIS
class SoNormalCacheP {
public:
  int numNormals;
  union {
    const SbVec3f *normals;
    SoNormalGenerator *generator;
  } normalData;
  SbList <int32_t> indices;
  SbList <SbVec3f> normalArray;
};
#endif // DOXYGEN_SKIP_THIS

//
// FIXME: add test to shrink normalArray.
//

#define NORMAL_EPSILON FLT_EPSILON

#undef THIS
#define THIS this->pimpl

#define NORMALCACHE_DEBUG 0 // Set to one for debug output

// *************************************************************************

/*!
  Contructor with \a state being the current state.
*/
SoNormalCache::SoNormalCache(SoState * const state)
  : SoCache(state)
{
  THIS = new SoNormalCacheP;
  THIS->normalData.normals = NULL;
  THIS->numNormals = 0;
}

/*!
  Destructor
*/
SoNormalCache::~SoNormalCache()
{
  this->clearGenerator();
  delete THIS;
}

/*!
  Sets an array of normals for this cache. The normals will not
  be deleted when the instance is deleted.
*/
void
SoNormalCache::set(const int num, const SbVec3f * const normals)
{
  this->clearGenerator();
  THIS->numNormals = num;
  THIS->normalData.normals = normals;
  THIS->indices.truncate(0, TRUE);
  THIS->normalArray.truncate(0, TRUE);
}

/*!
  Uses a normal generator in this cache. The normal generator will
  be deleted when the cache is deleted or reset.
*/
void
SoNormalCache::set(SoNormalGenerator * generator)
{
  this->clearGenerator();
  THIS->indices.truncate(0, TRUE);
  THIS->normalArray.truncate(0, TRUE);
  THIS->numNormals = 0;
  THIS->normalData.generator = generator;
}

/*!
  Returns the number of normals in the cache.
*/
int
SoNormalCache::getNum(void) const
{
  if (THIS->numNormals == 0 && THIS->normalData.generator) {
    return THIS->normalData.generator->getNumNormals();
  }
  return THIS->numNormals;
}

/*!
  Return a pointer to the normals in this cache.
*/
const SbVec3f *
SoNormalCache::getNormals(void) const
{
  if (THIS->numNormals == 0 && THIS->normalData.generator) {
    return THIS->normalData.generator->getNormals();
  }
  return THIS->normalData.normals;
}

/*!
  Returns the number of indices in this cache. Normals are
  generated with PER_VERTEX_INDEXED binding.
*/
int
SoNormalCache::getNumIndices(void) const
{
  return THIS->indices.getLength();
}

/*!
  Returns the normal indices.
*/
const int32_t *
SoNormalCache::getIndices(void) const
{
  if (THIS->indices.getLength()) return THIS->indices.getArrayPtr();
  return NULL;
}

//
// calculates the normal vector for a vertex, based on the
// normal vectors of all incident faces
//
static void
calc_normal_vec(const SbVec3f * facenormals, const int facenum, 
                const int numfacenorm, SbList <int32_t> & faceArray, 
                const float threshold, SbVec3f & vertnormal)
{
  // start with face normal vector
  const SbVec3f * facenormal = & facenormals[facenum];
  vertnormal = *facenormal;

  int n = faceArray.getLength();
  int currface;

  for (int i = 0; i < n; i++) {
    currface = faceArray[i];
    if (currface != facenum) { // check all but this face
      if (currface < numfacenorm || numfacenorm == -1) { // -1 means: assume
        const SbVec3f & normal = facenormals[currface];  // everything is ok
        if ((normal.dot(*facenormal)) > threshold) {
          // smooth towards this face
          vertnormal += normal;
        }
      }
      else {
        static int calc_norm_error = 0;
        if (calc_norm_error < 1) {
          SoDebugError::postWarning("SoNormalCache::calc_normal_vec", "Normals "
                                    "have not been specified for all faces. "
                                    "this warning will only be shown once, "
                                    "but there might be more errors");
        }

        calc_norm_error++;
      }
    }
  }
}

/*!
  Generates normals for each vertex for each face. It is possible to
  specify face normals if these have been calculated somewhere else,
  otherwise the face normals will be calculated before the vertex
  normals are calculated. \a tristrip should be \c TRUE if the
  geometry consists of triangle strips.
*/
void
SoNormalCache::generatePerVertex(const SbVec3f * const coords,
                                 const unsigned int numcoords,
                                 const int32_t * vindex,
                                 const int numvi,
                                 const float crease_angle,
                                 const SbVec3f * facenormals,
                                 const int numfacenormals,
                                 const SbBool ccw,
                                 const SbBool tristrip)
{
#if NORMALCACHE_DEBUG && COIN_DEBUG
  SoDebugError::postInfo("SoNormalCache::generatePerVertex", "generating normals");
#endif

  this->clearGenerator();
  THIS->indices.truncate(0);
  THIS->normalArray.truncate(0);


#if COIN_DEBUG && 0 // debug
  SoDebugError::postInfo("SoNormalCache::generatePerVertex", "%d", numvi);
  for (int vrtidx=0; vrtidx < numvi; vrtidx++)
    fprintf(stdout, "%d ", vindex[vrtidx]);
  fprintf(stdout, "\n");
#endif // debug


  int numfacenorm = numfacenormals;
  SoNormalCache tempcache(NULL);
  const SbVec3f * facenorm = (SbVec3f *) facenormals;
  if (facenorm == NULL) {
    // use a SoNormalCache to store temporary data
    if (tristrip) {
      tempcache.generatePerFaceStrip(coords, numcoords, vindex, numvi, ccw);
    }
    else {
      tempcache.generatePerFace(coords, numcoords, vindex, numvi, ccw);
    }

    facenorm = tempcache.getNormals();
    numfacenorm = tempcache.getNum();

    assert(facenorm && "Normals should be generated for all coords");
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
  SbList<int32_t> * vertexFaceArray = new SbList<int32_t>[maxi+1]; // [0, maxi]

  // for each vertex, store all normals that have been calculated
  SbList <int32_t>* vertexNormalArray = new SbList<int32_t>[maxi+1]; // [0, maxi]

  int numfaces = 0;

  if (tristrip) {
    // Find and save the faces belonging to the different vertices
    i = 0;
    while (i + 2 < numvi) {
      temp = vindex[i];
      if (temp >= 0 && (unsigned int)temp < numcoords) {
        vertexFaceArray[temp].append(numfaces);
      }
      else {
        i = i+1;
        numfaces++;
        continue;
      }

      temp = vindex[i+1];
      if (temp >= 0 && (unsigned int)temp < numcoords) {
        vertexFaceArray[temp].append(numfaces);
      }
      else {
        i = i+2;
        numfaces++;
        continue;
      }

      temp = vindex[i+2];
      if (temp >= 0 && (unsigned int)temp < numcoords) {
        vertexFaceArray[temp].append(numfaces);
      }
      else {
        i = i+3;
        numfaces++;
        continue;
      }

      temp = i+3 < numvi ? vindex[i+3] : -1;
      if (temp < 0 || (unsigned int)temp >= numcoords) {
        i = i + 4; // Jump to next possible face
        numfaces++;
        continue;
      }

      i++;
      numfaces++;
    }
  }
  else { // !tristrip
    for (i = 0; i < numvi; i++) {
      temp = vindex[i];
      if (temp >= 0 && (unsigned int)temp < numcoords) {
        vertexFaceArray[temp].append(numfaces);
      }
      else {
        numfaces++;
      }
    }
  }

  float threshold = (float)cos(SbClamp(crease_angle, 0.0f, (float) M_PI));
  SbBool found;
  int currindex = 0; // current normal index
  int nindex = 0;
  int j, n ;
  int facenum = 0;
  int stripcnt = 0;

  for (i = 0; i < numvi; i++) {
    currindex = vindex[i];
    if (currindex >= 0 && (unsigned int)currindex < numcoords) {
      if (tristrip) {
        if (++stripcnt > 3) facenum++; // next face
      }
      // calc normal for this vertex
      SbVec3f tmpvec;
      calc_normal_vec(facenorm, facenum, numfacenorm, vertexFaceArray[currindex], 
                      threshold, tmpvec);

      // Be robust when it comes to erroneously specified triangles.
      float len = tmpvec.length();

      if (len <= 0.0f) {
        static uint32_t normgenerrors_vertex = 0;
        if (normgenerrors_vertex < 1) {
          SoDebugError::postWarning("SoNormalCache::generatePerVertex","Unable to "
                                    "generate valid normal for face %d", facenum);
        }
        normgenerrors_vertex++;
      }

      if (len > 0.0f) tmpvec /= len;
      else tmpvec.setValue(1.0f, 0.0f, 0.0f); // dummy value

      if (THIS->normalArray.getLength() <= nindex)
        THIS->normalArray.append(tmpvec);
      else
        THIS->normalArray[nindex] = tmpvec;

      // try to find equal normal (total smoothing)
      SbList <int32_t> & array = vertexNormalArray[currindex];
      found = FALSE;
      n = array.getLength();
      int same_normal = -1;
      for (j = 0; j < n && !found; j++) {
        same_normal = array[j];
        found = THIS->normalArray[same_normal].equals(THIS->normalArray[nindex],
                                                      NORMAL_EPSILON);
      }
      if (found)
        THIS->indices.append(same_normal);
      // might be equal to the previous normal (when all normals for a face are equal)
      else if ((nindex > 0) &&
               THIS->normalArray[nindex].equals(THIS->normalArray[nindex-1],
                                                NORMAL_EPSILON)) {
        THIS->indices.append(nindex-1);
      }
      else {
        THIS->indices.append(nindex);
        array.append(nindex);
        nindex++;
      }
    }
    else { // new face
      facenum++;
      stripcnt = 0;
      THIS->indices.append(-1); // add a -1 for PER_VERTEX_INDEXED binding
    }
  }
  if (THIS->normalArray.getLength()) {
    THIS->normalData.normals = THIS->normalArray.getArrayPtr();
    THIS->numNormals = THIS->normalArray.getLength();
  }
#if NORMALCACHE_DEBUG && COIN_DEBUG
  SoDebugError::postInfo("SoNormalCache::generatePerVertex",
                         "generated normals per vertex: %p %d %d\n",
                         THIS->normalData.normals, THIS->numNormals, THIS->indices.getLength());
#endif
  delete [] vertexFaceArray;
  delete [] vertexNormalArray;
}

/*!
  Generates face normals for the faceset defined by \a coords
  and \a cind. 
*/
void
SoNormalCache::generatePerFace(const SbVec3f * const coords,
                               const unsigned int numcoords,
                               const int32_t * cind,
                               const int nv,
                               const SbBool ccw)
{
#if NORMALCACHE_DEBUG && COIN_DEBUG
    SoDebugError::postInfo("SoNormalCache::generatePerFace", "generating normals");
#endif

  this->clearGenerator();
  THIS->indices.truncate(0);
  THIS->normalArray.truncate(0, TRUE);

  const int32_t * cstart = cind;
  const int32_t * endptr = cind + nv;

  SbVec3f tmpvec;

  int maxcoordidx = numcoords - 1;

  while (cind + 2 < endptr) {
    int v0 = cind[0];
    int v1 = cind[1];
    int v2 = cind[2];

    if (v0 < 0 || v1 < 0 || v2 < 0 ||
        v0 > maxcoordidx || v1 > maxcoordidx || v2 > maxcoordidx) {

      SoDebugError::postWarning("SoNormalCache::generatePerFace",
                                "Polygon with less than three valid "
                                "vertices detected. (offset: %d, [%d %d %d]). "
                                "Should be within [0, %d].",
                                cind - cstart, v0, v1, v2, maxcoordidx);

       // Insert dummy normal for robustness
      SbVec3f dummynormal;
      dummynormal.setValue(1.0, 0.0, 0.0);
      THIS->normalArray.append(dummynormal);

      // Skip ahead to next possible index
      if (cind[0] < 0 || cind[0] > maxcoordidx) {
        cind += 1;
      }
      else if (cind[1] < 0 || cind[1] > maxcoordidx) {
        cind += 2;
      }
      else if (cind + 3 < endptr && (cind[2] < 0 || cind[2] > maxcoordidx)) {
        cind += 3;
      }
      else {
        cind += 3; // For robustness check after while loop
        break;
      }

      continue;
    }
    
    if (cind + 3 >= endptr || cind[3] < 0 || cind[3] > maxcoordidx) { // triangle
      if (!ccw)
        tmpvec = (coords[v0] - coords[v1]).cross(coords[v2] - coords[v1]);
      else
        tmpvec = (coords[v2] - coords[v1]).cross(coords[v0] - coords[v1]);

      // Be robust when it comes to erroneously specified triangles.
      float len = tmpvec.length();

      if (len <= 0.0f) {
        static uint32_t normgenerrors_face = 0;
        if (normgenerrors_face < 1) {
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
        normgenerrors_face++;
      }

      if (len > 0.0f) tmpvec /= len;
      else tmpvec.setValue(1.0f, 0.0f, 0.0f); // dummy value
      THIS->normalArray.append(tmpvec);
      cind += 4; // goto next triangle/polygon
    }
    else { // more than 3 vertices
      // use Newell's method to calculate normal vector
      const SbVec3f * vert1, * vert2;
      tmpvec.setValue(0.0f, 0.0f, 0.0f);
      vert2 = coords + v0;
      cind++; // v0 is already read

      // The cind < endptr check makes us robust with regard to a
      // missing "-1" termination of the coordIndex field of the
      // IndexedShape nodetype.
      while (cind < endptr && *cind >= 0 && *cind <= maxcoordidx) {
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

      if (len <= 0.0f) {
        static uint32_t normgenerrors_face = 0;
        if (normgenerrors_face < 1) {
          SoDebugError::postWarning("SoNormalCache::generatePerFace",
                                    "Erroneous polygon specification in model. "
                                    "Unable to generate normal; using dummy normal. "
                                    "(this warning will be printed only once, "
                                    "but there might be more errors).");
        }
        normgenerrors_face++;
      }

      if (len > 0.0f) tmpvec /= len;
      else tmpvec.setValue(1.0f, 0.0f, 0.0f); // dummy value
      THIS->normalArray.append(ccw ? tmpvec : -tmpvec);
      cind++; // skip the -1
    }
  }

  if (endptr - cind > 0) {
    SoDebugError::postWarning("SoNormalCache::generatePerFace", "Face "
                              "specification did not end with a valid "
                              "polygon. Too few points");

    SbVec3f dummynormal;
    dummynormal.setValue(1.0, 0.0, 0.0);
    THIS->normalArray.append(dummynormal);
  }

  if (THIS->normalArray.getLength()) {
    THIS->normalData.normals = THIS->normalArray.getArrayPtr();
    THIS->numNormals = THIS->normalArray.getLength();
  }

#if NORMALCACHE_DEBUG && COIN_DEBUG // debug
  SoDebugError::postInfo("SoNormalCache::generatePerFace",
                         "generated normals per face: %p %d",
                         THIS->normalData.normals, THIS->numNormals);
#endif // debug
}

/*!
  Generates face normals for triangle strips.
*/
void
SoNormalCache::generatePerFaceStrip(const SbVec3f * const coords,
                                    const unsigned int numcoords,
                                    const int32_t * cind,
                                    const int nv,
                                    const SbBool ccw)
{
#if NORMALCACHE_DEBUG && COIN_DEBUG
  SoDebugError::postInfo("SoNormalCache::generatePerFaceStrip", "generating normals");
#endif

  this->clearGenerator();
  THIS->indices.truncate(0);
  THIS->normalArray.truncate(0, TRUE);

  const int32_t * cstart = cind;
  const int32_t * endptr = cind + nv;

  const SbVec3f * c0, * c1, * c2;
  SbVec3f n;

  SbBool flip = ccw;

  const int maxcoordidx = numcoords - 1;

  while (cind + 2 < endptr) {
    if (cind[0] < 0 || cind[1] < 0 || cind[2] < 0 ||
        cind[0] > maxcoordidx || cind[1] > maxcoordidx || cind[2] > maxcoordidx) {

      SoDebugError::postWarning("SoNormalCache::generatePerFaceStrip", "Erroneous "
                                "coordinate index detected (offset: %d, [%d %d %d]). Should be "
                                "within [0, %d].",
                                cind - cstart, *(cind), *(cind+1), *(cind+2), maxcoordidx);

      // Insert dummy normal for robustness
      SbVec3f dummynormal;
      dummynormal.setValue(1.0, 0.0, 0.0);
      THIS->normalArray.append(dummynormal);

      // Skip to next possibly valid index
      if (cind[0] < 0 || cind[0] > maxcoordidx) {
        cind += 1;
      }
      else if (cind[1] < 0 || cind[1] > maxcoordidx) {
        cind += 2;
      }
      else if (cind + 3 < endptr && (cind[2] < 0 || cind[2] > maxcoordidx)) {
        cind += 3;
      }
      else {
        cind += 3; // For robustness check after while loop
        break;
      }

      continue;
    }

    flip = ccw;
    c0 = &coords[*cind++];
    c1 = &coords[*cind++];
    c2 = &coords[*cind++];

    if (!flip)
      n = (*c0 - *c1).cross(*c2 - *c1);
    else
      n = (*c2 - *c1).cross(*c0 - *c1);

    // Be robust when it comes to erroneously specified polygons.
    float len = n.length();
    
    static uint32_t normgenerrors_facestrip = 0;
    if (len <= 0.0f) {
      if (normgenerrors_facestrip < 1) {
        SoDebugError::postWarning("SoNormalCache::generatePerFaceStrip",
                                  "Erroneous triangle specification in model "
                                  "(coords=<%f, %f, %f>, <%f, %f, %f>, <%f, %f, %f>) "
                                  "(this warning will be printed only once, "
                                  "but there might be more errors).",
                                  c0[0][0], c0[0][1], c0[0][2],
                                  c1[0][0], c1[0][1], c1[0][2],
                                  c2[0][0], c2[0][1], c2[0][2]);



      }
      normgenerrors_facestrip++;
    }
    
    if (len > 0.0f) n /= len;
    else n.setValue(1.0f, 0.0f, 0.0f); // dummy value
    THIS->normalArray.append(n);

    int idx = cind < endptr ? *cind++ : -1;
    while (idx >= 0 && idx <= maxcoordidx) {
      c0 = c1;
      c1 = c2;
      c2 = &coords[idx];
      flip = !flip;
      if (!flip)
        n = (*c0 - *c1).cross(*c2 - *c1);
      else
        n = (*c2 - *c1).cross(*c0 - *c1);

      // Be robust when it comes to erroneously specified polygons.
      float len = n.length();

      if (len <= 0.0f) {
        if (normgenerrors_facestrip < 1) {
          SoDebugError::postWarning("SoNormalCache::generatePerFaceStrip",
                                    "Erroneous triangle specification in model "
                                    "(coords=<%f, %f, %f>, <%f, %f, %f>, <%f, %f, %f>) "
                                    "(this warning will be printed only once, "
                                    "but there might be more errors).",
                                    c0[0][0], c0[0][1], c0[0][2],
                                    c1[0][0], c1[0][1], c1[0][2],
                                    c2[0][0], c2[0][1], c2[0][2]);
        }
        normgenerrors_facestrip++;
      }

      if (len > 0.0f) n /= len;
      else n.setValue(1.0f, 0.0f, 0.0f); // dummy value
      THIS->normalArray.append(n);
      idx = cind < endptr ? *cind++ : -1;
    }

    if (idx > maxcoordidx) {
      static uint32_t normgenerrors_facestrip = 0;
      if (normgenerrors_facestrip < 1) {
        SoDebugError::postWarning("SoNormalCache::generatePerFaceStrip",
                                  "Erroneous polygon specification in model. "
                                  "Index out of bounds: %d. Max index: %d. "
                                  "(this warning will be printed only once, "
                                  "but there might be more errors).", 
                                  idx, maxcoordidx);
      }
      normgenerrors_facestrip++;
    }
  }

  if (endptr - cind > 0) {
    SoDebugError::postWarning("SoNormalCache::generatePerFaceStrip", "Strip "
                              "did not end with a valid polygon. Too few "
                              "points");

    SbVec3f dummynormal;
    dummynormal.setValue(1.0, 0.0, 0.0);
    THIS->normalArray.append(dummynormal);
  }

  if (THIS->normalArray.getLength()) {
    THIS->normalData.normals = THIS->normalArray.getArrayPtr();
    THIS->numNormals = THIS->normalArray.getLength();
  }

#if NORMALCACHE_DEBUG && COIN_DEBUG
  SoDebugError::postInfo("SoNormalCache::generatePerFaceStrip",
                         "generated tristrip normals per face: %p %d",
                         THIS->normalData.normals, THIS->numNormals);
#endif // debug

}

/*!
  Generates one normal per triangle strips (avarages all triangle normals).
*/
void
SoNormalCache::generatePerStrip(const SbVec3f * const coords,
                                const unsigned int numcoords,
                                const int32_t * cind,
                                const int nv,
                                const SbBool ccw)
{
#if NORMALCACHE_DEBUG && COIN_DEBUG
  SoDebugError::postInfo("SoNormalCache::generatePerStrip", "generating normals");
#endif

  this->clearGenerator();
  THIS->indices.truncate(0);
  THIS->normalArray.truncate(0, TRUE);

  const int32_t * cstart = cind;
  const int32_t * endptr = cind + nv;

  const SbVec3f * c0, * c1, * c2;
  SbVec3f n;

  SbBool flip = ccw;

  const int maxcoordidx = numcoords - 1;

  while (cind + 2 < endptr) {
    if (cind[0] < 0 || cind[1] < 0 || cind[2] < 0 ||
        cind[0] > maxcoordidx || cind[1] > maxcoordidx || cind[2] > maxcoordidx) {

      SoDebugError::postWarning("SoNormalCache::generatePerStrip", "Erroneous "
                                "coordinate index detected (offset: %d, [%d %d %d]). Should be "
                                "within [0, %d].",
                                cind - cstart, *(cind), *(cind+1), *(cind+2), maxcoordidx);

      // Insert dummy normal for robustness
      SbVec3f dummynormal;
      dummynormal.setValue(1.0, 0.0, 0.0);
      THIS->normalArray.append(dummynormal);

      // Skip to next possibly valid index
      if (cind[0] < 0 || cind[0] > maxcoordidx) {
        cind += 1;
      }
      else if (cind[1] < 0 || cind[1] > maxcoordidx) {
        cind += 2;
      }
      else if (cind + 3 < endptr && (cind[2] < 0 || cind[2] > maxcoordidx)) {
        cind += 3;
      }
      else {
        cind += 3; // For robustness check after while loop
        break;
      }

      continue;
    }
    
    flip = ccw;
    c0 = &coords[*cind++];
    c1 = &coords[*cind++];
    c2 = &coords[*cind++];

    if (!flip)
      n = (*c0 - *c1).cross(*c2 - *c1);
    else
      n = (*c2 - *c1).cross(*c0 - *c1);

    int idx = cind < endptr ? *cind++ : -1;
    while (idx >= 0 && idx <= maxcoordidx) {
      c0 = c1;
      c1 = c2;
      c2 = &coords[idx];
      flip = !flip;
      if (!flip)
        n += (*c0 - *c1).cross(*c2 - *c1);
      else
        n += (*c2 - *c1).cross(*c0 - *c1);
      idx = cind < endptr ? *cind++ : -1;
    }

    if (idx > maxcoordidx) {
      static uint32_t normgenerrors_strip = 0;
      if (normgenerrors_strip < 1) {
        SoDebugError::postWarning("SoNormalCache::generatePerStrip",
                                  "Erroneous polygon specification in model. "
                                  "Index out of bounds: %d. Max index: %d. "
                                  "(this warning will be printed only once, "
                                  "but there might be more errors).", 
                                  idx, maxcoordidx);
      }
      normgenerrors_strip++;
    }

    // Be robust when it comes to erroneously specified polygons.
    float len = n.length();
    
    if (len <= 0.0f) {
      static uint32_t normgenerrors_strip = 0;
      if (normgenerrors_strip < 1) {
        SoDebugError::postWarning("SoNormalCache::generatePerStrip",
                                  "Erroneous polygon specification in model.  "
                                  "Unable to generate non-zero normal. Using "
                                  "dummy normal. "
                                  "(this warning will be printed only once, "
                                  "but there might be more errors).");
      }
      normgenerrors_strip++;
    }
    
    if (len > 0.0f) n /= len;
    else n.setValue(1.0f, 0.0f, 0.0f); // dummy value
    THIS->normalArray.append(n);
  }

  if (endptr - cind > 0) {
    SoDebugError::postWarning("SoNormalCache::generatePerStrip", "Strip did "
                              "not end with a valid polygon. Too few points");

    SbVec3f dummynormal;
    dummynormal.setValue(1.0, 0.0, 0.0);
    THIS->normalArray.append(dummynormal);
  }

  if (THIS->normalArray.getLength()) {
    THIS->normalData.normals = THIS->normalArray.getArrayPtr();
    THIS->numNormals = THIS->normalArray.getLength();
  }

#if NORMALCACHE_DEBUG && COIN_DEBUG
  SoDebugError::postInfo("SoNormalCache::generatePerStrip",
                         "generated normals per strip: %p %d\n",
                         THIS->normalData.normals, THIS->numNormals);
#endif

}

/*!
  Generates PER_VERTEX normals for quad data.
*/
void
SoNormalCache::generatePerVertexQuad(const SbVec3f * const coords,
                                     const unsigned int numcoords,
                                     const int vPerRow,
                                     const int vPerColumn,
                                     const SbBool ccw)
{
#if NORMALCACHE_DEBUG && COIN_DEBUG
  SoDebugError::postInfo("SoNormalCache::generatePerVertexQuad", "generating normals");
#endif

  this->clearGenerator();
  THIS->normalArray.truncate(0, TRUE);

  SoNormalCache tempcache(NULL);
  tempcache.generatePerFaceQuad(coords, numcoords, vPerRow, vPerColumn, ccw);
  const SbVec3f * facenormals = tempcache.getNormals();
  int numfacenormals = tempcache.getNum(); // Used for extra robustness

#define IDX(r, c) ((r)*(vPerRow-1)+(c))

  for (int i = 0; i < vPerColumn; i++) {
    for (int j = 0; j < vPerRow; j++) {
      const int idx1 = IDX(i, j);
      const int idx2 = IDX(i-1, j);
      const int idx3 = IDX(i-1, j-1);
      const int idx4 = IDX(i, j-1);

      SbVec3f n(0, 0, 0);

      if (i < vPerColumn-1 && j < vPerRow-1 && idx1 < numfacenormals) n += facenormals[idx1];
      if (i > 0 && j < vPerRow-1 && idx2 < numfacenormals) n += facenormals[idx2];
      if (j > 0 && i > 0 && idx3 < numfacenormals) n += facenormals[idx3];
      if (j > 0 && i < vPerColumn-1 && idx4 < numfacenormals) n += facenormals[idx4];

      // Be robust when it comes to erroneously specified polygons.
      float len = n.length();

      if (len <= 0.0f) {
        static uint32_t normgenerrors_vertexquad = 0;
        if (normgenerrors_vertexquad < 1) {
          SoDebugError::postWarning("SoNormalCache::generatePerVertexQuad",
                                    "Erroneous polygon specification in model. "
                                    "Unable to generate valid normal, adding dummy. "
                                    "(this warning will be printed only once, "
                                    "but there might be more errors).");
        }
        normgenerrors_vertexquad++;
      }
        
      if (len > 0.0f) n /= len;
      else n.setValue(1.0f, 0.0f, 0.0f); // dummy value
      THIS->normalArray.append(ccw ? -n : n);
    }
  }

#undef IDX

  THIS->normalData.normals = THIS->normalArray.getArrayPtr();
  THIS->numNormals = THIS->normalArray.getLength();

#if NORMALCACHE_DEBUG && COIN_DEBUG
  SoDebugError::postInfo("SoNormalCache::generatePerVertexQuad",
                         "generated normals per vertex quad: %p %d\n",
                         THIS->normalData.normals, THIS->numNormals);
#endif
}

/*!
  Generates per face normals for quad data.
*/
void
SoNormalCache::generatePerFaceQuad(const SbVec3f * const coords,
                                   const unsigned int numcoords,
                                   const int vPerRow,
                                   const int vPerColumn,
                                   const SbBool ccw)
{
#if NORMALCACHE_DEBUG && COIN_DEBUG
  SoDebugError::postInfo("SoNormalCache::generatePerFaceQuad", "generating normals");
#endif

  this->clearGenerator();
  THIS->normalArray.truncate(0, TRUE);

  if (vPerRow <= 1 || vPerColumn <= 1 || 
      (unsigned int)(vPerRow * vPerColumn) > numcoords) {

    SoDebugError::postWarning("SoNormalCache::generatePerFaceQuad", "Illegal "
                              "facequad dimension: [%d %d] with %d coordinates "
                              "available. verticesPerRow and verticesPerColumn "
                              "should be > 1, and verticesPerRow * verticesPerColumn "
                              "<= number of coordinates available.", 
                              vPerRow, vPerColumn, numcoords);
  }

#define IDX(r, c) ((r)*(vPerRow)+(c))

  for (int i = 0; i < vPerColumn-1; i++) {
    for (int j = 0; j < vPerRow-1; j++) {
      const unsigned int idx1 = IDX(i, j);
      const unsigned int idx2 = IDX(i+1, j);
      const unsigned int idx3 = IDX(i, j+1);

      if (idx2 < numcoords) { // Check the largest index only
        SbVec3f n = (coords[idx2] - coords[idx1]).cross(coords[idx3] - coords[idx1]);

        // Be robust when it comes to erroneously specified polygons.
        float len = n.length();

        if (len <= 0.0f) {
          static uint32_t normgenerrors_facequad = 0;
          if (normgenerrors_facequad < 1) {
            SoDebugError::postWarning("SoNormalCache::generatePerFaceQuad",
                                      "Erroneous triangle specification in model "
                                      "(indices= [%d, %d, %d], "
                                      "coords=<%f, %f, %f>, <%f, %f, %f>, <%f, %f, %f>) "
                                      "(this warning will be printed only once, "
                                      "but there might be more errors).",
                                      idx1, idx2, idx3,
                                      coords[idx1][0], coords[idx1][1], coords[idx1][2],
                                      coords[idx2][0], coords[idx2][1], coords[idx2][2],
                                      coords[idx3][0], coords[idx3][1], coords[idx3][2]);
          }
          normgenerrors_facequad++;
        }
        
        if (len > 0.0f) n /= len;
        else n.setValue(1.0f, 0.0f, 0.0f); // dummy value
        THIS->normalArray.append(ccw ? -n : n);
      }
      else {
        // Generate normals even for invalid input
        SbVec3f dummynormal(1.0, 0.0, 0.0);
        THIS->normalArray.append(ccw ? -dummynormal : dummynormal);
      }
    }
  }

#undef IDX

  if (THIS->normalArray.getLength()) {
    THIS->normalData.normals = THIS->normalArray.getArrayPtr();
    THIS->numNormals = THIS->normalArray.getLength();
  }

#if NORMALCACHE_DEBUG && COIN_DEBUG
  SoDebugError::postInfo("SoNormalCache::generatePerFaceQuad",
                         "generated normals per face quad: %p %d\n",
                         THIS->normalData.normals, THIS->numNormals);
#endif

}

/*!
  Generates per row normals for quad data.
*/
void
SoNormalCache::generatePerRowQuad(const SbVec3f * const coords,
                                  const unsigned int numcoords,
                                  const int vPerRow,
                                  const int vPerColumn,
                                  const SbBool ccw)
{
#if NORMALCACHE_DEBUG && COIN_DEBUG
  SoDebugError::postInfo("SoNormalCache::generatePerRowQuad", "generating normals");
#endif

  this->clearGenerator();
  THIS->normalArray.truncate(0, TRUE);
  SbVec3f n;

  if (vPerRow <= 1 || vPerColumn <= 1 || 
      (unsigned int)(vPerRow * vPerColumn) > numcoords) {

    SoDebugError::postWarning("SoNormalCache::generatePerRowQuad", "Illegal "
                              "facequad dimension: [%d %d] with %d coordinates "
                              "available. verticesPerRow and verticesPerColumn "
                              "should be > 1, and verticesPerRow * verticesPerColumn "
                              "<= number of coordinates available.", 
                              vPerRow, vPerColumn, numcoords);
  }

#define IDX(r, c) ((r)*(vPerRow)+(c))

  for (int i = 0; i < vPerColumn-1; i++) {
    n.setValue(0.0f, 0.0f, 0.0f);
    for (int j = 0; j < vPerRow-1; j++) {
      const unsigned int idx1 = IDX(i, j);
      const unsigned int idx2 = IDX(i+1, j);
      const unsigned int idx3 = IDX(i, j+1);

      if (idx2 < numcoords) { // Check largest index only
        n += (coords[idx2] - coords[idx1]).cross(coords[idx3] - coords[idx1]);
      }
    }

    // Be robust when it comes to erroneously specified polygons.
    float len = n.length();

    if (len <= 0.0f) {
      static uint32_t normgenerrors_rowquad = 0;
      if (normgenerrors_rowquad < 1) {
        SoDebugError::postWarning("SoNormalCache::generatePerRowQuad",
                                  "Erroneous polygon specification in model. "
                                  "Unable to generate valid normal, adding dummy. "
                                  "(this warning will be printed only once, "
                                  "but there might be more errors).");
      }
      normgenerrors_rowquad++;
    }
    
    if (len > 0.0f) n /= len;
    else n.setValue(1.0f, 0.0f, 0.0f); // dummy value
    THIS->normalArray.append(ccw ? -n : n);
  }

#undef IDX

  if (THIS->normalArray.getLength()) {
    THIS->normalData.normals = THIS->normalArray.getArrayPtr();
    THIS->numNormals = THIS->normalArray.getLength();
  }

#if NORMALCACHE_DEBUG && COIN_DEBUG
  SoDebugError::postInfo("SoNormalCache::generatePerRowQuad",
                         "generated normals per row quad: %p %d\n",
                         THIS->normalData.normals, THIS->numNormals);
#endif

}

//
// frees generator and resets normal data.
//
void
SoNormalCache::clearGenerator(void)
{
  if (THIS->numNormals == 0 && THIS->normalData.generator) {
    delete THIS->normalData.generator;
  }
  THIS->normalData.normals = NULL;
  THIS->numNormals = 0;
}
