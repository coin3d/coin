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
  \class SoGLEnvironmentElement Inventor/elements/SoGLEnvironmentElement.h
  \brief The SoGLEnvironmentElement class is for setting GL fog etc.
  
*/

#include <Inventor/elements/SoGLEnvironmentElement.h>

#include <Inventor/SbName.h>
#include <Inventor/SbViewVolume.h>
#include <Inventor/SbColor4f.h>

#if !defined(COIN_EXCLUDE_SOVIEWVOLUMEELEMENT)
#include <Inventor/elements/SoViewVolumeElement.h>
#endif // ! COIN_EXCLUDE_SOVIEWVOLUMEELEMENT

#ifdef _WIN32
#include <windows.h>
#endif // !_WIN32

#include <GL/gl.h>
#include <assert.h>

//$ BEGIN TEMPLATE ElementSource( SoGLEnvironmentElement )

/*!
  \var SoGLEnvironmentElement::classTypeId

  This is the static class type identifier for the
  SoGLEnvironmentElement class.
*/

SoType SoGLEnvironmentElement::classTypeId = SoType::badType();

/*!
  This method returns the SoType object for the element class of
  the instance.
*/
SoType
SoGLEnvironmentElement::getClassTypeId(void)
{
  return SoGLEnvironmentElement::classTypeId;
}

/*!
  This static method creates an object instance of the SoGLEnvironmentElement class.
*/
void *
SoGLEnvironmentElement::createInstance(void)
{
  return (void *) new SoGLEnvironmentElement;
}

/*!
  \var SoGLEnvironmentElement::classStackIndex

  This is the static state stack index for the
  SoGLEnvironmentElement class.
*/
int SoGLEnvironmentElement::classStackIndex;

/*!
  This static method returns the state stack index for the SoGLEnvironmentElement class.
*/
int
SoGLEnvironmentElement::getClassStackIndex(void)
{
  return SoGLEnvironmentElement::classStackIndex;
}
//$ END TEMPLATE ElementSource

/*!
  This static method initializes static data for the
  SoGLEnvironmentElement class.
*/

void
SoGLEnvironmentElement::initClass()
{
//$ BEGIN TEMPLATE InitElementSource( SoGLEnvironmentElement )
  assert(SoGLEnvironmentElement::classTypeId == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoGLEnvironmentElement::classTypeId = SoType::createType(
    inherited::getClassTypeId(),
    "SoGLEnvironmentElement",
    &SoGLEnvironmentElement::createInstance);

  if (inherited::classStackIndex < 0) {
    SoGLEnvironmentElement::classStackIndex =
      createStackIndex( SoGLEnvironmentElement::classTypeId );
  } else {
    SoGLEnvironmentElement::classStackIndex =
      inherited::classStackIndex;
  }
//$ END TEMPLATE InitElementSource
}

/*!
  This static method cleans up static data for the
  SoGLEnvironmentElement class.
*/

void
SoGLEnvironmentElement::cleanClass()
{
//$ BEGIN TEMPLATE CleanElementSource( SoGLEnvironmentElement )
//$ END TEMPLATE CleanElementSource
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoGLEnvironmentElement::createInstance( void )
*/

SoGLEnvironmentElement::SoGLEnvironmentElement()
{
  setTypeId( SoGLEnvironmentElement::classTypeId );
  setStackIndex( SoGLEnvironmentElement::classStackIndex );
}

/*!
  The destructor.
*/

SoGLEnvironmentElement::~SoGLEnvironmentElement()
{
}

//! FIXME: write doc.

void
SoGLEnvironmentElement::init(SoState * state)
{
  inherited::init(state);
  this->updategl(state);
}

//! FIXME: write doc.

void
SoGLEnvironmentElement::pop(SoState * state,
			   const SoElement * prevTopElement)
{
  ((SoGLEnvironmentElement*)prevTopElement)->updategl(state);
  inherited::pop(state, prevTopElement);
}

//! FIXME: write doc.

SbBool
SoGLEnvironmentElement::matches(const SoElement * element) const
{
  assert(0 && "FIXME: not implemented");
  return FALSE;
}

//! FIXME: write doc.

SoElement *
SoGLEnvironmentElement::copyMatchInfo() const
{
  assert(0 && "FIXME: not implemented");
  return NULL;
}


//! FIXME: doc
void
SoGLEnvironmentElement::setElt(SoState * const state,
			       const float ambientIntensity,
			       const SbColor & ambientColor,
			       const SbVec3f & attenuation,
			       const int32_t fogType,
			       const SbColor & fogColor,
			       const float fogVisibility)
{
  inherited::setElt(state, ambientIntensity, ambientColor, attenuation,
		    fogType, fogColor, fogVisibility);
  this->updategl(state);
}


//! FIXME: write doc.

void
SoGLEnvironmentElement::updategl(SoState * const state)
{
  float ambient[4];
  ambient[0] = ambientColor[0] * ambientIntensity;
  ambient[1] = ambientColor[1] * ambientIntensity;
  ambient[2] = ambientColor[2] * ambientIntensity;
  ambient[3] = 1.0f;

  // FIXME: not 100% sure if I should do this here...
  // the ambientColor might be only for ligths, pederb 990630
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
  
  if (fogType == (int)NONE) {
    glDisable(GL_FOG);
    return;
  }  
  
  float nearval = 1.0f;
  float farval = 10.0f;

#if !defined(COIN_EXCLUDE_SOVIEWVOLUMEELEMENT)
  const SbViewVolume &vv = SoViewVolumeElement::get(state); 
  nearval = vv.getNearDist();
  farval = nearval + vv.getDepth();
#endif // ! COIN_EXCLUDE_SOVIEWVOLUMEELEMENT

  float dist = fogVisibility;
  if (dist > 0.0f) farval = dist;
  
  switch (fogType) {
  case HAZE:
    glFogi(GL_FOG_MODE, GL_LINEAR);
    glFogf(GL_FOG_START, nearval);
    glFogf(GL_FOG_END, farval);
    break;
  case FOG:
    glFogi(GL_FOG_MODE, GL_EXP);
    
    // formula used for finding density:
    // 
    // 1/256 = e ^ -(density * farval)
    //
    // ln (1/256) = ln (e ^ -(density * farval))
    // 
    // -5.545 = - density * farval
    //
    // density = 5.545 / farval;

    glFogf(GL_FOG_DENSITY, 5.545f / farval);
    break;
  case SMOKE:
    glFogi(GL_FOG_MODE, GL_EXP2);
    // formula used for finding density:
    // 
    // 1/256 = e ^ (-(density * farval)^2)
    //
    // ln (1/256) = ln (e ^ (-(density * farval)^2))
    // 
    // -5.545 = - (density * farval)^2
    //
    // sqrt(5.545) = density * farval
    //
    // density = 2.35 / farval
    //
    
    glFogf(GL_FOG_DENSITY, 2.35f / farval); 
    break;
  default:
    assert(0 && "FIXME: unknown fog value");
    break;
  }
  
  SbColor4f color(fogColor, 1.0f);
  glFogfv(GL_FOG_COLOR, color.getValue());
  glEnable(GL_FOG);
}











