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

#ifndef COIN_SOVRMLINLINE_H
#define COIN_SOVRMLINLINE_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoMFString.h>

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

  virtual void doAction(SoAction * action);
  virtual void callback(SoCallbackAction * action);
  virtual void GLRender(SoGLRenderAction * action);
  virtual void getBoundingBox(SoGetBoundingBoxAction * action);
  virtual void getMatrix(SoGetMatrixAction * action);
  virtual void handleEvent(SoHandleEventAction * action);
  virtual void search(SoSearchAction * action);
  virtual void pick(SoPickAction * action);
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);
  virtual SoChildList * getChildren(void) const;

private:
  virtual ~SoVRMLInline();

  virtual void addBoundingBoxChild(SbVec3f center, SbVec3f size);
  virtual SbBool readInstance(SoInput * in, unsigned short flags);
  virtual void copyContents(const SoFieldContainer * from, SbBool copyconn);
  virtual SbBool readLocalFile(SoInput * in);

  static void urlFieldModified(void * userdata, SoSensor * sensor);

  SoVRMLInlineP * pimpl;
};

#endif // ! COIN_SOVRMLINLINE_H


