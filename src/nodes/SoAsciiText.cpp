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
#include <Inventor/misc/SoGlyph.h>
#include <Inventor/misc/SoState.h>
#include <string.h>
#include <float.h> // FLT_MIN

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H
#include <Inventor/system/gl.h>


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
  float getWidth(const int idx, const float fontsize);
  SbList <const SoGlyph *> glyphs;
  SbList <float> glyphwidths;
  SbBool needsetup;
  void setUpGlyphs(SoState * state, SoAsciiText * textnode);
};

#endif // DOXYGEN_SKIP_THIS

// *************************************************************************

SO_NODE_SOURCE(SoAsciiText);

#undef THIS
#define THIS this->pimpl

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

  THIS = new SoAsciiTextP;
  THIS->needsetup = TRUE;
}

/*!
  Destructor.
*/
SoAsciiText::~SoAsciiText()
{
  // unref() glyphs before the list get destructed.
  for (int j = 0; j < THIS->glyphs.getLength(); j++) THIS->glyphs[j]->unref();
  delete THIS;
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
  THIS->setUpGlyphs(state, this);

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

  glBegin(GL_TRIANGLES);
  glNormal3f(0.0f, 0.0f, 1.0f);

  int glyphidx = 0;
  float ypos = 0.0f;

  for (i = 0; i < n; i++) {
    float currwidth = this->getWidth(i, size);
    float xpos = 0.0f;
    switch (this->justification.getValue()) {
    case SoAsciiText::RIGHT:
      xpos = -currwidth * size;
      break;
    case SoAsciiText::CENTER:
      xpos = -currwidth * size * 0.5f;
      break;
    }

    const char * str = this->string[i].getString();
    while (*str++) {
      const SoGlyph * glyph = THIS->glyphs[glyphidx++];
      const SbVec2f * coords = glyph->getCoords();
      const int * ptr = glyph->getFaceIndices();
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
      xpos += glyph->getWidth() * size;
    }
    ypos -= size * this->spacing.getValue();
  }
  glEnd();
}

// Doc in parent.
void
SoAsciiText::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  THIS->setUpGlyphs(action->getState(), this);

  if (action->is3DTextCountedAsTriangles()) {
    int n = THIS->glyphs.getLength();
    int numtris = 0;
    for (int i = 0; i < n; i++) {
      const SoGlyph * glyph = THIS->glyphs[i];
      int cnt = 0;
      const int * ptr = glyph->getFaceIndices();
      while (*ptr++ >= 0) cnt++;
      numtris += cnt / 3;
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
  THIS->setUpGlyphs(action->getState(), this);

  float size = SoFontSizeElement::get(action->getState());
  int i, n = this->string.getNum();
  if (n == 0 || THIS->glyphs.getLength() == 0) {
    center = SbVec3f(0.0f, 0.0f, 0.0f);
    box.setBounds(center, center);
    return;
  }

  float maxw = FLT_MIN;
  for (i = 0; i < n; i++) { maxw = SbMax(maxw, this->getWidth(i, size)); }

  SbBox2f maxbox;
  int numglyphs = THIS->glyphs.getLength();
  for (i = 0; i < numglyphs; i++) {
    maxbox.extendBy(THIS->glyphs[i]->getBoundingBox());
  }
  float maxglyphsize = maxbox.getMax()[1] - maxbox.getMin()[1];

  float maxy = size * maxbox.getMax()[1];
  float miny = maxy - (maxglyphsize*size + (n-1) * size * this->spacing.getValue());

  float minx, maxx;
  switch (this->justification.getValue()) {
  case SoAsciiText::LEFT:
    minx = 0.0f;
    maxx = maxw * size;
    break;
  case SoAsciiText::RIGHT:
    minx = -maxw * size;
    maxx = 0.0f;
    break;
  case SoAsciiText::CENTER:
    maxx = maxw * size * 0.5f;
    minx = -maxx;
    break;
  default:
    assert(0 && "unknown justification");
    minx = maxx = 0.0f;
    break;
  }

  box.setBounds(SbVec3f(minx, miny, 0.0f), SbVec3f(maxx, maxy, 0.0f));
  center = box.getCenter();
}

// Doc in parent.
void
SoAsciiText::generatePrimitives(SoAction * action)
{
  THIS->setUpGlyphs(action->getState(), this);

  float size = SoFontSizeElement::get(action->getState());

  int i, n = this->string.getNum();

  SoPrimitiveVertex vertex;
  SoTextDetail detail;
  detail.setPart(0);
  vertex.setDetail(&detail);
  vertex.setMaterialIndex(0);

  this->beginShape(action, SoShape::TRIANGLES, NULL);
  vertex.setNormal(SbVec3f(0.0f, 0.0f, 1.0f));

  int glyphidx = 0;
  float ypos = 0.0f;

  for (i = 0; i < n; i++) {
    float currwidth = this->getWidth(i, size);
    detail.setStringIndex(i);
    float xpos = 0.0f;
    switch (this->justification.getValue()) {
    case SoAsciiText::RIGHT:
      xpos = -currwidth * size;
      break;
    case SoAsciiText::CENTER:
      xpos = - currwidth * size * 0.5f;
      break;
    }

    const char * str = this->string[i].getString();
    int charidx = 0;

    while (*str++) {
      detail.setCharacterIndex(charidx++);
      const SoGlyph * glyph = THIS->glyphs[glyphidx++];
      const SbVec2f * coords = glyph->getCoords();
      const int * ptr = glyph->getFaceIndices();
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
      xpos += glyph->getWidth() * size;
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
  if (f == &this->string) THIS->needsetup = TRUE;
  inherited::notify(list);
}

// returns "normalized" width of specified string. When too few
// width values are supplied, the glyphwidths are used instead.
float
SoAsciiText::getWidth(const int idx, const float fontsize)
{
  if (idx < this->width.getNum() && this->width[idx] > 0.0f)
    return this->width[idx] / fontsize;
  return THIS->glyphwidths[idx];
}

// SoAsciiTextP methods implemented below

#undef THIS

#ifndef DOXYGEN_SKIP_THIS

// recalculate glyphs
void
SoAsciiTextP::setUpGlyphs(SoState * state, SoAsciiText * textnode)
{
  // Note that this code is duplicated in SoText3::setUpGlyphs(), so
  // migrate bugfixes and other improvements.

  if (!this->needsetup) return;
  this->needsetup = FALSE;

  // store old glyphs to avoid freeing glyphs too soon
  SbList<const SoGlyph *> oldglyphs;
  oldglyphs = this->glyphs;
  this->glyphs.truncate(0);
  this->glyphwidths.truncate(0);

  for (int i = 0; i < textnode->string.getNum(); i++) {
    const SbString & s = textnode->string[i];
    int strlen = s.getLength();
    const char * ptr = s.getString();
    float width = 0.0f;
    for (int j = 0; j < strlen; j++) {
      const SoGlyph * glyph = SoGlyph::getGlyph(ptr[j], SbName("default"));
      this->glyphs.append(glyph);
      width += glyph->getWidth();
    }
    this->glyphwidths.append(width);
  }

  // unref old glyphs
  for (int j = 0; j < oldglyphs.getLength(); j++) oldglyphs[j]->unref();
}

#endif // DOXYGEN_SKIP_THIS
