/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
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
#include <Inventor/elements/SoGLLazyElement.h>
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
  this->firsttime = TRUE; // other members will be set in setUpElements
  this->coloronly = 
    SoLazyElement::getLightModel(this->state) == SoLazyElement::BASE_COLOR;
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
  this->setupElements(FALSE);
}

/*!
  Sends the initial material values to GL. Must be done once in all
  geometry nodes before the rendering begins.
*/
void
SoMaterialBundle::sendFirst(void)
{
  this->setupElements(FALSE);
}

/*!
  Sends material values with index \a index to GL. Will test
  whether the current index equals \a index before sending.

  \a betweenBeginEnd should be \c TRUE if your program is
  between a glBegin() and glEnd() (it is illegal to change the
  polygon stipple between a glBegin() and glEnd()).
*/
void
SoMaterialBundle::send(const int index, const SbBool betweenbeginend)
{
  if (this->firsttime) this->setupElements(betweenbeginend);
  if (index != this->currindex) {
    this->reallySend(index);
    this->currindex = index;
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
  if (this->firsttime) this->setupElements(FALSE);
  this->reallySend(index);
  this->currindex = index;
}

/*!
  Returns \c TRUE if the current light model is BASE_COLOR.
*/
SbBool
SoMaterialBundle::isColorOnly(void) const
{
  return this->coloronly;
}

//
// private method. Will send needed material values to GL.
//
void
SoMaterialBundle::reallySend(const int index)
{
  this->lazyelem->sendDiffuseByIndex(index);
}
//
// private method. Stores info and element pointers.
//
void
SoMaterialBundle::setupElements(const SbBool isbetweenbeginend)
{
  this->lazyelem = (const SoGLLazyElement*) SoLazyElement::getInstance(this->state);
  this->currindex = 0;
  
  if (isbetweenbeginend || this->coloronly) {
    this->lazyelem->send(this->state, SoLazyElement::DIFFUSE_ONLY_MASK); 
  }
  else {
    this->lazyelem->send(this->state, SoLazyElement::ALL_MASK); 
  }
  this->firsttime = FALSE;
}
