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
  \class SoTextureScalePolicy SoTextureScalePolicy.h Inventor/nodes/SoTextureScalePolicy.h
  \brief The SoTextureScalePolicy class is a node for texture scale policy.
  \ingroup nodes

  This is currently an internal Coin node. The header file is not
  installed, and the API for this node might change without notice.
*/

#include <Inventor/nodes/SoTextureScalePolicy.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/actions/SoGLRenderAction.h>

/*!
  \enum SoTextureScalePolicy::Units

  Enumerates the available unit settings.
*/


/*!
  \var SoSFEnum SoTextureScalePolicy::policy
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
  
  SO_NODE_DEFINE_ENUM_VALUE(Policy, USE_TEXTURE_QUALITY);
  SO_NODE_DEFINE_ENUM_VALUE(Policy, SCALE_DOWN);
  SO_NODE_DEFINE_ENUM_VALUE(Policy, SCALE_UP);
  SO_NODE_DEFINE_ENUM_VALUE(Policy, DONT_SCALE);
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
  SO_NODE_INTERNAL_INIT_CLASS(SoTextureScalePolicy);
  SO_ENABLE(SoGLRenderAction, SoTextureScalePolicyElement);
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
}

