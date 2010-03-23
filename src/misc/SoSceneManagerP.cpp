/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#include "SoSceneManagerP.h"
#include "coindefs.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#include <Inventor/SoDB.h>
#include <Inventor/system/gl.h>
#include <Inventor/nodes/SoCamera.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/sensors/SoNodeSensor.h>
#ifdef HAVE_NODEKITS
#include <Inventor/nodekits/SoBaseKit.h>
#endif // HAVE_NODEKITS


#define PRIVATE(p) (p->pimpl)
#define PUBLIC(p) (p->publ)

SoSceneManagerP::SoSceneManagerP(SoSceneManager * publ) 
{
  PUBLIC(this) = publ;
  this->searchaction = new SoSearchAction;
}

SoSceneManagerP::~SoSceneManagerP()
{
  delete this->searchaction;
}

// defcamera is the default camera returned if none is found in root

SoCamera * 
SoSceneManagerP::searchForCamera(SoNode * root,
                                 SoCamera * defcamera)
{
  this->searchaction->setType(SoCamera::getClassTypeId());
  this->searchaction->setInterest(SoSearchAction::FIRST);
#ifdef HAVE_NODEKITS
  SbBool old = SoBaseKit::isSearchingChildren();
  SoBaseKit::setSearchingChildren(TRUE);
#endif // HAVE_NODEKITS
  this->searchaction->apply(root);
#ifdef HAVE_NODEKITS
  SoBaseKit::setSearchingChildren(old);
#endif // HAVE_NODEKITS
  SoFullPath * path = (SoFullPath*) this->searchaction->getPath();
  if (path) {
    SoNode * tail = path->getTail();
    this->searchaction->reset();
    return (SoCamera*) tail;
  }
  return defcamera;
}

void 
SoSceneManagerP::renderCB(void * userdata, class SoRenderManager * COIN_UNUSED_ARG(mgr))
{
  SoSceneManagerP * thisp = (SoSceneManagerP *) userdata;
  assert(thisp);
  if (thisp->rendercb) {
    thisp->rendercb(thisp->rendercbdata, PUBLIC(thisp));
  }
}

#undef PRIVATE
#undef PUBLIC
