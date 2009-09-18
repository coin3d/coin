/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2009 by Kongsberg SIM.  All rights reserved.
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

#include "FBXFileReader.h"

#include <Inventor/SbBSPTree.h>
#include <Inventor/SbMatrix.h>
#include <Inventor/SoOutput.h>
#include <Inventor/SbRotation.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/nodes/SoBaseColor.h>
#include <Inventor/nodes/SoBumpMap.h>
#include <Inventor/nodes/SoBumpMapTransform.h>
#include <Inventor/nodes/SoBumpMapCoordinate.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoLight.h>
#include <Inventor/nodes/SoMatrixTransform.h>
#include <Inventor/nodes/SoPointLight.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoSpotLight.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoMaterialBinding.h>
#include <Inventor/nodes/SoNormal.h>
#include <Inventor/nodes/SoNormalBinding.h>
#include <Inventor/nodes/SoRotation.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTexture2.h>
#include <Inventor/nodes/SoTexture2Transform.h>
#include <Inventor/nodes/SoTextureCoordinate2.h>
#include <Inventor/nodes/SoTextureCoordinateBinding.h>
#include <Inventor/nodes/SoTextureUnit.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>

#include <stdio.h>
#include <vector>
#include <algorithm>

#include "FBX2IVHelpers.h"
#include "Reindexer.h"

using namespace FBX2IVHelpers;

// Names of FBX attribute types for debugging purposes
#ifdef COIN_DEBUG
const char * const attributeNames [] = {
  "UNIDENTIFIED",
  "NULL",
  "MARKER",
  "SKELETON", 
  "MESH", 
  "NURB", 
  "PATCH", 
  "CAMERA", 
  "CAMERA_SWITCHER",
  "LIGHT",
  "OPTICAL_REFERENCE",
  "OPTICAL_MARKER",
  "NURBS_CURVE",
  "TRIM_NURBS_SURFACE",
  "BOUNDARY",
  "NURBS_SURFACE",
  "SHAPE",
  "LODGROUP"
};
#endif // COIN_DEBUG

static int COIN_DEBUG_FBX = -1;

/*!
  References existing internal nodes.
 */
void 
FBXFileReader::SubMeshInfo::refAll()
{
  if (faceset) {
    faceset->ref();
  }
  if (material) {
    material->ref();
  }
  for (TexInfoVector::iterator iter = textures.begin();
       iter != textures.end();
       iter++) {
    if (iter->textrans) {
      iter->textrans->ref();
    }
    if (iter->texture) {
      iter->texture->ref();
    }
    if (iter->bump) {
      iter->bump->ref();
    }
  }
}


/*!
  Dereferences existing internal nodes.
 */
void 
FBXFileReader::SubMeshInfo::unrefAll()
{
  if (faceset) {
    faceset->unref();
  }
  if (material) {
    material->unref();
  }
  for (TexInfoVector::iterator iter = textures.begin();
       iter != textures.end();
       iter++) {
    if (iter->textrans) {
      iter->textrans->unref();
    }
    if (iter->texture) {
      iter->texture->unref();
    }
    if (iter->bump) {
      iter->bump->unref();
    }
  }
}


/*!
  FIXME: At the moment, we must create a new file reader for each file to be converted.
  This should be fixed by either using a static converter method or lifting this restriction.
*/
FBXFileReader::FBXFileReader(KFbxSdkManager * sdkManager)
  : sdkManager(sdkManager)
{
  // This really only needs to be run once:
  const char * env = coin_getenv("COIN_DEBUG_FBX");
  if (env && atoi(env) == 1) {
    COIN_DEBUG_FBX = 1;
  }
  else {
    COIN_DEBUG_FBX = 0;
  }
}


/*!
  Read the given file and create a Coin scene graph from the fbx scene graph.
  This is the only entry point into this class.
*/
SoSeparator *
FBXFileReader::read(const char * filename)
{
  SoSeparator * root = NULL;
  KFbxScene *fbxScene = loadScene(filename);
  if (fbxScene) {
    root = createSceneGraph(fbxScene);
    fbxScene->Destroy();
  }

  return root;
}


/*!
  Loads an fbx scene from disk and returns a pointer to it.
  Returns NULL if an error occured.
*/
KFbxScene *
FBXFileReader::loadScene(const char * filename)
{
  KFbxScene *scene = NULL;

  KFbxImporter * importer = KFbxImporter::Create(this->sdkManager, "");
  assert(importer);

  int fileFormat;
  if (!this->sdkManager->GetIOPluginRegistry()->DetectFileFormat(filename, fileFormat)) {
    // Unrecognizable file format. Try to fall back to native format.
    // FIXME: Should we fail instead? kintel 20081217
    fileFormat = this->sdkManager->GetIOPluginRegistry()->GetNativeReaderFormat();
  }
  importer->SetFileFormat(fileFormat);

  // Initialize the importer by providing a filename.
  if (importer->Initialize(filename)) {
    scene = KFbxScene::Create(this->sdkManager, "");
    assert(scene);
    if (!importer->Import(scene)) {
#if COIN_DEBUG
      SoDebugError::post("FBXFileReader::loadScene",
                         "KFbxImporter::Import() failed: %s.",
                         importer->GetLastErrorString());
#endif // COIN_DEBUG
      scene->Destroy();
      scene = NULL;
    }
  }
  else {
#if COIN_DEBUG
    SoDebugError::post("FBXFileReader::loadScene",
                       "KFbxImporter::Initialize() failed: %s.",
                       importer->GetLastErrorString());
#endif // COIN_DEBUG
  }

  importer->Destroy();
  return scene;
}


/*!
  Converts the given FBX scene to a Coin scenegraph.

  Returns an SoSeparator with refcount 1
*/
SoSeparator *
FBXFileReader::createSceneGraph(KFbxScene * fbxScene)
{
  clearDefaults();
  KFbxMatrix identity;
  SoSeparator * root = (SoSeparator *)convertNode(fbxScene->GetRootNode(), identity);
  root->ref();
  clearDefaults();

  // Postprocess lights and cameras; move them in front of the scene as required by Coin
  if (root) {
    postProcessLights(root);
    postProcessCameras(root);
  } else {
#if COIN_DEBUG
    SoDebugError::post("FBXFileReader::createSceneGraph",
                       "FBX scene graph contains no supported data.");
#endif // COIN_DEBUG
  }

  return root;
}

/*!
  Helper function for removing unused Separators regarding to lights.
  This is because the transformations of FBX lights form a hierarchy.
  For conversion the hierarchy is converted into a single matrix.
  After building the matrix the part of the hiererchy only containing the 
  light will removed.
 */
SoGroup * getParent(SoPath * path, SoNode * child) {
  SoNode * node = NULL;
  int idx;
  for (int i = 0; i < path->getLength(); ++i) {
    node = path->getNodeFromTail(i);
    if (node->getTypeId().isDerivedFrom(SoGroup::getClassTypeId())) {
      idx = ((SoGroup *)node)->findChild(child);
      if (idx != -1) {
        return (SoGroup *)node;
      }
    }
  }
  return NULL;
}


/*!
  Clears the default value map by unreferencing the contents.
  Resources which were not used after conversion will be deleted by
  using "unref".
 */
void
FBXFileReader::clearDefaults()
{
  // clear map containing the default nodes
  for (std::map<int, SoNode *>::iterator i = defaults.begin(); i != defaults.end(); ++i) {
    i->second->unref();
    i->second = NULL;
  }
  defaults.clear();
}


/*!
  Create a rotation from three rotations about the major axes.
  Uses the given rotation order to concatenate the partial rotations.
 */
static SbRotation rotFromXYZRot(const KFbxVector4 & v, ERotationOrder order) 
{
  // single rotation around each major axis
  SbRotation rx = SbRotation(SbVec3f(1, 0, 0), v[0] * M_PI / 180.0f);
  SbRotation ry = SbRotation(SbVec3f(0, 1, 0), v[1] * M_PI / 180.0f);
  SbRotation rz = SbRotation(SbVec3f(0, 0, 1), v[2] * M_PI / 180.0f);
  
  SbRotation r;

  // multiply them in requested order
  switch (order) {
    case eEULER_XYZ:
      r = rx * ry * rz;
    return r;
    case eEULER_XZY:
      r = rx * rz * ry;
    break;
    case eEULER_YXZ:
      r = ry * rx * rz;
    break;
    case eEULER_YZX:
      r = ry * rz * rx;
    break;
    case eEULER_ZXY:
      r = rz * rx * ry;
    break;
    case eEULER_ZYX:
      r = rz * ry * rx;
    break;
    case eSPHERIC_XYZ:
#if COIN_DEBUG
        SoDebugError::post("FBXFileReader::rotFromXYZRot",
                           "unsupported spheric rotation.");
#endif // COIN_DEBUG
    default:
#if COIN_DEBUG
        SoDebugError::post("FBXFileReader::rotFromXYZRot",
                           "unknown rotation type.");
#endif // COIN_DEBUG
    break;
  }
  return r;
}

/*!
  Extracts the transformation from the given FBX texture and converts
  it to an SoTexture2Transform or SoBumpMapTransform node.
*/
template<typename T>
T * 
FBXFileReader::convertTextureTransform(KFbxTexture * fbxTexture)
{
  T *textrans = new T(); 
  KFbxVector4 v;
  textrans->translation = convert<SbVec2f>(fbxTexture->GetDefaultT(v)); 
  textrans->rotation = M_PI*fbxTexture->GetDefaultR(v)[2]/180;
  textrans->scaleFactor = convert<SbVec2f>(fbxTexture->GetDefaultS(v));
  return textrans;
}

/*!
  Converts the given FBX texture to an SoTexture2.
  Returns the converted texture or NULL if the filename of the 
  texture could not be found.
  The created node have a refcount of 1. After conversion all 
  Textures will be unrefd to delete unused ones.
*/
template<typename T>
T * 
FBXFileReader::convertTexture(KFbxTexture * fbxTexture)
{
  // Reuse texture
  TextureMap::iterator iter;
  if ((iter = this->textures.find(fbxTexture)) != this->textures.end()) {
    T *tex = (T*)(iter->second);
    assert(tex);
    return tex;
  }

  T * result = NULL;

  // Using relative filenames
  // Absolute filename: KString filename = fbxTexture->GetFileName();
  KString filename = KFbxExtractFileName(fbxTexture->GetFileName());
  if (!filename.IsEmpty()) {
      result = new T();
      result->setName(fbxTexture->GetName());
      result->ref();
      result->filename.set(filename);
      if (fbxTexture->GetWrapModeU() == KFbxTexture::eCLAMP) {
        result->wrapS = T::CLAMP;
      }
      if (fbxTexture->GetWrapModeV() == KFbxTexture::eCLAMP) {
        result->wrapT = T::CLAMP;
      }
  }

  this->textures[fbxTexture] = result;
  return result;
}

/*!
  Returns an SoTransform node.
*/
static SoTransform * createTransformNode(const SbVec3f & translation,
                                         const SbRotation & rotation,
                                         const SbVec3f & scale,
                                         const SbVec3f & center)
{
  SoTransform * transform = new SoTransform();

  transform->translation.setValue(translation);
  transform->rotation.setValue(rotation);
  transform->scaleFactor.setValue(scale);
  transform->center.setValue(center);

  return transform;
}


/*!
  Recursively traverses the FBX subgraph rooted by the given node and creates an 
  SoSeparator for each FBX node visited.

  Each FBX node contains the transformation of its attached attributes (usually geometry).

   SoSeparator
       |- SoTransform
       |- SoSeparator
            |- SoTransform
            |- [attribute nodes]
       |- [child nodes]
*/
SoSeparator *
FBXFileReader::convertNode(KFbxNode * fbxNode, KFbxMatrix & parentMatrix)
{
  assert(fbxNode);

  // if the node is set to invisible, ignore the node and its children
  KFbxTypedProperty<fbxDouble1> visible = fbxNode->Visibility;
  if (fbxNode->Visibility.IsValid() && fbxNode->Visibility.Get() == 0.0) {
    return NULL;
  }

  SoSeparator * soNodeRoot = new SoSeparator();
  soNodeRoot->setName(fbxNode->GetName());

  // Set up transform for node (inherited by children)
  ERotationOrder order;
  fbxNode->GetRotationOrder(KFbxNode::eSOURCE_SET, order);
 
  SoTransform * geomTransform = 
    createTransformNode(convert<SbVec3f>(fbxNode->GeometricTranslation.Get()), 
                        rotFromXYZRot(fbxNode->GeometricRotation.Get(), order), 
                        convert<SbVec3f>(fbxNode->GeometricScaling.Get()), 
                        convert<SbVec3f>(fbxNode->RotationPivot.Get()));
 
  // Inherited transform
  SoMatrixTransform * lclTransform = new SoMatrixTransform();
  KFbxMatrix globMat = fbxNode->GetGlobalFromCurrentTake(KTime());
  KFbxMatrix locMat = parentMatrix.Inverse() * globMat;
  lclTransform->matrix = convert<SbMatrix>(locMat);
  addNonDefault(soNodeRoot, (SoNode **)&lclTransform, defaults);

  // Local (geometry) transform
  SoSeparator * lclSeparator = new SoSeparator();
  lclSeparator->ref();
  addNonDefault(lclSeparator, (SoNode **)&geomTransform, defaults);

  // extract geometry
  bool added = false;
  for (int i = 0; i < fbxNode->GetNodeAttributeCount(); i++) {
    added = addAttribute(fbxNode->GetNodeAttributeByIndex(i), lclSeparator) || added;
  }
  // Don't add the separator if we didn't convert anything
  if (added) {
    soNodeRoot->addChild(lclSeparator);
  }
  lclSeparator->unref(); // To remove the unused node

  // traverse child nodes
  for (int i = 0; i < fbxNode->GetChildCount(); i++) {
    SoNode * child = convertNode(fbxNode->GetChild(i), globMat);
    if (child) {
      soNodeRoot->addChild(child);
      added = true;
    }
  }

  // If no attributes and no children were added, get rid of this Coin node
  if (!added) {
    soNodeRoot->ref();
    soNodeRoot->unref();
    soNodeRoot = NULL;
  }

  return soNodeRoot;
}


/*!
  Converts a single material into a SoMaterial node.
*/
SoMaterial * 
FBXFileReader::convertMaterial(KFbxSurfaceMaterial * fbxMaterial)
{
  assert(fbxMaterial && "FBXFileReader::convertMaterial: No FBX material given.");

  // Reuse material
  MaterialMap::iterator iter;
  if ((iter = this->materials.find(fbxMaterial)) != this->materials.end()) {
    return iter->second;
  }

  // Note! Using dynamic_cast<> instead of KFbxCast<> here since the inheritance
  // hierarchy of the KFbxSurface* are wrongly configured in the FBX library
  KFbxSurfaceLambert * fbxLambert = dynamic_cast<KFbxSurfaceLambert*>(fbxMaterial);
  SoMaterial * soMaterial = new SoMaterial();
  soMaterial->setName(fbxMaterial->GetName());

  // Handle materials with unknown shading models by using default values.
  // This is a known issue with DWG->FBX conversions by 3D Studio Max.
  // We could improve this by trying to read out individual properties.
  // This would require a bit of testing and access to example models,
  // so it's postponed until the feature is requested.
  if (!fbxLambert) return soMaterial;

  soMaterial->emissiveColor = 
    fbxLambert->GetEmissiveFactor().Get() * 
    convert<SbVec3f>(fbxLambert->GetEmissiveColor().Get());
  soMaterial->ambientColor =
    fbxLambert->GetAmbientFactor().Get() *
    convert<SbVec3f>(fbxLambert->GetAmbientColor().Get());
  soMaterial->diffuseColor = 
    fbxLambert->GetDiffuseFactor().Get() *
    convert<SbVec3f>(fbxLambert->GetDiffuseColor().Get());
  
  SbVec3f transcolor =
    fbxLambert->GetTransparencyFactor().Get() *
    convert<SbVec3f>(fbxLambert->GetTransparentColor().Get());
  soMaterial->transparency = (transcolor[0] + transcolor[1] + transcolor[2])/3;
  
  soMaterial->shininess.setValue(0.0f);
  
  // FIXME: How do we interpret these?
  // fbxMaterial->GetBump();
  // fbxMaterial->GetBumpFactor();

  KFbxSurfacePhong * fbxPhong = dynamic_cast<KFbxSurfacePhong*>(fbxMaterial);

  if (fbxPhong) {
    SbVec3f unclamped = 
      fbxPhong->GetSpecularFactor().Get() *
      convert<SbVec3f>(fbxPhong->GetSpecularColor().Get());

    soMaterial->specularColor.setValue(clamp(unclamped[0], 0.0f, 1.0f),
                                       clamp(unclamped[1], 0.0f, 1.0f),
                                       clamp(unclamped[2], 0.0f, 1.0f));
    
    soMaterial->shininess.setValue(clamp(fbxPhong->GetShininess().Get(), 0.0, 128.0)/128.0);
    // FIXME: How do we interpret these?
    // fbxMaterial->GetReflectionColor();
    // fbxMaterial->GetReflectionFactor();
  }

  this->materials[fbxMaterial] = soMaterial;
  return soMaterial;
}

/*!
  Work in progress:
  Analyze all FBX textures and create a list of actual Coin-compatible textures.
 */
FBXFileReader::FbxTextureVector
FBXFileReader::findTextures(KFbxSurfaceMaterial *mat, const char *property)
{
  FbxTextureVector textures;

  KFbxProperty prop = mat->FindProperty(property);
  if (prop.IsValid()) {
    // Check for layered texture
    int numLayeredTextures = prop.GetSrcObjectCount(KFbxLayeredTexture::ClassId);
    if (numLayeredTextures > 0) {
      if (numLayeredTextures == 1) {
        KFbxLayeredTexture *ltex = 
          KFbxCast<KFbxLayeredTexture>(prop.GetSrcObject(KFbxLayeredTexture::ClassId, 0));
        assert(ltex);
#if COIN_DEBUG
        SoDebugError::postInfo("FBXFileReader::findTextures",
                               "Layered %s texture found: %s\n",
                               prop.GetName().Buffer(),
                               ltex->GetName());
#endif // COIN_DEBUG
        int numtex = ltex->GetSrcObjectCount(KFbxTexture::ClassId);
        for (int i=0;i<numtex;i++) {
          FbxTexture fbxtex;
          fbxtex.texture = 
            KFbxCast<KFbxTexture>(ltex->GetSrcObject(KFbxTexture::ClassId, i));
          bool res = ltex->GetTextureBlendMode(i, fbxtex.blendmode);
          assert(res);
          textures.push_back(fbxtex);
#if COIN_DEBUG
          SoDebugError::postInfo("FBXFileReader::findTextures",
                                 "  layer %i: %s (blendmode = %d)",
                                 i, fbxtex.texture->GetName(), fbxtex.blendmode);
#endif // COIN_DEBUG
        }
      }
      else {
#if COIN_DEBUG
        SoDebugError::post("FBXFileReader::findTextures",
                           "%d layered textures found. "
                           "Only one layered texture is supported in this case\n",
                           numLayeredTextures);
#endif // COIN_DEBUG
        assert(false);
      }
    }
    else {
      int numTextures = prop.GetSrcObjectCount(KFbxTexture::ClassId);
      if (numTextures == 1) {
        FbxTexture fbxtex;
        fbxtex.texture = KFbxCast<KFbxTexture>(prop.GetSrcObject(KFbxTexture::ClassId, 0));
        assert(fbxtex.texture);
        textures.push_back(fbxtex);
#if COIN_DEBUG
        if (COIN_DEBUG_FBX > 0) {
          SoDebugError::postInfo("FBXFileReader::findTextures",
                                 "  %s", fbxtex.texture->GetName());
        }
#endif
      }
      else if (numTextures > 1) {
#if COIN_DEBUG
        SoDebugError::post("FBXFileReader::findTextures",
                           "%d textures found. "
                           "Only one texture is supported in this case\n",
                           numTextures);
#endif // COIN_DEBUG
        assert(false);
      }
    }
  }
  return textures;
}


/*!
  Prepares FaceSets, Normals, TextureCoordinates, Materials and so on for 
  conversion of a FBX mesh.

  FBX supports arbitrary indexed bindings of textures and materials.
  To Convert to Coin, we must split each FBX mesh into submeshes for
  each unique combination of textures and materials.

  FIXME: Old comment, update:
  A submesh is created for each diffuse texture.
  One default mesh is created too which is considered to be the one 
  using no materials with an attached texture.
  If all submeshes use textures, no default mesh is being created.
  Textures and Bumpmaps use the UV indices of the first layer.
  Some materials are not used by the mesh, so the submeshes created 
  for them must be released later.
  Special attention is not required for unknown material types. Tthe presence of them
  can cause errors in further processing of the mesh (Idea: convert Materials here, so no
  touching of material later is required).

  FIXME: whats in the case that mesh conversion results in degenerated polygons?
*/
void
FBXFileReader::createSubMeshes(KFbxMesh * fbxMesh, SubMeshMap & submeshes)
{
  KFbxLayer * fbxLayer = fbxMesh->GetLayer(0);
  assert(fbxLayer); // At the moment, we require layer 0 to be present

  int materialCount = fbxMesh->GetNode()->GetMaterialCount();

  // If no materials are found, create a submesh for the default material
  if (materialCount == 0) {
    SubMeshInfo submeshinfo;
    submeshinfo.faceset = new SoIndexedFaceSet();
    submeshinfo.faceset->normalIndex.setNum(0);
    submeshinfo.faceset->textureCoordIndex.setNum(0);
    submeshinfo.faceset->coordIndex.setNum(0);
    submeshinfo.refAll();
    submeshes[-1] = submeshinfo;
  }

  KFbxProperty fbxBumpProperty;
  for (int i=0;i<materialCount;i++) {
    KFbxSurfaceMaterial * fbxMaterial = fbxMesh->GetNode()->GetMaterial(i);
    SubMeshInfo submeshinfo;
    submeshinfo.material = convertMaterial(fbxMaterial);
    submeshinfo.faceset = new SoIndexedFaceSet();
    submeshinfo.faceset->normalIndex.setNum(0);
    submeshinfo.faceset->textureCoordIndex.setNum(0);
    submeshinfo.faceset->coordIndex.setNum(0);

#if COIN_DEBUG
    if (COIN_DEBUG_FBX > 0) {
      SoDebugError::postInfo("FBXFileReader::createSubMeshes",
                             "Mat: %s",   fbxMaterial->GetName());
    }
#endif

    // Check for unsupported textures
#if COIN_DEBUG
    const int NUMUNSUPPORTED = 12;
    const char *unsupported[NUMUNSUPPORTED] = {
      KFbxSurfaceMaterial::sDiffuseFactor,
      KFbxSurfaceMaterial::sEmissive,
      KFbxSurfaceMaterial::sEmissiveFactor,
      KFbxSurfaceMaterial::sAmbient,
      KFbxSurfaceMaterial::sAmbientFactor,
      KFbxSurfaceMaterial::sSpecular,
      KFbxSurfaceMaterial::sSpecularFactor,
      KFbxSurfaceMaterial::sShininess,
      KFbxSurfaceMaterial::sNormalMap,
      KFbxSurfaceMaterial::sTransparencyFactor,
      KFbxSurfaceMaterial::sReflection,
      KFbxSurfaceMaterial::sReflectionFactor
    };
    for (int j=0;j<NUMUNSUPPORTED;j++) {
      KFbxProperty tmpProp = fbxMaterial->FindProperty(unsupported[j]);
      if (tmpProp.IsValid() && tmpProp.GetSrcObjectCount(KFbxTexture::ClassId) > 0) {
        SoDebugError::post("FBXFileReader::createSubMeshes",
                           "Unsupported material texture property detected: %s\n",
                           unsupported[j]);
      }
    } 
#endif // COIN_DEBUG
 
    FbxTextureVector diffuseTextures =
      findTextures(fbxMaterial, KFbxSurfaceMaterial::sDiffuse);
    FbxTextureVector alphaTextures =
      findTextures(fbxMaterial, KFbxSurfaceMaterial::sTransparentColor);
    FbxTextureVector bumpTextures =
      findTextures(fbxMaterial, KFbxSurfaceMaterial::sBump);
    for (FbxTextureVector::iterator iter = diffuseTextures.begin();
         iter != diffuseTextures.end();
         iter++) {
      // FIXME: take blendmode into account
      TexInfo texinfo;
      texinfo.textrans = convertTextureTransform<SoTexture2Transform>(iter->texture);
      texinfo.texture = convertTexture<SoTexture2>(iter->texture);
      submeshinfo.textures.push_back(texinfo);
      // In Maya, diffuse textures are replacing diffuseColor.
      // By using MODULATE in Coin and setting diffuseColor to white,
      // we can emulate this effect.
      submeshinfo.material->diffuseColor.setValue(1.0f, 1.0f, 1.0f);
      
      if (diffuseTextures.size() == 1 && alphaTextures.size() == 1) {
        if (alphaTextures[0].texture != diffuseTextures[0].texture) {
#if COIN_DEBUG
          SoDebugError::post("FBXFileReader::createSubMeshes",
                             "Alpha texture different from diffuse texture. ");
#endif // COIN_DEBUG
        }
        else {
          // Set transparency to 0 to replace transparency values with the texture
          submeshinfo.material->transparency.setValue(0.0f);
#if COIN_DEBUG
          SoDebugError::postInfo("FBXFileReader::createSubMeshes",
                                 "Alpha texture found.");
#endif // COIN_DEBUG
        }
      }
    }

    for (FbxTextureVector::iterator iter = bumpTextures.begin();
         iter != bumpTextures.end();
         iter++) {
      TexInfo texinfo;
      texinfo.textrans = convertTextureTransform<SoBumpMapTransform>(iter->texture);
      texinfo.texture = convertTexture<SoBumpMap>(iter->texture);
      submeshinfo.textures.push_back(texinfo);
    }

    submeshinfo.refAll();
    submeshes[i] = submeshinfo;
  }
}


/*!
  Creates Coin normal indices for the given mesh and writes them to the
  normalIndex fields of the given submeshes.
*/
void
FBXFileReader::getNormals(KFbxMesh * fbxMesh, SubMeshMap & submeshes) 
{
  assert(fbxMesh && "FBXFileReader::getNormals: No mesh given.");

  KFbxLayer * fbxLayer;

  if (NULL != (fbxLayer = fbxMesh->GetLayer(0))) {
    KFbxLayerElementNormal * fbxNormals = fbxLayer->GetNormals();
    KFbxLayerElementMaterial * fbxMaterials = fbxLayer->GetMaterials();

    // nothing to do if no normals given.
    if (!fbxNormals) return;

    SoIndexedFaceSet * soFaceSet = NULL;

    int polygonCount = fbxMesh->GetPolygonCount();
    int polygonPointCount;
    int polygonPointNo;
    int indexNo = 0;
    int index;
    int size = 0;    
    int matIndex;

    for (int polygonNo = 0; polygonNo < polygonCount; ++polygonNo) {
      // FIXME: Look for degenerate polygons (<3 vertices)?
      polygonPointCount = fbxMesh->GetPolygonSize(polygonNo);
      for (polygonPointNo = 0; polygonPointNo < polygonPointCount; ++polygonPointNo, ++indexNo) {
        index = fbxMesh->GetPolygonVertex(polygonNo, polygonPointNo);
        if (fbxMaterials) {
          matIndex = getIndex<KFbxSurfaceMaterial *>(*fbxMaterials, index, indexNo, polygonNo);
        } else {
          matIndex = -1;
        } // if else

        SubMeshMap::iterator iter;
        if ((iter = submeshes.find(matIndex)) != submeshes.end()) {
          soFaceSet = iter->second.faceset;  
        } else {
          soFaceSet = submeshes[-1].faceset;
        }

        assert(soFaceSet && "FBXFileReader::getNormals: no FaceSet for the given material index found.");

        if (fbxNormals) {
          size = soFaceSet->normalIndex.getNum();
          soFaceSet->normalIndex.setNum(size + 1);
          soFaceSet->normalIndex.set1Value(size, getIndex(*fbxNormals, index, indexNo, polygonNo));
        }
      } // for
      // FIXME: This looks a little fishy. What if matIndex changes during the previous loop? See also getUVs
      if (polygonPointCount > 0) {
        if (fbxNormals) {
          size = soFaceSet->normalIndex.getNum();
          soFaceSet->normalIndex.setNum(size + 1);
          soFaceSet->normalIndex.set1Value(size, SO_END_FACE_INDEX);
        }
      }
    }  // for
  } // if
}

/*!
  Copies texture coordinates for all layers and creates the corresponding Coin nodes.
  Returns a list of Coin nodes which can be added together with coords and normals.

  Also creates Coin texture coordinate indices and writes them to the
  textureCoordIndex fields of the given submeshes.
*/
SoNodeList
FBXFileReader::getUVs(KFbxMesh * fbxMesh, SubMeshMap & submeshes)
{
  SoNodeList coinUVs;

  // Find all UV sets
  std::vector<KFbxLayerElementUV*> fbxuvarrays;
  for (int i=0;i<fbxMesh->GetLayerCount();i++) {
    KFbxLayer * fbxLayer = fbxMesh->GetLayer(i);
    KFbxLayerElementUV * fbxUVs = fbxLayer->GetDiffuseUV();
    if (fbxUVs) fbxuvarrays.push_back(fbxUVs);
  }

  SoIndexedFaceSet * soFaceSet = NULL;

  int polygonCount = fbxMesh->GetPolygonCount();
  int polygonPointCount;
  int polygonPointNo;
  int indexNo = 0;
  int index;
  int matIndex;
  
  KFbxLayer * fbxLayer = fbxMesh->GetLayer(0);
  KFbxLayerElementMaterial * fbxMaterials = fbxLayer->GetMaterials();
  Reindexer<Tuple<KFbxVector2> > reindexer;

  for (int polygonNo = 0; polygonNo < polygonCount; ++polygonNo) {
    // FIXME: Look for degenerate polygons (<3 vertices)?
    polygonPointCount = fbxMesh->GetPolygonSize(polygonNo);
    for (polygonPointNo = 0; polygonPointNo < polygonPointCount; ++polygonPointNo, ++indexNo) {
      index = fbxMesh->GetPolygonVertex(polygonNo, polygonPointNo);
      
      if (fbxMaterials) {
        matIndex = getIndex<KFbxSurfaceMaterial *>(*fbxMaterials, index, indexNo, polygonNo);
      } else {
        matIndex = -1;
      }
      SubMeshMap::iterator iter;
      if ((iter = submeshes.find(matIndex)) != submeshes.end()) {
        soFaceSet = iter->second.faceset;  
      } else {
        soFaceSet = submeshes[-1].faceset;
      }
      
      assert(soFaceSet && "FBXFileReader::getUVs: no FaceSet for the given material index found.");
      
      Tuple<KFbxVector2> tuple;
      std::vector<KFbxLayerElementUV*>::const_iterator uviter;
      for (uviter = fbxuvarrays.begin(); uviter != fbxuvarrays.end(); uviter++) {
        tuple.push_back((*uviter)->GetDirectArray()[getIndex(**uviter, index, indexNo, polygonNo)]);
      }
      int newindex = reindexer.lookup(tuple);
      soFaceSet->textureCoordIndex.set1Value(soFaceSet->textureCoordIndex.getNum(),
                                             newindex);
    }
    
    if (polygonPointCount > 0) {
      soFaceSet->textureCoordIndex.set1Value(soFaceSet->textureCoordIndex.getNum(),
                                             SO_END_FACE_INDEX);
    }
  }

  for (int i=0;i<fbxuvarrays.size();i++) {
    int uvCount = reindexer.size();
    SoTextureCoordinate2 *soTC2 = new SoTextureCoordinate2();
    soTC2->point.setNum(uvCount);
    const Tuple<KFbxVector2> *array = reindexer.getArray();
    SbVec2f *coinuvs = soTC2->point.startEditing();
    for (int j=0;j<uvCount;j++) {
      coinuvs[j] = FBX2IVHelpers::convert<SbVec2f>(array[j][i]);
    }
     soTC2->point.finishEditing();
     coinUVs.append(soTC2);
  }

  return coinUVs;
}

/*!
  Converts an FBX Mesh into an SoIndexedFaceSet.
  Normal and TextureCoordinate indices will be optimized to PER_FACE or OVERALL
  binding if possible. Unused Normal and TextureCoordinate values will be removed.
*/
void 
FBXFileReader::convertMeshToIndexedFaceSet(KFbxMesh * fbxMesh, SoGroup * parent)
{
  int * indices = fbxMesh->GetPolygonVertices();
  int polygonCount = fbxMesh->GetPolygonCount();
  int pointCount = fbxMesh->GetControlPointsCount();
  int indicesCount = fbxMesh->GetPolygonVertexCount();
  KFbxVector4 * vertices = fbxMesh->GetControlPoints();

  if (!polygonCount || !pointCount || !vertices || !indicesCount || !indices) {
#if COIN_DEBUG
    SoDebugError::post("FBXFileReader::convertMeshToIndexedFaceSet",
                       "Empty or invalid mesh: %s.",
                       fbxMesh->GetName());
#endif // COIN_DEBUG
    return;
  }

  SubMeshMap submeshes;
  createSubMeshes(fbxMesh, submeshes);

  // We need ShapeHints for each mesh (but not for submeshes)
  // FIXME: Shapehints are tricky to get right. Currently this
  // fails miserably for objects with negative scale factors.
  // Since this appears to be common (e.g. when mirroring objects),
  // shapehints are disableed for now.
//   SoShapeHints * soShapeHints = new SoShapeHints();
//   soShapeHints->faceType = SoShapeHints::UNKNOWN_FACE_TYPE;
//   if (fbxMesh->CheckIfVertexNormalsCCW()) {    
//     soShapeHints->vertexOrdering = SoShapeHints::COUNTERCLOCKWISE;
//   } 

  // Copy normals
  // FIXME: To detect same vertex normals it would be interesting to have a BSPTree 
  // which compares the vector components so that small variations in normals won't
  // produce distinct vertices. This happens often after normalization of vectors.
  SoNormal * soNormals = NULL;
  KFbxLayer * fbxLayer0 = fbxMesh->GetLayer(0);
  assert(fbxLayer0);
  KFbxLayerElementNormal * fbxNormals = fbxLayer0->GetNormals();
  if (fbxNormals) {
    const KFbxLayerElementArrayTemplate<KFbxVector4> & normals = 
      fbxNormals->GetDirectArray();
    int normalCount = normals.GetCount();
    soNormals = new SoNormal();
    soNormals->vector.setNum(normalCount);
    SbVec3f *coinnormals = soNormals->vector.startEditing();
    for (int i=0;i<normalCount;i++) {
      coinnormals[i] = FBX2IVHelpers::convert<SbVec3f>(normals[i]);
#if COIN_DEBUG
      float normlength = coinnormals[i].length();
      if (normlength < 0.9f || normlength > 1.1f) {
        static uint32_t normlength_error = 0;
        if (normlength_error < 1) {
          SoDebugError::post("FBXFileReader::convertMeshToIndexedFaceSet",
                             "Non-normalized normal detected: [%.2f %.2f %.2f]\n"
                             "(this warning will be printed only once, "
                             "but there might be more errors).",
                             coinnormals[i][0], coinnormals[i][1], coinnormals[i][2]);
        }
        normlength_error++;
      }
#endif // COIN_DEBUG
    }
    soNormals->vector.finishEditing();
  }

  // Copy texture coordinates

  SoNodeList coinUVs = getUVs(fbxMesh, submeshes);
  // FIXME: FBX saves two UV sets for textures having an alpha channel.
  // The second UV set is always identical to the first (but has separate
  // pointers since it's duplicated in the file). Thus, the second
  // UV set should be ignored.

  int maxnumtextures = 0;
  for (SubMeshMap::iterator texiter = submeshes.begin(); texiter != submeshes.end(); texiter++) {
    int num = texiter->second.textures.size();
    if (num > maxnumtextures) maxnumtextures = num;
  }
  if (coinUVs.getLength() > maxnumtextures) {
    coinUVs.truncate(maxnumtextures);
#if COIN_DEBUG
    static uint32_t superfluousuv_error = 0;
    if (superfluousuv_error < 1) {
      SoDebugError::postInfo("FBXFileReader::convertMeshToIndexedFaceSet",
                             "Superfluous UV coordinates detected and ignored "
                             "for mesh \"%s\"\n"
                             "(this warning will be printed only once, "
                             "but there might be more errors).",
                             fbxMesh->GetName());
    }
    superfluousuv_error++;
#endif // COIN_DEBUG
  }

  getNormals(fbxMesh, submeshes);

  SoMaterial * soMaterials  = NULL;
  SoMaterialBinding * soMaterialBinding = NULL;
  SoBaseColor * soColors = NULL;

  KFbxLayerElementVertexColor * fbxColors = NULL;
  KFbxLayerElementMaterial * fbxMaterials = NULL;

  bool useColors = false;

  // if no layer information is present we only convert geometry
  if (fbxLayer0) {
    fbxColors = fbxLayer0->GetVertexColors();
    fbxMaterials = fbxLayer0->GetMaterials();

    // check either we take materials, vertex colors or a single color
    // if there are materials present only materials will be used in the converted 
    // result. If no materials or only one material is present the vertex color array 
    // will be used if present. And at last if no color per vertex is given and no 
    // materials the Color property of the mesh will be used for all vertices.
    if (!fbxMaterials && fbxColors) {
      soMaterialBinding = new SoMaterialBinding();
      soMaterialBinding->value = SoMaterialBinding::PER_VERTEX;
      soMaterials = new SoMaterial();
      useColors = true;
    } else if (fbxMaterials && fbxColors) {
      if (fbxMaterials->GetMappingMode() == KFbxLayerElementMaterial::eALL_SAME) {
        soMaterialBinding = new SoMaterialBinding();
        soMaterialBinding->value = SoMaterialBinding::PER_VERTEX;
        soMaterials = new SoMaterial();
        useColors = true;
      }
    } else if (!fbxMaterials && !fbxColors) {
      KFbxProperty prop = fbxMesh->FindProperty("Color");
      KFbxColor nodeColor;
      if (prop.IsValid() && prop.Get(&nodeColor, eDOUBLE3)) {
        soColors = new SoBaseColor();
        soColors->rgb.setValue(nodeColor.mRed, nodeColor.mGreen, nodeColor.mBlue);
      }
    }
  } // if (fbxLayer0)

  int indexNo = 0;
  int polygonPointCount;
  int polygonPointNo = 0;

  for (int polygonNo = 0; polygonNo < polygonCount; ++polygonNo) {
    // Find material for this polygon
    int matIndex = -1;
    if (fbxMaterials) {
      if (useColors) {
        matIndex = 0;
      } else {
        matIndex = getIndex<KFbxSurfaceMaterial *>(*fbxMaterials, 0, 
                                                   indexNo, polygonNo);
      }
    }
    SubMeshMap::iterator iter = submeshes.find(matIndex);
    if (iter == submeshes.end()) iter = submeshes.find(-1);
    assert(iter != submeshes.end() && "No default faceset created.");
    SubMeshInfo &info = iter->second;
    info.used = true;
      
    polygonPointCount = fbxMesh->GetPolygonSize(polygonNo);
    for (polygonPointNo = 0; polygonPointNo < polygonPointCount; ++polygonPointNo, ++indexNo) {
      int fbxindex = fbxMesh->GetPolygonVertex(polygonNo, polygonPointNo);
      int size = info.faceset->coordIndex.getNum();
      info.faceset->coordIndex.setNum(size + 1);
      info.faceset->coordIndex.set1Value(size, fbxindex);
      
      if (useColors) {
        soMaterials->diffuseColor.
          set1Value(indexNo, getValue<SbColor>(*fbxColors, fbxindex, indexNo, polygonNo));
      }
    } // for (polygonPointNo
    
    if (polygonPointCount > 0) {
      int size = info.faceset->coordIndex.getNum();
      info.faceset->coordIndex.setNum(size + 1);
      info.faceset->coordIndex.set1Value(size, SO_END_FACE_INDEX);
    }
  } // for (polygonNo

  // See comment above about shapehints
  //  addNonDefault(parent, (SoNode **)&soShapeHints, defaults);
  addNonDefault(parent, (SoNode **)&soNormals, defaults);
  bool addeduv = false;
  int unit = 0;
  for (int i=0;i<coinUVs.getLength();i++) {
    if (i > 0) {
      SoTextureUnit *texunit = new SoTextureUnit;
      texunit->unit = ++unit;
      parent->addChild(texunit);  // Default nodes may change state
    }
    parent->addChild(coinUVs[i]);
    addeduv = true;
  }
  if (unit > 0) parent->addChild(new SoTextureUnit);
  addNonDefault(parent, (SoNode **)&soColors, defaults);
  addNonDefault(parent, (SoNode **)&soMaterials, defaults);
  addNonDefault(parent, (SoNode **)&soMaterialBinding, defaults);

  // copy vertices
  // FIXME: should we regenerate vertices and vertex indices using the Reindexer?
  // (kintel 20090401).
  SoCoordinate3 * soCoords = new SoCoordinate3();
  soCoords->point.setNum(pointCount);
  SbVec3f *points = soCoords->point.startEditing();
  for (int i=0;i<pointCount;i++) {
    points[i] = FBX2IVHelpers::convert<SbVec3f>(vertices[i]);
  }
  soCoords->point.finishEditing();
  parent->addChild(soCoords);

  SoGroup * subMeshSep = NULL;
  SoNormalBinding * soNormalBinding;
  bool forcePerFace;
  bool needSeparator;
  SoMaterial * soMaterial;
  bool allMaterialsOverall = true;

  // add facesets with preceeding textures, bump maps and normal- and material bindings
  for (SubMeshMap::iterator iter = submeshes.begin(); iter != submeshes.end(); iter++) {
    if (iter->second.used) {
      soNormalBinding = NULL;
      soMaterialBinding = NULL;
      soMaterial = NULL;
      // this variable stores information about the need of an additional separator
      // due to a non default binding or the use of a texture or bump map.
      needSeparator = false;
    
      if (soNormals) {
        // we have normal indices which can be reorganized for another binding style.
        // force per face indexing if this was the original setting in FBX.
        forcePerFace = KFbxLayerElement::eBY_POLYGON == fbxNormals->GetMappingMode();
        soNormalBinding = 
          createBindingAndOptimizedIndices<SoNormalBinding>(iter->second.faceset->normalIndex, 
                                                            defaults, 
                                                            forcePerFace);
        if (soNormalBinding) {
          needSeparator = true;
        }
      } else {
        iter->second.faceset->normalIndex.setNum(0);
        iter->second.faceset->normalIndex.setDefault(true);
      }

      if (!addeduv) {
        iter->second.faceset->textureCoordIndex.setNum(0);
        iter->second.faceset->textureCoordIndex.setDefault(true);
      }

      if (fbxMaterials && !useColors) {
        soMaterialBinding = 
          createBindingAndOptimizedIndices<SoMaterialBinding>(iter->second.faceset->materialIndex,
                                                              defaults);
        if (soMaterialBinding) {
          needSeparator = true;
          allMaterialsOverall = false;
        } else {
          soMaterial = iter->second.material;
        }
      } 

      if (iter->second.textures.size() > 0) needSeparator = true;
      
      // FIXME: what is if size is greater than 1 but have unused materials ?
      needSeparator = needSeparator && submeshes.size() > 1;

      // create an additional separator if we need one.
      if (needSeparator) {
        subMeshSep = new SoSeparator();
        parent->addChild(subMeshSep);
      } else {
        subMeshSep = parent;
      }
      
      // compose the submesh from its components
      if (soMaterial) {
        subMeshSep->addChild(soMaterial);
      }
      if (soNormalBinding) {
        subMeshSep->addChild(soNormalBinding);
      }
      if (soMaterialBinding) {
        subMeshSep->addChild(soMaterialBinding);
      }
      int unit = 0;
      for (TexInfoVector::iterator texiter = iter->second.textures.begin();
           texiter != iter->second.textures.end();
           texiter++) {
        if (texiter != iter->second.textures.begin()) {
          SoTextureUnit *texunit = new SoTextureUnit;
          texunit->unit = ++unit;
          subMeshSep->addChild(texunit);  // Default nodes may change state
        }
        if (texiter->textrans) {
          addNonDefault(subMeshSep, (SoNode **)&texiter->textrans, defaults);
        }
        if (texiter->texture) {
          subMeshSep->addChild(texiter->texture);
        }
        if (texiter->bump) {
          subMeshSep->addChild(texiter->bump);
        }
      }
      if (unit > 0) parent->addChild(new SoTextureUnit);
      addNonDefault(subMeshSep, (SoNode **)&iter->second.faceset, defaults);
    } 

    // free nodes that have not been used
    iter->second.unrefAll();
  } // for

  // remove materials if all materials are inside the submeshes
  if (allMaterialsOverall && soMaterials && !useColors) {
    parent->removeChild(soMaterials);
  }
}

/*!
  Converts an FBX camera and stores it for post-processing.

  After the complete scene has been read the cameras will be moved in
  front of the scene graph with their positions and directions
  calculated from the transfomation found in the FBX scene.
  /sa postProcessCameras().
*/
void
FBXFileReader::convertCamera(KFbxCamera * fbxCamera, SoGroup * soGroup)
{
  SoCamera * soCamera = NULL;

  soCamera = new SoPerspectiveCamera;
  soCamera->setName(fbxCamera->GetName());
  soCamera->position = convert<SbVec3f>(fbxCamera->Position.Get());

  if (soCamera) {
    this->cameras[soCamera] = soGroup;
    soGroup->addChild(soCamera);
  }
}


/*!
  Converts an FBX light and stores it for post-processing.

  After the complete scene has been read the lights will be moved in
  front of the scene graph with their positions and directions
  calculated from the transfomation found in the FBX scene.
  /sa postProcessLights().
*/
void
FBXFileReader::convertLight(KFbxLight * fbxLight, SoGroup * soGroup)
{
  SoLight * soLight = NULL;

  // The light's position and direction are calculated after conversion
  // of the other parts of the scene in postProcessLights(). 
  switch (fbxLight->LightType.Get()) {
    case KFbxLight::eDIRECTIONAL: {
      SoDirectionalLight * soDirectionalLight = new SoDirectionalLight();
      soDirectionalLight->direction.setValue(0.0f, 0.0f, -1.0f);
      soLight = soDirectionalLight;
    }
    break;
    case KFbxLight::eSPOT: {
      SoSpotLight * soSpotLight = new SoSpotLight();
      soSpotLight->location.setValue(0.0f, 0.0f, 0.0f);
      // we take the average of inner and outer cone angles
      soSpotLight->cutOffAngle = (fbxLight->HotSpot.Get() + fbxLight->ConeAngle.Get()) / 2.0f;
      soSpotLight->direction.setValue(0.0f, 0.0f, -1.0f);
      // FIXME: how do we convert dropOff ?
      soLight = soSpotLight;
    }
    break;
    case KFbxLight::ePOINT: {
      SoPointLight * soPointLight = new SoPointLight();
      soPointLight->location.setValue(0.0f, 0.0f, 0.0f);
      soLight = soPointLight;
    }
    break;
    default:
#ifdef COIN_DEBUG
    SoDebugError::post("FBXFileReader::convertLight",
                       "Unknown light type. The light source will be ignored.");
#endif
    break;
  }

  if (soLight) {
    soLight->intensity = fbxLight->Intensity.Get() / 100.0f;
    soLight->color.setValue(convert<SbColor>(fbxLight->Color.Get()));
    this->lights[soLight] = soGroup;
    soGroup->addChild(soLight);
  }
}

/*!
  Calculates absolute positions and directions of light sources from the
  transformation hierarchy in front of a FBX light attribute.
  FIXME: Add mechanism to update the lights position and direction if the 
  containg node or any parent node is being transformed.
  (maybe it could be more confortable to let the hierachy down to
  the light inside the model to easily calculate the new matrix for
  the light's position and direction).
 */
void
FBXFileReader::postProcessLights(SoSeparator * soScene)
{
  SbViewportRegion vp(100, 100);
  SoGetMatrixAction gma(vp);
  SoSearchAction sa;
  static SbVec3f origin(0.0f, 0.0f, 0.0f);
  static SbVec3f Z(0.0f, 0.0f, -1.0f);
  SbVec3f location;
  SbVec3f direction;

  for (LightMap::iterator iter = this->lights.begin(); iter != this->lights.end(); iter++) {
    SoLight * light = iter->first;
    // FIXME: hardcoded index - scary
    SoTransformation * transform = (SoTransformation *)iter->second->getChild(0);

    // get path to the transformation of this light
    // FIXME: If we have to search anyway, why do we store the light in a map in the first
    // place?
    sa.reset();
    sa.setNode(light);
    sa.apply(soScene);
    SoPath * path = sa.getPath();

    if (path && path->getLength() > 1) {
      gma.apply(path);
      SbMatrix matrix = gma.getMatrix();      
      matrix.multVecMatrix(origin, location);
      SbVec3f translation;
      SbRotation rotation;
      SbVec3f scalevector;
      SbRotation scaleorientation;

      // extract transformation parts for rotating the lights direction
      matrix.getTransform(translation, rotation, scalevector, scaleorientation);
      rotation.multVec(Z, direction);
      direction.normalize();
   
      if (light->getTypeId() == SoPointLight::getClassTypeId()) {
        ((SoPointLight *)light)->location = location;
      } else if (light->getTypeId() == SoDirectionalLight::getClassTypeId()) {
        ((SoDirectionalLight *)light)->direction = direction;
      } else if (light->getTypeId() == SoSpotLight::getClassTypeId()) {
        ((SoSpotLight *)light)->location = location;
        ((SoSpotLight *)light)->direction = direction;
      }
      resetDefaultFields(light, defaults);
      
      // put the lights in front of the scene
      soScene->insertChild(light, 0);
      
      // FIXME: find a better way to remove the unused path.
      // The question is also whether we should remove it at all,
      // since we could need it to recalculate the new position and 
      // direction of the lights when the hierarchy above the (original) light
      // is being transformed (e.g. when a light is following an object).
      SoGroup * main = getParent(path, light);
      if (main) {
        SoGroup * main1 = getParent(path, main);
        if (main1) {
          SoGroup * main2 = getParent(path, main1);
          if (main2) {
            main2->removeChild(main1);
          }
        }
      }
    }

    sa.reset();
  }
}

/*!
  FIXME: doc
 */
void
FBXFileReader::postProcessCameras(SoSeparator * soScene)
{
  SbViewportRegion vp(100, 100);
  SoGetMatrixAction gma(vp);
  SoSearchAction sa;
  static SbVec3f origin(0.0f, 0.0f, 0.0f);
  static SbVec3f Z(0.0f, 0.0f, -1.0f);
  SbVec3f location;
  SbVec3f direction;

  for (CameraMap::iterator iter = this->cameras.begin(); iter != this->cameras.end(); iter++) {
    SoCamera * camera = iter->first;
    // FIXME: hardcoded index - scary
    SoTransformation * transform = (SoTransformation *)iter->second->getChild(0);

    // get path to the transformation of this camera
    // FIXME: If we have to search anyway, why do we store the camera in a map in the first
    // place?
    sa.reset();
    sa.setNode(camera);
    sa.apply(soScene);
    SoPath * path = sa.getPath();

    if (path && path->getLength() > 1) {
      gma.apply(path);
      SbMatrix matrix = gma.getMatrix();      
      matrix.multVecMatrix(origin, location);
      SbVec3f translation;
      SbRotation rotation;
      SbVec3f scalevector;
      SbRotation scaleorientation;

      // extract transformation parts for rotating the cameras direction
      matrix.getTransform(translation, rotation, scalevector, scaleorientation);
   
      camera->position = location;
      camera->orientation = rotation;
      resetDefaultFields(camera, defaults);
      
      // put the cameras in front of the scene
      soScene->insertChild(camera, 0);
      
      // FIXME: find a better way to remove the unused path.
      // The question is also whether we should remove it at all,
      // since we could need it to recalculate the new position and 
      // direction of the cameras when the hierarchy above the (original) camera
      // is being transformed (e.g. when a camera is following an object).
      SoGroup * main = getParent(path, camera);
      if (main) {
        SoGroup * main1 = getParent(path, main);
        if (main1) {
          SoGroup * main2 = getParent(path, main1);
          if (main2) {
            main2->removeChild(main1);
          }
        }
      }
    }

    sa.reset();
  }
}


/*!
  Creates a SoNode by converting the given KFbxAttribute and
  adds it to the given parent SoNode.
  If there is no appropriate conversion for the given KFbxAttribute
  nothing will be done.
  Currenty supported attribute types:
  - KFbxMesh
  - KFbxLight
  Returns true if the given attribute has been converted and added to
  parent otherwise false.
*/
bool
FBXFileReader::addAttribute(KFbxNodeAttribute * attribute, SoGroup * parent) 
{
  static uint32_t skipped_attr[] = {
    0,0,0,0,0,
    0,0,0,0,0,
    0,0,0,0,0,
    0,0,0
  };

  assert(attribute);

  KFbxNodeAttribute::EAttributeType attributeType = attribute->GetAttributeType();
  switch (attributeType) {
    case KFbxNodeAttribute::eMESH: {
      KFbxMesh * fbxMesh = KFbxCast<KFbxMesh>(attribute);
      assert(fbxMesh);
      convertMeshToIndexedFaceSet(fbxMesh, parent);
      return true;
    }
    break;

    case KFbxNodeAttribute::eLIGHT: {
      KFbxLight * fbxLight = KFbxCast<KFbxLight>(attribute);
      if (fbxLight) {
        convertLight(fbxLight, parent);
        return true;
      }
    }
    break;
    case KFbxNodeAttribute::eCAMERA:
		{
#ifdef COIN_DEBUG
      SoDebugError::postInfo("FBXFileReader::addAttribute",
                             "Found camera.");
#endif // COIN_DEBUG
      KFbxCamera * fbxCamera = KFbxCast<KFbxCamera>(attribute);
      if (fbxCamera) {
        convertCamera(fbxCamera, parent);
        return true;
      }
      break;
		}
    case KFbxNodeAttribute::eUNIDENTIFIED:
    case KFbxNodeAttribute::eNULL:
    case KFbxNodeAttribute::eMARKER:
    case KFbxNodeAttribute::eSKELETON:
    case KFbxNodeAttribute::eNURB:
    case KFbxNodeAttribute::ePATCH:
    case KFbxNodeAttribute::eCAMERA_SWITCHER:
    case KFbxNodeAttribute::eOPTICAL_REFERENCE:
    case KFbxNodeAttribute::eOPTICAL_MARKER:
    case KFbxNodeAttribute::eNURBS_CURVE:
    case KFbxNodeAttribute::eTRIM_NURBS_SURFACE:
    case KFbxNodeAttribute::eBOUNDARY:
    case KFbxNodeAttribute::eNURBS_SURFACE:
    case KFbxNodeAttribute::eSHAPE:
    case KFbxNodeAttribute::eLODGROUP:
#ifdef COIN_DEBUG
      if (skipped_attr[attributeType] < 1) {
        SoDebugError::postInfo("FBXFileReader::addAttribute",
                               "Skipped attribute %s "
                               "(this warning will be printed only once, "
                               "but there might be more errors).",
                               attributeNames[attributeType]);
      }
      skipped_attr[attributeType]++;
#endif // COIN_DEBUG
      break;
    default:
#ifdef COIN_DEBUG
      SoDebugError::post("FBXFileReader::addAttribute",
                         "Skipped unknown attribute.",
                         attributeNames[attributeType]);
#endif // COIN_DEBUG
      break;
  }
  return false;
}
