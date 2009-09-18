#ifndef FBXFILEREADER_H
#define FBXFILEREADER_H

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

#include <fbxsdk.h>
#include <map>
#include <list>
#include <vector>


#include <Inventor/nodes/SoMaterialBinding.h>

class SoNode;
class SoGroup;
class SoSeparator;
class SoMaterial;
class SoIndexedFaceSet;
class SoBumpMap;
class SoLight;
class SoCamera;


class FBXFileReader
{
public:
  FBXFileReader(KFbxSdkManager * sdkManager);

  SoSeparator * read(const char * filename);

private:
  struct FbxTexture {
    FbxTexture() : texture(NULL), blendmode(KFbxLayeredTexture::EBlendMode(-1)) {}
    KFbxTexture *texture;
    KFbxLayeredTexture::EBlendMode blendmode;
  };
  typedef std::vector<FbxTexture> FbxTextureVector;

  struct TexInfo {
  TexInfo() : textrans(NULL), texture(NULL), bump(NULL) {}
    SoNode * textrans;
    SoNode * texture;
    SoBumpMap * bump;
  };
  typedef std::vector<TexInfo> TexInfoVector;

  struct SubMeshInfo {
  SubMeshInfo() : faceset(NULL), material(NULL), used(false) {}
    SoIndexedFaceSet * faceset;
    SoMaterial * material;
    TexInfoVector textures;
    bool used;

    void refAll();
    void unrefAll();
  };
  typedef std::map<int, FBXFileReader::SubMeshInfo> SubMeshMap;

  KFbxScene * loadScene(const char * filename);
  
  SoSeparator * createSceneGraph(KFbxScene * fbxScene);
  
  SoSeparator * convertNode(KFbxNode * fbxNode, KFbxMatrix & parentMatrix);

  bool addAttribute(KFbxNodeAttribute * fbxAttribute, SoGroup * soGroup);
  
  void createMaterialsIndices(KFbxMesh * fbxMesh, SoIndexedFaceSet * faceSet);
    
  void createSubMeshes(KFbxMesh * fbxMesh, std::map<int, FBXFileReader::SubMeshInfo> & result);

  void convertMeshToIndexedFaceSet(KFbxMesh * fbxMesh, SoGroup * soNode);

  void getNormals(KFbxMesh * fbxMesh, 
                  std::map<int, FBXFileReader::SubMeshInfo> & soFaceSets);
  SoNodeList getUVs(KFbxMesh * fbxMesh,
                    std::map<int, FBXFileReader::SubMeshInfo> & soFaceSets);

  void convertLight(KFbxLight * fbxLight, SoGroup * soGroup);
  void convertCamera(KFbxCamera * fbxCamera, SoGroup * soGroup);

  void postProcessLights(SoSeparator * soScene);
  void postProcessCameras(SoSeparator * soScene);

  SoMaterial * convertMaterial(KFbxSurfaceMaterial * fbxMaterial);
  template<typename T> T * convertTexture(KFbxTexture * fbxTexture);
  template<typename T> T * convertTextureTransform(KFbxTexture * fbxTexture);
  FbxTextureVector findTextures(KFbxSurfaceMaterial *mat, const char *property);

  // Cleares the default value map.
  void clearDefaults();

  // stores lights and camera so that they can set up after scene loading
  // without having to search through the scenegraph.
  typedef std::map<SoLight *, SoGroup *> LightMap;
  LightMap lights;
  typedef std::map<SoCamera *, SoGroup *> CameraMap;
  CameraMap cameras;

  // nodes that have to be deleted after conversion due to 
  // empty content.
  std::list<SoNode *> markedForDelete;
  
  // place to store default nodes for comparision
  // this map will be cleared after conversion is complete
  // key = typeId value = default node
  std::map<int, SoNode *> defaults;

  // Keep track of already converted materials for reuse
  typedef std::map<KFbxSurfaceMaterial*, SoMaterial*> MaterialMap;
  MaterialMap materials;

  // Keep track of already converted textures for reuse
  typedef std::map<KFbxTexture*, SoNode*> TextureMap;
  TextureMap textures;

  // FBX SDK manager object
  KFbxSdkManager * sdkManager;
};

#endif
