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
  \class SoAnnoText3Property SoAnnoText3Property.h Inventor/nodes/SoAnnoText3Property.h
  \brief The SoAnnoText3Property class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoAnnoText3Property.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/SbName.h>

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION
#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
#include <Inventor/actions/SoGLRenderAction.h>
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SOANNOTEXT3FONTSIZEHINTELEMENT)
#include <Inventor/elements/SoAnnoText3FontSizeHintElement.h>
#endif // !COIN_EXCLUDE_SOANNOTEXT3FONTSIZEHINTELEMENT
#if !defined(COIN_EXCLUDE_SOANNOTEXT3RENDERPRINTELEMENT)
#include <Inventor/elements/SoAnnoText3RenderPrintElement.h>
#endif // !COIN_EXCLUDE_SOANNOTEXT3RENDERPRINTELEMENT

/*!
  \enum SoAnnoText3Property::RenderPrintType
  FIXME: write documentation for enum
*/
/*!
  \var SoAnnoText3Property::RenderPrintType SoAnnoText3Property::RENDER3D_PRINT_VECTOR
  FIXME: write documentation for enum definition
*/
/*!
  \var SoAnnoText3Property::RenderPrintType SoAnnoText3Property::RENDER3D_PRINT_RASTER
  FIXME: write documentation for enum definition
*/
/*!
  \var SoAnnoText3Property::RenderPrintType SoAnnoText3Property::RENDER2D_PRINT_RASTER
  FIXME: write documentation for enum definition
*/

/*!
  \enum SoAnnoText3Property::FontSizeHint
  FIXME: write documentation for enum
*/
/*!
  \var SoAnnoText3Property::FontSizeHint SoAnnoText3Property::ANNOTATION
  FIXME: write documentation for enum definition
*/
/*!
  \var SoAnnoText3Property::FontSizeHint SoAnnoText3Property::FIT_VECTOR_TEXT
  FIXME: write documentation for enum definition
*/


/*!
  \var SoSFEnum SoAnnoText3Property::renderPrintType
  FIXME: write documentation for field
*/
/*!
  \var SoSFBool SoAnnoText3Property::isCharOrientedRasterPrint
  FIXME: write documentation for field
*/
/*!
  \var SoSFEnum SoAnnoText3Property::fontSizeHint
  FIXME: write documentation for field
*/



// *************************************************************************

//$ BEGIN TEMPLATE NodeSource(SoAnnoText3Property)
SoType SoAnnoText3Property::classTypeId = SoType::badType();

/*!
  Returns a new instance of the SoAnnoText3Property node class.
*/
void *
SoAnnoText3Property::createInstance(void)
{
  return new SoAnnoText3Property;
}

/*!
  Returns the unique type identifier for the SoAnnoText3Property class.
*/
SoType
SoAnnoText3Property::getClassTypeId(void)
{
  return SoAnnoText3Property::classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoAnnoText3Property::getTypeId(void) const
{
  return SoAnnoText3Property::classTypeId;
}
//$ END TEMPLATE NodeSource

/*!
  Constructor.
*/
SoAnnoText3Property::SoAnnoText3Property()
{
//$ BEGIN TEMPLATE NodeConstructor(SoAnnoText3Property)
  // Make sure the class has been initialized.
  assert(SoAnnoText3Property::classTypeId != SoType::badType());
//$ END TEMPLATE NodeConstructor
  
  SO_NODE_ADD_FIELD(fontSizeHint, (FIT_VECTOR_TEXT));
  SO_NODE_ADD_FIELD(renderPrintType, (RENDER3D_PRINT_VECTOR));
  SO_NODE_ADD_FIELD(isCharOrientedRasterPrint, (TRUE));

  SO_NODE_DEFINE_ENUM_VALUE(FontSizeHint, ANNOTATION);
  SO_NODE_DEFINE_ENUM_VALUE(FontSizeHint, FIT_VECTOR_TEXT);
  SO_NODE_SET_SF_ENUM_TYPE(fontSizeHint, FontSizeHint);

  SO_NODE_DEFINE_ENUM_VALUE(RenderPrintType, RENDER3D_PRINT_VECTOR);
  SO_NODE_DEFINE_ENUM_VALUE(RenderPrintType, RENDER3D_PRINT_RASTER);
  SO_NODE_DEFINE_ENUM_VALUE(RenderPrintType, RENDER2D_PRINT_RASTER);
  SO_NODE_SET_SF_ENUM_TYPE(renderPrintType, RenderPrintType);
}

/*!
  Destructor.
*/
SoAnnoText3Property::~SoAnnoText3Property()
{
}

/*!
  Does initialization common for all objects of the
  SoAnnoText3Property class. This includes setting up the
  type system, among other things.
*/
void
SoAnnoText3Property::initClass(void)
{
//$ BEGIN TEMPLATE InitNodeSource(AnnoText3Property)
  // Make sure we only initialize once.
  assert(SoAnnoText3Property::classTypeId == SoType::badType());
  // Make sure superclass get initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoAnnoText3Property::classTypeId =
    SoType::createType(inherited::getClassTypeId(), "AnnoText3Property",
                       &SoAnnoText3Property::createInstance,
                       SoNode::nextActionMethodIndex++);
//$ END TEMPLATE InitNodeSource

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  SO_ENABLE(SoGLRenderAction, SoAnnoText3FontSizeHintElement);
  SO_ENABLE(SoGLRenderAction, SoAnnoText3RenderPrintElement);
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
  SO_ENABLE(SoGetBoundingBoxAction, SoAnnoText3FontSizeHintElement);
  SO_ENABLE(SoGetBoundingBoxAction, SoAnnoText3RenderPrintElement);
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoAnnoText3Property::cleanClass(void)
{
}

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
/*!
  FIXME: write function documentation
*/
void 
SoAnnoText3Property::getBoundingBox(SoGetBoundingBoxAction * action)
{
  SoState * state = action->getState();

  SoAnnoText3FontSizeHintElement::FontSizeHint hint;
  switch (fontSizeHint.getValue()) {
  case ANNOTATION:
    hint = SoAnnoText3FontSizeHintElement::ANNOTATION;
    break;
  case FIT_VECTOR_TEXT:
    hint = SoAnnoText3FontSizeHintElement::FIT_TEXT_VECTOR;
    break;
  default:
    assert(0); // FIXME: do something a bit more elegant here. 19990315 mortene.
    break;
  }

  SoAnnoText3RenderPrintElement::RenderPrintType print;
  switch (renderPrintType.getValue()) {
  case RENDER3D_PRINT_VECTOR:
    print = SoAnnoText3RenderPrintElement::RENDER3D_PRINT_VECTOR;
    break;
  case RENDER3D_PRINT_RASTER:
    print = SoAnnoText3RenderPrintElement::RENDER3D_PRINT_RASTER;
    break;
  case RENDER2D_PRINT_RASTER:
    print = SoAnnoText3RenderPrintElement::RENDER2D_PRINT_RASTER;
    break;
  default:
    assert(0); // FIXME: do something a bit more elegant here. 19990315 mortene.
    break;
  }

  SoAnnoText3FontSizeHintElement::set(state, hint);
  SoAnnoText3RenderPrintElement::set(state, print);
}
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION


#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write doc
*/
void
SoAnnoText3Property::doAction(SoAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOACTION

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write doc
*/
void
SoAnnoText3Property::GLRender(SoGLRenderAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
/*!
  FIXME: write doc
*/
void
SoAnnoText3Property::callback(SoCallbackAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOCALLBACKACTION

#if !defined(COIN_EXCLUDE_SOPICKACTION)
/*!
  FIXME: write doc
*/
void
SoAnnoText3Property::pick(SoPickAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOPICKACTION

#if !defined(COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION)
/*!
  FIXME: write doc
*/
void
SoAnnoText3Property::getPrimitiveCount(SoGetPrimitiveCountAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION
