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
  \class SoLocateHighlight SoLocateHighlight.h Inventor/nodes/SoLocateHighlight.h
  \brief The SoLocateHighlight class ...
  \ingroup nodes

  FIXME: write class doc
*/


#include <Inventor/nodes/SoLocateHighlight.h>
#include <coindefs.h> // COIN_STUB()


/*!
  \enum SoLocateHighlight::Modes
  FIXME: write documentation for enum
*/
/*!
  \var SoLocateHighlight::Modes SoLocateHighlight::AUTO
  FIXME: write documentation for enum definition
*/
/*!
  \var SoLocateHighlight::Modes SoLocateHighlight::ON
  FIXME: write documentation for enum definition
*/
/*!
  \var SoLocateHighlight::Modes SoLocateHighlight::OFF
  FIXME: write documentation for enum definition
*/
/*!
  \enum SoLocateHighlight::Styles
  FIXME: write documentation for enum
*/
/*!
  \var SoLocateHighlight::Styles SoLocateHighlight::EMISSIVE
  FIXME: write documentation for enum definition
*/
/*!
  \var SoLocateHighlight::Styles SoLocateHighlight::EMISSIVE_DIFFUSE
  FIXME: write documentation for enum definition
*/


/*!
  \var SoSFColor SoLocateHighlight::color
  FIXME: write documentation for field
*/
/*!
  \var SoSFEnum SoLocateHighlight::style
  FIXME: write documentation for field
*/
/*!
  \var SoSFEnum SoLocateHighlight::mode
  FIXME: write documentation for field
*/


// *************************************************************************

SO_NODE_SOURCE(SoLocateHighlight);

/*!
  Constructor.
*/
SoLocateHighlight::SoLocateHighlight()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoLocateHighlight);

  SO_NODE_ADD_FIELD(color, (SbColor(0.3f, 0.3f, 0.3f)));
  SO_NODE_ADD_FIELD(style, (EMISSIVE));
  SO_NODE_ADD_FIELD(mode, (AUTO));

  SO_NODE_DEFINE_ENUM_VALUE(Styles, EMISSIVE);
  SO_NODE_DEFINE_ENUM_VALUE(Styles, EMISSIVE_DIFFUSE);
  SO_NODE_SET_SF_ENUM_TYPE(style, Styles);

  SO_NODE_DEFINE_ENUM_VALUE(Modes, AUTO);
  SO_NODE_DEFINE_ENUM_VALUE(Modes, ON);
  SO_NODE_DEFINE_ENUM_VALUE(Modes, OFF);
  SO_NODE_SET_SF_ENUM_TYPE(mode, Modes);
}

/*!
  Destructor.
*/
SoLocateHighlight::~SoLocateHighlight()
{
}

/*!
  Does initialization common for all objects of the
  SoLocateHighlight class. This includes setting up the
  type system, among other things.
*/
void
SoLocateHighlight::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoLocateHighlight);
}

/*!
  FIXME: write function documentation
*/
void
SoLocateHighlight::turnOffCurrentHighlight(SoGLRenderAction * /* action */)
{
  // FIXME: stub. 19990224 mortene.
}


/*!
  FIXME: write doc
 */
void
SoLocateHighlight::handleEvent(SoHandleEventAction *action)
{
  // FIXME: implement (pederb)
  inherited::handleEvent(action);
}

/*!
  FIXME: write doc
 */
void
SoLocateHighlight::GLRenderBelowPath(SoGLRenderAction * /* action */)
{
  COIN_STUB();
}

/*!
  FIXME: write doc
 */
void
SoLocateHighlight::GLRenderInPath(SoGLRenderAction * /* action */)
{
  COIN_STUB();
}

/*!
  FIXME: write doc
 */
void
SoLocateHighlight::redrawHighlighted(SoAction * /* act */, SbBool /* flag */)
{
  COIN_STUB();
}
