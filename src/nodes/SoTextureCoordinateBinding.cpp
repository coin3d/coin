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
  \class SoTextureCoordinateBinding SoTextureCoordinateBinding.h Inventor/nodes/SoTextureCoordinateBinding.h
  \brief The SoTextureCoordinateBinding class ...
  \ingroup nodes

  FIXME: write class doc
*/


#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/actions/SoCallbackAction.h>


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

SO_NODE_SOURCE(SoTextureCoordinateBinding);

/*!
  Constructor.
*/
SoTextureCoordinateBinding::SoTextureCoordinateBinding()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoTextureCoordinateBinding);

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

// doc in super
void
SoTextureCoordinateBinding::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoTextureCoordinateBinding);

  SO_ENABLE(SoGLRenderAction, SoTextureCoordinateBindingElement);
  SO_ENABLE(SoPickAction, SoTextureCoordinateBindingElement);
  SO_ENABLE(SoCallbackAction, SoTextureCoordinateBindingElement);
}

/*!
  FIXME: write function documentation
*/
void
SoTextureCoordinateBinding::GLRender(SoGLRenderAction * action)
{
  SoTextureCoordinateBinding::doAction(action);
}

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

/*!
  FIXME: write doc
 */
void
SoTextureCoordinateBinding::callback(SoCallbackAction *action)
{
  SoTextureCoordinateBinding::doAction(action);
}

/*!
  FIXME: write doc
 */
void
SoTextureCoordinateBinding::pick(SoPickAction *action)
{
  SoTextureCoordinateBinding::doAction(action);
}

/*!
  FIXME: write doc
*/
SbBool
SoTextureCoordinateBinding::readInstance(SoInput * in, unsigned short flags)
{
  SbBool ret = inherited::readInstance(in, flags);
  if (ret) {
    // test for obsolete values
    if (this->value.getValue() < 2) {
      this->value = PER_VERTEX_INDEXED;
    }
  }
  return ret;
}
