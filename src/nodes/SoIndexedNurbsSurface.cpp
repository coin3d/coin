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
  \class SoIndexedNurbsSurface SoIndexedNurbsSurface.h Inventor/nodes/SoIndexedNurbsSurface.h
  \brief The SoIndexedNurbsSurface class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoIndexedNurbsSurface.h>



/*!
  \var SoSFInt32 SoIndexedNurbsSurface::numUControlPoints
  FIXME: write documentation for field
*/
/*!
  \var SoSFInt32 SoIndexedNurbsSurface::numVControlPoints
  FIXME: write documentation for field
*/
/*!
  \var SoMFInt32 SoIndexedNurbsSurface::coordIndex
  FIXME: write documentation for field
*/
/*!
  \var SoMFFloat SoIndexedNurbsSurface::uKnotVector
  FIXME: write documentation for field
*/
/*!
  \var SoMFFloat SoIndexedNurbsSurface::vKnotVector
  FIXME: write documentation for field
*/
/*!
  \var SoSFInt32 SoIndexedNurbsSurface::numSControlPoints
  FIXME: write documentation for field
*/
/*!
  \var SoSFInt32 SoIndexedNurbsSurface::numTControlPoints
  FIXME: write documentation for field
*/
/*!
  \var SoMFInt32 SoIndexedNurbsSurface::textureCoordIndex
  FIXME: write documentation for field
*/
/*!
  \var SoMFFloat SoIndexedNurbsSurface::sKnotVector
  FIXME: write documentation for field
*/
/*!
  \var SoMFFloat SoIndexedNurbsSurface::tKnotVector
  FIXME: write documentation for field
*/


// *************************************************************************

SO_NODE_SOURCE(SoIndexedNurbsSurface);

/*!
  Constructor.
*/
SoIndexedNurbsSurface::SoIndexedNurbsSurface()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoIndexedNurbsSurface);

  SO_NODE_ADD_FIELD(numUControlPoints, (0));
  SO_NODE_ADD_FIELD(numVControlPoints, (0));
  SO_NODE_ADD_FIELD(coordIndex, (0));
  SO_NODE_ADD_FIELD(uKnotVector, (0));
  SO_NODE_ADD_FIELD(vKnotVector, (0));
  SO_NODE_ADD_FIELD(numSControlPoints, (0));
  SO_NODE_ADD_FIELD(numTControlPoints, (0));
  SO_NODE_ADD_FIELD(textureCoordIndex, (-1));
  SO_NODE_ADD_FIELD(sKnotVector, (0));
  SO_NODE_ADD_FIELD(tKnotVector, (0));
}

/*!
  Destructor.
*/
SoIndexedNurbsSurface::~SoIndexedNurbsSurface()
{
}

/*!
  Does initialization common for all objects of the
  SoIndexedNurbsSurface class. This includes setting up the
  type system, among other things.
*/
void
SoIndexedNurbsSurface::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoIndexedNurbsSurface);
}

/*!
  FIXME: write function documentation
*/
void
SoIndexedNurbsSurface::computeBBox(SoAction * /* action */,
                                 SbBox3f & /* box */, SbVec3f & /* center */)
{
  COIN_STUB();
}

/*!
  FIXME: write doc
*/
void
SoIndexedNurbsSurface::GLRender(SoGLRenderAction * /* action */)
{
  COIN_STUB();

  //
  // See comment in SoNurbsSurface::GLRender
  //

}

/*!
  FIXME: write doc
*/
void
SoIndexedNurbsSurface::rayPick(SoRayPickAction * /* action */)
{
  COIN_STUB();
}

/*!
  FIXME: write doc
*/
void
SoIndexedNurbsSurface::getPrimitiveCount(SoGetPrimitiveCountAction * /* action */)
{
  COIN_STUB();
}

/*!
  FIXME: write doc
*/
void
SoIndexedNurbsSurface::sendPrimitive(SoAction * ,  SoPrimitiveVertex *)
{
  COIN_STUB();
}

/*!
  FIXME: write doc
*/
void
SoIndexedNurbsSurface::generatePrimitives(SoAction * /* action */)
{
  COIN_STUB();
}
