#ifndef COIN_SOTEXTUREIMAGEELEMENT_H
#define COIN_SOTEXTUREIMAGEELEMENT_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

#include <Inventor/elements/SoReplacedElement.h>
#include <Inventor/SbVec2s.h>
#include <Inventor/SbVec3s.h>
#include <Inventor/SbColor.h>

class SoState;

class COIN_DLL_API SoTextureImageElement : public SoReplacedElement {
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
                  const SbVec3s & size, const int numComponents,
                  const unsigned char * bytes,
                  const int wrapS, const int wrapT, const int wrapR,
                  const int model, const SbColor & blendColor);

  static void set(SoState * const state, SoNode * const node,
                  const SbVec2s & size, const int numComponents,
                  const unsigned char * bytes,
                  const Wrap wrapS, const Wrap wrapT,
                  const Model model, const SbColor & blendColor);
  static void set(SoState * const state, SoNode * const node,
                  const SbVec3s & size, const int numComponents,
                  const unsigned char * bytes,
                  const Wrap wrapS, const Wrap wrapT, const Wrap wrapR,
                  const Model model, const SbColor & blendColor);

  static const unsigned char *get(SoState * const state,
                                  SbVec2s & size,
                                  int & numComponents,
                                  Wrap & wrapS,
                                  Wrap & wrapT,
                                  Model & model,
                                  SbColor & blendColor);
  static const unsigned char *get(SoState * const state,
                                  SbVec3s & size,
                                  int & numComponents,
                                  Wrap & wrapS,
                                  Wrap & wrapT,
                                  Wrap & wrapR,
                                  Model & model,
                                  SbColor & blendColor);

  static const unsigned char *get(SoState * const state,
                                  SbVec2s & size,
                                  int & numComponents,
                                  int & wrapS,
                                  int & wrapT,
                                  int & model,
                                  SbColor & blendColor);
  static const unsigned char *get(SoState * const state,
                                  SbVec3s & size,
                                  int & numComponents,
                                  int & wrapS,
                                  int & wrapT,
                                  int & wrapR,
                                  int & model,
                                  SbColor & blendColor);

  static const SbColor &getBlendColor(SoState * const state);
  static const unsigned char *getImage(SoState * const state,
                                       SbVec2s &size,
                                       int &numComponents);
  static const unsigned char *getImage(SoState * const state,
                                       SbVec3s &size,
                                       int &numComponents);
  static Model getModel(SoState * const state);
  static Wrap getWrapS(SoState * const state);
  static Wrap getWrapT(SoState * const state);
  static Wrap getWrapR(SoState * const state);

  static SbBool containsTransparency(SoState * const state);

  static const unsigned char *getDefault(SbVec2s & size, int & numComponents);
  static const unsigned char *getDefault(SbVec3s & size, int & numComponents);

protected:
  virtual void setElt(const SbVec2s & size, const int numComponents,
                      const unsigned char * bytes,
                      const Wrap wrapS, const Wrap wrapT,
                      const Model model, const SbColor & blendColor);
  virtual void setElt(const SbVec3s & size, const int numComponents,
                      const unsigned char * bytes,
                      const Wrap wrapS, const Wrap wrapT, const Wrap wrapR,
                      const Model model, const SbColor & blendColor);

  virtual SbBool hasTransparency(void) const;

  SbVec3s size;
  int numComponents;
  const unsigned char *bytes;
  Wrap wrapS, wrapT, wrapR;
  Model model;
  SbColor blendColor;

private:
  void setDefaultValues(void);

};

#endif // !COIN_SOTEXTUREIMAGEELEMENT_H
