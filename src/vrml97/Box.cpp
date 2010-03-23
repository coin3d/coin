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
  \class SoVRMLBox SoVRMLBox.h Inventor/VRMLnodes/SoVRMLBox.h
  \brief The SoVRMLBox class is used for representing a 3D box.
  \ingroup VRMLnodes

  \WEB3DCOPYRIGHT

  \verbatim
  Box {
    field    SFVec3f size  2 2 2        # (0, inf)
  }
  \endverbatim

  The Box node specifies a rectangular parallelepiped box centred at (0, 0, 0)
  in the local coordinate system and aligned with the local coordinate axes.
  By default, the box measures 2 units in each dimension, from -1 to +1. The
  size field specifies the extents of the box along the X-, Y-, and
  Z-axes respectively and each component value shall be greater than zero.
  Figure 6.2 illustrates the Box node.

  <center>
  <img src="http://www.web3d.org/x3d/specifications/vrml/ISO-IEC-14772-VRML97/Images/box.gif">
  Figure 6.2
  </center>

  Textures are applied individually to each face of the box. On the
  front (+Z), back (-Z), right (+X), and left (-X) faces of the box,
  when viewed from the outside with the +Y-axis up, the texture is
  mapped onto each face with the same orientation as if the image were
  displayed normally in 2D.  On the top face of the box (+Y), when
  viewed from above and looking down the Y-axis toward the origin with
  the -Z-axis as the view up direction, the texture is mapped onto the
  face with the same orientation as if the image were displayed
  normally in 2D. On the bottom face of the box (-Y), when viewed from
  below looking up the Y-axis toward the origin with the +Z-axis as
  the view up direction, the texture is mapped onto the face with the
  same orientation as if the image were displayed normally in
  2D. SoVRMLTextureTransform affects the texture coordinates of the
  Box.  The Box node's geometry requires outside faces only. When
  viewed from the inside the results are undefined.

*/

/*!
  \var SoVRMLBox::size

  Box size vector. Default value is (2,2,2).
*/

#include <Inventor/VRMLnodes/SoVRMLBox.h>
#include "coindefs.h"

#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/elements/SoGLMultiTextureEnabledElement.h>
#include <Inventor/elements/SoMultiTextureCoordinateElement.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/bundles/SoMaterialBundle.h>
#include <Inventor/misc/SoState.h>

#include "nodes/SoSubNodeP.h"
#include "rendering/SoGL.h"
#include "misc/SoGenerate.h"
#include "misc/SoPick.h"

SO_NODE_SOURCE(SoVRMLBox);

// Doc in parent
void
SoVRMLBox::initClass(void) // static
{
  SO_NODE_INTERNAL_INIT_CLASS(SoVRMLBox, SO_VRML97_NODE_TYPE);
}

/*!
  Constructor.
*/
SoVRMLBox::SoVRMLBox(void)
{
  SO_VRMLNODE_INTERNAL_CONSTRUCTOR(SoVRMLBox);

  SO_VRMLNODE_ADD_FIELD(size, (2.0f, 2.0f, 2.0f));
}

/*!
  Destructor.
*/
SoVRMLBox::~SoVRMLBox()
{
}

// Doc in parent
void
SoVRMLBox::GLRender(SoGLRenderAction * action)
{
 if (!this->shouldGLRender(action)) return;
  SoState * state = action->getState();

  SbBool doTextures = SoGLMultiTextureEnabledElement::get(state);

  SoMaterialBundle mb(action);
  mb.sendFirst();

  SbBool sendNormals = !mb.isColorOnly() ||
    (SoMultiTextureCoordinateElement::getType(state) == SoMultiTextureCoordinateElement::FUNCTION);

  unsigned int flags = 0;
  if (doTextures) flags |= SOGL_NEED_TEXCOORDS;
  if (sendNormals) flags |= SOGL_NEED_NORMALS;

  SbVec3f s = this->size.getValue();

  sogl_render_cube(s[0],
                   s[1],
                   s[2],
                   &mb,
                   flags, state);
}

// Doc in parent
void
SoVRMLBox::rayPick(SoRayPickAction * action)
{
  if (!shouldRayPick(action)) return;

  SbVec3f s = this->size.getValue();
  sopick_pick_cube(s[0],
                   s[1],
                   s[2],
                   0,
                   this, action);
}

// Doc in parent
void
SoVRMLBox::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  if (!this->shouldPrimitiveCount(action)) return;
  action->addNumTriangles(12);
}

// Doc in parent
void
SoVRMLBox::generatePrimitives(SoAction * action)
{
  SbVec3f s = this->size.getValue();
  sogen_generate_cube(s[0],
                      s[1],
                      s[2],
                      0,
                      this,
                      action);
}

// Doc in parent
void
SoVRMLBox::computeBBox(SoAction * action,
                       SbBox3f & COIN_UNUSED_ARG(box),
                       SbVec3f & center)
{
  center.setValue(0.0f, 0.0f, 0.0f);
  SbVec3f s = this->size.getValue();
  float w = s[0] * 0.5f;
  float h = s[1] * 0.5f;
  float d = s[2] * 0.5f;

  // Allow negative values.
  if (w < 0.0f) w = -w;
  if (h < 0.0f) h = -h;
  if (d < 0.0f) d = -d;

  box.setBounds(-w, -h, -d, w, h, d);
}

#endif // HAVE_VRML97
