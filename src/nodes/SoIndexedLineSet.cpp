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
  \class SoIndexedLineSet SoIndexedLineSet.h Inventor/nodes/SoIndexedLineSet.h
  \brief The SoIndexedLineSet class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoIndexedLineSet.h>

#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/actions/SoRayPickAction.h>

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
#if !defined(COIN_EXCLUDE_SOGLSHAPEHINTSELEMENT)
#include <Inventor/elements/SoGLShapeHintsElement.h>
#endif // !COIN_EXCLUDE_SOSHAPEHINTSELEMENT
#if !defined(COIN_EXCLUDE_SOTEXTURECOORDINATEBINDINGELEMENT)
#include <Inventor/elements/SoTextureCoordinateBindingElement.h>
#endif // !COIN_EXCLUDE_SOTEXTURECOORDINATEBINDINGELEMENT
#if !defined(COIN_EXCLUDE_SODRAWSTYLEELEMENT)
#include <Inventor/elements/SoDrawStyleElement.h>
#endif
#if !defined(COIN_EXCLUDE_SOGLLIGHTMODELELEMENT)
#include <Inventor/elements/SoGLLightModelElement.h>
#endif // !COIN_EXCLUDE_SOGLLIGHTMODELELEMENT

#include <assert.h>

#include <Inventor/bundles/SoTextureCoordinateBundle.h>
#include <Inventor/details/SoPointDetail.h>
#include <Inventor/details/SoLineDetail.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif

#include <GL/gl.h>


/*!
  \enum SoIndexedLineSet::Binding
  FIXME: write documentation for enum
*/
/*!
  \var SoIndexedLineSet::Binding SoIndexedLineSet::OVERALL
  FIXME: write documentation for enum definition
*/
/*!
  \var SoIndexedLineSet::Binding SoIndexedLineSet::PER_SEGMENT
  FIXME: write documentation for enum definition
*/
/*!
  \var SoIndexedLineSet::Binding SoIndexedLineSet::PER_SEGMENT_INDEXED
  FIXME: write documentation for enum definition
*/
/*!
  \var SoIndexedLineSet::Binding SoIndexedLineSet::PER_LINE
  FIXME: write documentation for enum definition
*/
/*!
  \var SoIndexedLineSet::Binding SoIndexedLineSet::PER_LINE_INDEXED
  FIXME: write documentation for enum definition
*/
/*!
  \var SoIndexedLineSet::Binding SoIndexedLineSet::PER_VERTEX
  FIXME: write documentation for enum definition
*/
/*!
  \var SoIndexedLineSet::Binding SoIndexedLineSet::PER_VERTEX_INDEXED
  FIXME: write documentation for enum definition
*/


// *************************************************************************

SO_NODE_SOURCE(SoIndexedLineSet);

/*!
  Constructor.
*/
SoIndexedLineSet::SoIndexedLineSet()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoIndexedLineSet);
}

/*!
  Destructor.
*/
SoIndexedLineSet::~SoIndexedLineSet()
{
}

/*!
  Does initialization common for all objects of the
  SoIndexedLineSet class. This includes setting up the
  type system, among other things.
*/
void
SoIndexedLineSet::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoIndexedLineSet);
}

/*!
  \internal
*/
int 
SoIndexedLineSet::numLines() const
{
  int i, n = coordIndex.getNum();
  int cnt = 0;
  for (i = 0; i < n ; i++) {
    if (coordIndex.getValues(0)[i] < 0) cnt++;
  }
  return cnt;
}

/*!
  \internal
*/
int 
SoIndexedLineSet::numLineSegments() const
{
  int i = 0, n = coordIndex.getNum();
  int cnt = 0;
  int tmpcnt;
  while (i < n) {
    tmpcnt = 0;
    // i < n not necessary if the array is guaranteed to be terminated
    // with -1.
    while (i < n && coordIndex.getValues(0)[i] >= 0) {
      tmpcnt++;
      i++;
    }
    cnt += tmpcnt - 1;
    i++;
  }
  return cnt;
}

/*!
  \internal
*/
SoIndexedLineSet::Binding 
SoIndexedLineSet::findNormalBinding(SoState * state)
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
    binding = PER_SEGMENT;
    break;
  case SoNormalBindingElement::PER_PART_INDEXED:
    binding = PER_SEGMENT_INDEXED;
    break;
  case SoNormalBindingElement::PER_FACE:
    binding = PER_LINE;
    break;
  case SoNormalBindingElement::PER_FACE_INDEXED:
    binding = PER_LINE_INDEXED;
    break;
  default:
#if COIN_DEBUG
    SoDebugError::postWarning("SoIndexedLineSet::findNormalBinding",
			      "unknown normal binding setting");
#endif // COIN_DEBUG
    break;
  }
#endif // !COIN_EXCLUDE_SONORMALBINDINGELEMENT
  
  return binding;
}

/*!
  \internal
*/
SoIndexedLineSet::Binding 
SoIndexedLineSet::findMaterialBinding(SoState * state)
{
  Binding binding = OVERALL;
  
#if !defined(COIN_EXCLUDE_SOMATERIALBINDINGELEMENT)
  SoMaterialBindingElement::Binding matbind =
    (SoMaterialBindingElement::Binding) SoMaterialBindingElement::get(state);
  
  switch (matbind) {
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
    binding = PER_SEGMENT;
    break;
  case SoNormalBindingElement::PER_PART_INDEXED:
    binding = PER_SEGMENT_INDEXED;
    break;
  case SoNormalBindingElement::PER_FACE:
    binding = PER_LINE;
    break;
  case SoNormalBindingElement::PER_FACE_INDEXED:
    binding = PER_LINE_INDEXED;
    break;
  default:
#if COIN_DEBUG
    SoDebugError::postWarning("SoIndexedFaceSet::findNormalBinding",
			      "unknown normal binding setting");
#endif // COIN_DEBUG
    break;
  }
#endif // !COIN_EXCLUDE_SOMATERIALBINDINGELEMENT
  
  return binding;
}


#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write function documentation
*/
void 
SoIndexedLineSet::GLRender(SoGLRenderAction * action)
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
  
  Binding mbind = this->findMaterialBinding(state);
  Binding nbind = this->findNormalBinding(state);

  const SoCoordinateElement * coords;
  const SbVec3f * normals;
  const int32_t * cindices;
  int32_t numindices;
  const int32_t * nindices;
  const int32_t * tindices;
  const int32_t * mindices;
  SbBool doTextures;
  SbBool sendNormals = TRUE;
  SbBool normalCacheUsed;

#if !defined(COIN_EXCLUDE_SOLIGHTMODELELEMENT)
  sendNormals =
    (SoLightModelElement::get(state) !=
     SoLightModelElement::BASE_COLOR);
#endif // !COIN_EXCLUDE_SOLOGHTMODELELEMENT

  getVertexData(state, coords, normals, cindices, 
		nindices, tindices, mindices, numindices, 
		sendNormals, normalCacheUsed);
  
  if (normals == NULL) sendNormals = FALSE;
  
  SoTextureCoordinateBundle tb(action, TRUE, FALSE);
  doTextures = tb.needCoordinates();

  Binding tbind = PER_VERTEX_INDEXED; // most common
  if (doTextures) {
    if (SoTextureCoordinateBindingElement::get(state) ==
	SoTextureCoordinateBindingElement::PER_VERTEX) {
      tbind = PER_VERTEX;
      tindices = NULL; // just in case
    }
    else if (tindices == NULL) {
      tindices = cindices;
    }
  }

  if (!sendNormals) {
    nbind = OVERALL;
#if !defined(COIN_EXCLUDE_SOGLLIGHTMODELELEMENT)
    const SoGLLightModelElement * lm = (SoGLLightModelElement *)
      state->getConstElement(SoGLLightModelElement::getClassStackIndex());
    lm->forceSend(SoLightModelElement::BASE_COLOR);
#endif // !COIN_EXCLUDE_SOGLLIGHTMODELELEMENT
  }
  else if (nbind == OVERALL) {
    glNormal3fv((const GLfloat *)normals);
  }  
  SoMaterialBundle mb(action);
  mb.sendFirst(); // make sure we have the correct material
  
  SbBool drawPoints = FALSE;
#if !defined(COIN_EXCLUDE_SODRAWSTYLEELEMENT)
  drawPoints = SoDrawStyleElement::get(state) ==
    SoDrawStyleElement::POINTS;
#endif
  
#if !defined(COIN_EXCLUDE_SOGLSHAPEHINTSELEMENT)
  const SoGLShapeHintsElement * sh = (SoGLShapeHintsElement *)
    action->getState()->getConstElement(SoGLShapeHintsElement::getClassStackIndex());
  sh->forceSend(TRUE, FALSE, TRUE); // enable twoside lighting
#endif
  
  sogl_render_lineset((SoGLCoordinateElement *)coords,
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
		      doTextures?1:0,
		      drawPoints?1:0);

  if (this->vertexProperty.getValue()) {
    state->pop();
  }
}

/*!
  FIXME: write function documentation
*/
SbBool 
SoIndexedLineSet::willSetShapeHints(void) const
{
  return TRUE;
}

#endif // COIN_EXCLUDE_SOGLRENDERACTION

/*!
  FIXME: write function documentation
*/
SbBool 
SoIndexedLineSet::generateDefaultNormals(SoState * ,
					 SoNormalCache * nc)
{
  // not possible to generate normals for IndexedLineSet
  nc->set(0, NULL);
  return TRUE;
}


#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
/*!
  FIXME: write doc
*/
void
SoIndexedLineSet::getBoundingBox(SoGetBoundingBoxAction * action)
{
  inherited::getBoundingBox(action);
  // FIXME: tell cache that geometry contain lines
}
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION

#if !defined(COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION)
/*!
  FIXME: write doc
*/
void
SoIndexedLineSet::getPrimitiveCount(SoGetPrimitiveCountAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION

#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write doc
*/
void
SoIndexedLineSet::generatePrimitives(SoAction *action)
{
  SoState * state = action->getState();

  if (this->vertexProperty.getValue()) {
    state->push();
    this->vertexProperty.getValue()->doAction(action);
  }
  
  Binding mbind = this->findMaterialBinding(state);
  Binding nbind = this->findNormalBinding(state);

  const SoCoordinateElement * coords;
  const SbVec3f * normals;
  const int32_t * cindices;
  int32_t numindices;
  const int32_t * normindices;
  const int32_t * texindices;
  const int32_t * matindices;
  SbBool doTextures;
  SbBool sendNormals = TRUE;
  SbBool normalCacheUsed;

  getVertexData(state, coords, normals, cindices, 
		normindices, texindices, matindices, numindices, 
		sendNormals, normalCacheUsed);
  
  if (normals == NULL) {
    sendNormals = FALSE;
    nbind = OVERALL;
  }

  SoTextureCoordinateBundle tb(action, FALSE, FALSE);
  doTextures = tb.needCoordinates();

  Binding tbind = PER_VERTEX_INDEXED; // most common
  if (doTextures) {
    if (SoTextureCoordinateBindingElement::get(state) ==
	SoTextureCoordinateBindingElement::PER_VERTEX) {
      tbind = PER_VERTEX;
      texindices = NULL; // just in case
    }
    else if (texindices == NULL) {
      texindices = cindices;
    }
  }
    
  if (mbind == PER_VERTEX_INDEXED && matindices == NULL) {
    matindices = cindices;
  }
  if (nbind == PER_VERTEX_INDEXED && normindices == NULL) {
    normindices = cindices;
  }
  if (mbind == PER_VERTEX || mbind == PER_LINE || mbind == PER_SEGMENT) {
    matindices = NULL;
  }
  if (nbind == PER_VERTEX || nbind == PER_LINE || nbind == PER_SEGMENT) {
    normindices = NULL;
  }

  if (nbind == OVERALL) normindices = NULL;
  if (mbind == OVERALL) matindices = NULL;

  int matnr = 0;
  int normnr = 0;
  int texidx = 0;
  int32_t i;
  const int32_t *end = cindices + numindices;
  
  SoPrimitiveVertex vertex;
  SoPointDetail pointDetail;
  SoLineDetail lineDetail;

  vertex.setDetail(&pointDetail);

  SbVec3f dummynormal(0.0f, 0.0f, 1.0f);
  const SbVec3f *currnormal = &dummynormal;
  if (normals) currnormal = normals;

  if (nbind == OVERALL) {
    vertex.setNormal(*currnormal);
  }  

  if (mbind == PER_SEGMENT || mbind == PER_SEGMENT_INDEXED ||
      nbind == PER_SEGMENT || nbind == PER_SEGMENT_INDEXED) {
    int previ;
    SbBool matPerPolyline = mbind == PER_LINE || mbind == PER_LINE_INDEXED;
    SbBool normPerPolyline = nbind == PER_LINE || nbind == PER_LINE_INDEXED;
    
    this->beginShape(action, SoShape::LINES, &lineDetail);
    
    while (cindices < end) {
      lineDetail.setLineIndex(0);
      previ = *cindices++;

      if (matPerPolyline) {
	if (matindices) vertex.setMaterialIndex(*matindices++);
	else vertex.setMaterialIndex(matnr++);
	pointDetail.setMaterialIndex(vertex.getMaterialIndex());
      }
      if (normPerPolyline) {
	if (normindices) {
	  pointDetail.setNormalIndex(*normindices);
	  currnormal = &normals[*normindices++];
	}
	else {
	  pointDetail.setNormalIndex(normnr);
	  currnormal = &normals[normnr++];
	}
	vertex.setNormal(*currnormal);
      }
      
      i = *cindices++;
      while (i >= 0) {
	if (!matPerPolyline && mbind != OVERALL) {
	  if (matindices) vertex.setMaterialIndex(*matindices++);
	  else vertex.setMaterialIndex(matnr++);
	  pointDetail.setMaterialIndex(vertex.getMaterialIndex());
	}
	if (!normPerPolyline && nbind != OVERALL) {
	  if (normindices) {
	    pointDetail.setNormalIndex(*normindices);
	    currnormal = &normals[*normindices++];
	  }
	  else {
	    pointDetail.setNormalIndex(normnr);
	    currnormal = &normals[normnr++];
	  }
	  vertex.setNormal(*currnormal);
	}
	if (doTextures) {
	  if (tb.isFunction()) {
	    vertex.setTextureCoords(tb.get(coords->get3(previ), *currnormal));
	  }
	  else {
	    pointDetail.setTextureCoordIndex(texindices?*texindices:texidx);
	    vertex.setTextureCoords(tb.get(texindices?*texindices++:texidx++));
	  }
	}
	pointDetail.setCoordinateIndex(previ);
	vertex.setPoint(coords->get3(previ));
	this->shapeVertex(&vertex);

	if (mbind >= PER_VERTEX) {
	  if (matindices) vertex.setMaterialIndex(*matindices++);
	  else vertex.setMaterialIndex(matnr++);
	  pointDetail.setMaterialIndex(vertex.getMaterialIndex());
	}
	if (nbind >= PER_VERTEX) {
	  if (normindices) {
	    pointDetail.setNormalIndex(*normindices);
	    currnormal = &normals[*normindices++];
	  }
	  else {
	    pointDetail.setNormalIndex(normnr);
	    currnormal = &normals[normnr++];
	  }
	  vertex.setNormal(*currnormal);
	}
	if (doTextures) {
	  if (tb.isFunction()) {
	    vertex.setTextureCoords(tb.get(coords->get3(i), *currnormal));
	  }
	  else {
	    pointDetail.setTextureCoordIndex(texindices?*texindices:texidx);
	    vertex.setTextureCoords(tb.get(texindices?*texindices++:texidx++));
	  }
	}
	pointDetail.setCoordinateIndex(i);
	vertex.setPoint(coords->get3(i));
	this->shapeVertex(&vertex);
	lineDetail.incLineIndex();
	previ = i;
	i = *cindices++;
      }
      lineDetail.incPartIndex();
      if (mbind == PER_VERTEX_INDEXED) matindices++;
      if (nbind == PER_VERTEX_INDEXED) normindices++;
      if (doTextures && texindices) texindices++;
    }
    this->endShape();
    return;
  }
    
  while (cindices < end) {
    this->beginShape(action, LINE_STRIP, &lineDetail);
    lineDetail.setLineIndex(0);
    i = *cindices++;
    assert(i >= 0);
    if (matindices) {
      pointDetail.setMaterialIndex(*matindices);
      vertex.setMaterialIndex(*matindices++);
    }
    else if (mbind != OVERALL) {
      pointDetail.setMaterialIndex(matnr);
      vertex.setMaterialIndex(matnr++);
    }
    if (normindices) {
      pointDetail.setNormalIndex(*normindices);
      currnormal = &normals[*normindices++];
    }
    else if (nbind != OVERALL) {
      pointDetail.setNormalIndex(normnr);
      currnormal = &normals[normnr++];
    }
    vertex.setNormal(*currnormal);
    if (doTextures) {
      if (tb.isFunction()) {
	vertex.setTextureCoords(tb.get(coords->get3(i), *currnormal));
      }
      else {
	pointDetail.setTextureCoordIndex(texindices?*texindices:texidx);
	vertex.setTextureCoords(tb.get(texindices?*texindices++:texidx++));
      }
    }
    pointDetail.setCoordinateIndex(i);
    vertex.setPoint(coords->get3(i));
    this->shapeVertex(&vertex);

    i = *cindices++;
    assert(i >= 0);
    if (mbind >= PER_VERTEX) {
      if (matindices) vertex.setMaterialIndex(*matindices++);
      else vertex.setMaterialIndex(matnr++);
      pointDetail.setMaterialIndex(vertex.getMaterialIndex());
    }
    if (nbind >= PER_VERTEX) {
      if (normindices) {
	pointDetail.setNormalIndex(*normindices);
	currnormal = &normals[*normindices++];
      }
      else {
	pointDetail.setNormalIndex(normnr);
	currnormal = &normals[normnr++];
      }
      vertex.setNormal(*currnormal);
    }
    if (doTextures) {
      if (tb.isFunction()) {
	vertex.setTextureCoords(tb.get(coords->get3(i), *currnormal));
      }
      else {
	pointDetail.setTextureCoordIndex(texindices?*texindices:texidx);
	vertex.setTextureCoords(tb.get(texindices?*texindices++:texidx++));
      }
    }
    pointDetail.setCoordinateIndex(i);
    vertex.setPoint(coords->get3(i));
    this->shapeVertex(&vertex);
    
    i = *cindices++;
    while (i >= 0) {
      assert(cindices <= end);
      if (mbind >= PER_VERTEX) {
	if (matindices) vertex.setMaterialIndex(*matindices++);
	else vertex.setMaterialIndex(matnr++);
	pointDetail.setMaterialIndex(vertex.getMaterialIndex());
      }
      if (nbind >= PER_VERTEX) {
	if (normindices) {
	  pointDetail.setNormalIndex(*normindices);
	  currnormal = &normals[*normindices++];
	}
	else {
	  pointDetail.setNormalIndex(normnr);
	  currnormal = &normals[normnr++];
	}
	vertex.setNormal(*currnormal);
      }
      if (doTextures) {
	if (tb.isFunction()) {
	  vertex.setTextureCoords(tb.get(coords->get3(i), *currnormal));
	}
	else {
	  pointDetail.setTextureCoordIndex(texindices?*texindices:texidx);
	  vertex.setTextureCoords(tb.get(texindices?*texindices++:texidx++));
	}
      }
      pointDetail.setCoordinateIndex(i);
      vertex.setPoint(coords->get3(i));
      this->shapeVertex(&vertex);
      lineDetail.incLineIndex();
      i = *cindices++;
    }
    lineDetail.incPartIndex();
    this->endShape(); // end of line strip
    if (mbind == PER_VERTEX_INDEXED) matindices++;
    if (nbind == PER_VERTEX_INDEXED) normindices++;
    if (doTextures && texindices) texindices++;
  }

  if (this->vertexProperty.getValue()) {
    state->pop();
  }
}
#endif // !COIN_EXCLUDE_SOACTION

#if !defined(COIN_EXCLUDE_SORAYPICKACTION)
/*!
  FIXME: write doc
*/
SoDetail *
SoIndexedLineSet::createLineSegmentDetail(SoRayPickAction * /* action */,
					  const SoPrimitiveVertex * /* v1 */,
					  const SoPrimitiveVertex * /* v2 */,
					  SoPickedPoint * /* pp */)
{
  assert(0 && "FIXME: not implemented");
  return NULL;
}
#endif // !COIN_EXCLUDE_SORAYPICKACTION


