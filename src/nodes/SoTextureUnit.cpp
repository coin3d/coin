/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoTextureUnit SoTextureUnit.h Inventor/nodes/SoTextureUnit.h
  \brief The SoTextureUnit class is a node for setting the active texture unit.
  \ingroup nodes

  \since Coin 2.2
*/

#include <Inventor/nodes/SoTextureUnit.h>
#include <Inventor/nodes/SoSubNodeP.h>

#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/elements/SoTextureUnitElement.h>
#include <Inventor/elements/SoGLMultiTextureCoordinateElement.h>
#include <Inventor/elements/SoGLMultiTextureImageElement.h>
#include <Inventor/elements/SoGLMultiTextureEnabledElement.h>
#include <Inventor/elements/SoGLMultiTextureMatrixElement.h>
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/C/glue/gl.h>
#include <Inventor/errors/SoDebugError.h>

/*!
  \var SoSFInt32 SoTextureUnit::unit

  The texture unit which will be used for texture nodes following this
  node in the traversal. Default value of the field is 0.
*/

/*!
  \var SoSFEnum SoTextureUnit::mappingMethod

  The mapping method for this unit. Default is IMAGE_MAPPING.
*/


/*!
  \var SoTextureUnit::MappingMethod SoTextureUnit::IMAGE_MAPPING

  Normal image mapping is used.
*/

/*!
  \var SoTextureUnit::MappingMethod SoTextureUnit::BUMP_MAPPING

  Bump mapping is used.
*/

// *************************************************************************

SO_NODE_SOURCE(SoTextureUnit);

/*!
  Constructor.
*/
SoTextureUnit::SoTextureUnit(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoTextureUnit);
  
  SO_NODE_ADD_FIELD(unit, (0));
  SO_NODE_ADD_FIELD(mappingMethod, (IMAGE_MAPPING));
  
  SO_NODE_DEFINE_ENUM_VALUE(MappingMethod, IMAGE_MAPPING);
  SO_NODE_DEFINE_ENUM_VALUE(MappingMethod, BUMP_MAPPING);
  
  SO_NODE_SET_SF_ENUM_TYPE(mappingMethod, MappingMethod);
}

/*!
  Destructor.
*/
SoTextureUnit::~SoTextureUnit()
{
}

// Doc from superclass.
void
SoTextureUnit::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoTextureUnit, SO_FROM_COIN_2_2);

  SO_ENABLE(SoGLRenderAction, SoTextureUnitElement);
  SO_ENABLE(SoGLRenderAction, SoGLMultiTextureCoordinateElement);
  SO_ENABLE(SoGLRenderAction, SoGLMultiTextureImageElement);
  SO_ENABLE(SoGLRenderAction, SoGLMultiTextureEnabledElement);
  SO_ENABLE(SoGLRenderAction, SoGLMultiTextureMatrixElement);
}

// Doc from superclass.
void
SoTextureUnit::GLRender(SoGLRenderAction * action)
{
  SoTextureUnit::doAction((SoAction*)action);

  SoState * state = action->getState();
  const cc_glglue * glue = cc_glglue_instance(SoGLCacheContextElement::get(state));
  int maxunits = cc_glglue_max_texture_units(glue);
  
  if (this->unit.getValue() >= maxunits) {
    static SbBool first = TRUE;
    if (first) {
      SoDebugError::postWarning("SoTextureUnit::GLRender",
                                "Texture unit %d (counting from 0) requested. "
                                "Your system only supports %d texture unit%s. "
                                "(This warning message only shown once, but "
                                "there could be more cases of this in the "
                                "scene graph.)",
                                this->unit.getValue(), maxunits,
                                maxunits == 1 ? "" : "s");
      first = FALSE;
    }
  }
}

// Doc from superclass.
void
SoTextureUnit::doAction(SoAction * action)
{
  SoTextureUnitElement::set(action->getState(), this,
                            this->unit.getValue());
}

// Doc from superclass.
void
SoTextureUnit::callback(SoCallbackAction * action)
{
  // So far only SoGLRenderAction supports SoTextureUnitElement.  We
  // may never support multiple texture units for SoCallbackAction,
  // but we reimplement the method just in case
  inherited::callback(action);
}

// Doc from superclass.
void
SoTextureUnit::pick(SoPickAction * action)
{
  // So far only SoGLRenderAction supports SoTextureUnitElement.  We
  // may never support multiple texture units for SoPickAction, but we
  // reimplement the method just in case
  inherited::pick(action);
}

/*!

  Returns the maximum number of texture units for the current GL
  context.  Do not call this method if you don't have a current active
  GL context. You should also know that your OpenGL driver supports
  multi-texturing.

  This function is provided only to be compatible with TGS Inventor.
  It's better to use cc_glglue_max_texture_units() if you're using
  Coin (declared in Inventor/C/glue/gl.h).
*/
uint32_t 
SoTextureUnit::getMaxTextureUnit(void)
{
  GLint tmp;
  glGetIntegerv(GL_MAX_TEXTURE_UNITS, &tmp);
  
  return (uint32_t) tmp;
}
