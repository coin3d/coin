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
  \class SoLight SoLight.h Inventor/nodes/SoLight.h
  \brief The SoLight class is the base class for light emitting nodes.
  \ingroup nodes

  This node type is abstract and does not in itself provide any light
  sources to the scene, you need to use one of its subclasses.

  There are a few important things to know about light sources in
  Coin. First of all, the more light sources you have in your scene,
  the slower the rendering will be. The impact on rendering speed is
  highly dependent on the graphics hardware and/or rendering subsystem
  software implementation (i.e. how optimized an OpenGL you or your
  users are running), but it could be severe.

  Another issue which is important to know is that OpenGL rendering
  engines usually have a fixed maximum number of available light
  sources which can be present in the state at the same time. If you
  reach the maximum number, further light sources will simply be
  ignored. The maximum number of light sources for OpenGL rendering
  can be found by using:

  \code
      #include <Inventor/elements/SoGLLightIdElement.h>
#include <Inventor/nodes/SoSubNodeP.h>
      // ...[snip]...
      int nrlights = SoGLLightIdElement::getMaxGLSources();
  \endcode

  If you are clever with how you use light sources, you can get away
  with using a lot more lights in a scene graph than the max available
  from the rendering system. This is because light sources are stacked
  on the traversal state, just like other appearance data. So if you
  put light sources under SoSeparator nodes, they will be popped off
  and "forgotten" for the remaining geometry of the scene graph after
  the subgraph below an SoSeparator has been traversed.
*/

#include <Inventor/nodes/SoLight.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLLightIdElement.h>
#include <Inventor/elements/SoLightAttenuationElement.h>
#include <Inventor/elements/SoLightElement.h>
#include <Inventor/elements/SoModelMatrixElement.h>

/*!
  \var SoSFBool SoLight::on

  Whether light source should be on or off. The on-flag defaults to \c
  TRUE.
*/
/*!
  \var SoSFFloat SoLight::intensity

  Intensity of light source. This decides how much the light source
  should affect the colors etc of the scene geometry. Valid range is
  0.0 (none) to 1.0 (maximum). Default value is 1.0.
*/
/*!
  \var SoSFColor SoLight::color

  Color of light source. Default is an all-white light source.
*/


SO_NODE_ABSTRACT_SOURCE(SoLight);

/*!
  Constructor.
*/
SoLight::SoLight(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoLight);

  SO_NODE_ADD_FIELD(on, (TRUE));
  SO_NODE_ADD_FIELD(intensity, (1.0f));
  SO_NODE_ADD_FIELD(color, (SbColor(1.0f, 1.0f, 1.0f)));
}

/*!
  Destructor.
*/
SoLight::~SoLight()
{
}

// Doc from superclass.
void
SoLight::initClass(void)
{
  SO_NODE_INTERNAL_INIT_ABSTRACT_CLASS(SoLight, SO_FROM_INVENTOR_1);

  SO_ENABLE(SoGLRenderAction, SoLightAttenuationElement);
  SO_ENABLE(SoGLRenderAction, SoGLLightIdElement);

  SO_ENABLE(SoCallbackAction, SoLightAttenuationElement);
  SO_ENABLE(SoCallbackAction, SoLightElement);
}

// Doc from superclass.
void
SoLight::callback(SoCallbackAction * action)
{
  SoState * state = action->getState();
  SoLightElement::add(state, this, SoModelMatrixElement::get(state));
}
