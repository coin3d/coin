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
#include <Inventor/C/tidbits.h> // coin_atexit()
#include <stdlib.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/system/gl.h>
#include <../glue/GLUWrapper.h>

#ifdef COIN_THREADSAFE
#include <Inventor/threads/SbStorage.h>
#endif // COIN_THREADSAFE


/*!
  \var SoMFFloat SoNurbsProfile::knotVector
  Knot values for the nurbs curve.
*/

typedef struct {
  SbList <float> * coordlist;
  SbList <float> * tmplist;
} so_nurbsprofile_data;

static void
so_nurbsprofile_construct_data(void * closure)
{
  so_nurbsprofile_data * data = (so_nurbsprofile_data*) closure;
  data->coordlist = NULL;
  data->tmplist = NULL;
}

static void
so_nurbsprofile_destruct_data(void * closure)
{
  so_nurbsprofile_data * data = (so_nurbsprofile_data*) closure;
  delete data->coordlist;
  delete data->tmplist;
}

#ifdef COIN_THREADSAFE
static SbStorage * so_nurbsprofile_storage;
#else // COIN_THREADSAFE
static so_nurbsprofile_data * so_nurbsprofile_single_data;
#endif // ! COIN_THREADSAFE

static void
so_nurbsprofile_cleanup(void)
{
#ifdef COIN_THREADSAFE
  delete so_nurbsprofile_storage;
#else // COIN_THREADSAFE
  so_nurbsprofile_destruct_data((void*) so_nurbsprofile_single_data);
  delete so_nurbsprofile_single_data;
#endif // ! COIN_THREADSAFE
}

static SbList <float> *
so_nurbsprofile_get_coordlist(const SbBool tmplist)
{
  so_nurbsprofile_data * data = NULL;
#ifdef COIN_THREADSAFE
  data = (so_nurbsprofile_data*) so_nurbsprofile_storage->get();
#else // COIN_THREADSAFE
  data = so_nurbsprofile_single_data;
#endif // ! COIN_THREADSAFE

  if (tmplist) {
    if (data->tmplist == NULL) {
      data->tmplist = new SbList<float>;
    }
    return data->tmplist;
  }
  else {
    if (data->coordlist == NULL) {
      data->coordlist = new SbList<float>;
    }
    return data->coordlist;
  }
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
  SO_NODE_INTERNAL_INIT_CLASS(SoNurbsProfile, SO_FROM_INVENTOR_1);
#ifdef COIN_THREADSAFE
  so_nurbsprofile_storage = new SbStorage(sizeof(so_nurbsprofile_data),
                                           so_nurbsprofile_construct_data,
                                           so_nurbsprofile_destruct_data);
#else // COIN_THREADSAFE
  so_nurbsprofile_single_data = new so_nurbsprofile_data;
  so_nurbsprofile_construct_data((void*) so_nurbsprofile_single_data);
#endif // ! COIN_THREADSAFE
  coin_atexit((coin_atexit_f*) so_nurbsprofile_cleanup);
}

// Doc from superclass.
void
SoNurbsProfile::getTrimCurve(SoState * state, int32_t & numpoints,
                             float *& points, int & floatspervec,
                             int32_t & numknots, float *& knotvector)
{
  SbList <float> * coordListNurbsProfile =
    so_nurbsprofile_get_coordlist(FALSE);

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


static void APIENTRY
nurbsprofile_tess_vertex(float * vertex)
{
  SbList <float> * coordListNurbsProfile =
    so_nurbsprofile_get_coordlist(FALSE);

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

  SbList <float> * coordListNurbsProfile =
    so_nurbsprofile_get_coordlist(FALSE);

  SbList <float> * nurbsProfileTempList =
    so_nurbsprofile_get_coordlist(TRUE);

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
      GLUWrapper()->gluNurbsCallback(this->nurbsrenderer, (GLenum) GLU_NURBS_VERTEX,
                                     (gluNurbsCallback_cb_t)nurbsprofile_tess_vertex);
      GLUWrapper()->gluNurbsProperty(this->nurbsrenderer, (GLenum) GLU_NURBS_MODE, GLU_NURBS_TESSELLATOR);
      GLUWrapper()->gluNurbsProperty(this->nurbsrenderer, (GLenum) GLU_AUTO_LOAD_MATRIX, FALSE);
      GLUWrapper()->gluNurbsProperty(this->nurbsrenderer, (GLenum) GLU_DISPLAY_MODE, GLU_POINT);
      GLUWrapper()->gluNurbsProperty(this->nurbsrenderer, (GLenum) GLU_SAMPLING_METHOD, GLU_DOMAIN_DISTANCE);
    }

    // this looks pretty good
    float cmplx = SoComplexityElement::get(state);
    cmplx += 1.0f;
    cmplx = cmplx * cmplx * cmplx;
    GLUWrapper()->gluNurbsProperty(this->nurbsrenderer, (GLenum) GLU_U_STEP, float(numpoints)*cmplx);

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
