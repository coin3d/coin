/**************************************************************************\
 * Copyright (c) Kongsberg Oil & Gas Technologies AS
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

  SbBool affectsState(void) const override;
  void addChild(SoNode * child) override;
  void insertChild(SoNode * child, int idx) override;
  SoNode * getChild(int idx) const override;
  int findChild(const SoNode * child) const override;
  int getNumChildren(void) const override;
  void removeChild(int idx) override;
  void removeChild(SoNode * child) override;
  void removeAllChildren(void) override;
  void replaceChild(int idx, SoNode * child) override;
  void replaceChild(SoNode * old, SoNode * child) override;
  SoChildList * getChildren(void) const override;

  void doAction(SoAction * action) override;
  void search(SoSearchAction * action) override;
  void write(SoWriteAction * action) override;
  void getPrimitiveCount(SoGetPrimitiveCountAction * action) override;

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

  void notify(SoNotList * list) override;
  SbBool readInstance(SoInput * in, unsigned short flags) override;
  void copyContents(const SoFieldContainer * from, SbBool copyConn) override;

private:
  static void field_sensor_cb(void * data, SoSensor * sensor);
  void commonConstructor(void);
  void processAddChildren(void);
  void processRemoveChildren(void);
  SoFieldData * makeWriteData(void);
  
  SoVRMLParentP * pimpl;
}; // class SoVRMLParent

#endif // ! COIN_SOVRMLPARENT_H
