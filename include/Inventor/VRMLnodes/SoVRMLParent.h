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

#ifndef COIN_SOVRMLPARENT_H
#define COIN_SOVRMLPARENT_H

#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoMFNode.h>

class SoVRMLParentP;
class SoSensor;

class COIN_DLL_API SoVRMLParent : public SoGroup
{
  typedef SoGroup inherited;
  SO_NODE_ABSTRACT_HEADER(SoVRMLParent);

public:
  static void initClass(void);

  SoMFNode children;

  virtual SbBool affectsState(void) const;
  virtual void addChild(SoNode * child);
  virtual void insertChild(SoNode * child, int idx);
  virtual SoNode * getChild(int idx) const;
  virtual int findChild(const SoNode * child) const;
  virtual int getNumChildren(void) const;
  virtual void removeChild(int idx);
  virtual void removeChild(SoNode * child);
  virtual void removeAllChildren(void);
  virtual void replaceChild(int idx, SoNode * child);
  virtual void replaceChild(SoNode * old, SoNode * child);
  virtual SoChildList * getChildren(void) const;

  virtual void doAction(SoAction * action);
  virtual void search(SoSearchAction * action);
  virtual void write(SoWriteAction * action);
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);

  static void updateChildList(const SoNode * const * nodes, const int numnodes,
                              SoChildList & cl);
  static void updateChildList(SoNode * nodewithsfnode,
                              SoChildList & cl);

protected:
  SoVRMLParent(void);
  SoVRMLParent(int approxchildren);
  virtual ~SoVRMLParent();

  SoMFNode addChildren;
  SoMFNode removeChildren;

  virtual void notify(SoNotList * list);
  virtual SbBool readInstance(SoInput * in, unsigned short flags);
  virtual void copyContents(const SoFieldContainer * from, SbBool copyConn);

private:
  static void field_sensor_cb(void * data, SoSensor * sensor);
  void commonConstructor(void);
  void processAddChildren(void);
  void processRemoveChildren(void);
  SoFieldData * makeWriteData(void);
  
  SoVRMLParentP * pimpl;
}; // class SoVRMLParent

#endif // ! COIN_SOVRMLPARENT_H
