/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
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
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#endif // !_WIN32
#include <GL/gl.h>
#include <GL/glu.h>

#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

/*!
  \var SoMFFloat SoNurbsProfile::knotVector
  Knot values for the nurbs curve.
*/

static SbList <float> * coordListNurbsProfile = NULL;
static SbList <float> * nurbsProfileTempList = NULL;

static void cleanupNurbsProfile(void)
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
#ifdef HAVE_GLU_NURBSOBJECT
    gluDeleteNurbsRenderer((HAVE_GLU_NURBSOBJECT *)this->nurbsrenderer);
#endif // HAVE_GLU_NURBSOBJECT
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
    atexit(cleanupNurbsProfile);
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


#if defined(HAVE_GLU_NURBSOBJECT) && GLU_VERSION_1_3
static void
nurbsprofile_tess_vertex(float * vertex)
{
  coordListNurbsProfile->append(vertex[0]);
  coordListNurbsProfile->append(vertex[1]);
}
#endif // HAVE_GLU_NURBSOBJECT && GLU_VERSION_1_3

// doc from superclass.
void
SoNurbsProfile::getVertices(SoState * state, int32_t & numvertices,
                            SbVec2f * & vertices)
{
  // FIXME: optimize by detecting when the previously calculated
  // vertices can be returned. pederb, 20000922
#if defined(HAVE_GLU_NURBSOBJECT) && GLU_VERSION_1_3
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
    atexit(cleanupNurbsProfile);
  }
  if (nurbsProfileTempList == NULL) {
    nurbsProfileTempList = new SbList <float>;
  }

  nurbsProfileTempList->truncate(0);
  for (int i = 0; i < numpoints; i++) {
    nurbsProfileTempList->append(points[i*floatspervec]);
    nurbsProfileTempList->append(points[i*floatspervec+1]);
    nurbsProfileTempList->append(0.0f); // gluNurbs needs 3D coordinates
  }
  
  // we will write into this array in the GLU callback
  coordListNurbsProfile->truncate(0);

  HAVE_GLU_NURBSOBJECT * nurbsobj = (HAVE_GLU_NURBSOBJECT *) this->nurbsrenderer;
  if (nurbsobj == NULL) {
    nurbsobj = gluNewNurbsRenderer();
    gluNurbsCallback(nurbsobj, (GLenum) GLU_NURBS_VERTEX, 
                     (void (*)())nurbsprofile_tess_vertex);
    gluNurbsProperty(nurbsobj, (GLenum) GLU_NURBS_MODE, GLU_NURBS_TESSELLATOR);
    gluNurbsProperty(nurbsobj, (GLenum) GLU_AUTO_LOAD_MATRIX, FALSE);
    gluNurbsProperty(nurbsobj, (GLenum) GLU_DISPLAY_MODE, GLU_POINT);
    gluNurbsProperty(nurbsobj, (GLenum) GLU_SAMPLING_METHOD, GLU_DOMAIN_DISTANCE);
    this->nurbsrenderer = (void*) nurbsobj;
  }

  // this looks pretty good
  float cmplx = SoComplexityElement::get(state);
  cmplx += 1.0f;
  cmplx = cmplx * cmplx * cmplx;
  gluNurbsProperty(nurbsobj, (GLenum) GLU_U_STEP, float(numpoints)*cmplx);
  
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
  gluLoadSamplingMatrices(nurbsobj,
                          modelmatrix[0],
                          proj[0],
                          viewport);

  // generate curve
  gluBeginCurve(nurbsobj);
  gluNurbsCurve(nurbsobj,
                numknots,
                (float*)knotvector,
                3,
                (float*)nurbsProfileTempList->getArrayPtr(),
                numknots - numpoints,
                GL_MAP1_VERTEX_3);
  gluEndCurve(nurbsobj);
  
  // when we get here, the GLU callback should have added the 
  // points to the list
  numvertices = coordListNurbsProfile->getLength() / 2;
  vertices = (SbVec2f*) coordListNurbsProfile->getArrayPtr();
#else // HAVE_GLU_NURBSOBJECT && GLU_VERSION_1_3
  numvertices = 0;
  vertices = NULL;
#endif // ! HAVE_GLU_NURBSOBJECT && GLU_VERSION_1_3
}
