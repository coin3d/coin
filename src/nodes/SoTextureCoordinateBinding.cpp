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
  \class SoTextureCoordinateBinding SoTextureCoordinateBinding.h Inventor/nodes/SoTextureCoordinateBinding.h
  \brief The SoTextureCoordinateBinding class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoTextureCoordinateBinding.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/SbName.h>

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
#include <Inventor/actions/SoGLRenderAction.h>
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SOPICKACTION)
#include <Inventor/actions/SoPickAction.h>
#endif // !COIN_EXCLUDE_SOPICKACTION

#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
#include <Inventor/actions/SoCallbackAction.h>
#endif // !COIN_EXCLUDE_SOCALLBACKACTION

#if !defined(COIN_EXCLUDE_SOTEXTURECOORDINATEBINDINGELEMENT)
#include <Inventor/elements/SoTextureCoordinateBindingElement.h>
#endif // !COIN_EXCLUDE_SOTEXTURECOORDINATEBINDINGELEMENT


/*!
  \enum SoTextureCoordinateBinding::Binding
  FIXME: write documentation for enum
*/
/*!
  \var SoTextureCoordinateBinding::Binding SoTextureCoordinateBinding::PER_VERTEX
  FIXME: write documentation for enum definition
*/
/*!
  \var SoTextureCoordinateBinding::Binding SoTextureCoordinateBinding::PER_VERTEX_INDEXED
  FIXME: write documentation for enum definition
*/


/*!
  \var SoSFEnum SoTextureCoordinateBinding::value
  FIXME: write documentation for field
*/


// *************************************************************************

//$ BEGIN TEMPLATE NodeSource(SoTextureCoordinateBinding)
SoType SoTextureCoordinateBinding::classTypeId = SoType::badType();

/*!
  Returns a new instance of the SoTextureCoordinateBinding node class.
*/
void *
SoTextureCoordinateBinding::createInstance(void)
{
  return new SoTextureCoordinateBinding;
}

/*!
  Returns the unique type identifier for the SoTextureCoordinateBinding class.
*/
SoType
SoTextureCoordinateBinding::getClassTypeId(void)
{
  return SoTextureCoordinateBinding::classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoTextureCoordinateBinding::getTypeId(void) const
{
  return SoTextureCoordinateBinding::classTypeId;
}
//$ END TEMPLATE NodeSource

/*!
  Constructor.
*/
SoTextureCoordinateBinding::SoTextureCoordinateBinding()
{
//$ BEGIN TEMPLATE NodeConstructor(SoTextureCoordinateBinding)
  // Make sure the class has been initialized.
  assert(SoTextureCoordinateBinding::classTypeId != SoType::badType());
//$ END TEMPLATE NodeConstructor
  
  SO_NODE_ADD_FIELD(value, (SoTextureCoordinateBinding::PER_VERTEX_INDEXED));

  SO_NODE_DEFINE_ENUM_VALUE(Binding, PER_VERTEX);
  SO_NODE_DEFINE_ENUM_VALUE(Binding, PER_VERTEX_INDEXED);
  SO_NODE_SET_SF_ENUM_TYPE(value, Binding);
}

/*!
  Destructor.
*/
SoTextureCoordinateBinding::~SoTextureCoordinateBinding()
{
}

/*!
  Does initialization common for all objects of the
  SoTextureCoordinateBinding class. This includes setting up the
  type system, among other things.
*/
void
SoTextureCoordinateBinding::initClass(void)
{
//$ BEGIN TEMPLATE InitNodeSource(TextureCoordinateBinding)
  // Make sure we only initialize once.
  assert(SoTextureCoordinateBinding::classTypeId == SoType::badType());
  // Make sure superclass get initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoTextureCoordinateBinding::classTypeId =
    SoType::createType(inherited::getClassTypeId(), "TextureCoordinateBinding",
                       &SoTextureCoordinateBinding::createInstance,
                       SoNode::nextActionMethodIndex++);
//$ END TEMPLATE InitNodeSource

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  SO_ENABLE(SoGLRenderAction, SoTextureCoordinateBindingElement);
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
#if !defined(COIN_EXCLUDE_SOPICKACTION)
  SO_ENABLE(SoPickAction, SoTextureCoordinateBindingElement);
#endif // !COIN_EXCLUDE_SOPICKACTION
#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
  SO_ENABLE(SoCallbackAction, SoTextureCoordinateBindingElement);
#endif // !COIN_EXCLUDE_SOCALLBACKACTION
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoTextureCoordinateBinding::cleanClass(void)
{
}

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write function documentation
*/
void 
SoTextureCoordinateBinding::GLRender(SoGLRenderAction * action)
{
  SoTextureCoordinateBinding::doAction(action);
}
#endif // !COIN_EXCLUDE_SOGLRENDERACTION



#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write doc
 */
void
SoTextureCoordinateBinding::doAction(SoAction *action)
{
  if (!value.isIgnored()) 
    SoTextureCoordinateBindingElement::set(action->getState(), this,
     (SoTextureCoordinateBindingElement::Binding)value.getValue());
}
#endif // !COIN_EXCLUDE_SOACTION

#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
/*!
  FIXME: write doc
 */
void
SoTextureCoordinateBinding::callback(SoCallbackAction *action)
{
  SoTextureCoordinateBinding::doAction(action);
}
#endif // !COIN_EXCLUDE_SOCALLBACKACTION

#if !defined(COIN_EXCLUDE_SOPICKACTION)
/*!
  FIXME: write doc
 */
void
SoTextureCoordinateBinding::pick(SoPickAction *action)
{
  SoTextureCoordinateBinding::doAction(action);
}
#endif // !COIN_EXCLUDE_SOPICKACTION

/*!
  FIXME: write doc
*/
SbBool
SoTextureCoordinateBinding::readInstance(SoInput * in, unsigned short flags)
{
  // FIXME: update binding? pederb, 990616
  return inherited::readInstance(in, flags);
}
