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
  \class SoLevelOfSimplification SoLevelOfSimplification.h Inventor/nodes/SoLevelOfSimplification.h
  \brief The SoLevelOfSimplification class ...
  \ingroup nodes

  FIXME: write class doc
*/


#include <Inventor/nodes/SoLevelOfSimplification.h>
#include <Inventor/nodes/SoSubNodeP.h>
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

  SO_NODE_ADD_FIELD(numTriangles, (-1));
  SO_NODE_ADD_FIELD(percentages, (0.0f));

  // Make multivalue field empty.
  this->percentages.setNum(0);
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
