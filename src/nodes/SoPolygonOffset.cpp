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
  \class SoPolygonOffset SoPolygonOffset.h Inventor/nodes/SoPolygonOffset.h
  \brief The SoPolygonOffset class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoPolygonOffset.h>



#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLPolygonOffsetElement.h>
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SOPOLYGONOFFSETELEMENT)
#include <Inventor/elements/SoPolygonOffsetElement.h>
#endif // !COIN_EXCLUDE_SOGLPOLYGONOFFSETELEMENT

#if !defined(COIN_EXCLUDE_SOOVERRIDEELEMENT)
#include <Inventor/elements/SoOverrideElement.h>
#endif // !COIN_EXCLUDE_SOOVERRIDEELEMENT

#include <Inventor/actions/SoCallbackAction.h>

/*!
  \enum SoPolygonOffset::Style
  FIXME: write documentation for enum
*/
/*!
  \var SoPolygonOffset::Style SoPolygonOffset::FILLED
  FIXME: write documentation for enum definition
*/
/*!
  \var SoPolygonOffset::Style SoPolygonOffset::LINES
  FIXME: write documentation for enum definition
*/
/*!
  \var SoPolygonOffset::Style SoPolygonOffset::POINTS
  FIXME: write documentation for enum definition
*/


/*!
  \var SoSFFloat SoPolygonOffset::factor
  FIXME: write documentation for field
*/
/*!
  \var SoSFFloat SoPolygonOffset::units
  FIXME: write documentation for field
*/
/*!
  \var SoSFBitMask SoPolygonOffset::styles
  FIXME: write documentation for field
*/
/*!
  \var SoSFBool SoPolygonOffset::on
  FIXME: write documentation for field
*/


// *************************************************************************

SO_NODE_SOURCE(SoPolygonOffset);

/*!
  Constructor.
*/
SoPolygonOffset::SoPolygonOffset()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoPolygonOffset);

  SO_NODE_ADD_FIELD(factor, (1.0f));
  SO_NODE_ADD_FIELD(units, (1.0f));
  SO_NODE_ADD_FIELD(styles, (SoPolygonOffset::FILLED));
  SO_NODE_ADD_FIELD(on, (TRUE));

  SO_NODE_DEFINE_ENUM_VALUE(Style, FILLED);
  SO_NODE_DEFINE_ENUM_VALUE(Style, LINES);
  SO_NODE_DEFINE_ENUM_VALUE(Style, POINTS);
  SO_NODE_SET_SF_ENUM_TYPE(styles, Style);
}

/*!
  Destructor.
*/
SoPolygonOffset::~SoPolygonOffset()
{
}

/*!
  Does initialization common for all objects of the
  SoPolygonOffset class. This includes setting up the
  type system, among other things.
*/
void
SoPolygonOffset::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoPolygonOffset);

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  SO_ENABLE(SoGLRenderAction, SoGLPolygonOffsetElement);
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

  SO_ENABLE(SoCallbackAction, SoPolygonOffsetElement);
}


#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write doc
 */
void
SoPolygonOffset::doAction(SoAction * action)
{
  SoState *state = action->getState();

#if !defined(COIN_EXCLUDE_SOOVERRIDEELEMENT)
  if (SoOverrideElement::getPolygonOffsetOverride(state)) return;
#endif

  float _factor, _units;
  SoPolygonOffsetElement::Style _styles;
  SbBool _on;

  SoPolygonOffsetElement::get(state, _factor, _units, _styles, _on);
  if (!factor.isIgnored()) _factor = factor.getValue();
  if (!units.isIgnored()) _units = units.getValue();
  if (!styles.isIgnored()) _styles = (SoPolygonOffsetElement::Style)styles.getValue();
  if (!on.isIgnored()) _on = on.getValue();

  SoPolygonOffsetElement::set(action->getState(),
                              this,
                              _factor,
                              _units,
                              _styles,
                              _on);
}
#endif // !COIN_EXCLUDE_SOACTION

#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
/*!
  FIXME: write doc
 */
void
SoPolygonOffset::callback(SoCallbackAction * action)
{
  SoPolygonOffset::doAction((SoAction*)action);
}
#endif // !COIN_EXCLUDE_SOCALLBACKACTION

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write doc
 */
void
SoPolygonOffset::GLRender(SoGLRenderAction * action)
{
  SoPolygonOffset::doAction((SoAction*)action);
}
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
