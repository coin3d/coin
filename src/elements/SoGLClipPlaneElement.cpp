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
  \class SoGLClipPlaneElement Inventor/elements/SoGLClipPlaneElement.h
  \brief The SoGLClipPlaneElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoGLClipPlaneElement.h>

#include <Inventor/SbName.h>

#ifdef _WIN32
#include <windows.h>
#endif // !_WIN32

#include <GL/gl.h>

// static variables
int SoGLClipPlaneElement::maxGLPlanes = -1;

//$ BEGIN TEMPLATE ElementSource(SoGLClipPlaneElement)

/*!
  \var SoGLClipPlaneElement::classTypeId

  This is the static class type identifier for the
  SoGLClipPlaneElement class.
*/

SoType SoGLClipPlaneElement::classTypeId = SoType::badType();

/*!
  This method returns the SoType object for the element class of
  the instance.
*/
SoType
SoGLClipPlaneElement::getClassTypeId(void)
{
  return SoGLClipPlaneElement::classTypeId;
}

/*!
  This static method creates an object instance of the SoGLClipPlaneElement class.
*/
void *
SoGLClipPlaneElement::createInstance(void)
{
  return (void *) new SoGLClipPlaneElement;
}

/*!
  \var SoGLClipPlaneElement::classStackIndex

  This is the static state stack index for the
  SoGLClipPlaneElement class.
*/
int SoGLClipPlaneElement::classStackIndex;

/*!
  This static method returns the state stack index for the SoGLClipPlaneElement class.
*/
int
SoGLClipPlaneElement::getClassStackIndex(void)
{
  return SoGLClipPlaneElement::classStackIndex;
}
//$ END TEMPLATE ElementSource

/*!
  This static method initializes static data for the SoGLClipPlaneElement
  class.
*/

void
SoGLClipPlaneElement::initClass(void)
{
//$ BEGIN TEMPLATE InitElementSource(SoGLClipPlaneElement)
  assert(SoGLClipPlaneElement::classTypeId == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoGLClipPlaneElement::classTypeId =
    SoType::createType(inherited::getClassTypeId(),
                       "SoGLClipPlaneElement",
                       &SoGLClipPlaneElement::createInstance);

  if (inherited::classStackIndex < 0) {
    SoGLClipPlaneElement::classStackIndex =
      createStackIndex(SoGLClipPlaneElement::classTypeId);
  }
  else {
    SoGLClipPlaneElement::classStackIndex = inherited::classStackIndex;
  }
//$ END TEMPLATE InitElementSource
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoGLClipPlaneElement::createInstance(void)
*/

SoGLClipPlaneElement::SoGLClipPlaneElement()
{
    setTypeId(SoGLClipPlaneElement::classTypeId);
    setStackIndex(SoGLClipPlaneElement::classStackIndex);
}

/*!
  The destructor.
*/

SoGLClipPlaneElement::~SoGLClipPlaneElement(void)
{
}

//! FIXME: write doc.

void
SoGLClipPlaneElement::init(SoState * state)
{
  inherited::init(state);
}

//! FIXME: write doc.

void
SoGLClipPlaneElement::pop(SoState * state,
                          const SoElement * prevTopElement)
{
  // disable used planes
  for (int i = this->startIndex; i < this->getNum(); i++)
    glDisable((GLenum)((int)GL_CLIP_PLANE0 + i));

  // pop plane data
  inherited::pop(state, prevTopElement);
}

//! FIXME: write doc.

int
SoGLClipPlaneElement::getMaxGLPlanes(void)
{
  if (SoGLClipPlaneElement::maxGLPlanes == -1) {
    // NB: don't try to be clever and move this code to the
    // initClass() method, as it won't work -- the GL variables may
    // not have been initialized yet when it's called. --mortene
    GLint val;
    glGetIntegerv(GL_MAX_CLIP_PLANES, &val);
    SoGLClipPlaneElement::maxGLPlanes = val;
  }

  return SoGLClipPlaneElement::maxGLPlanes;
}

//! FIXME: write doc.

void
SoGLClipPlaneElement::addToElt(const SbPlane & plane,
                               const SbMatrix & modelMatrix)
{
  int idxadd = getNum(); // num planes before this one
  inherited::addToElt(plane, modelMatrix); // store plane
  SbVec3f norm = plane.getNormal();
  double equation[4];
  equation[0] = norm[0];
  equation[1] = norm[1];
  equation[2] = norm[2];
  equation[3] = plane.getDistanceFromOrigin();
  glClipPlane((GLenum)((int)GL_CLIP_PLANE0 + idxadd), equation);
  glEnable((GLenum)((int)GL_CLIP_PLANE0 + idxadd));
}
