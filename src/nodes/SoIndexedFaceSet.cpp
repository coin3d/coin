/**************************************************************************\
 * 
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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
  \class SoIndexedFaceSet SoIndexedFaceSet.h Inventor/nodes/SoIndexedFaceSet.h
  \brief The SoIndexedFaceSet class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoIndexedFaceSet.h>


#include <Inventor/lists/SbPList.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/caches/SoConvexDataCache.h>
#include <Inventor/caches/SoNormalCache.h>
#include <Inventor/misc/SoState.h>

#if !defined(COIN_EXCLUDE_SOMATERIALBUNDLE)
#include <Inventor/bundles/SoMaterialBundle.h>
#endif // !COIN_EXCLUDE_SOMATERIALBUNDLE

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/misc/SoGL.h>
#ifdef _WIN32
#include <windows.h>
#endif // !_WIN32
#include <GL/gl.h>
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SONORMALBINDINGELEMENT)
#include <Inventor/elements/SoNormalBindingElement.h>
#endif // !COIN_EXCLUDE_SONORMALBINDINGELEMENT
#if !defined(COIN_EXCLUDE_SOMATERIALBINDINGELEMENT)
#include <Inventor/elements/SoMaterialBindingElement.h>
#endif // !COIN_EXCLUDE_SOMATERIALBINDINGELEMENT
#if !defined(COIN_EXCLUDE_SOTEXTURECOORDINATEELEMENT)
#include <Inventor/elements/SoTextureCoordinateElement.h>
#endif // !COIN_EXCLUDE_SOTEXTURECOORDINATEELEMENT
#if !defined(COIN_EXCLUDE_SOLIGHTMODELELEMENT)
#include <Inventor/elements/SoLightModelElement.h>
#endif // !COIN_EXCLUDE_SOLIGHTMODELELEMENT
#if !defined(COIN_EXCLUDE_SOCOORDINATEELEMENT)
#include <Inventor/elements/SoCoordinateElement.h>
#endif // !COIN_EXCLUDE_SOCOORDINATEELEMENT
#if !defined(COIN_EXCLUDE_SONORMALELEMENT)
#include <Inventor/elements/SoNormalElement.h>
#endif // !COIN_EXCLUDE_SONORMALELEMENT
#if !defined(COIN_EXCLUDE_SOGLTEXTUREENABLEDELEMENT)
#include <Inventor/elements/SoGLTextureEnabledElement.h>
#endif // !COIN_EXCLUDE_SOGLTEXTUREENABLEDELEMENT
#if !defined(COIN_EXCLUDE_SOSHAPEHINTSELEMENT)
#include <Inventor/elements/SoShapeHintsElement.h>
#endif // !COIN_EXCLUDE_SOSHAPEHINTSELEMENT
#if !defined(COIN_EXCLUDE_SOTEXTURECOORDINATEBINDINGELEMENT)
#include <Inventor/elements/SoTextureCoordinateBindingElement.h>
#endif // !COIN_EXCLUDE_SOTEXTURECOORDINATEBINDINGELEMENT
#include <assert.h>

#include <Inventor/bundles/SoTextureCoordinateBundle.h>


/*!
  \enum SoIndexedFaceSet::Binding
  FIXME: write documentation for enum
*/
/*!
  \var SoIndexedFaceSet::Binding SoIndexedFaceSet::OVERALL
  FIXME: write documentation for enum definition
*/
/*!
  \var SoIndexedFaceSet::Binding SoIndexedFaceSet::PER_FACE
  FIXME: write documentation for enum definition
*/
/*!
  \var SoIndexedFaceSet::Binding SoIndexedFaceSet::PER_FACE_INDEXED
  FIXME: write documentation for enum definition
*/
/*!
  \var SoIndexedFaceSet::Binding SoIndexedFaceSet::PER_VERTEX
  FIXME: write documentation for enum definition
*/
/*!
  \var SoIndexedFaceSet::Binding SoIndexedFaceSet::PER_VERTEX_INDEXED
  FIXME: write documentation for enum definition
*/
/*!
  \var SoIndexedFaceSet::Binding SoIndexedFaceSet::NONE
  FIXME: write documentation for enum definition
*/



// *************************************************************************

SO_NODE_SOURCE(SoIndexedFaceSet);

/*!
  Constructor.
*/
SoIndexedFaceSet::SoIndexedFaceSet()
{
  SO_NODE_CONSTRUCTOR(SoIndexedFaceSet);

  this->numQuads = this->numTriangles = this->numPolygons = -1;
  this->convexCache = NULL;
}


/*!
  Destructor.
*/
SoIndexedFaceSet::~SoIndexedFaceSet()
{
  delete this->convexCache;
}

/*!
  Does initialization common for all objects of the
  SoIndexedFaceSet class. This includes setting up the
  type system, among other things.
*/
void
SoIndexedFaceSet::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoIndexedFaceSet);
}

/*!
  \internal
*/
int
SoIndexedFaceSet::findNumFaces() const
{
  const int32_t * ptr = coordIndex.getValues(0);
  const int32_t * endptr = ptr + coordIndex.getNum();
  int cnt = 0;
  while (ptr < endptr) {
    if (*ptr++ < 0) cnt++;
  }
  return cnt;
} 

/*
  Counts the triangles, quads and polygons in the face set, while at
  the same time checking if the index array is valid. Updates the
  count variables, and returns true if index array is ok.
*/
SbBool
SoIndexedFaceSet::countPrimitives(void)
{
  this->numTriangles = -1;
  this->numQuads = -1;
  this->numPolygons = -1;
  
  int numt, numq, nump;
  numt = numq = nump = 0;
  
  const int32_t * ptr = coordIndex.getValues(0);
  const int32_t * endptr = ptr + coordIndex.getNum();
  int cnt = 0;
  while (ptr < endptr) {
    if (*ptr++ >= 0) cnt++;
    else {
      if (cnt < 3) {
#if COIN_DEBUG
	SoDebugError::postWarning("SoIndexedFaceSet::countPrimitives",
				  "coordIndex field not valid (polygon with %d"
				  " vertices)", cnt);
#endif // COIN_DEBUG
	// FIXME: shouldn't this be handled a bit more gracefully --
	// like just ignoring the offending polygon? 19990405 mortene.
	return FALSE;
      }
      else if (cnt == 3) numt++;
      else if (cnt == 4) numq++;
      else nump++;
    }
  }
  this->numTriangles = numt;
  this->numQuads = numq;
  this->numPolygons = nump;
  return TRUE;
}

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  \internal
*/
SoIndexedFaceSet::Binding 
SoIndexedFaceSet::findMaterialBinding(SoState * const state) const
{
  Binding binding = OVERALL;
#if !defined(COIN_EXCLUDE_SOMATERIALBINDINGELEMENT)
  SoMaterialBindingElement::Binding matbind =
    SoMaterialBindingElement::get(state);
  
  switch (matbind) {
  case SoMaterialBindingElement::OVERALL:
    binding = OVERALL;
    break;
  case SoMaterialBindingElement::PER_VERTEX:
    binding = PER_VERTEX;
    break;
  case SoMaterialBindingElement::PER_VERTEX_INDEXED:
    binding = PER_VERTEX_INDEXED;
    break;
  case SoMaterialBindingElement::PER_PART:
  case SoMaterialBindingElement::PER_FACE:
    binding = PER_FACE;
    break;
  case SoMaterialBindingElement::PER_PART_INDEXED:
  case SoMaterialBindingElement::PER_FACE_INDEXED:
    binding = PER_FACE_INDEXED;
    break;
  default:
#if COIN_DEBUG
    SoDebugError::postWarning("SoIndexedFaceSet::findMaterialBinding",
			      "unknown material binding setting");
#endif // COIN_DEBUG
    break;
  }
#endif // !COIN_EXCLUDE_SOMATERIALBINDINGELEMENT
  return binding;
}


/*!
  \internal
*/
SoIndexedFaceSet::Binding 
SoIndexedFaceSet::findNormalBinding(SoState * const state) const
{
  Binding binding = PER_VERTEX_INDEXED;
#if !defined(COIN_EXCLUDE_SONORMALBINDINGELEMENT)
  SoNormalBindingElement::Binding normbind =
    (SoNormalBindingElement::Binding) SoNormalBindingElement::get(state);
  
  switch (normbind) {
  case SoNormalBindingElement::OVERALL:
    binding = OVERALL;
    break;
  case SoNormalBindingElement::PER_VERTEX:
    binding = PER_VERTEX;
    break;
  case SoNormalBindingElement::PER_VERTEX_INDEXED:
    binding = PER_VERTEX_INDEXED;
    break;
  case SoNormalBindingElement::PER_PART:
  case SoNormalBindingElement::PER_FACE:
    binding = PER_FACE;
    break;
  case SoNormalBindingElement::PER_PART_INDEXED:
  case SoNormalBindingElement::PER_FACE_INDEXED:
    binding = PER_FACE_INDEXED;
    break;
  default:
#if COIN_DEBUG
    SoDebugError::postWarning("SoIndexedFaceSet::findNormalBinding",
			      "unknown normal binding setting");
#endif // COIN_DEBUG
    break;
  }
#endif // !COIN_EXCLUDE_SONORMALBINDINGELEMENT
  return binding;
}
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

/*!
  FIXME: write function documentation
*/
void 
SoIndexedFaceSet::notify(SoNotList * /* list */)
{
  // FIXME: implement what's necessary here (cache
  // destruction?). 19990405 mortene.
}


#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write function documentation
*/
void 
SoIndexedFaceSet::GLRender(SoGLRenderAction * action)
{
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

  if (coordIndex.getNum() && coordIndex[coordIndex.getNum()-1] >= 0) {
    coordIndex.set1Value(coordIndex.getNum(), -1);
  }
  
  Binding mbind = this->findMaterialBinding(state);
  Binding nbind = this->findNormalBinding(state);

  // FIXME: what the puck? 19990405 mortene.
  if (this->numTriangles == -1 &&
      this->numQuads == -1 &&
      this->numPolygons == -1) {
    SbBool ok = this->countPrimitives();
    assert(ok);
  }

  const SoCoordinateElement * coords;
  const SbVec3f * normals;
  const int32_t * cindices;
  int32_t numindices;
  const int32_t * nindices;
  const int32_t * tindices;
  const int32_t * mindices;
  SbBool doTextures;
  SbBool sendNormals;
  SbBool normalCacheUsed;

  getGLData(state, coords, normals, cindices, 
	    nindices, tindices, mindices, numindices, 
	    sendNormals, normalCacheUsed);

  SoTextureCoordinateBundle tb(action, TRUE, FALSE);
  doTextures = tb.needCoordinates();

  if (!sendNormals) nbind = OVERALL;
  else if (nbind == OVERALL) {
    if (normals) glNormal3fv(normals[0].getValue());
    else glNormal3f(0.0f, 0.0f, 1.0f);
  }
  else if (normalCacheUsed && nbind == PER_VERTEX) {
    nbind = PER_VERTEX_INDEXED;
  }  

  Binding tbind = NONE;
  if (doTextures) {
    if (SoTextureCoordinateBindingElement::get(state) ==
	SoTextureCoordinateBindingElement::PER_VERTEX) {
      tbind = PER_VERTEX;
      tindices = NULL;
    }
    else {
      tbind = PER_VERTEX_INDEXED;
      if (tindices == NULL) tindices = cindices;
    }
  }

  if (this->numPolygons > 0 && 
      SoShapeHintsElement::getFaceType(state) != 
      SoShapeHintsElement::CONVEX) {
    if (this->convexCache == NULL) {
      this->convexCache = new SoConvexDataCache(state);
      this->convexCache->generate(coords, cindices, numindices,
				  mindices, nindices, tindices,
				  (SoConvexDataCache::Binding)mbind, 
				  (SoConvexDataCache::Binding)nbind, 
				  (SoConvexDataCache::Binding)tbind);
      
    }
    cindices = this->convexCache->getCoordIndices();
    numindices = this->convexCache->getNumCoordIndices();
    mindices = this->convexCache->getMaterialIndices();
    nindices = this->convexCache->getNormalIndices();
    tindices = this->convexCache->getTexIndices();

    if (mbind == PER_VERTEX) mbind = PER_VERTEX_INDEXED;
    else if (mbind == PER_FACE) mbind = PER_FACE_INDEXED;
    if (nbind == PER_VERTEX) nbind = PER_VERTEX_INDEXED;
    else if (nbind == PER_FACE) nbind = PER_FACE_INDEXED;

    tbind = PER_VERTEX_INDEXED;
  }
  
  SoMaterialBundle mb(action);

  mb.sendFirst(); // make sure we have the correct material
  
  sogl_render_faceset((SoGLCoordinateElement *)coords,
		      cindices,
		      numindices,
		      normals,
		      nindices,
		      &mb,
		      mindices,
		      &tb,
		      tindices,
		      (int)nbind,
		      (int)mbind,
		      doTextures?1:0);

  if (this->vertexProperty.getValue()) {
    state->pop();
  }
}
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write function documentation
*/
void 
SoIndexedFaceSet::generatePrimitives(SoAction * /* action */)
{
  assert(0 && "FIXME: not implemented yet");
}
#endif // !COIN_EXCLUDE_SOACTION

#if !defined(COIN_EXCLUDE_SORAYPICKACTION)
/*!
  FIXME: write function documentation
*/
SoDetail *
SoIndexedFaceSet::createTriangleDetail(SoRayPickAction * /* action */,
				       const SoPrimitiveVertex * /* v1 */,
				       const SoPrimitiveVertex * /* v2 */,
				       const SoPrimitiveVertex * /* v3 */,
				       SoPickedPoint * /* pp */)
{
  assert(0 && "FIXME: not implemented yet");
  return NULL;
}
#endif // !COIN_EXCLUDE_SORAYPICKACTION


#if !defined(COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION)
/*!
  FIXME: write doc
*/
void
SoIndexedFaceSet::getPrimitiveCount(SoGetPrimitiveCountAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION

/*!
  FIXME: write doc
*/
SbBool
SoIndexedFaceSet::generateDefaultNormals(SoState * /* state */,
					 SoNormalBundle * /* nb */)
{
  assert(0 && "FIXME: not implemented");
  return FALSE;
}
