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

#ifndef __SOVRMLINTERPOLATOR_H__
#define __SOVRMLINTERPOLATOR_H__

#include <Inventor/nodes/SoNode.h>

class SoVRMLInterpolator : public SoNode {
  typedef SoNode inherited;

//$ BEGIN TEMPLATE NodeAbstractHeader(SoVRMLInterpolator)
private:
  static SoType classTypeId;

public:
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;
public:
  static void initClass(void);
  static void cleanClass(void);

protected:
  SoVRMLInterpolator(void);
  virtual ~SoVRMLInterpolator();
//$ END TEMPLATE NodeAbstractHeader

public:
  SbBool getOutputName(const SoVRMLInterpOutput * output,
		       SbName & outputName) const;
};

#endif // !__SOVRMLINTERPOLATOR_H__
