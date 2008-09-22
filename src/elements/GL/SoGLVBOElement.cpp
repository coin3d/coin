/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2008 by Kongsberg SIM.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg SIM about acquiring
 *  a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg SIM, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

/*!
  \class SoGLVBOElement Inventor/elements/SoGLVBOElement.h
  \brief The SoGLVBOElement class is used to store VBO state.
  \ingroup elements

  FIXME: write doc.

  \COIN_CLASS_EXTENSION

  \since Coin 2.5
*/

#include <Inventor/elements/SoGLVBOElement.h>

#include <cassert>

#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/lists/SbList.h>
#include <Inventor/C/glue/gl.h>
#include <Inventor/misc/SoGLDriverDatabase.h>

#include "misc/SoGL.h"
#include "glue/glp.h"
#include "misc/SoVBO.h"

#define PRIVATE(obj) obj->pimpl

class SoGLVBOElementP {
 public:

  SoVBO * vertexvbo;
  SoVBO * normalvbo;
  SoVBO * colorvbo;
  SbList <SoVBO*> texcoordvbo;
};

SO_ELEMENT_CUSTOM_CONSTRUCTOR_SOURCE(SoGLVBOElement);

/*!
  This static method initializes static data for the
  SoMultiTextureCoordinateElement class.
*/

void
SoGLVBOElement::initClass()
{
  SO_ELEMENT_INIT_CLASS(SoGLVBOElement, inherited);
}


/*!
  The constructor.
*/
SoGLVBOElement::SoGLVBOElement(void)
{
  PRIVATE(this) = new SoGLVBOElementP;

  this->setTypeId(SoGLVBOElement::classTypeId);
  this->setStackIndex(SoGLVBOElement::classStackIndex);
}

/*!
  The destructor.
*/

SoGLVBOElement::~SoGLVBOElement()
{
  delete PRIVATE(this);
}

/*!
  Sets the vertex VBO.
*/
void
SoGLVBOElement::setVertexVBO(SoState * state, SoVBO * vbo)
{
  SoGLVBOElement * elem = getElement(state);
  PRIVATE(elem)->vertexvbo = vbo;
}

/*!
  Sets the normal VBO.
*/
void
SoGLVBOElement::setNormalVBO(SoState * state, SoVBO * vbo)
{
  SoGLVBOElement * elem = getElement(state);
  PRIVATE(elem)->normalvbo = vbo;
}

/*!
  Sets the color VBO.
*/
void
SoGLVBOElement::setColorVBO(SoState * state, SoVBO * vbo)
{
  SoGLVBOElement * elem = getElement(state);
  PRIVATE(elem)->colorvbo = vbo;
}

/*!
  Sets the texture coordinate VBO.
*/
void
SoGLVBOElement::setTexCoordVBO(SoState * state, const int unit, SoVBO * vbo)
{
  SoGLVBOElement * elem = getElement(state);
  const int n = PRIVATE(elem)->texcoordvbo.getLength();
  for (int i = n; i < unit; i++) {
    PRIVATE(elem)->texcoordvbo.append(NULL);
  }
  PRIVATE(elem)->texcoordvbo.append(vbo);
}

// doc in parent
void
SoGLVBOElement::init(SoState *state)
{
  PRIVATE(this)->vertexvbo = NULL;
  PRIVATE(this)->normalvbo = NULL;
  PRIVATE(this)->colorvbo = NULL;
  PRIVATE(this)->texcoordvbo.truncate(0);
}

// doc in parent
void
SoGLVBOElement::push(SoState *state)
{
  SoGLVBOElement * prev = (SoGLVBOElement *)
    this->getNextInStack();

  PRIVATE(this)->vertexvbo = PRIVATE(prev)->vertexvbo;
  PRIVATE(this)->normalvbo = PRIVATE(prev)->normalvbo;
  PRIVATE(this)->colorvbo = PRIVATE(prev)->colorvbo;
  PRIVATE(this)->texcoordvbo.truncate(0);

  for (int i = 0; i < PRIVATE(prev)->texcoordvbo.getLength(); i++) {
    PRIVATE(this)->texcoordvbo.append(PRIVATE(prev)->texcoordvbo[i]);
  }
}

// doc in parent
void
SoGLVBOElement::pop(SoState *state, const SoElement * prevtopelement)
{
  // nothing to do
}

// doc in parent
SbBool
SoGLVBOElement::matches(const SoElement * elt) const
{
  assert(0 && "should never get here");
  return TRUE;
}

// doc in parent
SoElement *
SoGLVBOElement::copyMatchInfo(void) const
{
  assert(0 && "should never get here");
  return NULL;
}

/*!
  Returns a writable element instance.
*/
SoGLVBOElement *
SoGLVBOElement::getElement(SoState * state)
{
  return (SoGLVBOElement*) state->getElement(classStackIndex);
}

/*!
  Returns a read-only element instance.
*/
const SoGLVBOElement *
SoGLVBOElement::getInstance(SoState * state)
{
  return (SoGLVBOElement*) state->getConstElement(classStackIndex);
}

SoVBO *
SoGLVBOElement::getVertexVBO(void) const
{
  return PRIVATE(this)->vertexvbo;
}

SoVBO *
SoGLVBOElement::getNormalVBO(void) const
{
  return PRIVATE(this)->normalvbo;
}

SoVBO *
SoGLVBOElement::getColorVBO(void) const
{
  return PRIVATE(this)->colorvbo;
}

int
SoGLVBOElement::getNumTexCoordVBO(void) const
{
  return PRIVATE(this)->texcoordvbo.getLength();
}

SoVBO *
SoGLVBOElement::getTexCoordVBO(const int idx) const
{
  if (idx < PRIVATE(this)->texcoordvbo.getLength()) {
    return PRIVATE(this)->texcoordvbo[idx];
  }
  return NULL;
}

/*!
  Returns \a TRUE if VBO is supported for the current context,
  and if numdata is between the limits set for VBO rendering.

*/
SbBool
SoGLVBOElement::shouldCreateVBO(SoState * state, const int numdata)
{
  const cc_glglue * glue = sogl_glue_instance(state);
  // don't use SoGLCacheContextElement to find the current cache
  // context since we don't want this call to create a cache dependecy
  // on SoGLCacheContextElement.
  return
    SoGLDriverDatabase::isSupported(glue, SO_GL_FRAMEBUFFER_OBJECT) &&
    SoVBO::shouldCreateVBO(state, glue->contextid, numdata);
}

#undef PRIVATE

