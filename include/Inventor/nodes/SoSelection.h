#ifndef COIN_SOSELECTION_H
#define COIN_SOSELECTION_H

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

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/lists/SoPathList.h>

class SoSelection;
class SoPath;
class SoPickedPoint;
class SoCallbackList;

typedef void SoSelectionPathCB(void * data, SoPath * path);
typedef void SoSelectionClassCB(void * data, SoSelection * sel);
typedef SoPath * SoSelectionPickCB(void * data, const SoPickedPoint * pick);

class COIN_DLL_API SoSelection : public SoSeparator {
  typedef SoSeparator inherited;

  SO_NODE_HEADER(SoSelection);

public:
  static void initClass(void);
  SoSelection(void);

  enum Policy {
    SINGLE, TOGGLE, SHIFT
  };

  SoSFEnum policy;

  SoSelection(const int nChildren);

  void select(const SoPath * path);
  void select(SoNode *node);
  void deselect(const SoPath * path);
  void deselect(const int which);
  void deselect(SoNode * node);
  void toggle(const SoPath * path);
  void toggle(SoNode * node);
  SbBool isSelected(const SoPath * path) const;
  SbBool isSelected(SoNode * node) const;
  void deselectAll(void);
  int getNumSelected(void) const;
  const SoPathList * getList(void) const;
  SoPath * getPath(const int index) const;
  SoPath * operator[](const int i) const;
  void addSelectionCallback(SoSelectionPathCB * f, void * userData = NULL);
  void removeSelectionCallback(SoSelectionPathCB * f, void * userData = NULL);
  void addDeselectionCallback(SoSelectionPathCB * f, void * userData = NULL);
  void removeDeselectionCallback(SoSelectionPathCB * f,
                                 void * userData = NULL);
  void addStartCallback(SoSelectionClassCB * f, void * userData = NULL);
  void removeStartCallback(SoSelectionClassCB * f, void * userData = NULL);
  void addFinishCallback(SoSelectionClassCB * f, void * userData = NULL);
  void removeFinishCallback(SoSelectionClassCB * f, void * userData = NULL);
  void setPickFilterCallback(SoSelectionPickCB * f, void * userData = NULL,
                             const SbBool callOnlyIfSelectable = TRUE);
  void setPickMatching(const SbBool pickMatching);
  SbBool isPickMatching(void) const;
  SbBool getPickMatching(void) const;
  void addChangeCallback(SoSelectionClassCB * f, void * userData = NULL);
  void removeChangeCallback(SoSelectionClassCB * f, void * userData = NULL);

protected:
  virtual ~SoSelection();

  void invokeSelectionPolicy(SoPath *path, SbBool shiftDown);
  void performSingleSelection(SoPath *path);
  void performToggleSelection(SoPath *path);
  SoPath * copyFromThis(const SoPath * path) const;
  void addPath(SoPath *path);
  void removePath(const int which);
  int findPath(const SoPath *path) const;

  virtual void handleEvent(SoHandleEventAction * action);

protected: // unfortunately only protected in OIV

  SoPathList selectionList;

  SoCallbackList *selCBList;
  SoCallbackList *deselCBList;
  SoCallbackList *startCBList;
  SoCallbackList *finishCBList;

  SoSelectionPickCB *pickCBFunc;
  void *pickCBData;
  SbBool callPickCBOnlyIfSelectable;

  SoCallbackList *changeCBList;

  SoPath *mouseDownPickPath;
  SbBool pickMatching;

private:
  void init();
  SoPath *searchNode(SoNode * node) const;
  SoPath *getSelectionPath(SoHandleEventAction *action,
                           SbBool &ignorepick, SbBool &haltaction);
};

#endif // !COIN_SOSELECTION_H
