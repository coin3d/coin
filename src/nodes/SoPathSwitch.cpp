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
 *  http://www.sim.no/ sales@sim.no Voice: +47 73540378 Fax: +47 73943861
 *
\**************************************************************************/

/*!
  \class SoPathSwitch SoPathSwitch.h Inventor/nodes/SoPathSwitch.h
  \brief The SoPathSwitch class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/SbName.h>
#include <Inventor/nodes/SoPathSwitch.h>
#include <Inventor/nodes/SoSubNode.h>

/*!
  \var SoSFPath SoPathSwitch::path
  FIXME: write documentation for field
*/

// *************************************************************************

//$ BEGIN TEMPLATE NodeSource(SoPathSwitch)
SoType SoPathSwitch::classTypeId = SoType::badType();

/*!
  Returns a new instance of the SoPathSwitch node class.
*/
void *
SoPathSwitch::createInstance(void)
{
  return new SoPathSwitch;
}

/*!
  Returns the unique type identifier for the SoPathSwitch class.
*/
SoType
SoPathSwitch::getClassTypeId(void)
{
  return SoPathSwitch::classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoPathSwitch::getTypeId(void) const
{
  return SoPathSwitch::classTypeId;
}
//$ END TEMPLATE NodeSource

/*!
  Constructor.
*/
SoPathSwitch::SoPathSwitch()
{
//$ BEGIN TEMPLATE NodeConstructor(SoPathSwitch)
  // Make sure the class has been initialized.
  assert(SoPathSwitch::classTypeId != SoType::badType());
//$ END TEMPLATE NodeConstructor

  SO_NODE_ADD_FIELD(path, (NULL));
}

/*!
  Destructor.
*/
SoPathSwitch::~SoPathSwitch()
{
}

/*!
  Does initialization common for all objects of the
  SoPathSwitch class. This includes setting up the
  type system, among other things.
*/
void
SoPathSwitch::initClass(void)
{
//$ BEGIN TEMPLATE InitNodeSource(PathSwitch)
  // Make sure we only initialize once.
  assert(SoPathSwitch::classTypeId == SoType::badType());
  // Make sure superclass get initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoPathSwitch::classTypeId =
    SoType::createType(inherited::getClassTypeId(), "PathSwitch",
                       &SoPathSwitch::createInstance,
                       SoNode::nextActionMethodIndex++);
//$ END TEMPLATE InitNodeSource
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoPathSwitch::cleanClass(void)
{
}

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
/*!
  FIXME: write function documentation
*/
void
SoPathSwitch::getBoundingBox(SoGetBoundingBoxAction * action)
{
  assert(0 && "FIXME: not implemented yet");
}
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION


#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write doc
 */
void
SoPathSwitch::doAction(SoAction * action)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOACTION

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write doc
 */
void
SoPathSwitch::GLRender(SoGLRenderAction * action)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SOPICKACTION)
/*!
  FIXME: write doc
 */
void
SoPathSwitch::pick(SoPickAction * action)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOPICKACTION

#if !defined(COIN_EXCLUDE_SOHANDLEEVENTACTION)
/*!
  FIXME: write doc
 */
void
SoPathSwitch::handleEvent(SoHandleEventAction * action)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOHANDLEEVENTACTION

#if !defined(COIN_EXCLUDE_SOGETMATRIXACTION)
/*!
  FIXME: write doc
 */
void
SoPathSwitch::getMatrix(SoGetMatrixAction * action)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOGETMATRIXACTION

#if !defined(COIN_EXCLUDE_SOSEARCHACTION)
/*!
  FIXME: write doc
 */
void
SoPathSwitch::search(SoSearchAction * action)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOSEARCHACTION

#if !defined(COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION)
/*!
  FIXME: write doc
 */
void
SoPathSwitch::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION
