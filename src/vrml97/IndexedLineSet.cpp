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
  \class SoVRMLIndexedLineSet SoVRMLIndexedLineSet.h Inventor/VRMLnodes/SoVRMLIndexedLineSet.h
  \brief The SoVRMLIndexedLineSet class is used to represent a generic 3D line shape.
  \ingroup VRMLnodes

  \WEB3DCOPYRIGHT

  \verbatim
  IndexedLineSet {
    eventIn       MFInt32 set_colorIndex
    eventIn       MFInt32 set_coordIndex
    exposedField  SFNode  color             NULL
    exposedField  SFNode  coord             NULL
    field         MFInt32 colorIndex        []     # [-1, inf)
    field         SFBool  colorPerVertex    TRUE
    field         MFInt32 coordIndex        []     # [-1, inf)
  }
  \endverbatim

  The IndexedLineSet node represents a 3D geometry formed by
  constructing polylines from 3D vertices specified in the coord
  field. IndexedLineSet uses the indices in its coordIndex field to
  specify the polylines by connecting vertices from the coord
  field. An index of "- 1" indicates that the current polyline has
  ended and the next one begins. The last polyline may be (but does
  not have to be) followed by a "- 1".  IndexedLineSet is specified in
  the local coordinate system and is affected by the transformations
  of its ancestors.  

  The coord field specifies the 3D vertices of the line set and
  contains a Coordinate node.  Lines are not lit, are not
  texture-mapped, and do not participate in collision detection. The
  width of lines is implementation dependent and each line segment is
  solid (i.e., not dashed).  If the color field is not NULL, it shall
  contain a Color node.  The colours are applied to the line(s) as
  follows: 
  
  - If colorPerVertex is FALSE:

    - If the colorIndex field is not empty, one colour is used for
      each polyline of the IndexedLineSet. There shall be at least as
      many indices in the colorIndex field as there are polylines in the
      IndexedLineSet.  If the greatest index in the colorIndex field is
      N, there shall be N+1 colours in the Color node. The colorIndex
      field shall not contain any negative entries.
   
    - If the colorIndex field is empty, the colours from the Color
      node are applied to each polyline of the IndexedLineSet in
      order. There shall be at least as many colours in the Color node
      as there are polylines.  

  - If colorPerVertex is TRUE:

    - If the colorIndex field is not empty, colours are applied to
      each vertex of the IndexedLineSet in exactly the same manner that
      the coordIndex field is used to supply coordinates for each vertex
      from the Coordinate node. The colorIndex field shall contain at
      least as many indices as the coordIndex field and shall contain
      end-of-polyline markers (-1) in exactly the same places as the
      coordIndex field.  If the greatest index in the colorIndex field
      is N, there shall be N+1 colours in the Color node.
   
    - If the colorIndex field is empty, the coordIndex field is used
      to choose colours from the Color node. If the greatest index in
      the coordIndex field is N, there shall be N+1 colours in the Color
      node.  

  If the color field is NULL and there is a Material defined for the
  Appearance affecting this IndexedLineSet, the emissiveColor of the
  Material shall be used to draw the lines. Details on lighting
  equations as they affect IndexedLineSet nodes are described in 4.14,
  Lighting model
  (<http://www.web3d.org/technicalinfo/specifications/vrml97/part1/concepts.html#4.14>).  

*/

#include <Inventor/VRMLnodes/SoVRMLIndexedLineSet.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/nodes/SoSubNodeP.h>

#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/caches/SoNormalCache.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/misc/SoGL.h>
#include <Inventor/bundles/SoMaterialBundle.h>

#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/misc/SoGL.h>
#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H
#include <Inventor/system/gl.h>

#include <Inventor/actions/SoGetPrimitiveCountAction.h>

#include <Inventor/elements/SoNormalBindingElement.h>
#include <Inventor/elements/SoMaterialBindingElement.h>
#include <Inventor/elements/SoCoordinateElement.h>
#include <Inventor/elements/SoTextureCoordinateBindingElement.h>
#include <Inventor/elements/SoDrawStyleElement.h>
#include <Inventor/elements/SoGLTextureEnabledElement.h>
#include <Inventor/elements/SoGLTexture3EnabledElement.h>
#include <Inventor/elements/SoGLCoordinateElement.h>
#include <Inventor/elements/SoLazyElement.h>
#include <assert.h>
#include <Inventor/bundles/SoTextureCoordinateBundle.h>
#include <Inventor/details/SoLineDetail.h>
#include <Inventor/caches/SoBoundingBoxCache.h>
#include <Inventor/SbColor4f.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif


SO_NODE_SOURCE(SoVRMLIndexedLineSet);

void
SoVRMLIndexedLineSet::initClass(void) // static
{
  SO_NODE_INTERNAL_INIT_CLASS(SoVRMLIndexedLineSet, SO_VRML97_NODE_TYPE);
}

SoVRMLIndexedLineSet::SoVRMLIndexedLineSet(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoVRMLIndexedLineSet);
}

SoVRMLIndexedLineSet::~SoVRMLIndexedLineSet()
{
}

SoVRMLIndexedLineSet::Binding
SoVRMLIndexedLineSet::findMaterialBinding(void) const
{
  Binding binding = OVERALL;
  if (this->color.getValue()) {
    if (this->colorPerVertex.getValue()) {
      binding = PER_VERTEX_INDEXED;
      if (!this->colorIndex.getNum()) binding = PER_VERTEX;
    }
    else {
      binding = PER_LINE;
      if (this->colorIndex.getNum()) binding = PER_LINE_INDEXED;
    }
  }
  return binding;
}

void
SoVRMLIndexedLineSet::GLRender(SoGLRenderAction * action)
{
  if (this->coordIndex.getNum() < 2) return;
  
  SoState * state = action->getState();
  
  SoLazyElement::setLightModel(state, SoLazyElement::BASE_COLOR);
  SoGLTextureEnabledElement::set(state, this, FALSE);
  SoGLTexture3EnabledElement::set(state, this, FALSE);

  if (!this->shouldGLRender(action)) 
    return;

  SoVRMLVertexLine::GLRender(action);

  // If the coordIndex field is invalid by not including the
  // terminating -1, fix the field by adding it.
  //
  // (FIXME: this is a bit naughty, as we change a field without
  // warning from within the library code. Should really see if we
  // could find a better solution -- which also goes for the other
  // nodes using coordinate index fields, of course. 20010104
  // mortene.)

  if (this->coordIndex.getNum() && this->coordIndex[this->coordIndex.getNum()-1] >= 0) {
    this->coordIndex.set1Value(coordIndex.getNum(), -1);
  }

  const SoCoordinateElement * coords;
  const int32_t * cindices;
  int32_t numindices;
  const int32_t * mindices;

  coords = SoCoordinateElement::getInstance(state);
  cindices = this->coordIndex.getValues(0);
  numindices = this->coordIndex.getNum();
  mindices = this->colorIndex.getNum() ? this->colorIndex.getValues(0) : NULL;

  Binding mbind = this->findMaterialBinding();
  if (mbind == PER_VERTEX) {
    mbind = PER_VERTEX_INDEXED;
    mindices = cindices;
  }

  SbBool drawPoints =
    SoDrawStyleElement::get(state) == SoDrawStyleElement::POINTS;

  // place it here so that it will stay in stack scope
  uint32_t packedcolor;

  if (this->color.getValue() == NULL) {
    // FIXME: the vrml97 spec states that the emissiveColor should be
    // used when no color node is found, but this doesn't work for
    // some Hydro models, since diffuseColor is used to set the color
    // of lines. It might be a bug in the VRML2 exporter, but for now
    // I just add emissive and diffuse to support this. pederb,
    // 2001-11-21
    SbColor col = SoLazyElement::getEmissive(state);
    const SbColor & col2 = SoLazyElement::getDiffuse(state, 0);
    
    col += col2;
    
    SbColor4f c(SbClamp(col[0], 0.0f, 1.0f), 
                SbClamp(col[1], 0.0f, 1.0f),
                SbClamp(col[2], 0.0f, 1.0f),
                1.0f);
    packedcolor = col.getPackedValue();
    SoLazyElement::setPacked(state, this, 1, &packedcolor);
  }

  SoMaterialBundle mb(action);
  mb.sendFirst(); // make sure we have the correct material

  sogl_render_lineset((SoGLCoordinateElement*)coords,
                      cindices,
                      numindices,
                      NULL,
                      NULL,
                      &mb,
                      mindices,
                      NULL, 0,
                      0,
                      (int)mbind,
                      0,
                      drawPoints ? 1 : 0);
  // send approx number of triangles for autocache handling
  sogl_autocache_update(state, this->coordIndex.getNum() / 2);
}

void
SoVRMLIndexedLineSet::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  if (!this->shouldPrimitiveCount(action)) return;

  int n = this->coordIndex.getNum();
  if (n < 2) return;

  if (action->canApproximateCount()) {
    action->addNumLines(n/3);
  }
  else {
    const int32_t * ptr = coordIndex.getValues(0);
    const int32_t * endptr = ptr + n;
    int cnt = 0;
    int add = 0;
    while (ptr < endptr) {
      if (*ptr++ >= 0) cnt++;
      else {
        add += cnt-1;
        cnt = 0;
      }
    }
    // in case index array wasn't terminated by a -1
    if (cnt >= 2) add += cnt-1;
    action->addNumLines(add);
  }
}

void
SoVRMLIndexedLineSet::getBoundingBox(SoGetBoundingBoxAction * action)
{
  inherited::getBoundingBox(action);
  // notify open (if any) bbox caches about lines in this shape
  SoBoundingBoxCache::setHasLinesOrPoints(action->getState());
}

void
SoVRMLIndexedLineSet::generatePrimitives(SoAction * action)
{
  if (this->coordIndex.getNum() < 2) return;

  SoState * state = action->getState();
  state->push();

  SoVRMLVertexLine::doAction(action);

  const SoCoordinateElement * coords;
  int32_t numindices;
  const int32_t * cindices;
  const int32_t * matindices;

  coords = SoCoordinateElement::getInstance(state);
  cindices = this->coordIndex.getValues(0);
  numindices = this->coordIndex.getNum();
  matindices = this->colorIndex.getNum() ? this->colorIndex.getValues(0) : NULL;

  Binding mbind = this->findMaterialBinding();
  if (mbind == PER_VERTEX) {
    mbind = PER_VERTEX_INDEXED;
    matindices = cindices;
  }

  if (mbind == PER_LINE || mbind == OVERALL) {
    matindices = NULL;
  }

  int matnr = 0;
  int32_t i;
  const int32_t *end = cindices + numindices;

  SoPrimitiveVertex vertex;
  SoPointDetail pointDetail;
  SoLineDetail lineDetail;
  vertex.setDetail(&pointDetail);

  while (cindices < end) {
    this->beginShape(action, LINE_STRIP, &lineDetail);
    i = *cindices++;
    assert(i >= 0);
    if (matindices) {
      pointDetail.setMaterialIndex(*matindices);
      vertex.setMaterialIndex(*matindices++);
    }
    else if (mbind != OVERALL) {
      pointDetail.setMaterialIndex(matnr);
      vertex.setMaterialIndex(matnr++);
    }
    pointDetail.setCoordinateIndex(i);
    vertex.setPoint(coords->get3(i));
    this->shapeVertex(&vertex);

    i = *cindices++;
    assert(i >= 0);
    if (mbind >= PER_VERTEX) {
      if (matindices) vertex.setMaterialIndex(*matindices++);
      else vertex.setMaterialIndex(matnr++);
      pointDetail.setMaterialIndex(vertex.getMaterialIndex());
    }
    pointDetail.setCoordinateIndex(i);
    vertex.setPoint(coords->get3(i));
    this->shapeVertex(&vertex);
    lineDetail.incPartIndex();

    i = *cindices++;
    while (i >= 0) {
      assert(cindices <= end);
      if (mbind >= PER_VERTEX) {
        if (matindices) vertex.setMaterialIndex(*matindices++);
        else vertex.setMaterialIndex(matnr++);
        pointDetail.setMaterialIndex(vertex.getMaterialIndex());
      }
      pointDetail.setCoordinateIndex(i);
      vertex.setPoint(coords->get3(i));
      this->shapeVertex(&vertex);
      lineDetail.incPartIndex();
      i = *cindices++;
    }
    this->endShape(); // end of line strip
    if (mbind == PER_VERTEX_INDEXED) matindices++;
    lineDetail.incLineIndex();
  }
  state->pop();
}
