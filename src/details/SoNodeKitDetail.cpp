/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
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
#include <Inventor/nodekits/SoBaseKit.h>

/*!
  \var SoBaseKit * SoNodeKitDetail::myNodeKit
  The nodekit generating this details object.
*/
/*!
  \var SoNode * SoNodeKitDetail::myPart
  Node inside nodekit which was hit.
*/
/*!
  \var SbName SoNodeKitDetail::myPartName
  Catalog name of nodekit part which was hit.
*/


SO_DETAIL_SOURCE(SoNodeKitDetail);

/*!
  Constructor.
*/
SoNodeKitDetail::SoNodeKitDetail(void)
  : myNodeKit(NULL), myPart(NULL), myPartName("")
{
}

/*!
  Destructor.
*/
SoNodeKitDetail::~SoNodeKitDetail()
{
  if (this->myNodeKit) myNodeKit->unref();
  if (this->myPart) myPart->unref();
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
  copy->myNodeKit = this->myNodeKit;
  copy->myPart = this->myPart;
  copy->myPartName = this->myPartName;

  // got to ref once more if set
  if (this->myNodeKit) this->myNodeKit->ref();
  if (this->myPart) this->myPart->ref();

  return copy;
}

/*!
  Set the pointer indicating which nodekit generated this detail object.
*/
void
SoNodeKitDetail::setNodeKit(SoBaseKit * kit)
{
  if (this->myNodeKit) this->myNodeKit->unref();
  this->myNodeKit = kit;
  if (kit) kit->ref();
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
  if (this->myPart) this->myPart->unref();
  this->myPart = part;
  if (part) part->ref();
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
