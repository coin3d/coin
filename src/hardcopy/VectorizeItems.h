#ifndef COIN_SOVECTORIZEITEMS_H
#define COIN_SOVECTORIZEITEMS_H

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

// Some simple classes to store vectorized items. Inlined and with
// public data members.

#include <Inventor/SbBasic.h>
#include <Inventor/SbName.h>
#include <Inventor/SbVec2f.h>
#include <Inventor/SbVec2s.h>

class SoVectorizeItem {
public:
  SoVectorizeItem() {
    this->type = UNDEFINED;
    this->depth = 0.0f;
  }
  // quick and easy type system
  enum Type {
    UNDEFINED,
    LINE,
    TRIANGLE,
    TEXT,
    POINT,
    IMAGE
  };
  int type;
  float depth; // for depth sorting
};

class SoVectorizePoint : public SoVectorizeItem {
public:
  SoVectorizePoint(void) {
    this->type = POINT;
    this->size = 1.0f;
  }
  int vidx;       // index to BSPtree coordinate
  float size;     // Coin size (pixels)
  uint32_t col;
};

class SoVectorizeTriangle : public SoVectorizeItem {
public:
  SoVectorizeTriangle(void) {
    this->type = TRIANGLE;
  }
  int vidx[3];      // indices to BSPtree coordinates
  uint32_t col[3];
};

class SoVectorizeLine : public SoVectorizeItem {
public:
  SoVectorizeLine(void) {
    this->type = LINE;
    this->pattern = 0xffff;
    this->width = 1.0f;
  }
  int vidx[2];       // indices to BSPtree coordinates
  uint32_t col[2];
  uint16_t pattern;  // Coin line pattern
  float width;       // Coin line width (pixels)
};

class SoVectorizeText : public SoVectorizeItem {
public:
  SoVectorizeText(void) {
    this->type = TEXT;
  }

  enum Justification {
    LEFT,
    RIGHT,
    CENTER
  };

  SbName fontname;
  float fontsize;    // size in normalized coordinates
  SbString string;
  SbVec2f pos;       // pos in normalized coordinates
  uint32_t col;
  Justification justification;
};

class SoVectorizeImage : public SoVectorizeItem {
public:
  SoVectorizeImage(void) {
    this->type = IMAGE;
  }

  SbVec2f pos;        // pos in normalized coordinates
  SbVec2f size;       // size in normalized coordinates

  struct Image {
    const unsigned char * data;
    SbVec2s size;
    int nc;
  } image;
};

#endif // COIN_SOVECTORIZEITEMS_H
