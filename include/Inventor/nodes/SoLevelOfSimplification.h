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

#ifndef __SOLEVELOFSIMPLIFICATION_H__
#define __SOLEVELOFSIMPLIFICATION_H__

#include <Inventor/nodes/SoLOD.h>
#include <Inventor/fields/SoMFFloat.h>
#include <Inventor/fields/SoSFInt32.h>

#if defined(COIN_EXCLUDE_SOLEVELOFSIMPLIFICATION)
#error Configuration settings disrespected -- do not include this file!
#endif // COIN_EXCLUDE_SOLEVELOFSIMPLIFICATION

// *************************************************************************

class SoLevelOfSimplification : public SoLOD {
  typedef SoLOD inherited;

//$ BEGIN TEMPLATE NodeHeader( SoLevelOfSimplification )
private:
  static SoType classTypeId;

public:
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;
  static void * createInstance(void);
public:
  static void initClass(void);
  static void cleanClass(void);

  SoLevelOfSimplification(void);
protected:
  virtual ~SoLevelOfSimplification();
//$ END TEMPLATE NodeHeader

public:
  SoMFFloat percentages;
  SoSFInt32 numTriangles;

#if !defined(COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION)
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);
#endif // !COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION

protected:
#if !defined(COIN_EXCLUDE_SOACTION)
  virtual int whichToTraverse(SoAction *);
#endif // !COIN_EXCLUDE_SOACTION
};

#endif // !__SOLEVELOFSIMPLIFICATION_H__
