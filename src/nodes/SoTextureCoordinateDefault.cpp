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
  \class SoTextureCoordinateDefault SoTextureCoordinateDefault.h Inventor/nodes/SoTextureCoordinateDefault.h
  \brief The SoTextureCoordinateDefault class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoTextureCoordinateDefault.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/SbName.h>

#if !defined(COIN_EXCLUDE_SOTEXTURECOORDINATEELEMENT)
#include <Inventor/elements/SoTextureCoordinateElement.h>
#endif
#if !defined(COIN_EXCLUDE_SOACTION)
#include <Inventor/actions/SoAction.h>
#endif // !COIN_EXCLUDE_SOACTION

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLTextureCoordinateElement.h>
#endif // ! COIN_EXCLUDE_SOGLRENDERACTION

// *************************************************************************

//$ BEGIN TEMPLATE NodeSource(SoTextureCoordinateDefault)
SoType SoTextureCoordinateDefault::classTypeId = SoType::badType();

/*!
  Returns a new instance of the SoTextureCoordinateDefault node class.
*/
void *
SoTextureCoordinateDefault::createInstance(void)
{
  return new SoTextureCoordinateDefault;
}

/*!
  Returns the unique type identifier for the SoTextureCoordinateDefault class.
*/
SoType
SoTextureCoordinateDefault::getClassTypeId(void)
{
  return SoTextureCoordinateDefault::classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoTextureCoordinateDefault::getTypeId(void) const
{
  return SoTextureCoordinateDefault::classTypeId;
}
//$ END TEMPLATE NodeSource

/*!
  Constructor.
*/
SoTextureCoordinateDefault::SoTextureCoordinateDefault()
{
//$ BEGIN TEMPLATE NodeConstructor(SoTextureCoordinateDefault)
  // Make sure the class has been initialized.
  assert(SoTextureCoordinateDefault::classTypeId != SoType::badType());
//$ END TEMPLATE NodeConstructor
}

/*!
  Destructor.
*/
SoTextureCoordinateDefault::~SoTextureCoordinateDefault()
{
}

/*!
  Does initialization common for all objects of the
  SoTextureCoordinateDefault class. This includes setting up the
  type system, among other things.
*/
void
SoTextureCoordinateDefault::initClass(void)
{
//$ BEGIN TEMPLATE InitNodeSource(TextureCoordinateDefault)
  // Make sure we only initialize once.
  assert(SoTextureCoordinateDefault::classTypeId == SoType::badType());
  // Make sure superclass get initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoTextureCoordinateDefault::classTypeId =
    SoType::createType(inherited::getClassTypeId(), "TextureCoordinateDefault",
                       &SoTextureCoordinateDefault::createInstance,
                       SoNode::nextActionMethodIndex++);
//$ END TEMPLATE InitNodeSource
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoTextureCoordinateDefault::cleanClass(void)
{
}

#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write function documentation
*/
void 
SoTextureCoordinateDefault::doAction(SoAction * action)
{
  SoTextureCoordinateElement::setDefault(action->getState(), this);
}
#endif // !COIN_EXCLUDE_SOACTION

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write function documentation
*/
void 
SoTextureCoordinateDefault::GLRender(SoGLRenderAction * action)
{
  SoTextureCoordinateDefault::doAction((SoAction *)action);
  SoGLTextureCoordinateElement::setTexGen(action->getState(),
					  this, NULL);
}
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
/*!
  FIXME: write function documentation
*/
void 
SoTextureCoordinateDefault::callback(SoCallbackAction * action)
{
  SoTextureCoordinateDefault::doAction((SoAction *)action);
}
#endif // !COIN_EXCLUDE_SOCALLBACKACTION

#if !defined(COIN_EXCLUDE_SOPICKACTION)
/*!
  FIXME: write function documentation
*/
void 
SoTextureCoordinateDefault::pick(SoPickAction * action)
{
  SoTextureCoordinateDefault::doAction((SoAction *)action);
}
#endif // !COIN_EXCLUDE_SOPICKACTION
