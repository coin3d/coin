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

#include <Inventor/annex/Profiler/nodekits/SoProfilerTopKit.h>
#include "coindefs.h"

#include <boost/intrusive_ptr.hpp>

#include <Inventor/annex/Profiler/engines/SoProfilerTopEngine.h>
#include <Inventor/annex/Profiler/nodes/SoProfilerStats.h>
#include <Inventor/annex/Profiler/nodekits/SoScrollingGraphKit.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoBaseColor.h>
#include <Inventor/nodes/SoText2.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/engines/SoCalculator.h>

#include "nodekits/SoSubKitP.h"

#define PUBLIC(obj) ((obj)->master)
#define PRIVATE(obj) ((obj)->pimpl)

class SoProfilerTopKitP
{
public:
  SoProfilerTopKitP()
    : geometryEngine(NULL),
      topListEngine(NULL),
      last_stats(NULL)
  { }

  SoProfilerTopKit * master;

  static void statsNodeChanged(void *, SoSensor *);

  void detachFromStats();
  void attachToStats();

  boost::intrusive_ptr<SoCalculator> geometryEngine;
  boost::intrusive_ptr<SoProfilerTopEngine> topListEngine;
  SoProfilerStats * last_stats;
  SoFieldSensor * stats_sensor;
};

void
SoProfilerTopKitP::statsNodeChanged(void * userdata, SoSensor * COIN_UNUSED_ARG(sensor))
{
  SoProfilerTopKit * thisp = (SoProfilerTopKit *)userdata;

  SoProfilerStats * stats =
    (SoProfilerStats *)thisp->getPart("profilingStats", FALSE);
  if (PRIVATE(thisp)->last_stats != stats) {
    PRIVATE(thisp)->last_stats = stats;
    PRIVATE(thisp)->detachFromStats();
    PRIVATE(thisp)->attachToStats();
  }
}

void
SoProfilerTopKitP::detachFromStats()
{
  this->topListEngine->statisticsNames.disconnect();
  this->topListEngine->statisticsCounts.disconnect();
  this->topListEngine->statisticsTimings.disconnect();
  this->topListEngine->statisticsTimingsMax.disconnect();
  this->topListEngine->maxLines.disconnect();

  SoScrollingGraphKit * graph =
    (SoScrollingGraphKit *) PUBLIC(this)->getAnyPart("graph", TRUE);
  assert(graph && graph->isOfType(SoScrollingGraphKit::getClassTypeId()));
  graph->addKeys.disconnect();
  graph->addValues.disconnect();
}

void
SoProfilerTopKitP::attachToStats()
{
  SoProfilerStats * statsNode =
    (SoProfilerStats *)PUBLIC(this)->getPart("profilingStats",
                                             FALSE);
  if (statsNode == NULL)
    return ;

  this->topListEngine->statisticsNames.connectFrom(&statsNode->renderedNodeType);
  this->topListEngine->statisticsCounts.connectFrom(&statsNode->renderedNodeTypeCount);
  this->topListEngine->statisticsTimings.connectFrom(&statsNode->renderingTimePerNodeType);
  this->topListEngine->statisticsTimingsMax.connectFrom(&statsNode->renderingTimeMaxPerNodeType);
  this->topListEngine->maxLines.connectFrom(&PUBLIC(this)->lines);


  SoScrollingGraphKit * graph =
    (SoScrollingGraphKit *) PUBLIC(this)->getAnyPart("graph", TRUE);
  assert(graph && graph->isOfType(SoScrollingGraphKit::getClassTypeId()));
  graph->addKeys.connectFrom(&statsNode->profiledAction);
  graph->addValues.connectFrom(&statsNode->profiledActionTime);
}


SO_KIT_SOURCE(SoProfilerTopKit);

void SoProfilerTopKit::initClass()
{
  SO_KIT_INTERNAL_INIT_CLASS(SoProfilerTopKit, SO_FROM_COIN_3_0);
}

SoProfilerTopKit::SoProfilerTopKit(void)
{
  PRIVATE(this)->master = this;

  SO_KIT_INTERNAL_CONSTRUCTOR(SoProfilerTopKit);
  SO_KIT_ADD_CATALOG_ENTRY(textSep, SoSeparator, TRUE, overlaySep, graph, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(color, SoBaseColor, TRUE, textSep, translation, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(translation, SoTranslation, TRUE, textSep, text, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(text, SoText2, TRUE, textSep, graph, FALSE);

  SO_KIT_ADD_CATALOG_ENTRY(graph, SoScrollingGraphKit, TRUE, overlaySep, "", FALSE);

  SO_KIT_ADD_FIELD(txtColor, (1.0, 1.0, 1.0));
  SO_KIT_ADD_FIELD(lines, (16));

  SO_KIT_ADD_FIELD(topKitSize, (SbVec2f(100.0f, 100.0f)));
  SO_KIT_ADD_FIELD(position, (SbVec3f(4.0f, 4.0f, 0.0f)));

  SO_KIT_INIT_INSTANCE();

  PRIVATE(this)->topListEngine = new SoProfilerTopEngine;
  PRIVATE(this)->topListEngine->decay.setValue(0.99f);

  PRIVATE(this)->geometryEngine = new SoCalculator;

  const char * expr[] = {
    // A = viewportsize, B = wanted position
    // assuming translation from center (0,0), and flipped y-axis
    // trans = - (viewportsize_n / 2) + (pixelsize * position)
    "ta = -1.0 + ((2.0 / A[0]) * B[0])",
    "tb = 1.0 - ((2.0 / A[1]) * (B[1] + 10.0))",
    "oA = vec3f(ta, tb, -1)"
  };

  const int num = sizeof(expr) / sizeof(const char *);
  PRIVATE(this)->geometryEngine->expression.setNum(num);
  PRIVATE(this)->geometryEngine->expression.setValues(0, num, expr);

  PRIVATE(this)->geometryEngine->A.connectFrom(&viewportSize);
  PRIVATE(this)->geometryEngine->B.connectFrom(&position);

  SoTranslation * transe = (SoTranslation *) this->getAnyPart("translation",
                                                              TRUE);
  transe->translation.connectFrom(&(PRIVATE(this)->geometryEngine->oA));

  SoText2 * txt = (SoText2 *) this->getAnyPart("text", TRUE);
  txt->string.connectFrom(&PRIVATE(this)->topListEngine->prettyText);

  SoBaseColor * colorNd = (SoBaseColor *) this->getAnyPart("color", TRUE);
  colorNd->rgb.connectFrom(&this->txtColor);

  PRIVATE(this)->attachToStats();

  PRIVATE(this)->stats_sensor =
    new SoFieldSensor(PRIVATE(this)->statsNodeChanged, this);
  PRIVATE(this)->stats_sensor->attach(&this->profilingStats);
}

SoProfilerTopKit::~SoProfilerTopKit(void)
{
}

#undef PRIVATE
#undef PUBLIC
#endif // HAVE_NODEKITS
