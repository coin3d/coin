/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

#ifndef COIN_SOLAZYELEMENT_H
#define COIN_SOLAZYELEMENT_H

#include <Inventor/elements/SoElement.h>
#include <Inventor/elements/SoSubElement.h>
#include <Inventor/SbColor.h>

class SoMFFloat;
class SoMFColor;
class SoColorPacker;

//
// this is just a wraparound class to offer the same functionality as 
// Inventor's LazyElement. Most methods are routed directyly to the
// subelements.
//

class SoLazyElement : public SoElement {
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
    LAZYCASES_LAST // must be last
  }; 
  enum masks{
    LIGHT_MODEL_MASK = 1 << LIGHT_MODEL_CASE, 
    COLOR_MATERIAL_MASK	= 1 << COLOR_MATERIAL_CASE,  
    DIFFUSE_MASK = 1 << DIFFUSE_CASE, 
    AMBIENT_MASK = 1 << AMBIENT_CASE, 
    EMISSIVE_MASK = 1<<EMISSIVE_CASE,
    SPECULAR_MASK = 1 << SPECULAR_CASE,
    SHININESS_MASK = 1 << SHININESS_CASE,
    TRANSPARENCY_MASK = 1 << TRANSPARENCY_CASE,
    BLENDING_MASK = 1 << BLENDING_CASE, 
    ALL_MASK = (1 << LAZYCASES_LAST)-1
  };

  enum LightModel {
    BASE_COLOR,
    PHONG
  };
    
  virtual void init(SoState *state);
  virtual void push(SoState *state);
  virtual SbBool matches(const SoElement *) const; 
  virtual SoElement *copyMatchInfo(void) const; 
  virtual void print(FILE *fp) const;

  static void setDiffuse(SoState *state, SoNode *node, int32_t numcolors, 
                         const SbColor *colors, SoColorPacker *packer);		    
  static void setTransparency(SoState *state, SoNode *node, int32_t numvalues, 
                              const float *transparency, SoColorPacker *packer);
  static void setPacked(SoState *state, SoNode *node,
                        int32_t numcolors, const uint32_t *colors);
  static void setColorIndices(SoState *state, SoNode *node, 
                              int32_t numindices, const int32_t *indices);
  static void setAmbient(SoState *state, const SbColor* color);          
  static void setEmissive(SoState *state, const SbColor* color);
  static void setSpecular(SoState *state, const SbColor* color);
  static void setShininess(SoState *state, float value);
  static void setColorMaterial(SoState *state, SbBool value);
  static void setBlending(SoState *state,  SbBool value);
  static void setLightModel(SoState *state, const int32_t model);    
  static const SbColor &getDiffuse(SoState* state, int index);
  static float getTransparency(SoState*, int index);
  static const uint32_t	*getPackedColors(SoState*);
  static const int32_t	*getColorIndices(SoState*);
  static int32_t getColorIndex(SoState*, int num);
  static const SbColor &getAmbient(SoState*);
  static const SbColor &getEmissive(SoState*);
  static const SbColor &getSpecular(SoState*);
  static float getShininess(SoState*);
  static SbBool getColorMaterial(SoState*);
  static SbBool getBlending(SoState*);
  static int32_t getLightModel(SoState*);
    
  int32_t getNumDiffuse(void) const;
  int32_t getNumTransparencies(void) const;
  int32_t getNumColorIndices(void) const;
  SbBool isPacked(void) const;
  SbBool isTransparent(void) const;    
  static SoLazyElement *getInstance(SoState *state);
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
       
  static void setMaterials(SoState *state, SoNode *node, uint32_t bitmask,
                           SoColorPacker *cPacker,   
                           const SoMFColor& diffuse, const SoMFFloat& transp, 
                           const SoMFColor& ambient, const SoMFColor& emissive, 
                           const SoMFColor& specular, const SoMFFloat& shininess);
			  
  static void setMaterials(SoState *state, SoNode *node, uint32_t bitmask,
                           SoColorPacker *cPacker,
                           SoMFColor *diffuse, SoMFFloat *transp,
                           SoMFColor *ambient, SoMFColor *emissive,
                           SoMFColor *specular, SoMFFloat *shininess);

  
  static SoLazyElement * getWInstance(SoState *state);    
  
  const uint32_t *getPackedPointer(void) const;
  const SbColor* getDiffusePointer(void) const;
  const int32_t	*getColorIndexPointer(void) const;
  
  const float *getTransparencyPointer(void) const;
  static void setTransparencyType(SoState *state, int32_t type);


private:
 
  SoState *state;
  SbBool colorMaterial;
  SbBool blending;
  float shininess;
  int32_t transparencyType;

  // needed only for matches() and copyMatchInfo()
  SoElement *lightModelMatchInfo;
  SoElement *diffuseMatchInfo;
  SoElement *ambientMatchInfo;
  SoElement *emissiveMatchInfo;
  SoElement *specularMatchInfo;
  SoElement *shininessMatchInfo;
  SoElement *transparencyMatchInfo;
};

//
// We really don't care about this class
//
class SoColorPacker {
public:
  SoColorPacker() {}   
  ~SoColorPacker() {}
  uint32_t* getPackedColors(void) const {return NULL;}
  SbBool diffuseMatch(const uint32_t /*nodeid*/) const {return FALSE;}
  SbBool transpMatch(const uint32_t /*nodeid*/) const {return FALSE;}
  void setNodeIds(const uint32_t /*diffusenodeid*/, const uint32_t /*transparencynodeid*/) {}
  int32_t getSize(void) const {return 0;}
  void reallocate(const int32_t /*size*/) {}    
};
    

#endif // COIN_SOLAZYELEMENT_H

