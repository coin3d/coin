/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 2001 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

/*!
  \class SoVRMLSphere SoVRMLSphere.h Inventor/VRMLnodes/SoVRMLSphere.h
  \brief The SoVRMLSphere class is used to represent a spherical 3D object.
  \ingroup VRMLnodes

  \WEB3DCOPYRIGHT

  \verbatim
  Sphere {
    field SFFloat radius  1    # (0, inf)
  }
  \endverbatim
  
  The Sphere node specifies a sphere centred at (0, 0, 0) in the local
  coordinate system. The radius field specifies the radius of the
  sphere and shall be greater than zero. Figure 6.15 depicts the
  fields of the Sphere node.

  <center>
  <img src="http://www.web3d.org/technicalinfo/specifications/vrml97/Images/sphere.gif">
  Figure 6.15 -- Sphere node
  </center>

  When a texture is applied to a sphere, the texture covers the entire
  surface, wrapping counterclockwise from the back of the sphere
  (i.e., longitudinal arc intersecting the -Z-axis) when viewed from
  the top of the sphere. The texture has a seam at the back where the
  X=0 plane intersects the sphere and Z values are
  negative. TextureTransform affects the texture coordinates of the
  Sphere.  The Sphere node's geometry requires outside faces
  only. When viewed from the inside the results are undefined.  

*/

/*!
  \var SoSFFloat SoVRMLSphere::radius
  Sphere radius. Default value is 1.0.
*/


#include <Inventor/VRMLnodes/SoVRMLSphere.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/nodes/SoSubNodeP.h>

#include <Inventor/bundles/SoMaterialBundle.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/elements/SoGLTextureEnabledElement.h>
#include <Inventor/elements/SoGLNormalizeElement.h>
#include <Inventor/elements/SoLightModelElement.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/misc/SoGenerate.h>
#include <Inventor/misc/SoPick.h>
#include <Inventor/misc/SoGL.h>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/elements/SoGLShapeHintsElement.h>

#define SPHERE_NUM_SLICES 30.0f
#define SPHERE_NUM_STACKS 30.0f

SO_NODE_SOURCE(SoVRMLSphere);

// Doc in parent
void
SoVRMLSphere::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoVRMLSphere, SO_VRML97_NODE_TYPE);
}

/*!
  Constructor.
*/
SoVRMLSphere::SoVRMLSphere(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoVRMLSphere);

  SO_VRMLNODE_ADD_FIELD(radius, (1.0f));
}

/*!
  Destructor.
*/
SoVRMLSphere::~SoVRMLSphere()
{
}

// Doc in parent
void
SoVRMLSphere::GLRender(SoGLRenderAction * action)
{
  if (!shouldGLRender(action)) return;

  SoState * state = action->getState();

  SoMaterialBundle mb(action);
  mb.sendFirst();

  SbBool doTextures = SoGLTextureEnabledElement::get(state);

  SbBool sendNormals =
    (SoLightModelElement::get(state) !=
     SoLightModelElement::BASE_COLOR);

  float complexity = this->getComplexityValue(action);

  if (sendNormals) {
    SoGLNormalizeElement::forceSend(state, TRUE);
  }

  unsigned int flags = 0;
  if (sendNormals) flags |= SOGL_NEED_NORMALS;
  if (doTextures) flags |= SOGL_NEED_TEXCOORDS;

  // enable back face culling
  SoGLShapeHintsElement::forceSend(state, TRUE, TRUE);

  sogl_render_sphere(this->radius.getValue(),
                     (int)(SPHERE_NUM_SLICES * complexity),
                     (int)(SPHERE_NUM_STACKS * complexity),
                     &mb,
                     flags);

}

// Doc in parent
void
SoVRMLSphere::rayPick(SoRayPickAction * action)
{
  if (!shouldRayPick(action)) return;

  sopick_pick_sphere(this->radius.getValue(),
                     action);
}

// Doc in parent
void
SoVRMLSphere::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  if (!this->shouldPrimitiveCount(action)) return;

  float complexity = this->getComplexityValue(action);
  action->addNumTriangles((int)(complexity*2.0f*SPHERE_NUM_SLICES*(SPHERE_NUM_STACKS-1)));
}

void
SoVRMLSphere::generatePrimitives(SoAction * action)
{
  float complexity = this->getComplexityValue(action);

  sogen_generate_sphere(this->radius.getValue(),
                        (int)(SPHERE_NUM_SLICES * complexity),
                        (int)(SPHERE_NUM_STACKS * complexity),
                        this,
                        action);
}

// Doc in parent
void
SoVRMLSphere::computeBBox(SoAction * action,
                          SbBox3f & box,
                          SbVec3f & center)
{
  float r = this->radius.getValue();

  // Allow negative values.
  if (r < 0.0f) r = -r;

  box.setBounds(SbVec3f(-r, -r, -r), SbVec3f(r, r, r));
  center.setValue(0.0f, 0.0f, 0.0f);
}

#undef SPHERE_NUM_SLICES
#undef SPHERE_NUM_STACKS
