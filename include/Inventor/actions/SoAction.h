#ifndef COIN_SOACTION_H
#define COIN_SOACTION_H

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

#include <Inventor/SbBasic.h>
#include <Inventor/SoType.h>
#include <Inventor/misc/SoTempPath.h>

// Include instead of using forward declarations to be compatible with
// Open Inventor (and besides, all the other action class definitions
// needs to know about these lists).
#include <Inventor/lists/SoActionMethodList.h>
#include <Inventor/lists/SoEnabledElementsList.h>

// defined in Inventor/SbBasic.h
#ifdef COIN_UNDEF_IN_PATH_HACK
#include <sys/unistd.h>
#undef IN_PATH
// Avoid problem with HPUX 10.20 C library API headers, which defines IN_PATH
// in <sys/unistd.h>.  That define destroys the SoAction::PathCode enum, and
// the preprocessor is so broken that we can't store/restore the define for
// the duration of this header file.
#endif


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
class SoActionP;

class COIN_DLL_API SoAction {
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
  const SoPathList * getOriginalPathListAppliedTo(void) const;

  SbBool isLastPathListAppliedTo(void) const;

  PathCode getPathCode(int & numindices, const int * & indices);

  void traverse(SoNode * const node);
  SbBool hasTerminated(void) const;

  const SoPath * getCurPath(void);
  SoState * getState(void) const;

  PathCode getCurPathCode(void) const;
  virtual SoNode * getCurPathTail(void);
  void usePathCode(int & numindices, const int * & indices);
  
  void pushCurPath(const int childindex, SoNode * node = NULL);
  void popCurPath(const PathCode prevpathcode);
  void pushCurPath(void);
  
  void popPushCurPath(const int childindex, SoNode * node = NULL);
  void popCurPath(void);

public:
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

  /* These two methods are not available in the original OIV API.  The
     reason they have been added is explained in SoSubAction.h for the
     SO_ACTION_HEADER macro. */
  static SoEnabledElementsList * getClassEnabledElements(void);
  static SoActionMethodList * getClassActionMethods(void);

private:
  static SoType classTypeId;
  /* The enabledElements and methods variables are protected in the
     original OIV API. This is not such a good idea, see the comments
     in SoSubAction.h for SO_ACTION_HEADER. */
  static SoEnabledElementsList * enabledElements;
  static SoActionMethodList * methods;

  SoTempPath currentpath;
  PathCode currentpathcode;

  SoActionP * pimpl;
};

// inline methods

inline SoAction::PathCode
SoAction::getCurPathCode(void) const
{
  return this->currentpathcode;
}

inline void
SoAction::popCurPath(const PathCode prevpathcode)
{
  this->currentpath.pop();
  this->currentpathcode = prevpathcode;
}

#endif // !COIN_SOACTION_H
