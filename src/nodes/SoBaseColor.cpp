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
  \class SoBaseColor SoBaseColor.h Inventor/nodes/SoBaseColor.h
  \brief The SoBaseColor class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoBaseColor.h>
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
  \var SoMFColor SoBaseColor::rgb
  FIXME: write documentation for field
*/

// *************************************************************************

//$ BEGIN TEMPLATE NodeSource( SoBaseColor )
SoType SoBaseColor::classTypeId = SoType::badType();

/*!
  Returns a new instance of the SoBaseColor node class.
*/
void *
SoBaseColor::createInstance(void)
{
  return new SoBaseColor;
}

/*!
  Returns the unique type identifier for the SoBaseColor class.
*/
SoType
SoBaseColor::getClassTypeId(void)
{
  return SoBaseColor::classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoBaseColor::getTypeId(void) const
{
  return SoBaseColor::classTypeId;
}
//$ END TEMPLATE NodeSource

/*!
  Constructor.
*/
SoBaseColor::SoBaseColor()
{
//$ BEGIN TEMPLATE NodeConstructor( SoBaseColor )
  // Make sure the class has been initialized.
  assert(SoBaseColor::classTypeId != SoType::badType());
//$ END TEMPLATE NodeConstructor
  
  SO_NODE_ADD_FIELD(rgb, (SbColor(0.8f, 0.8f, 0.8f)));
}

/*!
  Destructor.
*/
SoBaseColor::~SoBaseColor()
{
}

/*!
  Does initialization common for all objects of the
  SoBaseColor class. This includes setting up the
  type system, among other things.
*/
void
SoBaseColor::initClass(void)
{
//$ BEGIN TEMPLATE InitNodeSource( BaseColor )
  // Make sure we only initialize once.
  assert(SoBaseColor::classTypeId == SoType::badType());
  // Make sure superclass get initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoBaseColor::classTypeId =
    SoType::createType(inherited::getClassTypeId(), "BaseColor",
                       &SoBaseColor::createInstance,
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
SoBaseColor::cleanClass(void)
{
}

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write function documentation
*/
void 
SoBaseColor::GLRender(SoGLRenderAction * action)
{
  SoState * state = action->getState();
  if (!rgb.isIgnored()) {
    SoGLDiffuseColorElement::set(action->getState(),
				 this,
				 rgb.getNum(),
				 rgb.getValues(0));
  }
}
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_DOACTION)
/*!
  FIXME: write doc
*/
void
SoBaseColor::doAction(SoAction * action)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_DOACTION

#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
/*!
  FIXME: write doc
*/
void
SoBaseColor::callback(SoCallbackAction * action)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOCALLBACKACTION
