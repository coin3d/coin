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
  \class SoTextureScalePolicy SoTextureScalePolicy.h Inventor/nodes/SoTextureScalePolicy.h
  \brief The SoTextureScalePolicy class is a node for controlling the texture scale policy.
  \ingroup nodes

  If a texture map is of size != 2^n, it must be scaled before OpenGL
  can handle it.  This node enables you to control how/if textures are
  scaled before it is sent to OpenGL.

  Also, if a texture map is bigger than the maximum OpenGL texture size
  (implementation and context dependent), it will be scaled down to the
  maximum size. You can avoid this by setting the texture policy to
  FRACTURE, in which case the texture will be split into several small
  subtextures before the geometry using the texture is rendered.

  \since 2003-02-26
*/

#include <Inventor/nodes/SoTextureScalePolicy.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoTextureScaleQualityElement.h>

/*!
  \enum SoTextureScalePolicy::Policy

  Enumerates the available policy settings.
*/

/*!
  \var SoTextureScalePolicy::Policy SoTextureScalePolicy::USE_TEXTURE_QUALITY

  Uses the texture quality to decide whether to scale up or down.
*/

/*!
  \var SoTextureScalePolicy::Policy SoTextureScalePolicy::SCALE_DOWN

  Always scales down.
*/

/*!
  \var SoTextureScalePolicy::Policy SoTextureScalePolicy::SCALE_UP

  Always scales up.
*/

/*!
  \var SoTextureScalePolicy::Policy SoTextureScalePolicy::FRACTURE

  Splits the texture into several subtextures, and clips the geometry
  into each subtexture. This makes it possible to have textures with
  almost unlimted size (the only real limit is the amount on memory on
  the system, since the entire texture most fit into memory).

  Be aware that the rendering is quite slow if you have lots of
  triangles.
*/

/*!
  \var SoSFEnum SoTextureScalePolicy::policy

  The policy setting. Default value is USE_TEXTURE_QUALITY.
*/

/*!
  \var SoSFFloat SoTextureScalePolicy::quality
  
  The texture scale/resize quality. Default value is 0.5.
*/

// *************************************************************************

SO_NODE_SOURCE(SoTextureScalePolicy);

/*!
  Constructor.
*/
SoTextureScalePolicy::SoTextureScalePolicy(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoTextureScalePolicy);
  SO_NODE_ADD_FIELD(policy, (SoTextureScalePolicy::USE_TEXTURE_QUALITY));
  SO_NODE_ADD_FIELD(quality, (0.5f));
  
  SO_NODE_DEFINE_ENUM_VALUE(Policy, USE_TEXTURE_QUALITY);
  SO_NODE_DEFINE_ENUM_VALUE(Policy, SCALE_DOWN);
  SO_NODE_DEFINE_ENUM_VALUE(Policy, SCALE_UP);
  SO_NODE_DEFINE_ENUM_VALUE(Policy, FRACTURE);
  SO_NODE_SET_SF_ENUM_TYPE(policy, Policy);
}

/*!
  Destructor.
*/
SoTextureScalePolicy::~SoTextureScalePolicy()
{
}

// Doc from superclass.
void
SoTextureScalePolicy::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoTextureScalePolicy, SO_FROM_COIN_2_0);
  SO_ENABLE(SoGLRenderAction, SoTextureScalePolicyElement);
  SO_ENABLE(SoGLRenderAction, SoTextureScaleQualityElement);
}

// Doc from superclass.
void
SoTextureScalePolicy::GLRender(SoGLRenderAction * action)
{
  if (!this->policy.isIgnored()) {
    SoTextureScalePolicyElement::set(action->getState(), this, 
                                     (SoTextureScalePolicyElement::Policy) 
                                     this->policy.getValue());
  }
  if (!this->quality.isIgnored()) {
    SoTextureScaleQualityElement::set(action->getState(), this,
                                      this->quality.getValue());
  }
}

