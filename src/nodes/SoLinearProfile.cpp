/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoLinearProfile SoLinearProfile.h Inventor/nodes/SoLinearProfile.h
  \brief The SoLinearProfile class is a node for specifying linear profile curves.
  \ingroup nodes

  Use nodes of this type if you want to set up profiles that are
  simply straight lines connected by control points.
*/

#include <Inventor/nodes/SoLinearProfile.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/elements/SoProfileCoordinateElement.h>
#include <Inventor/lists/SbList.h>
#include <Inventor/C/tidbits.h> // coin_atexit()
#include <stdlib.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#ifdef COIN_THREADSAFE
#include <Inventor/threads/SbStorage.h>
#endif // COIN_THREADSAFE

typedef struct {
  SbList <float> * coordlist;
} so_linearprofile_data;

static void
so_linearprofile_construct_data(void * closure)
{
  so_linearprofile_data * data = (so_linearprofile_data*) closure;
  data->coordlist = NULL;
}

static void
so_linearprofile_destruct_data(void * closure)
{
  so_linearprofile_data * data = (so_linearprofile_data*) closure;
  delete data->coordlist;
}

#ifdef COIN_THREADSAFE
static SbStorage * so_linearprofile_storage;
#else // COIN_THREADSAFE
static so_linearprofile_data * so_linearprofile_single_data;
#endif // ! COIN_THREADSAFE

static void
so_linearprofile_cleanup(void)
{
#ifdef COIN_THREADSAFE
  delete so_linearprofile_storage;
#else // COIN_THREADSAFE
  so_linearprofile_destruct_data((void*) so_linearprofile_single_data);
  delete so_linearprofile_single_data;
#endif // ! COIN_THREADSAFE
}

static SbList <float> *
so_linearprofile_get_coordlist(void)
{
  so_linearprofile_data * data = NULL;
#ifdef COIN_THREADSAFE
  data = (so_linearprofile_data*) so_linearprofile_storage->get();
#else // COIN_THREADSAFE
  data = so_linearprofile_single_data;
#endif // ! COIN_THREADSAFE
  if (data->coordlist == NULL) {
    data->coordlist = new SbList<float>;
  }
  return data->coordlist;
}

// *************************************************************************

SO_NODE_SOURCE(SoLinearProfile);

/*!
  Constructor.
*/
SoLinearProfile::SoLinearProfile(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoLinearProfile);
}

/*!
  Destructor.
*/
SoLinearProfile::~SoLinearProfile()
{
}

// Doc from superclass.
void
SoLinearProfile::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoLinearProfile, SO_FROM_INVENTOR_1);

#ifdef COIN_THREADSAFE
  so_linearprofile_storage = new SbStorage(sizeof(so_linearprofile_data),
                                           so_linearprofile_construct_data,
                                           so_linearprofile_destruct_data);
#else // COIN_THREADSAFE
  so_linearprofile_single_data = new so_linearprofile_data;
  so_linearprofile_construct_data((void*) so_linearprofile_single_data);
#endif // ! COIN_THREADSAFE
  coin_atexit((coin_atexit_f*) so_linearprofile_cleanup);
}

// Doc from superclass.
void
SoLinearProfile::getTrimCurve(SoState * state, int32_t & numpoints,
                              float *& points, int & floatspervec,
                              int32_t & numknots, float *& knotvector)
{
  SbList <float> * coordListLinearProfile = 
    so_linearprofile_get_coordlist();

  numknots = 0;
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
  coordListLinearProfile->truncate(0);
  int n = this->index.getNum();
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < floatspervec; j++) {
      coordListLinearProfile->append(points[this->index[i]*floatspervec+j]);
    }
  }
  points = (float*) coordListLinearProfile->getArrayPtr();
  numpoints = n;
}

// Doc from superclass.
void
SoLinearProfile::getVertices(SoState * state, int32_t & numvertices,
                             SbVec2f *& vertices)
{
  SbList <float> * coordListLinearProfile = 
    so_linearprofile_get_coordlist();

  const SoProfileCoordinateElement * elem = (const SoProfileCoordinateElement*)
    SoProfileCoordinateElement::getInstance(state);

  coordListLinearProfile->truncate(0);

  int32_t numcoords = elem->getNum();
  if (numcoords) {
    float * points = (float *)elem->getArrayPtr2();
    int n = this->index.getNum();
    for (int i = 0; i < n; i++) {
      coordListLinearProfile->append(points[this->index[i]*2  ]);
      coordListLinearProfile->append(points[this->index[i]*2+1]);
    }

    vertices = (SbVec2f *) coordListLinearProfile->getArrayPtr();
    numvertices = n;
  }
  else {
    vertices = NULL;
    numvertices = 0;
  }
}
