/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

/*!
  \class SoText3 SoText3.h Inventor/nodes/SoText3.h
  \brief The SoText3 class renders extruded 3D text.
  \ingroup nodes

  Render text as 3D geometry.

  Beware that using a lot of SoText3 text characters in a scene will
  usually have severe impact on the rendering performance, as each and
  every character of the text contributes a lot of polygon primitives
  to the rendering system.

  Due to the aove mentioned fact, SoText3 nodes are better used in
  situations where you need just one or a few characters to be placed
  in your scene, than to visualize e.g. complete sentences.

  \sa SoText2, SoAsciiText, SoProfile
*/

#include <Inventor/nodes/SoText3.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/bundles/SoMaterialBundle.h>
#include <Inventor/details/SoTextDetail.h>
#include <Inventor/elements/SoFontNameElement.h>
#include <Inventor/elements/SoFontSizeElement.h>
#include <Inventor/elements/SoGLNormalizeElement.h>
#include <Inventor/elements/SoGLShapeHintsElement.h>
#include <Inventor/elements/SoGLTextureEnabledElement.h>
#include <Inventor/elements/SoMaterialBindingElement.h>
#include <Inventor/misc/SoGlyph.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/sensors/SoFieldSensor.h>

#ifdef _WIN32
#include <windows.h> // *sigh* needed for gl.h
#endif // _WIN32
#include <GL/gl.h>

#include <coindefs.h> // COIN_STUB()

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG


/*!
  \enum SoText3::Part
  Used to specify which parts should be rendered/generated.
*/
/*!
  \var SoText3::Part SoText3::FRONT
  Front of characters.
*/
/*!
  \var SoText3::Part SoText3::SIDES
  Sides of characters.
*/
/*!
  \var SoText3::Part SoText3::BACK
  Back of characters.
*/
/*!
  \var SoText3::Part SoText3::ALL
  All parts.
*/

/*!
  \enum SoText3::Justification
  Used to specify horizontal string alignment.
*/
/*!
  \var SoText3::Justification SoText3::LEFT
  Left edges of strings are aligned at x = 0
*/
/*!
  \var SoText3::Justification SoText3::RIGHT
  Right edges of strings are aligned at x = 0.
*/
/*!
  \var SoText3::Justification SoText3::CENTER
  Strings are centered at x = 0
*/


/*!
  \var SoMFString SoText3::string
  The strings.
*/
/*!
  \var SoSFFloat SoText3::spacing
  Vertical spacing. A value of 1.0 is the normal spacing.
*/
/*!
  \var SoSFEnum SoText3::justification
  Horizontal justification.
*/
/*!
  \var SoSFBitMask SoText3::parts
  Character parts.
*/

// Metadon doc:
/*¡
  FIXME: missing features, pederb 20000224
  - Profile coordinates are not supported yet.
  - Texture coordinates are not generated yet.
  - Normals for SIDES should be smoothed.
*/

// *************************************************************************

SO_NODE_SOURCE(SoText3);

/*!
  Constructor.
*/
SoText3::SoText3(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoText3);

  SO_NODE_ADD_FIELD(string, (""));
  SO_NODE_ADD_FIELD(spacing, (1.0f));
  SO_NODE_ADD_FIELD(justification, (SoText3::LEFT));
  SO_NODE_ADD_FIELD(parts, (SoText3::FRONT));

  SO_NODE_DEFINE_ENUM_VALUE(Justification, LEFT);
  SO_NODE_DEFINE_ENUM_VALUE(Justification, RIGHT);
  SO_NODE_DEFINE_ENUM_VALUE(Justification, CENTER);
  SO_NODE_SET_SF_ENUM_TYPE(justification, Justification);

  SO_NODE_DEFINE_ENUM_VALUE(Part, FRONT);
  SO_NODE_DEFINE_ENUM_VALUE(Part, SIDES);
  SO_NODE_DEFINE_ENUM_VALUE(Part, BACK);
  SO_NODE_DEFINE_ENUM_VALUE(Part, ALL);
  SO_NODE_SET_SF_ENUM_TYPE(parts, Part);

  this->stringsensor = new SoFieldSensor(SoText3::fieldSensorCB, this);
  this->stringsensor->attach(&this->string);

  this->needsetup = TRUE;
}

/*!
  Destructor.
*/
SoText3::~SoText3()
{
  delete this->stringsensor;
}

// doc in parent
void
SoText3::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoText3);
}

// doc in parent
void
SoText3::computeBBox(SoAction * action, SbBox3f & box, SbVec3f & center)
{
  this->setUpGlyphs(action->getState());

  // FIXME: consider profile. pederb, 20000223
  float size = SoFontSizeElement::get(action->getState());
  int i, n = this->widths.getLength();
  if (n == 0) {
    center = SbVec3f(0.0f, 0.0f, 0.0f);
    box.setBounds(center, center);
    return;
  }
  float maxw = this->widths[0];
  for (i = 1; i < n; i++) {
    float tmp = this->widths[i];
    if (tmp > maxw) maxw = tmp;
  }

  SbBox2f maxbox;
  int numglyphs = this->glyphs.getLength();
  for (i = 0; i < numglyphs; i++) {
    maxbox.extendBy(this->glyphs[i]->getBoundingBox());
  }
  float maxglyphsize = maxbox.getMax()[1] - maxbox.getMin()[1];

  float maxy = size * maxbox.getMax()[1];
  float miny = maxy - (maxglyphsize*size + (n-1) * size * this->spacing.getValue());

  float minx, maxx;
  switch (this->justification.getValue()) {
  case SoText3::LEFT:
    minx = 0.0f;
    maxx = maxw * size;
    break;
  case SoText3::RIGHT:
    minx = -maxw * size;
    maxx = 0.0f;
    break;
  case SoText3::CENTER:
    maxx = maxw * size * 0.5f;
    minx = -maxx;
    break;
  default:
    assert(0 && "unknown justification");
    minx = maxx = 0.0f;
    break;
  }

  float minz = -1.0f, maxz = 0.0f;
  if (this->parts.getValue() == SoText3::BACK) {
    maxz = -1.0f;
  }
  else if (this->parts.getValue() == SoText3::FRONT) {
    minz = 0.0f;
  }
  box.setBounds(SbVec3f(minx, miny, minz), SbVec3f(maxx, maxy, maxz));
  center = box.getCenter();
}


/*!
  Not implemented. Should probably have been private in OIV. Let us
  know if you need this method for anything, and we'll implement it.
*/
SbBox3f
SoText3::getCharacterBounds(SoState * state, int stringindex, int charindex)
{
  COIN_STUB();
  return SbBox3f();
}

// doc in parent
void
SoText3::GLRender(SoGLRenderAction * action)
{
  if (!this->shouldGLRender(action)) return;

  SoState * state = action->getState();
  this->setUpGlyphs(state);

  SoMaterialBindingElement::Binding binding =
    SoMaterialBindingElement::get(state);

  SbBool matperpart = binding != SoMaterialBindingElement::OVERALL;

  SoMaterialBundle mb(action);
  mb.sendFirst();

  unsigned int prts = this->parts.getValue();

  const SoGLShapeHintsElement * sh = (SoGLShapeHintsElement *)
    state->getConstElement(SoGLShapeHintsElement::getClassStackIndex());
  if (prts == SoText3::ALL) {
    // turn on backface culling. Disable twoside lighting
    sh->forceSend(TRUE, TRUE, FALSE);
  }
  else {
    // twoside lightning and no backface culling
    sh->forceSend(TRUE);
  }

  const SoGLNormalizeElement * ne = (SoGLNormalizeElement *)
    state->getConstElement(SoGLNormalizeElement::getClassStackIndex());
  ne->forceSend(TRUE); // we will provide unit length normals

  if (prts & SoText3::FRONT) {
    this->render(state, SoText3::FRONT);
  }
  if (prts & SoText3::SIDES) {
    if (matperpart) mb.send(1, FALSE);
    this->render(state, SoText3::SIDES);
  }
  if (prts & SoText3::BACK) {
    if (matperpart) mb.send(2, FALSE);
    this->render(state, SoText3::BACK);
  }
}

// doc in parent
void
SoText3::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  if (action->is3DTextCountedAsTriangles()) {
    COIN_STUB();
  }
  else {
    action->addNumText(this->string.getNum());
  }
}

// doc in parent
void
SoText3::generatePrimitives(SoAction * action)
{
  this->setUpGlyphs(action->getState());

  unsigned int prts = this->parts.getValue();

  if (prts & SoText3::FRONT) {
    this->generate(action, SoText3::FRONT);
  }
  if (prts & SoText3::SIDES) {
    this->generate(action, SoText3::SIDES);
  }
  if (prts & SoText3::BACK) {
    this->generate(action, SoText3::BACK);
  }
}

// doc in parent
SbBool
SoText3::willSetShapeHints(void) const
{
  return TRUE;
}

// doc in parent
SbBool
SoText3::willUpdateNormalizeElement(SoState *) const
{
  return TRUE;
}

// doc in parent
SoDetail *
SoText3::createTriangleDetail(SoRayPickAction * action,
                              const SoPrimitiveVertex * v1,
                              const SoPrimitiveVertex * v2,
                              const SoPrimitiveVertex * v3,
                              SoPickedPoint * pp)
{
  // generatePrimitives() places text details inside each primitive vertex
  assert(v1->getDetail());
  return v1->getDetail()->copy();
}


// recalculate glyphs
void
SoText3::setUpGlyphs(SoState * state)
{
  if (!this->needsetup) return;
  this->needsetup = FALSE;

  // store old glyphs to avoid freeing glyphs too soon
  SbList <const SoGlyph *> oldglyphs;
  int i;
  int n = this->glyphs.getLength();
  for (i = 0; i < n; i++) {
    oldglyphs.append(this->glyphs[i]);
  }
  this->glyphs.truncate(0);
  this->widths.truncate(0);

  for (i = 0; i < this->string.getNum(); i++) {
    const SbString & s = this->string[i];
    int strlen = s.getLength();
    const char * ptr = s.getString();
    float width = 0.0f;
    for (int j = 0; j < strlen; j++) {
      const SoGlyph * glyph = SoGlyph::getGlyph(ptr[j], SbName("default"));
      this->glyphs.append(glyph);
      width += glyph->getWidth();
    }
    this->widths.append(width);
  }

  // unref old glyphs
  n = oldglyphs.getLength();
  for (i = 0; i < n; i++) {
    oldglyphs[i]->unref();
  }
}

// render text geometry
void
SoText3::render(SoState * state, unsigned int part)
{
  float size = SoFontSizeElement::get(state);
  // FIXME: not in use (yet?). 20000525 mortene.
//    SbBool doTextures = SoGLTextureEnabledElement::get(state);

  int i, n = this->widths.getLength();

  if (part == SoText3::SIDES) glBegin(GL_QUADS);
  else {
    glBegin(GL_TRIANGLES);
    if (part == SoText3::FRONT)
      glNormal3f(0.0f, 0.0f, 1.0f);
    else
      glNormal3f(0.0f, 0.0f, -1.0f);
  }

  int glyphidx = 0;
  float ypos = 0.0f;

  for (i = 0; i < n; i++) {
    float xpos = 0.0f;
    switch (this->justification.getValue()) {
    case SoText3::RIGHT:
      xpos = -this->widths[i] * size;
      break;
    case SoText3::CENTER:
      xpos = - this->widths[i] * size * 0.5f;
      break;
    }

    const char * str = this->string[i].getString();

    while (*str++) {
      const SoGlyph * glyph = this->glyphs[glyphidx++];
      const SbVec2f * coords = glyph->getCoords();
      if (part != SoText3::SIDES) {
        const int * ptr = glyph->getFaceIndices();
        while (*ptr >= 0) {
          SbVec2f v0, v1, v2;
          float zval;
          if (part == SoText3::FRONT) {
            v0 = coords[*ptr++];
            v1 = coords[*ptr++];
            v2 = coords[*ptr++];
            zval = 0.0f;
          }
          else {
            v2 = coords[*ptr++];
            v1 = coords[*ptr++];
            v0 = coords[*ptr++];
            zval = -1.0f;
          }
          glVertex3f(v0[0] * size + xpos, v0[1] * size + ypos, zval);
          glVertex3f(v1[0] * size + xpos, v1[1] * size + ypos, zval);
          glVertex3f(v2[0] * size + xpos, v2[1] * size + ypos, zval);
        }
      }
      else { // SIDES
        const int * ptr = glyph->getEdgeIndices();
        SbVec2f v0, v1;
        while (*ptr >= 0) {
          v0 = coords[*ptr++];
          v1 = coords[*ptr++];
          SbVec3f tmp(v1[0]-v0[0], v1[1] - v0[1], 0.0f);
          SbVec3f normal = tmp.cross(SbVec3f(0.0f, 0.0f, 1.0f));
          normal.normalize();
          v0[0] = v0[0] * size + xpos;
          v0[1] = v0[1] * size + ypos;
          v1[0] = v1[0] * size + xpos;
          v1[1] = v1[1] * size + ypos;
          glNormal3fv(normal.getValue());
          glVertex3f(v1[0], v1[1], 0.0f);
          glVertex3f(v0[0], v0[1], 0.0f);
          glVertex3f(v0[0], v0[1], -1.0f);
          glVertex3f(v1[0], v1[1], -1.0f);
        }
      }
      xpos += glyph->getWidth() * size;
    }
    ypos -= size * this->spacing.getValue();
  }
  glEnd();
}

// generate text geometry
void
SoText3::generate(SoAction * action, unsigned int part)
{
  float size = SoFontSizeElement::get(action->getState());

  int i, n = this->widths.getLength();

  int matidx = 0;
  if (part == SoText3::SIDES) matidx = 1;
  else if (part == SoText3::BACK) matidx = 2;

  SoPrimitiveVertex vertex;
  SoTextDetail detail;
  detail.setPart(part);
  vertex.setDetail(&detail);
  vertex.setMaterialIndex(matidx);

  if (part == SoText3::SIDES) this->beginShape(action, SoShape::QUADS, NULL);
  else {
    this->beginShape(action, SoShape::TRIANGLES, NULL);
    if (part == SoText3::FRONT) {
      vertex.setNormal(SbVec3f(0.0f, 0.0f, 1.0f));
    }
    else {
      vertex.setNormal(SbVec3f(0.0f, 0.0f, -1.0f));
    }
  }

  int glyphidx = 0;
  float ypos = 0.0f;

  for (i = 0; i < n; i++) {
    detail.setStringIndex(i);
    float xpos = 0.0f;
    switch (this->justification.getValue()) {
    case SoText3::RIGHT:
      xpos = -this->widths[i] * size;
      break;
    case SoText3::CENTER:
      xpos = - this->widths[i] * size * 0.5f;
      break;
    }

    const char * str = this->string[i].getString();

    int charidx = 0;

    while (*str++) {
      detail.setCharacterIndex(charidx++);
      const SoGlyph * glyph = this->glyphs[glyphidx++];
      const SbVec2f * coords = glyph->getCoords();
      if (part != SoText3::SIDES) {
        const int * ptr = glyph->getFaceIndices();
        while (*ptr >= 0) {
          SbVec2f v0, v1, v2;
          float zval;
          if (part == SoText3::FRONT) {
            v0 = coords[*ptr++];
            v1 = coords[*ptr++];
            v2 = coords[*ptr++];
            zval = 0.0f;
          }
          else {
            v2 = coords[*ptr++];
            v1 = coords[*ptr++];
            v0 = coords[*ptr++];
            zval = -1.0f;
          }
          vertex.setPoint(SbVec3f(v0[0] * size + xpos, v0[1] * size + ypos, zval));
          this->shapeVertex(&vertex);
          vertex.setPoint(SbVec3f(v1[0] * size + xpos, v1[1] * size + ypos, zval));
          this->shapeVertex(&vertex);
          vertex.setPoint(SbVec3f(v2[0] * size + xpos, v2[1] * size + ypos, zval));
          this->shapeVertex(&vertex);
        }
      }
      else { // SIDES
        const int * ptr = glyph->getEdgeIndices();
        SbVec2f v0, v1;
        while (*ptr >= 0) {
          v0 = coords[*ptr++];
          v1 = coords[*ptr++];
          SbVec3f tmp(v1[0]-v0[0], v1[1] - v0[1], 0.0f);
          SbVec3f normal = tmp.cross(SbVec3f(0.0f, 0.0f, 1.0f));
          normal.normalize();
          v0[0] = v0[0] * size + xpos;
          v0[1] = v0[1] * size + ypos;
          v1[0] = v1[0] * size + xpos;
          v1[1] = v1[1] * size + ypos;
          vertex.setNormal(normal);
          vertex.setPoint(SbVec3f(v1[0], v1[1], 0.0f));
          this->shapeVertex(&vertex);
          vertex.setPoint(SbVec3f(v0[0], v0[1], 0.0f));
          this->shapeVertex(&vertex);
          vertex.setPoint(SbVec3f(v0[0], v0[1], -1.0f));
          this->shapeVertex(&vertex);
          vertex.setPoint(SbVec3f(v1[0], v1[1], -1.0f));
          this->shapeVertex(&vertex);
        }
      }
      xpos += glyph->getWidth() * size;
    }
    ypos -= size * this->spacing.getValue();
  }
  this->endShape();
}

// called whenever SoText3::string is edited
void
SoText3::fieldSensorCB(void * d, SoSensor *)
{
  SoText3 * thisp = (SoText3 *)d;
  thisp->needsetup = TRUE;
}
