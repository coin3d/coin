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
  \class SoText3 SoText3.h Inventor/nodes/SoText3.h
  \brief The SoText3 class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoText3.h>
#include <coindefs.h> // COIN_STUB()

#include <Inventor/errors/SoDebugError.h>

/*!
  \enum SoText3::Part
  FIXME: write documentation for enum
*/
/*!
  \var SoText3::Part SoText3::FRONT
  FIXME: write documentation for enum definition
*/
/*!
  \var SoText3::Part SoText3::SIDES
  FIXME: write documentation for enum definition
*/
/*!
  \var SoText3::Part SoText3::BACK
  FIXME: write documentation for enum definition
*/
/*!
  \var SoText3::Part SoText3::ALL
  FIXME: write documentation for enum definition
*/

/*!
  \enum SoText3::Justification
  FIXME: write documentation for enum
*/
/*!
  \var SoText3::Justification SoText3::LEFT
  FIXME: write documentation for enum definition
*/
/*!
  \var SoText3::Justification SoText3::RIGHT
  FIXME: write documentation for enum definition
*/
/*!
  \var SoText3::Justification SoText3::CENTER
  FIXME: write documentation for enum definition
*/


/*!
  \var SoMFString SoText3::string
  FIXME: write documentation for field
*/
/*!
  \var SoSFFloat SoText3::spacing
  FIXME: write documentation for field
*/
/*!
  \var SoSFEnum SoText3::justification
  FIXME: write documentation for field
*/
/*!
  \var SoSFBitMask SoText3::parts
  FIXME: write documentation for field
*/

// *************************************************************************

SO_NODE_SOURCE(SoText3);

/*!
  Constructor.
*/
SoText3::SoText3()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoText3);

  SO_NODE_ADD_FIELD(string, (""));
  SO_NODE_ADD_FIELD(spacing, (1.0f));
  SO_NODE_ADD_FIELD(justification, (SoText3::LEFT));
  SO_NODE_ADD_FIELD(parts, (SoText3::FRONT));

  SO_NODE_DEFINE_ENUM_VALUE(Justification, LEFT);
  SO_NODE_DEFINE_ENUM_VALUE(Justification, RIGHT);
  SO_NODE_DEFINE_ENUM_VALUE(Justification, CENTER);
  SO_NODE_SET_SF_ENUM_TYPE(justification, Justification);

  SO_NODE_DEFINE_ENUM_VALUE(Part, FRONT);
  SO_NODE_DEFINE_ENUM_VALUE(Part, SIDES);
  SO_NODE_DEFINE_ENUM_VALUE(Part, BACK);
  SO_NODE_DEFINE_ENUM_VALUE(Part, ALL);
  SO_NODE_SET_SF_ENUM_TYPE(parts, Part);
}

/*!
  Destructor.
*/
SoText3::~SoText3()
{
}

/*!
  Does initialization common for all objects of the
  SoText3 class. This includes setting up the
  type system, among other things.
*/
void
SoText3::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoText3);
}

/*!
  FIXME: write function documentation
*/
void
SoText3::computeBBox(SoAction * /* action */,
                     SbBox3f & /* box */, SbVec3f & /* center */)
{
  COIN_STUB();
}


/*!
  FIXME: write doc
 */
SbBox3f
SoText3::getCharacterBounds(SoState * /* state */,
                            int /* stringIndex */, int /* charIndex */)
{
  COIN_STUB();
  return SbBox3f();
}

/*!
  FIXME: write doc
 */
void
SoText3::GLRender(SoGLRenderAction * /* action */)
{
  COIN_STUB();
}

/*!
  FIXME: write doc
 */
void
SoText3::rayPick(SoRayPickAction * /* action */)
{
  COIN_STUB();
}

/*!
  FIXME: write doc
 */
void
SoText3::getPrimitiveCount(SoGetPrimitiveCountAction * /* action */)
{
  COIN_STUB();
}

/*!
  FIXME: write doc
 */
void
SoText3::generatePrimitives(SoAction *)
{
  COIN_STUB();
}
