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
  \class SoDrawStyle SoDrawStyle.h Inventor/nodes/SoDrawStyle.h
  \brief The SoDrawStyle class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoDrawStyle.h>



#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
#include <Inventor/actions/SoGLRenderAction.h>
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
#include <Inventor/actions/SoCallbackAction.h>
#endif

#if !defined(COIN_EXCLUDE_SOGLDRAWSTYLEELEMENT)
#include <Inventor/elements/SoGLDrawStyleElement.h>
#endif // !COIN_EXCLUDE_SOGLDRAWSTYLEELEMENT
#if !defined(COIN_EXCLUDE_SOSHAPESTYLEELEMENT)
#include <Inventor/elements/SoShapeStyleElement.h>
#endif // !COIN_EXCLUDE_SOSHAPESTYLEELEMENT
#if !defined(COIN_EXCLUDE_SOGLLINEPATTERNELEMENT)
#include <Inventor/elements/SoGLLinePatternElement.h>
#endif // !COIN_EXCLUDE_SOGLLINEPATTERNELEMENT
#if !defined(COIN_EXCLUDE_SOGLLINEWIDTHELEMENT)
#include <Inventor/elements/SoGLLineWidthElement.h>
#endif // !COIN_EXCLUDE_SOGLLINEWIDTHELEMENT
#if !defined(COIN_EXCLUDE_SOGLPOINTSIZEELEMENT)
#include <Inventor/elements/SoGLPointSizeElement.h>
#endif // !COIN_EXCLUDE_SOGLPOINTSIZEELEMENT
#if !defined(COIN_EXCLUDE_SOGLPOLYGONSTIPPLEELEMENT)
#include <Inventor/elements/SoGLPolygonStippleElement.h>
#endif // !COIN_EXCLUDE_SOGLPOLYGONSTIPPLEELEMENT
#if !defined(COIN_EXCLUDE_SOOVERRIDEELEMENT)
#include <Inventor/elements/SoOverrideElement.h>
#endif // !COIN_EXCLUDE_SOOVERRIDEELEMENT

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

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  SO_ENABLE(SoGLRenderAction, SoGLDrawStyleElement);
  SO_ENABLE(SoGLRenderAction, SoShapeStyleElement);
  SO_ENABLE(SoGLRenderAction, SoGLLinePatternElement);
  SO_ENABLE(SoGLRenderAction, SoGLLineWidthElement);
  SO_ENABLE(SoGLRenderAction, SoGLPointSizeElement);
  SO_ENABLE(SoGLRenderAction, SoGLPolygonStippleElement);
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
}

/*!
  FIXME: write function documentation
*/
void 
SoDrawStyle::doAction(SoAction * /* action */)
{
  assert(0 && "FIXME: not implemented yet");
}

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write function documentation
*/
void 
SoDrawStyle::GLRender(SoGLRenderAction * action)
{
  SoState * state = action->getState();

#if !defined(COIN_EXCLUDE_SOOVERRIDEELEMENT)
  uint32_t flags = SoOverrideElement::getFlags(state);
#define TEST_OVERRIDE(bit) ((SoOverrideElement::bit & flags) != 0)
#else // COIN_EXCLUDE_SOOVERRIDEELEMENT
#define TEST_OVERRIDE(x,y) FALSE
#endif // COIN_EXCLUDE_SOOVERRIDEELEMENT
  
  if (!style.isIgnored() && !TEST_OVERRIDE(DRAW_STYLE)) {
    SoDrawStyleElement::set(state, this, 
			    (SoDrawStyleElement::Style)style.getValue());
  }
  if (!linePattern.isIgnored() && !TEST_OVERRIDE(LINE_PATTERN)) {
    SoLinePatternElement::set(state, this, linePattern.getValue());
  }
  if (!lineWidth.isIgnored() && !TEST_OVERRIDE(LINE_WIDTH)) {
    SoLineWidthElement::set(state, this, lineWidth.getValue());
  }
  if (!pointSize.isIgnored() && !TEST_OVERRIDE(POINT_SIZE)) {
    SoPointSizeElement::set(state, this, pointSize.getValue());
  }
#undef TEST_OVERRIDE
}
#endif // !COIN_EXCLUDE_SOGLRENDERACTION


#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
/*!
  FIXME: write doc
*/
void
SoDrawStyle::callback(SoCallbackAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOCALLBACKACTION
