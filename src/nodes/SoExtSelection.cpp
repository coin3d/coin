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
  \class SoExtSelection SoExtSelection.h Inventor/nodes/SoExtSelection.h
  \brief The SoExtSelection class can be used for extended selection functionality.
  \ingroup nodes

  This class enables you to select geometry by specifying a lasso (a
  polygon) or a rectangle on screen. When objects are selected, you'll
  receive the same callbacks as for the SoSelection node.

  This node class is an extension versus the original SGI Inventor
  v2.1 API. It is based on the API of TGS Inventor's SoExtSelection,
  and we aim to be fully compatible with this node to enable users to
  switch between using Coin and TGS Inventor.  Please contact us if
  you find discrepancies between Coin's SoExtSelection and TGS's
  SoExtSelection node.

  The application programmer interface of this class is somewhat
  complex, due to it's non-trivial functionality. To see an \e
  extensive usage example of the SoExtSelection node, we advise you to
  go look at the "extselection" example application in the "nodes/"
  directory of Systems in Motion's SoGuiExamples CVS module. Further
  information and links for downloading and building this module
  should be available at <a href="http://www.coin3d.org">www.coin3d.org</a>.

  \since TGS Inventor 2.5
  \since Coin 1.0
*/

#include <Inventor/nodes/SoExtSelection.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <coindefs.h> // COIN_OBSOLETED()

#include <Inventor/events/SoEvent.h>
#include <Inventor/actions/SoHandleEventAction.h>
#include <Inventor/events/SoMouseButtonEvent.h>
#include <Inventor/events/SoKeyboardEvent.h>
#include <Inventor/events/SoLocation2Event.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/SbTime.h>

#include <Inventor/nodes/SoCallback.h> 
#include <Inventor/sensors/SoTimerSensor.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/nodes/SoShape.h>
#include <Inventor/nodes/SoCamera.h>
#include <Inventor/details/SoFaceDetail.h> 
#include <Inventor/lists/SoPathList.h> 

#include <Inventor/SbBox3f.h>
#include <Inventor/SbBox2s.h>
#include <Inventor/SbVec3f.h>
#include <Inventor/SbVec2s.h>
#include <Inventor/SbViewVolume.h>
#include <Inventor/SbMatrix.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/elements/SoViewportRegionElement.h>
#include <Inventor/elements/SoProjectionMatrixElement.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/elements/SoViewingMatrixElement.h>
#include <Inventor/elements/SoViewVolumeElement.h>
#include <Inventor/elements/SoCullElement.h>
#include <Inventor/elements/SoPointSizeElement.h> 
#include <Inventor/lists/SoCallbackList.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/SbMatrix.h>
#include <Inventor/nodes/SoVertexShape.h> 

#include <Inventor/SoOffscreenRenderer.h> 
#include <Inventor/SbTesselator.h> 


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "../misc/GLWrapper.h"
#include <float.h>
#include <math.h>


/*!
  \enum SoExtSelection::LassoType
  Enum for type of lasso selection.
*/
/*!
  \var SoExtSelection::LassoType SoExtSelection::NOLASSO
  Makes this node behave like a normal SoSelection node.
*/
/*!
  \var SoExtSelection::LassoType SoExtSelection::LASSO
  Selecti objects using a lasso.
*/
/*!
  \var SoExtSelection::LassoType SoExtSelection::RECTANGLE
  Select objects using a rectangle.
*/

/*!
  \enum SoExtSelection::LassoPolicy
  Enum for specifying how objects are selected.
*/

/*!
  \var SoExtSelection::LassoPolicy SoExtSelection::FULL_BBOX
  The entire bounding box must be inside the lasso/rectangle.
*/
/*!
  \var SoExtSelection::LassoPolicy SoExtSelection::PART_BBOX
  Some part of the bounding box must intersect the lasso/rectangle.
*/
/*!
  \var SoExtSelection::LassoPolicy SoExtSelection::FULL
  All primitives must be completely inside the lasso/rectangle.
*/
/*!
  \var SoExtSelection::LassoPolicy SoExtSelection::PART
  Some primitive must intersect the lasso/rectangle.
*/


/*!
  \var SoSFEnum SoExtSelection::lassoType
  Field for lasso type. Default value is NOLASSO.
*/
/*!
  \var SoSFEnum SoExtSelection::lassoPolicy
  Field for lasso policy. Default value is FULL_BBOX.
*/
/*!
  \var SoSFEnum SoExtSelection::lassoMode

  Field for lasso mode. Default value is ALL_SHAPES.

  Set this field to VISIBLE_SHAPES to make only the primitives visible
  from the current viewpoint be selected.
*/

// *************************************************************************

#ifndef DOXYGEN_SKIP_THIS

// Debug defines... Used to force lower color resolution for testing purposes (handegar)
//#define BITCOLOR3 1
//#define BITCOLOR6 1
//#define BITCOLOR9 1


#define PRIVATE(p) (p->pimpl)
#define PUBLIC(p) (p->master)

class SoExtSelectionP {
public:
  SoExtSelectionP(SoExtSelection * master) {
    this->master = master;
  }

  SbColor lassocolor;
  float lassowidth;
  SbBool lassopatternanimate;
  unsigned short lassopattern;

  enum SelectionState {
    NONE,
    RECTANGLE,
    LASSO
  };

  SelectionState selectionstate;
  SbBool isDragging;  // 0=no, 1=currently dragging a new point (mouse = last pos)

  SbVec2s previousmousecoords;

  SbList <SbVec2s> coords;
  SoTimerSensor * timersensor;
  SoCallbackAction * cbaction;

  const SbList <SbVec2s> & getCoords(void) const {
    return coords;
  }

  SoExtSelection * master;
  SbViewportRegion curvp;

  void addTriangleToOffscreenBuffer(SoCallbackAction * action,
                                    const SoPrimitiveVertex * v1,
                                    const SoPrimitiveVertex * v2,
                                    const SoPrimitiveVertex * v3,
                                    SbBool renderAsBlack);

  void addLineToOffscreenBuffer(SoCallbackAction * action,
                                const SoPrimitiveVertex * v1,
                                const SoPrimitiveVertex * v2,
                                SbBool renderAsBlack);

  void addPointToOffscreenBuffer(SoCallbackAction * action,
                                 const SoPrimitiveVertex * v1,
                                 SbBool renderAsBlack);
  
  int scanOffscreenBuffer(SoNode * root);

  void checkOffscreenRendererCapabilities();

  static void offscreenRenderCallback(void * userdata, SoAction * action);

  static void offscreenRenderLassoCallback(void * userdata, SoAction * action);

  static void timercallback(void * data, SoSensor * sensor);
  
  static SoCallbackAction::Response preShapeCallback(void * data,
                                                     SoCallbackAction * action,
                                                     const SoNode * node);

  static SoCallbackAction::Response postShapeCallback(void * data,
                                                      SoCallbackAction * action,
                                                      const SoNode * node);

  static SoCallbackAction::Response cameraCB(void * data,
                                             SoCallbackAction * action,
                                             const SoNode * node);

  SoCallbackAction::Response testShape(SoCallbackAction * action, const SoShape * shape);

  SoCallbackAction::Response testBBox(SoCallbackAction * action,
                                      const SbMatrix & projmatrix,
                                      const SoShape * shape,
                                      const SbBox2s & lassorect,
                                      const SbBool full);

  SoCallbackAction::Response testPrimitives(SoCallbackAction * action,
                                            const SbMatrix & projmatrix,
                                            const SoShape * shape,
                                            const SbBox2s & lassorect,
                                            const SbBool full);

  static void offscreenLassoTesselatorCallback(void * v0, void * v1, void * v2, void * userdata);

  static void triangleCB(void * userData,
                         SoCallbackAction * action,
                         const SoPrimitiveVertex * v1,
                         const SoPrimitiveVertex * v2,
                         const SoPrimitiveVertex * v3);

  static void lineSegmentCB(void * userData,
                            SoCallbackAction * action,
                            const SoPrimitiveVertex * v1,
                            const SoPrimitiveVertex * v2);

  static void pointCB(void * userData,
                      SoCallbackAction * action,
                      const SoPrimitiveVertex * v);

  void performSelection(SoHandleEventAction * action);

  // Note: Microsoft Visual C++ 6.0 needs to have a type definition
  // and an explicit variable declaration, just using
  //     struct { ... } structname;
  // won't do.
  typedef struct
  {
    SbMatrix projmatrix;
    SbBool fulltest;
    SbBox2s lassorect;
    SbBool hit;
    SbVec2s vporg;
    SbVec2s vpsize;
    SbBool abort;
    SbBool allhit;
    SbBool onlyrect;
    SbBool allshapes;
  } primcbdata_t;
  primcbdata_t primcbdata;

  void doSelect(const SoPath * path);
  void selectPaths();
  SoLassoSelectionFilterCB * filterCB;
  void * filterCBData;

  SoExtSelectionTriangleCB * triangleFilterCB;
  void * triangleFilterCBData;
  SoExtSelectionLineSegmentCB * lineFilterCB;
  void * lineFilterCBData;
  SoExtSelectionPointCB * pointFilterCB;
  void * pointFilterCBData;

  SbBool callfiltercbonlyifselectable;
  SbBool wasshiftdown;

  SoHandleEventAction *offscreenaction;
  SbViewVolume offscreenviewvolume;
  int offscreencolorcounter;
  int offscreencolorcounterpasses;
  int offscreenskipcounter;
  SbBool offscreencolorcounteroverflow;
  SoOffscreenRenderer * renderer;
  SoOffscreenRenderer * lassorenderer;

  SbBool lassostencilisdrawed;
  SbBool applyonlyonselectedtriangles;

  int colorbitsred;
  int colorbitsgreen;
  int colorbitsblue;
  int colorbitsalpha;
  int maximumcolorcounter;

  unsigned char *visibletrianglesbitarray;

  SoNode *offscreenheadnode;
  int drawcallbackcounter;
  int drawcounter;
  SoPathList *visitedshapepaths;
  SbBool somefacesvisible;

};

#endif // DOXYGEN_SKIP_THIS


//
// Faster line segment intersection by Franklin Antonio, from Graphics Gems III.
//

static SbBool
line_line_intersect(const SbVec2s &p00, const SbVec2s & p01,
                    const SbVec2s &p10, const SbVec2s & p11)
{
  /* The SAME_SIGNS macro assumes arithmetic where the exclusive-or    */
  /* operation will work on sign bits.  This works for twos-complement,*/
  /* and most other machine arithmetic.                                */
#define SAME_SIGNS( a, b ) \
	(((int) ((unsigned int) a ^ (unsigned int) b)) >= 0 )

#define DONT_INTERSECT FALSE
#define COLINEAR TRUE
#define DO_INTERSECT TRUE

  int x1 = p00[0];
  int y1 = p00[1];
  int x2 = p01[0];
  int y2 = p01[1];

  int x3 = p10[0];
  int y3 = p10[1];
  int x4 = p11[0];
  int y4 = p11[1];

  int Ax,Bx,Cx,Ay,By,Cy,d,e,f;
  int x1lo,x1hi,y1lo,y1hi;

  Ax = x2-x1;
  Bx = x3-x4;

  if(Ax < 0) {						/* X bound box test*/
    x1lo = x2; x1hi = x1;
  }
  else {
    x1hi = x2; x1lo = x1;
  }
  if(Bx > 0) {
    if(x1hi < x4 || x3 < x1lo) return DONT_INTERSECT;
  }
  else {
    if(x1hi < x3 || x4 < x1lo) return DONT_INTERSECT;
  }

  Ay = y2-y1;
  By = y3-y4;

  if (Ay < 0) {						/* Y bound box test*/
    y1lo = y2; y1hi = y1;
  }
  else {
    y1hi = y2; y1lo = y1;
  }
  if (By > 0) {
    if (y1hi < y4 || y3 < y1lo) return DONT_INTERSECT;
  }
  else {
    if (y1hi < y3 || y4 < y1lo) return DONT_INTERSECT;
  }

  Cx = x1-x3;
  Cy = y1-y3;
  d = By*Cx - Bx*Cy;					/* alpha numerator*/
  f = Ay*Bx - Ax*By;					/* both denominator*/

  if (f > 0) {						/* alpha tests*/
    if (d < 0 || d > f) return DONT_INTERSECT;
  }
  else {
    if (d > 0 || d < f) return DONT_INTERSECT;
  }

  e = Ax*Cy - Ay*Cx;					/* beta numerator*/
  if (f > 0) {						/* beta tests*/
    if (e < 0 || e > f) return DONT_INTERSECT;
  }
  else {
    if (e > 0 || e < f) return DONT_INTERSECT;
  }

  /*compute intersection coordinates*/
  if (f == 0) return COLINEAR;
#if 0 // we don't need the intersection point, disabled
  int num, offset;
  int x, y;
  num = d*Ax;						/* numerator */
  offset = SAME_SIGNS(num,f) ? f/2 : -f/2;		/* round direction*/
  x = x1 + (num+offset) / f;				/* intersection x */

  num = d*Ay;
  offset = SAME_SIGNS(num,f) ? f/2 : -f/2;
  y = y1 + (num+offset) / f;				/* intersection y */
#endif // disabled code

  return DO_INTERSECT;
#undef COLINEAR
#undef DONT_INTERSECT
#undef DO_INTERSECT
#undef SAME_SIGNS
}

// The following code is by Randolph Franklin,
// it returns 1 for interior points and 0 for exterior points.
// http://astronomy.swin.edu.au/pbourke/geometry/insidepoly/

static SbBool
point_in_poly(const SbList <SbVec2s> & coords, const SbVec2s & point)
{
  int i, j;
  SbBool c = FALSE;
  int npol = coords.getLength();
  float x = (float) point[0];
  float y = (float) point[1];
  SbVec2f pi, pj;

  for (i = 0, j = npol-1; i < npol; j = i++) {

    pi[0] = (float) coords[i][0];
    pi[1] = (float) coords[i][1];
    pj[0] = (float) coords[j][0];
    pj[1] = (float) coords[j][1];

    if ((((pi[1] <= y) && (y < pj[1])) ||
	 ((pj[1] <= y) && (y < pi[1]))) &&
	(x < (pj[0] - pi[0]) * (y - pi[1]) / (pj[1] - pi[1]) + pi[0]))
      c = !c;
  }
  return c;
}

// do a bbox rejection test before calling this method. It's not fast,
// but testing will usually (always) be done on polygon vs triangle in
// which case it should be pretty fast.
static SbBool
poly_poly_intersect(const SbList <SbVec2s> & poly1,
                    const SbList <SbVec2s> & poly2)
{
  int i;
  int n1 = poly1.getLength();
  int n2 = poly2.getLength();

  if (n1 < n2) {
    for (i = 0; i < n1; i++) {
      if (point_in_poly(poly2, poly1[i])) return TRUE;
    }
    for (i = 0; i < n2; i++) {
      if (point_in_poly(poly1, poly2[i])) return TRUE;
    }
  }
  else {
    for (i = 0; i < n2; i++) {
      if (point_in_poly(poly1, poly2[i])) return TRUE;
    }
    for (i = 0; i < n1; i++) {
      if (point_in_poly(poly2, poly1[i])) return TRUE;
    }
  }
  // warning O(n^2)
  SbVec2s prev1 = poly1[n1-1];
  for (i = 0; i < n1; i++) {
    SbVec2s prev2 = poly2[n2-1];
    for (int j = 0; j < n2; j++) {
      if (line_line_intersect(prev1, poly1[i], prev2, poly2[j])) return TRUE;
      prev2 = poly2[j];
    }
    prev1 = poly1[i];
  }
  return FALSE;
}

static SbBool
poly_line_intersect(const SbList <SbVec2s> & poly,
                    const SbVec2s & p0,
                    const SbVec2s & p1,
                    const SbBool checkcontained = TRUE)
{
  if (checkcontained && point_in_poly(poly, p0)) return TRUE;
  if (checkcontained && point_in_poly(poly, p1)) return TRUE;

  int n = poly.getLength();
  SbVec2s prev = poly[n-1];
  for (int i = 0; i < n; i++) {
    if (line_line_intersect(prev, poly[i], p0, p1)) return TRUE;
    prev = poly[i];
  }
  return FALSE;
}

// do a bbox rejection test before calling this method
static SbBool
poly_tri_intersect(const SbList <SbVec2s> & poly,
                   const SbVec2s & v0,
                   const SbVec2s & v1,
                   const SbVec2s & v2)
{
  SbList <SbVec2s> poly2;
  poly2.append(v0);
  poly2.append(v1);
  poly2.append(v2);
  return poly_poly_intersect(poly, poly2);
}

// only used by polyprojboxintersect()
static SbBool
test_quad_intersect(const SbList <SbVec2s> & poly,
                    const SbVec2s & p0,
                    const SbVec2s & p1,
                    const SbVec2s & p2,
                    const SbVec2s & p3)
{
  // test if front facing:
  SbVec2s v0 = p1-p0;
  SbVec2s v1 = p3-p0;
  int crossz = v0[0]*v1[1] - v0[1]*v1[0];
  if (crossz > 0) {
    SbList <SbVec2s> poly2;
    poly2.append(p0);
    poly2.append(p1);
    poly2.append(p2);
    poly2.append(p3);
    return poly_poly_intersect(poly, poly2);
  }
  return FALSE;
}

// do a bbox rejection test before calling this method
static SbBool
poly_projbox_intersect(const SbList <SbVec2s> & poly,
                       const SbVec2s * projpts)
{
  // test all size quads in the box
  if (test_quad_intersect(poly, projpts[0], projpts[1],
                          projpts[3], projpts[2])) return TRUE;
  if (test_quad_intersect(poly, projpts[1], projpts[5],
                          projpts[7], projpts[3])) return TRUE;
  if (test_quad_intersect(poly, projpts[2], projpts[3],
                          projpts[7], projpts[6])) return TRUE;
  if (test_quad_intersect(poly, projpts[4], projpts[0],
                          projpts[2], projpts[6])) return TRUE;
  if (test_quad_intersect(poly, projpts[4], projpts[5],
                          projpts[1], projpts[0])) return TRUE;
  if (test_quad_intersect(poly, projpts[6], projpts[7],
                          projpts[5], projpts[4])) return TRUE;

  return FALSE;
}


SO_NODE_SOURCE(SoExtSelection);

/*!
  Constructor.
*/
SoExtSelection::SoExtSelection(void)
{
  PRIVATE(this) = new SoExtSelectionP(this);

  SO_NODE_INTERNAL_CONSTRUCTOR(SoExtSelection);

  SO_NODE_ADD_FIELD(lassoType, (SoExtSelection::NOLASSO));
  SO_NODE_ADD_FIELD(lassoPolicy, (SoExtSelection::FULL_BBOX));
  SO_NODE_ADD_FIELD(lassoMode, (SoExtSelection::ALL_SHAPES));

  SO_NODE_DEFINE_ENUM_VALUE(LassoType, NOLASSO);
  SO_NODE_DEFINE_ENUM_VALUE(LassoType, LASSO);
  SO_NODE_DEFINE_ENUM_VALUE(LassoType, RECTANGLE);
  SO_NODE_SET_SF_ENUM_TYPE(lassoType, LassoType);

  SO_NODE_DEFINE_ENUM_VALUE(LassoPolicy, FULL_BBOX);
  SO_NODE_DEFINE_ENUM_VALUE(LassoPolicy, PART_BBOX);
  SO_NODE_DEFINE_ENUM_VALUE(LassoPolicy, FULL);
  SO_NODE_DEFINE_ENUM_VALUE(LassoPolicy, PART);
  SO_NODE_SET_SF_ENUM_TYPE(lassoPolicy, LassoPolicy);

  SO_NODE_DEFINE_ENUM_VALUE(LassoMode, VISIBLE_SHAPES);
  SO_NODE_DEFINE_ENUM_VALUE(LassoMode, ALL_SHAPES);
  SO_NODE_SET_SF_ENUM_TYPE(lassoMode, LassoMode);
 

  // setup timer
  PRIVATE(this)->timersensor = new SoTimerSensor(&SoExtSelectionP::timercallback,
                                        (void *)this);
  PRIVATE(this)->timersensor->setBaseTime(SbTime(0.0));
  PRIVATE(this)->timersensor->setInterval(SbTime(0.3));



  PRIVATE(this)->cbaction = new SoCallbackAction();
  
  PRIVATE(this)->cbaction->addPreCallback(SoShape::getClassTypeId(), 
				 SoExtSelectionP::preShapeCallback,
				 (void *) this);

  PRIVATE(this)->cbaction->addPostCallback(SoShape::getClassTypeId(), 
				  SoExtSelectionP::postShapeCallback,
                                  (void *) this);

  PRIVATE(this)->cbaction->addTriangleCallback(SoShape::getClassTypeId(), 
				      SoExtSelectionP::triangleCB,
                                      (void*) this);

  PRIVATE(this)->cbaction->addLineSegmentCallback(SoShape::getClassTypeId(), 
					 SoExtSelectionP::lineSegmentCB,
                                         (void*) this);

  PRIVATE(this)->cbaction->addPointCallback(SoShape::getClassTypeId(), 
				   SoExtSelectionP::pointCB,
                                   (void*) this);

  PRIVATE(this)->cbaction->addPostCallback(SoCamera::getClassTypeId(), 
				  SoExtSelectionP::cameraCB,
                                  (void *) this);


  // some init (just to be sure?)
  PRIVATE(this)->lassocolor = SbColor(1.0f, 1.0f, 1.0f);
  PRIVATE(this)->lassowidth = 1.0f;
  PRIVATE(this)->lassopatternanimate = TRUE;
  PRIVATE(this)->lassopattern = 0xf0f0;

  PRIVATE(this)->selectionstate = SoExtSelectionP::NONE;
  PRIVATE(this)->isDragging = FALSE;
  PRIVATE(this)->coords.truncate(0);

  PRIVATE(this)->filterCB = NULL;
  PRIVATE(this)->triangleFilterCB = NULL;
  PRIVATE(this)->lineFilterCB = NULL;
  PRIVATE(this)->pointFilterCB = NULL;
  PRIVATE(this)->previousmousecoords = SbVec2s(32767, 32767);

  PRIVATE(this)->drawcallbackcounter=0;
  PRIVATE(this)->drawcounter=0;
  PRIVATE(this)->visitedshapepaths = new SoPathList();
  PRIVATE(this)->somefacesvisible = FALSE;
}

/*!
  Destructor.
*/
SoExtSelection::~SoExtSelection()
{
  if (PRIVATE(this)->timersensor->isScheduled()) PRIVATE(this)->timersensor->unschedule();
  delete PRIVATE(this)->timersensor;
  delete PRIVATE(this)->cbaction;
}

// doc in parent
void
SoExtSelection::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoExtSelection, SO_FROM_INVENTOR_1);
}


/*!
  Specifies whether the overlay planes should be used to render the
  lasso.

  This method has been obsoleted in Coin, as most graphics cards comes
  without support for overlay rendering. A better strategy is to just
  "overlay" the lasso graphics on top of the scene after everything
  else has been rendered -- and this is the strategy we apply in Coin.
*/
void
SoExtSelection::useOverlay(SbBool overlay)
{
  COIN_OBSOLETED();
}

/*!
  Returns whether overlay planes are used to draw the lasso.

  \sa useOverlay().
*/
SbBool
SoExtSelection::isUsingOverlay(void)
{
  COIN_OBSOLETED();
  return FALSE;
}

/*!
  Returns the scene graph for overlay rendering. Will always return
  NULL in Coin, as this method has been obsoleted.

  (It is probably used in TGS Inventor from the SoXt / SoWin
  libraries' So[Xt|Win]RenderArea class to fetch the overlay graph to
  draw, and as such should be treated as an internal method.)
*/
SoSeparator *
SoExtSelection::getOverlaySceneGraph(void)
{
  COIN_OBSOLETED();
  return NULL;
}

/*!
  Obsoleted in Coin, use SoExtSelection::setLassoColor() instead.
*/
void
SoExtSelection::setOverlayLassoColorIndex(const int index)
{
  COIN_OBSOLETED();
}

/*!
  Obsoleted in Coin, use SoExtSelection::getLassoColor() instead.

  \sa setOverlayLassoColorIndex().
*/
int
SoExtSelection::getOverlayLassoColorIndex(void)
{
  COIN_OBSOLETED();
  return 0;
}

/*!
  Sets the lasso/rectangle line color. Default value is (1.0, 1.0,
  1.0).
*/
void
SoExtSelection::setLassoColor(const SbColor & color)
{
  PRIVATE(this)->lassocolor = color;
}

/*!
  Returns the lasso color.
*/
const SbColor &
SoExtSelection::getLassoColor(void)
{
  return PRIVATE(this)->lassocolor;
}

/*!
  Sets the lasso line width. Default value is 1.0.
*/
void
SoExtSelection::setLassoWidth(const float width)
{
  PRIVATE(this)->lassowidth = width;
}

/*!
  Returns the lasso line width.
*/
float
SoExtSelection::getLassoWidth(void)
{
  return PRIVATE(this)->lassowidth;
}

/*!
  Sets the lasso line pattern. Default value is 0xf0f0.
*/
void
SoExtSelection::setOverlayLassoPattern(const unsigned short pattern)
{
  PRIVATE(this)->lassopattern = pattern;
}

/*!
  Returns the lasso line pattern.
*/
unsigned short
SoExtSelection::getOverlayLassoPattern(void)
{
  return PRIVATE(this)->lassopattern;
}

/*!
  Sets whether the lasso should be animated by scrolling
  the line pattern.
*/
void
SoExtSelection::animateOverlayLasso(const SbBool animate)
{
  PRIVATE(this)->lassopatternanimate = animate;
}

/*!
  Returns whether the lasso is set to animate or not.
*/
SbBool
SoExtSelection::isOverlayLassoAnimated(void)
{
  return PRIVATE(this)->lassopatternanimate;
}

// Documented in superclass.
void
SoExtSelection::handleEvent(SoHandleEventAction * action)
{
  // Overridden to handle lasso selection.

  if (this->lassoType.getValue() == NOLASSO) {
    inherited::handleEvent(action);
    return;
  }
  SoSeparator::handleEvent(action);
  if (action->isHandled()) return;

  const SoEvent * event = action->getEvent();
  const SbVec2s mousecoords = event->getPosition();

  switch (this->lassoType.getValue()) {

    // ---------- NO LASSO ----------

  case SoExtSelection::NOLASSO:
    // nothing to do here..
    break;

    // ---------- RECTANGLE ----------

  case SoExtSelection::RECTANGLE:
    // mouse click
    if (SO_MOUSE_PRESS_EVENT(event,BUTTON1)) {
      PRIVATE(this)->isDragging = TRUE;
      PRIVATE(this)->selectionstate = SoExtSelectionP::RECTANGLE;
      PRIVATE(this)->coords.truncate(0);
      PRIVATE(this)->coords.append(mousecoords);
      PRIVATE(this)->coords.append(mousecoords);
      if (!PRIVATE(this)->timersensor->isScheduled()) PRIVATE(this)->timersensor->schedule();
    }
    // mouse release
    else if (SO_MOUSE_RELEASE_EVENT(event,BUTTON1)) {
      PRIVATE(this)->timersensor->unschedule();
      PRIVATE(this)->isDragging = FALSE;
      PRIVATE(this)->selectionstate = SoExtSelectionP::NONE;
      PRIVATE(this)->performSelection(action);
    }
    // mouse move
    else if ((event->isOfType( SoLocation2Event::getClassTypeId()))) {
      if (PRIVATE(this)->isDragging == TRUE) {
	assert(PRIVATE(this)->coords.getLength() >= 2);
	PRIVATE(this)->coords[1] = mousecoords;
	this->touch();
      }
    }

    break;

    // ---------- LASSO ----------

  case SoExtSelection::LASSO:
    // mouse click
    if (SO_MOUSE_PRESS_EVENT(event,BUTTON1)) {
      if ((SbAbs(mousecoords[0] - PRIVATE(this)->previousmousecoords[0]) > 2) ||
          (SbAbs(mousecoords[1] - PRIVATE(this)->previousmousecoords[1]) > 2)) {
	if (PRIVATE(this)->selectionstate == SoExtSelectionP::NONE) {
	  PRIVATE(this)->coords.truncate(0);
	  PRIVATE(this)->coords.append(mousecoords);
	  PRIVATE(this)->selectionstate = SoExtSelectionP::LASSO;
	}
        PRIVATE(this)->previousmousecoords = mousecoords;
	PRIVATE(this)->isDragging = TRUE;
	PRIVATE(this)->coords.append(mousecoords);
        if (!PRIVATE(this)->timersensor->isScheduled()) PRIVATE(this)->timersensor->schedule();
	this->touch();
      }
      else { // clicked twice on same coord (double click) -> end selection
	if (PRIVATE(this)->timersensor->isScheduled()) PRIVATE(this)->timersensor->unschedule();
        PRIVATE(this)->previousmousecoords = SbVec2s(32767, 32767);
	if (PRIVATE(this)->selectionstate == SoExtSelectionP::LASSO) {
          PRIVATE(this)->performSelection(action);
	}
	PRIVATE(this)->isDragging = FALSE;
	PRIVATE(this)->selectionstate = SoExtSelectionP::NONE;
	PRIVATE(this)->coords.truncate(0);
      }
    }
    // mouse move
    else if ( ( event->isOfType( SoLocation2Event::getClassTypeId() ) ) ) {
      if (PRIVATE(this)->isDragging == TRUE) {
	assert(PRIVATE(this)->coords.getLength());
	PRIVATE(this)->coords[PRIVATE(this)->coords.getLength()-1] = mousecoords;
	this->touch();
      }
    }
    // end selection with right-click
    else if (SO_MOUSE_PRESS_EVENT(event,BUTTON2)) {
      if (PRIVATE(this)->timersensor->isScheduled()) PRIVATE(this)->timersensor->unschedule();
      if (PRIVATE(this)->selectionstate == SoExtSelectionP::LASSO) {
        PRIVATE(this)->performSelection(action);
      }
      PRIVATE(this)->isDragging = FALSE;
      if (PRIVATE(this)->timersensor->isScheduled()) PRIVATE(this)->timersensor->unschedule();
      PRIVATE(this)->selectionstate = SoExtSelectionP::NONE;
      PRIVATE(this)->previousmousecoords = SbVec2s(32767, 32767);
      PRIVATE(this)->coords.truncate(0);
    }
    break;
  }
}

// internal method for drawing lasso
void
SoExtSelection::draw(SoGLRenderAction *action)
{
  const GLWrapper_t * glw = GLWrapper(action->getCacheContext());

  SbViewportRegion vp = SoViewportRegionElement::get(action->getState());
  SbVec2s vpo = vp.getViewportOriginPixels();
  SbVec2s vps = vp.getViewportSizePixels();

  // matrices
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(vpo[0], vpo[0]+vps[0]-1,
	  vpo[1], vpo[0]+vps[1]-1,
	  -1, 1);

  // attributes
  glPushAttrib(GL_LIGHTING_BIT|
	       GL_FOG_BIT|
	       GL_DEPTH_BUFFER_BIT|
	       GL_TEXTURE_BIT|
	       GL_LINE_BIT|
	       GL_CURRENT_BIT);
  glDisable(GL_LIGHTING);
  glDisable(GL_TEXTURE_2D);
  if (glw->has3DTextures) glDisable(GL_TEXTURE_3D);
  glDisable(GL_FOG);
  glDisable(GL_DEPTH_TEST);

  // line color & width
  glColor3f(PRIVATE(this)->lassocolor[0],PRIVATE(this)->lassocolor[1],PRIVATE(this)->lassocolor[2]);
  glLineWidth(PRIVATE(this)->lassowidth);

  // stipple pattern
  glEnable(GL_LINE_STIPPLE);
  glLineStipple(1, PRIVATE(this)->lassopattern);

  // --- RECTANGLE ---

  if (PRIVATE(this)->selectionstate == SoExtSelectionP::RECTANGLE) {
    assert(PRIVATE(this)->coords.getLength() >= 2);
    SbVec2s c1 = PRIVATE(this)->coords[0];
    SbVec2s c2 = PRIVATE(this)->coords[1];
    glBegin(GL_LINE_LOOP);
    glVertex2s(c1[0], c1[1]);
    glVertex2s(c2[0], c1[1]);
    glVertex2s(c2[0], c2[1]);
    glVertex2s(c1[0], c2[1]);
    glEnd();
  }

  // --- LASSO ---

  else if (PRIVATE(this)->selectionstate == SoExtSelectionP::LASSO) {
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < PRIVATE(this)->coords.getLength(); i++) {
      SbVec2s temp = PRIVATE(this)->coords[i];
      glVertex2s(temp[0],temp[1]);
    }
    glEnd();
  }

  // finish - restore state
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  glPopAttrib();
}

// Documented in superclass.
void
SoExtSelection::GLRenderBelowPath(SoGLRenderAction * action)
{
  // Overridden to render lasso.

  inherited::GLRenderBelowPath(action);
  SoState * state = action->getState();
  state->push();

  if (action->isRenderingDelayedPaths()) {

    SbViewportRegion vp = SoViewportRegionElement::get(state);
    SbVec2s vpo = vp.getViewportOriginPixels();
    SbVec2s vps = vp.getViewportSizePixels();
    this->draw(action);
  }
  // render this path after all other (delayed)
  else {
    if (PRIVATE(this)->selectionstate != SoExtSelectionP::NONE) {
      action->addDelayedPath(action->getCurPath()->copy());
    }
  }
  state->pop();
}

/*!

  The lasso selection filter callback is called when a node is about
  to be selected, and enables the application programmer to return a
  new path to be used when selecting. The new returned path should
  not be ref'd. SoExtSelection will ref() and unref() it.

  To cancel the selection, return NULL from the callback.

  if \a callonlyifselectable is TRUE, the callback will only be
  invoked when the path to the new node pass through the
  SoExtSelection node.

  This method is specific to Coin, and is not part of TGS OIV.
*/

void
SoExtSelection::setLassoFilterCallback(SoLassoSelectionFilterCB * f, void * userdata,
                                       const SbBool callonlyifselectable)
{
  PRIVATE(this)->filterCB = f;
  PRIVATE(this)->filterCBData = userdata;
  PRIVATE(this)->callfiltercbonlyifselectable = callonlyifselectable;
}

/*!
  Sets the callback that will be called for every triangle inside the
  lasso/rectangle when selecting. The user should return \e FALSE if
  he/she wants to continue receiving callbacks. When the user returns
  \e TRUE, the object/shape is selected, and no more callbacks will be
  invoked for the object.
*/
void
SoExtSelection::setTriangleFilterCallback(SoExtSelectionTriangleCB * func,
                                          void * userdata)
{
  PRIVATE(this)->triangleFilterCB = func;
  PRIVATE(this)->triangleFilterCBData = userdata;
}

/*!
  Sets the callback that will be called for every line segment inside
  the lasso/rectangle when selecting. The user should return \e FALSE
  if he/she wants to continue receiving callbacks. When the user
  returns \e TRUE, the object/shape is selected, and no more callbacks
  will be invoked for the object.
*/
void
SoExtSelection::setLineSegmentFilterCallback(SoExtSelectionLineSegmentCB * func,
                                             void * userdata)
{
  PRIVATE(this)->lineFilterCB = func;
  PRIVATE(this)->lineFilterCBData = userdata;
}

/*!
  Sets the callback that will be called for every point inside the
  lasso/rectangle when selecting. The user should return \e FALSE if
  he/she wants to continue receiving callbacks. When the user returns
  \e TRUE, the object/shape is selected, and no more callbacks will be
  invoked for the object.
*/
void
SoExtSelection::setPointFilterCallback(SoExtSelectionPointCB * func,
                                       void * userdata)
{
  PRIVATE(this)->pointFilterCB = func;
  PRIVATE(this)->pointFilterCBData = userdata;
}

/*!
  Returns whether the SHIFT key was pressed during the latest
  user interaction. This is useful if you want to respect the
  shift policy while selecting primitives.

  This method is specific to Coin, and is not part of TGS OIV.
*/
SbBool
SoExtSelection::wasShiftDown(void) const
{
  return PRIVATE(this)->wasshiftdown;
}

#ifndef DOXYGEN_SKIP_THIS

// timer callback for rendering lasso animation.
void
SoExtSelectionP::timercallback(void *data, SoSensor *sensor)
{
  SoExtSelection * ext = (SoExtSelection *)data;
  if (ext == NULL) return;
  if (ext->isOverlayLassoAnimated()) {
    int pat = ext->getOverlayLassoPattern();
    int pat2 = pat << 1;
    if ((pat & 0x8000) != 0) pat2 |= 1;
    ext->setOverlayLassoPattern(pat2 & 0xffff);
    ext->touch();
  }
}

// callback that is called for a shape before processing primitives.
SoCallbackAction::Response
SoExtSelectionP::preShapeCallback(void *data, SoCallbackAction *action, const SoNode *node)
{
  SoExtSelection * ext = (SoExtSelection *)data;
  assert(node->isOfType(SoShape::getClassTypeId()));

  PRIVATE(ext)->somefacesvisible = FALSE;
  
  return PRIVATE(ext)->testShape(action, (const SoShape*) node);
}


// callback that is called for a shape after all primitives have been processed
SoCallbackAction::Response
SoExtSelectionP::postShapeCallback(void *data, SoCallbackAction *action, const SoNode *node)
{
  SoExtSelection * ext = (SoExtSelection*)data;

  SbBool hit = FALSE;
  switch (ext->lassoPolicy.getValue()) {
  case SoExtSelection::FULL:
    hit = PRIVATE(ext)->primcbdata.allhit;
    break;
  case SoExtSelection::PART:
    hit = PRIVATE(ext)->primcbdata.hit;
    break;
  default:
    break;
  }
  
  if(hit){

    if(!PRIVATE(ext)->primcbdata.allshapes){ //VISIBLE_SHAPES

      if((ext->lassoPolicy.getValue() == SoExtSelection::FULL) &&
         (!PRIVATE(ext)->somefacesvisible))
        return SoCallbackAction::CONTINUE;

      // FIXME: This is a linear search. Storing paths should have be done using
      // something more sophisticated for better performance. (handegar)
      const SoPath * curpath = action->getCurPath();
      if(PRIVATE(ext)->visitedshapepaths->findPath(*curpath) < 0)
        PRIVATE(ext)->visitedshapepaths->append(curpath->copy());

    } else {
      PRIVATE(ext)->doSelect(action->getCurPath());
    }
    
  }

  return SoCallbackAction::CONTINUE;
}

// SoCamera callback for callback action. Will set view volume planes
// in SoCullElement to optimize picking.
SoCallbackAction::Response
SoExtSelectionP::cameraCB(void * data,
                          SoCallbackAction * action,
                          const SoNode * node)
{
  SoExtSelection * thisp = (SoExtSelection*) data;

  SoState * state = action->getState();
  SbViewVolume vv = SoViewVolumeElement::get(state);
  const SbViewportRegion & vp = SoViewportRegionElement::get(state);

  SbBox2s rectbbox;
  for (int i = 0; i < PRIVATE(thisp)->coords.getLength(); i++) {
    rectbbox.extendBy(PRIVATE(thisp)->coords[i]);
  }

  SbVec2s org = vp.getViewportOriginPixels();
  SbVec2s siz = vp.getViewportSizePixels();
  float left = float(rectbbox.getMin()[0] - org[0]) / float(siz[0]);
  float bottom = float(rectbbox.getMin()[1] - org[1]) / float(siz[1]);

  float right = float(rectbbox.getMax()[0] - org[0]) / float(siz[0]);
  float top = float(rectbbox.getMax()[1] - org[1]) / float(siz[1]);

  // increment to avoid empty view volume
  if (left == right) right += 0.001f;
  if (top == bottom) top += 0.001f;

  vv = vv.narrow(left, bottom, right, top);
  SoCullElement::setViewVolume(state, vv);
  return SoCallbackAction::CONTINUE;
}

//
SoCallbackAction::Response
SoExtSelectionP::testShape(SoCallbackAction * action, const SoShape * shape)
{
  int i;
  SoState * state = action->getState();

  SbBox2s rectbbox;
  for (i = 0; i < this->coords.getLength(); i++) {
    rectbbox.extendBy(this->coords[i]);
  }

  SbMatrix projmatrix;
  projmatrix = (SoModelMatrixElement::get(state) *
                SoViewingMatrixElement::get(state) *
                SoProjectionMatrixElement::get(state));

  SbBool full = FALSE;
  switch (PUBLIC(this)->lassoPolicy.getValue()) {
  case SoExtSelection::FULL_BBOX:
    full = TRUE;
  case SoExtSelection::PART_BBOX:
    return testBBox(action, projmatrix, shape, rectbbox, full);
  case SoExtSelection::FULL:
    full = TRUE;
  case SoExtSelection::PART:
    return testPrimitives(action, projmatrix, shape, rectbbox, full);
  default:
    assert(0 && "unknown lasso policy");
    break;
  }
  return SoCallbackAction::CONTINUE;
}

// project a point to screen
static SbVec2s
project_pt(const SbMatrix & projmatrix, const SbVec3f & v,
           const SbVec2s & vporg, const SbVec2s & vpsize)
{
  SbVec3f normpt;
  projmatrix.multVecMatrix(v, normpt);
  normpt[0] += 1.0f;
  normpt[1] += 1.0f;
  normpt[0] *= 0.5f;
  normpt[1] *= 0.5f;

  normpt[0] *= (float) vpsize[0];
  normpt[1] *= (float) vpsize[1];
  normpt[0] += (float) vporg[0];
  normpt[1] += (float) vporg[1];

  return SbVec2s((short) SbClamp(normpt[0], -32768.0f, 32767.0f),
                 (short) SbClamp(normpt[1], -32768.0f, 32767.0f));
}

// test for intersection between bbox and lasso/rectangle
SoCallbackAction::Response
SoExtSelectionP::testBBox(SoCallbackAction * action,
                          const SbMatrix & projmatrix,
                          const SoShape * shape,
                          const SbBox2s & lassorect,
                          const SbBool full)
{
  SbBox3f bbox;
  SbVec3f center;
  ((SoShape *)shape)->computeBBox(action, bbox, center);

  SbVec3f mincorner = bbox.getMin();
  SbVec3f maxcorner = bbox.getMax();

  SbBox2s shapebbox;

  SbVec2s vppt;
  SbVec3f normpt;
  SbVec2s vpo = this->curvp.getViewportOriginPixels();
  SbVec2s vps = this->curvp.getViewportSizePixels();

  SbVec2s projpts[8];

  for (int i = 0; i < 8; i++) {
    SbVec3f corner(i & 1 ? maxcorner[0] : mincorner[0],
                   i & 2 ? maxcorner[1] : mincorner[1],
                   i & 4 ? maxcorner[2] : mincorner[2]);
    vppt = project_pt(projmatrix, corner, vpo, vps);
    projpts[i] = vppt;
    shapebbox.extendBy(vppt);
  }
  if (lassorect.intersect(shapebbox)) { // quick reject
    int i;
    int hit = 0;
    switch (PUBLIC(this)->lassoType.getValue()) {
    case SoExtSelection::LASSO:
      if (full) {
        for (i = 0; i < 8; i++) {
          if (!point_in_poly(this->coords, projpts[i])) break;
        }
        if (i == 8) hit = TRUE;
      }
      else {
        hit = poly_projbox_intersect(this->coords, projpts);
      }
      break;
    case SoExtSelection::RECTANGLE:
      if (full) {
        for (i = 0; i < 8; i++) {
          if (!lassorect.intersect(projpts[i])) break;
        }
        if (i == 8) hit = TRUE;
      }
      else {
        for (i = 0; i < 8; i++) {
          if (lassorect.intersect(projpts[i])) { hit = TRUE; break; }
        }
      }
      break;
    default:
      break;
    }

    if(hit) 
      this->doSelect(action->getCurPath());
    
  }
  return SoCallbackAction::PRUNE; // we don't need do callbacks for primitives

}

// initialize some variables needed before receiving primitive callbacks.
SoCallbackAction::Response
SoExtSelectionP::testPrimitives(SoCallbackAction * action,
                                const SbMatrix & projmatrix,
                                const SoShape * /* shape */,
                                const SbBox2s & lassorect,
                                const SbBool full)
{
  // FIXME: consider quick reject based on bounding box for now we
  // just initialize some variables, and trust that the user has a
  // sensible scene graph so that shapes are culled in the separators.

  this->primcbdata.fulltest = full;
  this->primcbdata.projmatrix = projmatrix;
  this->primcbdata.lassorect = lassorect;
  this->primcbdata.hit = FALSE;
  this->primcbdata.allhit = TRUE;
  this->primcbdata.vporg = SoViewportRegionElement::get(action->getState()).getViewportOriginPixels();
  this->primcbdata.vpsize = SoViewportRegionElement::get(action->getState()).getViewportSizePixels();
  this->primcbdata.abort = FALSE;
  this->primcbdata.onlyrect = (PUBLIC(this)->lassoType.getValue() == SoExtSelection::LASSO);
  // signal to callback action that we want to generate primitives for
  // this shape
  return SoCallbackAction::CONTINUE;
}



// triangle callback from SoCallbackAction
void
SoExtSelectionP::triangleCB(void * userData,
                            SoCallbackAction * action,
                            const SoPrimitiveVertex * v1,
                            const SoPrimitiveVertex * v2,
                            const SoPrimitiveVertex * v3)
{
  SoExtSelectionP * thisp = ((SoExtSelection*)userData)->pimpl;
  
  thisp->drawcallbackcounter++;

  thisp->addTriangleToOffscreenBuffer(action, v1, v2, v3, TRUE);
  
  // Shall we skip a certain amount of triangles before we start processing?
  if(thisp->offscreenskipcounter < (thisp->maximumcolorcounter*thisp->offscreencolorcounterpasses-1)){
    ++thisp->offscreenskipcounter;
    return;
  }

  // Increase draw counter. Used below.
  thisp->drawcounter++;


  if(thisp->primcbdata.abort) return;

  if(!thisp->triangleFilterCB && thisp->primcbdata.fulltest &&
     !thisp->primcbdata.allhit) {
    thisp->primcbdata.abort = TRUE;
    return;
  }
  if(!thisp->triangleFilterCB && !thisp->primcbdata.fulltest &&
     thisp->primcbdata.hit) {
    thisp->primcbdata.abort = TRUE;
    return;
  }
  

  SbVec2s p0 = project_pt(thisp->primcbdata.projmatrix, v1->getPoint(),
                          thisp->primcbdata.vporg, thisp->primcbdata.vpsize);
  SbVec2s p1 = project_pt(thisp->primcbdata.projmatrix, v2->getPoint(),
                          thisp->primcbdata.vporg, thisp->primcbdata.vpsize);
  SbVec2s p2 = project_pt(thisp->primcbdata.projmatrix, v3->getPoint(),
                          thisp->primcbdata.vporg, thisp->primcbdata.vpsize);


 
  if(thisp->primcbdata.fulltest) { // entire triangle must be inside lasso
    
    if(thisp->master->lassoType.getValue() == SoExtSelection::RECTANGLE){ // Rectangle check only
      if (!thisp->primcbdata.lassorect.intersect(p0) || (!point_in_poly(thisp->coords, p0))) {
        thisp->primcbdata.allhit = FALSE;
        return;
      }
      if (!thisp->primcbdata.lassorect.intersect(p1) || (!point_in_poly(thisp->coords, p1))) {
        thisp->primcbdata.allhit = FALSE;
        return;
      }
      if (!thisp->primcbdata.lassorect.intersect(p2) || (!point_in_poly(thisp->coords, p2))) {
        thisp->primcbdata.allhit = FALSE;
        return;
      }      
    }
    
    if(poly_line_intersect(thisp->coords, p0, p1, FALSE)) {
      thisp->primcbdata.allhit = FALSE;
      return;
    }
    if(poly_line_intersect(thisp->coords, p1, p2, FALSE)) {
      thisp->primcbdata.allhit = FALSE;
      return;
    }
    if(poly_line_intersect(thisp->coords, p2, p0, FALSE)) {
      thisp->primcbdata.allhit = FALSE;
      return;
    }
    

  } else { // some part of the triangle must be inside lasso
    if (!poly_tri_intersect(thisp->coords, p0, p1, p2)) {
      thisp->primcbdata.allhit = FALSE;
      return;
    }
  }
  
  
  
  if(!thisp->applyonlyonselectedtriangles){ // --- First pass
    
    if(!thisp->primcbdata.allshapes){ // LassoMode==VISIBLE_SHAPES
      
      if(thisp->drawcounter > thisp->maximumcolorcounter)
        thisp->offscreencolorcounteroverflow = TRUE;
      
      thisp->addTriangleToOffscreenBuffer(action, v1, v2, v3, thisp->offscreencolorcounteroverflow);

    } else if (thisp->triangleFilterCB && thisp->primcbdata.allshapes) {
      
      // Present accepted triangle to 'user' through a callback. 
      if (thisp->triangleFilterCB(thisp->triangleFilterCBData,
				  action, v1, v2, v3)) {
	// Select shape.
	thisp->primcbdata.hit = TRUE;
	thisp->primcbdata.allhit = TRUE;
	thisp->primcbdata.abort = TRUE;
      }
      
    } else {
      thisp->primcbdata.hit = TRUE;
    }  
    
  } else {  // --- Second pass. Feeding visible tris to client.
    

    if(thisp->drawcounter > thisp->maximumcolorcounter){
      thisp->offscreencolorcounteroverflow = TRUE;
      return;
    }
    
    int flag = 0x1 << (thisp->offscreencolorcounter & 0x07);
    int index = thisp->offscreencolorcounter >> 3;
    
    if(thisp->visibletrianglesbitarray[index] & flag){

      thisp->somefacesvisible = TRUE;
      if(thisp->triangleFilterCB(thisp->triangleFilterCBData, action, v1, v2, v3)){
	thisp->primcbdata.hit = TRUE;
	thisp->primcbdata.allhit = TRUE;
      }

    }

    ++thisp->offscreencolorcounter;
    
  }
  
}



void
SoExtSelectionP::addTriangleToOffscreenBuffer(SoCallbackAction * action,
					      const SoPrimitiveVertex * v1,
					      const SoPrimitiveVertex * v2,
					      const SoPrimitiveVertex * v3,
					      SbBool renderAsBlack)
{

  if(primcbdata.allshapes)
    return;

  SoState * state = action->getState();
  SbMatrix proj, affine;
  const SbMatrix & mm = SoModelMatrixElement::get(state);
  offscreenviewvolume.getMatrices(affine, proj);
  affine.multLeft(mm);

  glMatrixMode(GL_PROJECTION);
  glLoadMatrixf((float *)proj);
  glMatrixMode(GL_MODELVIEW);
  glLoadMatrixf((float *)affine);

  glDepthFunc(GL_LEQUAL);

  // Check vertex ordrering
  SoShapeHintsElement::VertexOrdering vertexorder;
  SoShapeHintsElement::ShapeType shapetype;
  SoShapeHintsElement::FaceType facetype; //Unused.
  SoShapeHintsElement::get(state, vertexorder, shapetype, facetype);

    
  if(shapetype == SoShapeHintsElement::SOLID){
    if(vertexorder == SoShapeHintsElement::CLOCKWISE){
      glFrontFace(GL_CW);
      glEnable(GL_CULL_FACE);
    } else if(vertexorder == SoShapeHintsElement::COUNTERCLOCKWISE){
      glFrontFace(GL_CCW);
      glEnable(GL_CULL_FACE);
    } else {
      glDisable(GL_CULL_FACE);
    }
  } else {
    glDisable(GL_CULL_FACE);
  }
  
  glBegin(GL_TRIANGLES);    

  if(!renderAsBlack){
    glColor3ub((unsigned char) (this->offscreencolorcounter>>(8+8)),
	       (unsigned char) (this->offscreencolorcounter>>(8)),
	       (unsigned char) (this->offscreencolorcounter));
    ++offscreencolorcounter;
  } else {
    glColor3f(0,0,0);
  }

  glVertex3fv(v1->getPoint().getValue());
  glVertex3fv(v2->getPoint().getValue());
  glVertex3fv(v3->getPoint().getValue());
  glEnd();

}


// line segment callback from SoCallbackAction
void
SoExtSelectionP::lineSegmentCB(void *userData,
                               SoCallbackAction * action,
                               const SoPrimitiveVertex * v1,
                               const SoPrimitiveVertex * v2)
{
  SoExtSelectionP * thisp = ((SoExtSelection*)userData)->pimpl;
  thisp->drawcallbackcounter++;
  thisp->addLineToOffscreenBuffer(action, v1, v2, TRUE);

  // Shall we skip a certain amount of lines before we start processing?
  if(thisp->offscreenskipcounter < (thisp->maximumcolorcounter*thisp->offscreencolorcounterpasses-1)){
    ++thisp->offscreenskipcounter;
    //thisp->addLineToOffscreenBuffer(action, v1, v2, TRUE);
 
    return;
  }

  // Increase draw counter. Used below.
  thisp->drawcounter++;

  if(thisp->primcbdata.abort) return;

  if (!thisp->lineFilterCB && thisp->primcbdata.fulltest &&
      !thisp->primcbdata.allhit) {
    thisp->primcbdata.abort = TRUE;
    return;
  }
  if (!thisp->lineFilterCB && !thisp->primcbdata.fulltest &&
      thisp->primcbdata.hit) {
    thisp->primcbdata.abort = TRUE;
    return;
  }
  
  SbBool onlyrect = thisp->primcbdata.onlyrect;

  SbVec2s p0 = project_pt(thisp->primcbdata.projmatrix, v1->getPoint(),
                          thisp->primcbdata.vporg, thisp->primcbdata.vpsize);
  
  SbVec2s p1 = project_pt(thisp->primcbdata.projmatrix, v2->getPoint(),
                          thisp->primcbdata.vporg, thisp->primcbdata.vpsize);
  
  if (thisp->primcbdata.fulltest) {


    if(thisp->master->lassoType.getValue() == SoExtSelection::RECTANGLE){ // Rectangle check only  
      if (!thisp->primcbdata.lassorect.intersect(p0) || (onlyrect || !point_in_poly(thisp->coords, p0))) {
        thisp->primcbdata.allhit = FALSE;
        return;
      }
      
      if (!thisp->primcbdata.lassorect.intersect(p1) || (onlyrect || !point_in_poly(thisp->coords, p1))) {
        thisp->primcbdata.allhit = FALSE;
        return;
      }
    }
    
    if (poly_line_intersect(thisp->coords, p0, p1, FALSE)) {
      thisp->primcbdata.allhit = FALSE;
      return;
    }
  }
  else {
    if (!poly_line_intersect(thisp->coords, p0, p1, TRUE)) {
      thisp->primcbdata.allhit = FALSE;
      return;
    }
  }


  if(!thisp->applyonlyonselectedtriangles){ // --- First pass

    // line segment is hit/surrounded by the lasso
    if(!thisp->primcbdata.allshapes){

      if(thisp->drawcounter > thisp->maximumcolorcounter)
        thisp->offscreencolorcounteroverflow = TRUE;
      
      thisp->addLineToOffscreenBuffer(action, v1, v2, thisp->offscreencolorcounteroverflow);
      
    } else if (thisp->lineFilterCB && thisp->primcbdata.allshapes) {

      if (thisp->lineFilterCB(thisp->lineFilterCBData,
                              action, v1, v2)) {
        // select shape
        thisp->primcbdata.hit = TRUE;
        thisp->primcbdata.allhit = TRUE;
        thisp->primcbdata.abort = TRUE;
      }
    }
    else thisp->primcbdata.hit = TRUE;

  } else { // ---- Second pass
    
    if(thisp->drawcounter > thisp->maximumcolorcounter){
      thisp->offscreencolorcounteroverflow = TRUE;
      return;
    }
    
    int flag = 0x1 << (thisp->offscreencolorcounter & 0x07);
    int index = thisp->offscreencolorcounter >> 3;
    
    if(thisp->visibletrianglesbitarray[index] & flag){
      
      if(thisp->lineFilterCB(thisp->lineFilterCBData, action, v1, v2)){
	thisp->primcbdata.hit = TRUE;
	thisp->primcbdata.allhit = TRUE;
      }
    }
    
    ++thisp->offscreencolorcounter;
    
  }


}



void
SoExtSelectionP::addLineToOffscreenBuffer(SoCallbackAction * action,
                                          const SoPrimitiveVertex * v1,
                                          const SoPrimitiveVertex * v2,
                                          SbBool renderAsBlack)
{

  if(primcbdata.allshapes)
    return;

  SoState * state = action->getState();
  SbMatrix proj, affine;
  const SbMatrix & mm = SoModelMatrixElement::get(state);
  offscreenviewvolume.getMatrices(affine, proj);
  affine.multLeft(mm);
  
  glMatrixMode(GL_PROJECTION);
  glLoadMatrixf((float *)proj);
  glMatrixMode(GL_MODELVIEW);
  glLoadMatrixf((float *)affine);
  
  glDepthFunc(GL_LEQUAL);


  glBegin(GL_LINES);    

  if(!renderAsBlack){
    glColor3ub((unsigned char) (this->offscreencolorcounter>>(8+8)),
	       (unsigned char) (this->offscreencolorcounter>>(8)),
	       (unsigned char) (this->offscreencolorcounter));
    ++offscreencolorcounter;
  } else {
    glColor3f(0,0,0);
  }

  glVertex3fv(v1->getPoint().getValue());
  glVertex3fv(v2->getPoint().getValue());
  glEnd();

}



// line segment callback from SoCallbackAction
void
SoExtSelectionP::pointCB(void *userData,
                         SoCallbackAction *action,
                         const SoPrimitiveVertex * v)
{

  SoExtSelectionP * thisp = ((SoExtSelection*)userData)->pimpl;
  thisp->drawcallbackcounter++;
  
  thisp->addPointToOffscreenBuffer(action, v, TRUE);
  
  // Shall we skip a certain amount of lines before we start processing?
  if(thisp->offscreenskipcounter < (thisp->maximumcolorcounter*thisp->offscreencolorcounterpasses-1)){
    ++thisp->offscreenskipcounter;
    return;
  }

  // Increase draw counter. (Used below).
  thisp->drawcounter++;
  

  if(thisp->primcbdata.abort){ 
    return;
  }  

  if(!thisp->pointFilterCB && thisp->primcbdata.fulltest &&
      !thisp->primcbdata.allhit) {
    thisp->primcbdata.abort = TRUE;
    return;
  }
  if(!thisp->pointFilterCB && !thisp->primcbdata.fulltest &&
      thisp->primcbdata.hit) {
    thisp->primcbdata.abort = TRUE;
    return;
  }
  
 
  SbVec2s p = project_pt(thisp->primcbdata.projmatrix, v->getPoint(),
                         thisp->primcbdata.vporg, thisp->primcbdata.vpsize);
  
  if(thisp->master->lassoType.getValue() == SoExtSelection::RECTANGLE){ // Rectangle check only  
    
    SbBool onlyrect = thisp->primcbdata.onlyrect;
    if (!thisp->primcbdata.lassorect.intersect(p) || (onlyrect || !point_in_poly(thisp->coords, p))) {
      thisp->primcbdata.allhit = FALSE;
      return;
    }
    
  } else if(!point_in_poly(thisp->coords, p)) {
    thisp->primcbdata.allhit = FALSE;
    return;
  }
  
  
  if(!thisp->applyonlyonselectedtriangles){ // ---- First pass
    
    if(!thisp->primcbdata.allshapes){
      
      // Draw to offscreen
      if(thisp->drawcounter > thisp->maximumcolorcounter)
        thisp->offscreencolorcounteroverflow = TRUE;
            
      thisp->addPointToOffscreenBuffer(action, v, thisp->offscreencolorcounteroverflow);
      
    } else if (thisp->pointFilterCB && thisp->primcbdata.allshapes) {
      
      if (thisp->pointFilterCB(thisp->pointFilterCBData, action, v)) {
        // select shape
        thisp->primcbdata.hit = TRUE;
        thisp->primcbdata.allhit = TRUE;
        thisp->primcbdata.abort = TRUE;
      }
    }
    else thisp->primcbdata.hit = TRUE;
    
  } else { // ---- Second pass

    if(thisp->drawcounter > thisp->maximumcolorcounter){
      thisp->offscreencolorcounteroverflow = TRUE;
      return;
    }
    
    int flag = 0x1 << (thisp->offscreencolorcounter & 0x07);
    int index = thisp->offscreencolorcounter >> 3;
    
    if(thisp->visibletrianglesbitarray[index] & flag){
        
      if(thisp->pointFilterCB(thisp->pointFilterCBData, action, v)){
	thisp->primcbdata.hit = TRUE;
	thisp->primcbdata.allhit = TRUE;
      }
    }
    ++thisp->offscreencolorcounter;
    
  }
  
}

void
SoExtSelectionP::addPointToOffscreenBuffer(SoCallbackAction * action,
                                           const SoPrimitiveVertex * v1,
                                           SbBool renderAsBlack)
{

  if(primcbdata.allshapes)
    return;

  SoState * state = action->getState();
  SbMatrix proj, affine;
  const SbMatrix & mm = SoModelMatrixElement::get(state);
  offscreenviewvolume.getMatrices(affine, proj);
  affine.multLeft(mm);
  
  glMatrixMode(GL_PROJECTION);
  glLoadMatrixf((float *)proj);
  glMatrixMode(GL_MODELVIEW);
  glLoadMatrixf((float *)affine);

  glDepthFunc(GL_LEQUAL);
  glPointSize(SoPointSizeElement::get(action->getState()));


  glBegin(GL_POINTS);    

  if(!renderAsBlack){
    glColor3ub((unsigned char) (this->offscreencolorcounter>>(8+8)),
	       (unsigned char) (this->offscreencolorcounter>>(8)),
	       (unsigned char) (this->offscreencolorcounter));
    ++offscreencolorcounter;
  } else {
    glColor3f(0,0,0);
  }

  glVertex3fv(v1->getPoint().getValue());
  glEnd();

}


// invoke selection policy on a shape
void
SoExtSelectionP::doSelect(const SoPath * path)
{
  SoPath * newpath = (SoPath*) path;
  
  if (this->filterCB && (!this->callfiltercbonlyifselectable ||
                         path->findNode(PUBLIC(this)) >= 0)) {
    newpath = this->filterCB(this->filterCBData, path);
  }
  if (newpath == NULL) 
    return;
  if (newpath != path) 
    newpath->ref();
  PUBLIC(this)->invokeSelectionPolicy(newpath, TRUE);
  if (newpath != path) 
    newpath->unref();
}


// Call a doSelect for all paths in pathlist
void
SoExtSelectionP::selectPaths()
{
 
  int length = this->visitedshapepaths->getLength();
 
  for(int i=0;i<length;++i)
    doSelect((*(this->visitedshapepaths))[i]);
 
  this->visitedshapepaths->truncate(0);
}

void 
SoExtSelectionP::offscreenLassoTesselatorCallback(void * v0, void * v1, void * v2, void * userdata)
{

  SoExtSelectionP * pimpl = (SoExtSelectionP *) userdata;

  // Set flag indicating that a callback was executed.
  pimpl->lassostencilisdrawed = TRUE;

  SbVec3f *vec0 = (SbVec3f *) v0;
  SbVec3f *vec1 = (SbVec3f *) v1;
  SbVec3f *vec2 = (SbVec3f *) v2;

  glBegin(GL_TRIANGLES);
   glColor3f(1,1,1);
   glVertex2f(vec0->getValue()[0],vec0->getValue()[1]);
   glVertex2f(vec1->getValue()[0],vec1->getValue()[1]);
   glVertex2f(vec2->getValue()[0],vec2->getValue()[1]); 
  glEnd();

}

void 
SoExtSelectionP::offscreenRenderLassoCallback(void * userdata, SoAction * action)
{

  SoExtSelectionP * pimpl = (SoExtSelectionP *) userdata;

  
  // Setup optimal screen-aspect according to lasso-size
  SoHandleEventAction * eventAction = pimpl->offscreenaction;
  pimpl->offscreenviewvolume = SoViewVolumeElement::get(eventAction->getState());
  const SbViewportRegion & vp = SoViewportRegionElement::get(eventAction->getState());
  
  
  //
  // FIXME: This commented code is suppose to 'zoom' in the lasso-polygon like we 'zoom' in
  // on the selected tris in 'offscreenRenderCallback'. But alas, it doesn't work yet... (handegar)
  //
  /*
  SbBox2s rectbbox;
  for(int i = 0; i < pimpl->coords.getLength(); i++)
    rectbbox.extendBy(pimpl->coords[i]);
  
  SbVec2s org = vp.getViewportOriginPixels();
  SbVec2s siz = vp.getViewportSizePixels();
  float left = float(rectbbox.getMin()[0] - org[0]) / float(siz[0]);
  float bottom = float(rectbbox.getMin()[1] - org[1]) / float(siz[1]);
  
  float right = float(rectbbox.getMax()[0] - org[0]) / float(siz[0]);
  float top = float(rectbbox.getMax()[1] - org[1]) / float(siz[1]);
  
  // increment to avoid empty view volume
  if (left == right) right += 0.001f;
  if (top == bottom) top += 0.001f;
  
  // Reshape view volume
  pimpl->offscreenviewvolume = pimpl->offscreenviewvolume.narrow(left, bottom, right, top);
  */
  
  SbVec2s vpo = vp.getViewportOriginPixels();
  SbVec2s vps = vp.getViewportSizePixels();
  
  glMatrixMode(GL_PROJECTION);
  glOrtho(vpo[0], vpo[0]+vps[0]-1,
	  vpo[1], vpo[0]+vps[1]-1,
	  -1, 1);
  
  glPushAttrib(GL_LIGHTING_BIT|
	       GL_FOG_BIT|
	       GL_DEPTH_BUFFER_BIT|
	       GL_TEXTURE_BIT|
	       GL_LINE_BIT|
	       GL_CURRENT_BIT);

  glDisable(GL_LIGHTING);
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_FOG);
  glDisable(GL_BLEND);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);

  // This flag will be set to TRUE if the tesselatorcallbacks was executed.
  pimpl->lassostencilisdrawed = FALSE;

  // Render all tris to offscreen buffer via a tesselator.
  SbTesselator tesselator(pimpl->offscreenLassoTesselatorCallback,pimpl);
  tesselator.beginPolygon();

  int i;
  SbList <SbVec3f> tmplist;
  for(i = 0; i < pimpl->coords.getLength(); i++){
    tmplist.append(SbVec3f((float) pimpl->coords[i][0],(float) pimpl->coords[i][1],0));
  }
  const SbVec3f * tmparray = tmplist.getArrayPtr();
  for(i = 0; i < pimpl->coords.getLength(); i++)
    tesselator.addVertex(tmparray[i],(void*)&tmparray[i]);
  tesselator.endPolygon();
 
 
  glPopAttrib();
}

void 
SoExtSelectionP::offscreenRenderCallback(void * userdata, SoAction * action)
{
 
  SoExtSelectionP * pimpl = (SoExtSelectionP *) userdata;
  
  //
  // FIXME: This commented code is used to zoom in the selection. Dont work yet when combined
  // with lasso-stencil test, since the lasso-stencil is not 'zoomed'.
  //
  /*
  // --- Setup optimal screen-aspect according to lasso-size
  SoHandleEventAction * eventAction = pimpl->offscreenaction;
  pimpl->offscreenviewvolume = SoViewVolumeElement::get(eventAction->getState());
  const SbViewportRegion & vp = SoViewportRegionElement::get(eventAction->getState());
  SbBox2s rectbbox;

  for (int i = 0; i < pimpl->coords.getLength(); i++)
    rectbbox.extendBy(pimpl->coords[i]);
  
  SbVec2s org = vp.getViewportOriginPixels();
  SbVec2s siz = vp.getViewportSizePixels();
  float left = float(rectbbox.getMin()[0] - org[0]) / float(siz[0]);
  float bottom = float(rectbbox.getMin()[1] - org[1]) / float(siz[1]);

  float right = float(rectbbox.getMax()[0] - org[0]) / float(siz[0]);
  float top = float(rectbbox.getMax()[1] - org[1]) / float(siz[1]);
  
  // increment to avoid empty view volume
  if (left == right) right += 0.001f;
  if (top == bottom) top += 0.001f;

  // Reshape view volume
  pimpl->offscreenviewvolume = pimpl->offscreenviewvolume.narrow(left, bottom, right, top);
  */


  GLint depthFunc;
  glGetIntegerv(GL_DEPTH_FUNC,&depthFunc);

  glPushAttrib(GL_LIGHTING_BIT|
	       GL_FOG_BIT|
	       GL_DEPTH_BUFFER_BIT|
	       GL_TEXTURE_BIT|
	       GL_LINE_BIT|
	       GL_CURRENT_BIT);

  // Setup GL-state for offscreen context
  glDisable(GL_LIGHTING);
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_TEXTURE_3D);
  glDisable(GL_FOG);
  glDisable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);

  // --- Render all tris to offscreen buffer.
  pimpl->cbaction->apply(pimpl->offscreenheadnode);
  pimpl->master->touch();

  // Restore all OpenGL States
  glPopAttrib();

  glDepthFunc(depthFunc);

}

void
SoExtSelectionP::checkOffscreenRendererCapabilities()
{
  
  // Check color-channel bitresolution
  GLint red = 0;
  GLint green = 0;
  GLint blue = 0;
  GLint alpha = 0;
  GLboolean rgbmode;
  glGetIntegerv(GL_RED_BITS,&red);
  glGetIntegerv(GL_GREEN_BITS,&green);
  glGetIntegerv(GL_BLUE_BITS,&blue);
  glGetIntegerv(GL_ALPHA_BITS,&alpha);
  
  glGetBooleanv(GL_RGBA_MODE,&rgbmode);
  // FIXME: Here we have to give a better message to the client (handegar)
  assert(rgbmode && "Offscreen renderer in non-RGB mode is not supported when using SoExtSelection");
  
  this->colorbitsred = red;
  this->colorbitsgreen = green;
  this->colorbitsblue = blue;
  this->colorbitsalpha = alpha;

  // This is debug/testing stuff, forcing lower colorresolution. VERY useful!
#ifdef BITCOLOR9 
  this->colorbitsred = 3;
  this->colorbitsgreen = 3;
  this->colorbitsblue = 3;
#endif  
#ifdef BITCOLOR3
  this->colorbitsred = 1;
  this->colorbitsgreen = 1;
  this->colorbitsblue = 1;
#endif  
#ifdef BITCOLOR6
  this->colorbitsred = 2;
  this->colorbitsgreen = 2;
  this->colorbitsblue = 2;
#endif  


  // Calculate maximum colorcounter from RGB bit depth.
  this->maximumcolorcounter = ((int) pow(2,this->colorbitsred) - 1) << (this->colorbitsgreen + this->colorbitsblue);
  this->maximumcolorcounter += ((int) pow(2,this->colorbitsgreen) - 1) << (this->colorbitsblue);
  this->maximumcolorcounter += ((int) pow(2,this->colorbitsblue));
  
}

SbBool
SoExtSelectionP::scanOffscreenBuffer(SoNode *sceneRoot)
{
  
  const SbViewportRegion vpr = renderer->getViewportRegion();

  int offscreenSizeX = vpr.getViewportSizePixels()[0];
  int offscreenSizeY = vpr.getViewportSizePixels()[1];

  unsigned char * rgbBuffer = renderer->getBuffer();
  unsigned char * stencilBuffer = lassorenderer->getBuffer();
  int pixelValue = 0;
  int index = 0;
  int flag = 0;
  SbBool hitflag = FALSE;


  // Clear entire 'visibletrianglesbitarray' table
  for(int i=0;i<((this->maximumcolorcounter >> 3)+1);i++)
    this->visibletrianglesbitarray[i] = 0;
  

  for(int j=0; j < offscreenSizeY; ++j){
    for(int i=0; i < offscreenSizeX*3; i+=3){
      
      // If needed, we consult the stencil buffer before fetching pixelvalue
      if(this->lassostencilisdrawed){
	if(stencilBuffer[j*offscreenSizeX*3 + i] == 0)
	  continue;
      }
      
      pixelValue = (rgbBuffer[j*offscreenSizeX*3 + i]<< (8+8));
      pixelValue += (rgbBuffer[j*offscreenSizeX*3 + i + 1]<< (8));
      pixelValue += (rgbBuffer[j*offscreenSizeX*3 + i + 2]);
      
      
      if(pixelValue != 0){
        
	flag = 0x1 << (pixelValue & 0x07);
	index = pixelValue >> 3;
        
	visibletrianglesbitarray[index] |= flag;
  	hitflag = TRUE;
      }

    }
  }

  return(hitflag);
}


// start a selecting for the current lasso/rectangle
void
SoExtSelectionP::performSelection(SoHandleEventAction * action)
{

  if (PUBLIC(this)->lassoType.getValue() == SoExtSelection::RECTANGLE) {
    assert(this->coords.getLength() == 2);
    SbVec2s p0 = this->coords[0];
    SbVec2s p1 = this->coords[1];
    this->coords.truncate(0);

    // Is this actually a rectangle and not just a point or line?
    if(((p0[0] == p1[0]) && (p0[1] == p1[0])) ||
       ((p0[0] == p1[0]) || (p0[1] == p1[1])))
      return;

    // convert the rectangle to a polygon
    this->coords.append(p0);
    this->coords.append(SbVec2s(p1[0], p0[1]));
    this->coords.append(p1);
    this->coords.append(SbVec2s(p0[0], p1[1]));
  }

  this->curvp = SoViewportRegionElement::get(action->getState());
  this->cbaction->setViewportRegion(this->curvp);

  this->wasshiftdown = action->getEvent()->wasShiftDown();
  switch (PUBLIC(this)->policy.getValue()) {
  case SoSelection::SINGLE:
    PUBLIC(this)->deselectAll();
    break;
  case SoSelection::SHIFT:
    if (!this->wasshiftdown) PUBLIC(this)->deselectAll();
    break;
  default:
    break;
  }
  

  if(PUBLIC(this)->lassoMode.getValue() == SoExtSelection::ALL_SHAPES) {

    this->offscreencolorcounter = 1;
    this->offscreenskipcounter = 0;
    this->applyonlyonselectedtriangles = FALSE;
    this->offscreencolorcounteroverflow = FALSE;
    this->drawcallbackcounter = 0;
    this->drawcounter = 0;

    // Execute 'search' for triangles
    primcbdata.allshapes = TRUE;
    PUBLIC(this)->startCBList->invokeCallbacks(PUBLIC(this));
    this->cbaction->apply(action->getCurPath()->getHead());
    PUBLIC(this)->finishCBList->invokeCallbacks(PUBLIC(this));
    PUBLIC(this)->touch();

  } else {

    //
    // -- Search for visible triangles inside lasso
    //

    // Send signalt to client that tris are comming up
    PUBLIC(this)->startCBList->invokeCallbacks(PUBLIC(this));

    this->offscreenaction = action;
    this->offscreenheadnode = action->getCurPath()->getHead();
    
    // Check OpenGL capabilities
    checkOffscreenRendererCapabilities();
    this->visibletrianglesbitarray = new unsigned char[((this->maximumcolorcounter) >> 3)+1];
 
    // --- Do this procedure several times if colorcounter overflows
    this->offscreencolorcounterpasses = 0;
    this->renderer = new SoOffscreenRenderer(action->getViewportRegion());
    this->lassorenderer = new SoOffscreenRenderer(action->getViewportRegion());


    SoCallback * cbnode = new SoCallback;
    cbnode->ref();
    cbnode->setCallback(offscreenRenderCallback, this);

 
    do { // --- Render and processing loop.

      primcbdata.allshapes = FALSE;
 
      this->offscreencolorcounter = 1;
      this->offscreenskipcounter = 0;
      this->applyonlyonselectedtriangles = FALSE;
      this->offscreencolorcounteroverflow = FALSE;
      this->drawcallbackcounter = 0;
      this->drawcounter = 0;
       

      // Render a offscreen stencil buffer of lasso-shape if needed
      if(PUBLIC(this)->lassoType.getValue() != SoExtSelection::NOLASSO){
	// Only draw stencil buffer on the first pass
	if(this->offscreencolorcounterpasses == 0){
	  SoCallback * cbnode2 = new SoCallback;
	  cbnode2->ref();
	  cbnode2->setCallback(offscreenRenderLassoCallback, this);
	  this->lassorenderer->render(cbnode2);
	  cbnode2->unref();
	}
      }


      // First render pass to offscreen buffer.
      this->renderer->render(cbnode);


      // Used to save offscreen to disk for debugging purposes. (handegar)
      //this->renderer->writeToRGB("renderdump1.rgb");


      // Scan buffer marking visible colors in the 'visibletrianglesbitarray' array.
      if(scanOffscreenBuffer(action->getCurPath()->getHead()) != 0){
        
	// Render once more, but only selected triangles which are forwarded
	// to client code through 'triangleFilterCB'.

	this->offscreencolorcounteroverflow = FALSE;
	this->offscreencolorcounter = 1;
	this->offscreenskipcounter = 0;
        this->drawcallbackcounter = 0;
        this->drawcounter = 0;
 
	this->applyonlyonselectedtriangles = TRUE;
	this->cbaction->apply(action->getCurPath()->getHead());
	PUBLIC(this)->touch();

      } else {

	this->offscreencolorcounteroverflow = FALSE;
	this->offscreencolorcounter = 1;
	this->offscreenskipcounter = 0;
      }
      ++this->offscreencolorcounterpasses;

    } while(this->offscreencolorcounterpasses*this->maximumcolorcounter < this->drawcallbackcounter);

    // Send signal to client that we are finished searching for tris
    PUBLIC(this)->finishCBList->invokeCallbacks(PUBLIC(this));    
    delete [] this->visibletrianglesbitarray;
    delete this->renderer;
    delete this->lassorenderer;
    cbnode->unref();

    selectPaths(); // Execute a 'doSelect' on all stored paths.
   
  }

}

#endif // DOXYGEN_SKIP_THIS
