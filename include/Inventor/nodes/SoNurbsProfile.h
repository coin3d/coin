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

#ifndef COIN_SONURBSPROFILE_H
#define COIN_SONURBSPROFILE_H

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
