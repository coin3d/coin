/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoText2 SoText2.h Inventor/nodes/SoText2.h
  \brief The SoText2 class is a node type for visualizing 2D text aligned with the camera plane.
  \ingroup nodes

  SoText2 text is not scaled according to the distance from the
  camera, and is not influenced by rotation or scaling as 3D
  primitives are. If these are properties that you want the text to
  have, you should instead use an SoText3 or SoAsciiText node.

  Note that even though the size of the 2D text is not influenced by
  the distance from the camera, the text is still subject to the usual
  rules with regard to the depthbuffer, so it \e will be obscured by
  graphics laying in front of it.

  The text will be \e positioned according to the current transformation.
  The x origin of the text is the first pixel of the leftmost character
  of the text. The y origin of the text is the baseline of the first line
  of text (the baseline being the imaginary line on which all upper case
  characters are standing).

  One important issue about rendering performance: since the
  positioning and rendering of an SoText2 node depends on the current
  viewport and camera, having SoText2 nodes in the scene graph will
  lead to a cache dependency on the previously encountered
  SoCamera-node. This can have severe influence on the rendering
  performance, since if the camera is above the SoText2's nearest
  parent SoSeparator, the SoSeparator will not be able to cache the
  geometry under it.

  (Even worse rendering performance will be forced if the
  SoSeparator::renderCaching flag is explicitly set to \c ON, as the
  SoSeparator node will then continuously generate and destruct the
  same cache as the camera moves.)

  SoText2 nodes are therefore best positioned under their own
  SoSeparator node, outside areas in the scene graph that otherwise
  contains static geometry.

  Also note that SoText2 nodes cache the ids and positions of each glyph
  bitmap used to render \c string. This means that \c USE of a \c DEF'ed
  SoText2 node, with a different font, will be noticeably slower than using
  two separate SoText2 nodes, one for each font, since it will have to
  recalculate glyph bitmap ids and positions for each call to \c GLrender().

  SoText2 uses the SoGlyph class to generate glyph bitmaps.

  \sa SoFont, SoFontStyle, SoText3, SoAsciiText, SoGlyph
*/

#include <Inventor/nodes/SoText2.h>
#include <Inventor/nodes/SoSubNodeP.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#ifdef HAVE_GLX
#include <GL/glx.h>
#endif // HAVE_GLX

#include <Inventor/system/gl.h>
#include <Inventor/elements/SoViewVolumeElement.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/elements/SoViewportRegionElement.h>
#include <Inventor/elements/SoFontNameElement.h>
#include <Inventor/elements/SoFontSizeElement.h>
#include <Inventor/elements/SoLazyElement.h>
#include <Inventor/elements/SoCullElement.h>
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/bundles/SoMaterialBundle.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/details/SoTextDetail.h>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/SbPlane.h>
#include <Inventor/SbLine.h>
#include <Inventor/SbString.h>
#include <Inventor/SbBox2s.h>
#include <Inventor/misc/SoGlyph.h>

#include <limits.h>
#include <string.h>

static const unsigned int NOT_AVAILABLE = UINT_MAX;

/*!
  \enum SoText2::Justification

  Enum contains the various options for how the horizontal text layout
  text should be done. Valid values are LEFT, RIGHT and CENTER.
*/


/*!
  \var SoMFString SoText2::string

  The set of strings to render.  Each string in the multiple value
  field will be rendered on it's own line.

  The default value of the field is a single empty string.
*/
/*!
  \var SoSFFloat SoText2::spacing

  Spacing between each consecutive vertical line.  Default value is
  1.0, which means that the space between the uppermost line of each
  rendered string will equal the vertical size of the highest
  character in the bitmap alphabet.
*/
/*!
  \var SoSFEnum SoText2::justification

  Decides how the horizontal layout of the text strings is done.
*/

class SoText2P {
public:
  SoText2P(SoText2 * textnode) : textnode(textnode) {}

  SoText2 * textnode;
  
  SbBool useglyphcache;
  SoGlyph *** glyphs;
  SbVec2s ** positions;
  // FIXME: use notify() to handle changes, not comparison with last value.
  SbString ** laststring;
  int * stringwidth;
  SbBox2s bbox;
  int linecnt;
  int validarraydims;
  SbName prevfontname;
  float prevfontsize;
  SbBool hasbuiltglyphcache;
  
public:
  void getQuad(SoState * state, SbVec3f & v0, SbVec3f & v1,
               SbVec3f & v2, SbVec3f & v3);
  void flushGlyphCache(const SbBool unrefglyphs);
  int buildGlyphCache(SoState * state);
  SbBool shouldBuildGlyphCache(SoState * state);
  void dumpGlyphCache();
  void dumpBuffer(unsigned char * buffer, SbVec2s size, SbVec2s pos);
};

// *************************************************************************

#undef THIS
#define THIS this->pimpl

SO_NODE_SOURCE(SoText2);

/*!
  Constructor.
*/
SoText2::SoText2(void)
{
  THIS = new SoText2P(this);
  THIS->glyphs = NULL;
  THIS->positions = NULL;
  THIS->laststring = NULL;
  THIS->linecnt = 0;
  THIS->validarraydims = 0;
  THIS->stringwidth = NULL;
  THIS->bbox.makeEmpty();
  THIS->useglyphcache = TRUE;
  THIS->prevfontname = SbName("");
  THIS->prevfontsize = 0.0;
  THIS->hasbuiltglyphcache = FALSE;
  
  SO_NODE_INTERNAL_CONSTRUCTOR(SoText2);

  SO_NODE_ADD_FIELD(string, (""));
  SO_NODE_ADD_FIELD(spacing, (1.0f));
  SO_NODE_ADD_FIELD(justification, (SoText2::LEFT));

  SO_NODE_DEFINE_ENUM_VALUE(Justification, LEFT);
  SO_NODE_DEFINE_ENUM_VALUE(Justification, RIGHT);
  SO_NODE_DEFINE_ENUM_VALUE(Justification, CENTER);
  SO_NODE_SET_SF_ENUM_TYPE(justification, Justification);
}

/*!
  Destructor.
*/
SoText2::~SoText2()
{
  THIS->flushGlyphCache(TRUE);
  delete THIS;
}

// doc in super
void
SoText2::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoText2, SO_FROM_INVENTOR_2_1);
}


// **************************************************************************

// doc in super
void
SoText2::GLRender(SoGLRenderAction * action)
{
  if (!this->shouldGLRender(action)) return;

  SoState * state = action->getState();

  state->push();
  SoLazyElement::setLightModel(state, SoLazyElement::BASE_COLOR);
  
  // Render using SoGlyphs
  if (THIS->buildGlyphCache(state) == 0) {
    // Render only if bbox not outside cull planes.
    SbBox3f box;
    SbVec3f center;
    this->computeBBox(action, box, center);
    if (!SoCullElement::cullTest(state, box, TRUE)) {
      SoMaterialBundle mb(action);
      mb.sendFirst();
      SbVec3f nilpoint(0.0f, 0.0f, 0.0f);
      const SbMatrix & mat = SoModelMatrixElement::get(state);
      mat.multVecMatrix(nilpoint, nilpoint);
      const SbViewVolume & vv = SoViewVolumeElement::get(state);
      // this function will also modify the z-value of nilpoint
      // according to the view matrix
      vv.projectToScreen(nilpoint, nilpoint);
      // change z-range from [0,1] to [-1,1]
      nilpoint[2] *= 2.0f;
      nilpoint[2] -= 1.0f;

      const SbViewportRegion & vp = SoViewportRegionElement::get(state);
      SbVec2s vpsize = vp.getViewportSizePixels();
      nilpoint[0] = nilpoint[0] * float(vpsize[0]);
      nilpoint[1] = nilpoint[1] * float(vpsize[1]);
      // Set new state.
      glMatrixMode(GL_MODELVIEW);
      glPushMatrix();
      glLoadIdentity();
      glMatrixMode(GL_PROJECTION);
      glPushMatrix();
      glLoadIdentity();
      glOrtho(0, vpsize[0], 0, vpsize[1], -1.0f, 1.0f);
      glPixelStorei(GL_UNPACK_ALIGNMENT,1);

      float xpos = nilpoint[0];      // to get rid of compiler warning..
      float ypos = nilpoint[1];
      float fx, fy, rasterx, rastery, rpx, rpy, offsetx, offsety;
      int ix, iy, charcnt, offvp;
      SbVec2s thispos;
      SbVec2s position;
      SbVec2s thissize;
      unsigned char * buffer;
      for (int i = 0; i < THIS->linecnt; i++) {
        switch (this->justification.getValue()) {
        case SoText2::LEFT:
          xpos = nilpoint[0];
          break;
        case SoText2::RIGHT:
          xpos = nilpoint[0] - THIS->stringwidth[i];
          break;
        case SoText2::CENTER:
          xpos = nilpoint[0] - THIS->stringwidth[i]/2.0f;
          break;
        }
        charcnt = THIS->laststring[i]->getLength();
        for (int i2 = 0; i2 < charcnt; i2++) {
          buffer = THIS->glyphs[i][i2]->getBitmap(thissize, thispos, FALSE);
          ix = thissize[0];
          iy = thissize[1];
          position = THIS->positions[i][i2];
          fx = (float)position[0];
          fy = (float)position[1];
          
          rasterx = xpos + fx;
          rpx = rasterx >= 0 ? rasterx : 0;
          offvp = rasterx < 0 ? 1 : 0;
          offsetx = rasterx >= 0 ? 0 : rasterx;
          
          rastery = ypos + fy;
          rpy = rastery >= 0 ? rastery : 0;
          offvp = offvp || rastery < 0 ? 1 : 0;
          offsety = rastery >= 0 ? 0 : rastery;
          
          glRasterPos3f(rpx, rpy, -nilpoint[2]);
          if (offvp)
            glBitmap(0,0,0,0,offsetx,offsety,NULL);
          glBitmap(ix,iy,0,0,0,0,(const GLubyte *)buffer);
        }
      }
      
      glPixelStorei(GL_UNPACK_ALIGNMENT,4);
      // Pop old GL state.
      glMatrixMode(GL_PROJECTION);
      glPopMatrix();
      glMatrixMode(GL_MODELVIEW);
      glPopMatrix();
    }
  }
  
  state->pop();

  // don't auto cache SoText2 nodes.
  SoGLCacheContextElement::shouldAutoCache(action->getState(),
                                           SoGLCacheContextElement::DONT_AUTO_CACHE);
}

// **************************************************************************


// doc in super
void
SoText2::computeBBox(SoAction * action, SbBox3f & box, SbVec3f & center)
{
  SbVec3f v0, v1, v2, v3;
  // this will cause a cache dependency on the view volume,
  // model matrix and viewport.
  THIS->getQuad(action->getState(), v0, v1, v2, v3);

  box.makeEmpty();
  box.extendBy(v0);
  box.extendBy(v1);
  box.extendBy(v2);
  box.extendBy(v3);
  center = box.getCenter();
}

// doc in super
void
SoText2::rayPick(SoRayPickAction * action)
{
  if (!this->shouldRayPick(action)) return;
  THIS->buildGlyphCache(action->getState());
  action->setObjectSpace();
  SbVec3f v0, v1, v2, v3;
  THIS->getQuad(action->getState(), v0, v1, v2, v3);
  if (v0 == v1 || v0 == v3) return; // empty

  SbVec3f isect;
  SbVec3f bary;
  SbBool front;
  SbBool hit = action->intersect(v0, v1, v2, isect, bary, front);
  if (!hit) hit = action->intersect(v0, v2, v3, isect, bary, front);

  if (hit && action->isBetweenPlanes(isect)) {
    // find normalized 2D hitpoint on quad
    float h = (v3-v0).length();
    float w = (v1-v0).length();
    SbLine horiz(v2,v3);
    SbVec3f ptonline = horiz.getClosestPoint(isect);
    float vdist = (ptonline-isect).length();
    vdist /= h;

    SbLine vert(v0,v3);
    ptonline = vert.getClosestPoint(isect);
    float hdist = (ptonline-isect).length();
    hdist /= w;

    // find which string was hit
    float fonth =  1.0f / float(this->string.getNum());
    int stringidx = SbClamp(int(vdist/fonth), 0, this->string.getNum()-1);

    int maxlen = 0;
    for (int i = 0; i < this->string.getNum(); i++) {
      int len = this->string[i].getLength();
      if (len > maxlen) maxlen = len;
    }

    // find the character
    int charidx = -1;
    int strlength = this->string[stringidx].getLength();
    short minx, miny, maxx, maxy;
    THIS->bbox.getBounds(minx, miny, maxx, maxy);
    float bbleft = minx;
    float bbwidth = (float)(maxx - minx);
    float strleft = (bbwidth - THIS->stringwidth[stringidx]) / bbwidth;
    float strright = 1.0;
    switch (this->justification.getValue()) {
    case LEFT:
      strleft = 0.0;
      strright = THIS->stringwidth[stringidx] / bbwidth;
      break;
    case RIGHT:
      break;
    case CENTER:
      strleft /= 2.0;
      strright = (float)1.0 - strleft;
      break;
    default:
      assert(0 && "SoText2::rayPick: unknown justification");
    }
    
    float charleft, charright;
    for (i=0; i<strlength; i++) {
      charleft = strleft + (THIS->positions[stringidx][i][0] / bbwidth);
      charright = (i==strlength-1 ? strright : strleft + (THIS->positions[stringidx][i+1][0] / bbwidth));
      if (hdist >= charleft && hdist <= charright) {
        charidx = i;
        i = strlength;
      }
    }
    
    if (charidx >= 0 && charidx < strlength) { // we have a hit!
      SoPickedPoint * pp = action->addIntersection(isect);
      if (pp) {
        SoTextDetail * detail = new SoTextDetail;
        detail->setStringIndex(stringidx);
        detail->setCharacterIndex(charidx);
        pp->setDetail(detail, this);
        pp->setMaterialIndex(0);
        pp->setObjectNormal(SbVec3f(0.0f, 0.0f, 1.0f));
      }
    }
  }
}

// doc in super
void
SoText2::getPrimitiveCount(SoGetPrimitiveCountAction *action)
{
  if (!this->shouldPrimitiveCount(action)) return;

  action->addNumText(this->string.getNum());
}

// doc in super
void
SoText2::generatePrimitives(SoAction * action)
{
  // This is supposed to be empty. There are no primitives.
}


// SoText2P methods below
#undef THIS

void
SoText2P::flushGlyphCache(const SbBool unrefglyphs)
{
  if (this->glyphs && validarraydims > 0) {
    free(this->stringwidth);
    for (int i=0; i<this->linecnt; i++) {
      if (validarraydims == 2) {
        if (this->laststring[i] && unrefglyphs) {
          for (int j=0; j<this->laststring[i]->getLength(); j++) {
            if (this->glyphs[i][j])
              this->glyphs[i][j]->unref();
          }
        }
        free(this->positions[i]);
      }
      delete this->laststring[i];
      free(this->glyphs[i]);
    }
    free(this->positions);
    free(this->glyphs);
  }
  this->glyphs = NULL;
  this->positions = NULL;
  this->laststring = NULL;
  this->linecnt = 0;
  this->validarraydims = 0;
  this->stringwidth = NULL;
  this->bbox.makeEmpty();
}

// Debug convenience method.
void
SoText2P::dumpGlyphCache()
{
  // FIXME: pure debug method, remove. preng 2003-03-18.
  fprintf(stderr,"dumpGlyphCache: validarraydims=%d\n", validarraydims);
  if (this->glyphs && validarraydims > 0) {
    for (int i=0; i<this->linecnt; i++) {
      fprintf(stderr,"  stringwidth[%d]=%d\n", i, this->stringwidth[i]);
      fprintf(stderr,"  laststring[%d]=%s\n", i, this->laststring[i]->getString());
      if (validarraydims == 2) {
        for (int j = 0; j < (int) strlen(this->laststring[i]->getString()); j++) {
          fprintf(stderr,"    glyph[%d][%d]=%p\n", i, j, this->glyphs[i][j]);
          fprintf(stderr,"    position[%d][%d]=(%d, %d)\n", i, j, this->positions[i][j][0], this->positions[i][j][1]);
        }
      }
    }
  }
}

// Calculates a quad around the text in 3D.
void
SoText2P::getQuad(SoState * state, SbVec3f & v0, SbVec3f & v1,
                  SbVec3f & v2, SbVec3f & v3)
{
  this->buildGlyphCache(state);

  SbVec3f nilpoint(0.0f, 0.0f, 0.0f);
  const SbMatrix & mat = SoModelMatrixElement::get(state);
  mat.multVecMatrix(nilpoint, nilpoint);

  const SbViewVolume &vv = SoViewVolumeElement::get(state);

  SbVec3f screenpoint;
  vv.projectToScreen(nilpoint, screenpoint);
  
  const SbViewportRegion & vp = SoViewportRegionElement::get(state);
  SbVec2s vpsize = vp.getViewportSizePixels();
  
  SbVec2f n0, n1, n2, n3, center;
  short xmin, ymin, xmax, ymax;
  float minx, miny, maxx, maxy;
  this->bbox.getBounds(xmin, ymin, xmax, ymax);
  center = SbVec2f( (float)(xmin+xmax)/(float)2.0, (float)(ymin+ymax)/(float)2.0);
  minx = (xmin - center[0]) / vpsize[0];
  miny = (ymin - center[1]) / vpsize[1];
  maxx = (xmax - center[0]) / vpsize[0];
  maxy = (ymax - center[1]) / vpsize[1];
  n0 = SbVec2f(screenpoint[0] + minx, screenpoint[1] + miny);
  n1 = SbVec2f(screenpoint[0] + maxx, screenpoint[1] + miny);
  n2 = SbVec2f(screenpoint[0] + maxx, screenpoint[1] + maxy);
  n3 = SbVec2f(screenpoint[0] + minx, screenpoint[1] + maxy);
  
  float halfw = (maxx - minx) / (float)2.0;
  switch (this->textnode->justification.getValue()) {
  case SoText2::LEFT:
    n0[0] += halfw;
    n1[0] += halfw;
    n2[0] += halfw;
    n3[0] += halfw;
    break;
  case SoText2::RIGHT:
    n0[0] -= halfw;
    n1[0] -= halfw;
    n2[0] -= halfw;
    n3[0] -= halfw;
    break;
  case SoText2::CENTER:
    break;
  default:
    assert(0 && "unknown alignment");
    break;
  }
  
  // get distance from nilpoint to camera plane
  float dist = -vv.getPlane(0.0f).getDistance(nilpoint);

  // find the four image points in the plane
  v0 = vv.getPlanePoint(dist, n0);
  v1 = vv.getPlanePoint(dist, n1);
  v2 = vv.getPlanePoint(dist, n2);
  v3 = vv.getPlanePoint(dist, n3);

  // transform back to object space
  SbMatrix inv = mat.inverse();
  inv.multVecMatrix(v0, v0);
  inv.multVecMatrix(v1, v1);
  inv.multVecMatrix(v2, v2);
  inv.multVecMatrix(v3, v3);
}

// Debug convenience method.
void
SoText2P::dumpBuffer(unsigned char * buffer, SbVec2s size, SbVec2s pos)
{
  // FIXME: pure debug method, remove. preng 2003-03-18.
  if (!buffer) {
    fprintf(stderr,"bitmap error: buffer pointer NULL.\n");
  } else {
    int rows = size[1];
    int bytes = size[0] >> 3;
    fprintf(stderr,"bitmap dump %d * %d bytes at %d, %d:\n", rows, bytes, pos[0], pos[1]);
    for (int y=rows-1; y>=0; y--) {
      for (int byte=0; byte<bytes; byte++) {
        for (int bit=0; bit<8; bit++)
          fprintf(stderr,"%d", buffer[y*bytes + byte] & 0x80>>bit ? 1 : 0);
      }
      fprintf(stderr,"\n");
    }
  }
}

SbBool
SoText2P::shouldBuildGlyphCache(SoState * state)
{
  if (!this->hasbuiltglyphcache)
    return TRUE;
  if (!this->useglyphcache)
    return FALSE;
  SbName curfontname = SoFontNameElement::get(state);
  float curfontsize = SoFontSizeElement::get(state);
  SbBool fonthaschanged = (this->prevfontname != curfontname 
                           || this->prevfontsize != curfontsize);
  if (fonthaschanged)
    return fonthaschanged;
  // FIXME: Use notify() mechanism to detect field changes. For Coin3. preng, 2003-03-10.
  if (this->linecnt != this->textnode->string.getNum())
    return TRUE;
  for (int i=0; i<this->linecnt; i++) {
    if (strcmp(laststring[i]->getString(), this->textnode->string[i].getString()) != 0)
      return TRUE;
  }
  return FALSE;
}

int
SoText2P::buildGlyphCache(SoState * state)
{
  if (this->shouldBuildGlyphCache(state)) {
    SoText2 * t = this->textnode;
    const char * s;
    int len;
    SbVec2s penpos, advance, kerning, thissize, thispos;
    unsigned int idx;
    SbName curfontname;
    float curfontsize;
    
    curfontname = SoFontNameElement::get(state);
    curfontsize = SoFontSizeElement::get(state);
    this->prevfontname = curfontname;
    this->prevfontsize = curfontsize;
    this->flushGlyphCache(FALSE);
    this->hasbuiltglyphcache = TRUE;
    this->linecnt = t->string.getNum();
    this->validarraydims = 0;
    this->glyphs = (SoGlyph ***)malloc(this->linecnt*sizeof(SoGlyph*));
    this->positions = (SbVec2s **)malloc(this->linecnt*sizeof(SbVec2s*));
    this->laststring = (SbString **)malloc(this->linecnt*sizeof(SbString*));
    this->stringwidth = (int *)malloc(this->linecnt*sizeof(int));
    if (!this->glyphs || !this->positions || !this->laststring || !this->stringwidth) {
      flushGlyphCache(FALSE);
      return -1;
    }
    // Avoid confusing flushGlyphCache with halfway init'l'ed arrays
    memset(this->glyphs, 0, this->linecnt*sizeof(SoGlyph*));
    memset(this->positions, 0, this->linecnt*sizeof(SbVec2s*));
    memset(this->laststring, 0, this->linecnt*sizeof(SbString*));
    memset(this->stringwidth, 0, this->linecnt*sizeof(int));
    this->validarraydims = 1;
    penpos[0] = 0;
    penpos[1] = 0;
    advance = penpos;
    kerning = penpos;
    for (int i=0; i<this->linecnt; i++) {
      s = t->string[i].getString();
      if ((len = strlen(s)) > 0) {
        this->glyphs[i] = (SoGlyph **)malloc(len*sizeof(SoGlyph*));
        this->positions[i] = (SbVec2s *)malloc(len*sizeof(SbVec2s));
        if (!this->glyphs[i] || !this->positions[i]) {
          flushGlyphCache(FALSE);
          return -1;
        }
        // Avoid confusing flushGlyphCache with halfway init'l'ed arrays
        memset(this->glyphs[i], 0, len*sizeof(SoGlyph*));
        memset(this->positions[i], 0, len*sizeof(SbVec2s));
        this->validarraydims = 2;
        this->laststring[i] = new SbString(s);
        for (int j=0; j<len; j++) {
          idx = (unsigned char)s[j];
          this->glyphs[i][j] = (SoGlyph *)(SoGlyph::getGlyph(state, idx, SbVec2s(0,0), 0.0));
          if (!this->glyphs[i][j]) {
            this->flushGlyphCache(FALSE);
            this->useglyphcache = FALSE;
#if COIN_DEBUG
            SoDebugError::postWarning("SoText2::buildGlyphCache", "unable to build glyph cache for '%s'", s);
#endif
            return -1;
          }
          this->glyphs[i][j]->getBitmap(thissize, thispos, FALSE);
          if (j > 0) {
            kerning = this->glyphs[i][j-1]->getKerning((const SoGlyph &)*this->glyphs[i][j]);
            advance = this->glyphs[i][j-1]->getAdvance();
          } else {
            kerning = SbVec2s(0,0);
            advance = SbVec2s(0,0);
          }
          penpos = penpos + advance + kerning;
          this->positions[i][j] = penpos + thispos + SbVec2s(0, -thissize[1]);
          this->bbox.extendBy(this->positions[i][j]);
          this->bbox.extendBy(this->positions[i][j] + SbVec2s(thissize[0], -thissize[1]));
        }
        this->stringwidth[i] = this->positions[i][len-1][0] + thissize[0];
        penpos = SbVec2s(0, penpos[1] - (short)(this->prevfontsize * t->spacing.getValue()));
      }
    }
  }
  return 0;
}
