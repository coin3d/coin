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

#ifndef __SOPICKACTION_H__
#define __SOPICKACTION_H__

#include <Inventor/actions/SoAction.h>
#include <Inventor/SbViewportRegion.h>

class SbViewportRegion;


class SoPickAction : public SoAction {
  typedef SoAction inherited;

//$ BEGIN TEMPLATE ActionHeader(SoPickAction)
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

  virtual ~SoPickAction();
//$ END TEMPLATE ActionHeader

public:
  void enableCulling(const SbBool flag);
  SbBool isCullingEnabled() const;

protected:
  SoPickAction(const SbViewportRegion & viewportRegion);
  virtual void beginTraversal(SoNode *node);
  SbViewportRegion vpRegion;

private:
  SbBool cullingEnabled;
};

#endif // !__SOPICKACTION_H__
