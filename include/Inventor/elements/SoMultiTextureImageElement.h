#ifndef COIN_SOMULTITEXTUREIMAGEELEMENT_H
#define COIN_SOMULTITEXTUREIMAGEELEMENT_H

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

#include <Inventor/elements/SoReplacedElement.h>
#include <Inventor/elements/SoTextureImageElement.h>
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

  virtual void init(SoState * state);
  static void setDefault(SoState * const state, SoNode * const node, const int unit);
  
  static void set(SoState * const state, SoNode * const node,
                  const int unit,
                  const SbVec2s & size, const int numComponents,
                  const unsigned char * bytes,
                  const SoTextureImageElement::Wrap wrapS, 
                  const SoTextureImageElement::Wrap wrapT,
                  const SoTextureImageElement::Model model, const SbColor & blendColor);
  static void set(SoState * const state, SoNode * const node,
                  const int unit,
                  const SbVec3s & size, const int numComponents,
                  const unsigned char * bytes,
                  const SoTextureImageElement::Wrap wrapS, 
                  const SoTextureImageElement::Wrap wrapT, 
                  const SoTextureImageElement::Wrap wrapR,
                  const SoTextureImageElement::Model model, const SbColor & blendColor);

  static const unsigned char *get(SoState * const state,
                                  const int unit,
                                  SbVec2s & size,
                                  int & numComponents,
                                  SoTextureImageElement::Wrap & wrapS,
                                  SoTextureImageElement::Wrap & wrapT,
                                  SoTextureImageElement::Model & model,
                                  SbColor & blendColor);
  static const unsigned char *get(SoState * const state,
                                  const int unit,
                                  SbVec3s & size,
                                  int & numComponents,
                                  SoTextureImageElement::Wrap & wrapS,
                                  SoTextureImageElement::Wrap & wrapT,
                                  SoTextureImageElement::Wrap & wrapR,
                                  SoTextureImageElement::Model & model,
                                  SbColor & blendColor);

  static const unsigned char *getImage(SoState * const state,
                                       const int unit,
                                       SbVec2s & size,
                                       int & numComponents);
  static const unsigned char *getImage(SoState * const state,
                                       const int unit,
                                       SbVec3s &size,
                                       int &numComponents);

  static const SbColor & getBlendColor(SoState * const state, const int unit);
  static SoTextureImageElement::Model getModel(SoState * const state, const int unit);
  static SoTextureImageElement::Wrap getWrapS(SoState * const state, const int unit);
  static SoTextureImageElement::Wrap getWrapT(SoState * const state, const int unit);
  static SoTextureImageElement::Wrap getWrapR(SoState * const state, const int unit);

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
                      const SoTextureImageElement::Wrap wrapS, 
                      const SoTextureImageElement::Wrap wrapT,
                      const SoTextureImageElement::Model model, 
                      const SbColor & blendColor);
  virtual void setElt(const int unit,
                      const uint32_t nodeid,
                      const SbVec3s & size, const int numComponents,
                      const unsigned char * bytes,
                      const SoTextureImageElement::Wrap wrapS, 
                      const SoTextureImageElement::Wrap wrapT, 
                      const SoTextureImageElement::Wrap wrapR,
                      const SoTextureImageElement::Model model, 
                      const SbColor & blendColor);
  
  virtual SbBool hasTransparency(const int unit) const;

  class UnitData {
  public:
    uint32_t nodeid;
    SbVec3s size;
    int numComponents;
    const unsigned char * bytes;
    SoTextureImageElement::Wrap wrapS, wrapT, wrapR;
    SoTextureImageElement::Model model;
    SbColor blendColor;
  };

protected:
  const UnitData & getUnitData(const int unit) const;

private:
  void setDefaultValues(const int unit);
  SoMultiTextureImageElementP * pimpl;
};

#endif // !COIN_SOMULTITEXTUREIMAGEELEMENT_H
