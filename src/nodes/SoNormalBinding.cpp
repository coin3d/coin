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
  \class SoNormalBinding SoNormalBinding.h Inventor/nodes/SoNormalBinding.h
  \brief The SoNormalBinding class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoNormalBinding.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/SbName.h>

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
#include <Inventor/actions/SoGLRenderAction.h>
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SONORMALBINDINGELEMENT)
#include <Inventor/elements/SoNormalBindingElement.h>
#endif // !COIN_EXCLUDE_SONORMALBINDINGELEMENT

/*!
  \enum SoNormalBinding::Binding
  FIXME: write documentation for enum
*/
/*!
  \var SoNormalBinding::Binding SoNormalBinding::OVERALL
  FIXME: write documentation for enum definition
*/
/*!
  \var SoNormalBinding::Binding SoNormalBinding::PER_PART
  FIXME: write documentation for enum definition
*/
/*!
  \var SoNormalBinding::Binding SoNormalBinding::PER_PART_INDEXED
  FIXME: write documentation for enum definition
*/
/*!
  \var SoNormalBinding::Binding SoNormalBinding::PER_FACE
  FIXME: write documentation for enum definition
*/
/*!
  \var SoNormalBinding::Binding SoNormalBinding::PER_FACE_INDEXED
  FIXME: write documentation for enum definition
*/
/*!
  \var SoNormalBinding::Binding SoNormalBinding::PER_VERTEX
  FIXME: write documentation for enum definition
*/
/*!
  \var SoNormalBinding::Binding SoNormalBinding::PER_VERTEX_INDEXED
  FIXME: write documentation for enum definition
*/
/*!
  \var SoNormalBinding::Binding SoNormalBinding::DEFAULT
  FIXME: write documentation for enum definition
*/
/*!
  \var SoNormalBinding::Binding SoNormalBinding::NONE
  FIXME: write documentation for enum definition
*/

/*!
  \var SoSFEnum SoNormalBinding::value
  FIXME: write documentation for field
*/

// *************************************************************************

//$ BEGIN TEMPLATE NodeSource(SoNormalBinding)
SoType SoNormalBinding::classTypeId = SoType::badType();

/*!
  Returns a new instance of the SoNormalBinding node class.
*/
void *
SoNormalBinding::createInstance(void)
{
  return new SoNormalBinding;
}

/*!
  Returns the unique type identifier for the SoNormalBinding class.
*/
SoType
SoNormalBinding::getClassTypeId(void)
{
  return SoNormalBinding::classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoNormalBinding::getTypeId(void) const
{
  return SoNormalBinding::classTypeId;
}
//$ END TEMPLATE NodeSource

/*!
  Constructor.
*/
SoNormalBinding::SoNormalBinding()
{
//$ BEGIN TEMPLATE NodeConstructor(SoNormalBinding)
  // Make sure the class has been initialized.
  assert(SoNormalBinding::classTypeId != SoType::badType());
//$ END TEMPLATE NodeConstructor

  SO_NODE_ADD_FIELD(value, (DEFAULT));

  SO_NODE_DEFINE_ENUM_VALUE(Binding, DEFAULT);
  SO_NODE_DEFINE_ENUM_VALUE(Binding, OVERALL);
  SO_NODE_DEFINE_ENUM_VALUE(Binding, PER_PART);
  SO_NODE_DEFINE_ENUM_VALUE(Binding, PER_PART_INDEXED);
  SO_NODE_DEFINE_ENUM_VALUE(Binding, PER_FACE);
  SO_NODE_DEFINE_ENUM_VALUE(Binding, PER_FACE_INDEXED);
  SO_NODE_DEFINE_ENUM_VALUE(Binding, PER_VERTEX);
  SO_NODE_DEFINE_ENUM_VALUE(Binding, PER_VERTEX_INDEXED);

  SO_NODE_SET_SF_ENUM_TYPE(value, Binding);
}

/*!
  Destructor.
*/
SoNormalBinding::~SoNormalBinding()
{
}

/*!
  Does initialization common for all objects of the
  SoNormalBinding class. This includes setting up the
  type system, among other things.
*/
void
SoNormalBinding::initClass(void)
{
//$ BEGIN TEMPLATE InitNodeSource(NormalBinding)
  // Make sure we only initialize once.
  assert(SoNormalBinding::classTypeId == SoType::badType());
  // Make sure superclass get initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoNormalBinding::classTypeId =
    SoType::createType(inherited::getClassTypeId(), "NormalBinding",
                       &SoNormalBinding::createInstance,
                       SoNode::nextActionMethodIndex++);
//$ END TEMPLATE InitNodeSource

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  SO_ENABLE(SoGLRenderAction, SoNormalBindingElement);
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoNormalBinding::cleanClass(void)
{
}

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write function documentation
*/
void 
SoNormalBinding::GLRender(SoGLRenderAction * action)
{
  SoNormalBindingElement::set(action->getState(), this,
			      (SoNormalBindingElement::Binding)
			      value.getValue());
}
#endif // !COIN_EXCLUDE_SOGLRENDERACTION


#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write doc
 */
void
SoNormalBinding::doAction(SoAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOACTION

#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
/*!
  FIXME: write doc
 */
void
SoNormalBinding::callback(SoCallbackAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOCALLBACKACTION

#if !defined(COIN_EXCLUDE_SOPICKACTION)
/*!
  FIXME: write doc
 */
void
SoNormalBinding::pick(SoPickAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOPICKACTION

#if !defined(COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION)
/*!
  FIXME: write doc
 */
void
SoNormalBinding::getPrimitiveCount(SoGetPrimitiveCountAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION

/*!
  FIXME: write doc
 */
SbBool
SoNormalBinding::readInstance(SoInput * in, unsigned short flags)
{
  // FIXME: what else?
  return inherited::readInstance(in, flags);
}
