/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
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

#ifndef COIN_SOTEXTUREIMAGEELEMENT_H
#define COIN_SOTEXTUREIMAGEELEMENT_H

#include <Inventor/elements/SoReplacedElement.h>
#include <Inventor/SbVec2s.h>
#include <Inventor/SbColor.h>

class SoState;


class COIN_DLL_EXPORT SoTextureImageElement : public SoReplacedElement {
  typedef SoReplacedElement inherited;

  SO_ELEMENT_HEADER(SoTextureImageElement);
public:
  static void initClass(void);
protected:
  virtual ~SoTextureImageElement();

public:
  enum Model {
    MODULATE,
    DECAL,
    BLEND
  };

  enum Wrap {
    REPEAT,
    CLAMP
  };

  virtual void init(SoState * state);

  static void setDefault(SoState * const state, SoNode * const node);

  static void set(SoState * const state, SoNode * const node,
                  const SbVec2s & size, const int numComponents,
                  const unsigned char * bytes,
                  const int wrapS, const int wrapT,
                  const int model, const SbColor & blendColor);

  static void set(SoState * const state, SoNode * const node,
                  const SbVec2s & size, const int numComponents,
                  const unsigned char * bytes,
                  const Wrap wrapS, const Wrap wrapT,
                  const Model model, const SbColor & blendColor);

  static const unsigned char *get(SoState * const state,
                                  SbVec2s & size,
                                  int & numComponents,
                                  Wrap & wrapS,
                                  Wrap & wrapT,
                                  Model & model,
                                  SbColor & blendColor);

  static const unsigned char *get(SoState * const state,
                                  SbVec2s & size,
                                  int & numComponents,
                                  int & wrapS,
                                  int & wrapT,
                                  int & model,
                                  SbColor & blendColor);

  static const SbColor &getBlendColor(SoState * const state);
  static const unsigned char *getImage(SoState * const state,
                                       SbVec2s &size,
                                       int &numComponents);
  static Model getModel(SoState * const state);
  static Wrap getWrapS(SoState * const state);
  static Wrap getWrapT(SoState * const state);

  static SbBool containsTransparency(SoState * const state);

  static const unsigned char *getDefault(SbVec2s & size, int & numComponents);

protected:
  virtual void setElt(const SbVec2s & size, const int numComponents,
                      const unsigned char * bytes,
                      const Wrap wrapS, const Wrap wrapT,
                      const Model model, const SbColor & blendColor);

  virtual SbBool hasTransparency(void) const;

  SbVec2s size;
  int numComponents;
  const unsigned char *bytes;
  Wrap wrapS, wrapT;
  Model model;
  SbColor blendColor;

private:
  void setDefaultValues();
};

#endif // !COIN_SOTEXTUREIMAGEELEMENT_H
