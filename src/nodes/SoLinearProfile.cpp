/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
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
#include <stdlib.h>
#include <coindefs.h> // COIN_STUB()

static SbList <float> * coordList = NULL;

static void cleanup(void)
{
  delete coordList;
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
  SO_NODE_INTERNAL_INIT_CLASS(SoLinearProfile);
}

// Doc from superclass.
void
SoLinearProfile::getTrimCurve(SoState * state, int32_t & numpoints,
                              float *& points, int & floatspervec,
                              int32_t & numknots, float *& knotvector)
{
  if (coordList == NULL) {
    coordList = new SbList <float>;
    atexit(cleanup);
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
  coordList->truncate(0);
  int n = this->index.getNum();
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < floatspervec; j++) {
      coordList->append(points[this->index[i]*floatspervec+j]);
    }    
  }
  points = (float*) coordList->getArrayPtr();
  numpoints = n;
}

// Doc from superclass.
void
SoLinearProfile::getVertices(SoState * state, int32_t & numvertices,
                             SbVec2f *& vertices)
{
  COIN_STUB();
  numvertices = 0;
  vertices = NULL;
}
