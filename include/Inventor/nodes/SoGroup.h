/**************************************************************************\
 * 
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

#ifndef __SOGROUP_H__
#define __SOGROUP_H__

#include <Inventor/nodes/SoNode.h>

#if defined(COIN_EXCLUDE_SOGROUP)
#error Configuration settings disrespected -- do not include this file!
#endif // COIN_EXCLUDE_SOGROUP

// *************************************************************************

class SoGroup : public SoNode {
  typedef SoNode inherited;

//$ BEGIN TEMPLATE NodeHeader(SoGroup)
private:
  static SoType classTypeId;

public:
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;
  static void * createInstance(void);
public:
  static void initClass(void);
  static void cleanClass(void);

  SoGroup(void);
protected:
  virtual ~SoGroup();
//$ END TEMPLATE NodeHeader

public:
  SoGroup(int nchildren);

  void addChild(SoNode * const node);
  void insertChild(SoNode * const child, const int newChildIndex);
  SoNode * getChild(const int index) const;
  int findChild(const SoNode * const node) const;
  int getNumChildren(void) const;
  void removeChild(const int childIndex); 
  void removeChild(SoNode * const child);
  void removeAllChildren(void);
  void replaceChild(const int index, SoNode * const newChild);
  void replaceChild(SoNode * const oldChild, SoNode * const newChild);

#if !defined(COIN_EXCLUDE_SOACTION)
  virtual void doAction(SoAction * action);
#endif // !COIN_EXCLUDE_SOACTION
#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  virtual void GLRender(SoGLRenderAction * action);
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
  virtual void callback(SoCallbackAction * action);
#endif // COIN_EXCLUDE_SOCALLBACKACTION
#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
  virtual void getBoundingBox(SoGetBoundingBoxAction * action);
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION
#if !defined(COIN_EXCLUDE_SOGETMATRIXACTION)
  virtual void getMatrix(SoGetMatrixAction * action);
#endif // !COIN_EXCLUDE_SOGETMATRIXACTION
#if !defined(COIN_EXCLUDE_SOHANDLEEVENTACTION)
  virtual void handleEvent(SoHandleEventAction * action);
#endif // !COIN_EXCLUDE_SOHANDLEEVENTACTION
#if !defined(COIN_EXCLUDE_SOPICKACTION)
  virtual void pick(SoPickAction * action);
#endif // !COIN_EXCLUDE_SOPICKACTION
#if !defined(COIN_EXCLUDE_SOSEARCHACTION)
  virtual void search(SoSearchAction * action);
#endif // !COIN_EXCLUDE_SOSEARCHACTION
#if !defined(COIN_EXCLUDE_SOWRITEACTION)
  virtual void write(SoWriteAction * action);
#endif // !COIN_EXCLUDE_SOWRITEACTION
#if !defined(COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION)
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);
#endif // !COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION
  SoChildList * getChildren(void) const;

  // FIXME: this method is supposed to be protected, but its now used
  // in SoDB. 19990611 mortene.
  virtual SbBool readChildren(SoInput * in);

protected:
  virtual SbBool readInstance(SoInput * in, unsigned short flags);

  SoChildList * children;

  // -- clean out this
  virtual SoNode * copy(void);
};

#endif // !__SOGROUP_H__
