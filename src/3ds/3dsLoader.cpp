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

//
//  3DS File Loader for Open Inventor
//
//  developed originally by PC John (peciva@fit.vutbr.cz)
//
//
//  Comments about 3ds files: Structure of the 3ds files is now well known
//  (http://www.cyberloonies.com/3dsftk.html). However, it is often hard to
//  understand what is the informations in 3ds file about and how to
//  interpret them. For example, texture coordinates are not always
//  represented by OpenGL texture coordinates, and some unimplemented magic
//  should be done with them.
//
//  TODO list:
//
//  - incomplete texture implementation - in 3ds files there is possible to
//    make materials with about 20 textures (diffuse color texture, specular
//    texture, bump-map texture,... Also texture coordinates are not always
//    loaded right, probably we should read more chunks to get this working
//    right.
//
//  - unimplemented lights
//  - per-vertex normals generation
//  - investigate the color of material-less objects
//  - ?environment? (ambient light, fog,...)
//  - ?emissiveColor? (I am really confused how to get this from 3ds.)
//  - maybe some animations?
//



#include <Inventor/SbPlane.h>
#include <Inventor/SbRotation.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoIndexedTriangleStripSet.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoMaterialBinding.h>
#include <Inventor/nodes/SoMatrixTransform.h>
#include <Inventor/nodes/SoNormal.h>
#include <Inventor/nodes/SoNormalBinding.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTexture2.h>
#include <Inventor/nodes/SoTextureCoordinate2.h>
#include <Inventor/nodes/SoTextureCoordinateBinding.h>
#include <Inventor/nodes/SoTexture2Transform.h>
#include <Inventor/nodes/SoTriangleStripSet.h>
#include <Inventor/SoInput.h>
#include <string.h>
#include "SoStream.h"



// File Header Chunks
#define M3DMAGIC    0x4D4D
#define M3D_VERSION 0x0002
#define MLIBMAGIC   0x3DAA
#define CMAGIC      0xC23D

// Major Section Chunks
#define MDATA  0x3D3D
#define KFDATA 0xB000

// Viewport Control Chunks
#define VIEWPORT_LAYOUT 0x7001
#define VIEWPORT_DATA   0x7011
#define VIEWPORT_DATA_3 0x7012
#define VIEWPORT_SIZE   0x7020


// MDATA Section Chunks
// Common Chunks
#define COLOR_F 0x0010
#define COLOR_24 0x0011
#define LIN_COLOR_24 0x0012
#define LIN_COLOR_F 0x0013
#define INT_PERCENTAGE 0x0030
#define FLOAT_PERCENTAGE 0x0031

// Section Settings Chunks
#define MESH_VERSION 0x3D3E
#define MASTER_SCALE 0x0100
#define LO_SHADOW_BIAS 0x1400
#define HI_SHADOW_BIAS 0x1410
#define SHADOW_MAP_SIZE 0x1420
#define SHADOW_SAMPLES 0x1430
#define SHADOW_RANGE 0x1440
#define SHADOW_FILTER 0x1450
#define RAY_BIAS 0x1460
#define O_CONSTS 0x1500 
#define AMBIENT_LIGHT 0x2100

// Background Settings Chunks
#define BIT_MAP 0x1100
#define SOLID_BGND 0x1200
#define V_GRADIENT 0x1300
#define USE_BIT_MAP 0x1101 
#define USE_SOLID_BGND 0x1201 
#define USE_V_GRADIENT 0x1301

// Atmosphere Settings Chunks
#define FOG 0x2200
#define FOG_BGND 0x2210
#define LAYER_FOG 0x2302
#define DISTANCE_CUE 0x2300 
#define DCUE_BGND 0x2310 
#define USE_FOG 0x2201 
#define USE_LAYER_FOG 0x2303
#define USE_DISTANCE_CUE 0x2301

// Viewport Chunks
#define DEFAULT_VIEW 0x3000
#define VIEW_TOP 0x3010
#define VIEW_BOTTOM 0x3020 
#define VIEW_LEFT 0x3030
#define VIEW_RIGHT 0x3040
#define VIEW_FRONT 0x3050
#define VIEW_BACK 0x3060
#define VIEW_USER 0x3070
#define VIEW_CAMERA 0x3080

// Materials Chunks
#define MAT_ENTRY 0xAFFF 
#define MAT_NAME 0xA000
#define MAT_AMBIENT 0xA010
#define MAT_DIFFUSE 0xA020
#define MAT_SPECULAR 0xA030
#define MAT_SHININESS 0xA040
#define MAT_SHIN2PCT 0xA041
#define MAT_TRANSPARENCY 0xA050
#define MAT_XPFALL 0xA052
#define MAT_USE_XPFALL 0xA240
#define MAT_REFBLUR 0xA053
#define MAT_SHADING 0xA100 
#define MAT_USE_REFBLUR 0xA250
#define MAT_SELF_ILLUM 0xA080
#define MAT_TWO_SIDE 0xA081
#define MAT_DECAL 0xA082 
#define MAT_ADDITIVE 0xA083 
#define MAT_WIRE 0xA085
#define MAT_FACEMAP 0xA088
#define MAT_PHONGSOFT 0xA08C
#define MAT_WIREABS 0xA08E
#define MAT_WIRESIZE 0xA087
#define MAT_TEXMAP 0xA200 
#define MAT_SXP_TEXT_DATA 0xA320
#define MAT_TEXMASK 0xA3EH
#define MAT_SXP_TEXTMASK_DATA 0xA32A
#define MAT_TEX2MAP 0xA33A
#define MAT_SXP_TEXT2_DATA 0xA321
#define MAT_TEX2MASK 0xA340H
#define MAT_SXP_TEXT2MASK_DATA 0xA32C
#define MAT_OPACMAP 0xA210
#define MAT_SXP_OPAC_DATA 0xA322
#define MAT_OPACMASK 0xA342
#define MAT_SXP_OPACMASK_DATA 0xA32E
#define MAT_BUMPMAP 0xA230
#define MAT_SXP_BUMP_DATA 0xA324
#define MAT_BUMPMASK 0xA344
#define MAT_SXP_BUMPMASK_DATA 0xA330
#define MAT_SPECMAP 0xA204
#define MAT_SXP_SPEC_DATA 0xA325
#define MAT_SPECMASK 0xA348
#define MAT_SXP_SPECMASK_DATA 0xA332
#define MAT_SHINMAP 0xA33C
#define MAT_SXP_SHIN_DATA 0xA326
#define MAT_SHINMASK 0xA346
#define MAT_SXP_SHINMASK_DATA 0xA334
#define MAT_SELFIMAP 0xA33D
#define MAT_SXP_SELFI_DATA 0xA328
#define MAT_SELFIMASK 0xA34A
#define MAT_SXP_SELFIMASK_DATA 0xA336
#define MAT_REFLMAP 0xA220
#define MAT_REFLMASK 0xA34C
#define MAT_SXP_REFLMASK_DATA 0xA338
#define MAT_ACUBIC 0xA310
#define MAT_MAPNAME 0xA300
#define MAT_MAP_TILING 0xA351
#define MAT_MAT_TEXBLUR 0xA353
#define MAT_MAP_USCALE 0xA354
#define MAT_MAP_VSCALE 0xA356
#define MAT_MAP_UOFFSET 0xA358
#define MAT_MAP_VOFFSET 0xA35A
#define MAT_MAP_ANG 0xA35C
#define MAT_MAP_COL1 0xA360
#define MAT_MAP_COL2 0xA362
#define MAT_MAP_RCOL 0xA364
#define MAT_MAP_GCOL 0xA366
#define MAT_MAP_BCOL 0xA368

// Object Chunks
#define NAMED_OBJECT 0x4000
#define N_TRI_OBJECT 0x4100 
#define POINT_ARRAY 0x4110
#define POINT_FLAG_ARRAY 0x4111
#define FACE_ARRAY 0x4120
#define MSH_MAT_GROUP 0x4130
#define SMOOTH_GROUP 0x4150
#define MSH_BOXMAP 0x4190
#define TEX_VERTS 0x4140
#define MESH_MATRIX 0x4160
#define MESH_COLOR 0x4165
#define MESH_TEXTURE_INFO 0x4170
#define PROC_NAME 0x4181
#define PROC_DATA 0x4182 
#define N_DIRECT_LIGHT 0x4600
#define DL_OFF 0x4620
#define DL_OUTER_RANGE 0x465A
#define DL_INNER_RANGE 0x4659
#define DL_MULTIPLIER 0x465B
#define DL_EXCLUDE 0x4654
#define DL_ATTENUATE 0x4625
#define DL_SPOTLIGHT 0x4610
#define DL_SPOT_ROLL 0x4656
#define DL_SHADOWED 0x4630
#define DL_LOCAL_SHADOW2 0x4641
#define DL_SEE_CONE 0x4650
#define DL_SPOT_RECTANGULAR 0x4651
#define DL_SPOT_ASPECT 0x4657
#define DL_SPOT_PROJECTOR 0x4653
#define DL_SPOT_OVERSHOOT 0x4652
#define DL_RAY_BIAS 0x4658
#define DL_RAYSHAD 0x4627
#define N_CAMERA 0x4700
#define CAM_SEE_CONE 0x4710
#define CAM_RANGES 0x4720
#define OBJ_HIDDEN 0x4010
#define OBJ_VIS_LOFTER 0x4011
#define OBJ_DOESNT_CAST 0x4012
#define OBJ_DONT_RCVSHADOW 0x4017
#define OBJ_MATTE 0x4013
#define OBJ_FAST 0x4014
#define OBJ_PROCEDURAL 0x4015
#define OBJ_FROZEN 0x4016




struct tagFace;
struct tagFaceGroup;
struct tagMaterial;
struct tagContext;


typedef struct {
  SbVec3f point;
  SbVec2f texturePoint;
  SbList<tagFace*> faceList;

  SbVec3f getNormal(tagContext *con, uint16_t myIndex) const;
} Vertex;

typedef struct tagFace {
  uint16_t v1,v2,v3;
  uint16_t flags;
  tagFaceGroup *faceGroup;
  uint32_t e12,e23,e31;
  bool isDegenerated;

  SbVec3f getNormal(tagContext *con) const;
  float getAngle(tagContext *con, uint16_t vertexIndex) const;
  SbVec3f getWeightedNormal(tagContext *con, uint16_t vertexIndex) const;
  void init(tagContext *con, uint16_t a, uint16_t b, uint16_t c, uint16_t f);
} Face;

typedef struct tagFaceGroup {
  tagMaterial *mat;

  SbList<Face*> faceList;
  uint16_t numDegFaces;

  bool hasTexture2(tagContext *con);
  SoTexture2* getSoTexture2(tagContext *con);
  bool hasTexture2Transform(tagContext *con);
  SoTexture2Transform* getSoTexture2Transform(tagContext *con);
  SoMaterial* getSoMaterial(tagContext *con);
  SoNormal* createSoNormal(tagContext *con);

  SoCoordinate3* createSoCoordinate3_n(tagContext *con);
  SoTextureCoordinate2* createSoTextureCoordinate2_n(tagContext *con);
  SoTriangleStripSet* createSoTriStripSet_n(tagContext *con);

  SoIndexedTriangleStripSet* createSoIndexedTriStripSet_i(tagContext *con);
} FaceGroup;

namespace DefaultFaceGroup {
  static tagMaterial* getMaterial(tagContext *con);
  static bool isEmpty(tagContext *con);

  static SoMaterial* getSoMaterial(tagContext *con);
  static SoNormal* createSoNormal(tagContext *con);

  static SoCoordinate3* createSoCoordinate3_n(tagContext *con);
  static SoTriangleStripSet* createSoTriStripSet_n(tagContext *con);

  static SoIndexedTriangleStripSet* createSoIndexedTriStripSet_i(tagContext *con);
};

typedef struct {
  SbList<Face*> faceList;
} Edge;

typedef struct tagMaterial {
  SbString name;
  SbColor ambient;
  SbColor diffuse;
  SbColor specular;
  int16_t shininess;
  int16_t transparency;
  SbString textureName;
  float uscale;
  float vscale;
  float uoffset;
  float voffset;

  SoMaterial *matCache;
  SoTexture2 *texture2Cache;
  SoTexture2Transform *texture2TransformCache;

  void updateSoMaterial(int index, SoMaterial *m);
  SoMaterial* getSoMaterial(tagContext *con);
  bool hasTexture2(tagContext *con);
  SoTexture2* getSoTexture2(tagContext *con);
  bool hasTexture2Transform(tagContext *con);
  SoTexture2Transform* getSoTexture2Transform(tagContext *con);

  tagMaterial() : matCache(NULL), texture2Cache(NULL),
                  texture2TransformCache(NULL)  {}
  ~tagMaterial()  {
    if (matCache) matCache->unref();
    if (texture2Cache) texture2Cache->unref();
    if (texture2TransformCache) texture2TransformCache->unref();
  }
} Material;



typedef struct tagContext {
  // flags "What to load"
  int appendNormals;
  bool loadMaterials;
  bool loadTextures;
  bool loadObjNames;
  bool useIndexedTriSet;
  bool centerModel;

  // basic loading stuff
  SoStream &s;
  size_t stopPos;
  SoSeparator *root;
  SoSeparator *cObj;
  bool minMaxValid;
  float minX,maxX;
  float minY,maxY;
  float minZ,maxZ;

  // material stuff
  SbList<FaceGroup*> faceGroupList;
  SbList<Material*> matList;
  Material defaultMat;
  Material *cMat;
  SbColor cColor;
  int16_t cColorInt;
  bool textureCoordsFound;

  // geometry stuff
  Vertex *vertexList;
  uint16_t numVertices;
  Face *faceList;
  uint16_t numFaces;
  uint16_t numDefaultDegFaces;
  Edge *edgeList;
  uint32_t numEdges;


  SoCoordinate3* createSoCoordinate3_i(tagContext *con) const;
  SoTextureCoordinate2* createSoTextureCoordinate2_i(tagContext *con) const;

  tagContext(SoStream &stream) : s(stream), root(NULL), cObj(NULL),
      vertexList(NULL), faceList(NULL)  {}
  ~tagContext() {
    for (int i=matList.getLength()-1; i>=1; i--) {
      delete matList[i];
    }
    assert(!root && !cObj && !vertexList && !faceList &&
        "Forgot to free some memory.");
  }
} Context;



#define CHUNK_DECL(_name_)  static void _name_(Context *con)

CHUNK_DECL(SkipChunk);
CHUNK_DECL(LoadNamedObject);
CHUNK_DECL(LoadNTriObject);
CHUNK_DECL(LoadPointArray);
CHUNK_DECL(LoadFaceArray);
CHUNK_DECL(LoadMshMatGroup);
CHUNK_DECL(LoadTexVerts);
CHUNK_DECL(LoadMatEntry);
CHUNK_DECL(LoadMatName);
CHUNK_DECL(LoadMatAmbient);
CHUNK_DECL(LoadMatDiffuse);
CHUNK_DECL(LoadMatSpecular);
CHUNK_DECL(LoadShininess);
CHUNK_DECL(LoadTransparency);
CHUNK_DECL(LoadColor24);
CHUNK_DECL(LoadLinColor24);
CHUNK_DECL(LoadIntPercentage);
CHUNK_DECL(LoadM3DMagic);
CHUNK_DECL(LoadM3DVersion);
CHUNK_DECL(LoadMData);
CHUNK_DECL(LoadMeshVersion);
CHUNK_DECL(LoadTexMap);
CHUNK_DECL(LoadMapName);
CHUNK_DECL(LoadMapUScale);
CHUNK_DECL(LoadMapVScale);
CHUNK_DECL(LoadMapUOffset);
CHUNK_DECL(LoadMapVOffset);




bool is3dsFile(SoInput *in)
{
  if (in->getHeader().getLength() > 0)  return false;

  char c1,c2;
  if (!in->get(c1))  return false;
  if (!in->get(c2)) { in->putBack(c1); return false; }
  in->putBack(c2);
  in->putBack(c1);
  if (c1 != 0x4d)  return false;
  if (c2 != 0x4d)  return false;

  return true;
}



// forward declaration
bool read3dsFile(SoStream *in, SoSeparator *&root,
                 int appendNormals = 2, float creaseAngle = 25.f/180.f*M_PI,
                 bool loadMaterials = true, bool loadTextures = true,
                 bool loadObjNames = false, bool indexedTriSet = false,
                 bool centerModel = true, float modelSize = 10.f);



bool read3dsFile(SoInput *in, SoSeparator *&root,
                 int appendNormals = 2, float creaseAngle = 25.f/180.f*M_PI,
                 bool loadMaterials = true, bool loadTextures = true,
                 bool loadObjNames = false, bool indexedTriSet = false,
                 bool centerModel = true, float modelSize = 10.f)
{
  SoStream s;
  s.setBinary(true);
  s.setEndianOrdering(SoStream::LITTLE_ENDIAN_STREAM);
  s.wrapSoInput(in);

  return read3dsFile(&s, root, appendNormals, creaseAngle, loadMaterials,
                     loadTextures, loadObjNames, indexedTriSet,
                     centerModel, modelSize);
}



bool read3dsFile(SoStream *in, SoSeparator *&root,
                 int appendNormals = 2, float creaseAngle = 25.f/180.f*M_PI,
                 bool loadMaterials = true, bool loadTextures = true,
                 bool loadObjNames = false, bool indexedTriSet = false,
                 bool centerModel = true, float modelSize = 10.f)
{
  // read the stream header
  uint16_t header;
  *in >> header;
  if (header != M3DMAGIC)
    return false;

  // prepare Context structure
  Context con(*in);
  con.stopPos = 0;
  con.root = new SoSeparator;
  con.root->ref();
  con.minMaxValid = false;

  // customize loader
  if (appendNormals >= 2)  appendNormals = 1; // per-vertex normals are
                                              // not currently supported
  con.appendNormals = appendNormals;
  con.loadMaterials = loadMaterials;
  con.loadTextures = loadTextures;
  con.loadObjNames = loadObjNames;
  con.useIndexedTriSet = indexedTriSet;
  con.centerModel = centerModel;

  // initialize materials and prepare default one
  con.matList.append(&con.defaultMat);
  // FIXME: values for the default material are probably completely wrong
  con.defaultMat.ambient = SbColor(0.6f, 0.6f, 0.6f);
  con.defaultMat.diffuse = SbColor(0.8f, 0.8f, 0.8f);
  con.defaultMat.specular = SbColor(0.f, 0.f, 0.f);
  con.defaultMat.shininess = 20;
  con.defaultMat.transparency = 0;
  con.cMat = 0;

  // build base scene graph

  SoMatrixTransform *matrix = NULL;
  if (con.centerModel || modelSize != 0.f) {
    matrix = new SoMatrixTransform;
    con.root->addChild(matrix);
  }

  if (con.loadMaterials) {
    // material binding
    SoMaterialBinding *mb = new SoMaterialBinding;
    mb->value.setValue(SoMaterialBinding::OVERALL);
    con.root->addChild(mb);
  }

  if (con.appendNormals) {
    // normal binding
    SoNormalBinding *nb = new SoNormalBinding;
    if (con.appendNormals == 1)
      nb->value.setValue(SoNormalBinding::PER_FACE);
    else
      nb->value.setValue(SoNormalBinding::PER_VERTEX);
    con.root->addChild(nb);
  }

  if (con.loadTextures) {
    SoTextureCoordinateBinding *tb = new SoTextureCoordinateBinding;
    if (con.useIndexedTriSet)
      tb->value.setValue(SoTextureCoordinateBinding::PER_VERTEX_INDEXED);
    else
      tb->value.setValue(SoTextureCoordinateBinding::PER_VERTEX);
    con.root->addChild(tb);
  }


  // read the stream
  LoadM3DMagic(&con);

  // handle errors
  if (con.s.isBad()) {
    con.root->unref();
    con.root = NULL;
    return false;
  }

  if (con.centerModel || modelSize != 0.f) {
    SbMatrix m;

    // center model
    if (con.centerModel)
      m.setTranslate(SbVec3f(-(con.maxX+con.minX)/2.f, -(con.maxY+con.minY)/2.f,
                             -(con.maxZ+con.minZ)/2.f));
    else
      m.makeIdentity();

    // set model size
    if (modelSize != 0.f) {
      SbMatrix m2;
      float max = SbMax(SbMax(con.maxX-con.minX, con.maxY-con.minY), con.maxZ-con.minZ);
      m2.setScale(modelSize/max);
      m.multRight(m2);
    }

    matrix->matrix.setValue(m);
  }

  // return root
  con.root->unrefNoDelete();
  root = con.root;
  con.root = NULL;
  return true;
}




#define CHUNK(_name_) CHUNK_DECL(_name_)

#define HEADER \
  if (con->s.isBad()) \
    return; \
 \
  uint32_t size; \
  con->s >> size; \
  uint32_t cpos = (uint32_t)con->s.getPos(); \
  uint32_t stopPos = cpos + size - 6;


#define READ_SUBCHUNKS(_subChunkSwitch_) \
while (con->s.getPos() < stopPos) { \
  uint16_t chid; \
  con->s >> chid; \
  if (con->s.isBad()) \
    break; \
 \
  switch (chid) { \
    _subChunkSwitch_; \
  default: \
    SkipChunk(con); \
  }; \
}





CHUNK(SkipChunk)
{
  HEADER;

  // move on the position of the next chunk
  con->s.setPos(stopPos);
}



CHUNK(LoadM3DMagic)
{
  HEADER;
  con->stopPos = con->s.getPos() + size - 6;

  READ_SUBCHUNKS(
    case M3D_VERSION: LoadM3DVersion(con); break;
    case MDATA:       LoadMData(con); break;
    case KFDATA:      SkipChunk(con); break;
  )
}



CHUNK(LoadM3DVersion)
{
  HEADER;

  int32_t version;
  con->s >> version;
}



CHUNK(LoadMData)
{
  HEADER;

  READ_SUBCHUNKS(
    case MESH_VERSION: LoadMeshVersion(con); break;
    case MAT_ENTRY:    LoadMatEntry(con); break;
    case NAMED_OBJECT: LoadNamedObject(con); break;
  )
}



CHUNK(LoadMeshVersion)
{
  HEADER;

  int32_t version;
  con->s >> version;
}



CHUNK(LoadNamedObject)
{
  HEADER;

  assert(!con->cObj && "Forgot to free the current object.");

  // read object name
  char objectName[11]; // inconsistence with documentation,
                       // object name can be 11 chars (doc says 10)
  con->s.readZString(objectName, 11);

  READ_SUBCHUNKS(
    case N_TRI_OBJECT:  LoadNTriObject(con); break;
  )

  if (con->cObj) {
    // set object name
    if (con->loadObjNames && strlen(objectName) > 0)
      con->cObj->setName(objectName);

    // add cObj to the main scene graph
    if (con->cObj->getNumChildren() > 0)
      con->root->addChild(con->cObj);
    con->cObj->unref();
    con->cObj = NULL;
  }
}



CHUNK(LoadNTriObject)
{
  HEADER;

  assert(con->faceGroupList.getLength() == 0);
  con->numVertices = 0;
  con->numFaces = 0;
  con->numDefaultDegFaces = 0;
  con->textureCoordsFound = false;

  READ_SUBCHUNKS(
    case POINT_ARRAY:       LoadPointArray(con); break;
    case POINT_FLAG_ARRAY:  SkipChunk(con); break;
    case FACE_ARRAY:        LoadFaceArray(con); break;
    case MSH_MAT_GROUP:     LoadMshMatGroup(con); break;
    case TEX_VERTS:         LoadTexVerts(con); break;
    case MESH_MATRIX:       SkipChunk(con); break;
    case MESH_COLOR:        SkipChunk(con); break;
    case MESH_TEXTURE_INFO: SkipChunk(con); break;
    case PROC_NAME:         SkipChunk(con); break;
    case PROC_DATA:         SkipChunk(con); break;
  )

  // create object separator
  con->cObj = new SoSeparator;
  con->cObj->ref();

  // create coordinates (in indexed mode)
  if (con->useIndexedTriSet) {
    // coordinates
    con->cObj->addChild(con->createSoCoordinate3_i(con));
    // texture coordinates
    if (con->textureCoordsFound && con->loadTextures)
      con->cObj->addChild(con->createSoTextureCoordinate2_i(con));
  }

  // create nodes from "default material"
  if (!DefaultFaceGroup::isEmpty(con)) {

    // default material have not a texture => switch it off
    if (con->loadTextures)
      con->cObj->addChild(new SoTexture2);
    // materials
    if (con->loadMaterials)
      con->cObj->addChild(DefaultFaceGroup::getSoMaterial(con));
    // normals
    if (con->appendNormals)
      con->cObj->addChild(DefaultFaceGroup::createSoNormal(con));

    // load default material geometry
    if (con->useIndexedTriSet) {
      // indexed triStripSet
      con->cObj->addChild(DefaultFaceGroup::createSoIndexedTriStripSet_i(con));
    } else {
      // coordinates
      con->cObj->addChild(DefaultFaceGroup::createSoCoordinate3_n(con));
      // triStripSet
      con->cObj->addChild(DefaultFaceGroup::createSoTriStripSet_n(con));
    }
  }

  // create nodes with different materials
  bool textureActive = false;
  bool textureTransformActive = false;
  for (int i=0; i<con->faceGroupList.getLength(); i++) {
    FaceGroup *fg = con->faceGroupList[i];

    // materials
    if (con->loadMaterials)
      con->cObj->addChild(fg->getSoMaterial(con));
    // normals
    if (con->appendNormals)
      con->cObj->addChild(fg->createSoNormal(con));

    if (con->loadTextures) {
      // texture
      if (fg->hasTexture2(con)) {
        con->cObj->addChild(fg->getSoTexture2(con));
        textureActive = true;
      } else
        if (textureActive) {
          con->cObj->addChild(new SoTexture2);
          textureActive = false;
        }
      // texture transform
      if (fg->hasTexture2Transform(con)) {
        con->cObj->addChild(fg->getSoTexture2Transform(con));
        textureTransformActive = true;
      } else
        if (textureTransformActive) {
          con->cObj->addChild(new SoTexture2Transform);
          textureTransformActive = false;
        }
    }

    if (con->useIndexedTriSet) {
      // indexed triStripSet
      con->cObj->addChild(fg->createSoIndexedTriStripSet_i(con));
    } else {
      // coordinates
      con->cObj->addChild(fg->createSoCoordinate3_n(con));
      // textures
      if (con->loadTextures && fg->hasTexture2(con))
        con->cObj->addChild(fg->createSoTextureCoordinate2_n(con));
      // triStripSet
      con->cObj->addChild(fg->createSoTriStripSet_n(con));
    }
  }

  // clean up memory
  delete[] con->vertexList;
  con->vertexList = NULL;
  delete[] con->faceList;
  con->faceList = NULL;
  for (int j=con->faceGroupList.getLength()-1; j>=0; j--) {
    delete con->faceGroupList[j];
    con->faceGroupList.removeFast(j);
  }
}



CHUNK(LoadPointArray)
{
  HEADER;

  // number of vertices
  uint16_t num;
  con->s >> num;

  // alloc memory for Vertices
  assert(con->vertexList == NULL && "Forgot to free memory.");
  con->vertexList = new Vertex[num];
  con->numVertices = num;

  // read points
  float x,y,z;
  for (int i=0; i<num; i++) {
    con->s >> x;
    con->s >> y;
    con->s >> z;
    con->vertexList[i].point = SbVec3f(x,z,-y); // 3ds has different
        //coordinate system. Z is up, and Y goes into the scene.
  }
}



CHUNK(LoadFaceArray)
{
  HEADER;

  // number of faces
  uint16_t num;
  con->s >> num;

  // alloc memory for Faces
  assert(con->faceList == NULL && "Forgot to free memory.");
  con->faceList = new Face[num];
  con->numFaces = num;

  // alloc memory for edges
  if (con->appendNormals == 2) {
    con->numEdges = 0;
    con->edgeList = new Edge[num*3];
  }

  // make sure vertices are present yet
  if (num > 0) {
    if (con->vertexList == NULL) {
      assert(false && "Vertex list not present.");
      con->s.setBadBit();
      return;
    }
  }

  // read Faces
  uint16_t a,b,c;
  uint16_t flags;
  for (int i=0; i<num; i++) {
    con->s >> a;
    con->s >> b;
    con->s >> c;
    con->s >> flags; // STUB: decode flags (edge visibility and texture
                     // wrapping, but first get idea what's the flags meaning)
    con->faceList[i].init(con, a,b,c,flags);

    if (!con->minMaxValid) {
      con->minMaxValid = true;
      con->minX = con->maxX = con->vertexList[a].point[0];
      con->minY = con->maxY = con->vertexList[a].point[1];
      con->minZ = con->maxZ = con->vertexList[a].point[2];
    }
    #define PROCESS_VERTEX(_abc_, _xyz_, _index_) \
      if (con->min##_xyz_ > con->vertexList[_abc_].point[_index_]) \
        con->min##_xyz_ = con->vertexList[_abc_].point[_index_]; \
      else \
        if (con->max##_xyz_ < con->vertexList[_abc_].point[_index_]) \
          con->max##_xyz_ = con->vertexList[_abc_].point[_index_]

    PROCESS_VERTEX(a, X, 0);
    PROCESS_VERTEX(a, Y, 1);
    PROCESS_VERTEX(a, Z, 2);
    PROCESS_VERTEX(b, X, 0);
    PROCESS_VERTEX(b, Y, 1);
    PROCESS_VERTEX(b, Z, 2);
    PROCESS_VERTEX(c, X, 0);
    PROCESS_VERTEX(c, Y, 1);
    PROCESS_VERTEX(c, Z, 2);
    #undef PROCESS_VERTEX
  }

  READ_SUBCHUNKS(
    case MSH_MAT_GROUP: LoadMshMatGroup(con); break;
    case SMOOTH_GROUP:  SkipChunk(con); break;
    case MSH_BOXMAP:    SkipChunk(con); break;
  )
}



CHUNK(LoadMshMatGroup)
{
  HEADER;

  if (!con->loadMaterials && !con->loadTextures) {
    // move on the position of the next chunk
    con->s.setPos(stopPos);
    return;
  }

  // material name
  char materialName[16];
  con->s.readZString(materialName, 16);
  int matIndex;
  for (matIndex=1; matIndex<con->matList.getLength(); matIndex++) {
    if (strcmp(con->matList[matIndex]->name.getString(), materialName) == 0)
      break;
  }
  if (matIndex == con->matList.getLength()) {
    assert(false && "Wrong material name in the file.");
    con->s.setBadBit();
    return;
  }

  // create FaceGroup
  FaceGroup *mm = new FaceGroup;
  mm->mat = con->matList[matIndex];
  con->faceGroupList.append(mm);
  mm->numDegFaces = 0;

  // number of faces
  uint16_t num;
  con->s >> num;

  // make sure faces are present yet
  if (num > 0) {
    if (con->faceList == NULL) {
      assert(false && "Face list not present.");
      con->s.setBadBit();
      return;
    }
  }

  // face indexes
  uint16_t faceMatIndex;
  for (int i=0; i<num; i++) {
    con->s >> faceMatIndex;
    if (faceMatIndex < con->numFaces) {
      assert(con->faceList[faceMatIndex].faceGroup == NULL &&
             "3ds file error: Two materials on one face.");
      con->faceList[faceMatIndex].faceGroup = mm;
      mm->faceList.append(&con->faceList[faceMatIndex]);
      if (con->faceList[faceMatIndex].isDegenerated) {
        mm->numDegFaces++;
        con->numDefaultDegFaces--;
      }
    } else {
      assert(false && "Wrong face material index.");
      con->s.setBadBit();
      return;
    }
  }
}



CHUNK(LoadTexVerts)
{
  HEADER;
  con->textureCoordsFound = true;

  // number of faces
  uint16_t num;
  con->s >> num;

  // make sure vertices are present yet
  if (num > 0) {
    if (con->vertexList == NULL) {
      assert(false && "Vertex list not present.");
      con->s.setBadBit();
      return;
    }
  }

  // texture coordinates
  float u;
  float v;
  for (int i=0; i<num; i++) {
    con->s >> u;
    con->s >> v;
    con->vertexList[i].texturePoint = SbVec2f(u,v);
  }
}



CHUNK(LoadMatEntry)
{
  HEADER;

  if (!con->loadMaterials && !con->loadTextures) {
    // move on the position of the next chunk
    con->s.setPos(stopPos);
    return;
  }

  assert(con->cMat == NULL);
  int index = con->matList.getLength();
  con->cMat = new Material;
  con->matList.append(con->cMat);

  READ_SUBCHUNKS(
    case MAT_NAME:     LoadMatName(con); break;
    case MAT_AMBIENT:  LoadMatAmbient(con); break;
    case MAT_DIFFUSE:  LoadMatDiffuse(con); break;
    case MAT_SPECULAR: LoadMatSpecular(con); break;
    case MAT_SHININESS: LoadShininess(con); break;
    case MAT_TRANSPARENCY: LoadTransparency(con); break;
    case MAT_TEXMAP:   LoadTexMap(con); break;
  )

  // create new SoMaterial
  con->cMat->matCache = new SoMaterial;
  con->cMat->matCache->ref();
  con->cMat->updateSoMaterial(0, con->cMat->matCache);

  con->cMat = NULL;
}



CHUNK(LoadMatName)
{
  HEADER;

  char materialName[16];
  con->s.readZString(materialName, 16);

  con->cMat->name = materialName;
}



CHUNK(LoadMatAmbient)
{
  HEADER;

  READ_SUBCHUNKS(
    case COLOR_24:     LoadColor24(con); break;
    case LIN_COLOR_24: LoadLinColor24(con); break;
  )
  con->cMat->ambient = con->cColor;
}



CHUNK(LoadMatDiffuse)
{
  HEADER;

  READ_SUBCHUNKS(
    case COLOR_24:     LoadColor24(con); break;
    case LIN_COLOR_24: LoadLinColor24(con); break;
  )
  con->cMat->diffuse = con->cColor;
}



CHUNK(LoadMatSpecular)
{
  HEADER;

  READ_SUBCHUNKS(
    case COLOR_24:     LoadColor24(con); break;
    case LIN_COLOR_24: LoadLinColor24(con); break;
  )
  con->cMat->specular = con->cColor;
}



CHUNK(LoadShininess)
{
  HEADER;

  READ_SUBCHUNKS(
    case INT_PERCENTAGE: LoadIntPercentage(con); break;
  )
  // FIXME: should we combine shininess chunk with shin2pct
  // to get right shininess value?
  con->cMat->shininess = con->cColorInt;
}



CHUNK(LoadTransparency)
{
  HEADER;

  READ_SUBCHUNKS(
    case INT_PERCENTAGE: LoadIntPercentage(con); break;
  )
  con->cMat->transparency = con->cColorInt;
}



CHUNK(LoadTexMap)
{
  HEADER;

  if (!con->loadTextures) {
    // move on the position of the next chunk
    con->s.setPos(stopPos);
    return;
  }

  con->cMat->uscale = con->cMat->vscale = 1.f;
  con->cMat->uoffset = con->cMat->voffset = 0.f;

  READ_SUBCHUNKS(
    case INT_PERCENTAGE: LoadIntPercentage(con); break;
    case MAT_MAPNAME:    LoadMapName(con); break;
    case MAT_MAP_USCALE:  LoadMapUScale(con); break;
    case MAT_MAP_VSCALE:  LoadMapVScale(con); break;
    case MAT_MAP_UOFFSET: LoadMapUOffset(con); break;
    case MAT_MAP_VOFFSET: LoadMapVOffset(con); break;
  )

  if (con->cMat->textureName.getLength() > 0) {
    SoTexture2 *t = new SoTexture2;
    t->ref();
    t->filename.setValue(con->cMat->textureName);
    t->model.setValue(SoTexture2::DECAL);
    con->cMat->texture2Cache = t;
  }

  if (con->cMat->uscale  != 1.f || con->cMat->vscale  != 1.f ||
      con->cMat->uoffset != 0.f || con->cMat->voffset != 0.f) {
    SoTexture2Transform *tt = new SoTexture2Transform;
    tt->ref();
    tt->scaleFactor.setValue(SbVec2f(con->cMat->uscale, con->cMat->vscale));
    tt->translation.setValue(SbVec2f(con->cMat->uoffset, con->cMat->voffset));
    con->cMat->texture2TransformCache = tt;
  }
}



CHUNK(LoadMapName)
{
  HEADER;

  char textureName[13];
  con->s.readZString(textureName, 13);

  con->cMat->textureName = textureName;
}



CHUNK(LoadMapUScale)
{
  HEADER;
  con->s >> con->cMat->uscale;
}



CHUNK(LoadMapVScale)
{
  HEADER;
  con->s >> con->cMat->vscale;
}



CHUNK(LoadMapUOffset)
{
  HEADER;
  con->s >> con->cMat->uoffset;
}



CHUNK(LoadMapVOffset)
{
  HEADER;
  con->s >> con->cMat->voffset;
}



CHUNK(LoadColor24)
{
  HEADER;

  uint8_t r,g,b;
  con->s >> r;
  con->s >> g;
  con->s >> b;

  con->cColor = SbColor(float(r)/255.f, float(g)/255.f, float(b)/255.f);
}



CHUNK(LoadLinColor24)
{
  HEADER;

  uint8_t r,g,b;
  con->s >> r;
  con->s >> g;
  con->s >> b;

  con->cColor = SbColor(float(r)/255.f, float(g)/255.f, float(b)/255.f);
}



CHUNK(LoadIntPercentage)
{
  HEADER;

  int16_t i;
  con->s >> i;

  con->cColorInt = i;
}





SbVec3f Vertex::getNormal(tagContext *con, uint16_t myIndex) const
{
  int c = this->faceList.getLength();
  SbVec3f normal(0.f,0.f,0.f);
  for (int i=0; i<c; i++)
    normal += this->faceList[i]->getWeightedNormal(con, myIndex);
  normal.normalize();
  return normal;
}



SbVec3f Face::getNormal(tagContext *con) const
{
  SbPlane plane(con->vertexList[v1].point, con->vertexList[v2].point,
      con->vertexList[v3].point);
  return plane.getNormal();
}



float Face::getAngle(tagContext *con, uint16_t vertexIndex) const
{
  int i1, i2;
  if (v1 == vertexIndex) {
    i1 = v2; i2 = v3;
  } else {
    i1 = v1;
    if (v2 == vertexIndex) i2 = v3;
    else {
      assert(v3 == vertexIndex);
      i2 = v2;
    }
  }

  SbVec3f vec1(con->vertexList[i1].point - con->vertexList[vertexIndex].point);
  SbVec3f vec2(con->vertexList[i2].point - con->vertexList[vertexIndex].point);

  SbRotation rot(vec1, vec2);
  float value;
  rot.getValue(vec1, value);
  return value;
}



SbVec3f Face::getWeightedNormal(tagContext *con, uint16_t vertexIndex) const
{
  return getNormal(con) * getAngle(con, vertexIndex);
}



void Face::init(tagContext *con, uint16_t a, uint16_t b, uint16_t c, uint16_t f)
{
  v1=a; v2=b; v3=c; flags=f;
  faceGroup = NULL;

  isDegenerated = (con->vertexList[v2].point-con->vertexList[v1].point).cross(
      con->vertexList[v3].point - con->vertexList[v1].point).sqrLength() == 0.f;
  if (isDegenerated)  con->numDefaultDegFaces++;

  int n,i;
  Face *face;
  if (con->appendNormals == 2) {
    e12 = e23 = e31 = 0xffffffff;

#define VERT_CODE(_nc_, _ns1_, _ns2_) \
    n = con->vertexList[v##_nc_].faceList.getLength(); \
    for (i=0; i<n; i++) { \
      face = con->vertexList[v##_nc_].faceList[i]; \
      if (v##_ns1_ == face->v##_ns1_) { \
        e##_nc_##_ns1_ = face->e##_nc_##_ns1_; \
        con->edgeList[face->e##_nc_##_ns1_].faceList.append(this); \
      } \
      if (v##_ns2_ == face->v##_ns1_) { \
        e##_ns2_##_nc_ = face->e##_nc_##_ns1_; \
        con->edgeList[face->e##_nc_##_ns1_].faceList.append(this); \
      } \
      if (v##_ns1_ == face->v##_ns2_) { \
        e##_nc_##_ns1_ = face->e##_ns2_##_nc_; \
        con->edgeList[face->e##_ns2_##_nc_].faceList.append(this); \
      } \
      if (v##_ns2_ == face->v##_ns2_) { \
        e##_ns2_##_nc_ = face->e##_ns2_##_nc_; \
        con->edgeList[face->e##_ns2_##_nc_].faceList.append(this); \
      } \
    } \
    con->vertexList[v##_nc_].faceList.append(this);

    VERT_CODE(1,2,3);
    VERT_CODE(2,3,1);
    VERT_CODE(3,1,2);

#undef VERT_CODE

    if (e12 == 0xffffffff)
      e12 = con->numEdges++;
    if (e23 == 0xffffffff)
      e23 = con->numEdges++;
    if (e31 == 0xffffffff)
      e31 = con->numEdges++;
  }
}



bool FaceGroup::hasTexture2(tagContext *con)
{ return mat->hasTexture2(con); }

SoTexture2* FaceGroup::getSoTexture2(tagContext *con)
{ return mat->getSoTexture2(con); }

bool FaceGroup::hasTexture2Transform(tagContext *con)
{ return mat->hasTexture2Transform(con); }

SoTexture2Transform* FaceGroup::getSoTexture2Transform(tagContext *con)
{ return mat->getSoTexture2Transform(con); }

SoMaterial* FaceGroup::getSoMaterial(tagContext *con)
{ return mat->getSoMaterial(con); }



SoNormal* FaceGroup::createSoNormal(tagContext *con)
{
  SoNormal *normals = new SoNormal;
  int num = faceList.getLength();
  if (con->appendNormals == 1) {
    normals->vector.setNum(num-numDegFaces);
    SbVec3f *v = normals->vector.startEditing();
    for (int i=0; i<num; i++) {
      Face *f = faceList[i];
      if (!f->isDegenerated)
        *(v++) = f->getNormal(con);
    }
    normals->vector.finishEditing();
  } else {
/*  FIXME: This is incomplete implementation of per-vertex generator.

    normals->vector.setNum(num*3);
    SbVec3f *v = normals->vector.startEditing();
    SbPlane p1, p2;
    for (int i=0; i<num; i++) {
      Face *f = faceList[i];
      Edge *e = &con->edgeList[f->e12];
      int fnum = e->faceList.getLength();
      int j;
      for (j=0; i<fnum; j++) {
        if (currface != facenum) { // check all but this face
      const SbVec3f &normal = facenormals[currface];
      if ((normal.dot(*facenormal)) > threshold) {
        // smooth towards this face
        vertnormal += normal;
      }
    }
  }
      *(v++) = con->vertexList[f->v1].getNormal(con, f->v1);
      *(v++) = con->vertexList[f->v2].getNormal(con, f->v2);
      *(v++) = con->vertexList[f->v3].getNormal(con, f->v3);
    }
    normals->vector.finishEditing();*/
  }
  return normals;
}



SoCoordinate3* FaceGroup::createSoCoordinate3_n(tagContext *con)
{
  assert(!con->useIndexedTriSet && "Improper use.");

  SoCoordinate3 *coords = new SoCoordinate3;
  int num = faceList.getLength();
  coords->point.setNum((num-numDegFaces)*3);
  SbVec3f *c = coords->point.startEditing();
  for (int i=0; i<num; i++) {
    Face *f = faceList[i];
    if (!f->isDegenerated) {
      *(c++) = con->vertexList[f->v1].point;
      *(c++) = con->vertexList[f->v2].point;
      *(c++) = con->vertexList[f->v3].point;
    }
  }
  coords->point.finishEditing();
  return coords;
}



SoTextureCoordinate2* FaceGroup::createSoTextureCoordinate2_n(tagContext *con)
{
  assert(!con->useIndexedTriSet && "Improper use.");

  SoTextureCoordinate2 *tCoords = new SoTextureCoordinate2;
  int num = faceList.getLength();
  tCoords->point.setNum((num-numDegFaces)*3);
  SbVec2f *c = tCoords->point.startEditing();
  for (int i=0; i<num; i++) {
    Face *f = faceList[i];
    if (!f->isDegenerated) {
      *(c++) = con->vertexList[f->v1].texturePoint;
      *(c++) = con->vertexList[f->v2].texturePoint;
      *(c++) = con->vertexList[f->v3].texturePoint;
    }
  }
  tCoords->point.finishEditing();
  return tCoords;
}



SoTriangleStripSet* FaceGroup::createSoTriStripSet_n(tagContext *con)
{
  assert(!con->useIndexedTriSet && "Improper use.");

  SoTriangleStripSet *triSet = new SoTriangleStripSet;
  int num = faceList.getLength() - numDegFaces;
  triSet->numVertices.setNum(num);
  int32_t *n = triSet->numVertices.startEditing();
  for (int i=0; i<num; i++) {
    *(n++) = 3;
  }
  triSet->numVertices.finishEditing();
  return triSet;
}



SoIndexedTriangleStripSet* FaceGroup::createSoIndexedTriStripSet_i(tagContext *con)
{
  assert(con->useIndexedTriSet && "Improper use.");

  SoIndexedTriangleStripSet *triSet = new SoIndexedTriangleStripSet;
  int num = faceList.getLength();
  int i;

  // coords
  triSet->coordIndex.setNum((num-numDegFaces)*4);
  int32_t *c = triSet->coordIndex.startEditing();
  for (i=0; i<num; i++) {
    Face *f = faceList[i];
    if (!f->isDegenerated) {
      *(c++) = f->v1;
      *(c++) = f->v2;
      *(c++) = f->v3;
      *(c++) = SO_END_STRIP_INDEX;
    }
  }
  triSet->coordIndex.finishEditing();

  // texture
  if (mat->hasTexture2(con) && con->loadTextures) {
    triSet->textureCoordIndex.setNum((num-numDegFaces)*4);
    int32_t *tc = triSet->textureCoordIndex.startEditing();
    for (i=0; i<num; i++) {
      Face *f = faceList[i];
      if (!f->isDegenerated) {
        *(tc++) = f->v1;
        *(tc++) = f->v2;
        *(tc++) = f->v3;
        *(tc++) = SO_END_STRIP_INDEX;
      }
    }
    triSet->textureCoordIndex.finishEditing();
  }

  return triSet;
}



Material* DefaultFaceGroup::getMaterial(tagContext *con)
{ return &con->defaultMat; }



bool DefaultFaceGroup::isEmpty(Context *con)
{
  int num = con->numFaces;
  for (int i=0; i<num; i++)
    if (con->faceList[i].faceGroup == NULL) return false;
  return true;
}



SoMaterial* DefaultFaceGroup::getSoMaterial(tagContext *con)
{
  return getMaterial(con)->getSoMaterial(con);
}



SoNormal* DefaultFaceGroup::createSoNormal(tagContext *con)
{
  SoNormal *normals = new SoNormal;
  int num = con->numFaces;
  int j = 0;
  if (con->appendNormals == 1) {
    normals->vector.setNum(num - con->numDefaultDegFaces);
    SbVec3f *v = normals->vector.startEditing();
    for (int i=0; i<num; i++) {
      Face *f = &con->faceList[i];
      if (f->faceGroup == NULL && !f->isDegenerated) {
        *(v++) = f->getNormal(con);
        j++;
      }
    }
    normals->vector.finishEditing();
    normals->vector.setNum(j);
  } else {
    assert(false);
  }
  return normals;
}



SoCoordinate3* DefaultFaceGroup::createSoCoordinate3_n(tagContext *con)
{
  assert(!con->useIndexedTriSet && "Improper use.");

  SoCoordinate3 *coords = new SoCoordinate3;
  int num = con->numFaces;
  coords->point.setNum((num - con->numDefaultDegFaces) * 3);
  SbVec3f *c = coords->point.startEditing();
  int j = 0;
  for (int i=0; i<num; i++) {
    Face *f = &con->faceList[i];
    if (f->faceGroup == NULL && !f->isDegenerated) {
      *(c++) = con->vertexList[f->v1].point;
      *(c++) = con->vertexList[f->v2].point;
      *(c++) = con->vertexList[f->v3].point;
      j += 3;
    }
  }
  coords->point.finishEditing();
  coords->point.setNum(j);
  return coords;
}



SoTriangleStripSet* DefaultFaceGroup::createSoTriStripSet_n(tagContext *con)
{
  assert(!con->useIndexedTriSet && "Improper use.");

  SoTriangleStripSet *triSet = new SoTriangleStripSet;
  int num = con->numFaces - con->numDefaultDegFaces;
  int i;
  int j = 0;
  for (i=0; i<num; i++) {
    Face *f = &con->faceList[i];
    if (f->faceGroup == NULL)  j++;
  }
  j -= con->numDefaultDegFaces;

  triSet->numVertices.setNum(j);
  int32_t *n = triSet->numVertices.startEditing();
  for (i=0; i<j; i++) {
    *(n++) = 3;
  }
  triSet->numVertices.finishEditing();

  return triSet;
}



SoIndexedTriangleStripSet* DefaultFaceGroup::createSoIndexedTriStripSet_i(tagContext *con)
{
  assert(con->useIndexedTriSet && "Improper use.");

  SoIndexedTriangleStripSet *triSet = new SoIndexedTriangleStripSet;
  int num = con->numFaces;
  int i;
  int j = 0;
  for (i=0; i<num; i++) {
    Face *f = &con->faceList[i];
    if (f->faceGroup == NULL)  j++;
  }
  j -= con->numDefaultDegFaces;

  // coords
  triSet->coordIndex.setNum(j*4);
  int32_t *c = triSet->coordIndex.startEditing();
  for (i=0; i<num; i++) {
    Face *f = &con->faceList[i];
    if (f->faceGroup == NULL && !f->isDegenerated) {
      *(c++) = f->v1;
      *(c++) = f->v2;
      *(c++) = f->v3;
      *(c++) = SO_END_STRIP_INDEX;
    }
  }
  triSet->coordIndex.finishEditing();

  return triSet;
}



void Material::updateSoMaterial(int index, SoMaterial *m)
{
  m->ambientColor.set1Value(index, ambient);
  m->diffuseColor.set1Value(index, diffuse);
  m->specularColor.set1Value(index, specular);
  m->emissiveColor.set1Value(index, SbColor(0.f,0.f,0.f));
  m->shininess.set1Value(index, float(shininess)/100.f);
  m->transparency.set1Value(index, float(transparency)/100.f);
}



SoMaterial* Material::getSoMaterial(Context *con)
{ return matCache; }



bool Material::hasTexture2(tagContext *con)
{ return texture2Cache; }



SoTexture2* Material::getSoTexture2(tagContext *con)
{ return texture2Cache; }



bool Material::hasTexture2Transform(tagContext *con)
{ return texture2TransformCache; }



SoTexture2Transform* Material::getSoTexture2Transform(tagContext *con)
{ return texture2TransformCache; }



SoCoordinate3* Context::createSoCoordinate3_i(Context *con) const
{
  assert(con->useIndexedTriSet && "Improper use.");

  SoCoordinate3 *coords = new SoCoordinate3;
  coords->point.setNum(con->numVertices);
  SbVec3f *c = coords->point.startEditing();
  for (int i=0; i<con->numVertices; i++)
    c[i] = con->vertexList[i].point;
  coords->point.finishEditing();
  return coords;
}



SoTextureCoordinate2* Context::createSoTextureCoordinate2_i(tagContext *con) const
{
  assert(con->useIndexedTriSet && "Improper use.");

  SoTextureCoordinate2 *tCoords = new SoTextureCoordinate2;
  tCoords->point.setNum(con->numVertices);
  SbVec2f *c = tCoords->point.startEditing();
  for (int i=0; i<con->numVertices; i++)
    c[i] = con->vertexList[i].texturePoint;
  tCoords->point.finishEditing();
  return tCoords;
}
