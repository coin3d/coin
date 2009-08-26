/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2009 by Kongsberg SIM.  All rights reserved.
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
  \class SoShadowDirectionalLight SoShadowDirectionalLight.h Inventor/annex/FXViz/nodes/SoShadowDirectionalLight.h
  \brief The SoShadowDirectionalLight class is a node for setting up a directional light which casts shadows.

  Directional lights usually affect everything, but since it's not
  always feasible to generate shadows for the entire scene graph, this
  node has some extra features to work around this.
  
  It calculates the intersection between the current view volume and 
  either the scene bounding box, or the bounding box provided in this node. 
  The shadows are only calculated for this volume.

  In addition, you can set the maximum distance from the camera which
  will be shaded with shadows. Think of this a new far plane for the
  camera which only affects shadows.

  As with SoShadowSpotLight, it's possible to optimize further by
  setting your own shadow caster scene graph in the shadowMapScene
  field.


  ShadowGroup {
    quality 1 # to get per pixel lighting

    ShadowDirectionalLight {
      direction 1 1 -1

      shadowMapScene
      DEF sphere Separator {
          Complexity { value 1.0 }
          Material { diffuseColor 1 1 0 specularColor 1 1 1 shininess 0.9 }
          Shuttle { translation0 -3 1 0 translation1 3 -5 0 speed 0.25 on TRUE }
          Translation { translation -5 0 2 }
          Sphere { radius 2.0 }
       }
    }
    # need to insert the sphere in the regular scene graph as well
    USE sphere

    Separator {
      Material { diffuseColor 1 0 0 specularColor 1 1 1 shininess 0.9 }
      Shuttle { translation0 0 -5 0 translation1 0 5 0 speed 0.15 on TRUE }
      Translation { translation 0 0 -3 }
      Cube { depth 1.8 }
    }
    Separator {
      Material { diffuseColor 0 1 0 specularColor 1 1 1 shininess 0.9 }
      Shuttle { translation0 -5 0 0 translation1 5 0 0 speed 0.3 on TRUE }
      Translation { translation 0 0 -3 }
      Cube { }
    }

    Coordinate3 { point [ -10 -10 -3, 10 -10 -3, 10 10 -3, -10 10 -3 ] }
    Material { specularColor 1 1 1 shininess 0.9 }

    Complexity { textureQuality 0.1 }
    Texture2 { image 2 2 3 0xffffff 0x225588 0x225588 0xffffff }
    Texture2Transform { scaleFactor 4 4 }
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
  \var SoSFFloat SoShadowGroup::nearDistance

  Can be used to set a fixed near distance for this directional light. The value in this
  field will be used if it's set to > 0.0. Default value is -1.0.
*/

/*!
  \var SoSFFloat SoShadowGroup::farDistance

  Can be used to set a fixed far distance for this directional light. The value in this
  field will be used if it's set to > 0.0. Default value is -1.0.
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
  SO_NODE_ADD_FIELD(nearDistance, (-1.0f));
  SO_NODE_ADD_FIELD(farDistance, (-1.0f));
  SO_NODE_ADD_FIELD(maxShadowDistance, (-1.0f));
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
