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

#ifndef COIN_SOELEMENT_H
#define COIN_SOELEMENT_H

#include <Inventor/SbBasic.h>
#include <Inventor/SoType.h>
#include <stdio.h>

#ifdef COIN_INTERNAL
class SoState;
#else // !COIN_INTERNAL
// For compatibility with applications originally developed on top of
// Open Inventor.
#include <Inventor/misc/SoState.h>
#endif // !COIN_INTERNAL

class SoNode;


class COIN_DLL_EXPORT SoElement {
public:
  static void initClass(void);

  static SoType getClassTypeId(void);
  static int getClassStackIndex(void);
  const SoType getTypeId(void) const;
  int getStackIndex(void) const;

  virtual void init(SoState * state);

  virtual void push(SoState * state);
  virtual void pop(SoState * state, const SoElement * prevTopElement);

  virtual SbBool matches(const SoElement * element) const = 0;
  virtual SoElement * copyMatchInfo(void) const = 0;

  static void initElements(void); // only for base class (SoElement)

  static int getNumStackIndices(void);
  static SoType getIdFromStackIndex(const int stackIndex);

  void setDepth(const int depth);

  int getDepth(void) const;

  virtual void print(FILE * file = stdout) const;
  virtual ~SoElement();

protected:
  SoElement(void);
  static int classStackIndex;

  static SoElement * getElement(SoState * const state, const int stackIndex);
  static const SoElement * getConstElement(SoState * const state,
                                           const int stackIndex);

  void capture(SoState * const state) const;
  virtual void captureThis(SoState * state) const;

  void setTypeId(const SoType typeId);
  void setStackIndex(const int index);

  SoType typeId;
  int stackIndex;

  static int createStackIndex(const SoType id);
  static SoTypeList * stackToType;

  int depth;

  SoElement * next;
  SoElement * prev;

private:
  static SoType classTypeId;

  friend class SoState; // FIXME: bad design. 19990629 mortene.
  static void cleanup(void);
};

#endif // !COIN_SOELEMENT_H
