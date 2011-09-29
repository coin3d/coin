/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
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

  The size of the fonts on screen is decided from the SoFont::size
  field of a preceding SoFont-node in the scene graph, which specifies
  the size in pixel dimensions. This value sets the approximate
  vertical dimension of the letters.  The default value if no
  SoFont-nodes are used, is 10.

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

  SoScale nodes can not be used to influence the dimensions of the
  rendering output of SoText2 nodes.

  <b>FILE FORMAT/DEFAULTS:</b>
  \code
    Text2 {
        string ""
        spacing 1
        justification LEFT
    }
  \endcode

  \sa SoFont, SoFontStyle, SoText3, SoAsciiText
*/

#include <Inventor/nodes/SoText2.h>
#include "coindefs.h"

#include <limits.h>
#include <string.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

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
#include <Inventor/elements/SoComplexityTypeElement.h>
#include <Inventor/elements/SoComplexityElement.h>
#include <Inventor/elements/SoLazyElement.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/elements/SoProjectionMatrixElement.h>
#include <Inventor/elements/SoViewingMatrixElement.h>
#include <Inventor/elements/SoViewVolumeElement.h>
#include <Inventor/elements/SoViewportRegionElement.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/elements/SoCacheElement.h>
#include <Inventor/elements/SoMultiTextureEnabledElement.h>
#include <Inventor/elements/SoGLMultiTextureEnabledElement.h>

#ifdef COIN_THREADSAFE
#include <Inventor/threads/SbMutex.h>
#endif // COIN_THREADSAFE

#include "nodes/SoSubNodeP.h"
#include "caches/SoGlyphCache.h"

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
#undef WIN32_LEAN_AND_MEAN
// UPDATE, FIXME: due to some reorganization of header files GL/glx.h
// should not be included anywhere for this source code file any
// more. This means the hack above should no longer be necessary. To
// test, try building this file with g++ on a Cygwin system where both
// windows.h and GL/glx.h are available. If that works fine, remove
// the "#define WIN32_LEAN_AND_MEAN" hack. 20030625 mortene.


#include "fonts/glyph2d.h"

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
  }

  SbBool getQuad(SoState * state, SbVec3f & v0, SbVec3f & v1,
                 SbVec3f & v2, SbVec3f & v3);
  void flushGlyphCache();
  void buildGlyphCache(SoState * state);
  SbBool shouldBuildGlyphCache(SoState * state);
  void dumpBuffer(unsigned char * buffer, SbVec2s size, SbVec2s pos, SbBool mono);
  void computeBBox(SoAction * action, SbBox3f & box, SbVec3f & center);


  SbList <int> stringwidth;
  SbList< SbList<SbVec2s> > positions;
  SbBox2s bbox;

  SoGlyphCache * cache;
  SoFieldSensor * spacingsensor;
  SoFieldSensor * stringsensor;
  unsigned char * pixel_buffer;
  int pixel_buffer_size;

  static void sensor_cb(void * userdata, SoSensor * COIN_UNUSED_ARG(s)) {
    SoText2P * thisp = (SoText2P*) userdata;
    thisp->lock();
    if (thisp->cache) thisp->cache->invalidate();
    thisp->unlock();
  }
  void lock(void) {
#ifdef COIN_THREADSAFE
    this->mutex.lock();
#endif // COIN_THREADSAFE
  }
  void unlock(void) {
#ifdef COIN_THREADSAFE
    this->mutex.unlock();
#endif // COIN_THREADSAFE
  }
private:
#ifdef COIN_THREADSAFE
  // FIXME: a mutex for every instance seems a bit excessive,
  // especially since MSWindows might have rather strict limits on the
  // total amount of mutex resources a process (or even a user) can
  // allocate. so consider making this a class-wide instance instead.
  // -mortene.
  SbMutex mutex;
#endif // COIN_THREADSAFE
  SoText2 * master;
};

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

  PRIVATE(this)->stringsensor = new SoFieldSensor(SoText2P::sensor_cb, PRIVATE(this));
  PRIVATE(this)->stringsensor->attach(&this->string);
  PRIVATE(this)->stringsensor->setPriority(0);
  PRIVATE(this)->spacingsensor = new SoFieldSensor(SoText2P::sensor_cb, PRIVATE(this));
  PRIVATE(this)->spacingsensor->attach(&this->spacing);
  PRIVATE(this)->spacingsensor->setPriority(0);
  PRIVATE(this)->cache = NULL;
  PRIVATE(this)->pixel_buffer = NULL;
  PRIVATE(this)->pixel_buffer_size = 0;
}

/*!
  Destructor.
*/
SoText2::~SoText2()
{
  if (PRIVATE(this)->cache) PRIVATE(this)->cache->unref();
  delete[] PRIVATE(this)->pixel_buffer;
  delete PRIVATE(this)->stringsensor;
  delete PRIVATE(this)->spacingsensor;

  PRIVATE(this)->flushGlyphCache();
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

  PRIVATE(this)->lock();
  PRIVATE(this)->buildGlyphCache(state);
  SoCacheElement::addCacheDependency(state, PRIVATE(this)->cache);

  const cc_font_specification * fontspec = PRIVATE(this)->cache->getCachedFontspec();

  // Render only if bbox not outside cull planes.
  SbBox3f box;
  SbVec3f center;
  PRIVATE(this)->computeBBox(action, box, center);
  if (!SoCullElement::cullTest(state, box, TRUE)) {
    SoMaterialBundle mb(action);
    mb.sendFirst();
    SbVec3f nilpoint(0.0f, 0.0f, 0.0f);
    const SbMatrix & mat = SoModelMatrixElement::get(state);
    const SbMatrix & projmatrix = (mat * SoViewingMatrixElement::get(state) *
                                   SoProjectionMatrixElement::get(state));
    const SbViewportRegion & vp = SoViewportRegionElement::get(state);
    SbVec2s vpsize = vp.getViewportSizePixels();

    projmatrix.multVecMatrix(nilpoint, nilpoint);
    nilpoint[0] = (nilpoint[0] + 1.0f) * 0.5f * vpsize[0];
    nilpoint[1] = (nilpoint[1] + 1.0f) * 0.5f * vpsize[1];      
 
    // Set new state.
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, vpsize[0], 0, vpsize[1], -1.0f, 1.0f);
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);

    float fontsize = SoFontSizeElement::get(state);
    float xpos = nilpoint[0];      // to get rid of compiler warning..
    float ypos = nilpoint[1];
    float rasterx, rastery, rpx, rpy, offsetx, offsety;
    int ix=0, iy=0;
    int offvp;
    int thispos[2];
    int thissize[2];
    const unsigned char * buffer = NULL;
    cc_glyph2d * prevglyph = NULL;
    
    const int nrlines = this->string.getNum();

    // get the current diffuse color
    const SbColor & diffuse = SoLazyElement::getDiffuse(state, 0);
    unsigned char red   = (unsigned char) (diffuse[0] * 255.0f);
    unsigned char green = (unsigned char) (diffuse[1] * 255.0f);
    unsigned char blue  = (unsigned char) (diffuse[2] * 255.0f);
    const float alpha = 1.0f - SoLazyElement::getTransparency(state, 0);
    
    state->push();
    
    // disable textures for all units
    SoGLMultiTextureEnabledElement::disableAll(state);
    
    glPushAttrib(GL_ENABLE_BIT | GL_PIXEL_MODE_BIT | GL_COLOR_BUFFER_BIT);
    glPushClientAttrib(GL_CLIENT_PIXEL_STORE_BIT);
    
    SbBool didenableblend = FALSE;

    for (int i = 0; i < nrlines; i++) {
      SbString str = this->string[i];
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

      const char * p = str.getString();
      size_t length = cc_string_utf8_validate_length(p);

      for (unsigned int strcharidx = 0; strcharidx < length; strcharidx++) {
        uint32_t glyphidx = 0;

        glyphidx = cc_string_utf8_get_char(p);
        p = cc_string_utf8_next_char(p);

        cc_glyph2d * glyph = cc_glyph2d_ref(glyphidx, fontspec, 0.0f);
        
        buffer = cc_glyph2d_getbitmap(glyph, thissize, thispos);
        
        ix = thissize[0];
        iy = thissize[1];
        
        int advancex, advancey;
        cc_glyph2d_getadvance(glyph, &advancex, &advancey);

        int kerningx, kerningy;
        if (strcharidx > 0) {
          cc_glyph2d_getkerning(prevglyph, glyph, &kerningx, &kerningy);
        } 
        else {
          kerningx = 0;
          kerningy = 0;          
        }

        rasterx = xpos + kerningx + thispos[0]; 
        rpx = rasterx >= 0 ? rasterx : 0;
        offvp = rasterx < 0 ? 1 : 0;
        offsetx = rasterx >= 0 ? 0 : rasterx;
          
        rastery = ypos + (thispos[1] - thissize[1]);
        rpy = rastery>= 0 ? rastery : 0;
        offvp = offvp || rastery < 0 ? 1 : 0;
        offsety = rastery >= 0 ? 0 : rastery;

        glRasterPos3f(rpx, rpy, -nilpoint[2]);

        if (offvp) { glBitmap(0,0,0,0,offsetx,offsety,NULL); }

        if (buffer) {
          if (cc_glyph2d_getmono(glyph)) {
            if (didenableblend) {
              glDisable(GL_BLEND);
              glDisable(GL_ALPHA_TEST);
              didenableblend = FALSE;
            }
            glBitmap(ix,iy,0,0,0,0,(const GLubyte *)buffer);
          } 
          else {
            if (!didenableblend) {
              glEnable(GL_ALPHA_TEST);
              glAlphaFunc(GL_GREATER, 0.3f);
          
              glEnable(GL_BLEND);
              glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
              didenableblend = TRUE;
            }
            glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
        
            int numpixels = ix * iy;
 
            if (numpixels > PRIVATE(this)->pixel_buffer_size) {
              delete[] PRIVATE(this)->pixel_buffer;
              PRIVATE(this)->pixel_buffer = new unsigned char[numpixels*4];
              PRIVATE(this)->pixel_buffer_size = numpixels;
            }
            unsigned char * dst = PRIVATE(this)->pixel_buffer;
            const unsigned char * src = buffer;

            // Ouch. This must lead to pretty slow rendering
            if (alpha == 1.0f) {
              for (int i = 0; i < numpixels; i++) {
                *dst++ = red; *dst++ = green; *dst++ = blue;
                // alpha from the gray level pixel value
                *dst++ = *src++;
              }
            } else {
              for (int i = 0; i < numpixels; i++) {
                *dst++ = red; *dst++ = green; *dst++ = blue;
                // alpha from the gray level pixel value
                *dst++ = (((unsigned int)(alpha * 256.0f)) * *src++) >> 8;
              }
            }
            glDrawPixels(ix,iy,GL_RGBA,GL_UNSIGNED_BYTE,(const GLubyte *)PRIVATE(this)->pixel_buffer);

          }
        }

        xpos += (advancex + kerningx);

        if (prevglyph) {
          // should be safe to unref here. SoGlyphCache will have a
          // ref'ed instance
          cc_glyph2d_unref(prevglyph);
        }
        prevglyph = glyph;
      }
      
      ypos -= (((int) fontsize) * this->spacing.getValue());
    }

    if (prevglyph) {
      // should be safe to unref here. SoGlyphCache will have a ref'ed
      // instance
      cc_glyph2d_unref(prevglyph);
    }

    // pop old state
    glPopClientAttrib();
    glPopAttrib();
    state->pop();
      
    glPixelStorei(GL_UNPACK_ALIGNMENT,4);
    // Pop old GL matrix state.
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
  }
  
  PRIVATE(this)->unlock();

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
  PRIVATE(this)->lock();
  PRIVATE(this)->computeBBox(action, box, center);
  SoCacheElement::addCacheDependency(action->getState(), PRIVATE(this)->cache);
  PRIVATE(this)->unlock();
}

// doc in super
void
SoText2::rayPick(SoRayPickAction * action)
{
  if (!this->shouldRayPick(action)) return;

  PRIVATE(this)->lock();
  PRIVATE(this)->buildGlyphCache(action->getState());
  action->setObjectSpace();
  
  SbVec3f v0, v1, v2, v3;
  if (!PRIVATE(this)->getQuad(action->getState(), v0, v1, v2, v3)) {
    PRIVATE(this)->unlock();
    return; // empty
  }

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
  PRIVATE(this)->unlock();
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
SoText2::generatePrimitives(SoAction * COIN_UNUSED_ARG(action))
{
  // This is supposed to be empty. There are no primitives.
}


// SoText2P methods below

void
SoText2P::flushGlyphCache()
{
  this->stringwidth.truncate(0);
  this->positions.truncate(0);
  this->bbox.makeEmpty();
}


// Calculates a quad around the text in 3D.
//  Return FALSE if the quad is empty.
SbBool
SoText2P::getQuad(SoState * state, SbVec3f & v0, SbVec3f & v1,
                  SbVec3f & v2, SbVec3f & v3)
{
  this->buildGlyphCache(state);

  short xmin, ymin, xmax, ymax;
  this->bbox.getBounds(xmin, ymin, xmax, ymax);
  
  // FIXME: Why doesn't the SbBox2s have an 'isEmpty()' method as well?
  // (20040308 handegar)
  if (xmax < xmin) return FALSE;
  
  SbVec3f nilpoint(0.0f, 0.0f, 0.0f);
  const SbMatrix & mat = SoModelMatrixElement::get(state);
  mat.multVecMatrix(nilpoint, nilpoint);

  const SbViewVolume &vv = SoViewVolumeElement::get(state);

  SbVec3f screenpoint;
  vv.projectToScreen(nilpoint, screenpoint);
  
  const SbViewportRegion & vp = SoViewportRegionElement::get(state);
  SbVec2s vpsize = vp.getViewportSizePixels();
  
  SbVec2f n0, n1, n2, n3, center;
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

  // test if the quad is outside the view frustum, ignore it in that case
  SbBox3f testbox;
  testbox.extendBy(v0);
  testbox.extendBy(v1);
  testbox.extendBy(v2);
  testbox.extendBy(v3);
  if (!vv.intersect(testbox)) return FALSE;

  // transform back to object space
  SbMatrix inv = mat.inverse();
  inv.multVecMatrix(v0, v0);
  inv.multVecMatrix(v1, v1);
  inv.multVecMatrix(v2, v2);
  inv.multVecMatrix(v3, v3);

  return TRUE;
}

// Debug convenience method.
void
SoText2P::dumpBuffer(unsigned char * buffer, SbVec2s size, SbVec2s pos, SbBool mono)
{
  // FIXME: pure debug method, remove. preng 2003-03-18.
  if (!buffer) {
    fprintf(stderr,"bitmap error: buffer pointer NULL.\n");
  } else {
    int rows = size[1];
    int bytes = mono ? size[0] >> 3 : size[0];
    fprintf(stderr, "%s bitmap dump %d * %d bytes at %d, %d:\n",
            mono ? "mono": "gray level", rows, bytes, pos[0], pos[1]);
    for (int y=rows-1; y>=0; y--) {
      for (int byte=0; byte<bytes; byte++) {
        for (int bit=0; bit<8; bit++) {
          fprintf(stderr, "%d", buffer[y*bytes + byte] & 0x80>>bit ? 1 : 0);
        }
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
  if (this->cache == NULL) return TRUE;
  return !this->cache->isValid(state);
}

void
SoText2P::buildGlyphCache(SoState * state)
{
  if (!this->shouldBuildGlyphCache(state)) { 
    return; 
  }
  
  this->flushGlyphCache();

  // don't unref the old cache until after we've created the new
  // cache.
  SoGlyphCache * oldcache = this->cache;

  state->push();
  SbBool storedinvalid = SoCacheElement::setInvalid(FALSE);
  this->cache = new SoGlyphCache(state); 
  this->cache->ref();
  SoCacheElement::set(state, this->cache);
  this->cache->readFontspec(state);

  const int nrlines = PUBLIC(this)->string.getNum();
  SbVec2s penpos(0, 0);

  const cc_font_specification * fontspec = this->cache->getCachedFontspec();

  this->bbox.makeEmpty();

  for (int i=0; i < nrlines; i++) {
    SbString str = PUBLIC(this)->string[i];
    this->positions.append(SbList<SbVec2s>());

    int actuallength = 0;
    int kerningx = 0;
    int kerningy = 0;
    int advancex = 0;
    int advancey = 0;
    int bitmapsize[2];
    int bitmappos[2];
    const cc_glyph2d * prevglyph = NULL;
    const char * p = str.getString();
    unsigned int length = cc_string_utf8_validate_length(p);

    // fetch all glyphs first
    for (unsigned int strcharidx = 0; strcharidx < length; strcharidx++) {
      uint32_t glyphidx = 0;

      glyphidx = cc_string_utf8_get_char(p);
      p = cc_string_utf8_next_char(p);

      cc_glyph2d * glyph = cc_glyph2d_ref(glyphidx, fontspec, 0.0f);
      // Should _always_ be able to get hold of a glyph -- if no
      // glyph is available for a specific character, a default
      // empty rectangle should be used.  -mortene.
      assert(glyph);

      this->cache->addGlyph(glyph);

      // Must fetch special modifiers so that heights for chars like
      // 'q' and 'g' will be taken into account when creating a
      // boundingbox.
      (void) cc_glyph2d_getbitmap(glyph, bitmapsize, bitmappos);

      // Advance & Kerning
      if (strcharidx > 0) 
        cc_glyph2d_getkerning(prevglyph, glyph, &kerningx, &kerningy);
      cc_glyph2d_getadvance(glyph, &advancex, &advancey);           
      SbVec2s kerning((short) kerningx, (short) kerningy);
      SbVec2s advance((short) advancex, (short) advancey);
  
      SbVec2s pos = penpos + SbVec2s((short) bitmappos[0], 
                                     (short) bitmappos[1]) + SbVec2s(0, (short) -bitmapsize[1]);

      this->bbox.extendBy(pos);
      this->bbox.extendBy(pos + SbVec2s(advancex + kerning[0], bitmapsize[1]));
      this->positions[i].append(pos);

      actuallength += (advancex + kerningx);

      penpos += kerning + SbVec2s(advancex,0);
      prevglyph = glyph;
    }

    this->stringwidth.append(actuallength);
    penpos = SbVec2s(0, penpos[1] - (short)(SoFontSizeElement::get(state) * PUBLIC(this)->spacing.getValue()));

  }
  state->pop();
  SoCacheElement::setInvalid(storedinvalid);

  if (oldcache) oldcache->unref();
}

void
SoText2P::computeBBox(SoAction * action, SbBox3f & box, SbVec3f & center)
{
  SbVec3f v0, v1, v2, v3;
  // this will cause a cache dependency on the view volume,
  // model matrix and viewport.
  if (!this->getQuad(action->getState(), v0, v1, v2, v3)) {
    return; // empty
  }
  box.makeEmpty();
  
  box.extendBy(v0);
  box.extendBy(v1);
  box.extendBy(v2);
  box.extendBy(v3);

  center = box.getCenter();
}

#undef PRIVATE
#undef PUBLIC
