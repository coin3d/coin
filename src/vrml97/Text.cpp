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
  <http://www.web3d.org/technicalinfo/specifications/vrml97/part1/references.html#[UTF8]>).
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <float.h> // FLT_MIN
#include <stddef.h>

#include <Inventor/VRMLnodes/SoVRMLText.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/VRMLnodes/SoVRMLFontStyle.h>

#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/bundles/SoMaterialBundle.h>
#include <Inventor/details/SoTextDetail.h>
#include <Inventor/elements/SoGLTexture3EnabledElement.h>
#include <Inventor/elements/SoGLTextureEnabledElement.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/lists/SbList.h>
#include <Inventor/misc/SoGlyph.h>
#include <Inventor/system/gl.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/nodes/SoAsciiText.h>



static void fontstylechangeCB(void * data, SoSensor * sensor);

class SoVRMLTextP {
public:
  SoVRMLTextP(SoVRMLText * master) : master(master) { }
  SoVRMLText * master;

  float getWidth(const int idx, const float fontsize);
  SbList <const SoGlyph *> glyphs;
  SbList <float> glyphwidths;
  SbBool needsetup;
  void setUpGlyphs(SoState * state, SoVRMLText * textnode);

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

  // Default text setup
  PRIVATE(this)->textsize = 1.0f;
  PRIVATE(this)->textspacing = 1.0f;
  PRIVATE(this)->maxglyphheight = 1.0f;
  PRIVATE(this)->maxglyphwidth = 1.0f;
  PRIVATE(this)->lefttorighttext = TRUE;
  PRIVATE(this)->toptobottomtext = TRUE;
  PRIVATE(this)->horizontaltext = TRUE;
  PRIVATE(this)->justificationmajor = SoAsciiText::LEFT;
  PRIVATE(this)->justificationminor = SoAsciiText::LEFT;
  
  PRIVATE(this)->fontstylesensor = new SoFieldSensor(fontstylechangeCB, PRIVATE(this));
  PRIVATE(this)->fontstylesensor->attach(&fontStyle);
  PRIVATE(this)->fontstylesensor->setPriority(0);
  
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
  
  delete PRIVATE(this)->fontstylesensor;
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

  int i;
  const int n = this->string.getNum();

  glBegin(GL_TRIANGLES);
  glNormal3f(0.0f, 0.0f, 1.0f);

  int glyphidx = 0;
  const float spacing = PRIVATE(this)->textspacing * PRIVATE(this)->textsize;
  int maxstringchars = 0;
  float ypos = 0.0f;

  for (i=0;i<n;++i) 
    maxstringchars = SbMax(maxstringchars, this->string[i].getLength());


  for (i = 0; i < n; i++) {

    float xpos = 0.0f;
    const char * str = this->string[i].getString();
  
    // FIXME: According to reference images at the "NIST VTS v1.0"
    // test suite for VRML97 fonts, the string-stretching is not
    // exactly correct. The text seems to become abit
    // overstretched... This might be caused by the fact that Coin3D
    // seems to have a more compressed text in the first place (due to
    // the glyph values returned from SoGlyph). Go visit
    // http://xsun.sdct.itl.nist.gov/mkass-bin/write_list.pl?mix-compexp
    // for an example. (13Aug2003 handegar)

    float stretchlength = 0;
    if (i < this->length.getNum()) 
      stretchlength = this->length[i];
    float stretchfactor = (stretchlength * PRIVATE(this)->textsize) / strlen(str);
    
    float compressfactor = 1;
    if (this->maxExtent.getValue() > 0) {
      if (PRIVATE(this)->glyphwidths[i] > this->maxExtent.getValue()) {
        assert(PRIVATE(this)->glyphwidths[i] != 0 && "String length == 0! Cannot divide");
        compressfactor = (this->maxExtent.getValue() * PRIVATE(this)->textsize) / PRIVATE(this)->glyphwidths[i];
      }
    }
    
    
    if (PRIVATE(this)->horizontaltext) { // -- Horizontal text ----------------------
      
      switch (PRIVATE(this)->justificationmajor) {
      case SoAsciiText::LEFT:
        xpos = 0;
        break;
      case SoAsciiText::CENTER:
        if (PRIVATE(this)->lefttorighttext)
          xpos = - PRIVATE(this)->glyphwidths[i] * PRIVATE(this)->textsize * 0.5f;
        else
          xpos = PRIVATE(this)->glyphwidths[i] * PRIVATE(this)->textsize * 0.5f;
        break;
      case SoAsciiText::RIGHT:
        if (PRIVATE(this)->lefttorighttext)
          xpos = -PRIVATE(this)->glyphwidths[i] * PRIVATE(this)->textsize;
        else
          xpos = PRIVATE(this)->glyphwidths[i] * PRIVATE(this)->textsize;
        break;
      default:
        break;
      }

      switch (PRIVATE(this)->justificationminor) {
      case SoAsciiText::LEFT:
        break;
      case SoAsciiText::CENTER:
        ypos = (i * spacing) + ((n-1) * spacing) * 0.5f;
        break;
      case SoAsciiText::RIGHT:
        ypos = (i * spacing) + ((n-1) * spacing);
        break;        
      default:
        break;
      }
    
    }
    else { // -- Vertical text -----------------------------------------

      if (PRIVATE(this)->lefttorighttext)
        xpos = i * spacing;
      else
        xpos = -i * spacing;
        
      switch (PRIVATE(this)->justificationmajor) {
      case SoAsciiText::LEFT:        
        ypos = -PRIVATE(this)->maxglyphheight;//-PRIVATE(this)->textsize;
        break;
      case SoAsciiText::RIGHT:
        if (PRIVATE(this)->toptobottomtext)
          ypos = this->string[i].getLength() * spacing;
        else
          ypos = -this->string[i].getLength() * spacing;
        break;
      case SoAsciiText::CENTER:
        if (PRIVATE(this)->toptobottomtext)
          ypos = this->string[i].getLength() * PRIVATE(this)->textsize * 0.5f;
        else
          ypos = -this->string[i].getLength() * PRIVATE(this)->textsize * 0.5f;
        break;
      default:
        break;
      }

      switch (PRIVATE(this)->justificationminor) {
      case SoAsciiText::LEFT:
        break;
      case SoAsciiText::CENTER:
        xpos -= ((n-1) * spacing) * PRIVATE(this)->textsize * 0.5f;
        break;
      case SoAsciiText::RIGHT:
        xpos -= ((n-1) * spacing) * PRIVATE(this)->textsize;
        break;        
      default:
        break;
      }

    }
     

    const SoGlyph * lastglyph = NULL;

    while (*str++) {
      const SoGlyph * glyph = PRIVATE(this)->glyphs[glyphidx++];
      float width = glyph->getWidth();
      if (width == 0) 
        width = PRIVATE(this)->textsize / 3; // SPACE width is always == 0...

      // Kerning adjustments
      if (lastglyph != NULL) 
        xpos += ((float) lastglyph->getKerning(*PRIVATE(this)->glyphs[glyphidx-1])[0])/PRIVATE(this)->textsize; 
      lastglyph = glyph;


      const SbVec2f * coords = glyph->getCoords();
      const int * ptr = glyph->getFaceIndices();

      if (PRIVATE(this)->horizontaltext) {
        if (!PRIVATE(this)->lefttorighttext) {// Right to left text.
          xpos -= ((glyph->getWidth()*PRIVATE(this)->textsize) + stretchfactor) * compressfactor;
          if (lastglyph != NULL) 
            xpos -= ((float) lastglyph->getKerning(*PRIVATE(this)->glyphs[glyphidx-1])[0])/PRIVATE(this)->textsize;           
        }
      }             

      while ((*ptr >= 0) && (*ptr != -1)) {
        SbVec2f v0, v1, v2;
        v0 = coords[*ptr++];
        v1 = coords[*ptr++];
        v2 = coords[*ptr++];

        if (do2Dtextures) {
          glTexCoord2f(v0[0], v0[1]);
        }
        glVertex3f((v0[0]*PRIVATE(this)->textsize) + xpos, (v0[1]*PRIVATE(this)->textsize) + ypos, 0.0f);
        if (do2Dtextures) {
          glTexCoord2f(v1[0], v1[1]);
        }
        glVertex3f(v1[0] * PRIVATE(this)->textsize + xpos, v1[1] * PRIVATE(this)->textsize + ypos, 0.0f);
        if (do2Dtextures) {
          glTexCoord2f(v2[0], v2[1]);
        }
        glVertex3f(v2[0] * PRIVATE(this)->textsize + xpos, v2[1] * PRIVATE(this)->textsize + ypos, 0.0f);
      }
      
      if (!PRIVATE(this)->horizontaltext) {
        
        if (PRIVATE(this)->toptobottomtext)
          ypos -= PRIVATE(this)->textsize;//maxglyphheight;
        else 
          ypos += PRIVATE(this)->textsize;//maxglyphheight;
        
      } else if (PRIVATE(this)->lefttorighttext) {
        xpos += (width + stretchfactor) * compressfactor;
        //xpos += ((glyph->getWidth()*PRIVATE(this)->textsize) + stretchfactor) * compressfactor;
        
        
      }
    }

    if (PRIVATE(this)->horizontaltext) {
      if (PRIVATE(this)->toptobottomtext)
        ypos -= spacing * PRIVATE(this)->maxglyphheight;
      else
        ypos += spacing * PRIVATE(this)->maxglyphheight;
    }


   
  }
  glEnd();
}


// Doc in parent
void
SoVRMLText::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  PRIVATE(this)->setUpGlyphs(action->getState(), this);

  if (action->is3DTextCountedAsTriangles()) {
    const int n = PRIVATE(this)->glyphs.getLength();
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

  int i;
  const int n = this->string.getNum();
  if (n == 0 || PRIVATE(this)->glyphs.getLength() == 0) {
    center = SbVec3f(0.0f, 0.0f, 0.0f);
    box.setBounds(center, center);
    return;
  }

  const float linespacing = PRIVATE(this)->textspacing * PRIVATE(this)->maxglyphheight;

  float maxw = FLT_MIN;
  int maxstringchars = 0;
  for (i = 0; i < n; i++) {
    maxw = SbMax(maxw, PRIVATE(this)->glyphwidths[i]); 
    maxstringchars = SbMax(maxstringchars, this->string[i].getLength());
  }

  SbBox2f maxbox;
  int numglyphs = PRIVATE(this)->glyphs.getLength();
  for (i = 0; i < numglyphs; i++) 
    maxbox.extendBy(PRIVATE(this)->glyphs[i]->getBoundingBox());  
  float maxglyphsize = PRIVATE(this)->maxglyphheight;

  
  float maxlength = 0;
  for (i = 0; i < this->length.getNum();++i) 
    maxlength = SbMax(maxlength, this->length[i]);
  float stretchfactor = (maxlength * PRIVATE(this)->textsize);

  float maxy, miny;
  float minx, maxx;
  

  if (PRIVATE(this)->horizontaltext) {  // -- Horizontal text -----------------
    
    if (PRIVATE(this)->toptobottomtext) {
      maxy = PRIVATE(this)->textsize * maxbox.getMax()[1];
      miny = maxy - (maxglyphsize + (n-1) * PRIVATE(this)->textspacing * maxglyphsize);           
    } 
    else {
      miny = 0;
      maxy = (maxglyphsize + (n-1) * PRIVATE(this)->textspacing);    
    }
    
    minx = 0.0f;  
    maxx = maxw + stretchfactor;
 
    switch (PRIVATE(this)->justificationmajor) {
    case SoAsciiText::LEFT:
      break;
    case SoAsciiText::RIGHT:
      if (PRIVATE(this)->lefttorighttext) {
        maxx = 0;
        minx = -maxw  * PRIVATE(this)->textsize; 
      }
      else {
        minx = 0;
        maxx = maxw  * PRIVATE(this)->textsize;      
      }
      break;
    case SoAsciiText::CENTER:
      maxx -= maxw * PRIVATE(this)->textsize * 0.5f;
      minx -= maxw * PRIVATE(this)->textsize * 0.5f;
      break;
    default:
      assert(0 && "Unknown justification");
      minx = maxx = 0.0f;
      break;
    }
    
    switch (PRIVATE(this)->justificationminor) {
    case SoAsciiText::LEFT:
      break;
    case SoAsciiText::RIGHT:
      miny += ((n-1) * PRIVATE(this)->maxglyphheight);
      maxy += ((n-1) * PRIVATE(this)->maxglyphheight);
      break;
    case SoAsciiText::CENTER:
      miny += ((n-1) * PRIVATE(this)->maxglyphheight) * 0.5f;
      maxy += ((n-1) * PRIVATE(this)->maxglyphheight) * 0.5f;
      break;
    default:
      break;
    }

  }
  else { // -- Vertical text ----------------------------------------


    if (PRIVATE(this)->lefttorighttext) {
      minx = 0;
      maxx = ((n-1) * linespacing) + PRIVATE(this)->textsize;
    }
    else {
      // FIXME: This is probably not the right way of doing this. The
      // box tends to be abit larger on the right side than
      // needed. (14Aug2003 handegar)
      maxx = ((n-1) * linespacing * 0.5) - PRIVATE(this)->textsize;
      minx = -(n+2) * linespacing * 0.5f;
    }
    

    // FIXME: The 'stretchfactor' addon for vertical text not tested
    // yet... Might be wrong but it works for horizontal
    // text. (27Aug2003 handegar)
    if (PRIVATE(this)->toptobottomtext) {
      maxy = 0;
      miny = -maxstringchars - stretchfactor;
    }
    else {
      miny = 0;
      maxy = maxstringchars + stretchfactor;
    }

    switch (PRIVATE(this)->justificationmajor) {
    case SoAsciiText::LEFT:
      break;
    case SoAsciiText::RIGHT:
      maxy += maxstringchars;
      miny += maxstringchars;
      break;
    case SoAsciiText::CENTER:
      maxy += maxstringchars * 0.5f;
      miny += maxstringchars * 0.5f;
      break;
    default:
      assert(0 && "unknown justification");
      minx = maxx = 0.0f;
      break;
    }

    switch (PRIVATE(this)->justificationminor) {
    case SoAsciiText::LEFT:
      break;
    case SoAsciiText::CENTER:      
      maxx -= ((n-1) * PRIVATE(this)->maxglyphheight) * 0.5f;
      minx -= ((n-1) * PRIVATE(this)->maxglyphheight) * 0.5f;
      break;
    case SoAsciiText::RIGHT:
      maxx -= ((n-1) * PRIVATE(this)->maxglyphheight);
      minx -= ((n-1) * PRIVATE(this)->maxglyphheight);
      break;        
    default:
      break;
    }    
 
  }

  
  box.setBounds(SbVec3f(minx, miny, 0.0f), SbVec3f(maxx, maxy, 0.0f));
  center = box.getCenter();
}

// Doc in parent
void
SoVRMLText::generatePrimitives(SoAction * action)
{

  PRIVATE(this)->setUpGlyphs(action->getState(), this);

  int i, n = this->string.getNum();
  const float spacing = PRIVATE(this)->textspacing * PRIVATE(this)->textsize;

  SoPrimitiveVertex vertex;
  SoTextDetail detail;
  detail.setPart(0);
  vertex.setDetail(&detail);
  vertex.setMaterialIndex(0);

  this->beginShape(action, SoShape::TRIANGLES, NULL);
  vertex.setNormal(SbVec3f(0.0f, 0.0f, 1.0f));

  float ypos = 0.0f;
  int glyphidx = 0;
  int maxstringchars = 0;
  for (i=0;i<n;++i) 
    maxstringchars = SbMax(maxstringchars, this->string[i].getLength());


  for (i = 0; i < n; i++) {
    detail.setStringIndex(i);
    float xpos = 0.0f;
    const char * str = this->string[i].getString();
    
    float stretchlength = 0;
    if (i < this->length.getNum()) 
      stretchlength = this->length[i];
    float stretchfactor = (stretchlength * PRIVATE(this)->textsize) / strlen(str);
    
    float compressfactor = 1;
    if (this->maxExtent.getValue() > 0) {
      if (PRIVATE(this)->glyphwidths[i] > this->maxExtent.getValue()) {
        assert(PRIVATE(this)->glyphwidths[i] != 0 && "String length == 0! Cannot divide");
        compressfactor = (this->maxExtent.getValue() * PRIVATE(this)->textsize) / PRIVATE(this)->glyphwidths[i];
      }
    }
    
    
    if (PRIVATE(this)->horizontaltext) { // -- Horizontal text ----------------------
      
      switch (PRIVATE(this)->justificationmajor) {
      case SoAsciiText::LEFT:
        xpos = 0;
        break;
      case SoAsciiText::CENTER:
        if (PRIVATE(this)->lefttorighttext)
          xpos = - PRIVATE(this)->glyphwidths[i] * 0.5f;
        else
          xpos = PRIVATE(this)->glyphwidths[i] * 0.5f;
        break;
      case SoAsciiText::RIGHT:
        if (PRIVATE(this)->lefttorighttext) 
          xpos = -PRIVATE(this)->glyphwidths[i];
        else
          xpos = PRIVATE(this)->glyphwidths[i];
        break;
      default:
        break;
      }

      switch (PRIVATE(this)->justificationminor) {
      case SoAsciiText::LEFT:
        break;
      case SoAsciiText::CENTER:
        ypos = (i * spacing) + ((n-1) * spacing) * 0.5f;
        break;
      case SoAsciiText::RIGHT:
        ypos = (i * spacing) + ((n-1) * spacing);
        break;        
      default:
        break;
      }
    
    }
    else { // -- Vertical text -----------------------------------------

      if (PRIVATE(this)->lefttorighttext)
        xpos = i * spacing;
      else
        xpos = -i * spacing;
        
      switch (PRIVATE(this)->justificationmajor) {
      case SoAsciiText::LEFT:        
        ypos = 0;
        break;
      case SoAsciiText::RIGHT:
        if (PRIVATE(this)->toptobottomtext)
          ypos = this->string[i].getLength() * spacing;
        else
          ypos = -this->string[i].getLength() * spacing;
        break;
      case SoAsciiText::CENTER:
        if (PRIVATE(this)->toptobottomtext)
          ypos = this->string[i].getLength() * PRIVATE(this)->textsize * 0.5f;
        else
          ypos = -this->string[i].getLength() * PRIVATE(this)->textsize * 0.5f;
        break;
      default:
        break;
      }

      switch (PRIVATE(this)->justificationminor) {
      case SoAsciiText::LEFT:
        break;
      case SoAsciiText::CENTER:
        xpos -= ((n-1) * spacing) * 0.5f;
        break;
      case SoAsciiText::RIGHT:
        xpos -= ((n-1) * spacing);
        break;        
      default:
        break;
      }

    }
    
    int charidx = 0;
    
    while (*str++) {
      detail.setCharacterIndex(charidx++);
      const SoGlyph * glyph = PRIVATE(this)->glyphs[glyphidx++];
      const SbVec2f * coords = glyph->getCoords();
      const int * ptr = glyph->getFaceIndices();

      if (PRIVATE(this)->horizontaltext) {
        if (!PRIVATE(this)->lefttorighttext)
          xpos -= ((glyph->getWidth() * PRIVATE(this)->textsize) + stretchfactor) * compressfactor;
      }             

      while (*ptr >= 0) {
        SbVec2f v0, v1, v2;
        v0 = coords[*ptr++];
        v1 = coords[*ptr++];
        v2 = coords[*ptr++];
        vertex.setTextureCoords(SbVec2f(v0[0], v0[1]));
        vertex.setPoint(SbVec3f(v0[0] * PRIVATE(this)->textsize + xpos, v0[1] * PRIVATE(this)->textsize + ypos, 0.0f));
        this->shapeVertex(&vertex);
        vertex.setTextureCoords(SbVec2f(v1[0], v1[1]));
        vertex.setPoint(SbVec3f(v1[0] * PRIVATE(this)->textsize + xpos, v1[1] * PRIVATE(this)->textsize + ypos, 0.0f));
        this->shapeVertex(&vertex);
        vertex.setTextureCoords(SbVec2f(v2[0], v2[1]));
        vertex.setPoint(SbVec3f(v2[0] * PRIVATE(this)->textsize + xpos, v2[1] * PRIVATE(this)->textsize + ypos, 0.0f));
        this->shapeVertex(&vertex);
      }


      if (!PRIVATE(this)->horizontaltext) {       
        if (PRIVATE(this)->toptobottomtext)
          ypos -= PRIVATE(this)->textsize;
        else 
          ypos += PRIVATE(this)->textsize;       
      } else if (PRIVATE(this)->lefttorighttext)
        xpos += ((glyph->getWidth() * PRIVATE(this)->textsize) + stretchfactor) * compressfactor; 
    }

    if (PRIVATE(this)->horizontaltext) {
      if (PRIVATE(this)->toptobottomtext)
        ypos -= spacing;
      else
        ypos += spacing;
    }


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
    float glyphwidth = 0.0f;
    for (int j = 0; j < strlen; j++) {

      // FIXME: This 'getGlyph' call is obsolete. Should call the
      // other one which is based on the current state instead
      // (20Aug2003 handegar)
      //const SoGlyph * glyph = SoGlyph::getGlyph(ptr[j], SbName("default"));
      const SoGlyph * glyph = SoGlyph::getGlyph(state, (unsigned char) ptr[j], SbVec2s(this->textsize,this->textsize), 0);      
      assert(glyph);
      this->glyphs.append(glyph);

      glyphwidth = glyph->getWidth();
      if(glyphwidth == 0)
        glyphwidth = this->textsize/3; // SPACE width is always == 0.

      width += glyphwidth;
      
      // Find the line skipping length and character width, ie. the
      // highest and widest character.
      float boxwidth, boxheight;
      const SbBox2f bbox = glyph->getBoundingBox();
      bbox.getSize(boxwidth, boxheight);
      if(boxheight > this->maxglyphheight) this->maxglyphheight = boxheight;      
      if(boxwidth > this->maxglyphwidth) this->maxglyphwidth = boxwidth;
      
    }
    this->glyphwidths.append(width);
  }

  // unref old glyphs
  for (int j = 0; j < oldglyphs.getLength(); j++) oldglyphs[j]->unref();
}

void 
fontstylechangeCB(void * data, SoSensor * sensor)
{

  SoVRMLTextP * pimpl = (SoVRMLTextP *) data;

  SoVRMLFontStyle * fs = (SoVRMLFontStyle*) pimpl->master->fontStyle.getValue();
  if (!fs) {
    pimpl->textsize = 1.0f;
    pimpl->textspacing = 1.0f;
    pimpl->lefttorighttext = TRUE;
    pimpl->toptobottomtext = TRUE;
    pimpl->horizontaltext = TRUE;
    pimpl->justificationmajor = SoAsciiText::LEFT;
    pimpl->justificationminor = SoAsciiText::LEFT;
    return;
  }

  // Major mode
  if (!strcmp(fs->justify[0].getString(),"BEGIN") || 
      !strcmp(fs->justify[0].getString(),"FIRST") ||
      (fs->justify[0].getLength() == 0)) {
    pimpl->justificationmajor = SoAsciiText::LEFT;  
  } 
  else if (!strcmp(fs->justify[0].getString(),"MIDDLE")) {
    pimpl->justificationmajor = SoAsciiText::CENTER;
  } 
  else if (!strcmp(fs->justify[0].getString(),"END")) {
    pimpl->justificationmajor = SoAsciiText::RIGHT;
  }
    
  // Minor mode
  if (fs->justify.getNum() > 1) {
    if (!strcmp(fs->justify[1].getString(),"BEGIN") || 
        !strcmp(fs->justify[1].getString(),"FIRST") ||
        (fs->justify[1].getLength() == 0))
      pimpl->justificationminor = SoAsciiText::LEFT;  
    else if (!strcmp(fs->justify[1].getString(),"MIDDLE")) 
      pimpl->justificationminor = SoAsciiText::CENTER;
    else if (!strcmp(fs->justify[1].getString(),"END")) 
      pimpl->justificationminor = SoAsciiText::RIGHT;
  }
  
  pimpl->lefttorighttext = fs->leftToRight.getValue();
  pimpl->toptobottomtext = fs->topToBottom.getValue();
  pimpl->horizontaltext = fs->horizontal.getValue();
  pimpl->textsize = fs->size.getValue();
  pimpl->textspacing = fs->spacing.getValue();

}
