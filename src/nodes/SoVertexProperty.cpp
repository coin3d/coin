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
  \class SoVertexProperty SoVertexProperty.h Inventor/nodes/SoVertexProperty.h
  \brief The SoVertexProperty class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoVertexProperty.h>
#include <Inventor/nodes/SoSubNode.h>

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION
#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
#include <Inventor/actions/SoGLRenderAction.h>
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SOCOORDINATEELEMENT)
#include <Inventor/elements/SoCoordinateElement.h>
#endif // !COIN_EXCLUDE_SOCOORDINATEELEMENT
#if !defined(COIN_EXCLUDE_SOGLCOORDINATEELEMENT)
#include <Inventor/elements/SoGLCoordinateElement.h>
#endif // !COIN_EXCLUDE_SOGLCOORDINATEELEMENT
#if !defined(COIN_EXCLUDE_SOMATERIALBINDINGELEMENT)
#include <Inventor/elements/SoMaterialBindingElement.h>
#endif // !COIN_EXCLUDE_SOMATERIALBINDINGELEMENT
#if !defined(COIN_EXCLUDE_SONORMALBINDINGELEMENT)
#include <Inventor/elements/SoNormalBindingElement.h>
#endif // !COIN_EXCLUDE_SONORMALBINDINGELEMENT
#if !defined(COIN_EXCLUDE_SOGLNORMALELEMENT)
#include <Inventor/elements/SoGLNormalElement.h>
#endif // !COIN_EXCLUDE_SOGLNORMALELEMENT
#if !defined(COIN_EXCLUDE_SOGLTEXTURECOORDINATEELEMENT)
#include <Inventor/elements/SoGLTextureCoordinateElement.h>
#endif // !COIN_EXCLUDE_SOGLTEXTURECOORDINATEELEMENT
#if !defined(COIN_EXCLUDE_SODIFFUSECOLORELEMENT)
#include <Inventor/elements/SoDiffuseColorElement.h>
#endif 

/*!
  \enum SoVertexProperty::Binding
  FIXME: write documentation for enum
*/
/*!
  \var SoVertexProperty::Binding SoVertexProperty::OVERALL
  FIXME: write documentation for enum definition
*/
/*!
  \var SoVertexProperty::Binding SoVertexProperty::PER_PART
  FIXME: write documentation for enum definition
*/
/*!
  \var SoVertexProperty::Binding SoVertexProperty::PER_PART_INDEXED
  FIXME: write documentation for enum definition
*/
/*!
  \var SoVertexProperty::Binding SoVertexProperty::PER_FACE
  FIXME: write documentation for enum definition
*/
/*!
  \var SoVertexProperty::Binding SoVertexProperty::PER_FACE_INDEXED
  FIXME: write documentation for enum definition
*/
/*!
  \var SoVertexProperty::Binding SoVertexProperty::PER_VERTEX
  FIXME: write documentation for enum definition
*/
/*!
  \var SoVertexProperty::Binding SoVertexProperty::PER_VERTEX_INDEXED
  FIXME: write documentation for enum definition
*/


/*!
  \var SoMFVec3f SoVertexProperty::vertex
  FIXME: write documentation for field
*/
/*!
  \var SoMFVec2f SoVertexProperty::texCoord
  FIXME: write documentation for field
*/
/*!
  \var SoMFVec3f SoVertexProperty::normal
  FIXME: write documentation for field
*/
/*!
  \var SoSFEnum SoVertexProperty::normalBinding
  FIXME: write documentation for field
*/
/*!
  \var SoMFUInt32 SoVertexProperty::orderedRGBA
  FIXME: write documentation for field
*/
/*!
  \var SoSFEnum SoVertexProperty::materialBinding
  FIXME: write documentation for field
*/


//$ BEGIN TEMPLATE NodeSource(SoVertexProperty)
SoType SoVertexProperty::classTypeId = SoType::badType();

/*!
  Returns a new instance of the SoVertexProperty node class.
*/
void *
SoVertexProperty::createInstance(void)
{
  return new SoVertexProperty;
}

/*!
  Returns the unique type identifier for the SoVertexProperty class.
*/
SoType
SoVertexProperty::getClassTypeId(void)
{
  return SoVertexProperty::classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoVertexProperty::getTypeId(void) const
{
  return SoVertexProperty::classTypeId;
}
//$ END TEMPLATE NodeSource

/*!
  Constructor.
*/
SoVertexProperty::SoVertexProperty()
{
//$ BEGIN TEMPLATE NodeConstructor(SoVertexProperty)
  // Make sure the class has been initialized.
  assert(SoVertexProperty::classTypeId != SoType::badType());
//$ END TEMPLATE NodeConstructor

  SO_NODE_ADD_FIELD(vertex, (0));
  SO_NODE_ADD_FIELD(texCoord, (0));
  SO_NODE_ADD_FIELD(normal, (0));
  SO_NODE_ADD_FIELD(orderedRGBA, (0));
  // FIXME: is this the correct way of making empty multifields?
  // 19990301 mortene.
  vertex.setNum(0);
  texCoord.setNum(0);
  normal.setNum(0);
  orderedRGBA.setNum(0);

  SO_NODE_ADD_FIELD(normalBinding, (SoVertexProperty::PER_VERTEX_INDEXED));
  SO_NODE_ADD_FIELD(materialBinding, (SoVertexProperty::OVERALL));

  SO_NODE_DEFINE_ENUM_VALUE(Binding, OVERALL);
  SO_NODE_DEFINE_ENUM_VALUE(Binding, PER_PART);
  SO_NODE_DEFINE_ENUM_VALUE(Binding, PER_PART_INDEXED);
  SO_NODE_DEFINE_ENUM_VALUE(Binding, PER_FACE);
  SO_NODE_DEFINE_ENUM_VALUE(Binding, PER_FACE_INDEXED);
  SO_NODE_DEFINE_ENUM_VALUE(Binding, PER_VERTEX);
  SO_NODE_DEFINE_ENUM_VALUE(Binding, PER_VERTEX_INDEXED);

  SO_NODE_SET_SF_ENUM_TYPE(normalBinding, Binding);
  SO_NODE_SET_SF_ENUM_TYPE(materialBinding, Binding);
}

/*!
  Destructor.
*/
SoVertexProperty::~SoVertexProperty()
{
}

/*!
  Does initialization common for all objects of the
  SoVertexProperty class. This includes setting up the
  type system, among other things.
*/
void
SoVertexProperty::initClass()
{
//$ BEGIN TEMPLATE InitNodeSource(VertexProperty)
  // Make sure we only initialize once.
  assert(SoVertexProperty::classTypeId == SoType::badType());
  // Make sure superclass get initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoVertexProperty::classTypeId =
    SoType::createType(inherited::getClassTypeId(), "VertexProperty",
                       &SoVertexProperty::createInstance,
                       SoNode::nextActionMethodIndex++);
//$ END TEMPLATE InitNodeSource

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
  SO_ENABLE(SoGetBoundingBoxAction, SoCoordinateElement);
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  SO_ENABLE(SoGLRenderAction, SoGLCoordinateElement);
  SO_ENABLE(SoGLRenderAction, SoMaterialBindingElement);
  SO_ENABLE(SoGLRenderAction, SoNormalBindingElement);
  SO_ENABLE(SoGLRenderAction, SoGLNormalElement);
  SO_ENABLE(SoGLRenderAction, SoGLTextureCoordinateElement);
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoVertexProperty::cleanClass()
{
}

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
/*!
  FIXME: write function documentation
*/
void 
SoVertexProperty::getBoundingBox(SoGetBoundingBoxAction * action)
{
  if (vertex.getNum() > 0) {
    SoCoordinateElement::set3(action->getState(), this,
			      vertex.getNum(), vertex.getValues(0));
  }
}
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write function documentation
*/
void 
SoVertexProperty::GLRender(SoGLRenderAction * action)
{
  SoState * state = action->getState();
  if (vertex.getNum() > 0 && !vertex.isIgnored()) 
    SoCoordinateElement::set3(state, this, vertex.getNum(),
			      vertex.getValues(0));
  
  if (texCoord.getNum() > 0 && !texCoord.isIgnored()) {
    SoTextureCoordinateElement::set2(state, this, texCoord.getNum(),
				     texCoord.getValues(0));
  }
  if (normal.getNum() > 0 && !normal.isIgnored()) {
    SoNormalElement::set(state, this, normal.getNum(),
			 normal.getValues(0));
    if (!normalBinding.isIgnored()) {
      SoNormalBindingElement::set(state, this,
				  (SoNormalBindingElement::Binding)
				  normalBinding.getValue());
    }
  }
  if (orderedRGBA.getNum() > 0 && !orderedRGBA.isIgnored()) {
    SoDiffuseColorElement::set(state, this, orderedRGBA.getNum(),
			       orderedRGBA.getValues(0));
    if (!materialBinding.isIgnored()) {
      SoMaterialBindingElement::set(state, this, 
				    (SoMaterialBindingElement::Binding)
				    materialBinding.getValue());
    }
  }
}

#endif // !COIN_EXCLUDE_SOGLRENDERACTION


#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write doc
 */
void
SoVertexProperty::doAction(SoAction * action)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOACTION

#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
/*!
  FIXME: write doc
 */
void
SoVertexProperty::callback(SoCallbackAction * action)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOCALLBACKACTION

#if !defined(COIN_EXCLUDE_SOPICKACTION)
/*!
  FIXME: write doc
 */
void
SoVertexProperty::pick(SoPickAction * action)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOPICKACTION

#if !defined(COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION)
/*!
  FIXME: write doc
 */
void
SoVertexProperty::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION
