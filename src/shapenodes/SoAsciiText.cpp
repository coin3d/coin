/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2004 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

/*!
  \class SoAsciiText SoAsciiText.h Inventor/nodes/SoAsciiText.h
  \brief The SoAsciiText class renders flat 3D text.
  \ingroup nodes

  The text is rendered using 3D polygon geometry.

  The complexity of the glyphs is controlled by a preceding
  SoComplexity node with \e Type set to OBJECT_SPACE. Please note
  that the default builtin 3D font will not be affected by the
  SoComplexity node.

  This node is different from the SoText2 node in that it rotates,
  scales, translates etc just like other geometry in the scene. It is
  different from the SoText3 node in that it renders the text "flat",
  i.e. does not extrude the fonts to have depth.

  To get an intuitive feeling for how SoAsciiText works, take a look
  at this sample Inventor file in examinerviewer:

  \verbatim
  #Inventor V2.1 ascii

  Separator {
    Font {
      size 10
      name "Arial:Bold Italic"
    }

    BaseColor {
      rgb 1 0 0 #red
    }
    AsciiText {
      width [ 0, 3, 20 ]
      justification LEFT #Standard alignment
      string [ "LEFT", "LEFT", "LEFT", "LEFT", "LEFT LEFT" ]
    }
    BaseColor { 
      rgb 1 1 0
    }
    Sphere { radius 1.5 }

    Translation {
      translation 0 -50 0
    }
    BaseColor {
      rgb 0 1 0 #green
    }
    AsciiText {
      width [ 0, 3, 20 ]
      justification RIGHT
      string [ "RIGHT", "RIGHT", "RIGHT", "RIGHT", "RIGHT RIGHT" ]
    }
    BaseColor { 
      rgb 0 1 1
    }
    Sphere { radius 1.5 }

    Translation {
      translation 0 -50 0
    }
    BaseColor {
      rgb 0 0 1 #blue
    }
    AsciiText {
      width [ 0, 3, 20 ]
      justification CENTER
      string [ "CENTER", "CENTER", "CENTER", "CENTER", "CENTER CENTER" ]
    }
    BaseColor { 
      rgb 1 0 1
    }
    Sphere { radius 1.5 }
  }
  \endverbatim

  In examinerviewer the Inventor file looks something like this:

  <center>
    <img src="http://doc.coin3d.org/images/Coin/nodes/asciitext.png">
  </center>

  \since Inventor 2.1
*/

// FIXME: Write doc about how text is textured. jornskaa 20040716

#include <Inventor/nodes/SoAsciiText.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/bundles/SoMaterialBundle.h>
#include <Inventor/details/SoTextDetail.h>
#include <Inventor/elements/SoFontNameElement.h>
#include <Inventor/elements/SoFontSizeElement.h>
#include <Inventor/elements/SoGLTextureEnabledElement.h>
#include <Inventor/elements/SoGLTexture3EnabledElement.h>
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/elements/SoComplexityTypeElement.h>
#include <Inventor/elements/SoComplexityElement.h>
#include <Inventor/elements/SoCacheElement.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/caches/SoGlyphCache.h>
#include <string.h>
#include <float.h> // FLT_MIN

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H
#include <Inventor/system/gl.h>

#ifdef COIN_THREADSAFE
#include <Inventor/threads/SbMutex.h>
#endif // COIN_THREADSAFE

#include "../fonts/glyph3d.h"


/*!  \enum SoAsciiText::Justification
  The font justification values control the text
  alignment. Justification can have three distinct values.  The
  default value is SoAsciiText::LEFT, and the strings are rendered
  with a common left border. The second value is SoAsciiText::RIGHT,
  and renders the strings with a common right border. The last value
  is SoAsciiText::CENTER, in which the strings are rendered with their
  centers aligned. The origo of the three alignments are respectively
  left, right and center, located at the baseline of the first line of
  text.
*/

/*!  \var SoAsciiText::Justification SoAsciiText::LEFT
  The strings are left-aligned; rendered with a common left
  border. This is the default alignment.
*/

/*!  \var SoAsciiText::Justification SoAsciiText::RIGHT
  The strings are right-aligned; rendered with a common right border.
*/

/*!  \var SoAsciiText::Justification SoAsciiText::CENTER
  The text is center-aligned; all strings are centered.
*/

/*!  \var SoMFString SoAsciiText::string 
  Lines of text to render. Several strings can be specified for this
  multifield, where each string represents a line. Default value is
  empty.
*/
/*!
  \var SoSFFloat SoAsciiText::spacing
  Spacing between each line. Defaults to 1.0.
*/
/*!
  \var SoSFEnum SoAsciiText::justification
  Horizontal alignment. Default SoAsciiText::LEFT.
*/

/*!  \var SoMFFloat SoAsciiText::width
  Defines the width of each line. The text is scaled to be within the
  specified width. The size of the text will remain the same; only the
  advance in the x-direction is scaled. Width equal to the number of
  characters in your text will not alter the text at all. When a width
  is set to 0, the width field is ignored and the text rendered as
  normal. The exact width of the rendered text depends in every case
  on the font being used. If fewer widths are specified than the
  number of strings, the strings without matching widths are rendered
  with default width.
*/

#ifndef DOXYGEN_SKIP_THIS

class SoAsciiTextP {
public:

  SoAsciiTextP(SoAsciiText * master) : master(master) { }
  SoAsciiText * master;

  void setUpGlyphs(SoState * state, SoAsciiText * textnode);
  
  SbList <float> glyphwidths;
  SbList <float> stringwidths;
  SbBox3f maxglyphbbox;

  SoGlyphCache * cache;

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
  SbMutex mutex;
#endif // COIN_THREADSAFE
};

#endif // DOXYGEN_SKIP_THIS

#define PRIVATE(p) ((p)->pimpl)

// *************************************************************************

SO_NODE_SOURCE(SoAsciiText);

/*!
  Constructor.
*/
SoAsciiText::SoAsciiText(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoAsciiText);

  SO_NODE_ADD_FIELD(string, (""));
  SO_NODE_ADD_FIELD(spacing, (1.0f));
  SO_NODE_ADD_FIELD(justification, (SoAsciiText::LEFT));
  SO_NODE_ADD_FIELD(width, (0.0f));

  SO_NODE_DEFINE_ENUM_VALUE(Justification, LEFT);
  SO_NODE_DEFINE_ENUM_VALUE(Justification, RIGHT);
  SO_NODE_DEFINE_ENUM_VALUE(Justification, CENTER);
  SO_NODE_SET_SF_ENUM_TYPE(justification, Justification);

  PRIVATE(this) = new SoAsciiTextP(this);
  PRIVATE(this)->cache = NULL;
}

/*!
  Destructor.
*/
SoAsciiText::~SoAsciiText()
{
  if (PRIVATE(this)->cache) PRIVATE(this)->cache->unref();
  delete PRIVATE(this);
}

// Doc in parent.
void
SoAsciiText::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoAsciiText, SO_FROM_INVENTOR_2_1|SoNode::VRML1);
}

// Doc in parent.
void
SoAsciiText::GLRender(SoGLRenderAction * action)
{  
  if (!this->shouldGLRender(action)) 
    return;

  PRIVATE(this)->lock();
  SoState * state = action->getState();
  PRIVATE(this)->setUpGlyphs(state, this);
  SoCacheElement::addCacheDependency(state, PRIVATE(this)->cache);

  const cc_font_specification * fontspec = PRIVATE(this)->cache->getCachedFontspec(); 

  SbBool do2Dtextures = FALSE;
  SbBool do3Dtextures = FALSE;
  if (SoGLTextureEnabledElement::get(state)) do2Dtextures = TRUE;
  else if (SoGLTexture3EnabledElement::get(state)) do3Dtextures = TRUE;

  // FIXME: implement proper support for 3D-texturing, and get rid of
  // this. 20020120 mortene.
  if (do3Dtextures) {
    static SbBool first = TRUE;
    if (first) {
      first = FALSE;
      SoDebugError::postWarning("SoAsciiText::GLRender",
                                "3D-textures not properly supported for this node type yet.");
    }
  }

  SoMaterialBundle mb(action);
  mb.sendFirst();
  
  glBegin(GL_TRIANGLES);
  glNormal3f(0.0f, 0.0f, 1.0f);

  int glyphidx = 0;
  float ypos = 0.0f;

  int i, n = this->string.getNum();
  for (i = 0; i < n; i++) {

    float stretchfactor = 1.0f;
    if (i < this->width.getNum() && this->width[i] != 0) {
      stretchfactor = this->width[i] / strlen(this->string[i].getString());
    }

    float xpos = 0.0f;
    const float currwidth = PRIVATE(this)->stringwidths[i];
    switch (this->justification.getValue()) {
    case SoAsciiText::RIGHT:
      xpos = -currwidth;
      break;
    case SoAsciiText::CENTER:
      xpos = -currwidth * 0.5f;
      break;
    }

    cc_glyph3d * prevglyph = NULL;
    const unsigned int length = this->string[i].getLength();
    for (unsigned int strcharidx = 0; strcharidx < length; strcharidx++) {

      // Note that the "unsigned char" cast is needed to avoid 8-bit
      // chars using the highest bit (i.e. characters above the ASCII
      // set up to 127) be expanded to huge int numbers that turn
      // negative when casted to integer size.
      const uint32_t glyphidx = (const unsigned char) this->string[i][strcharidx];
      cc_glyph3d * glyph = cc_glyph3d_ref(glyphidx, fontspec);

      // Get kerning
      if (strcharidx > 0) {
        float kerningx, kerningy;
        cc_glyph3d_getkerning(prevglyph, glyph, &kerningx, &kerningy);
        xpos += kerningx * fontspec->size;
      }

      if (prevglyph) {
        cc_glyph3d_unref(prevglyph);
      }
      prevglyph = glyph;

      const SbVec2f * coords = (SbVec2f *) cc_glyph3d_getcoords(glyph);
      const int * ptr = cc_glyph3d_getfaceindices(glyph);

      while (*ptr >= 0) {
        SbVec2f v0, v1, v2;
        v2 = coords[*ptr++];
        v1 = coords[*ptr++];
        v0 = coords[*ptr++];

        // FIXME: Is the text textured correctly when stretching is
        // applied (when width values have been given that are
        // not the same as the length of the string)? jornskaa 20040716
        if (do2Dtextures) {
          glTexCoord2f(v0[0] + xpos/fontspec->size, v0[1] + ypos/fontspec->size);
        }
        glVertex3f(v0[0] * fontspec->size + xpos, v0[1] * fontspec->size + ypos, 0.0f);
        
        if (do2Dtextures) {
          glTexCoord2f(v1[0] + xpos/fontspec->size, v1[1] + ypos/fontspec->size);
        }
        glVertex3f(v1[0] * fontspec->size + xpos, v1[1] * fontspec->size + ypos, 0.0f);
        
        if (do2Dtextures) {
          glTexCoord2f(v2[0] + xpos/fontspec->size, v2[1] + ypos/fontspec->size);
        }
        glVertex3f(v2[0] * fontspec->size + xpos, v2[1] * fontspec->size + ypos, 0.0f);

      }

      float advancex, advancey;
      cc_glyph3d_getadvance(glyph, &advancex, &advancey);
      xpos += (advancex * stretchfactor * fontspec->size);
    }
    if (prevglyph) {
      cc_glyph3d_unref(prevglyph);
      prevglyph = NULL;
    }

    ypos -= fontspec->size * this->spacing.getValue();
  }
  glEnd();

  PRIVATE(this)->unlock();

  if (SoComplexityTypeElement::get(state) == SoComplexityTypeElement::OBJECT_SPACE) 
    SoGLCacheContextElement::shouldAutoCache(state, SoGLCacheContextElement::DO_AUTO_CACHE);
  
}

// Doc in parent.
void
SoAsciiText::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  if (action->is3DTextCountedAsTriangles()) {        
    PRIVATE(this)->lock();
    SoState * state = action->getState();
    PRIVATE(this)->setUpGlyphs(state, this);

    const cc_font_specification * fontspec = PRIVATE(this)->cache->getCachedFontspec();

    const int lines = this->string.getNum();
    int numtris = 0;      
    for (int i = 0;i < lines; ++i) {

      const int length = this->string[i].getLength();
      for (int strcharidx = 0;strcharidx < length; strcharidx++) {
        
        // Note that the "unsigned char" cast is needed to avoid 8-bit
        // chars using the highest bit (i.e. characters above the ASCII
        // set up to 127) be expanded to huge int numbers that turn
        // negative when casted to integer size.        
        const uint32_t glyphidx = (const unsigned char) this->string[i][strcharidx];
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
    PRIVATE(this)->unlock();
  }
  else {
    action->addNumText(this->string.getNum());
  }

}

// Doc in parent.
void
SoAsciiText::computeBBox(SoAction * action, SbBox3f & box, SbVec3f & center)
{
  PRIVATE(this)->lock();
  SoState * state = action->getState();

  PRIVATE(this)->setUpGlyphs(state, this);
  SoCacheElement::addCacheDependency(state, PRIVATE(this)->cache);

  const cc_font_specification * fontspec = PRIVATE(this)->cache->getCachedFontspec();

  int i;
  float maxw = FLT_MIN;
  for (i = 0;i<PRIVATE(this)->stringwidths.getLength();++i)
    maxw = SbMax(maxw, PRIVATE(this)->stringwidths[i]);
  
  if (maxw == FLT_MIN) { // There is no text to bound. Returning.
    PRIVATE(this)->unlock();
    return; 
  }

  float maxy, miny;
  float minx, maxx;
  
  minx = 0;
  maxx = maxw;

  miny = -fontspec->size * this->spacing.getValue() * (this->string.getNum() - 1);
  maxy = fontspec->size;

  switch (this->justification.getValue()) {
  case SoAsciiText::LEFT:
    break;
  case SoAsciiText::RIGHT:
    maxx -= maxw;
    minx -= maxw;
    break;
  case SoAsciiText::CENTER:
    maxx -= maxw * 0.5f;
    minx -= maxw * 0.5f;
    break;
  default:
    assert(0 && "Unknown justification");
    minx = maxx = 0.0f;
    break;
  }

  box.setBounds(SbVec3f(minx, miny, 0.0f), SbVec3f(maxx, maxy, 0.0f));

  // maxglyphbbox should never be empty here.
  assert(!PRIVATE(this)->maxglyphbbox.isEmpty());

  // Expanding bbox so that glyphs like 'j's and 'q's are completely inside.
  box.extendBy(SbVec3f(0, PRIVATE(this)->maxglyphbbox.getMin()[1] - (this->string.getNum() - 1) * fontspec->size, 0));  
  box.extendBy(PRIVATE(this)->maxglyphbbox);
  center = box.getCenter();

  PRIVATE(this)->unlock();
}

// Doc in parent.
void
SoAsciiText::generatePrimitives(SoAction * action)
{
  PRIVATE(this)->lock();
  SoState * state = action->getState();
  PRIVATE(this)->setUpGlyphs(state, this);

  const cc_font_specification * fontspec = PRIVATE(this)->cache->getCachedFontspec();

  SbBool do2Dtextures = FALSE;
  SbBool do3Dtextures = FALSE;
  if (SoGLTextureEnabledElement::get(state)) do2Dtextures = TRUE;
  else if (SoGLTexture3EnabledElement::get(state)) do3Dtextures = TRUE;

  // FIXME: implement proper support for 3D-texturing, and get rid of
  // this. 20020120 mortene.
  if (do3Dtextures) {
    static SbBool first = TRUE;
    if (first) {
      first = FALSE;
      SoDebugError::postWarning("SoAsciiText::generatePrimitives",
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

  int glyphidx = 0;
  float ypos = 0.0f;

  int i, n = this->string.getNum();
  for (i = 0; i < n; i++) {

    float stretchfactor = 1.0f;
    if (i < this->width.getNum() && this->width[i] != 0) {
      stretchfactor = this->width[i] / strlen(this->string[i].getString());
    }

    detail.setStringIndex(i);
    float xpos = 0.0f;
    const float currwidth = PRIVATE(this)->stringwidths[i];
    switch (this->justification.getValue()) {
    case SoAsciiText::RIGHT:
      xpos = -currwidth;
      break;
    case SoAsciiText::CENTER:
      xpos = - currwidth * 0.5f;
      break;
    }


    cc_glyph3d * prevglyph = NULL;
    const unsigned int length = this->string[i].getLength();
    for (unsigned int strcharidx = 0; strcharidx < length; strcharidx++) {
      
      // Note that the "unsigned char" cast is needed to avoid 8-bit
      // chars using the highest bit (i.e. characters above the ASCII
      // set up to 127) be expanded to huge int numbers that turn
      // negative when casted to integer size.
      const uint32_t glyphidx = (const unsigned char) this->string[i][strcharidx];
      cc_glyph3d * glyph = cc_glyph3d_ref(glyphidx, fontspec);
      
      // Get kerning
      if (strcharidx > 0) {
        float kerningx, kerningy;
        cc_glyph3d_getkerning(prevglyph, glyph, &kerningx, &kerningy);
        xpos += kerningx * fontspec->size;
      }
      if (prevglyph) {
        cc_glyph3d_unref(prevglyph);
      }
      prevglyph = glyph;
      detail.setCharacterIndex(strcharidx);

      const SbVec2f * coords = (SbVec2f *) cc_glyph3d_getcoords(glyph);
      const int * ptr = cc_glyph3d_getfaceindices(glyph);

      while (*ptr >= 0) {
        SbVec2f v0, v1, v2;
        v2 = coords[*ptr++];
        v1 = coords[*ptr++];
        v0 = coords[*ptr++];

        // FIXME: Is the text textured correctly when stretching is
        // applied (when width values have been given that are
        // not the same as the length of the string)? jornskaa 20040716
        if(do2Dtextures) {
          vertex.setTextureCoords(SbVec2f(v0[0] + xpos/fontspec->size, v0[1] + ypos/fontspec->size));
        }
        vertex.setPoint(SbVec3f(v0[0] * fontspec->size + xpos, v0[1] * fontspec->size + ypos, 0.0f));
        this->shapeVertex(&vertex);

        if(do2Dtextures) {
          vertex.setTextureCoords(SbVec2f(v1[0] + xpos/fontspec->size, v1[1] + ypos/fontspec->size));
        }
        vertex.setPoint(SbVec3f(v1[0] * fontspec->size + xpos, v1[1] * fontspec->size + ypos, 0.0f));
        this->shapeVertex(&vertex);

        if(do2Dtextures) {
          vertex.setTextureCoords(SbVec2f(v2[0] + xpos/fontspec->size, v2[1] + ypos/fontspec->size));
        }
        vertex.setPoint(SbVec3f(v2[0] * fontspec->size + xpos, v2[1] * fontspec->size + ypos, 0.0f));
        this->shapeVertex(&vertex);
      }
      float advancex, advancey;
      cc_glyph3d_getadvance(glyph, &advancex, &advancey);
      xpos += (advancex * stretchfactor * fontspec->size);
    }
    ypos -= fontspec->size * this->spacing.getValue();
    if (prevglyph) {
      cc_glyph3d_unref(prevglyph);
      prevglyph = NULL;
    }
  }
  this->endShape();
  PRIVATE(this)->unlock();
}

// doc in parent
SoDetail *
SoAsciiText::createTriangleDetail(SoRayPickAction * action,
                              const SoPrimitiveVertex * v1,
                              const SoPrimitiveVertex * v2,
                              const SoPrimitiveVertex * v3,
                              SoPickedPoint * pp)
{
  // generatePrimitives() places text details inside each primitive vertex
  assert(v1->getDetail());
  return v1->getDetail()->copy();
}


void 
SoAsciiText::notify(SoNotList * list)
{
  PRIVATE(this)->lock();
  if (PRIVATE(this)->cache) {
    SoField * f = list->getLastField();
    if (f == &this->string) {
      PRIVATE(this)->cache->invalidate();
    }
  }
  PRIVATE(this)->unlock();
  inherited::notify(list);
}

// returns "normalized" width of specified string. When too few
// width values are supplied, the glyphwidths are used instead.
float
SoAsciiText::getWidth(const int idx, const float fontsize)
{
  if (idx < this->width.getNum() && this->width[idx] > 0.0f)
    return this->width[idx] / fontsize;
  return PRIVATE(this)->glyphwidths[idx];
}

// SoAsciiTextP methods implemented below


#ifndef DOXYGEN_SKIP_THIS

// recalculate glyphs
void
SoAsciiTextP::setUpGlyphs(SoState * state, SoAsciiText * textnode)
{
  if (this->cache && this->cache->isValid(state)) return;
  SoGlyphCache * oldcache = this->cache;
  
  state->push();
  SbBool storedinvalid = SoCacheElement::setInvalid(FALSE);
  this->cache = new SoGlyphCache(state); 
  this->cache->ref();
  SoCacheElement::set(state, this->cache);
  this->cache->readFontspec(state);
  const cc_font_specification * fontspecptr = this->cache->getCachedFontspec(); 

  this->glyphwidths.truncate(0);
  this->stringwidths.truncate(0);
  this->maxglyphbbox.makeEmpty();

  float kerningx = 0;
  float kerningy = 0;
  float advancex = 0;
  float advancey = 0;
  cc_glyph3d * prevglyph = NULL;

  for (int i = 0; i < textnode->string.getNum(); i++) {
    const unsigned int length = textnode->string[i].getLength();
    float stringwidth = 0.0f;
    const float * maxbbox;

    float stretchfactor = 1.0f;
    if (i < this->master->width.getNum() && this->master->width[i] != 0) { 
      stretchfactor = this->master->width[i] / strlen(this->master->string[i].getString());
    }

    for (unsigned int strcharidx = 0; strcharidx < length; strcharidx++) {

      // Note that the "unsigned char" cast is needed to avoid 8-bit
      // chars using the highest bit (i.e. characters above the ASCII
      // set up to 127) be expanded to huge int numbers that turn
      // negative when casted to integer size.
      const uint32_t glyphidx = (const unsigned char) textnode->string[i][strcharidx];
      cc_glyph3d * glyph = cc_glyph3d_ref(glyphidx, fontspecptr);
      this->cache->addGlyph(glyph);
      assert(glyph);

      maxbbox = cc_glyph3d_getboundingbox(glyph); // Get max height
      this->maxglyphbbox.extendBy(SbVec3f(0, maxbbox[0] * fontspecptr->size, 0));
      this->maxglyphbbox.extendBy(SbVec3f(0, maxbbox[1] * fontspecptr->size, 0));

      // FIXME: Shouldn't it be the 'advance' value be stored in this
      // list?  This data is only accessed via the public 'getWidth()'
      // method. (20031002 handegar)
      this->glyphwidths.append(cc_glyph3d_getwidth(glyph));
   
      if (strcharidx > 0) 
        cc_glyph3d_getkerning(prevglyph, glyph, &kerningx, &kerningy);          
      cc_glyph3d_getadvance(glyph, &advancex, &advancey);

      stringwidth += (advancex * stretchfactor + kerningx) * fontspecptr->size;
      prevglyph = glyph;
    }
    
    if (prevglyph != NULL) {
      // Have to remove the appended advance and add the last character to the calculated with
      stringwidth += (cc_glyph3d_getwidth(prevglyph) - advancex * stretchfactor) * fontspecptr->size;
      prevglyph = NULL; // To make sure the next line starts with blank sheets
    }

    this->stringwidths.append(stringwidth);
  }

  state->pop();
  SoCacheElement::setInvalid(storedinvalid);

  // unref old cache after creating the new one to avoid recreating glyphs
  if (oldcache) oldcache->unref();
}

#endif // DOXYGEN_SKIP_THIS
