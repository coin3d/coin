/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
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

#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/elements/SoFontNameElement.h>
#include <Inventor/elements/SoFontSizeElement.h>
#include <Inventor/elements/SoOverrideElement.h>

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
  SO_NODE_INTERNAL_CONSTRUCTOR(SoFont);

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

  SO_ENABLE(SoGetBoundingBoxAction, SoFontNameElement);
  SO_ENABLE(SoGetBoundingBoxAction, SoFontSizeElement);

  SO_ENABLE(SoGLRenderAction, SoFontNameElement);
  SO_ENABLE(SoGLRenderAction, SoFontSizeElement);

  SO_ENABLE(SoCallbackAction, SoFontNameElement);
  SO_ENABLE(SoCallbackAction, SoFontSizeElement);

  SO_ENABLE(SoPickAction, SoFontNameElement);
  SO_ENABLE(SoPickAction, SoFontSizeElement);

  SO_ENABLE(SoGetPrimitiveCountAction, SoFontNameElement);
  SO_ENABLE(SoGetPrimitiveCountAction, SoFontSizeElement);
}

/*!
  FIXME: write function documentation
*/
void
SoFont::doAction(SoAction * action)
{
  SoState *state = action->getState();
  uint32_t flags = SoOverrideElement::getFlags(state);
#define TEST_OVERRIDE(bit) ((SoOverrideElement::bit & flags) != 0)

  if (!name.isIgnored() && !TEST_OVERRIDE(FONT_NAME)) {
    SoFontNameElement::set(state, this, this->name.getValue());
    if (this->isOverride()) {
      SoOverrideElement::setFontNameOverride(state, this, TRUE);
    }
  }
  if (!size.isIgnored() && !TEST_OVERRIDE(FONT_SIZE)) {
    SoFontSizeElement::set(state, this, this->size.getValue());
    if (this->isOverride()) {
      SoOverrideElement::setFontSizeOverride(state, this, TRUE);
    }
  }
#undef TEST_OVERRIDE
}

/*!
  FIXME: write function documentation
*/
void
SoFont::getBoundingBox(SoGetBoundingBoxAction * action)
{
  SoFont::doAction(action);
}

/*!
  FIXME: write function documentation
*/
void
SoFont::GLRender(SoGLRenderAction * action)
{
  SoFont::doAction(action);
}

/*!
  FIXME: write doc
 */
void
SoFont::callback(SoCallbackAction *action)
{
  SoFont::doAction((SoAction*)action);
}

/*!
  FIXME: write doc
 */
void
SoFont::pick(SoPickAction *action)
{
  SoFont::doAction(action);
}

/*!
  FIXME: write doc
*/
void
SoFont::getPrimitiveCount(SoGetPrimitiveCountAction *action)
{
  SoFont::doAction(action);
}
