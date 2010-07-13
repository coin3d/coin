/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

/*!
  \class SoAlphaTest SoAlphaTest.h
  \brief The SoAlphaTest class is a node used to control the GL alpha test function.

  With this node you can control the OpenGL alpha test function. The
  alpha test function enables you to discard fragments based on its
  alpha value. For instance, if you set the function to GREATER and
  value to 0.5, only fragments with alpha value greater than 0.5 will
  be rendered.

  Alpha testing is typically used when rendering textures where all
  pixels are either completely opaque or completely
  transparent. Transparency sorting propblems are avoided when alpha
  testing is used instead of blending, since depth testing can still
  be enabled and no sorting or delayed rendering is needed.

  To enable alpha testing instead of transparency blending, insert a
  TransparencyType node with value = NONE, and an AlphaTest node with
  \e function set to GREATER and \e value set to for instance 0.01.

  \ingroup nodes
  \COIN_CLASS_EXTENSION
  \since Coin 4.0
*/

#include <Inventor/nodes/SoAlphaTest.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoLazyElement.h>
#include <Inventor/system/gl.h>

#include "nodes/SoSubNodeP.h"

/*!
  \enum SoAlphaTest::Function
  Enumeration for the various alpha functions.
*/

/*!
  \var SoAlphaTest::Function SoAlphaTest::NEVER
  Never passes.
*/

/*!
  \var SoAlphaTest::Function SoAlphaTest::ALWAYS
  Always passes.
*/

/*!
  \var SoAlphaTest::Function SoAlphaTest::LESS
  Passes if the incoming depth value is less than the stored depth value.
*/

/*!
  \var SoAlphaTest::Function SoAlphaTest::LEQUAL
  Passes if the incoming depth value is less than or equal to the stored depth value.
*/

/*!
  \var SoAlphaTest::Function SoAlphaTest::EQUAL
  Passes if the incoming depth value is equal to the stored depth value.
*/

/*!
  \var SoAlphaTest::Function SoAlphaTest::GEQUAL
  Passes if the incoming depth value is greater than or equal to the stored depth value.
*/

/*!
  \var SoAlphaTest::Function SoAlphaTest::GREATER
  Passes if the incoming depth value is greater than the stored depth value.
*/

/*!
  \var SoAlphaTest::Function SoAlphaTest::NOTEQUAL
  Passes if the incoming depth value is not equal to the stored depth value.
*/

/*!
  \var SoSFEnum SoAlphaTest::function

  Which depth function to use. Defaults to NONE.
*/

/*!
  \var SoSFFloat SoAlphaTest::value

  The value the function will compare against when applicable. Default is 0.5.
*/

SO_NODE_SOURCE(SoAlphaTest);

void
SoAlphaTest::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoAlphaTest, SO_FROM_COIN_4_0);
}

/*!
  Constructor.
*/
SoAlphaTest::SoAlphaTest(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoAlphaTest);

  SO_NODE_ADD_FIELD(function, (NONE));
  SO_NODE_ADD_FIELD(value, (0.5f));

  SO_NODE_DEFINE_ENUM_VALUE(Function, NEVER);
  SO_NODE_DEFINE_ENUM_VALUE(Function, ALWAYS);
  SO_NODE_DEFINE_ENUM_VALUE(Function, LESS);
  SO_NODE_DEFINE_ENUM_VALUE(Function, LEQUAL);
  SO_NODE_DEFINE_ENUM_VALUE(Function, EQUAL);
  SO_NODE_DEFINE_ENUM_VALUE(Function, GEQUAL);
  SO_NODE_DEFINE_ENUM_VALUE(Function, GREATER);
  SO_NODE_DEFINE_ENUM_VALUE(Function, NOTEQUAL);
  SO_NODE_SET_SF_ENUM_TYPE(function, Function);
}

/*!
  Destructor.
*/
SoAlphaTest::~SoAlphaTest()
{
}

// Doc from parent
void
SoAlphaTest::GLRender(SoGLRenderAction * action)
{
  int func = this->function.getValue();
  GLenum glfunc = 0;
  switch (func) {
  case NONE:
    glfunc = 0;
    break;
  case NEVER:
    glfunc = GL_NEVER;
    break;
  case ALWAYS:
    glfunc = GL_ALWAYS;
    break;
  case LESS:
    glfunc = GL_LESS;
    break;
  case LEQUAL:
    glfunc = GL_LEQUAL;
    break;
  case EQUAL:
    glfunc = GL_EQUAL;
    break;
  case GEQUAL:
    glfunc = GL_GEQUAL;
    break;
  case GREATER:
    glfunc = GL_GREATER;
    break;
  case NOTEQUAL:
    glfunc = GL_NOTEQUAL;
    break;
  default:
    break;
  }
  SoLazyElement::setAlphaTest(action->getState(),
                              glfunc, this->value.getValue());
}
