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

  float getWidth(const int idx, const float fontsize);
  void setUpGlyphs(SoState * state, SoAsciiText * textnode);

  SbList <float> glyphwidths;
  SbList <float> stringwidths;
  SbBool needsetup;
  cc_font_specification * fontspec;
  float textsize;
  SbBox3f maxglyphbbox;

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
  PRIVATE(this)->textsize = 1.0f;
  PRIVATE(this)->fontspec = NULL;

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
  if (!this->shouldGLRender(action)) return;

  SoState * state = action->getState();
  PRIVATE(this)->setUpGlyphs(state, this);

  SoMaterialBundle mb(action);
  mb.sendFirst();

  float size = SoFontSizeElement::get(state);
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

  int i, n = this->string.getNum();

  float longeststring = FLT_MIN;
  for (i = 0;i<PRIVATE(this)->stringwidths.getLength();++i)
    longeststring = SbMax(longeststring, PRIVATE(this)->stringwidths[i]);

  // FIXME: Must add support for stretching/compressing text
  // according to the 'width' field. (20030910 handegar)

  glBegin(GL_TRIANGLES);
  glNormal3f(0.0f, 0.0f, 1.0f);

  int glyphidx = 0;
  float ypos = 0.0f;

  for (i = 0; i < n; i++) {
    float currwidth = PRIVATE(this)->stringwidths[i];
    float xpos = 0.0f;
    switch (this->justification.getValue()) {
    case SoAsciiText::RIGHT:
      xpos = -currwidth + longeststring;
      break;
    case SoAsciiText::CENTER:
      xpos = -currwidth * 0.5f;
      break;
    }

    const char * str = this->string[i].getString();
    while (*str++) {

      cc_glyph3d * glyph = cc_glyph3d_getglyph(*(str-1), PRIVATE(this)->fontspec);

      float width = cc_glyph3d_getwidth(glyph);
      if (width == 0) 
        width = (1.0f / 3.0f); // setting spacesize to fontsize/3
      const SbVec2f * coords = (SbVec2f *) cc_glyph3d_getcoords(glyph);
      const int * ptr = cc_glyph3d_getfaceindices(glyph);

      while (*ptr >= 0) {
        SbVec2f v0, v1, v2;
        v0 = coords[*ptr++];
        v1 = coords[*ptr++];
        v2 = coords[*ptr++];
        if (do2Dtextures) {
          glTexCoord2f(v0[0], v0[1]);
        }
        glVertex3f(v0[0] * size + xpos, v0[1] * size + ypos, 0.0f);
        if (do2Dtextures) {
          glTexCoord2f(v1[0], v1[1]);
        }
        glVertex3f(v1[0] * size + xpos, v1[1] * size + ypos, 0.0f);
        if (do2Dtextures) {
          glTexCoord2f(v2[0], v2[1]);
        }
        glVertex3f(v2[0] * size + xpos, v2[1] * size + ypos, 0.0f);
      }

      // FIXME: next charpos should be based on 'advancement' and
      // 'kerning' from the font, not just glyph-width. (20030916
      // handegar)
      xpos += width * PRIVATE(this)->textsize;

    }
    ypos -= size * this->spacing.getValue();
  }
  glEnd();
}

// Doc in parent.
void
SoAsciiText::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  PRIVATE(this)->setUpGlyphs(action->getState(), this);

  if (action->is3DTextCountedAsTriangles()) {        
    int lines = this->string.getNum();
    int numtris = 0;      
    for (int i = 0;i < lines; ++i) {
      int n = this->string[i].getLength();
      const char * str = this->string[i].getString();
      for (int j = 0; j < n; j++) {
        cc_glyph3d * glyph = cc_glyph3d_getglyph(*str++, PRIVATE(this)->fontspec);
        int cnt = 0;
        const int * ptr = cc_glyph3d_getfaceindices(glyph);
        while (*ptr++ >= 0) cnt++;
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
  PRIVATE(this)->setUpGlyphs(action->getState(), this);

  int i;
  const int n = this->string.getNum();


  const float linespacing = this->spacing.getValue();

  float maxw = FLT_MIN;
  for (i = 0;i<PRIVATE(this)->stringwidths.getLength();++i) {
    maxw = SbMax(maxw, PRIVATE(this)->stringwidths[i]);
    // FIXME: Must add support for stretching/compressing text
    // according to the 'width' field here. (20030910 handegar)
    /*if((this->width[i] * PRIVATE(this)->textsize) < maxw)
      maxw = this->width[i] * PRIVATE(this)->textsize;*/
  }


  // FIXME: Glyphs which have parts below the writing line (ie. 'q',
  // 'g' etc.) are not completely inside the boundingbox. Must
  // fix. (20030910 handegar)

  float maxy, miny;
  float minx, maxx;
  
  minx = 0;
  maxx = maxw;

  miny = -PRIVATE(this)->textsize * this->spacing.getValue() * (n-1);
  maxy = PRIVATE(this)->textsize;

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
  box.extendBy(SbVec3f(0,PRIVATE(this)->maxglyphbbox.getMin()[1] - (n-1)*PRIVATE(this)->textsize, 0));
  
  box.extendBy(PRIVATE(this)->maxglyphbbox);
  center = box.getCenter();

}

// Doc in parent.
void
SoAsciiText::generatePrimitives(SoAction * action)
{
  PRIVATE(this)->setUpGlyphs(action->getState(), this);

  float size = SoFontSizeElement::get(action->getState());

  int i, n = this->string.getNum();

  SoPrimitiveVertex vertex;
  SoTextDetail detail;
  detail.setPart(0);
  vertex.setDetail(&detail);
  vertex.setMaterialIndex(0);

  this->beginShape(action, SoShape::TRIANGLES, NULL);
  vertex.setNormal(SbVec3f(0.0f, 0.0f, 1.0f));

  float longeststring = FLT_MIN;
  for (i = 0;i<PRIVATE(this)->stringwidths.getLength();++i)
    longeststring = SbMax(longeststring, PRIVATE(this)->stringwidths[i]);

  int glyphidx = 0;
  float ypos = 0.0f;

  for (i = 0; i < n; i++) {
    float currwidth = PRIVATE(this)->stringwidths[i];
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

    const char * str = this->string[i].getString();
    int charidx = 0;

    while (*str++) {
      detail.setCharacterIndex(charidx++);

      cc_glyph3d * glyph = cc_glyph3d_getglyph(*(str-1), PRIVATE(this)->fontspec);

      float width = cc_glyph3d_getwidth(glyph);
      if (width == 0) 
        width = (1.0f / 3.0f); // setting spacesize to fontsize/3   
      const SbVec2f * coords = (SbVec2f *) cc_glyph3d_getcoords(glyph);
      const int * ptr = cc_glyph3d_getfaceindices(glyph);

      while (*ptr >= 0) {
        SbVec2f v0, v1, v2;
        v0 = coords[*ptr++];
        v1 = coords[*ptr++];
        v2 = coords[*ptr++];
        vertex.setTextureCoords(SbVec2f(v0[0], v0[1]));
        vertex.setPoint(SbVec3f(v0[0] * size + xpos, v0[1] * size + ypos, 0.0f));
        this->shapeVertex(&vertex);
        vertex.setTextureCoords(SbVec2f(v1[0], v1[1]));
        vertex.setPoint(SbVec3f(v1[0] * size + xpos, v1[1] * size + ypos, 0.0f));
        this->shapeVertex(&vertex);
        vertex.setTextureCoords(SbVec2f(v2[0], v2[1]));
        vertex.setPoint(SbVec3f(v2[0] * size + xpos, v2[1] * size + ypos, 0.0f));
        this->shapeVertex(&vertex);
      }
      xpos += width * size;
    }
    ypos -= size * this->spacing.getValue();
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
SoAsciiTextP::setUpGlyphs(SoState * state, SoAsciiText * textnode)
{
  // Note that this code is duplicated in SoText3::setUpGlyphs(), so
  // migrate bugfixes and other improvements.

  if (!this->needsetup) return;
  this->needsetup = FALSE;

  // Build up font-spesification struct
  if (this->fontspec != NULL) {
    cc_fontspec_clean(this->fontspec);
    delete this->fontspec;
  }

  this->fontspec = new cc_font_specification;
  cc_fontspec_construct(this->fontspec,
                        SoFontNameElement::get(state).getString(),
                        SoFontSizeElement::get(state),
                        this->master->getComplexityValue(state->getAction()));

  this->textsize = SoFontSizeElement::get(state); 
  this->glyphwidths.truncate(0);

  float kerningx = 0;
  float kerningy = 0;
  float advancex = 0;
  float advancey = 0;
  cc_glyph3d * prevglyph = NULL;


  for (int i = 0; i < textnode->string.getNum(); i++) {
    const SbString & s = textnode->string[i];
    int strlen = s.getLength();
    // Note that the "unsigned char" cast is needed to avoid 8-bit
    // chars using the highest bit (i.e. characters above the ASCII
    // set up to 127) be expanded to huge int numbers that turn
    // negative when casted to integer size.
    const unsigned char * ptr = (const unsigned char *)s.getString();
    float stringwidth = 0.0f;
    float glyphwidth = 0.0f;
    const float * maxbbox;
    this->maxglyphbbox.makeEmpty();

    for (int j = 0; j < strlen; j++) {

      cc_glyph3d * glyph = cc_glyph3d_getglyph(ptr[j], this->fontspec);
      assert(glyph);

      maxbbox = cc_glyph3d_getboundingbox(glyph); // Get max height
      this->maxglyphbbox.extendBy(SbVec3f(0, maxbbox[0] * this->fontspec->size, 0));
      this->maxglyphbbox.extendBy(SbVec3f(0, maxbbox[1] * this->fontspec->size, 0));

      glyphwidth = cc_glyph3d_getwidth(glyph);
      if (glyphwidth == 0)
        glyphwidth = 1.0f / 3.0f;

      stringwidth += glyphwidth * this->textsize;
     
      
      if (j > 0) 
        cc_glyph3d_getkerning(prevglyph, glyph, &kerningx, &kerningy);          
      cc_glyph3d_getadvance(glyph, &advancex, &advancey);
      this->glyphwidths.append(advancex + kerningx);
      
      prevglyph = glyph;
    }

    this->stringwidths.append(stringwidth);
  }



}

#endif // DOXYGEN_SKIP_THIS
