
#include <Inventor/SbXfBox3f.h>
#include <Inventor/SoPath.h>
#include <Inventor/lists/SbPList.h>

#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/nodes/SoShape.h>
#include <Inventor/draggers/SoDragger.h>
#include <Inventor/manips/SoTransformManip.h>
#include <Inventor/manips/SoClipPlaneManip.h>
#include <Inventor/manips/SoDirectionalLightManip.h>
#include <Inventor/manips/SoPointLightManip.h>
#include <Inventor/manips/SoSpotLightManip.h>

#include <Inventor/actions/SoIntersectionDetectionAction.h>

// *************************************************************************

class SoIntersectionDetectionActionP {
public:
  SoIntersectionDetectionActionP(void);
  ~SoIntersectionDetectionActionP(void);

  static float staticepsilon;
  float epsilon;
  SbBool epsilonset;

  SbBool draggersenabled;
  SbBool manipsenabled;

  float getEpsilon(void) const;

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

  void reset(void);
  void doIntersectionTesting(void);

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

// typedef SbBool SoIntersectionFilterCB(void * closure, const SoPath * p1, const SoPath * p2);
// typedef Response SoIntersectionCB(void * closure, const SoIntersectingPrimitive *, const SoIntersectingPrimitive *);

#define PRIVATE(obj) ((obj)->pimpl)

SO_ACTION_SOURCE(SoIntersectionDetectionAction);


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
*/

void
SoIntersectionDetectionAction::setIntersectionEpsilon(float epsilon) // static
{
  SoIntersectionDetectionActionP::staticepsilon = epsilon;
}

/*!
  Returns the global intersection detection distance epsilon value.
*/

float
SoIntersectionDetectionAction::getIntersectionEpsilon(void) // static
{
  return SoIntersectionDetectionActionP::staticepsilon;
}

/*!
  Sets the intersection detection distance epsilon value for the action object.
  This overrides the global value.
*/

void
SoIntersectionDetectionAction::setIntersectionDetectionEpsilon(float epsilon)
{
  PRIVATE(this)->epsilon = epsilon;
  PRIVATE(this)->epsilonset = TRUE;
}

/*!
  Returns the intersection detection distance epsilon value for the action object.
*/

float
SoIntersectionDetectionAction::getIntersectionDetectionEpsilon(void) const
{
  return PRIVATE(this)->getEpsilon();
}

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

SbBool
SoIntersectionDetectionAction::isTypeEnabled(SoType type, SbBool checkgroups) const
{
  if ( PRIVATE(this)->prunetypes->find(type) ) return FALSE;
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

void
SoIntersectionDetectionAction::setManipsEnabled(SbBool enable)
{
  PRIVATE(this)->manipsenabled = enable;
}

SbBool
SoIntersectionDetectionAction::isManipsEnabled(void) const
{
  return PRIVATE(this)->manipsenabled;
}

void
SoIntersectionDetectionAction::setDraggersEnabled(SbBool enable)
{
  PRIVATE(this)->draggersenabled = enable;
}

SbBool
SoIntersectionDetectionAction::isDraggersEnabled(void) const
{
  return PRIVATE(this)->draggersenabled;
}

void
SoIntersectionDetectionAction::addVisitationCallback(SoType type, SoIntersectionVisitationCB * cb, void * closure)
{
  PRIVATE(this)->traversaltypes->append(type);
  PRIVATE(this)->traversalcallbacks->append((void *) cb);
  PRIVATE(this)->traversalcallbacks->append(closure);
}

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
  
  If the callback returns TRUE, the intersection test will be performed.
  If the callback returns FALSE, the intersection testing will be skipped.
*/

void
SoIntersectionDetectionAction::setFilterCallback(SoIntersectionFilterCB * cb, void * closure)
{
  PRIVATE(this)->filtercb = cb;
  PRIVATE(this)->filterclosure = closure;
}

void
SoIntersectionDetectionAction::addIntersectionCallback(SoIntersectionCB * cb, void * closure)
{
  PRIVATE(this)->callbacks->append((void *) cb);
  PRIVATE(this)->callbacks->append(closure);
}

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
  const float epsilon = this->getEpsilon();
  delete this->traverser;
  this->traverser = NULL;
  fprintf(stderr, "checking bbox intersection between %d shapes\n", this->shapedata->getLength());
  int i, j;
  for ( i = 0; i < this->shapedata->getLength(); i++ ) {
    for ( j = i + 1; j < this->shapedata->getLength(); j++ ) {
      ShapeData * shape1 = (ShapeData *) (*(this->shapedata))[i];
      ShapeData * shape2 = (ShapeData *) (*(this->shapedata))[j];
      // support for negative epsilons can be added here
      if ( shape1->bbox.intersect(shape2->bbox) ) {
        if ( this->filtercb != NULL ) {
	  if ( this->filtercb(this->filterclosure, shape1->path, shape2->path) ) {
	    // shapely testing
	    // fprintf(stderr, "intersection\n");
          }
	}
      } else
      if ( epsilon > 0.0f ) {
	SbVec3f border(epsilon, epsilon, epsilon);
	SbBox3f shape2box(shape2->bbox);
	shape2->bbox.getTransform().multVecMatrix(border, border); // move border to object space
	shape2box.getMin() -= border;
	shape2box.getMax() += border;
        SbXfBox3f shape2xfbox(shape2box);
        shape2xfbox.setTransform(shape2->bbox.getTransform());
      }
    }
  }
}

