/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
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
  \brief The SoNormalBinding class is a node for specifying normal vector bindings.
  \ingroup nodes

  Use nodes of this type to specify how to map normal vectors from
  SoNormal nodes in the scene graph to shape nodes.
*/


#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/elements/SoGLShadeModelElement.h>
#include <Inventor/elements/SoNormalBindingElement.h>

/*!
  \enum SoNormalBinding::Binding

  Enumeration of available types of mappings. See documentation of
  SoMaterialBinding node for explanation of the different values.

  Note that SoNormalBinding::DEFAULT and SoNormalBinding::NONE has
  been obsoleted and are both mapped to
  SoNormalBinding::PER_VERTEX_INDEXED.
*/

/*!
  \var SoSFEnum SoNormalBinding::value

  The normal binding to use for subsequent shape nodes in the scene
  graph.
*/

// *************************************************************************

SO_NODE_SOURCE(SoNormalBinding);

/*!
  Constructor.
*/
SoNormalBinding::SoNormalBinding(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoNormalBinding);

  SO_NODE_ADD_FIELD(value, (DEFAULT));

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
SoNormalBinding::~SoNormalBinding()
{
}

// Doc from superclass.
void
SoNormalBinding::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoNormalBinding);

  SO_ENABLE(SoGLRenderAction, SoNormalBindingElement);
  SO_ENABLE(SoPickAction, SoNormalBindingElement);
  SO_ENABLE(SoCallbackAction, SoNormalBindingElement);
  SO_ENABLE(SoGetPrimitiveCountAction, SoNormalBindingElement);
}

// Doc from superclass.
void
SoNormalBinding::GLRender(SoGLRenderAction * action)
{
  SoNormalBinding::doAction(action);
  if (!this->value.isIgnored()) {
    Binding binding = (Binding)this->value.getValue();
    SoGLShadeModelElement::setNormal(action->getState(),
                                     binding == PER_VERTEX ||
                                     binding == PER_VERTEX_INDEXED);
  }
}

// Doc from superclass.
void
SoNormalBinding::doAction(SoAction * action)
{
  if (!this->value.isIgnored()) {
    SoNormalBindingElement::set(action->getState(), this,
                                (SoNormalBindingElement::Binding)
                                this->value.getValue());
  }
}

// Doc from superclass.
void
SoNormalBinding::callback(SoCallbackAction * action)
{
  SoNormalBinding::doAction(action);
}

// Doc from superclass.
void
SoNormalBinding::pick(SoPickAction * action)
{
  SoNormalBinding::doAction(action);
}

// Doc from superclass.
void
SoNormalBinding::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  SoNormalBinding::doAction(action);
}

SbBool
SoNormalBinding::readInstance(SoInput * in, unsigned short flags)
{
  // FIXME: what else?
  return inherited::readInstance(in, flags);
}
