/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
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

#ifndef COIN_SOWINDOWELEMENT_H
#define COIN_SOWINDOWELEMENT_H

#include <Inventor/elements/SoSubElement.h>

// Note:
// We want to avoid platform specific stuff in this header file, 
// so we changed all the X-specific types into generic void pointers.

class SoGLRenderAction;

class COIN_DLL_API SoWindowElement : public SoElement {
  typedef SoElement inherited;

  SO_ELEMENT_HEADER(SoWindowElement);
public:
  static void initClass(void);
protected:
  virtual ~SoWindowElement();

public:
  virtual void init(SoState * state);

  virtual void push(SoState * state);
  virtual void pop(SoState * state, const SoElement * prevTopElement);

  virtual SbBool matches(const SoElement * element) const;
  virtual SoElement * copyMatchInfo(void) const;

  static  void set(SoState * state, void * window,
                   void * context, void * display,
                   SoGLRenderAction * action);
  static  void get(SoState * state, void *& window,
                   void *& context, void * & display,
                   SoGLRenderAction * & action);
  
protected:
  void * window;   // was Window
  void * context;  // was GLXContext
  void * display;  // was Display *
  SoGLRenderAction * glRenderAction;
};

#endif // !COIN_SOWINDOWELEMENT_H
