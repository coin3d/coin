#ifndef COIN_SONURBSPROFILE_H
#define COIN_SONURBSPROFILE_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoProfile.h>
#include <Inventor/fields/SoMFFloat.h>


class COIN_DLL_API SoNurbsProfile : public SoProfile {
  typedef SoProfile inherited;

  SO_NODE_HEADER(SoNurbsProfile);

public:
  static void initClass(void);
  SoNurbsProfile(void);

  SoMFFloat knotVector;

  virtual void getTrimCurve(SoState * state, int32_t & numpoints,
                            float *& points, int & floatspervec,
                            int32_t & numknots, float *& knotvector);
  virtual void getVertices(SoState * state, int32_t & numvertices,
                           SbVec2f *& vertices);

protected:
  virtual ~SoNurbsProfile();

private:
  void * nurbsrenderer;
};

#endif // !COIN_SONURBSPROFILE_H
