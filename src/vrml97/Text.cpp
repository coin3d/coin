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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#ifdef HAVE_VRML97

/*!
  \class SoVRMLText SoVRMLText.h Inventor/VRMLnodes/SoVRMLText.h
  \brief The SoVRMLText class is used to represent text in a scene.

  \ingroup coin_VRMLnodes

  \WEB3DCOPYRIGHT

  Important note: currently, the implementation of this node is not
  complete, and some of the features mentioned in the documentation
  below may not be working yet.

  \WEB3DCOPYRIGHT
  
  \verbatim
  Text { 
    exposedField  MFString string    []
    exposedField  SFNode   fontStyle NULL
    exposedField  MFFloat  length    []      # [0,)
    exposedField  SFFloat  maxExtent 0.0     # [0,)
  }
  \endverbatim

  The Text node specifies a two-sided, flat text string object
  positioned in the Z=0 plane of the local coordinate system based on
  values defined in the fontStyle field (see SoVRMLFontStyle).
  Text nodes may contain multiple text strings specified using the
  UTF-8 encoding as specified by ISO 10646-1:1993 (see
  <http://www.web3d.org/documents/specifications/14772/V2.0/part1/references.html#[UTF8]>).
  The text strings are stored in the order in which the text mode
  characters are to be produced as defined by the parameters in the
  FontStyle node.

  The text strings are contained in the string field. The fontStyle
  field contains one FontStyle node that specifies the font size, font
  family and style, direction of the text strings, and any specific
  language rendering techniques used for the text.

  The maxExtent field limits and compresses all of the text strings if
  the length of the maximum string is longer than the maximum extent,
  as measured in the local coordinate system. If the text string with
  the maximum length is shorter than the maxExtent, then there is no
  compressing. The maximum extent is measured horizontally for
  horizontal text (FontStyle node: horizontal=TRUE) and vertically for
  vertical text (FontStyle node: horizontal=FALSE). The maxExtent
  field shall be greater than or equal to zero.

  The length field contains an MFFloat value that specifies the length
  of each text string in the local coordinate system. If the string is
  too short, it is stretched (either by scaling the text or by adding
  space between the characters). If the string is too long, it is
  compressed (either by scaling the text or by subtracting space
  between the characters). If a length value is missing (for example,
  if there are four strings but only three length values), the missing
  values are considered to be 0. The length field shall be greater
  than or equal to zero.

  Specifying a value of 0 for both the maxExtent and length fields
  indicates that the string may be any length.

  \sa SoVRMLFontStyle
*/

/*!
  \var SoMFString SoVRMLText::string
  The strings. Empty by default.
*/

/*!
  \var SoMFFloat SoVRMLText::length
  Length of each string in the local coordinate system.
*/

/*!
  \var SoSFNode SoVRMLText::fontStyle
  Can contain an SoVRMLFontStyle node.
*/

/*!
  \var SoSFFloat SoVRMLText::maxExtent
  Maximum object space extent of longest string.
*/

#include <Inventor/VRMLnodes/SoVRMLText.h>
#include "coindefs.h"

#include <cstddef>
#include <cstring>

#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/SbBox2f.h>
#include <Inventor/VRMLnodes/SoVRMLFontStyle.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/bundles/SoMaterialBundle.h>
#include <Inventor/details/SoTextDetail.h>
#include <Inventor/elements/SoCacheElement.h>
#include <Inventor/elements/SoComplexityTypeElement.h>
#include <Inventor/elements/SoFontNameElement.h>
#include <Inventor/elements/SoFontSizeElement.h>
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/elements/SoGLMultiTextureEnabledElement.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/lists/SbList.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/system/gl.h>
#ifdef HAVE_THREADS
#include <Inventor/threads/SbMutex.h>
#endif // HAVE_THREADS

#include "nodes/SoSubNodeP.h"
#include "caches/SoGlyphCache.h"
#include "fonts/glyph3d.h"


// *************************************************************************

class SoVRMLTextP {
public:
  enum Justification {
    FIRST,
    BEGIN,
    MIDDLE,
    END
  };

  struct LineLayout {
    float x;
    float y;
    float majorscale;
    size_t numcharacters;
    SbBox2f bounds;
  };

  SoVRMLTextP(SoVRMLText * master) : master(master) { }
  SoVRMLText * master;

  float getWidth(const int idx, const float fontsize);
  void buildLayout(const cc_font_specification * fontspec,
                   SbList<LineLayout> & lines);
  SbList <float> glyphwidths;
  void setUpGlyphs(SoState * state, SoVRMLText * textnode);
  SoGlyphCache * cache;

  void updateFontStyle(void);

  SoFieldSensor * fontstylesensor;

  int justificationmajor;
  int justificationminor;
  SbBool lefttorighttext;
  SbBool toptobottomtext;
  SbBool horizontaltext;
  float textspacing;
  float textsize;
  float maxglyphheight;
  float maxglyphwidth;
  SbBox3f maxglyphbbox;
  int fontfamily;
  int fontstyle;

#ifdef COIN_THREADSAFE
  void lock(void) { this->mutex.lock(); }
  void unlock(void) { this->mutex.unlock(); }
private:
  SbMutex mutex;
#else // !COIN_THREADSAFE
  void lock(void) { }
  void unlock(void) { }
#endif // !COIN_THREADSAFE
};

#define PRIVATE(obj) (obj->pimpl)
#define PUBLIC(obj) (obj->master)

// *************************************************************************

SO_NODE_SOURCE(SoVRMLText);

// *************************************************************************

/*!
  \copydetails SoNode::initClass(void)
*/
void
SoVRMLText::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoVRMLText, SO_VRML97_NODE_TYPE);
}

static void 
fontstylechangeCB(void * data, SoSensor * COIN_UNUSED_ARG(sensor))
{
  SoVRMLTextP * pimpl = (SoVRMLTextP *) data;
  pimpl->lock();
  if (pimpl->cache) pimpl->cache->invalidate();
  pimpl->unlock();
}

/*!
  Constructor.
*/
SoVRMLText::SoVRMLText(void)
{
  PRIVATE(this) = new SoVRMLTextP(this);

  SO_VRMLNODE_INTERNAL_CONSTRUCTOR(SoVRMLText);

  SO_VRMLNODE_ADD_EMPTY_EXPOSED_MFIELD(string);

  SO_VRMLNODE_ADD_EXPOSED_FIELD(fontStyle, (NULL));
  SO_VRMLNODE_ADD_EXPOSED_FIELD(maxExtent, (0.0f));
  SO_VRMLNODE_ADD_EMPTY_EXPOSED_MFIELD(length);

  // Default text setup
  PRIVATE(this)->textsize = 1.0f;
  PRIVATE(this)->textspacing = 1.0f;
  PRIVATE(this)->maxglyphheight = 1.0f;
  PRIVATE(this)->maxglyphwidth = 1.0f;
  PRIVATE(this)->lefttorighttext = TRUE;
  PRIVATE(this)->toptobottomtext = TRUE;
  PRIVATE(this)->horizontaltext = TRUE;
  PRIVATE(this)->justificationmajor = SoVRMLTextP::BEGIN;
  PRIVATE(this)->justificationminor = SoVRMLTextP::FIRST;
  
  PRIVATE(this)->fontstylesensor = new SoFieldSensor(fontstylechangeCB, PRIVATE(this));
  PRIVATE(this)->fontstylesensor->attach(&fontStyle);
  PRIVATE(this)->fontstylesensor->setPriority(0);
  
  PRIVATE(this)->cache = NULL;
}

float
SoVRMLTextP::getWidth(const int idx, const float COIN_UNUSED_ARG(fontsize))
{
  float w = this->glyphwidths[idx];
  float maxe = PUBLIC(this)->maxExtent.getValue();
  if (maxe > 0.0f && w > maxe) w = maxe;
  return w;
}

static void
sovrmltext_getlinebounds(const SbString & string,
                         const cc_font_specification * fontspec,
                         const SbBool horizontal,
                         const SbBool lefttoright,
                         const SbBool toptobottom,
                         const float textsize,
                         const float majorscale,
                         SbBox2f & bounds)
{
  bounds.makeEmpty();
  const char * p = string.getString();
  const size_t len = cc_string_utf8_validate_length(p);
  float cursor = 0.0f;
  cc_glyph3d * prevglyph = NULL;

  for (size_t charidx = 0; charidx < len; charidx++) {
    const uint32_t glyphidx = cc_string_utf8_get_char(p);
    p = cc_string_utf8_next_char(p);
    cc_glyph3d * glyph = cc_glyph3d_ref(glyphidx, fontspec);

    float advancex, advancey;
    cc_glyph3d_getadvance(glyph, &advancex, &advancey);
    float kerningx = 0.0f;
    float kerningy = 0.0f;
    if (prevglyph) {
      cc_glyph3d_getkerning(prevglyph, glyph, &kerningx, &kerningy);
    }

    if (horizontal) {
      if (lefttoright) cursor += kerningx * textsize * majorscale;
      else cursor -= (advancex + kerningx) * textsize * majorscale;
    }

    const SbVec2f * coords =
      reinterpret_cast<const SbVec2f *>(cc_glyph3d_getcoords(glyph));
    const int * indices = cc_glyph3d_getfaceindices(glyph);
    while (*indices >= 0) {
      const SbVec2f & coord = coords[*indices++];
      if (horizontal) {
        bounds.extendBy(SbVec2f(coord[0] * textsize * majorscale + cursor,
                                coord[1] * textsize));
      }
      else {
        bounds.extendBy(SbVec2f(coord[0] * textsize,
                                coord[1] * textsize * majorscale + cursor));
      }
    }

    if (horizontal) {
      if (lefttoright) cursor += advancex * textsize * majorscale;
    }
    else {
      cursor += (toptobottom ? -1.0f : 1.0f) * textsize * majorscale;
    }

    if (prevglyph) cc_glyph3d_unref(prevglyph);
    prevglyph = glyph;
  }
  if (prevglyph) cc_glyph3d_unref(prevglyph);
}

void
SoVRMLTextP::buildLayout(const cc_font_specification * fontspec,
                         SbList<LineLayout> & lines)
{
  const int n = PUBLIC(this)->string.getNum();
  lines.truncate(0);

  float maximumextent = 0.0f;
  for (int i = 0; i < n; i++) {
    LineLayout line;
    line.x = line.y = 0.0f;
    line.numcharacters = cc_string_utf8_validate_length(
      PUBLIC(this)->string[i].getString());
    const float naturalextent = this->horizontaltext ?
      this->glyphwidths[i] * this->textsize :
      static_cast<float>(line.numcharacters) * this->textsize;
    float extent = naturalextent;
    if (i < PUBLIC(this)->length.getNum() && PUBLIC(this)->length[i] > 0.0f) {
      extent = PUBLIC(this)->length[i];
    }
    line.majorscale = naturalextent > 0.0f ? extent / naturalextent : 1.0f;
    line.bounds.makeEmpty();
    lines.append(line);
    maximumextent = SbMax(maximumextent, extent);
  }

  float setscale = 1.0f;
  const float maxextent = PUBLIC(this)->maxExtent.getValue();
  if (maxextent > 0.0f && maximumextent > maxextent) {
    setscale = maxextent / maximumextent;
  }

  const float minorsign = this->horizontaltext ?
    (this->toptobottomtext ? -1.0f : 1.0f) :
    (this->lefttorighttext ? 1.0f : -1.0f);
  const float minorstep = this->textspacing * this->textsize;
  float blockmin = 0.0f;
  float blockmax = 0.0f;
  float firstlinemin = 0.0f;
  float firstlinemax = 0.0f;
  float lastlinemin = 0.0f;
  float lastlinemax = 0.0f;
  SbBool firstbound = TRUE;

  for (int i = 0; i < n; i++) {
    LineLayout & line = lines[i];
    line.majorscale *= setscale;
    sovrmltext_getlinebounds(PUBLIC(this)->string[i], fontspec,
                             this->horizontaltext, this->lefttorighttext,
                             this->toptobottomtext, this->textsize,
                             line.majorscale, line.bounds);

    const float majorstart = this->horizontaltext ?
      (line.bounds.isEmpty() ? 0.0f : line.bounds.getMin()[0]) :
      (line.bounds.isEmpty() ? 0.0f : line.bounds.getMin()[1]);
    const float majorend = this->horizontaltext ?
      (line.bounds.isEmpty() ? 0.0f : line.bounds.getMax()[0]) :
      (line.bounds.isEmpty() ? 0.0f : line.bounds.getMax()[1]);
    const float majorsign = this->horizontaltext ?
      (this->lefttorighttext ? 1.0f : -1.0f) :
      (this->toptobottomtext ? -1.0f : 1.0f);
    float majoroffset = 0.0f;
    switch (this->justificationmajor) {
    case FIRST:
    case BEGIN:
      majoroffset = majorsign > 0.0f ? -majorstart : -majorend;
      break;
    case MIDDLE:
      majoroffset = -(majorstart + majorend) * 0.5f;
      break;
    case END:
      majoroffset = majorsign > 0.0f ? -majorend : -majorstart;
      break;
    }
    if (this->horizontaltext) line.x = majoroffset;
    else line.y = majoroffset;

    const float minororigin = minorsign * static_cast<float>(i) * minorstep;
    if (this->horizontaltext) line.y = minororigin;
    else line.x = minororigin;

    float linemin = minororigin;
    float linemax = minororigin;
    if (!line.bounds.isEmpty()) {
      if (this->horizontaltext) {
        linemin += line.bounds.getMin()[1];
        linemax += line.bounds.getMax()[1];
      }
      else {
        linemin += line.bounds.getMin()[0];
        linemax += line.bounds.getMax()[0];
      }
    }
    if (firstbound) {
      blockmin = linemin;
      blockmax = linemax;
      firstbound = FALSE;
    }
    else {
      blockmin = SbMin(blockmin, linemin);
      blockmax = SbMax(blockmax, linemax);
    }
    if (i == 0) {
      firstlinemin = linemin;
      firstlinemax = linemax;
    }
    if (i == n - 1) {
      lastlinemin = linemin;
      lastlinemax = linemax;
    }
  }

  float minoroffset = 0.0f;
  switch (this->justificationminor) {
  case FIRST:
    break;
  case BEGIN:
    minoroffset = minorsign > 0.0f ? -firstlinemin : -firstlinemax;
    break;
  case MIDDLE:
    minoroffset = -(blockmin + blockmax) * 0.5f;
    break;
  case END:
    minoroffset = minorsign > 0.0f ? -lastlinemax : -lastlinemin;
    break;
  }
  for (int i = 0; i < n; i++) {
    if (this->horizontaltext) lines[i].y += minoroffset;
    else lines[i].x += minoroffset;
  }
}


/*!
  Destructor.
*/
SoVRMLText::~SoVRMLText()
{
  if (PRIVATE(this)->cache) PRIVATE(this)->cache->unref();

  delete PRIVATE(this)->fontstylesensor;
  delete PRIVATE(this);
}

// Doc in parent
void
SoVRMLText::GLRender(SoGLRenderAction * action)
{
  if (!this->shouldGLRender(action)) return;

  PRIVATE(this)->lock();
  SoState * state = action->getState();
  PRIVATE(this)->setUpGlyphs(state, this);
  SoCacheElement::addCacheDependency(state, PRIVATE(this)->cache);

  const cc_font_specification * fontspec = PRIVATE(this)->cache->getCachedFontspec();

  SoMaterialBundle mb(action);
  mb.sendFirst();

  SbBool do2Dtextures = FALSE;
  SbBool do3Dtextures = FALSE;
  if (SoGLMultiTextureEnabledElement::get(state)) {
    do2Dtextures = TRUE;
    if (SoGLMultiTextureEnabledElement::getMode(state) ==
        SoGLMultiTextureEnabledElement::TEXTURE3D) {
      do3Dtextures = TRUE;
    }
  }
  // FIXME: implement proper support for 3D-texturing, and get rid of
  // this. 20020120 mortene.
  if (do3Dtextures) {
    static SbBool first = TRUE;
    if (first) {
      first = FALSE;
      SoDebugError::postWarning("SoVRMLText::GLRender",
                                "3D-textures not properly supported for this node type yet.");
    }
  }

  int i;
  const int n = this->string.getNum();

  SbList<SoVRMLTextP::LineLayout> layout;
  PRIVATE(this)->buildLayout(fontspec, layout);

  glBegin(GL_TRIANGLES);
  glNormal3f(0.0f, 0.0f, 1.0f);
  
  for (i = 0; i < n; i++) {
    float xpos = layout[i].x;
    float ypos = layout[i].y;
    const float majorscale = layout[i].majorscale;
     
    cc_glyph3d * prevglyph = NULL;

    SbString str = this->string[i];
    const char * p = str.getString();
    size_t len = cc_string_utf8_validate_length(p);

    for (unsigned int strcharidx = 0; strcharidx < len; strcharidx++) {
      uint32_t glyphidx = 0;

      glyphidx = cc_string_utf8_get_char(p);
      p = cc_string_utf8_next_char(p);

      cc_glyph3d * glyph = cc_glyph3d_ref(glyphidx, fontspec);

      float advancex, advancey;
      cc_glyph3d_getadvance(glyph, &advancex, &advancey);

      const SbVec2f * coords = (SbVec2f *) cc_glyph3d_getcoords(glyph);
      const int * ptr = cc_glyph3d_getfaceindices(glyph);

      float kerningx = 0.0f;
      float kerningy = 0.0f;
      if (strcharidx > 0) {
        cc_glyph3d_getkerning(prevglyph, glyph, &kerningx, &kerningy);
      }
      if (PRIVATE(this)->horizontaltext) {
        if (PRIVATE(this)->lefttorighttext) {
          xpos += kerningx * PRIVATE(this)->textsize * majorscale;
        }
        else {
          xpos -= (advancex + kerningx) * PRIVATE(this)->textsize * majorscale;
        }
      }
      if (prevglyph) {
        cc_glyph3d_unref(prevglyph);
      }
      prevglyph = glyph;

      while (*ptr >= 0) {
        SbVec2f v0, v1, v2;
        v2 = coords[*ptr++];
        v1 = coords[*ptr++];
        v0 = coords[*ptr++];
        
     
        if (do2Dtextures) {
          glTexCoord2f(v0[0] * (PRIVATE(this)->horizontaltext ? majorscale : 1.0f) + xpos/PRIVATE(this)->textsize,
                       v0[1] * (PRIVATE(this)->horizontaltext ? 1.0f : majorscale) + ypos/PRIVATE(this)->textsize);
        }
        glVertex3f((v0[0] * PRIVATE(this)->textsize * (PRIVATE(this)->horizontaltext ? majorscale : 1.0f)) + xpos,
                   (v0[1] * PRIVATE(this)->textsize * (PRIVATE(this)->horizontaltext ? 1.0f : majorscale)) + ypos, 0.0f);

        if (do2Dtextures) {
          glTexCoord2f(v1[0] * (PRIVATE(this)->horizontaltext ? majorscale : 1.0f) + xpos/PRIVATE(this)->textsize,
                       v1[1] * (PRIVATE(this)->horizontaltext ? 1.0f : majorscale) + ypos/PRIVATE(this)->textsize);
        }
        glVertex3f(v1[0] * PRIVATE(this)->textsize * (PRIVATE(this)->horizontaltext ? majorscale : 1.0f) + xpos,
                   v1[1] * PRIVATE(this)->textsize * (PRIVATE(this)->horizontaltext ? 1.0f : majorscale) + ypos, 0.0f);

        if (do2Dtextures) {
          glTexCoord2f(v2[0] * (PRIVATE(this)->horizontaltext ? majorscale : 1.0f) + xpos/PRIVATE(this)->textsize,
                       v2[1] * (PRIVATE(this)->horizontaltext ? 1.0f : majorscale) + ypos/PRIVATE(this)->textsize);
        }
        glVertex3f(v2[0] * PRIVATE(this)->textsize * (PRIVATE(this)->horizontaltext ? majorscale : 1.0f) + xpos,
                   v2[1] * PRIVATE(this)->textsize * (PRIVATE(this)->horizontaltext ? 1.0f : majorscale) + ypos, 0.0f);

      }
      
      if (!PRIVATE(this)->horizontaltext) {
        ypos += (PRIVATE(this)->toptobottomtext ? -1.0f : 1.0f) *
          PRIVATE(this)->textsize * majorscale;
      } else if (PRIVATE(this)->lefttorighttext) {
        xpos += advancex * PRIVATE(this)->textsize * majorscale;
      }
    }

    if (prevglyph) {
      cc_glyph3d_unref(prevglyph);
      prevglyph = NULL;
    }
  }
  glEnd();
  PRIVATE(this)->unlock();

  if (SoComplexityTypeElement::get(state) == SoComplexityTypeElement::OBJECT_SPACE) {
    SoGLCacheContextElement::shouldAutoCache(state, SoGLCacheContextElement::DO_AUTO_CACHE);
    SoGLCacheContextElement::incNumShapes(state);
  }
}


// Doc in parent
void
SoVRMLText::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  if (action->is3DTextCountedAsTriangles()) {        
    PRIVATE(this)->lock();
    // can't regenerate the cache in this action traversal since SoCacheElement isn't enabled
    if (PRIVATE(this)->cache) {
      const cc_font_specification * fontspec = PRIVATE(this)->cache->getCachedFontspec();
      const int lines = this->string.getNum();
      int numtris = 0;      
      
      for (int i = 0;i < lines; ++i) {
	SbString str = this->string[i];
	const char * p = str.getString();
	size_t len = cc_string_utf8_validate_length(p);

        for (unsigned int strcharidx = 0; strcharidx < len; strcharidx++) {
	  uint32_t glyphidx = 0;

	  glyphidx = cc_string_utf8_get_char(p);
	  p = cc_string_utf8_next_char(p);

          cc_glyph3d * glyph = cc_glyph3d_ref(glyphidx, fontspec);
          
          int cnt = 0;
          const int * ptr = cc_glyph3d_getfaceindices(glyph);
          while (*ptr++ >= 0) 
            cnt++;
          
          numtris += cnt / 3;
          
          cc_glyph3d_unref(glyph);
        }
      }
      action->addNumTriangles(numtris);
    }
    PRIVATE(this)->unlock();
  }
  else {
    action->addNumText(this->string.getNum());
  }

}


// Doc in parent
void
SoVRMLText::notify(SoNotList * list)
{
  PRIVATE(this)->lock();
  if (PRIVATE(this)->cache) {
    SoField * f = list->getLastField();
    if (f == &this->string) PRIVATE(this)->cache->invalidate();
  }
  PRIVATE(this)->unlock();
  inherited::notify(list);
}

// Doc in parent
SoChildList *
SoVRMLText::getChildren(void) const
{
  return NULL;
}


// Doc in parent
void
SoVRMLText::computeBBox(SoAction * action,
                        SbBox3f & box,
                        SbVec3f & center)
{
  PRIVATE(this)->lock();

  PRIVATE(this)->setUpGlyphs(action->getState(), this);
  SoCacheElement::addCacheDependency(action->getState(), PRIVATE(this)->cache);

  {
    const cc_font_specification * fontspec =
      PRIVATE(this)->cache->getCachedFontspec();
    SbList<SoVRMLTextP::LineLayout> layout;
    PRIVATE(this)->buildLayout(fontspec, layout);
    box.makeEmpty();
    for (int lineidx = 0; lineidx < layout.getLength(); lineidx++) {
      if (!layout[lineidx].bounds.isEmpty()) {
        box.extendBy(SbVec3f(layout[lineidx].bounds.getMin()[0] +
                             layout[lineidx].x,
                             layout[lineidx].bounds.getMin()[1] +
                             layout[lineidx].y, 0.0f));
        box.extendBy(SbVec3f(layout[lineidx].bounds.getMax()[0] +
                             layout[lineidx].x,
                             layout[lineidx].bounds.getMax()[1] +
                             layout[lineidx].y, 0.0f));
      }
    }
    center = box.isEmpty() ? SbVec3f(0.0f, 0.0f, 0.0f) : box.getCenter();
    PRIVATE(this)->unlock();
  }
}

// Doc in parent
void
SoVRMLText::generatePrimitives(SoAction * action)
{
  PRIVATE(this)->lock();

  PRIVATE(this)->setUpGlyphs(action->getState(), this);
  const cc_font_specification * fontspec = PRIVATE(this)->cache->getCachedFontspec();

  int i, n = this->string.getNum();
  SbList<SoVRMLTextP::LineLayout> layout;
  PRIVATE(this)->buildLayout(fontspec, layout);

  SbBool do2Dtextures = FALSE;
  SbBool do3Dtextures = FALSE;
  if (SoMultiTextureEnabledElement::get(action->getState())) do2Dtextures = TRUE;
  
  // FIXME: implement proper support for 3D-texturing, and get rid of
  // this. 20020120 mortene.
  if (do3Dtextures) {
    static SbBool first = TRUE;
    if (first) {
      first = FALSE;
      SoDebugError::postWarning("SoVRMLText::generatePrimitives",
                                "3D-textures not properly supported for this node type yet.");
    }
  }


  SoPrimitiveVertex vertex;
  SoTextDetail detail;
  detail.setPart(0);
  vertex.setDetail(&detail);
  vertex.setMaterialIndex(0);

  this->beginShape(action, SoShape::TRIANGLES, NULL);
  vertex.setNormal(SbVec3f(0.0f, 0.0f, 1.0f));

  for (i = 0; i < n; i++) {
    detail.setStringIndex(i);
    float xpos = layout[i].x;
    float ypos = layout[i].y;
    const float majorscale = layout[i].majorscale;
    
    SbString str = this->string[i];
    cc_glyph3d * prevglyph = NULL;
    const char * p = str.getString();
    size_t len = cc_string_utf8_validate_length(p);

    for (unsigned int strcharidx = 0; strcharidx < len; strcharidx++) {
      uint32_t glyphidx = 0;

      glyphidx = cc_string_utf8_get_char(p);
      p = cc_string_utf8_next_char(p);

      cc_glyph3d * glyph = cc_glyph3d_ref(glyphidx, fontspec);

      float advancex, advancey;
      cc_glyph3d_getadvance(glyph, &advancex, &advancey);

      const SbVec2f * coords = (SbVec2f *) cc_glyph3d_getcoords(glyph);
      const int * ptr = cc_glyph3d_getfaceindices(glyph);

      detail.setCharacterIndex(strcharidx);

      float kerningx = 0.0f;
      float kerningy = 0.0f;
      if (strcharidx > 0) {
        cc_glyph3d_getkerning(prevglyph, glyph, &kerningx, &kerningy);
      }
      if (PRIVATE(this)->horizontaltext) {
        if (PRIVATE(this)->lefttorighttext) {
          xpos += kerningx * PRIVATE(this)->textsize * majorscale;
        }
        else {
          xpos -= (advancex + kerningx) * PRIVATE(this)->textsize * majorscale;
        }
      }
      if (prevglyph) {
        cc_glyph3d_unref(prevglyph);
      }
      prevglyph = glyph;

      while (*ptr >= 0) {
        SbVec2f v0, v1, v2;
        v2 = coords[*ptr++];
        v1 = coords[*ptr++];
        v0 = coords[*ptr++];

        if (do2Dtextures) {
          vertex.setTextureCoords(SbVec2f(v0[0] * (PRIVATE(this)->horizontaltext ? majorscale : 1.0f) + xpos/PRIVATE(this)->textsize,
                                          v0[1] * (PRIVATE(this)->horizontaltext ? 1.0f : majorscale) + ypos/PRIVATE(this)->textsize));
        }
        vertex.setPoint(SbVec3f(v0[0] * PRIVATE(this)->textsize * (PRIVATE(this)->horizontaltext ? majorscale : 1.0f) + xpos,
                                v0[1] * PRIVATE(this)->textsize * (PRIVATE(this)->horizontaltext ? 1.0f : majorscale) + ypos, 0.0f));
        this->shapeVertex(&vertex);

        if (do2Dtextures) {
          vertex.setTextureCoords(SbVec2f(v1[0] * (PRIVATE(this)->horizontaltext ? majorscale : 1.0f) + xpos/PRIVATE(this)->textsize,
                                          v1[1] * (PRIVATE(this)->horizontaltext ? 1.0f : majorscale) + ypos/PRIVATE(this)->textsize));
        }
        vertex.setPoint(SbVec3f(v1[0] * PRIVATE(this)->textsize * (PRIVATE(this)->horizontaltext ? majorscale : 1.0f) + xpos,
                                v1[1] * PRIVATE(this)->textsize * (PRIVATE(this)->horizontaltext ? 1.0f : majorscale) + ypos, 0.0f));
        this->shapeVertex(&vertex);

        if (do2Dtextures) {
          vertex.setTextureCoords(SbVec2f(v2[0] * (PRIVATE(this)->horizontaltext ? majorscale : 1.0f) + xpos/PRIVATE(this)->textsize,
                                          v2[1] * (PRIVATE(this)->horizontaltext ? 1.0f : majorscale) + ypos/PRIVATE(this)->textsize));
        }
        vertex.setPoint(SbVec3f(v2[0] * PRIVATE(this)->textsize * (PRIVATE(this)->horizontaltext ? majorscale : 1.0f) + xpos,
                                v2[1] * PRIVATE(this)->textsize * (PRIVATE(this)->horizontaltext ? 1.0f : majorscale) + ypos, 0.0f));
        this->shapeVertex(&vertex);
      }


      if (!PRIVATE(this)->horizontaltext) {       
        ypos += (PRIVATE(this)->toptobottomtext ? -1.0f : 1.0f) *
          PRIVATE(this)->textsize * majorscale;
      }
      else if (PRIVATE(this)->lefttorighttext) {
        xpos += advancex * PRIVATE(this)->textsize * majorscale;
      }
    }
    if (prevglyph) {
      cc_glyph3d_unref(prevglyph);
      prevglyph = NULL;
    }
  }

  this->endShape();
  PRIVATE(this)->unlock();
}

void
SoVRMLTextP::updateFontStyle(void)
{
  // the defaults
  this->textsize = 1.0f;
  this->textspacing = 1.0f;
  this->lefttorighttext = TRUE;
  this->toptobottomtext = TRUE;
  this->horizontaltext = TRUE;
  this->justificationmajor = BEGIN;
  this->justificationminor = FIRST;
  this->fontfamily = SoVRMLFontStyle::SERIF;
  this->fontstyle = SoVRMLFontStyle::PLAIN;

  SoVRMLFontStyle * fs = (SoVRMLFontStyle *)PUBLIC(this)->fontStyle.getValue();
  if (!fs) { return; }

  if (fs->justify.getNum() > 0) {
    // Major mode
    SbString s = fs->justify[0];
    if (s == "FIRST") {
      this->justificationmajor = FIRST;
    }
    else if ((s == "BEGIN") || (s == "")) {
      this->justificationmajor = BEGIN;
    } 
    else if (s == "MIDDLE") {
      this->justificationmajor = MIDDLE;
    } 
    else if (s == "END") {
      this->justificationmajor = END;
    }
    // FIXME: else... Improve robustness & error reporting. 20051207 mortene.
  }
    
  if (fs->justify.getNum() > 1) {
    // Minor mode
    SbString s = fs->justify[1];
    if ((s == "FIRST") || (s == ""))
      this->justificationminor = FIRST;
    else if (s == "BEGIN")
      this->justificationminor = BEGIN;
    else if (s == "MIDDLE")
      this->justificationminor = MIDDLE;
    else if (s == "END")
      this->justificationminor = END;
    // FIXME: else... Improve robustness & error reporting. 20051207 mortene.
  }
  
  this->lefttorighttext = fs->leftToRight.getValue();
  this->toptobottomtext = fs->topToBottom.getValue();
  this->horizontaltext = fs->horizontal.getValue();
  this->textsize = fs->size.getValue();
  this->textspacing = fs->spacing.getValue();

  this->fontfamily = SoVRMLFontStyle::SERIF;
  this->fontstyle = SoVRMLFontStyle::PLAIN;

  const char * family = fs->family[0].getString();
  if (family && family[0] != '\0') {
    if (!strcmp(family, "SERIF"))
      this->fontfamily = SoVRMLFontStyle::SERIF;
    else if (!strcmp(family, "SANS"))
      this->fontfamily = SoVRMLFontStyle::SANS;
    else if (!strcmp(family, "TYPEWRITER"))
      this->fontfamily = SoVRMLFontStyle::TYPEWRITER;
  }
      
  const char * style = fs->style[0].getString();
  if (style && style[0] != '\0') {
    if (!strcmp(style, "PLAIN"))
      this->fontstyle = SoVRMLFontStyle::PLAIN;
    else if (!strcmp(style, "BOLD"))
      this->fontstyle = SoVRMLFontStyle::BOLD;
    else if (!strcmp(style, "ITALIC"))
      this->fontstyle = SoVRMLFontStyle::ITALIC;
    else if (!strcmp(style, "BOLDITALIC"))
      this->fontstyle = SoVRMLFontStyle::BOLDITALIC;
  }
}

// recalculate glyphs
void
SoVRMLTextP::setUpGlyphs(SoState * state, SoVRMLText * textnode)
{
  if (this->cache && this->cache->isValid(state)) return;
  
  this->updateFontStyle();

  SoGlyphCache * oldcache = this->cache;

  state->push();
  SbBool storedinvalid = SoCacheElement::setInvalid(FALSE);
  this->cache = new SoGlyphCache(state); 
  this->cache->ref();
  SoCacheElement::set(state, this->cache);
  this->cache->readFontspec(state);
  const cc_font_specification * fontspec = this->cache->getCachedFontspec(); 

  SbString fontstr;
  switch (this->fontfamily) {
  case SoVRMLFontStyle::PLAIN: fontstr = "Times New Roman"; break;
  case SoVRMLFontStyle::SANS: fontstr = "Arial"; break;
  case SoVRMLFontStyle::TYPEWRITER: fontstr = "Courier New"; break;
  default: /* FIXME: warn on faulty input data. 20030921 mortene. */ fontstr = "defaultFont"; break;
  }

  switch (this->fontstyle) {
  case SoVRMLFontStyle::BOLD: fontstr += ":Bold"; break;
  case SoVRMLFontStyle::ITALIC: fontstr += ":Italic"; break;
  case  SoVRMLFontStyle::BOLDITALIC: fontstr += ":Bold Italic"; break;
  default: /* FIXME: check for and warn on faulty input data. 20030921 mortene. */ break;
  }

  this->glyphwidths.truncate(0);

  for (int i = 0; i < textnode->string.getNum(); i++) {
    SbString str = textnode->string[i];
    const char * p = str.getString();
    size_t len = cc_string_utf8_validate_length(p);

    float stringwidth = 0.0f;
    const float * maxbbox;
    float kerningx = 0;
    float kerningy = 0;
    float advancex = 0;
    float advancey = 0;
    cc_glyph3d * prevglyph = NULL;
    this->maxglyphbbox.makeEmpty();

    for (unsigned int strcharidx = 0; strcharidx < len; strcharidx++) {
      uint32_t glyphidx = 0;

      glyphidx = cc_string_utf8_get_char(p);
      p = cc_string_utf8_next_char(p);

      cc_glyph3d * glyph = cc_glyph3d_ref(glyphidx, fontspec);
      assert(glyph);
      this->cache->addGlyph(glyph);

      maxbbox = cc_glyph3d_getboundingbox(glyph); // Get max height
      this->maxglyphbbox.extendBy(SbVec3f(0, maxbbox[0] * fontspec->size, 0));
      this->maxglyphbbox.extendBy(SbVec3f(0, maxbbox[1] * fontspec->size, 0));

      if (strcharidx > 0) 
        cc_glyph3d_getkerning(prevglyph, glyph, &kerningx, &kerningy);          
      cc_glyph3d_getadvance(glyph, &advancex, &advancey);
  
      stringwidth += (advancex + kerningx);
      prevglyph = glyph;
          
    }

    if (prevglyph != NULL) {
      // Italic font might cause last letter to be outside bbox. Add width if needed.
      if (advancex < cc_glyph3d_getwidth(prevglyph)) 
        stringwidth += (cc_glyph3d_getwidth(prevglyph) - advancex);
    }      

    this->glyphwidths.append(stringwidth);
  }
  state->pop();
  SoCacheElement::setInvalid(storedinvalid);

  // unref old cache after creating the new one to avoid recreating glyphs
  if (oldcache) oldcache->unref();
}

#undef PRIVATE
#undef PUBLIC

#endif // HAVE_VRML97
