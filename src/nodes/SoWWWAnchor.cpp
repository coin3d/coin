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
  \class SoWWWAnchor SoWWWAnchor.h Inventor/nodes/SoWWWAnchor.h
  \brief The SoWWWAnchor class ...
  \ingroup nodes

  FIXME: write class doc
*/


#include <Inventor/nodes/SoWWWAnchor.h>


/*!
  \enum SoWWWAnchor::Mapping
  FIXME: write documentation for enum
*/
/*!
  \var SoWWWAnchor::Mapping SoWWWAnchor::NONE
  FIXME: write documentation for enum definition
*/
/*!
  \var SoWWWAnchor::Mapping SoWWWAnchor::POINT
  FIXME: write documentation for enum definition
*/


/*!
  \var SoSFString SoWWWAnchor::name
  FIXME: write documentation for field
*/
/*!
  \var SoSFString SoWWWAnchor::description
  FIXME: write documentation for field
*/
/*!
  \var SoSFEnum SoWWWAnchor::map
  FIXME: write documentation for field
*/

// *************************************************************************

SO_NODE_SOURCE(SoWWWAnchor);

/*!
  Constructor.
*/
SoWWWAnchor::SoWWWAnchor()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoWWWAnchor);

  SO_NODE_ADD_FIELD(name, ("<Undefined URL>"));
  SO_NODE_ADD_FIELD(description, (""));
  SO_NODE_ADD_FIELD(map, (NONE));

  SO_NODE_DEFINE_ENUM_VALUE(Map, NONE);
  SO_NODE_DEFINE_ENUM_VALUE(Map, POINT);
  SO_NODE_SET_SF_ENUM_TYPE(map, Map);
}

/*!
  Destructor.
*/
SoWWWAnchor::~SoWWWAnchor()
{
}

/*!
  Does initialization common for all objects of the
  SoWWWAnchor class. This includes setting up the
  type system, among other things.
*/
void
SoWWWAnchor::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoWWWAnchor);
}


/*!
  FIXME: write doc
 */
void
SoWWWAnchor::setFullURLName(const SbString & /* url */)
{
  COIN_STUB();
}

/*!
  FIXME: write doc
 */
const SbString &
SoWWWAnchor::getFullURLName(void)
{
  COIN_STUB();
  static SbString s;
  return s;
}

#if !defined(COIN_EXCLUDE_SOHANDLEEVENTACTION)
/*!
  FIXME: write doc
 */
void
SoWWWAnchor::handleEvent(SoHandleEventAction * /* action */)
{
  COIN_STUB();
}
#endif // !COIN_EXCLUDE_SOHANDLEEVENTACTION

/*!
  FIXME: write doc
 */
void
SoWWWAnchor::setFetchURLCallBack(SoWWWAnchorCB * /* f */, void * /* userData */)
{
  COIN_STUB();
}

/*!
  FIXME: write doc
 */
void
SoWWWAnchor::setHighlightURLCallBack(SoWWWAnchorCB * /* f */, void * /* userData */)
{
  COIN_STUB();
}

/*!
  FIXME: write doc
 */
void
SoWWWAnchor::redrawHighlighted(SoAction * /* act */, SbBool /* isNowHighlighting */)
{
  COIN_STUB();
}
