/**************************************************************************\
 * 
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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
  \class SoText2 SoText2.h Inventor/nodes/SoText2.h
  \brief The SoText2 class ...
  \ingroup nodes

  FIXME: write class doc
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
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/SbName.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
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
#include <Inventor/elements/SoViewingMatrixElement.h>
#include <Inventor/elements/SoViewportRegionElement.h>
#include <Inventor/elements/SoFontNameElement.h>
#include <Inventor/elements/SoFontSizeElement.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/bundles/SoMaterialBundle.h>
#include <Inventor/elements/SoGLLightModelElement.h>
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

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

//$ BEGIN TEMPLATE NodeSource(SoText2)
SoType SoText2::classTypeId = SoType::badType();

/*!
  Returns a new instance of the SoText2 node class.
*/
void *
SoText2::createInstance(void)
{
  return new SoText2;
}

/*!
  Returns the unique type identifier for the SoText2 class.
*/
SoType
SoText2::getClassTypeId(void)
{
  return SoText2::classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoText2::getTypeId(void) const
{
  return SoText2::classTypeId;
}
//$ END TEMPLATE NodeSource

/*!
  Constructor.
*/
SoText2::SoText2()
{
//$ BEGIN TEMPLATE NodeConstructor(SoText2)
  // Make sure the class has been initialized.
  assert(SoText2::classTypeId != SoType::badType());
//$ END TEMPLATE NodeConstructor

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
//$ BEGIN TEMPLATE InitNodeSource(Text2)
  // Make sure we only initialize once.
  assert(SoText2::classTypeId == SoType::badType());
  // Make sure superclass get initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoText2::classTypeId =
    SoType::createType(inherited::getClassTypeId(), "Text2",
                       &SoText2::createInstance,
                       SoNode::nextActionMethodIndex++);
//$ END TEMPLATE InitNodeSource
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoText2::cleanClass(void)
{
}


// **************************************************************************

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)

#if defined(_WIN32) || defined(__BEOS__)

#else // !_WIN32 && !__BEOS__

static Display * d = NULL;

static XFontStruct *
tryFont(const char * fs)
{
  XFontStruct * font = XLoadQueryFont(d, fs);
  // FIXME: match call with XFreeFont. 19990418 mortene.
#if COIN_DEBUG
  SoDebugError::postInfo("tryFont", "'%s': %s", fs, font ? "hit!" : "miss...");
#endif // COIN_DEBUG
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
  SbString fs("*");
  fs += fontname;
  fs += "*-r-*-*-";
  SbString is;
  is.intToString(fontsize);
  fs += is;
  fs += "-*-*-*-*-*-*-*";

  if ((fstruc = tryFont(fs.getString()))) return fstruc;

  // Try with the full fontname and size setting -- any style.
  fs = "*";
  fs += fontname;
  fs += "*-";
  is.intToString(fontsize);
  fs += is;
  fs += "-*-*-*-*-*-*-*";

  if ((fstruc = tryFont(fs.getString()))) return fstruc;

  // Can't seem to find a way to use the fontname, so lets try to get
  // _any_ non-italic font at the correct size.
  fs = "-*-*-*-r-*-*-";
  is.intToString(fontsize);
  fs += is;
  fs += "-*-*-*-*-*-*-*";

  if ((fstruc = tryFont(fs.getString()))) return fstruc;

  // That didn't work out either, so lets try settle for any font of
  // the correct size.
  fs = "-*-*-*-*-*-*-";
  is.intToString(fontsize);
  fs += is;
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
  static SbDict fontdict;

  SoState * state = action->getState();
  SbName fontname = SoFontNameElement::get(state);
  int fontsize = int(SoFontSizeElement::get(state));

  SbString fontid(fontname.getString());
  SbString is;
  is.intToString(fontsize);
  fontid += is;

  // FIXME: hack. Need a proper (templatized?) dict which can do
  // mapping based on string keys. 19990418 mortene.
  uint32_t fontkey = fontid.hash();

  void * fontptrs;
  if (fontdict.find(fontkey, fontptrs)) {
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
    fontdict.enter(fontkey, (void *)ptrs);

    return base;
  }
}

#endif // _WIN32 || __BEOS__

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
  unsigned int fontlistbase = getGLList(action, fontstruct);

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

      float xpos;
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
	xpos = nilpoint[0];
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
#endif // _WIN32 || __BEOS__
}
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

// **************************************************************************


#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
/*!
  FIXME: write function documentation
*/
void
SoText2::computeBBox(SoAction * /* action */, SbBox3f & box, SbVec3f & center)
{
  // FIXME: implement properly. 19990418 mortene.
  box.setBounds(SbVec3f(-1, -1, -0.1), SbVec3f(1, 1, 0.1));
  center.setValue(0.0f, 0.0f, 0.0f);
}
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION


#if !defined(COIN_EXCLUDE_SORAYPICKACTION)
/*!
  FIXME: write doc
 */
void
SoText2::rayPick(SoRayPickAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SORAYPICKACTION

#if !defined(COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION)
/*!
  FIXME: write doc
 */
void
SoText2::getPrimitiveCount(SoGetPrimitiveCountAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION

#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write doc
 */
void
SoText2::generatePrimitives(SoAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOACTION
