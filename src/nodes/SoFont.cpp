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
  \class SoFont SoFont.h Inventor/nodes/SoFont.h
  \brief The SoFont class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoFont.h>



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
#if !defined(COIN_EXCLUDE_SOOVERRIDEELEMENT)
#include <Inventor/elements/SoOverrideElement.h>
#endif // !COIN_EXCLUDE_SOOVERRIDEELEMENT

/*!
  \var SoSFName SoFont::name
  FIXME: write documentation for field
*/
/*!
  \var SoSFFloat SoFont::size
  FIXME: write documentation for field
*/

// *************************************************************************

SO_NODE_SOURCE(SoFont);

/*!
  Constructor.
*/
SoFont::SoFont()
{
  SO_NODE_CONSTRUCTOR(SoFont);

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
  SO_NODE_INTERNAL_INIT_CLASS(SoFont);

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
  SO_ENABLE(SoGetBoundingBoxAction, SoFontNameElement);
  SO_ENABLE(SoGetBoundingBoxAction, SoFontSizeElement);
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  SO_ENABLE(SoGLRenderAction, SoFontNameElement);
  SO_ENABLE(SoGLRenderAction, SoFontSizeElement);
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
  SO_ENABLE(SoCallbackAction, SoFontNameElement);
  SO_ENABLE(SoCallbackAction, SoFontSizeElement);
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SOPICKACTION)
  SO_ENABLE(SoPickAction, SoFontNameElement);
  SO_ENABLE(SoPickAction, SoFontSizeElement);
#endif // !COIN_EXCLUDE_SOPICKACTION
}

#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write function documentation
*/
void
SoFont::doAction(SoAction * action)
{
  SoState *state = action->getState();
#if !defined(COIN_EXCLUDE_SOOVERRIDEELEMENT)
  uint32_t flags = SoOverrideElement::getFlags(state);
#define TEST_OVERRIDE(bit) ((SoOverrideElement::bit & flags) != 0)
#else // COIN_EXCLUDE_SOOVERRIDEELEMENT
#define TEST_OVERRIDE(x,y) FALSE
#endif // COIN_EXCLUDE_SOOVERRIDEELEMENT

#if !defined(COIN_EXCLUDE_SOFONTNAMEELEMENT)
  if (!name.isIgnored() && !TEST_OVERRIDE(FONT_NAME))
    SoFontNameElement::set(state, this, this->name.getValue());
#endif // !COIN_EXCLUDE_SOFONTNAMEELEMENT
#if !defined(COIN_EXCLUDE_SOFONTSIZEELEMENT)
  if (!size.isIgnored() && !TEST_OVERRIDE(FONT_SIZE))
    SoFontSizeElement::set(state, this, this->size.getValue());
#endif // !COIN_EXCLUDE_SOFONTSIZEELEMENT
#undef TEST_OVERRIDE
}
#endif // !COIN_EXCLUDE_SOACTION

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
/*!
  FIXME: write function documentation
*/
void
SoFont::getBoundingBox(SoGetBoundingBoxAction * action)
{
  SoFont::doAction(action);
}
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write function documentation
*/
void
SoFont::GLRender(SoGLRenderAction * action)
{
  SoFont::doAction(action);
}
#endif // !COIN_EXCLUDE_SOGLRENDERACTION


#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
/*!
  FIXME: write doc
 */
void
SoFont::callback(SoCallbackAction *action)
{
  SoFont::doAction((SoAction*)action);
}
#endif // !COIN_EXCLUDE_SOCALLBACKACTION

#if !defined(COIN_EXCLUDE_SOPICKACTION)
/*!
  FIXME: write doc
 */
void
SoFont::pick(SoPickAction *action)
{
  SoFont::doAction(action);
}
#endif // !COIN_EXCLUDE_SOPICKACTION

#if !defined(COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION)
/*!
  FIXME: write doc
 */
void
SoFont::getPrimitiveCount(SoGetPrimitiveCountAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION
