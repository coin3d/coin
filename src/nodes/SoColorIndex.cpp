/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

/*!
  \class SoColorIndex SoColorIndex.h Inventor/nodes/SoColorIndex.h
  \brief The SoColorIndex class is used to specify color indices for subsequent shapes.
  \ingroup nodes

  This node should only be used in OpenGL color-index mode, and 
  only when light model is BASE_COLOR.
*/
// FIXME: couldn't we check for the above mentioned pre-conditions and
// assert() or SoDebugError::post() if any of the two is not met?
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
  SO_NODE_INTERNAL_INIT_CLASS(SoColorIndex);

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
