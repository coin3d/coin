/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

/*!
  \class SoMaterialBundle include/Inventor/SoMaterialBundle.h
  \brief The SoMaterialBundle class simplifies material handling.
  \ingroup bundles

  Every shape node should create (on the stack) an instance of this
  class and call sendFirst() before sending anything to GL. During
  rendering, send() should be used to send material values to GL.
*/

#include <Inventor/bundles/SoMaterialBundle.h>
#include <Inventor/elements/SoGLAmbientColorElement.h>
#include <Inventor/elements/SoGLDiffuseColorElement.h>
#include <Inventor/elements/SoGLEmissiveColorElement.h>
#include <Inventor/elements/SoGLPolygonStippleElement.h>
#include <Inventor/elements/SoGLShininessElement.h>
#include <Inventor/elements/SoGLSpecularColorElement.h>
#include <Inventor/elements/SoTransparencyElement.h>
#include <Inventor/elements/SoLightModelElement.h>
#include <Inventor/elements/SoShapeStyleElement.h>
#include <Inventor/elements/SoGLColorIndexElement.h>
#include <Inventor/misc/SoState.h>

#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/system/gl.h>

/*!
  Constructor with \a action being the action applied to the
  geometry node.
*/
SoMaterialBundle::SoMaterialBundle(SoAction *action)
  : SoBundle(action)
{
  this->firstTime = TRUE; // other members will be set in setUpElements
}

/*!
  Destructor
*/
SoMaterialBundle::~SoMaterialBundle()
{
}

/*!
  Currently not in use. It is only provided for OIV compliance.
*/
void
SoMaterialBundle::setUpMultiple(void)
{
  // FIXME: does this represent an unimplemented feature, or is it an
  // obsoleted method? Mark it properly.  20010903 mortene.
}

/*!
  Sends the initial material values to GL. Must be done once in all
  geometry nodes before the rendering begins.
*/
void
SoMaterialBundle::sendFirst(void)
{
  this->setupElements(FALSE);
  this->reallySend(0, FALSE);
  if (this->stippleElt) this->stippleElt->lazyEvaluate();

  // a small optimization to avoid unnecessary material
  // testing (it is most common to only have multiple diffuse values)
  if (!this->diffuseOnly) { // diffuseOnly is TRUE when lightModel == BASE_COLOR
    if (this->ambientElt->getNum() <= 1 &&
        this->specularElt->getNum() <= 1 &&
        this->emissiveElt->getNum() <= 1 &&
        this->shininessElt->getNum() <= 1)
      this->diffuseOnly = TRUE;
  }
}

/*!
  Sends material values with index \a index to GL. Will test
  whether the current index equals \a index before sending.

  \a betweenBeginEnd should be \c TRUE if your program is
  between a glBegin() and glEnd() (it is illegal to change the
  polygon stipple between a glBegin() and glEnd()).
*/
void
SoMaterialBundle::send(const int index, const SbBool betweenBeginEnd)
{
  if (this->firstTime) this->setupElements(FALSE);
  if (index != this->currIndex) {
    this->reallySend(index, betweenBeginEnd);
  }
}

/*!
  Will send the material to GL even though \a index equals the current
  index.

  Provided for compatibility with the SGI Open Inventor v2.1 API.
*/
void
SoMaterialBundle::forceSend(const int index)
{
  if (this->firstTime) this->setupElements(FALSE);
  this->reallySend(index, FALSE);
}

/*!
  Returns \c TRUE if the current light model is BASE_COLOR.
*/
SbBool
SoMaterialBundle::isColorOnly(void) const
{
  return this->colorOnly;
}

//
// private method. Will send needed material values to GL.
//
void
SoMaterialBundle::reallySend(const int index, const SbBool isBetweenBeginEnd)
{
  if (this->indexElt) {
    this->indexElt->send(index);
  }
  else {
    if (this->doStipple) {
      // make sure alpha value is 1.0, since blending might be
      // enabled if we are doing point and line smoothing
      this->diffuseElt->send(index, 1.0f);
    }
    else if (!this->diffusePacked) {
      float alpha = 
        this->multiTrans ? 1.0f -  this->transparencyElt->get(index) : 
        this->firstAlpha;
      
      this->diffuseElt->send(index, alpha);
    }
    else { // packed or stipple
      this->diffuseElt->send(index);
    }

    if (!this->diffuseOnly) {
      this->ambientElt->send(index);
      this->emissiveElt->send(index);
      this->specularElt->send(index);
      this->shininessElt->send(index);
    }
  }
  // store current index
  this->currIndex = index;
}
//
// private method. Stores info and element pointers.
//
void
SoMaterialBundle::setupElements(const SbBool /* betweenBeginEnd */)
{
  this->currIndex = -1; // set to an impossible value
  this->firstTime = FALSE;
  if (SoGLColorIndexElement::isColorIndexMode(this->state)) {
    this->indexElt = (SoGLColorIndexElement*) SoGLColorIndexElement::getInstance(this->state);
    // need to set these as they are read in sendFirst()
    this->stippleElt = NULL;
    this->diffuseOnly = TRUE;
    return;
  }
  this->indexElt = NULL;

  this->diffuseOnly = this->colorOnly =
    SoLightModelElement::get(this->state) ==
    SoLightModelElement::BASE_COLOR;

  this->diffuseElt = (SoGLDiffuseColorElement*)
    state->getConstElement(SoGLDiffuseColorElement::getClassStackIndex());
  this->diffusePacked = this->diffuseElt->isPacked();

  if (!this->diffusePacked) {
    this->transparencyElt = (SoTransparencyElement*)
      state->getConstElement(SoTransparencyElement::getClassStackIndex());
    // if there are fewer transparency values than diffuse, use only
    // the first transparency value
    this->multiTrans =
      (this->transparencyElt->getNum() >= this->diffuseElt->getNum());
    if (!this->multiTrans) {
      this->firstAlpha = 1.0f - this->transparencyElt->get(0);
    }
  }

  this->doStipple = SoShapeStyleElement::isScreenDoor(this->state);
  this->stippleElt = (SoGLPolygonStippleElement*)
    state->getConstElement(SoGLPolygonStippleElement::getClassStackIndex());

  if (!this->colorOnly) {
    this->ambientElt = (SoGLAmbientColorElement*)
      state->getConstElement(SoGLAmbientColorElement::getClassStackIndex());
    this->emissiveElt = (SoGLEmissiveColorElement*)
      state->getConstElement(SoGLEmissiveColorElement::getClassStackIndex());
    this->specularElt = (SoGLSpecularColorElement*)
      state->getConstElement(SoGLSpecularColorElement::getClassStackIndex());
    this->shininessElt = (SoGLShininessElement*)
      state->getConstElement(SoGLShininessElement::getClassStackIndex());
  }
}
