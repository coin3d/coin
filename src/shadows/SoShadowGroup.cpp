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

// use to increase the VSM precision by using all four components
#define DISTRIBUTE_FACTOR 32.0

// *************************************************************************

#include <FXViz/nodes/SoShadowGroup.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/nodes/SoSpotLight.h>
#include <Inventor/nodes/SoPointLight.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoSceneTexture2.h>
#include <Inventor/nodes/SoTransparencyType.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoVertexShader.h>
#include <Inventor/nodes/SoFragmentShader.h>
#include <Inventor/nodes/SoShaderProgram.h>
#include <Inventor/nodes/SoShaderParameter.h>
#include <Inventor/elements/SoShapeStyleElement.h>
#include <Inventor/elements/SoLightElement.h>
#include <Inventor/elements/SoTextureMatrixElement.h>
#include <Inventor/elements/SoMultiTextureMatrixElement.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/elements/SoViewingMatrixElement.h>
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/annex/FXViz/elements/SoShadowStyleElement.h>
#include <Inventor/annex/FXViz/nodes/SoShadowStyle.h>
#include <Inventor/SoPath.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/elements/SoShapeStyleElement.h>
#include <Inventor/elements/SoTextureUnitElement.h>
#include <Inventor/elements/SoGLTextureEnabledElement.h>
#include <Inventor/elements/SoGLMultiTextureEnabledElement.h>
#include <Inventor/elements/SoCacheElement.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/misc/SoShaderGenerator.h>
#include <Inventor/caches/SoShaderProgramCache.h>
#include <Inventor/lists/SbList.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/SbMatrix.h>
#include <Inventor/C/glue/gl.h>
#include <math.h>
#include "../shaders/SoShader.h"

// *************************************************************************

class SoShadowSpotLightCache {
public:
  SoShadowSpotLightCache(const SoPath * path, SoGroup * scene, 
                         const SbBool vsm, 
                         const int gausskernelsize,
                         const float gaussstandarddeviation)
  {
    const int TEXSIZE = 1024;
    
    this->vsm_program = NULL;
    this->vsm_farval = NULL;
    this->gaussmap = NULL;

    this->fragment_farval = new SoShaderParameter1f;
    this->fragment_farval->ref();
    
    if (vsm) this->createVSMProgram();

    this->path = path->copy();
    this->path->ref();
    assert(((SoFullPath*)path)->getTail()->isOfType(SoSpotLight::getClassTypeId()));
    
    this->light = (SoSpotLight*) ((SoFullPath*)path)->getTail();
    this->light->ref();
    this->depthmap = new SoSceneTexture2;
    this->depthmap->ref();
    this->depthmap->transparencyFunction = SoSceneTexture2::NONE;
    this->depthmap->size = SbVec2s(TEXSIZE, TEXSIZE);
    
    if (this->vsm_program) {
      this->depthmap->type = SoSceneTexture2::RGBA32F;
      this->depthmap->backgroundColor = SbVec4f(1.0f, 1.0f, 1.0f, 1.0f);
    }
    else {
      this->depthmap->type = SoSceneTexture2::DEPTH;
    }
    SoTransparencyType * tt = new SoTransparencyType;
    tt->value = SoTransparencyType::NONE;
    
    this->depthmap->sceneTransparencyType = tt;

    this->camera = new SoPerspectiveCamera;
    this->camera->ref();
    this->camera->viewportMapping = SoCamera::LEAVE_ALONE;

    SoSeparator * sep = new SoSeparator;
    sep->addChild(this->camera);

    if (this->vsm_program) sep->addChild(this->vsm_program);

    for (int i = 0; i < scene->getNumChildren(); i++) {
      sep->addChild(scene->getChild(i));
    }
    this->depthmap->scene = sep;
    this->matrix = SbMatrix::identity();

    if (gausskernelsize > 0) {
      this->gaussmap = new SoSceneTexture2;
      this->gaussmap->ref();
      this->gaussmap->transparencyFunction = SoSceneTexture2::NONE;
      this->gaussmap->size = SbVec2s(TEXSIZE, TEXSIZE);
    
      this->gaussmap->type = SoSceneTexture2::RGBA32F;
      this->gaussmap->backgroundColor = SbVec4f(1.0f, 1.0f, 1.0f, 1.0f);

      SoShaderProgram * shader = this->createGaussFilter(TEXSIZE, gausskernelsize, gaussstandarddeviation);
      this->gaussmap->scene = this->createGaussSG(shader, this->depthmap);
    }
  }
  ~SoShadowSpotLightCache() {
    if (this->vsm_program) this->vsm_program->unref();
    if (this->vsm_farval) this->vsm_farval->unref();
    if (this->fragment_farval) this->fragment_farval->unref();
    if (this->light) this->light->unref();
    if (this->path) this->path->unref();
    if (this->gaussmap) this->gaussmap->unref();
    if (this->depthmap) this->depthmap->unref();
    if (this->camera) this->camera->unref();
  }

  void createVSMProgram(void);
  SoShaderProgram * createGaussFilter(const int texsize, const int size, const float stdev);
  SoSeparator * createGaussSG(SoShaderProgram * program, SoSceneTexture2 * tex);
  

  SbMatrix matrix;
  SoPath * path;
  SoSpotLight * light;
  SoSceneTexture2 * depthmap;
  SoSceneTexture2 * gaussmap;
  SoPerspectiveCamera * camera;
  float farval;
  float nearval;
  
  SoShaderProgram * vsm_program;
  SoShaderParameter1f * vsm_farval;
  SoShaderParameter1f * fragment_farval;
  SoShaderGenerator vsm_vertex_generator;
  SoShaderGenerator vsm_fragment_generator;

};

class SoShadowGroupP {
public:
  SoShadowGroupP(SoShadowGroup * master) : 
    master(master), 
    matrixaction(SbViewportRegion(SbVec2s(100,100))),
    bboxaction(SbViewportRegion(SbVec2s(100,100))),
    spotlightsvalid(FALSE),
    shaderprogram(NULL),
    vertexshader(NULL),
    fragmentshader(NULL),
    vertexshadercache(NULL),
    fragmentshadercache(NULL)
  { 
    this->shaderprogram = new SoShaderProgram;
    this->shaderprogram->ref();
    this->shaderprogram->setEnableCallback(shader_enable_cb, this);
    this->vertexshader = new SoVertexShader;
    this->vertexshader->ref();
    this->fragmentshader = new SoFragmentShader;
    this->fragmentshader->ref();

    this->cameratransform = new SoShaderParameterMatrix;
    this->cameratransform->name = "cameraTransform";
    this->cameratransform->ref();

    this->vertexshader->parameter.set1Value(0, this->cameratransform);

    this->shaderprogram->shaderObject.set1Value(0, this->vertexshader);
    this->shaderprogram->shaderObject.set1Value(1, this->fragmentshader);
  }
  ~SoShadowGroupP() {
    if (this->vertexshadercache) this->vertexshadercache->unref();
    if (this->fragmentshadercache) this->vertexshadercache->unref();
    if (this->cameratransform) this->cameratransform->unref();
    if (this->vertexshader) this->vertexshader->unref();
    if (this->fragmentshader) this->fragmentshader->unref();
    if (this->shaderprogram) this->shaderprogram->unref();
    this->deleteSpotLights();
  }

  SoShaderProgram * createVSMProgram(void);

  void deleteSpotLights(void) {
    for (int i = 0; i < this->spotlights.getLength(); i++) {
      delete this->spotlights[i];
    }
    this->spotlights.truncate(0);
  }

  static void shader_enable_cb(void * closure, 
                               SoState * state,
                               const SbBool enable);

  void GLRender(SoGLRenderAction * action, const SbBool inpath);
  void setVertexShader(SoState * state);
  void setFragmentShader(SoState * state);
  void updateCamera(SoShadowSpotLightCache * cache, const SbMatrix & transform);
  void renderDepthMap(SoShadowSpotLightCache * cache,
                      SoGLRenderAction * action);
  void updateSpotLights(SoGLRenderAction * action);

  SoShadowGroup * master;
  SoSearchAction search;
  SoGetMatrixAction matrixaction;
  SoGetBoundingBoxAction bboxaction;

  SbBool spotlightsvalid;
  SbList <SoShadowSpotLightCache*> spotlights;

  SoShaderProgram * shaderprogram;
  SoVertexShader * vertexshader;
  SoFragmentShader * fragmentshader;

  SoShaderGenerator vertexgenerator;
  SoShaderGenerator fragmentgenerator;
  SoShaderParameterMatrix * cameratransform;

  SoShaderProgramCache * vertexshadercache;
  SoShaderProgramCache * fragmentshadercache;
  
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
  SO_NODE_ADD_FIELD(epsilon, (0.00001f));
  SO_NODE_ADD_FIELD(gaussStandardDeviation, (0.8f));
  SO_NODE_ADD_FIELD(gaussMatrixSize, (0));

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
  PRIVATE(this)->GLRender(action, FALSE);
}

void 
SoShadowGroup::GLRenderInPath(SoGLRenderAction * action)
{
  PRIVATE(this)->GLRender(action, TRUE);
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
  if (PRIVATE(this)->vertexshadercache) {
    PRIVATE(this)->vertexshadercache->invalidate();
  }

  if (PRIVATE(this)->fragmentshadercache) {
    PRIVATE(this)->fragmentshadercache->invalidate();
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
        SoShadowSpotLightCache * cache = new SoShadowSpotLightCache(pl[i], PUBLIC(this), TRUE,
                                                                    PUBLIC(this)->gaussMatrixSize.getValue(),
                                                                    PUBLIC(this)->gaussStandardDeviation.getValue());
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

    // SbMatrix mm = SoModelMatrixElement::get(state) * SoViewingMatrixElement::get(state);
    SbMatrix mat = cache->matrix;

    if (i == 0) {
      SoTextureMatrixElement::mult(state, PUBLIC(this), mat);
    }
    else {
      SoMultiTextureMatrixElement::mult(state, PUBLIC(this), i, cache->matrix);
    }
    
    const cc_glglue * glue = cc_glglue_instance(SoGLCacheContextElement::get(state));
    
    // glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    
    this->renderDepthMap(cache, action);

    if (i == 0) {
      SoGLTextureEnabledElement::set(state, PUBLIC(this), FALSE);
    }
    else {
      SoGLMultiTextureEnabledElement::set(state, PUBLIC(this), i,
                                          SoGLMultiTextureEnabledElement::DISABLED);
    }


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


  SbVec3f dir = light->direction.getValue();
  (void) dir.normalize();

  cam->orientation.setValue(SbRotation(SbVec3f(0.0f, 0.0f, -1.0f), dir));
  cam->heightAngle.setValue(light->cutOffAngle.getValue() * 2.0f);
  
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
  // if (cache->farval <= 0.0f) return;
  
  const int depthbits = 12;
  float r = (float) pow(2.0, (double) depthbits);
  float nearlimit = cache->farval / r;
  
  if (cache->nearval < nearlimit) {
    cache->nearval = nearlimit;
  }

  const float SLACK = 0.001f;

  cache->nearval = cache->nearval * (1.0f - SLACK);
  cache->farval = cache->farval * (1.0f + SLACK);

  if (cache->nearval != cam->nearDistance.getValue()) {
    cam->nearDistance = cache->nearval;
  }
  if (cache->farval != cam->farDistance.getValue()) {
    cam->farDistance = cache->farval;
  }

  cache->fragment_farval->value = cache->farval;
  cache->vsm_farval->value = cache->farval;
  
  
  SbViewVolume vv = cam->getViewVolume(1.0f);
  SbMatrix affine, proj;
  
  vv.getMatrices(affine, proj);

  cache->matrix = affine * proj;

#if 0
  fprintf(stderr,"matrix:\n"
          "%.2f %.2f %.2f %.2f\n"
          "%.2f %.2f %.2f %.2f\n"
          "%.2f %.2f %.2f %.2f\n"
          "%.2f %.2f %.2f %.2f\n",
          cache->matrix[0][0], cache->matrix[0][1], cache->matrix[0][2], cache->matrix[0][3],
          cache->matrix[1][0], cache->matrix[1][1], cache->matrix[1][2], cache->matrix[1][3],
          cache->matrix[2][0], cache->matrix[2][1], cache->matrix[2][2], cache->matrix[2][3],
          cache->matrix[3][0], cache->matrix[3][1], cache->matrix[3][2], cache->matrix[3][3]);
#endif

}


void 
SoShadowGroupP::renderDepthMap(SoShadowSpotLightCache * cache,
                               SoGLRenderAction * action)
{
  cache->depthmap->GLRender(action);
  if (cache->gaussmap) cache->gaussmap->GLRender(action);
}

void 
SoShadowGroupP::setVertexShader(SoState * state)
{
  int i;
  SoShaderGenerator & gen = this->vertexgenerator;
  gen.reset(FALSE);

  SbBool storedinvalid = SoCacheElement::setInvalid(FALSE);
  state->push();

  if (this->vertexshadercache) {
    this->vertexshadercache->unref();
  }
  this->vertexshadercache = new SoShaderProgramCache(state);
  this->vertexshadercache->ref();
  
  // set active cache to record cache dependencies
  SoCacheElement::set(state, this->vertexshadercache);
  const SoNodeList & lights = SoLightElement::getLights(state);
  
  int numspots = this->spotlights.getLength();

  for (i = 0; i < numspots; i++) {
    SbString str;
    str.sprintf("varying vec4 shadowCoord%d;", i);
    gen.addDeclaration(str, FALSE);
  }

  gen.addDeclaration("uniform mat4 cameraTransform;", FALSE);
  gen.addDeclaration("varying vec3 ecPosition3;", FALSE);
  gen.addDeclaration("varying vec3 fragmentNormal;", FALSE);
  gen.addDeclaration("varying vec3 perVertexColor;", FALSE);
  
  SbBool dirlight = FALSE;
  SbBool pointlight = FALSE;
  SbBool spotlight = FALSE;
  SbString str;

  gen.addMainStatement("vec4 ecPosition = gl_ModelViewMatrix * gl_Vertex;\n"
                       "ecPosition3 = ecPosition.xyz / ecPosition.w;");

  gen.addMainStatement("vec3 normal = normalize(gl_NormalMatrix * gl_Normal);\n"
                       "vec3 eye = -normalize(ecPosition3);\n"
                       "vec4 ambient = vec4(0.0);\n"
                       "vec4 diffuse = vec4(0.0);\n"
                       "vec4 specular = vec4(0.0);");
  
  gen.addMainStatement("fragmentNormal = normal;");


  // FIXME: decide based on quality
  SbBool perpixel = TRUE;
  
  if (!perpixel) {
    for (i = 0; i < lights.getLength(); i++) {
      SoLight * l = (SoLight*) lights[i];
      if (l->isOfType(SoDirectionalLight::getClassTypeId())) {
        str.sprintf("DirectionalLight(%d, normal, ambient, diffuse, specular);", i);
        gen.addMainStatement(str);
        dirlight = TRUE;
      }
      else if (l->isOfType(SoSpotLight::getClassTypeId())) {
        str.sprintf("SpotLight(%d, eye, ecPosition3, normal, ambient, diffuse, specular);", i);
        gen.addMainStatement(str);
        spotlight = TRUE;
        
      }
      else if (l->isOfType(SoPointLight::getClassTypeId())) {
        str.sprintf("PointLight(%d, eye, ecPosition3, normal, ambient, diffuse, specular);", i);
        gen.addMainStatement(str);
        pointlight = TRUE;      
      }
      else {
        SoDebugError::postWarning("SoShadowGroupP::setVertexShader",
                                  "Unknown light type: %s\n",
                                  l->getTypeId().getName().getString());
      }
    }
    
    if (dirlight) gen.addNamedFunction(SbName("lights/DirectionalLight"), FALSE);
    if (pointlight) gen.addNamedFunction(SbName("lights/PointLight"), FALSE);
    if (spotlight) gen.addNamedFunction(SbName("lights/SpotLight"), FALSE);
    
    gen.addMainStatement(
                         "vec4 color = gl_FrontLightModelProduct.sceneColor + "
                         "  ambient * gl_FrontMaterial.ambient + "
                         "  diffuse * gl_Color + "
                         "  specular * gl_FrontMaterial.specular;\n"
                         );
  }
  else {
    gen.addMainStatement("vec4 color = gl_FrontLightModelProduct.sceneColor;\n");
  }
  
  gen.addMainStatement("vec4 pos = cameraTransform * ecPosition;\n"); // in world space
  
  for (i = 0; i < numspots; i++) {
    SbString str;
    str.sprintf("shadowCoord%d = gl_TextureMatrix[%d] * pos;\n", i,i); // in light space
    gen.addMainStatement(str);

    gen.addMainStatement(
                         "perVertexColor = color.rgb;"
                         "gl_Position = ftransform();\n"
                         "gl_FrontColor = gl_Color;");
  }

  // never update unless the program has actually changed. Creating a
  // new GLSL program is very slow on current drivers.
  if (this->vertexshader->sourceProgram.getValue() != gen.getShaderProgram()) {
    this->vertexshader->sourceProgram = gen.getShaderProgram();
    this->vertexshader->sourceType = SoShaderObject::GLSL_PROGRAM;
    this->vertexshadercache->set(gen.getShaderProgram());
  }
  this->vertexshadercache->set(gen.getShaderProgram());
  state->pop();
  SoCacheElement::setInvalid(storedinvalid);
  
}

void 
SoShadowGroupP::setFragmentShader(SoState * state)
{
  int i;

  SoShaderGenerator & gen = this->fragmentgenerator;
  gen.reset(FALSE);

  SbBool storedinvalid = SoCacheElement::setInvalid(FALSE);
  state->push();

  if (this->fragmentshadercache) {
    this->fragmentshadercache->unref();
  }
  this->fragmentshadercache = new SoShaderProgramCache(state);
  this->fragmentshadercache->ref();

  // set active cache to record cache dependencies
  SoCacheElement::set(state, this->fragmentshadercache);

  int numspots = this->spotlights.getLength();

  SbString eps;
  eps.sprintf("const float EPSILON = %f;\n",
              PUBLIC(this)->epsilon.getValue());
  gen.addDeclaration(eps, FALSE);
  
  for (int i = 0; i < numspots; i++) {
    SbString str;
    str.sprintf("uniform sampler2D shadowMap%d;", i);
    gen.addDeclaration(str, FALSE);
    
    str.sprintf("uniform float farval%d;", i);
    gen.addDeclaration(str, FALSE);
    
    str.sprintf("varying vec4 shadowCoord%d;", i);
    gen.addDeclaration(str, FALSE);
  }
  
#ifdef DISTRIBUTE_FACTOR
  SbString str;
  str.sprintf("const float DISTRIBUTE_FACTOR = %.1f;\n", DISTRIBUTE_FACTOR);
  gen.addDeclaration(str, FALSE);
#endif

  gen.addDeclaration("varying vec3 ecPosition3;", FALSE);
  gen.addDeclaration("varying vec3 fragmentNormal;", FALSE);
  gen.addDeclaration("varying vec3 perVertexColor;", FALSE);

  // FIXME: decide based on quality
 
  SbBool perpixelspot = TRUE;
  SbBool perpixelother = TRUE;

  if (perpixelspot) {
    gen.addNamedFunction(SbName("lights/SpotLight"), FALSE);
  }

  const SoNodeList & lights = SoLightElement::getLights(state);

  gen.addNamedFunction("vsm/VsmLookup", FALSE);
  gen.addMainStatement("vec3 eye = -normalize(ecPosition3);\n");
  gen.addMainStatement("vec4 ambient = vec4(0.0);\n"
                       "vec4 diffuse = vec4(0.0);\n"
                       "vec4 specular = vec4(0.0);");

  if (perpixelspot) {
    gen.addMainStatement("vec3 color = perVertexColor;\n"
                         "float dist;\n"
                         "float shadeFactor;\n"
                         "vec3 coord;\n"
                         "vec4 map;\n");
    for (i = 0; i < numspots; i++) {
      SbString str;
      
      str.sprintf("diffuse = vec4(0.0); specular = vec4(0);"
                  "dist = SpotLight(%d, eye, ecPosition3, normalize(fragmentNormal), ambient, diffuse, specular);\n"
                  "coord = 0.5 * (shadowCoord%d.xyz / shadowCoord%d.w + vec3(1.0));\n"
                  "map = texture2D(shadowMap%d, coord.xy);\n"
#ifdef DISTRIBUTE_FACTOR
                  "map.xy += map.zw / DISTRIBUTE_FACTOR;\n"
#endif
                  "shadeFactor = shadowCoord%d.z > -1.0 ? VsmLookup(map, dist/farval%d, EPSILON) : 0.0;\n"
                  "color += shadeFactor * diffuse.rgb * gl_Color.rgb + "
                  "shadeFactor * gl_FrontMaterial.specular.rgb * specular.rgb;\n",
                  lights.getLength(), i , i, i, i, i);
      gen.addMainStatement(str);

    }

    if (perpixelother) {
      gen.addMainStatement("diffuse = vec4(0.0); ambient = vec4(0.0); specular = vec4(0.0);");
      
      SbBool dirlight = FALSE;
      SbBool pointlight = FALSE;
      
      for (i = 0; i < lights.getLength(); i++) {
        SoLight * l = (SoLight*) lights[i];
        if (l->isOfType(SoDirectionalLight::getClassTypeId())) {
          str.sprintf("DirectionalLight(%d, normalize(fragmentNormal), ambient, diffuse, specular);", i);
          gen.addMainStatement(str);
          dirlight = TRUE;
        }
        else if (l->isOfType(SoSpotLight::getClassTypeId())) {
          str.sprintf("SpotLight(%d, eye, ecPosition3, normalize(fragmentNormal), ambient, diffuse, specular);", i);
          gen.addMainStatement(str);
        }
        else if (l->isOfType(SoPointLight::getClassTypeId())) {
          str.sprintf("PointLight(%d, eye, ecPosition3, normalize(fragmentNormal), ambient, diffuse, specular);", i);
          gen.addMainStatement(str);
          pointlight = TRUE;      
        }
        else {
          SoDebugError::postWarning("SoShadowGroupP::setFragmentShader",
                                    "Unknown light type: %s\n",
                                    l->getTypeId().getName().getString());
        }
      }
      
      if (dirlight) gen.addNamedFunction(SbName("lights/DirectionalLight"), FALSE);
      if (pointlight) gen.addNamedFunction(SbName("lights/PointLight"), FALSE);

      gen.addMainStatement("color += ambient.rgb * gl_FrontMaterial.specular.rgb + "
                           "diffuse.rgb * gl_Color.rgb + specular.rgb * gl_FrontMaterial.specular.rgb;\n");
    }    
  }
  
  else {
    assert(0 && "not supported");
    SbString str;
    str.sprintf(
                "vec3 VP = vec3(gl_LightSource[1].position) - ecPosition3;\n"
                "float d = length(VP);\n"
                "float shadeFactor = shadowCoord0.z > -1.0 ? lookup(shadowMap0, shadowCoord0, d / farval0) : 0.0;\n"
                "gl_FragColor = vec4(perVertexColor + shadeFactor * perVertexColor, gl_Color.a);");
    
    gen.addMainStatement(str);  
  }
  gen.addMainStatement("gl_FragColor = vec4(color, gl_Color.a);\n");


  // never update unless the program has actually changed. Creating a
  // new GLSL program is very slow on current drivers.
  this->fragmentshader->parameter.setNum(numspots*2);
  
  if (this->fragmentshader->sourceProgram.getValue() != gen.getShaderProgram()) {
    this->fragmentshader->sourceProgram = gen.getShaderProgram();
    this->fragmentshader->sourceType = SoShaderObject::GLSL_PROGRAM;
    
    for (int i = 0; i < numspots; i++) {
      SoShaderParameter1i * shadowmap = 
        new SoShaderParameter1i();
      SbString str;
      str.sprintf("shadowMap%d", i);
      shadowmap->name = str;
      shadowmap->value = i;
      this->fragmentshader->parameter.set1Value(i*2, shadowmap); 
    }
  }

  for (int i = 0; i < numspots; i++) {
    SbString str;
    SoShaderParameter1f *farval = this->spotlights[i]->fragment_farval;
    str.sprintf("farval%d", i);
    if (farval->name.getValue() != str) {
      farval->name = str;
    }
    this->fragmentshader->parameter.set1Value(i*2+1, farval); 
  }

  this->fragmentshadercache->set(gen.getShaderProgram());
  state->pop();
  SoCacheElement::setInvalid(storedinvalid);
}

void
SoShadowSpotLightCache::createVSMProgram(void)
{
  SoShaderProgram * program = new SoShaderProgram;
  program->ref();

  SoVertexShader * vshader = new SoVertexShader;
  SoFragmentShader * fshader = new SoFragmentShader;

  program->shaderObject.set1Value(0, vshader);
  program->shaderObject.set1Value(1, fshader);

  SoShaderGenerator & vgen = this->vsm_vertex_generator;
  SoShaderGenerator & fgen = this->vsm_fragment_generator;
  
  vgen.reset(FALSE);

  vgen.addDeclaration("varying vec3 light_vec;", FALSE);
  vgen.addMainStatement("light_vec = (gl_ModelViewMatrix * gl_Vertex).xyz;\n"
                        "gl_Position = ftransform();");
  
  vshader->sourceProgram = vgen.getShaderProgram();
  vshader->sourceType = SoShaderObject::GLSL_PROGRAM;
  
  fgen.reset(FALSE);
#ifdef DISTRIBUTE_FACTOR
  SbString str;
  str.sprintf("const float DISTRIBUTE_FACTOR = %.1f;\n", DISTRIBUTE_FACTOR);
  fgen.addDeclaration(str, FALSE);
#endif
  fgen.addDeclaration("varying vec3 light_vec;", FALSE);
  fgen.addDeclaration("uniform float farval;", FALSE);
  fgen.addMainStatement("float l = length(light_vec) / farval;\n"
#ifdef DISTRIBUTE_FACTOR
                        "vec2 m = vec2(l, l*l);\n"
                        "vec2 f = fract(m * DISTRIBUTE_FACTOR);\n"
                        
                        "gl_FragColor.rg = m - (f / DISTRIBUTE_FACTOR);\n"
                        "gl_FragColor.ba = f;\n"
#else
                        "gl_FragColor = vec4(l, l*l, 0.0, 0.0);"
#endif
                        );
  fshader->sourceProgram = fgen.getShaderProgram();
  fshader->sourceType = SoShaderObject::GLSL_PROGRAM;

  this->vsm_program = program;
  this->vsm_program->ref();

  this->vsm_farval = new SoShaderParameter1f;
  this->vsm_farval->ref();
  this->vsm_farval->name = "farval";
  fshader->parameter = this->vsm_farval;
}

void
SoShadowGroupP::shader_enable_cb(void * closure,
                                 SoState * state,
                                 const SbBool enable)
{
  SoShadowGroupP * thisp = (SoShadowGroupP*) closure;

  for (int i = 0; i < thisp->spotlights.getLength(); i++) {
    
    int unit = i; // FIXME: read out from spotlight cache
    if (unit == 0) {
      SoGLTextureEnabledElement::set(state, enable);
    }
    else {
      SoMultiTextureEnabledElement::set(state, NULL, unit, 
                                        enable);
    }
  }
}

void 
SoShadowGroupP::GLRender(SoGLRenderAction * action, const SbBool inpath)
{
  SoState * state = action->getState();

  state->push();
  
  if (!this->vertexshadercache || !this->vertexshadercache->isValid(state)) {
    // a bit hackish, but saves creating yet another cache
    this->spotlightsvalid = FALSE;
  }

  SbMatrix camtransform = SoViewingMatrixElement::get(state).inverse();
  if (camtransform != this->cameratransform->value.getValue()) {
    this->cameratransform->value = camtransform;
  }
  
  SoShadowStyleElement::set(state, PUBLIC(this), SoShadowStyleElement::CASTS_SHADOW_AND_SHADOWED);
  SoShapeStyleElement::setShadowMapRendering(state, TRUE);
  this->updateSpotLights(action);
  SoShapeStyleElement::setShadowMapRendering(state, FALSE);


  if (!this->vertexshadercache || !this->vertexshadercache->isValid(state)) {
    this->setVertexShader(state);
  }
  
  if (!this->fragmentshadercache || !this->fragmentshadercache->isValid(state)) {
    this->setFragmentShader(state);
  }
  this->shaderprogram->GLRender(action);
  
  SoShapeStyleElement::setShadowsRendering(state, TRUE);
  if (inpath) PUBLIC(this)->SoSeparator::GLRenderInPath(action);
  else PUBLIC(this)->SoSeparator::GLRenderBelowPath(action);
  SoShapeStyleElement::setShadowsRendering(state, FALSE);
  state->pop();
}

SoShaderProgram * 
SoShadowSpotLightCache::createGaussFilter(const int texsize, const int size, const float gaussstandarddeviation)
{
  SoVertexShader * vshader = new SoVertexShader;
  SoFragmentShader * fshader = new SoFragmentShader;
  SoShaderProgram * program = new SoShaderProgram;

  SoShaderParameterArray2f * offset = new SoShaderParameterArray2f;
  offset->name = "offset";
  SoShaderParameterArray1f * kernel = new SoShaderParameterArray1f;
  kernel->name = "kernelvalue";
  SoShaderParameter1i * baseimage = new SoShaderParameter1i;
  baseimage->name = "baseimage";
  baseimage->value = 3; // FIXME

  int kernelsize = size*size;
  
  offset->value.setNum(kernelsize);
  kernel->value.setNum(kernelsize);

  SoShaderGenerator fgen;
  SbString str;
  
  str.sprintf("const int KernelSize = %d;", kernelsize);
  fgen.addDeclaration(str, FALSE);
  fgen.addDeclaration("uniform vec2 offset[KernelSize];", FALSE);
  fgen.addDeclaration("uniform float kernelvalue[KernelSize];", FALSE);
  fgen.addDeclaration("uniform sampler2D baseimage;", FALSE);
  
  fgen.addMainStatement(
                        "int i;\n"
                        "vec4 sum = vec4(0.0);\n"
                        "for (i = 0; i < KernelSize; i++) {\n"
                        "  vec4 tmp = texture2D(baseimage, gl_TexCoord[3].st + offset[i]);\n"
                        "  sum += tmp * kernelvalue[i];\n"
                        "}\n"
                        "gl_FragColor = sum;\n"
                        );
  
  const double sigma = (double) gaussstandarddeviation;
  const int center = size / 2;
  const float dt = 1.0 / float(texsize);
  
  SbVec2f * offsetptr = offset->value.startEditing();
  float * kernelptr = kernel->value.startEditing();
  
  int c = 0;
  for (int y = 0; y < size; y++) {
    int dy = SbAbs(y - center);
    for (int x = 0; x < size; x++) {
      int dx = SbAbs(x - center);
      
      kernelptr[c] = (1.0 /  (2.0 * M_PI * sigma * sigma)) * exp(- double(dx*dx + dy*dy) / (2.0 * sigma * sigma)); 
      offsetptr[c] = SbVec2f(float(x-center) * dt, float(y-center)*dt);
      c++;

    }
  }
  offset->value.finishEditing();
  kernel->value.finishEditing();

  program->shaderObject = vshader;
  program->shaderObject.set1Value(1, fshader);

  fshader->sourceProgram = fgen.getShaderProgram();
  fshader->sourceType = SoShaderObject::GLSL_PROGRAM;
  
  fshader->parameter.set1Value(0, offset);
  fshader->parameter.set1Value(1, kernel);
  fshader->parameter.set1Value(2, baseimage);

  SoShaderGenerator vgen;
  vgen.addMainStatement("gl_TexCoord[3] = gl_Vertex;\n");
  vgen.addMainStatement("gl_Position = ftransform();");

  vshader->sourceProgram = vgen.getShaderProgram();
  vshader->sourceType = SoShaderObject::GLSL_PROGRAM;

  return program;
}

#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoTextureUnit.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/nodes/SoFaceSet.h>
#include <Inventor/nodes/SoOrthographicCamera.h>

SoSeparator * 
SoShadowSpotLightCache::createGaussSG(SoShaderProgram * program, SoSceneTexture2 * tex)
{
  SoSeparator * sep = new SoSeparator;
  SoOrthographicCamera * camera = new SoOrthographicCamera;
  SoShapeHints * sh = new SoShapeHints;

  const float verts[][3] = {
    {0.0f, 0.0f, 0.0f},
    {1.0f, 0.0f, 0.0f},
    {1.0f, 1.0f, 0.0f},
    {0.0f, 1.0f, 0.0f}
    
  };

  sh->vertexOrdering = SoShapeHints::COUNTERCLOCKWISE;
  sh->faceType = SoShapeHints::CONVEX;
  sh->shapeType = SoShapeHints::SOLID;
  
  sep->addChild(sh);

  camera->position = SbVec3f(0.5f, 0.5f, 2.0f);
  camera->height = 1.0f;
  camera->aspectRatio = 1.0f;
  camera->viewportMapping = SoCamera::LEAVE_ALONE;

  sep->addChild(camera);
  SoTextureUnit * unit = new SoTextureUnit;
  unit->unit = 3; // FIXME: temporary
  sep->addChild(unit);

  sep->addChild(tex);
  sep->addChild(program);

  SoCoordinate3 * coord = new SoCoordinate3;
  sep->addChild(coord);

  coord->point.setValues(0,4,verts);

  SoFaceSet * fs = new SoFaceSet;
  fs->numVertices = 4;
  sep->addChild(fs);

  return sep;
}



#undef PUBLIC
