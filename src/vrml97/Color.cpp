/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
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
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

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
  (<http://www.web3d.org/technicalinfo/specifications/vrml97/part1/concepts.html#4.14>).

*/

/*!
  \var SoMFColor SoVRMLColor::color
  The color field. Empty by default.
*/

#include <Inventor/VRMLnodes/SoVRMLColor.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/elements/SoLazyElement.h>
#include <Inventor/elements/SoOverrideElement.h>

#ifndef DOXYGEN_SKIP_THIS
class SoVRMLColorP {
public:
  SoColorPacker colorpacker;
};
#endif // DOXYGEN_SKIP_THIS

#undef THIS
#define THIS this->pimpl

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
  THIS = new SoVRMLColorP;
  SO_VRMLNODE_INTERNAL_CONSTRUCTOR(SoVRMLColor);

  SO_VRMLNODE_ADD_EMPTY_EXPOSED_MFIELD(color);
}

/*!
  Destructor.
*/
SoVRMLColor::~SoVRMLColor() // virtual, protected
{
  delete THIS;
}

// Doc in parent
void
SoVRMLColor::doAction(SoAction * action)
{
  SoState * state = action->getState();
  if (this->color.getNum() &&
      !this->color.isIgnored() &&
      !SoOverrideElement::getDiffuseColorOverride(state)) {
    SoLazyElement::setDiffuse(state,
                              this,
                              this->color.getNum(),
                              this->color.getValues(0),
                              &THIS->colorpacker);
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

#undef THIS
