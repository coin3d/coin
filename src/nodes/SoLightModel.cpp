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
  \class SoLightModel SoLightModel.h Inventor/nodes/SoLightModel.h
  \brief The SoLightModel class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoLightModel.h>



#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
#include <Inventor/actions/SoGLRenderAction.h>
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SOGLLIGHTMODELELEMENT)
#include <Inventor/elements/SoGLLightModelElement.h>
#endif // !COIN_EXCLUDE_SOLIGHTMODELELEMENT
#if !defined(COIN_EXCLUDE_SOOVERRIDEELEMENT)
#include <Inventor/elements/SoOverrideElement.h>
#endif // !COIN_EXCLUDE_SOOVERRIDEELEMENT

/*!
  \enum SoLightModel::Model
  FIXME: write documentation for enum
*/
/*!
  \var SoLightModel::Model SoLightModel::BASE_COLOR
  FIXME: write documentation for enum definition
*/
/*!
  \var SoLightModel::Model SoLightModel::PHONG
  FIXME: write documentation for enum definition
*/


/*!
  \var SoSFEnum SoLightModel::model
  FIXME: write documentation for field
*/

// *************************************************************************

SO_NODE_SOURCE(SoLightModel);

/*!
  Constructor.
*/
SoLightModel::SoLightModel()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoLightModel);
  
  SO_NODE_ADD_FIELD(model, (SoLightModel::PHONG));
  SO_NODE_DEFINE_ENUM_VALUE(Model, BASE_COLOR);
  SO_NODE_DEFINE_ENUM_VALUE(Model, PHONG);
  SO_NODE_SET_SF_ENUM_TYPE(model, Model);
}

/*!
  Destructor.
*/
SoLightModel::~SoLightModel()
{
}

/*!
  Does initialization common for all objects of the
  SoLightModel class. This includes setting up the
  type system, among other things.
*/
void
SoLightModel::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoLightModel);

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  SO_ENABLE(SoGLRenderAction, SoGLLightModelElement);
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
}

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write function documentation
*/
void 
SoLightModel::GLRender(SoGLRenderAction * action)
{
  if (!model.isIgnored()
#if !defined(COIN_EXCLUDE_SOOVERRIDEELEMENT)
      && !SoOverrideElement::getLightModelOverride(action->getState())
#endif // !COIN_EXCLUDE_SOOVERRIDEELEMENT
      ) {
    SoLightModelElement::set(action->getState(), this,
			     (SoLightModelElement::Model)model.getValue());
  }
}
#endif // !COIN_EXCLUDE_SOGLRENDERACTION


#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write doc
 */
void
SoLightModel::doAction(SoAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOACTION

#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
/*!
  FIXME: write doc
 */
void
SoLightModel::callback(SoCallbackAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOCALLBACKACTION
