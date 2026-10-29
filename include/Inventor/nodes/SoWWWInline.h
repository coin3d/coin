#ifndef COIN_SOWWWINLINE_H
#define COIN_SOWWWINLINE_H

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

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoSFNode.h>

#ifndef COIN_INTERNAL
#include <Inventor/actions/SoCallbackAction.h>
#endif // !COIN_INTERNAL

class SbColor;
class SoGroup;
class SoWWWInlineP;

// *************************************************************************

class SoWWWInline;
typedef void SoWWWInlineFetchURLCB(const SbString & url, void * userData,
                                   SoWWWInline * node);

// *************************************************************************

class COIN_DLL_API SoWWWInline : public SoNode {
  typedef SoNode inherited;

  SO_NODE_HEADER(SoWWWInline);

public:
  static void initClass(void);
  SoWWWInline(void);

  SoSFString name;
  SoSFVec3f bboxCenter;
  SoSFVec3f bboxSize;
  SoSFNode alternateRep;

  enum BboxVisibility {
    NEVER,
    UNTIL_LOADED,
    ALWAYS
  };

  void setFullURLName(const SbString & url);
  const SbString & getFullURLName(void);

  SoGroup * copyChildren(void) const;

  void requestURLData(void);
  SbBool isURLDataRequested(void) const;
  SbBool isURLDataHere(void) const;
  void cancelURLDataRequest(void);

  void setChildData(SoNode * urldata);
  SoNode * getChildData(void) const;

  SoChildList * getChildren(void) const override;

  static void setFetchURLCallBack(SoWWWInlineFetchURLCB * f, void * userdata);

  static void setBoundingBoxVisibility(BboxVisibility b);
  static BboxVisibility getBoundingBoxVisibility(void);

  static void setBoundingBoxColor(SbColor & c);
  static const SbColor & getBoundingBoxColor(void);

  static void setReadAsSoFile(SbBool onoff);
  static SbBool getReadAsSoFile(void);

  void doAction(SoAction * action) override;
  virtual void doActionOnKidsOrBox(SoAction * action);
  void callback(SoCallbackAction * action) override;
  void GLRender(SoGLRenderAction * action) override;
  void getBoundingBox(SoGetBoundingBoxAction * action) override;
  void getMatrix(SoGetMatrixAction * action) override;
  void handleEvent(SoHandleEventAction * action) override;
  void search(SoSearchAction * action) override;
  void pick(SoPickAction * action) override;
  void getPrimitiveCount(SoGetPrimitiveCountAction * action) override;
  void audioRender(SoAudioRenderAction * action) override;

protected:
  virtual ~SoWWWInline();

  virtual void addBoundingBoxChild(SbVec3f center, SbVec3f size);
  SbBool readInstance(SoInput * in, unsigned short flags) override;
  void copyContents(const SoFieldContainer * fromfC,
                    SbBool copyconnections) override;

private:
  friend class SoWWWInlineP;
  static SoWWWInlineFetchURLCB * fetchurlcb;
  static void * fetchurlcbdata;
  static SbBool readassofile;
  static SbColor * bboxcolor;
  static BboxVisibility bboxvisibility;

  static void cleanup(void);

  SoWWWInlineP * pimpl;
};

#endif // !COIN_SOWWWINLINE_H
