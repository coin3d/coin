#ifndef COIN_SOGLVBOELEMENT_H
#define COIN_SOGLVBOELEMENT_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2005 by Systems in Motion.  All rights reserved.
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
 *  See <URL:http://www.coin3d.org/> for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

#include <Inventor/elements/SoSubElement.h>

class SoVBO;
class SoGLVBOElementP;

class SoGLVBOElement : public SoElement {
  typedef SoElement inherited;

  SO_ELEMENT_HEADER(SoGLVBOElement);

 public:
  static void initClass(void);
 protected:
  virtual ~SoGLVBOElement();

 public:

  static SbBool shouldCreateVBO(SoState * state, const int numdata);
  static void setVertexVBO(SoState * state, SoVBO * vbo);
  static void setNormalVBO(SoState * state, SoVBO * vbo);
  static void setColorVBO(SoState * state, SoVBO * vbo);
  static void setTexCoordVBO(SoState * state, const int unit, SoVBO * vbo);

  static const SoGLVBOElement * getInstance(SoState * state);

 public:
  virtual void init(SoState *state);
  virtual void push(SoState *state);
  virtual void pop(SoState *state, const SoElement * prevtopelement);
  virtual SbBool matches(const SoElement * elt) const;
  virtual SoElement * copyMatchInfo(void) const;

  SoVBO * getVertexVBO(void) const;
  SoVBO * getNormalVBO(void) const;
  SoVBO * getColorVBO(void) const;
  int getNumTexCoordVBO(void) const;
  SoVBO * getTexCoordVBO(const int idx) const;

 protected:  
  static SoGLVBOElement * getElement(SoState * state);

 private:
  SoGLVBOElementP * pimpl;
};

#endif // COIN_SOGLVBOELEMENT_H
