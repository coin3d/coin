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
  \class SoColorIndex SoColorIndex.h Inventor/nodes/SoColorIndex.h
  \brief The SoColorIndex class is used to specify color indices for subsequent shapes.
  \ingroup nodes

  This node should only be used in OpenGL color-index mode, and only
  when the current light model is set to SoLightModel::BASE_COLOR.

  OpenGL colorindex mode is where the colors for individual pixels are
  fetched from a color lookup table ("CLUT"). The usual thing to do is
  to set up a canvas in RGBA truecolor mode.

  One common use for colorindex mode OpenGL canvases is to use one in
  the overlay planes (which are usually limited to only 2 or 4
  available colors), if supported by the OpenGL hardware and / or
  driver.
*/

// FIXME: couldn't we check for the above mentioned pre-conditions
// (colorindex mode, SoLightModel::BASE_COLOR) and assert() or
// SoDebugError::post() if any of the two is not met?
//
// UPDATE: use glGetBooleanv(GL_RGBA_MODE, ...) or
// SoGLColorIndexElement::isColorIndexMode() for the color-index
// mode-check.
//
// 20010809 mortene.

#include <Inventor/nodes/SoColorIndex.h>
#include <Inventor/elements/SoOverrideElement.h>
#include <Inventor/elements/SoGLColorIndexElement.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/nodes/SoSubNodeP.h>

/*!
  \var SoMFInt32 SoColorIndex::index
  Color indices which can be used by shapes.
*/

// *************************************************************************

SO_NODE_SOURCE(SoColorIndex);

/*!
  Constructor.
*/
SoColorIndex::SoColorIndex()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoColorIndex);

  SO_NODE_ADD_FIELD(index, (1));
}

/*!
  Destructor.
*/
SoColorIndex::~SoColorIndex()
{
}

// doc in parent
void
SoColorIndex::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoColorIndex, SO_FROM_INVENTOR_1);

  SO_ENABLE(SoGLRenderAction, SoGLColorIndexElement);
}

// doc in parent
void
SoColorIndex::GLRender(SoGLRenderAction * action)
{
  SoState * state = action->getState();
  if (!this->index.isIgnored() &&
      !SoOverrideElement::getColorIndexOverride(state)) {
    if (this->isOverride()) {
      SoOverrideElement::setColorIndexOverride(state, this, TRUE);
    }
    SoGLColorIndexElement::set(state, this,
                               this->index.getNum(),
                               this->index.getValues(0));
  }
}
