/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
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
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#include "soshape_primdata.h"
#include <Inventor/actions/SoAction.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/details/SoFaceDetail.h>
#include <Inventor/details/SoLineDetail.h>
#include <Inventor/details/SoPointDetail.h>
#include <Inventor/SbTesselator.h>
#include <Inventor/elements/SoShapeHintsElement.h>
#include <string.h>

soshape_primdata::soshape_primdata(void)
{
  this->counter = 0;
  this->action = NULL;
  this->shape = NULL;
  this->faceCounter = 0;
  this->arraySize = 4;
  this->vertsArray = new SoPrimitiveVertex[this->arraySize];
  this->pointDetails = new SoPointDetail[this->arraySize];
  this->tess = new SbTesselator();
  this->faceDetail = NULL;
  this->lineDetail = NULL;
}

soshape_primdata::~soshape_primdata()
{
  delete[] this->vertsArray;
  delete[] this->pointDetails;
  delete this->tess;
}

void
soshape_primdata::beginShape(SoShape * shape, SoAction * action,
                               SoShape::TriangleShape shapetype,
                               SoDetail * detail)
{
  this->shape = shape;
  this->action = action;
  this->shapetype = shapetype;
  // this is a hack. Only one of these will be used, and the
  // other one is an illegal cast.
  this->faceDetail = (SoFaceDetail *)detail;
  this->lineDetail = (SoLineDetail *)detail;
  this->counter = 0;
}

void
soshape_primdata::endShape(void)
{
  if (this->shapetype == SoShape::POLYGON) {
    this->handleFaceDetail(this->counter);

    if (SoShapeHintsElement::getFaceType(action->getState()) ==
        SoShapeHintsElement::CONVEX) {
      for (int i = 1; i < this->counter-1; i++) {
        this->shape->invokeTriangleCallbacks(this->action,
                                             &vertsArray[0],
                                             &vertsArray[i],
                                             &vertsArray[i+1]);
      }
    }
    else {
      this->tess->setCallback(soshape_primdata::tess_callback, this);
      this->tess->beginPolygon(TRUE);
      for (int i = 0; i < counter; i++) {
        this->tess->addVertex(vertsArray[i].getPoint(), &vertsArray[i]);
      }
      this->tess->endPolygon();
    }
  }
}

void
soshape_primdata::shapeVertex(const SoPrimitiveVertex * const v)
{
  switch (shapetype) {
  case SoShape::TRIANGLE_STRIP:
    if (this->counter >= 3) {
      if (this->counter & 1) this->copyVertex(2, 0);
      else this->copyVertex(2, 1);
    }
    this->setVertex(SbMin(this->counter, 2), v);
    this->counter++;
    if (this->counter >= 3) {
      this->handleFaceDetail(3);
      this->shape->invokeTriangleCallbacks(this->action,
                                           &vertsArray[0],
                                           &vertsArray[1],
                                           &vertsArray[2]);
    }
    break;
  case SoShape::TRIANGLE_FAN:
    if (this->counter == 3) {
      this->copyVertex(2, 1);
      this->setVertex(2, v);
    }
    else {
      this->setVertex(this->counter++, v);
    }
    if (this->counter == 3) {
      this->handleFaceDetail(3);
      this->shape->invokeTriangleCallbacks(this->action,
                                           &vertsArray[0],
                                           &vertsArray[1],
                                           &vertsArray[2]);
    }
    break;
  case SoShape::TRIANGLES:
    this->setVertex(counter++, v);
    if (this->counter == 3) {
      this->handleFaceDetail(3);
      this->shape->invokeTriangleCallbacks(this->action,
                                           &vertsArray[0],
                                           &vertsArray[1],
                                           &vertsArray[2]);
      this->counter = 0;
    }
    break;
  case SoShape::POLYGON:
    if (this->counter >= this->arraySize) {
      this->arraySize <<= 1;
      SoPrimitiveVertex * newArray = new SoPrimitiveVertex[this->arraySize];
      memcpy(newArray, this->vertsArray,
             sizeof(SoPrimitiveVertex)* this->counter);
      delete [] this->vertsArray;
      this->vertsArray = newArray;

      SoPointDetail * newparray = new SoPointDetail[this->arraySize];
      memcpy(newparray, this->pointDetails,
             sizeof(SoPointDetail)* this->counter);
      delete [] this->pointDetails;
      this->pointDetails = newparray;

      if (this->faceDetail) {
        for (int i = 0; i < this->counter; i++) {
          this->vertsArray[i].setDetail(&this->pointDetails[i]);
        }
      }
    }
    this->setVertex(this->counter++, v);
    break;
  case SoShape::QUADS:
    this->setVertex(this->counter++, v);
    if (this->counter == 4) {
      this->handleFaceDetail(4);
      this->shape->invokeTriangleCallbacks(this->action,
                                           &vertsArray[0],
                                           &vertsArray[1],
                                           &vertsArray[2]);
      this->shape->invokeTriangleCallbacks(this->action,
                                           &vertsArray[0],
                                           &vertsArray[2],
                                           &vertsArray[3]);
      this->counter = 0;
    }
    break;
  case SoShape::QUAD_STRIP:
    this->setVertex(this->counter++, v);
    if (counter == 4) {
      // can't use handleFaceDetail(), because of the vertex
      // order.
      if (this->faceDetail) {
        this->faceDetail->setNumPoints(4);
        this->faceDetail->setPoint(0, &this->pointDetails[0]);
        this->vertsArray[0].setDetail(this->faceDetail);
        this->faceDetail->setPoint(1, &this->pointDetails[1]);
        this->vertsArray[1].setDetail(this->faceDetail);
        this->faceDetail->setPoint(2, &this->pointDetails[3]);
        this->vertsArray[2].setDetail(this->faceDetail);
        this->faceDetail->setPoint(3, &this->pointDetails[2]);
        this->vertsArray[3].setDetail(this->faceDetail);
      }
      this->shape->invokeTriangleCallbacks(this->action,
                                           &vertsArray[0],
                                           &vertsArray[1],
                                           &vertsArray[3]);
      this->shape->invokeTriangleCallbacks(this->action,
                                           &vertsArray[0],
                                           &vertsArray[3],
                                           &vertsArray[2]);
      this->copyVertex(2, 0);
      this->copyVertex(3, 1);
      this->counter = 2;
    }
    break;
  case SoShape::POINTS:
    this->shape->invokePointCallbacks(this->action, v);
    break;
  case SoShape::LINES:
    this->setVertex(this->counter++, v);
    if (this->counter == 2) {
      this->handleLineDetail();
      this->shape->invokeLineSegmentCallbacks(this->action,
                                              &vertsArray[0],
                                              &vertsArray[1]);
      this->counter = 0;
    }
    break;
  case SoShape::LINE_STRIP:
    this->setVertex(this->counter++, v);
    if (this->counter == 2) {
      this->handleLineDetail();
      this->shape->invokeLineSegmentCallbacks(this->action,
                                              &vertsArray[0],
                                              &vertsArray[1]);
      this->copyVertex(1, 0);
      this->counter = 1;
    }
    break;
  default:
    assert(0 && "Unknown shape type");
  }
}

void
soshape_primdata::copyVertex(const int src, const int dest)
{
  this->vertsArray[dest] = this->vertsArray[src];
  if (this->faceDetail) {
    this->pointDetails[dest] = this->pointDetails[src];
    this->vertsArray[dest].setDetail(&this->pointDetails[dest]);
  }
}

void
soshape_primdata::setVertex(const int idx, const SoPrimitiveVertex * const v)
{
  this->vertsArray[idx] = *v;
  if (this->faceDetail) {
    SoPointDetail * pd = (SoPointDetail *)v->getDetail();
    assert(pd);
    this->pointDetails[idx] = * pd;
    this->vertsArray[idx].setDetail(&this->pointDetails[idx]);
    }
}

void
soshape_primdata::handleFaceDetail(const int numv)
{
  if (this->faceDetail) {
    this->faceDetail->setNumPoints(numv);
    for (int i = 0; i < numv; i++) {
      this->faceDetail->setPoint(i, &this->pointDetails[i]);
        this->vertsArray[i].setDetail(this->faceDetail);
    }
  }
}

void
soshape_primdata::handleLineDetail(void)
{
  if (this->lineDetail) {
    this->lineDetail->setPoint0(&this->pointDetails[0]);
    this->lineDetail->setPoint1(&this->pointDetails[1]);
  }
}

SoDetail *
soshape_primdata::createPickDetail(void)
{
  switch (this->shapetype) {
  case SoShape::TRIANGLE_STRIP:
  case SoShape::TRIANGLE_FAN:
  case SoShape::TRIANGLES:
    {
      SoFaceDetail * detail = (SoFaceDetail *)this->faceDetail->copy();
      detail->setNumPoints(3);
      detail->setPoint(0, &this->pointDetails[0]);
      detail->setPoint(1, &this->pointDetails[1]);
      detail->setPoint(2, &this->pointDetails[2]);
      return detail;
    }
  case SoShape::POLYGON:
    {
      SoFaceDetail * detail = (SoFaceDetail *)this->faceDetail->copy();
      detail->setNumPoints(this->counter);
      for (int i = 0; i < this->counter; i++) {
        detail->setPoint(i, &this->pointDetails[i]);
      }
      return detail;
    }
  case SoShape::QUADS:
  case SoShape::QUAD_STRIP:
    {
      SoFaceDetail * detail = (SoFaceDetail *)this->faceDetail->copy();
      detail->setNumPoints(4);
      detail->setPoint(0, &this->pointDetails[0]);
      detail->setPoint(1, &this->pointDetails[1]);
      detail->setPoint(2, &this->pointDetails[2]);
      detail->setPoint(3, &this->pointDetails[3]);
      return detail;
    }
  case SoShape::POINTS:
    {
      assert(0 && "should not get here");
      return NULL;
    }
  case SoShape::LINES:
  case SoShape::LINE_STRIP:
    {
      SoLineDetail * detail = (SoLineDetail *)this->lineDetail->copy();
      detail->setPoint0(&this->pointDetails[0]);
      detail->setPoint1(&this->pointDetails[1]);
      return detail;
    }
  default:
    assert(0 && "unknown shape type");
    return NULL;
  }
}

void
soshape_primdata::tess_callback(void * v0, void * v1, void * v2, void * data)
{
  soshape_primdata * thisp = (soshape_primdata *) data;
  thisp->shape->invokeTriangleCallbacks(thisp->action,
                                        (SoPrimitiveVertex *)v0,
                                        (SoPrimitiveVertex *)v1,
                                        (SoPrimitiveVertex *)v2);
}
