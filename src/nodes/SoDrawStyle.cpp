/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

/*!
  \class SoDrawStyle SoDrawStyle.h Inventor/nodes/SoDrawStyle.h
  \brief The SoDrawStyle class specificies common rendering properties for shapes.
  \ingroup nodes

  Use SoDrawStyle nodes to influence how shape nodes following them in
  the scenegraph will be rendered.  This node type have fields to help
  decide how certain aspects of point-based shapes, line-based shapes
  and filled shape primitives are rendered.
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

  The valid range of pointsize settings varies according to which
  OpenGL implementation is used. For the purpose of not trying to set
  illegal values, the application programmer should at run-time use
  SoGLPointSizeElement::getValidRange() to find the valid range.

  Default value is 0.
*/
// FIXME: default value 0? Weird -- shouldn't that mean that no points
// were drawn? Test what SGI Inventor does in that case. 20010823 mortene.

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

  if (!this->style.isIgnored() && !TEST_OVERRIDE(DRAW_STYLE)) {
    SoDrawStyleElement::set(state, this,
                            (SoDrawStyleElement::Style)this->style.getValue());
    if (this->isOverride()) {
      SoOverrideElement::setDrawStyleOverride(state, this, TRUE);
    }
  }
  if (!this->linePattern.isIgnored() && !TEST_OVERRIDE(LINE_PATTERN)) {
    SoLinePatternElement::set(state, this, this->linePattern.getValue());
    if (this->isOverride()) {
      SoOverrideElement::setLinePatternOverride(state, this, TRUE);
    }
  }
  if (!this->lineWidth.isIgnored() && !TEST_OVERRIDE(LINE_WIDTH)) {
    SoLineWidthElement::set(state, this, this->lineWidth.getValue());
    if (this->isOverride()) {
      SoOverrideElement::setLineWidthOverride(state, this, TRUE);
    }
  }
  if (!this->pointSize.isIgnored() && !TEST_OVERRIDE(POINT_SIZE)) {

    float val = this->pointSize.getValue();
    // FIXME: this is a fix to avoid sending an invalid 0.0 value
    // (which is the pointSize field default value) to the
    // SoGLPointSizeElement (which will result in a
    // SoDebugError::postWarning()).  The "FIXME" part of this is that
    // the default pointSize value is stupid -- but we can't really
    // fix that without breaking the API 100% compatible goal with SGI
    // Open Inventor v2.1, even though it would be a very minor
    // incompatibility issue.  20010823 mortene.
    if (val == 0.0f) { val = 1.0f; }

    SoPointSizeElement::set(state, this, val);
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
