/**************************************************************************\
 * 
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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

#ifndef __SOPATH_H__
#define __SOPATH_H__

#include <Inventor/misc/SoBase.h>
#include <Inventor/lists/SbIntList.h>
#include <Inventor/lists/SoNodeList.h>

#if defined(COIN_EXCLUDE_SOPATH)
#error "Configuration settings disrespected -- do not include this file!"
#endif // COIN_EXCLUDE_SOPATH


class SoPath;
typedef SoPath SoLightPath;
typedef SoPath SoTempPath;

class SoNode;
class SoWriteAction;
class SoNotList;
class SoInput;
class SoPathList;

class SoPath : public SoBase {
  typedef SoBase inherited;

  friend class SoAction; // FIXME: needed to compile on SGI? 19980916 mortene.

// The Type System
protected:
  static SoType classTypeId;
public:
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;
  static void * createInstance(void);
  static void initClass(void);
  static void cleanClass(void);

// construction // destruction
public:
  SoPath(const int approxLength = 0);
  SoPath(SoNode * const head);
  SoPath(const SoPath & rhs);
  SoPath & operator = (const SoPath & rhs);
protected:
  virtual ~SoPath(void); // destructed by ref()

public:
  void setHead(SoNode * const head);
  SoNode * getHead(void) const;
  void append(const int childIndex);
  void append(SoNode * const node);
  void append(const SoPath * const fromPath);
  void push(const int childIndex);
  void pop(void);
  SoNode * getTail(void) const;
  SoNode * getNode(const int index) const;
  SoNode * getNodeFromTail(const int index) const;
  int getIndex(const int index) const;
  int getIndexFromTail(const int index) const;
  int getLength(void) const;
  void truncate(const int length);

  int findFork(const SoPath * const path) const;

  SbBool containsNode(const SoNode * const node) const;
  SbBool containsPath(const SoPath * const path) const;
  SoPath * copy(const int startFromNodeIndex = 0,
                 int numNodes = 0) const;
  friend SbBool operator == (const SoPath & lhs, const SoPath & rhs);
  friend SbBool operator != (const SoPath & lhs, const SoPath & rhs);

  static SoPath * getByName(const SbName name);
  static int getByName(const SbName name, SoPathList & list);

// internal (for path updating on graph changes)
  void insertIndex(SoNode * const parent, const int newIndex);
  void removeIndex(SoNode * const parent, const int oldIndex);
  void replaceIndex(SoNode * const parent, const int index,
                     SoNode * const newChild);
  SbBool isRelevantNotification(SoNotList * const list) const;

  virtual void write(SoWriteAction * writeAction) const;

protected:
  void auditPath(const SbBool flag);

  void append(SoNode * const node, const int index);
  SoNodeList nodes;
  SbIntList indices;
  int numPublic;
  int minNumPublic;
  SbBool doAuditors;

  int getFullLength(void) const;

  void truncate(const int length, const SbBool doNotify);

  virtual SbBool readInstance(SoInput * in, unsigned short flags);
};

#ifndef __SOLIB_INTERNAL__
#include <Inventor/SoFullPath.h>
#endif // __SOLIB_INTERNAL__

#endif // !__SOPATH_H__
