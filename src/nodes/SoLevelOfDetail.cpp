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
  \class SoLevelOfDetail SoLevelOfDetail.h Inventor/nodes/SoLevelOfDetail.h
  \brief The SoLevelOfDetail class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/SbName.h>
#include <Inventor/nodes/SoLevelOfDetail.h>
#include <Inventor/nodes/SoSubNode.h>

/*!
  \var SoMFFloat SoLevelOfDetail::screenArea
  FIXME: write documentation for field
*/

// *************************************************************************

//$ BEGIN TEMPLATE NodeSource(SoLevelOfDetail)
SoType SoLevelOfDetail::classTypeId = SoType::badType();

/*!
  Returns a new instance of the SoLevelOfDetail node class.
*/
void *
SoLevelOfDetail::createInstance(void)
{
  return new SoLevelOfDetail;
}

/*!
  Returns the unique type identifier for the SoLevelOfDetail class.
*/
SoType
SoLevelOfDetail::getClassTypeId(void)
{
  return SoLevelOfDetail::classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoLevelOfDetail::getTypeId(void) const
{
  return SoLevelOfDetail::classTypeId;
}
//$ END TEMPLATE NodeSource

/*!
  Constructor.
*/
SoLevelOfDetail::SoLevelOfDetail()
{
//$ BEGIN TEMPLATE NodeConstructor(SoLevelOfDetail)
  // Make sure the class has been initialized.
  assert(SoLevelOfDetail::classTypeId != SoType::badType());
//$ END TEMPLATE NodeConstructor

  SO_NODE_ADD_FIELD(screenArea, (0));
}

/*!
  Destructor.
*/
SoLevelOfDetail::~SoLevelOfDetail()
{
}

/*!
  Does initialization common for all objects of the
  SoLevelOfDetail class. This includes setting up the
  type system, among other things.
*/
void
SoLevelOfDetail::initClass(void)
{
//$ BEGIN TEMPLATE InitNodeSource(LevelOfDetail)
  // Make sure we only initialize once.
  assert(SoLevelOfDetail::classTypeId == SoType::badType());
  // Make sure superclass get initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoLevelOfDetail::classTypeId =
    SoType::createType(inherited::getClassTypeId(), "LevelOfDetail",
                       &SoLevelOfDetail::createInstance,
                       SoNode::nextActionMethodIndex++);
//$ END TEMPLATE InitNodeSource
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoLevelOfDetail::cleanClass(void)
{
}


#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write doc
 */
void
SoLevelOfDetail::doAction(SoAction * action)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOACTION

#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
/*!
  FIXME: write doc
 */
void
SoLevelOfDetail::callback(SoCallbackAction * action)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOCALLBACKACTION

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write doc
 */
void
SoLevelOfDetail::GLRender(SoGLRenderAction * action)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SORAYPICKACTION)
/*!
  FIXME: write doc
 */
void
SoLevelOfDetail::rayPick(SoRayPickAction * action)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SORAYPICKACTION
