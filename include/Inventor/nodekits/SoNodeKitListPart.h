#ifndef COIN_SONODEKITLISTPART_H
#define COIN_SONODEKITLISTPART_H

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

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/lists/SoTypeList.h>
#include <Inventor/fields/SoSFNode.h>
#include <Inventor/fields/SoSFName.h>
#include <Inventor/fields/SoMFName.h>

class SoGroup;


class COIN_DLL_API SoNodeKitListPart : public SoNode {
  typedef SoNode inherited;

  SO_NODE_HEADER(SoNodeKitListPart);

public:
  static void initClass(void);
  SoNodeKitListPart(void);

  SoType getContainerType(void) const;
  void setContainerType(SoType newContainerType);
  const SoTypeList & getChildTypes(void) const;
  void addChildType(SoType typeToAdd);
  SbBool isTypePermitted(SoType typeToCheck) const;
  SbBool isChildPermitted(const SoNode * child) const;
  void containerSet(const char * fieldDataString);
  void lockTypes(void);
  SbBool isTypeLocked(void) const;
  void addChild(SoNode * child);
  void insertChild(SoNode * child, int childIndex);
  SoNode * getChild(int index) const;
  int findChild(SoNode * child) const;
  int getNumChildren(void) const;
  void removeChild(int index);
  void removeChild(SoNode * child);
  void replaceChild(int index, SoNode * newChild);
  void replaceChild(SoNode * oldChild, SoNode * newChild);
  virtual SbBool affectsState(void) const;
  virtual void doAction(SoAction * action);
  virtual void callback(SoCallbackAction * action);
  virtual void GLRender(SoGLRenderAction * action);
  virtual void getBoundingBox(SoGetBoundingBoxAction * action);
  virtual void getMatrix(SoGetMatrixAction * action);
  virtual void handleEvent(SoHandleEventAction * action);
  virtual void pick(SoPickAction * action);
  virtual void search(SoSearchAction * action);
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);
  virtual SoChildList * getChildren(void) const;

protected:
  virtual ~SoNodeKitListPart();

  SoGroup * getContainerNode(void);
  virtual SbBool readInstance(SoInput * in, unsigned short flags);
  virtual void copyContents(const SoFieldContainer * fromFC,
                            SbBool copyConnections);
  SoChildList * children;

private:
  void syncInternalData(void);

  SoSFNode containerNode;
  SoSFName containerTypeName;
  SoMFName childTypeNames;

  SbBool typelistlocked;
  SoTypeList allowedtypes;

  SbBool canCreateDefaultChild(void) const;
  SoNode * createAndAddDefaultChild(void);
  SoType getDefaultChildType(void) const;

  friend class SoBaseKit;
};

#endif // !COIN_SONODEKITLISTPART_H
