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
  \class SoLinearProfile SoLinearProfile.h Inventor/nodes/SoLinearProfile.h
  \brief The SoLinearProfile class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoLinearProfile.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/SbName.h>

// *************************************************************************

//$ BEGIN TEMPLATE NodeSource(SoLinearProfile)
SoType SoLinearProfile::classTypeId = SoType::badType();

/*!
  Returns a new instance of the SoLinearProfile node class.
*/
void *
SoLinearProfile::createInstance(void)
{
  return new SoLinearProfile;
}

/*!
  Returns the unique type identifier for the SoLinearProfile class.
*/
SoType
SoLinearProfile::getClassTypeId(void)
{
  return SoLinearProfile::classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoLinearProfile::getTypeId(void) const
{
  return SoLinearProfile::classTypeId;
}
//$ END TEMPLATE NodeSource

/*!
  Constructor.
*/
SoLinearProfile::SoLinearProfile()
{
//$ BEGIN TEMPLATE NodeConstructor(SoLinearProfile)
  // Make sure the class has been initialized.
  assert(SoLinearProfile::classTypeId != SoType::badType());
//$ END TEMPLATE NodeConstructor
}

/*!
  Destructor.
*/
SoLinearProfile::~SoLinearProfile()
{
}

/*!
  Does initialization common for all objects of the
  SoLinearProfile class. This includes setting up the
  type system, among other things.
*/
void
SoLinearProfile::initClass(void)
{
//$ BEGIN TEMPLATE InitNodeSource(LinearProfile)
  // Make sure we only initialize once.
  assert(SoLinearProfile::classTypeId == SoType::badType());
  // Make sure superclass get initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoLinearProfile::classTypeId =
    SoType::createType(inherited::getClassTypeId(), "LinearProfile",
                       &SoLinearProfile::createInstance,
                       SoNode::nextActionMethodIndex++);
//$ END TEMPLATE InitNodeSource
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoLinearProfile::cleanClass(void)
{
}

/*!
  FIXME: write doc
 */
void
SoLinearProfile::getTrimCurve(SoState * /* state */, int32_t & /* numPoints */,
			      float *& /* points */, int & /* floatsPerVec */,
			      int32_t & /* numKnots */, float *& /* knotVector */)
{
  assert(0 && "FIXME: not implemented");
}

/*!
  FIXME: write doc
 */
void
SoLinearProfile::getVertices(SoState * /* state */, int32_t & /* nVertices */,
			     SbVec2f *& /* vertices */)
{
  assert(0 && "FIXME: not implemented");
}
