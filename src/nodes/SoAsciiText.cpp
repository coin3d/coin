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
  \class SoAsciiText SoAsciiText.h Inventor/nodes/SoAsciiText.h
  \brief The SoAsciiText class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoAsciiText.h>



/*!
  \enum SoAsciiText::Justification
  FIXME: write documentation for enum
*/
/*!
  \var SoAsciiText::Justification SoAsciiText::LEFT
  FIXME: write documentation for enum definition
*/
/*!
  \var SoAsciiText::Justification SoAsciiText::RIGHT
  FIXME: write documentation for enum definition
*/
/*!
  \var SoAsciiText::Justification SoAsciiText::CENTER
  FIXME: write documentation for enum definition
*/


/*!
  \var SoMFString SoAsciiText::string
  FIXME: write documentation for field
*/
/*!
  \var SoSFFloat SoAsciiText::spacing
  FIXME: write documentation for field
*/
/*!
  \var SoSFEnum SoAsciiText::justification
  FIXME: write documentation for field
*/
/*!
  \var SoMFFloat SoAsciiText::width
  FIXME: write documentation for field
*/


// *************************************************************************

SO_NODE_SOURCE(SoAsciiText);

/*!
  Constructor.
*/
SoAsciiText::SoAsciiText()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoAsciiText);

  SO_NODE_ADD_FIELD(string, (""));
  SO_NODE_ADD_FIELD(spacing, (1.0f));
  SO_NODE_ADD_FIELD(justification, (SoAsciiText::LEFT));
  SO_NODE_ADD_FIELD(width, (0.0f));

  SO_NODE_DEFINE_ENUM_VALUE(Justification, LEFT);
  SO_NODE_DEFINE_ENUM_VALUE(Justification, RIGHT);
  SO_NODE_DEFINE_ENUM_VALUE(Justification, CENTER);
  SO_NODE_SET_SF_ENUM_TYPE(justification, Justification);
}

/*!
  Destructor.
*/
SoAsciiText::~SoAsciiText()
{
}

/*!
  Does initialization common for all objects of the
  SoAsciiText class. This includes setting up the
  type system, among other things.
*/
void
SoAsciiText::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoAsciiText);
}

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write doc
*/
void
SoAsciiText::GLRender(SoGLRenderAction * /* action */)
{
  COIN_STUB();
}
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SORAYPICKACTION)
/*!
  FIXME: write doc
*/
void
SoAsciiText::rayPick(SoRayPickAction * /* action */)
{
  COIN_STUB();
}
#endif // !COIN_EXCLUDE_SORAYPICKACTION

#if !defined(COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION)
/*!
  FIXME: write doc
*/
void
SoAsciiText::getPrimitiveCount(SoGetPrimitiveCountAction * /* action */)
{
  COIN_STUB();
}
#endif // !COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION

#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write doc
*/
void
SoAsciiText::computeBBox(SoAction * /* action */,
                         SbBox3f & /* box */, SbVec3f & /* center */)
{
  COIN_STUB();
}
#endif // !COIN_EXCLUDE_SOACTION

#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write doc
*/
void
SoAsciiText::generatePrimitives(SoAction *)
{
  COIN_STUB();
}
#endif // !COIN_EXCLUDE_SOACTION
