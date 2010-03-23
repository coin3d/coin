#ifndef COIN_SONODEVISUALIZE_H
#define COIN_SONODEVISUALIZE_H

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

#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/lists/SoNodeList.h>
#include <Inventor/tools/SbPimplPtr.h>
#include <Inventor/SbVec2s.h>
#include <Inventor/SbVec3f.h>

// *************************************************************************

// FIXME: i don't like the name of this class. "SoScenegraphStructure"
// or some such would be better.  -mortene.

class SoProfilerStats;

class COIN_DLL_API SoNodeVisualize : public SoBaseKit
{
  typedef SoBaseKit inherited;

  SO_KIT_HEADER(SoNodeVisualize);
  SO_KIT_CATALOG_ENTRY_HEADER(topSeparator);
  SO_KIT_CATALOG_ENTRY_HEADER(childrenVisible);
  SO_KIT_CATALOG_ENTRY_HEADER(color);
  SO_KIT_CATALOG_ENTRY_HEADER(texture);
  SO_KIT_CATALOG_ENTRY_HEADER(textureTransform);
  SO_KIT_CATALOG_ENTRY_HEADER(shape);
  SO_KIT_CATALOG_ENTRY_HEADER(rotSwitch);
  SO_KIT_CATALOG_ENTRY_HEADER(rotation);
  SO_KIT_CATALOG_ENTRY_HEADER(childGeometry);
  SO_KIT_CATALOG_ENTRY_HEADER(translation);
  SO_KIT_CATALOG_ENTRY_HEADER(lines);
  SO_KIT_CATALOG_ENTRY_HEADER(lineSep);

public:
  static void initClass(void);
  static void cleanClass(void);

  SoNodeVisualize * visualize(SoNode *);
  static SoNodeVisualize* visualizeTree(SoNode * node,int depth=-1);
  bool clicked();
  void setAlternate(bool alternating=true);
  bool nodeHasChildren();
  unsigned int nodeNumChildren();
  bool isAlternating() const;

  virtual void handleEvent(SoHandleEventAction * action);

  void traverse(SoProfilerStats * stats);

protected:
  SoNodeVisualize(void);
  virtual ~SoNodeVisualize();
  SbVec2s getWidth() ;

  void setupChildCatalog(SoNode * node, int depth);
  void visualizeSubTree(SoNode * node,int depth=-1);
  void recalculate();
  SbVec2s recalculateWidth();
  SoNodeVisualize* getSoNodeVisualizeRoot();
  void internalAlternating(bool alternating, int direction);
  void reset();
  SoNodeList * getChildGeometry();

 private:
  SbPimplPtr<class SoNodeVisualizeP> pimpl;

  // NOT IMPLEMENTED
  SoNodeVisualize(const SoNodeVisualize &);
  SoNodeVisualize & operator = (const SoNodeVisualize &);

  bool dirty;
  SbVec2s width;
  SoNodeVisualize *parent;

  SoNode *node;
};

#endif // !COIN_SONODEVISUALIZE_H
