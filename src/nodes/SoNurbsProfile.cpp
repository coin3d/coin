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
#include <Inventor/lists/SbList.h>
#include <stdlib.h>
#include <coindefs.h> // COIN_STUB()


/*!
  \var SoMFFloat SoNurbsProfile::knotVector
  Knot values for the nurbs curve.
*/

static SbList <float> * coordListNurbsProfile = NULL;

static void cleanupNurbsProfile(void)
{
  delete coordListNurbsProfile;
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
}

/*!
  Destructor.
*/
SoNurbsProfile::~SoNurbsProfile()
{
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

// doc from superclass.
void
SoNurbsProfile::getVertices(SoState * state, int32_t & numvertices,
                            SbVec2f *& vertices)
{
  COIN_STUB();
  numvertices = 0;
  vertices = NULL;
}
