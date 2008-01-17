#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#ifdef HAVE_SCENE_PROFILING

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2008 by Kongsberg SIM.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg SIM about acquiring
 *  a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg SIM, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#include <Inventor/annex/Profiler/nodekits/SoScrollingGraphKit.h>

#include <stdlib.h>
#include <stdio.h>

#include <boost/scoped_ptr.hpp>
#include <boost/scoped_array.hpp>
#include <boost/intrusive_ptr.hpp>

#include <Inventor/SbTime.h>
#include <Inventor/SbColor.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoBaseColor.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoScale.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>

#include "misc/SbHash.h"
#include "nodekits/SoSubKitP.h"

// *************************************************************************

//namespace {

class Graph {
public:
  SbName key;
  int index;
  SbColor color;
};

class Datum {
public:
  Datum(void) : next(NULL) { }
  SbTime when;
  SbList<float> datum;
  Datum * next;
}; // Datum

//} // anonymous

// *************************************************************************

class SoScrollingGraphKitP {
public:
  SoScrollingGraphKitP(void) : kit(NULL), first(NULL), last(NULL) { }
  ~SoScrollingGraphKitP(void) {
    SbList<const char *> keys;
    this->graphs.makeKeyList(keys);
    for (int c = 0; c < keys.getLength(); ++c) {
      Graph * obj = NULL;
      if (this->graphs.get(keys[c], obj)) { delete obj; }
    }
    this->graphs.clear();

    while (this->first) {
      Datum * datum = this->first;
      this->first = this->first->next;
      delete datum;
    }
  }

  boost::intrusive_ptr<SoSeparator> chart;
  boost::scoped_ptr<SoFieldSensor> addValuesSensor;

  void pullStatistics(void);
  Graph * getGraph(const SbName & key);
  Graph * getGraph(int idx);
  void addDatum(Datum * newDatum);

  void generateLineChart(void);
  void generateStackedBarsChart(void);

  SbHash<Graph *, const char *> graphs;

  SoScrollingGraphKit * kit;
  Datum * first;
  Datum * last;
}; // SoScrollingGraphKitP

// *************************************************************************

#define PRIVATE(obj) ((obj)->pimpl)

SO_KIT_SOURCE(SoScrollingGraphKit);

void
SoScrollingGraphKit::initClass(void)
{
  SO_KIT_INIT_CLASS(SoScrollingGraphKit, SoBaseKit, "BaseKit");
}

SoScrollingGraphKit::SoScrollingGraphKit(void)
{
  PRIVATE(this)->kit = this;

  SO_KIT_INTERNAL_CONSTRUCTOR(SoScrollingGraphKit);
  SO_KIT_ADD_CATALOG_ENTRY(scene, SoSeparator, TRUE, this, "", FALSE);
  SO_KIT_INIT_INSTANCE();

  SO_KIT_DEFINE_ENUM_VALUE(GraphicsType, LINES);
  SO_KIT_DEFINE_ENUM_VALUE(GraphicsType, STACKED_BARS);
  SO_KIT_DEFINE_ENUM_VALUE(GraphicsType, DEFAULT_GRAPHICS);
  SO_KIT_SET_SF_ENUM_TYPE(graphicsType, GraphicsType);

  SO_KIT_DEFINE_ENUM_VALUE(RangeType, ABSOLUTE_ACCUMULATIVE);
  //SO_KIT_DEFINE_ENUM_VALUE(RangeType, ABSOLUTE_OVERWRITE);
  //SO_KIT_DEFINE_ENUM_VALUE(RangeType, RELATIVE_ACCUMULATIVE);
  //SO_KIT_DEFINE_ENUM_VALUE(RangeType, RELATIVE_OVERWRITE);
  SO_KIT_DEFINE_ENUM_VALUE(RangeType, DEFAULT_RANGETYPE);
  SO_KIT_SET_SF_ENUM_TYPE(rangeType, RangeType);

  SO_KIT_ADD_FIELD(graphicsType, (DEFAULT_GRAPHICS));
  SO_KIT_ADD_FIELD(rangeType, (DEFAULT_RANGETYPE));
  SO_KIT_ADD_FIELD(seconds, (SbTime(5.0)));
  SO_KIT_ADD_FIELD(colors, (SbColor(0.0f, 0.0f, 0.0f)));
  SbColor default_colors[] = {
    SbColor(1.0f, 0.0f, 0.0f),
    SbColor(0.0f, 1.0f, 0.0f),
    SbColor(0.0f, 0.0f, 1.0f),
    SbColor(1.0f, 0.0f, 1.0f),
    SbColor(1.0f, 1.0f, 0.0f),
    SbColor(0.0f, 1.0f, 1.0f)
  };
  const int numdefaultcolors =
    sizeof(default_colors) / sizeof(default_colors[0]);
  this->colors.setNum(numdefaultcolors);
  this->colors.setValues(0, numdefaultcolors, default_colors);
  this->colors.setDefault(TRUE);

  SO_KIT_ADD_FIELD(viewportSize, (SbVec3f(512.0f, 512.0f, 0.0f)));
  SO_KIT_ADD_FIELD(position, (SbVec3f(4.0f, 4.0f, 0.0f)));
  SO_KIT_ADD_FIELD(size, (SbVec3f(256.0f, 100.0f, 0.0f)));

  SO_KIT_ADD_FIELD(addKeys, (SbName::empty()));
  this->addKeys.setNum(0);
  this->addKeys.setDefault(TRUE);
  SO_KIT_ADD_FIELD(addValues, (0.0f));
  this->addValues.setNum(0);
  this->addValues.setDefault(TRUE);

  PRIVATE(this)->addValuesSensor.reset(new SoFieldSensor);
  PRIVATE(this)->addValuesSensor->setFunction(SoScrollingGraphKit::addValuesCB);
  PRIVATE(this)->addValuesSensor->setData(this);
  PRIVATE(this)->addValuesSensor->attach(&(this->addValues));

  PRIVATE(this)->chart = static_cast<SoSeparator *>(this->getAnyPart("scene", TRUE));
}

SoScrollingGraphKit::~SoScrollingGraphKit(void)
{
}

void
SoScrollingGraphKit::addValuesCB(void * closure, SoSensor * sensor)
{
  SoScrollingGraphKit * kit = static_cast<SoScrollingGraphKit *>(closure);

  PRIVATE(kit)->pullStatistics();
  switch (kit->graphicsType.getValue()) {
  case SoScrollingGraphKit::LINES:
    PRIVATE(kit)->generateLineChart();
    break;
  case SoScrollingGraphKit::STACKED_BARS:
    PRIVATE(kit)->generateStackedBarsChart();
    break;
  default:
    PRIVATE(kit)->generateLineChart();
    break;
  }
}

void
SoScrollingGraphKitP::pullStatistics(void)
{
  SbTime now(SbTime::getTimeOfDay());
  const int numvalues = kit->addValues.getNum();
  if (kit->addKeys.getNum() != numvalues) {
    SoDebugError::post("SoScrollingGraphKitP::pullStatistics",
                       "data out of sync (%d keys, %d values)",
                       numvalues, kit->addKeys.getNum());
  }

  Datum * datum = new Datum;
  datum->when = now;

  int c;
  for (c = 0; c < numvalues; ++c) { // make sure all the grraph structs needed are created
    SbName key = kit->addKeys[c];
    Graph * graph = this->getGraph(key);
  }
  const int numgraphs = this->graphs.getNumElements();
  for (c = 0; c < numgraphs; ++c) { // initialize default for all graphs
    datum->datum.append(0.0f);
  }
  for (c = 0; c < numvalues; ++c) { // fill values for the graphs we have data for
    SbName key = kit->addKeys[c];
    Graph * graph = this->getGraph(key);
    datum->datum[graph->index] = kit->addValues[c];
  }
  this->addDatum(datum);
}

void
SoScrollingGraphKitP::addDatum(Datum * newDatum)
{
  if (this->first == NULL) {
    this->first = this->last = newDatum;
  } else {
    this->last->next = newDatum;
    this->last = newDatum;
  }

  SbTime maxtime(this->kit->seconds.getValue());
  while ((this->first->next != NULL) &&
         ((newDatum->when - this->first->next->when) > maxtime)) {
    Datum * datum = this->first;
    this->first = this->first->next;
    delete datum;
  }
}

Graph *
SoScrollingGraphKitP::getGraph(const SbName & key)
{
  assert(key != SbName::empty());
  Graph * graph = NULL;
  if (!this->graphs.get(key.getString(), graph)) {
    graph = new Graph;
    graph->key = key;
    graph->index = this->graphs.getNumElements();
    graph->color = this->kit->colors[graph->index % this->kit->colors.getNum()];
    this->graphs.put(key.getString(), graph);

    printf("Adding graph #%d for '%s', color #%02x%02x%02x\n",
           graph->index + 1, key.getString(), (uint8_t)(graph->color[0] * 255.0f),
           (uint8_t) (graph->color[1] * 255.0), (uint8_t) (graph->color[2] * 255.0));
  }
  assert(graph);
  return graph;
}

Graph *
SoScrollingGraphKitP::getGraph(int idx)
{
  SbList<const char *> keys;
  this->graphs.makeKeyList(keys);
  for (int i = 0; i < keys.getLength(); ++i) {
    Graph * graph = NULL;
    this->graphs.get(keys[i], graph);
    if (graph->index == idx) return graph;
  }
  assert(!"serious problem - did not find graph index data");
  return NULL;
}

void
SoScrollingGraphKitP::generateLineChart(void)
{
  const int numgraphs = this->graphs.getNumElements();

  boost::scoped_array<SoBaseColor *> colors(new SoBaseColor * [numgraphs]);
  boost::scoped_array<SoCoordinate3 *> coords(new SoCoordinate3 * [numgraphs]);
  boost::scoped_array<SoLineSet *> lines(new SoLineSet * [numgraphs]);

  SoScale * scale = NULL;
  SoTranslation * translation = NULL;

  if (this->chart->getNumChildren() != (numgraphs * 3 + 2) ||
      !(this->chart->getChild(2+2)->isOfType(SoLineSet::getClassTypeId()))) {
    this->chart->removeAllChildren();

    this->chart->addChild(translation = new SoTranslation);
    this->chart->addChild(scale = new SoScale);

    translation->translation.setValue(-1.0f, -0.99f, 0.0f);
    scale->scaleFactor.setValue(2.0f, 0.5f, 1.0f);

    for (int c = 0; c < numgraphs; ++c) {
      this->chart->addChild(colors[c] = new SoBaseColor);
      this->chart->addChild(coords[c] = new SoCoordinate3);
      this->chart->addChild(lines[c] = new SoLineSet);
    }
  } else {
    //scale = (SoScale *) this->chart->getChild(1);
    //assert(scale && scale->isOfType(SoScale::getClassTypeId()));
    //translation = (SoTranslation *) this->chart->getChild(0);
    //assert(translation && translation->isOfType(SoTranslation::getClassTypeId()));
    for (int c = 0; c < numgraphs; ++c) {
      colors[c] = static_cast<SoBaseColor *>(this->chart->getChild(2 + c * 3 + 0));
      assert(colors[c]->isOfType(SoBaseColor::getClassTypeId()));
      coords[c] = static_cast<SoCoordinate3 *>(this->chart->getChild(2 + c * 3 + 1));
      assert(coords[c]->isOfType(SoCoordinate3::getClassTypeId()));
      lines[c] = static_cast<SoLineSet *>(this->chart->getChild(2 + c * 3 + 2));
      assert(lines[c]->isOfType(SoLineSet::getClassTypeId()));
    }
  }

  int numdata = 0;
  SbTime end(SbTime::zero());
  Datum * datum = this->first;
  float maxvalue = 0.0f;
  while (datum) {
    ++numdata;
    float sum = 0.0f;
    for (int j = 0; j < datum->datum.getLength(); ++j) {
      sum += datum->datum[j];
    }
    if (sum > maxvalue) maxvalue = sum;
    end = datum->when;
    datum = datum->next;
  }
  // printf("values: %d, maxvalue: %f\n", numdata, maxvalue);

  // printf("num data values: %d\n", numdata);
  const float seconds = (float) this->kit->seconds.getValue().getValue();

  for (int graphidx = 0; graphidx < numgraphs; ++graphidx) {
    Graph * graph = this->getGraph(graphidx);

    colors[graphidx]->rgb = graph->color;

    coords[graphidx]->point.setNum(numdata);
    SbVec3f * coordarray = coords[graphidx]->point.startEditing();

    datum = this->first;
    int idx = 0;
    while (datum) {
      const int numvalues = datum->datum.getLength();

      float value = 0.0f;
      for (int j = 0; j <= graph->index && j < numvalues; ++j) {
        value += datum->datum[j];
      }

      const float xpos = 1.0f - ((float)((end - datum->when).getValue()) / seconds);
      const float ypos = (maxvalue > 0.0f) ? (value / maxvalue) : value;

      //printf("coord %d: %f %f\n", idx, xpos, ypos);
      coordarray[idx] = SbVec3f(xpos, ypos, 0.0f);

      ++idx;
      datum = datum->next;
    }
    coords[graphidx]->point.finishEditing();
    
    lines[graphidx]->numVertices.setNum(1);
    lines[graphidx]->numVertices.set1Value(0, numdata);
  }

  // graph space is now between 0-1 in both directions

}












void
SoScrollingGraphKitP::generateStackedBarsChart(void)
{
  const int numgraphs = this->graphs.getNumElements();
  if (numgraphs == 0)
    return ;

  boost::scoped_array<SoBaseColor *> colors(new SoBaseColor * [numgraphs]);
  boost::scoped_array<SoCoordinate3 *> coords(new SoCoordinate3 * [numgraphs]);
  boost::scoped_array<SoLineSet *> lines(new SoLineSet * [numgraphs]);

  SoScale * scale = NULL;
  SoTranslation * translation = NULL;

  if (this->chart->getNumChildren() != (numgraphs * 3 + 2) ||
      !(this->chart->getChild(2+2)->isOfType(SoLineSet::getClassTypeId()))) {
    this->chart->removeAllChildren();

    this->chart->addChild(translation = new SoTranslation);
    this->chart->addChild(scale = new SoScale);

    translation->translation.setValue(-1.0f, -0.99f, 0.0f);
    scale->scaleFactor.setValue(2.0f, 0.5f, 1.0f);

    for (int c = 0; c < numgraphs; ++c) {
      this->chart->addChild(colors[c] = new SoBaseColor);
      this->chart->addChild(coords[c] = new SoCoordinate3);
      this->chart->addChild(lines[c] = new SoLineSet);
    }
  } else {
    //scale = (SoScale *) this->chart->getChild(1);
    //assert(scale && scale->isOfType(SoScale::getClassTypeId()));
    //translation = (SoTranslation *) this->chart->getChild(0);
    //assert(translation && translation->isOfType(SoTranslation::getClassTypeId()));
    for (int c = 0; c < numgraphs; ++c) {
      colors[c] = static_cast<SoBaseColor *>(this->chart->getChild(2 + c * 3 + 0));
      assert(colors[c]->isOfType(SoBaseColor::getClassTypeId()));
      coords[c] = static_cast<SoCoordinate3 *>(this->chart->getChild(2 + c * 3 + 1));
      assert(coords[c]->isOfType(SoCoordinate3::getClassTypeId()));
      lines[c] = static_cast<SoLineSet *>(this->chart->getChild(2 + c * 3 + 2));
      assert(lines[c]->isOfType(SoLineSet::getClassTypeId()));
    }
  }

  int numdata = 0;
  SbTime end(SbTime::zero());
  Datum * datum = this->first;
  float maxvalue = 0.0f;
  while (datum) {
    ++numdata;
    float sum = 0.0f;
    for (int j = 0; j < datum->datum.getLength(); ++j) {
      sum += datum->datum[j];
    }
    if (sum > maxvalue) maxvalue = sum;
    end = datum->when;
    datum = datum->next;
  }
  // printf("values: %d, maxvalue: %f\n", numdata, maxvalue);

  // printf("num data values: %d\n", numdata);
  const float seconds = (float) this->kit->seconds.getValue().getValue();

  for (int graphidx = 0; graphidx < numgraphs; ++graphidx) {
    Graph * graph = this->getGraph(graphidx);

    colors[graphidx]->rgb = graph->color;

    coords[graphidx]->point.setNum(numdata * 2);
    SbVec3f * coordarray = coords[graphidx]->point.startEditing();

    datum = this->first;
    int idx = 0;
    while (datum) {
      const int numvalues = datum->datum.getLength();

      float value = 0.0f, prev = 0.0f;
      for (int j = 0; j <= graph->index && j < numvalues; ++j) {
        prev = value;
        value += datum->datum[j];
      }

      const float xpos = 1.0f - ((float)((end - datum->when).getValue()) / seconds);
      const float ypos = (maxvalue > 0.0f) ? (value / maxvalue) : value;
      const float prevypos = (maxvalue > 0.0f) ? (prev / maxvalue) : prev;

      //printf("coord %d: %f %f\n", idx, xpos, ypos);
      coordarray[idx*2] = SbVec3f(xpos, prevypos, 0.0f);
      coordarray[idx*2+1] = SbVec3f(xpos, ypos, 0.0f);

      ++idx;
      datum = datum->next;
    }
    coords[graphidx]->point.finishEditing();
    
    lines[graphidx]->numVertices.setNum(idx);
    int32_t * countarray = lines[graphidx]->numVertices.startEditing();
    for (int j = 0; j < idx; ++j) {
      countarray[j] = 2;
    }
    lines[graphidx]->numVertices.finishEditing();
  }

  // graph space is now between 0-1 in both directions

}

// *************************************************************************

#endif // HAVE_SCENE_PROFILING
