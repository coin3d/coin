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
  \class SoTextureCoordinate2 SoTextureCoordinate2.h Inventor/nodes/SoTextureCoordinate2.h
  \brief The SoTextureCoordinate2 class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/SbName.h>
#include <Inventor/nodes/SoTextureCoordinate2.h>
#include <Inventor/nodes/SoSubNode.h>

#if !defined(COIN_EXCLUDE_SOACTION)
#include <Inventor/actions/SoAction.h>
#endif // !COIN_EXCLUDE_SOACTION
#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
#include <Inventor/actions/SoGLRenderAction.h>
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SOGLTEXTURECOORDINATEELEMENT)
#include <Inventor/elements/SoGLTextureCoordinateElement.h>
#endif // !COIN_EXCLUDE_SOGLTEXTURECOORDINATEELEMENT
#if !defined(COIN_EXCLUDE_SOTEXTURECOORDINATEELEMENT)
#include <Inventor/elements/SoTextureCoordinateElement.h>
#endif // !COIN_EXCLUDE_SOTEXTURECOORDINATEELEMENT

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
#include <Inventor/elements/SoGLTextureCoordinateElement.h>
#endif // ! COIN_EXCLUDE_SOGLRENDERACTION

/*!
  \var SoMFVec2f SoTextureCoordinate2::point
  FIXME: write documentation for field
*/

// *************************************************************************

//$ BEGIN TEMPLATE NodeSource(SoTextureCoordinate2)
SoType SoTextureCoordinate2::classTypeId = SoType::badType();

/*!
  Returns a new instance of the SoTextureCoordinate2 node class.
*/
void *
SoTextureCoordinate2::createInstance(void)
{
  return new SoTextureCoordinate2;
}

/*!
  Returns the unique type identifier for the SoTextureCoordinate2 class.
*/
SoType
SoTextureCoordinate2::getClassTypeId(void)
{
  return SoTextureCoordinate2::classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoTextureCoordinate2::getTypeId(void) const
{
  return SoTextureCoordinate2::classTypeId;
}
//$ END TEMPLATE NodeSource

/*!
  Constructor.
*/
SoTextureCoordinate2::SoTextureCoordinate2()
{
//$ BEGIN TEMPLATE NodeConstructor(SoTextureCoordinate2)
  // Make sure the class has been initialized.
  assert(SoTextureCoordinate2::classTypeId != SoType::badType());
//$ END TEMPLATE NodeConstructor
  SO_NODE_ADD_FIELD(point, (NULL));
}

/*!
  Destructor.
*/
SoTextureCoordinate2::~SoTextureCoordinate2()
{
}

/*!
  Does initialization common for all objects of the
  SoTextureCoordinate2 class. This includes setting up the
  type system, among other things.
*/
void
SoTextureCoordinate2::initClass(void)
{
//$ BEGIN TEMPLATE InitNodeSource(TextureCoordinate2)
  // Make sure we only initialize once.
  assert(SoTextureCoordinate2::classTypeId == SoType::badType());
  // Make sure superclass get initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoTextureCoordinate2::classTypeId =
    SoType::createType(inherited::getClassTypeId(), "TextureCoordinate2",
                       &SoTextureCoordinate2::createInstance,
                       SoNode::nextActionMethodIndex++);
//$ END TEMPLATE InitNodeSource

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  SO_ENABLE(SoGLRenderAction, SoGLTextureCoordinateElement);
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoTextureCoordinate2::cleanClass(void)
{
}

#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write function documentation
*/
void 
SoTextureCoordinate2::doAction(SoAction * action)
{
  SoTextureCoordinateElement::set2(action->getState(), this,
				   point.getNum(),
				   point.getValues(0));
}
#endif // !COIN_EXCLUDE_SOACTION

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write function documentation
*/
void 
SoTextureCoordinate2::GLRender(SoGLRenderAction * action)
{
  SoGLTextureCoordinateElement::setTexGen(action->getState(),
					  this, NULL);
  SoTextureCoordinate2::doAction((SoAction *)action);
}
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
/*!
  FIXME: write function documentation
*/
void 
SoTextureCoordinate2::callback(SoCallbackAction * action)
{
  SoTextureCoordinate2::doAction((SoAction *)action);
}
#endif // !COIN_EXCLUDE_SOCALLBACKACTION

#if !defined(COIN_EXCLUDE_SOPICKACTION)
/*!
  FIXME: write function documentation
*/
void 
SoTextureCoordinate2::pick(SoPickAction * action)
{
  SoTextureCoordinate2::doAction((SoAction *)action);
}
#endif // !COIN_EXCLUDE_SOPICKACTION
