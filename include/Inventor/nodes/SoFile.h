#ifndef COIN_SOFILE_H
#define COIN_SOFILE_H

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

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoSFString.h>

class SoFieldSensor;
class SoGroup;
class SoSensor;

class COIN_DLL_API SoFile : public SoNode {
  typedef SoNode inherited;

  SO_NODE_HEADER(SoFile);

public:
  static void initClass(void);
  SoFile(void);

  SoSFString name;

  virtual void doAction(SoAction * action);
  virtual void callback(SoCallbackAction * action);
  virtual void GLRender(SoGLRenderAction * action);
  virtual void getBoundingBox(SoGetBoundingBoxAction * action);
  virtual void getMatrix(SoGetMatrixAction * action);
  virtual void handleEvent(SoHandleEventAction * action);
  virtual void pick(SoPickAction * action);
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);
  virtual void audioRender(SoAudioRenderAction * action);

  SoGroup * copyChildren(void) const;
  virtual SoChildList * getChildren(void) const;
  virtual void copyContents(const SoFieldContainer * from,
                            SbBool copyconnections);

  const SbString & getFullName(void) const;

protected:
  virtual ~SoFile();

  virtual SbBool readInstance(SoInput * in, unsigned short flags);
  virtual SbBool readNamedFile(SoInput * in);

private:
  static void nameFieldModified(void * userdata, SoSensor * sensor);

  SoChildList * children;
  SoFieldSensor * namesensor;
  SbString fullname;
};

#endif // !COIN_SOFILE_H
