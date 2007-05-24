/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2007 by Systems in Motion.  All rights reserved.
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
 *  See http://www.coin3d.org/ for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

/*!
  \class SoShadowCulling SoShadowCulling.h Inventor/annex/FXViz/nodes/SoShadowCulling.h
  \brief The SoShadowCulling class is a node for setting the shadow style on nodes.

  \ingroup nodes

  <b>FILE FORMAT/DEFAULTS:</b>
  \code
    ShadowCulling {
        style CASTS_SHADOW_AND_SHADOWED
    }
  \endcode

  \since Coin 2.5
*/

// *************************************************************************

#include <Inventor/annex/FXViz/nodes/SoShadowCulling.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/elements/SoShapeStyleElement.h>
#include <Inventor/elements/SoShapeHintsElement.h>
#include <Inventor/elements/SoOverrideElement.h>
#include <Inventor/elements/SoGLShaderProgramElement.h>
#include "../shaders/SoGLShaderProgram.h"
#include <Inventor/actions/SoGLRenderAction.h>
#include <stdio.h>

// *************************************************************************

/*!
  \enum SoShadowCulling::Style

  Enumerates the available style settings.
*/

/*!
  \var SoSFEnum SoShadowCulling::style

  The units which will be used for nodes following this node in the
  traversal (until the next SoUnit node, if any). Default value of the
  field is SoShadowCulling::CASTS_SHADOW_AND_SHADOWED.
*/

/*!
  \var SoShadowCulling::Style SoShadowCulling::NO_SHADOWING
  Neither casts or recieves shadows.
*/

/*!
  \var SoShadowCulling::Style SoShadowCulling::CASTS_SHADOW
  Casts shadow, but will no recieve any shadows.
*/

/*!
  \var SoShadowCulling::Style SoShadowCulling::SHADOWED
  Receives shadows, but will not cast any shadow.
*/

/*!
  \var SoShadowCulling::Style SoShadowCulling::CAST_SHADOW_AND_SHADOWED
  Will cast and recieve shadows.
*/

// *************************************************************************


SO_NODE_SOURCE(SoShadowCulling);

/*!
  Constructor.
*/
SoShadowCulling::SoShadowCulling(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoShadowCulling);

  SO_NODE_ADD_FIELD(mode, (AS_IS_CULLING));

  SO_NODE_DEFINE_ENUM_VALUE(Mode, AS_IS_CULLING);
  SO_NODE_DEFINE_ENUM_VALUE(Mode, NO_CULLING);
  SO_NODE_SET_SF_ENUM_TYPE(mode, Mode);
}

/*!
  Destructor.
*/
SoShadowCulling::~SoShadowCulling()
{
}

// Doc from superclass.
void
SoShadowCulling::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoShadowCulling, SO_FROM_COIN_2_5);
  SO_ENABLE(SoGLRenderAction, SoGLShadowCullingElement);
}

// Doc from superclass.
void
SoShadowCulling::GLRender(SoGLRenderAction * action)
{
  SoState * state = action->getState();

  if (SoShapeStyleElement::get(state)->getFlags() & SoShapeStyleElement::SHADOWMAP) {
    int32_t mode = this->mode.getValue();
    SoGLShadowCullingElement::set(state, this, mode);
    
    if (mode == NO_CULLING) {
      SoShapeHintsElement::set(state, NULL, 
                               SoShapeHintsElement::UNKNOWN_ORDERING,
                               SoShapeHintsElement::UNKNOWN_SHAPE_TYPE,
                               SoShapeHintsElement::UNKNOWN_FACE_TYPE);
      SoOverrideElement::setShapeHintsOverride(state, NULL, TRUE);
    }
    else {
      // FIXME: need to restore the previous ShapeHints settings in some way,
      // or require that this node is used only inside a separator
      SoOverrideElement::setShapeHintsOverride(state, NULL, FALSE);
    }
  }
}
