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
 *  http://www.sim.no/ sales@sim.no Voice: +47 73540378 Fax: +47 73943861
 *
\**************************************************************************/

#ifndef __SOLINEARPROFILE_H__
#define __SOLINEARPROFILE_H__

#include <Inventor/nodes/SoProfile.h>

#if defined(COIN_EXCLUDE_SOLINEARPROFILE)
#error Configuration settings disrespected -- do not include this file!
#endif // COIN_EXCLUDE_SOLINEARPROFILE

// *************************************************************************

class SoLinearProfile : public SoProfile {
  typedef SoProfile inherited;

//$ BEGIN TEMPLATE NodeHeader( SoLinearProfile )
private:
  static SoType classTypeId;

public:
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;
  static void * createInstance(void);
public:
  static void initClass(void);
  static void cleanClass(void);

  SoLinearProfile(void);
protected:
  virtual ~SoLinearProfile();
//$ END TEMPLATE NodeHeader

public:
  virtual void getTrimCurve(SoState * state, int32_t & numPoints,
			    float *& points, int & floatsPerVec,
			    int32_t & numKnots, float *& knotVector);
  virtual void getVertices(SoState * state, int32_t & nVertices,
			   SbVec2f *& vertices);
};

#endif // !__SOLINEARPROFILE_H__
