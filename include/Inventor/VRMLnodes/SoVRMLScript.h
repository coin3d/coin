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

#ifndef COIN_SOVRMLSCRIPT_H
#define COIN_SOVRMLSCRIPT_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoMFString.h>

class SoVRMLScript;
class SoVRMLScriptP;
class SoSensor;

typedef void SoVRMLScriptEvaluateCB(void * closure, SoVRMLScript * node);

class COIN_DLL_API SoVRMLScript : public SoNode
{
  typedef SoNode inherited;

public:
  static void initClass(void);
  SoVRMLScript(void);

  static SoType getClassTypeId(void);
  SoType getTypeId(void) const override;

  SoMFString url;
  SoSFBool directOutput;
  SoSFBool mustEvaluate;

  void doAction(SoAction * action) override;
  void callback(SoCallbackAction * action) override;
  void GLRender(SoGLRenderAction * action) override;
  void getBoundingBox(SoGetBoundingBoxAction * action) override;
  void pick(SoPickAction * action) override;
  void handleEvent(SoHandleEventAction * action) override;
  void write(SoWriteAction * action) override;

  static void setScriptEvaluateCB(SoVRMLScriptEvaluateCB * cb,
                                  void * closure);

protected:
  virtual ~SoVRMLScript();
  void copyContents(const SoFieldContainer * from, SbBool copyconn) override;
  void notify(SoNotList * list) override;
private:
  static SoType classTypeId;
  static void * createInstance(void);
  SoFieldData * fielddata;
  const SoFieldData * getFieldData(void) const override;

private:
  SbBool readInstance(SoInput * in, unsigned short flags) override;

  static void eval_cb(void * data, SoSensor *);
  void initFieldData(void);
  SoVRMLScriptP * pimpl;
  friend class SoVRMLScriptP;
}; // class SoVRMLScript

#endif // ! COIN_SOVRMLSCRIPT_H
