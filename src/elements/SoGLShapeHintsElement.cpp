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
  \class SoGLShapeHintsElement Inventor/elements/SoGLShapeHintsElement.h
  \brief The SoGLShapeHintsElement class is yet to be documented.
  \ingroup elements

  FIXME: write doc.
*/

//
// handling of cases (from the original oiv man-pages):
//
// 1) ordered, solid   :   backface culling, ! twoside lighting
// 2) ordered, ! solid : ! backface culling,   twoside lighting
// 3) ! ordered        : ! backface culling, ! twoside lighting
//
// (this table of rendering rules is duplicated in the SoShapeHints
// classdoc)

//
// I find this strategy quite odd. My gut-feeling says that
// when vertexordering is unknown, two-side-lighting should be
// enabled, since it will be difficult to know if a normal
// points in or out of a polygon...
//

//
// nodes with automatically generated normals should probably
// force two-side lighting when vertexordering is unknown, since
// it is impossible to know if normals are pointing in or out.
//
// use SoGLShapeHintsElement::forceSend(twoside) for this purpose
//

#include <Inventor/elements/SoGLShapeHintsElement.h>
#include <Inventor/elements/SoGLLazyElement.h>


#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/system/gl.h>

#include <assert.h>

SO_ELEMENT_SOURCE(SoGLShapeHintsElement);

/*!
  This static method initializes static data for the
  SoGLShapeHintsElement class.
*/

void
SoGLShapeHintsElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoGLShapeHintsElement, inherited);
}

/*!
  The destructor.
*/

SoGLShapeHintsElement::~SoGLShapeHintsElement(void)
{
}

//! FIXME: write doc.

void
SoGLShapeHintsElement::init(SoState * state)
{
  inherited::init(state);  
  glCullFace(GL_BACK);
  this->state = state;
}

//! FIXME: write doc.

void
SoGLShapeHintsElement::push(SoState * state)
{
  inherited::push(state);
  SoGLShapeHintsElement * prev =
    (SoGLShapeHintsElement *) this->getNextInStack();
  this->state = prev->state;
}

//! FIXME: write doc.

void
SoGLShapeHintsElement::pop(SoState * state,
                           const SoElement * prevTopElement)
{
  inherited::pop(state, prevTopElement);
}

//! FIXME: write doc.

void
SoGLShapeHintsElement::setElt(VertexOrdering vertexOrdering,
                              ShapeType shapeType,
                              FaceType faceType)
{
  inherited::setElt(vertexOrdering, shapeType, faceType);
  // do nothing since GL stuff is handled by SoGLLazyElement
}

/*!
  Update gl state. Use this is you only want to modify the
  twoside lighting feature.
*/

void
SoGLShapeHintsElement::forceSend(SoState * const state,
                                 const SbBool twoside)
{
  SoGLLazyElement::sendTwosideLighting(state, twoside);
}

/*!
  Update GL state. Use this if you don't care about the two-side
  lighting state.
*/

void
SoGLShapeHintsElement::forceSend(SoState * const state,
                                 const SbBool ccw, const SbBool cull)
{
  SoGLLazyElement::sendVertexOrdering(state, ccw ? SoLazyElement::CCW : SoLazyElement::CW);
  SoGLLazyElement::sendBackfaceCulling(state, cull);
}

//! FIXME: write doc.

void
SoGLShapeHintsElement::forceSend(SoState * const state,
                                 const SbBool ccw, const SbBool cull,
                                 const SbBool twoside)
{
  SoGLLazyElement::sendVertexOrdering(state, ccw ? SoLazyElement::CCW : SoLazyElement::CW);
  SoGLLazyElement::sendBackfaceCulling(state, cull);
  SoGLLazyElement::sendTwosideLighting(state, twoside);
}
