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
#include <../tidbits.h> // coin_atexit()
#include <stdlib.h>

static SbList <float> * coordListLinearProfile = NULL;

static void
cleanupLinearProfile(void)
{
  delete coordListLinearProfile;
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
}

// Doc from superclass.
void
SoLinearProfile::getTrimCurve(SoState * state, int32_t & numpoints,
                              float *& points, int & floatspervec,
                              int32_t & numknots, float *& knotvector)
{
  if (coordListLinearProfile == NULL) {
    coordListLinearProfile = new SbList <float>;
    coin_atexit((coin_atexit_f *)cleanupLinearProfile);
  }
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
  if (coordListLinearProfile == NULL) {
    coordListLinearProfile = new SbList <float>;
    coin_atexit((coin_atexit_f *)cleanupLinearProfile);
  }
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
