/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#ifndef COIN_SOVRMLDRAGSENSOR_H
#define COIN_SOVRMLDRAGSENSOR_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/VRMLnodes/SoVRMLSensor.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/SbVec2s.h>
#include <Inventor/SbVec2f.h>
#include <Inventor/SbMatrix.h>
#include <Inventor/SbViewVolume.h>

class SbMatrix;

class COIN_DLL_API SoVRMLDragSensor : public SoVRMLSensor {
  typedef SoVRMLSensor inherited;

  SO_NODE_ABSTRACT_HEADER(SoVRMLDragSensor);

public:
  SoSFVec3f trackPoint_changed;
  SoSFBool autoOffset;

  //  virtual void GLRender(SoGLRenderAction * action);
  virtual void handleEvent(SoHandleEventAction * action);
  static void initClass(void);

protected:
  
  virtual SbBool dragStart(void) = 0;
  virtual void drag(void) = 0;
  virtual void dragFinish(void) = 0;

  SoVRMLDragSensor(void);
  virtual ~SoVRMLDragSensor();

  const SbVec3f & getLocalStartingPoint(void) const;
  const SbMatrix & getLocalToWorldMatrix(void) const;
  const SbMatrix & getWorldToLocalMatrix(void) const;
  const SbViewVolume & getViewVolume(void) const;
  const SbVec2f & getNormalizedLocaterPosition(void) const;

private:
  // FIXME: move to private class
  SbVec3f hitpt;
  SbMatrix obj2world;
  SbMatrix world2obj;
  SbVec2s mousepos;
  SbVec2f normpos;
  SbViewVolume viewvolume;
};

#endif // ! COIN_SOVRMLDRAGSENSOR_H
