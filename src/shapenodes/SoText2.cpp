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

#include <limits.h>
#include <string.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/nodes/SoText2.h>

#include <Inventor/SbBox2s.h>
#include <Inventor/SbLine.h>
#include <Inventor/SbPlane.h>
#include <Inventor/SbString.h>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/bundles/SoMaterialBundle.h>
#include <Inventor/details/SoTextDetail.h>
#include <Inventor/elements/SoCullElement.h>
#include <Inventor/elements/SoFontNameElement.h>
#include <Inventor/elements/SoFontSizeElement.h>
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/elements/SoLazyElement.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/elements/SoViewVolumeElement.h>
#include <Inventor/elements/SoViewportRegionElement.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/nodes/SoSubNodeP.h>

// The "lean and mean" define is a workaround for a Cygwin bug: when
// windows.h is included _after_ one of the X11 or GLX headers above
// (as it is indirectly from Inventor/system/gl.h), compilation of
// winspool.h (included from windows.h) will bail out with an error
// message due to the use of "Status" as a struct member ("Status" is
// probably #defined somewhere in the X11 or GLX header files).
//
// The WIN32_LEAN_AND_MEAN causes windows.h to not include winspool.h.
//
//        -mortene
#define WIN32_LEAN_AND_MEAN
#include <Inventor/system/gl.h>
// UPDATE, FIXME: due to some reorganization of header files GL/glx.h
// should not be included anywhere for this source code file any
// more. This means the hack above should no longer be necessary. To
// test, try building this file with g++ on a Cygwin system where both
// windows.h and GL/glx.h are available. If that works fine, remove
// the "#define WIN32_LEAN_AND_MEAN" hack. 20030625 mortene.


#include "../fonts/glyph2d.h"

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
  SoText2P(SoText2 * textnode) : master(textnode)
  {
    this->bbox.makeEmpty();
    this->prevfontname = SbName("");
    this->prevfontsize = 0.0;
  }

  void getQuad(SoState * state, SbVec3f & v0, SbVec3f & v1,
               SbVec3f & v2, SbVec3f & v3);
  void flushGlyphCache(const SbBool unrefglyphs);
  void buildGlyphCache(SoState * state);
  SbBool shouldBuildGlyphCache(SoState * state);
  void dumpBuffer(unsigned char * buffer, SbVec2s size, SbVec2s pos);

  cc_font_specification * fontspec;

  SbList< SbString > laststring;
  SbList<int> stringwidth;
  SbList< SbList<SbVec2s> > positions;
  SbBox2s bbox;
  SbName prevfontname;
  float prevfontsize;
  int numberoflines;

private:
  SoText2 * master;
};

#undef PRIVATE
#undef PUBLIC
#define PRIVATE(p) (p->pimpl)
#define PUBLIC(p) (p->master)

// *************************************************************************

SO_NODE_SOURCE(SoText2);

/*!
  Constructor.
*/
SoText2::SoText2(void)
{
  PRIVATE(this) = new SoText2P(this);

  SO_NODE_INTERNAL_CONSTRUCTOR(SoText2);

  SO_NODE_ADD_FIELD(string, (""));
  SO_NODE_ADD_FIELD(spacing, (1.0f));
  SO_NODE_ADD_FIELD(justification, (SoText2::LEFT));

  SO_NODE_DEFINE_ENUM_VALUE(Justification, LEFT);
  SO_NODE_DEFINE_ENUM_VALUE(Justification, RIGHT);
  SO_NODE_DEFINE_ENUM_VALUE(Justification, CENTER);
  SO_NODE_SET_SF_ENUM_TYPE(justification, Justification);

  PRIVATE(this)->fontspec = NULL;
  PRIVATE(this)->numberoflines = -1;

}

/*!
  Destructor.
*/
SoText2::~SoText2()
{
  PRIVATE(this)->flushGlyphCache(TRUE);
  delete PRIVATE(this);
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
  PRIVATE(this)->buildGlyphCache(state);

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

    float spacesize = SoFontSizeElement::get(state) / 3;  
    float xpos = nilpoint[0];      // to get rid of compiler warning..
    float ypos = nilpoint[1];
    float fx, fy, rasterx, rastery, rpx, rpy, offsetx, offsety;
    int ix=0, iy=0;
    int charcnt, offvp;
    int thispos[2];
    int thissize[2];
    unsigned char * buffer = NULL;
    const cc_glyph2d * prevglyph = NULL;
    
    const int nrlines = PRIVATE(this)->laststring.getLength();

    for (int i = 0; i < nrlines; i++) {
      switch (this->justification.getValue()) {
      case SoText2::LEFT:
        xpos = nilpoint[0];
        break;
      case SoText2::RIGHT:
        xpos = nilpoint[0] - PRIVATE(this)->stringwidth[i];
        break;
      case SoText2::CENTER:
        xpos = nilpoint[0] - PRIVATE(this)->stringwidth[i]/2.0f;
        break;
      }
      charcnt = PRIVATE(this)->laststring[i].getLength();

      for (int i2 = 0; i2 < charcnt; i2++) {

        const cc_glyph2d * glyph = cc_glyph2d_getglyph((int) PRIVATE(this)->laststring[i][i2], 
                                                       PRIVATE(this)->fontspec, 0.0f);

        buffer = cc_glyph2d_getbitmap(glyph, thissize, thispos);

        ix = thissize[0];
        iy = thissize[1];

        int charwidth = (int) cc_glyph2d_getwidth(glyph);
      
        rasterx = xpos + thispos[0]; 
        rpx = rasterx >= 0 ? rasterx : 0;
        offvp = rasterx < 0 ? 1 : 0;
        offsetx = rasterx >= 0 ? 0 : rasterx;
          
        rastery = ypos + (thispos[1] - thissize[1]);
        rpy = rastery>= 0 ? rastery : 0;
        offvp = offvp || rastery < 0 ? 1 : 0;
        offsety = rastery >= 0 ? 0 : rastery;

        int advancex, advancey;
        cc_glyph2d_getadvance(glyph, &advancex, &advancey);

        int kerningx, kerningy;
        if (i2 > 0) {
          cc_glyph2d_getkerning(prevglyph, glyph, &kerningx, &kerningy);
        } else {
          kerningx = 0;
          kerningy = 0;          
        }

        glRasterPos3f(rpx + advancex + kerningx, 
                      rpy + advancey + kerningy,
                      -nilpoint[2]);

        if (offvp) { glBitmap(0,0,0,0,offsetx,offsety,NULL); }
        if (buffer) { glBitmap(ix,iy,0,0,0,0,(const GLubyte *)buffer); }

        if (charwidth != 0) xpos += charwidth + 1;  // +1 so that TTFont letters dont get too close to each other...
        else xpos += (int) spacesize;
        
        prevglyph = glyph;

      }

      ypos -= (((int) SoFontSizeElement::get(state)) * this->spacing.getValue());

    }
      
    glPixelStorei(GL_UNPACK_ALIGNMENT,4);
    // Pop old GL state.
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
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
  PRIVATE(this)->getQuad(action->getState(), v0, v1, v2, v3);

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
  PRIVATE(this)->buildGlyphCache(action->getState());
  action->setObjectSpace();
  SbVec3f v0, v1, v2, v3;
  PRIVATE(this)->getQuad(action->getState(), v0, v1, v2, v3);
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
    const int numstr = this->string.getNum();
    float fonth =  1.0f / float(numstr);
    int stringidx = (numstr - SbClamp(int(vdist/fonth), 0, numstr-1)) - 1;

    int maxlen = 0;
    int i;
    for (i = 0; i < numstr; i++) {
      int len = this->string[i].getLength();
      if (len > maxlen) maxlen = len;
    }

    // find the character
    int charidx = -1;
    int strlength = this->string[stringidx].getLength();
    short minx, miny, maxx, maxy;
    PRIVATE(this)->bbox.getBounds(minx, miny, maxx, maxy);
    float bbleft = minx;
    float bbwidth = (float)(maxx - minx);
    float strleft = (bbwidth - PRIVATE(this)->stringwidth[stringidx]) / bbwidth;
    float strright = 1.0;
    switch (this->justification.getValue()) {
    case LEFT:
      strleft = 0.0;
      strright = PRIVATE(this)->stringwidth[stringidx] / bbwidth;
      break;
    case RIGHT:
      break;
    case CENTER:
      strleft /= 2.0;
      strright = 1.0f - strleft;
      break;
    default:
      assert(0 && "SoText2::rayPick: unknown justification");
    }

    float charleft, charright;
    for (i=0; i<strlength; i++) {
      charleft = strleft + PRIVATE(this)->positions[stringidx][i][0] / bbwidth;
      charright = (i==strlength-1 ? strright : strleft + (PRIVATE(this)->positions[stringidx][i+1][0] / bbwidth));
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
  if (!this->shouldPrimitiveCount(action)) 
    return;

  action->addNumText(this->string.getNum());
}

// doc in super
void
SoText2::generatePrimitives(SoAction * action)
{
  // This is supposed to be empty. There are no primitives.
}


// SoText2P methods below

void
SoText2P::flushGlyphCache(const SbBool unrefglyphs)
{
  this->stringwidth.truncate(0);
  this->positions.truncate(0);
  this->laststring.truncate(0);
  this->bbox.makeEmpty();
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
  this->bbox.getBounds(xmin, ymin, xmax, ymax);
  SbVec2s sp((short) (screenpoint[0] * vpsize[0]), (short)(screenpoint[1] * vpsize[1]));
  
  n0 = SbVec2f(float(sp[0] + xmin)/float(vpsize[0]), 
               float(sp[1] + ymax)/float(vpsize[1]));
  n1 = SbVec2f(float(sp[0] + xmax)/float(vpsize[0]), 
               float(sp[1] + ymax)/float(vpsize[1]));
  n2 = SbVec2f(float(sp[0] + xmax)/float(vpsize[0]), 
               float(sp[1] + ymin)/float(vpsize[1]));
  n3 = SbVec2f(float(sp[0] + xmin)/float(vpsize[0]), 
               float(sp[1] + ymin)/float(vpsize[1]));
  
  float w = n1[0]-n0[0];
  float halfw = w*0.5f;
  switch (PUBLIC(this)->justification.getValue()) {
  case SoText2::LEFT:
    break;
  case SoText2::RIGHT:
    n0[0] -= w;
    n1[0] -= w;
    n2[0] -= w;
    n3[0] -= w;
    break;
  case SoText2::CENTER:
    n0[0] -= halfw;
    n1[0] -= halfw;
    n2[0] -= halfw;
    n3[0] -= halfw;
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

// FIXME: Use notify() mechanism to detect field changes. For
// Coin3. preng, 2003-03-10.
//
// UPDATE 20030408 mortene: that wouldn't be sufficient, as
// e.g. changes to SoFont and SoFontStyle nodes in the scene graph can
// also have an influence on which glyphs to render.
//
// The best solution would be to create a new cache; SoGlyphCache or
// something. This cache would automatically store SoFont and
// SoFontStyle elements and be marked as invalid when they change
// (that's what caches are for). pederb, 2003-04-08

SbBool
SoText2P::shouldBuildGlyphCache(SoState * state)
{

  const SbName curfontname = SoFontNameElement::get(state);
  const float curfontsize = SoFontSizeElement::get(state);

  if (this->prevfontname != curfontname ||
      this->prevfontsize != curfontsize)  
    return TRUE; 
  
  if (this->numberoflines != PUBLIC(this)->string.getNum())  
    return TRUE; 

  for (int i=0; i < this->numberoflines; i++) {
    if (this->laststring[i] != PUBLIC(this)->string[i]) 
      return TRUE;
  }

  return FALSE;

}

void
SoText2P::buildGlyphCache(SoState * state)
{
  if (!this->shouldBuildGlyphCache(state)) { return; }

  this->prevfontname = SoFontNameElement::get(state);
  this->prevfontsize = SoFontSizeElement::get(state);
  this->flushGlyphCache(FALSE);
  const int nrlines = PUBLIC(this)->string.getNum();
  this->numberoflines = nrlines;

  // Build up font-spesification struct
  if (fontspec != NULL)
    delete fontspec; 
  this->fontspec = new cc_font_specification;
  this->fontspec->name = cc_string_construct_new();
  cc_string_set_text(this->fontspec->name, SoFontNameElement::get(state).getString());   
  this->fontspec->family = NULL;
  this->fontspec->style = NULL;
  this->fontspec->size = SoFontSizeElement::get(state);
 
  SbVec2s penpos(0, 0);


  for (int i=0; i < nrlines; i++) {

    const int strlength = PUBLIC(this)->string[i].getLength();
    this->laststring.append(SbString(PUBLIC(this)->string[i]));
    this->positions.append(SbList<SbVec2s>());

    int glyphwidth = 0;
    int actuallength = 0;
    int kerningx = 0;
    int kerningy = 0;
    int advancex = 0;
    int advancey = 0;
    int bitmapsize[2];
    int bitmappos[2];
    const cc_glyph2d * prevglyph= NULL;

    int j;
    // fetch all glyphs first
    for (j = 0; j < strlength; j++) {
      // Note that the "unsigned char" cast is needed to avoid 8-bit
      // chars using the highest bit (i.e. characters above the ASCII
      // set up to 127) be expanded to huge int numbers that turn
      // negative when casted to integer size.
      const unsigned int idx = (unsigned char)this->laststring[i][j];

      cc_glyph2d * glyph = cc_glyph2d_getglyph(idx, this->fontspec, 0.0f);

      // Should _always_ be able to get hold of a glyph -- if no
      // glyph is available for a specific character, a default
      // empty rectangle should be used.  -mortene.
      assert(glyph);
      
      glyphwidth = (int) cc_glyph2d_getwidth(glyph);
      if (glyphwidth == 0) // SPACE width is always returned 0, set to standardwidth/3.
        glyphwidth = (int) SoFontSizeElement::get(state) / 3; 
      actuallength += (glyphwidth+1);
        
      // Must fetch special modifiers so that heights for chars like
      // 'q' and 'g' will be taken into account when creating a
      // boundingbox.
      (void) cc_glyph2d_getbitmap(glyph, bitmapsize, bitmappos);

      // Advance & Kerning
      if (j > 0) cc_glyph2d_getkerning(prevglyph, glyph, &kerningx, &kerningy);
      cc_glyph2d_getadvance(glyph, &advancex, &advancey);           
      SbVec2s kerning((short) kerningx, (short) kerningy);
      SbVec2s advance((short) advancex, (short) advancey);
      
      SbVec2s pos = penpos + SbVec2s((short) bitmappos[0], (short) bitmappos[1]) + SbVec2s(0, (short) -bitmapsize[1]);
      this->bbox.extendBy(pos);
      this->bbox.extendBy(pos + SbVec2s(advance[0] + kerning[0] + glyphwidth, bitmapsize[1]));
      this->positions[i].append(pos);

      penpos += kerning + advance + SbVec2s(glyphwidth + 1, 0);
      prevglyph = glyph;

    }

    this->stringwidth.append(actuallength);
    penpos = SbVec2s(0, penpos[1] - (short)(SoFontSizeElement::get(state) * PUBLIC(this)->spacing.getValue()));

  }
}
