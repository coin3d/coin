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
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/misc/SoShaderGenerator.h>
#include <Inventor/lists/SbList.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/SbMatrix.h>
#include <Inventor/C/glue/gl.h>
#include <math.h>
#include "../shaders/SoShader.h"

// *************************************************************************

class SoShadowSpotLightCache {
public:
  SoShadowSpotLightCache(const SoPath * path, SoGroup * scene, const SbBool vsm) {

    this->vsm_program = NULL;
    this->vsm_farval = NULL;

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
    this->depthmap->size = SbVec2s(1024, 1024);
    
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
  }
  ~SoShadowSpotLightCache() {
    if (this->vsm_program) this->vsm_program->unref();
    if (this->vsm_farval) this->vsm_farval->unref();
    if (this->fragment_farval) this->fragment_farval->unref();
    if (this->light) this->light->unref();
    if (this->path) this->path->unref();
    if (this->depthmap) this->depthmap->unref();
    if (this->camera) this->camera->unref();
  }

  void createVSMProgram(void);

  SbMatrix matrix;
  SoPath * path;
  SoSpotLight * light;
  SoSceneTexture2 * depthmap;
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
    shadersvalid(FALSE),
    shaderprogram(NULL),
    vertexshader(NULL),
    fragmentshader(NULL)
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
  SbBool shadersvalid;
  SbList <SoShadowSpotLightCache*> spotlights;

  SoShaderProgram * shaderprogram;
  SoVertexShader * vertexshader;
  SoFragmentShader * fragmentshader;

  SoShaderGenerator vertexgenerator;
  SoShaderGenerator fragmentgenerator;
  SoShaderParameterMatrix * cameratransform;

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
  SO_NODE_ADD_FIELD(epsilon, (0.001f));
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

  SbMatrix camtransform = SoViewingMatrixElement::get(state).inverse();
  if (camtransform != PRIVATE(this)->cameratransform->value.getValue()) {
    PRIVATE(this)->cameratransform->value = camtransform;
  }

  if (!PRIVATE(this)->shadersvalid) {
    PRIVATE(this)->setVertexShader(state);
    PRIVATE(this)->setFragmentShader(state);
    PRIVATE(this)->shadersvalid = TRUE;
  }

  PRIVATE(this)->shaderprogram->GLRender(action);
  
  SoShapeStyleElement::setShadowsRendering(state, TRUE);
  inherited::GLRenderBelowPath(action);
  SoShapeStyleElement::setShadowsRendering(state, FALSE);
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
  PRIVATE(this)->shadersvalid = FALSE;
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
        SoShadowSpotLightCache * cache = new SoShadowSpotLightCache(pl[i], PUBLIC(this), TRUE);
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
    
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    
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
	//cam->heightAngle.setValue(60 * M_PI / 180.0);

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
  
  const int depthbits = 16;
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

  // fprintf(stderr,"cache farval: %f\n", cache->farval);
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
}

void 
SoShadowGroupP::setVertexShader(SoState * state)
{
  int i;
  SoShaderGenerator & gen = this->vertexgenerator;

  gen.reset(FALSE);
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
  }
}

void 
SoShadowGroupP::setFragmentShader(SoState * state)
{
  int i;

  SoShaderGenerator & gen = this->fragmentgenerator;
  gen.reset(FALSE);

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

  // FIXME: move this function to Coin/data/shaders/
  gen.addFunction("float vsm_lookup(in vec4 map, in float dist) {\n"
                  "  float mapdist = map.x;\n"
                  "  float lit_factor = dist <= mapdist ? 1.0 : 0.0;\n"
                  "  float E_x2 = map.y;\n"
                  "  float Ex_2 = mapdist * mapdist;\n"
                  "  float variance = min(max(E_x2 - Ex_2, 0.0) + EPSILON, 1.0);\n"
                  "  float m_d = mapdist - dist;\n"
                  "  float p_max = variance / (variance + m_d * m_d);\n"
                  "  return max(lit_factor, p_max);\n"
                  "}\n", FALSE);
  
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
                  "shadeFactor = shadowCoord%d.z > -1.0 ? vsm_lookup(map, dist/farval%d) : 0.0;\n"
                  "color += shadeFactor * diffuse.rgb * gl_Color.rgb + "
                  "shadeFactor * gl_FrontMaterial.specular.rgb * specular.rgb;\n",
                  i+1, i , i, i, i, i);
      gen.addMainStatement(str);

    }

    if (perpixelother) {
      gen.addMainStatement("diffuse = vec4(0.0); ambient = vec4(0.0); specular = vec4(0.0);");
      
      SbBool dirlight = FALSE;
      SbBool pointlight = FALSE;
      
      const SoNodeList & lights = SoLightElement::getLights(state);
      
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
#if 0
  "gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);";
#else

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
#endif
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



#undef PUBLIC
