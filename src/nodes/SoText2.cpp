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
  \class SoText2 SoText2.h Inventor/nodes/SoText2.h
  \brief The SoText2 class is a node type for visualizing 2D text aligned with the camera plane.
  \ingroup nodes

  SoText2 text is not scaled according to the distance from the
  camera, and is not influenced by rotation or scaling as 3D
  primitives are. If these are properties that you want the text to
  have, you should instead use an SoText3 or SoAsciiText node.

  Note that even though the size of the 2D text is not influenced by
  the distance from the camera, the text is still subject to the usual
  rules with regard to the depthbuffer, so it \e will be obscured by
  graphics laying in front of it.

  The text will be \e positioned according to the current transformation.
  The x origin of the text is the first pixel of the leftmost character
  of the text. The y origin of the text is the baseline of the first line
  of text (the baseline being the imaginary line on which all upper case
  characters are standing).

  \sa SoFont, SoFontStyle, SoText3, SoAsciiText
*/

// FIXME -- FIXME -- FIXME
//
//  * computeBoundingBox() is not implemented properly, rayPick() and
//    generatePrimitives() are just stubs
//  * allocations aren't cleaned out on exit (Display *, XFontStructs,
//    SbDict, OpenGL display lists, ...)
//  * integrate with libfreetype to remove dependency on X11.
//
//         -- 19990418 mortene.

#include <Inventor/nodes/SoText2.h>
#include <Inventor/nodes/SoSubNodeP.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#ifdef HAVE_X11_AVAILABLE
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#endif // HAVE_X11_AVAILABLE

#ifdef HAVE_GLX
#include <GL/glx.h>
#endif // HAVE_GLX

#include <Inventor/system/gl.h>
#include <Inventor/elements/SoViewVolumeElement.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/elements/SoViewportRegionElement.h>
#include <Inventor/elements/SoFontNameElement.h>
#include <Inventor/elements/SoFontSizeElement.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/bundles/SoMaterialBundle.h>
#include <Inventor/elements/SoGLLightModelElement.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/details/SoTextDetail.h>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/SbPlane.h>
#include <Inventor/SbLine.h>

#include <limits.h>
#include <string.h>

static const unsigned int NOT_AVAILABLE = UINT_MAX;

/*!
  \enum SoText2::Justification

  Enum contains the various options for how the horizontal text layout
  text should be done.
*/


/*!
  \var SoMFString SoText2::string

  The set of strings to render.  Each string in the multiple value
  field will be rendered on it's own line.

  The default value of the field is a single empty string.
*/
/*!
  \var SoSFFloat SoText2::spacing

  Spacing between each consecutive vertical line.  Default value is
  1.0, which means that the space between the uppermost line of each
  rendered string will equal the vertical size of the highest
  character in the bitmap alphabet.
*/
/*!
  \var SoSFEnum SoText2::justification

  Decides how the horizontal layout of the text strings is done.
*/

#ifndef DOXYGEN_SKIP_THIS

class SoText2P {
public:
  SoText2P(SoText2 * textnode) : textnode(textnode) {}

  SoText2 * textnode;

public:
  void getQuad(SoState * state, SbVec3f & v0, SbVec3f & v1,
               SbVec3f & v2, SbVec3f & v3);
  SbVec2f getFontSize(SoState * state);
};

#ifdef HAVE_X11_AVAILABLE
struct FontStructMapping {
  XFontStruct * fontstruct;
  unsigned int glbase;
};
#endif // HAVE_X11_AVAILABLE

#endif // DOXYGEN_SKIP_THIS

// *************************************************************************

#undef THIS
#define THIS this->pimpl

SO_NODE_SOURCE(SoText2);

/*!
  Constructor.
*/
SoText2::SoText2(void)
{
  THIS = new SoText2P(this);

  SO_NODE_INTERNAL_CONSTRUCTOR(SoText2);

  SO_NODE_ADD_FIELD(string, (""));
  SO_NODE_ADD_FIELD(spacing, (1.0f));
  SO_NODE_ADD_FIELD(justification, (SoText2::LEFT));

  SO_NODE_DEFINE_ENUM_VALUE(Justification, LEFT);
  SO_NODE_DEFINE_ENUM_VALUE(Justification, RIGHT);
  SO_NODE_DEFINE_ENUM_VALUE(Justification, CENTER);
  SO_NODE_SET_SF_ENUM_TYPE(justification, Justification);
}

/*!
  Destructor.
*/
SoText2::~SoText2()
{
  delete THIS;
}

// doc in super
void
SoText2::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoText2, SO_FROM_INVENTOR_2_1);
}


// **************************************************************************

#ifdef HAVE_X11_AVAILABLE

static Display * d = NULL;

static XFontStruct *
tryFont(const char * fs)
{
#if COIN_DEBUG && 0 // debug
  SoDebugError::postInfo("tryFont", "'%s'", fs);
#endif // debug
  XFontStruct * font = XLoadQueryFont(d, fs);
  // FIXME: match call with XFreeFont. 19990418 mortene.
#if COIN_DEBUG && 0 // debug
  SoDebugError::postInfo("tryFont", "%s", font ? "hit!" : "miss...");
#endif // debug
  return font;
}

static XFontStruct *
setFont(SbName fontname, int fontsize)
{
  if (!d) d = XOpenDisplay(NULL);
  // FIXME: handle !d. match with XCloseDisplay. 19990418 mortene.

#if 0 // debug
  int num;
  char ** fontnames = XListFonts(d, "*", INT_MAX, &num);
  for (int i=0; i < num; i++) {
    SoDebugError::postInfo("findFont", "'%s'", fontnames[i]);
  }
#endif // debug

  XFontStruct * fstruc = NULL;

  // First try with only the fontname setting, in case the user
  // specified a full X11 font string.
  if ((fstruc = tryFont(fontname.getString()))) return fstruc;

  // Try with the full fontname and size setting with non-italic style.
  SbString fs("-*-");
  fs += fontname;
  fs += "-*-r-*-*-";
  fs.addIntString(fontsize);
  fs += "-*-*-*-*-*-*-*";

  if ((fstruc = tryFont(fs.getString()))) return fstruc;

  // Try with the full fontname and size setting -- any style.
  fs = "-*-";
  fs += fontname;
  fs += "-*-*-*-*-";
  fs.addIntString(fontsize);
  fs += "-*-*-*-*-*-*-*";

  if ((fstruc = tryFont(fs.getString()))) return fstruc;

  // Can't seem to find a way to use the fontname, so lets try to get
  // _any_ non-italic font at the correct size.
  fs = "-*-*-*-r-*-*-";
  fs.addIntString(fontsize);
  fs += "-*-*-*-*-*-*-*";

  if ((fstruc = tryFont(fs.getString()))) return fstruc;

  // That didn't work out either, so lets try settle for any font of
  // the correct size.
  fs = "-*-*-*-*-*-*-";
  fs.addIntString(fontsize);
  fs += "-*-*-*-*-*-*-*";

  if ((fstruc = tryFont(fs.getString()))) return fstruc;

  // Last resort -- try to read _any_ font on the system. If this
  // doesn't work, something is seriously pucked up (either on the
  // user's system or in this code). --mortene
  fs = "-*-*-*-*-*-*-*-*-*-*-*-*-*-*";

  if ((fstruc = tryFont(fs.getString()))) return fstruc;

#if COIN_DEBUG
  SoDebugError::postWarning("setFont", "Couldn't load a font!");
#endif // COIN_DEBUG

  return NULL;
}

static unsigned int
getGLList(SoGLRenderAction * action, XFontStruct *& fontstruct)
{
  // (Don't use a static constructor, as that is not portable.)
  static SbDict * fontdict = new SbDict; // FIXME: should deallocate on exit. 20000406 mortene.

  SoState * state = action->getState();
  SbName fontname = SoFontNameElement::get(state);
  int fontsize = int(SoFontSizeElement::get(state));

  SbString fontid(fontname.getString());
  fontid.addIntString(fontsize);

  // FIXME: hack. Need a proper (templatized?) dict which can do
  // mapping based on string keys. 19990418 mortene.
  uint32_t fontkey = fontid.hash();

  void * fontptrs;
  if (fontdict->find(fontkey, fontptrs)) {
    struct FontStructMapping * fsm = (struct FontStructMapping *)fontptrs;
    fontstruct = fsm->fontstruct;
    return fsm->glbase;
  }
  else {
    unsigned int base = NOT_AVAILABLE;
    if ((fontstruct = setFont(fontname, fontsize))) {
      base = glGenLists(256);
      // FIXME: be robust -- don't just assert here! GL displaylists
      // can be a scarce resource. 20020212 mortene.
      assert(base != 0 && "could not reserve a displaylist resource");

#ifdef HAVE_GLX
      glXUseXFont(fontstruct->fid, 0, 256, base);
#endif // HAVE_GLX
    }

    struct FontStructMapping * fsm = new struct FontStructMapping;
    fsm->glbase = base;
    fsm->fontstruct = fontstruct;

    fontdict->enter(fontkey, (void *)fsm);

    return base;
  }
}

#endif // HAVE_X11_AVAILABLE

static void
string_dimensions(void * fontdata, const char * s,
                  float & strwidth, float & strheight)
{
#ifdef HAVE_X11_AVAILABLE
  int direction, ascent, descent;
  XCharStruct cs;
  XTextExtents((XFontStruct *)fontdata, s, strlen(s),
               &direction, &ascent, &descent, &cs);
  strwidth = cs.width;
  strheight = ascent + descent;
  return;
#endif // HAVE_X11_AVAILABLE

  strwidth = strheight = 0.0f;
}


extern unsigned char coin_default2dfont[][12];
extern int coin_default2dfont_isolatin1_mapping[];

// doc in super
void
SoText2::GLRender(SoGLRenderAction * action)
{
  if (!this->shouldGLRender(action)) return;

  SoState * state = action->getState();
  // Default to no available GL displaylist with font bitmaps.
  unsigned int fontlistbase = NOT_AVAILABLE;
  void * fontdata = NULL;

#ifdef HAVE_X11_AVAILABLE
  // FIXME: crashes on freya.sim.no? 20000905 mortene.
// fontlistbase = getGLList(action, (XFontStruct *)fontdata);
#endif // HAVE_X11_AVAILABLE

  if (fontlistbase != NOT_AVAILABLE) {
    SoMaterialBundle mb(action);
    mb.sendFirst();

    // SoGLLightModelElement is lazy,
    // changing GL state without affecting state of element
    SoGLLightModelElement::forceSend(state, SoLightModelElement::BASE_COLOR);

    glListBase(fontlistbase);

    SbVec3f nilpoint(0.0f, 0.0f, 0.0f);
    const SbMatrix & mat = SoModelMatrixElement::get(state);
    mat.multVecMatrix(nilpoint, nilpoint);

    const SbViewVolume & vv = SoViewVolumeElement::get(state);

    // this function will also modify the z-value of nilpoint
    // according to the view matrix
    vv.projectToScreen(nilpoint, nilpoint);
    // change z range from [0,1] to [-1,1]
    nilpoint[2] *= 2.0f;
    nilpoint[2] -= 1.0f;

#if 0 // debug
    SoDebugError::postInfo("SoText2::GLRender",
                           "nilpoint projected: <%f, %f>",
                           nilpoint[0], nilpoint[1]);
#endif // debug

    const SbViewportRegion & vp = SoViewportRegionElement::get(state);
    SbVec2s vpsize = vp.getViewportSizePixels();
    nilpoint[0] = nilpoint[0] * float(vpsize[0]);
    nilpoint[1] = nilpoint[1] * float(vpsize[1]);

#if 0 // debug
    SoDebugError::postInfo("SoText2::GLRender",
                           "nilpoint de-normalized: <%f, %f>",
                           nilpoint[0], nilpoint[1]);
#endif // debug

    // Set new state.
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    glOrtho(0, vpsize[0], 0, vpsize[1], -1.0f, 1.0f);

    float linepos = nilpoint[1];
    for (int i = 0; i < this->string.getNum(); i++) {
      const char * s = this->string[i].getString();

      // Find text field dimensions.
      float strwidth, strheight;
      string_dimensions(fontdata, s, strwidth, strheight);

      float xpos = 0.0; // init unnecessary, but kills a compiler warning.
      switch (this->justification.getValue()) {
      case SoText2::LEFT:
        xpos = nilpoint[0];
        break;
      case SoText2::RIGHT:
        xpos = nilpoint[0] - strwidth;
        break;
      case SoText2::CENTER:
        xpos = nilpoint[0] - strwidth/2;
        break;
#if COIN_DEBUG
      default:
        SoDebugError::post("SoText2::GLRender",
                           "value of justification field is invalid");
        break;
#endif // COIN_DEBUG
      }

      glRasterPos3f(xpos, linepos - strheight/2.0f, -nilpoint[2]);
      glCallLists(strlen(s), GL_UNSIGNED_BYTE, (GLubyte *)s);
      linepos -= strheight * this->spacing.getValue();
    }

    // Pop old GL state.
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
  }

  // Default font drawing.
  else {
    SoMaterialBundle mb(action);
    mb.sendFirst();
    SoGLLightModelElement::forceSend(state, SoLightModelElement::BASE_COLOR);
    SbVec3f nilpoint(0.0f, 0.0f, 0.0f);
    const SbMatrix & mat = SoModelMatrixElement::get(state);
    mat.multVecMatrix(nilpoint, nilpoint);
    const SbViewVolume & vv = SoViewVolumeElement::get(state);
    // this function will also modify the z-value of nilpoint
    // according to the view matrix
    vv.projectToScreen(nilpoint, nilpoint);
    // change z-range from [0,1] to [-1,1]
    nilpoint[2] *= 2.0f;
    nilpoint[2] -= 1.0f;

    const SbViewportRegion & vp = SoViewportRegionElement::get(state);
    SbVec2s vpsize = vp.getViewportSizePixels();
    nilpoint[0] = nilpoint[0] * float(vpsize[0]);
    nilpoint[1] = nilpoint[1] * float(vpsize[1]);
    // Set new state.
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, vpsize[0], 0, vpsize[1], -1.0f, 1.0f);
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);

    int strwidth;

    float xpos = nilpoint[0];      // to get rid of compiler warning..
    float ypos = nilpoint[1];
    for (int i = 0; i < this->string.getNum(); i++) {
      const unsigned char *s = 
        (const unsigned char *) this->string[i].getString();
      strwidth = strlen((const char *)s);
      switch (this->justification.getValue()) {
      case SoText2::LEFT:
        xpos = nilpoint[0];
        break;
      case SoText2::RIGHT:
        xpos = nilpoint[0] - (strwidth * 8.0f);
        break;
      case SoText2::CENTER:
        xpos = nilpoint[0] - (strwidth * 8.0f)/2.0f;
        break;
      }
      for (int i2 = 0; i2 < strwidth; i2++) {
        if (s[i2] >= 32) { // just in case?
          glRasterPos3f(xpos, ypos, -nilpoint[2]);
          glBitmap(8,12,0,0,0,0,(const GLubyte *)coin_default2dfont 
                   + 12 * coin_default2dfont_isolatin1_mapping[s[i2]]);
        }
        xpos += 8.0f;
      }
      // - instead of + because of OpenGL's "inverted" y coordinate...
      ypos -= ( 12.0f * this->spacing.getValue() );
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT,4);
    // Pop old GL state.
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
  }
}

// **************************************************************************


// doc in super
void
SoText2::computeBBox(SoAction * action, SbBox3f & box, SbVec3f & center)
{
  SbVec3f v0, v1, v2, v3;
  // this will cause a cache dependency on the view volume,
  // model matrix and viewport.
  THIS->getQuad(action->getState(), v0, v1, v2, v3);

  box.makeEmpty();
  box.extendBy(v0);
  box.extendBy(v1);
  box.extendBy(v2);
  box.extendBy(v3);
  center = box.getCenter();
}

// doc in super
void
SoText2::rayPick(SoRayPickAction * action)
{
  if (!this->shouldRayPick(action)) return;
  action->setObjectSpace();

  SbVec3f v0, v1, v2, v3;
  THIS->getQuad(action->getState(), v0, v1, v2, v3);
  if (v0 == v1 || v0 == v3) return; // empty

  SbVec3f isect;
  SbVec3f bary;
  SbBool front;
  SbBool hit = action->intersect(v0, v1, v2, isect, bary, front);
  if (!hit) hit = action->intersect(v0, v2, v3, isect, bary, front);

  if (hit && action->isBetweenPlanes(isect)) {
    // find normalized 2D hitpoint on quad
    float h = (v3-v0).length();
    float w = (v1-v0).length();
    SbLine horiz(v2,v3);
    SbVec3f ptonline = horiz.getClosestPoint(isect);
    float vdist = (ptonline-isect).length();
    vdist /= h;

    SbLine vert(v0,v3);
    ptonline = vert.getClosestPoint(isect);
    float hdist = (ptonline-isect).length();
    hdist /= w;

    // find which string and character was hit
    float fonth =  1.0f / float(this->string.getNum());
    int stringidx = SbClamp(int(vdist/fonth), 0, this->string.getNum()-1);

    int maxlen = 0;
    for (int i = 0; i < this->string.getNum(); i++) {
      int len = this->string[i].getLength();
      if (len > maxlen) maxlen = len;
    }
    // assumes all characters are equal size...
    float fontw = 1.0f / float(maxlen);

    // find the character
    int charidx = -1;
    int strlength = this->string[stringidx].getLength();
    switch (this->justification.getValue()) {
    case LEFT:
      charidx = int(hdist / fontw);
      break;
    case RIGHT:
      charidx = (strlength-1) - int((1.0f-hdist)/fontw);
      break;
    case CENTER:
      {
        float strstart = 0.5f - fontw*float(strlength)*0.5f;
        charidx = int((hdist-strstart) / fontw);
      }
      break;
    default:
      assert(0 && "unknown justification");
    }

    if (charidx >= 0 && charidx < strlength) { // we have a hit!
      SoPickedPoint * pp = action->addIntersection(isect);
      if (pp) {
        SoTextDetail * detail = new SoTextDetail;
        detail->setStringIndex(stringidx);
        detail->setCharacterIndex(charidx);
        pp->setDetail(detail, this);
        pp->setMaterialIndex(0);
        pp->setObjectNormal(SbVec3f(0.0f, 0.0f, 1.0f));
      }
    }
  }
}

// doc in super
void
SoText2::getPrimitiveCount(SoGetPrimitiveCountAction *action)
{
  if (!this->shouldPrimitiveCount(action)) return;

  action->addNumText(this->string.getNum());
}

// doc in super
void
SoText2::generatePrimitives(SoAction * action)
{
  // This is supposed to be empty. There are no primitives.
}


// SoText2P methods below
#undef THIS

#ifndef DOXYGEN_SKIP_THIS

// Calculates a quad around the text in 3D.
void
SoText2P::getQuad(SoState * state, SbVec3f & v0, SbVec3f & v1,
                  SbVec3f & v2, SbVec3f & v3)
{
  SbVec3f nilpoint(0.0f, 0.0f, 0.0f);
  const SbMatrix & mat = SoModelMatrixElement::get(state);
  mat.multVecMatrix(nilpoint, nilpoint);

  const SbViewVolume &vv = SoViewVolumeElement::get(state);

  SbVec3f screenpoint;
  vv.projectToScreen(nilpoint, screenpoint);

  const SbViewportRegion & vp = SoViewportRegionElement::get(state);
  SbVec2s vpsize = vp.getViewportSizePixels();

  // find normalized width and height of text

  // FIXME: this only works for the default font
  SbVec2f fontsize = this->getFontSize(state);

  // normalize size
  fontsize[0] /= vpsize[0];
  fontsize[1] /= vpsize[1];

  float nh = this->textnode->string.getNum() * fontsize[1] * this->textnode->spacing.getValue();
  float nw = 0.0f;
  for (int i = 0; i < this->textnode->string.getNum(); i++) {
    const SbString & s = this->textnode->string[i];
    float w = s.getLength()*fontsize[0];
    if (w > nw) nw = w;
  }

  float halfw = nw * 0.5f;
  SbVec2f n0, n1, n2, n3;

  n0 = SbVec2f(screenpoint[0]-halfw, screenpoint[1]-nh+fontsize[1]);
  n1 = SbVec2f(screenpoint[0]+halfw, screenpoint[1]-nh+fontsize[1]);
  n2 = SbVec2f(screenpoint[0]+halfw, screenpoint[1]+fontsize[1]);
  n3 = SbVec2f(screenpoint[0]-halfw, screenpoint[1]+fontsize[1]);

  switch (this->textnode->justification.getValue()) {
  case SoText2::LEFT:
    n0[0] += halfw;
    n1[0] += halfw;
    n2[0] += halfw;
    n3[0] += halfw;
    break;
  case SoText2::RIGHT:
    n0[0] -= halfw;
    n1[0] -= halfw;
    n2[0] -= halfw;
    n3[0] -= halfw;
    break;
  case SoText2::CENTER:
    break;
  default:
    assert(0 && "unknown alignment");
    break;
  }

  // get distance from nilpoint to camera plane
  float dist = -vv.getPlane(0.0f).getDistance(nilpoint);

  // find the four image points in the plane
  v0 = vv.getPlanePoint(dist, n0);
  v1 = vv.getPlanePoint(dist, n1);
  v2 = vv.getPlanePoint(dist, n2);
  v3 = vv.getPlanePoint(dist, n3);

  // transform back to object space
  SbMatrix inv = mat.inverse();
  inv.multVecMatrix(v0, v0);
  inv.multVecMatrix(v1, v1);
  inv.multVecMatrix(v2, v2);
  inv.multVecMatrix(v3, v3);
}

SbVec2f
SoText2P::getFontSize(SoState * /* state */)
{
  // FIXME: consider state when we support font loading
  return SbVec2f(8.0f, 12.0f);
}

#endif // DOXYGEN_SKIP_THIS
