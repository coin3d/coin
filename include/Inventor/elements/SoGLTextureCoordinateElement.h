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

#ifndef __SOGLTEXTURECOORDINATEELEMENT_H__
#define __SOGLTEXTURECOORDINATEELEMENT_H__

#include <Inventor/confdep.h>
#if defined(COIN_EXCLUDE_SOGLTEXTURECOORDINATEELEMENT)
#error "Configuration settings disrespected -- do not include this file!"
#endif // COIN_EXCLUDE_SOGLTEXTURECOORDINATEELEMENT

#include <Inventor/elements/SoTextureCoordinateElement.h>

typedef void SoTexCoordTexgenCB(void * data);

class SoGLTextureCoordinateElement : public SoTextureCoordinateElement {
  typedef SoTextureCoordinateElement inherited;

//$ BEGIN TEMPLATE ElementHeader(SoGLTextureCoordinateElement)
public:
  static SoType classTypeId;
  static SoType getClassTypeId(void);
  static void * createInstance(void);
public:
  static int getClassStackIndex(void);
  virtual ~SoGLTextureCoordinateElement(void);

  static void initClass(void);

protected:
  SoGLTextureCoordinateElement(void);
  static int classStackIndex;
//$ END TEMPLATE ElementHeader

public:
  virtual void init(SoState * state);
  virtual void push(SoState * state);
  virtual void pop(SoState * state,
		   const SoElement * prevTopElement);

  static  void setTexGen(SoState * const state, SoNode * const node,
			 SoTexCoordTexgenCB * const texgenFunc,
			 void * const texgenData = NULL,
			 SoTextureCoordinateFunctionCB * const func = NULL,
			 void * const funcData = NULL);
  
  virtual CoordType getType() const;

  static const SoGLTextureCoordinateElement * 
  getInstance(SoState * const state);
  
  void send(const int index) const;
  void send(const int index, const SbVec3f &c, const SbVec3f &n) const;
  
protected:
  virtual void setElt(SoTexCoordTexgenCB *func,
		      void *data = NULL);
  
private:
  SoTexCoordTexgenCB *texgenCB;
  void *texgenData;

  void doCallback() const;
};

#endif // !__SOGLTEXTURECOORDINATEELEMENT_H__
