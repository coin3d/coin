/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
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

class SoVRMLSwitchP;

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
  virtual void audioRender(SoAudioRenderAction * action);

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
  SoVRMLSwitchP * pimpl;
  friend class SoVRMLSwitchP;
}; // class SoVRMLSwitch

#endif // ! COIN_SOVRMLSWITCH_H
