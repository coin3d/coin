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
#include <Inventor/misc/SoState.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/C/glue/gl.h>

#include <Inventor/bundles/SoMaterialBundle.h>
#include <coindefs.h> // COIN_OBSOLETED
#include <assert.h>
#include <Inventor/errors/SoDebugError.h>

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
}

inline void
SoGLLazyElement::sendLightModel(const int32_t model) const
{
  ((SoGLLazyElement*)this)->glstate.lightmodel = model;
  if (model == PHONG) glEnable(GL_LIGHTING);
  else glDisable(GL_LIGHTING);
}

void 
SoGLLazyElement::init(SoState * state)
{
  inherited::init(state);
  this->colorindex = FALSE;
  this->glstate.ambient.setValue(-1.0f, -1.0f, -1.0f);
  this->glstate.emissive.setValue(-1.0f, -1.0f, -1.0f); 
  this->glstate.specular.setValue(-1.0f, -1.0f, -1.0f);
  this->glstate.shininess = -1.0f;
  this->glstate.lightmodel = -1;
  this->glstate.blending = -1;
  this->glstate.stipplenum = 0;
  this->packedpointer = NULL;
  this->transpmask = 0xff;
  this->colorpacker = NULL;
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
  this->glstate = prev->glstate;
  this->colorindex = prev->colorindex;
  this->transpmask = prev->transpmask;
  this->colorpacker = prev->colorpacker;
}

void 
SoGLLazyElement::pop(SoState *state, const SoElement * prevtopelement)
{
  inherited::pop(state, prevtopelement);
  SoGLLazyElement * prev = (SoGLLazyElement*) prevtopelement;
  this->glstate = prev->glstate;
  this->colorindex = prev->colorindex;
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

inline void
send_gl_material(GLenum pname, const SbColor & color)
{
  GLfloat col[4];
  color.getValue(col[0], col[1], col[2]);
  col[3] = 1.0f;
  glMaterialfv(GL_FRONT_AND_BACK, pname, col);
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
  
  if (mask & LIGHT_MODEL_MASK) {
    int32_t model = this->coinstate.lightmodel;
    if (model != this->glstate.lightmodel) {
      this->sendLightModel(model);
    }
  }
  if (mask & DIFFUSE_MASK) {
    this->sendDiffuseByIndex(0);
  }
  if (mask & AMBIENT_MASK) {
    const SbColor & color = this->coinstate.ambient;
    if (color != this->glstate.ambient) {
      send_gl_material(GL_AMBIENT, color);
      ((SoGLLazyElement*)this)->glstate.ambient = color;
    }
  }
  if (mask & SPECULAR_MASK) {
    const SbColor & color = this->coinstate.specular;
    if (color != this->glstate.specular) {
      send_gl_material(GL_SPECULAR, color);
      ((SoGLLazyElement*)this)->glstate.specular = color;
    }
  }
  if (mask & EMISSIVE_MASK) {
    const SbColor & color = this->coinstate.emissive;
    if (color != this->glstate.emissive) {
      send_gl_material(GL_EMISSION, color);
      ((SoGLLazyElement*)this)->glstate.emissive = color;
    }
  }
  if (mask & SHININESS_MASK) {
    float shine = this->coinstate.shininess;
    if (shine != this->glstate.shininess) {
      glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shine*128.0f);
      ((SoGLLazyElement*)this)->glstate.shininess = shine;
    }
  }
  if (mask & BLENDING_MASK) {
    int blend = (int) this->coinstate.blending;
    if (blend != this->glstate.blending) {
      ((SoGLLazyElement*)this)->glstate.blending = blend;
      if (blend) glEnable(GL_BLEND);
      else glDisable(GL_BLEND);
    }
    // FIXME: move from SoGLRenderAction
  }
  if (mask & TRANSPARENCY_MASK) {
    int stipplenum = 
      this->coinstate.transptype == SoGLRenderAction::SCREEN_DOOR ? 
      this->coinstate.stipplenum : 0;

    if (stipplenum != this->glstate.stipplenum) {
      if (stipplenum == 0) {
        glDisable(GL_POLYGON_STIPPLE);
      }
      else {
        if (this->glstate.stipplenum == 0) glEnable(GL_POLYGON_STIPPLE);
        glPolygonStipple(stipple_patterns[stipplenum]);
        
      }
      ((SoGLLazyElement*)this)->glstate.stipplenum = stipplenum;
    }
  }
}

//! FIXME: write doc

void
SoGLLazyElement::sendVPPacked(SoState* state, const unsigned char* pcolor)
{
  assert(0 && "Not implemented yet. Provided for API compatibility.");
}

//! FIXME: write doc

void
SoGLLazyElement::reset(SoState * state,  uint32_t mask) const
{
  assert(0 && "Not implemented yet. Provided for API compatibility.");
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
SoGLLazyElement::sendPackedDiffuse(SoState * state, const uint32_t diffuse)
{
  SoGLLazyElement * elem = getInstance(state);
  if (elem->glstate.diffuse != diffuse) {
    elem->sendPackedDiffuse(diffuse);
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
                                const float shininess)
{
  inherited::setMaterialElt(node, bitmask,
                            packer, diffuse, numdiffuse,
                            transp, numtransp, ambient,
                            emissive, specular, shininess);
  this->colorpacker = packer;
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

