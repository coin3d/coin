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
 *  http://www.sim.no/ sales@sim.no Voice: +47 73540378 Fax: +47 73943861
 *
\**************************************************************************/

#ifndef _SO_GL_AMBIENT_COLOR_ELEMENT_H_
#define _SO_GL_AMBIENT_COLOR_ELEMENT_H_

#include <Inventor/confdep.h>
#if defined(COIN_EXCLUDE_SOGLAMBIENTCOLORELEMENT)
#error "Configuration settings disrespected -- do not include this file!"
#endif // COIN_EXCLUDE_SOGLAMBIENTCOLORELEMENT

#include <Inventor/elements/SoAmbientColorElement.h>
#include <Inventor/SbColor.h>

class SoGLAmbientColorElement : public SoAmbientColorElement {
  typedef SoAmbientColorElement inherited;

//$ BEGIN TEMPLATE ElementHeader( SoGLAmbientColorElement )
public:
  static SoType classTypeId;
  static SoType getClassTypeId(void);
  static void * createInstance(void);
public:
  static int getClassStackIndex(void);
  virtual ~SoGLAmbientColorElement(void);

  static void initClass(void);
  static void cleanClass(void);

protected:
  SoGLAmbientColorElement(void);
  static int classStackIndex;
//$ END TEMPLATE ElementHeader

public:
  virtual void init(SoState *state);
  virtual void push(SoState *state);
  virtual void pop(SoState *state, const SoElement *prevTopElement);

  void send(const int index);

protected:
  virtual void setElt(const int32_t numColors, const SbColor * const colors);

private:
  void updategl();
  SbColor current;

}; // class SoGLAmbientColorElement

#endif // ! _SO_GL_AMBIENT_COLOR_ELEMENT_H_
