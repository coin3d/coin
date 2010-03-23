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
  \class SoShadowCulling SoShadowCulling.h Inventor/annex/FXViz/nodes/SoShadowCulling.h
  \brief The SoShadowCulling class is a node for setting the shadow style on nodes.

  \ingroup nodes

  <b>FILE FORMAT/DEFAULTS:</b>
  \code
    ShadowCulling {
        mode AS_IS_CULLING
    }
  \endcode

  \since Coin 2.5
*/

// *************************************************************************

#include <Inventor/annex/FXViz/nodes/SoShadowCulling.h>

#include <stdio.h>

#include <Inventor/elements/SoShapeStyleElement.h>
#include <Inventor/elements/SoShapeHintsElement.h>
#include <Inventor/elements/SoOverrideElement.h>
#include <Inventor/elements/SoGLShaderProgramElement.h>
#include <Inventor/actions/SoGLRenderAction.h>

#include "nodes/SoSubNodeP.h"
#include "shaders/SoGLShaderProgram.h"

// *************************************************************************

/*!
  \enum SoShadowCulling::Mode

  Enumerates the available culling modes.
*/

/*!
  \var SoSFEnum SoShadowCulling::mode
  
  Sets the culling mode. Default is AS_IS_CULLING.
*/

/*!
  \var SoShadowCulling::Mode SoShadowCulling::AS_IS_CULLING
  
  Use the culling specified in the scene graph.
*/

/*!
  \var SoShadowCulling::Style SoShadowCulling::NO_CULLING

  Render both backfacing and frontfacting triangles into the shadow map.
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


#ifdef COIN_TEST_SUITE

BOOST_AUTO_TEST_CASE(initialized)
{
  SoShadowCulling * node = new SoShadowCulling;
  assert(node);
  node->ref();
  BOOST_CHECK_MESSAGE(node->getTypeId() != SoType::badType(),
                      "missing class initialization");
  node->unref();
}

#endif // COIN_TEST_SUITE
