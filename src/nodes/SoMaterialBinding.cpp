/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

/*!
  \class SoMaterialBinding SoMaterialBinding.h Inventor/nodes/SoMaterialBinding.h
  \brief The SoMaterialBinding class is a node for setting up how materials are mapped to shapes.
  \ingroup nodes

  The material binding specified in nodes of this type decides how the
  material values of SoMaterial nodes are mapped on the builtin
  geometry shape nodes.

  The exact meaning of a binding depends on what particular shape type
  a material or a set of materials are applied to.

  \sa SoMaterial
*/


#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/elements/SoMaterialBindingElement.h>
#include <Inventor/elements/SoOverrideElement.h>

/*!
  \enum SoMaterialBinding::Binding
  Enumeration of available types of material binding.
*/
/*!
  \var SoMaterialBinding::Binding SoMaterialBinding::OVERALL
  Apply the same material to the complete shape.
*/
/*!
  \var SoMaterialBinding::Binding SoMaterialBinding::PER_PART

  Get a new material from the pool of material values for each \e part
  of the shape, where the definition of a \e part depends on the shape
  type.

  Materials are fetched from index 0 and onwards, incrementing the
  index into the material pool by 1 for each new part in the order
  defined by the particular shape type.
*/
/*!
  \var SoMaterialBinding::Binding SoMaterialBinding::PER_PART_INDEXED

  Get a new material from the pool of material values for each \e part
  of the shape, where the definition of a \e part depends on the shape
  type.

  Materials are fetched by the index value settings of the shape.
*/
/*!
  \var SoMaterialBinding::Binding SoMaterialBinding::PER_FACE

  Get a new material from the pool of material values for each polygon
  \e face of the shape.

  Materials are fetched from index 0 and onwards, incrementing the
  index into the material pool by 1 for each new face of the shape
  node.

  Note that individual faces after tessellation of complex shapes
  (like SoCylinder nodes) are \e not considered to be separate
  entities, and so this binding will be treated in the same manner as
  SoMaterialBinding::PER_PART for those.
*/
/*!
  \var SoMaterialBinding::Binding SoMaterialBinding::PER_FACE_INDEXED

  Get a new material from the pool of material values for each polygon
  \e face of the shape.

  Materials are fetched by the index value settings of the shape.

  Note that individual faces after tessellation of complex shapes
  (like SoCylinder nodes) are \e not considered to be separate
  entities, and so this binding will be treated in the same manner as
  SoMaterialBinding::PER_PART_INDEXED for those.
*/
/*!
  \var SoMaterialBinding::Binding SoMaterialBinding::PER_VERTEX

  Get a new material from the pool of material values for each
  polygon, line or point \e vertex of the shape.

  Materials are fetched from index 0 and onwards, incrementing the
  index into the material pool by 1 for each new vertex of the shape
  node.
*/
/*!
  \var SoMaterialBinding::Binding SoMaterialBinding::PER_VERTEX_INDEXED

  Get a new material from the pool of material values for each
  polygon, line or point \e vertex of the shape.

  Materials are fetched by the index value settings of the shape.
*/



// Note: Inventor format files with SoMaterialBinding nodes with one
// of the two obsoleted values below will be updated upon import to
// contain the value for SoMaterialBinding::OVERALL. Ditto for calls
// to SoMaterialBinding::value.setValue([DEFAULT | NONE]).
//
// This happens automatically due to the way So[SM]FEnum fields are
// implemented.
//                 --mortene

/*!
  \var SoMaterialBinding::Binding SoMaterialBinding::DEFAULT
  Obsolete. Don't use this value.
*/
/*!
  \var SoMaterialBinding::Binding SoMaterialBinding::NONE
  Obsolete. Don't use this value.
*/


/*!
  \var SoSFEnum SoMaterialBinding::value

  The material binding to use for subsequent shape nodes in the scene
  graph. The default binding is SoMaterialBinding::OVERALL.
*/


// *************************************************************************

SO_NODE_SOURCE(SoMaterialBinding);

/*!
  Constructor.
*/
SoMaterialBinding::SoMaterialBinding(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoMaterialBinding);

  SO_NODE_ADD_FIELD(value, (OVERALL));

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

// Doc from superclass.
void
SoMaterialBinding::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoMaterialBinding);

  SO_ENABLE(SoGLRenderAction, SoMaterialBindingElement);
  SO_ENABLE(SoPickAction, SoMaterialBindingElement);
  SO_ENABLE(SoCallbackAction, SoMaterialBindingElement);
  SO_ENABLE(SoGetPrimitiveCountAction, SoMaterialBindingElement);
}

// Doc from superclass.
void
SoMaterialBinding::GLRender(SoGLRenderAction * action)
{
  SoMaterialBinding::doAction(action);
}

// Doc from superclass.
void
SoMaterialBinding::doAction(SoAction * action)
{
  if (!this->value.isIgnored()
      && !SoOverrideElement::getMaterialBindingOverride(action->getState())) {
    SoMaterialBindingElement::set(action->getState(),
                                  (SoMaterialBindingElement::Binding)
                                  this->value.getValue());
    if (this->isOverride()) {
      SoOverrideElement::setMaterialBindingOverride(action->getState(), this, TRUE);
    }
  }
}

// Doc from superclass.
void
SoMaterialBinding::callback(SoCallbackAction * action)
{
  SoMaterialBinding::doAction((SoAction *)action);
}

// Doc from superclass.
void
SoMaterialBinding::pick(SoPickAction * action)
{
  SoMaterialBinding::doAction(action);
}

// Doc from superclass.
void
SoMaterialBinding::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  SoMaterialBinding::doAction(action);
}
