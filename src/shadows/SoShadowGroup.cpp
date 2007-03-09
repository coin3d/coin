/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2005 by Systems in Motion.  All rights reserved.
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
 *  See <URL:http://www.coin3d.org/> for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

/*!
  \class SoShadowGroup SoShadowGroup.h FXViz/nodes/SoSeparator.h
  \brief The SoShadowGroup node is a group node used for shadow rendering.
  \ingroup fxviz

  <b>FILE FORMAT/DEFAULTS:</b>
  \code
    SoShadowGroup {
      isActive TRUE
      intensity 0.5
      precision 0.5
      quality 0.5
      shadowCachingEnabled TRUE
      visibilityRadius -1.0
      visibilityFlag FIXME
    }
  \endcode
*/

// *************************************************************************

#include <FXViz/nodes/SoShadowGroup.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/nodes/SoSpotLight.h>
#include <Inventor/nodes/SoSceneTexture2.h>
#include <Inventor/nodes/SoTransparencyType.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/elements/SoShapeStyleElement.h>
#include <Inventor/annex/FXViz/elements/SoShadowStyleElement.h>
#include <Inventor/annex/FXViz/nodes/SoShadowStyle.h>
#include <Inventor/SoPath.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/elements/SoShapeStyleElement.h>
#include <Inventor/elements/SoTextureUnitElement.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/lists/SbList.h>
#include <Inventor/SbMatrix.h>
#include <math.h>

// *************************************************************************

class SoShadowSpotLightCache {
public:
  SoShadowSpotLightCache(const SoPath * path, SoGroup * scene) {
    this->path = path->copy();
    this->path->ref();
    assert(((SoFullPath*)path)->getTail()->isOfType(SoSpotLight::getClassTypeId()));
    
    this->light = (SoSpotLight*) ((SoFullPath*)path)->getTail();
    this->light->ref();
    this->depthmap = new SoSceneTexture2;
    this->depthmap->ref();
    this->depthmap->transparencyFunction = SoSceneTexture2::NONE;
    this->depthmap->size = SbVec2s(512, 512);
    this->depthmap->type = SoSceneTexture2::DEPTH_COMPONENT;

    SoTransparencyType * tt = new SoTransparencyType;
    tt->value = SoTransparencyType::NONE;
    
    this->depthmap->sceneTransparencyType = tt;

    this->camera = new SoPerspectiveCamera;
    this->camera->ref();

    SoSeparator * sep = new SoSeparator;
    sep->addChild(this->camera);
    
    for (int i = 0; i < scene->getNumChildren(); i++) {
      sep->addChild(scene->getChild(i));
    }
    this->depthmap->scene = sep;
  }
  ~SoShadowSpotLightCache() {
    if (this->light) this->light->unref();
    if (this->path) this->path->unref();
    if (this->depthmap) this->depthmap->unref();
    if (this->camera) this->camera->unref();
  }

  SoPath * path;
  SoSpotLight * light;
  SoSceneTexture2 * depthmap;
  SoPerspectiveCamera * camera;
  float farval;
  float nearval;
};

class SoShadowGroupP {
public:
  SoShadowGroupP(SoShadowGroup * master) : 
    matrixaction(SbViewportRegion(SbVec2s(100,100))),
    bboxaction(SbViewportRegion(SbVec2s(100,100))),
    master(master), 
    spotlightsvalid(FALSE) 
  { }
  ~SoShadowGroupP() {
    this->deleteSpotLights();
  }

  void deleteSpotLights(void) {
    for (int i = 0; i < this->spotlights.getLength(); i++) {
      delete this->spotlights[i];
    }
    this->spotlights.truncate(0);
  }
  
  void updateCamera(SoShadowSpotLightCache * cache, const SbMatrix & transform);
  void renderDepthMap(SoShadowSpotLightCache * cache,
                      SoGLRenderAction * action);
  void updateSpotLights(SoGLRenderAction * action);
  
  SoSearchAction search;
  SoGetMatrixAction matrixaction;
  SoGetBoundingBoxAction bboxaction;

  SoShadowGroup * master;
  SbBool spotlightsvalid;
  SbList <SoShadowSpotLightCache*> spotlights;
};

// *************************************************************************

#define PRIVATE(obj) ((obj)->pimpl)
#define PUBLIC(obj) ((obj)->master)

SO_NODE_SOURCE(SoShadowGroup);

/*!
  Default constructor.
*/
SoShadowGroup::SoShadowGroup(void)
{
  PRIVATE(this) = new SoShadowGroupP(this);

  SO_NODE_INTERNAL_CONSTRUCTOR(SoShadowGroup);

  SO_NODE_ADD_FIELD(isActive, (TRUE));
  SO_NODE_ADD_FIELD(intensity, (0.5f));
  SO_NODE_ADD_FIELD(precision, (0.5f));
  SO_NODE_ADD_FIELD(quality, (0.5f));
  SO_NODE_ADD_FIELD(shadowCachingEnabled, (TRUE));
  SO_NODE_ADD_FIELD(visibilityRadius, (-1.0f));
  // SO_NODE_ADD_FIELD(visibilityFlag, ());
}

/*!
  Destructor.
*/
SoShadowGroup::~SoShadowGroup()
{
  delete PRIVATE(this);
}

// Doc from superclass.
void
SoShadowGroup::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoShadowGroup, SO_FROM_COIN_2_4); // FIXME: add define for 2.5
}

void 
SoShadowGroup::init(void)
{
  SoShadowGroup::initClass();
  SoShadowStyleElement::initClass();
  SoShadowStyle::initClass();
}

void 
SoShadowGroup::GLRenderBelowPath(SoGLRenderAction * action)
{
  SoState * state = action->getState();
  state->push();
  SoShadowStyleElement::set(state, this, SoShadowStyleElement::CASTS_SHADOW_AND_SHADOWED);
  SoShapeStyleElement::setShadowMapRendering(state, TRUE);
  PRIVATE(this)->updateSpotLights(action);
  SoShapeStyleElement::setShadowMapRendering(state, FALSE);
  inherited::GLRenderBelowPath(action);
  state->pop();
}

void 
SoShadowGroup::GLRenderInPath(SoGLRenderAction * action)
{
  assert(0 && "not supported yet");
  inherited::GLRenderInPath(action);
}

void 
SoShadowGroup::notify(SoNotList * nl)
{
  // FIXME: examine notification chain, and detect when an SoSpotLight is
  // changed. When this happens we can just invalidate the depth map for 
  // that spot light, and not the others.

  SoNotRec * rec = nl->getLastRec();
  if (rec->getBase() != this) {
    // was not notified through a field, subgraph was changed
    PRIVATE(this)->spotlightsvalid = FALSE;
  }
  inherited::notify(nl);
}

#undef PRIVATE

// *************************************************************************

void 
SoShadowGroupP::updateSpotLights(SoGLRenderAction * action)
{
  int i;
  SoState * state = action->getState();

  if (!this->spotlightsvalid) {
    this->search.setType(SoSpotLight::getClassTypeId());
    this->search.setInterest(SoSearchAction::ALL);
    this->search.setSearchingAll(FALSE);
    this->search.apply(PUBLIC(this));
    SoPathList & pl = this->search.getPaths();

    if (pl.getLength() != this->spotlights.getLength()) {
      // just delete and recreate all if the number of spot lights have changed
      this->deleteSpotLights();
      for (i = 0; i < pl.getLength(); i++) {
        SoShadowSpotLightCache * cache = new SoShadowSpotLightCache(pl[i], PUBLIC(this));
        this->spotlights.append(cache);
      }
    }
    // validate if spot light paths are still valid
    for (i = 0; i < pl.getLength(); i++) {
      SoPath * path = pl[i];
      SoShadowSpotLightCache * cache = this->spotlights[i];
      if (*(cache->path) != *path) {
        cache->path->unref();
        cache->path = path->copy();
      }
      SoSpotLight * sl = (SoSpotLight*) ((SoFullPath*)path)->getTail();
      this->updateCamera(cache, SbMatrix::identity());
    }
    this->spotlightsvalid = TRUE;
  }
  
  for (i = 0; i < this->spotlights.getLength(); i++) {
    SoShadowSpotLightCache * cache = this->spotlights[i];
    SoTextureUnitElement::set(state, PUBLIC(this), i);
    this->renderDepthMap(cache, action);
  }
  SoTextureUnitElement::set(state, PUBLIC(this), 0);
}

void 
SoShadowGroupP::updateCamera(SoShadowSpotLightCache * cache, const SbMatrix & transform)
{
  SoPerspectiveCamera * cam = cache->camera;
  SoSpotLight * light = cache->light;

  SbVec3f pos = light->location.getValue();
  transform.multVecMatrix(pos, pos);
  cam->position.setValue(pos);
	cam->orientation.setValue(SbRotation(SbVec3f(0.0f, 0.0f, -1.0f), light->direction.getValue()));
	cam->heightAngle.setValue(light->cutOffAngle.getValue());

  // FIXME: cache bbox in the pimpl class
  this->bboxaction.apply(cache->depthmap->scene.getValue());
  SbXfBox3f xbox = this->bboxaction.getXfBoundingBox();

  SbMatrix mat;
  mat.setTranslate(- cam->position.getValue());
  xbox.transform(mat);
  mat = cam->orientation.getValue().inverse();
  xbox.transform(mat);
  SbBox3f box = xbox.project();

  // Bounding box was calculated in camera space, so we need to "flip"
  // the box (because camera is pointing in the (0,0,-1) direction
  // from origo.
  cache->nearval = -box.getMax()[2];
  cache->farval = -box.getMin()[2];

  // light won't hit the scene
  if (cache->farval <= 0.0f) return;
  
  const int depthbits = 24;
  float r = (float) pow(2.0, (double) depthbits);
  float nearlimit = cache->farval / r;
  
  if (cache->nearval < nearlimit) {
    cache->nearval = nearlimit;
  }    
  const float SLACK = 0.001f;
  
  cache->nearval = cam->nearDistance = cache->nearval * (1.0f - SLACK);
  cache->farval = cam->farDistance = cache->farval * (1.0f + SLACK);
#if 0  
  fprintf(stderr,"nearfar %g %g\n",
          cache->nearval,
          cache->farval);
#endif
}

void 
SoShadowGroupP::renderDepthMap(SoShadowSpotLightCache * cache,
                              SoGLRenderAction * action)
{
  cache->depthmap->GLRender(action);
}

#undef PUBLIC
