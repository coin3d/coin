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
  \class SoLevelOfSimplification SoLevelOfSimplification.h Inventor/nodes/SoLevelOfSimplification.h
  \brief The SoLevelOfSimplification class ...
  \ingroup nodes

  FIXME: write class doc
*/


#include <Inventor/nodes/SoLevelOfSimplification.h>
#include <coindefs.h> // COIN_STUB()
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/elements/SoDecimationTypeElement.h>
#include <Inventor/elements/SoDecimationPercentageElement.h>

/*!
  \var SoMFFloat SoLevelOfSimplification::percentages
  FIXME: write documentation for field
*/
/*!
  \var SoSFInt32 SoLevelOfSimplification::numTriangles
  FIXME: write documentation for field
*/

// *************************************************************************

SO_NODE_SOURCE(SoLevelOfSimplification);

/*!
  Constructor.
*/
SoLevelOfSimplification::SoLevelOfSimplification()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoLevelOfSimplification);

  SO_NODE_ADD_FIELD(percentages, (0.0f));
  // FIXME: is this the correct way of making an empty multifield?
  // 19990301 mortene.
  percentages.setNum(0);

  SO_NODE_ADD_FIELD(numTriangles, (-1));
}

/*!
  Destructor.
*/
SoLevelOfSimplification::~SoLevelOfSimplification()
{
}

/*!
  Does initialization common for all objects of the
  SoLevelOfSimplification class. This includes setting up the
  type system, among other things.
*/
void
SoLevelOfSimplification::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoLevelOfSimplification);

  SO_ENABLE(SoGetBoundingBoxAction, SoDecimationTypeElement);
  SO_ENABLE(SoGetBoundingBoxAction, SoDecimationPercentageElement);
}


/*!
  FIXME: write doc
 */
void
SoLevelOfSimplification::getPrimitiveCount(SoGetPrimitiveCountAction * /* action */)
{
  COIN_STUB();
}

/*!
  FIXME: write doc
 */
int
SoLevelOfSimplification::whichToTraverse(SoAction *)
{
  COIN_STUB();
  return 0;
}
