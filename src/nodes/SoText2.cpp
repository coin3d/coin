/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
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

  \sa SoFont, SoFontStyle, SoText3, SoAsciiText
*/

// FIXME -- FIXME -- FIXME
//
// Stuff missing in the GLRender implementation:
//  * doesn't heed the justification field
//
// Besides, we need to fix or look into these things:
//  * the getBoundingBox() method is just a stub now -- it needs to be correct
//  * must clean up the local font cache stuff and move it into
//    a global scheme (because fonts are also used in other nodes
//    (SoAsciiText, SoAnnoText3, others?)) in caches/SoFontCache or some such
//  * the font acquisition is now dependent on X11 in the method
//    interfaces -- we should abstract away from window system dependencies
//    as much as possible
//  * allocations aren't cleaned out on exit (Display *, XFontStructs,
//    SbDict, OpenGL display lists, ...)
//  * should we make hooks into the component class libraries for font handling
//    methods to meet the goal of making the Coin core 100% window system
//    independent?
//  * could we then perhaps favorably use Qt's fonthandling?
//
//         -- 19990418 mortene.

#include <Inventor/nodes/SoText2.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <coindefs.h> // COIN_STUB()

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#if defined(_WIN32)
#include <windows.h>
#elif !defined(__BEOS__)
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <GL/glx.h>
#endif // _WIN32 || __BEOS__
#include <GL/gl.h>
#include <Inventor/elements/SoViewVolumeElement.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/elements/SoViewportRegionElement.h>
#include <Inventor/elements/SoFontNameElement.h>
#include <Inventor/elements/SoFontSizeElement.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/bundles/SoMaterialBundle.h>
#include <Inventor/elements/SoGLLightModelElement.h>

#include <Inventor/actions/SoGetPrimitiveCountAction.h>

#include <limits.h>

static const unsigned int NOT_AVAILABLE = UINT_MAX;

/*!
  \enum SoText2::Justification
  FIXME: write documentation for enum
*/
/*!
  \var SoText2::Justification SoText2::LEFT
  FIXME: write documentation for enum definition
*/
/*!
  \var SoText2::Justification SoText2::RIGHT
  FIXME: write documentation for enum definition
*/
/*!
  \var SoText2::Justification SoText2::CENTER
  FIXME: write documentation for enum definition
*/


/*!
  \var SoMFString SoText2::string
  FIXME: write documentation for field
*/
/*!
  \var SoSFFloat SoText2::spacing
  FIXME: write documentation for field
*/
/*!
  \var SoSFEnum SoText2::justification
  FIXME: write documentation for field
*/


// *************************************************************************

SO_NODE_SOURCE(SoText2);

/*!
  Constructor.
*/
SoText2::SoText2(void)
{
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
}

/*!
  Does initialization common for all objects of the
  SoText2 class. This includes setting up the
  type system, among other things.
*/

void
SoText2::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoText2);
}


// **************************************************************************

#if defined(_WIN32) || defined(__BEOS__)

#else // !_WIN32 && !__BEOS__

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
  // (Don't use a static constructor.)
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
    void ** ptrs = (void **)fontptrs;
    fontstruct = (XFontStruct *)(ptrs[1]);
    return (unsigned int)(ptrs[0]);
  }
  else {
    unsigned int base = NOT_AVAILABLE;
    if ((fontstruct = setFont(fontname, fontsize))) {
      base = glGenLists(256);
      glXUseXFont(fontstruct->fid, 0, 256, base);
    }

    void ** ptrs = new void*[2];
    ptrs[0] = (void *)base;
    ptrs[1] = (void *)fontstruct;
    fontdict->enter(fontkey, (void *)ptrs);

    return base;
  }
}

#endif // _WIN32 || __BEOS__


extern unsigned char coin_default2dfont[][12];

/*!
  FIXME: write function documentation
*/
void
SoText2::GLRender(SoGLRenderAction * action)
{
  if (!this->shouldGLRender(action)) return;
#if defined(_WIN32) || defined(__BEOS__)

#else // !_WIN32 && !__BEOS__
  SoState * state = action->getState();
  XFontStruct * fontstruct;

#if 0 // FIXME: crashes on freya.sim.no? 20000905 mortene.
  unsigned int fontlistbase = getGLList(action, fontstruct);
#else
  // FIXME: hack. 20000905 skei.
  unsigned int fontlistbase = NOT_AVAILABLE;
#endif

  if (fontlistbase != NOT_AVAILABLE) {
    SoMaterialBundle mb(action);
    mb.sendFirst();

    // SoGLLightModelElement is lazy,
    // changing GL state without affecting state of element
    SoGLLightModelElement::getInstance(state)->
      forceSend(SoLightModelElement::BASE_COLOR);

    glListBase(fontlistbase);

    SbVec3f nilpoint(0.0f, 0.0f, 0.0f);
    const SbMatrix & mat = SoModelMatrixElement::get(state);
    mat.multVecMatrix(nilpoint, nilpoint);

    const SbViewVolume & vv = SoViewVolumeElement::get(state);

    // this function will also modify the z-value of nilpoint
    // according to the view matrix
    vv.projectToScreen(nilpoint, nilpoint);

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
      int direction, ascent, descent;
      XCharStruct cs;
      XTextExtents(fontstruct, s, strlen(s),
                   &direction, &ascent, &descent, &cs);

      float xpos = 0.0; // init unnecessary, but kills a compiler warning.
      switch (this->justification.getValue()) {
      case SoText2::LEFT:
        xpos = nilpoint[0];
        break;
      case SoText2::RIGHT:
        xpos = nilpoint[0] - cs.width;
        break;
      case SoText2::CENTER:
        xpos = nilpoint[0] - cs.width/2;
        break;
#if COIN_DEBUG
      default:
        SoDebugError::post("SoText2::GLRender",
                           "value of justification field is invalid");
        break;
#endif // COIN_DEBUG
      }

      float textheight = float(ascent + descent);
      glRasterPos3f(xpos, linepos - textheight/2.0f, -nilpoint[2]);
      glCallLists(strlen(s), GL_UNSIGNED_BYTE, (GLubyte *)s);
      linepos -= textheight * this->spacing.getValue();
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
    SoGLLightModelElement::getInstance(state)->forceSend(SoLightModelElement::BASE_COLOR);
    SbVec3f nilpoint(0.0f, 0.0f, 0.0f);
    const SbMatrix & mat = SoModelMatrixElement::get(state);
    mat.multVecMatrix(nilpoint, nilpoint);
    const SbViewVolume & vv = SoViewVolumeElement::get(state);
    // this function will also modify the z-value of nilpoint
    // according to the view matrix
    vv.projectToScreen(nilpoint, nilpoint);
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
		
    int xpos,ypos,width;
		
    ypos = nilpoint[1];
    for (int i = 0; i < this->string.getNum(); i++) {
      const char *s = this->string[i].getString();
      width = strlen(s);
      //xpos = nilpoint[0];
      switch (this->justification.getValue()) {
      case SoText2::LEFT:
        xpos = nilpoint[0];
        break;
      case SoText2::RIGHT:
        xpos = nilpoint[0] - (width*8);
        break;
      case SoText2::CENTER:
        xpos = nilpoint[0] - (width*8)/2;
        break;
      }
      for (int i2=0;i2<width;i2++) {
        if ( (s[i2] >= 32) && (s[i2] <= 127) ) { // just in case?
          glRasterPos2i(0,0);
          glBitmap(0,0,0,0,xpos,ypos,NULL);
          glBitmap(8,12,0,0,0,0,(const GLubyte *)coin_default2dfont + 12 * (s[i2]-32) );
        }
        xpos += 8;
      }
      // - instead of + because of OpenGL's "inverted" y coordinate...
      ypos -= ( 12 * this->spacing.getValue() );
    }
		
    glPixelStorei(GL_UNPACK_ALIGNMENT,4);
    // Pop old GL state.
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
  }

#endif // _WIN32 || __BEOS__
}

// **************************************************************************


/*!
  FIXME: write function documentation
*/
void
SoText2::computeBBox(SoAction * /* action */, SbBox3f & box, SbVec3f & center)
{
  // FIXME: implement properly. 19990418 mortene.
  box.setBounds(SbVec3f(-1.0f, -1.0f, -0.1f),
	            SbVec3f(1.0f, 1.0f, 0.1f));
  center.setValue(0.0f, 0.0f, 0.0f);
}

/*!
  FIXME: write doc
 */
void
SoText2::rayPick(SoRayPickAction * /* action */)
{
  COIN_STUB();
}

/*!
  FIXME: write doc
 */
void
SoText2::getPrimitiveCount(SoGetPrimitiveCountAction *action)
{
  if (!this->shouldPrimitiveCount(action)) return;

  action->addNumText(this->string.getNum());
}

/*!
  FIXME: write doc
 */
void
SoText2::generatePrimitives(SoAction * /* action */)
{
  COIN_STUB();
}
