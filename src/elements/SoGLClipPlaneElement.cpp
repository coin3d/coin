/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

/*!
  \class SoGLClipPlaneElement Inventor/elements/SoGLClipPlaneElement.h
  \brief The SoGLClipPlaneElement class is yet to be documented.
  \ingroup elements

  FIXME: write doc.
*/

#include <Inventor/elements/SoGLClipPlaneElement.h>

#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/system/gl.h>

// static variables
int SoGLClipPlaneElement::maxGLPlanes = -1;

SO_ELEMENT_SOURCE(SoGLClipPlaneElement);

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

  if (SoGLClipPlaneElement::maxGLPlanes == -1) {
    // NB: don't try to be clever and move this code to the
    // initClass() method, as it won't work -- the GL variables may
    // not have been initialized yet when it's called. --mortene
    GLint val;

    // FIXME: this needs a valid OpenGL context to work -- which we
    // either should make sure we have, or post an error or warning if
    // we don't. 20011011 mortene.
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
  equation[3] = - plane.getDistanceFromOrigin();
  glClipPlane((GLenum)((int)GL_CLIP_PLANE0 + idxadd), equation);
  glEnable((GLenum)((int)GL_CLIP_PLANE0 + idxadd));
}
