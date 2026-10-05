/**************************************************************************\
 * Copyright (c) Kongsberg Oil & Gas Technologies AS
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
\**************************************************************************/

/*!
  \class SoText2 SoText2.h Inventor/nodes/SoText2.h
  \brief The SoText2 class is a node type for visualizing 2D text aligned with the camera plane.

  \ingroup coin_nodes

  SoText2 text is not scaled according to the distance from the
  camera, and is not influenced by rotation or scaling as 3D
  primitives are. If these are properties that you want the text to
  have, you should instead use an SoText3 or SoAsciiText node.

  Note that even though the size of the 2D text is not influenced by
  the distance from the camera, the text is still subject to the usual
  rules with regard to the depth buffer, so it \e will be obscured by
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

  SoScale nodes cannot be used to influence the dimensions of the
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

#include <climits>
#include <cmath>
#include <cstring>
#include <limits>
#include <new>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/SbBox2i32.h>
#include <Inventor/SbDPMatrix.h>
#include <Inventor/SbDPViewVolume.h>
#include <Inventor/SbVec2d.h>
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
  Used to specify horizontal string alignment.
*/
/*!
  \var SoText2::Justification SoText2::LEFT
  Left edges of strings are aligned.
*/
/*!
  \var SoText2::Justification SoText2::RIGHT
  Right edges of strings are aligned.
*/
/*!
  \var SoText2::Justification SoText2::CENTER
  Centers of strings are aligned.
*/

/*!
  \var SoMFString SoText2::string

  The set of strings to render.  Each string in the multiple value
  field will be rendered on a separate line.

  The default value of the field is a single empty string.
*/
/*!
  \var SoSFFloat SoText2::spacing

  Vertical spacing between the baselines of two consecutive horizontal lines.
  Default value is 1.0, which means that it is equal to the vertical size of
  the highest character in the bitmap alphabet.
*/
/*!
  \var SoSFEnum SoText2::justification

  Determines horizontal alignment of text strings.

  If justification is set to SoText2::LEFT, the left edge of the first string
  is at the origin and all strings are aligned with their left edges.
  If set to SoText2::RIGHT, the right edge of the first string is
  at the origin and all strings are aligned with their right edges. Otherwise,
  if set to SoText2::CENTER, the center of the first string is at the
  origin and all strings are aligned with their centers.
  The origin is always located at the baseline of the first line of text.

  Default value is SoText2::LEFT.
*/

class SoText2P {
public:
  SoText2P(SoText2 * textnode) : maxwidth(0), master(textnode)
  {
    this->bbox.makeEmpty();
  }

  SbBool getQuad(SoState * state, SbVec3f & v0, SbVec3f & v1,
                 SbVec3f & v2, SbVec3f & v3, SbBool visibleonly = TRUE);
  void flushGlyphCache();
  void buildGlyphCache(SoState * state);
  SbBool shouldBuildGlyphCache(SoState * state);
  void dumpBuffer(unsigned char * buffer, SbVec2s size, SbVec2s pos, SbBool mono);
  void computeBBox(SoAction * action, SbBox3f & box, SbVec3f & center, SbBool visibleonly = TRUE);
  static void setRasterPos3f(GLfloat x, GLfloat y, GLfloat z);


  SbList <int> stringwidth;
  int maxwidth;
  SbList< SbList<SbVec2i32> > positions;
  SbBox2i32 bbox;

  SoGlyphCache * cache;
  SoFieldSensor * spacingsensor;
  SoFieldSensor * stringsensor;
  unsigned char * pixel_buffer;
  size_t pixel_buffer_size;

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
  // especially since Microsoft Windows might have rather strict limits on the
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

/*!
  \copydetails SoNode::initClass(void)
*/
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

  // Render only if bbox not outside cull planes.
  SbBox3f box;
  SbVec3f center;
  PRIVATE(this)->computeBBox(action, box, center);
  if (!box.isEmpty() && !SoCullElement::cullTest(state, box, TRUE)) {
    const cc_font_specification * fontspec = PRIVATE(this)->cache->getCachedFontspec();
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

    const SbVec2i32 bbmin = PRIVATE(this)->bbox.getMin();
    const SbVec2i32 bbmax = PRIVATE(this)->bbox.getMax();

    float textscreenoffsetx = nilpoint[0]+bbmin[0];
    switch (this->justification.getValue()) {
    case SoText2::LEFT:
      break;
    case SoText2::RIGHT:
      textscreenoffsetx = nilpoint[0] + bbmin[0] - PRIVATE(this)->maxwidth;
      break;
    case SoText2::CENTER:
      textscreenoffsetx = (nilpoint[0] + bbmin[0] - PRIVATE(this)->maxwidth / 2.0f);
      break;
    }

    // Set new state.
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, vpsize[0], 0, vpsize[1], -1.0f, 1.0f);
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);

    const double lineadvance = std::trunc(std::trunc(double(SoFontSizeElement::get(state))) *
                                          double(this->spacing.getValue()));
    int64_t xpos = 0;
    int64_t ypos = 0;
    int64_t rasterx, rastery;
    const double originx = std::floor(double(textscreenoffsetx) + 0.5);
    const double originy = std::floor(double(nilpoint[1]) + 0.5) + bbmin[1];
    const int bufferx = int(SbClamp(originx, 0.0, double(vpsize[0])));
    const int buffery = int(SbClamp(originy, 0.0, double(vpsize[1])));
    const int bufferwidth = int(SbClamp(originx + double(bbmax[0]) - bbmin[0],
                                      0.0, double(vpsize[0]))) - bufferx;
    const int bufferheight = int(SbClamp(originy + double(bbmax[1]) - bbmin[1],
                                       0.0, double(vpsize[1]))) - buffery;
    int ix=0, iy=0;
    int bitmappos[2];
    int bitmapsize[2];
    const unsigned char * buffer = NULL;
    cc_glyph2d * prevglyph = NULL;

    const int nrlines = this->string.getNum();

    // get the current diffuse color
    const SbColor & diffuse = SoLazyElement::getDiffuse(state, 0);
    unsigned char red   = (unsigned char) (diffuse[0] * 255.0f);
    unsigned char green = (unsigned char) (diffuse[1] * 255.0f);
    unsigned char blue  = (unsigned char) (diffuse[2] * 255.0f);
    const unsigned int alpha = (unsigned int)((1.0f - SoLazyElement::getTransparency(state, 0)) * 256);

    state->push();

    // disable textures for all units
    SoGLMultiTextureEnabledElement::disableAll(state);

    glPushAttrib(GL_ENABLE_BIT | GL_PIXEL_MODE_BIT | GL_COLOR_BUFFER_BIT);
    glPushClientAttrib(GL_CLIENT_PIXEL_STORE_BIT);

    SbBool drawPixelBuffer = FALSE;
    SbBool bufferfailed = FALSE;

    for (int i = 0; i < nrlines; i++) {
      SbString str = this->string[i];
      switch (this->justification.getValue()) {
      case SoText2::LEFT:
        xpos = 0;
        break;
      case SoText2::RIGHT:
        xpos = PRIVATE(this)->maxwidth - PRIVATE(this)->stringwidth[i];
        break;
      case SoText2::CENTER:
        xpos = (PRIVATE(this)->maxwidth - PRIVATE(this)->stringwidth[i]) / 2;
        break;
      }

      int kerningx = 0;
      int kerningy = 0;
      int advancex = 0;
      int advancey = 0;

      const char * p = str.getString();
      size_t length = cc_string_utf8_validate_length(p);

      for (unsigned int strcharidx = 0; strcharidx < length; strcharidx++) {
        uint32_t glyphidx = 0;

        glyphidx = cc_string_utf8_get_char(p);
        p = cc_string_utf8_next_char(p);

        cc_glyph2d * glyph = cc_glyph2d_ref(glyphidx, fontspec, 0.0f);

        buffer = cc_glyph2d_getbitmap(glyph, bitmapsize, bitmappos);

        ix = bitmapsize[0];
        iy = bitmapsize[1];

        // Advance & Kerning
        if (strcharidx > 0)
          cc_glyph2d_getkerning(prevglyph, glyph, &kerningx, &kerningy);
        cc_glyph2d_getadvance(glyph, &advancex, &advancey);

        rasterx = xpos + kerningx + int64_t(bitmappos[0]);
        rastery = ypos + int64_t(bitmappos[1]) - bitmapsize[1];

        if (buffer) {
          if (cc_glyph2d_getmono(glyph)) {
            SoText2P::setRasterPos3f((float)rasterx + textscreenoffsetx, GLfloat(double(rastery) + std::trunc(double(nilpoint[1]))), -nilpoint[2]);
            glBitmap(ix,iy,0,0,0,0,(const GLubyte *)buffer);
          }
          else {
            if (!drawPixelBuffer && !bufferfailed && bufferwidth > 0 && bufferheight > 0) {
              const size_t width = size_t(bufferwidth);
              const size_t height = size_t(bufferheight);
              if (width > std::numeric_limits<size_t>::max() / 4 / height) {
                bufferfailed = TRUE;
              }
              else {
                const size_t bytes = width * height * 4;
                if (bytes > PRIVATE(this)->pixel_buffer_size) {
                  unsigned char * replacement = new (std::nothrow) unsigned char[bytes];
                  if (!replacement) bufferfailed = TRUE;
                  else {
                    delete[] PRIVATE(this)->pixel_buffer;
                    PRIVATE(this)->pixel_buffer = replacement;
                    PRIVATE(this)->pixel_buffer_size = bytes;
                  }
                }
                if (!bufferfailed) {
                  memset(PRIVATE(this)->pixel_buffer, 0, bytes);
                  drawPixelBuffer = TRUE;
                }
              }
            }
            if (drawPixelBuffer) {
              const int64_t memx = int64_t(originx) + rasterx - bbmin[0] - bufferx;
              const int64_t memy = int64_t(originy) + rastery - bbmin[1] - buffery;
              const int64_t left = SbMax(int64_t(0), memx);
              const int64_t bottom = SbMax(int64_t(0), memy);
              const int64_t right = SbMin(int64_t(bufferwidth), memx + ix);
              const int64_t top = SbMin(int64_t(bufferheight), memy + iy);
              for (int64_t y = bottom; y < top && left < right; ++y) {
                unsigned char * dst = PRIVATE(this)->pixel_buffer +
                  (size_t(y) * size_t(bufferwidth) + size_t(left)) * 4;
                const unsigned char * src = buffer + size_t(y - memy) * size_t(ix) + size_t(left - memx);
                for (int64_t x = left; x < right; ++x) {
                  *dst++ = red; *dst++ = green; *dst++ = blue;
                  const int srcval = *src++;
                  const int oldval = *dst;
                  *dst++ = ((oldval * (256 - srcval) + alpha * srcval) >> 8);
                }
              }
            }
          }
        }

        xpos += int64_t(advancex) + kerningx;

        if (prevglyph) {
          // should be safe to unref here. SoGlyphCache will have a
          // ref'ed instance
          cc_glyph2d_unref(prevglyph);
        }
        prevglyph = glyph;
      }

      ypos -= int64_t(lineadvance);
    }

    if (prevglyph) {
      // should be safe to unref here. SoGlyphCache will have a ref'ed
      // instance
      cc_glyph2d_unref(prevglyph);
    }

    if (drawPixelBuffer) {
      glEnable(GL_ALPHA_TEST);
      glAlphaFunc(GL_GREATER, 0.3f);
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

      SoText2P::setRasterPos3f(GLfloat(bufferx), GLfloat(buffery), -nilpoint[2]);
      glDrawPixels(bufferwidth, bufferheight, GL_RGBA, GL_UNSIGNED_BYTE, (const GLubyte *)PRIVATE(this)->pixel_buffer);
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
  // Bounds must include offscreen text so viewAll can find it.
  PRIVATE(this)->computeBBox(action, box, center, FALSE);
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
    int32_t minx, miny, maxx, maxy;
    PRIVATE(this)->bbox.getBounds(minx, miny, maxx, maxy);
    float bbwidth = float(double(maxx) - double(minx));
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
  this->maxwidth=0;
  this->positions.truncate(0);
  this->bbox.makeEmpty();
}

// Calculates a quad around the text in 3D.
//  Return FALSE if the quad is empty.
SbBool
SoText2P::getQuad(SoState * state, SbVec3f & v0, SbVec3f & v1,
                  SbVec3f & v2, SbVec3f & v3, SbBool visibleonly)
{
  this->buildGlyphCache(state);

  int32_t xmin, ymin, xmax, ymax;
  this->bbox.getBounds(xmin, ymin, xmax, ymax);

  if (this->bbox.isEmpty()) return FALSE;

  const SbDPMatrix model(SoModelMatrixElement::get(state));
  SbVec3d anchor;
  model.multVecMatrix(SbVec3d(0.0, 0.0, 0.0), anchor);
  const SbViewVolume & vv = SoViewVolumeElement::get(state);
  const SbDPViewVolume & dpvv = vv.getDPViewVolume();
  const SbVec2s vpsize = SoViewportRegionElement::get(state).getViewportSizePixels();
  if (vpsize[0] <= 0 || vpsize[1] <= 0) return FALSE;
  for (int axis = 0; axis < 3; ++axis) {
    if (!std::isfinite(anchor[axis])) return FALSE;
  }

  SbVec3d direction = dpvv.getProjectionDirection();
  direction.normalize();
  double distance = (anchor - dpvv.getProjectionPoint()).dot(direction);
  const SbBool atcamera = dpvv.getProjectionType() == SbDPViewVolume::PERSPECTIVE && distance == 0.0;
  SbVec3d screenpoint;
  SbVec3d offset(0.0, 0.0, 0.0);
  if (atcamera) {
    // The projection is singular at the eye plane. Bounds still need a
    // finite footprint there so viewAll can move the camera away from it.
    // Drawing and picking retain the ordinary visibility contract.
    if (visibleonly) return FALSE;
    distance = std::fabs(dpvv.getNearDist());
    if (!(distance > 0.0) || !std::isfinite(distance)) return FALSE;
    screenpoint.setValue(0.5, 0.5, 0.0);
    offset = anchor - dpvv.getPlanePoint(distance, SbVec2d(0.5, 0.5));
  }
  else dpvv.projectToScreen(anchor, screenpoint);

  const double sx = screenpoint[0] * vpsize[0];
  const double sy = screenpoint[1] * vpsize[1];
  if (!std::isfinite(sx) || !std::isfinite(sy) || !std::isfinite(distance)) return FALSE;
  const double px = std::trunc(sx);
  const double py = std::trunc(sy);
  double alignment = 0.0;
  const double width = double(xmax) - xmin;
  switch (PUBLIC(this)->justification.getValue()) {
  case SoText2::LEFT: break;
  case SoText2::RIGHT: alignment = width; break;
  case SoText2::CENTER: alignment = width * 0.5; break;
  default: assert(0 && "unknown alignment"); break;
  }
  const SbVec2d corners[4] = {
    SbVec2d((px + xmin - alignment) / vpsize[0], (py + ymax) / vpsize[1]),
    SbVec2d((px + xmax - alignment) / vpsize[0], (py + ymax) / vpsize[1]),
    SbVec2d((px + xmax - alignment) / vpsize[0], (py + ymin) / vpsize[1]),
    SbVec2d((px + xmin - alignment) / vpsize[0], (py + ymin) / vpsize[1])
  };
  SbVec3d points[4];
  SbBox3f testbox;
  for (int i = 0; i < 4; ++i) {
    points[i] = dpvv.getPlanePoint(distance, corners[i]) + offset;
    SbVec3f point;
    for (int axis = 0; axis < 3; ++axis) {
      if (!std::isfinite(points[i][axis]) ||
          std::fabs(points[i][axis]) > std::numeric_limits<float>::max()) return FALSE;
      point[axis] = float(points[i][axis]);
    }
    testbox.extendBy(point);
  }
  if (visibleonly && !vv.intersect(testbox)) return FALSE;

  if (!visibleonly) {
    // Preserve a conservative world footprint before the bounding-box action
    // applies the float model matrix to the object-space bounds again.
    for (int axis = 0; axis < 3; ++axis) {
      double low = points[0][axis], high = low;
      for (int i = 1; i < 4; ++i) {
        low = SbMin(low, points[i][axis]);
        high = SbMax(high, points[i][axis]);
      }
      if (high > low && float(low) == float(high)) {
        const float value = float(low);
        const float lower = std::nextafter(value, -std::numeric_limits<float>::infinity());
        const float upper = std::nextafter(value, std::numeric_limits<float>::infinity());
        if (!std::isfinite(lower) || !std::isfinite(upper)) return FALSE;
        points[0][axis] = lower;
        points[1][axis] = upper;
      }
    }
  }

  const SbDPMatrix inverse = model.inverse();
  SbVec3f * output[4] = { &v0, &v1, &v2, &v3 };
  for (int i = 0; i < 4; ++i) {
    inverse.multVecMatrix(points[i], points[i]);
    for (int axis = 0; axis < 3; ++axis) {
      if (!std::isfinite(points[i][axis]) ||
          std::fabs(points[i][axis]) > std::numeric_limits<float>::max()) return FALSE;
      (*output[i])[axis] = float(points[i][axis]);
    }
  }
  if (!visibleonly) {
    // SbBox3f cannot represent a sub-ULP footprint at a large coordinate.
    // Keep bounds conservative rather than collapsing a nonzero extent.
    for (int axis = 0; axis < 3; ++axis) {
      double low = points[0][axis], high = low;
      SbBool collapsed = TRUE;
      for (int i = 1; i < 4; ++i) {
        low = SbMin(low, points[i][axis]);
        high = SbMax(high, points[i][axis]);
        collapsed = collapsed && (*output[i])[axis] == (*output[0])[axis];
      }
      if (collapsed && high > low) {
        const float value = (*output[0])[axis];
        const float lower = std::nextafter(value, -std::numeric_limits<float>::infinity());
        const float upper = std::nextafter(value, std::numeric_limits<float>::infinity());
        if (!std::isfinite(lower) || !std::isfinite(upper)) return FALSE;
        (*output[0])[axis] = lower;
        (*output[1])[axis] = upper;
      }
    }
  }

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
  // Do not pass non-finite or out-of-range sizes to the font backend.
  const double requestedsize = SoFontSizeElement::get(state);
  if (!std::isfinite(requestedsize) || requestedsize <= 0.0 || requestedsize > INT_MAX) {
    state->pop();
    SoCacheElement::setInvalid(storedinvalid);
    if (oldcache) oldcache->unref();
    return;
  }
  this->cache->readFontspec(state);

  float fontsize = SoFontSizeElement::get(state);
  const double spacing = PUBLIC(this)->spacing.getValue();
  const double lineadvance = std::trunc(std::trunc(double(fontsize)) * spacing);
  SbBool valid = std::isfinite(fontsize) && fontsize > 0.0f &&
    double(fontsize) <= INT_MAX && std::isfinite(lineadvance) &&
    std::fabs(lineadvance) <= INT_MAX;
  int64_t ypos = 0;
  int64_t maxoverhang = INT_MIN;

  const int nrlines = PUBLIC(this)->string.getNum();

  const cc_font_specification * fontspec = this->cache->getCachedFontspec();

  this->bbox.makeEmpty();

  for (int i=0; valid && i < nrlines; i++) {
    SbString str = PUBLIC(this)->string[i];
    this->positions.append(SbList<SbVec2i32>());

    SbBox2i32 linebbox;
    int64_t xpos = 0;
    int64_t actuallength = 0;
    int kerningx = 0;
    int kerningy = 0;
    int advancex = 0;
    int advancey = 0;
    int bitmapsize[2];
    int bitmappos[2];
    const cc_glyph2d * prevglyph = NULL;
    const char * p = str.getString();
    size_t length = cc_string_utf8_validate_length(p);

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

      const int64_t px = xpos + kerningx + int64_t(bitmappos[0]);
      const int64_t py = ypos + int64_t(bitmappos[1]) - bitmapsize[1];
      const int64_t nextx = xpos + int64_t(advancex) + kerningx;
      valid = bitmapsize[0] >= 0 && bitmapsize[1] >= 0 &&
        px >= INT32_MIN && px <= INT32_MAX &&
        py >= INT32_MIN && py <= INT32_MAX &&
        px + bitmapsize[0] <= INT32_MAX && py + bitmapsize[1] <= INT32_MAX &&
        nextx >= 0 && nextx <= INT32_MAX;
      if (!valid) break;
      const SbVec2i32 pos = SbVec2i32(int32_t(px), int32_t(py));
      linebbox.extendBy(pos);
      linebbox.extendBy(SbVec2i32(int32_t(px + bitmapsize[0]), int32_t(py + bitmapsize[1])));
      this->positions[i].append(pos);
      actuallength = nextx;
      xpos = nextx;
      prevglyph = glyph;
    }

    this->bbox.extendBy(linebbox);
    this->stringwidth.append(int(actuallength));
    if (actuallength > this->maxwidth) this->maxwidth=int(actuallength);

    // bitmap of last character can end before or beyond starting position of next character
    if (!linebbox.isEmpty())
    {
      int64_t overhang = int64_t(linebbox.getMax()[0]) - actuallength;
      if (overhang > maxoverhang) maxoverhang = overhang;
    }

    if (valid) ypos -= int64_t(lineadvance);
  }

  // extent bbox to include maxoverhang at the maxwidth string
  // this is needed for right-aligned text which gets aligned at the maxwidth
  // position, because there can be other strings with bitmaps going beyond
  if (valid && maxoverhang > INT_MIN) {
    const int64_t right = int64_t(this->maxwidth) + maxoverhang;
    valid = right >= INT32_MIN && right <= INT32_MAX;
    if (valid) this->bbox.extendBy(SbVec2i32(int32_t(right), this->bbox.getMax()[1]));
  }
  if (!valid) this->flushGlyphCache();

  state->pop();
  SoCacheElement::setInvalid(storedinvalid);

  if (oldcache) oldcache->unref();
}

void
SoText2P::computeBBox(SoAction * action, SbBox3f & box, SbVec3f & center, SbBool visibleonly)
{
  SbVec3f v0, v1, v2, v3;
  // this will cause a cache dependency on the view volume,
  // model matrix and viewport.
  if (!this->getQuad(action->getState(), v0, v1, v2, v3, visibleonly)) {
    return; // empty
  }
  box.makeEmpty();

  box.extendBy(v0);
  box.extendBy(v1);
  box.extendBy(v2);
  box.extendBy(v3);

  center = box.getCenter();
}

// Sets the raster position for GL raster operations.
// Handles the special case where the x/y coordinates are negative
void
SoText2P::setRasterPos3f(GLfloat x, GLfloat y, GLfloat z)
{
  float rpx = x >= 0 ? x : 0;
  int offvp = x < 0 ? 1 : 0;
  float offsetx = x >= 0 ? 0 : x;

  float rpy = y >= 0 ? y : 0;
  offvp = offvp || y < 0 ? 1 : 0;
  float offsety = y >= 0 ? 0 : y;

  glRasterPos3f(rpx,rpy,z);
  if (offvp) { glBitmap(0, 0, 0, 0,offsetx,offsety, NULL); }
}

#undef PRIVATE
#undef PUBLIC
