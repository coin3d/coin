#ifndef COIN_SOGLLAZYELEMENT_H
#define COIN_SOGLLAZYELEMENT_H

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

#include <Inventor/elements/SoLazyElement.h>

class SoGLRenderCache;

class COIN_DLL_API SoGLLazyElement : public SoLazyElement {
  typedef SoLazyElement inherited;

  SO_ELEMENT_HEADER(SoGLLazyElement);
  
public:
  static void initClass();
protected:
  ~SoGLLazyElement();
public:
  
  virtual void init(SoState *state);
  virtual void push(SoState *state);
  virtual void pop(SoState *state, const SoElement * prevtopelement);

  static void sendAllMaterial(SoState * state);
  static void sendNoMaterial(SoState * state);
  static void sendOnlyDiffuseColor(SoState * state);
  static void sendLightModel(SoState * state, const int32_t model);
  static void sendPackedDiffuse(SoState * state, const uint32_t diffuse);

  void sendDiffuseByIndex(const int index) const;
  static SbBool isColorIndex(SoState *state);
  static SoGLLazyElement * getInstance(const SoState *state);
  void send(const SoState *state, uint32_t mask) const;

  void sendVPPacked(SoState* state, const unsigned char* pcolor);

  void reset(SoState* state,  uint32_t bitmask) const;

  typedef struct {
    uint32_t diffuse;
    SbColor ambient;
    SbColor emissive;
    SbColor specular;
    float shininess;
    int32_t lightmodel;
    int32_t blending;
    int32_t stipplenum;
  } GLState;

  GLState glstate;
  SbBool colorindex;
  SoColorPacker * colorpacker;
  const uint32_t * packedpointer;
  uint32_t transpmask;

  virtual void setDiffuseElt(SoNode*,  int32_t numcolors, 
                             const SbColor * colors, SoColorPacker * packer);   
  virtual void setPackedElt(SoNode * node, int32_t numcolors, 
                            const uint32_t * colors, const SbBool packedtransparency);
  virtual void setColorIndexElt(SoNode * node, int32_t numindices, 
                                const int32_t * indices);	
  virtual void setTranspElt(SoNode * node, int32_t numtransp, 
                            const float * transp, SoColorPacker * packer);
  
  virtual void setTranspTypeElt(int32_t type);
  virtual void setAmbientElt(const SbColor* color);
  virtual void setEmissiveElt(const SbColor* color);
  virtual void setSpecularElt(const SbColor* color);
  virtual void setShininessElt(float value);
  virtual void setColorMaterialElt(SbBool value);
  virtual void setBlendingElt(SbBool value);
  virtual void setLightModelElt(SoState *state, int32_t model);
  virtual void setMaterialElt(SoNode * node, uint32_t bitmask, 
                              SoColorPacker * packer, 
                              const SbColor * diffuse, const int numdiffuse,
                              const float * transp, const int numtransp,
                              const SbColor & ambient,
                              const SbColor & emissive,
                              const SbColor & specular,
                              const float shininess);

private:
  void sendPackedDiffuse(const uint32_t diffuse) const;
  void sendLightModel(const int32_t model) const;
  void initGL(void);
  void packColors(SoColorPacker * packer) const;
};

#endif // !COIN_SOGLLAZYELEMENT_H
