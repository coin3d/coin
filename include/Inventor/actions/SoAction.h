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

#ifndef COIN_SOACTION_H
#define COIN_SOACTION_H

#include <Inventor/SbBasic.h>
#include <Inventor/SoType.h>
#include <Inventor/misc/SoTempPath.h>

// Include instead of using a forward declaration to be compatible
// with Open Inventor (and besides, all the other action class
// definitions needs to know about the SoActionMethodList).
#include <Inventor/lists/SoActionMethodList.h>


// Avoid problem with HPUX 10.20 C library API headers, which defines
// IN_PATH in <sys/unistd.h>. IN_PATH is "re-defined" at the end of
// this file.
#if defined(IN_PATH)
#define SOACTION_STORE_INPATH_DEF IN_PATH
#undef IN_PATH
#endif // IN_PATH already defined


/*!
  The SO_ENABLE macro is a convenient mechanism for letting an action
  class specify which elements it needs during its operation.
*/
#define SO_ENABLE(action, element) \
  do { \
    assert(!element::getClassTypeId().isBad()); \
    action::enableElement(element::getClassTypeId(), \
                          element::getClassStackIndex()); \
  } while (0)


class SoEnabledElementsList;
class SoNode;
class SoPath;
class SoPathList;
class SoState;


class SoAction {
public:
  enum AppliedCode { NODE = 0, PATH = 1, PATH_LIST = 2 };
  enum PathCode { NO_PATH = 0, IN_PATH = 1, BELOW_PATH = 2, OFF_PATH = 3 };

  virtual ~SoAction();

  static void initClass(void);
  static void initClasses(void);
  
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const = 0;
  virtual SbBool isOfType(SoType type) const;


  virtual void apply(SoNode * root);
  virtual void apply(SoPath * path);
  virtual void apply(const SoPathList & pathlist, SbBool obeysrules = FALSE);
  virtual void invalidateState(void);

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

  void switchToPathTraversal(SoPath * path);
  void switchToNodeTraversal(SoNode * node);


protected:
  SoAction(void);

  virtual void beginTraversal(SoNode * node);
  virtual void endTraversal(SoNode * node);
  void setTerminated(const SbBool flag);

  virtual const SoEnabledElementsList & getEnabledElements(void) const;
  virtual SbBool shouldCompactPathList(void) const;

  SoState * state;
  SoActionMethodList * traversalMethods;
  static SoEnabledElementsList * enabledElements;
  static SoActionMethodList * methods;

private:
  static SoType classTypeId;

  AppliedCode appliedcode;

  union AppliedData {
    SoNode * node;
    SoPath * path;
    struct {
      const SoPathList * pathlist;
      const SoPathList * origpathlist;
    } pathlistdata;
  } applieddata;

  SoTempPath currentpath;
  SbBool terminated;
  PathCode currentpathcode;
  int pathcodearray[1];
};

// Avoid problem with HPUX API headers (see near the start of this
// file).
#if defined(SOACTION_STORE_INPATH_DEF)
#define IN_PATH SOACTION_STORE_INPATH_DEF
#undef SOACTION_STORE_INPATH_DEF
#endif // SOACTION_STORE_INPATH_DEF

#endif // !COIN_SOACTION_H
