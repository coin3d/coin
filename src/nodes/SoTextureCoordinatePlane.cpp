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
  \class SoTextureCoordinatePlane SoTextureCoordinatePlane.h Inventor/nodes/SoTextureCoordinatePlane.h
  \brief The SoTextureCoordinatePlane class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoTextureCoordinatePlane.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/SbName.h>

#if !defined(COIN_EXCLUDE_SOTEXTURECOORDINATEELEMENT)
#include <Inventor/elements/SoTextureCoordinateElement.h>
#endif

#if !defined(COIN_EXCLUDE_SOACTION)
#include <Inventor/actions/SoGLRenderAction.h>
#endif

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
#include <Inventor/elements/SoGLTextureCoordinateElement.h>
#ifdef _WIN32
#include <windows.h>
#endif // _WIN32
#include <GL/gl.h>
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

/*!
  \var SoSFVec3f SoTextureCoordinatePlane::directionS
  FIXME: write documentation for field
*/
/*!
  \var SoSFVec3f SoTextureCoordinatePlane::directionT
  FIXME: write documentation for field
*/

// *************************************************************************

//$ BEGIN TEMPLATE NodeSource(SoTextureCoordinatePlane)
SoType SoTextureCoordinatePlane::classTypeId = SoType::badType();

/*!
  Returns a new instance of the SoTextureCoordinatePlane node class.
*/
void *
SoTextureCoordinatePlane::createInstance(void)
{
  return new SoTextureCoordinatePlane;
}

/*!
  Returns the unique type identifier for the SoTextureCoordinatePlane class.
*/
SoType
SoTextureCoordinatePlane::getClassTypeId(void)
{
  return SoTextureCoordinatePlane::classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoTextureCoordinatePlane::getTypeId(void) const
{
  return SoTextureCoordinatePlane::classTypeId;
}
//$ END TEMPLATE NodeSource

/*!
  Constructor.
*/
SoTextureCoordinatePlane::SoTextureCoordinatePlane()
{
//$ BEGIN TEMPLATE NodeConstructor(SoTextureCoordinatePlane)
  // Make sure the class has been initialized.
  assert(SoTextureCoordinatePlane::classTypeId != SoType::badType());
//$ END TEMPLATE NodeConstructor

  SO_NODE_ADD_FIELD(directionS, (1.0f, 0.0f, 0.0f));
  SO_NODE_ADD_FIELD(directionT, (0.0f, 1.0f, 0.0f));
}

/*!
  Destructor.
*/
SoTextureCoordinatePlane::~SoTextureCoordinatePlane()
{
}

/*!
  Does initialization common for all objects of the
  SoTextureCoordinatePlane class. This includes setting up the
  type system, among other things.
*/
void
SoTextureCoordinatePlane::initClass(void)
{
//$ BEGIN TEMPLATE InitNodeSource(TextureCoordinatePlane)
  // Make sure we only initialize once.
  assert(SoTextureCoordinatePlane::classTypeId == SoType::badType());
  // Make sure superclass get initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoTextureCoordinatePlane::classTypeId =
    SoType::createType(inherited::getClassTypeId(), "TextureCoordinatePlane",
                       &SoTextureCoordinatePlane::createInstance,
                       SoNode::nextActionMethodIndex++);
//$ END TEMPLATE InitNodeSource
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoTextureCoordinatePlane::cleanClass(void)
{
}

/*!
  FIXME: write function documentation
*/
const SbVec4f &
SoTextureCoordinatePlane::generate(void * userdata,
				   const SbVec3f & p,
				   const SbVec3f & n)
{
  assert(0 && "FIXME: not implemented yet");
  return SbVec4f(0,0,0,1);
}

#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write function documentation
*/
void 
SoTextureCoordinatePlane::doAction(SoAction * action)
{
  SoTextureCoordinateElement::setFunction(action->getState(), this,
					  generate,
					  this);
}
#endif // !COIN_EXCLUDE_SOACTION

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write function documentation
*/
void 
SoTextureCoordinatePlane::GLRender(SoGLRenderAction * action)
{
  SoTextureCoordinatePlane::doAction((SoAction *)action);
  SoGLTextureCoordinateElement::setTexGen(action->getState(),
					  this, handleTexgen, this);
}
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
/*!
  FIXME: write function documentation
*/
void 
SoTextureCoordinatePlane::callback(SoCallbackAction * action)
{
  SoTextureCoordinatePlane::doAction((SoAction *)action);
}
#endif // !COIN_EXCLUDE_SOCALLBACKACTION

#if !defined(COIN_EXCLUDE_SOPICKACTION)
/*!
  FIXME: write function documentation
*/
void 
SoTextureCoordinatePlane::pick(SoPickAction * action)
{
  SoTextureCoordinatePlane::doAction((SoAction *)action);
}
#endif // !COIN_EXCLUDE_SOPICKACTION

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
void 
SoTextureCoordinatePlane::handleTexgen(void *data)
{
  SoTextureCoordinatePlane *thisp = (SoTextureCoordinatePlane*)data;
  glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
  glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);

  float plane[4];
  plane[3] = 0.0f;
  const SbVec3f &s = thisp->directionS.getValue();
  plane[0] = s[0];
  plane[1] = s[1];
  plane[2] = s[2];
  glTexGenfv(GL_S, GL_OBJECT_PLANE, plane);
  const SbVec3f &t = thisp->directionT.getValue();
  plane[0] = t[0];
  plane[1] = t[1];
  plane[2] = t[2];
  glTexGenfv(GL_T, GL_OBJECT_PLANE, plane);

  glEnable(GL_TEXTURE_GEN_S);
  glEnable(GL_TEXTURE_GEN_T);
}
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

