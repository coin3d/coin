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

#ifndef __SOSHUTTLE_H__
#define __SOSHUTTLE_H__

#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFBool.h>

#if defined(COIN_EXCLUDE_SOSHUTTLE)
#error Configuration settings disrespected -- do not include this file!
#endif // COIN_EXCLUDE_SOSHUTTLE

// *************************************************************************

class SoShuttle : public SoTranslation {
  typedef SoTranslation inherited;
  
//$ BEGIN TEMPLATE NodeHeader(SoShuttle)
private:
  static SoType classTypeId;

public:
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;
  static void * createInstance(void);
public:
  static void initClass(void);
  static void cleanClass(void);

  SoShuttle(void);
protected:
  virtual ~SoShuttle();
//$ END TEMPLATE NodeHeader

public:
  SoSFVec3f translation0;
  SoSFVec3f translation1;
  SoSFFloat speed;
  SoSFBool on;
};

#endif // !__SOSHUTTLE_H__
