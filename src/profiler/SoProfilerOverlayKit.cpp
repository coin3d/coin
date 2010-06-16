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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#ifdef HAVE_NODEKITS

#include <Inventor/annex/Profiler/nodekits/SoProfilerOverlayKit.h>
#include "coindefs.h"

#include <Inventor/system/gl.h>
#include <Inventor/SbVec2s.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/actions/SoAction.h>
#include <Inventor/nodes/SoCallback.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/annex/Profiler/nodes/SoProfilerStats.h>
#include <Inventor/elements/SoViewportRegionElement.h>

#include "nodekits/SoSubKitP.h"

namespace {
  void enableDepthTest(void * COIN_UNUSED_ARG(userdata), SoAction * COIN_UNUSED_ARG(action))
  {
    glEnable(GL_DEPTH_TEST);
  }

  void disableDepthTest(void * COIN_UNUSED_ARG(userdata), SoAction * COIN_UNUSED_ARG(action))
  {
    glDisable(GL_DEPTH_TEST);
  }

  void grabViewportInfo(void * userdata, SoAction * action)
  {
    SoState * state = action->getState();
    const int eltindex = SoViewportRegionElement::getClassStackIndex();
    if (state->isElementEnabled(eltindex)) {
      const SbViewportRegion & vp = SoViewportRegionElement::get(state);
      SoProfilerOverlayKit * kit = static_cast<SoProfilerOverlayKit *>(userdata);
      SbVec2s pixels = vp.getViewportSizePixels();
      kit->viewportSize = SbVec3f(float(pixels[0]), float(pixels[1]), 0.0f);
    }
  }
};

struct SoProfilerOverlayKitP
{
};

#define PRIVATE(obj) ((obj)->pimpl)

SO_KIT_SOURCE(SoProfilerOverlayKit);

// Doc in superclass.
void
SoProfilerOverlayKit::initClass(void)
{
  SO_KIT_INTERNAL_INIT_CLASS(SoProfilerOverlayKit, SO_FROM_COIN_3_0);
}

/*!
  Constructor.
 */
SoProfilerOverlayKit::SoProfilerOverlayKit(void)
{
  SO_KIT_INTERNAL_CONSTRUCTOR(SoProfilerOverlayKit);
  SO_KIT_ADD_CATALOG_ENTRY(topSeparator, SoSeparator, TRUE, this, "", FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(profilingStats, SoProfilerStats, FALSE,
                           topSeparator, viewportInfo, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(viewportInfo, SoCallback, TRUE, topSeparator,
                           overlayCamera, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(overlayCamera, SoOrthographicCamera, TRUE,
                           topSeparator, depthTestOff, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(depthTestOff, SoCallback, TRUE, topSeparator,
                           overlaySep, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(overlaySep, SoSeparator, TRUE, topSeparator,
                           depthTestOn, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(depthTestOn, SoCallback, TRUE, topSeparator, "",
                           FALSE);

  SO_KIT_INIT_INSTANCE();

  SO_KIT_ADD_FIELD(viewportSize, (SbVec3f(100.0f, 100.0f, 0.0f)));

  SoOrthographicCamera * camera =
    static_cast<SoOrthographicCamera *>(this->getAnyPart("overlayCamera", TRUE));
  camera->viewportMapping = SoCamera::LEAVE_ALONE;

  SoCallback * viewportCB =
    static_cast<SoCallback *>(this->getAnyPart("viewportInfo", TRUE));
  assert(viewportCB &&
         (viewportCB->getTypeId() == SoCallback::getClassTypeId()));
  viewportCB->setCallback(grabViewportInfo, this);

  SoCallback * beforeCB =
    static_cast<SoCallback *>(this->getAnyPart("depthTestOff", TRUE));
  beforeCB->setCallback(disableDepthTest);
  SoCallback * afterCB =
    static_cast<SoCallback *>(this->getAnyPart("depthTestOn", TRUE));
  afterCB->setCallback(enableDepthTest);
}

/*!
  Desctructor.
 */
SoProfilerOverlayKit::~SoProfilerOverlayKit(void)
{
}

/*!
  Display profiling scenegraph on top of the regular viewport.

  \param node Scene graph to display
 */
void
SoProfilerOverlayKit::addOverlayGeometry(SoNode * node)
{
  SoNode * sep = this->getAnyPart("overlaySep", TRUE);
  assert(sep->isOfType(SoGroup::getClassTypeId()));
  static_cast<SoGroup *>(sep)->addChild(node);
}

#undef PRIVATE

#endif // HAVE_NODEKITS
