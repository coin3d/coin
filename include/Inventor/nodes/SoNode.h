#ifndef COIN_SONODE_H
#define COIN_SONODE_H

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
class SbDict;

class COIN_DLL_API SoNode : public SoFieldContainer {
  typedef SoFieldContainer inherited;

public:
  // Probably not interesting for the application programmer..?
#ifndef DOXYGEN_SKIP_THIS
  enum Stage { FIRST_INSTANCE, PROTO_INSTANCE, OTHER_INSTANCE };
#endif // !DOXYGEN_SKIP_THIS

  void setOverride(const SbBool state);
  SbBool isOverride(void) const;

  enum NodeType {
    INVENTOR     = 0x000,
    VRML1        = 0x001,
    VRML2        = 0x002,
    INVENTOR_1   = 0x004,
    INVENTOR_2_0 = 0x008,
    INVENTOR_2_1 = 0x010,
    INVENTOR_2_5 = 0x020,
    INVENTOR_2_6 = 0x040,
    COIN_1_0     = 0x080,
    COIN_2_0     = 0x100,
    EXTENSION    = 0x200
  };

  static uint32_t getCompatibilityTypes(const SoType & nodetype);
  void setNodeType(const NodeType type);
  NodeType getNodeType(void) const;

  virtual SoNode * copy(SbBool copyconnections = FALSE) const;
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
  virtual void notify(SoNotList * l);

  uint32_t getNodeId(void) const;
  virtual SoChildList * getChildren(void) const;

  virtual void writeInstance(SoOutput * out);
  virtual SoNode * addToCopyDict(void) const;
  virtual void copyContents(const SoFieldContainer * from,
                            SbBool copyconnections);
  virtual SoFieldContainer * copyThroughConnection(void) const;


  static SoType getClassTypeId(void);
  static SoNode * getByName(const SbName & name);
  static int getByName(const SbName & name, SoNodeList & l);

  static void initClass(void);
  static void initClasses(void);

  static uint32_t getNextNodeId(void);
  static int getActionMethodIndex(const SoType type);

  static void getBoundingBoxS(SoAction * action, SoNode * node);
  static void GLRenderS(SoAction * action, SoNode * node);
  static void callbackS(SoAction * action, SoNode * node);
  static void getMatrixS(SoAction * action, SoNode * node);
  static void handleEventS(SoAction * action, SoNode * node);
  static void pickS(SoAction * action, SoNode * node);
  static void rayPickS(SoAction * action, SoNode * node);
  static void searchS(SoAction * action, SoNode * node);
  static void writeS(SoAction * action, SoNode * node);
  static void getPrimitiveCountS(SoAction * action, SoNode * node);

protected:
  SoNode(void);
  virtual ~SoNode();

  virtual SbBool readInstance(SoInput * in, unsigned short flags);

  static const SoFieldData ** getFieldDataPtr(void);

  // These are necessary to avoid problems with us not exporting the
  // nextActionMethodIndex member into Win32 DLLs (we'll get
  // unresolved symbol for extension node classes if the SoSubNode
  // macros accesses the nextActionMethodIndex directly).
  static void setNextActionMethodIndex(int index);
  static int getNextActionMethodIndex(void);
  static void incNextActionMethodIndex(void);

  static void setCompatibilityTypes(const SoType & nodetype, const uint32_t bitmask);

  uint32_t uniqueId;
  static uint32_t nextUniqueId;
  static int nextActionMethodIndex;

private:
  static SoType classTypeId;
  uint32_t stateflags;
  void clearStateFlags(const unsigned int bits);
  void setStateFlags(const unsigned int bits);
  SbBool getState(const unsigned int bits) const;
  static SbDict * compatibilitydict;
  static void cleanupClass(void);
};

#endif // !COIN_SONODE_H
