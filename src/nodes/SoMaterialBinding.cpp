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
  \class SoMaterialBinding SoMaterialBinding.h Inventor/nodes/SoMaterialBinding.h
  \brief The SoMaterialBinding class ...
  \ingroup nodes

  FIXME: write class doc
*/


#include <Inventor/nodes/SoMaterialBinding.h>

#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/elements/SoMaterialBindingElement.h>
#include <Inventor/elements/SoOverrideElement.h>
#include <Inventor/elements/SoGLShadeModelElement.h>

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

SO_NODE_SOURCE(SoMaterialBinding);

/*!
  Constructor.
*/
SoMaterialBinding::SoMaterialBinding()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoMaterialBinding);

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
  SO_NODE_INTERNAL_INIT_CLASS(SoMaterialBinding);

  SO_ENABLE(SoGLRenderAction, SoMaterialBindingElement);

  SO_ENABLE(SoPickAction, SoMaterialBindingElement);

  SO_ENABLE(SoCallbackAction, SoMaterialBindingElement);

  SO_ENABLE(SoGetPrimitiveCountAction, SoMaterialBindingElement);
}

/*!
  FIXME: write function documentation
*/
void
SoMaterialBinding::GLRender(SoGLRenderAction * action)
{
  if (!value.isIgnored()
      && !SoOverrideElement::getMaterialBindingOverride(action->getState())) {
    Binding binding = (Binding)value.getValue();
    SoMaterialBindingElement::set(action->getState(),
                                  (SoMaterialBindingElement::Binding)
                                  binding);
    SoGLShadeModelElement::setMaterial(action->getState(),
                                       binding == PER_VERTEX ||
                                       binding == PER_VERTEX_INDEXED);
  }
}

/*!
  FIXME: write doc
 */
void
SoMaterialBinding::doAction(SoAction *action)
{
  if (!value.isIgnored()
      && !SoOverrideElement::getMaterialBindingOverride(action->getState())) {
    SoMaterialBindingElement::set(action->getState(),
                                  (SoMaterialBindingElement::Binding)
                                  value.getValue());
  }
}

/*!
  FIXME: write doc
 */
void
SoMaterialBinding::callback(SoCallbackAction *action)
{
  SoMaterialBinding::doAction((SoAction*)action);
}

/*!
  FIXME: write doc
 */
void
SoMaterialBinding::pick(SoPickAction *action)
{
  SoMaterialBinding::doAction(action);
}

/*!
  FIXME: write doc
 */
void
SoMaterialBinding::getPrimitiveCount(SoGetPrimitiveCountAction *action)
{
  SoMaterialBinding::doAction(action);
}

/*!
  FIXME: write doc
 */
SbBool
SoMaterialBinding::readInstance(SoInput * in, unsigned short flags)
{
  return inherited::readInstance(in, flags);
  // FIXME: update old bindings?
}
