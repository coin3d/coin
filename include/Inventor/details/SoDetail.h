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

#ifndef __SODETAIL_H__
#define __SODETAIL_H__

#include <Inventor/SoType.h>

class SoDetail {
public:
  virtual ~SoDetail();

  virtual SoDetail *copy() const = 0;

  virtual SoType getTypeId() const = 0;
  SbBool isOfType(const SoType type) const;

  static SoType getClassTypeId();

public: // internal

  static void initClass();
  static void initClasses();

protected:
  SoDetail();

private:
  static SoType classTypeId;
};


#endif // __SODETAIL_H__
