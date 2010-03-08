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
  \class SoTexture3Transform SoTexture3Transform.h Inventor/nodes/SoTexture3Transform.h
  \brief The SoTexture3Transform class is used to define 3D texture transformations.
  \ingroup nodes

  Textures applied to shapes in the scene can be transformed by
  "prefixing" in the state with instances of this node
  type. Translations, rotations and scaling in 3D can all be done.

  The default settings of this node's fields equals a "null
  transform", ie no transformation.

  \COIN_CLASS_EXTENSION

  <b>FILE FORMAT/DEFAULTS:</b>
  \code
    Texture3Transform {
        translation 0 0 0
        rotation 0 0 1  0
        scaleFactor 1 1 1
        scaleOrientation 0 0 1  0
        center 0 0 0
    }
  \endcode

  \sa SoTexture2Transform
  \since Coin 2.0
  \since TGS Inventor 2.6
*/

// *************************************************************************

#include <Inventor/nodes/SoTexture3Transform.h>

#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/elements/SoGLMultiTextureMatrixElement.h>
#include <Inventor/elements/SoTextureUnitElement.h>
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/C/glue/gl.h>

#include "nodes/SoSubNodeP.h"

// *************************************************************************

/*!
  \var SoSFVec3f SoTexture3Transform::translation

  Texture coordinate translation. Default value is [0, 0, 0].
*/
/*!
  \var SoSFRotation SoTexture3Transform::rotation

  Texture coordinate rotation (s is x-axis, t is y-axis and r is
  z-axis).  Defaults to an identity rotation (ie zero rotation).
*/
/*!
  \var SoSFVec3f SoTexture3Transform::scaleFactor

  Texture coordinate scale factors. Default value is [1, 1, 1].
*/
/*!
  \var SoSFRotation SoTexture3Transform::scaleOrientation

  The orientation the texture is set to before scaling.  Defaults to
  an identity rotation (ie zero rotation).
*/
/*!
  \var SoSFVec3f SoTexture3Transform::center

  Center for scale and rotation. Default value is [0, 0, 0].
*/

// *************************************************************************

SO_NODE_SOURCE(SoTexture3Transform);

/*!
  Constructor.
*/
SoTexture3Transform::SoTexture3Transform(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoTexture3Transform);

  SO_NODE_ADD_FIELD(translation, (0.0f, 0.0f, 0.0f));
  SO_NODE_ADD_FIELD(rotation, (SbRotation(SbVec3f(0.0f, 0.0f, 1.0f), 0.0f)));
  SO_NODE_ADD_FIELD(scaleFactor, (1.0f, 1.0f, 1.0f));
  SO_NODE_ADD_FIELD(scaleOrientation, (SbRotation(SbVec3f(0.0f, 0.0f, 1.0f), 0.0f)));
  SO_NODE_ADD_FIELD(center, (0.0f, 0.0f, 0.0f));
}

/*!
  Destructor.
*/
SoTexture3Transform::~SoTexture3Transform()
{
}

// Documented in superclass.
void
SoTexture3Transform::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoTexture3Transform, SO_FROM_INVENTOR_1);

  SO_ENABLE(SoGLRenderAction, SoGLMultiTextureMatrixElement);
  SO_ENABLE(SoCallbackAction, SoMultiTextureMatrixElement);
  SO_ENABLE(SoPickAction, SoMultiTextureMatrixElement);
}


// Documented in superclass.
void
SoTexture3Transform::GLRender(SoGLRenderAction * action)
{
  SoState * state = action->getState();
  int unit = SoTextureUnitElement::get(state); 
  const cc_glglue * glue = 
    cc_glglue_instance(SoGLCacheContextElement::get(state));
  int maxunits = cc_glglue_max_texture_units(glue);
  
  if (unit < maxunits) {
    SbMatrix mat;
    mat.setTransform(this->translation.getValue(),
                     this->rotation.getValue(),
                     this->scaleFactor.getValue(),
                     this->scaleOrientation.getValue(),
                     this->center.getValue());
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
SoTexture3Transform::doAction(SoAction *action)
{
  SoState * state = action->getState();
  int unit = SoTextureUnitElement::get(state); 
  SbMatrix mat;
  mat.setTransform(this->translation.getValue(),
                   this->rotation.getValue(),
                   this->scaleFactor.getValue(),
                   this->scaleOrientation.getValue(),
                   this->center.getValue());
  SoMultiTextureMatrixElement::mult(action->getState(), this, unit, mat);
}

// Documented in superclass.
void
SoTexture3Transform::callback(SoCallbackAction *action)
{
  SoTexture3Transform::doAction(action);
}

// Documented in superclass.
void
SoTexture3Transform::getMatrix(SoGetMatrixAction * action)
{
  int unit = SoTextureUnitElement::get(action->getState()); 
  if (unit == 0) {
    SbMatrix mat;
    mat.setTransform(this->translation.getValue(),
                     this->rotation.getValue(),
                     this->scaleFactor.getValue(),
                     this->scaleOrientation.getValue(),
                     this->center.getValue());
    action->getTextureMatrix().multLeft(mat);
    action->getTextureInverse().multRight(mat.inverse());
  }
}

// Documented in superclass.
void
SoTexture3Transform::pick(SoPickAction * action)
{
  SoTexture3Transform::doAction(action);
}
