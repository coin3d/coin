/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

/*!
  \class SoNurbsProfile SoNurbsProfile.h Inventor/nodes/SoNurbsProfile.h
  \brief The SoNurbsProfile class is a node for specifying smooth profile curves.
  \ingroup nodes

  Use nodes of this type if you want to set up profiles that are
  smooth curves.
*/

#include <Inventor/nodes/SoNurbsProfile.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/elements/SoProfileCoordinateElement.h>
#include <Inventor/elements/SoComplexityElement.h>
#include <Inventor/lists/SbList.h>
#include <Inventor/SbMatrix.h>
#include <Inventor/SbViewVolume.h>
#include <../tidbits.h> // coin_atexit()
#include <stdlib.h>

#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/system/gl.h>
#include <../misc/GLUWrapper.h>



/*!
  \var SoMFFloat SoNurbsProfile::knotVector
  Knot values for the nurbs curve.
*/

static SbList <float> * coordListNurbsProfile = NULL;
static SbList <float> * nurbsProfileTempList = NULL;

static void
cleanupNurbsProfile(void)
{
  delete coordListNurbsProfile;
  delete nurbsProfileTempList;
}

// *************************************************************************

SO_NODE_SOURCE(SoNurbsProfile);

/*!
  Constructor.
*/
SoNurbsProfile::SoNurbsProfile(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoNurbsProfile);

  SO_NODE_ADD_FIELD(knotVector, (0.0f));
  this->nurbsrenderer = NULL;
}

/*!
  Destructor.
*/
SoNurbsProfile::~SoNurbsProfile()
{
  if (this->nurbsrenderer) {
    GLUWrapper()->gluDeleteNurbsRenderer(this->nurbsrenderer);
  }
}

// Doc from superclass.
void
SoNurbsProfile::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoNurbsProfile);
}

// Doc from superclass.
void
SoNurbsProfile::getTrimCurve(SoState * state, int32_t & numpoints,
                             float *& points, int & floatspervec,
                             int32_t & numknots, float *& knotvector)
{
  if (coordListNurbsProfile == NULL) {
    coordListNurbsProfile = new SbList <float>;
    coin_atexit(cleanupNurbsProfile);
  }
  numknots = this->knotVector.getNum();
  if (numknots) knotvector = (float *)(this->knotVector.getValues(0));

  const SoProfileCoordinateElement * elem = (const SoProfileCoordinateElement*)
    SoProfileCoordinateElement::getInstance(state);

  numpoints = elem->getNum();
  if (numpoints) {
    if (elem->is2D()) {
      points = (float*) elem->getArrayPtr2();
      floatspervec = 2;
    }
    else {
      points = (float*) elem->getArrayPtr3();
      floatspervec = 3;
    }
  }
  coordListNurbsProfile->truncate(0);
  int n = this->index.getNum();
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < floatspervec; j++) {
      coordListNurbsProfile->append(points[this->index[i]*floatspervec+j]);
    }
  }
  points = (float*) coordListNurbsProfile->getArrayPtr();
  numpoints = n;
}


static void
nurbsprofile_tess_vertex(float * vertex)
{
  coordListNurbsProfile->append(vertex[0]);
  coordListNurbsProfile->append(vertex[1]);
}

// doc from superclass.
void
SoNurbsProfile::getVertices(SoState * state, int32_t & numvertices,
                            SbVec2f * & vertices)
{
  // FIXME: optimize by detecting when the previously calculated
  // vertices can be returned. pederb, 20000922
  int numpoints;
  float * points;
  int floatspervec;
  int32_t numknots;
  float * knotvector;
  this->getTrimCurve(state, numpoints, points, floatspervec, numknots, knotvector);
  if (numpoints == 0 || numknots == 0) {
    numvertices = 0;
    vertices = NULL;
    return;
  }

  if (coordListNurbsProfile == NULL) {
    coordListNurbsProfile = new SbList <float>;
    coin_atexit(cleanupNurbsProfile);
  }
  if (nurbsProfileTempList == NULL) {
    nurbsProfileTempList = new SbList <float>;
  }

  nurbsProfileTempList->truncate(0);
  for (int i = 0; i < numpoints; i++) {
    nurbsProfileTempList->append(points[i*floatspervec]);
    nurbsProfileTempList->append(points[i*floatspervec+1]);
    if (GLUWrapper()->available &&
        GLUWrapper()->versionMatchesAtLeast(1, 3, 0)) {
      nurbsProfileTempList->append(0.0f); // gluNurbs needs 3D coordinates
    }
  }
  if (GLUWrapper()->available &&
      GLUWrapper()->versionMatchesAtLeast(1, 3, 0)) {
    // we will write into this array in the GLU callback
    coordListNurbsProfile->truncate(0);

    if (this->nurbsrenderer == NULL) {
      this->nurbsrenderer = GLUWrapper()->gluNewNurbsRenderer();
      GLUWrapper()->gluNurbsCallback(this->nurbsrenderer, (GLenum) GLU_W_NURBS_VERTEX,
                                     (gluNurbsCallback_cb_t)nurbsprofile_tess_vertex);
      GLUWrapper()->gluNurbsProperty(this->nurbsrenderer, (GLenum) GLU_W_NURBS_MODE, GLU_W_NURBS_TESSELLATOR);
      GLUWrapper()->gluNurbsProperty(this->nurbsrenderer, (GLenum) GLU_W_AUTO_LOAD_MATRIX, FALSE);
      GLUWrapper()->gluNurbsProperty(this->nurbsrenderer, (GLenum) GLU_W_DISPLAY_MODE, GLU_W_POINT);
      GLUWrapper()->gluNurbsProperty(this->nurbsrenderer, (GLenum) GLU_W_SAMPLING_METHOD, GLU_W_DOMAIN_DISTANCE);
    }

    // this looks pretty good
    float cmplx = SoComplexityElement::get(state);
    cmplx += 1.0f;
    cmplx = cmplx * cmplx * cmplx;
    GLUWrapper()->gluNurbsProperty(this->nurbsrenderer, (GLenum) GLU_W_U_STEP, float(numpoints)*cmplx);

    // these values are not important as we're not using screen-space
    // complexity (yet)
    SbMatrix modelmatrix = SbMatrix::identity();
    SbMatrix affine, proj;
    SbViewVolume vv;
    vv.ortho(0.0f, 1.0f,
             0.0f, 1.0f,
             -1.0f, 1.0f);
    vv.getMatrices(affine, proj);
    GLint viewport[4];
    viewport[0] = 0;
    viewport[1] = 0;
    viewport[2] = 256;
    viewport[3] = 256;
    GLUWrapper()->gluLoadSamplingMatrices(this->nurbsrenderer,
                                          modelmatrix[0],
                                          proj[0],
                                          viewport);

    // generate curve
    GLUWrapper()->gluBeginCurve(this->nurbsrenderer);
    GLUWrapper()->gluNurbsCurve(this->nurbsrenderer,
                                numknots,
                                (float*)knotvector,
                                3,
                                (float*)nurbsProfileTempList->getArrayPtr(),
                                numknots - numpoints,
                                GL_MAP1_VERTEX_3);
    GLUWrapper()->gluEndCurve(this->nurbsrenderer);

    // when we get here, the GLU callback should have added the
    // points to the list
    numvertices = coordListNurbsProfile->getLength() / 2;
    vertices = (SbVec2f*) coordListNurbsProfile->getArrayPtr();
  }
  else {
    // just send the control points when GLU v1.3 is not available
    numvertices = numpoints;
    vertices = (SbVec2f*) nurbsProfileTempList->getArrayPtr();
  }
}
