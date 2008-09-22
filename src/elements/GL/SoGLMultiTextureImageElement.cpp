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
  \class SoGLMultiTextureImageElement Inventor/elements/SoGLMultiTextureImageElement.h
  \brief The SoGLMultiTextureImageElement is used to control the current GL texture for texture units.
  \ingroup elements
*/

// *************************************************************************

#include <Inventor/elements/SoGLMultiTextureImageElement.h>

#include <cstdlib>

#include <Inventor/elements/SoTextureQualityElement.h>
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/elements/SoGLDisplayList.h>
#include <Inventor/elements/SoTextureImageElement.h>
#include <Inventor/elements/SoTextureCombineElement.h>
#include <Inventor/elements/SoGLShaderProgramElement.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/misc/SoGLImage.h>
#include <Inventor/SbImage.h>
#include <Inventor/C/tidbits.h>

#include <Inventor/errors/SoDebugError.h>
#include <Inventor/system/gl.h>
#include <Inventor/C/glue/gl.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#include "shaders/SoGLShaderProgram.h"
#include "misc/SoGL.h" // GL wrapper.

// *************************************************************************

#define MAX_UNITS 16
#define PRIVATE(obj) obj->pimpl

class SoGLMultiTextureImageElementP {
public:
  SoGLMultiTextureImageElement::GLUnitData unitdata[MAX_UNITS];
  SoState * state;
  uint32_t cachecontext;
};

SO_ELEMENT_CUSTOM_CONSTRUCTOR_SOURCE(SoGLMultiTextureImageElement);

/*!
  This static method initializes static data in the
  SoGLMultiTextureImageElement class.
*/

void
SoGLMultiTextureImageElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoGLMultiTextureImageElement, inherited);
}

SoGLMultiTextureImageElement::SoGLMultiTextureImageElement(void)
{
  PRIVATE(this) = new SoGLMultiTextureImageElementP;

  this->setTypeId(SoGLMultiTextureImageElement::classTypeId);
  this->setStackIndex(SoGLMultiTextureImageElement::classStackIndex);
}

/*!
  The destructor.
*/

SoGLMultiTextureImageElement::~SoGLMultiTextureImageElement(void)
{
  delete PRIVATE(this);
}

// doc from parent
void
SoGLMultiTextureImageElement::init(SoState * state)
{
  inherited::init(state);

  SoAction * action = state->getAction();
  assert(action->isOfType(SoGLRenderAction::getClassTypeId()));

  // fetch cache context from action since SoGLCacheContextElement
  // might not be initialized yet.
  SoGLRenderAction * glaction = (SoGLRenderAction*) action;
  PRIVATE(this)->cachecontext = glaction->getCacheContext();

  for (int i = 0; i < MAX_UNITS; i++) {
    GLUnitData & ud = PRIVATE(this)->unitdata[i];
    ud.glimage = NULL;
  }
}


// Documented in superclass. Overridden to pass GL state to the next
// element.
void
SoGLMultiTextureImageElement::push(SoState * state)
{
  inherited::push(state);
  SoGLMultiTextureImageElement * prev = (SoGLMultiTextureImageElement*)
    this->getNextInStack();
  PRIVATE(this)->state = state;
  PRIVATE(this)->cachecontext = PRIVATE(prev)->cachecontext;

  for (int i = 0; i < MAX_UNITS; i++) {
    PRIVATE(this)->unitdata[i] = PRIVATE(prev)->unitdata[i];
  }

  // capture previous element since we might or might not change the
  // GL state in set/pop
  prev->capture(state);
}


// Documented in superclass. Overridden to pass GL state to the
// previous element.
void
SoGLMultiTextureImageElement::pop(SoState * state,
                                  const SoElement * prevTopElement)
{
  inherited::pop(state, prevTopElement);
  SoGLMultiTextureImageElement * prev = (SoGLMultiTextureImageElement*)
    prevTopElement;

  SoGLShaderProgram * prog = SoGLShaderProgramElement::get(state);
  SbString str;

  for (int i = 1; i < MAX_UNITS; i++) {
    const GLUnitData & prevud = PRIVATE(prev)->unitdata[i];
    // FIXME: buggy. Find some solution to handle this. pederb, 2003-11-12
    // if (prevud.glimage && prevud.glimage->getImage()) prevud.glimage->getImage()->readUnlock();

    const GLUnitData & thisud = PRIVATE(this)->unitdata[i];
    if (thisud.glimage != prevud.glimage) this->updateGL(i);

    str.sprintf("coin_texunit%d_model", i);
    if (prog) prog->updateCoinParameter(state, SbName(str.getString()),
                                        thisud.glimage != NULL ? this->getUnitData(i).model : 0);
  }
}

static SoTextureImageElement::Wrap
multi_translateWrap(const SoGLImage::Wrap wrap)
{
  if (wrap == SoGLImage::REPEAT) return SoTextureImageElement::REPEAT;
  return SoTextureImageElement::CLAMP;
}

/*!
  Sets the current texture. Id \a didapply is TRUE, it is assumed
  that the texture image already is the current GL texture. Do not
  use this feature unless you know what you're doing.
*/
void
SoGLMultiTextureImageElement::set(SoState * const state, SoNode * const node,
                                  const int unit,
                                  SoGLImage * image,
                                  const SoTextureImageElement::Model model,
                                  const SbColor & blendColor)
{
  assert(unit >= 0 && unit < MAX_UNITS);

  SoGLMultiTextureImageElement * elem = (SoGLMultiTextureImageElement*)
    state->getElement(classStackIndex);

  GLUnitData & ud = PRIVATE(elem)->unitdata[unit];

  // FIXME: buggy. Find some solution to handle this. pederb, 2003-11-12
  // if (ud.glimage && ud.glimage->getImage()) ud.glimage->getImage()->readUnlock();

  if (image) {
    // keep SoMultiTextureImageElement "up-to-date"
    inherited::set(state, node,
                   unit,
                   SbVec3s(0,0,0),
                   0,
                   NULL,
                   multi_translateWrap(image->getWrapS()),
                   multi_translateWrap(image->getWrapT()),
                   multi_translateWrap(image->getWrapR()),
                   model,
                   blendColor);
    ud.glimage = image;
    // make sure image isn't changed while this is the active texture
    // FIXME: buggy. Find some solution to handle this. pederb, 2003-11-12
    // if (image->getImage()) image->getImage()->readLock();
  }
  else {
    ud.glimage = NULL;
    inherited::setDefault(state, node, unit);
  }
  elem->updateGL(unit);

  SoGLShaderProgram * prog = SoGLShaderProgramElement::get(state);
  if (prog) {
    SbString str;
    str.sprintf("coin_texunit%d_model", unit);
    prog->updateCoinParameter(state, SbName(str.getString()), ud.glimage ? model : 0);
  }
}

void
SoGLMultiTextureImageElement::restore(SoState * state, const int unit)
{
  SoGLMultiTextureImageElement * elem = (SoGLMultiTextureImageElement*)
    state->getConstElement(classStackIndex);

  elem->updateGL(unit);
}

SoGLImage *
SoGLMultiTextureImageElement::get(SoState * state,
                                  const int unit,
                                  SoTextureImageElement::Model & model,
                                  SbColor & blendcolor)
{
  const SoGLMultiTextureImageElement * elem = (const SoGLMultiTextureImageElement*)
    getConstElement(state, classStackIndex);

  const UnitData & ud = elem->getUnitData(unit);

  model = ud.model;
  blendcolor = ud.blendColor;
  return PRIVATE(elem)->unitdata[unit].glimage;
}

// doc from parent
SbBool
SoGLMultiTextureImageElement::hasTransparency(const int unit) const
{
  assert(unit >= 0 && unit< MAX_UNITS);
  const GLUnitData & ud = PRIVATE(this)->unitdata[unit];
  if (ud.glimage) {
    // only return TRUE if the image has transparency, and if it can't
    // be rendered using glAlphaTest()
    return ud.glimage->hasTransparency() && !ud.glimage->useAlphaTest();
  }
  return FALSE;
}

void
SoGLMultiTextureImageElement::updateGL(const int unit)
{
  assert(unit >= 0 && unit< MAX_UNITS);
  const GLUnitData & glud = PRIVATE(this)->unitdata[unit];
  if (glud.glimage) {
    const cc_glglue * glue = cc_glglue_instance(PRIVATE(this)->cachecontext);
    cc_glglue_glActiveTexture(glue, (GLenum) (int(GL_TEXTURE0) + unit));

    const UnitData & ud = this->getUnitData(unit);
    SoState * state = PRIVATE(this)->state;
    SoGLDisplayList * dl = glud.glimage->getGLDisplayList(state);

    // tag image (for GLImage LRU cache).
    SoGLImage::tagImage(state, glud.glimage);

    if (SoTextureCombineElement::isDefault(state, unit)) {
      switch (ud.model) {
      case SoTextureImageElement::DECAL:
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
        break;
      case SoTextureImageElement::MODULATE:
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        break;
      case SoTextureImageElement::BLEND:
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
        glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, ud.blendColor.getValue());
        break;
      case SoTextureImageElement::REPLACE:
        // GL_REPLACE mode was introduced with OpenGL 1.1. It is
        // considered the client code's responsibility to check
        // that it can use this mode.
        //
        // FIXME: ..but we should do a sanity check anyway.
        // 20030901 mortene.
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        break;
      default:
        assert(0 && "unknown model");
        break;
      }
    }
    else {
      SoTextureCombineElement::apply(state, unit);
    }

    dl->call(state);
    cc_glglue_glActiveTexture(glue, (GLenum) GL_TEXTURE0);
  }
}

#undef MAX_UNITS
#undef PRIVATE

