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
#include <Inventor/elements/SoGLShapeHintsElement.h>
#include <Inventor/elements/SoGLTextureEnabledElement.h>
#include <Inventor/elements/SoMaterialBindingElement.h>
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/elements/SoComplexityTypeElement.h>
#include <Inventor/elements/SoComplexityElement.h>
#include <Inventor/misc/SoGlyph.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/nodes/SoProfile.h>
#include <Inventor/nodes/SoNurbsProfile.h>
#include <Inventor/SbLine.h>
#include <string.h>

#include <coindefs.h> // COIN_OBSOLETED()

#include <float.h> // FLT_MAX, FLT_MIN

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H
#include <Inventor/system/gl.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#include "../fonts/glyph3d.h"

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

class SoText3P {
public:
  SoText3P(SoText3 * master) : master(master) { }

  void render(SoState * state, const cc_font_specification * fontspec, unsigned int part);
  void generate(SoAction * action, const cc_font_specification * fontspec, unsigned int part);

  SbList <float> widths;
  void setUpGlyphs(SoState * state, const cc_font_specification * fontspec, SoText3 * textnode);
  SbBool needsetup;
  SbBox3f maxglyphbbox;

  cc_string * prevfontname; // Store important fontspecs so that changes can be detected
  cc_string * prevfontstyle;
  float prevfontsize;

private:
  SoText3 * master;
};

#define PRIVATE(p) ((p)->pimpl)
#define PUBLIC(p) ((p)->master)

// *************************************************************************

SO_NODE_SOURCE(SoText3);

// *************************************************************************

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

  PRIVATE(this) = new SoText3P(this);
  PRIVATE(this)->needsetup = TRUE;
  PRIVATE(this)->prevfontname = cc_string_construct_new();
  PRIVATE(this)->prevfontstyle = cc_string_construct_new();
  PRIVATE(this)->prevfontsize = -1;
}

SoText3::~SoText3()
{
  delete PRIVATE(this);
}

// doc in parent
void
SoText3::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoText3, SO_FROM_INVENTOR_2_1);
}

// doc in parent
void
SoText3::computeBBox(SoAction * action, SbBox3f & box, SbVec3f & center)
{
  SoState * state = action->getState();

  cc_font_specification fontspec;
  cc_fontspec_construct(&fontspec, SoFontNameElement::get(state).getString(),
                        SoFontSizeElement::get(state),
                        SoComplexityElement::get(state));

  PRIVATE(this)->setUpGlyphs(state, &fontspec, this);
  int i, n = PRIVATE(this)->widths.getLength();
  if (n == 0) {
    center = SbVec3f(0.0f, 0.0f, 0.0f);
    box.setBounds(center, center);
    cc_fontspec_clean(&fontspec);
    return;
  }

  float maxw = FLT_MIN;
  for (i = 0; i < n; i++) 
    maxw = SbMax(maxw, PRIVATE(this)->widths[i]); 
  
  if(maxw == FLT_MIN) { // There is no text to bound. Returning.
    box.setBounds(SbVec3f(0.0f, 0.0f, 0.0f), SbVec3f(0.0f, 0.0f, 0.0f));
    center = SbVec3f(0,0,0);
    return; 
  }

  SbBox2f maxbox;
  float maxglyphsize = 1;
  
  float maxy = fontspec.size;
  float miny = this->spacing.getValue() * fontspec.size * (n-1);

  float minx, maxx;
  switch (this->justification.getValue()) {
  case SoText3::LEFT:
    minx = 0.0f;
    maxx = maxw;
    break;
  case SoText3::RIGHT:
    minx = -maxw;
    maxx = 0.0f;
    break;
  case SoText3::CENTER:
    maxx = maxw * 0.5f;
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
        int32_t numknots;
        int dim;
        int32_t numpts;
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
        int32_t num;
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

  // Expanding bbox so that glyphs like 'j's and 'q's are completely inside.
  box.extendBy(SbVec3f(0,PRIVATE(this)->maxglyphbbox.getMin()[1] - (n-1) * fontspec.size, 0));
  box.extendBy(PRIVATE(this)->maxglyphbbox);

  center = box.getCenter();

  cc_fontspec_clean(&fontspec);
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

  cc_font_specification fontspec;
  cc_fontspec_construct(&fontspec, SoFontNameElement::get(state).getString(),
                        SoFontSizeElement::get(state),
                        this->getComplexityValue(state->getAction()));

  PRIVATE(this)->setUpGlyphs(state, &fontspec, this);

  SoMaterialBindingElement::Binding binding =
    SoMaterialBindingElement::get(state);

  SbBool matperpart = (binding != SoMaterialBindingElement::OVERALL);

  SoMaterialBundle mb(action);
  mb.sendFirst();

  unsigned int prts = this->parts.getValue();

  if (prts & SoText3::FRONT) {
    PRIVATE(this)->render(state, &fontspec, SoText3::FRONT);
  }
  if (prts & SoText3::SIDES) {
    if (matperpart) mb.send(1, FALSE);
    PRIVATE(this)->render(state, &fontspec, SoText3::SIDES);
  }
  if (prts & SoText3::BACK) {
    if (matperpart) mb.send(2, FALSE);
    PRIVATE(this)->render(state, &fontspec, SoText3::BACK);
  }
  
  if (SoComplexityTypeElement::get(state) == SoComplexityTypeElement::OBJECT_SPACE) 
    SoGLCacheContextElement::shouldAutoCache(state, SoGLCacheContextElement::DO_AUTO_CACHE);
    
  cc_fontspec_clean(&fontspec);
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
  SoState * state = action->getState();

  cc_font_specification fontspec;
  cc_fontspec_construct(&fontspec, SoFontNameElement::get(state).getString(),
                        SoFontSizeElement::get(state),
                        this->getComplexityValue(state->getAction()));

  PRIVATE(this)->setUpGlyphs(state, &fontspec, this);

  unsigned int prts = this->parts.getValue();

  if (prts & SoText3::FRONT) {
    PRIVATE(this)->generate(action, &fontspec, SoText3::FRONT);
  }
  if (prts & SoText3::SIDES) {
    PRIVATE(this)->generate(action, &fontspec, SoText3::SIDES);
  }
  if (prts & SoText3::BACK) {
    PRIVATE(this)->generate(action, &fontspec, SoText3::BACK);
  }

  cc_fontspec_clean(&fontspec);
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

void
SoText3P::render(SoState * state, const cc_font_specification * fontspec,
                 unsigned int part)
{
  int i, n = this->widths.getLength();

  // FIXME: not in use (yet?). 20000525 mortene.
  //  SbBool doTextures = SoGLTextureEnabledElement::get(state);
  //  SbBool do3DTextures = SoGLTexture3EnabledElement::get(state);

  int firstprofile = -1;
  int32_t profnum;
  SbVec2f *profcoords;
  float nearz =  FLT_MAX;
  float farz  = -FLT_MAX;

  const SoNodeList profilenodes = SoProfileElement::get(state);
  int numprofiles = profilenodes.getLength();

  if (numprofiles > 0) {
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
    switch (PUBLIC(this)->justification.getValue()) {
    case SoText3::RIGHT:
      xpos = -this->widths[i];
      break;
    case SoText3::CENTER:
      xpos = - this->widths[i] * 0.5f;
      break;
    }

 
    const unsigned int length = PUBLIC(this)->string[i].getLength();
    for (unsigned int strcharidx = 0; strcharidx < length; strcharidx++) {
 
      // Note that the "unsigned char" cast is needed to avoid 8-bit
      // chars using the highest bit (i.e. characters above the ASCII
      // set up to 127) be expanded to huge int numbers that turn
      // negative when casted to integer size.
      const uint32_t glyphidx = (const unsigned char) PUBLIC(this)->string[i][strcharidx];
      const cc_glyph3d * glyph = cc_glyph3d_getglyph(glyphidx, fontspec);
      
      float glyphwidth = cc_glyph3d_getwidth(glyph);
      if (glyphwidth == 0) 
        glyphwidth = 1.0f/3.0f; // SPACE width is set to 'font size'/3
      
      const SbVec2f * coords = (SbVec2f *) cc_glyph3d_getcoords(glyph);

      if (part != SoText3::SIDES) {  // FRONT & BACK
        const int * ptr = cc_glyph3d_getfaceindices(glyph);

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
          glVertex3f(v0[0] * fontspec->size + xpos, v0[1] * fontspec->size + ypos, zval);
          glVertex3f(v1[0] * fontspec->size + xpos, v1[1] * fontspec->size + ypos, zval);
          glVertex3f(v2[0] * fontspec->size + xpos, v2[1] * fontspec->size + ypos, zval);
        }
      }
      else { // SIDES
        if (profilenodes.getLength() == 0) {  // no profile - extrude
          const int * ptr = cc_glyph3d_getedgeindices(glyph);
          SbVec2f v0, v1;
          int counter = 0;
          glBegin(GL_QUADS);

          while (*ptr >= 0) {            
            v0 = coords[*ptr++];
            v1 = coords[*ptr++];

            SbVec3f tmp(v0[0]-v1[0], v0[1] - v1[1], 0.0f);
            SbVec3f normal = tmp.cross(SbVec3f(0.0f, 0.0f, 1.0f));
            normal.normalize();
            v0[0] = v0[0] * fontspec->size + xpos;
            v0[1] = v0[1] * fontspec->size + ypos;
            v1[0] = v1[0] * fontspec->size + xpos;
            v1[1] = v1[1] * fontspec->size + ypos;
            glNormal3fv(normal.getValue());
            glVertex3f(v1[0], v1[1], 0.0f);
            glVertex3f(v0[0], v0[1], 0.0f);
            glVertex3f(v0[0], v0[1], -1.0f);
            glVertex3f(v1[0], v1[1], -1.0f);
          }
          glEnd();
        }
        else {  // profile
          const int *indices = cc_glyph3d_getedgeindices(glyph);
          int ind = 0;

          while (*indices >= 0) {
            int i0 = *indices++;
            int i1 = *indices++;
            SbVec3f va(coords[i0][0], coords[i0][1], nearz);
            SbVec3f vb(coords[i1][0], coords[i1][1], nearz);
            int *ccw = (int *) cc_glyph3d_getnextccwedge(glyph, ind);
            int *cw  = (int *) cc_glyph3d_getnextcwedge(glyph, ind);
            SbVec3f vc(coords[*(ccw+1)][0], coords[*(ccw+1)][1], nearz);
            SbVec3f vd(coords[*cw][0], coords[*cw][1], nearz);
            ind++;

            va[0] = va[0] * fontspec->size + xpos;
            va[1] = va[1] * fontspec->size + ypos;
            vb[0] = vb[0] * fontspec->size + xpos;
            vb[1] = vb[1] * fontspec->size + ypos;
            vc[0] = vc[0] * fontspec->size + xpos;
            vc[1] = vc[1] * fontspec->size + ypos;
            vd[0] = vd[0] * fontspec->size + xpos;
            vd[1] = vd[1] * fontspec->size + ypos;

            // create two 'normal' vectors pointing out from the
            // edges, for aligning the profile
            SbVec3f tmp1(vc[0]-va[0], vc[1]-va[1], 0.0f);
            tmp1 = tmp1.cross(SbVec3f(0.0f, 0.0f,  -1.0f));
            if (tmp1.length() > 0) 
              tmp1.normalize(); // Generates a warning on Win32, not on Linux... (handegar)

            SbVec3f tmp2(vb[0]-vd[0], vb[1]-vd[1], 0.0f);
            tmp2 = tmp2.cross(SbVec3f(0.0f, 0.0f,  -1.0f));
            if (tmp2.length() > 0)
              tmp2.normalize();


            SoProfile *pn = (SoProfile *)profilenodes[firstprofile];
            pn->getVertices(state, profnum, profcoords);

            SbVec3f edgea(va[0]+(profcoords[0][1]*tmp2[0]), va[1]+(profcoords[0][1]*tmp2[1]), -profcoords[0][0] );
            SbVec3f edgeb(vb[0]+(profcoords[0][1]*tmp1[0]), vb[1]+(profcoords[0][1]*tmp1[1]), -profcoords[0][0] );
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
                  SbVec3f normal(edgea[0] - vd[0], edgea[1]-vd[1],  edgez + profcoords[k][0]);
                  normal = normal.cross(SbVec3f(edgeb[0]-edgea[0], edgeb[1]-edgea[1], 0));
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
      xpos += glyphwidth * fontspec->size;
    }
    ypos -= fontspec->size * PUBLIC(this)->spacing.getValue();
  }
  if (part != SoText3::SIDES) glEnd();
}

// render text geometry
void
SoText3::render(SoState * state, unsigned int part)
{
  assert(FALSE && "obsoleted");
}

void
SoText3::generate(SoAction * action, unsigned int part)
{
  assert(FALSE && "obsoleted");
}

// generate text geometry
void
SoText3P::generate(SoAction * action, const cc_font_specification * fontspec,
                   unsigned int part)
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

  int i, n = this->widths.getLength();

  int firstprofile = -1;
  int32_t profnum;
  SbVec2f *profcoords;
  float nearz =  FLT_MAX;
  float farz  = -FLT_MAX;

  const SoNodeList profilenodes = SoProfileElement::get(state);
  int numprofiles = profilenodes.getLength();

  if (numprofiles > 0) {
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
    PUBLIC(this)->beginShape(action, SoShape::TRIANGLES, NULL);
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
    switch (PUBLIC(this)->justification.getValue()) {
    case SoText3::RIGHT:
      xpos = -this->widths[i];
      break;
    case SoText3::CENTER:
      xpos = - this->widths[i] * 0.5f;
      break;
    }

    const unsigned int length = PUBLIC(this)->string[i].getLength();
    for (unsigned int strcharidx = 0; strcharidx < length; strcharidx++) {

      // Note that the "unsigned char" cast is needed to avoid 8-bit
      // chars using the highest bit (i.e. characters above the ASCII
      // set up to 127) be expanded to huge int numbers that turn
      // negative when casted to integer size.      
      const uint32_t glyphidx = (const unsigned char) PUBLIC(this)->string[i][strcharidx];
      const cc_glyph3d * glyph = cc_glyph3d_getglyph(glyphidx, fontspec);

      float glyphwidth = cc_glyph3d_getwidth(glyph);
      if (glyphwidth == 0) 
        glyphwidth = 1.0f/3.0f; // SPACE width is set to 'font size'/3
    
      const SbVec2f * coords = (SbVec2f *) cc_glyph3d_getcoords(glyph);
      detail.setCharacterIndex(strcharidx);

      if (part != SoText3::SIDES) {  // FRONT & BACK
        const int * ptr = cc_glyph3d_getfaceindices(glyph);

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
          vertex.setPoint(SbVec3f(v0[0] * fontspec->size + xpos, v0[1] * fontspec->size + ypos, zval));
          PUBLIC(this)->shapeVertex(&vertex);
          vertex.setPoint(SbVec3f(v1[0] * fontspec->size + xpos, v1[1] * fontspec->size + ypos, zval));
          PUBLIC(this)->shapeVertex(&vertex);
          vertex.setPoint(SbVec3f(v2[0] * fontspec->size + xpos, v2[1] * fontspec->size + ypos, zval));
          PUBLIC(this)->shapeVertex(&vertex);
        }
      }
      else { // SIDES
        if (profilenodes.getLength() == 0) {  // no profile - extrude
          const int * ptr = cc_glyph3d_getedgeindices(glyph);
          SbVec2f v0, v1;
          PUBLIC(this)->beginShape(action, SoShape::QUADS, NULL);

          while (*ptr >= 0) {
            v0 = coords[*ptr++];
            v1 = coords[*ptr++];

            SbVec3f tmp(v0[0]-v1[0], v0[1] - v1[1], 0.0f);
            SbVec3f normal = tmp.cross(SbVec3f(0.0f, 0.0f, 1.0f));
            normal.normalize();
            v0[0] = v0[0] * fontspec->size + xpos;
            v0[1] = v0[1] * fontspec->size + ypos;
            v1[0] = v1[0] * fontspec->size + xpos;
            v1[1] = v1[1] * fontspec->size + ypos;
            vertex.setNormal(normal);
            vertex.setPoint(SbVec3f(v1[0], v1[1], 0.0f));
            PUBLIC(this)->shapeVertex(&vertex);
            vertex.setPoint(SbVec3f(v0[0], v0[1], 0.0f));
            PUBLIC(this)->shapeVertex(&vertex);
            vertex.setPoint(SbVec3f(v0[0], v0[1], -1.0f));
            PUBLIC(this)->shapeVertex(&vertex);
            vertex.setPoint(SbVec3f(v1[0], v1[1], -1.0f));
            PUBLIC(this)->shapeVertex(&vertex);
          }
          PUBLIC(this)->endShape();
        }
        else {  // profile
          const int *indices = cc_glyph3d_getedgeindices(glyph);
          int ind = 0;

          while (*indices >= 0) {
            int i0 = *indices++;
            int i1 = *indices++;
            SbVec3f va(coords[i0][0], coords[i0][1], nearz);
            SbVec3f vb(coords[i1][0], coords[i1][1], nearz);
            int *ccw = (int *) cc_glyph3d_getnextccwedge(glyph, ind);
            int *cw  = (int *) cc_glyph3d_getnextcwedge(glyph, ind);
            SbVec3f vc(coords[*(ccw+1)][0], coords[*(ccw+1)][1], nearz);
            SbVec3f vd(coords[*cw][0], coords[*cw][1], nearz);
            ind++;

            va[0] = va[0] * fontspec->size + xpos;
            va[1] = va[1] * fontspec->size + ypos;
            vb[0] = vb[0] * fontspec->size + xpos;
            vb[1] = vb[1] * fontspec->size + ypos;
            vc[0] = vc[0] * fontspec->size + xpos;
            vc[1] = vc[1] * fontspec->size + ypos;
            vd[0] = vd[0] * fontspec->size + xpos;
            vd[1] = vd[1] * fontspec->size + ypos;

            // create two 'normal' vectors pointing out from the
            // edges, for aligning the profile
            SbVec3f tmp1(vc[0]-va[0], vc[1]-va[1], 0.0f);
            tmp1 = tmp1.cross(SbVec3f(0.0f, 0.0f,  -1.0f));
            if (tmp1.length() > 0)
              tmp1.normalize();

            SbVec3f tmp2(vb[0]-vd[0], vb[1]-vd[1], 0.0f);
            tmp2 = tmp2.cross(SbVec3f(0.0f, 0.0f,  -1.0f));
            if (tmp2.length() > 0)
              tmp2.normalize();

            SoProfile *pn = (SoProfile *)profilenodes[firstprofile];
            pn->getVertices(state, profnum, profcoords);

            SbVec3f edgea(va[0]+(profcoords[0][1]*tmp2[0]), va[1]+(profcoords[0][1]*tmp2[1]), -profcoords[0][0] );
            SbVec3f edgeb(vb[0]+(profcoords[0][1]*tmp1[0]), vb[1]+(profcoords[0][1]*tmp1[1]), -profcoords[0][0] );
            float edgez = -profcoords[0][0];  // -----
            // look through all profiles.
            int twisted = 0;

            for (int j=firstprofile; j<numprofiles; j++) {
              SoProfile *pn = (SoProfile *)profilenodes[j];
              pn->getVertices(state, profnum, profcoords);
              // iterate through all profile coords, drawing quads
              // (and calculating normals)
              PUBLIC(this)->beginShape(action, SoShape::QUADS, NULL);

              for (int k=0; k<profnum; k++) {
                if (profcoords[k][0] != 0) {
                  vd[0] = va[0] + ((profcoords[k][1] * tmp2[0]));
                  vd[1] = va[1] + ((profcoords[k][1] * tmp2[1]));
                  vd[2] = -profcoords[k][0];
                  vc[0] = vb[0] + ((profcoords[k][1] * tmp1[0]));
                  vc[1] = vb[1] + ((profcoords[k][1] * tmp1[1]));
                  vc[2] = -profcoords[k][0];
                  // normal
                  SbVec3f normal(edgea[0] - vd[0],edgea[1] - vd[1], edgez + profcoords[k][0]);
                  normal = normal.cross( SbVec3f( edgeb[0]-edgea[0], edgeb[1]-edgea[1], 0));
                  // FIXME: check if 'valid' normals (resulting
                  // triangle instead if quad, etc), 20000926 skei.

                  if (normal.length() > 0) {
                    normal.normalize();
                    //check if resulting quad-edges will cross
                    SbVec3f edge1 = edgeb-edgea;
                    SbVec3f edge2 = vc-vd;
                    //leftedge.normalize(); // ok not to normalize those? skei
                    //rightedge.normalize();

                    if (edge1.dot(edge2) < 0) {
                      SbLine leftline(edgeb,vc);
                      SbLine rightline(edgea,vd);
                      SbVec3f inter1,inter2;
                      leftline.getClosestPoints(rightline,inter1,inter2);

                      if (twisted == 0) {
                        vertex.setNormal(normal);
                        vertex.setPoint(SbVec3f(edgeb[0], edgeb[1], edgez));
                        PUBLIC(this)->shapeVertex(&vertex);
                        vertex.setPoint(SbVec3f(edgea[0], edgea[1], edgez));
                        PUBLIC(this)->shapeVertex(&vertex);
                        vertex.setPoint(SbVec3f(inter1[0],inter1[1],inter1[2]));
                        PUBLIC(this)->shapeVertex(&vertex);
                        vertex.setPoint(SbVec3f(inter1[0],inter1[1],inter1[2]));
                        PUBLIC(this)->shapeVertex(&vertex);
                        twisted = 1;
                      }
                      else {
                        vertex.setNormal(-normal);
                        vertex.setPoint(SbVec3f(inter1[0],inter1[1],inter1[2]));
                        PUBLIC(this)->shapeVertex(&vertex);
                        vertex.setPoint(SbVec3f(inter1[0],inter1[1],inter1[2]));
                        PUBLIC(this)->shapeVertex(&vertex);
                        vertex.setPoint(SbVec3f(vd[0], vd[1], -profcoords[k][0]));
                        PUBLIC(this)->shapeVertex(&vertex);
                        vertex.setPoint(SbVec3f(vc[0], vc[1], -profcoords[k][0]));
                        PUBLIC(this)->shapeVertex(&vertex);
                        twisted = 0;
                      }
                    }
                    else {
                      vertex.setNormal(normal);
                      vertex.setPoint(SbVec3f(edgeb[0], edgeb[1], edgez));
                      PUBLIC(this)->shapeVertex(&vertex);
                      vertex.setPoint(SbVec3f(edgea[0], edgea[1], edgez));
                      PUBLIC(this)->shapeVertex(&vertex);
                      vertex.setPoint(SbVec3f(vd[0], vd[1], -profcoords[k][0]));
                      PUBLIC(this)->shapeVertex(&vertex);
                      vertex.setPoint(SbVec3f(vc[0], vc[1], -profcoords[k][0]));
                      PUBLIC(this)->shapeVertex(&vertex);
                    }
                  }
                  edgeb = vc;
                  edgea = vd;
                  edgez = -profcoords[k][0];
                }
              }
              PUBLIC(this)->endShape();
            }
          }
        }
      }
      xpos += glyphwidth * fontspec->size;
    }
    ypos -= fontspec->size * PUBLIC(this)->spacing.getValue();
  }

  if (part != SoText3::SIDES) PUBLIC(this)->endShape();

}

// Documented in superclass.
void 
SoText3::notify(SoNotList * list)
{
  // Overridden to detect when the string field changes.
  SoField * f = list->getLastField();
  if (f == &this->string) PRIVATE(this)->needsetup = TRUE;
  inherited::notify(list);
}

// recalculate glyphs
void
SoText3P::setUpGlyphs(SoState * state, const cc_font_specification * fontspec, SoText3 * textnode)
{
  // Note that this code is duplicated in SoAsciiText::setUpGlyphs(),
  // so migrate bugfixes and other improvements.
 
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

  this->widths.truncate(0);

  for (int i = 0; i < textnode->string.getNum(); i++) {

    const unsigned int length = textnode->string[i].getLength(); 
    float stringwidth = 0.0f;
    float glyphwidth;
    const float * maxbbox;
    this->maxglyphbbox.makeEmpty();

    for (unsigned int strcharidx = 0; strcharidx < length; strcharidx++) {

      // Note that the "unsigned char" cast is needed to avoid 8-bit
      // chars using the highest bit (i.e. characters above the ASCII
      // set up to 127) be expanded to huge int numbers that turn
      // negative when casted to integer size.
      const uint32_t glyphidx = (const unsigned char) textnode->string[i][strcharidx];
      const cc_glyph3d * glyph = cc_glyph3d_getglyph(glyphidx, fontspec);
      assert(glyph);

      maxbbox = cc_glyph3d_getboundingbox(glyph); // Get max height
      this->maxglyphbbox.extendBy(SbVec3f(0, maxbbox[0] * fontspec->size, 0));
      this->maxglyphbbox.extendBy(SbVec3f(0, maxbbox[1] * fontspec->size, 0));

      glyphwidth = cc_glyph3d_getwidth(glyph);
      if (glyphwidth == 0)
        glyphwidth = 1.0f/3.0f; // SPACE width is always == 0.

      stringwidth += glyphwidth * fontspec->size;
    }
    this->widths.append(stringwidth);
  }

  // Make sure boundingbox is updated if this method was called due to
  // a fontspec change.
  this->master->touch();

}
