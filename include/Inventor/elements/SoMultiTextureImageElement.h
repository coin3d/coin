#ifndef COIN_SOMULTITEXTUREIMAGEELEMENT_H
#define COIN_SOMULTITEXTUREIMAGEELEMENT_H

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

#include <Inventor/elements/SoReplacedElement.h>
#include <Inventor/SbVec2s.h>
#include <Inventor/SbVec3s.h>
#include <Inventor/SbColor.h>

class SoState;
class SoMultiTextureImageElementP;

class COIN_DLL_API SoMultiTextureImageElement : public SoElement {
  typedef SoElement inherited;

  SO_ELEMENT_HEADER(SoMultiTextureImageElement);
public:
  static void initClass(void);
protected:
  virtual ~SoMultiTextureImageElement();

public:

  enum Model {
    // These should match GL_BLEND, GL_MODULATE and GL_DECAL for SGI
    // Inventor compatibility (these are also used by SoTexture2 and
    // SoTexture3).
    BLEND = 0x0be2,
    MODULATE = 0x2100,
    DECAL = 0x2101,
    REPLACE = 0x1E01 // must match GL_REPLACE
  };

  enum Wrap {
    // These should match GL_CLAMP and GL_REPEAT for SGI Inventor
    // compatibility (these are also used by SoTexture2 and
    // SoTexture3).
    CLAMP = 0x2900,
    REPEAT = 0x2901,
    CLAMP_TO_BORDER = 0x812D
  };

  virtual void init(SoState * state);
  static void setDefault(SoState * const state, SoNode * const node, const int unit = 0);
  
  static void set(SoState * const state, SoNode * const node,
                  const int unit,
                  const SbVec2s & size, const int numComponents,
                  const unsigned char * bytes,
                  const Wrap wrapS, 
                  const Wrap wrapT,
                  const Model model, const SbColor & blendColor);
  static void set(SoState * const state, SoNode * const node,
                  const int unit,
                  const SbVec3s & size, const int numComponents,
                  const unsigned char * bytes,
                  const Wrap wrapS, 
                  const Wrap wrapT, 
                  const Wrap wrapR,
                  const Model model, const SbColor & blendColor);

  static const unsigned char *get(SoState * const state,
                                  const int unit,
                                  SbVec2s & size,
                                  int & numComponents,
                                  Wrap & wrapS,
                                  Wrap & wrapT,
                                  Model & model,
                                  SbColor & blendColor);
  static const unsigned char *get(SoState * const state,
                                  const int unit,
                                  SbVec3s & size,
                                  int & numComponents,
                                  Wrap & wrapS,
                                  Wrap & wrapT,
                                  Wrap & wrapR,
                                  Model & model,
                                  SbColor & blendColor);

  static const unsigned char *getImage(SoState * const state,
                                       const int unit,
                                       SbVec2s & size,
                                       int & numComponents);
  static const unsigned char *getImage(SoState * const state,
                                       const int unit,
                                       SbVec3s &size,
                                       int &numComponents);

  static const SbColor & getBlendColor(SoState * const state, const int unit = 0);
  static Model getModel(SoState * const state, const int unit = 0);
  static Wrap getWrapS(SoState * const state, const int unit = 0);
  static Wrap getWrapT(SoState * const state, const int unit = 0);
  static Wrap getWrapR(SoState * const state, const int unit = 0);

  static SbBool containsTransparency(SoState * const state);

  static const unsigned char * getDefault(SbVec2s & size, int & numComponents);
  static const unsigned char * getDefault(SbVec3s & size, int & numComponents);

  virtual void push(SoState * state);
  virtual SbBool matches(const SoElement * elem) const;
  SoElement * copyMatchInfo(void) const;

  virtual void setElt(const int unit,
                      const uint32_t nodeid,
                      const SbVec2s & size, const int numComponents,
                      const unsigned char * bytes,
                      const Wrap wrapS, 
                      const Wrap wrapT,
                      const Model model, 
                      const SbColor & blendColor);
  virtual void setElt(const int unit,
                      const uint32_t nodeid,
                      const SbVec3s & size, const int numComponents,
                      const unsigned char * bytes,
                      const Wrap wrapS, 
                      const Wrap wrapT, 
                      const Wrap wrapR,
                      const Model model, 
                      const SbColor & blendColor);
  
  virtual SbBool hasTransparency(const int unit = 0) const;

  class UnitData {
  public:
    UnitData();
    UnitData(const UnitData & org);
    uint32_t nodeid;
    SbVec3s size;
    int numComponents;
    const unsigned char * bytes;
    Wrap wrapS, wrapT, wrapR;
    Model model;
    SbColor blendColor;
  };

protected:
  const UnitData & getUnitData(const int unit) const;
  int getNumUnits() const;
  
private:
  SoMultiTextureImageElementP * pimpl;

 public: // Coin-3 support
  static void set(SoState * const state, SoNode * const node,
                  const SbVec2s & size, const int numComponents,
                  const unsigned char * bytes,
                  const int wrapS, const int wrapT,
                  const int model, const SbColor & blendColor) {
    set(state, node,0, size, numComponents, bytes,
        (Wrap) wrapS, (Wrap) wrapT, (Model) model, blendColor);
  }
  static void set(SoState * const state, SoNode * const node,
                  const SbVec2s & size, const int numComponents,
                  const unsigned char * bytes,
                  const Wrap wrapS, const Wrap wrapT,
                  const Model model, const SbColor & blendColor) {
    set(state, node, 0, size, numComponents, bytes,
        wrapS, wrapT, model, blendColor);
  }

  static void set(SoState * const state, SoNode * const node,
                  const SbVec3s & size, const int numComponents,
                  const unsigned char * bytes,
                  const int wrapS, const int wrapT, const int wrapR,
                  const int model, const SbColor & blendColor) {
    set(state, node, 0, size, numComponents, bytes,
        (Wrap) wrapS, (Wrap) wrapT, (Wrap) wrapR, (Model) model, blendColor);
  }

  static void set(SoState * const state, SoNode * const node,
                  const SbVec3s & size, const int numComponents,
                  const unsigned char * bytes,
                  const Wrap wrapS, const Wrap wrapT, const Wrap wrapR,
                  const Model model, const SbColor & blendColor) {
    set(state, node, 0, size, numComponents, bytes,
        wrapS, wrapT, wrapR, model, blendColor);
  }
  
  static const unsigned char *get(SoState * const state,
                                  SbVec2s & size,
                                  int & numComponents,
                                  Wrap & wrapS,
                                  Wrap & wrapT,
                                  Model & model,
                                  SbColor & blendColor) {
    return get(state, 0, size, numComponents, wrapS, wrapT, model, blendColor);
  }
  static const unsigned char *get(SoState * const state,
                                  SbVec3s & size,
                                  int & numComponents,
                                  Wrap & wrapS,
                                  Wrap & wrapT,
                                  Wrap & wrapR,
                                  Model & model,
                                  SbColor & blendColor) {
    return get(state, 0, size, numComponents, wrapS, wrapT, wrapR, model, blendColor);
  }
  
  static const unsigned char *get(SoState * const state,
                                  SbVec2s & size,
                                  int & numComponents,
                                  int & wrapS,
                                  int & wrapT,
                                  int & model,
                                  SbColor & blendColor) {
    Wrap s, t;
    Model m;
    const unsigned char * bytes = get(state, 0, size, numComponents, s, t, m, blendColor);
    wrapS = (int) s;
    wrapT = (int) t;
    model = (int) m;
    return bytes;
  }
  static const unsigned char *get(SoState * const state,
                                  SbVec3s & size,
                                  int & numComponents,
                                  int & wrapS,
                                  int & wrapT,
                                  int & wrapR,
                                  int & model,
                                  SbColor & blendColor) {
    Wrap s, t, r;
    Model m;
    const unsigned char * bytes = get(state, 0, size, numComponents, s, t, r, m, blendColor);
    wrapS = (int) s;
    wrapT = (int) t;
    wrapR = (int) r;
    model = (int) m;
    return bytes;
  }
  static const unsigned char *getImage(SoState * const state,
                                       SbVec2s &size,
                                       int &numComponents) {
    return getImage(state, 0, size, numComponents);
  }
  static const unsigned char *getImage(SoState * const state,
                                       SbVec3s &size,
                                       int &numComponents) {
    return getImage(state, 0, size, numComponents);
  }
};

#endif // !COIN_SOMULTITEXTUREIMAGEELEMENT_H
