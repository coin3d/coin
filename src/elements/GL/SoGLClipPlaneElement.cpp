/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2005 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org/> for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

/*!
  \class SoGLClipPlaneElement Inventor/elements/SoGLClipPlaneElement.h
  \brief The SoGLClipPlaneElement class is yet to be documented.
  \ingroup elements

  FIXME: write doc.
*/

#include <Inventor/elements/SoGLClipPlaneElement.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/system/gl.h>
#include <Inventor/errors/SoDebugError.h>

// *************************************************************************

SO_ELEMENT_SOURCE(SoGLClipPlaneElement);

// *************************************************************************

/*!
  This static method initializes static data for the SoGLClipPlaneElement
  class.
*/

void
SoGLClipPlaneElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoGLClipPlaneElement, inherited);
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
  this->capture(state);
  const SoGLClipPlaneElement * prev = (const SoGLClipPlaneElement*)
    prevTopElement;

  // disable used planes
  for (int i = prev->startIndex; i < prev->getNum(); i++)
    glDisable((GLenum)((int)GL_CLIP_PLANE0 + i));
  
  inherited::pop(state, prevTopElement);
}

//! FIXME: write doc.

int
SoGLClipPlaneElement::getMaxGLPlanes(void)
{
  // FIXME: should also make a likewise method available as part of
  // the So*GLWidget classes. 20020802 mortene.

  SoDebugError::postWarning("SoGLClipPlaneElement::getMaxGLPlanes",
                            "This function is obsoleted. It should not "
                            "be used because its interface is fubar: "
                            "the number of clip planes available from "
                            "the OpenGL driver depends on the context, and "
                            "this function does not know which context this "
                            "information is requested for.");

  GLint val;
  glGetIntegerv(GL_MAX_CLIP_PLANES, &val);

  assert(glGetError() == GL_NO_ERROR &&
         "GL error when calling glGetInteger() -- no current GL context?");

  return (int)val;
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
  equation[3] = - plane.getDistanceFromOrigin();
  glClipPlane((GLenum)((int)GL_CLIP_PLANE0 + idxadd), equation);
  glEnable((GLenum)((int)GL_CLIP_PLANE0 + idxadd));
}
