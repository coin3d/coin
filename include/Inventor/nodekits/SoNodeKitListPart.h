/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
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

#ifndef COIN_SONODEKITLISTPART_H
#define COIN_SONODEKITLISTPART_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/lists/SoTypeList.h>
#include <Inventor/fields/SoSFNode.h>
#include <Inventor/fields/SoSFName.h>
#include <Inventor/fields/SoMFName.h>

class SoGroup;


class SoNodeKitListPart : public SoNode
{
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

  friend class SoBaseKit;
};

#endif // !COIN_SONODEKITLISTPART_H
