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
  \class SoVRMLPointSet SoVRMLPointSet.h Inventor/VRMLnodes/SoVRMLPointSet.h
  \brief The SoVRMLPointSet class is used to represent a set of 3D points.
  \ingroup VRMLnodes

  \WEB3DCOPYRIGHT

  \verbatim
  PointSet {
    exposedField  SFNode  color      NULL
    exposedField  SFNode  coord      NULL
  }
  \endverbatim
  
  The PointSet node specifies a set of 3D points, in the local
  coordinate system, with associated colours at each point. The coord
  field specifies a SoVRMLCoordinate node (or instance of a Coordinate
  node). The results are undefined if the coord field specifies any
  other type of node. PointSet uses the coordinates in order. If the
  coord field is NULL, the point set is considered empty.  PointSet
  nodes are not lit, not texture-mapped, nor do they participate in
  collision detection. The size of each point is implementation-
  dependent.  If the color field is not NULL, it shall specify a
  SoVRMLColor node that contains at least the number of points
  contained in the coord node. The results are undefined if the color
  field specifies any other type of node. Colours shall be applied to
  each point in order. The results are undefined if the number of
  values in the Color node is less than the number of values specified
  in the Coordinate node.  If the color field is NULL and there is a
  SoVRMLMaterial node defined for the SoVRMLAppearance node affecting
  this PointSet node, the emissiveColor of the Material node shall be
  used to draw the points. More details on lighting equations can be
  found in 4.14, Lighting model
  (<http://www.web3d.org/technicalinfo/specifications/vrml97/part1/concepts.html#4.14>).

*/

#include <Inventor/VRMLnodes/SoVRMLPointSet.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/VRMLnodes/SoVRMLCoordinate.h>
#include <Inventor/nodes/SoSubNodeP.h>

#include <Inventor/misc/SoState.h>
#include <Inventor/bundles/SoTextureCoordinateBundle.h>
#include <Inventor/SoPrimitiveVertex.h>

#include <Inventor/actions/SoGLRenderAction.h>
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H
#include <Inventor/system/gl.h>

#include <Inventor/nodes/SoVertexProperty.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/elements/SoGLCoordinateElement.h>
#include <Inventor/elements/SoGLTextureEnabledElement.h>
#include <Inventor/elements/SoGLTexture3EnabledElement.h>
#include <Inventor/elements/SoNormalBindingElement.h>
#include <Inventor/elements/SoMaterialBindingElement.h>
#include <Inventor/elements/SoGLLazyElement.h>
#include <Inventor/bundles/SoMaterialBundle.h>
#include <Inventor/caches/SoNormalCache.h>
#include <Inventor/details/SoPointDetail.h>
#include <Inventor/caches/SoBoundingBoxCache.h>
#include <Inventor/misc/SoGL.h>
#include <Inventor/SbColor.h>
#include <Inventor/SbColor4f.h>
#include <Inventor/elements/SoOverrideElement.h>
#include <Inventor/elements/SoMaterialBindingElement.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG


static SbBool
is_material_per_vertex(SoVRMLPointSet * ps, SoState * state)
{
  if (SoOverrideElement::getMaterialBindingOverride(state)) {
    if (SoMaterialBindingElement::get(state) !=
        SoMaterialBindingElement::OVERALL) return TRUE;
    return FALSE;
  }
  return ps->color.getValue() != NULL;
}

SO_NODE_SOURCE(SoVRMLPointSet);

// Doc in parent
void
SoVRMLPointSet::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoVRMLPointSet, SO_VRML97_NODE_TYPE);
}

/*!
  Constructor.
*/
SoVRMLPointSet::SoVRMLPointSet(void)
{
  SO_VRMLNODE_INTERNAL_CONSTRUCTOR(SoVRMLPointSet);
}

/*!
  Destructor.
*/
SoVRMLPointSet::~SoVRMLPointSet()
{
}

// Doc in parent
void
SoVRMLPointSet::GLRender(SoGLRenderAction * action)
{
  SoState * state = action->getState();

  SoLazyElement::setLightModel(state, SoLazyElement::BASE_COLOR);
  SoGLTextureEnabledElement::set(state, this, FALSE);
  SoGLTexture3EnabledElement::set(state, this, FALSE);

  if (!this->shouldGLRender(action)) return;

  SoVRMLVertexPoint::GLRender(action);

  const SoCoordinateElement * coords;
  coords = SoCoordinateElement::getInstance(state);

  SoMaterialBundle mb(action);

  SbBool matpervertex = is_material_per_vertex(this, state);
  if (!matpervertex) {
    const SbColor & col = SoLazyElement::getEmissive(state);
    SbColor4f c(col[0], col[1], col[2], 1.0f);
    SoGLLazyElement::sendPackedDiffuse(state, c.getPackedValue());
  }
  else {
    mb.sendFirst();
  }

  sogl_render_pointset((SoGLCoordinateElement*) coords,
                       NULL,
                       matpervertex ? &mb : NULL,
                       NULL,
                       coords->getNum(), 0);


  // send approx number of points for autocache handling
  sogl_autocache_update(state, coords->getNum()); 
}

// Doc in parent
void
SoVRMLPointSet::getBoundingBox(SoGetBoundingBoxAction * action)
{
  inherited::getBoundingBox(action);
  // notify open (if any) bbox caches about points in this shape
  SoBoundingBoxCache::setHasLinesOrPoints(action->getState());
}

// Doc in parent
void
SoVRMLPointSet::generatePrimitives(SoAction * action)
{
  SoVRMLCoordinate * coordnode = (SoVRMLCoordinate*) this->coord.getValue();
  if (!coordnode || coordnode->point.getNum() == 0) return;
  SoNode * colornode = this->color.getValue();

  const SbVec3f * coords = coordnode->point.getValues(0);

  SoPrimitiveVertex vertex;
  SoPointDetail pointDetail;
  vertex.setDetail(&pointDetail);

  int32_t numpts = coordnode->point.getNum();

  int matnr = 0;
  int idx = 0;

  SbBool matpervertex = is_material_per_vertex(this, action->getState());

  this->beginShape(action, SoShape::POINTS);
  for (int i = 0; i < numpts; i++) {
    if (matpervertex) {
      pointDetail.setMaterialIndex(matnr);
      vertex.setMaterialIndex(matnr++);
    }
    pointDetail.setCoordinateIndex(idx);
    vertex.setPoint(coords[idx++]);
    this->shapeVertex(&vertex);
  }
  this->endShape();
}
