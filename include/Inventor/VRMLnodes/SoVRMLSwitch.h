/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 2001 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#ifndef COIN_SOVRMLSWITCH_H
#define COIN_SOVRMLSWITCH_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/fields/SoMFNode.h>
#include <Inventor/fields/SoSFInt32.h>

// undef in case Inventor/nodes/SoSwitch.h has been included first
#undef SO_SWITCH_NONE
#undef SO_SWITCH_INHERIT
#undef SO_SWITCH_ALL

#define SO_SWITCH_NONE     (-1)
#define SO_SWITCH_INHERIT  (-2)
#define SO_SWITCH_ALL      (-3)

class COIN_DLL_API SoVRMLSwitch : public SoGroup
{
  typedef SoGroup inherited;
  SO_NODE_HEADER(SoVRMLSwitch);

public:
  static void initClass(void);
  SoVRMLSwitch(void);
  SoVRMLSwitch( int choices);

  SoMFNode choice;
  SoSFInt32 whichChoice;

  virtual SbBool affectsState(void) const;

  void addChoice(SoNode * choice);
  void insertChoice(SoNode * choice, int idx);
  SoNode * getChoice(int idx) const;
  int findChoice(SoNode * choice) const;
  int getNumChoices(void) const;
  void removeChoice(int idx);
  void removeChoice(SoNode * node);
  void removeAllChoices(void);
  void replaceChoice(int idx, SoNode * choice);
  void replaceChoice(SoNode * old, SoNode * choice);

  virtual void doAction(SoAction * action);
  virtual void callback(SoCallbackAction * action);
  virtual void GLRender(SoGLRenderAction * action);
  virtual void pick(SoPickAction * action);
  virtual void getBoundingBox(SoGetBoundingBoxAction * action);
  virtual void handleEvent(SoHandleEventAction * action);
  virtual void getMatrix(SoGetMatrixAction * action);
  virtual void search(SoSearchAction * action);
  virtual void write(SoWriteAction * action);

  void addChild(SoNode * child);
  void insertChild(SoNode * child, int idx);
  SoNode * getChild(int idx) const;
  int findChild(const SoNode * child) const;
  int getNumChildren(void) const;
  void removeChild(int idx);
  void removeChild(SoNode * child);
  void removeAllChildren(void);
  void replaceChild(int idx, SoNode * node);
  void replaceChild(SoNode * old, SoNode * node);
  virtual SoChildList * getChildren(void) const;

protected:
  virtual ~SoVRMLSwitch(void);

  virtual void notify(SoNotList * list);
  virtual SbBool readInstance(SoInput * in, unsigned short flags);
  virtual void copyContents(const SoFieldContainer * from, SbBool copyConn);

private:
  void commonConstructor(void);
  SbBool childlistvalid;


}; // class SoVRMLSwitch

#endif // ! COIN_SOVRMLSWITCH_H
