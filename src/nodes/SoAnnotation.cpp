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
  \class SoAnnotation SoAnnotation.h Inventor/nodes/SoAnnotation.h
  \brief The SoAnnotation node draws all its children geometry on top of other geometry.
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoAnnotation.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/SbName.h>

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
#include <Inventor/actions/SoGLRenderAction.h>
#include <GL/gl.h>
#endif // !COIN_EXCLUDE_SOGLRENDERACTION



// *************************************************************************

//$ BEGIN TEMPLATE NodeSource( SoAnnotation )
SoType SoAnnotation::classTypeId = SoType::badType();

/*!
  Returns a new instance of the SoAnnotation node class.
*/
void *
SoAnnotation::createInstance(void)
{
  return new SoAnnotation;
}

/*!
  Returns the unique type identifier for the SoAnnotation class.
*/
SoType
SoAnnotation::getClassTypeId(void)
{
  return SoAnnotation::classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoAnnotation::getTypeId(void) const
{
  return SoAnnotation::classTypeId;
}
//$ END TEMPLATE NodeSource

/*!
  Constructor.
*/
SoAnnotation::SoAnnotation()
{
//$ BEGIN TEMPLATE NodeConstructor( SoAnnotation )
  // Make sure the class has been initialized.
  assert(SoAnnotation::classTypeId != SoType::badType());
//$ END TEMPLATE NodeConstructor
}

/*!
  Destructor.
*/
SoAnnotation::~SoAnnotation()
{
}

/*!
  Does initialization common for all objects of the
  SoAnnotation class. This includes setting up the
  type system, among other things.
*/
void
SoAnnotation::initClass(void)
{
//$ BEGIN TEMPLATE InitNodeSource( Annotation )
  // Make sure we only initialize once.
  assert(SoAnnotation::classTypeId == SoType::badType());
  // Make sure superclass get initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoAnnotation::classTypeId =
    SoType::createType(inherited::getClassTypeId(), "Annotation",
                       &SoAnnotation::createInstance,
                       SoNode::nextActionMethodIndex++);
//$ END TEMPLATE InitNodeSource
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoAnnotation::cleanClass(void)
{
}

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write function documentation
*/
void 
SoAnnotation::GLRender(SoGLRenderAction * action)
{
#if !defined(COIN_EXCLUDE_SOPATH)
  if (action->isRenderingDelayedPaths()) {
    inherited::GLRender(action);
  }
  else {
    action->addDelayedPath((SoPath*)action->getCurPath());
  }
#else //  COIN_EXCLUDE_SOPATH
  inherited::GLRender(action);
#endif // COIN_EXCLUDE_SOPATH
}

/*!
  FIXME: write doc
*/
void
SoAnnotation::GLRenderBelowPath(SoGLRenderAction * action)
{
#if !defined(COIN_EXCLUDE_SOPATH)
  if (action->isRenderingDelayedPaths()) {
    inherited::GLRenderBelowPath(action);
  }
#else // COIN_EXCLUDE_SOPATH
  inherited::GLRenderInPath(action);
#endif // COIN_EXCLUDE_SOPATH  
}

/*!
  FIXME: write doc
*/
void
SoAnnotation::GLRenderInPath(SoGLRenderAction * action)
{
#if !defined(COIN_EXCLUDE_SOPATH)
  if (action->isRenderingDelayedPaths()) {
    inherited::GLRenderInPath(action);
  }
#else // COIN_EXCLUDE_SOPATH
  inherited::GLRenderInPath(action);
#endif // COIN_EXCLUDE_SOPATH
}

/*!
  FIXME: write doc
*/
void
SoAnnotation::GLRenderOffPath(SoGLRenderAction *)
{
  // should never render
}

#endif // !COIN_EXCLUDE_SOGLRENDERACTION
