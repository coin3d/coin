/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
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

  Due to the above mentioned fact, SoText3 nodes are best used in
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
#include <Inventor/nodes/SoProfile.h>
#include <Inventor/nodes/SoNurbsProfile.h>
#include <Inventor/SbLine.h>

#include <coindefs.h> // COIN_OBSOLETED()

#include <float.h> // FLT_MAX, FLT_MIN

#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H
#include <Inventor/system/gl.h>

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
  Left edges of strings are aligned.
*/
/*!
  \var SoText3::Justification SoText3::RIGHT
  Right edges of strings are aligned.
*/
/*!
  \var SoText3::Justification SoText3::CENTER
  Strings are centered.
*/


/*!
  \var SoMFString SoText3::string
  The strings. Defaults to empty set.
*/
/*!
  \var SoSFFloat SoText3::spacing
  Vertical spacing. 1.0 is the default spacing.
*/
/*!
  \var SoSFEnum SoText3::justification
  Horizontal justification. Default is alignment at the left border.
*/
/*!
  \var SoSFBitMask SoText3::parts
  Character parts. Default is to show only the front-facing part.
*/

// FIXME: missing features, pederb 20000224
//   - Texture coordinates are not generated yet.
//   - Normals for SIDES should be smoothed.

// *************************************************************************

#ifndef DOXYGEN_SKIP_THIS
class SoText3P {
public:
  SbList <const SoGlyph *> glyphs;
  SbList <float> widths;
  void setUpGlyphs(SoState * state, SoText3 * textnode);
  SbBool needsetup;
};
#endif // DOXYGEN_SKIP_THIS

#undef THIS
#define THIS this->pimpl

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

  THIS = new SoText3P;
  THIS->needsetup = TRUE;
}

/*!
  Destructor.
*/
SoText3::~SoText3()
{
  // unref() glyphs before the list get destructed.
  for (int j = 0; j < THIS->glyphs.getLength(); j++) THIS->glyphs[j]->unref();
  delete THIS;
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
  THIS->setUpGlyphs(action->getState(), this);
  int numglyphs = THIS->glyphs.getLength();
  if (numglyphs == 0) return;

  float size = SoFontSizeElement::get(action->getState());
  int i, n = THIS->widths.getLength();
  if (n == 0) {
    center = SbVec3f(0.0f, 0.0f, 0.0f);
    box.setBounds(center, center);
    return;
  }

  float maxw = FLT_MIN;
  for (i = 0; i < n; i++) { maxw = SbMax(maxw, THIS->widths[i]); }

  SbBox2f maxbox;
  for (i = 0; i < numglyphs; i++) {
    maxbox.extendBy(THIS->glyphs[i]->getBoundingBox());
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

  // check profiles and extend bounding box if necessary
  float profsize = 0;
  float minz = -1.0f, maxz = 0.0f;
  SoState *state = action->getState();
  const SoNodeList profilenodes = SoProfileElement::get(state);
  int numprofiles = profilenodes.getLength();
  if ( numprofiles > 0) {
    assert(profilenodes[0]->getTypeId().isDerivedFrom(SoProfile::getClassTypeId()));
    for (int i = numprofiles-1; i >= 0; i--) {
      SoProfile *pn = (SoProfile *)profilenodes[i];
      if (pn->isOfType(SoNurbsProfile::getClassTypeId())) {
        // Don't use SoProfile::getVertices() for SoNurbsProfile
        // nodes as this would cause a call to the GLU library, which
        // requires a valid GL context. Instead we approximate using
        // SoNurbsProfile::getTrimCurve(), and use the control points
        // to calculate the bounding box. This is an approximation,
        // but the same technique is used in So[Indexed]NurbsSurface
        // and So[Indexed]NurbsCurve. To avoid this approximation we
        // would need our own NURBS library.    pederb, 20000926
        SoNurbsProfile * np = (SoNurbsProfile*) pn;
        float * knots;
        int numknots;
        int dim;
        int numpts;
        float * points;
        np->getTrimCurve(state, numpts, points, dim,
                         numknots, knots);
        for (int j = 0; j < numpts; j++) {
          if (-points[j*dim] > maxz) maxz = -points[j*dim];
          if (-points[j*dim] < minz) minz = -points[j*dim];
          if (points[j*dim+1] > profsize) profsize = points[j*dim+1];
        }
      }
      else {
        int num;
        SbVec2f *coords;
        pn->getVertices(state, num, coords);
        for (int j = 0; j < num; j++) {
          if (-coords[j][0] > maxz) maxz = -coords[j][0];
          if (-coords[j][0] < minz) minz = -coords[j][0];
          if (coords[j][1] > profsize) profsize = coords[j][1];
        }
      }
    }
  }
  else {
    // extrude
    if (this->parts.getValue() == SoText3::BACK) {
      maxz = -1.0f;
    }
    else if (this->parts.getValue() == SoText3::FRONT) {
      minz = 0.0f;
    }
  }
  minx -= profsize;
  maxx += profsize;
  miny -= profsize;
  maxy += profsize;
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
  COIN_OBSOLETED();
  return SbBox3f();
}

// doc in parent
void
SoText3::GLRender(SoGLRenderAction * action)
{
  if (!this->shouldGLRender(action)) return;

  SoState * state = action->getState();
  THIS->setUpGlyphs(state, this);

  SoMaterialBindingElement::Binding binding =
    SoMaterialBindingElement::get(state);

  SbBool matperpart = binding != SoMaterialBindingElement::OVERALL;

  SoMaterialBundle mb(action);
  mb.sendFirst();

  unsigned int prts = this->parts.getValue();

  // we will provide unit length normals
  SoGLNormalizeElement::forceSend(state, TRUE);

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
    // will cause a call to generatePrimitives() 
    // slow, but we can't be bothered to implement a new loop to count triangles
    inherited::getPrimitiveCount(action);
  }
  else {
    action->addNumText(this->string.getNum());
  }
}

// doc in parent
void
SoText3::generatePrimitives(SoAction * action)
{
  THIS->setUpGlyphs(action->getState(), this);

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

// render text geometry
void
SoText3::render(SoState * state, unsigned int part)
{
  float size = SoFontSizeElement::get(state);
  // FIXME: not in use (yet?). 20000525 mortene.
  // SbBool doTextures = SoGLTextureEnabledElement::get(state);
  // SbBool do3DTextures = SoGLTexture3EnabledElement::get(state);
  int i, n = THIS->widths.getLength();

  int firstprofile = -1;
  int profnum;
  SbVec2f *profcoords;
  float nearz =  FLT_MAX;
  float farz  = -FLT_MAX;

  const SoNodeList profilenodes = SoProfileElement::get(state);
  int numprofiles = profilenodes.getLength();
  if ( numprofiles > 0) {
    assert(profilenodes[0]->getTypeId().isDerivedFrom(SoProfile::getClassTypeId()));
    // Find near/far z (for modifying position of front/back)
    for (int l=numprofiles-1; l>=0; l--) {
      SoProfile *pn = (SoProfile *)profilenodes[l];
      pn->getVertices(state, profnum, profcoords);
      if (profnum > 0) {
        if (profcoords[profnum-1][0] > farz) farz = profcoords[profnum-1][0];
        if (profcoords[0][0] < nearz) nearz = profcoords[0][0];
        if (pn->linkage.getValue() == SoProfile::START_FIRST) {
          if (firstprofile == -1) firstprofile = l;
          break;
        }
      }
    }
    nearz = -nearz;
    farz = -farz;
  }
  else {
    nearz = 0.0;
    farz = -1.0;
  }
  if (part != SoText3::SIDES) {
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
      xpos = -THIS->widths[i] * size;
      break;
    case SoText3::CENTER:
      xpos = - THIS->widths[i] * size * 0.5f;
      break;
    }

    const char * str = this->string[i].getString();
    while (*str++) {
      assert((glyphidx < THIS->glyphs.getLength()) && "glyph-list not in sync");
      const SoGlyph * glyph = THIS->glyphs[glyphidx++];
      const SbVec2f * coords = glyph->getCoords();
      if (part != SoText3::SIDES) {  // FRONT & BACK
        const int * ptr = glyph->getFaceIndices();
        while (*ptr >= 0) {
          SbVec2f v0, v1, v2;
          float zval;
          if (part == SoText3::FRONT) {
            v0 = coords[*ptr++];
            v1 = coords[*ptr++];
            v2 = coords[*ptr++];
            zval = nearz;
          }
          else {  // BACK
            v2 = coords[*ptr++];
            v1 = coords[*ptr++];
            v0 = coords[*ptr++];
            zval = farz;
          }
          glVertex3f(v0[0] * size + xpos, v0[1] * size + ypos, zval);
          glVertex3f(v1[0] * size + xpos, v1[1] * size + ypos, zval);
          glVertex3f(v2[0] * size + xpos, v2[1] * size + ypos, zval);
        }
      }
      else { // SIDES
        if (profilenodes.getLength() == 0) {  // no profile - extrude
          const int * ptr = glyph->getEdgeIndices();
          SbVec2f v0, v1;
          glBegin(GL_QUADS);
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
          glEnd();
        }
        else {  // profile
          const int *indices = glyph->getEdgeIndices();
          int ind = 0;
          while (*indices >= 0) {
            int i0 = *indices++;
            int i1 = *indices++;
            SbVec3f va(coords[i0][0], coords[i0][1], nearz);
            SbVec3f vb(coords[i1][0], coords[i1][1], nearz);
            int *ccw = (int *)glyph->getNextCCWEdge(ind);
            int *cw  = (int *)glyph->getNextCWEdge(ind);
            SbVec3f vc(coords[*(ccw+1)][0], coords[*(ccw+1)][1], nearz);
            SbVec3f vd(coords[*cw][0], coords[*cw][1], nearz);
            ind++;

            va[0] = va[0] * size + xpos;
            va[1] = va[1] * size + ypos;
            vb[0] = vb[0] * size + xpos;
            vb[1] = vb[1] * size + ypos;
            vc[0] = vc[0] * size + xpos;
            vc[1] = vc[1] * size + ypos;
            vd[0] = vd[0] * size + xpos;
            vd[1] = vd[1] * size + ypos;

            // create two 'normal' vectors pointing out from the edges, for aligning the profile
            SbVec3f tmp1(va[0]-vc[0], va[1]-vc[1], 0.0f);
            tmp1 = tmp1.cross(SbVec3f(0.0f, 0.0f,  -1.0f));
            tmp1.normalize();

            SbVec3f tmp2(vd[0]-vb[0], vd[1]-vb[1], 0.0f);
            tmp2 = tmp2.cross(SbVec3f(0.0f, 0.0f,  -1.0f));
            tmp2.normalize();

            SoProfile *pn = (SoProfile *)profilenodes[firstprofile];
            pn->getVertices(state, profnum, profcoords);

            SbVec3f edgea( va[0]+(profcoords[0][1]*tmp2[0]), va[1]+(profcoords[0][1]*tmp2[1]), -profcoords[0][0] );
            SbVec3f edgeb( vb[0]+(profcoords[0][1]*tmp1[0]), vb[1]+(profcoords[0][1]*tmp1[1]), -profcoords[0][0] );
            float edgez = -profcoords[0][0];  // -----
            // look through all profiles.
            int twisted = 0;
            for (int j=firstprofile; j<numprofiles; j++) {
              SoProfile *pn = (SoProfile *)profilenodes[j];
              pn->getVertices(state, profnum, profcoords);
              // iterate through all profile coords, drawing quads (and calculating normals)
              glBegin(GL_QUADS);
              for (int k=0; k<profnum; k++) {
                if (profcoords[k][0] != 0) {

                  vd[0] = va[0] + ((profcoords[k][1] * tmp2[0]));
                  vd[1] = va[1] + ((profcoords[k][1] * tmp2[1]));
                  vd[2] = -profcoords[k][0];
                  vc[0] = vb[0] + ((profcoords[k][1] * tmp1[0]));
                  vc[1] = vb[1] + ((profcoords[k][1] * tmp1[1]));
                  vc[2] = -profcoords[k][0];
                  // normal
                  SbVec3f normal( vd[0]-edgea[0], vd[1]-edgea[1], -profcoords[k][0] - edgez );
                  normal = normal.cross( SbVec3f( edgeb[0]-edgea[0], edgeb[1]-edgea[1], 0 ) );
                  // FIXME: check if 'valid' normals (resulting triangle instead if quad, etc), 20000926 skei.
                  if (normal.length() > 0) {
                    normal.normalize();
                    //check if resulting quad-edges will cross
                    SbVec3f edge1 = edgeb-edgea;
                    SbVec3f edge2 = vc-vd;
                    //leftedge.normalize();          // ok not to normalize those? skei
                    //rightedge.normalize();
                    if (edge1.dot(edge2) < 0) {
                      SbLine leftline(edgeb,vc);
                      SbLine rightline(edgea,vd);
                      SbVec3f inter1,inter2;
                      leftline.getClosestPoints(rightline,inter1,inter2);
                      if (twisted == 0) {
                        glNormal3f( normal[0], normal[1], normal[2] );
                        glVertex3f(edgeb[0], edgeb[1], edgez);
                        glVertex3f(edgea[0], edgea[1], edgez);
                        glVertex3f(inter1[0],inter1[1],inter1[2]);
                        glVertex3f(inter1[0],inter1[1],inter1[2]);
                        twisted = 1;
                      }
                      else {
                        glNormal3f( -normal[0], -normal[1], -normal[2] );
                        glVertex3f(inter1[0],inter1[1],inter1[2]);
                        glVertex3f(inter1[0],inter1[1],inter1[2]);
                        glVertex3f(vd[0], vd[1], -profcoords[k][0] );
                        glVertex3f(vc[0], vc[1], -profcoords[k][0] );
                        twisted = 0;
                      }
                    }
                    else {
                      glNormal3f( normal[0], normal[1], normal[2] );
                      // vertices
                      glVertex3f(edgeb[0], edgeb[1], edgez);
                      glVertex3f(edgea[0], edgea[1], edgez);
                      glVertex3f(vd[0], vd[1], -profcoords[k][0] );
                      glVertex3f(vc[0], vc[1], -profcoords[k][0] );
                    }
                  }
                  edgeb = vc;
                  edgea = vd;
                  edgez = -profcoords[k][0];
                }
              }
              glEnd();
            }
          }
        }
      }
      xpos += glyph->getWidth() * size;
    }
    ypos -= size * this->spacing.getValue();
  }
  if (part != SoText3::SIDES) glEnd();
}

// generate text geometry
void
SoText3::generate(SoAction * action, unsigned int part)
{
  SoState * state = action->getState();

  int matidx = 0;
  if (SoMaterialBindingElement::get(state) != 
      SoMaterialBindingElement::OVERALL) {
    if (part == SoText3::SIDES) matidx = 1;
    else if (part == SoText3::BACK) matidx = 2;
  }

  SoPrimitiveVertex vertex;
  SoTextDetail detail;
  detail.setPart(part);
  vertex.setDetail(&detail);
  vertex.setMaterialIndex(matidx);

  float size = SoFontSizeElement::get(state);
  int i, n = THIS->widths.getLength();

  int firstprofile = -1;
  int profnum;
  SbVec2f *profcoords;
  float nearz =  FLT_MAX;
  float farz  = -FLT_MAX;

  const SoNodeList profilenodes = SoProfileElement::get(state);
  int numprofiles = profilenodes.getLength();
  if ( numprofiles > 0) {
    assert(profilenodes[0]->getTypeId().isDerivedFrom(SoProfile::getClassTypeId()));
    // Find near/far z (for modifying position of front/back)
    for (int l = numprofiles-1; l >= 0; l--) {
      SoProfile * pn = (SoProfile *)profilenodes[l];
      pn->getVertices(state, profnum, profcoords);
      if (profnum > 0) {
        if (profcoords[profnum-1][0] > farz) farz = profcoords[profnum-1][0];
        if (profcoords[0][0] < nearz) nearz = profcoords[0][0];
        if (pn->linkage.getValue() == SoProfile::START_FIRST) {
          if (firstprofile == -1) firstprofile = l;
          break;
        }
      }
    }
    nearz = -nearz;
    farz = -farz;
  }
  else {
    nearz = 0.0;
    farz = -1.0;
  }
  if (part != SoText3::SIDES) {
    this->beginShape(action, SoShape::TRIANGLES, NULL);
    if (part == SoText3::FRONT)
      vertex.setNormal(SbVec3f(0.0f, 0.0f, 1.0f));
    else
      vertex.setNormal(SbVec3f(0.0f, 0.0f, -1.0f));
  }
  int glyphidx = 0;
  float ypos = 0.0f;

  for (i = 0; i < n; i++) {
    detail.setStringIndex(i);
    float xpos = 0.0f;
    switch (this->justification.getValue()) {
    case SoText3::RIGHT:
      xpos = -THIS->widths[i] * size;
      break;
    case SoText3::CENTER:
      xpos = - THIS->widths[i] * size * 0.5f;
      break;
    }

    const char * str = this->string[i].getString();
    
    int charidx = 0;

    while (*str++) {
      detail.setCharacterIndex(charidx++);
      const SoGlyph * glyph = THIS->glyphs[glyphidx++];
      const SbVec2f * coords = glyph->getCoords();
      if (part != SoText3::SIDES) {  // FRONT & BACK
        const int * ptr = glyph->getFaceIndices();
        while (*ptr >= 0) {
          SbVec2f v0, v1, v2;
          float zval;
          if (part == SoText3::FRONT) {
            v0 = coords[*ptr++];
            v1 = coords[*ptr++];
            v2 = coords[*ptr++];
            zval = nearz;
          }
          else {  // BACK
            v2 = coords[*ptr++];
            v1 = coords[*ptr++];
            v0 = coords[*ptr++];
            zval = farz;
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
        if (profilenodes.getLength() == 0) {  // no profile - extrude
          const int * ptr = glyph->getEdgeIndices();
          SbVec2f v0, v1;
          this->beginShape(action, SoShape::QUADS, NULL);
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
          this->endShape();
        }
        else {  // profile
          const int *indices = glyph->getEdgeIndices();
          int ind = 0;
          while (*indices >= 0) {
            int i0 = *indices++;
            int i1 = *indices++;
            SbVec3f va(coords[i0][0], coords[i0][1], nearz);
            SbVec3f vb(coords[i1][0], coords[i1][1], nearz);
            int *ccw = (int *)glyph->getNextCCWEdge(ind);
            int *cw  = (int *)glyph->getNextCWEdge(ind);
            SbVec3f vc(coords[*(ccw+1)][0], coords[*(ccw+1)][1], nearz);
            SbVec3f vd(coords[*cw][0], coords[*cw][1], nearz);
            ind++;

            va[0] = va[0] * size + xpos;
            va[1] = va[1] * size + ypos;
            vb[0] = vb[0] * size + xpos;
            vb[1] = vb[1] * size + ypos;
            vc[0] = vc[0] * size + xpos;
            vc[1] = vc[1] * size + ypos;
            vd[0] = vd[0] * size + xpos;
            vd[1] = vd[1] * size + ypos;

            // create two 'normal' vectors pointing out from the edges, for aligning the profile
            SbVec3f tmp1(va[0]-vc[0], va[1]-vc[1], 0.0f);
            tmp1 = tmp1.cross(SbVec3f(0.0f, 0.0f,  -1.0f));
            tmp1.normalize();

            SbVec3f tmp2(vd[0]-vb[0], vd[1]-vb[1], 0.0f);
            tmp2 = tmp2.cross(SbVec3f(0.0f, 0.0f,  -1.0f));
            tmp2.normalize();

            SoProfile *pn = (SoProfile *)profilenodes[firstprofile];
            pn->getVertices(state, profnum, profcoords);

            SbVec3f edgea( va[0]+(profcoords[0][1]*tmp2[0]), va[1]+(profcoords[0][1]*tmp2[1]), -profcoords[0][0] );
            SbVec3f edgeb( vb[0]+(profcoords[0][1]*tmp1[0]), vb[1]+(profcoords[0][1]*tmp1[1]), -profcoords[0][0] );
            float edgez = -profcoords[0][0];  // -----
            // look through all profiles.
            int twisted = 0;
            for (int j=firstprofile; j<numprofiles; j++) {
              SoProfile *pn = (SoProfile *)profilenodes[j];
              pn->getVertices(state, profnum, profcoords);
              // iterate through all profile coords, drawing quads (and calculating normals)
              this->beginShape(action, SoShape::QUADS, NULL);
              for (int k=0; k<profnum; k++) {
                if (profcoords[k][0] != 0) {
                  vd[0] = va[0] + ((profcoords[k][1] * tmp2[0]));
                  vd[1] = va[1] + ((profcoords[k][1] * tmp2[1]));
                  vd[2] = -profcoords[k][0];
                  vc[0] = vb[0] + ((profcoords[k][1] * tmp1[0]));
                  vc[1] = vb[1] + ((profcoords[k][1] * tmp1[1]));
                  vc[2] = -profcoords[k][0];
                  // normal
                  SbVec3f normal( vd[0]-edgea[0], vd[1]-edgea[1], -profcoords[k][0] - edgez );
                  normal = normal.cross( SbVec3f( edgeb[0]-edgea[0], edgeb[1]-edgea[1], 0 ) );
                  // FIXME: check if 'valid' normals (resulting triangle instead if quad, etc), 20000926 skei.
                  if (normal.length() > 0) {
                    normal.normalize();
                    //check if resulting quad-edges will cross
                    SbVec3f edge1 = edgeb-edgea;
                    SbVec3f edge2 = vc-vd;
                    //leftedge.normalize();          // ok not to normalize those? skei
                    //rightedge.normalize();
                    if (edge1.dot(edge2) < 0) {
                      SbLine leftline(edgeb,vc);
                      SbLine rightline(edgea,vd);
                      SbVec3f inter1,inter2;
                      leftline.getClosestPoints(rightline,inter1,inter2);
                      if (twisted == 0) {
                        vertex.setNormal(normal);
                        vertex.setPoint(SbVec3f(edgeb[0], edgeb[1], edgez));
                        this->shapeVertex(&vertex);
                        vertex.setPoint(SbVec3f(edgea[0], edgea[1], edgez));
                        this->shapeVertex(&vertex);
                        vertex.setPoint(SbVec3f(inter1[0],inter1[1],inter1[2]));
                        this->shapeVertex(&vertex);
                        vertex.setPoint(SbVec3f(inter1[0],inter1[1],inter1[2]));
                        this->shapeVertex(&vertex);
                        twisted = 1;
                      }
                      else {
                        vertex.setNormal(-normal);
                        vertex.setPoint(SbVec3f(inter1[0],inter1[1],inter1[2]));
                        this->shapeVertex(&vertex);
                        vertex.setPoint(SbVec3f(inter1[0],inter1[1],inter1[2]));
                        this->shapeVertex(&vertex);
                        vertex.setPoint(SbVec3f(vd[0], vd[1], -profcoords[k][0]));
                        this->shapeVertex(&vertex);
                        vertex.setPoint(SbVec3f(vc[0], vc[1], -profcoords[k][0]));
                        this->shapeVertex(&vertex);
                        twisted = 0;
                      }
                    }
                    else {
                      vertex.setNormal(normal);
                      vertex.setPoint(SbVec3f(edgeb[0], edgeb[1], edgez));
                      this->shapeVertex(&vertex);
                      vertex.setPoint(SbVec3f(edgea[0], edgea[1], edgez));
                      this->shapeVertex(&vertex);
                      vertex.setPoint(SbVec3f(vd[0], vd[1], -profcoords[k][0]));
                      this->shapeVertex(&vertex);
                      vertex.setPoint(SbVec3f(vc[0], vc[1], -profcoords[k][0]));
                      this->shapeVertex(&vertex);
                    }
                  }
                  edgeb = vc;
                  edgea = vd;
                  edgez = -profcoords[k][0];
                }
              }
              this->endShape();
            }
          }
        }
      }
      xpos += glyph->getWidth() * size;
    }
    ypos -= size * this->spacing.getValue();
  }
  if (part != SoText3::SIDES) this->endShape();

}

/*!
  Overloaded to detect when the string field changes.
*/
void 
SoText3::notify(SoNotList * list)
{
  SoField * f = list->getLastField();
  if (f == &this->string) THIS->needsetup = TRUE;
  inherited::notify(list);
}

#undef THIS

// SoText3P methods are below
#ifndef DOXYGEN_SKIP_THIS

// recalculate glyphs
void
SoText3P::setUpGlyphs(SoState * state, SoText3 * textnode)
{
  // Note that this code is duplicated in SoAsciiText::setUpGlyphs(),
  // so migrate bugfixes and other improvements.
  const SbString & dummy = textnode->string[0];

  if (!this->needsetup) return;
  this->needsetup = FALSE;

  // store old glyphs to avoid freeing glyphs too soon
  SbList <const SoGlyph *> oldglyphs;
  oldglyphs = this->glyphs;
  this->glyphs.truncate(0);
  this->widths.truncate(0);

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
    this->widths.append(width);
  }

  // unref old glyphs
  for (int j = 0; j < oldglyphs.getLength(); j++) oldglyphs[j]->unref();
}

#endif // DOXYGEN_SKIP_THIS
