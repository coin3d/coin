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
  \class SoCube SoCube.h Inventor/nodes/SoCube.h
  \brief The SoCube class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoCube.h>


#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/misc/SoState.h>

#if !defined(COIN_EXCLUDE_SOMATERIALBUNDLE)
#include <Inventor/bundles/SoMaterialBundle.h>
#endif // !COIN_EXCLUDE_SOMATERIALBUNDLE

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/misc/SoGL.h>
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SORAYPICKACTION)
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/details/SoCubeDetail.h>
#endif // !COIN_EXCLUDE_SORAYPICKACTION

#if !defined(COIN_EXCLUDE_SOMATERIALBINDINGELEMENT)
#include <Inventor/elements/SoMaterialBindingElement.h>
#endif // !COIN_EXCLUDE_SOMATERIALBINDINGELEMENT
#if !defined(COIN_EXCLUDE_SOLIGHTMODELELEMENT)
#include <Inventor/elements/SoLightModelElement.h>
#endif // !COIN_EXCLUDE_SOLIGHTMODELELEMENT
#if !defined(COIN_EXCLUDE_SOTEXTURECOORDINATEELEMENT)
#include <Inventor/elements/SoTextureCoordinateElement.h>
#endif // !COIN_EXCLUDE_SOTEXTURECOORDINATEELEMENT
#if !defined(COIN_EXCLUDE_SOGLTEXTUREENABLEDELEMENT)
#include <Inventor/elements/SoGLTextureEnabledElement.h>
#endif // !COIN_EXCLUDE_SOGLTEXTUREENABLEDELEMENT
#if !defined(COIN_EXCLUDE_SOGLSHAPEHINTSELEMENT)
#include <Inventor/elements/SoGLShapeHintsElement.h>
#endif // !COIN_EXCLUDE_SOGLSHAPEHINTSELEMENY
#if !defined(COIN_EXCLUDE_SOGLSHADEMODELELEMENT)
#include <Inventor/elements/SoGLShadeModelElement.h>
#endif // !COIN_EXCLUDE_SOGLSHADEMODELELEMENT
#if !defined(COIN_EXCLUDE_SOGLNORMALIZEELEMENT)
#include <Inventor/elements/SoGLNormalizeElement.h>
#endif // !COIN_EXCLUDE_SOGLNORMALIZEELEMENT

#include <assert.h>

/*!
  \var SoSFFloat SoCube::width
  FIXME: write documentation for field
*/
/*!
  \var SoSFFloat SoCube::height
  FIXME: write documentation for field
*/
/*!
  \var SoSFFloat SoCube::depth
  FIXME: write documentation for field
*/


// *************************************************************************

SO_NODE_SOURCE(SoCube);

/*!
  Constructor.
*/
SoCube::SoCube()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoCube);

    SO_NODE_ADD_FIELD(width,(2.0f));
    SO_NODE_ADD_FIELD(height,(2.0f));
    SO_NODE_ADD_FIELD(depth,(2.0f));
}

/*!
  Destructor.
*/
SoCube::~SoCube()
{
}

/*!
  Does initialization common for all objects of the
  SoCube class. This includes setting up the
  type system, among other things.
*/
void
SoCube::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoCube);
}

//
// the 12 triangles in the cube
//
static int vindices[] = 
{
  0, 1, 3,
  0, 3, 2,
  1, 5, 7,
  1, 7, 3,
  5, 4, 6,
  5, 6, 7,
  4, 0, 2,
  4, 2, 6,
  4, 5, 1,
  4, 1, 0,
  2, 3, 7,
  2, 7, 6
};

//
// a cube needs 6 normals
//
static SbVec3f normals[] = 
{
  SbVec3f(0.0f, 0.0f, 1.0f),
  SbVec3f(-1.0f, 0.0f, 0.0f),
  SbVec3f(0.0f, 0.0f, -1.0f),
  SbVec3f(1.0f, 0.0f, 0.0f),
  SbVec3f(0.0f, 1.0f, 0.0f),
  SbVec3f(0.0f, -1.0f, 0.0f),
};


#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write function documentation
*/
void 
SoCube::GLRender(SoGLRenderAction * action)
{
  if (!this->shouldGLRender(action)) return;
  SoState * state = action->getState();

  SbVec3f varray[8];
  this->generateVertices(varray);

#if !defined(COIN_EXCLUDE_SOMATERIALBINDINGELEMENT)
  SoMaterialBindingElement::Binding binding = 
    SoMaterialBindingElement::get(state);
  
  SbBool materialPerPart = 
    (binding == SoMaterialBindingElement::PER_PART ||
     binding == SoMaterialBindingElement::PER_PART_INDEXED);
#else // COIN_EXCLUDE_SOMATERIALBINDINGELEMENT
  SbBool materialPerPart = FALSE;
#endif // COIN_EXCLUDE_SOMATERIALBINDINGELEMENT

#if !defined(COIN_EXCLUDE_SOGLTEXTUREENABLEDELEMENT)
  SbBool doTextures = SoGLTextureEnabledElement::get(state);
#else // COIN_EXCLUDE_SOGLTEXTUREENABLEDELEMENT
  SbBool doTextures = FALSE;
#endif // !COIN_EXCLUDE_SOGLTEXTUREENABLEDELEMENT

#if !defined(COIN_EXCLUDE_SOTEXTURECOORDINATEELEMENT)
  SbBool useTexFunc = 
    (SoTextureCoordinateElement::getType(state) ==
     SoTextureCoordinateElement::FUNCTION);
  
  const SoTextureCoordinateElement * tce;
  if (useTexFunc) 
    tce = SoTextureCoordinateElement::getInstance(state);
#else // COIN_EXCLUDE_SOTEXTURECOORDINATEELEMENT
  SbBool useTexFunc = FALSE;
#endif // COIN_EXCLUDE_SOTEXTURECOORDINATEELEMENT

#if !defined(COIN_EXCLUDE_SOLIGHTMODELELEMENT)
  SbBool sendNormals =
    (SoLightModelElement::get(state) !=
     SoLightModelElement::BASE_COLOR);
#else // COIN_EXCLUDE_SOLIGHTMODELELEMENT
  SbBool sendNormals = FALSE;
#endif // COIN_EXCLUDE_SOLIGHTMODELELEMENT

  SoMaterialBundle mb(action);
  mb.sendFirst();

#if !defined(COIN_EXCLUDE_SOGLSHAPEHINTSELEMENT)
  const SoGLShapeHintsElement * sh = (SoGLShapeHintsElement *)
    action->getState()->getConstElement(SoGLShapeHintsElement::getClassStackIndex());
  sh->forceSend(TRUE, TRUE);
#endif

#if !defined(COIN_EXCLUDE_SOGLSHADEMODELELEMENT)
  const SoGLShadeModelElement * sm = (SoGLShadeModelElement *)
    state->getConstElement(SoGLShadeModelElement::getClassStackIndex());
  sm->forceSend(TRUE);
#endif

#if !defined(COIN_EXCLUDE_SOGLNORMALIZEELEMENT)
  if (sendNormals) {
    const SoGLNormalizeElement * ne = (SoGLNormalizeElement *)
      state->getConstElement(SoGLNormalizeElement::getClassStackIndex());
    ne->forceSend(TRUE);
  }
#endif // !COIN_EXCLUDE_SOGLNORMALIZEELEMENT

  unsigned int flags = 0;
  if (materialPerPart) flags |= SOGL_MATERIAL_PER_PART;
  if (doTextures) flags |= SOGL_NEED_TEXCOORDS;
  if (sendNormals) flags |= SOGL_NEED_NORMALS;

  sogl_render_cube(width.getValue(),
		   height.getValue(),
		   depth.getValue(),
		   &mb,
		   flags);
}

/*!
  FIXME: write function documentation
*/
void 
SoCube::generatePrimitives(SoAction * action)
{
  SoPrimitiveVertex pv;
  SoState * state = action->getState();
  float w, h, d;
  getHalfSize(w, h, d);
  
#if !defined(COIN_EXCLUDE_SOTEXTURECOORDINATEELEMENT)
  SbBool useTexFunc = 
     (SoTextureCoordinateElement::getType(state) ==
      SoTextureCoordinateElement::FUNCTION);
  
  const SoTextureCoordinateElement * tce = NULL;
  SbVec4f texCoord;
  if (useTexFunc) 
    tce = SoTextureCoordinateElement::getInstance(state);
  else {
    texCoord[2] = 0.0f;
    texCoord[3] = 1.0f;
  }
#else // COIN_EXCLUDE_SOTEXTURECOORDINATEELEMENT
  SbBool useTexFunc = FALSE;
  const SoTextureCoordinateElement * tce = NULL;
#endif // COIN_EXCLUDE_SOTEXTURECOORDINATEELEMENT
  
#if !defined(COIN_EXCLUDE_SOMATERIALBINDINGELEMENT)
  // FIXME: varibles not in use? 19990925 mortene.
#if 0 // disabled
  SoMaterialBindingElement::Binding binding = 
    SoMaterialBindingElement::get(state);
  
  SbBool materialPerPart = 
    (binding == SoMaterialBindingElement::PER_PART ||
     binding == SoMaterialBindingElement::PER_PART_INDEXED);
#endif // disabled

#else // COIN_EXCLUDE_SOMATERIALBINDINGELEMENT

  // FIXME: variable not in use? 19990925 mortene.
#if 0 // disabled
  SbBool materialPerPart = FALSE;
#endif // disabled

#endif // COIN_EXCLUDE_SOMATERIALBINDINGELEMENT
  
  SbVec3f varray[8];
  generateVertices(varray);
  int * iptr = vindices;
  SbVec3f * nptr = normals;

  beginShape((SoGLRenderAction *)action, TRIANGLES);
  
  for (int i = 0; i < 6; i++) {
    generateVertex(&pv, varray[*iptr++], useTexFunc, tce, 
		   0.0f, 0.0f, *nptr);
    generateVertex(&pv, varray[*iptr++], useTexFunc, tce, 
		   0.0f, 0.0f, *nptr);
    generateVertex(&pv, varray[*iptr++], useTexFunc, tce, 
		   0.0f, 0.0f, *nptr);
    generateVertex(&pv, varray[*iptr++], useTexFunc, tce, 
		   0.0f, 0.0f, *nptr);
    generateVertex(&pv, varray[*iptr++], useTexFunc, tce, 
		   0.0f, 0.0f, *nptr);
    nptr++;
  }
  endShape();
}

/*!
  FIXME: write function documentation
*/
SbBool 
SoCube::willSetShadeModel(void) const
{
  return TRUE;
}

/*!
  FIXME: write function documentation
*/
SbBool 
SoCube::willSetShapeHints(void) const
{
  return TRUE;
}

//!
SbBool 
SoCube::willUpdateNormalizeElement(SoState *) const
{
  return TRUE;
}

#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
/*!
  FIXME: write function documentation
*/
void 
SoCube::computeBBox(SoAction * /* action */, SbBox3f & box, SbVec3f & center)
{
  center.setValue(0.0f, 0.0f, 0.0f);
  float w,h,d;
  this->getHalfSize(w,h,d);
  box.setBounds(-w, -h, -d, w, h, d);
}
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION


#if !defined(COIN_EXCLUDE_SORAYPICKACTION)
/*!
  FIXME: write function documentation
*/
void 
SoCube::rayPick(SoRayPickAction *action)
{
  if (!shouldRayPick(action)) return;
  
  static int translation[6] = {2,3,5,4,1,0}; // translate into detail part-num
  action->setObjectSpace();
  const SbLine &line = action->getLine();
  float size[3];
  this->getHalfSize(size[0],size[1],size[2]);
#if 0
  fprintf(stderr,"obj line: %g %g %g, %g %g %g\n",
	  line.getPosition()[0], line.getPosition()[1], line.getPosition()[2],
	  line.getDirection()[0], line.getDirection()[1], line.getDirection()[2]);
#endif
  int cnt = 0;
  // test intersection with all six planes
  for (int i = 0; i < 3; i++) {
    for (float j = -1.0f; j <= 1.0f; j += 2.0f) {
      SbVec3f norm(0,0,0);
      norm[i] = j;
      SbVec3f isect;
      
      SbPlane plane(norm, size[i]);
      if (plane.intersect(line, isect)) {
	int i1 = (i+1) % 3;
	int i2 = (i+2) % 3;

	if (isect[i1] >= -size[i1] && isect[i1] <= size[i1] &&
	    isect[i2] >= -size[i2] && isect[i2] <= size[i2] &&
	    action->isBetweenPlanes(isect)) {
	  SoPickedPoint *pp = action->addIntersection(isect);
	  if (pp) {
	    SoCubeDetail *detail = new SoCubeDetail();
	    detail->setPart(translation[cnt]);
	    pp->setDetail(detail, this);
	  }
	}
      }
      cnt++;
    }
  }
}

/*!
  FIXME: write function documentation
*/
SoDetail *
SoCube::createTriangleDetail(SoRayPickAction * /* action */,
			     const SoPrimitiveVertex * /* v1 */,
			     const SoPrimitiveVertex * /* v2 */,
			     const SoPrimitiveVertex * /* v3 */,
			     SoPickedPoint * /* pp */)
{
  assert(0 && "FIXME: not implemented yet");
  return NULL;
}
#endif // !COIN_EXCLUDE_SORAYPICKACTION

/*!
  \internal
*/
void 
SoCube::getHalfSize(float & w, float & h, float & d)
{
  w = (width.isIgnored() ? 1.0f :
       width.getValue() / 2.0f);
  h = (height.isIgnored() ? 1.0f :
       height.getValue() / 2.0f);
  d = (depth.isIgnored() ? 1.0f : 
       depth.getValue() / 2.0f);
}

/*!
  \internal
*/
void 
SoCube::generateVertices(SbVec3f * const varray)
{
  float w, h, d;
  getHalfSize(w, h, d);
  for (int i = 0; i < 8; i++) {
    varray[i].setValue((i & 1) ? -w : w,
		       (i & 2) ? -h : h,
		       (i & 4) ? -d : d);
  }
}

#if !defined(COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION)
/*!
  FIXME: write doc
*/
void
SoCube::getPrimitiveCount(SoGetPrimitiveCountAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION
