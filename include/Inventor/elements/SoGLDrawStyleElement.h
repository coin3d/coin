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

#ifndef __SOGLDRAWSTYLEELEMENT_H__
#define __SOGLDRAWSTYLEELEMENT_H__

#include <Inventor/confdep.h>
#if defined(COIN_EXCLUDE_SOGLDRAWSTYLEELEMENT)
#error "Configuration settings disrespected -- do not include this file!"
#endif // COIN_EXCLUDE_SOGLDRAWSTYLEELEMENT

#include <Inventor/elements/SoDrawStyleElement.h>

class SoGLDrawStyleElement : public SoDrawStyleElement {
  typedef SoDrawStyleElement inherited;

//$ BEGIN TEMPLATE ElementHeader(SoGLDrawStyleElement)
public:
  static SoType classTypeId;
  static SoType getClassTypeId(void);
  static void * createInstance(void);
public:
  static int getClassStackIndex(void);
  virtual ~SoGLDrawStyleElement(void);

  static void initClass(void);

protected:
  SoGLDrawStyleElement(void);
  static int classStackIndex;
//$ END TEMPLATE ElementHeader

public:
  virtual void init(SoState * state);

  virtual void push(SoState * state);
  virtual void pop(SoState * state,
		   const SoElement * prevTopElement);
  
protected:
  virtual void setElt(int32_t style);
  
private:
  void updategl();
  
};

#endif // !__SOGLDRAWSTYLEELEMENT_H__
