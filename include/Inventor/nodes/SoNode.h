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

#ifndef __SONODE_H__
#define __SONODE_H__

#include <Inventor/fields/SoFieldContainer.h>


class SoAction;
class SoCallbackAction;
class SoChildList;
class SoGLRenderAction;
class SoGetBoundingBoxAction;
class SoGetMatrixAction;
class SoGetPrimitiveCountAction;
class SoHandleEventAction;
class SoNodeList;
class SoNotList;
class SoOutput;
class SoPickAction;
class SoRayPickAction;
class SoSearchAction;
class SoWriteAction;


class SoNode : public SoFieldContainer {
  typedef SoFieldContainer inherited;

public:
  enum Stage { FIRST_INSTANCE,
               PROTO_INSTANCE,
               OTHER_INSTANCE
  };

  void setOverride(const SbBool state);
  SbBool isOverride(void) const;

  SoNode * copy(SbBool copyConnections = FALSE) const;
  virtual SbBool affectsState(void) const;

  virtual void doAction(SoAction * action);
  virtual void GLRender(SoGLRenderAction * action);
  virtual void GLRenderBelowPath(SoGLRenderAction * action);
  virtual void GLRenderInPath(SoGLRenderAction * action);
  virtual void GLRenderOffPath(SoGLRenderAction * action);
  virtual void callback(SoCallbackAction * action);
  virtual void getBoundingBox(SoGetBoundingBoxAction * action);
  virtual void getMatrix(SoGetMatrixAction * action);
  virtual void handleEvent(SoHandleEventAction * action);
  virtual void pick(SoPickAction * action);
  virtual void rayPick(SoRayPickAction * action);
  virtual void search(SoSearchAction * action);
  virtual void write(SoWriteAction * action);
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);

  virtual void grabEventsSetup(void);
  virtual void grabEventsCleanup(void);

  virtual void startNotify(void);
  virtual void notify(SoNotList * list);

  uint32_t getNodeId(void) const;
  virtual SoChildList * getChildren(void) const;

  virtual void writeInstance(SoOutput * out);
  virtual SoNode * addToCopyDict(void) const;
  virtual void copyContents(const SoFieldContainer * fromFC,
                            SbBool copyConnections);
  virtual SoFieldContainer * copyThroughConnection(void) const;


  static SoType getClassTypeId(void);
  static SoNode * getByName(const SbName & name);
  static int getByName(const SbName & name, SoNodeList & list);

  static void initClass(void);
  static void initClasses(void);

  static uint32_t getNextNodeId(void);
  static int getActionMethodIndex(const SoType type);

  static void getBoundingBoxS(SoAction * const action,SoNode * const node);
  static void GLRenderS(SoAction * const action, SoNode * const node);
  static void callbackS(SoAction * const action, SoNode * const node);
  static void getMatrixS(SoAction * const action, SoNode * const node);
  static void handleEventS(SoAction * const action, SoNode * const node);
  static void pickS(SoAction * const action, SoNode * const node);
  static void rayPickS(SoAction * const action, SoNode * const node);
  static void searchS(SoAction * const action, SoNode * const node);
  static void writeS(SoAction * const action, SoNode * const node);
  static void getPrimitiveCountS(SoAction * const action, SoNode * const node);

protected:
  SoNode(void);
  virtual ~SoNode();

  static const SoFieldData ** getFieldDataPtr(void);

  uint32_t uniqueId;
  static uint32_t nextUniqueId;
  static int nextActionMethodIndex;

private:
  Stage stage;
  static SoType classTypeId;

  struct {
    unsigned int override : 1;
  } stateflags;
};

#endif // !__SONODE_H__
