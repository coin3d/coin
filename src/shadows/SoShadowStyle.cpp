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
  \class SoShadowStyle SoShadowStyle.h Inventor/annex/FXViz/nodes/SoShadowStyle.h
  \brief The SoShadowStyle class is a node for setting the shadow style on nodes.

  \ingroup nodes

  <b>FILE FORMAT/DEFAULTS:</b>
  \code
    ShadowStyle {
        style CASTS_SHADOW_AND_SHADOWED
    }
  \endcode

  \since Coin 2.5
*/

// *************************************************************************

#include <Inventor/annex/FXViz/nodes/SoShadowStyle.h>

#include <stdio.h>

#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoShapeStyleElement.h>
#include <Inventor/elements/SoGLShaderProgramElement.h>

#include "nodes/SoSubNodeP.h"
#include "shaders/SoGLShaderProgram.h"

// *************************************************************************

/*!
  \enum SoShadowStyle::Style

  Enumerates the available style settings.
*/

/*!
  \var SoSFEnum SoShadowStyle::style

  Set the current shadow style. Default value is CAST_SHADOW_AND_SHADOWED.
*/

/*!
  \var SoShadowStyle::Style SoShadowStyle::NO_SHADOWING
  Neither casts or recieves shadows.
*/

/*!
  \var SoShadowStyle::Style SoShadowStyle::CASTS_SHADOW
  Casts shadow, but will no recieve any shadows.
*/

/*!
  \var SoShadowStyle::Style SoShadowStyle::SHADOWED
  Receives shadows, but will not cast any shadow.
*/

/*!
  \var SoShadowStyle::Style SoShadowStyle::CAST_SHADOW_AND_SHADOWED
  Will cast and recieve shadows.
*/

// *************************************************************************


SO_NODE_SOURCE(SoShadowStyle);

/*!
  Constructor.
*/
SoShadowStyle::SoShadowStyle(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoShadowStyle);

  SO_NODE_ADD_FIELD(style, (SoShadowStyle::CASTS_SHADOW_AND_SHADOWED));

  SO_NODE_DEFINE_ENUM_VALUE(Style, NO_SHADOWING);
  SO_NODE_DEFINE_ENUM_VALUE(Style, CASTS_SHADOW);
  SO_NODE_DEFINE_ENUM_VALUE(Style, SHADOWED);
  SO_NODE_DEFINE_ENUM_VALUE(Style, CASTS_SHADOW_AND_SHADOWED);
  SO_NODE_SET_SF_ENUM_TYPE(style, Style);
}

/*!
  Destructor.
*/
SoShadowStyle::~SoShadowStyle()
{
}

// Doc from superclass.
void
SoShadowStyle::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoShadowStyle, SO_FROM_COIN_2_5);
  SO_ENABLE(SoGLRenderAction, SoShadowStyleElement);
}

// Doc from superclass.
void
SoShadowStyle::GLRender(SoGLRenderAction * action)
{
  SoState * state = action->getState();

  SoShadowStyleElement::set(state,
                            this,
                            (int) this->style.getValue());

  if (SoShapeStyleElement::get(state)->getFlags() & SoShapeStyleElement::SHADOWS) {

    if (this->style.getValue() & SHADOWED) {
      SoGLShaderProgramElement::enable(state, TRUE);
    }
    else {
      SoGLShaderProgramElement::enable(state, FALSE);
    }
  }
}



#ifdef COIN_TEST_SUITE

BOOST_AUTO_TEST_CASE(initialized)
{
  SoShadowStyle * node = new SoShadowStyle;
  assert(node);
  node->ref();
  BOOST_CHECK_MESSAGE(node->getTypeId() != SoType::badType(),
                      "missing class initialization");
  node->unref();
}

#endif // COIN_TEST_SUITE
