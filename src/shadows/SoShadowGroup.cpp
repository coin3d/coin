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
#include <Inventor/nodes/SoPointLight.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoSceneTexture2.h>
#include <Inventor/nodes/SoTransparencyType.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoVertexShader.h>
#include <Inventor/nodes/SoFragmentShader.h>
#include <Inventor/nodes/SoShaderProgram.h>
#include <Inventor/nodes/SoShaderParameter.h>
#include <Inventor/nodes/SoShader.h>
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
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/misc/SoShaderGenerator.h>
#include <Inventor/lists/SbList.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/SbMatrix.h>
#include <Inventor/C/glue/gl.h>
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
    this->depthmap->type = SoSceneTexture2::DEPTH;

    SoTransparencyType * tt = new SoTransparencyType;
    tt->value = SoTransparencyType::NONE;
    
    this->depthmap->sceneTransparencyType = tt;

    this->camera = new SoPerspectiveCamera;
    this->camera->ref();
    this->camera->viewportMapping = SoCamera::LEAVE_ALONE;

    SoSeparator * sep = new SoSeparator;
    sep->addChild(this->camera);
    
    for (int i = 0; i < scene->getNumChildren(); i++) {
      sep->addChild(scene->getChild(i));
    }
    this->depthmap->scene = sep;

    this->matrix = SbMatrix::identity();
  }
  ~SoShadowSpotLightCache() {
    if (this->light) this->light->unref();
    if (this->path) this->path->unref();
    if (this->depthmap) this->depthmap->unref();
    if (this->camera) this->camera->unref();
  }

  SbMatrix matrix;
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
    master(master), 
    matrixaction(SbViewportRegion(SbVec2s(100,100))),
    bboxaction(SbViewportRegion(SbVec2s(100,100))),
    spotlightsvalid(FALSE),
    shaderprogram(NULL),
    vertexshader(NULL),
    fragmentshader(NULL)
  { 
    this->shaderprogram = new SoShaderProgram;
    this->shaderprogram->ref();
    this->vertexshader = new SoVertexShader;
    this->vertexshader->ref();
    this->fragmentshader = new SoFragmentShader;
    this->fragmentshader->ref();

    this->cameratransform = new SoShaderParameterMatrix;
    this->cameratransform->name = "cameraTransform";
    this->cameratransform->ref();

    this->vertexshader->parameter.set1Value(0, this->cameratransform);

    // FIXME: just testing
    this->shaderprogram->shaderObject.set1Value(0, this->vertexshader);
    this->shaderprogram->shaderObject.set1Value(1, this->fragmentshader);

    this->vsm_program = NULL;
    this->createVSMProgram();
  }
  ~SoShadowGroupP() {
    if (this->vsm_program) this->vsm_program->unref();
    if (this->cameratransform) this->cameratransform->unref();
    if (this->vertexshader) this->vertexshader->unref();
    if (this->fragmentshader) this->fragmentshader->unref();
    if (this->shaderprogram) this->shaderprogram->unref();
    this->deleteSpotLights();
  }

  void createVSMProgram(void);

  void deleteSpotLights(void) {
    for (int i = 0; i < this->spotlights.getLength(); i++) {
      delete this->spotlights[i];
    }
    this->spotlights.truncate(0);
  }
  
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
  SoShaderParameterMatrix * cameratransform;

  SoShaderGenerator vertexgenerator;
  SoShaderGenerator fragmentgenerator;

  SoShaderProgram * vsm_program;
  SoShaderGenerator vsm_vertex_generator;
  SoShaderGenerator vsm_fragment_generator;
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

  SbMatrix camtransform = SoViewingMatrixElement::get(state).inverse();
  if (camtransform != PRIVATE(this)->cameratransform->value.getValue()) {
    PRIVATE(this)->cameratransform->value = camtransform;
  }
  PRIVATE(this)->setVertexShader(state);
  PRIVATE(this)->setFragmentShader(state);
  PRIVATE(this)->shaderprogram->GLRender(action);
  
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

    // SbMatrix mm = SoModelMatrixElement::get(state) * SoViewingMatrixElement::get(state);
    SbMatrix mat = cache->matrix;

    if (i == 0) {
      SoTextureMatrixElement::mult(state, PUBLIC(this), mat);
    }
    else {
      SoMultiTextureMatrixElement::mult(state, PUBLIC(this), i, cache->matrix);
    }
    
    const cc_glglue * glue = cc_glglue_instance(SoGLCacheContextElement::get(state));
    
    cc_glglue_glPolygonOffsetEnable(glue, TRUE, cc_glglue_FILLED);
    cc_glglue_glPolygonOffset(glue, 3.0f, 1.0f);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    
    this->renderDepthMap(cache, action);
    cc_glglue_glPolygonOffsetEnable(glue, FALSE, cc_glglue_FILLED);
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
  
  const int depthbits = 24;
  float r = (float) pow(2.0, (double) depthbits);
  float nearlimit = cache->farval / r;
  
  if (cache->nearval < nearlimit) {
    cache->nearval = nearlimit;
  }    

  const float SLACK = 0.001f;

  cache->nearval = 1.0f;

  cache->nearval = cam->nearDistance = cache->nearval * (1.0f - SLACK);
  cache->farval = cam->farDistance = cache->farval * (1.0f + SLACK);

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
  if (this->vertexshader->sourceProgram.getValue().getLength()) return;

  int i;
  SoShaderGenerator & gen = this->vertexgenerator;

  gen.reset(FALSE);
  const SoNodeList & lights = SoLightElement::getLights(state);
  
  gen.addDeclaration("varying vec4 shadowCoord0;", FALSE);
  gen.addDeclaration("uniform mat4 cameraTransform;", FALSE);
  gen.addDeclaration("varying vec3 ecPosition3;", FALSE);
  gen.addDeclaration("varying vec3 fragmentNormal;", FALSE);
  
  SbBool dirlight = FALSE;
  SbBool pointlight = FALSE;
  SbBool spotlight = FALSE;
  SbString str;

  gen.addMainStatement("vec3 normal = gl_NormalMatrix * gl_Normal;\n"
                       "vec3 eye = vec3(0.0);\n" // FIXME: support local viewer
                       "vec4 ambient = vec4(0.0);\n"
                       "vec4 diffuse = vec4(0.0);\n"
                       "vec4 specular = vec4(0.0);");
  
  gen.addMainStatement("vec4 ecPosition = gl_ModelViewMatrix * gl_Vertex;\n"
                       "ecPosition3 = ecPosition.xyz / ecPosition.w;");
  gen.addMainStatement("fragmentNormal = normal;");
  
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

                       //"vec4 color = gl_FrontLightModelProduct.sceneColor + "
                       "vec4 color = "
                       "  ambient * gl_FrontMaterial.ambient + "
                       "  diffuse * gl_Color + "
                       "  specular * gl_FrontMaterial.specular;\n"
                       
                       "vec4 pos = cameraTransform * ecPosition;\n" // in world space
                       
                       "shadowCoord0 = gl_TextureMatrix[0] * pos;\n" // in light space
                       "gl_Position = ftransform();\n"
                       "gl_FrontColor = color;");
  
  this->vertexshader->sourceProgram = gen.getShaderProgram();
  this->vertexshader->sourceType = SoShaderObject::GLSL_PROGRAM;
}

void 
SoShadowGroupP::setFragmentShader(SoState * state)
{
  if (this->fragmentshader->sourceProgram.getValue().getLength()) return;

  SoShaderGenerator & gen = this->fragmentgenerator;
  gen.reset(FALSE);

  gen.addDeclaration("uniform sampler2DShadow shadowMap0;", FALSE);
  gen.addDeclaration("varying vec4 shadowCoord0;", FALSE);
  gen.addDeclaration("varying vec3 ecPosition3;", FALSE);
  gen.addDeclaration("varying vec3 fragmentNormal;", FALSE);

  gen.addNamedFunction(SbName("lights/SpotLight"), SoShader::GLSL_SHADER);

  gen.addFunction("float lookup() {\n"
                  "  vec3 coord = 0.5 * (shadowCoord0.xyz / shadowCoord0.w + 1.0);\n"
                  "  return shadowCoord0.z > 0.0 ? (shadow2D(shadowMap0, coord).s == 1.0 ? 1.0 : 0.0) : 0.0;\n"
                  "}", FALSE);
  
  gen.addMainStatement("vec3 eye = vec3(0.0);");
  gen.addMainStatement("vec4 ambient = vec4(0.0);\n"
                       "vec4 diffuse = vec4(0.0);\n"
                       "vec4 specular = vec4(0.0);");

#if 1
  gen.addMainStatement("float shadeFactor = lookup();\n"
                       "SpotLight(1, eye, ecPosition3, fragmentNormal, ambient, diffuse, specular);\n"
                       "gl_FragColor = vec4(shadeFactor * diffuse.rgb, gl_Color.a);");

#else
  gen.addMainStatement("float shadeFactor = lookup();\n"
                       "gl_FragColor = vec4(shadeFactor * gl_Color.rgb, gl_Color.a);");

#endif  


  this->fragmentshader->sourceProgram = gen.getShaderProgram();
  this->fragmentshader->sourceType = SoShaderObject::GLSL_PROGRAM;

  SoShaderParameter1i * shadowmap0 = 
    new SoShaderParameter1i();

  shadowmap0->name = "shadowMap0";
  shadowmap0->value = 0;

  this->fragmentshader->parameter = shadowmap0; 
}

void
SoShadowGroupP::createVSMProgram(void)
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
  fgen.addDeclaration("varying vec3 light_vec;", FALSE);
  fgen.addMainStatement("float l = length(light_vec);\n"
                        "gl_FragColor = vec4(l, l*l, 0.0, 0.0);");

  fshader->sourceProgram = fgen.getShaderProgram();
  fshader->sourceType = SoShaderObject::GLSL_PROGRAM;

}

#undef PUBLIC
