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
  \class SoVRMLText SoVRMLText.h Inventor/VRMLnodes/SoVRMLText.h
  \brief The SoVRMLText class is used to represent text in a scene.
  \ingroup VRMLnodes

  FIXME: implement and document
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
#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/misc/SoGlyph.h>
#include <Inventor/lists/SbList.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/bundles/SoMaterialBundle.h>
#include <Inventor/elements/SoGLTextureEnabledElement.h>
#include <Inventor/elements/SoGLTexture3EnabledElement.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/details/SoTextDetail.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/VRMLnodes/SoVRMLFontStyle.h>
#include <float.h> // FLT_MIN

#include <stddef.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H
#include <Inventor/system/gl.h>


// FIXME: The rendering code here is a quick'n'dirty hack, just based
// on the code found in SoAsciiText. Lots of stuff is missing. It will
// be fixed as soon as I have some spare time.  pederb, 2003-04-24

class SoVRMLTextP {
public:
  SoVRMLTextP(SoVRMLText * master) : master(master) { }

  float getWidth(const int idx, const float fontsize);
  SbList <const SoGlyph *> glyphs;
  SbList <float> glyphwidths;
  SbBool needsetup;
  void setUpGlyphs(SoState * state, SoVRMLText * textnode);

  SoVRMLText * master;
};


#define PRIVATE(obj) (obj->pimpl)

SO_NODE_SOURCE(SoVRMLText);

// Doc in parent
void
SoVRMLText::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoVRMLText, SO_VRML97_NODE_TYPE);
}

/*!
  Constructor.
*/
SoVRMLText::SoVRMLText(void)
{
  PRIVATE(this) = new SoVRMLTextP(this);
  PRIVATE(this)->needsetup = TRUE;

  SO_VRMLNODE_INTERNAL_CONSTRUCTOR(SoVRMLText);

  SO_VRMLNODE_ADD_EMPTY_EXPOSED_MFIELD(string);

  SO_VRMLNODE_ADD_EXPOSED_FIELD(fontStyle, (NULL));
  SO_VRMLNODE_ADD_EXPOSED_FIELD(maxExtent, (0.0f));
  SO_VRMLNODE_ADD_EMPTY_EXPOSED_MFIELD(length);
}

float
SoVRMLTextP::getWidth(const int idx, const float fontsize)
{
  float w = this->glyphwidths[idx];
  float maxe = this->master->maxExtent.getValue();
  if (maxe > 0.0f && w > maxe) w = maxe;
  return w;
}


/*!
  Destructor.
*/
SoVRMLText::~SoVRMLText()
{
  // unref() glyphs before the list get destructed.
  for (int j = 0; j < PRIVATE(this)->glyphs.getLength(); j++) {
    PRIVATE(this)->glyphs[j]->unref();
  }
  delete PRIVATE(this);
}

// Doc in parent
void
SoVRMLText::GLRender(SoGLRenderAction * action)
{
  if (!this->shouldGLRender(action)) return;

  SoState * state = action->getState();
  PRIVATE(this)->setUpGlyphs(state, this);

  SoMaterialBundle mb(action);
  mb.sendFirst();

  float size = 1.0f;
  float spacing = 1.0f;
  SoVRMLFontStyle * fs = (SoVRMLFontStyle*) this->fontStyle.getValue();
  if (fs) {
    size = fs->size.getValue();
    spacing = fs->spacing.getValue();
  }

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
      SoDebugError::postWarning("SoVRMLText::GLRender",
                                "3D-textures not properly supported for this node type yet.");
    }
  }

  int i, n = this->string.getNum();

  glBegin(GL_TRIANGLES);
  glNormal3f(0.0f, 0.0f, 1.0f);

  int glyphidx = 0;
  float ypos = 0.0f;

  for (i = 0; i < n; i++) {
    float currwidth = PRIVATE(this)->getWidth(i, size);
    float xpos = 0.0f;

    // FIXME: use justification from SoVRMLFontStyle. pederb, 2003-04-24

//     switch (this->justification.getValue()) {
//     case SoAsciiText::RIGHT:
//       xpos = -currwidth * size;
//       break;
//     case SoAsciiText::CENTER:
//       xpos = -currwidth * size * 0.5f;
//       break;
//     }

    const char * str = this->string[i].getString();
    while (*str++) {
      const SoGlyph * glyph = PRIVATE(this)->glyphs[glyphidx++];
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
    ypos -= size;
  }
  glEnd();
}


// Doc in parent
void
SoVRMLText::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  PRIVATE(this)->setUpGlyphs(action->getState(), this);

  if (action->is3DTextCountedAsTriangles()) {
    int n = PRIVATE(this)->glyphs.getLength();
    int numtris = 0;
    for (int i = 0; i < n; i++) {
      const SoGlyph * glyph = PRIVATE(this)->glyphs[i];
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

// Doc in parent
void
SoVRMLText::notify(SoNotList * list)
{
  SoField * f = list->getLastField();
  if (f == &this->string) PRIVATE(this)->needsetup = TRUE;
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
  PRIVATE(this)->setUpGlyphs(action->getState(), this);

  float size = 1.0f;
  float spacing = 1.0f;
  SoVRMLFontStyle * fs = (SoVRMLFontStyle*) this->fontStyle.getValue();
  if (fs) {
    size = fs->size.getValue();
    spacing = fs->spacing.getValue();
  }

  int i, n = this->string.getNum();
  if (n == 0 || PRIVATE(this)->glyphs.getLength() == 0) {
    center = SbVec3f(0.0f, 0.0f, 0.0f);
    box.setBounds(center, center);
    return;
  }

  float maxw = FLT_MIN;
  for (i = 0; i < n; i++) { maxw = SbMax(maxw, PRIVATE(this)->getWidth(i, size)); }

  SbBox2f maxbox;
  int numglyphs = PRIVATE(this)->glyphs.getLength();
  for (i = 0; i < numglyphs; i++) {
    maxbox.extendBy(PRIVATE(this)->glyphs[i]->getBoundingBox());
  }
  float maxglyphsize = maxbox.getMax()[1] - maxbox.getMin()[1];
  
  float maxy = size * maxbox.getMax()[1];
  float miny = maxy - (maxglyphsize*size + (n-1) * size * spacing);
  
  float minx, maxx;
  // FIXME: use justification from SoVRMLFontStyle. pederb, 2003-04-24

//   switch (this->justification.getValue()) {
//   case SoAsciiText::LEFT:
    minx = 0.0f;
    maxx = maxw * size;
//     break;
//   case SoAsciiText::RIGHT:
//     minx = -maxw * size;
//     maxx = 0.0f;
//     break;
//   case SoAsciiText::CENTER:
//     maxx = maxw * size * 0.5f;
//     minx = -maxx;
//     break;
//   default:
//     assert(0 && "unknown justification");
//     minx = maxx = 0.0f;
//     break;
//   }

  box.setBounds(SbVec3f(minx, miny, 0.0f), SbVec3f(maxx, maxy, 0.0f));
  center = box.getCenter();
}

// Doc in parent
void
SoVRMLText::generatePrimitives(SoAction * action)
{
  PRIVATE(this)->setUpGlyphs(action->getState(), this);

  float size = 1.0f;
  float spacing = 1.0f;
  SoVRMLFontStyle * fs = (SoVRMLFontStyle*) this->fontStyle.getValue();
  if (fs) {
    size = fs->size.getValue();
    spacing = fs->spacing.getValue();
  }

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
    float currwidth = PRIVATE(this)->getWidth(i, size);
    detail.setStringIndex(i);
    float xpos = 0.0f;
    // FIXME: use justification from SoVRMLFontStyle. pederb, 2003-04-24

//     switch (this->justification.getValue()) {
//     case SoAsciiText::RIGHT:
//       xpos = -currwidth * size;
//       break;
//     case SoAsciiText::CENTER:
//       xpos = - currwidth * size * 0.5f;
//       break;
//     }

    const char * str = this->string[i].getString();
    int charidx = 0;

    while (*str++) {
      detail.setCharacterIndex(charidx++);
      const SoGlyph * glyph = PRIVATE(this)->glyphs[glyphidx++];
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
    ypos -= size * spacing;
  }
  this->endShape();
}

#undef PRIVATE

// recalculate glyphs
void
SoVRMLTextP::setUpGlyphs(SoState * state, SoVRMLText * textnode)
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
    // Note that the "unsigned char" cast is needed to avoid 8-bit
    // chars using the highest bit (i.e. characters above the ASCII
    // set up to 127) be expanded to huge int numbers that turn
    // negative when casted to integer size.
    const unsigned char * ptr = (const unsigned char *)s.getString();
    float width = 0.0f;
    for (int j = 0; j < strlen; j++) {
      const SoGlyph * glyph = SoGlyph::getGlyph(ptr[j], SbName("default"));
      assert(glyph);
      this->glyphs.append(glyph);
      width += glyph->getWidth();
    }
    this->glyphwidths.append(width);
  }

  // unref old glyphs
  for (int j = 0; j < oldglyphs.getLength(); j++) oldglyphs[j]->unref();
}
