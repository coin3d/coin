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

#include <Inventor/SbXfBox3f.h>
#include <Inventor/SoPath.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/lists/SbPList.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/nodes/SoShape.h>
#include <Inventor/draggers/SoDragger.h>
#include <Inventor/manips/SoTransformManip.h>
#include <Inventor/manips/SoClipPlaneManip.h>
#include <Inventor/manips/SoDirectionalLightManip.h>
#include <Inventor/manips/SoPointLightManip.h>
#include <Inventor/manips/SoSpotLightManip.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/collision/SoIntersectionDetectionAction.h>

#include "SbTri3f.ih"
#include "SbTri3f.icc"

// *************************************************************************

struct ShapeData;
struct PrimitiveData;

class SoIntersectionDetectionActionP {
public:
  SoIntersectionDetectionActionP(void);
  ~SoIntersectionDetectionActionP(void);

  static float staticepsilon;
  float epsilon;
  SbBool epsilonset;
  float getEpsilon(void) const;

  SbBool draggersenabled;
  SbBool manipsenabled;
  SbBool internalsenabled;

  SoIntersectionDetectionAction::SoIntersectionFilterCB * filtercb;
  void * filterclosure;
  SbPList * callbacks;
  SoCallbackAction * traverser;

  SoCallbackAction::Response shape(SoCallbackAction * action, SoShape * shape);
  static SoCallbackAction::Response shapeCB(void * closure, SoCallbackAction * action, const SoNode * node);
  SoCallbackAction::Response traverse(SoCallbackAction * action, const SoNode * node);
  static SoCallbackAction::Response traverseCB(void * closure, SoCallbackAction * action, const SoNode * node);
  SoCallbackAction::Response dragger(SoCallbackAction * action, const SoNode * node);
  static SoCallbackAction::Response draggerCB(void * closure, SoCallbackAction * action, const SoNode * node);
  static SoCallbackAction::Response pruneCB(void * closure, SoCallbackAction * action, const SoNode * node);

  static void triangleCB(void * closure, SoCallbackAction * action, const SoPrimitiveVertex * v1, const SoPrimitiveVertex * v2, const SoPrimitiveVertex * v3);
  static PrimitiveData * generatePrimitives(ShapeData * shape);
  static void deletePrimitives(PrimitiveData * primitives);

  void reset(void);
  void doIntersectionTesting(void);
  SbBool doPrimitiveIntersectionTesting(PrimitiveData * primitives1, PrimitiveData * primitives2);
  SbBool doInternalPrimitiveIntersectionTesting(PrimitiveData * primitives);

  SoTypeList * prunetypes;

  SoTypeList * traversaltypes;
  SbPList * traversalcallbacks;

  SbPList * shapedata;
};

float SoIntersectionDetectionActionP::staticepsilon = 0.0f;

SoIntersectionDetectionActionP::SoIntersectionDetectionActionP(void)
{
  this->epsilon = 0.0f;
  this->epsilonset = FALSE;
  this->draggersenabled = TRUE;
  this->manipsenabled = TRUE;
  this->internalsenabled = FALSE;
  this->filtercb = NULL;
  this->filterclosure = NULL;
  this->callbacks = new SbPList;
  this->traverser = NULL;
  this->shapedata = new SbPList;
  this->prunetypes = new SoTypeList;
  this->traversaltypes = new SoTypeList;
  this->traversalcallbacks = new SbPList;
}

SoIntersectionDetectionActionP::~SoIntersectionDetectionActionP(void)
{
  this->reset();
  delete this->callbacks;
  delete this->traverser;
  delete this->shapedata;
  delete this->prunetypes;
  delete this->traversaltypes;
  delete this->traversalcallbacks;
}

float
SoIntersectionDetectionActionP::getEpsilon(void) const
{
  if ( this->epsilonset ) return this->epsilon;
  return SoIntersectionDetectionActionP::staticepsilon;
}

// *************************************************************************

#define PRIVATE(obj) ((obj)->pimpl)

SO_ACTION_SOURCE(SoIntersectionDetectionAction);

/*!
  \class SoIntersectionDetectionAction Inventor/collision/SoIntersectionDetectionAction.h
  \brief The SoIntersectionDetectionAction class is for detecting intersecting
  primitives in a scene.
  \ingroup actions
  \ingroup collision

  Note that the implementation is somewhat incomplete.  The epsilon setting is
  not supported yet, only intersection testing between triangles is done, and
  the algorithm used is O(N^2) - no significant optimizations is done at this
  time.

  \since 20021022
*/

void
SoIntersectionDetectionAction::initClass(void)
{
  SO_ACTION_INIT_CLASS(SoIntersectionDetectionAction, SoAction);
}

SoIntersectionDetectionAction::SoIntersectionDetectionAction(void)
{
  PRIVATE(this) = new SoIntersectionDetectionActionP;
}

SoIntersectionDetectionAction::~SoIntersectionDetectionAction(void)
{
  delete PRIVATE(this);
}

/*!
  Sets the global intersection detection distance epsilon value.
  This will affect all intersection detection action objects in use that
  don't have a locally set value.

  The epsilon value is a worldspace value.

  Epsilon is not supported yet.
*/

void
SoIntersectionDetectionAction::setIntersectionEpsilon(float epsilon) // static
{
  SoIntersectionDetectionActionP::staticepsilon = epsilon;
}

/*!
  Returns the globally set intersection detection distance epsilon value.
*/

float
SoIntersectionDetectionAction::getIntersectionEpsilon(void) // static
{
  return SoIntersectionDetectionActionP::staticepsilon;
}

/*!
  Sets the intersection detection distance epsilon value for the action object.
  This overrides the global value.

  The epsilon value is a worldspace value.

  Epsilon is not supported yet.
*/

void
SoIntersectionDetectionAction::setIntersectionDetectionEpsilon(float epsilon)
{
  PRIVATE(this)->epsilon = epsilon;
  PRIVATE(this)->epsilonset = TRUE;
}

/*!
  Returns the set intersection detection distance epsilon value for the action object.
*/

float
SoIntersectionDetectionAction::getIntersectionDetectionEpsilon(void) const
{
  return PRIVATE(this)->getEpsilon();
}

/*!
  Sets whether nodes of specific types (including derived objects) should be tested
  for intersection or not.

  \sa isTypeEnabled(), setManipsEnabled(), setDraggersEnabled()
*/

void
SoIntersectionDetectionAction::setTypeEnabled(SoType type, SbBool enable)
{
  if ( enable ) {
    int idx = PRIVATE(this)->prunetypes->find(type);
    if ( idx >= 0 ) PRIVATE(this)->prunetypes->remove(idx);
  } else if ( PRIVATE(this)->prunetypes->find(type) ) {
    PRIVATE(this)->prunetypes->append(type);
  }
}

/*!
  Returns whether nodes of specific types are enabled or not.  The \a checkgroups
  argument can be set to TRUE if you wan't the return value to reflect whether
  the node will be implicit enabled/disabled through the settings controlled by the
  setManipsEnabled() and setDraggersEnabled() functions.

  The default is that all node types are enabled.

  Note that derivation checks are not performed - the type needs to be the exact
  same type as has been disabled with setTypeEnabled()

  \sa setTypeEnabled()
*/

SbBool
SoIntersectionDetectionAction::isTypeEnabled(SoType type, SbBool checkgroups) const
{
  if ( PRIVATE(this)->prunetypes->find(type) != -1 ) return FALSE;
  if ( checkgroups ) {
    // is type a dragger?
    if ( !PRIVATE(this)->draggersenabled &&
	 type.isDerivedFrom(SoDragger::getClassTypeId()) ) return FALSE;
    // is type a manip?
    if ( !PRIVATE(this)->manipsenabled ) {
      if ( type.isDerivedFrom(SoTransformManip::getClassTypeId()) ||
	   type.isDerivedFrom(SoClipPlaneManip::getClassTypeId()) ||
	   type.isDerivedFrom(SoDirectionalLightManip::getClassTypeId()) ||
	   type.isDerivedFrom(SoPointLightManip::getClassTypeId()) ||
	   type.isDerivedFrom(SoSpotLightManip::getClassTypeId()) ) return FALSE;
    }
  }
  return TRUE;
}

/*!
  Sets whether manipulators in the scene graph should be tested for intersection
  with other geometry or not.
  
  Note that when draggers are disabled with setDraggersEnabled(), this setting
  has no effect - manipulators are disabled too.

  \sa isManipsEnabled(), setDraggersEnabled(), setTypeEnabled()
*/

void
SoIntersectionDetectionAction::setManipsEnabled(SbBool enable)
{
  PRIVATE(this)->manipsenabled = enable;
}

/*!
  Returns whether the actions is set up to test intersection on manipulators
  in the scene or not.

  Note that when draggers are disabled with setDraggersEnabled(), this setting
  has no effect - manipulators are disabled too.

  The default is that manipulators are enabled for intersection testing with
  other geometry in the scene.

  \sa setManipsEnabled()
*/

SbBool
SoIntersectionDetectionAction::isManipsEnabled(void) const
{
  return PRIVATE(this)->manipsenabled;
}

/*!
  Sets whether draggers in the scene graph should be tested for intersection
  with other geometry or not.

  Note that when you disable draggers, manipulators are also automatically
  disabled, although the isManipsDisabled() setting might reflect otherwise.

  \sa isDraggersEnabled(), setManipsEnabled(), setTypeEnabled()
*/

void
SoIntersectionDetectionAction::setDraggersEnabled(SbBool enable)
{
  PRIVATE(this)->draggersenabled = enable;
}

/*!
  Returns whether the actions is set up to test intersection on draggers
  in the scene or not.

  The default is that draggers are enabled for intersection testing with
  other geometry in the scene.

  \sa setDraggersEnabled()
*/

SbBool
SoIntersectionDetectionAction::isDraggersEnabled(void) const
{
  return PRIVATE(this)->draggersenabled;
}

/*!
  Sets whether nodes in the scene graph should be checked for intersecting
  primitives within themselves.

  This setting is not supported yet.

  \sa isShapeInternalsEnabled()
*/

void
SoIntersectionDetectionAction::setShapeInternalsEnabled(SbBool enable)
{
  PRIVATE(this)->internalsenabled = enable;
}

/*!
  Returns whether nodes in the scene graph will be checked for
  intersecting primitives within themselves.

  The default value for this setting is FALSE.

  \sa setShapeInternalsEnabled()
*/

SbBool
SoIntersectionDetectionAction::isShapeInternalsEnabled(void) const
{
  return PRIVATE(this)->internalsenabled;
}

/*!
  The scene graph traversal can be controlled with callbacks which
  you set with this method.  Use just like you would use
  SoCallbackAction::addPreCallback().

  \sa SoCallbackAction::addPreCallback()
*/

void
SoIntersectionDetectionAction::addVisitationCallback(SoType type, SoIntersectionVisitationCB * cb, void * closure)
{
  PRIVATE(this)->traversaltypes->append(type);
  PRIVATE(this)->traversalcallbacks->append((void *) cb);
  PRIVATE(this)->traversalcallbacks->append(closure);
}

/*!
  The scene graph traversal can be controlled with callbacks which
  you remove with this method.  Use just like you would use
  SoCallbackAction::removePreCallback().
 
  \sa SoCallbackAction::removePreCallback()
*/

void
SoIntersectionDetectionAction::removeVisitationCallback(SoType type, SoIntersectionVisitationCB * cb, void * closure)
{
  int idx = 0;
  while ( idx < PRIVATE(this)->traversaltypes->getLength() ) {
    if ( (*(PRIVATE(this)->traversaltypes))[idx] == type ) {
      if ( ((*(PRIVATE(this)->traversalcallbacks))[idx*2] == (void *) cb) &&
           ((*(PRIVATE(this)->traversalcallbacks))[idx*2+1] == closure) ) {
        PRIVATE(this)->traversaltypes->remove(idx);
        PRIVATE(this)->traversalcallbacks->remove(idx*2+1);
        PRIVATE(this)->traversalcallbacks->remove(idx*2);
      } else {
        idx += 1;
      }
    } else {
      idx += 1;
    }
  }
}

/*!
  This callback is called when two shapes are found to have intersecting
  bounding boxes, and are about to be checked for real intersection between
  their primitives.

  When intersection epsilon values are in use, bounding box intersection
  testing is done approximately and will trigger the filter callback on
  boxes that are further from each other than the epsilon length.

  If the callback returns TRUE, the intersection test will be performed.
  If the callback returns FALSE, the intersection testing will be skipped.

  The API allows only one filter callback.
*/

void
SoIntersectionDetectionAction::setFilterCallback(SoIntersectionFilterCB * cb, void * closure)
{
  PRIVATE(this)->filtercb = cb;
  PRIVATE(this)->filterclosure = closure;
}

/*!
  Adds a callback to be called when two intersecting primitives are found in
  the scene.

  If the callback returns ABORT, the intersection detection is aborted.  If
  the callback returns NEXT_SHAPE, the intersection detection between these
  two shapes are aborted and the action continues checking other shapes.
  If the callback returns NEXT_PRIMITIVE, the intersection detection testing
  continues checking the other primitives in these two shapes.

  \sa removeIntersectionCallback()
*/

void
SoIntersectionDetectionAction::addIntersectionCallback(SoIntersectionCB * cb, void * closure)
{
  PRIVATE(this)->callbacks->append((void *) cb);
  PRIVATE(this)->callbacks->append(closure);
}

/*!
  Removes a callback set with addIntersectionCallback().

  \sa addIntersectionCallback()
*/

void
SoIntersectionDetectionAction::removeIntersectionCallback(SoIntersectionCB * cb, void * closure)
{
  int i;
  for ( i = 0; i < PRIVATE(this)->callbacks->getLength(); i += 2 ) {
    if ( ((*PRIVATE(this)->callbacks)[i] == (void *) cb) &&
         ((*PRIVATE(this)->callbacks)[i+1] == closure) ) {
      PRIVATE(this)->callbacks->remove(i+1);
      PRIVATE(this)->callbacks->remove(i);
    }
  }
}

// *************************************************************************

void
SoIntersectionDetectionAction::apply(SoNode * node)
{
  PRIVATE(this)->reset();
  PRIVATE(this)->traverser->apply(node);
  PRIVATE(this)->doIntersectionTesting();
}

void
SoIntersectionDetectionAction::apply(SoPath * path)
{
  PRIVATE(this)->reset();
  PRIVATE(this)->traverser->apply(path);
  PRIVATE(this)->doIntersectionTesting();
}

void
SoIntersectionDetectionAction::apply(const SoPathList & paths, SbBool obeysRules)
{
  PRIVATE(this)->reset();
  PRIVATE(this)->traverser->apply(paths, obeysRules);
  PRIVATE(this)->doIntersectionTesting();
}

// *************************************************************************

struct ShapeData {
  SoPath * path;
  SbXfBox3f bbox;
};

struct PrimitiveData {
  SbPList * triangles;
  SoPath * path;
  SbMatrix transform;
  SbMatrix invtransform;
};

void
SoIntersectionDetectionActionP::triangleCB(void * closure, SoCallbackAction * action, const SoPrimitiveVertex * v1, const SoPrimitiveVertex * v2, const SoPrimitiveVertex * v3)
{
  PrimitiveData * primitives = (PrimitiveData *) closure;
  const SbVec3f & oa =  v1->getPoint();
  const SbVec3f & ob =  v2->getPoint();
  const SbVec3f & oc =  v3->getPoint();
  SbVec3f wa, wb, wc;
  primitives->transform.multVecMatrix(oa, wa);
  primitives->transform.multVecMatrix(ob, wb);
  primitives->transform.multVecMatrix(oc, wc);
  SbTri3f * triangle = new SbTri3f(wa, wb, wc);
  primitives->triangles->append(triangle);
}

PrimitiveData *
SoIntersectionDetectionActionP::generatePrimitives(ShapeData * shape)
{
  PrimitiveData * primitives = new PrimitiveData;
  primitives->triangles = new SbPList;
  primitives->path = shape->path;
  primitives->transform = shape->bbox.getTransform();
  primitives->invtransform = primitives->transform.inverse();
  SoCallbackAction generator;
  generator.addTriangleCallback(SoShape::getClassTypeId(), triangleCB, primitives);
  generator.apply(shape->path);
  return primitives;
}

void
SoIntersectionDetectionActionP::deletePrimitives(PrimitiveData * primitives)
{
  if ( !primitives ) return;
  int i;
  for ( i = 0; i < primitives->triangles->getLength(); i++ ) {
    SbTri3f * triangle = (SbTri3f *) (*(primitives->triangles))[i];
    delete triangle;
  }
  delete primitives->triangles;
  delete primitives;
}

SoCallbackAction::Response
SoIntersectionDetectionActionP::shape(SoCallbackAction * action, SoShape * shape)
{
  SbBox3f bbox;
  SbVec3f center;
  shape->computeBBox(action, bbox, center);
  ShapeData * data = new ShapeData;
  data->path = new SoPath(*(action->getCurPath()));
  data->path->ref();
  data->bbox = bbox;
  data->bbox.setTransform(action->getModelMatrix());
  this->shapedata->append(data);
  return SoCallbackAction::CONTINUE;
}

SoCallbackAction::Response
SoIntersectionDetectionActionP::shapeCB(void * closure, SoCallbackAction * action, const SoNode * node)
{
  assert(node && node->isOfType(SoShape::getClassTypeId()));
  return ((SoIntersectionDetectionActionP *) closure)->shape(action, (SoShape *) node);
}

SoCallbackAction::Response
SoIntersectionDetectionActionP::traverse(SoCallbackAction * action, const SoNode * node)
{
  const SoPath * curpath = action->getCurPath();
  int i;
  for ( i = 0; i < this->traversaltypes->getLength(); i++ ) {
    if ( node->getTypeId().isDerivedFrom((*(this->traversaltypes))[i]) ) {
      SoIntersectionDetectionAction::SoIntersectionVisitationCB * cb =
        (SoIntersectionDetectionAction::SoIntersectionVisitationCB *)
        (*(this->traversalcallbacks))[i*2];
      SoCallbackAction::Response response = cb((*(this->traversalcallbacks))[i*2+1], curpath);
      if ( response != SoCallbackAction::CONTINUE ) return response;
    }
  }
  return SoCallbackAction::CONTINUE;
}

SoCallbackAction::Response
SoIntersectionDetectionActionP::traverseCB(void * closure, SoCallbackAction * action, const SoNode * node)
{
  return ((SoIntersectionDetectionActionP *) closure)->traverse(action, node);
}

SoCallbackAction::Response
SoIntersectionDetectionActionP::dragger(SoCallbackAction * action, const SoNode * node)
{
  if ( !this->draggersenabled ) // dragger setting overrides setting for manipulators
    return SoCallbackAction::PRUNE;
  if ( !this->manipsenabled ) {
    const SoPath * path = action->getCurPath();
    SoNode * tail = path->getTail();
    SoType type = tail->getTypeId();
    if ( type.isDerivedFrom(SoTransformManip::getClassTypeId()) ||
         type.isDerivedFrom(SoClipPlaneManip::getClassTypeId()) ||
         type.isDerivedFrom(SoDirectionalLightManip::getClassTypeId()) ||
         type.isDerivedFrom(SoPointLightManip::getClassTypeId()) ||
         type.isDerivedFrom(SoSpotLightManip::getClassTypeId()) )
      return SoCallbackAction::PRUNE;
  }
  return SoCallbackAction::CONTINUE;
}

SoCallbackAction::Response
SoIntersectionDetectionActionP::draggerCB(void * closure, SoCallbackAction * action, const SoNode * node)
{
  return ((SoIntersectionDetectionActionP *) closure)->dragger(action, node);
}

SoCallbackAction::Response
SoIntersectionDetectionActionP::pruneCB(void * closure, SoCallbackAction * action, const SoNode * node)
{
  return SoCallbackAction::PRUNE;
}

void
SoIntersectionDetectionActionP::reset(void)
{
  int i;
  for ( i = 0; i < this->shapedata->getLength(); i++ ) {
    ShapeData * data = (ShapeData *) (*(this->shapedata))[i];
    data->path->unref();
    delete data;
  }
  this->shapedata->truncate(0);
  this->traverser = new SoCallbackAction;
  this->traverser->addPreCallback(SoDragger::getClassTypeId(), draggerCB, this);
  this->traverser->addPreCallback(SoNode::getClassTypeId(), traverseCB, this);
  for ( i = 0; i < this->prunetypes->getLength(); i++ )
    this->traverser->addPreCallback((*(this->prunetypes))[i], pruneCB, NULL);
  this->traverser->addPreCallback(SoShape::getClassTypeId(), shapeCB, this);
}

void
SoIntersectionDetectionActionP::doIntersectionTesting(void)
{
  if ( this->callbacks->getLength() == 0 ) return;
  const float epsilon = this->getEpsilon();
  delete this->traverser;
  this->traverser = NULL;
  int i, j;
  for ( i = 0; i < this->shapedata->getLength(); i++ ) {
    ShapeData * shape1 = (ShapeData *) (*(this->shapedata))[i];
    PrimitiveData * primitives1 = NULL;
    if ( this->internalsenabled ) {
      primitives1 = SoIntersectionDetectionActionP::generatePrimitives(shape1);
      SbBool cont = this->doInternalPrimitiveIntersectionTesting(primitives1);
      if ( !cont ) {
        SoIntersectionDetectionActionP::deletePrimitives(primitives1);
        return;
      }
    }
    for ( j = i + 1; j < this->shapedata->getLength(); j++ ) {
      ShapeData * shape2 = (ShapeData *) (*(this->shapedata))[j];
      // support for negative epsilons can be added here
      // note that if support is added for this, negative bounding box volumes must be filtered
      /*
      if ( epsilon > 0.0f ) {
        SbVec3f epsilonvec(epsilon, epsilon, epsilon);
        SbBox3f shape2box(shape2->bbox);
        shape2->bbox.getTransform().multDirMatrix(epsilonvec, epsilonvec); // move epsilon to object space
        float localepsilon = epsilonvec.length(); // yes, it's a bit large...
        shape2box.getMin() -= SbVec3f(localepsilon, localepsilon, localepsilon);
        shape2box.getMax() += SbVec3f(localepsilon, localepsilon, localepsilon);
        SbXfBox3f shape2xfbbox(shape2box);
        shape2xfbbox.setTransform(shape2->bbox.getTransform());
        if ( shape1->bbox.intersect(shape2xfbbox) ) {
          if ( !this->filtercb || this->filtercb(this->filterclosure, shape1->path, shape2->path) ) {
            fprintf(stderr, "FIXME: primitive testing\n");
            // do primitive testing
          }
        }
      }
      else
      */
      if ( shape1->bbox.intersect(shape2->bbox) ) {
        if ( !this->filtercb || this->filtercb(this->filterclosure, shape1->path, shape2->path) ) {
          if ( primitives1 == NULL ) primitives1 = SoIntersectionDetectionActionP::generatePrimitives(shape1);
          PrimitiveData * primitives2 = SoIntersectionDetectionActionP::generatePrimitives(shape2);
          SbBool cont = this->doPrimitiveIntersectionTesting(primitives1, primitives2);
          SoIntersectionDetectionActionP::deletePrimitives(primitives2);
          if ( !cont ) {
	    SoIntersectionDetectionActionP::deletePrimitives(primitives1);
            return;
          }
        }
      }
    }
    SoIntersectionDetectionActionP::deletePrimitives(primitives1);
  }
}

SbBool
SoIntersectionDetectionActionP::doPrimitiveIntersectionTesting(PrimitiveData * primitives1, PrimitiveData * primitives2)
{
  int i, j;
  for ( i = 0; i < primitives1->triangles->getLength(); i++ ) {
    SbTri3f * t1 = (SbTri3f *) (*(primitives1->triangles))[i];
    for ( j = 0; j < primitives2->triangles->getLength(); j++ ) {
      SbTri3f * t2 = (SbTri3f *) (*(primitives2->triangles))[j];
      if ( t1->intersect(*t2) ) {
        SoIntersectingPrimitive p1;
        p1.path = primitives1->path;
        p1.type = SoIntersectingPrimitive::TRIANGLE;
        t1->getValue(p1.xf_vertex[0], p1.xf_vertex[1], p1.xf_vertex[2]);
        primitives1->invtransform.multVecMatrix(p1.xf_vertex[0], p1.vertex[0]);
        primitives1->invtransform.multVecMatrix(p1.xf_vertex[1], p1.vertex[1]);
        primitives1->invtransform.multVecMatrix(p1.xf_vertex[2], p1.vertex[2]);
        SoIntersectingPrimitive p2;
        p2.path = primitives2->path;
        p2.type = SoIntersectingPrimitive::TRIANGLE;
        t2->getValue(p2.xf_vertex[0], p2.xf_vertex[1], p2.xf_vertex[2]);
        primitives2->invtransform.multVecMatrix(p2.xf_vertex[0], p2.vertex[0]);
        primitives2->invtransform.multVecMatrix(p2.xf_vertex[1], p2.vertex[1]);
        primitives2->invtransform.multVecMatrix(p2.xf_vertex[2], p2.vertex[2]);
	int c;
        for ( c = 0; c < this->callbacks->getLength(); c += 2 ) {
          SoIntersectionDetectionAction::SoIntersectionCB * cb =
            (SoIntersectionDetectionAction::SoIntersectionCB *) (*(this->callbacks))[c];
          switch ( cb((*(this->callbacks))[c+1], &p1, &p2) ) {
          case SoIntersectionDetectionAction::NEXT_PRIMITIVE:
            break;
          case SoIntersectionDetectionAction::NEXT_SHAPE:
            return TRUE;
          case SoIntersectionDetectionAction::ABORT:
            return FALSE;
	  default:
	    assert(0);
          }
        }
      }
    }
  }
  return TRUE;
}

SbBool
SoIntersectionDetectionActionP::doInternalPrimitiveIntersectionTesting(PrimitiveData * primitives)
{
  int i, j;
  const int numprimitives = primitives->triangles->getLength();
  for ( i = 0; i < numprimitives; i++ ) {
    SbTri3f * t1 = (SbTri3f *) (*(primitives->triangles))[i];
    for ( j = i + 1; j < numprimitives; j++ ) {
      SbTri3f * t2 = (SbTri3f *) (*(primitives->triangles))[j];
      if ( t1->intersect(*t2) ) {
        SoIntersectingPrimitive p1;
        p1.path = primitives->path;
        p1.type = SoIntersectingPrimitive::TRIANGLE;
        t1->getValue(p1.xf_vertex[0], p1.xf_vertex[1], p1.xf_vertex[2]);
        primitives->invtransform.multVecMatrix(p1.xf_vertex[0], p1.vertex[0]);
        primitives->invtransform.multVecMatrix(p1.xf_vertex[1], p1.vertex[1]);
        primitives->invtransform.multVecMatrix(p1.xf_vertex[2], p1.vertex[2]);
        SoIntersectingPrimitive p2;
        p2.path = primitives->path;
        p2.type = SoIntersectingPrimitive::TRIANGLE;
        t2->getValue(p2.xf_vertex[0], p2.xf_vertex[1], p2.xf_vertex[2]);
        primitives->invtransform.multVecMatrix(p2.xf_vertex[0], p2.vertex[0]);
        primitives->invtransform.multVecMatrix(p2.xf_vertex[1], p2.vertex[1]);
        primitives->invtransform.multVecMatrix(p2.xf_vertex[2], p2.vertex[2]);
	int c;
        for ( c = 0; c < this->callbacks->getLength(); c += 2 ) {
          SoIntersectionDetectionAction::SoIntersectionCB * cb =
            (SoIntersectionDetectionAction::SoIntersectionCB *) (*(this->callbacks))[c];
          switch ( cb((*(this->callbacks))[c+1], &p1, &p2) ) {
          case SoIntersectionDetectionAction::NEXT_PRIMITIVE:
            break;
          case SoIntersectionDetectionAction::NEXT_SHAPE:
            return TRUE;
          case SoIntersectionDetectionAction::ABORT:
            return FALSE;
	  default:
	    assert(0);
          }
        }
      }
    }
  }
  return TRUE;
}

