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
  \class SoNodeKitDetail SoNodeKitDetail.h Inventor/details/SoNodeKitDetail.h
  \brief The SoNodeKitDetail class is yet to be documented.
  \ingroup details

  When a pick action is executed and geometry within a nodekit is hit,
  the nodekit generates an SoNodeKitDetail object which contains
  information about the specific part inside the nodekit hit by the pick
  ray.

  \sa SoRayPickAction
*/

#include <Inventor/details/SoNodeKitDetail.h>

SO_DETAIL_SOURCE(SoNodeKitDetail);


/*!
  Constructor.
*/
SoNodeKitDetail::SoNodeKitDetail(void)
  : myNodeKit(NULL), myPart(NULL)
{
}

/*!
  Destructor.
*/
SoNodeKitDetail::~SoNodeKitDetail()
{
  // FIXME: unref() nodekit and node if they where ref'ed in the
  // set*() methods. 19991109 mortene.
}

// Doc in superclass.
void
SoNodeKitDetail::initClass(void)
{
  SoNodeKitDetail::classTypeId =
    SoType::createType(inherited::getClassTypeId(),
                       SbName("SoNodeKitDetail"));
}

// Doc in superclass.
SoDetail *
SoNodeKitDetail::copy(void) const
{
  SoNodeKitDetail * copy = new SoNodeKitDetail();
  *copy = *this;
  // FIXME: ref() nodekit and node? 19991109 mortene.
  return copy;
}

/*!
  Set the pointer indicating which nodekit generated this detail object.
*/
void
SoNodeKitDetail::setNodeKit(SoBaseKit * kit)
{
  this->myNodeKit = kit;
  // FIXME: ref() kit? 19991109 mortene.
}

/*!
  Returns a pointer to the nodekit generating this details object.
*/
SoBaseKit *
SoNodeKitDetail::getNodeKit(void) const
{
  return this->myNodeKit;
}

/*!
  Set the pointer indicating which node inside the nodekit was hit
  by a pick.
*/
void
SoNodeKitDetail::setPart(SoNode * part)
{
  this->myPart = part;
  // FIXME: ref() node? 19991109 mortene.
}

/*!
  Return node inside nodekit which was hit.
*/
SoNode *
SoNodeKitDetail::getPart(void) const
{
  return this->myPart;
}

/*!
  Set catalog name of node part which was hit.
*/
void
SoNodeKitDetail::setPartName(const SbName & name)
{
  this->myPartName = name;
}

/*!
  Return catalog name of nodekit part which was hit.
*/
const SbName &
SoNodeKitDetail::getPartName(void) const
{
  return this->myPartName;
}
