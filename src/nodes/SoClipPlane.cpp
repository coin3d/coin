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
  \class SoClipPlane SoClipPlane.h Inventor/nodes/SoClipPlane.h
  \brief The SoClipPlane class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoClipPlane.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/SbName.h>

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
#include <Inventor/actions/SoGLRenderAction.h>
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
#if !defined(COIN_EXCLUDE_SOGLCLIPPLANEELEMENT)
#include <Inventor/elements/SoGLClipPlaneElement.h>
#endif // !COIN_EXCLUDE_SOGLCLIPPLANEELEMENT

#if !defined(COIN_EXCLUDE_SOCLIPPLANEELEMENT)
#include <Inventor/elements/SoClipPlaneElement.h>
#endif

/*!
  \var SoSFPlane SoClipPlane::plane
  FIXME: write documentation for field
*/
/*!
  \var SoSFBool SoClipPlane::on
  FIXME: write documentation for field
*/


// *************************************************************************

//$ BEGIN TEMPLATE NodeSource( SoClipPlane )
SoType SoClipPlane::classTypeId = SoType::badType();

/*!
  Returns a new instance of the SoClipPlane node class.
*/
void *
SoClipPlane::createInstance(void)
{
  return new SoClipPlane;
}

/*!
  Returns the unique type identifier for the SoClipPlane class.
*/
SoType
SoClipPlane::getClassTypeId(void)
{
  return SoClipPlane::classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoClipPlane::getTypeId(void) const
{
  return SoClipPlane::classTypeId;
}
//$ END TEMPLATE NodeSource

/*!
  Constructor.
*/
SoClipPlane::SoClipPlane()
{
//$ BEGIN TEMPLATE NodeConstructor( SoClipPlane )
  // Make sure the class has been initialized.
  assert(SoClipPlane::classTypeId != SoType::badType());
//$ END TEMPLATE NodeConstructor

  SO_NODE_ADD_FIELD(plane, (SbPlane(SbVec3f(1.0f, 0.0f, 0.0f), 0.0f)));
  SO_NODE_ADD_FIELD(on, (TRUE));
}

/*!
  Destructor.
*/
SoClipPlane::~SoClipPlane()
{
}

/*!
  Does initialization common for all objects of the
  SoClipPlane class. This includes setting up the
  type system, among other things.
*/
void
SoClipPlane::initClass(void)
{
//$ BEGIN TEMPLATE InitNodeSource( ClipPlane )
  // Make sure we only initialize once.
  assert(SoClipPlane::classTypeId == SoType::badType());
  // Make sure superclass get initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoClipPlane::classTypeId =
    SoType::createType(inherited::getClassTypeId(), "ClipPlane",
                       &SoClipPlane::createInstance,
                       SoNode::nextActionMethodIndex++);
//$ END TEMPLATE InitNodeSource

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  SO_ENABLE(SoGLRenderAction, SoGLClipPlaneElement);
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoClipPlane::cleanClass(void)
{
}

#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write function documentation
*/
void 
SoClipPlane::doAction(SoAction *action)
{
  SbBool ison = on.isIgnored() ? TRUE : on.getValue();
  if (ison && !plane.isIgnored()) {
    SoClipPlaneElement::add(action->getState(), this, plane.getValue());
  }
}
#endif

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write function documentation
*/
void 
SoClipPlane::GLRender(SoGLRenderAction *action)
{
  SoClipPlane::doAction(action);
}
#endif

#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
/*!
  FIXME: write doc
*/
void
SoClipPlane::callback(SoCallbackAction *action)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOCALLBACKACTION

#if !defined(COIN_EXCLUDE_SOPICKACTION)
/*!
  FIXME: write doc
*/
void
SoClipPlane::pick(SoPickAction *action)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOPICKACTION
