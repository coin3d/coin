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
  \class SoComplexity SoComplexity.h Inventor/nodes/SoComplexity.h
  \brief The SoComplexity class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoComplexity.h>



#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION
#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
#include <Inventor/actions/SoGLRenderAction.h>
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
#if !defined(COIN_EXCLUDE_SOPICKACTION)
#include <Inventor/actions/SoPickAction.h>
#endif // !COIN_EXCLUDE_SOPICKACTION
#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
#include <Inventor/actions/SoCallbackAction.h>
#endif // !COIN_EXCLUDE_SOCALLBACKACTION

#if !defined(COIN_EXCLUDE_SOCOMPLEXITYELEMENT)
#include <Inventor/elements/SoComplexityElement.h>
#endif // !COIN_EXCLUDE_SOCOMPLEXITYELEMENT
#if !defined(COIN_EXCLUDE_SOCOMPLEXITYTYPEELEMENT)
#include <Inventor/elements/SoComplexityTypeElement.h>
#endif // !COIN_EXCLUDE_SOCOMPLEXITYTYPEELEMENT
#if !defined(COIN_EXCLUDE_SOSHAPESTYLEELEMENT)
#include <Inventor/elements/SoShapeStyleElement.h>
#endif // !COIN_EXCLUDE_SOSHAPESTYLEELEMENT
#if !defined(COIN_EXCLUDE_SOTEXTUREQUALITYELEMENT)
#include <Inventor/elements/SoTextureQualityElement.h>
#endif // !COIN_EXCLUDE_SOTEXTUREQUALITYELEMENT

#if !defined(COIN_EXCLUDE_SOOVERRIDEELEMENT)
#include <Inventor/elements/SoOverrideElement.h>
#endif // !COIN_EXCLUDE_SOOVERRIDEELEMENT

/*!
  \enum SoComplexity::Type
  FIXME: write documentation for enum
*/
/*!
  \var SoComplexity::Type SoComplexity::OBJECT_SPACE
  FIXME: write documentation for enum definition
*/
/*!
  \var SoComplexity::Type SoComplexity::SCREEN_SPACE
  FIXME: write documentation for enum definition
*/
/*!
  \var SoComplexity::Type SoComplexity::BOUNDING_BOX
  FIXME: write documentation for enum definition
*/


/*!
  \var SoSFEnum SoComplexity::type
  FIXME: write documentation for field
*/
/*!
  \var SoSFFloat SoComplexity::value
  FIXME: write documentation for field
*/
/*!
  \var SoSFFloat SoComplexity::textureQuality
  FIXME: write documentation for field
*/


// *************************************************************************

SO_NODE_SOURCE(SoComplexity);

/*!
  Constructor.
*/
SoComplexity::SoComplexity(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoComplexity);
  
  SO_NODE_ADD_FIELD(type, (SoComplexity::OBJECT_SPACE));
  SO_NODE_ADD_FIELD(value, (0.5f));
  SO_NODE_ADD_FIELD(textureQuality, (0.5f));

  SO_NODE_DEFINE_ENUM_VALUE(Type, SCREEN_SPACE);
  SO_NODE_DEFINE_ENUM_VALUE(Type, OBJECT_SPACE);
  SO_NODE_DEFINE_ENUM_VALUE(Type, BOUNDING_BOX);
  SO_NODE_SET_SF_ENUM_TYPE(type, Type);
}

/*!
  Destructor.
*/
SoComplexity::~SoComplexity()
{
}

/*!
  Does initialization common for all objects of the
  SoComplexity class. This includes setting up the
  type system, among other things.
*/
void
SoComplexity::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoComplexity);

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
  SO_ENABLE(SoGetBoundingBoxAction, SoComplexityElement);
  SO_ENABLE(SoGetBoundingBoxAction, SoComplexityTypeElement);
  SO_ENABLE(SoGetBoundingBoxAction, SoShapeStyleElement);
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  SO_ENABLE(SoGLRenderAction, SoComplexityElement);
  SO_ENABLE(SoGLRenderAction, SoComplexityTypeElement);
  SO_ENABLE(SoGLRenderAction, SoShapeStyleElement);
  SO_ENABLE(SoGLRenderAction, SoTextureQualityElement);
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
  SO_ENABLE(SoCallbackAction, SoComplexityElement);
  SO_ENABLE(SoCallbackAction, SoComplexityTypeElement);
  SO_ENABLE(SoCallbackAction, SoShapeStyleElement);
  SO_ENABLE(SoCallbackAction, SoTextureQualityElement);
#endif // !COIN_EXCLUDE_SOCALLBACKACTION

#if !defined(COIN_EXCLUDE_SOPICKACTION)
  SO_ENABLE(SoPickAction, SoComplexityElement);
  SO_ENABLE(SoPickAction, SoComplexityTypeElement);
  SO_ENABLE(SoPickAction, SoShapeStyleElement);
#endif // !COIN_EXCLUDE_SOPICKACTION

}

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
/*!
  FIXME: write function documentation
*/
void 
SoComplexity::getBoundingBox(SoGetBoundingBoxAction * action)
{
  SoComplexity::doAction(action);
}
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write function documentation
*/
void 
SoComplexity::GLRender(SoGLRenderAction * action)
{
  SoComplexity::doAction(action);
  if (!textureQuality.isIgnored()) {
    SoTextureQualityElement::set(action->getState(), this, 
				 textureQuality.getValue());
  }
}
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION


#if !defined(COIN_EXCLUDE_DOACTION)
/*!
  FIXME: write doc
*/
void
SoComplexity::doAction(SoAction *action)
{
  SoState * state = action->getState();
#if !defined(COIN_EXCLUDE_SOCOMPLEXITYELEMENT)
  if (!value.isIgnored() 
#if !defined(COIN_EXCLUDE_SOOVERRIDEELEMENT)
      && !SoOverrideElement::getComplexityOverride(state)
#endif // ! COIN_EXCLUDE_SOOVERRIDEELEMENT
      ) 
    SoComplexityElement::set(state, value.getValue());
#endif // ! COIN_EXCLUDE_SOCOMPLEXITYELEMENT

#if !defined(COIN_EXCLUDE_SOCOMPLEXITYTYPEELEMENT)
  if (!type.isIgnored()
#if !defined(COIN_EXCLUDE_SOOVERRIDEELEMENT)
      && !SoOverrideElement::getComplexityTypeOverride(state)
#endif // ! COIN_EXCLUDE_SOOVERRIDEELEMENT
      )
    SoComplexityTypeElement::set(state, (SoComplexityTypeElement::Type) 
				 type.getValue());
#endif // ! COIN_EXCLUDE_SOCOMPLEXITYTYPEELEMENT 
}
#endif // !COIN_EXCLUDE_DOACTION

#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
/*!
  FIXME: write doc
*/
void
SoComplexity::callback(SoCallbackAction *action)
{
  SoComplexity::doAction((SoAction*)action);
  if (!textureQuality.isIgnored()) {
    SoTextureQualityElement::set(action->getState(), this, 
				 textureQuality.getValue());
  }
}
#endif // !COIN_EXCLUDE_SOCALLBACKACTION

#if !defined(COIN_EXCLUDE_SOPICKACTION)
/*!
  FIXME: write doc
*/
void
SoComplexity::pick(SoPickAction *action)
{
  SoComplexity::doAction(action);
}
#endif // !COIN_EXCLUDE_SOPICKACTION

#if !defined(COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION)
/*!
  FIXME: write doc
*/
void
SoComplexity::getPrimitiveCount(SoGetPrimitiveCountAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION

