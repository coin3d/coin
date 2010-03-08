#ifndef COIN_SOGLMULTITEXTUREIMAGEELEMENT_H
#define COIN_SOGLMULTITEXTUREIMAGEELEMENT_H

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

#include <Inventor/elements/SoMultiTextureImageElement.h>
#include <cstdlib>

class SoGLImage;
class SoGLDisplayList;
class SoGLMultiTextureImageElementP;

class COIN_DLL_API SoGLMultiTextureImageElement : public SoMultiTextureImageElement {
  typedef SoMultiTextureImageElement inherited;

  SO_ELEMENT_HEADER(SoGLMultiTextureImageElement);
public:
  static void initClass(void);
protected:
  virtual ~SoGLMultiTextureImageElement();

public:
  virtual void init(SoState * state);
  virtual void push(SoState * state);
  virtual void pop(SoState * state,
                   const SoElement * prevTopElement);

  static void set(SoState * const state, SoNode * const node,
                  const int unit,
                  SoGLImage * image, const Model model,
                  const SbColor & blendColor);

  static void restore(SoState * state, const int unit);

  static SoGLImage * get(SoState * state,
                         const int unit,
                         Model & model,
                         SbColor & blendcolor);

  class GLUnitData {
  public:
  GLUnitData() : glimage(NULL) {}
  GLUnitData(const GLUnitData & org) : glimage(org.glimage) {}
    SoGLImage * glimage;
  };
  
  static SbBool hasTransparency(SoState * state);
  
 protected:
  virtual SbBool hasTransparency(const int unit = 0) const;
  
private:
  void updateGL(const int unit);
  SoGLMultiTextureImageElementP * pimpl;

 public: // Coin-3 support
  
  static void set(SoState * const state, SoNode * const node,
                  SoGLImage * image, const Model model,
                  const SbColor & blendColor) {
    set(state, node, 0, image, model, blendColor);
  }

  static SoGLImage * get(SoState * state, Model & model,
                         SbColor & blendcolor) {
    return get(state, 0, model, blendcolor);
  }
  static int32_t getMaxGLTextureSize(void);
};

#endif // !COIN_SOGLMULTITEXTUREIMAGEELEMENT_H
