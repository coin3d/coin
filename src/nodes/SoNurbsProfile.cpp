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
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/SbName.h>

/*!
  \var SoMFFloat SoNurbsProfile::knotVector
  FIXME: write documentation for field
*/

// *************************************************************************

//$ BEGIN TEMPLATE NodeSource(SoNurbsProfile)
SoType SoNurbsProfile::classTypeId = SoType::badType();

/*!
  Returns a new instance of the SoNurbsProfile node class.
*/
void *
SoNurbsProfile::createInstance(void)
{
  return new SoNurbsProfile;
}

/*!
  Returns the unique type identifier for the SoNurbsProfile class.
*/
SoType
SoNurbsProfile::getClassTypeId(void)
{
  return SoNurbsProfile::classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoNurbsProfile::getTypeId(void) const
{
  return SoNurbsProfile::classTypeId;
}
//$ END TEMPLATE NodeSource

/*!
  Constructor.
*/
SoNurbsProfile::SoNurbsProfile()
{
//$ BEGIN TEMPLATE NodeConstructor(SoNurbsProfile)
  // Make sure the class has been initialized.
  assert(SoNurbsProfile::classTypeId != SoType::badType());
//$ END TEMPLATE NodeConstructor

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
//$ BEGIN TEMPLATE InitNodeSource(NurbsProfile)
  // Make sure we only initialize once.
  assert(SoNurbsProfile::classTypeId == SoType::badType());
  // Make sure superclass get initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoNurbsProfile::classTypeId =
    SoType::createType(inherited::getClassTypeId(), "NurbsProfile",
                       &SoNurbsProfile::createInstance,
                       SoNode::nextActionMethodIndex++);
//$ END TEMPLATE InitNodeSource
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoNurbsProfile::cleanClass(void)
{
}


/*!
  FIXME: write doc
 */
void
SoNurbsProfile::getTrimCurve(SoState * state, int32_t & numPoints,
			     float *& points, int & floatsPerVec,
			     int32_t & numKnots, float *& knotVector)
{
  assert(0 && "FIXME: not implemented");
}

/*!
  FIXME: write doc
 */
void
SoNurbsProfile::getVertices(SoState * state, int32_t & nVertices,
			    SbVec2f *& vertices)
{
  assert(0 && "FIXME: not implemented");
}
