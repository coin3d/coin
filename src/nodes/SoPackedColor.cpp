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
  \class SoPackedColor SoPackedColor.h Inventor/nodes/SoPackedColor.h
  \brief The SoPackedColor class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoPackedColor.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/SbName.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
#include <Inventor/actions/SoGLRenderAction.h>
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
#if !defined(COIN_EXCLUDE_SOGLDIFFUSECOLORELEMENT)
#include <Inventor/elements/SoGLDiffuseColorElement.h>
#endif // !COIN_EXCLUDE_SOGLDIFFUSECOLORELEMENT

/*!
  \var SoMFUInt32 SoPackedColor::orderedRGBA
  FIXME: write documentation for field
*/

// *************************************************************************

//$ BEGIN TEMPLATE NodeSource(SoPackedColor)
SoType SoPackedColor::classTypeId = SoType::badType();

/*!
  Returns a new instance of the SoPackedColor node class.
*/
void *
SoPackedColor::createInstance(void)
{
  return new SoPackedColor;
}

/*!
  Returns the unique type identifier for the SoPackedColor class.
*/
SoType
SoPackedColor::getClassTypeId(void)
{
  return SoPackedColor::classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoPackedColor::getTypeId(void) const
{
  return SoPackedColor::classTypeId;
}
//$ END TEMPLATE NodeSource

/*!
  Constructor.
*/
SoPackedColor::SoPackedColor()
{
//$ BEGIN TEMPLATE NodeConstructor(SoPackedColor)
  // Make sure the class has been initialized.
  assert(SoPackedColor::classTypeId != SoType::badType());
//$ END TEMPLATE NodeConstructor
  
  SO_NODE_ADD_FIELD(orderedRGBA, (0xccccccff));
}

/*!
  Destructor.
*/
SoPackedColor::~SoPackedColor()
{
}

/*!
  Does initialization common for all objects of the
  SoPackedColor class. This includes setting up the
  type system, among other things.
*/
void
SoPackedColor::initClass(void)
{
//$ BEGIN TEMPLATE InitNodeSource(PackedColor)
  // Make sure we only initialize once.
  assert(SoPackedColor::classTypeId == SoType::badType());
  // Make sure superclass get initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoPackedColor::classTypeId =
    SoType::createType(inherited::getClassTypeId(), "PackedColor",
                       &SoPackedColor::createInstance,
                       SoNode::nextActionMethodIndex++);
//$ END TEMPLATE InitNodeSource

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  SO_ENABLE(SoGLRenderAction, SoGLDiffuseColorElement);
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoPackedColor::cleanClass(void)
{
}

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write function documentation
*/
void 
SoPackedColor::GLRender(SoGLRenderAction * action)
{
  if (!orderedRGBA.isIgnored()) {
    SoDiffuseColorElement::set(action->getState(),
			       this,
			       orderedRGBA.getNum(),
			       orderedRGBA.getValues(0));
  }
}
#endif // !COIN_EXCLUDE_SOGLRENDERACTION


#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write doc
 */
void
SoPackedColor::doAction(SoAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOACTION

#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
/*!
  FIXME: write doc
 */
void
SoPackedColor::callback(SoCallbackAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOCALLBACKACTION

/*!
  FIXME: write doc
 */
SbBool
SoPackedColor::isTransparent(void)
{
  assert(0 && "FIXME: not implemented");
  return FALSE;
}
