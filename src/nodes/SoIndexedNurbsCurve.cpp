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
  \class SoIndexedNurbsCurve SoIndexedNurbsCurve.h Inventor/nodes/SoIndexedNurbsCurve.h
  \brief The SoIndexedNurbsCurve class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoIndexedNurbsCurve.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/SbName.h>

/*!
  \var SoSFInt32 SoIndexedNurbsCurve::numControlPoints
  FIXME: write documentation for field
*/
/*!
  \var SoMFInt32 SoIndexedNurbsCurve::coordIndex
  FIXME: write documentation for field
*/
/*!
  \var SoMFFloat SoIndexedNurbsCurve::knotVector
  FIXME: write documentation for field
*/


// *************************************************************************

//$ BEGIN TEMPLATE NodeSource(SoIndexedNurbsCurve)
SoType SoIndexedNurbsCurve::classTypeId = SoType::badType();

/*!
  Returns a new instance of the SoIndexedNurbsCurve node class.
*/
void *
SoIndexedNurbsCurve::createInstance(void)
{
  return new SoIndexedNurbsCurve;
}

/*!
  Returns the unique type identifier for the SoIndexedNurbsCurve class.
*/
SoType
SoIndexedNurbsCurve::getClassTypeId(void)
{
  return SoIndexedNurbsCurve::classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoIndexedNurbsCurve::getTypeId(void) const
{
  return SoIndexedNurbsCurve::classTypeId;
}
//$ END TEMPLATE NodeSource

/*!
  Constructor.
*/
SoIndexedNurbsCurve::SoIndexedNurbsCurve()
{
//$ BEGIN TEMPLATE NodeConstructor(SoIndexedNurbsCurve)
  // Make sure the class has been initialized.
  assert(SoIndexedNurbsCurve::classTypeId != SoType::badType());
//$ END TEMPLATE NodeConstructor

  SO_NODE_ADD_FIELD(numControlPoints, (0));
  SO_NODE_ADD_FIELD(coordIndex, (0));
  SO_NODE_ADD_FIELD(knotVector, (0));
}

/*!
  Destructor.
*/
SoIndexedNurbsCurve::~SoIndexedNurbsCurve()
{
}

/*!
  Does initialization common for all objects of the
  SoIndexedNurbsCurve class. This includes setting up the
  type system, among other things.
*/
void
SoIndexedNurbsCurve::initClass(void)
{
//$ BEGIN TEMPLATE InitNodeSource(IndexedNurbsCurve)
  // Make sure we only initialize once.
  assert(SoIndexedNurbsCurve::classTypeId == SoType::badType());
  // Make sure superclass get initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoIndexedNurbsCurve::classTypeId =
    SoType::createType(inherited::getClassTypeId(), "IndexedNurbsCurve",
                       &SoIndexedNurbsCurve::createInstance,
                       SoNode::nextActionMethodIndex++);
//$ END TEMPLATE InitNodeSource
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoIndexedNurbsCurve::cleanClass(void)
{
}

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
/*!
  FIXME: write function documentation
*/
void
SoIndexedNurbsCurve::computeBBox(SoAction * /* action */,
				 SbBox3f & /* box */, SbVec3f & /* center */)
{
  assert(0 && "FIXME: not implemented yet");
}
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION


#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write doc
*/
void
SoIndexedNurbsCurve::GLRender(SoGLRenderAction * /* action */)
{
  assert(0 && "FIXME: not implemented");

  //
  // See comment in SoNurbsSurface::GLRender
  //
}
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SORAYPICKACTION)
/*!
  FIXME: write doc
*/
void
SoIndexedNurbsCurve::rayPick(SoRayPickAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SORAYPICKACTION

#if !defined(COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION)
/*!
  FIXME: write doc
*/
void
SoIndexedNurbsCurve::getPrimitiveCount(SoGetPrimitiveCountAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
/*!
  FIXME: write doc
*/
void
SoIndexedNurbsCurve::getBoundingBox(SoGetBoundingBoxAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION

#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write doc
*/
void
SoIndexedNurbsCurve::sendPrimitive(SoAction * ,  SoPrimitiveVertex *)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOACTION

#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write doc
*/
void
SoIndexedNurbsCurve::generatePrimitives(SoAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOACTION
