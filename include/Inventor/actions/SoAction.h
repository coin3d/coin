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

#ifndef __SOACTION_H__
#define __SOACTION_H__

#include <Inventor/SbBasic.h>
#include <Inventor/SoType.h>
#include <Inventor/SoPath.h>
#include <Inventor/lists/SoActionMethodList.h>

#define SO_ENABLE(actionClass, elementClass) \
  assert(! elementClass::getClassTypeId().isBad()); \
  actionClass::enableElement(elementClass::getClassTypeId(), \
                             elementClass::getClassStackIndex())

#define ENABLE_ELEMENT(element) \
  assert(! element::getClassTypeId().isBad()); \
  enableElement(element::getClassTypeId(), element::getClassStackIndex())

class SoCompactPathList;
class SoEnabledElementsList;
class SoNode;
class SoPath;
class SoPathList;
class SoState;


class SoAction {
public:
  static void initClass(void);

  static  SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;
  virtual ~SoAction();

protected:
  virtual const SoEnabledElementsList & getEnabledElements(void) const;
  static SoEnabledElementsList * enabledElements;
  static SoActionMethodList * methods;

private:
  static SoType classTypeId;

  // end of code defined in template

protected:
  SoAction(void); // abstract class

public:
  static void initClasses(void) { initActions(); }; // OI compatibility.

public:
  virtual void apply(SoNode * rootNode);
  virtual void apply(SoPath * path);
  virtual void apply(const SoPathList & pathList, SbBool obeysRules = FALSE);
  virtual void invalidateState(void);

  enum AppliedCode { NODE       = 0,
                     PATH       = 1,
                     PATH_LIST  = 2
  };

  enum PathCode {
    NO_PATH       = 0,
    IN_PATH       = 1,
    BELOW_PATH    = 2,
    OFF_PATH      = 3
  };

  static void nullAction(SoAction * action, SoNode * node);

  AppliedCode getWhatAppliedTo(void) const;
  SoNode * getNodeAppliedTo(void) const;
  SoPath * getPathAppliedTo(void) const;
  const SoPathList * getPathListAppliedTo(void) const;
  const SoPathList * getOriginalPathListAppliedto(void) const;

  SbBool isLastPathListAppliedTo(void) const;

  PathCode getPathCode(int & numIndices, const int * & indices);
  void traverse(SoNode * const node);

  SbBool hasTerminated(void) const;

  const SoPath * getCurPath(void);
  SoState * getState(void) const;

  PathCode getCurPathCode(void) const;

  void pushCurPath(const int childIndex);
  void popCurPath(const PathCode prevPathCode);
  virtual SoNode * getCurPathTail(void);
  void usePathCode(int & numIndices, const int * & indices);
  void pushCurPath(void);

  void popPushCurPath(const int childIndex);
  void popCurPath(void);

  void switchToPathTraversal(SoPath *path);
  void switchToNodeTraversal(SoNode *node);

protected:
  virtual void beginTraversal(SoNode * node);
  virtual void endTraversal(SoNode * node);
  void setTerminated(const SbBool flag);

  virtual SbBool shouldCompactPathList(void) const;

  SoState * state;
  SoActionMethodList * traversalMethods;

private:
  static void initActions(void);

  AppliedCode appliedCode;
  union AppliedData {
    SoNode * node;
    SoPath * path;
    struct {
      const SoPathList * pathList;
      const SoPathList * origPathList;
    } pathListData;
  } appliedData;

  SoPath currentPath;
  SbBool isTerminated;
  PathCode currentPathCode;
  int dummyArray[1];
  int nextInPathChildIndex;
};

#endif // !__SOACTION_H__
