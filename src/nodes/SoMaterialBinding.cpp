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
  \class SoMaterialBinding SoMaterialBinding.h Inventor/nodes/SoMaterialBinding.h
  \brief The SoMaterialBinding class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/SbName.h>
#include <Inventor/nodes/SoMaterialBinding.h>
#include <Inventor/nodes/SoSubNode.h>

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
#include <Inventor/actions/SoGLRenderAction.h>
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
#include <Inventor/actions/SoCallbackAction.h>
#endif // !COIN_EXCLUDE_SOCALLBACKACTION
#if !defined(COIN_EXCLUDE_SOPICKACTION)
#include <Inventor/actions/SoPickAction.h>
#endif // !COIN_EXCLUDE_SOPICKACTION

#if !defined(COIN_EXCLUDE_SOMATERIALBINDINGELEMENT)
#include <Inventor/elements/SoMaterialBindingElement.h>
#endif // !COIN_EXCLUDE_SOMATERIALBINDINGELEMENT

/*!
  \enum SoMaterialBinding::Binding
  FIXME: write documentation for enum
*/
/*!
  \var SoMaterialBinding::Binding SoMaterialBinding::OVERALL
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMaterialBinding::Binding SoMaterialBinding::PER_PART
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMaterialBinding::Binding SoMaterialBinding::PER_PART_INDEXED
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMaterialBinding::Binding SoMaterialBinding::PER_FACE
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMaterialBinding::Binding SoMaterialBinding::PER_FACE_INDEXED
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMaterialBinding::Binding SoMaterialBinding::PER_VERTEX
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMaterialBinding::Binding SoMaterialBinding::PER_VERTEX_INDEXED
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMaterialBinding::Binding SoMaterialBinding::DEFAULT
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMaterialBinding::Binding SoMaterialBinding::NONE
  FIXME: write documentation for enum definition
*/


/*!
  \var SoSFEnum SoMaterialBinding::value
  FIXME: write documentation for field
*/


// *************************************************************************

//$ BEGIN TEMPLATE NodeSource(SoMaterialBinding)
SoType SoMaterialBinding::classTypeId = SoType::badType();

/*!
  Returns a new instance of the SoMaterialBinding node class.
*/
void *
SoMaterialBinding::createInstance(void)
{
  return new SoMaterialBinding;
}

/*!
  Returns the unique type identifier for the SoMaterialBinding class.
*/
SoType
SoMaterialBinding::getClassTypeId(void)
{
  return SoMaterialBinding::classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoMaterialBinding::getTypeId(void) const
{
  return SoMaterialBinding::classTypeId;
}
//$ END TEMPLATE NodeSource

/*!
  Constructor.
*/
SoMaterialBinding::SoMaterialBinding()
{
//$ BEGIN TEMPLATE NodeConstructor(SoMaterialBinding)
  // Make sure the class has been initialized.
  assert(SoMaterialBinding::classTypeId != SoType::badType());
//$ END TEMPLATE NodeConstructor

  SO_NODE_ADD_FIELD(value,(OVERALL));

  SO_NODE_DEFINE_ENUM_VALUE(Binding, OVERALL);
  SO_NODE_DEFINE_ENUM_VALUE(Binding, PER_PART);
  SO_NODE_DEFINE_ENUM_VALUE(Binding, PER_PART_INDEXED);
  SO_NODE_DEFINE_ENUM_VALUE(Binding, PER_FACE);
  SO_NODE_DEFINE_ENUM_VALUE(Binding, PER_FACE_INDEXED);
  SO_NODE_DEFINE_ENUM_VALUE(Binding, PER_VERTEX);
  SO_NODE_DEFINE_ENUM_VALUE(Binding, PER_VERTEX_INDEXED);
  SO_NODE_DEFINE_ENUM_VALUE(Binding, DEFAULT);
  SO_NODE_DEFINE_ENUM_VALUE(Binding, NONE);

  SO_NODE_SET_SF_ENUM_TYPE(value, Binding);
}


/*!
  Destructor.
*/
SoMaterialBinding::~SoMaterialBinding()
{
}

/*!
  Does initialization common for all objects of the
  SoMaterialBinding class. This includes setting up the
  type system, among other things.
*/
void
SoMaterialBinding::initClass(void)
{
//$ BEGIN TEMPLATE InitNodeSource(MaterialBinding)
  // Make sure we only initialize once.
  assert(SoMaterialBinding::classTypeId == SoType::badType());
  // Make sure superclass get initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoMaterialBinding::classTypeId =
    SoType::createType(inherited::getClassTypeId(), "MaterialBinding",
                       &SoMaterialBinding::createInstance,
                       SoNode::nextActionMethodIndex++);
//$ END TEMPLATE InitNodeSource

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  SO_ENABLE(SoGLRenderAction, SoMaterialBindingElement);
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoMaterialBinding::cleanClass(void)
{
}

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write function documentation
*/
void 
SoMaterialBinding::GLRender(SoGLRenderAction * action)
{
  SoMaterialBindingElement::set(action->getState(), 
				(SoMaterialBindingElement::Binding)
				value.getValue());
}
#endif // !COIN_EXCLUDE_SOGLRENDERACTION


#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write doc
 */
void
SoMaterialBinding::doAction(SoAction * action)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOACTION

#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
/*!
  FIXME: write doc
 */
void
SoMaterialBinding::callback(SoCallbackAction * action)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOCALLBACKACTION

#if !defined(COIN_EXCLUDE_SOPICKACTION)
/*!
  FIXME: write doc
 */
void
SoMaterialBinding::pick(SoPickAction * action)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOPICKACTION

#if !defined(COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION)
/*!
  FIXME: write doc
 */
void
SoMaterialBinding::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION

/*!
  FIXME: write doc
 */
SbBool
SoMaterialBinding::readInstance(SoInput * in, unsigned short flags)
{
  return inherited::readInstance(in, flags);
  // FIXME: update old bindings?
}
