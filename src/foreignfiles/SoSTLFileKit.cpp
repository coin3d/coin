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

#include <Inventor/SbBasic.h>
#include <Inventor/lists/SbList.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoReorganizeAction.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/nodes/SoTexture2.h>
#include <Inventor/nodes/SoNormal.h>
#include <Inventor/nodes/SoNormalBinding.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoMaterialBinding.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodekits/SoSubKitP.h>
#include <Inventor/SbBSPTree.h>
#include <Inventor/SoPrimitiveVertex.h>

#include "steel.h"

#include <ForeignFiles/SoSTLFileKit.h>


#if 0
  SoCallback         "callbackList"         SoBaseKit
  SoSeparator        "topSeparator"         SoForeignFileKit
  SoShapeHints         "shapehints"           SoSTLFileKit
  SoTexture2           "texture"              SoSTLFileKit
  SoNormalBinding      "normalbinding"        SoSTLFileKit
  SoNormal             "normals"              SoSTLFileKit
  SoMaterialBinding    "materialbinding"      SoSTLFileKit
  SoMaterial           "material"             SoSTLFileKit
  SoCoordinate3        "coordinates"          SoSTLFileKit
  SoIndexedFaceSet     "facets"               SoSTLFileKit
#endif // 0

class SoSTLFileKitP {
public:
  SoSTLFileKitP(SoSTLFileKit * pub)
  : api(pub) {
    this->data = NULL;
    this->points = NULL;
    this->normals = NULL;
  }
  ~SoSTLFileKitP(void) {
    if ( this->data ) delete this->data;
    if ( this->points ) delete this->points;
    if ( this->normals ) delete this->normals;
  }

public:
  SoSTLFileKit * const api;

  SbList<uint16_t> * data;
  SbBSPTree * points;
  SbBSPTree * normals;

  int numfacets;
  int numvertices;
  int numnormals;
  int numsharedvertices;
  int numsharednormals;
  int numredundantfacets;
}; // SoSTLFileKitP

// *************************************************************************

/*!
  \class SoSTLFileKit ForeignFiles/SoSTLFileKit.h

  Class for using STL models with Open Inventor.

  \relates foreignfileformats
*/

#define PRIVATE(obj) ((obj)->pimpl)

SO_KIT_SOURCE(SoSTLFileKit)

void
SoSTLFileKit::initClass(void)
{
  SO_KIT_INIT_CLASS(SoSTLFileKit, SoForeignFileKit, SoForeignFileKit);

  SoType type = SoSTLFileKit::getClassTypeId();
  SoForeignFileKit::registerFileExtension(type, "stl", SoSTLFileKit::identify);
}

/*!
  Constructor.
*/

SoSTLFileKit::SoSTLFileKit(void)
{
  PRIVATE(this) = new SoSTLFileKitP(this);

  SO_KIT_INTERNAL_CONSTRUCTOR(SoSTLFileKit);

  SO_KIT_ADD_FIELD(proper, (FALSE));
  SO_KIT_ADD_FIELD(colorization, (NONE));

  SO_KIT_ADD_CATALOG_ENTRY(facets, SoIndexedFaceSet,
                           TRUE, topSeparator, \x0, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(coordinates, SoCoordinate3,
                           TRUE, topSeparator, facets, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(material, SoMaterial,
                           TRUE, topSeparator, coordinates, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(materialbinding, SoMaterialBinding,
                           TRUE, topSeparator, material, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(normals, SoNormal,
                           TRUE, topSeparator, materialbinding, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(normalbinding, SoNormalBinding,
                           TRUE, topSeparator, normals, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(texture, SoTexture2,
                           TRUE, topSeparator, normalbinding, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(shapehints, SoShapeHints,
                           TRUE, topSeparator, texture, TRUE);

  SO_KIT_INIT_INSTANCE();
}

/*!
  Destructor.
*/

SoSTLFileKit::~SoSTLFileKit(void)
{
  delete PRIVATE(this);
  PRIVATE(this) = NULL;
}

/*!
  Resets the STL blob so it contains nothing.
*/

void
SoSTLFileKit::reset(void)
{
  PRIVATE(this)->numvertices = 0;
  PRIVATE(this)->numfacets = 0;
  PRIVATE(this)->numnormals = 0;
  PRIVATE(this)->numsharedvertices = 0;
  PRIVATE(this)->numsharednormals = 0;
  PRIVATE(this)->numredundantfacets = 0;

  if ( PRIVATE(this)->data ) {
    PRIVATE(this)->data->truncate(0);
  }
  if ( PRIVATE(this)->points ) {
    PRIVATE(this)->points->clear();
  }
  if ( PRIVATE(this)->normals ) {
    PRIVATE(this)->normals->clear();
  }

  // this->setAnyPart("shapehints", new SoShapeHints, TRUE);
  // this->setAnyPart("normalbinding", new SoNormalBinding, TRUE);
  this->setAnyPart("normals", new SoNormal, TRUE);
  this->setAnyPart("coordinates", new SoCoordinate3, TRUE);
  this->setAnyPart("material", new SoMaterial, TRUE);
  this->setAnyPart("facets", new SoIndexedFaceSet, TRUE);
}

SbBool
SoSTLFileKit::identify(const char * filename)
{
  assert(filename);
  stl_reader * reader = stl_reader_create(filename);
  if ( !reader ) {
    return FALSE;
  }
  stl_reader_destroy(reader);
  return TRUE;
}

SbBool
SoSTLFileKit::canReadFile(const char * filename) const
{
  if ( !filename ) return TRUE; // we can read (in general)
  return SoSTLFileKit::identify(filename);
}

SbBool
SoSTLFileKit::readFile(const char * filename)
{
  assert(filename);

  this->reset();

  stl_reader * reader = stl_reader_create(filename);
  if ( !reader ) {
    SoDebugError::postInfo("SoSTLFileKit::readFile",
                           "unable to create STL reader for '%s'.",
                           filename);
    return FALSE;
  }

  SbBool binary = (stl_reader_flags(reader) & STL_BINARY) ? TRUE : FALSE;

  if ( binary && (PRIVATE(this)->data == NULL) ) {
    PRIVATE(this)->data = new SbList<uint16_t>;
  }

  if ( PRIVATE(this)->points == NULL ) {
    PRIVATE(this)->points = new SbBSPTree;
  }

  if ( PRIVATE(this)->normals == NULL ) {
    PRIVATE(this)->normals = new SbBSPTree;
  }

  SoShapeHints * hints =
    SO_GET_ANY_PART(this, "shapehints", SoShapeHints);
  hints->vertexOrdering.setValue(SoShapeHints::UNKNOWN_ORDERING);
  // what it should have been
  // hints->vertexOrdering.setValue(SoShapeHints::COUNTERCLOCKWISE);
  hints->shapeType.setValue(SoShapeHints::SOLID);
  hints->faceType.setValue(SoShapeHints::UNKNOWN_FACE_TYPE);

  SoNormalBinding * normalbinding =
    SO_GET_ANY_PART(this, "normalbinding", SoNormalBinding);
  normalbinding->value = SoNormalBinding::PER_FACE_INDEXED;

  stl_facet * facet = stl_facet_create();
  SbBool loop = TRUE, success = TRUE;
  while ( loop ) {
    const int peekval = stl_reader_peek(reader);
    if ( peekval == STL_BEGIN ) {
    } else if ( peekval == STL_INIT_INFO ) {
    } else if ( peekval == STL_EXIT_INFO ) {
    } else if ( peekval == STL_END ) {
      loop = FALSE;
    } else if ( peekval == STL_FACET ) {
      stl_real x, y, z;
      stl_reader_fill_facet(reader, facet);
      stl_facet_get_normal(facet, &x, &y, &z);
      SbVec3f normal((float) x, (float) y, (float) z);
      stl_facet_get_vertex1(facet, &x, &y, &z);
      SbVec3f vertex1((float) x, (float) y, (float) z);
      stl_facet_get_vertex2(facet, &x, &y, &z);
      SbVec3f vertex2((float) x, (float) y, (float) z);
      stl_facet_get_vertex3(facet, &x, &y, &z);
      SbVec3f vertex3((float) x, (float) y, (float) z);
      if ( normal.length() == 0.0f ) { // auto-calculate
        SbVec3f v1(vertex2-vertex1);
        SbVec3f v2(vertex3-vertex1);
        normal = v1.cross(v2);
        float len = normal.length();
        if ( len > 0 ) normal /= len;
      }

      this->addFacet(vertex1, vertex2, vertex3, normal);

      if ( binary ) {
        // binary contains padding, which might be colorization
        unsigned int data = stl_facet_get_padding(facet);
        PRIVATE(this)->data->append((uint16_t) data);
        // fprintf(stderr, "facet data: %04x\n", data);
      }
    } else if ( peekval == STL_ERROR ) {
      SoDebugError::post("SoSTLFileKit::readFile",
                         "error '%s' after %d facets, line %d.",
                         stl_reader_get_error(reader),
                         PRIVATE(this)->numfacets,
                         stl_reader_get_line_number(reader));
      loop = FALSE;
      success = FALSE;
    }
  }
  // done

  // no need for the BSP trees to contain data any more
  PRIVATE(this)->points->clear();
  PRIVATE(this)->normals->clear();

  stl_facet_destroy(facet);
  stl_reader_destroy(reader);

  if ( !success ) {
    this->reset();
#if defined(COIN_EXTRA_DEBUG)
  } else {
    SoDebugError::postInfo("SoSTLFileKit::readFile",
                           "file read successfully. "
                           "%d unique vertices, %d reuses. "
                           "%d unique normals, %d reuses. "
                           "%d facets, %d redundant facets.",
                           PRIVATE(this)->numvertices,
                           PRIVATE(this)->numsharedvertices,
                           PRIVATE(this)->numnormals,
                           PRIVATE(this)->numsharednormals,
                           PRIVATE(this)->numfacets,
                           PRIVATE(this)->numredundantfacets);
#endif // COIN_EXTRA_DEBUG
  }
  return success;
}

SbBool
SoSTLFileKit::canReadScene(void) const
{
  return TRUE;
}

SbBool
SoSTLFileKit::addFacet(const SbVec3f & v1, const SbVec3f & v2, const SbVec3f & v3, const SbVec3f & n)
{
  SoNormal * normals =
    SO_GET_ANY_PART(this, "normals", SoNormal);
  SoCoordinate3 * coordinates =
    SO_GET_ANY_PART(this, "coordinates", SoCoordinate3);
  SoIndexedFaceSet * facets =
    SO_GET_ANY_PART(this, "facets", SoIndexedFaceSet);

  // find existing indexes if any
  int v1idx = PRIVATE(this)->points->findPoint(v1);
  int v2idx = PRIVATE(this)->points->findPoint(v2);
  int v3idx = PRIVATE(this)->points->findPoint(v3);
  if (v1idx != -1) { v1idx = (int) PRIVATE(this)->points->getUserData(v1idx); }
  if (v2idx != -1) { v2idx = (int) PRIVATE(this)->points->getUserData(v2idx); }
  if (v3idx != -1) { v3idx = (int) PRIVATE(this)->points->getUserData(v3idx); }
  int nidx = PRIVATE(this)->normals->findPoint(n);
  if (nidx != -1) { nidx = (int) PRIVATE(this)->normals->getUserData(nidx); }

  // toss out invalid facets (of which there are often many)...
  // what are these - are they lines and points or something?
  if ((v1 == v2) || (v2 == v3) || (v1 == v3)) {
    // IDEA: optimize above test for the cases where you have vertex
    // indexes. we can't use only indices of course since the vertices
    // may not have been registered yet, but...
    PRIVATE(this)->numredundantfacets += 1;
    return FALSE;
  }

  // toss out redundant facets, if any...
  if ( (v1idx != -1) && (v2idx != -1) && (v3idx != -1) ) {
    int count = facets->coordIndex.getNum();
    const int32_t * points = facets->coordIndex.getValues(0);
    int i;
    for ( i = 0; i < count; i++ ) {
      if (points[i] == v1idx) {
        int beg = i - (i % 4);
        if ( ((points[beg] == v1idx) && (points[beg+1] == v2idx) &&
              (points[beg+2] == v3idx)) ||
             ((points[beg] == v2idx) && (points[beg+1] == v3idx) &&
              (points[beg+2] == v1idx)) ||
             ((points[beg] == v3idx) && (points[beg+1] == v1idx) &&
              (points[beg+2] == v2idx)) ) {
          // same vertices, same vertex ordering (we drop comparing normal)
          PRIVATE(this)->numredundantfacets += 1;
          return FALSE;
        }
      }
    }
  }

  // add facet (triangle) to faceset
  if (v1idx == -1) {
    v1idx = PRIVATE(this)->numvertices;
    coordinates->point.set1Value(v1idx, v1);
    PRIVATE(this)->points->addPoint(v1, (void *) v1idx);
    PRIVATE(this)->numvertices++;
  } else {
    PRIVATE(this)->numsharedvertices++;
  }
  facets->coordIndex.set1Value(PRIVATE(this)->numfacets*4, v1idx);
  
  if (v2idx == -1) {
    v2idx = PRIVATE(this)->numvertices;
    coordinates->point.set1Value(v2idx, v2);
    PRIVATE(this)->points->addPoint(v2, (void *) v2idx);
    PRIVATE(this)->numvertices++;
  } else {
    PRIVATE(this)->numsharedvertices++;
  }
  facets->coordIndex.set1Value(PRIVATE(this)->numfacets*4+1, v2idx);

  if (v3idx == -1) {
    v3idx = PRIVATE(this)->numvertices;
    coordinates->point.set1Value(v3idx, v3);
    PRIVATE(this)->points->addPoint(v3, (void *) v3idx);
    PRIVATE(this)->numvertices++;
  } else {
    PRIVATE(this)->numsharedvertices++;
  }
  facets->coordIndex.set1Value(PRIVATE(this)->numfacets*4+2, v3idx);
  facets->coordIndex.set1Value(PRIVATE(this)->numfacets*4+3, -1);

  if (nidx == -1) {
    nidx = PRIVATE(this)->numnormals;
    normals->vector.set1Value(nidx, n);
    PRIVATE(this)->normals->addPoint(n, (void *) nidx);
    PRIVATE(this)->numnormals++;
  } else {
    PRIVATE(this)->numsharednormals++;
  }
  facets->normalIndex.set1Value(PRIVATE(this)->numfacets, nidx);

  PRIVATE(this)->numfacets++;
  return TRUE;
}

void
SoSTLFileKit::triangle_cb(void * closure,
                          SoCallbackAction * action,
                          const SoPrimitiveVertex * v1,
                          const SoPrimitiveVertex * v2,
                          const SoPrimitiveVertex * v3)
{
  assert(closure); assert(v1); assert(v2); assert(v3);
  SoSTLFileKit * filekit = (SoSTLFileKit *) closure;

  SbVec3f vertex1(v1->getPoint());
  SbVec3f vertex2(v2->getPoint());
  SbVec3f vertex3(v3->getPoint());

  SbVec3f vec1(vertex2-vertex1);
  SbVec3f vec2(vertex3-vertex1);
  SbVec3f normal(vec1.cross(vec2));
  float len = normal.length();
  if ( len > 0 ) normal /= len;

  filekit->addFacet(vertex1, vertex2, vertex3, normal);
}

/*!
  Convert a scene graph into an SoSTLFileKit.
  Useful in relation to writable foreign file kits to convert between
  various file formats.
*/

SbBool
SoSTLFileKit::readScene(SoNode * scene)
{
  this->reset();

  SoCallbackAction cba;
  cba.addTriangleCallback(SoType::fromName("SoNode"), triangle_cb, this);
  cba.apply(scene);
  // done

  // no need for the BSP trees to contain data any more
  PRIVATE(this)->points->clear();
  PRIVATE(this)->normals->clear();

#if defined(COIN_EXTRA_DEBUG)
  SoDebugError::postInfo("SoSTLFileKit::readScene",
                         "scene graph read in. "
                         "%d unique vertices, %d reuses. "
                         "%d unique normals, %d reuses.",
                         PRIVATE(this)->numvertices,
                         PRIVATE(this)->numsharedvertices,
                         PRIVATE(this)->numnormals,
                         PRIVATE(this)->numsharednormals);
#endif // COIN_EXTRA_DEBUG
  return TRUE;
}

/*!
*/

SbBool
SoSTLFileKit::canWriteScene(const char * format) const
{
  if ( !format ) return TRUE;
  // FIXME: implement format checking (VRML1, VRML97)
  return TRUE;
}

/*!
*/

SbBool
SoSTLFileKit::writeScene(SoNode *& root, const char * format)
{
  static const char default_format[] = "#VRML 1.0"; // syntax for this?
  if ( !format ) format = default_format;

  SbBool success = true;

  // FIXME: implement format check to specify scene graph setup
  enum Format {
    VRML1,
    VRML97
  };

  Format build = VRML1;

  if ( build == VRML1 ) {
    // create VRML1 scene
    SoSeparator * sceneroot = new SoSeparator;
    sceneroot->ref();
    
    SoShapeHints * shapehints_orig =
      SO_GET_ANY_PART(this, "shapehints", SoShapeHints);
    SoShapeHints * shapehints_copy = new SoShapeHints;
    shapehints_copy->copyContents(shapehints_orig, FALSE);
    sceneroot->addChild(shapehints_copy);

    SoTexture2 * texture_orig = SO_GET_ANY_PART(this, "texture", SoTexture2);
    SoTexture2 * texture_copy = new SoTexture2;
    texture_copy->copyContents(texture_orig, FALSE);
    sceneroot->addChild(texture_copy);

    SoNormalBinding * normalbinding_orig =
      SO_GET_ANY_PART(this, "normalbinding", SoNormalBinding);
    SoNormalBinding * normalbinding_copy = new SoNormalBinding;
    normalbinding_copy->copyContents(normalbinding_orig, FALSE);
    sceneroot->addChild(normalbinding_copy);

    SoNormal * normals_orig = SO_GET_ANY_PART(this, "normals", SoNormal);
    SoNormal * normals_copy = new SoNormal;
    normals_copy->copyContents(normals_orig, FALSE);
    sceneroot->addChild(normals_copy);

    SoMaterialBinding * materialbinding_orig =
      SO_GET_ANY_PART(this, "materialbinding", SoMaterialBinding);
    SoMaterialBinding * materialbinding_copy = new SoMaterialBinding;
    materialbinding_copy->copyContents(materialbinding_orig, FALSE);
    sceneroot->addChild(materialbinding_copy);

    SoMaterial * material_orig = SO_GET_ANY_PART(this, "material", SoMaterial);
    SoMaterial * material_copy = new SoMaterial;
    material_copy->copyContents(material_orig, FALSE);
    sceneroot->addChild(material_copy);

    SoCoordinate3 * coordinates_orig =
      SO_GET_ANY_PART(this, "coordinates", SoCoordinate3);
    SoCoordinate3 * coordinates_copy = new SoCoordinate3;
    coordinates_copy->copyContents(coordinates_orig, FALSE);
    sceneroot->addChild(coordinates_copy);

    SoIndexedFaceSet * facets_orig =
      SO_GET_ANY_PART(this, "facets", SoIndexedFaceSet);
    SoIndexedFaceSet * facets_copy = new SoIndexedFaceSet;
    facets_copy->copyContents(facets_orig, FALSE);
    sceneroot->addChild(facets_copy);

    // optimize/reorganize mesh
    SoReorganizeAction ra;
    ra.apply(sceneroot);

    // FIXME: remove redundant scene graph nodes after scene reorganization

    sceneroot->unrefNoDelete();
    root = sceneroot;
  } else {
    SoDebugError::postWarning("SoSTLFileKit::writeScene",
                              "unsupported format - could not create scene.");
    success = FALSE;
  }

  return success;
}

/*!
  Returns 
*/

SbBool
SoSTLFileKit::canWriteFile(const char * filename) const
{
  return inherited::canWriteFile(filename);
}

SbBool
SoSTLFileKit::writeFile(const char * filename)
{
  // writing to file is not implemented yet
#if 0
  unsigned int flags = 0;
  // set up flags for binary and color if wanted
  stl_writer * writer = stl_writer_create(filename, flags);
  if ( !writer ) {
    return FALSE;
  }

  // write header info
  // write facets

  stl_writer_destroy(writer);
#endif

  return inherited::writeFile(filename);
}

#undef PRIVATE

