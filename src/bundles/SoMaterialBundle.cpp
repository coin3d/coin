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
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

/*!
  \class SoMaterialBundle include/Inventor/SoMaterialBundle.h
  \brief The SoMaterialBundle class simplifies material handling.

  bla, bla, bla, FIXME, doc
*/


#include <Inventor/bundles/SoMaterialBundle.h>

#if !defined(COIN_EXCLUDE_SOSTATE)
#include <Inventor/misc/SoState.h>
#endif // !COIN_EXCLUDE_SOSTATE

#if !defined(COIN_EXCLUDE_SOGLAMBIENTCOLORELEMENT)
#include <Inventor/elements/SoGLAmbientColorElement.h>
#endif // !COIN_EXCLUDE_SOGLAMBIENTCOLORELEMENT
#if !defined(COIN_EXCLUDE_SOGLDIFFUSECOLORELEMENT)
#include <Inventor/elements/SoGLDiffuseColorElement.h>
#endif // !COIN_EXCLUDE_SOGLDIFFUSECOLORELEMENT
#if !defined(COIN_EXCLUDE_SOGLEMISSIVECOLORELEMENT)
#include <Inventor/elements/SoGLEmissiveColorElement.h>
#endif // !COIN_EXCLUDE_SOGLEMISSIVECOLORELEMENT
#if !defined(COIN_EXCLUDE_SOGLPOLYGONSTIPPLEELEMENT)
#include <Inventor/elements/SoGLPolygonStippleElement.h>
#endif // !COIN_EXCLUDE_SOGLPOLYGONSTIPPLEELEMENT
#if !defined(COIN_EXCLUDE_SOGLSHININESSELEMENT)
#include <Inventor/elements/SoGLShininessElement.h>
#endif // !COIN_EXCLUDE_SOGLSHININESSELEMENT
#if !defined(COIN_EXCLUDE_SOGLSPECULARCOLORELEMENT)
#include <Inventor/elements/SoGLSpecularColorElement.h>
#endif // !COIN_EXCLUDE_SOGLSPECULARCOLORELEMENT
#if !defined(COIN_EXCLUDE_SOTRANSPARENCYELEMENT)
#include <Inventor/elements/SoTransparencyElement.h>
#endif // !COIN_EXCLUDE_SOTRANSPARENCYELEMENT
#if !defined(COIN_EXCLUDE_SOLIGHTMODELELEMENT)
#include <Inventor/elements/SoLightModelElement.h>
#endif // ! COIN_EXCLUDE_SOLIGHTMODELELEMENT
#if !defined(COIN_EXCLUDE_SOSHAPESTYLEELEMENT)
#include <Inventor/elements/SoShapeStyleElement.h>
#endif // ! COIN_EXCLUDE_SOSHAPESTYLEELEMENT

#include <Inventor/misc/SoState.h>

#ifdef _WIN32
#include <windows.h>
#endif // _WIN32

#include <GL/gl.h>

#include <assert.h>

/*!
  Constructor with \a action being the action applied to the
  geometry node.
*/
SoMaterialBundle::SoMaterialBundle(SoAction *action)
  : SoBundle(action)
{
  this->firstTime = TRUE;
  this->packedColors = NULL;
  this->state->push(); // for stipple element.. FIXME: might be avoided.
}

/*!
  Destructor
*/
SoMaterialBundle::~SoMaterialBundle()
{
  this->state->pop(); // for stipple element... FIXME: might be avoided
}

/*!
  Currently not in use. It is only provided for OIV comliance.
*/
void 
SoMaterialBundle::setUpMultiple()
{
}

/*!
  Sends the initial material values to GL. Must be done once in all
  geometry nodes before the rendering begins.
*/
void 
SoMaterialBundle::sendFirst()
{
  this->setupElements(FALSE);
  this->reallySend(0, FALSE);  

  // a small optimization to avoid uneccesary material
  // testing (it is most common to have multiple diffuse values)
  if (!diffuseOnly) {
    if (TRUE && 
#if !defined(COIN_EXCLUDE_SOGLAMBIENTCOLORELEMENT)
	ambientElt->getNum() <= 1 &&
#endif
#if !defined(COIN_EXCLUDE_SOGLSPECULARCOLORELEMENT)
	specularElt->getNum() <= 1 &&
#endif
#if !defined(COIN_EXCLUDE_SOGLEMISSIVECOLORELEMENT)
	emissiveElt->getNum() <= 1 &&
#endif
#if !defined(COIN_EXCLUDE_SOGLSHININESSELEMENT)
	shininessElt->getNum() <= 1
#endif
	) diffuseOnly = TRUE;
  }
}

/*!
  Sends material values with index \a index to GL. Will test
  whether the current index equals \a index before sending.

  \a betweenBeginEnd should be \e TRUE if your program is 
  between a glBegin() and glEnd() (it is illegal to change
  polygon stipple between a glBegin() and glEnd()).

*/
void 
SoMaterialBundle::send(const int index, const SbBool betweenBeginEnd)
{
  if (this->firstTime) this->setupElements(FALSE);
  if (index != this->currIndex) {
    reallySend(index, betweenBeginEnd);
  }
}

/*!
  Will send the material to GL even though \a index equals
  the current index. Provided for OIV compability.
*/
void 
SoMaterialBundle::forceSend(const int index)
{
  if (this->firstTime) this->setupElements(FALSE);
  this->reallySend(index, FALSE);
}

/*!
  Returns \e TRUE if the current light model is BASE_COLOR.
*/
SbBool 
SoMaterialBundle::isColorOnly() const
{
  return this->colorOnly;
}

//
// private method. Will send the material values to GL.
//
void 
SoMaterialBundle::reallySend(const int index, const SbBool isBetweenBeginEnd)
{
#if !defined(COIN_EXCLUDE_SOGLPOLYGONSTIPPLEELEMENT)
  if (doStipple && !isBetweenBeginEnd) {
#if !defined(COIN_EXCLUDE_SOTRANSPARENCYELEMENT)
    float trans = transparencyElt->get(index);
    if (packedColors) {
      trans = (255 - (packedColors[index] & 0xff)) / 255.0f;
    }
    if (trans > 0.0f) {
      SoGLPolygonStippleElement::set(this->state, TRUE);
      SoGLPolygonStippleElement::setTransparency(this->state, trans);
    }
    else
#endif // !COIN_EXCLUDE_SOTRANSPARENCYELEMENT
      SoGLPolygonStippleElement::set(this->state, FALSE);
    stippleElt->evaluate(); // this is a lazy element. Force send.
  }
#endif // !COIN_EXCLUDE_SOGLPOLYGONSTIPPLEELEMENT

  // FIXME: probably not needed anymore. It was an ugly piece 
  // of code anyway... pederb, 990608
  if (packedColors) {
    if (1 || index != this->currPacked) {
      this->currPacked = index;
      if (this->currPacked >= this->numPacked) 
	this->currPacked = this->numPacked - 1;
      uint32_t p = packedColors[this->currPacked];
      glColor4ub((p&0xff000000)>>24,
		 (p&0xff0000)>>16,
		 (p&0xff00)>>8,
		 p&0xff);
    }
  }
#if !defined(COIN_EXCLUDE_SOGLDIFFUSECOLORELEMENT)
  else {
    if (!isBetweenBeginEnd) {
#if !defined(COIN_EXCLUDE_SOTRANSPARENCYELEMENT)
      diffuseElt->send(index, 1.0f - transparencyElt->get(index));
#else // COIN_EXCLUDE_SOTRANSPARENCYELEMENT
      diffuseElt->send(index);
#endif // ! COIN_EXCLUDE_SOTRANSPARENCYELEMENT
    }
    else {
      diffuseElt->send(index);
    }
  }
#endif // !COIN_EXCLUDE_SOGLDIFFUSECOLORELEMENT

  if (!diffuseOnly) {
#if !defined(COIN_EXCLUDE_SOGLAMBIENTCOLORELEMENT)
    ambientElt->send(index);
#endif // !COIN_EXCLUDE_SOGLAMBIENTCOLORELEMENT
#if !defined(COIN_EXCLUDE_SOGLEMISSIVECOLORELEMENT)
    emissiveElt->send(index);
#endif // !COIN_EXCLUDE_SOGLEMISSIVECOLORELEMENT
#if !defined(COIN_EXCLUDE_SOGLSPECULARCOLORELEMENT)
    specularElt->send(index);
#endif // !COIN_EXCLUDE_SOGLSPECULARCOLORELEMENT
#if !defined(COIN_EXCLUDE_SOGLSHININESSELEMENT)
    shininessElt->send(index);
#endif // !COIN_EXCLUDE_SOGLSHININESSELEMENT
#if !defined(COIN_EXCLUDE_SOGLPOLYGONSTIPPLEELEMENT)
    //    if (doStipple) stippleElt->send();
#endif // !COIN_EXCLUDE_SOGLPOLYGONSTIPPLEELEMENT
  }
  // store current index
  this->currIndex = index;
}

//
// private method. Stores info and element pointers.
//
void 
SoMaterialBundle::setupElements(const SbBool betweenBeginEnd)
{
  this->currIndex = -1; // set to an impossible value
  this->firstTime = FALSE;
#if !defined(COIN_EXCLUDE_SOSHAPESTYLEELEMENT)
  this->doStipple = SoShapeStyleElement::isScreenDoor(this->state);
#else // COIN_EXCLUDE_SOSHAPESTYLEELEMENT
  this->doStipple = TRUE;
#endif // ! COIN_EXCLUDE_SOSHAPESTYLEELEMENT
  this->diffuseOnly = this->colorOnly = 
    SoLightModelElement::get(this->state) ==
    SoLightModelElement::BASE_COLOR;
  
#if !defined(COIN_EXCLUDE_SOGLDIFFUSECOLORELEMENT)
  diffuseElt = (SoGLDiffuseColorElement*)
    state->getConstElement(SoGLDiffuseColorElement::getClassStackIndex());
  if (diffuseElt->isPacked()) 
    setPacked(diffuseElt->packedColors, diffuseElt->numColors);
#endif // !COIN_EXCLUDE_SOGLDIFFUSECOLORELEMENT
#if !defined(COIN_EXCLUDE_SOTRANSPARENCYELEMENT)
  transparencyElt = (SoTransparencyElement*)
    state->getConstElement(SoTransparencyElement::getClassStackIndex());
#endif // !COIN_EXCLUDE_SOTRANSPARENCYELEMENT

#if !defined(COIN_EXCLUDE_SOGLPOLYGONSTIPPLEELEMENT)
  if (doStipple) {
    stippleElt = (SoGLPolygonStippleElement*)
      state->getElement(SoGLPolygonStippleElement::getClassStackIndex());
  }
#endif // !COIN_EXCLUDE_SOGLPOLYGONSTIPPLEELEMENT

  if (!colorOnly) {
#if !defined(COIN_EXCLUDE_SOGLAMBIENTCOLORELEMENT)
    ambientElt = (SoGLAmbientColorElement*)
      state->getConstElement(SoGLAmbientColorElement::getClassStackIndex());
#endif // !COIN_EXCLUDE_SOGLAMBIENTCOLORELEMENT
#if !defined(COIN_EXCLUDE_SOGLEMISSIVECOLORELEMENT)
    emissiveElt = (SoGLEmissiveColorElement*)
      state->getConstElement(SoGLEmissiveColorElement::getClassStackIndex());
#endif // !COIN_EXCLUDE_SOGLEMISSIVECOLORELEMENT
#if !defined(COIN_EXCLUDE_SOGLSPECULARCOLORELEMENT)
    specularElt = (SoGLSpecularColorElement*)
      state->getConstElement(SoGLSpecularColorElement::getClassStackIndex());
#endif // !COIN_EXCLUDE_SOGLSPECULARCOLORELEMENT
#if !defined(COIN_EXCLUDE_SOGLSHININESSELEMENT)
    shininessElt = (SoGLShininessElement*)
      state->getConstElement(SoGLShininessElement::getClassStackIndex());
#endif // !COIN_EXCLUDE_SOGLSHININESSELEMENT
  }
}

/*!
  FIXME: document (pederb)
*/
void 
SoMaterialBundle::setPacked(const uint32_t *packed,
			    const int num)
{
//   this->packedColors = packed;
//   this->numPacked = num;
//   this->currPacked = -1;
}
