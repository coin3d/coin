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
  \class SoDepthBuffer SoDepthBuffer.h
  \brief The SoDepthBuffer class is a node used to control the GL depth buffer.

  With this node you can control properties related to the OpenGL depth buffer
  in a variety of ways.

  - you can enable and disable depth buffer testing during rendering,

  - you can enable and disable writing to the depth buffer during rendering,

  - you can set the function used for the depth buffer testing, and

  - you can set the value range used in the depth buffer.

  The value range setting is useful if you need to segment the 3D world into
  different segments with different depth buffer resolutions to get a more
  optimal depth buffer resolution distribution than what a single, uniform
  depth buffer value range can give you.

  \ingroup nodes
  \COIN_CLASS_EXTENSION
  \since Coin 3.0
*/

#include <Inventor/nodes/SoDepthBuffer.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLDepthBufferElement.h>
#include <Inventor/system/gl.h>

#include "nodes/SoSubNodeP.h"

/*!
  \enum SoDepthBuffer::DepthWriteFunction
  Enumeration for the various depth functions.
*/

/*!
  \var SoDepthBuffer::DepthWriteFunction SoDepthBuffer::NEVER
  Never passes.
*/

/*!
  \var SoDepthBuffer::DepthWriteFunction SoDepthBuffer::ALWAYS
  Always passes.
*/

/*!
  \var SoDepthBuffer::DepthWriteFunction SoDepthBuffer::LESS
  Passes if the incoming depth value is less than the stored depth value.
*/

/*!
  \var SoDepthBuffer::DepthWriteFunction SoDepthBuffer::LEQUAL
  Passes if the incoming depth value is less than or equal to the stored depth value.
*/

/*!
  \var SoDepthBuffer::DepthWriteFunction SoDepthBuffer::EQUAL
  Passes if the incoming depth value is equal to the stored depth value.
*/

/*!
  \var SoDepthBuffer::DepthWriteFunction SoDepthBuffer::GEQUAL
  Passes if the incoming depth value is greater than or equal to the stored depth value.
*/

/*!
  \var SoDepthBuffer::DepthWriteFunction SoDepthBuffer::GREATER
  Passes if the incoming depth value is greater than the stored depth value.
*/

/*!
  \var SoDepthBuffer::DepthWriteFunction SoDepthBuffer::NOTEQUAL
  Passes if the incoming depth value is not equal to the stored depth value.
*/

/*!
  \var SoSFEnum SoDepthBuffer::function

  Which depth function to use. Defaults to LESS.
*/

/*!
  \var SoSFBool SoDepthBuffer::test

  Enable depth buffer testing. Defaults to TRUE.
*/

/*!
  \var SoSFBool SoDepthBuffer::write

  Enable depth buffer writing. Defaults to TRUE.
*/

/*!
  \var SoSFVec2f SoDepthBuffer::range

  The value range for the depth buffer data. Defaults to [0.0-1.0].
  The range will be clamped to [0.0-1.0].
*/

SO_NODE_SOURCE(SoDepthBuffer);

void
SoDepthBuffer::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoDepthBuffer, SO_FROM_COIN_3_0);

  SO_ENABLE(SoGLRenderAction, SoGLDepthBufferElement);
}

/*!
  Constructor.
*/
SoDepthBuffer::SoDepthBuffer(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoDepthBuffer);

  SO_NODE_ADD_FIELD(test, (TRUE));
  SO_NODE_ADD_FIELD(write, (TRUE));
  SO_NODE_ADD_FIELD(function, (SoDepthBuffer::LESS));
  SO_NODE_ADD_FIELD(range, (SbVec2f(0.0f, 1.0f)));

  SO_NODE_DEFINE_ENUM_VALUE(DepthWriteFunction, NEVER);
  SO_NODE_DEFINE_ENUM_VALUE(DepthWriteFunction, ALWAYS);
  SO_NODE_DEFINE_ENUM_VALUE(DepthWriteFunction, LESS);
  SO_NODE_DEFINE_ENUM_VALUE(DepthWriteFunction, LEQUAL);
  SO_NODE_DEFINE_ENUM_VALUE(DepthWriteFunction, EQUAL);
  SO_NODE_DEFINE_ENUM_VALUE(DepthWriteFunction, GEQUAL);
  SO_NODE_DEFINE_ENUM_VALUE(DepthWriteFunction, GREATER);
  SO_NODE_DEFINE_ENUM_VALUE(DepthWriteFunction, NOTEQUAL);
  SO_NODE_SET_SF_ENUM_TYPE(function, DepthWriteFunction);
}

/*!
  Destructor.
*/
SoDepthBuffer::~SoDepthBuffer()
{
}

// Doc from parent
void
SoDepthBuffer::GLRender(SoGLRenderAction * action)
{
  SoState * state = action->getState();
  SbBool testenable = this->test.getValue();
  SbBool writeenable = this->write.getValue();
  SoDepthBufferElement::DepthWriteFunction function =
    static_cast<SoDepthBufferElement::DepthWriteFunction>(this->function.getValue());
  SbVec2f depthrange = this->range.getValue();

  // accommodate for ignored fields
  if (this->test.isIgnored()) {
    testenable = SoDepthBufferElement::getTestEnable(state);
  }
  // if we're rendering transparent objects, let SoGLRenderAction decide if
  // depth write should be enabled
  if (this->write.isIgnored() || action->isRenderingTranspPaths()) {
    writeenable = SoDepthBufferElement::getWriteEnable(state);
  }
  if (this->function.isIgnored()) {
    function = SoDepthBufferElement::getFunction(state);
  }
  if (this->range.isIgnored()) {
    range = SoDepthBufferElement::getRange(state);
  }

  // update element
  SoDepthBufferElement::set(state, testenable, writeenable,
                            function, depthrange);
}
