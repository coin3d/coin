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
  \class SoLevelOfDetail SoLevelOfDetail.h Inventor/nodes/SoLevelOfDetail.h
  \brief The SoLevelOfDetail class ...
  \ingroup nodes

  FIXME: write class doc
*/


#include <Inventor/nodes/SoLevelOfDetail.h>


/*!
  \var SoMFFloat SoLevelOfDetail::screenArea
  FIXME: write documentation for field
*/

// *************************************************************************

SO_NODE_SOURCE(SoLevelOfDetail);

/*!
  Constructor.
*/
SoLevelOfDetail::SoLevelOfDetail()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoLevelOfDetail);

  SO_NODE_ADD_FIELD(screenArea, (0));
}

/*!
  Destructor.
*/
SoLevelOfDetail::~SoLevelOfDetail()
{
}

/*!
  Does initialization common for all objects of the
  SoLevelOfDetail class. This includes setting up the
  type system, among other things.
*/
void
SoLevelOfDetail::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoLevelOfDetail);
}


#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write doc
 */
void
SoLevelOfDetail::doAction(SoAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOACTION

#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
/*!
  FIXME: write doc
 */
void
SoLevelOfDetail::callback(SoCallbackAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOCALLBACKACTION

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write doc
 */
void
SoLevelOfDetail::GLRender(SoGLRenderAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SORAYPICKACTION)
/*!
  FIXME: write doc
 */
void
SoLevelOfDetail::rayPick(SoRayPickAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SORAYPICKACTION
