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

#ifndef COIN_SOVRMLANCHOR_H
#define COIN_SOVRMLANCHOR_H

#include <Inventor/VRMLnodes/SoVRMLParent.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoSFVec3f.h>

class SoVRMLAnchor;

typedef void SoVRMLAnchorCB( const SbString &, void *,  SoVRMLAnchor *);

class COIN_DLL_API SoVRMLAnchor : public SoVRMLParent
{
  typedef SoVRMLParent inherited;
  SO_NODE_HEADER(SoVRMLAnchor);

public:
  static void initClass(void);

  SoVRMLAnchor(void);
  SoMFString url;
  SoSFString description;
  SoMFString parameter;

  SoSFVec3f bboxCenter;
  SoSFVec3f bboxSize;

  static void setFetchURLCallBack(SoVRMLAnchorCB *, void * closure);

  virtual void handleEvent(SoHandleEventAction * action);

protected:
  virtual ~SoVRMLAnchor();

private:
  static SoVRMLAnchorCB * fetchurlcb;
  static void * userdata;

}; // class SoVRMLAnchor

#endif // ! COIN_SOVRMLANCHOR_H
