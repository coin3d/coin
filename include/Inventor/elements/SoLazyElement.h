#ifndef COIN_SOLAZYELEMENT_H
#define COIN_SOLAZYELEMENT_H

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

#include <Inventor/elements/SoElement.h>
#include <Inventor/elements/SoSubElement.h>
#include <Inventor/SbColor.h>

class SoMFFloat;
class SoMFColor;
class SoColorPacker;
class SoLazyElementP;


#define SO_LAZY_SHINY_THRESHOLD 0.005f


class COIN_DLL_API SoLazyElement : public SoElement {
  typedef SoElement inherited;

  SO_ELEMENT_HEADER(SoLazyElement);

public:
  static void initClass();
protected:
  ~SoLazyElement();
public:
  enum cases {
    LIGHT_MODEL_CASE = 0,
    COLOR_MATERIAL_CASE,
    DIFFUSE_CASE,
    AMBIENT_CASE,
    EMISSIVE_CASE,
    SPECULAR_CASE,
    SHININESS_CASE,
    BLENDING_CASE,
    TRANSPARENCY_CASE,
    VERTEXORDERING_CASE,
    TWOSIDE_CASE,
    CULLING_CASE,
    SHADE_MODEL_CASE,
    LAZYCASES_LAST // must be last
  };
  enum masks{
    LIGHT_MODEL_MASK = 1 << LIGHT_MODEL_CASE,
    COLOR_MATERIAL_MASK = 1 << COLOR_MATERIAL_CASE,
    DIFFUSE_MASK = 1 << DIFFUSE_CASE,
    AMBIENT_MASK = 1 << AMBIENT_CASE,
    EMISSIVE_MASK = 1<<EMISSIVE_CASE,
    SPECULAR_MASK = 1 << SPECULAR_CASE,
    SHININESS_MASK = 1 << SHININESS_CASE,
    TRANSPARENCY_MASK = 1 << TRANSPARENCY_CASE,
    BLENDING_MASK = 1 << BLENDING_CASE,
    VERTEXORDERING_MASK = 1 << VERTEXORDERING_CASE,
    TWOSIDE_MASK = 1 << TWOSIDE_CASE,
    CULLING_MASK = 1 << CULLING_CASE,
    SHADE_MODEL_MASK = 1 << SHADE_MODEL_CASE,
    ALL_MASK = (1 << LAZYCASES_LAST)-1
  };
  
  enum internalMasks{
    OTHER_COLOR_MASK = AMBIENT_MASK|EMISSIVE_MASK|SPECULAR_MASK|SHININESS_MASK,   
    ALL_COLOR_MASK = OTHER_COLOR_MASK|DIFFUSE_MASK, 
    NO_COLOR_MASK = ALL_MASK & (~ALL_COLOR_MASK), 
    ALL_BUT_DIFFUSE_MASK = ALL_MASK &(~ DIFFUSE_MASK), 
    DIFFUSE_ONLY_MASK = ALL_MASK &(~ OTHER_COLOR_MASK)
  };
  
  enum LightModel {
    BASE_COLOR,
    PHONG
  };

  enum VertexOrdering {
    CW,
    CCW
  };

  virtual void init(SoState *state);
  virtual void push(SoState *state);
  virtual SbBool matches(const SoElement *) const;
  virtual SoElement *copyMatchInfo(void) const;

  static void setDiffuse(SoState * state, SoNode * node, int32_t numcolors,
                         const SbColor * colors, SoColorPacker * packer);
  static void setTransparency(SoState *state, SoNode *node, int32_t numvalues,
                              const float * transparency, SoColorPacker * packer);
  static void setPacked(SoState * state, SoNode * node,
                        int32_t numcolors, const uint32_t * colors,
                        const SbBool packedtransparency = FALSE);
  static void setColorIndices(SoState *state, SoNode *node,
                              int32_t numindices, const int32_t *indices);
  static void setAmbient(SoState *state, const SbColor * color);
  static void setEmissive(SoState *state, const SbColor * color);
  static void setSpecular(SoState *state, const SbColor * color);
  static void setShininess(SoState *state, float value);
  static void setColorMaterial(SoState *state, SbBool value);
  static void setBlending(SoState *state,  SbBool value);
  static void setLightModel(SoState *state, const int32_t model);
  static void setVertexOrdering(SoState * state, VertexOrdering ordering);
  static void setBackfaceCulling(SoState * state, SbBool onoff);
  static void setTwosideLighting(SoState * state, SbBool onoff);
  static void setShadeModel(SoState * state, SbBool flatshading);
  static const SbColor & getDiffuse(SoState* state, int index);
  static float getTransparency(SoState*, int index);
  static const uint32_t * getPackedColors(SoState*);
  static const int32_t  * getColorIndices(SoState*);
  static int32_t getColorIndex(SoState*, int num);
  static const SbColor & getAmbient(SoState *);
  static const SbColor & getEmissive(SoState *);
  static const SbColor & getSpecular(SoState *);
  static float getShininess(SoState*);
  static SbBool getColorMaterial(SoState*);
  static SbBool getBlending(SoState*);
  static int32_t getLightModel(SoState*);

  int32_t getNumDiffuse(void) const;
  int32_t getNumTransparencies(void) const;
  int32_t getNumColorIndices(void) const;
  SbBool isPacked(void) const;
  SbBool isTransparent(void) const;
  static SoLazyElement * getInstance(SoState *state);
  static float getDefaultAmbientIntensity(void);

  static SbColor getDefaultDiffuse(void);
  static SbColor getDefaultAmbient(void);
  static SbColor getDefaultSpecular(void);
  static SbColor getDefaultEmissive(void);
  static float getDefaultShininess(void);
  static uint32_t getDefaultPacked(void);
  static float getDefaultTransparency(void);
  static int32_t getDefaultLightModel(void);
  static int32_t getDefaultColorIndex(void);

  static void setMaterials(SoState * state, SoNode *node, uint32_t bitmask,
                           SoColorPacker * cPacker,
                           const SbColor * diffuse, 
                           const int numdiffuse,
                           const float * transp,
                           const int numtransp,
                           const SbColor & ambient,
                           const SbColor & emissive,
                           const SbColor & specular,
                           const float shininess);

  static SoLazyElement * getWInstance(SoState *state);

  const uint32_t * getPackedPointer(void) const;
  const SbColor * getDiffusePointer(void) const;
  const int32_t * getColorIndexPointer(void) const;

  const float * getTransparencyPointer(void) const;
  static void setTransparencyType(SoState * state, int32_t type);

protected:
  
  struct CoinState {
    SbColor ambient;
    SbColor specular;
    SbColor emissive;
    float shininess;
    SbBool blending;
    int32_t lightmodel;
    SbBool packeddiffuse;
    int32_t numdiffuse;
    int32_t numtransp;
    const SbColor * diffusearray;
    const uint32_t * packedarray;
    const float * transparray;
    const int32_t * colorindexarray;
    int32_t transptype;
    SbBool packedtransparency;
    uint32_t diffusenodeid;
    uint32_t transpnodeid;
    int32_t stipplenum;
    VertexOrdering vertexordering;
    SbBool twoside;
    SbBool culling;
    SbBool flatshading;
  } coinstate;

protected:
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
  virtual void setVertexOrderingElt(VertexOrdering ordering);
  virtual void setBackfaceCullingElt(SbBool onoff);
  virtual void setTwosideLightingElt(SbBool onoff);
  virtual void setShadeModelElt(SbBool flatshading);

private:
  SoLazyElementP * pimpl; // for future use

};

class COIN_DLL_API SoColorPacker {
public:
  SoColorPacker(void);
  ~SoColorPacker();

  uint32_t * getPackedColors(void) const {
    return this->array;
  }
  SbBool diffuseMatch(const uint32_t nodeid) const {
    return nodeid == this->diffuseid;
  }
  SbBool transpMatch(const uint32_t nodeid) const {
    return nodeid == this->transpid;
  }
  void setNodeIds(const uint32_t diffuseid, const uint32_t transpid) {
    this->diffuseid = diffuseid;
    this->transpid = transpid;
  }
  int32_t getSize(void) const {
    return this->arraysize;
  }
  void reallocate(const int32_t size);

private:
  uint32_t transpid;
  uint32_t diffuseid;
  uint32_t * array;
  int32_t arraysize;
};

#endif // !COIN_SOLAZYELEMENT_H
