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



#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
#include <Inventor/actions/SoGLRenderAction.h>
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
#if !defined(COIN_EXCLUDE_SOPICKACTION)
#include <Inventor/actions/SoPickAction.h>
#endif // !COIN_EXCLUDE_SOPICKACTION
#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
#include <Inventor/actions/SoCallbackAction.h>
#endif // !COIN_EXCLUDE_SOCALLBACKACTION

#if !defined(COIN_EXCLUDE_SONORMALBINDINGELEMENT)
#include <Inventor/elements/SoNormalBindingElement.h>
#endif // !COIN_EXCLUDE_SONORMALBINDINGELEMENT
#if !defined(COIN_EXCLUDE_SOGLSHADEMODELELEMENT)
#include <Inventor/elements/SoGLShadeModelElement.h>
#endif

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

SO_NODE_SOURCE(SoNormalBinding);

/*!
  Constructor.
*/
SoNormalBinding::SoNormalBinding()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoNormalBinding);

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
  SO_NODE_INTERNAL_INIT_CLASS(SoNormalBinding);

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  SO_ENABLE(SoGLRenderAction, SoNormalBindingElement);
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SOPICKACTION)
  SO_ENABLE(SoPickAction, SoNormalBindingElement);
#endif // !COIN_EXCLUDE_SOPICKACTION

#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
  SO_ENABLE(SoCallbackAction, SoNormalBindingElement);
#endif // !COIN_EXCLUDE_SOCALLBACKACTION
}

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write function documentation
*/
void 
SoNormalBinding::GLRender(SoGLRenderAction * action)
{
  SoNormalBinding::doAction(action);
#if !defined(COIN_EXCLUDE_SOGLSHADEMODELELEMENT)
  if (!value.isIgnored()) {
    Binding binding = (Binding)value.getValue();
    SoGLShadeModelElement::setNormal(action->getState(),
				     binding == PER_VERTEX ||
				     binding == PER_VERTEX_INDEXED);
  }
#endif
}
#endif // !COIN_EXCLUDE_SOGLRENDERACTION


#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write doc
 */
void
SoNormalBinding::doAction(SoAction *action)
{
  if (!value.isIgnored()) {
    SoNormalBindingElement::set(action->getState(), this,
				(SoNormalBindingElement::Binding)
				value.getValue());
  }
}
#endif // !COIN_EXCLUDE_SOACTION

#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
/*!
  FIXME: write doc
 */
void
SoNormalBinding::callback(SoCallbackAction *action)
{
  SoNormalBinding::doAction(action);
}
#endif // !COIN_EXCLUDE_SOCALLBACKACTION

#if !defined(COIN_EXCLUDE_SOPICKACTION)
/*!
  FIXME: write doc
 */
void
SoNormalBinding::pick(SoPickAction *action)
{
  SoNormalBinding::doAction(action);
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
