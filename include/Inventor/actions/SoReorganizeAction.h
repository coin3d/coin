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

#ifndef __SOREORGANIZEACTION_H__
#define __SOREORGANIZEACTION_H__

#include <Inventor/confdep.h>
#if defined(COIN_EXCLUDE_SOREORGANIZEACTION)
#error "This file is excluded from Coin through the configuration system!"
#endif // COIN_EXCLUDE_SOREORGANIZEACTION

#include <Inventor/actions/SoSimplifyAction.h>

class SoReorganizeAction : public SoSimplifyAction {
  typedef SoSimplifyAction inherited;

//$ BEGIN TEMPLATE ActionHeader(SoReorganizeAction)
private:
  static SoType classTypeId;

public:
  virtual SoType getTypeId(void) const;
  static SoType getClassTypeId(void);

protected:
  virtual const SoEnabledElementsList & getEnabledElements(void) const;
  static SoEnabledElementsList * enabledElements;
  static SoActionMethodList * methods;

public:
  static void addMethod(const SoType type, SoActionMethod method);
  static void enableElement(const SoType type, const int stackIndex);

  static void initClass(void);

  virtual ~SoReorganizeAction();
//$ END TEMPLATE ActionHeader

public:
  SoReorganizeAction(void);

protected:
  virtual void beginTraversal(SoNode * node);

};

#endif // !__SOREORGANIZEACTION_H__
