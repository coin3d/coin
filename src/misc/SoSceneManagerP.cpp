/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2007 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#include "SoSceneManagerP.h"

#include <Inventor/nodes/SoCamera.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoDrawStyleElement.h>
#include <Inventor/elements/SoComplexityTypeElement.h>
#include <Inventor/elements/SoPolygonOffsetElement.h>
#include <Inventor/elements/SoMaterialBindingElement.h>
#include <Inventor/elements/SoOverrideElement.h>
#include <Inventor/elements/SoTextureOverrideElement.h>
#include <Inventor/elements/SoTextureQualityElement.h>
#include <Inventor/elements/SoLightModelElement.h>
#include <Inventor/elements/SoLazyElement.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/sensors/SoNodeSensor.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/fields/SoSFTime.h>
#include <Inventor/SoDB.h>

SbBool SoSceneManagerP::touchtimer = TRUE;
SbBool SoSceneManagerP::cleanupfunctionset = FALSE;

#define PRIVATE(p) (p->pimpl)
#define PUBLIC(p) (p->publ)

SoSceneManagerP::SoSceneManagerP(SoSceneManager * publ) 
{
  PUBLIC(this) = publ;
  this->doublebuffer = TRUE;
}

SoSceneManagerP::~SoSceneManagerP()
{
  if (this->camera) {
    this->camera->unref();
  }
}

// Internal callback.
void
SoSceneManagerP::redrawshotTriggeredCB(void * data, SoSensor * /* sensor */)
{
#if COIN_DEBUG && 0 // debug
  SoDebugError::postInfo("SoSceneManager::redrawshotTriggeredCB", "start");
#endif // debug

  SoSceneManager * sm = (SoSceneManager *)data;

  // Need to recheck the "active" flag, as it could have changed since
  // it was tested in the SoSceneManager::scheduleRedraw() call.
  if (PRIVATE(sm)->isActive()) { sm->redraw(); }

#if COIN_DEBUG && 0 // debug
  SoDebugError::postInfo("SoSceneManager::redrawshotTriggeredCB", "done\n\n");
#endif // debug
}

void
SoSceneManagerP::setCamera(SoCamera * camera)
{
  if (this->camera) {
    this->camera->unref();
  }
  this->camera = camera;
  if (camera) camera->ref();
}

SoCamera * 
SoSceneManagerP::getCamera(void) 
{
  if (this->camera) return this->camera;
  
  this->searchaction.setType(SoCamera::getClassTypeId());
  this->searchaction.setInterest(SoSearchAction::FIRST);
  SbBool old = SoBaseKit::isSearchingChildren();
  SoBaseKit::setSearchingChildren(TRUE);
  this->searchaction.apply(PUBLIC(this)->getSceneGraph());
  SoBaseKit::setSearchingChildren(old);
  SoFullPath * path = (SoFullPath*) this->searchaction.getPath();
  if (path) {
    SoNode * tail = path->getTail();
    this->searchaction.reset();
    return (SoCamera*) tail;
  }
  return NULL;
}

void
SoSceneManagerP::cleanup(void)
{
  SoSceneManagerP::touchtimer = TRUE;
  SoSceneManagerP::cleanupfunctionset = FALSE;
}

//**********************************************************************************
// Superimposition
//**********************************************************************************

class SuperimpositionP {
public:
  SoNode * scene;
  SbBool enabled;
  SoSceneManager * manager;
  SoNodeSensor * sensor;
  uint32_t stateflags;
};

Superimposition::Superimposition(SoNode * scene,
                                 SbBool enabled,
                                 SoSceneManager * manager,
                                 uint32_t flags)
{
  assert(scene != NULL);
  PRIVATE(this) = new SuperimpositionP;

  PRIVATE(this)->scene = scene;
  PRIVATE(this)->scene->ref();
  
  PRIVATE(this)->enabled = enabled;
  PRIVATE(this)->stateflags = flags;
  
  PRIVATE(this)->manager = manager;
  PRIVATE(this)->sensor = new SoNodeSensor(Superimposition::changeCB, this);
  PRIVATE(this)->sensor->attach(PRIVATE(this)->scene);
}

Superimposition::~Superimposition()
{
  PRIVATE(this)->scene->unref();
  delete PRIVATE(this)->sensor;
  delete PRIVATE(this);
}

void 
Superimposition::render(void) 
{
  if (!PRIVATE(this)->enabled) return;

  SbBool zbufferwason = glIsEnabled(GL_DEPTH_TEST) ? TRUE : FALSE;
  
  PRIVATE(this)->stateflags & Superimposition::ZBUFFERON ?
    glEnable(GL_DEPTH_TEST):
    glDisable(GL_DEPTH_TEST);

  if (PRIVATE(this)->stateflags & Superimposition::CLEARZBUFFER)
    glClear(GL_DEPTH_BUFFER_BIT);
  
  PRIVATE(this)->manager->getGLRenderAction()->apply(PRIVATE(this)->scene);
  
  zbufferwason ?
    glEnable(GL_DEPTH_TEST):
    glDisable(GL_DEPTH_TEST);
}

void 
Superimposition::setEnabled(SbBool yes)
{
  PRIVATE(this)->enabled = yes;
}

void 
Superimposition::changeCB(void * data, SoSensor * sensor) 
{
  Superimposition * thisp = (Superimposition *) data;
  assert(thisp && PRIVATE(thisp)->manager);
  if (PRIVATE(thisp)->stateflags & Superimposition::AUTOREDRAW) {
    PRIVATE(thisp)->manager->scheduleRedraw();
  }
}


#undef PRIVATE
#undef PUBLIC
