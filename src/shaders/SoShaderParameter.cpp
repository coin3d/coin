#include <Inventor/nodes/SoShaderParameter.h>

#include "SoGLShaderObject.h"
#include "SoGLCgShader.h"
#include <iostream>
#include <assert.h>

/* **************************************************************************
 * ***                             SoShaderParameter                      ***
 * **************************************************************************/

SO_NODE_ABSTRACT_SOURCE(SoShaderParameter);

void SoShaderParameter::initClass()
{
  SO_NODE_INIT_ABSTRACT_CLASS(SoShaderParameter, SoNode, "Node");
}

SoShaderParameter::SoShaderParameter()
{
  SO_NODE_CONSTRUCTOR(SoShaderParameter);

  SO_NODE_ADD_FIELD(name,       (""));
  SO_NODE_ADD_FIELD(identifier, (0));
}

SoShaderParameter::~SoShaderParameter()
{
}

/* **************************************************************************
 * ***                      SoUniformShaderParameter                      ***
 * **************************************************************************/


SO_NODE_ABSTRACT_SOURCE(SoUniformShaderParameter);

void SoUniformShaderParameter::initClass()
{
  SO_NODE_INIT_ABSTRACT_CLASS(SoUniformShaderParameter, 
			      SoShaderParameter, "SoShaderParameter");
}

SoUniformShaderParameter::SoUniformShaderParameter()
{
  SO_NODE_CONSTRUCTOR(SoUniformShaderParameter);

  this->parameter = NULL;
}

SoUniformShaderParameter::~SoUniformShaderParameter()
{
  if (this->parameter) { delete (this->parameter); this->parameter = NULL; }
}

void SoUniformShaderParameter::reset()
{
  if (this->parameter) { delete (this->parameter); this->parameter = NULL; }
}

SbBool SoUniformShaderParameter::ensureParameter(SoGLShaderObject*     shader, 
						 SoGLShader::ValueType type)
{
  if (shader == NULL) return FALSE;
  
  if (this->parameter == NULL) {
    const char* str  = this->name.getValue().getString();
    int index        = this->identifier.getValue();
    this->parameter  = shader->getParameter(index, str, type);
  }
  return TRUE;
}


/* **************************************************************************
 * ***                         SoShaderParameter1f                        ***
 * **************************************************************************/


SO_NODE_SOURCE(SoShaderParameter1f);

void SoShaderParameter1f::initClass()
{
  SO_NODE_INIT_CLASS(SoShaderParameter1f, SoUniformShaderParameter,
		     "SoUniformShaderParameter");
}

SoShaderParameter1f::SoShaderParameter1f()
{
  SO_NODE_CONSTRUCTOR(SoShaderParameter1f);
  SO_NODE_ADD_FIELD(value,       (0));
}

SoShaderParameter1f::~SoShaderParameter1f()
{  
}

void SoShaderParameter1f::updateParameter(SoGLShaderObject *shader)
{
  if (!ensureParameter(shader, SoGLShader::FLOAT)) return;
  this->parameter->set1f(this->value.getValue(),
			 this->name.getValue().getString(),
			 this->identifier.getValue());
}

/* **************************************************************************
 * ***                          SoShaderParameter2f                       ***
 * **************************************************************************/


SO_NODE_SOURCE(SoShaderParameter2f);

void SoShaderParameter2f::initClass()
{
  SO_NODE_INIT_CLASS(SoShaderParameter2f, SoUniformShaderParameter,
		     "SoUniformShaderParameter");
}

SoShaderParameter2f::SoShaderParameter2f()
{
  SO_NODE_CONSTRUCTOR(SoShaderParameter2f);
  SO_NODE_ADD_FIELD(value,       (0,0));
}

SoShaderParameter2f::~SoShaderParameter2f()
{  
}

void SoShaderParameter2f::updateParameter(SoGLShaderObject *shader)
{
  if (!ensureParameter(shader, SoGLShader::FLOAT2)) return;
  this->parameter->set2f(this->value.getValue().getValue(),
			 this->name.getValue().getString(),
			 this->identifier.getValue());
}

/* **************************************************************************
 * ***                          SoShaderParameter3f                       ***
 * **************************************************************************/


SO_NODE_SOURCE(SoShaderParameter3f);

void SoShaderParameter3f::initClass()
{
  SO_NODE_INIT_CLASS(SoShaderParameter3f, SoUniformShaderParameter,
		     "SoUniformShaderParameter");
}

SoShaderParameter3f::SoShaderParameter3f()
{
  SO_NODE_CONSTRUCTOR(SoShaderParameter3f);
  SO_NODE_ADD_FIELD(value,       (0,0,0));
}

SoShaderParameter3f::~SoShaderParameter3f()
{  
}

void SoShaderParameter3f::updateParameter(SoGLShaderObject *shader)
{
  if (!ensureParameter(shader, SoGLShader::FLOAT3)) return;
  this->parameter->set3f(this->value.getValue().getValue(),
			 this->name.getValue().getString(),
			 this->identifier.getValue());
}

/* **************************************************************************
 * ***                          SoShaderParameter4f                       ***
 * **************************************************************************/


SO_NODE_SOURCE(SoShaderParameter4f);

void SoShaderParameter4f::initClass()
{
  SO_NODE_INIT_CLASS(SoShaderParameter4f, SoUniformShaderParameter,
		     "SoUniformShaderParameter");
}

SoShaderParameter4f::SoShaderParameter4f()
{
  SO_NODE_CONSTRUCTOR(SoShaderParameter4f);
  SO_NODE_ADD_FIELD(value,       (0,0,0,0));
}

SoShaderParameter4f::~SoShaderParameter4f()
{  
}

void SoShaderParameter4f::updateParameter(SoGLShaderObject *shader)
{
  if (!ensureParameter(shader, SoGLShader::FLOAT4)) return;
  this->parameter->set4f(this->value.getValue().getValue(),
			 this->name.getValue().getString(),
			 this->identifier.getValue());
}

/* **************************************************************************
 * ***                     SoShaderStateMatrixParameter                   ***
 * **************************************************************************/

#if defined(SO_CG_SHADER_SUPPORT)

SO_NODE_SOURCE(SoShaderStateMatrixParameter);

void SoShaderStateMatrixParameter::initClass()
{
  SO_NODE_INIT_CLASS(SoShaderStateMatrixParameter, SoUniformShaderParameter,
		     "SoUniformShaderParameter");
}

SoShaderStateMatrixParameter::SoShaderStateMatrixParameter()
{
  SO_NODE_CONSTRUCTOR(SoShaderStateMatrixParameter);

  SO_NODE_DEFINE_ENUM_VALUE(MatrixType, MODELVIEW);
  SO_NODE_DEFINE_ENUM_VALUE(MatrixType, PROJECTION);
  SO_NODE_DEFINE_ENUM_VALUE(MatrixType, TEXTURE);
  SO_NODE_DEFINE_ENUM_VALUE(MatrixType, MODELVIEW_PROJECTION);

  SO_NODE_ADD_FIELD(matrixType,         (MODELVIEW));
  SO_NODE_SET_SF_ENUM_TYPE(matrixType,  MatrixType);


  SO_NODE_DEFINE_ENUM_VALUE(MatrixTransform, IDENTITY);
  SO_NODE_DEFINE_ENUM_VALUE(MatrixTransform, TRANSPOSE);
  SO_NODE_DEFINE_ENUM_VALUE(MatrixTransform, INVERSE);
  SO_NODE_DEFINE_ENUM_VALUE(MatrixTransform, INVERSE_TRANSPOSE);
  
  SO_NODE_ADD_FIELD(matrixTransform,        (IDENTITY));
  SO_NODE_SET_SF_ENUM_TYPE(matrixTransform, MatrixTransform);
}

SoShaderStateMatrixParameter::~SoShaderStateMatrixParameter()
{
  
}

// State matrices only work with CG!!!
void SoShaderStateMatrixParameter::updateParameter(SoGLShaderObject *shader)
{
  if (shader->shaderType() != SoGLShader::CG_SHADER) return;
  if (this->name.isDefault()) return;

  if (!ensureParameter(shader, SoGLShader::FLOAT_MATRIX4)) return;
  
  CGGLenum type  = getType((MatrixType)matrixType.getValue());
  CGGLenum tform = getTransform((MatrixTransform)matrixTransform.getValue());

  SoGLCgShaderParameter *param = (SoGLCgShaderParameter *)this->parameter;
  param->setState(type, tform, this->name.getValue().getString());
}

CGGLenum SoShaderStateMatrixParameter::getType(MatrixType type)
{
  switch (type) {
    case            MODELVIEW: return CG_GL_MODELVIEW_MATRIX;
    case           PROJECTION: return CG_GL_PROJECTION_MATRIX;
    case              TEXTURE: return CG_GL_TEXTURE_MATRIX;
    case MODELVIEW_PROJECTION: return CG_GL_MODELVIEW_PROJECTION_MATRIX;
                      default: assert(FALSE); return CG_GL_MODELVIEW_MATRIX;
  }
}

CGGLenum SoShaderStateMatrixParameter::getTransform(MatrixTransform tform)
{
  switch (tform) {
    case          IDENTITY: return CG_GL_MATRIX_IDENTITY;
    case         TRANSPOSE: return CG_GL_MATRIX_TRANSPOSE;
    case           INVERSE: return CG_GL_MATRIX_INVERSE;
    case INVERSE_TRANSPOSE: return CG_GL_MATRIX_INVERSE_TRANSPOSE;
                   default: assert(FALSE); return CG_GL_MATRIX_IDENTITY;
  }
}

#endif /* SO_CG_SHADER_SUPPORT */

/* **************************************************************************
 * ***                      SoShaderParameterSampler2D                    ***
 * **************************************************************************/

SO_NODE_SOURCE(SoShaderParameterSampler2D);

void SoShaderParameterSampler2D::initClass()
{
  SO_NODE_INIT_CLASS(SoShaderParameterSampler2D, SoUniformShaderParameter,
		     "SoUniformShaderParameter");
}

SoShaderParameterSampler2D::SoShaderParameterSampler2D()
{
  SO_NODE_CONSTRUCTOR(SoShaderParameterSampler2D);

  SO_NODE_ADD_FIELD(filename, (""));
  //SO_NODE_ADD_FIELD(image,    ());
}

SoShaderParameterSampler2D::~SoShaderParameterSampler2D()
{  
}

void SoShaderParameterSampler2D::updateParameter(SoGLShaderObject *shader)
{
  if (!ensureParameter(shader, SoGLShader::TEXTURE2D)) return;
  //this->parameter->set4f(this->value.getValue().getValue());
}
