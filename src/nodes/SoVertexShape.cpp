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
  \class SoVertexShape SoVertexShape.h Inventor/nodes/SoVertexShape.h
  \brief The SoVertexShape class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoVertexShape.h>
#include <Inventor/SbName.h>
#include <Inventor/caches/SoNormalCache.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/misc/SoState.h>

#if !defined(COIN_EXCLUDE_SOVERTEXPROPERTY)
#include <Inventor/nodes/SoVertexProperty.h>
#endif // !COIN_EXCLUDE_SOVERTEXPROPERTY

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
#include <Inventor/actions/SoGLRenderAction.h>
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SOLIGHTMODELELEMENT)
#include <Inventor/elements/SoLightModelElement.h>
#endif // !COIN_EXCLUDE_SOLIGHTMODELELEMENT
#if !defined(COIN_EXCLUDE_SONORMALELEMENT)
#include <Inventor/elements/SoNormalElement.h>
#endif // !COIN_EXCLUDE_SONORMALELEMENT
#if !defined(COIN_EXCLUDE_SOGLNORMALIZEELEMENT)
#include <Inventor/elements/SoGLNormalizeElement.h>
#endif // !COIN_EXCLUDE_SOGLNORMALIZEELEMENT
#if !defined(COIN_EXCLUDE_SOCOORDINATEELEMENT)
#include <Inventor/elements/SoCoordinateElement.h>
#endif // !COIN_EXCLUDE_SOCOORDINATEELEMENT
#if !defined(COIN_EXCLUDE_SOGLSHAPEHINTSELEMENT)
#include <Inventor/elements/SoGLShapeHintsElement.h>
#endif // ! COIN_EXCLUDE_SOGLSHAPEHINTSELEMENT

/*!
  \var SoSFNode SoVertexShape::vertexProperty
  FIXME: write documentation for field
*/


//$ BEGIN TEMPLATE NodeAbstractSource(SoVertexShape)
SoType SoVertexShape::classTypeId = SoType::badType();

/*!
  Returns the unique type identifier for the SoVertexShape class.
*/
SoType
SoVertexShape::getClassTypeId(void)
{
  return SoVertexShape::classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoVertexShape::getTypeId(void) const
{
  return SoVertexShape::classTypeId;
}
//$ END TEMPLATE NodeAbstractSource

/*!
  Constructor.
*/
SoVertexShape::SoVertexShape()
{
//$ BEGIN TEMPLATE NodeConstructor(SoVertexShape)
  // Make sure the class has been initialized.
  assert(SoVertexShape::classTypeId != SoType::badType());
//$ END TEMPLATE NodeConstructor
  
  SO_NODE_ADD_FIELD(vertexProperty,(NULL));

  this->normalCache = NULL;
}

/*!
  Destructor.
*/
SoVertexShape::~SoVertexShape()
{
  delete this->normalCache;
}

/*!
  Does initialization common for all objects of the
  SoVertexShape class. This includes setting up the
  type system, among other things.
*/
void 
SoVertexShape::initClass()
{
//$ BEGIN TEMPLATE InitNodeAbstractSource(VertexShape)
  // Make sure we only initialize once.
  assert(SoVertexShape::classTypeId == SoType::badType());
  // Make sure superclass get initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoVertexShape::classTypeId =
    SoType::createType(inherited::getClassTypeId(), "VertexShape",
                       NULL,
                       SoNode::nextActionMethodIndex++);
//$ END TEMPLATE InitNodeAbstractSource
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void 
SoVertexShape::cleanClass()
{
}

/*!
  FIXME: write function documentation
*/
void 
SoVertexShape::notify(SoNotList * /* list */)
{
  // FIXME: cache(s) need invalidation. 19990327 mortene.
}

/*!
  Subclasses should overload this method to generate default
  normals using the SoNormalBundle class. \e TRUE should
  be returned if normals were generated, \e FALSE otherwise.
*/
SbBool 
SoVertexShape::generateDefaultNormals(SoState * ,
				      SoNormalBundle *)
{
  // FIXME: create to SoNormalBundle class.
  return FALSE;
}


/*!
  Subclasses should overload this method to generate default
  normals using the SoNormalCache class. This is more
  effective than using SoNormalGenerator. Return \e TRUE if
  normals were generated, \e FALSE otherwise.
  
  This method is not part of the original OIV API. Do not overload it if
  you intend to create a node which can be used on both Coin and OIV.
*/
SbBool
SoVertexShape::generateDefaultNormals(SoState * /* state */,
				      SoNormalCache * /* nc */)
{
  return FALSE;
}

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write function documentation
*/
SbBool 
SoVertexShape::shouldGLRender(SoGLRenderAction * action)
{
  if (!SoShape::shouldGLRender(action)) return FALSE;
  
  SoState * state = action->getState();

#if !defined(COIN_EXCLUDE_SOLIGHTMODELELEMENT)
  SbBool needNormals =
    (SoLightModelElement::get(state) !=
     SoLightModelElement::BASE_COLOR);
#else // COIN_EXCLUDE_SOLIGHTMODELELEMENT
  SbBool needNormals = FALSE;
#endif // COIN_EXCLUDE_SOLIGHTMODELELEMENT
  
  if (needNormals) {
#if !defined(COIN_EXCLUDE_SONORMALELEMENT)
    const SoNormalElement * elem = SoNormalElement::getInstance(state);
    const SoVertexProperty * vp = 
      (SoVertexProperty *) this->vertexProperty.getValue();
    if (elem->getNum() == 0 && 
	(!vp || vp->normal.getNum() <= 0)) {
      if (this->normalCache == NULL ||  
	  !this->normalCache->isValid(state)) {
	generateNormals(state);
      }
#if !defined(COIN_EXCLUDE_SOGLSHAPEHINTSELEMENT)      
      // if normals are automatically generated, and vertexordering
      // is unknown, force tow-side lighting
      if (SoShapeHintsElement::getVertexOrdering(state) ==
	  SoShapeHintsElement::UNKNOWN_ORDERING) {
	const SoGLShapeHintsElement * sh = (SoGLShapeHintsElement *)
	  state->getConstElement(SoGLShapeHintsElement::getClassStackIndex());
	sh->forceSend(TRUE);
      }
#endif // ! COIN_EXCLUDE_SOGLSHAPEHINTSELEMENT
    }
#endif // !COIN_EXCLUDE_SONORMALELEMENT

#if !defined(COIN_EXCLUDE_SOGLNORMALIZEELEMENT)
    /* will supply unit normals when normal cache is used */
    if (SoVertexShape::willUpdateNormalizeElement(state)) {
      const SoGLNormalizeElement * ne = (SoGLNormalizeElement *)
	state->getConstElement(SoGLNormalizeElement::getClassStackIndex());
      ne->forceSend(TRUE);
    } 
#endif // ! COIN_EXCLUDE_SOGLNORMALIZEELEMENT
    
  }
  return TRUE;
}

/*!
  Will return TRUE if normal cache is used (normals are then known
  to be unit length).
*/
SbBool 
SoVertexShape::willUpdateNormalizeElement(SoState *state) const
{
  const SoNormalElement * elem = SoNormalElement::getInstance(state);
  const SoVertexProperty * vp = 
    (SoVertexProperty *) this->vertexProperty.getValue();
  
  // TODO:
  // could perhaps add an areNormalsUnitLength() to the normal
  // cache also...
  //  

  if (elem->getNum() <= 0 && (!vp || vp->normal.getNum() <= 0) && 
      this->normalCache) return TRUE;
  return FALSE;
}

#endif // !COIN_EXCLUDE_SOGLRENDERACTION

/*!
  FIXME: write function documentation
*/
void 
SoVertexShape::setNormalCache(SoState * const state,
			      const int num,
			      const SbVec3f * normals)
{
  if (this->normalCache == NULL) {
    this->normalCache = new SoNormalCache(state);
  }
  this->normalCache->set(num, normals);
}

/*!
  FIXME: write function documentation
*/
SoNormalCache *
SoVertexShape::getNormalCache() const
{
  return this->normalCache;
}

/*!
  FIXME: write function documentation
*/
void 
SoVertexShape::generateNormals(SoState * const state)
{
  if (this->normalCache == NULL) {
    this->normalCache = new SoNormalCache(state);
  }
  
  //
  // See if the node supports the Coin-way of generating normals
  //
  if (!generateDefaultNormals(state, this->normalCache)) {
    //
    // try to generate normals the old Inventor way, using
    // SoNormalBundle.
    //
    if (!generateDefaultNormals(state, (SoNormalBundle *)NULL)) {
      // FIXME: what now?
    }
  }
}

/*!
  FIXME: write function documentation
*/
void 
SoVertexShape::getGLData(SoState * state,
			 const SoCoordinateElement *& coords,
			 const SbVec3f *& normals,
			 SbBool & needNormals)
{  
  needNormals =
    (SoLightModelElement::get(state) !=
     SoLightModelElement::BASE_COLOR);
  
  coords = SoCoordinateElement::getInstance(state);
  assert(coords);
  
  normals = NULL;
  if (needNormals) {
    normals = SoNormalElement::getArrayPtr(state);
  }
}


#if !defined(COIN_EXCLUDE_SOWRITEACTION)
/*!
  FIXME: write doc
 */
void
SoVertexShape::write(SoWriteAction * /* writeAction */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOWRITEACTION
