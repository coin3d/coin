#ifndef COIN_SOFILE_H
#define COIN_SOFILE_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
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

  SoGroup * copyChildren(void) const;
  virtual SoChildList * getChildren(void) const;
  virtual void copyContents(const SoFieldContainer * from,
                            SbBool copyconnections);

  const SbString & getFullName(void) const;
  
protected:
  virtual ~SoFile();

  virtual SbBool readInstance(SoInput * in, unsigned short flags);

private:
  SbBool readNamedFile(SoInput * in);
  static void nameFieldModified(void * userdata, SoSensor * sensor);

  SoChildList * children;
  SoFieldSensor * namesensor;
  SbString fullname;
};

#endif // !COIN_SOFILE_H
