/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
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

#ifndef COIN_SOMFNODE_H
#define COIN_SOMFNODE_H

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
