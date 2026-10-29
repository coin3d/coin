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

#ifndef COIN_SOVRMLINLINE_H
#define COIN_SOVRMLINLINE_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoMFString.h>

#ifndef COIN_INTERNAL
#include <Inventor/actions/SoCallbackAction.h>
#endif // !COIN_INTERNAL

class SbColor;
class SoVRMLInline;
class SoVRMLInlineP;
class SoGroup;
class SoSensor;

typedef void SoVRMLInlineFetchURLCB(const SbString &, void *, SoVRMLInline *);

class COIN_DLL_API SoVRMLInline : public SoNode
{
  typedef SoNode inherited;
  SO_NODE_HEADER(SoVRMLInline);

public:
  static void initClass(void);
  SoVRMLInline(void);

  enum BboxVisibility {
    NEVER,
    UNTIL_LOADED,
    ALWAYS
  };

  SoSFVec3f bboxCenter;
  SoSFVec3f bboxSize;
  SoMFString url;

  void setFullURLName(const SbString & url);
  const SbString & getFullURLName(void);

  SoGroup * copyChildren(void) const;
  void requestURLData(void);
  SbBool isURLDataRequested(void) const;
  SbBool isURLDataHere(void) const;

  void cancelURLDataRequest(void);
  void setChildData(SoNode * urlData);
  SoNode * getChildData(void) const;

  static void setFetchURLCallBack(SoVRMLInlineFetchURLCB * f, void * closure);
  static void setBoundingBoxVisibility(BboxVisibility b);
  static BboxVisibility getBoundingBoxVisibility(void);
  static void setBoundingBoxColor(SbColor & color);
  static SbColor & getBoundingBoxColor(void);
  static void setReadAsSoFile(SbBool enable);
  static SbBool getReadAsSoFile(void);

  void doAction(SoAction * action) override;
  void callback(SoCallbackAction * action) override;
  void GLRender(SoGLRenderAction * action) override;
  void getBoundingBox(SoGetBoundingBoxAction * action) override;
  void getMatrix(SoGetMatrixAction * action) override;
  void handleEvent(SoHandleEventAction * action) override;
  void search(SoSearchAction * action) override;
  void pick(SoPickAction * action) override;
  void getPrimitiveCount(SoGetPrimitiveCountAction * action) override;
  SoChildList * getChildren(void) const override;

protected:
  virtual ~SoVRMLInline();

private:
  virtual void addBoundingBoxChild(SbVec3f center, SbVec3f size);
  SbBool readInstance(SoInput * in, unsigned short flags) override;
  void copyContents(const SoFieldContainer * from, SbBool copyconn) override;
  virtual SbBool readLocalFile(SoInput * in);

  static void urlFieldModified(void * userdata, SoSensor * sensor);

  SoVRMLInlineP * pimpl;
};

#endif // ! COIN_SOVRMLINLINE_H
