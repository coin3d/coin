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

#ifndef _SO_GL_DIFFUSE_COLOR_ELEMENT_H_
#define _SO_GL_DIFFUSE_COLOR_ELEMENT_H_

#include <Inventor/confdep.h>
#if defined(COIN_EXCLUDE_SOGLDIFFUSECOLORELEMENT)
#error "Configuration settings disrespected -- do not include this file!"
#endif // COIN_EXCLUDE_SOGLDIFFUSECOLORELEMENT

#include <Inventor/elements/SoDiffuseColorElement.h>
#include <Inventor/SbVec4f.h>

class SoGLDiffuseColorElement : public SoDiffuseColorElement {
  typedef SoDiffuseColorElement inherited;

//$ BEGIN TEMPLATE ElementHeader( SoGLDiffuseColorElement )
public:
  static SoType classTypeId;
  static SoType getClassTypeId(void);
  static void * createInstance(void);
public:
  static int getClassStackIndex(void);
  virtual ~SoGLDiffuseColorElement(void);

  static void initClass(void);
  static void cleanClass(void);

protected:
  SoGLDiffuseColorElement(void);
  static int classStackIndex;
//$ END TEMPLATE ElementHeader

public:
  virtual void init(SoState *state);
  virtual void push(SoState *state);
  virtual void pop(SoState *state, const SoElement *prevTopElement);

  void send(const int index, const float alpha);
  void send(const int index);

protected:

private:
  void updategl();
  SbVec4f current;
  uint32_t currentPacked; // for fast comparing

}; // class SoGLDiffuseColorElement

#endif // ! _SO_GL_DIFFUSE_COLOR_ELEMENT_H_
