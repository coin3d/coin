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
  \class SoVRMLElevationGrid SoVRMLElevationGrid.h Inventor/VRMLnodes/SoVRMLElevationGrid.h
  \brief The SoVRMLElevationGrid class is used to represent elevation grids.
*/

/*!
  \var SoSFBool SoVRMLElevationGrid::ccw
  Specifies whether generated triangles are counterclockwise. Default value is TRUE.
*/

/*!
  \var SoSFBool SoVRMLElevationGrid::solid
  Specifies whether backface culling should be done. Default value is TRUE.
*/

/*!
  \var SoSFFloat SoVRMLElevationGrid::creaseAngle
  Specifies the normal calculation crease angle.
*/

/*!
  \var SoSFInt32 SoVRMLElevationGrid::zDimension
  Specifies the number of points in the z dimension.
*/

/*!
  \var SoSFInt32 SoVRMLElevationGrid::xDimension
  Specifies the number of points in the x dimension.
*/

/*!
  \var SoSFFloat SoVRMLElevationGrid::zSpacing
  Specifies the spacing between points in the z dimension. Default value is 1.0.
*/

/*!
  \var SoSFFloat SoVRMLElevationGrid::xSpacing
  Specifies the spacing between points in the x dimension. Default value is 1.0.
*/

/*!
  \var SoMFFloat SoVRMLElevationGrid::height
  Contains the array of height values.
*/

/*!
  \var SoSFNode SoVRMLElevationGrid::texCoord
  Can contain an SoVRMLTextureCoordinate node containing one texture coordinate per grid point.
*/

/*!
  \var SoSFNode SoVRMLElevationGrid::normal
  Can contain an SoVRMLTextureCoordinate node containing normals for the grid.
*/

/*!
  \var SoSFNode SoVRMLElevationGrid::color
  Can contain an SoVRMLColor node containing grid colors.
*/

/*!
  \var SoSFBool SoVRMLElevationGrid::colorPerVertex
  Specifies whether colors should be applied per vertex. Default value is TRUE.
*/

/*!
  \var SoSFBool SoVRMLElevationGrid::normalPerVertex
  Specifies whether normals should be applied per vertex. Default value is TRUE.
*/


#include <Inventor/VRMLnodes/SoVRMLElevationGrid.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/bundles/SoMaterialBundle.h>
#include <Inventor/elements/SoLightModelElement.h>
#include <Inventor/elements/SoGLTextureEnabledElement.h>
#include <Inventor/elements/SoGLShadeModelElement.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/VRMLnodes/SoVRMLColor.h>
#include <Inventor/VRMLnodes/SoVRMLNormal.h>
#include <Inventor/VRMLnodes/SoVRMLTextureCoordinate.h>
#include <Inventor/misc/SoNormalGenerator.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/details/SoFaceDetail.h>
#include <Inventor/details/SoPointDetail.h>
#include <Inventor/errors/SoDebugError.h>
#include <float.h>
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H
#include <Inventor/system/gl.h>



#ifndef DOXYGEN_SKIP_THIS

class SoVRMLElevationGridP {
public:
  SoVRMLElevationGridP(void)
    : dirty(TRUE),
      ngen(TRUE)
  { }

  SbBool dirty;
  SoNormalGenerator ngen;
  SoVRMLElevationGrid::Binding nbind;
};

#endif // DOXYGEN_SKIP_THIS

#undef THIS
#define THIS this->pimpl

SO_NODE_SOURCE(SoVRMLElevationGrid);

// Doc in parent
void
SoVRMLElevationGrid::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoVRMLElevationGrid, SO_VRML97_NODE_TYPE);
}

/*!
  Constructor.
*/
SoVRMLElevationGrid::SoVRMLElevationGrid(void)
{
  THIS = new SoVRMLElevationGridP;

  SO_NODE_INTERNAL_CONSTRUCTOR(SoVRMLElevationGrid);

  SO_VRMLNODE_ADD_FIELD(ccw, (TRUE));
  SO_VRMLNODE_ADD_FIELD(solid, (TRUE));
  SO_VRMLNODE_ADD_FIELD(creaseAngle, (0.0f));
  SO_VRMLNODE_ADD_FIELD(zDimension, (0));
  SO_VRMLNODE_ADD_FIELD(xDimension, (0));
  SO_VRMLNODE_ADD_FIELD(zSpacing, (1.0f));
  SO_VRMLNODE_ADD_FIELD(xSpacing, (1.0f));
  SO_VRMLNODE_ADD_EMPTY_MFIELD(height);
  SO_VRMLNODE_ADD_EXPOSED_FIELD(texCoord, (NULL));
  SO_VRMLNODE_ADD_EXPOSED_FIELD(normal, (NULL));
  SO_VRMLNODE_ADD_EXPOSED_FIELD(color, (NULL));
  SO_VRMLNODE_ADD_FIELD(colorPerVertex, (TRUE));
  SO_VRMLNODE_ADD_FIELD(normalPerVertex, (TRUE));
}

/*!
  Destructor.
*/
SoVRMLElevationGrid::~SoVRMLElevationGrid(void)
{
  delete THIS;
}

// Doc in parent
void
SoVRMLElevationGrid::GLRender(SoGLRenderAction * action)
{
  const int xdim = this->xDimension.getValue();
  const int zdim = this->zDimension.getValue();
  if (xdim < 2 || zdim < 2) return;

  if (this->height.getNum() != xdim*zdim) {
    SoDebugError::postWarning("SoVRMLElevationGrid::GLRender",
                              "Wrong number of height values.");
    return;
  }

  if (!this->shouldGLRender(action)) return;

  SoState * state = action->getState();

  this->setupShapeHints(state, this->ccw.getValue(), this->solid.getValue());

  SbBool dotex = SoGLTextureEnabledElement::get(state);
  SbBool donorm = SoLightModelElement::get(state) !=
    SoLightModelElement::BASE_COLOR;

  Binding nbind = this->findNormalBinding();
  Binding mbind = this->findMaterialBinding();

  if (!donorm) nbind = OVERALL;

  const SbVec2f * tcoords = NULL;
  SoVRMLTextureCoordinate * tnode = (SoVRMLTextureCoordinate*)
    this->texCoord.getValue();

  if (tnode) tcoords = tnode->point.getValues(0);

  SoMaterialBundle mb(action);
  mb.sendFirst();

  SbBool normalcache = FALSE;
  const SbVec3f * normals = NULL;
  if (nbind != OVERALL) {
    SoVRMLNormal * nnode = (SoVRMLNormal*) this->normal.getValue();
    if (nnode) normals = nnode->vector.getValues(0);
    if (normals == NULL) {
      normals = this->updateNormalCache(nbind);
      normalcache = TRUE;
    }
  }

  const float * h = this->height.getValues(0);

  float currz = 0.0f;

  const float zspace = this->zSpacing.getValue();
  const float xspace = this->xSpacing.getValue();

  int idx = 0;
  int idx2 = xdim;

  SbBool drawasstrip = TRUE;
  if (nbind == PER_VERTEX && normalcache) drawasstrip = FALSE; // crease angle normals
  if (nbind == PER_QUAD && mbind == PER_VERTEX) drawasstrip = FALSE;

  if (nbind <= PER_QUAD && mbind <= PER_QUAD) {
    SoGLShadeModelElement::forceSend(state, TRUE);
  }

  // FIXME: Probably too slow. Need several rendering loops
  if (drawasstrip) {
    for (int z = 0; z < zdim-1; z++) {
      float currx = 0.0f;
      const float * nexth = h + xdim;
      float nextz = currz + zspace;

      glBegin(GL_QUAD_STRIP);

      if (nbind == PER_VERTEX) {
        glNormal3fv(normals[idx].getValue());
      }
      if (dotex) {
        if (tcoords) glTexCoord2fv(tcoords[idx].getValue());
        else glTexCoord2f(0.0f, float(z)/float(zdim-1));
      }
      if (mbind == PER_VERTEX) {
        mb.send(idx, TRUE);
      }
      glVertex3f(currx, h[0], currz);
      idx++;

      if (nbind == PER_VERTEX) {
        glNormal3fv(normals[idx2].getValue());
      }
      if (dotex) {
        if (tcoords) glTexCoord2fv(tcoords[idx2].getValue());
        else glTexCoord2f(0.0f, float(z)/float(zdim-1));
      }
      if (mbind == PER_VERTEX) {
        mb.send(idx2, TRUE);
      }
      glVertex3f(currx, nexth[0], nextz);
      idx2++;
      currx += xspace;

      for (int x = 1; x < xdim; x++) {
        if (dotex) {
          if (tcoords) glTexCoord2fv(tcoords[idx].getValue());
          else glTexCoord2f(0.0f, float(z)/float(zdim-1));
        }
        if (nbind == PER_VERTEX) {
          glNormal3fv(normals[idx].getValue());
        }
        if (mbind == PER_VERTEX) {
          mb.send(idx, TRUE);
        }
        glVertex3f(currx, h[x], currz);
        idx++;

        if (dotex) {
          if (tcoords) glTexCoord2fv(tcoords[idx2].getValue());
          else glTexCoord2f(0.0f, float(z)/float(zdim-1));
        }
        if (nbind == PER_VERTEX) {
          glNormal3fv(normals[idx2].getValue());
        }
        else if (nbind == PER_QUAD) {
          glNormal3fv(normals[0].getValue());
          normals++;
        }
        if (mbind != OVERALL) {
          mb.send(idx2, TRUE);
        }
        glVertex3f(currx, nexth[x], nextz);
        idx2++;
        currx += xspace;
      }
      currz += zspace;
      nextz += zspace;
      h += xdim;
      nexth += xdim;
      glEnd();
    }
  }
  else {
    glBegin(GL_QUADS);

    for (int z = 0; z < zdim-1; z++) {
      float currx = 0.0f;
      float nextz = currz + zspace;

      int mcnt = 0;

      for (int x = 0; x < xdim-1; x++) {
        // vertex 1
        idx = x + (z+1) * xdim;
        if (nbind == PER_VERTEX) {
          if (normalcache) {
            glNormal3fv(normals->getValue());
            normals++;
          }
          else {
            glNormal3fv(normals[idx].getValue());
          }
        }
        if (dotex) {
          if (tcoords) glTexCoord2fv(tcoords[idx].getValue());
          else glTexCoord2f(float(x) / float(xdim-1), float(z+1)/float(zdim-1));
        }
        if (mbind == PER_VERTEX) {
          mb.send(idx, TRUE);
        }
        glVertex3f(currx, h[idx], nextz);

        // vertex 2
        idx++; // use next on this line
        if (nbind == PER_VERTEX) {
          if (normalcache) {
            glNormal3fv(normals->getValue());
            normals++;
          }
          else {
            glNormal3fv(normals[idx].getValue());
          }
        }
        if (dotex) {
          if (tcoords) glTexCoord2fv(tcoords[idx].getValue());
          else glTexCoord2f(float(x+1)/float(xdim-1), float(z+1)/float(zdim-1));
        }
        if (mbind == PER_VERTEX) {
          mb.send(idx, TRUE);
        }
        glVertex3f(currx+xspace, h[idx], nextz);

        // vertex 3
        idx -= xdim; // use previous line
        if (nbind == PER_VERTEX) {
          if (normalcache) {
            glNormal3fv(normals->getValue());
            normals++;
          }
          else {
            glNormal3fv(normals[idx].getValue());
          }
        }
        if (dotex) {
          if (tcoords) glTexCoord2fv(tcoords[idx].getValue());
          else glTexCoord2f(float(x+1)/float(xdim-1), float(z)/float(zdim-1));
        }
        if (mbind == PER_VERTEX) {
          mb.send(idx, TRUE);
        }
        glVertex3f(currx+xspace, h[idx], currz);

        // vertex 4
        idx--; // use previous on this line
        if (nbind == PER_QUAD  || (nbind == PER_VERTEX && normalcache)) {
          glNormal3fv(normals->getValue());
          normals++;
        }
        else if (nbind == PER_VERTEX) {
          glNormal3fv(normals[idx].getValue());
        }
        if (dotex) {
          if (tcoords) glTexCoord2fv(tcoords[idx].getValue());
          else glTexCoord2f(float(x)/float(xdim-1), float(z)/float(zdim-1));
        }
        if (mbind == PER_VERTEX) {
          mb.send(idx, TRUE);
        }
        else if (mbind == PER_QUAD) {
          mb.send(mcnt++, TRUE);
        }
        glVertex3f(currx, h[idx], currz);
        currx += xspace;
      }

      currz += zspace;
      nextz += zspace;
    }
    glEnd();
  }
}

// Doc in parent
void
SoVRMLElevationGrid::rayPick(SoRayPickAction * action)
{
  // FIXME: implement optimized version
  inherited::rayPick(action);
}

// Doc in parent
void
SoVRMLElevationGrid::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  if (this->shouldPrimitiveCount(action)) {
    action->addNumTriangles((xDimension.getValue()-1)*(zDimension.getValue()-1)*2);
  }
}

// Doc in parent
void
SoVRMLElevationGrid::computeBBox(SoAction * action,
                                 SbBox3f & bbox,
                                 SbVec3f & center)
{
  // FIXME: consider caching minh, maxh in the private class.

  const int n = this->height.getNum();
  if (n == 0) return;

  const float * h = this->height.getValues(0);

  float minh, maxh;
  minh = maxh = h[0];
  for (int i = 1; i < n; i++) {
    if (h[i] < minh) minh = h[i];
    else if (h[i] > maxh) maxh = h[i];
  }

  bbox.setBounds(0.0f, minh, 0.0f,
                 this->xDimension.getValue() * this->xSpacing.getValue(),
                 maxh,
                 this->zDimension.getValue() * this->zSpacing.getValue());

  center = bbox.getCenter();
}

// Doc in parent
void
SoVRMLElevationGrid::generatePrimitives(SoAction * action)
{
  const int xdim = this->xDimension.getValue();
  const int zdim = this->zDimension.getValue();
  if (xdim < 2 || zdim < 2) return;

  if (this->height.getNum() != xdim*zdim) {
    SoDebugError::postWarning("SoVRMLElevationGrid::generatePrimitives",
                              "Wrong number of height values.");
    return;
  }

  SoState * state = action->getState();

  SbBool donorm = SoLightModelElement::get(state) !=
    SoLightModelElement::BASE_COLOR;

  Binding nbind = this->findNormalBinding();
  Binding mbind = this->findMaterialBinding();

  if (!donorm) nbind = OVERALL;

  const SbVec2f * tcoords = NULL;
  SoVRMLTextureCoordinate * tnode = (SoVRMLTextureCoordinate*)
    this->texCoord.getValue();

  if (tnode) tcoords = tnode->point.getValues(0);

  SbBool normalcache = FALSE;
  const SbVec3f * normals = NULL;
  if (nbind != OVERALL) {
    SoVRMLNormal * nnode = (SoVRMLNormal*) this->normal.getValue();
    if (nnode) normals = nnode->vector.getValues(0);
    if (normals == NULL) {
      normals = this->updateNormalCache(nbind);
      normalcache = TRUE;
    }
  }

  const float * h = this->height.getValues(0);
  const float * nexth = h + xdim;

  float currz = 0.0f;

  const float zspace = this->zSpacing.getValue();
  const float xspace = this->xSpacing.getValue();

  SoPrimitiveVertex vertex;
  SoPointDetail pointDetail;
  SoFaceDetail faceDetail;

  vertex.setDetail(&pointDetail);

  SbVec3f dummynormal(0, 1, 0);
  const SbVec3f * currnormal = &dummynormal;
  if (normals) currnormal = normals;
  vertex.setNormal(*currnormal);

  this->beginShape(action, QUADS, &faceDetail);

  int idx;
  int cnt = 0;

  for (int z = 0; z < zdim-1; z++) {
    float currx = 0.0f;
    for (int x = 0; x < xdim-1; x++) {

      // vertex 1
      idx = x+(z+1)*xdim;
      SbVec3f v0(currx, nexth[x], currz+zspace);
      if (mbind == PER_VERTEX) {
        pointDetail.setMaterialIndex(idx);
        vertex.setMaterialIndex(idx);
      }
      else if (mbind == PER_QUAD) {
        pointDetail.setMaterialIndex(cnt);
        vertex.setMaterialIndex(cnt);
      }
      if (nbind == PER_VERTEX) {
        pointDetail.setNormalIndex(idx);
        if (normalcache) {
          vertex.setNormal(*currnormal);
          currnormal++;
        }
        else {
          vertex.setNormal(normals[idx]);
        }
      }
      else if (nbind == PER_QUAD) {
        pointDetail.setNormalIndex(cnt);
        vertex.setNormal(*currnormal);
        currnormal++;
      }
      pointDetail.setTextureCoordIndex(idx);
      pointDetail.setCoordinateIndex(idx);
      if (tcoords) vertex.setTextureCoords(tcoords[idx]);
      vertex.setPoint(v0);
      this->shapeVertex(&vertex);

      // vertex 2
      SbVec3f v1(currx+xspace, nexth[x+1], currz+zspace);
      idx++;
      if (mbind == PER_VERTEX) {
        pointDetail.setMaterialIndex(idx);
        vertex.setMaterialIndex(idx);
      }
      if (nbind == PER_VERTEX) {
        pointDetail.setNormalIndex(idx);
        if (normalcache) {
          vertex.setNormal(*currnormal);
          currnormal++;
        }
        else {
          vertex.setNormal(normals[idx]);
        }
      }
      pointDetail.setTextureCoordIndex(idx);
      pointDetail.setCoordinateIndex(idx);
      if (tcoords) vertex.setTextureCoords(tcoords[idx]);
      vertex.setPoint(v1);
      this->shapeVertex(&vertex);

      // vertex 3
      SbVec3f v2(currx+xspace, h[x+1], currz);
      idx -= xdim;
      if (mbind == PER_VERTEX) {
        pointDetail.setMaterialIndex(idx);
        vertex.setMaterialIndex(idx);
      }
      if (nbind == PER_VERTEX) {
        pointDetail.setNormalIndex(idx);
        if (normalcache) {
          vertex.setNormal(*currnormal);
          currnormal++;
        }
        else {
          vertex.setNormal(normals[idx]);
        }
      }
      pointDetail.setTextureCoordIndex(idx);
      pointDetail.setCoordinateIndex(idx);
      if (tcoords) vertex.setTextureCoords(tcoords[idx]);
      vertex.setPoint(v2);
      this->shapeVertex(&vertex);

      // vertex 4
      SbVec3f v3(currx, h[x], currz);
      idx--;
      if (mbind == PER_VERTEX) {
        pointDetail.setMaterialIndex(idx);
        vertex.setMaterialIndex(idx);
      }
      if (nbind == PER_VERTEX) {
        pointDetail.setNormalIndex(idx);
        if (normalcache) {
          vertex.setNormal(*currnormal);
          currnormal++;
        }
        else {
          vertex.setNormal(normals[idx]);
        }
      }
      pointDetail.setCoordinateIndex(idx);
      if (tcoords) vertex.setTextureCoords(tcoords[idx]);
      vertex.setPoint(v3);
      this->shapeVertex(&vertex);

      currx += xspace;
      faceDetail.incFaceIndex();
    }
    h += xdim;
    nexth += xdim;
    currz += zspace;
    cnt++;
  }
  this->endShape();
}

//
// private method used to find the material/color binding.
//
SoVRMLElevationGrid::Binding
SoVRMLElevationGrid::findMaterialBinding(void) const
{
  Binding binding = OVERALL;
  if (this->color.getValue()) {
    if (this->colorPerVertex.getValue()) binding = PER_VERTEX;
    else binding = PER_QUAD;
  }
  return binding;
}

//
// private method used to find the normal binding.
//
SoVRMLElevationGrid::Binding
SoVRMLElevationGrid::findNormalBinding(void) const
{
  Binding binding = PER_VERTEX;
  if (this->normal.getValue()) {
    if (!this->normalPerVertex.getValue()) binding = PER_QUAD;
  }
  return binding;
}

// Doc in parent
void
SoVRMLElevationGrid::notify(SoNotList * list)
{
  SoField * f = list->getLastField();
  if (f == &this->height ||
      f == &this->creaseAngle ||
      f == &this->ccw ||
      f == &this->xDimension ||
      f == &this->zDimension ||
      f == &this->xSpacing ||
      f == &this->zSpacing) {
    THIS->dirty = TRUE;
  }
  inherited::notify(list);
}

//
// private method that recalculates the normal cache
//
const SbVec3f *
SoVRMLElevationGrid::updateNormalCache(Binding & nbind)
{
  if (THIS->dirty) {
    // FIXME: optimize by using a specialized algorithm for
    // calculating the normals.
    THIS->dirty = FALSE;
    THIS->ngen.reset(this->ccw.getValue());

    const int xdim = this->xDimension.getValue();
    const int zdim = this->zDimension.getValue();
    if (xdim < 2 || zdim < 2) return NULL;

    const float * h = this->height.getValues(0);

    const float * nexth = h + xdim;

    float currz = 0.0f;

    const float zspace = this->zSpacing.getValue();
    const float xspace = this->xSpacing.getValue();

    for (int z = 0; z < zdim-1; z++) {
      float currx = 0.0f;
      for (int x = 0; x < xdim-1; x++) {

        SbVec3f v0(currx, nexth[x], currz+zspace);
        SbVec3f v1(currx+xspace, nexth[x+1], currz+zspace);
        SbVec3f v2(currx+xspace, h[x+1], currz);
        SbVec3f v3(currx, h[x], currz);

        THIS->ngen.quad(v0, v1, v2, v3);
        currx += xspace;
      }
      h += xdim;
      nexth += xdim;
      currz += zspace;
    }

    if (this->creaseAngle.getValue() <= FLT_EPSILON) {
      THIS->nbind = PER_QUAD;
      THIS->ngen.generatePerFace();
    }
    else {
      THIS->nbind = PER_VERTEX;
      THIS->ngen.generate(this->creaseAngle.getValue());
    }
  }

  nbind = THIS->nbind;
  return THIS->ngen.getNormals();
}
