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
  \class SoPickStyle SoPickStyle.h Inventor/nodes/SoPickStyle.h
  \brief The SoPickStyle class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoPickStyle.h>
#include <Inventor/elements/SoPickStyleElement.h>



#if !defined(COIN_EXCLUDE_SOPICKACTION)
#include <Inventor/actions/SoPickAction.h>
#endif // ! COIN_EXCLUDE_SOPICKACTION

#if !defined(COIN_EXCLUDE_SOOVERRIDEELEMENT)
#include <Inventor/elements/SoOverrideElement.h>
#endif // !COIN_EXCLUDE_SOOVERRIDEELEMENT


/*!
  \enum SoPickStyle::Style
  FIXME: write documentation for enum
*/
/*!
  \var SoPickStyle::Style SoPickStyle::SHAPE
  FIXME: write documentation for enum definition
*/
/*!
  \var SoPickStyle::Style SoPickStyle::BOUNDING_BOX
  FIXME: write documentation for enum definition
*/
/*!
  \var SoPickStyle::Style SoPickStyle::UNPICKABLE
  FIXME: write documentation for enum definition
*/


/*!
  \var SoSFEnum SoPickStyle::style
  FIXME: write documentation for field
*/

// *************************************************************************

SO_NODE_SOURCE(SoPickStyle);

/*!
  Constructor.
*/
SoPickStyle::SoPickStyle()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoPickStyle);
  
  SO_NODE_ADD_FIELD(style, (SoPickStyle::SHAPE));

  SO_NODE_DEFINE_ENUM_VALUE(Style, SHAPE);
  SO_NODE_DEFINE_ENUM_VALUE(Style, BOUNDING_BOX);
  SO_NODE_DEFINE_ENUM_VALUE(Style, UNPICKABLE);
  SO_NODE_SET_SF_ENUM_TYPE(style, Style);
}

/*!
  Destructor.
*/
SoPickStyle::~SoPickStyle()
{
}

/*!
  Does initialization common for all objects of the
  SoPickStyle class. This includes setting up the
  type system, among other things.
*/
void
SoPickStyle::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoPickStyle);

#if !defined(COIN_EXCLUDE_SOPICKACTION)
  SO_ENABLE(SoPickAction, SoPickStyleElement);
#endif // ! COIN_EXCLUDE_SOPICKACTION
}


#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write doc
 */
void
SoPickStyle::doAction(SoAction *action)
{
  if (!style.isIgnored() 
#if !defined(COIN_EXCLUDE_SOOVERRIDEELEMENT)
      && !SoOverrideElement::getPickStyleOverride(action->getState())
#endif // !COIN_EXCLUDE_SOOVERRIDEELEMENT
      ) {
    SoPickStyleElement::set(action->getState(), this, 
			    (int32_t) style.getValue());
  }
}
#endif // !COIN_EXCLUDE_SOACTION

#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
/*!
  FIXME: write doc
 */
void
SoPickStyle::callback(SoCallbackAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOCALLBACKACTION

#if !defined(COIN_EXCLUDE_SOPICKACTION)
/*!
  FIXME: write doc
 */
void
SoPickStyle::pick(SoPickAction *action)
{
  SoPickStyle::doAction(action);
}
#endif // !COIN_EXCLUDE_SOPICKACTION
