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
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/SbName.h>

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
#include <Inventor/actions/SoGLRenderAction.h>
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SOGLLIGHTMODELELEMENT)
#include <Inventor/elements/SoGLLightModelElement.h>
#endif // !COIN_EXCLUDE_SOLIGHTMODELELEMENT

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

//$ BEGIN TEMPLATE NodeSource(SoLightModel)
SoType SoLightModel::classTypeId = SoType::badType();

/*!
  Returns a new instance of the SoLightModel node class.
*/
void *
SoLightModel::createInstance(void)
{
  return new SoLightModel;
}

/*!
  Returns the unique type identifier for the SoLightModel class.
*/
SoType
SoLightModel::getClassTypeId(void)
{
  return SoLightModel::classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoLightModel::getTypeId(void) const
{
  return SoLightModel::classTypeId;
}
//$ END TEMPLATE NodeSource

/*!
  Constructor.
*/
SoLightModel::SoLightModel()
{
//$ BEGIN TEMPLATE NodeConstructor(SoLightModel)
  // Make sure the class has been initialized.
  assert(SoLightModel::classTypeId != SoType::badType());
//$ END TEMPLATE NodeConstructor
  
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
//$ BEGIN TEMPLATE InitNodeSource(LightModel)
  // Make sure we only initialize once.
  assert(SoLightModel::classTypeId == SoType::badType());
  // Make sure superclass get initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoLightModel::classTypeId =
    SoType::createType(inherited::getClassTypeId(), "LightModel",
                       &SoLightModel::createInstance,
                       SoNode::nextActionMethodIndex++);
//$ END TEMPLATE InitNodeSource

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  SO_ENABLE(SoGLRenderAction, SoGLLightModelElement);
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoLightModel::cleanClass(void)
{
}

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write function documentation
*/
void 
SoLightModel::GLRender(SoGLRenderAction * action)
{
  SoLightModelElement::set(action->getState(), this,
			   (SoLightModelElement::Model)model.getValue());
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
