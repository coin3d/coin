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

#ifndef __SOFILE_H__
#define __SOFILE_H__

#include <Inventor/nodes/SoNode.h>
#include <Inventor/fields/SoSFString.h>

#if defined(COIN_EXCLUDE_SOFILE)
#error Configuration settings disrespected -- do not include this file!
#endif // COIN_EXCLUDE_SOFILE

class SoGroup;

// *************************************************************************

class SoFile : public SoNode {
  typedef SoNode inherited;

//$ BEGIN TEMPLATE NodeHeader( SoFile )
private:
  static SoType classTypeId;

public:
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;
  static void * createInstance(void);
public:
  static void initClass(void);
  static void cleanClass(void);

  SoFile(void);
protected:
  virtual ~SoFile();
//$ END TEMPLATE NodeHeader

public:
  SoSFString name;

#if !defined(COIN_EXCLUDE_SOACTION)
  virtual void doAction(SoAction * action);
#endif // !COIN_EXCLUDE_SOACTION
#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
  virtual void callback(SoCallbackAction * action);
#endif // !COIN_EXCLUDE_SOCALLBACKACTION
#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  virtual void GLRender(SoGLRenderAction * action);
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
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
#if !defined(COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION)
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);
#endif // !COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION

  SoGroup * copyChildren(void) const;
  virtual SoChildList * getChildren(void) const;
  virtual void copyContents(const SoFieldContainer * fromFC,
			    SbBool copyConnections);

protected:
  virtual SbBool readInstance(SoInput * in, unsigned short flags);

private:
  SbString path;
  class SoChildList * children;
  SbBool readState;
};

#endif // !__SOFILE_H__
