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

#ifndef __SOSELECTION_H__
#define __SOSELECTION_H__

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/fields/SoSFEnum.h>

#if defined(COIN_EXCLUDE_SOSELECTION)
#error Configuration settings disrespected -- do not include this file!
#endif // COIN_EXCLUDE_SOSELECTION

class SoPathList;
class SoSelection;
class SoPath;
class SoPickedPoint;

typedef void SoSelectionPathCB(void * data, SoPath * path);
typedef void SoSelectionClassCB(void * data, SoSelection * sel);
typedef SoPath * SoSelectionPickCB(void * data, const SoPickedPoint * pick);

// *************************************************************************

class SoSelection : public SoSeparator {
  typedef SoSeparator inherited;

  SO_NODE_HEADER(SoSelection);

public:
  static void initClass(void);
  SoSelection(void);

  enum Policy {
    SINGLE, TOGGLE, SHIFT
  };

  SoSFEnum policy;

  SoSelection(int nChildren);

  void select(const SoPath * path);
  void select(SoNode * node);
  void deselect(const SoPath * path);
  void deselect(int which);
  void deselect(SoNode * node);
  void toggle(const SoPath * path);
  void toggle(SoNode * node);
  SbBool isSelected(const SoPath * path) const;
  SbBool isSelected(SoNode * node) const;
  void deselectAll(void);
  int getNumSelected(void) const;
  const SoPathList * getList(void) const;
  SoPath * getPath(int index) const;
  SoPath * operator[](int i) const;
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
			     SbBool callOnlyIfSelectable = TRUE);
  void setPickMatching(SbBool pickTwice);
  SbBool isPickMatching(void) const;
  SbBool getPickMatching(void) const;
  void addChangeCallback(SoSelectionClassCB * f, void * userData = NULL);
  void removeChangeCallback(SoSelectionClassCB * f, void * userData = NULL);

protected:
  virtual ~SoSelection();

  void invokeSelectionPolicy(SoPath * path, SbBool shiftDown);
  void performSingleSelection(SoPath * path);
  void performToggleSelection(SoPath * path);
  SoPath * copyFromThis(const SoPath * path) const;
  void addPath(SoPath * path);
  void removePath(int which);
  int findPath(const SoPath * path) const;

  virtual void handleEvent(SoHandleEventAction * action);
};

#endif // !__SOSELECTION_H__
