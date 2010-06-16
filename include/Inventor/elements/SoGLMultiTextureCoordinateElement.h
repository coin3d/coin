#ifndef COIN_SOGLMULTITEXTURECOORDINATEELEMENT_H
#define COIN_SOGLMULTITEXTURECOORDINATEELEMENT_H

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

#include <Inventor/elements/SoMultiTextureCoordinateElement.h>

class SoGLMultiTextureCoordinateElementP;
typedef void SoTexCoordTexgenCB(void * data);

class COIN_DLL_API SoGLMultiTextureCoordinateElement : public SoMultiTextureCoordinateElement {
  typedef SoMultiTextureCoordinateElement inherited;

  SO_ELEMENT_HEADER(SoGLMultiTextureCoordinateElement);
public:
  static void initClass(void);
protected:
  virtual ~SoGLMultiTextureCoordinateElement();

public:
  virtual void init(SoState * state);
  virtual void push(SoState * state);
  virtual void pop(SoState * state,
                   const SoElement * prevTopElement);

  static  void setTexGen(SoState * const state, SoNode * const node,
                         const int unit,
                         SoTexCoordTexgenCB * const texgenFunc,
                         void * const texgenData = NULL,
                         SoTextureCoordinateFunctionCB * const func = NULL,
                         void * const funcData = NULL);

  virtual CoordType getType(const int unit = 0) const;

  static const SoGLMultiTextureCoordinateElement * getInstance(SoState * const state);

  void send(const int unit, const int index) const;
  void send(const int unit, const int index, const SbVec3f &c, const SbVec3f &n) const;

  class GLUnitData {
  public:
    GLUnitData() : texgenCB(NULL), texgenData(NULL) {}
    GLUnitData(const GLUnitData & org) : texgenCB(org.texgenCB), texgenData(org.texgenData) {}
    SoTexCoordTexgenCB * texgenCB;
    void * texgenData;
  };

  void initRender(const SbBool * enabled, const int maxenabled) const;

  // Coin-3 support
  void send(const int index) const {
    for (int i = 0; i <= this->multimax; i++) {
      if (this->multienabled[i]) {
        this->send(i, index);
      }
    }
  }
  void send(const int index, const SbVec3f &c, const SbVec3f &n) const {
    for (int i = 0; i <= this->multimax; i++) {
      if (this->multienabled[i]) {
        this->send(i, index, c, n);
      }
    }
  }
  void initMulti(SoState * state) const;
  static  void setTexGen(SoState * const state, SoNode * const node,
                         SoTexCoordTexgenCB * const texgenFunc,
                         void * const texgenData = NULL,
                         SoTextureCoordinateFunctionCB * const func = NULL,
                         void * const funcData = NULL) {
    setTexGen(state, node, 0, texgenFunc, texgenData, func, funcData);
  }

protected:
  virtual void setElt(const int unit,
                      SoTexCoordTexgenCB *func,
                      void *data = NULL);

private:
  void doCallback(const int unit) const;
  SoGLMultiTextureCoordinateElementP * pimpl;
  mutable const SbBool * multienabled;
  mutable int multimax;
};

#endif // !COIN_SOGLMULTITEXTURECOORDINATEELEMENT_H
