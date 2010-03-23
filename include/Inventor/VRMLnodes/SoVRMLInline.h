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

protected:
  virtual ~SoVRMLInline();

private:
  virtual void addBoundingBoxChild(SbVec3f center, SbVec3f size);
  virtual SbBool readInstance(SoInput * in, unsigned short flags);
  virtual void copyContents(const SoFieldContainer * from, SbBool copyconn);
  virtual SbBool readLocalFile(SoInput * in);

  static void urlFieldModified(void * userdata, SoSensor * sensor);

  SoVRMLInlineP * pimpl;
};

#endif // ! COIN_SOVRMLINLINE_H
