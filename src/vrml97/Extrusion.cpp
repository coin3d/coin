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
  \class SoVRMLExtrusion SoVRMLExtrusion.h Inventor/VRMLnodes/SoVRMLExtrusion.h
  \brief The SoVRMLExtrusion class is a a geometry node for extruding a cross section along a spine.
  \ingroup VRMLnodes

  \WEB3DCOPYRIGHT

  \verbatim
  Extrusion {
    eventIn MFVec2f    set_crossSection
    eventIn MFRotation set_orientation
    eventIn MFVec2f    set_scale
    eventIn MFVec3f    set_spine
    field   SFBool     beginCap         TRUE
    field   SFBool     ccw              TRUE
    field   SFBool     convex           TRUE
    field   SFFloat    creaseAngle      0                # [0,inf)
    field   MFVec2f    crossSection     [ 1 1, 1 -1, -1 -1, -1 1, 1  1 ]    # (-inf,inf)
    field   SFBool     endCap           TRUE
    field   MFRotation orientation      0 0 1 0          # [-1,1],(-inf,inf)
    field   MFVec2f    scale            1 1              # (0,inf)
    field   SFBool     solid            TRUE
    field   MFVec3f    spine            [ 0 0 0, 0 1 0 ] # (-inf,inf)
  }
  \endverbatim

  \e Introduction

  The Extrusion node specifies geometric shapes based on a two
  dimensional cross-section extruded along a three dimensional spine
  in the local coordinate system. The cross-section can be scaled and
  rotated at each spine point to produce a wide variety of shapes.  An
  Extrusion node is defined by:

  \li a 2D crossSection piecewise linear curve (described as a series
  of connected vertices);

  \li a 3D spine piecewise linear curve (also described as a series
  of connected vertices);

  \li a list of 2D scale parameters;

  \li a list of 3D orientation parameters.

  \e Algorithmic \e description

  Shapes are constructed as follows. The cross-section curve, which
  starts as a curve in the Y=0 plane, is first scaled about the origin
  by the first scale parameter (first value scales in X, second value
  scales in Z). It is then translated by the first spine point and
  oriented using the first orientation parameter (as explained
  later). The same procedure is followed to place a cross- section at
  the second spine point, using the second scale and orientation
  values. Corresponding vertices of the first and second
  cross-sections are then connected, forming a quadrilateral polygon
  between each pair of vertices. This same procedure is then repeated
  for the rest of the spine points, resulting in a surface extrusion
  along the spine.

  The final orientation of each cross-section is computed by first
  orienting it relative to the spine segments on either side of point
  at which the cross-section is placed. This is known as the
  spine-aligned cross-section plane (SCP), and is designed to provide
  a smooth transition from one spine segment to the next (see Figure
  6.6). The SCP is then rotated by the corresponding orientation
  value. This rotation is performed relative to the SCP. For example,
  to impart twist in the cross- section, a rotation about the Y-axis
  (0 1 0) would be used. Other orientations are valid and rotate the
  cross-section out of the SCP.

  <center>
  <img src="http://www.web3d.org/technicalinfo/specifications/vrml97/Images/Extrusion.gif">
  Figure 6.6
  </center>

  The SCP is computed by first computing its Y-axis and Z-axis, then
  taking the cross product of these to determine the X-axis. These
  three axes are then used to determine the rotation value needed to
  rotate the Y=0 plane to the SCP. This results in a plane that is the
  approximate tangent of the spine at each point, as shown in Figure
  6.6. First the Y-axis is determined, as follows:

  Let n be the number of spines and let i be the index variable
  satisfying 0 <= i < n:

  \li For all points other than the first or last: The Y-axis for
  spine[i] is found by normalizing the vector defined by (spine[i+1]
  - spine[i-1]).

  \li If the spine curve is closed: The SCP for the first and last
  points is the same and is found using (spine[1] - spine[n-2])
  to compute the Y-axis.

  \li If the spine curve is not closed: The Y-axis used for the
  first point is the vector from spine[0] to spine[1], and for the
  last it is the vector from spine[n-2] to spine[n-1].

  The Z-axis is determined as follows:

  \li For all points other than the first or last: Take the following
  cross-product:

  \verbatim
  Z = (spine[i+1] - spine[i]) × (spine[i-1] - spine[i])
  \endverbatim

  \li If the spine curve is closed: The SCP for the first and last
  points is the same and is found by taking the following cross- product:

  \verbatim
  Z = (spine[1] - spine[0]) × (spine[n-2] - spine[0])
  \endverbatim

  \li If the spine curve is not closed: The Z-axis used for the first
  spine point is the same as the Z-axis for spine[1]. The Z- axis used for
  the last spine point is the same as the Z-axis for spine[n-2].

  \li After determining the Z-axis, its dot product with the Z-axis of the
  previous spine point is computed. If this value is negative, the
  Z-axis is flipped (multiplied by -1). In most cases, this prevents
  small changes in the spine segment angles from flipping the
  cross-section 180 degrees.

  Once the Y- and Z-axes have been computed, the X-axis can be
  calculated as their cross-product.

  \e Special \e Cases

  If the number of scale or orientation values is greater than the
  number of spine points, the excess values are ignored. If they
  contain one value, it is applied at all spine points. The results
  are undefined if the number of scale or orientation values is
  greater than one but less than the number of spine points. The scale
  values shall be positive.

  If the three points used in computing the Z-axis are collinear, the
  cross-product is zero so the value from the previous point is used
  instead.  If the Z-axis of the first point is undefined (because the
  spine is not closed and the first two spine segments are collinear)
  then the Z-axis for the first spine point with a defined Z-axis is
  used.

  If the entire spine is collinear, the SCP is computed by finding the
  rotation of a vector along the positive Y-axis (v1) to the vector
  formed by the spine points (v2). The Y=0 plane is then rotated by
  this value.  If two points are coincident, they both have the same
  SCP. If each point has a different orientation value, then the
  surface is constructed by connecting edges of the cross-sections as
  normal. This is useful in creating revolved surfaces.

  Note: combining coincident and non-coincident spine segments, as
  well as other combinations, can lead to interpenetrating surfaces
  which the extrusion algorithm makes no attempt to avoid.

  \e Common \e Cases

  The following common cases are among the effects which are supported
  by the Extrusion node:

  \li Surfaces of revolution: If the cross-section is an approximation
  of a circle and the spine is straight, the Extrusion is equivalent
  to a surface of revolution, where the scale parameters define the
  size of the cross-section along the spine.

  \li Uniform extrusions: If the scale is (1, 1) and the spine is
  straight, the cross-section is extruded uniformly without twisting
  or scaling along the spine. The result is a cylindrical shape with a
  uniform cross section.

  \li Bend/twist/taper objects: These shapes are the result of using
  all fields. The spine curve bends the extruded shape defined by the
  cross-section, the orientation parameters (given as rotations about
  the Y-axis) twist it around the spine, and the scale parameters
  taper it (by scaling about the spine).

  \e Other \e Fields

  Extrusion has three parts: the sides, the beginCap (the
  surface at the initial end of the spine) and the endCap (the surface
  at the final end of the spine). The caps have an associated SFBool field
  that indicates whether each exists (TRUE) or doesn't exist (FALSE).

  When the beginCap or endCap fields are specified as TRUE, planar cap
  surfaces will be generated regardless of whether the crossSection is
  a closed curve. If crossSection is not a closed curve, the caps are
  generated by adding a final point to crossSection that is equal to
  the initial point. An open surface can still have a cap, resulting
  (for a simple case) in a shape analogous to a soda can sliced in
  half vertically.  These surfaces are generated even if spine is also
  a closed curve.  If a field value is FALSE, the corresponding cap is
  not generated.

  Texture coordinates are automatically generated by Extrusion
  nodes. Textures are mapped so that the coordinates range in the U
  direction from 0 to 1 along the crossSection curve (with 0
  corresponding to the first point in crossSection and 1 to the last)
  and in the V direction from 0 to 1 along the spine curve (with 0
  corresponding to the first listed spine point and 1 to the last). If
  either the endCap or beginCap exists, the crossSection curve is
  uniformly scaled and translated so that the larger dimension of the
  cross-section (X or Z) produces texture coordinates that range from
  0.0 to 1.0. The beginCap and endCap textures' S and T directions
  correspond to the X and Z directions in which the crossSection
  coordinates are defined.

  The browser shall automatically generate normals for the Extrusion
  node,using the creaseAngle field to determine if and how normals are
  smoothed across the surface. Normals for the caps are generated
  along the Y-axis of the SCP, with the ordering determined by viewing
  the cross-section from above (looking along the negative Y-axis of
  the SCP). By default, a beginCap with a counterclockwise ordering
  shall have a normal along the negative Y-axis. An endCap with a
  counterclockwise ordering shall have a normal along the positive
  Y-axis.

  Each quadrilateral making up the sides of the extrusion are ordered
  from the bottom cross-section (the one at the earlier spine point)
  to the top.  So, one quadrilateral has the points:

  \verbatim
  spine[0](crossSection[0], crossSection[1])
  spine[1](crossSection[1], crossSection[0])
  \endverbatim

  in that order. By default, normals for the sides are generated as
  described in 4.6.3, Shapes and geometry
  (http://www.web3d.org/technicalinfo/specifications/vrml97/part1/concepts.html#4.6.3).

  For instance, a circular crossSection with counter-clockwise
  ordering and the default spine form a cylinder. With solid TRUE and
  ccw TRUE, the cylinder is visible from the outside. Changing ccw to
  FALSE makes it visible from the inside.  The ccw, solid, convex, and
  creaseAngle fields are described in 4.6.3, Shapes and geometry
  (http://www.web3d.org/technicalinfo/specifications/vrml97/part1/concepts.html#4.6.3).

*/

/*!
  SoSFBool SoVRMLExtrusion::beginCap
  Used to enable/disable begin cap. Default value is TRUE.
*/

/*!
  SoSFBool SoVRMLExtrusion::ccw
  Specifies counterclockwise vertex ordering. Default value is TRUE.
*/

/*!
  SoSFBool SoVRMLExtrusion::convex
  Specifies if cross sections is convex. Default value is TRUE.
*/

/*!
  SoSFFloat SoVRMLExtrusion::creaseAngle
  Specifies the crease angle for the generated normals. Default value is 0.0.
*/

/*!
  SoMFVec2f SoVRMLExtrusion::crossSection
  The cross section.
*/

/*!
  SoSFBool SoVRMLExtrusion::endCap
  Used to enable/disable end cap. Default value is TRUE.

*/

/*!
  SoMFRotation SoVRMLExtrusion::orientation
  Orientation for the cross section at each spine point.
*/

/*!
  SoMFVec2f SoVRMLExtrusion::scale
  Scaling for the cross section at each spine point.
*/

/*!
  SoSFBool SoVRMLExtrusion::solid
  When TRUE, backface culling will be enabled. Default value is TRUE.
*/

/*!
  SoMFVec3f SoVRMLExtrusion::spine
  The spine points.
*/


#include <Inventor/VRMLnodes/SoVRMLExtrusion.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/lists/SbList.h>
#include <Inventor/misc/SoNormalGenerator.h>
#include <Inventor/bundles/SoMaterialBundle.h>
#include <Inventor/bundles/SoTextureCoordinateBundle.h>
#include <Inventor/elements/SoCoordinateElement.h>
#include <Inventor/elements/SoTextureCoordinateElement.h>
#include <Inventor/elements/SoGLTextureEnabledElement.h>
#include <Inventor/SbTesselator.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/misc/SoGL.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/errors/SoDebugError.h>
#include <float.h>
#include <math.h>


//
// needed to avoid warnings generated by SbVec3f::normalize
//
static float
my_normalize(SbVec3f & vec)
{
  float len = vec.length();
  if (len > FLT_EPSILON) {
    vec /= len;
  }
  return len;
}


#ifndef DOXYGEN_SKIP_THIS
class SoVRMLExtrusionP {
public:
  SoVRMLExtrusionP(SoVRMLExtrusion * master)
    :master(master),
     coord(32),
     tcoord(32),
     idx(32),
     gen(TRUE),
     tess(tess_callback, this),
     dirty(TRUE)
  {
  }

  SoVRMLExtrusion * master;
  SbList <SbVec3f> coord;
  SbList <SbVec2f> tcoord;
  SbList <int> idx;
  SoNormalGenerator gen;
  SbTesselator tess;
  SbBool dirty;

  static void tess_callback(void *, void *, void *, void *);
  void generateCoords(void);
  void generateNormals(void);
};
#endif // DOXYGEN_SKIP_THIS

#undef THIS
#define THIS this->pimpl


SO_NODE_SOURCE(SoVRMLExtrusion);

// Doc in parent
void
SoVRMLExtrusion::initClass(void) // static
{
  SO_NODE_INTERNAL_INIT_CLASS(SoVRMLExtrusion, SO_VRML97_NODE_TYPE);
}

/*!
  Constructor.
*/
SoVRMLExtrusion::SoVRMLExtrusion(void)
{
  THIS = new SoVRMLExtrusionP(this);

  SO_NODE_INTERNAL_CONSTRUCTOR(SoVRMLExtrusion);

  SO_VRMLNODE_ADD_FIELD(beginCap, (TRUE));
  SO_VRMLNODE_ADD_FIELD(endCap, (TRUE));
  SO_VRMLNODE_ADD_FIELD(solid, (TRUE));
  SO_VRMLNODE_ADD_FIELD(ccw, (TRUE));
  SO_VRMLNODE_ADD_FIELD(convex, (TRUE));
  SO_VRMLNODE_ADD_FIELD(creaseAngle, (0.0f));

  SO_NODE_ADD_FIELD(crossSection, (0.0f, 0.0f));
  this->crossSection.setNum(5);
  SbVec2f * cs = this->crossSection.startEditing();
  cs[0] = SbVec2f(1.0f, 1.0f);
  cs[1] = SbVec2f(1.0f, -1.0f);
  cs[2] = SbVec2f(-1.0f, -1.0f);
  cs[3] = SbVec2f(-1.0f, 1.0f);
  cs[4] = SbVec2f(1.0f, 1.0f);
  this->crossSection.finishEditing();
  this->crossSection.setDefault(TRUE);

  SO_NODE_ADD_FIELD(orientation, (SbRotation::identity()));
  SO_NODE_ADD_FIELD(scale, (1.0f, 1.0f));

  SO_NODE_ADD_FIELD(spine, (0.0f, 0.0f, 0.0f));
  this->spine.setNum(2);
  this->spine.set1Value(1, 0.0f, 1.0f, 0.0f);
  this->spine.setDefault(TRUE);
}

/*!
  Destructor.
*/
SoVRMLExtrusion::~SoVRMLExtrusion()
{
  delete THIS;
}


// Doc in parent
void
SoVRMLExtrusion::GLRender(SoGLRenderAction * action)
{
  if (!this->shouldGLRender(action)) return;

  this->updateCache();

  SoState * state = action->getState();

  SbBool doTextures = SoGLTextureEnabledElement::get(state);
  const SbVec3f * normals = THIS->gen.getNormals();

  SoCoordinateElement::set3(state, this, THIS->coord.getLength(), THIS->coord.getArrayPtr());
  const SoCoordinateElement * coords = SoCoordinateElement::getInstance(state);

  if (doTextures) {
    SoTextureCoordinateElement::set2(state, this, THIS->tcoord.getLength(),
                                     THIS->tcoord.getArrayPtr());
  }

  SoTextureCoordinateBundle tb(action, TRUE, FALSE);
  doTextures = tb.needCoordinates();

  SoMaterialBundle mb(action);
  mb.sendFirst();

  this->setupShapeHints(state, this->ccw.getValue(), this->solid.getValue());

  sogl_render_faceset((SoGLCoordinateElement *) coords,
                      THIS->idx.getArrayPtr(),
                      THIS->idx.getLength(),
                      normals,
                      NULL,
                      &mb,
                      NULL,
                      &tb,
                      THIS->idx.getArrayPtr(),
                      3, /* SoIndexedFaceSet::PER_VERTEX */
                      0,
                      doTextures?1:0);
}

// Doc in parent
void
SoVRMLExtrusion::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  this->updateCache();
}

// Doc in parent
void
SoVRMLExtrusion::computeBBox(SoAction * action,
                             SbBox3f & box,
                             SbVec3f & center)
{
  this->updateCache();

  int num = THIS->coord.getLength();
  const SbVec3f * coords = THIS->coord.getArrayPtr();

  box.makeEmpty();
  while (num--) {
    box.extendBy(*coords++);
  }
  if (!box.isEmpty()) center = box.getCenter();

}

// Doc in parent
void
SoVRMLExtrusion::generatePrimitives(SoAction * action)
{
  this->updateCache();

  const SbVec3f * normals = THIS->gen.getNormals();
  const SbVec2f * tcoords = THIS->tcoord.getArrayPtr();
  const SbVec3f * coords = THIS->coord.getArrayPtr();
  const int32_t * iptr = THIS->idx.getArrayPtr();
  const int32_t * endptr = iptr + THIS->idx.getLength();

  SoPrimitiveVertex vertex;

  int idx;
  this->beginShape(action, TRIANGLES);
  while (iptr < endptr) {
    idx = *iptr++;
    while (idx >= 0) {
      vertex.setNormal(*normals++);
      vertex.setTextureCoords(tcoords[idx]);
      vertex.setPoint(coords[idx]);
      this->shapeVertex(&vertex);
      idx = *iptr++;
    }
  }
  this->endShape();
}

//
// private method that updates the coordinate and normal cache.
//
void
SoVRMLExtrusion::updateCache(void)
{
  if (THIS->dirty) {
    THIS->generateCoords();
    THIS->generateNormals();
    THIS->dirty = FALSE;
  }
}

// Doc in parent
void
SoVRMLExtrusion::notify(SoNotList * list)
{
  THIS->dirty = TRUE;
  inherited::notify(list);
}


// Doc in parent
SoDetail *
SoVRMLExtrusion::createTriangleDetail(SoRayPickAction * action,
                                      const SoPrimitiveVertex * v1,
                                      const SoPrimitiveVertex * v2,
                                      const SoPrimitiveVertex * v3,
                                      SoPickedPoint * pp)
{
  // no triangle detail for Extrusion
  return NULL;
}

#undef THIS
#ifndef DOXYGEN_SKIP_THIS


static SbVec3f
calculate_y_axis(const SbVec3f * spine, const int i,
                 const int numspine, const SbBool closed)
{
  SbVec3f Y;
  if (closed) {
    if (i > 0) {
      Y = spine[i+1] - spine[i-1];
    }
    else {
      Y = spine[1] - spine[numspine-1];
    }
  }
  else {
    if (i == 0) Y = spine[1] - spine[0];
    else if (i == numspine-1) Y = spine[numspine-1] - spine[numspine-2];
    else Y = spine[i+1] - spine[i-1];
  }
  my_normalize(Y);
  return Y;
}

static SbVec3f
calculate_z_axis(const SbVec3f * spine, const int i,
                 const int numspine, const SbBool closed)
{
  SbVec3f z0, z1;

  if (closed) {
    if (i > 0) {
      z0 = spine[i+1] - spine[i];
      z1 = spine[i-1] - spine[i];
    }
    else {
      z0 = spine[1] - spine[0];
      z1 = spine[numspine-1] - spine[0];
    }
  }
  else {
    if (numspine == 2) return SbVec3f(0.0f, 0.0f, 0.0f);
    else if (i == 0) {
      z0 = spine[2] - spine[1];
      z1 = spine[0] - spine[1];
    }
    else if (i == numspine-1) {
      z0 = spine[numspine-1] - spine[numspine-2];
      z1 = spine[numspine-3] - spine[numspine-2];
    }
    else {
      z0 = spine[i+1] - spine[i];
      z1 = spine[i-1] - spine[i];
    }
  }

  my_normalize(z0);
  my_normalize(z1);

  // test if spine segments are collinear. If they are, the cross
  // product will not be reliable, and we should just use the previous
  // Z-axis instead.
  if (SbAbs(z0.dot(z1)) > 0.999f) {
    return SbVec3f(0.0f, 0.0f, 0.0f);
  }
  SbVec3f tmp = z0.cross(z1);
  tmp.normalize();
  return tmp;
}

//
// generates extruded coordinates
//
void
SoVRMLExtrusionP::generateCoords(void)
{
  this->coord.truncate(0);
  this->tcoord.truncate(0);
  this->idx.truncate(0);

  if (this->master->crossSection.getNum() == 0 ||
      this->master->spine.getNum() == 0) return;

  SbMatrix matrix = SbMatrix::identity();

  int i, j, numcross;
  SbBool connected = FALSE;   // is cross section closed
  SbBool closed = FALSE;      // is spine closed
  numcross = this->master->crossSection.getNum();
  const SbVec2f * cross =  master->crossSection.getValues(0);
  if (cross[0] == cross[numcross-1]) {
    connected = TRUE;
    numcross--;
  }

  int numspine = master->spine.getNum();
  const SbVec3f * spine = master->spine.getValues(0);
  if (spine[0] == spine[numspine-1]) {
    closed = TRUE;
    numspine--;
  }

  SbVec3f prevY(0.0f, 0.0f, 0.0f);
  SbVec3f prevZ(0.0f, 0.0f, 0.0f);
  const SbVec3f empty(0.0f, 0.0f, 0.0f);

  SbBool colinear = FALSE;
  SbVec3f X, Y, Z;

  // find first non-collinear spine segments and calculate the first
  // valid Y and Z axis
  for (i = 0; i < numspine && (prevY == empty || prevZ == empty); i++) {
    if (prevY == empty) {
      Y = calculate_y_axis(spine, i, numspine, closed);
      if (Y != empty) prevY = Y;
    }
    if (prevZ == empty) {
      Z = calculate_z_axis(spine, i, numspine, closed);
      if (Z != empty) prevZ = Z;
    }
  }

  if (prevY == empty) prevY = SbVec3f(0.0f, 1.0f, 0.0f);
  if (prevZ == empty) { // all spine segments are colinear, calculate constant Z axis
    prevZ = SbVec3f(0.0f, 0.0f, 1.0f);
    if (prevY != SbVec3f(0.0f, 1.0f, 0.0f)) {
      SbRotation rot(SbVec3f(0.0f, 1.0f, 0.0f), prevY);
      rot.multVec(prevZ, prevZ);
    }
    colinear = TRUE;
  }

  int numorient = this->master->orientation.getNum();
  const SbRotation * orient = this->master->orientation.getValues(0);

  int numscale = this->master->scale.getNum();
  const SbVec2f * scale = this->master->scale.getValues(0);

  // loop through all spines
  for (i = 0; i < numspine; i++) {
    if (colinear) {
      Y = prevY;
      Z = prevZ;
    }
    else {
      Y = calculate_y_axis(spine, i, numspine, closed);
      if (Y == empty) Y = prevY;
      Z = calculate_z_axis(spine, i, numspine, closed);
      if (Z == empty) Z = prevZ;

      if (Z.dot(prevZ) < 0) Z = -Z;
    }

    X = Y.cross(Z);
    my_normalize(X);

    prevY = Y;
    prevZ = Z;

    matrix[0][0] = X[0];
    matrix[0][1] = X[1];
    matrix[0][2] = X[2];
    matrix[0][3] = 0.0f;

    matrix[1][0] = Y[0];
    matrix[1][1] = Y[1];
    matrix[1][2] = Y[2];
    matrix[1][3] = 0.0f;

    matrix[2][0] = Z[0];
    matrix[2][1] = Z[1];
    matrix[2][2] = Z[2];
    matrix[2][3] = 0.0f;

    matrix[3][0] = spine[i][0];
    matrix[3][1] = spine[i][1];
    matrix[3][2] = spine[i][2];
    matrix[3][3] = 1.0f;

    if (numorient) {
      SbMatrix rmat;
      orient[SbMin(i, numorient-1)].getValue(rmat);
      matrix.multLeft(rmat);
    }

    if (numscale) {
      SbMatrix smat = SbMatrix::identity();
      SbVec2f s = scale[SbMin(i, numscale-1)];
      smat[0][0] = s[0];
      smat[2][2] = s[1];
      matrix.multLeft(smat);
    }

    for (j = 0; j < numcross; j++) {
      SbVec3f c;
      c[0] = cross[j][0];
      c[1] = 0.0f;
      c[2] = cross[j][1];

      matrix.multVecMatrix(c, c);
      this->coord.append(c);
      this->tcoord.append(SbVec2f(float(j)/float(connected ? numcross : numcross-1),
                                  float(i)/float(closed ? numspine : numspine-1)));
    }
  }

  // this macro makes the code below more readable
#define ADD_TRIANGLE(i0, j0, i1, j1, i2, j2) \
  do { \
    this->idx.append((i0)*numcross+(j0)); \
    this->idx.append((i2)*numcross+(j2)); \
    this->idx.append((i1)*numcross+(j1)); \
    this->idx.append(-1); \
  } while (0)

  // create beginCap polygon
  if (this->master->beginCap.getValue() && !closed) {
    if (this->master->convex.getValue()) {
      for (i = 1; i < numcross-1; i++) {
        ADD_TRIANGLE(0, 0, 0, i, 0, i+1);
      }
    }
    else {
      // let the tesselator create triangles
      this->tess.beginPolygon();
      for (i = 0; i < numcross; i++) {
        this->tess.addVertex(this->coord[i], (void*) i);
      }
      this->tess.endPolygon();
    }
  }


  // create endCap polygon
  if (this->master->endCap.getValue() && !closed) {
    if (this->master->convex.getValue()) {
      for (i = 1; i < numcross-1; i++) {
        ADD_TRIANGLE(numspine-1, numcross-1,
                     numspine-1, numcross-1-i,
                     numspine-1, numcross-2-i);
      }
    }
    else {
      // let the tesselator create triangles
      this->tess.beginPolygon();
      for (i = 0; i < numcross; i++) {
        int idx = (numspine-1)*numcross + numcross - 1 - i;
        this->tess.addVertex(this->coord[idx], (void*) idx);
      }
      this->tess.endPolygon();
    }
  }

  // create walls
  for (i = 0; i < numspine-1; i++) {
    for (j = 0; j < numcross-1; j++) {
      ADD_TRIANGLE(i, j, i+1, j, i+1, j+1);
      ADD_TRIANGLE(i, j, i+1, j+1, i, j+1);
    }
    if (connected) {
      ADD_TRIANGLE(i, j, i+1, j, i+1, 0);
      ADD_TRIANGLE(i, j, i+1, 0, i, 0);
    }
  }
  if (closed) {
    for (j = 0; j < numcross-1; j++) {
      ADD_TRIANGLE(numspine-1, j, 0, j, 0, j+1);
      ADD_TRIANGLE(numspine-1, j, 0, j+1, numspine-1, j+1);
    }
    if (connected) {
      ADD_TRIANGLE(numspine-1, j, 0, j, 0, 0);
      ADD_TRIANGLE(numspine-1, j, 0, 0, numspine-1, 0);
    }
  }
#undef ADD_TRIANGLE
}

//
// generates per-verex normals for the extrusion.
//
void
SoVRMLExtrusionP::generateNormals(void)
{
  this->gen.reset(this->master->ccw.getValue());
  const SbVec3f * c = this->coord.getArrayPtr();
  const int * iptr = this->idx.getArrayPtr();
  const int * endptr = iptr + this->idx.getLength();

  while (iptr < endptr) {
    this->gen.beginPolygon();
    int idx = *iptr++;
    while (idx >= 0) {
      this->gen.polygonVertex(c[idx]);
      idx = *iptr++;
    }
    this->gen.endPolygon();
  }
  this->gen.generate(this->master->creaseAngle.getValue());
}

//
// callback from the polygon tessellator
//
void
SoVRMLExtrusionP::tess_callback(void * v0, void * v1, void * v2, void * data)
{
  SoVRMLExtrusionP * thisp = (SoVRMLExtrusionP*) data;
  thisp->idx.append((int) v0);
  thisp->idx.append((int) v1);
  thisp->idx.append((int) v2);
  thisp->idx.append(-1);
}

#endif // DOXYGEN_SKIP_THIS
