/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#ifdef HAVE_VRML97

/*!
  \class SoVRMLColor SoVRMLColor.h Inventor/VRMLnodes/SoVRMLColor.h
  \brief The SoVRMLColor class is used to specify multiple colors for a single shape.
  \ingroup VRMLnodes

  \WEB3DCOPYRIGHT
  
  \verbatim
  Color {
    exposedField MFColor color  []         # [0,1]
  }
  \endverbatim

  This node defines a set of RGB colours to be used in the fields of
  another node.  
  
  Color nodes are only used to specify multiple colours for a single
  geometric shape, such as colours for the faces or vertices of an
  IndexedFaceSet. A Material node is used to specify the overall
  material parameters of lit geometry. If both a Material node and a
  Color node are specified for a geometric shape, the colours shall
  replace the diffuse component of the material.  RGB or RGBA textures
  take precedence over colours; specifying both an RGB or RGBA texture
  and a Color node for geometric shape will result in the Color node
  being ignored. Details on lighting equations can be found in 4.14,
  Lighting model
  (<http://www.web3d.org/x3d/specifications/vrml/ISO-IEC-14772-VRML97/part1/concepts.html#4.14>).

*/

/*!
  \var SoMFColor SoVRMLColor::color
  The color field. Empty by default.
*/

#include <Inventor/VRMLnodes/SoVRMLColor.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/elements/SoLazyElement.h>
#include <Inventor/elements/SoOverrideElement.h>
#include <Inventor/elements/SoGLVBOElement.h>
#ifdef COIN_THREADSAFE
#include <Inventor/threads/SbStorage.h>
#endif // COIN_THREADSAFE

#include "nodes/SoSubNodeP.h"
#include "rendering/SoVBO.h"

class SoVRMLColorP {
 public:
  SoVRMLColorP() : 
#ifdef COIN_THREADSAFE
    colorpacker_storage(sizeof(void*), alloc_colorpacker, free_colorpacker),
#endif // COIN_THREADSAFE
    vbo(NULL) 
  { }

  ~SoVRMLColorP() { delete this->vbo; }

#ifdef COIN_THREADSAFE
  SbStorage colorpacker_storage;
#else // COIN_THREADSAFE
  SoColorPacker single_colorpacker;
#endif // COIN_THREADSAFE
  
  SoColorPacker * getColorPacker(void) {
#ifdef COIN_THREADSAFE
    SoColorPacker ** cptr = (SoColorPacker**) this->colorpacker_storage.get();
    return * cptr;
#else // COIN_THREADSAFE
    return &this->single_colorpacker;
#endif // COIN_THREADSAFE
  }

  SoVBO * vbo;

#ifdef COIN_THREADSAFE
private:
  static void alloc_colorpacker(void * data) {
    SoColorPacker ** cptr = (SoColorPacker**) data;
    *cptr = new SoColorPacker;
  }
  static void free_colorpacker(void * data) {
    SoColorPacker ** cptr = (SoColorPacker**) data;
    delete *cptr;
  }
#endif // COIN_THREADSAFE
};

#define PRIVATE(obj) ((obj)->pimpl)

SO_NODE_SOURCE(SoVRMLColor);

// doc in parent
void
SoVRMLColor::initClass(void) // static
{
  SO_NODE_INTERNAL_INIT_CLASS(SoVRMLColor, SO_VRML97_NODE_TYPE);
}

/*!
  Constructor.
*/
SoVRMLColor::SoVRMLColor(void)
{
  PRIVATE(this) = new SoVRMLColorP;
  SO_VRMLNODE_INTERNAL_CONSTRUCTOR(SoVRMLColor);

  SO_VRMLNODE_ADD_EMPTY_EXPOSED_MFIELD(color);
}

/*!
  Destructor.
*/
SoVRMLColor::~SoVRMLColor() // virtual, protected
{
  delete PRIVATE(this);
}

// Doc in parent
void
SoVRMLColor::doAction(SoAction * action)
{
  SoState * state = action->getState();
  const int num = this->color.getNum();
  if (num &&
      !this->color.isIgnored() &&
      !SoOverrideElement::getDiffuseColorOverride(state)) {

    SoLazyElement::setDiffuse(state,
                              this,
                              num,
                              this->color.getValues(0),
                              PRIVATE(this)->getColorPacker());

    if (state->isElementEnabled(SoGLVBOElement::getClassStackIndex())) {
      SbBool setvbo = FALSE;
      SoBase::staticDataLock();
      if (SoGLVBOElement::shouldCreateVBO(state, num)) {
        setvbo = TRUE;
        if (PRIVATE(this)->vbo == NULL) {
          PRIVATE(this)->vbo = new SoVBO(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
        }
      }
      else if (PRIVATE(this)->vbo) {
        PRIVATE(this)->vbo->setBufferData(NULL, 0, 0);
      }
      // don't fill in any data in the VBO. Data will be filled in
      // using the ColorPacker right before the VBO is used
      SoBase::staticDataUnlock();
      if (setvbo) {
        SoGLVBOElement::setColorVBO(state, PRIVATE(this)->vbo);
      }
    }
    if (this->isOverride()) {
      SoOverrideElement::setDiffuseColorOverride(state, this, TRUE);
    }
  }
}

// Doc in parent
void
SoVRMLColor::GLRender(SoGLRenderAction * action)
{
  SoVRMLColor::doAction((SoAction*) action);
}

// Doc in parent
void
SoVRMLColor::callback(SoCallbackAction * action)
{
  SoVRMLColor::doAction((SoAction*) action);
}

#undef PRIVATE

#endif // HAVE_VRML97
