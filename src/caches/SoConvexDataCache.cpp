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
  \class SoConvexDataCache include/Inventor/caches/SoConvexDataCache.h
  \brief The SoConvexDataCache class is used to cache convexified polygons.

  This class is not part of the original OIV API. It is used to
  speed up rendering of concave polygons by tesselating all
  polygons into triangles.
*/

#include <Inventor/caches/SoConvexDataCache.h>
#include <Inventor/elements/SoCoordinateElement.h>
#include <Inventor/SbTesselator.h>
#include <Inventor/SbMatrix.h>
#include <Inventor/errors/SoDebugError.h>
#include <assert.h>


/*!
  \enum SoConvexDataCache::Binding
  \brief The Binding enum is used to specify bindings.

  Binding applies to normals, materials and texture coordinates.
*/


/*!
  Constructor with \a state being the current state.
*/
SoConvexDataCache::SoConvexDataCache(SoState * const state)
  : SoCache(state)
{
}

/*!
  Destructor.
*/
SoConvexDataCache::~SoConvexDataCache()
{
}

/*!
  Returns a pointer to the convexified coordinate indices.
  \sa SoConvexDataCache::getNumCoordIndices()
*/
const int32_t *
SoConvexDataCache::getCoordIndices(void) const
{
  if (this->coordIndices.getLength()) return this->coordIndices.getArrayPtr();
  return NULL;
}

/*!
  Returns the number of coordinate indices.
  \sa SoConvexDataCache::getCoordIndices()
*/
int
SoConvexDataCache::getNumCoordIndices(void) const
{
  return this->coordIndices.getLength();
}

/*!
  Returns the convexified material indices.
  \sa SoConvexDataCache::getNumMaterialIndices()
*/
const int32_t *
SoConvexDataCache::getMaterialIndices(void) const
{
  if (this->materialIndices.getLength()) return this->materialIndices.getArrayPtr();
  return NULL;
}

/*!
  Returns the number of material indices.
  \sa SoConvexDataCache::getMaterialIndices()
*/
int
SoConvexDataCache::getNumMaterialIndices(void) const
{
  return this->materialIndices.getLength();
}

/*!
  Returns the convexified normal indices.
  \sa SoConvexDataCache::getNumNormalIndices()
*/
const int32_t *
SoConvexDataCache::getNormalIndices(void) const
{
  if (this->normalIndices.getLength()) return this->normalIndices.getArrayPtr();
  return NULL;
}

/*!
  Returns the number of normal indices.
  \sa SoConvexDataCache::getNormalIndices()
*/
int
SoConvexDataCache::getNumNormalIndices(void) const
{
  return this->normalIndices.getLength();
}

/*!
  Returns the convexified texture coordinate indices.
  \sa SoConvexDataCache::getNumTexIndices()
*/
const int32_t *
SoConvexDataCache::getTexIndices(void) const
{
  if (this->texIndices.getLength()) return this->texIndices.getArrayPtr();
  return NULL;
}

/*!
  Returns the number of texture coordinate indices.
  \sa SoConvexDataCache::getTexIndices()
*/
int
SoConvexDataCache::getNumTexIndices(void) const
{
  return this->texIndices.getLength();
}


typedef struct
{
  int  matnr;
  int  texnr;
  int  normnr;
  int  vertexnr;
} tVertexInfo;

// callback function
static void do_triangle(void *vo, void *v1, void *v2, void *data);

//
// struct used to hold data for the tessellator callback
//
typedef struct {
  SbBool firstvertex;
  tVertexInfo *vertexInfo;
  SoConvexDataCache::Binding matbind;
  SoConvexDataCache::Binding normbind;
  SoConvexDataCache::Binding texbind;

  SbList <int32_t> *vertexIndex;
  SbList <int32_t> *matIndex;
  SbList <int32_t> *normIndex;
  SbList <int32_t> *texIndex;
  int numvertexind;
  int nummatind;
  int numnormind;
  int numtexind;
} tTessData;

/*!
  Generates the convexified data. FIXME: doc
*/
void
SoConvexDataCache::generate(const SoCoordinateElement * const coords,
                            const SbMatrix & matrix,
                            const int32_t *vind,
                            const int numv,
                            const int32_t *mind, const int32_t *nind,
                            const int32_t *tind,
                            const Binding matbind, const Binding normbind,
                            const Binding texbind)
{
#if COIN_DEBUG
  SoDebugError::postInfo("SoConvexDataCache::generate",
                         "generating convex data");
#endif

  SbBool identity = matrix == SbMatrix::identity();

  // remove old data
  this->coordIndices.truncate(0);
  this->materialIndices.truncate(0);
  this->normalIndices.truncate(0);
  this->texIndices.truncate(0);

  int matnr = 0;
  int texnr = 0;
  int normnr = 0;

  // initialize the struct with data needed during tessellation
  tTessData tessdata;
  tessdata.matbind = matbind;
  tessdata.normbind = normbind;
  tessdata.texbind = texbind;
  tessdata.numvertexind = 0;
  tessdata.nummatind = 0;
  tessdata.numnormind = 0;
  tessdata.numtexind = 0;
  // FIXME: stupid to have a separate struct for each coordIndex
  // should only allocate enough to hold the largest polygon
  tessdata.vertexInfo = new tVertexInfo[numv];
  tessdata.vertexIndex = NULL;
  tessdata.matIndex = NULL;
  tessdata.normIndex = NULL;
  tessdata.texIndex = NULL;
  tessdata.firstvertex = TRUE;

  // create tessellator
  SbTesselator tessellator(do_triangle, &tessdata);

  // if PER_FACE binding, the binding must change to PER_FACE_INDEXED
  // if convexify data is used.
  tessdata.vertexIndex = &this->coordIndices;
  if (matbind != NONE)
    tessdata.matIndex = &this->materialIndices;
  if (normbind != NONE)
    tessdata.normIndex = &this->normalIndices;
  if (texbind != NONE)
    tessdata.texIndex = &this->texIndices;

  tessellator.beginPolygon();
  for (int i = 0; i < numv; i++) {
    if (vind[i] < 0) {
      tessellator.endPolygon();
      if (matbind != PER_VERTEX) matnr++;
      if (normbind != PER_VERTEX) normnr++;
      if (texbind == PER_VERTEX_INDEXED) texnr++;
      if (i < numv - 1) { // if not last polygon
        tessellator.beginPolygon();
      }
    }
    else {
      tessdata.vertexInfo[i].vertexnr = vind[i];
      if (mind)
        tessdata.vertexInfo[i].matnr = mind[matnr];
      else tessdata.vertexInfo[i].matnr = matnr;
      if (matbind >= PER_VERTEX) {
        matnr++;
      }
      if (nind)
        tessdata.vertexInfo[i].normnr = nind[normnr];
      else tessdata.vertexInfo[i].normnr = normnr;
      if (normbind >= PER_VERTEX)
        normnr++;
      if (tind)
        tessdata.vertexInfo[i].texnr = tind[texnr++];
      else
        tessdata.vertexInfo[i].texnr = texnr++;

      SbVec3f v = coords->get3(vind[i]);
      if (!identity) matrix.multVecMatrix(v,v);
      tessellator.addVertex(v,
                            (void*)&tessdata.vertexInfo[i]);
    }
  }

  delete [] tessdata.vertexInfo;

  // get rid of wasted memory. Is not absolutely necessary,
  // and it will be faster not to do this.
//   this->coordIndices.fit();
//   if (tessdata.matIndex) this->materialIndices.fit();
//   if (tessdata.normIndex) this->normalIndices.fit();
//   if (tessdata.texIndex) this->texIndices.fit();
}

//
// helper function for do_triangle() below
//
static void
vertex_tri(tVertexInfo *info, tTessData *tessdata)
{
  tessdata->vertexIndex->append(info->vertexnr);
  tessdata->numvertexind++;

  if (tessdata->matIndex &&
      (tessdata->firstvertex ||
       tessdata->matbind >= SoConvexDataCache::PER_VERTEX)) {
    tessdata->matIndex->append(info->matnr);
    tessdata->nummatind++;
  }

  if (tessdata->normIndex &&
      (tessdata->firstvertex ||
       tessdata->normbind >= SoConvexDataCache::PER_VERTEX)) {
    tessdata->normIndex->append(info->normnr);
    tessdata->numnormind++;
  }
  if (tessdata->texIndex &&
      tessdata->texbind != SoConvexDataCache::NONE) {
    tessdata->texIndex->append(info->texnr);
    tessdata->numtexind++;
  }
  tessdata->firstvertex = FALSE;
}

//
// handles callbacks from SbTesselator
//
static void
do_triangle(void *v0, void *v1, void *v2, void *data)
{
  tTessData *tessdata = (tTessData*)data;
  tessdata->firstvertex = TRUE;
  vertex_tri((tVertexInfo*)v0, tessdata);
  vertex_tri((tVertexInfo*)v1, tessdata);
  vertex_tri((tVertexInfo*)v2, tessdata);

  tessdata->vertexIndex->append(-1);
  if (tessdata->matIndex &&
      tessdata->matbind >= SoConvexDataCache::PER_VERTEX) {
    tessdata->matIndex->append(-1);
    tessdata->nummatind++;
  }
  if (tessdata->normIndex &&
      tessdata->normbind >= SoConvexDataCache::PER_VERTEX) {
    tessdata->normIndex->append(-1);
    tessdata->numnormind++;
  }
  if (tessdata->texIndex &&
      tessdata->texbind != SoConvexDataCache::NONE) {
    tessdata->texIndex->append(-1);
    tessdata->numtexind++;
  }
}
