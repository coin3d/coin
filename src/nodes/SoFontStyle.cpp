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
  \class SoFontStyle SoFontStyle.h Inventor/nodes/SoFontStyle.h
  \brief The SoFontStyle class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoFontStyle.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/SbName.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

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

#if !defined(COIN_EXCLUDE_SOFONTNAMEELEMENT)
#include <Inventor/elements/SoFontNameElement.h>
#endif // !COIN_EXCLUDE_SOFONTNAMEELEMENT
#if !defined(COIN_EXCLUDE_SOFONTSIZEELEMENT)
#include <Inventor/elements/SoFontSizeElement.h>
#endif // !COIN_EXCLUDE_SOFONTSIZEELEMENT

/*!
  \enum SoFontStyle::Family
  FIXME: write documentation for enum
*/
/*!
  \var SoFontStyle::Family SoFontStyle::SERIF
  FIXME: write documentation for enum definition
*/
/*!
  \var SoFontStyle::Family SoFontStyle::SANS
  FIXME: write documentation for enum definition
*/
/*!
  \var SoFontStyle::Family SoFontStyle::TYPEWRITER
  FIXME: write documentation for enum definition
*/

/*!
  \enum SoFontStyle::Style
  FIXME: write documentation for enum
*/
/*!
  \var SoFontStyle::Style SoFontStyle::NONE
  FIXME: write documentation for enum definition
*/
/*!
  \var SoFontStyle::Style SoFontStyle::BOLD
  FIXME: write documentation for enum definition
*/
/*!
  \var SoFontStyle::Style SoFontStyle::ITALIC
  FIXME: write documentation for enum definition
*/


/*!
  \var SoSFEnum SoFontStyle::family
  FIXME: write documentation for field
*/
/*!
  \var SoSFBitMask SoFontStyle::style
  FIXME: write documentation for field
*/


// *************************************************************************

//$ BEGIN TEMPLATE NodeSource(SoFontStyle)
SoType SoFontStyle::classTypeId = SoType::badType();

/*!
  Returns a new instance of the SoFontStyle node class.
*/
void *
SoFontStyle::createInstance(void)
{
  return new SoFontStyle;
}

/*!
  Returns the unique type identifier for the SoFontStyle class.
*/
SoType
SoFontStyle::getClassTypeId(void)
{
  return SoFontStyle::classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoFontStyle::getTypeId(void) const
{
  return SoFontStyle::classTypeId;
}
//$ END TEMPLATE NodeSource

/*!
  Constructor.
*/
SoFontStyle::SoFontStyle()
{
//$ BEGIN TEMPLATE NodeConstructor(SoFontStyle)
  // Make sure the class has been initialized.
  assert(SoFontStyle::classTypeId != SoType::badType());
//$ END TEMPLATE NodeConstructor

  SO_NODE_ADD_FIELD(family, (SoFontStyle::SERIF));
  SO_NODE_ADD_FIELD(style, (SoFontStyle::NONE));

  SO_NODE_DEFINE_ENUM_VALUE(Family, SERIF);
  SO_NODE_DEFINE_ENUM_VALUE(Family, SANS);
  SO_NODE_DEFINE_ENUM_VALUE(Family, TYPEWRITER);
  SO_NODE_SET_SF_ENUM_TYPE(family, Family);

  SO_NODE_DEFINE_ENUM_VALUE(Style, NONE);
  SO_NODE_DEFINE_ENUM_VALUE(Style, BOLD);
  SO_NODE_DEFINE_ENUM_VALUE(Style, ITALIC);
  SO_NODE_SET_SF_ENUM_TYPE(style, Style);
}

/*!
  Destructor.
*/
SoFontStyle::~SoFontStyle()
{
}

/*!
  Does initialization common for all objects of the
  SoFontStyle class. This includes setting up the
  type system, among other things.
*/
void
SoFontStyle::initClass(void)
{
//$ BEGIN TEMPLATE InitNodeSource(FontStyle)
  // Make sure we only initialize once.
  assert(SoFontStyle::classTypeId == SoType::badType());
  // Make sure superclass get initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoFontStyle::classTypeId =
    SoType::createType(inherited::getClassTypeId(), "FontStyle",
                       &SoFontStyle::createInstance,
                       SoNode::nextActionMethodIndex++);
//$ END TEMPLATE InitNodeSource
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoFontStyle::cleanClass(void)
{
}

/*!
  FIXME: write function documentation
*/
SbString
SoFontStyle::getFontName(void) const
{
  // FIXME: this is valid only under X11! 19990418 mortene.

  SbString fontname(this->name.getValue().getString());
  if (fontname == "defaultFont") {
    switch (this->family.getValue()) {
    case SoFontStyle::SERIF:
      fontname = "times";
      break;
    case SoFontStyle::SANS:
      fontname = "helvetica";
      break;
    case SoFontStyle::TYPEWRITER:
      fontname = "courier";
      break;
#if COIN_DEBUG
    default:
      SoDebugError::postWarning("SoFontStyle::getFontName",
				"value of family field is invalid, "
				"setting to SERIF");
      fontname = "times";
      break;
#endif // COIN_DEUG
    }
  }

  // If this doesn't hold up, we need to include a few more cases in
  // the switch block.
  assert(SoFontStyle::NONE == 0);

  switch (this->style.getValue()) {
  case SoFontStyle::NONE:
    fontname += "-*-r-";
    break;
  case SoFontStyle::BOLD:
    fontname += "-bold-r-";
    break;
  case SoFontStyle::ITALIC:
    fontname += "-*-i-";
    break;
  case (SoFontStyle::BOLD | SoFontStyle::ITALIC):
    fontname += "-bold-i-";
    break;
#if COIN_DEBUG
  default:
    SoDebugError::postWarning("SoFontStyle::getFontName",
			      "value of style field is invalid");
    break;
#endif // COIN_DEUG
  }

  return fontname;
}

#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write function documentation
*/
void
SoFontStyle::doAction(SoAction * action)
{
#if !defined(COIN_EXCLUDE_SOFONTNAMEELEMENT)
  SoFontNameElement::set(action->getState(), this, this->getFontName());
#endif // !COIN_EXCLUDE_SOFONTNAMEELEMENT
#if !defined(COIN_EXCLUDE_SOFONTSIZEELEMENT)
  SoFontSizeElement::set(action->getState(), this, this->size.getValue());
#endif // !COIN_EXCLUDE_SOFONTSIZEELEMENT
}
#endif // !COIN_EXCLUDE_SOACTION

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
/*!
  FIXME: write function documentation
*/
void
SoFontStyle::getBoundingBox(SoGetBoundingBoxAction * action)
{
  this->doAction(action);
}
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write function documentation
*/
void
SoFontStyle::GLRender(SoGLRenderAction * action)
{
  this->doAction(action);
}
#endif // !COIN_EXCLUDE_SOGLRENDERACTION


#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
/*!
  FIXME: write doc
 */
void
SoFontStyle::callback(SoCallbackAction *action)
{
  SoFontStyle::doAction((SoAction*)action);
}
#endif // !COIN_EXCLUDE_SOCALLBACKACTION

#if !defined(COIN_EXCLUDE_SOPICKACTION)
/*!
  FIXME: write doc
 */
void
SoFontStyle::pick(SoPickAction *action)
{
  SoFontStyle::doAction((SoAction*)action);
}
#endif // !COIN_EXCLUDE_SOPICKACTION

#if !defined(COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION)
/*!
  FIXME: write doc
 */
void
SoFontStyle::getPrimitiveCount(SoGetPrimitiveCountAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION
