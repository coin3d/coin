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
  \class SoGeometryShader SoGeometryShader.h Inventor/nodes/SoGeometryShader.h
  \brief The SoGeometryShader class is used for loading geometry shader programs.
  \ingroup nodes

  See \link coin_shaders Shaders in Coin \endlink for more information
  on how to set up a scene graph with shaders.

  <b>FILE FORMAT/DEFAULTS:</b>
  \code
    GeometryShader {
      isActive TRUE
      sourceType FILENAME
      sourceProgram ""
      parameter []
      inputType TRIANGLES_IN
      outputType TRIANGLE_STRIP_OUT
      maxEmit 8
    }
  \endcode

  \sa SoShaderObject
  \sa SoShaderProgram
  \since Coin 2.5
*/

/*!
  \enum SoGeometryShader::InputType

  Enumerates the input types.
*/

/*!
  \enum SoGeometryShader::OutputType

  Enumerates the output types.
*/

/*!
  \var SoGeometryShader::InputType SoGeometryShader::TRIANGLES_IN.

  Shader uses triangles as input. OpenGL will automatically convert quads
  and polygons into triangles.
*/

/*!
  \var SoGeometryShader::InputType SoGeometryShader::LINES_IN.

  Shader uses lines as input.
*/

/*!
  \var SoGeometryShader::InputType SoGeometryShader::POINTS_IN.

  Shader uses points as input.
*/

/*!
  \var SoGeometryShader::OutputType SoGeometryShader::POINTS_OUT.

  Shader generates points.
*/

/*!
  \var SoGeometryShader::OutputType SoGeometryShader::LINE_STRIP_OUT.

  Shader generates line strips.
*/

/*!
  \var SoGeometryShader::OutputType SoGeometryShader::TRIANLE_STRIP_OUT.

  Shader generates triangle strips.
*/


/*!
  \var SoSFEnum SoGeometryShader::inputType

  The type of geometry used as input to the shader. Default value is TRIANGLES_IN.
*/

/*!
  \var SoSFEnum SoGeometryShader::outputType

  The type of geometry generated from the shader. Default value is TRIANGLE_FAN_OUT.
*/

/*!
  \var SoSFInt32 SoGeometryShader::maxEmit

  The maximum number of vertices emitted from the shader. Default
  value is 8. This corresponds to the GL_GEOMETRY_VERTICES_OUT_EXT
  enum.
*/

#include <Inventor/nodes/SoGeometryShader.h>

#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLShaderProgramElement.h>
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/misc/SoGLDriverDatabase.h>

#include "shaders/SoGLShaderProgram.h"
#include "nodes/SoSubNodeP.h"
#include "glue/glp.h"

// *************************************************************************

SO_NODE_SOURCE(SoGeometryShader);

// *************************************************************************

// doc from parent
void
SoGeometryShader::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoGeometryShader,
                              SO_FROM_COIN_2_5);
}

/*!
  Constructor.
*/
SoGeometryShader::SoGeometryShader(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoGeometryShader);

  SO_NODE_ADD_FIELD(inputType, (TRIANGLES_IN));
  SO_NODE_ADD_FIELD(outputType, (TRIANGLE_STRIP_OUT));
  SO_NODE_ADD_FIELD(maxEmit, (8));

  SO_NODE_DEFINE_ENUM_VALUE(InputType, POINTS_IN);
  SO_NODE_DEFINE_ENUM_VALUE(InputType, LINES_IN);
  SO_NODE_DEFINE_ENUM_VALUE(InputType, TRIANGLES_IN);

  SO_NODE_DEFINE_ENUM_VALUE(OutputType, POINTS_OUT);
  SO_NODE_DEFINE_ENUM_VALUE(OutputType, LINE_STRIP_OUT);
  SO_NODE_DEFINE_ENUM_VALUE(OutputType, TRIANGLE_STRIP_OUT);

  SO_NODE_SET_SF_ENUM_TYPE(inputType, InputType);
  SO_NODE_SET_SF_ENUM_TYPE(outputType, OutputType);
}

/*!
  Destructor.
*/
SoGeometryShader::~SoGeometryShader()
{
}

void
SoGeometryShader::GLRender(SoGLRenderAction * action)
{
  if (this->isActive.getValue()) {
    SoState * state = action->getState();
    const uint32_t cachecontext = SoGLCacheContextElement::get(state);
    const cc_glglue * glue = cc_glglue_instance(cachecontext);
    
    if (!SoGLDriverDatabase::isSupported(glue, "GL_EXT_geometry_shader4")) {
      static int first = 1;
      if (first) {
        first = 0;
        SoDebugError::post("SoGeometryShader::GLRender",
                           "Geometry shaders not support by hardware/driver");
      }
      return; // don't add this object to the program
    }
    else {
      GLenum input = 0;
      GLenum output = 0;

      switch (this->inputType.getValue()) {
      default:
        assert(0 && "invalid input type");
      case POINTS_IN:
        input = GL_POINTS;
        break;
      case LINES_IN:
        input = GL_LINES;
        break;
      case TRIANGLES_IN:
        input = GL_TRIANGLES;
        break;
      }
      switch (this->outputType.getValue()) {
      default:
        assert(0 && "invalid input type");
      case POINTS_OUT:
        output = GL_POINTS;
        break;
      case LINE_STRIP_OUT:
        output = GL_LINE_STRIP;
        break;
      case TRIANGLE_STRIP_OUT:
        output = GL_TRIANGLE_STRIP;
        break;
      }
      SoGLShaderProgram * shaderProgram = SoGLShaderProgramElement::get(action->getState());
      assert(shaderProgram);

      shaderProgram->addProgramParameter(GL_GEOMETRY_INPUT_TYPE_EXT, input);
      shaderProgram->addProgramParameter(GL_GEOMETRY_OUTPUT_TYPE_EXT, output);
      shaderProgram->addProgramParameter(GL_GEOMETRY_VERTICES_OUT_EXT, this->maxEmit.getValue());
    }
  }
  inherited::GLRender(action);
}

// *************************************************************************

/*!
  Returns a boolean indicating whether the requested source type is
  supported by the OpenGL driver or not.

  <i>Beware:</i> To get a correct answer, a valid OpenGL context must
  be available.
*/
SbBool
SoGeometryShader::isSupported(SourceType sourceType)
{
  // The function signature is not very well designed, as we really
  // need a guaranteed GL context for this. (We've chosen to be
  // compatible with TGS Inventor, so don't change the signature.)

  void * ptr = coin_gl_current_context();
  assert(ptr && "No active OpenGL context found!");
  if (!ptr) return FALSE; // Always bail out. Even when compiled in 'release' mode.

  const cc_glglue * glue = cc_glglue_instance_from_context_ptr(ptr);

  if (sourceType == ARB_PROGRAM) {
    return FALSE;
  }
  else if (sourceType == GLSL_PROGRAM) {
    return SoGLDriverDatabase::isSupported(glue, SO_GL_ARB_SHADER_OBJECT);
  }
  // AFAIK Cg has no support for geometry shaders (yet).
  // pederb, 20070410
  else if (sourceType == CG_PROGRAM) return FALSE;

  return FALSE;
}

// *************************************************************************

#ifdef COIN_TEST_SUITE

BOOST_AUTO_TEST_CASE(initialized)
{
  SoGeometryShader * node = new SoGeometryShader;
  assert(node);
  node->ref();
  BOOST_CHECK_MESSAGE(node->getTypeId() != SoType::badType(),
                      "missing class initialization");
  node->unref();
}

#endif // COIN_TEST_SUITE
