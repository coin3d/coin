#ifndef COIN_SOWWWINLINE_H
#define COIN_SOWWWINLINE_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoSFNode.h>

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

  virtual SoChildList * getChildren(void) const;

  static void setFetchURLCallBack(SoWWWInlineFetchURLCB * f, void * userdata);

  static void setBoundingBoxVisibility(BboxVisibility b);
  static BboxVisibility getBoundingBoxVisibility(void);

  static void setBoundingBoxColor(SbColor & c);
  static const SbColor & getBoundingBoxColor(void);

  static void setReadAsSoFile(SbBool onoff);
  static SbBool getReadAsSoFile(void);

  virtual void doAction(SoAction * action);
  virtual void doActionOnKidsOrBox(SoAction * action);
  virtual void callback(SoCallbackAction * action);
  virtual void GLRender(SoGLRenderAction * action);
  virtual void getBoundingBox(SoGetBoundingBoxAction * action);
  virtual void getMatrix(SoGetMatrixAction * action);
  virtual void handleEvent(SoHandleEventAction * action);
  virtual void search(SoSearchAction * action);
  virtual void pick(SoPickAction * action);
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);
  virtual void audioRender(SoAudioRenderAction * action);

protected:
  virtual ~SoWWWInline();

  virtual void addBoundingBoxChild(SbVec3f center, SbVec3f size);
  virtual SbBool readInstance(SoInput * in, unsigned short flags);
  virtual void copyContents(const SoFieldContainer * fromfC,
                            SbBool copyconnections);

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
