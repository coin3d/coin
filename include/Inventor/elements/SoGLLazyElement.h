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

//    virtual void init(SoState *state);
//    virtual void push(SoState *state);
//    virtual void pop(SoState *state, const SoElement *prevTopElement);

  static void sendAllMaterial(SoState *state);
  static void sendNoMaterial(SoState *state);
  static void sendOnlyDiffuseColor(SoState *state);
  void sendDiffuseByIndex(int index) const;
  static SbBool isColorIndex(SoState *state);
  static SoGLLazyElement *getInstance(const SoState *state);
  void send(const SoState *state, uint32_t mask) const;

  void sendVPPacked(SoState* state, const unsigned char* pcolor);

  // These are just stubs. SoGLRenderCache is not supported yet.
  void reset(SoState* state,  uint32_t bitmask) const;
  SbBool lazyMatches(uint32_t checkGLFlag, uint32_t checkIVFlag,
                     const SoGLLazyElement* eltInState);
  SoGLLazyElement* copyLazyMatchInfo(SoState* state);

  typedef struct {
    uint32_t        GLDiffuseNodeId;
    uint32_t        GLTranspNodeId;
    float           GLAmbient[4];
    float           GLEmissive[4];
    float           GLSpecular[4];
    float           GLShininess;
    SbBool          GLColorMaterial;
    int32_t         GLLightModel;
    SbBool          GLblending;
    int32_t         GLStippleNum;
  } GLLazyState;

  void getCopyGL(SoGLLazyElement* cacheLazyElement,
                 SoGLLazyElement::GLLazyState& cacheGLState);

  void copyBackGL(SoGLLazyElement* cacheLazyElement,
                  SoGLLazyElement::GLLazyState& cacheGLState);
  void mergeCacheInfo(SoGLRenderCache* childCache, SoGLRenderCache*
                      parentCache, uint32_t doSendFlag, uint32_t checkIVFlag,
                      uint32_t checkGLFlag);
};

#endif // !COIN_SOGLLAZYELEMENT_H
