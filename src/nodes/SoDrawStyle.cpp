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


#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLDrawStyleElement.h>
#include <Inventor/elements/SoGLLinePatternElement.h>
#include <Inventor/elements/SoGLLineWidthElement.h>
#include <Inventor/elements/SoGLPointSizeElement.h>
#include <Inventor/elements/SoGLPolygonStippleElement.h>
#include <Inventor/elements/SoOverrideElement.h>
#include <Inventor/elements/SoShapeStyleElement.h>

/*!
  \enum SoDrawStyle::Style
  Enumeration for the various ways to render geometry.
*/
/*!
  \var SoDrawStyle::Style SoDrawStyle::FILLED
  Render all geometry as-is.
*/
/*!
  \var SoDrawStyle::Style SoDrawStyle::LINES
  Render all geometry as border lines.
*/
/*!
  \var SoDrawStyle::Style SoDrawStyle::POINTS
  Render all geometry as vertex points.
*/
/*!
  \var SoDrawStyle::Style SoDrawStyle::INVISIBLE
  Don't render geometry.
*/


/*!
  \var SoSFEnum SoDrawStyle::style

  How to render the geometry following a drawstyle node in the scene
  graph. Default SoDrawStyle::FILLED.
*/
/*!
  \var SoSFFloat SoDrawStyle::pointSize

  Size in screen pixels of SoPointSet points, and also of geometry
  vertex points if setting the SoDrawStyle::style to
  SoDrawStyle::POINTS.

  Default value is 0.
*/
/*!
  \var SoSFFloat SoDrawStyle::lineWidth

  Width in screen pixels of SoLineSet and SoIndexedLineSet lines, and
  also of geometry border lines if setting the SoDrawStyle::style to
  SoDrawStyle::LINES.

  Default value is 0.
*/
/*!
  \var SoSFUShort SoDrawStyle::linePattern

  Pattern as a bitmask used when drawing lines. Default is 0xffff (no
  "holes").
*/


// *************************************************************************

SO_NODE_SOURCE(SoDrawStyle);

/*!
  Constructor.
*/
SoDrawStyle::SoDrawStyle(void)
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

// Doc from superclass.
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

// Doc from superclass.
void
SoDrawStyle::doAction(SoAction * action)
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

// Doc from superclass.
void
SoDrawStyle::GLRender(SoGLRenderAction * action)
{
  SoDrawStyle::doAction(action);
}

// Doc from superclass.
void
SoDrawStyle::callback(SoCallbackAction * action)
{
  SoDrawStyle::doAction(action);
}
