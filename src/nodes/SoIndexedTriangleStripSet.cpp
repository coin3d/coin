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
 *  http://www.sim.no/ sales@sim.no Voice: +47 73540378 Fax: +47 73943861
 *
\**************************************************************************/

/*!
  \class SoIndexedTriangleStripSet SoIndexedTriangleStripSet.h Inventor/nodes/SoIndexedTriangleStripSet.h
  \brief The SoIndexedTriangleStripSet class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoIndexedTriangleStripSet.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/SbName.h>

#include <Inventor/lists/SbPList.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/caches/SoNormalCache.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <Inventor/misc/SoState.h>

#if !defined(COIN_EXCLUDE_SOMATERIALBUNDLE)
#include <Inventor/bundles/SoMaterialBundle.h>
#endif // !COIN_EXCLUDE_SOMATERIALBUNDLE

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/misc/SoGL.h>
#include <GL/gl.h>
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SONORMALBINDINGELEMENT)
#include <Inventor/elements/SoNormalBindingElement.h>
#endif // !COIN_EXCLUDE_SONORMALBINDINGELEMENT
#if !defined(COIN_EXCLUDE_SOMATERIALBINDINGELEMENT)
#include <Inventor/elements/SoMaterialBindingElement.h>
#endif // !COIN_EXCLUDE_SOMATERIALBINDINGELEMENT
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
#if !defined(COIN_EXCLUDE_SOCREASEANGLEELEMENT)
#include <Inventor/elements/SoCreaseAngleElement.h>
#endif // !COIN_EXCLUDE_SOCREASEANGLEELEMENT
#if !defined(COIN_EXCLUDE_SOTEXTURECOORDINATEBINDINGELEMENT)
#include <Inventor/elements/SoTextureCoordinateBindingElement.h>
#endif // !COIN_EXCLUDE_SOTEXTURECOORDINATEBINDINGELEMENT

#include <Inventor/bundles/SoTextureCoordinateBundle.h>

#include <assert.h>


/*!
  \enum SoIndexedTriangleStripSet::Binding
  FIXME: write documentation for enum
*/
/*!
  \var SoIndexedTriangleStripSet::Binding SoIndexedTriangleStripSet::OVERALL
  FIXME: write documentation for enum definition
*/
/*!
  \var SoIndexedTriangleStripSet::Binding SoIndexedTriangleStripSet::PER_STRIP
  FIXME: write documentation for enum definition
*/
/*!
  \var SoIndexedTriangleStripSet::Binding SoIndexedTriangleStripSet::PER_STRIP_INDEXED
  FIXME: write documentation for enum definition
*/
/*!
  \var SoIndexedTriangleStripSet::Binding SoIndexedTriangleStripSet::PER_TRIANGLE
  FIXME: write documentation for enum definition
*/
/*!
  \var SoIndexedTriangleStripSet::Binding SoIndexedTriangleStripSet::PER_TRIANGLE_INDEXED
  FIXME: write documentation for enum definition
*/
/*!
  \var SoIndexedTriangleStripSet::Binding SoIndexedTriangleStripSet::PER_VERTEX
  FIXME: write documentation for enum definition
*/
/*!
  \var SoIndexedTriangleStripSet::Binding SoIndexedTriangleStripSet::PER_VERTEX_INDEXED
  FIXME: write documentation for enum definition
*/


// *************************************************************************

//$ BEGIN TEMPLATE NodeSource(SoIndexedTriangleStripSet)
SoType SoIndexedTriangleStripSet::classTypeId = SoType::badType();

/*!
  Returns a new instance of the SoIndexedTriangleStripSet node class.
*/
void *
SoIndexedTriangleStripSet::createInstance(void)
{
  return new SoIndexedTriangleStripSet;
}

/*!
  Returns the unique type identifier for the SoIndexedTriangleStripSet class.
*/
SoType
SoIndexedTriangleStripSet::getClassTypeId(void)
{
  return SoIndexedTriangleStripSet::classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoIndexedTriangleStripSet::getTypeId(void) const
{
  return SoIndexedTriangleStripSet::classTypeId;
}
//$ END TEMPLATE NodeSource

/*!
  Constructor.
*/
SoIndexedTriangleStripSet::SoIndexedTriangleStripSet()
{
//$ BEGIN TEMPLATE NodeConstructor(SoIndexedTriangleStripSet)
  // Make sure the class has been initialized.
  assert(SoIndexedTriangleStripSet::classTypeId != SoType::badType());
//$ END TEMPLATE NodeConstructor

  this->numTriangles = -1;
  this->numStrips = -1;
}

/*!
  Destructor.
*/
SoIndexedTriangleStripSet::~SoIndexedTriangleStripSet()
{
}

/*!
  Does initialization common for all objects of the
  SoIndexedTriangleStripSet class. This includes setting up the
  type system, among other things.
*/
void
SoIndexedTriangleStripSet::initClass(void)
{
//$ BEGIN TEMPLATE InitNodeSource(IndexedTriangleStripSet)
  // Make sure we only initialize once.
  assert(SoIndexedTriangleStripSet::classTypeId == SoType::badType());
  // Make sure superclass get initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoIndexedTriangleStripSet::classTypeId =
    SoType::createType(inherited::getClassTypeId(), "IndexedTriangleStripSet",
                       &SoIndexedTriangleStripSet::createInstance,
                       SoNode::nextActionMethodIndex++);
//$ END TEMPLATE InitNodeSource
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoIndexedTriangleStripSet::cleanClass(void)
{
}

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  \internal
*/
SoIndexedTriangleStripSet::Binding 
SoIndexedTriangleStripSet::findMaterialBinding(SoState * const state) const
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
    binding = PER_STRIP;
    break;
  case SoMaterialBindingElement::PER_FACE:
    binding = PER_TRIANGLE;
    break;
  case SoMaterialBindingElement::PER_PART_INDEXED:
    binding = PER_STRIP_INDEXED;
    break;
  case SoMaterialBindingElement::PER_FACE_INDEXED:
    binding = PER_TRIANGLE_INDEXED;
    break;
  default:
#if COIN_DEBUG
    SoDebugError::postWarning("SoIndexedTriangleStripSet::findMaterialBinding",
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
SoIndexedTriangleStripSet::Binding 
SoIndexedTriangleStripSet::findNormalBinding(SoState * const state) const
{
  Binding binding = PER_VERTEX_INDEXED;
#if !defined(COIN_EXCLUDE_SONORMALBINDINGELEMENT)
  SoNormalBindingElement::Binding normbind = 
    SoNormalBindingElement::get(state);

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
    binding = PER_STRIP;
    break;
  case SoNormalBindingElement::PER_FACE:
    binding = PER_TRIANGLE;
    break;
  case SoNormalBindingElement::PER_PART_INDEXED:
    binding = PER_STRIP_INDEXED;
    break;
  case SoNormalBindingElement::PER_FACE_INDEXED:
    binding = PER_TRIANGLE_INDEXED;
    break;
  default:
#if COIN_DEBUG
    SoDebugError::postWarning("SoIndexedTriangleStripSet::findNormalBinding",
			      "unknown normal binding setting");
#endif // COIN_DEBUG
    break;
  }
#endif // !COIN_EXCLUDE_SONORMALBINDINGELEMENT
  return binding;
}

/*!
  FIXME: write function documentation
*/
void 
SoIndexedTriangleStripSet::GLRender(SoGLRenderAction * action)
{
  SoState * state = action->getState();

  if (this->vertexProperty.getValue()) {
    state->push();
    this->vertexProperty.getValue()->GLRender(action);
  }
  

  if (!this->shouldGLRender(action)) {
    if (this->vertexProperty.getValue()) {
      state->pop();
    }
    return;
  }

  Binding mbind = this->findMaterialBinding(state);
  Binding nbind = this->findNormalBinding(state);

  // FIXME: what the puck? 19990405 mortene.
  if (this->numTriangles == -1) {
    this->getNumTriangles();
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

  Binding tbind = PER_VERTEX_INDEXED; // most common
  if (doTextures) {
    if (SoTextureCoordinateBindingElement::get(state) ==
	SoTextureCoordinateBindingElement::PER_VERTEX) {
      tbind = PER_VERTEX;
      tindices = NULL;
    }
    else if (tindices == NULL) {
      tindices = cindices;
    }
  }

  if (!sendNormals) nbind = OVERALL;
  else if (nbind == OVERALL) {
    if (normals) glNormal3fv(normals[0].getValue());
    else glNormal3f(0.0f, 0.0f, 1.0f);
  }
  else if (normalCacheUsed && nbind == PER_VERTEX) {
    nbind == PER_VERTEX_INDEXED;
  }  
  
  SoMaterialBundle mb(action);

  mb.sendFirst(); // make sure we have the correct material
  
  sogl_render_tristrip((SoGLCoordinateElement *)coords,
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

/*!
  FIXME: write function documentation
*/
SbBool
SoIndexedTriangleStripSet::generateDefaultNormals(SoState * state,
						  SoNormalCache * nc)
{
  SbBool ccw = TRUE;
  if (SoShapeHintsElement::getVertexOrdering(state) ==
      SoShapeHintsElement::CLOCKWISE) ccw = FALSE;
  
  SoVertexProperty * vp = (SoVertexProperty *) this->vertexProperty.getValue();
  assert(!vp ||
	 vp->getTypeId().isDerivedFrom(SoVertexProperty::getClassTypeId()));
  SbBool vpvtx = vp && (vp->vertex.getNum() > 0);
  SbBool vpnorm = vp && (vp->normal.getNum() > 0);

  const SbVec3f * coords = vpvtx ?
    vp->vertex.getValues(0) :
    SoCoordinateElement::getArrayPtr3(state);
  assert(coords);
  
#if !defined(COIN_EXCLUDE_SONORMALBINDINGELEMENT)
  SoNormalBindingElement::Binding normbind = vpnorm ?
    (SoNormalBindingElement::Binding) vp->normalBinding.getValue() :
    SoNormalBindingElement::get(state);
  
  
  switch (normbind) {
  case SoNormalBindingElement::PER_VERTEX:
  case SoNormalBindingElement::PER_VERTEX_INDEXED:
    nc->generatePerVertex(coords, 
			  coordIndex.getValues(0),
			  coordIndex.getNum(),
#if !defined(COIN_EXCLUDE_SOCREASEANGLEELEMENT)
			  SoCreaseAngleElement::get(state),
#else // COIN_EXCLUDE_SOCREASEANGLEELEMENT
			  0.0f, // default element value (0.5 is nicer though)
#endif // COIN_EXCLUDE_SOCREASEANGLEELEMENT
			  NULL,
			  ccw,
			  -1,
			  TRUE);
    break;
  case SoNormalBindingElement::PER_FACE:
  case SoNormalBindingElement::PER_FACE_INDEXED:
    nc->generatePerFaceStrip(coords,
			     coordIndex.getValues(0),
			     coordIndex.getNum(),
			     ccw,
			     getNumTriangles());
    break;

  case SoNormalBindingElement::PER_PART:
  case SoNormalBindingElement::PER_PART_INDEXED:
    nc->generatePerStrip(coords,
			 coordIndex.getValues(0),
			 coordIndex.getNum(),
			 ccw,
			 getNumStrips());
    break;
  default:
    break;
  }
#else // COIN_EXCLUDE_SONORMALBINDINGELEMENT
  // As for PER_VERTEX_INDEXED (default value).
  nc->generatePerVertex(coords, 
			coordIndex.getValues(0),
			coordIndex.getNum(),
#if !defined(COIN_EXCLUDE_SOCREASEANGLEELEMENT)
			SoCreaseAngleElement::get(state),
#else // COIN_EXCLUDE_SOCREASEANGLEELEMENT
			0.0f, // default element value
#endif // COIN_EXCLUDE_SOCREASEANGLEELEMENT
			NULL,
			ccw,
			-1,
			TRUE);
#endif // COIN_EXCLUDE_SONORMALBINDINGELEMENT
  return TRUE;
}
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

/*!
  FIXME: write function documentation
*/
int
SoIndexedTriangleStripSet::getNumTriangles()
{
  if (numTriangles == -1) {
    const int32_t * ptr = coordIndex.getValues(0);
    const int32_t * endptr = ptr + coordIndex.getNum();
    int cnt = 0;
    int stripcnt = 0;
    int32_t idx;
    while (ptr < endptr) {
      idx = *ptr++;
      assert(idx >= 0);
      idx = *ptr++;
      assert(idx >= 0);
      idx = *ptr++;
      assert(idx >= 0);
      cnt++;

      idx = *ptr++;
      while (idx >= 0) {
	assert(ptr < endptr);
	cnt++;
	idx = *ptr++;
      }
      stripcnt++;
    }
    this->numTriangles = cnt;
    this->numStrips = stripcnt;
  }
  return this->numTriangles;
}

/*!
  FIXME: write function documentation
*/
int
SoIndexedTriangleStripSet::getNumStrips()
{
  getNumTriangles();
  return this->numStrips;
}


#if !defined(COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION)
/*!
  FIXME: write doc
*/
void
SoIndexedTriangleStripSet::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION

/*!
  FIXME: write doc
*/
SbBool
SoIndexedTriangleStripSet::generateDefaultNormals(SoState * state, SoNormalBundle * nb)
{
  assert(0 && "FIXME: not implemented");
}

#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write doc
*/
void
SoIndexedTriangleStripSet::generatePrimitives(SoAction * action)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOACTION

#if !defined(COIN_EXCLUDE_SOPRIMITIVEVERTEX)
/*!
  FIXME: write doc
*/
SoDetail *
SoIndexedTriangleStripSet::createTriangleDetail(SoRayPickAction * action,
						const SoPrimitiveVertex * v1,
						const SoPrimitiveVertex * v2,
						const SoPrimitiveVertex * v3,
						SoPickedPoint * pp)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOPRIMITIVEVERTEX
