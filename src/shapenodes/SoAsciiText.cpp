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

  \since Inventor 2.1
*/

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
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/misc/SoGlyph.h> /* SbBox2f */
#include <Inventor/misc/SoState.h>
#include <string.h>
#include <float.h> // FLT_MIN

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H
#include <Inventor/system/gl.h>

#include "../fonts/glyph3d.h"


/*!
  \enum SoAsciiText::Justification
  Font justification values.
*/


/*!
  \var SoMFString SoAsciiText::string
  Lines of text to render. Default value is empty.
*/
/*!
  \var SoSFFloat SoAsciiText::spacing
  Spacing between each line. Defaults to 1.0.
*/
/*!
  \var SoSFEnum SoAsciiText::justification
  Horizontal alignment. Default SoAsciiText::LEFT.
*/

/*!
  \var SoMFFloat SoAsciiText::width
  Defines the width of each line.
*/

#ifndef DOXYGEN_SKIP_THIS

class SoAsciiTextP {
public:

  SoAsciiTextP(SoAsciiText * master) : master(master) { }
  SoAsciiText * master;

  void setUpGlyphs(SoState * state, const cc_font_specification * fontspec, SoAsciiText * textnode);

  SbList <float> glyphwidths;
  SbList <float> stringwidths;
  SbBool needsetup;
  cc_font_specification * fontspec;
  SbBox3f maxglyphbbox;

  cc_string * prevfontname; // Store important fontspecs so that changes can be detected
  cc_string * prevfontstyle;
  float prevfontsize;

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
  PRIVATE(this)->needsetup = TRUE;

  PRIVATE(this)->prevfontname = cc_string_construct_new();
  PRIVATE(this)->prevfontstyle = cc_string_construct_new();
  PRIVATE(this)->prevfontsize = -1;

}

/*!
  Destructor.
*/
SoAsciiText::~SoAsciiText()
{
  
  if (PRIVATE(this)->fontspec != NULL) {
    cc_fontspec_clean(PRIVATE(this)->fontspec);
  }

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

  SoState * state = action->getState();

  cc_font_specification fontspec;
  cc_fontspec_construct(&fontspec, SoFontNameElement::get(state).getString(),
                        SoFontSizeElement::get(state),
                        this->getComplexityValue(state->getAction()));

  PRIVATE(this)->setUpGlyphs(state, &fontspec, this);

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

  int i, n = this->string.getNum();

  float longeststring = FLT_MIN;
  for (i = 0;i<PRIVATE(this)->stringwidths.getLength();++i)
    longeststring = SbMax(longeststring, PRIVATE(this)->stringwidths[i]);
 
  
  glBegin(GL_TRIANGLES);
  glNormal3f(0.0f, 0.0f, 1.0f);

  int glyphidx = 0;
  float ypos = 0.0f;

  for (i = 0; i < n; i++) {

    const float currwidth = PRIVATE(this)->stringwidths[i];
    float stretchlength = 0.0f;
    if (i < this->width.getNum()) 
      stretchlength = this->width[i];
    float stretchfactor =  (stretchlength) / strlen(this->string[i].getString());

    float xpos = 0.0f;
    switch (this->justification.getValue()) {
    case SoAsciiText::RIGHT:
      xpos = -currwidth + longeststring;
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
      cc_glyph3d * glyph = cc_glyph3d_getglyph(glyphidx, &fontspec);


      // Get kerning
      if (strcharidx > 0) {
        float kerningx, kerningy;
        cc_glyph3d_getkerning(prevglyph, glyph, &kerningx, &kerningy);
        xpos += kerningx* fontspec.size;
      }
      prevglyph = glyph;

      const SbVec2f * coords = (SbVec2f *) cc_glyph3d_getcoords(glyph);
      const int * ptr = cc_glyph3d_getfaceindices(glyph);

      while (*ptr >= 0) {
        SbVec2f v0, v1, v2;
        v0 = coords[*ptr++];
        v1 = coords[*ptr++];
        v2 = coords[*ptr++];

        if (do2Dtextures) {
          glTexCoord2f(v0[0] + xpos/fontspec.size, v0[1] + ypos/fontspec.size);
        }
        glVertex3f(v0[0] * fontspec.size + xpos, v0[1] * fontspec.size + ypos, 0.0f);
        
        if (do2Dtextures) {
          glTexCoord2f(v1[0] + xpos/fontspec.size, v1[1] + ypos/fontspec.size);
        }
        glVertex3f(v1[0] * fontspec.size + xpos, v1[1] * fontspec.size + ypos, 0.0f);
        
        if (do2Dtextures) {
          glTexCoord2f(v2[0] + xpos/fontspec.size, v2[1] + ypos/fontspec.size);
        }
        glVertex3f(v2[0] * fontspec.size + xpos, v2[1] * fontspec.size + ypos, 0.0f);

      }

      float advancex, advancey;
      cc_glyph3d_getadvance(glyph, &advancex, &advancey);
      xpos += (advancex + stretchfactor) * fontspec.size;

    }
    ypos -= fontspec.size * this->spacing.getValue();
  }
  glEnd();

  
  if (SoComplexityTypeElement::get(state) == SoComplexityTypeElement::OBJECT_SPACE) 
    SoGLCacheContextElement::shouldAutoCache(state, SoGLCacheContextElement::DO_AUTO_CACHE);
  
  
}

// Doc in parent.
void
SoAsciiText::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{

  if (action->is3DTextCountedAsTriangles()) {        
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
        const cc_glyph3d * glyph = cc_glyph3d_getglyph(glyphidx, PRIVATE(this)->fontspec);

        int cnt = 0;
        const int * ptr = cc_glyph3d_getfaceindices(glyph);
        while (*ptr++ >= 0) 
          cnt++;

        numtris += cnt / 3;
      }
    }
    action->addNumTriangles(numtris);
  }
  else {
    action->addNumText(this->string.getNum());
  }

}

// Doc in parent.
void
SoAsciiText::computeBBox(SoAction * action, SbBox3f & box, SbVec3f & center)
{

  SoState * state = action->getState();

  cc_font_specification fontspec;
  cc_fontspec_construct(&fontspec, SoFontNameElement::get(state).getString(),
                        SoFontSizeElement::get(state),
                        SoComplexityElement::get(state));

  PRIVATE(this)->setUpGlyphs(state, &fontspec, this);

  int i;
  const int n = this->string.getNum();
  const float linespacing = this->spacing.getValue();

  float maxw = FLT_MIN;
  for (i = 0;i<PRIVATE(this)->stringwidths.getLength();++i) 
    maxw = SbMax(maxw, PRIVATE(this)->stringwidths[i]);
  
  if(maxw == FLT_MIN) { // There is no text to bound. Returning.
    box.setBounds(SbVec3f(0.0f, 0.0f, 0.0f), SbVec3f(0.0f, 0.0f, 0.0f));
    center = SbVec3f(0,0,0);
    return; 
  }

  float longeststring = FLT_MIN;
  float maxwidth = 0.0f;
  for (i = 0;i<PRIVATE(this)->stringwidths.getLength();++i) 
    longeststring = SbMax(longeststring, PRIVATE(this)->stringwidths[i]);
  for (i = 0;(i<this->width.getNum()) && (i < n);++i)  
    maxwidth = SbMax(maxwidth, this->width[i]);
  
  float maxy, miny;
  float minx, maxx;
  
  minx = 0;
  maxx = maxw + maxwidth*fontspec.size;

  miny = -fontspec.size * this->spacing.getValue() * (n-1);
  maxy = fontspec.size;

  switch (this->justification.getValue()) {
  case SoAsciiText::LEFT:
    break;
  case SoAsciiText::RIGHT:
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

  // Expanding bbox so that glyphs like 'j's and 'q's are completely inside.
  box.extendBy(SbVec3f(0,PRIVATE(this)->maxglyphbbox.getMin()[1] - (n-1)*fontspec.size, 0));  
  box.extendBy(PRIVATE(this)->maxglyphbbox);
  center = box.getCenter();

}

// Doc in parent.
void
SoAsciiText::generatePrimitives(SoAction * action)
{

  SoState * state = action->getState();
  cc_font_specification fontspec;
  cc_fontspec_construct(&fontspec, SoFontNameElement::get(state).getString(),
                        SoFontSizeElement::get(state),
                        this->getComplexityValue(action));

  PRIVATE(this)->setUpGlyphs(state, &fontspec, this);

  int i, n = this->string.getNum();

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

  float longeststring = FLT_MIN;
  for (i = 0;i<PRIVATE(this)->stringwidths.getLength();++i)
    longeststring = SbMax(longeststring, PRIVATE(this)->stringwidths[i]);

  this->beginShape(action, SoShape::TRIANGLES, NULL);
  vertex.setNormal(SbVec3f(0.0f, 0.0f, 1.0f));

  int glyphidx = 0;
  float ypos = 0.0f;

  for (i = 0; i < n; i++) {

    const float currwidth = PRIVATE(this)->stringwidths[i];
    float stretchlength = 0.0f;
    if (i < this->width.getNum()) 
      stretchlength = this->width[i];
    float stretchfactor =  stretchlength / strlen(this->string[i].getString());

    detail.setStringIndex(i);
    float xpos = 0.0f;
    switch (this->justification.getValue()) {
    case SoAsciiText::RIGHT:
      xpos = -currwidth + longeststring;
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
      cc_glyph3d * glyph = cc_glyph3d_getglyph(glyphidx, &fontspec);
      
      // Get kerning
      if (strcharidx > 0) {
        float kerningx, kerningy;
        cc_glyph3d_getkerning(prevglyph, glyph, &kerningx, &kerningy);
        xpos += kerningx * fontspec.size;
      }
      prevglyph = glyph;
      detail.setCharacterIndex(strcharidx);

      const SbVec2f * coords = (SbVec2f *) cc_glyph3d_getcoords(glyph);
      const int * ptr = cc_glyph3d_getfaceindices(glyph);

      while (*ptr >= 0) {
        SbVec2f v0, v1, v2;
        v0 = coords[*ptr++];
        v1 = coords[*ptr++];
        v2 = coords[*ptr++];

        if(do2Dtextures) {
          vertex.setTextureCoords(SbVec2f(v0[0] + xpos/fontspec.size, v0[1] + ypos/fontspec.size));
        }
        vertex.setPoint(SbVec3f(v0[0] * fontspec.size + xpos, v0[1] * fontspec.size + ypos, 0.0f));
        this->shapeVertex(&vertex);

        if(do2Dtextures) {
          vertex.setTextureCoords(SbVec2f(v1[0] + xpos/fontspec.size, v1[1] + ypos/fontspec.size));
        }
        vertex.setPoint(SbVec3f(v1[0] * fontspec.size + xpos, v1[1] * fontspec.size + ypos, 0.0f));
        this->shapeVertex(&vertex);

        if(do2Dtextures) {
          vertex.setTextureCoords(SbVec2f(v2[0] + xpos/fontspec.size, v2[1] + ypos/fontspec.size));
        }
        vertex.setPoint(SbVec3f(v2[0] * fontspec.size + xpos, v2[1] * fontspec.size + ypos, 0.0f));
        this->shapeVertex(&vertex);

      }

      float advancex, advancey;
      cc_glyph3d_getadvance(glyph, &advancex, &advancey);
      xpos += (advancex + stretchfactor) * fontspec.size;
    }
    ypos -= fontspec.size * this->spacing.getValue();
  }
  this->endShape();
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
  SoField * f = list->getLastField();
  if (f == &this->string) PRIVATE(this)->needsetup = TRUE;
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
SoAsciiTextP::setUpGlyphs(SoState * state, const cc_font_specification * fontspec, SoAsciiText * textnode)
{

  // Note that this code is duplicated in SoText3::setUpGlyphs(), so
  // migrate bugfixes and other improvements.

  // We have to force a new setup if style, size or font has
  // changed. This must be done if boundingbox and text alignment
  // shall stay correct
  if (cc_string_compare(&fontspec->name, this->prevfontname) != 0 ||
      cc_string_compare(&fontspec->style, this->prevfontstyle)) {
    this->needsetup = TRUE; // Force new a setup
    cc_string_set_text(this->prevfontname, cc_string_get_text(&fontspec->name));
    cc_string_set_text(this->prevfontstyle, cc_string_get_text(&fontspec->style));
  }
  if(fontspec->size != this->prevfontsize) {
    this->prevfontsize = fontspec->size;
    this->needsetup = TRUE;
  }

  if (!this->needsetup) return;
  this->needsetup = FALSE;

  this->glyphwidths.truncate(0);
  this->stringwidths.truncate(0);

  float kerningx = 0;
  float kerningy = 0;
  float advancex = 0;
  float advancey = 0;
  cc_glyph3d * prevglyph = NULL;

  for (int i = 0; i < textnode->string.getNum(); i++) {
    const unsigned int length = textnode->string[i].getLength();
    float stringwidth = 0.0f;
    const float * maxbbox;
    this->maxglyphbbox.makeEmpty();

    for (unsigned int strcharidx = 0; strcharidx < length; strcharidx++) {

      // Note that the "unsigned char" cast is needed to avoid 8-bit
      // chars using the highest bit (i.e. characters above the ASCII
      // set up to 127) be expanded to huge int numbers that turn
      // negative when casted to integer size.
      const uint32_t glyphidx = (const unsigned char) textnode->string[i][strcharidx];
      cc_glyph3d * glyph = cc_glyph3d_getglyph(glyphidx, fontspec);
      assert(glyph);

      maxbbox = cc_glyph3d_getboundingbox(glyph); // Get max height
      this->maxglyphbbox.extendBy(SbVec3f(0, maxbbox[0] * fontspec->size, 0));
      this->maxglyphbbox.extendBy(SbVec3f(0, maxbbox[1] * fontspec->size, 0));

      // FIXME: Shouldn't it be the 'advance' value be stored in this
      // list?  This data is only accessed via the public 'getWidth()'
      // method. (20031002 handegar)
      this->glyphwidths.append(cc_glyph3d_getwidth(glyph));
   
      if (strcharidx > 0) 
        cc_glyph3d_getkerning(prevglyph, glyph, &kerningx, &kerningy);          
      cc_glyph3d_getadvance(glyph, &advancex, &advancey);

      stringwidth += (advancex + kerningx) * fontspec->size;
      prevglyph = glyph;
    }
    
    if (prevglyph != NULL) {
      // Italic font might cause last letter to be outside bbox. Add width if needed.
      if (advancex < cc_glyph3d_getwidth(prevglyph)) 
        stringwidth += (cc_glyph3d_getwidth(prevglyph) - advancex) * fontspec->size;
    }

    this->stringwidths.append(stringwidth);
  }

  // Make sure the boundingbox is updated if this method was called due to
  // a fontspec change.
  this->master->touch();

}

#endif // DOXYGEN_SKIP_THIS
