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

/*!
  \class SoShadowDirectionalLight SoShadowDirectionalLight.h Inventor/annex/FXViz/nodes/SoShadowDirectionalLight.h
  \brief The SoShadowDirectionalLight class is a node for setting up a directional light which casts shadows.

  Directional lights usually affect everything, but since it's not
  always feasible to use one shadow map for the entire scene
  graph. This node has some extra features to work around this.
  
  It calculates the intersection between the current view volume and
  either the scene bounding box, or the bounding box provided in this
  node.  The shadows are only calculated for this volume. This means
  that you'll get more detailed shadows as you zoom in on items in the
  scene graph.

  In addition, you can set the maximum distance from the camera which
  will be shaded with shadows. Think of this a new far plane for the
  camera which only affects shadows.

  As with SoShadowSpotLight, it's possible to optimize further by
  setting your own shadow caster scene graph in the shadowMapScene
  field.

  The example scene graph below demonstrates how you can use this node
  to create shadows on a large number of objects, and still get decent
  precision when zooming in. To further reduce the volume covered by
  the shadowmap, you can set \a maxShadowDistance to some number > 0.
  This is the distance from the camera where shadows will be visible.

  \code

  DirectionalLight {
     direction 0 0 -1
     intensity 0.2
  }

  ShadowGroup {
    quality 1 # to get per pixel lighting
    precision 1

    ShadowDirectionalLight {
      direction 1 1 -1
      intensity 0.8
      # enable this to reduce the shadow view distance
      # maxShadowDistance 200
    }

    # 900 cubes spaced out over a fairly large area
    Array {
      origin CENTER
      numElements1 30
      numElements2 30
      numElements3 1
      separation1 20 0 0
      separation2 0 20 0
      separation3 0 0 0

      Material { diffuseColor 1 0 0 specularColor 1 1 1 }
      Cube { width 4 height 4 depth 4 }
    }

    # a chess board
    Coordinate3 { point [ -400 -400 -3, 400 -400 -3, 400 400 -3, -400 400 -3 ] }
    Material { specularColor 1 1 1 shininess 0.9 }
    Complexity { textureQuality 0.1 }
    Texture2 { image 2 2 3 0xffffff 0x225588 0x225588 0xffffff }
    Texture2Transform { scaleFactor 20 20 }
    FaceSet { numVertices 4 }
  }
  \endcode

  \since Coin 3.0
*/

/*!
  \var SoSFNode SoShadowGroup::shadowMapScene

  The shadow map scene graph. If this is NULL (the default), the node
  will behave as a normal SoDirectionalLight node.

*/

/*!
  \var SoSFFloat SoShadowGroup::maxShadowDistance

  The maximum distance (from the camera) that we'll see shadows from this light source.
*/

/*!
  \var SoSFVec3f SoShadowGroup::bboxCenter

  Can be used to specify the volume that should be used for
  calculating the resulting shadow volume.
*/

/*!
  \var SoSFVec3f SoShadowGroup::bboxSize

  Can be used to specify the volume that should be used for
  calculating the resulting shadow volume.
*/

// *************************************************************************

#include <Inventor/annex/FXViz/nodes/SoShadowDirectionalLight.h>

#include <stdio.h>
#include <Inventor/actions/SoGLRenderAction.h>

#include "nodes/SoSubNodeP.h"

// *************************************************************************


SO_NODE_SOURCE(SoShadowDirectionalLight);

/*!
  Constructor.
*/
SoShadowDirectionalLight::SoShadowDirectionalLight(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoShadowDirectionalLight);
  SO_NODE_ADD_FIELD(shadowMapScene, (NULL));
  SO_NODE_ADD_FIELD(maxShadowDistance, (-1.0f));
  SO_NODE_ADD_FIELD(bboxCenter, (0.0f, 0.0f, 0.0f));
  SO_NODE_ADD_FIELD(bboxSize, (-1.0f, -1.0f, -1.0f));
}

/*!
  Destructor.
*/
SoShadowDirectionalLight::~SoShadowDirectionalLight()
{
}

// Doc from superclass.
void
SoShadowDirectionalLight::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoShadowDirectionalLight, SO_FROM_COIN_4_0);
}

// Doc from superclass.
void
SoShadowDirectionalLight::GLRender(SoGLRenderAction * action)
{
  inherited::GLRender(action);
}

#ifdef COIN_TEST_SUITE

BOOST_AUTO_TEST_CASE(initialized)
{
  SoShadowDirectionalLight * node = new SoShadowDirectionalLight;
  assert(node);
  node->ref();
  BOOST_CHECK_MESSAGE(node->getTypeId() != SoType::badType(),
                      "missing class initialization");
  node->unref();
}

#endif // COIN_TEST_SUITE
