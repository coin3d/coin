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
  \class SoTextureMatrixTransform SoTextureMatrixTransform.h Inventor/nodes/SoTextureMatrixTransform.h
  \brief The SoTextureMatrixTransform class is used to define a texture matrix transformation.
  \ingroup nodes

  Textures applied to shapes in the scene can be transformed by
  "prefixing" in the state with instances of this node type.

  \COIN_CLASS_EXTENSION

  <b>FILE FORMAT/DEFAULTS:</b>
  \code
    TextureMatrixTransform {
        matrix
          1 0 0 0
          0 1 0 0
          0 0 1 0
          0 0 0 1
    }
  \endcode

  \sa SoTexture3Transform
  \since Coin 2.5
*/

// *************************************************************************

#include <Inventor/nodes/SoTextureMatrixTransform.h>

#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/elements/SoGLMultiTextureMatrixElement.h>
#include <Inventor/elements/SoTextureUnitElement.h>
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/elements/SoShapeStyleElement.h>
#include <Inventor/C/glue/gl.h>

#include "nodes/SoSubNodeP.h"

// *************************************************************************

/*!
  \var SoSFVec3f SoTextureMatrixTransform::matrix

  Texture coordinate matrix. Default is the identity matrix.
*/

// *************************************************************************

SO_NODE_SOURCE(SoTextureMatrixTransform);

/*!
  Constructor.
*/
SoTextureMatrixTransform::SoTextureMatrixTransform(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoTextureMatrixTransform);

  SO_NODE_ADD_FIELD(matrix, (SbMatrix::identity()));
}

/*!
  Destructor.
*/
SoTextureMatrixTransform::~SoTextureMatrixTransform()
{
}

// Documented in superclass.
void
SoTextureMatrixTransform::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoTextureMatrixTransform, SO_FROM_INVENTOR_1);

  SO_ENABLE(SoGLRenderAction, SoGLMultiTextureMatrixElement);
  SO_ENABLE(SoCallbackAction, SoMultiTextureMatrixElement);
  SO_ENABLE(SoPickAction, SoMultiTextureMatrixElement);
}


// Documented in superclass.
void
SoTextureMatrixTransform::GLRender(SoGLRenderAction * action)
{
  SoState * state = action->getState();

  // don't modify the texture matrix while rendering the shadow map
  if (SoShapeStyleElement::get(state)->getFlags() & SoShapeStyleElement::SHADOWMAP) return;

  int unit = SoTextureUnitElement::get(state);
  const cc_glglue * glue =
    cc_glglue_instance(SoGLCacheContextElement::get(state));
  int maxunits = cc_glglue_max_texture_units(glue);
  
  if (unit < maxunits) {
    SbMatrix mat = this->matrix.getValue();
    SoMultiTextureMatrixElement::mult(state, this, unit, mat);
  }
  else {
    // we already warned in SoTextureUnit. I think it's best to just
      // ignore the texture here so that all textures for non-supported
      // units will be ignored. pederb, 2003-11-11
  }
}

// Documented in superclass.
void
SoTextureMatrixTransform::doAction(SoAction *action)
{
  SbMatrix mat = this->matrix.getValue();
  int unit = SoTextureUnitElement::get(action->getState());
  SoMultiTextureMatrixElement::mult(action->getState(), this, unit, mat);
}

// Documented in superclass.
void
SoTextureMatrixTransform::callback(SoCallbackAction *action)
{
  SoTextureMatrixTransform::doAction(action);
}

// Documented in superclass.
void
SoTextureMatrixTransform::getMatrix(SoGetMatrixAction * action)
{
  int unit = SoTextureUnitElement::get(action->getState()); 
  if (unit == 0) {
    SbMatrix mat = this->matrix.getValue();
    action->getTextureMatrix().multLeft(mat);
    action->getTextureInverse().multRight(mat.inverse());
  }
}

// Documented in superclass.
void
SoTextureMatrixTransform::pick(SoPickAction * action)
{
  SoTextureMatrixTransform::doAction(action);
}
