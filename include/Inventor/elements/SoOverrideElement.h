/**************************************************************************\
 * 
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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
 *  http://www.sim.no/ sales@sim.no Voice: +47 73540378 Fax: +47 73943861
 *
\**************************************************************************/

#ifndef __SOOVERRIDEELEMENT_H__
#define __SOOVERRIDEELEMENT_H__

#include <Inventor/elements/SoElement.h>
#include <Inventor/system/inttypes.h>

#if defined(COIN_EXCLUDE_SOOVERRIDEELEMENT)
#error "Configuration settings disable this class!"
#endif // COIN_EXCLUDE_SOOVERRIDEELEMENT

#define SO_GET_OVERRIDE(flag)                             \
    do {                                                    \
        const SoOverrideElement * const element =           \
            (const SoOverrideElement *)                     \
            getConstElement(state, classStackIndex);      \
        return (element->flags & flag);                     \
    } while (0)

#define SO_SET_OVERRIDE(flag)                             \
    do {                                                    \
        SoOverrideElement * const element =                 \
            (SoOverrideElement *)                           \
            getElement(state, classStackIndex);           \
        if (override)                                     \
            element->flags |= flag;                         \
        else                                                \
            element->flags &= ~flag;                        \
    } while (0)

class SoType; // lame doxygen "fix"

class SoOverrideElement : public SoElement {
  typedef SoElement inherited;

//$ BEGIN TEMPLATE ElementHeader( SoOverrideElement )
public:
  static SoType classTypeId;
  static SoType getClassTypeId(void);
  static void * createInstance(void);
public:
  static int getClassStackIndex(void);
  virtual ~SoOverrideElement(void);

  static void initClass(void);
  static void cleanClass(void);

protected:
  SoOverrideElement(void);
  static int classStackIndex;
//$ END TEMPLATE ElementHeader

public:
  enum FlagBits {
    AMBIENT_COLOR    = 0x00000001,
    COLOR_INDEX      = 0x00000002,
    COMPLEXITY       = 0x00000004,
    COMPLEXITY_TYPE  = 0x00000008,
    CREASE_ANGLE     = 0x00000010,
    DIFFUSE_COLOR    = 0x00000020,
    TRANSPARENCY     = DIFFUSE_COLOR,
    DRAW_STYLE       = 0x00000040,
    EMISSIVE_COLOR   = 0x00000080,
    FONT_NAME        = 0x00000100,
    FONT_SIZE        = 0x00000200,
    LIGHT_MODEL      = 0x00000400,
    LINE_PATTERN     = 0x00000800,
    LINE_WIDTH       = 0x00001000,
    MATERIAL_BINDING = 0x00002000,
    POINT_SIZE       = 0x00004000,
    PICK_STYLE       = 0x00008000,
    SHAPE_HINTS      = 0x00010000,
    SHININESS        = 0x00020000,
    SPECULAR_COLOR   = 0x00040000,
    POLYGON_OFFSET   = 0x00080000
  };

  virtual void init(SoState * state);

  virtual void push(SoState * state);
  virtual void pop(SoState * state,
		   const SoElement * prevTopElement);

  virtual SbBool matches(const SoElement * element) const;
  virtual SoElement * copyMatchInfo(void) const;

  static SbBool getAmbientColorOverride(SoState * const state);
  static SbBool getColorIndexOverride(SoState * const state);
  static SbBool getComplexityOverride(SoState * const state);
  static SbBool getComplexityTypeOverride(SoState * const state);
  static SbBool getCreaseAngleOverride(SoState * const state);
  static SbBool getDiffuseColorOverride(SoState * const state);
  static SbBool getDrawStyleOverride(SoState * const state);
  static SbBool getEmissiveColorOverride(SoState * const state);
  static SbBool getFontNameOverride(SoState * const state);
  static SbBool getFontSizeOverride(SoState * const state);
  static SbBool getLightModelOverride(SoState * const state);
  static SbBool getLinePatternOverride(SoState * const state);
  static SbBool getLineWidthOverride(SoState * const state);
  static SbBool getMaterialBindingOverride(SoState * const state);
  static SbBool getPickStyleOverride(SoState * const state);
  static SbBool getPointSizeOverride(SoState * const state);
  static SbBool getPolygonOffsetOverride(SoState * const state);
  static SbBool getShapeHintsOverride(SoState * const state);
  static SbBool getShininessOverride(SoState * const state);
  static SbBool getSpecularColorOverride(SoState * const state);
  static SbBool getTransparencyOverride(SoState * const state);

  static void setAmbientColorOverride(SoState * const state,
				      SoNode * const node,
				      const SbBool override);
  static void setColorIndexOverride(SoState * const state,
				    SoNode * const node,
				    const SbBool override);
  static void setComplexityOverride(SoState * const state,
				    SoNode * const node,
				    const SbBool override);
  static void setComplexityTypeOverride(SoState * const state,
					SoNode * const node,
					const SbBool override);
  static void setCreaseAngleOverride(SoState * const state,
				     SoNode * const node,
				     const SbBool override);
  static void setDiffuseColorOverride(SoState * const state,
				      SoNode * const node,
				      const SbBool override);
  static void setDrawStyleOverride(SoState * const state,
				   SoNode * const node,
				   const SbBool override);
  static void setEmissiveColorOverride(SoState * const state,
				       SoNode * const node,
				       const SbBool override);
  static void setFontNameOverride(SoState * const state,
				  SoNode * const node,
				  const SbBool override);
  static void setFontSizeOverride(SoState * const state,
				  SoNode * const node,
				  const SbBool override);
  static void setLightModelOverride(SoState * const state,
				    SoNode * const node,
				    const SbBool override);
  static void setLinePatternOverride(SoState * const state,
				     SoNode * const node,
				     const SbBool override);
  static void setLineWidthOverride(SoState * const state,
				   SoNode * const node,
				   const SbBool override);
  static void setMaterialBindingOverride(SoState * const state,
					 SoNode * const node,
					 const SbBool override);
  static void setPickStyleOverride(SoState * const state,
				   SoNode * const node,
				   const SbBool override);
  static void setPointSizeOverride(SoState * const state,
				   SoNode * const node,
				   const SbBool override);
  static void setPolygonOffsetOverride(SoState * const state,
				       SoNode * const node,
				       const SbBool override);
  static void setShapeHintsOverride(SoState * const state,
				    SoNode * const node,
				    const SbBool override);
  static void setShininessOverride(SoState * const state,
				   SoNode * const node,
				   const SbBool override);
  static void setSpecularColorOverride(SoState * const state,
				       SoNode * const node,
				       const SbBool override);
  static void setTransparencyOverride(SoState * const state,
				      SoNode * const node,
				      const SbBool override);

  virtual void print(FILE * file) const;

private:
  void pFlag(FILE * const file, const char * const format,
	     const int32_t flag) const;

  uint32_t flags;
};

#endif // !__SOOVERRIDEELEMENT_H__
