/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

/*!
  \class SoGLLazyElement Inventor/elements/SoGLLazyElement.h
  \brief The SoGLLazyElement class is meant to optimize GL rendering.
  \ingroup elements

  This is just a wrap-around implementation for compatibility. It should
  (hopefully) work in the same way as the Inventor class though.
*/

#include <Inventor/elements/SoGLLazyElement.h>
#include <Inventor/elements/SoShapeStyleElement.h>
#include <Inventor/elements/SoGLTextureImageElement.h>
#include <Inventor/elements/SoGLTextureEnabledElement.h>
#include <Inventor/elements/SoGLTexture3EnabledElement.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/C/glue/gl.h>
#include <Inventor/misc/SoGLImage.h>
#include <Inventor/SbImage.h>
#include <Inventor/elements/SoGLCacheContextElement.h>

#include <Inventor/bundles/SoMaterialBundle.h>
#include <coindefs.h> // COIN_OBSOLETED
#include <assert.h>
#include <Inventor/errors/SoDebugError.h>

#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

// Some data and functions to create Bayer dither matrices (used for
// screen door transparency)
static unsigned char stipple_patterns[64 + 1][32 * 4];
static unsigned int two_by_two[] = {0, 2, 3, 1};

//
// Used to generate a matrix twice the size of the input
//
static void 
generate_next_matrix(unsigned int * old, int oldsize,
                     unsigned int * matrix)
{
  int i,j;
  int newsize = oldsize << 1;
  for (i = 0; i <  newsize; i++) {
    for (j = 0; j < newsize; j++) {
      matrix[i*newsize+j] = 4 * old[(i%oldsize)*oldsize + (j%oldsize)];
      matrix[i*newsize+j] += two_by_two[(i/oldsize)*2 + (j/oldsize)];
    }
  }
}

//
// Creates a matrix by starting with a 2x2 and doubling until size
//
static void 
make_dither_matrix(uint32_t * ptr, int size)
{
  int currsize = 2;

  unsigned int * currmatrix = two_by_two;
  unsigned int * nextmatrix = NULL;
  int nextsize;

  while (currsize < size) {
    nextsize = currsize << 1;
    nextmatrix = new unsigned int[nextsize*nextsize];
    generate_next_matrix(currmatrix, currsize, nextmatrix);
    if (currmatrix != two_by_two) delete[] currmatrix;
    currmatrix = nextmatrix;
    currsize = nextsize;
  }
  // copy matrix
  int i;
  for (i = 0; i < size*size; i++)
    ptr[i] = currmatrix[i];

  if (currmatrix != two_by_two) delete[] currmatrix;
}

//
// Sets a bit bitnr bits from ptr
//
static void 
set_bit(int bitnr, unsigned char * ptr)
{
  int byte = bitnr / 8;
  int bit = bitnr % 8;
  
  unsigned char mask = (unsigned char) (0x80 >> bit);

  ptr[byte] |= mask;
}

//
// Create a bitmap from a 32x32 matrix
//
static void 
create_matrix_bitmap(int intensity, unsigned char * bitmap,
                     uint32_t *matrix, int size)
{
  int cnt = 0;
  int i,j;
  for (i = 0; i < 32*4; i++) bitmap[i] = 0;
  for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      if (matrix[i*size+j] > (unsigned int) intensity) {
        set_bit(i*32+j, bitmap);
        cnt++;
      }
    }
  }
}


SO_ELEMENT_SOURCE(SoGLLazyElement);

/*!
  This static method initializes static data for the
  SoDiffuseColorElement class.
*/

void
SoGLLazyElement::initClass()
{
  SO_ELEMENT_INIT_CLASS(SoGLLazyElement, inherited);

  // create stipple patterns
  int i;
  unsigned int matrix[32*32];
  make_dither_matrix(matrix, 32); 
  for (i = 0; i <= 64; i++) {
    int intensity = (32 * 32 * i) / 64 - 1;
    create_matrix_bitmap((intensity >= 0) ? intensity : 0,
                         stipple_patterns[i], matrix, 32);
  }
}

// ! FIXME: write doc

SoGLLazyElement::~SoGLLazyElement()
{
}

//! FIXME: write doc
inline SoGLLazyElement *
SoGLLazyElement::getInstance(const SoState *state)
{
  return (SoGLLazyElement*)
    state->getConstElement(classStackIndex);
}

inline void 
SoGLLazyElement::sendPackedDiffuse(const uint32_t col) const
{
  glColor4ub((col>>24)&0xff, (col>>16)&0xff, (col>>8)&0xff, col&0xff);
  ((SoGLLazyElement*)this)->glstate.diffuse = col;
  ((SoGLLazyElement*)this)->cachebitmask |= DIFFUSE_MASK;
}

inline void
SoGLLazyElement::sendLightModel(const int32_t model) const
{
  if (model == PHONG) glEnable(GL_LIGHTING);
  else glDisable(GL_LIGHTING);
  ((SoGLLazyElement*)this)->glstate.lightmodel = model;
  ((SoGLLazyElement*)this)->cachebitmask |= LIGHT_MODEL_MASK;
}

inline void 
SoGLLazyElement::sendFlatshading(const SbBool onoff) const
{
  if (onoff) glShadeModel(GL_FLAT);
  else glShadeModel(GL_SMOOTH);
  ((SoGLLazyElement*)this)->glstate.flatshading = (int32_t) onoff;
  ((SoGLLazyElement*)this)->cachebitmask |= SHADE_MODEL_MASK;
}

void
SoGLLazyElement::sendGLImage(const uint32_t glimageid) const
{
  SoGLImage * glimage = NULL;
  SbBool alphatest = FALSE;

  if (glimageid != 0) {
    SoTextureImageElement::Model model;
    SbColor blendcolor;
    SoGLImage * glimage = SoGLTextureImageElement::get(this->state, model, blendcolor);
    
    if (glimage) {
      SoGLDisplayList * dl = glimage->getGLDisplayList(this->state);
      
      SbBool enabled = 
        SoGLTextureEnabledElement::get(this->state) ||
        SoGLTexture3EnabledElement::get(this->state);
      
#ifdef HAVE_THREADS
      // if threads is enabled, the image is loaded on demand, and we
      // should trigger an image load by just attempting to fetch the
      // data from the image.
      if (!enabled && glimage->getImage()) {
        SbVec3s size;
        int nc;
        (void) glimage->getImage()->getValue(size, nc);
      }
#endif // HAVE_THREADS
      if (enabled && dl) {
        // tag image (for GLImage LRU cache).
        SoGLImage::tagImage(this->state, glimage);
        switch (model) {
        case SoTextureImageElement::DECAL:
          glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
          break;
        case SoTextureImageElement::MODULATE:
          glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
          break;
        case SoTextureImageElement::BLEND:
          glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
          glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, blendcolor.getValue());
          break;
        }
        dl->call(this->state);
      }
    }
  }
  ((SoGLLazyElement*)this)->cachebitmask |= GLIMAGE_MASK;
  ((SoGLLazyElement*)this)->glstate.glimageid = (int32_t) glimageid;
}

inline void 
SoGLLazyElement::sendAlphaTest(const SbBool onoff) const
{
  if (onoff) {
    glAlphaFunc(GL_GREATER, 0.5f);
    glEnable(GL_ALPHA_TEST);
  }
  else {
    glDisable(GL_ALPHA_TEST);
  }
  ((SoGLLazyElement*)this)->cachebitmask |= ALPHATEST_MASK;
  ((SoGLLazyElement*)this)->glstate.alphatest = onoff;
}


inline void 
SoGLLazyElement::sendVertexOrdering(const VertexOrdering ordering) const
{
  glFrontFace(ordering == CW ? GL_CW : GL_CCW);
  ((SoGLLazyElement*)this)->glstate.vertexordering = (int32_t) ordering;
  ((SoGLLazyElement*)this)->cachebitmask |= VERTEXORDERING_MASK;
}

inline void 
SoGLLazyElement::sendTwosideLighting(const SbBool onoff) const
{
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, onoff ? GL_TRUE : GL_FALSE);
  ((SoGLLazyElement*)this)->glstate.twoside = (int32_t) onoff;
  ((SoGLLazyElement*)this)->cachebitmask |= TWOSIDE_MASK;
}

inline void 
SoGLLazyElement::sendBackfaceCulling(const SbBool onoff) const
{
  if (onoff) glEnable(GL_CULL_FACE);
  else glDisable(GL_CULL_FACE);
  ((SoGLLazyElement*)this)->glstate.flatshading = onoff;
  ((SoGLLazyElement*)this)->cachebitmask |= CULLING_MASK;
}

inline void
send_gl_material(GLenum pname, const SbColor & color)
{
  GLfloat col[4];
  color.getValue(col[0], col[1], col[2]);
  col[3] = 1.0f;
  glMaterialfv(GL_FRONT_AND_BACK, pname, col);
} 


inline void 
SoGLLazyElement::sendAmbient(const SbColor & color) const
{
  send_gl_material(GL_AMBIENT, color);
  ((SoGLLazyElement*)this)->glstate.ambient = color;
  ((SoGLLazyElement*)this)->cachebitmask |= AMBIENT_MASK;
}

inline void 
SoGLLazyElement::sendEmissive(const SbColor & color) const
{
  send_gl_material(GL_EMISSION, color);
  ((SoGLLazyElement*)this)->glstate.emissive = color;
  ((SoGLLazyElement*)this)->cachebitmask |= EMISSIVE_MASK;
}

inline void 
SoGLLazyElement::sendSpecular(const SbColor & color) const
{
  send_gl_material(GL_SPECULAR, color);
  ((SoGLLazyElement*)this)->glstate.specular = color;
  ((SoGLLazyElement*)this)->cachebitmask |= SPECULAR_MASK;
}

inline void 
SoGLLazyElement::sendShininess(const float shine) const
{
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shine*128.0f);
  ((SoGLLazyElement*)this)->glstate.shininess = shine;
  ((SoGLLazyElement*)this)->cachebitmask |= SHININESS_MASK;
}

inline void 
SoGLLazyElement::sendTransparency(const int stipplenum) const
{
  if (stipplenum == 0) {
    glDisable(GL_POLYGON_STIPPLE);
  }
  else {
    if (this->glstate.stipplenum <= 0) glEnable(GL_POLYGON_STIPPLE);
    glPolygonStipple(stipple_patterns[stipplenum]);
  }
  ((SoGLLazyElement*)this)->glstate.stipplenum = stipplenum;
  ((SoGLLazyElement*)this)->cachebitmask |= TRANSPARENCY_MASK;
}

inline void 
SoGLLazyElement::sendBlending(const SbBool blend) const
{
  if (blend) glEnable(GL_BLEND);
  else glDisable(GL_BLEND);
  ((SoGLLazyElement*)this)->glstate.blending = blend;
  ((SoGLLazyElement*)this)->cachebitmask |= BLENDING_MASK;
}

void 
SoGLLazyElement::init(SoState * state)
{
  inherited::init(state);
  this->state = state; // needed to send GL texture
  this->colorindex = FALSE;
  this->glstate.ambient.setValue(-1.0f, -1.0f, -1.0f);
  this->glstate.emissive.setValue(-1.0f, -1.0f, -1.0f); 
  this->glstate.specular.setValue(-1.0f, -1.0f, -1.0f);
  this->glstate.shininess = -1.0f;
  this->glstate.lightmodel = -1;
  this->glstate.blending = -1;
  this->glstate.stipplenum = -1;
  this->glstate.vertexordering = -1;
  this->glstate.twoside = -1;
  this->glstate.culling = -1;
  this->glstate.flatshading = -1;
  this->glstate.glimageid = -1;
  this->glstate.alphatest = -1;
  this->packedpointer = NULL;
  this->transpmask = 0xff;
  this->colorpacker = NULL;
  this->precachestate = NULL;
  this->postcachestate = NULL;
  glDisable(GL_POLYGON_STIPPLE);

  GLboolean rgba;
  glGetBooleanv(GL_RGBA_MODE, &rgba);
  if (!rgba) this->colorindex = TRUE;
  else {
    this->sendPackedDiffuse(0xccccccff);
  }
}

void 
SoGLLazyElement::push(SoState * state)
{
  inherited::push(state);
  SoGLLazyElement * prev = (SoGLLazyElement*) this->getNextInStack();
  this->state = state; // needed to send GL texture
  this->glstate = prev->glstate;
  this->colorindex = prev->colorindex;
  this->transpmask = prev->transpmask;
  this->colorpacker = prev->colorpacker;
  this->precachestate = prev->precachestate;
  this->postcachestate = prev->postcachestate;
  this->didsetbitmask = prev->didsetbitmask;
  this->didntsetbitmask = prev->didntsetbitmask;
  this->cachebitmask = prev->cachebitmask;
}

void 
SoGLLazyElement::pop(SoState *state, const SoElement * prevtopelement)
{
  inherited::pop(state, prevtopelement);
  SoGLLazyElement * prev = (SoGLLazyElement*) prevtopelement;
  this->glstate = prev->glstate;
  this->colorindex = prev->colorindex;
  this->didsetbitmask = prev->didsetbitmask;
  this->didntsetbitmask = prev->didntsetbitmask;
  this->cachebitmask = prev->cachebitmask;
}

//! FIXME: write doc

void
SoGLLazyElement::sendAllMaterial(SoState * state)
{
  assert(0 && "Not implemented yet. Provided for API compatibility.");
}

//! FIXME: write doc

void
SoGLLazyElement::sendNoMaterial(SoState * state)
{
  assert(0 && "Not implemented yet. Provided for API compatibility.");
}

//! FIXME: write doc

void
SoGLLazyElement::sendOnlyDiffuseColor(SoState * state)
{
  assert(0 && "Not implemented yet. Provided for API compatibility.");
}

//! FIXME: write doc

void
SoGLLazyElement::sendDiffuseByIndex(int index) const
{
#if COIN_DEBUG
  if (index < 0 || index >= this->coinstate.numdiffuse) {
    static int first = 1;
    if (first) {
      SoDebugError::postWarning("SoGLLazyElement::sendDiffuseByIndex",
                                "index %d out of bounds [0, %d] "
                                "(this warning will only be printed once, but there "
                                "might be more errors)",
                                index,
                                this->coinstate.numdiffuse-1);
      first = 0;
    }
    
    index = SbClamp(index, 0, this->coinstate.numdiffuse-1);
  }
#endif // COIN_DEBUG

  if (this->colorindex) {
    glIndexi((GLint)this->coinstate.colorindexarray[index]);
  }
  else {
    uint32_t col = this->packedpointer[index] | this->transpmask;
    if (col != this->glstate.diffuse) this->sendPackedDiffuse(col);
  }
}

//! FIXME: write doc

SbBool
SoGLLazyElement::isColorIndex(SoState * state)
{
  SoGLLazyElement * elem = getInstance(state);
  return elem->colorindex;
}

//! FIXME: write doc

void
SoGLLazyElement::send(const SoState * state, uint32_t mask) const
{
  if (this->colorpacker) {    
    if (!this->colorpacker->diffuseMatch(this->coinstate.diffusenodeid) ||
        !this->colorpacker->transpMatch(this->coinstate.transpnodeid)) {
      this->packColors(this->colorpacker);
    }
    ((SoGLLazyElement*)this)->packedpointer = this->colorpacker->getPackedColors();
  }
  else ((SoGLLazyElement*)this)->packedpointer = this->coinstate.packedarray; 
    
  assert(this->packedpointer);

  int stipplenum;
  
  for (int i = 0; (i < LAZYCASES_LAST)&&mask; i++, mask>>=1) {
    if (mask&1) {
      switch (i) {
      case LIGHT_MODEL_CASE:
        if (this->coinstate.lightmodel != this->glstate.lightmodel) {
          this->sendLightModel(this->coinstate.lightmodel);
        }
        break;
      case DIFFUSE_CASE:
        this->sendDiffuseByIndex(0);
        break;
      case AMBIENT_CASE:
        if (this->coinstate.ambient != this->glstate.ambient) {
          this->sendAmbient(this->coinstate.ambient);
        }
        break;
      case SPECULAR_CASE:
        if (this->coinstate.specular != this->glstate.specular) {
          this->sendSpecular(this->coinstate.specular);
        }
        break;
      case EMISSIVE_CASE:
        if (this->coinstate.emissive != this->glstate.emissive) {
          this->sendEmissive(this->coinstate.emissive);
        }
        break;
      case SHININESS_CASE:
        if (this->coinstate.shininess != this->glstate.shininess) {
          this->sendShininess(this->coinstate.shininess);
        }
        break;
      case BLENDING_CASE:
        if (this->coinstate.blending != this->glstate.blending) {
          this->sendBlending(this->coinstate.blending);
        }
      case TRANSPARENCY_CASE:
        stipplenum = 
          this->coinstate.transptype == SoGLRenderAction::SCREEN_DOOR ? 
          this->coinstate.stipplenum : 0;

        if (stipplenum != this->glstate.stipplenum) {
          this->sendTransparency(stipplenum);
        }
        break;
      case VERTEXORDERING_CASE:
        if (this->glstate.vertexordering != this->coinstate.vertexordering) {
          this->sendVertexOrdering(this->coinstate.vertexordering);
        }
        break;
      case CULLING_CASE:
        if (this->glstate.culling != this->coinstate.culling) {
          this->sendBackfaceCulling(this->coinstate.culling);
        }
        break;
      case TWOSIDE_CASE:
        if (this->glstate.twoside != this->coinstate.twoside) {
          this->sendTwosideLighting(this->coinstate.twoside);
        }
        break;
      case SHADE_MODEL_CASE:
        if (this->glstate.flatshading != this->coinstate.flatshading) {
          this->sendFlatshading(this->coinstate.flatshading);
        }
        break;
      case GLIMAGE_CASE:
        if (this->glstate.glimageid != (int32_t) this->coinstate.glimageid) {
          this->sendGLImage(this->coinstate.glimageid);
        }
        break;
      case ALPHATEST_CASE:
        if (this->glstate.alphatest != (int32_t) this->coinstate.alphatest) {
          this->sendAlphaTest(this->coinstate.alphatest);
        }
        break;
      }
      
    }
  }
}

//! FIXME: write doc

void
SoGLLazyElement::sendVPPacked(SoState* state, const unsigned char* pcolor)
{
  assert(0 && "Not implemented yet. Provided for API compatibility.");
}

/*!  
  Reset element GL state (set state to invalid). Use this method to
  notify this element when you use your own GL code that changes the
  OpenGL state.  
*/
void
SoGLLazyElement::reset(SoState * state,  uint32_t mask) const
{
  SoGLLazyElement * elem = getInstance(state);

  if (state->isCacheOpen()) {
    elem->cachebitmask |= mask;
  }

  for (int i = 0; (i < LAZYCASES_LAST)&&mask; i++, mask>>=1) {
    if (mask&1) {
      switch (i) {
      case LIGHT_MODEL_CASE:
        elem->glstate.lightmodel = -1;
        break;
      case DIFFUSE_CASE:
        elem->sendPackedDiffuse(0xccccccff);
        break;
      case AMBIENT_CASE:
        elem->glstate.ambient = SbColor(-1.f, -1.0f, -1.0f);
        break;
      case SPECULAR_CASE:
        elem->glstate.specular = SbColor(-1.0f, -1.0f, -1.0f);
        break;
      case EMISSIVE_CASE:
        elem->glstate.emissive = SbColor(-1.0f, -1.0f, -1.0f);
        break;
      case SHININESS_CASE:
        elem->glstate.shininess = -1.0f;
        break;
      case BLENDING_CASE:
        elem->glstate.blending = -1;
        break;
      case TRANSPARENCY_CASE:
        elem->glstate.stipplenum = -1;
        break;
      case VERTEXORDERING_CASE:
        elem->glstate.vertexordering = -1;
        break;
      case CULLING_CASE:
        elem->glstate.culling = -1;
        break;
      case TWOSIDE_CASE:
        elem->glstate.twoside = -1;
        break;
      case SHADE_MODEL_CASE:
        elem->glstate.flatshading = -1;
        break;
      case GLIMAGE_CASE:
        elem->glstate.glimageid = -1;
        break;
      case ALPHATEST_CASE:
        elem->glstate.alphatest = -1;
        break;
      }
    }
  }
}

void 
SoGLLazyElement::sendPackedDiffuse(SoState * state, const uint32_t diffuse)
{
  SoGLLazyElement * elem = getInstance(state);
  if (elem->glstate.diffuse != diffuse) {
    elem->sendPackedDiffuse(diffuse);
  }
}

void 
SoGLLazyElement::sendLightModel(SoState * state, const int32_t model)
{
  SoGLLazyElement * elem = getInstance(state);
  if (elem->glstate.lightmodel != model) {
    elem->sendLightModel(model);
  }
}

void 
SoGLLazyElement::sendFlatshading(SoState * state, SbBool onoff)
{
  SoGLLazyElement * elem = getInstance(state);
  if (elem->glstate.flatshading != onoff) {
    elem->sendFlatshading(onoff);
  }
}

void 
SoGLLazyElement::sendVertexOrdering(SoState * state, const VertexOrdering ordering)
{
  SoGLLazyElement * elem = getInstance(state);
  if (elem->glstate.vertexordering != (int32_t) ordering) {
    elem->sendVertexOrdering(ordering);
  }
}

void 
SoGLLazyElement::sendTwosideLighting(SoState * state, const SbBool onoff)
{
  SoGLLazyElement * elem = getInstance(state);
  if (elem->glstate.twoside != (int32_t) onoff) {
    elem->sendTwosideLighting(onoff);
  }
}

void 
SoGLLazyElement::sendBackfaceCulling(SoState * state, const SbBool onoff)
{
  SoGLLazyElement * elem = getInstance(state);
  if (elem->glstate.culling != (int32_t) onoff) {
    elem->sendBackfaceCulling(onoff);
  }
}

void 
SoGLLazyElement::setDiffuseElt(SoNode * node,  int32_t numcolors, 
                               const SbColor * colors, SoColorPacker * packer)
{
  inherited::setDiffuseElt(node, numcolors, colors, packer);
  this->colorpacker = packer;
}

void 
SoGLLazyElement::setPackedElt(SoNode * node, int32_t numcolors, 
                              const uint32_t * colors, const SbBool packedtransparency)
{
  inherited::setPackedElt(node, numcolors, colors, packedtransparency);
  this->colorpacker = NULL;
  this->packedpointer = colors;
}

void 
SoGLLazyElement::setColorIndexElt(SoNode * node, int32_t numindices, 
                                  const int32_t * indices)
{
  inherited::setColorIndexElt(node, numindices, indices);
}

void 
SoGLLazyElement::setTranspElt(SoNode * node, int32_t numtransp, 
                              const float * transp, SoColorPacker * packer)
{
  inherited::setTranspElt(node, numtransp, transp, packer);
  this->colorpacker = packer;
}


void 
SoGLLazyElement::setTranspTypeElt(int32_t type)
{ 
  inherited::setTranspTypeElt(type);
  this->transpmask = type == SoGLRenderAction::SCREEN_DOOR ? 0xff : 0x00;
}

void 
SoGLLazyElement::setAmbientElt(const SbColor* color)
{
  inherited::setAmbientElt(color);
}

void 
SoGLLazyElement::setEmissiveElt(const SbColor* color)
{ 
  inherited::setEmissiveElt(color);
}

void 
SoGLLazyElement::setSpecularElt(const SbColor* color)
{ 
  inherited::setSpecularElt(color);
}

void 
SoGLLazyElement::setShininessElt(float value)
{ 
  inherited::setShininessElt(value);
}

void 
SoGLLazyElement::setColorMaterialElt(SbBool value)
{ 
  inherited::setColorMaterialElt(value);
}

void 
SoGLLazyElement::setBlendingElt(SbBool value)
{ 
  inherited::setBlendingElt(value);
}

void 
SoGLLazyElement::setLightModelElt(SoState * state, int32_t model)
{ 
  inherited::setLightModelElt(state, model);
}

void 
SoGLLazyElement::setMaterialElt(SoNode * node, uint32_t bitmask, 
                                SoColorPacker * packer, 
                                const SbColor * diffuse, const int numdiffuse,
                                const float * transp, const int numtransp,
                                const SbColor & ambient,
                                const SbColor & emissive,
                                const SbColor & specular,
                                const float shininess,
                                const SbBool istransparent)
{
  inherited::setMaterialElt(node, bitmask,
                            packer, diffuse, numdiffuse,
                            transp, numtransp, ambient,
                            emissive, specular, shininess, istransparent);
  this->colorpacker = packer;
}

void 
SoGLLazyElement::setVertexOrderingElt(VertexOrdering ordering)
{
  inherited::setVertexOrderingElt(ordering);
}

void 
SoGLLazyElement::setBackfaceCullingElt(SbBool onoff)
{
  inherited::setBackfaceCullingElt(onoff);
}

void 
SoGLLazyElement::setTwosideLightingElt(SbBool onoff)
{
  inherited::setTwosideLightingElt(onoff);
}

void 
SoGLLazyElement::setShadeModelElt(SbBool flatshading)
{
  inherited::setShadeModelElt(flatshading);
}

void 
SoGLLazyElement::setGLImageIdElt(uint32_t glimageid, SbBool alphatest)
{
  inherited::setGLImageIdElt(glimageid, alphatest);
}

void
SoGLLazyElement::setAlphaTestElt(const SbBool onoff)
{
  inherited::setAlphaTestElt(onoff);
}

void 
SoGLLazyElement::packColors(SoColorPacker * packer) const
{
  const int n = this->coinstate.numdiffuse;
  const SbColor * diffuse = this->coinstate.diffusearray;
  const int numtransp = this->coinstate.numtransp;
  const float * transp = this->coinstate.transparray;

  if (packer->getSize() < n) packer->reallocate(n);
  uint32_t * ptr = packer->getPackedColors();

  int ti = 0;

  for (int i = 0; i < n; i++) {
    ptr[i] = diffuse[i].getPackedValue(transp[ti]);
    if (ti < numtransp) ti++;
  }

  packer->setNodeIds(this->coinstate.diffusenodeid,
                     this->coinstate.transpnodeid);
}

void 
SoGLLazyElement::beginCaching(SoState * state, GLState * prestate,
                              GLState * poststate)
{
  SoGLLazyElement * elem = getInstance(state);
  *prestate = elem->glstate; // copy current GL state
  elem->precachestate = prestate;
  elem->postcachestate = poststate;
  elem->precachestate->cachebitmask = 0;
  elem->postcachestate->cachebitmask = 0;
  elem->didsetbitmask = 0;
  elem->didntsetbitmask = 0;
  elem->cachebitmask = 0;
}

void 
SoGLLazyElement::endCaching(SoState * state)
{  
  SoGLLazyElement * elem = getInstance(state);
  *elem->postcachestate = elem->glstate;
  elem->postcachestate->cachebitmask = elem->cachebitmask;
  elem->precachestate->cachebitmask = elem->didntsetbitmask;
  elem->precachestate = NULL;
  elem->postcachestate = NULL;
}

void 
SoGLLazyElement::postCacheCall(SoState * state, GLState * poststate)
{
  SoGLLazyElement * elem = getInstance(state);
  uint32_t mask = poststate->cachebitmask;

  for (int i = 0; (i < LAZYCASES_LAST)&&mask; i++, mask>>=1) {
    if (mask&1) {
      switch (i) {
      case LIGHT_MODEL_CASE:
        elem->glstate.lightmodel = poststate->lightmodel;
        break;
      case DIFFUSE_CASE:
        elem->glstate.diffuse = poststate->diffuse;
        break;
      case AMBIENT_CASE:
        elem->glstate.ambient = poststate->ambient;
        break;
      case SPECULAR_CASE:
        elem->glstate.specular = poststate->specular;
        break;
      case EMISSIVE_CASE:
        elem->glstate.emissive = poststate->emissive;
        break;
      case SHININESS_CASE:
        elem->glstate.shininess = poststate->shininess;
        break;
      case BLENDING_CASE:
        elem->glstate.blending = poststate->blending;
        break;
      case TRANSPARENCY_CASE:
        elem->glstate.stipplenum = poststate->stipplenum;
        break;
      case VERTEXORDERING_CASE:
        elem->glstate.vertexordering = poststate->vertexordering;
        break;
      case CULLING_CASE:
        elem->glstate.culling = poststate->culling;
        break;
      case TWOSIDE_CASE:
        elem->glstate.twoside = poststate->twoside;
        break;
      case SHADE_MODEL_CASE:
        elem->glstate.flatshading = poststate->flatshading;
        break;
      case GLIMAGE_CASE:
        elem->glstate.glimageid = poststate->glimageid;
        break;
      case ALPHATEST_CASE:
        elem->glstate.alphatest = poststate->alphatest;
        break;
      }
    }
  }
}
  
SbBool
SoGLLazyElement::preCacheCall(SoState * state, GLState * prestate)
{
  SoGLLazyElement * elem = getInstance(state);
  const GLState & curr = elem->glstate;
  uint32_t mask = prestate->cachebitmask;

  for (int i = 0; (i < LAZYCASES_LAST)&&mask; i++, mask>>=1) {
    if (mask&1) {
      switch (i) {
      case LIGHT_MODEL_CASE:
        if (curr.lightmodel != prestate->lightmodel) return FALSE;
        break;
      case DIFFUSE_CASE:
        if (curr.diffuse != prestate->diffuse) return FALSE;
        break;
      case AMBIENT_CASE:
        if (curr.ambient != prestate->ambient) return FALSE;
        break;
      case SPECULAR_CASE:
        if (curr.specular != prestate->specular) return FALSE;
        break;
      case EMISSIVE_CASE:
        if (curr.emissive != prestate->emissive) return FALSE;
        break;
      case SHININESS_CASE:
        if (curr.shininess != prestate->shininess) return FALSE;
        break;
      case BLENDING_CASE:
        if (curr.blending != prestate->blending) return FALSE;
        break;
      case TRANSPARENCY_CASE:
        if (curr.stipplenum != prestate->stipplenum) return FALSE;
        break;
      case VERTEXORDERING_CASE:
        if (curr.vertexordering != prestate->vertexordering) return FALSE;
        break;
      case CULLING_CASE:
        if (curr.culling != prestate->culling) return FALSE;
        break;
      case TWOSIDE_CASE:
        if (curr.twoside != prestate->twoside) return FALSE;
        break;
      case SHADE_MODEL_CASE:
        if (curr.flatshading != prestate->flatshading) return FALSE;
        break;
      case GLIMAGE_CASE:
        if (curr.glimageid != prestate->glimageid) return FALSE;
        break;
      case ALPHATEST_CASE:
        if (curr.alphatest != prestate->alphatest) return FALSE;
        break;
      }
    }
  }
  return TRUE;
}


void 
SoGLLazyElement::lazyDidSet(uint32_t mask)
{
  this->didsetbitmask |= mask;
}

void 
SoGLLazyElement::lazyDidntSet(uint32_t mask)
{
  uint32_t pre = this->didntsetbitmask;
  this->didntsetbitmask |= mask&(~this->didsetbitmask);
}

