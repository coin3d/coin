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
 *  http://www.sim.no/ sales@sim.no Voice: +47 73540378 Fax: +47 73943861
 *
\**************************************************************************/

/*!
  \class SoFont SoFont.h Inventor/nodes/SoFont.h
  \brief The SoFont class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoFont.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/SbName.h>

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION
#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
#include <Inventor/actions/SoGLRenderAction.h>
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SOFONTNAMEELEMENT)
#include <Inventor/elements/SoFontNameElement.h>
#endif // !COIN_EXCLUDE_SOFONTNAMEELEMENT
#if !defined(COIN_EXCLUDE_SOFONTSIZEELEMENT)
#include <Inventor/elements/SoFontSizeElement.h>
#endif // !COIN_EXCLUDE_SOFONTSIZEELEMENT

/*!
  \var SoSFName SoFont::name
  FIXME: write documentation for field
*/
/*!
  \var SoSFFloat SoFont::size
  FIXME: write documentation for field
*/

// *************************************************************************

//$ BEGIN TEMPLATE NodeSource(SoFont)
SoType SoFont::classTypeId = SoType::badType();

/*!
  Returns a new instance of the SoFont node class.
*/
void *
SoFont::createInstance(void)
{
  return new SoFont;
}

/*!
  Returns the unique type identifier for the SoFont class.
*/
SoType
SoFont::getClassTypeId(void)
{
  return SoFont::classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoFont::getTypeId(void) const
{
  return SoFont::classTypeId;
}
//$ END TEMPLATE NodeSource

/*!
  Constructor.
*/
SoFont::SoFont()
{
//$ BEGIN TEMPLATE NodeConstructor(SoFont)
  // Make sure the class has been initialized.
  assert(SoFont::classTypeId != SoType::badType());
//$ END TEMPLATE NodeConstructor

  SO_NODE_ADD_FIELD(name, ("defaultFont"));
  SO_NODE_ADD_FIELD(size, (10.0f));
}

/*!
  Destructor.
*/
SoFont::~SoFont()
{
}

/*!
  Does initialization common for all objects of the
  SoFont class. This includes setting up the
  type system, among other things.
*/
void
SoFont::initClass(void)
{
//$ BEGIN TEMPLATE InitNodeSource(Font)
  // Make sure we only initialize once.
  assert(SoFont::classTypeId == SoType::badType());
  // Make sure superclass get initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoFont::classTypeId =
    SoType::createType(inherited::getClassTypeId(), "Font",
                       &SoFont::createInstance,
                       SoNode::nextActionMethodIndex++);
//$ END TEMPLATE InitNodeSource

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
  SO_ENABLE(SoGetBoundingBoxAction, SoFontNameElement);
  SO_ENABLE(SoGetBoundingBoxAction, SoFontSizeElement);
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  SO_ENABLE(SoGLRenderAction, SoFontNameElement);
  SO_ENABLE(SoGLRenderAction, SoFontSizeElement);
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoFont::cleanClass(void)
{
}

#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write function documentation
*/
void
SoFont::doAction(SoAction * action)
{
#if !defined(COIN_EXCLUDE_SOFONTNAMEELEMENT)
  SoFontNameElement::set(action->getState(), this, this->name.getValue());
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
SoFont::getBoundingBox(SoGetBoundingBoxAction * action)
{
  this->doAction(action);
}
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write function documentation
*/
void
SoFont::GLRender(SoGLRenderAction * action)
{
  this->doAction(action);
}
#endif // !COIN_EXCLUDE_SOGLRENDERACTION


#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
/*!
  FIXME: write doc
 */
void
SoFont::callback(SoCallbackAction * action)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOCALLBACKACTION

#if !defined(COIN_EXCLUDE_SOPICKACTION)
/*!
  FIXME: write doc
 */
void
SoFont::pick(SoPickAction * action)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOPICKACTION

#if !defined(COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION)
/*!
  FIXME: write doc
 */
void
SoFont::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION
