/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
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
