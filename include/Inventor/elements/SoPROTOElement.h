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

#ifndef __SOPROTOELEMENT_H__
#define __SOPROTOELEMENT_H__

#include <Inventor/elements/SoElement.h>

class SoPROTOElement : public SoElement {
  typedef SoElement inherited;

//$ BEGIN TEMPLATE ElementHeader(SoPROTOElement)
public:
  static SoType classTypeId;
  static SoType getClassTypeId(void);
  static void * createInstance(void);
public:
  static int getClassStackIndex(void);
  virtual ~SoPROTOElement(void);

  static void initClass(void);

protected:
  SoPROTOElement(void);
  static int classStackIndex;
//$ END TEMPLATE ElementHeader

public:
  virtual void init(SoState * state);

  virtual void push(SoState * state);
  virtual void pop(SoState * state,
              const SoElement * prevTopElement);

  virtual SbBool matches(const SoElement * element) const;
  virtual SoElement * copyMatchInfo(void) const;

  // FIXME: quick hack. 19980810 mortene.
  class SoPROTO *nodeptr;
  // larsa: SoElement got dataNode member for this :(
  // - might fix this later
};

#endif // !__SOPROTOELEMENT_H__
