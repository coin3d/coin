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

/*!
  \class SoOverrideElement Inventor/elements/SoOverrideElement.h
  \brief The SoOverrideElement maintains a list of overridable elements and a list over which elements should be overridden.

  Only certain elements can be overridden.
*/

#include <Inventor/elements/SoOverrideElement.h>

#include <Inventor/SbName.h>
#include <assert.h>

/*!
  \enum SoOverrideElement::FlagBits

  FIXME: write doc.
*/

//$ BEGIN TEMPLATE ElementSource( SoOverrideElement )

/*!
  \var SoOverrideElement::classTypeId

  This is the static class type identifier for the
  SoOverrideElement class.
*/

SoType SoOverrideElement::classTypeId = SoType::badType();

/*!
  This method returns the SoType object for the element class of
  the instance.
*/
SoType
SoOverrideElement::getClassTypeId(void)
{
  return SoOverrideElement::classTypeId;
}

/*!
  This static method creates an object instance of the SoOverrideElement class.
*/
void *
SoOverrideElement::createInstance(void)
{
  return (void *) new SoOverrideElement;
}

/*!
  \var SoOverrideElement::classStackIndex

  This is the static state stack index for the
  SoOverrideElement class.
*/
int SoOverrideElement::classStackIndex;

/*!
  This static method returns the state stack index for the SoOverrideElement class.
*/
int
SoOverrideElement::getClassStackIndex(void)
{
  return SoOverrideElement::classStackIndex;
}
//$ END TEMPLATE ElementSource

/*!
  This static method initializes static data for the
  SoOverrideElement class.
*/

void
SoOverrideElement::initClass(void)
{
//$ BEGIN TEMPLATE InitElementSource( SoOverrideElement )
  assert(SoOverrideElement::classTypeId == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoOverrideElement::classTypeId = SoType::createType(
    inherited::getClassTypeId(),
    "SoOverrideElement",
    &SoOverrideElement::createInstance);

  if (inherited::classStackIndex < 0) {
    SoOverrideElement::classStackIndex =
      createStackIndex( SoOverrideElement::classTypeId );
  } else {
    SoOverrideElement::classStackIndex =
      inherited::classStackIndex;
  }
//$ END TEMPLATE InitElementSource
}

/*!
  This static method cleans up static data for the SoOverrideElement class.
*/

void
SoOverrideElement::cleanClass(void)
{
//$ BEGIN TEMPLATE CleanElementSource( SoOverrideElement )
//$ END TEMPLATE CleanElementSource
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoOverrideElement::createInstance(void)
*/

SoOverrideElement::SoOverrideElement(void)
  : flags(0)
{
  setTypeId(SoOverrideElement::classTypeId);
  setStackIndex(SoOverrideElement::classStackIndex);
}

/*!
  The destructor.
*/

SoOverrideElement::~SoOverrideElement(void)
{
}

//! FIXME: write doc.

void
SoOverrideElement::init(SoState * state)
{
  inherited::init(state);
}

//! FIXME: write doc.

void
SoOverrideElement::push(SoState * state)
{
  inherited::push(state);
}

//! FIXME: write doc.

void
SoOverrideElement::pop(SoState * state, const SoElement * prevTopElement)
{
  inherited::pop(state, prevTopElement);
}

//! FIXME: write doc.

SbBool
SoOverrideElement::matches(const SoElement * element) const
{
  assert(0 && "FIXME: not implemented");
  return FALSE;
}

//! FIXME: write doc.

SoElement *
SoOverrideElement::copyMatchInfo(void) const
{
  assert(0 && "FIXME: not implemented");
  return FALSE;
}

//! FIXME: write doc.

void
SoOverrideElement::print(FILE * file) const
{
}

//! FIXME: write doc.

void
SoOverrideElement::pFlag(FILE * const file, const char * const format,
			 const int32_t flag) const
{
  assert(0 && "FIXME: not implemented");
}


/*!
  FIXME: write doc.
*/

SbBool
SoOverrideElement::getAmbientColorOverride(SoState * const state)
{
  SO_GET_OVERRIDE(AMBIENT_COLOR);
}

/*!
  FIXME: write doc.
*/

SbBool
SoOverrideElement::getColorIndexOverride(SoState * const state)
{
  SO_GET_OVERRIDE(COLOR_INDEX);
}

/*!
  FIXME: write doc.
*/

SbBool
SoOverrideElement::getComplexityOverride(SoState * const state)
{
  SO_GET_OVERRIDE(COMPLEXITY);
}

/*!
  FIXME: write doc.
*/

SbBool
SoOverrideElement::getComplexityTypeOverride(SoState * const state)
{
  SO_GET_OVERRIDE(COMPLEXITY_TYPE);
}

/*!
  FIXME: write doc.
*/

SbBool
SoOverrideElement::getCreaseAngleOverride(SoState * const state)
{
  SO_GET_OVERRIDE(CREASE_ANGLE);
}

/*!
  FIXME: write doc.
*/

SbBool
SoOverrideElement::getDiffuseColorOverride(SoState * const state)
{
  SO_GET_OVERRIDE(DIFFUSE_COLOR);
}

/*!
  FIXME: write doc.
*/

SbBool
SoOverrideElement::getDrawStyleOverride(SoState * const state)
{
  SO_GET_OVERRIDE(DRAW_STYLE);
}

/*!
  FIXME: write doc.
*/

SbBool
SoOverrideElement::getEmissiveColorOverride(SoState * const state)
{
  SO_GET_OVERRIDE(EMISSIVE_COLOR);
}

/*!
  FIXME: write doc.
*/

SbBool
SoOverrideElement::getFontNameOverride(SoState * const state)
{
  SO_GET_OVERRIDE(FONT_NAME);
}

/*!
  FIXME: write doc.
*/

SbBool
SoOverrideElement::getFontSizeOverride(SoState * const state)
{
  SO_GET_OVERRIDE(FONT_SIZE);
}

/*!
  FIXME: write doc.
*/

SbBool
SoOverrideElement::getLightModelOverride(SoState * const state)
{
  SO_GET_OVERRIDE(LIGHT_MODEL);
}

/*!
  FIXME: write doc.
*/

SbBool
SoOverrideElement::getLinePatternOverride(SoState * const state)
{
  SO_GET_OVERRIDE(LINE_PATTERN);
}

/*!
  FIXME: write doc.
*/

SbBool
SoOverrideElement::getLineWidthOverride(SoState * const state)
{
  SO_GET_OVERRIDE(LINE_WIDTH);
}

/*!
  FIXME: write doc.
*/

SbBool
SoOverrideElement::getMaterialBindingOverride(SoState * const state)
{
  SO_GET_OVERRIDE(MATERIAL_BINDING);
}

/*!
  FIXME: write doc.
*/

SbBool
SoOverrideElement::getPointSizeOverride(SoState * const state)
{
  SO_GET_OVERRIDE(POINT_SIZE);
}

/*!
  FIXME: write doc.
*/

SbBool
SoOverrideElement::getPickStyleOverride(SoState * const state)
{
  SO_GET_OVERRIDE(PICK_STYLE);
}

/*!
  FIXME: write doc.
*/

SbBool
SoOverrideElement::getShapeHintsOverride(SoState * const state)
{
  SO_GET_OVERRIDE(SHAPE_HINTS);
}

/*!
  FIXME: write doc.
*/

SbBool
SoOverrideElement::getShininessOverride(SoState * const state)
{
  SO_GET_OVERRIDE(SHININESS);
}

/*!
  FIXME: write doc.
*/

SbBool
SoOverrideElement::getSpecularColorOverride(SoState * const state)
{
  SO_GET_OVERRIDE(SPECULAR_COLOR);
}

/*!
  FIXME: write doc.
*/

SbBool
SoOverrideElement::getTransparencyOverride(SoState * const state)
{
  SO_GET_OVERRIDE(TRANSPARENCY);
}

/*!
  FIXME: write doc.
*/

SbBool
SoOverrideElement::getPolygonOffsetOverride(SoState * const state)
{
  SO_GET_OVERRIDE(POLYGON_OFFSET);
}

/*!
  FIXME: write doc.
*/

void
SoOverrideElement::setAmbientColorOverride(SoState * const state,
					   SoNode * const node,
					   const SbBool override)
{
  SO_SET_OVERRIDE(AMBIENT_COLOR);
}

/*!
  FIXME: write doc.
*/

void
SoOverrideElement::setColorIndexOverride(SoState * const state,
					 SoNode * const node,
					 const SbBool override)
{
  SO_SET_OVERRIDE(COLOR_INDEX);
}

/*!
  FIXME: write doc.
*/

void
SoOverrideElement::setComplexityOverride(SoState * const state,
					 SoNode * const node,
					 const SbBool override)
{
  SO_SET_OVERRIDE(COMPLEXITY);
}

/*!
  FIXME: write doc.
*/

void
SoOverrideElement::setComplexityTypeOverride(SoState * const state,
					     SoNode * const node,
					     const SbBool override)
{
  SO_SET_OVERRIDE(COMPLEXITY_TYPE);
}

/*!
  FIXME: write doc.
*/

void
SoOverrideElement::setCreaseAngleOverride(SoState * const state,
					  SoNode * const node,
					  const SbBool override)
{
  SO_SET_OVERRIDE(CREASE_ANGLE);
}

//! FIXME: write doc.

void
SoOverrideElement::setDiffuseColorOverride(SoState * const state,
					   SoNode * const node,
					   const SbBool override)
{
  assert(0 && "FIXME: not implemented");
}

/*!
  FIXME: write doc.
*/

void
SoOverrideElement::setDrawStyleOverride(SoState * const state,
					SoNode * const node,
					const SbBool override)
{
  SO_SET_OVERRIDE(DRAW_STYLE);
}

/*!
  FIXME: write doc.
*/

void
SoOverrideElement::setEmissiveColorOverride(SoState * const state,
					    SoNode * const node,
					    const SbBool override)
{
  SO_SET_OVERRIDE(EMISSIVE_COLOR);
}

/*!
  FIXME: write doc.
*/

void
SoOverrideElement::setFontNameOverride(SoState * const state,
				       SoNode * const node,
				       const SbBool override)
{
  SO_SET_OVERRIDE(FONT_NAME);
}

/*!
  FIXME: write doc.
*/

void
SoOverrideElement::setFontSizeOverride(SoState * const state,
				       SoNode * const node,
				       const SbBool override)
{
  SO_SET_OVERRIDE(FONT_SIZE);
}

/*!
  FIXME: write doc.
*/

void
SoOverrideElement::setLightModelOverride(SoState * const state,
					 SoNode * const node,
					 const SbBool override)
{
  SO_SET_OVERRIDE(LIGHT_MODEL);
}

/*!
  FIXME: write doc.
*/

void
SoOverrideElement::setLinePatternOverride(SoState * const state,
					  SoNode * const node,
					  const SbBool override)
{
  SO_SET_OVERRIDE(LINE_PATTERN);
}

/*!
  FIXME: write doc.
*/

void
SoOverrideElement::setLineWidthOverride(SoState * const state,
					SoNode * const node,
					const SbBool override)
{
  SO_SET_OVERRIDE(LINE_WIDTH);
}

//! FIXME: write doc.

void
SoOverrideElement::setMaterialBindingOverride(SoState * const state,
					      SoNode * const node,
					      const SbBool override)
{
  assert(0 && "FIXME: not implemented");
}

/*!
  FIXME: write doc.
*/

void
SoOverrideElement::setPickStyleOverride(SoState * const state,
					SoNode * const node,
					const SbBool override)
{
  SO_SET_OVERRIDE(PICK_STYLE);
}

/*!
  FIXME: write doc.
*/

void
SoOverrideElement::setPointSizeOverride(SoState * const state,
					SoNode * const node,
					const SbBool override)
{
  SO_SET_OVERRIDE(POINT_SIZE);
}

/*!
  FIXME: write doc.
*/

void
SoOverrideElement::setPolygonOffsetOverride(SoState * const state,
					    SoNode * const node,
					    const SbBool override)
{
  SO_SET_OVERRIDE(POLYGON_OFFSET);
}

/*!
  FIXME: write doc.
*/

void
SoOverrideElement::setShapeHintsOverride(SoState * const state,
					 SoNode * const node,
					 const SbBool override)
{
  SO_SET_OVERRIDE(SHAPE_HINTS);
}

/*!
  FIXME: write doc.
*/

void
SoOverrideElement::setShininessOverride(SoState * const state,
					SoNode * const node,
					const SbBool override)
{
  SO_SET_OVERRIDE(SHININESS);
}

/*!
  FIXME: write doc.
*/

void
SoOverrideElement::setSpecularColorOverride(SoState * const state,
					    SoNode * const node,
					    const SbBool override)
{
  SO_SET_OVERRIDE(SPECULAR_COLOR);
}

//! FIXME: write doc.

void
SoOverrideElement::setTransparencyOverride(SoState * const state,
					   SoNode * const node,
					   const SbBool override)
{
  assert(0 && "FIXME: not implemented");
}
