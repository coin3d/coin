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
  \class SoAntiSquish SoAntiSquish.h Inventor/nodes/SoAntiSquish.h
  \brief The SoAntiSquish class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoAntiSquish.h>



/*!
  \enum SoAntiSquish::Sizing
  FIXME: write documentation for enum
*/
/*!
  \var SoAntiSquish::Sizing SoAntiSquish::X
  FIXME: write documentation for enum definition
*/
/*!
  \var SoAntiSquish::Sizing SoAntiSquish::Y
  FIXME: write documentation for enum definition
*/
/*!
  \var SoAntiSquish::Sizing SoAntiSquish::Z
  FIXME: write documentation for enum definition
*/
/*!
  \var SoAntiSquish::Sizing SoAntiSquish::AVERAGE_DIMENSION
  FIXME: write documentation for enum definition
*/
/*!
  \var SoAntiSquish::Sizing SoAntiSquish::BIGGEST_DIMENSION
  FIXME: write documentation for enum definition
*/
/*!
  \var SoAntiSquish::Sizing SoAntiSquish::SMALLEST_DIMENSION
  FIXME: write documentation for enum definition
*/
/*!
  \var SoAntiSquish::Sizing SoAntiSquish::LONGEST_DIAGONAL
  FIXME: write documentation for enum definition
*/


/*!
  \var SoSFEnum SoAntiSquish::sizing
  FIXME: write documentation for field
*/
/*!
  \var SoSFBool SoAntiSquish::recalcAlways
  FIXME: write documentation for field
*/


// *************************************************************************

SO_NODE_SOURCE(SoAntiSquish);

/*!
  Constructor.
*/
SoAntiSquish::SoAntiSquish()
{
  SO_NODE_CONSTRUCTOR(SoAntiSquish);

  SO_NODE_ADD_FIELD(recalcAlways, (TRUE));
  SO_NODE_ADD_FIELD(sizing, (SoAntiSquish::AVERAGE_DIMENSION));

  SO_NODE_DEFINE_ENUM_VALUE(Sizing, X);
  SO_NODE_DEFINE_ENUM_VALUE(Sizing, Y);
  SO_NODE_DEFINE_ENUM_VALUE(Sizing, Z);
  SO_NODE_DEFINE_ENUM_VALUE(Sizing, AVERAGE_DIMENSION);
  SO_NODE_DEFINE_ENUM_VALUE(Sizing, BIGGEST_DIMENSION);
  SO_NODE_DEFINE_ENUM_VALUE(Sizing, SMALLEST_DIMENSION);
  SO_NODE_DEFINE_ENUM_VALUE(Sizing, LONGEST_DIAGONAL);
  SO_NODE_SET_SF_ENUM_TYPE(sizing, Sizing);
}

/*!
  Destructor.
*/
SoAntiSquish::~SoAntiSquish()
{
}

/*!
  Does initialization common for all objects of the
  SoAntiSquish class. This includes setting up the
  type system, among other things.
*/
void
SoAntiSquish::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoAntiSquish);
}

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
/*!
  FIXME: write function documentation
*/
void 
SoAntiSquish::getBoundingBox(SoGetBoundingBoxAction * /* action */)
{
  assert(0 && "FIXME: not implemented yet");
}
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION

/*!
  FIXME: write doc
*/
void
SoAntiSquish::recalc(void)
{
  assert(0 && "FIXME: not implemented");
}

#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write doc
*/
void
SoAntiSquish::doAction(SoAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOACTION

#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
/*!
  FIXME: write doc
*/
void
SoAntiSquish::callback(SoCallbackAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOCALLBACKACTION

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write doc
*/
void
SoAntiSquish::GLRender(SoGLRenderAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SOGETMATRIXACTION)
/*!
  FIXME: write doc
*/
void
SoAntiSquish::getMatrix(SoGetMatrixAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOGETMATRIXACTION

#if !defined(COIN_EXCLUDE_SOPICKACTION)
/*!
  FIXME: write doc
*/
void
SoAntiSquish::pick(SoPickAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOPICKACTION

/*!
  FIXME: write doc
*/
SbMatrix
SoAntiSquish::getUnsquishingMatrix(SbMatrix /* squishedMatrix */,
				   SbBool /* doInverse */,
				   SbMatrix & /* inverseAnswer */)
{
  assert(0 && "FIXME: not implemented");
  return SbMatrix::identity();
}
