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
  \class SoDrawStyle SoDrawStyle.h Inventor/nodes/SoDrawStyle.h
  \brief The SoDrawStyle class ...
  \ingroup nodes

  FIXME: write class doc
*/


#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/elements/SoGLDrawStyleElement.h>
#include <Inventor/elements/SoShapeStyleElement.h>
#include <Inventor/elements/SoGLLinePatternElement.h>
#include <Inventor/elements/SoGLLineWidthElement.h>
#include <Inventor/elements/SoGLPointSizeElement.h>
#include <Inventor/elements/SoGLPolygonStippleElement.h>
#include <Inventor/elements/SoOverrideElement.h>

/*!
  \enum SoDrawStyle::Style
  FIXME: write documentation for enum
*/
/*!
  \var SoDrawStyle::Style SoDrawStyle::FILLED
  FIXME: write documentation for enum definition
*/
/*!
  \var SoDrawStyle::Style SoDrawStyle::LINES
  FIXME: write documentation for enum definition
*/
/*!
  \var SoDrawStyle::Style SoDrawStyle::POINTS
  FIXME: write documentation for enum definition
*/
/*!
  \var SoDrawStyle::Style SoDrawStyle::INVISIBLE
  FIXME: write documentation for enum definition
*/


/*!
  \var SoSFEnum SoDrawStyle::style
  FIXME: write documentation for field
*/
/*!
  \var SoSFFloat SoDrawStyle::pointSize
  FIXME: write documentation for field
*/
/*!
  \var SoSFFloat SoDrawStyle::lineWidth
  FIXME: write documentation for field
*/
/*!
  \var SoSFUShort SoDrawStyle::linePattern
  FIXME: write documentation for field
*/


// *************************************************************************

SO_NODE_SOURCE(SoDrawStyle);

/*!
  Constructor.
*/
SoDrawStyle::SoDrawStyle()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoDrawStyle);

  SO_NODE_ADD_FIELD(style, (SoDrawStyle::FILLED));
  SO_NODE_ADD_FIELD(pointSize, (0));
  SO_NODE_ADD_FIELD(lineWidth, (0));
  SO_NODE_ADD_FIELD(linePattern, (0xffff));

  SO_NODE_DEFINE_ENUM_VALUE(Style, FILLED);
  SO_NODE_DEFINE_ENUM_VALUE(Style, LINES);
  SO_NODE_DEFINE_ENUM_VALUE(Style, POINTS);
  SO_NODE_DEFINE_ENUM_VALUE(Style, INVISIBLE);
  SO_NODE_SET_SF_ENUM_TYPE(style, Style);
}

/*!
  Destructor.
*/
SoDrawStyle::~SoDrawStyle()
{
}

/*!
  Does initialization common for all objects of the
  SoDrawStyle class. This includes setting up the
  type system, among other things.
*/
void
SoDrawStyle::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoDrawStyle);

  SO_ENABLE(SoGLRenderAction, SoGLDrawStyleElement);
  SO_ENABLE(SoGLRenderAction, SoShapeStyleElement);
  SO_ENABLE(SoGLRenderAction, SoGLLinePatternElement);
  SO_ENABLE(SoGLRenderAction, SoGLLineWidthElement);
  SO_ENABLE(SoGLRenderAction, SoGLPointSizeElement);
  SO_ENABLE(SoGLRenderAction, SoGLPolygonStippleElement);

  SO_ENABLE(SoCallbackAction, SoDrawStyleElement);
  SO_ENABLE(SoCallbackAction, SoShapeStyleElement);
  SO_ENABLE(SoCallbackAction, SoLinePatternElement);
  SO_ENABLE(SoCallbackAction, SoLineWidthElement);
  SO_ENABLE(SoCallbackAction, SoPointSizeElement);
}

/*!
  FIXME: write function documentation
*/
void
SoDrawStyle::doAction(SoAction *action)
{
  SoState * state = action->getState();

  uint32_t flags = SoOverrideElement::getFlags(state);
#define TEST_OVERRIDE(bit) ((SoOverrideElement::bit & flags) != 0)

  if (!style.isIgnored() && !TEST_OVERRIDE(DRAW_STYLE)) {
    SoDrawStyleElement::set(state, this,
                            (SoDrawStyleElement::Style)style.getValue());
    if (this->isOverride()) {
      SoOverrideElement::setDrawStyleOverride(state, this, TRUE);
    }
  }
  if (!linePattern.isIgnored() && !TEST_OVERRIDE(LINE_PATTERN)) {
    SoLinePatternElement::set(state, this, linePattern.getValue());
    if (this->isOverride()) {
      SoOverrideElement::setLinePatternOverride(state, this, TRUE);
    }
  }
  if (!lineWidth.isIgnored() && !TEST_OVERRIDE(LINE_WIDTH)) {
    SoLineWidthElement::set(state, this, lineWidth.getValue());
    if (this->isOverride()) {
      SoOverrideElement::setLineWidthOverride(state, this, TRUE);
    }
  }
  if (!pointSize.isIgnored() && !TEST_OVERRIDE(POINT_SIZE)) {
    SoPointSizeElement::set(state, this, pointSize.getValue());
    if (this->isOverride()) {
      SoOverrideElement::setPointSizeOverride(state, this, TRUE);
    }
  }
#undef TEST_OVERRIDE

}

/*!
  FIXME: write function documentation
*/
void
SoDrawStyle::GLRender(SoGLRenderAction * action)
{
  SoDrawStyle::doAction(action);
}

/*!
  FIXME: write doc
*/
void
SoDrawStyle::callback(SoCallbackAction *action)
{
  SoDrawStyle::doAction(action);
}

