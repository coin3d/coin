#ifndef COIN_SOMFNODE_H
#define COIN_SOMFNODE_H

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

#include <Inventor/fields/SoMField.h>
#include <Inventor/fields/SoSubField.h>

class SoNode;

class COIN_DLL_API SoMFNode : public SoMField {
  typedef SoMField inherited;

  SO_MFIELD_HEADER(SoMFNode, SoNode *, SoNode *);

public:
  static void initClass(void);

  virtual void fixCopy(SbBool copyconnections);
  virtual SbBool referencesCopy(void) const;

  void addNode(SoNode * node);
  void insertNode(SoNode * node, int idx);
  SoNode * getNode(int idx) const;
  int findNode(const SoNode * node) const;
  int getNumNodes(void) const;
  void removeNode(int index);
  void removeNode(SoNode * node);
  void removeAllNodes(void);
  void replaceNode(int idx, SoNode * newnode);
  void replaceNode(SoNode * oldnode, SoNode * newnode);
  
  virtual void deleteValues(int start, int num = -1);
  virtual void insertSpace(int start, int num);

private:
  virtual void countWriteRefs(SoOutput * out) const;
};

#endif // !COIN_SOMFNODE_H
