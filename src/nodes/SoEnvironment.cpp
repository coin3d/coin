/**************************************************************************\
 *
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

/*!
  \class SoEnvironment SoEnvironment.h Inventor/nodes/SoEnvironment.h
  \brief The SoEnvironment class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoEnvironment.h>

#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLEnvironmentElement.h>
#include <Inventor/elements/SoEnvironmentElement.h>
#include <Inventor/actions/SoCallbackAction.h>

/*!
  \enum SoEnvironment::FogType
  FIXME: write documentation for enum
*/
/*!
  \var SoEnvironment::FogType SoEnvironment::NONE
  FIXME: write documentation for enum definition
*/
/*!
  \var SoEnvironment::FogType SoEnvironment::HAZE
  FIXME: write documentation for enum definition
*/
/*!
  \var SoEnvironment::FogType SoEnvironment::FOG
  FIXME: write documentation for enum definition
*/
/*!
  \var SoEnvironment::FogType SoEnvironment::SMOKE
  FIXME: write documentation for enum definition
*/


/*!
  \var SoSFFloat SoEnvironment::ambientIntensity
  FIXME: write documentation for field
*/
/*!
  \var SoSFColor SoEnvironment::ambientColor
  FIXME: write documentation for field
*/
/*!
  \var SoSFVec3f SoEnvironment::attenuation
  FIXME: write documentation for field
*/
/*!
  \var SoSFEnum SoEnvironment::fogType
  FIXME: write documentation for field
*/
/*!
  \var SoSFColor SoEnvironment::fogColor
  FIXME: write documentation for field
*/
/*!
  \var SoSFFloat SoEnvironment::fogVisibility
  FIXME: write documentation for field
*/


// *************************************************************************

SO_NODE_SOURCE(SoEnvironment);

/*!
  Constructor.
*/
SoEnvironment::SoEnvironment()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoEnvironment);

  SO_NODE_ADD_FIELD(ambientIntensity, (0.2f));
  SO_NODE_ADD_FIELD(ambientColor, (1.0f, 1.0f, 1.0f));
  SO_NODE_ADD_FIELD(attenuation, (0.0f, 0.0f, 1.0f));
  SO_NODE_ADD_FIELD(fogType, (SoEnvironment::NONE));
  SO_NODE_ADD_FIELD(fogColor, (1.0f, 1.0f, 1.0f));
  SO_NODE_ADD_FIELD(fogVisibility, (0.0f));

  SO_NODE_DEFINE_ENUM_VALUE(FogType, NONE);
  SO_NODE_DEFINE_ENUM_VALUE(FogType, HAZE);
  SO_NODE_DEFINE_ENUM_VALUE(FogType, FOG);
  SO_NODE_DEFINE_ENUM_VALUE(FogType, SMOKE);
  SO_NODE_SET_SF_ENUM_TYPE(fogType, FogType);
}

/*!
  Destructor.
*/
SoEnvironment::~SoEnvironment()
{
}

/*!
  Does initialization common for all objects of the
  SoEnvironment class. This includes setting up the
  type system, among other things.
*/
void
SoEnvironment::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoEnvironment);

  SO_ENABLE(SoGLRenderAction, SoGLEnvironmentElement);

  SO_ENABLE(SoCallbackAction, SoEnvironmentElement);
}

/*!
  FIXME: write doc
*/
void
SoEnvironment::GLRender(SoGLRenderAction * action)
{
  SoEnvironmentElement::set(action->getState(),
                            this,
                            ambientIntensity.getValue(),
                            ambientColor.getValue(),
                            attenuation.getValue(),
                            (int32_t)fogType.getValue(),
                            fogColor.getValue(),
                            fogVisibility.getValue());
}

/*!
  FIXME: write doc
*/
void
SoEnvironment::callback(SoCallbackAction *action)
{
  SoEnvironmentElement::set(action->getState(),
                            this,
                            ambientIntensity.getValue(),
                            ambientColor.getValue(),
                            attenuation.getValue(),
                            (int32_t)fogType.getValue(),
                            fogColor.getValue(),
                            fogVisibility.getValue());
}

