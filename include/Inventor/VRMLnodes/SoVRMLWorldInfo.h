/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 2001 by Systems in Motion. All rights reserved.
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

#ifndef COIN_SOVRMLWORLDINFO_H
#define COIN_SOVRMLWORLDINFO_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/fields/SoSFString.h>

class COIN_DLL_API SoVRMLWorldInfo : public SoNode
{
  typedef SoNode inherited;
  SO_NODE_HEADER(SoVRMLWorldInfo);

public:
  static void initClass(void);
  SoVRMLWorldInfo(void);

  SoSFString title;
  SoMFString info;

protected:
  virtual ~SoVRMLWorldInfo();

}; // class SoVRMLWorldInfo

#endif // ! COIN_SOVRMLWORLDINFO_H
