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
  \class SoNurbsProfile SoNurbsProfile.h Inventor/nodes/SoNurbsProfile.h
  \brief The SoNurbsProfile class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoNurbsProfile.h>



/*!
  \var SoMFFloat SoNurbsProfile::knotVector
  FIXME: write documentation for field
*/

// *************************************************************************

SO_NODE_SOURCE(SoNurbsProfile);

/*!
  Constructor.
*/
SoNurbsProfile::SoNurbsProfile()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoNurbsProfile);

  SO_NODE_ADD_FIELD(knotVector, (0.0f));
}

/*!
  Destructor.
*/
SoNurbsProfile::~SoNurbsProfile()
{
}

/*!
  Does initialization common for all objects of the
  SoNurbsProfile class. This includes setting up the
  type system, among other things.
*/
void
SoNurbsProfile::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoNurbsProfile);
}


/*!
  FIXME: write doc
 */
void
SoNurbsProfile::getTrimCurve(SoState * /* state */, int32_t & /* numPoints */,
                             float *& /* points */, int & /* floatsPerVec */,
                             int32_t & /* numKnots */, float *& /* knotVector */)
{
  COIN_STUB();
}

/*!
  FIXME: write doc
 */
void
SoNurbsProfile::getVertices(SoState * /* state */, int32_t & /* nVertices */,
                            SbVec2f *& /* vertices */)
{
  COIN_STUB();
}
