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
  \class SoIndexedLineSet SoIndexedLineSet.h Inventor/nodes/SoIndexedLineSet.h
  \brief The SoIndexedLineSet class is used to render and otherwise represent indexed lines.
  \ingroup nodes

  The indexed counterpart of SoLineSet. Lines can specified using
  indices for coordinates, normals, materials and texture coordinates.

  If no normals are supplied on the stack (or in the vertexProperty
  field), the lines will be rendered with lighting disabled.

  For more information about line sets, see documentation in
  SoLineSet.  For more information about indexed shapes, see
  documentation in SoIndexedShape and SoIndexedFaceSet.

  <b>FILE FORMAT/DEFAULTS:</b>
  \code
    IndexedLineSet {
        vertexProperty NULL
        coordIndex 0
        materialIndex -1
        normalIndex -1
        textureCoordIndex -1
    }
  \endcode

*/

#include <Inventor/nodes/SoIndexedLineSet.h>
#include <Inventor/nodes/SoSubNodeP.h>

#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/caches/SoNormalCache.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/bundles/SoMaterialBundle.h>

#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/misc/SoGL.h>
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H
#include <Inventor/system/gl.h>

#include <Inventor/actions/SoGetPrimitiveCountAction.h>

#include <Inventor/elements/SoNormalBindingElement.h>
#include <Inventor/elements/SoMaterialBindingElement.h>
#include <Inventor/elements/SoCoordinateElement.h>
#include <Inventor/elements/SoGLShapeHintsElement.h>
#include <Inventor/elements/SoTextureCoordinateBindingElement.h>
#include <Inventor/elements/SoDrawStyleElement.h>
#include <Inventor/elements/SoGLCoordinateElement.h>
#include <Inventor/elements/SoGLLazyElement.h>
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/elements/SoGLVBOElement.h>
#include <Inventor/elements/SoMultiTextureCoordinateElement.h>
#include <Inventor/elements/SoMultiTextureEnabledElement.h>

#include <assert.h>

#include <Inventor/bundles/SoTextureCoordinateBundle.h>
#include <Inventor/details/SoLineDetail.h>
#include <Inventor/caches/SoBoundingBoxCache.h>
#include "../misc/SoVertexArrayIndexer.h"
#include "../misc/SoVBO.h"

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif


SO_NODE_SOURCE(SoIndexedLineSet);


#define PRIVATE(obj) obj->pimpl
#define LOCK_VAINDEXER(obj) SoBase::staticDataLock()
#define UNLOCK_VAINDEXER(obj) SoBase::staticDataUnlock()

class SoIndexedLineSetP {
public:
  SoVertexArrayIndexer * vaindexer;
};

/*!
  Constructor.
*/
SoIndexedLineSet::SoIndexedLineSet()
{
  PRIVATE(this) = new SoIndexedLineSetP;
  PRIVATE(this)->vaindexer = NULL;

  SO_NODE_INTERNAL_CONSTRUCTOR(SoIndexedLineSet);
}

/*!
  Destructor.
*/
SoIndexedLineSet::~SoIndexedLineSet()
{
  delete PRIVATE(this)->vaindexer;
  delete PRIVATE(this);
}

// doc from parent
void
SoIndexedLineSet::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoIndexedLineSet, SO_FROM_INVENTOR_1|SoNode::VRML1);
}

//
// translates current normal binding into the internal Binding enum
//
SoIndexedLineSet::Binding
SoIndexedLineSet::findNormalBinding(SoState * state)
{
  Binding binding = PER_VERTEX_INDEXED;

  SoNormalBindingElement::Binding normbind =
    (SoNormalBindingElement::Binding) SoNormalBindingElement::get(state);

  switch (normbind) {
  case SoNormalBindingElement::OVERALL:
    binding = OVERALL;
    break;
  case SoNormalBindingElement::PER_VERTEX:
    binding = PER_VERTEX;
    break;
  case SoNormalBindingElement::PER_VERTEX_INDEXED:
    binding = PER_VERTEX_INDEXED;
    break;
  case SoNormalBindingElement::PER_PART:
    binding = PER_SEGMENT;
    break;
  case SoNormalBindingElement::PER_PART_INDEXED:
    binding = PER_SEGMENT_INDEXED;
    break;
  case SoNormalBindingElement::PER_FACE:
    binding = PER_LINE;
    break;
  case SoNormalBindingElement::PER_FACE_INDEXED:
    binding = PER_LINE_INDEXED;
    break;
  default:
#if COIN_DEBUG
    SoDebugError::postWarning("SoIndexedLineSet::findNormalBinding",
                              "unknown normal binding setting");
#endif // COIN_DEBUG
    break;
  }

  return binding;
}

//
// translates current material binding into the internal Binding enum
//
SoIndexedLineSet::Binding
SoIndexedLineSet::findMaterialBinding(SoState * state)
{
  Binding binding = OVERALL;

  SoMaterialBindingElement::Binding matbind =
    (SoMaterialBindingElement::Binding) SoMaterialBindingElement::get(state);

  switch (matbind) {
  case SoNormalBindingElement::OVERALL:
    binding = OVERALL;
    break;
  case SoNormalBindingElement::PER_VERTEX:
    binding = PER_VERTEX;
    break;
  case SoNormalBindingElement::PER_VERTEX_INDEXED:
    binding = PER_VERTEX_INDEXED;
    break;
  case SoNormalBindingElement::PER_PART:
    binding = PER_SEGMENT;
    break;
  case SoNormalBindingElement::PER_PART_INDEXED:
    binding = PER_SEGMENT_INDEXED;
    break;
  case SoNormalBindingElement::PER_FACE:
    binding = PER_LINE;
    break;
  case SoNormalBindingElement::PER_FACE_INDEXED:
    binding = PER_LINE_INDEXED;
    break;
  default:
#if COIN_DEBUG
    SoDebugError::postWarning("SoIndexedFaceSet::findNormalBinding",
                              "unknown normal binding setting");
#endif // COIN_DEBUG
    break;
  }

  return binding;
}


// doc from parent
void
SoIndexedLineSet::GLRender(SoGLRenderAction * action)
{
  if (this->coordIndex.getNum() < 2) return;
  SoState * state = action->getState();

  if (!this->shouldGLRender(action)) return;
  
  SbBool didpush = FALSE;

  if (this->vertexProperty.getValue()) {
    state->push();
    didpush = TRUE;
    this->vertexProperty.getValue()->GLRender(action);
  }

  SoMaterialBundle mb(action);
  SoTextureCoordinateBundle tb(action, TRUE, FALSE);
  SbBool doTextures = tb.needCoordinates();

  const SoCoordinateElement * coords;
  const SbVec3f * normals;
  const int32_t * cindices;
  int numindices;
  const int32_t * nindices;
  const int32_t * tindices;
  const int32_t * mindices;
  SbBool normalCacheUsed;

  SbBool sendNormals = !mb.isColorOnly() || tb.isFunction();

  getVertexData(state, coords, normals, cindices,
                nindices, tindices, mindices, numindices,
                sendNormals, normalCacheUsed);

  if (sendNormals && normals == NULL) {
    if (!didpush) {
      state->push();
      didpush = TRUE;
    }
    sendNormals = FALSE;
    SoLazyElement::setLightModel(state, SoLazyElement::BASE_COLOR);
  }

  Binding mbind = this->findMaterialBinding(state);
  Binding nbind = this->findNormalBinding(state);

  if (this->getNodeType() == SoNode::VRML1) {
    // For VRML1, PER_VERTEX means per vertex in shape, not PER_VERTEX
    // on the state.
    if (mbind == PER_VERTEX) {
      mbind = PER_VERTEX_INDEXED;
      mindices = cindices;
    }
    if (nbind == PER_VERTEX) {
      nbind = PER_VERTEX_INDEXED;
      nindices = cindices;
    }
  }

  if (doTextures) {
    if (SoTextureCoordinateBindingElement::get(state) ==
        SoTextureCoordinateBindingElement::PER_VERTEX) {
      tindices = NULL; // just in case
    }
    else if (tindices == NULL) {
      tindices = cindices;
    }
  }

  mb.sendFirst(); // make sure we have the correct material

  if (!sendNormals) nbind = OVERALL;
  else if (nbind == OVERALL) {
    glNormal3fv((const GLfloat *)normals);
  }

  SbBool drawPoints =
    SoDrawStyleElement::get(state) == SoDrawStyleElement::POINTS;
  
  SoGLLazyElement * lelem = NULL;
  SbBool dova = 
    (numindices > 50) &&
    ((nbind == OVERALL) || ((nbind == PER_VERTEX_INDEXED) && ((nindices == cindices) || (nindices == NULL)))) &&
    (!doTextures || (tindices == cindices)) &&
    ((mbind == OVERALL) || ((mbind == PER_VERTEX_INDEXED) && ((mindices == cindices) || (mindices == NULL)))) &&
    cc_glglue_has_vertex_array(sogl_glue_instance(state));

  const SoGLVBOElement * vboelem = SoGLVBOElement::getInstance(state);
  SoVBO * colorvbo = NULL;

  if (dova && (mbind != OVERALL)) {
    dova = FALSE;
    if ((mbind == PER_VERTEX_INDEXED) && ((mindices == cindices) || (mindices == NULL))) {
      lelem = (SoGLLazyElement*) SoLazyElement::getInstance(state);
      colorvbo = vboelem->getColorVBO();
      if (colorvbo) dova = TRUE;
      else {
        // we might be able to do VA-rendering, but need to check the
        // diffuse color type first.
        if (!lelem->isPacked() && lelem->getNumTransparencies() <= 1) {
          dova = TRUE;
        }
      }
    }
  }
  if (dova) {
    SbBool dovbo = TRUE;
    const GLvoid * dataptr = NULL;
    const uint32_t contextid = action->getCacheContext();
    const cc_glglue * glue = sogl_glue_instance(state);
    SoVBO * vbo = vboelem->getVertexVBO();
    if (!vbo) dovbo = FALSE;

    const SoTextureCoordinateElement * telem = NULL;
    const SoMultiTextureCoordinateElement * mtelem = NULL;
    const SbBool * enabledunits = NULL;
    int lastenabled;

    if (doTextures) {
      telem = SoTextureCoordinateElement::getInstance(state);
      enabledunits = SoMultiTextureEnabledElement::getEnabledUnits(state, lastenabled);
      if (enabledunits) {
        mtelem = SoMultiTextureCoordinateElement::getInstance(state);
      }
    }

    if (mbind != OVERALL) {
      dataptr = NULL;
      assert(lelem != NULL);
      if (colorvbo) {
        lelem->updateColorVBO(colorvbo);
        colorvbo->bindBuffer(contextid);
      }
      else {
        cc_glglue_glBindBuffer(glue, GL_ARRAY_BUFFER, 0);
        dataptr = (const GLvoid*) lelem->getDiffusePointer();
      }
      if (colorvbo) {
        cc_glglue_glColorPointer(glue, 4, GL_UNSIGNED_BYTE, 0, dataptr);
      }
      else {
        cc_glglue_glColorPointer(glue, 3, GL_FLOAT, 0, dataptr);
      }
      cc_glglue_glEnableClientState(glue, GL_COLOR_ARRAY);
    }
    
    if (doTextures) {
      if (telem->getNum()) {
        int dim = telem->getDimension();
        const GLvoid * tptr;
        switch (dim) {
        default:
        case 2: tptr = (const GLvoid*) telem->getArrayPtr2(); break;
        case 3: tptr = (const GLvoid*) telem->getArrayPtr3(); break;
        case 4: tptr = (const GLvoid*) telem->getArrayPtr4(); break;
        }
        vbo = dovbo ? vboelem->getTexCoordVBO(0) : NULL;
        if (vbo) {
          vbo->bindBuffer(contextid);
          tptr = NULL;
        }
        else {
          cc_glglue_glBindBuffer(glue, GL_ARRAY_BUFFER, 0);
        }
        cc_glglue_glTexCoordPointer(glue, dim, GL_FLOAT, 0, tptr);
        cc_glglue_glEnableClientState(glue, GL_TEXTURE_COORD_ARRAY);
      }
      for (int i = 1; i <= lastenabled; i++) {
        if (enabledunits[i] && mtelem->getNum(i)) {
          int dim = mtelem->getDimension(i);
          const GLvoid * tptr;
          switch (dim) {
          default:
          case 2: tptr = (const GLvoid*) mtelem->getArrayPtr2(i); break;
          case 3: tptr = (const GLvoid*) mtelem->getArrayPtr3(i); break;
          case 4: tptr = (const GLvoid*) mtelem->getArrayPtr4(i); break;
          }
          cc_glglue_glClientActiveTexture(glue, GL_TEXTURE0 + i);
          vbo = dovbo ? vboelem->getTexCoordVBO(i) : NULL;
          if (vbo) {
            vbo->bindBuffer(contextid);
            tptr = NULL;
          }
          else {
            cc_glglue_glBindBuffer(glue, GL_ARRAY_BUFFER, 0);
          }
          cc_glglue_glTexCoordPointer(glue, dim, GL_FLOAT, 0, tptr);
          cc_glglue_glEnableClientState(glue, GL_TEXTURE_COORD_ARRAY);
        }
      }
    }
    if (nbind != OVERALL) {
      vbo = dovbo ? vboelem->getNormalVBO() : NULL;
      dataptr = NULL;
      if (vbo) {
        vbo->bindBuffer(contextid);
      }
      else {
        dataptr = (const GLvoid*) normals;
        cc_glglue_glBindBuffer(glue, GL_ARRAY_BUFFER, 0);
      }
      cc_glglue_glNormalPointer(glue, GL_FLOAT, 0, dataptr);
      cc_glglue_glEnableClientState(glue, GL_NORMAL_ARRAY);
    }
    vbo = vboelem->getVertexVBO();
    dataptr = NULL;
    if (vbo) {
      vbo->bindBuffer(contextid);
    }
    else {
      dataptr = coords->is3D() ? 
        ((const GLvoid *)coords->getArrayPtr3()) : 
        ((const GLvoid *)coords->getArrayPtr4());
    }
    cc_glglue_glVertexPointer(glue, coords->is3D() ? 3 : 4, GL_FLOAT, 0,
                              dataptr);
    cc_glglue_glEnableClientState(glue, GL_VERTEX_ARRAY);


    LOCK_VAINDEXER(this);
    if (PRIVATE(this)->vaindexer == NULL) {
      SoVertexArrayIndexer * indexer = new SoVertexArrayIndexer;
      
      int i = 0;
      while (i < numindices) {
        int cnt = 0;
        while (i + cnt < numindices && cindices[i+cnt] >= 0) cnt++;
        if (cnt >= 2) {
          for (int j = 1; j < cnt;j++) {
            indexer->addLine(cindices[i+j-1],
                             cindices[i+j]);
          }
        }
        i += cnt + 1;
      }
      indexer->close();
      if (indexer->getNumVertices()) {
        PRIVATE(this)->vaindexer = indexer;
      }
      else {
        delete indexer;
      }
#if 1
      fprintf(stderr,"XXX: ILS create VertexArrayIndexer: %d\n", indexer->getNumVertices());
#endif
    }
    
    if (PRIVATE(this)->vaindexer) {
      // don't cache when rendering with vertex arrays
      SoGLCacheContextElement::shouldAutoCache(state, SoGLCacheContextElement::DONT_AUTO_CACHE);
      PRIVATE(this)->vaindexer->render(glue, dovbo, contextid);
      if (mbind != OVERALL) {
        assert(lelem != NULL);
        lelem->reset(state, SoLazyElement::DIFFUSE_MASK);
      }
    }
    if (dovbo) {
      cc_glglue_glBindBuffer(glue, GL_ARRAY_BUFFER, 0);
    }
    UNLOCK_VAINDEXER(this);
    
    // disable client states again
    cc_glglue_glDisableClientState(glue, GL_VERTEX_ARRAY);
    if (nbind != OVERALL) {
      cc_glglue_glDisableClientState(glue, GL_NORMAL_ARRAY);
    }
    if (doTextures) {
      for (int i = 1; i <= lastenabled; i++) {
        if (enabledunits[i] && mtelem->getNum(i)) {
          cc_glglue_glClientActiveTexture(glue, GL_TEXTURE0 + i);
          cc_glglue_glDisableClientState(glue, GL_TEXTURE_COORD_ARRAY);
        }
      }
      cc_glglue_glClientActiveTexture(glue, GL_TEXTURE0);
      if (telem->getNum()) {
        cc_glglue_glDisableClientState(glue, GL_TEXTURE_COORD_ARRAY);
      }
    }
  }
  else {
    sogl_render_lineset((SoGLCoordinateElement*)coords,
                        cindices,
                        numindices,
                        normals,
                        nindices,
                        &mb,
                        mindices,
                        &tb,
                        tindices,
                        (int)nbind,
                        (int)mbind,
                        doTextures ? 1 : 0,
                        drawPoints ? 1 : 0);
  }

  if (didpush) {
    state->pop();
  }
  // send approx number of lines for autocache handling
  sogl_autocache_update(state, this->coordIndex.getNum() / 2);
}

// Documented in superclass.
SbBool
SoIndexedLineSet::generateDefaultNormals(SoState *, SoNormalBundle *)
{
  return FALSE;
}

// Documented in superclass.
SbBool
SoIndexedLineSet::generateDefaultNormals(SoState * state, SoNormalCache * nc)
{
  // not possible to generate normals for IndexedLineSet
  nc->set(0, NULL);
  return TRUE;
}

// doc from parent
void
SoIndexedLineSet::getBoundingBox(SoGetBoundingBoxAction * action)
{
  inherited::getBoundingBox(action);
  // notify open (if any) bbox caches about lines in this shape
  SoBoundingBoxCache::setHasLinesOrPoints(action->getState());
}

// doc from parent
void
SoIndexedLineSet::getPrimitiveCount(SoGetPrimitiveCountAction *action)
{
  if (!this->shouldPrimitiveCount(action)) return;

  int n = this->coordIndex.getNum();
  if (n < 2) return;

  if (action->canApproximateCount()) {
    action->addNumLines(n/3);
  }
  else {
    const int32_t * ptr = coordIndex.getValues(0);
    const int32_t * endptr = ptr + n;
    int cnt = 0;
    int add = 0;
    while (ptr < endptr) {
      if (*ptr++ >= 0) cnt++;
      else {
        add += cnt-1;
        cnt = 0;
      }
    }
    // in case index array wasn't terminated by a -1
    if (cnt >= 2) add += cnt-1;
    action->addNumLines(add);
  }
}

// doc from parent
void
SoIndexedLineSet::generatePrimitives(SoAction *action)
{
  if (this->coordIndex.getNum() < 2) return;

  SoState * state = action->getState();

  if (this->vertexProperty.getValue()) {
    state->push();
    this->vertexProperty.getValue()->doAction(action);
  }

  Binding mbind = this->findMaterialBinding(state);
  Binding nbind = this->findNormalBinding(state);

  const SoCoordinateElement * coords;
  const SbVec3f * normals;
  const int32_t * cindices;
  int numindices;
  const int32_t * normindices;
  const int32_t * texindices;
  const int32_t * matindices;
  SbBool doTextures;
  SbBool sendNormals = TRUE;
  SbBool normalCacheUsed;

  getVertexData(state, coords, normals, cindices,
                normindices, texindices, matindices, numindices,
                sendNormals, normalCacheUsed);

  if (normals == NULL) {
    sendNormals = FALSE;
    nbind = OVERALL;
  }

  if (this->getNodeType() == SoNode::VRML1) {
    // For VRML1, PER_VERTEX means per vertex in shape, not PER_VERTEX
    // on the state.
    if (mbind == PER_VERTEX) {
      mbind = PER_VERTEX_INDEXED;
      matindices = cindices;
    }
    if (nbind == PER_VERTEX) {
      nbind = PER_VERTEX_INDEXED;
      normindices = cindices;
    }
  }

  SoTextureCoordinateBundle tb(action, FALSE, FALSE);
  doTextures = tb.needCoordinates();

  if (doTextures) {
    if (SoTextureCoordinateBindingElement::get(state) ==
        SoTextureCoordinateBindingElement::PER_VERTEX) {
      texindices = NULL; // just in case
    }
    else if (texindices == NULL) {
      texindices = cindices;
    }
  }

  if (mbind == PER_VERTEX_INDEXED && matindices == NULL) {
    matindices = cindices;
  }
  if (nbind == PER_VERTEX_INDEXED && normindices == NULL) {
    normindices = cindices;
  }
  if (mbind == PER_VERTEX || mbind == PER_LINE || mbind == PER_SEGMENT) {
    matindices = NULL;
  }
  if (nbind == PER_VERTEX || nbind == PER_LINE || nbind == PER_SEGMENT) {
    normindices = NULL;
  }

  if (nbind == OVERALL) normindices = NULL;
  if (mbind == OVERALL) matindices = NULL;

  int matnr = 0;
  int normnr = 0;
  int texidx = 0;
  int32_t i;
  const int32_t *end = cindices + numindices;

  SoPrimitiveVertex vertex;
  SoPointDetail pointDetail;
  SoLineDetail lineDetail;

  vertex.setDetail(&pointDetail);

  SbVec3f dummynormal(0.0f, 0.0f, 1.0f);
  const SbVec3f *currnormal = &dummynormal;
  if (normals) currnormal = normals;

  if (nbind == OVERALL) {
    vertex.setNormal(*currnormal);
  }

  if (mbind == PER_SEGMENT || mbind == PER_SEGMENT_INDEXED ||
      nbind == PER_SEGMENT || nbind == PER_SEGMENT_INDEXED) {
    int previ;
    SbBool matPerPolyline = mbind == PER_LINE || mbind == PER_LINE_INDEXED;
    SbBool normPerPolyline = nbind == PER_LINE || nbind == PER_LINE_INDEXED;

    this->beginShape(action, SoShape::LINES, &lineDetail);

    while (cindices + 1 < end) { // need at least two vertices
      previ = *cindices++;

      if (matPerPolyline || mbind >= PER_VERTEX) {
        if (matindices) vertex.setMaterialIndex(*matindices++);
        else vertex.setMaterialIndex(matnr++);
        pointDetail.setMaterialIndex(vertex.getMaterialIndex());
      }
      if (normPerPolyline || nbind >= PER_VERTEX) {
        if (normindices) {
          pointDetail.setNormalIndex(*normindices);
          currnormal = &normals[*normindices++];
        }
        else {
          pointDetail.setNormalIndex(normnr);
          currnormal = &normals[normnr++];
        }
        vertex.setNormal(*currnormal);
      }
      if (doTextures) {
        if (tb.isFunction()) {
          vertex.setTextureCoords(tb.get(coords->get3(previ), *currnormal));
        }
        else {
          pointDetail.setTextureCoordIndex(texindices?*texindices:texidx);
          vertex.setTextureCoords(tb.get(texindices?*texindices++:texidx++));
        }
      }

      while (cindices < end && (i = *cindices++) >= 0) {
        if (mbind == PER_SEGMENT || mbind == PER_SEGMENT_INDEXED) {
          if (matindices) vertex.setMaterialIndex(*matindices++);
          else vertex.setMaterialIndex(matnr++);
          pointDetail.setMaterialIndex(vertex.getMaterialIndex());
        }
        if (nbind == PER_SEGMENT || nbind == PER_SEGMENT_INDEXED) {
          if (normindices) {
            pointDetail.setNormalIndex(*normindices);
            currnormal = &normals[*normindices++];
          }
          else {
            pointDetail.setNormalIndex(normnr);
            currnormal = &normals[normnr++];
          }
          vertex.setNormal(*currnormal);
        }
        pointDetail.setCoordinateIndex(previ);
        vertex.setPoint(coords->get3(previ));
        this->shapeVertex(&vertex);

        if (mbind >= PER_VERTEX) {
          if (matindices) vertex.setMaterialIndex(*matindices++);
          else vertex.setMaterialIndex(matnr++);
          pointDetail.setMaterialIndex(vertex.getMaterialIndex());
        }
        if (nbind >= PER_VERTEX) {
          if (normindices) {
            pointDetail.setNormalIndex(*normindices);
            currnormal = &normals[*normindices++];
          }
          else {
            pointDetail.setNormalIndex(normnr);
            currnormal = &normals[normnr++];
          }
          vertex.setNormal(*currnormal);
        }
        if (doTextures) {
          if (tb.isFunction()) {
            vertex.setTextureCoords(tb.get(coords->get3(i), *currnormal));
          }
          else {
            pointDetail.setTextureCoordIndex(texindices?*texindices:texidx);
            vertex.setTextureCoords(tb.get(texindices?*texindices++:texidx++));
          }
        }
        pointDetail.setCoordinateIndex(i);
        vertex.setPoint(coords->get3(i));
        this->shapeVertex(&vertex);
        lineDetail.incPartIndex();
        previ = i;
      }
      lineDetail.incLineIndex();
      if (mbind == PER_VERTEX_INDEXED) matindices++;
      if (nbind == PER_VERTEX_INDEXED) normindices++;
      if (doTextures && texindices) texindices++;
    }
    this->endShape();
    return;
  }

  while (cindices + 1 < end) { // need at least two vertices
    this->beginShape(action, LINE_STRIP, &lineDetail);
    i = *cindices++;
    assert(i >= 0);
    if (matindices) {
      pointDetail.setMaterialIndex(*matindices);
      vertex.setMaterialIndex(*matindices++);
    }
    else if (mbind != OVERALL) {
      pointDetail.setMaterialIndex(matnr);
      vertex.setMaterialIndex(matnr++);
    }
    if (normindices) {
      pointDetail.setNormalIndex(*normindices);
      currnormal = &normals[*normindices++];
    }
    else if (nbind != OVERALL) {
      pointDetail.setNormalIndex(normnr);
      currnormal = &normals[normnr++];
    }
    vertex.setNormal(*currnormal);
    if (doTextures) {
      if (tb.isFunction()) {
        vertex.setTextureCoords(tb.get(coords->get3(i), *currnormal));
      }
      else {
        pointDetail.setTextureCoordIndex(texindices?*texindices:texidx);
        vertex.setTextureCoords(tb.get(texindices?*texindices++:texidx++));
      }
    }
    pointDetail.setCoordinateIndex(i);
    vertex.setPoint(coords->get3(i));
    this->shapeVertex(&vertex);

    i = *cindices++;
    assert(i >= 0);
    if (mbind >= PER_VERTEX) {
      if (matindices) vertex.setMaterialIndex(*matindices++);
      else vertex.setMaterialIndex(matnr++);
      pointDetail.setMaterialIndex(vertex.getMaterialIndex());
    }
    if (nbind >= PER_VERTEX) {
      if (normindices) {
        pointDetail.setNormalIndex(*normindices);
        currnormal = &normals[*normindices++];
      }
      else {
        pointDetail.setNormalIndex(normnr);
        currnormal = &normals[normnr++];
      }
      vertex.setNormal(*currnormal);
    }
    if (doTextures) {
      if (tb.isFunction()) {
        vertex.setTextureCoords(tb.get(coords->get3(i), *currnormal));
      }
      else {
        pointDetail.setTextureCoordIndex(texindices?*texindices:texidx);
        vertex.setTextureCoords(tb.get(texindices?*texindices++:texidx++));
      }
    }
    pointDetail.setCoordinateIndex(i);
    vertex.setPoint(coords->get3(i));
    this->shapeVertex(&vertex);
    lineDetail.incPartIndex();

    while (cindices < end && (i = *cindices++) >= 0) {
      assert(cindices <= end);
      if (mbind >= PER_VERTEX) {
        if (matindices) vertex.setMaterialIndex(*matindices++);
        else vertex.setMaterialIndex(matnr++);
        pointDetail.setMaterialIndex(vertex.getMaterialIndex());
      }
      if (nbind >= PER_VERTEX) {
        if (normindices) {
          pointDetail.setNormalIndex(*normindices);
          currnormal = &normals[*normindices++];
        }
        else {
          pointDetail.setNormalIndex(normnr);
          currnormal = &normals[normnr++];
        }
        vertex.setNormal(*currnormal);
      }
      if (doTextures) {
        if (tb.isFunction()) {
          vertex.setTextureCoords(tb.get(coords->get3(i), *currnormal));
        }
        else {
          pointDetail.setTextureCoordIndex(texindices?*texindices:texidx);
          vertex.setTextureCoords(tb.get(texindices?*texindices++:texidx++));
        }
      }
      pointDetail.setCoordinateIndex(i);
      vertex.setPoint(coords->get3(i));
      this->shapeVertex(&vertex);
      lineDetail.incPartIndex();
    }
    this->endShape(); // end of line strip
    if (mbind == PER_VERTEX_INDEXED) matindices++;
    if (nbind == PER_VERTEX_INDEXED) normindices++;
    if (doTextures && texindices) texindices++;
    lineDetail.incLineIndex();
  }

  if (this->vertexProperty.getValue()) {
    state->pop();
  }
}

void 
SoIndexedLineSet::notify(SoNotList * list)
{
  SoField *f = list->getLastField();
  if (f == &this->coordIndex) {
    LOCK_VAINDEXER(this);
    if (PRIVATE(this)->vaindexer) {
      delete PRIVATE(this)->vaindexer;
      PRIVATE(this)->vaindexer = NULL;
    }
    UNLOCK_VAINDEXER(this);
  }
  inherited::notify(list);
}


#undef LOCK_VAINDEXER
#undef PRIVATE
#undef UNLOCK_VAINDEXER
