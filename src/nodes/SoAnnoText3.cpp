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
  \class SoAnnoText3 SoAnnoText3.h Inventor/nodes/SoAnnoText3.h
  \brief The SoAnnoText3 class ...
  \ingroup nodes

  FIXME: write class doc
*/

// Metadon doc:
/*¡
  This class is not implemented yet.
 */

#include <Inventor/nodes/SoAnnoText3.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <coindefs.h> // COIN_STUB()


/*!
  \enum SoAnnoText3::Justification
  FIXME: write documentation for enum
*/
/*!
  \var SoAnnoText3::Justification SoAnnoText3::LEFT
  FIXME: write documentation for enum definition
*/
/*!
  \var SoAnnoText3::Justification SoAnnoText3::RIGHT
  FIXME: write documentation for enum definition
*/
/*!
  \var SoAnnoText3::Justification SoAnnoText3::CENTER
  FIXME: write documentation for enum definition
*/


/*!
  \var SoMFString SoAnnoText3::string
  FIXME: write documentation for field
*/
/*!
  \var SoSFFloat SoAnnoText3::spacing
  FIXME: write documentation for field
*/
/*!
  \var SoSFEnum SoAnnoText3::justification
  FIXME: write documentation for field
*/
/*!
  \var SoSFNode SoAnnoText3::alternateRep
  FIXME: write documentation for field
*/

// *************************************************************************

SO_NODE_SOURCE(SoAnnoText3);

/*!
  Constructor.
*/
SoAnnoText3::SoAnnoText3()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoAnnoText3);

  SO_NODE_ADD_FIELD(string, (""));
  SO_NODE_ADD_FIELD(spacing, (1.0f));
  SO_NODE_ADD_FIELD(justification, (SoAnnoText3::LEFT));
  // FIXME: this is not correct -- fix.
  SO_NODE_ADD_FIELD(alternateRep, (NULL));

  SO_NODE_DEFINE_ENUM_VALUE(Justification, LEFT);
  SO_NODE_DEFINE_ENUM_VALUE(Justification, RIGHT);
  SO_NODE_DEFINE_ENUM_VALUE(Justification, CENTER);
  SO_NODE_SET_SF_ENUM_TYPE(justification, Justification);
}

/*!
  Destructor.
*/
SoAnnoText3::~SoAnnoText3()
{
}


/*!
  Does initialization common for all objects of the
  SoAnnoText3 class. This includes setting up the
  type system, among other things.
*/
void
SoAnnoText3::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoAnnoText3);
}


/*!
  FIXME: write doc
*/
void
SoAnnoText3::computeBBox(SoAction * /* action */,
                         SbBox3f & /* box */, SbVec3f & /* center */)
{
  COIN_STUB();
}

/*!
  FIXME: write doc
*/
SbBox3f
SoAnnoText3::getCharacterBounds(SoState * /* state */,
                                int /* stringIndex */, int /* charIndex */)
{
  COIN_STUB();
  static SbBox3f b;
  return b;
}

/*!
  FIXME: write doc
*/
void
SoAnnoText3::GLRender(SoGLRenderAction * /* action */)
{
  COIN_STUB();
}

/*!
  FIXME: write doc
*/
void
SoAnnoText3::rayPick(SoRayPickAction * /* action */)
{
  COIN_STUB();
}

/*!
  FIXME: write doc
*/
void
SoAnnoText3::write(SoWriteAction * /* action */)
{
  COIN_STUB();
}

/*!
  FIXME: write doc
*/
void
SoAnnoText3::getPrimitiveCount(SoGetPrimitiveCountAction * /* action */)
{
  COIN_STUB();
}

/*!
  FIXME: write doc
*/
void
SoAnnoText3::generatePrimitives(SoAction *)
{
  COIN_STUB();
}
