/**************************************************************************\
 * 
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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

#ifndef __SONURBSPROFILE_H__
#define __SONURBSPROFILE_H__

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoProfile.h>
#include <Inventor/fields/SoMFFloat.h>

#if defined(COIN_EXCLUDE_SONURBSPROFILE)
#error Configuration settings disrespected -- do not include this file!
#endif // COIN_EXCLUDE_SONURBSPROFILE

// *************************************************************************

class SoNurbsProfile : public SoProfile {
  typedef SoProfile inherited;

  SO_NODE_HEADER(SoNurbsProfile);

public:
  static void initClass(void);
  SoNurbsProfile(void);

  SoMFFloat knotVector;

  virtual void getTrimCurve(SoState * state, int32_t & numPoints,
			    float *& points, int & floatsPerVec,
			    int32_t & numKnots, float *& knotVector);
  virtual void getVertices(SoState * state, int32_t & nVertices,
			   SbVec2f *& vertices);

protected:
  virtual ~SoNurbsProfile();
};

#endif // !__SONURBSPROFILE_H__
