/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

/*!
  \class SoWWWAnchor SoWWWAnchor.h Inventor/nodes/SoWWWAnchor.h
  \brief The SoWWWAnchor class ...
  \ingroup nodes

  FIXME: write class doc

  \since Inventor 2.1
*/


#include <Inventor/nodes/SoWWWAnchor.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <coindefs.h> // COIN_STUB()


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

// doc in super
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

/*!
  FIXME: write doc
 */
void
SoWWWAnchor::handleEvent(SoHandleEventAction * /* action */)
{
  COIN_STUB();
}

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
