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
  \class SoTransformation SoTransformation.h Inventor/nodes/SoTransformation.h
  \brief The SoTransformation class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoTransformation.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/SbName.h>

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
#include <Inventor/actions/SoGLRenderAction.h>
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION
#if !defined(COIN_EXCLUDE_SOPICKACTION)
#include <Inventor/actions/SoPickAction.h>
#endif // !COIN_EXCLUDE_SOPICKACTION

#if !defined(COIN_EXCLUDE_SOMODELMATRIXELEMENT)
#include <Inventor/elements/SoModelMatrixElement.h>
#endif // !COIN_EXCLUDE_SOMODELMATRIXELEMENT
#if !defined(COIN_EXCLUDE_SOGLMODELMATRIXELEMENT)
#include <Inventor/elements/SoGLModelMatrixElement.h>
#endif // !COIN_EXCLUDE_SOMODELMATRIXELEMENT
#if !defined(COIN_EXCLUDE_SOBBOXMODELMATRIXELEMENT)
#include <Inventor/elements/SoBBoxModelMatrixElement.h>
#endif // !COIN_EXCLUDE_SOBBOXMODELMATRIXELEMENT
#if !defined(COIN_EXCLUDE_SOLOCALBBOXMATRIXELEMENT)
#include <Inventor/elements/SoLocalBBoxMatrixElement.h>
#endif // !COIN_EXCLUDE_SOLOCALBBOXMATRIXELEMENT


//$ BEGIN TEMPLATE NodeAbstractSource(SoTransformation)
SoType SoTransformation::classTypeId = SoType::badType();

/*!
  Returns the unique type identifier for the SoTransformation class.
*/
SoType
SoTransformation::getClassTypeId(void)
{
  return SoTransformation::classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoTransformation::getTypeId(void) const
{
  return SoTransformation::classTypeId;
}
//$ END TEMPLATE NodeAbstractSource

/*!
  Constructor.
*/
SoTransformation::SoTransformation()
{
//$ BEGIN TEMPLATE NodeConstructor(SoTransformation)
  // Make sure the class has been initialized.
  assert(SoTransformation::classTypeId != SoType::badType());
//$ END TEMPLATE NodeConstructor
}

/*!
  Destructor.
*/
SoTransformation::~SoTransformation()
{
}


/*!
  Does initialization common for all objects of the
  SoTransformation class. This includes setting up the
  type system, among other things.
*/
void
SoTransformation::initClass(void)
{
//$ BEGIN TEMPLATE InitNodeAbstractSource(Transformation)
  // Make sure we only initialize once.
  assert(SoTransformation::classTypeId == SoType::badType());
  // Make sure superclass get initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoTransformation::classTypeId =
    SoType::createType(inherited::getClassTypeId(), "Transformation",
                       NULL,
                       SoNode::nextActionMethodIndex++);
//$ END TEMPLATE InitNodeAbstractSource

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
  SO_ENABLE(SoGetBoundingBoxAction, SoModelMatrixElement);
  SO_ENABLE(SoGetBoundingBoxAction, SoBBoxModelMatrixElement);
  SO_ENABLE(SoGetBoundingBoxAction, SoLocalBBoxMatrixElement);
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION
#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  SO_ENABLE(SoGLRenderAction, SoGLModelMatrixElement);
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
#if !defined(COIN_EXCLUDE_SOPICKACTION)
  SO_ENABLE(SoPickAction, SoModelMatrixElement);
#endif // !COIN_EXCLUDE_SOPICKACTION
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoTransformation::cleanClass(void)
{
}
