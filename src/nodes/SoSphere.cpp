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

// Note: the class documentation for the basic primitive shapes
// SoSphere, SoCylinder, SoCone and SoCube have many common, or at
// least close to common, paragraphs. If you make any changes, check
// those other shapes too, to see if your updates / fixes should be
// migrated. <mortene@sim.no>.
/*!
  \class SoSphere SoSphere.h Inventor/nodes/SoSphere.h
  \brief The SoSphere class is for rendering sphere shapes.
  \ingroup nodes

  Renders a sphere with the size given by the SoSphere::radius
  field. The sphere is rendered with the current material, texture and
  drawstyle settings (if any, otherwise the default settings are
  used).

  The SoSphere node class is provided as a convenient abstraction for
  the application programmer to use "complex" shapes of this type
  without having to do the tessellation to polygons and other
  low-level programming herself.

  A good trick for rendering ellipsoidal 3D shapes is to use an
  SoSphere prefixed with an SoScale transformation to "flatten" it
  along one or more of the principal axes. (Ie use for instance an
  SoScale node with SoScale::scaleFactor equal to [1, 1, 0.1] to
  flatten it along the Z direction.)

  A sphere is visualized by the underlying rendering system by first
  tessellating the conceptual sphere into a set of polygons. To
  control the trade-off between an as much as possible correct visual
  appearance of the sphere versus fast rendering, use an SoComplexity
  node to influence the number of polygons generated from the
  tessellation process. (The higher the complexity value, the more
  polygons will be generated, the more \e rounded the sphere will
  look.) Set the SoComplexity::value field to what you believe would
  be a good trade-off between correctness and speed for your
  particular application.

  \sa SoCone, SoCylinder, SoCube
*/

#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoSubNodeP.h>

#include <Inventor/SbSphere.h>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/bundles/SoMaterialBundle.h>
#include <Inventor/elements/SoGLTextureEnabledElement.h>
#include <Inventor/elements/SoGLTexture3EnabledElement.h>
#include <Inventor/misc/SoGL.h>
#include <Inventor/misc/SoGenerate.h>
#include <Inventor/misc/SoPick.h>
#include <Inventor/misc/SoState.h>

/*!
  \var SoSFFloat SoSphere::radius

  Radius of sphere. Default value is 1.0.
*/

#define SPHERE_NUM_SLICES 30.0f
#define SPHERE_NUM_STACKS 30.0f

// *************************************************************************

SO_NODE_SOURCE(SoSphere);

/*!
  Constructor.
*/
SoSphere::SoSphere(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoSphere);

  SO_NODE_ADD_FIELD(radius, (1.0f));
}

/*!
  Destructor.
*/
SoSphere::~SoSphere()
{
}

// Documented in superclass.
void
SoSphere::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoSphere, SO_FROM_INVENTOR_1|SoNode::VRML1);
}

// Documented in superclass.
void
SoSphere::GLRender(SoGLRenderAction * action)
{
  if (!this->shouldGLRender(action)) return;

  SoState * state = action->getState();

  SoMaterialBundle mb(action);
  mb.sendFirst();

  SbBool doTextures = FALSE;
  SbBool do3DTextures = FALSE;
  if (SoGLTextureEnabledElement::get(state)) doTextures = TRUE;
  else if (SoGLTexture3EnabledElement::get(state)) do3DTextures = TRUE;

  SbBool sendNormals = !mb.isColorOnly();

  float complexity = SbClamp(this->getComplexityValue(action), 0.0f, 1.0f);

  unsigned int flags = 0;
  if (sendNormals) flags |= SOGL_NEED_NORMALS;
  if (doTextures) flags |= SOGL_NEED_TEXCOORDS;
  else if (do3DTextures) flags |= SOGL_NEED_3DTEXCOORDS;

  sogl_render_sphere(this->radius.getValue(),
                     (int)(SPHERE_NUM_SLICES * complexity),
                     (int)(SPHERE_NUM_STACKS * complexity),
                     &mb,
                     flags);
}

// Documented in superclass.
void
SoSphere::computeBBox(SoAction * action, SbBox3f & box, SbVec3f & center)
{
  float r = this->radius.getValue();

  // Allow negative values.
  if (r < 0.0f) r = -r;

  box.setBounds(SbVec3f(-r, -r, -r), SbVec3f(r, r, r));
  center.setValue(0.0f, 0.0f, 0.0f);
}

// Documented in superclass.
void
SoSphere::rayPick(SoRayPickAction *action)
{
  if (!shouldRayPick(action)) return;

  sopick_pick_sphere(this->radius.getValue(),
                     action);
}

// Documented in superclass.
void
SoSphere::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  if (!this->shouldPrimitiveCount(action)) return;

  float complexity = this->getComplexityValue(action);
  action->addNumTriangles((int)(complexity*2.0f*SPHERE_NUM_SLICES*(SPHERE_NUM_STACKS-1)));
}

// Documented in superclass.
void
SoSphere::generatePrimitives(SoAction * action)
{
  float complexity = this->getComplexityValue(action);

  sogen_generate_sphere(this->radius.getValue(),
                        (int)(SPHERE_NUM_SLICES * complexity),
                        (int)(SPHERE_NUM_STACKS * complexity),
                        this,
                        action);
}
