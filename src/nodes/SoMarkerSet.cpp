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
  \class SoMarkerSet SoMarkerSet.h Inventor/nodes/SoMarkerSet.h
  \brief The SoMarkerSet class displays a set of 2D bitmap markers in 3D.
  \ingroup nodes

  This node uses the coordinates currently on the state (or in the
  vertexProperty field) in order. The numPoints field specifies the
  number of points in the set.

  In addition to supplying the user with a set of standard markers to
  choose from, it is also possible to specify one's own bitmaps for
  markers.

  This node class is an extension versus the original SGI Inventor
  v2.1 API.  In addition to being a Coin extension, it is also present
  in TGS' Inventor implementation (except TGS doesn't support the NONE 
  markerIndex value).
*/

// FIXME: clean up everything.... it's quite messy at the moment... :-/ skei
// FIXME: change standard markers to use GL_UNPACK_ALIGNMENT 1, instead of 4, as it is now... skei 200009005

#include <Inventor/nodes/SoMarkerSet.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/misc/SoState.h>

#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/elements/SoGLCoordinateElement.h>
#include <Inventor/elements/SoMaterialBindingElement.h>
#include <Inventor/bundles/SoMaterialBundle.h>
#include <Inventor/elements/SoGLLightModelElement.h>
#include <Inventor/elements/SoGLTextureEnabledElement.h>
#include <Inventor/elements/SoGLTexture3EnabledElement.h>
#include <Inventor/elements/SoGLLightModelElement.h>
#include <Inventor/elements/SoViewVolumeElement.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/elements/SoViewportRegionElement.h>
#include <../tidbits.h> // coin_atexit()
#include <math.h>
#include <string.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/system/gl.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#include <coindefs.h> // COIN_OBSOLETED

/*!
  \enum SoMarkerSet::MarkerType
  Defines the different standard markers.
*/
// FIXME: should have a png picture in the doc showing the various
// marker graphics. 20010815 mortene.

/*!
  \var SoMFInt32 SoMarkerSet::markerIndex
  Contains the set of index markers to display, defaults to 0 (CROSS_5_5).
  The special value NONE renders nothing for that marker.
*/

// *************************************************************************

SO_NODE_SOURCE(SoMarkerSet);

/*!
  Constructor.
*/
SoMarkerSet::SoMarkerSet()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoMarkerSet);
  SO_NODE_ADD_FIELD(markerIndex, (0));
}

/*!
  Destructor.
*/
SoMarkerSet::~SoMarkerSet()
{
}

// ----------------------------------------------------------------------

typedef struct {
  int width;
  int height;
  int align;
  unsigned char *data;
  bool deletedata;
} so_marker;

static SbList <so_marker> * markerlist;
static GLubyte * markerimages;
static void convert_bitmaps(void);
// -----------------------------------------------------------------------
static void
free_marker_images(void)
{
  delete[] markerimages;
  if (markerlist->getLength() > SoMarkerSet::NUM_MARKERS) {
    // markers have been added.. free marker->data
    for (int i = SoMarkerSet::NUM_MARKERS; i < markerlist->getLength(); i++) {
      so_marker * tmp = &(*markerlist)[i];
      if (tmp->deletedata) delete tmp->data;
    }
  }
  delete markerlist;
}

// doc in super
void
SoMarkerSet::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoMarkerSet);
  markerimages = new GLubyte[NUM_MARKERS*9*4]; // hardcoded markers, 32x9 bitmaps (9x9 used), dword alignment
  markerlist = new SbList<so_marker>;
  coin_atexit((coin_atexit_f *)free_marker_images);
  convert_bitmaps();
  so_marker temp;
  for (int i = 0; i < NUM_MARKERS; i++) {
    temp.width  = 9;
    temp.height = 9;
    temp.align  = 4;
    temp.data   = markerimages + (i * 36);
    temp.deletedata = FALSE;
    markerlist->append(temp);
  }
}

// Internal method which translates the current material binding found
// on the state to a material binding for this node.  PER_PART,
// PER_FACE, PER_VERTEX and their indexed counterparts are translated
// to PER_VERTEX binding. OVERALL means overall binding for point set
// also, of course. The default material binding is OVERALL.
SoMarkerSet::Binding
SoMarkerSet::findMaterialBinding(SoState * const state) const
{
  Binding binding = OVERALL;
  if (SoMaterialBindingElement::get(state) !=
      SoMaterialBindingElement::OVERALL) binding = PER_VERTEX;
  return binding;
}

static char marker_char_bitmaps[] =
{
  // CROSS_5_5
  "         "
  "         "
  "  x   x  "
  "   x x   "
  "    x    "
  "   x x   "
  "  x   x  "
  "         "
  "         "
  // PLUS_5_5
  "         "
  "         "
  "    x    "
  "    x    "
  "  xxxxx  "
  "    x    "
  "    x    "
  "         "
  "         "
  // MINUS_5_5
  "         "
  "         "
  "         "
  "         "
  "  xxxxx  "
  "         "
  "         "
  "         "
  "         "
  // SLASH_5_5
  "         "
  "         "
  "      x  "
  "     x   "
  "    x    "
  "   x     "
  "  x      "
  "         "
  "         "
  // BACKSLASH_5_5
  "         "
  "         "
  "  x      "
  "   x     "
  "    x    "
  "     x   "
  "      x  "
  "         "
  "         "
  // BAR_5_5
  "         "
  "         "
  "    x    "
  "    x    "
  "    x    "
  "    x    "
  "    x    "
  "         "
  "         "
  // STAR_5_5
  "         "
  "         "
  "  x x x  "
  "   xxx   "
  "  xxxxx  "
  "   xxx   "
  "  x x x  "
  "         "
  "         "
  // Y_5_5
  "         "
  "         "
  "  x   x  "
  "   x x   "
  "    x    "
  "    x    "
  "    x    "
  "         "
  "         "
  // LIGHTNING_5_5
  "         "
  "         "
  "    x    "
  "     x   "
  "  xxxxx  "
  "   x     "
  "    x    "
  "         "
  "         "
  // WELL_5_5
  "         "
  "         "
  "    x    "
  "    x    "
  "   x x   "
  "   xxx   "
  "  x   x  "
  "         "
  "         "
  // CIRCLE_LINE_5_5
  "         "
  "         "
  "   xxx   "
  "  x   x  "
  "  x   x  "
  "  x   x  "
  "   xxx   "
  "         "
  "         "
  // SQUARE_LINE_5_5
  "         "
  "         "
  "  xxxxx  "
  "  x   x  "
  "  x   x  "
  "  x   x  "
  "  xxxxx  "
  "         "
  "         "
  // DIAMOND_LINE_5_5
  "         "
  "         "
  "    x    "
  "   x x   "
  "  x   x  "
  "   x x   "
  "    x    "
  "         "
  "         "
  // TRIANGLE_LINE_5_5
  "         "
  "         "
  "    x    "
  "    x    "
  "   x x   "
  "   x x   "
  "  xxxxx  "
  "         "
  "         "
  // RHOMBUS_LINE_5_5
  "         "
  "         "
  "         "
  "   xxxxx "
  "  x   x  "
  " xxxxx   "
  "         "
  "         "
  "         "
  // HOURGLASS_LINE_5_5
  "         "
  "         "
  "  xxxxx  "
  "   x x   "
  "    x    "
  "   x x   "
  "  xxxxx  "
  "         "
  "         "
  // SATELLITE_LINE_5_5
  "         "
  "         "
  "  x   x  "
  "   xxx   "
  "   x x   "
  "   xxx   "
  "  x   x  "
  "         "
  "         "
  // PINE_TREE_LINE_5_5
  "         "
  "         "
  "    x    "
  "   x x   "
  "  xxxxx  "
  "    x    "
  "    x    "
  "         "
  "         "
  // CAUTION_LINE_5_5
  "         "
  "         "
  "  xxxxx  "
  "   x x   "
  "   x x   "
  "    x    "
  "    x    "
  "         "
  "         "
  // SHIP_LINE_5_5
  "         "
  "         "
  "    x    "
  "    x    "
  "  xxxxx  "
  "   x x   "
  "    x    "
  "         "
  "         "
  // CIRCLE_FILLED_5_5
  "         "
  "         "
  "   xxx   "
  "  xxxxx  "
  "  xxxxx  "
  "  xxxxx  "
  "   xxx   "
  "         "
  "         "
  // SQUARE_FILLED_5_5
  "         "
  "         "
  "  xxxxx  "
  "  xxxxx  "
  "  xxxxx  "
  "  xxxxx  "
  "  xxxxx  "
  "         "
  "         "
  // DIAMOND_FILLED_5_5
  "         "
  "         "
  "    x    "
  "   xxx   "
  "  xxxxx  "
  "   xxx   "
  "    x    "
  "         "
  "         "
  // TRIANGLE_FILLED_5_5
  "         "
  "         "
  "    x    "
  "    x    "
  "   xxx   "
  "   xxx   "
  "  xxxxx  "
  "         "
  "         "
  // RHOMBUS_FILLED_5_5
  "         "
  "         "
  "         "
  "   xxxxx "
  "  xxxxx  "
  " xxxxx   "
  "         "
  "         "
  "         "
  // HOURGLASS_FILLED_5_5
  "         "
  "         "
  "  xxxxx  "
  "   xxx   "
  "    x    "
  "   xxx   "
  "  xxxxx  "
  "         "
  "         "
  // SATELLITE_FILLED_5_5
  "         "
  "         "
  "  x   x  "
  "   xxx   "
  "   xxx   "
  "   xxx   "
  "  x   x  "
  "         "
  "         "
  // PINE_TREE_FILLED_5_5
  "         "
  "         "
  "    x    "
  "   xxx   "
  "  xxxxx  "
  "    x    "
  "    x    "
  "         "
  "         "
  // CAUTION_FILLED_5_5
  "         "
  "         "
  "  xxxxx  "
  "   xxx   "
  "   xxx   "
  "    x    "
  "    x    "
  "         "
  "         "
  // SHIP_FILLED_5_5
  "         "
  "         "
  "    x    "
  "    x    "
  "  xxxxx  "
  "   xxx   "
  "    x    "
  "         "
  "         "
  // CROSS_7_7
  "         "
  " x     x "
  "  x   x  "
  "   x x   "
  "    x    "
  "   x x   "
  "  x   x  "
  " x     x "
  "         "
  // PLUS_7_7
  "         "
  "    x    "
  "    x    "
  "    x    "
  " xxxxxxx "
  "    x    "
  "    x    "
  "    x    "
  "         "
  // MINUS_7_7
  "         "
  "         "
  "         "
  "         "
  " xxxxxxx "
  "         "
  "         "
  "         "
  "         "
  // SLASH_7_7
  "         "
  "       x "
  "      x  "
  "     x   "
  "    x    "
  "   x     "
  "  x      "
  " x       "
  "         "
  // BACKSLASH_7_7
  "         "
  " x       "
  "  x      "
  "   x     "
  "    x    "
  "     x   "
  "      x  "
  "       x "
  "         "
  // BAR_7_7,
  "         "
  "    x    "
  "    x    "
  "    x    "
  "    x    "
  "    x    "
  "    x    "
  "    x    "
  "         "
  // STAR_7_7
  "         "
  "    x    "
  "  x x x  "
  "   xxx   "
  " xxxxxxx "
  "   xxx   "
  "  x x x  "
  "    x    "
  "         "
  // Y_7_7
  "         "
  " x     x "
  "  x   x  "
  "   x x   "
  "    x    "
  "    x    "
  "    x    "
  "    x    "
  "         "
  // LIGHTNING_7_7
  "         "
  "    x    "
  "     x   "
  "      x  "
  " xxxxxxx "
  "  x      "
  "   x     "
  "    x    "
  "         "
  // WELL_7_7
  "         "
  "    x    "
  "   x x   "
  "   xxx   "
  "  xx xx  "
  "  x x x  "
  "  xx xx  "
  " xx   xx "
  "         "
  // CIRCLE_LINE_7_7
  "         "
  "  xxxxx  "
  " x     x "
  " x     x "
  " x     x "
  " x     x "
  " x     x "
  "  xxxxx  "
  "         "
  // SQUARE_LINE_7_7
  "         "
  " xxxxxxx "
  " x     x "
  " x     x "
  " x     x "
  " x     x "
  " x     x "
  " xxxxxxx "
  "         "
  // DIAMOND_LINE_7_7
  "         "
  "    x    "
  "   x x   "
  "  x   x  "
  " x     x "
  "  x   x  "
  "   x x   "
  "    x    "
  "         "
  // TRIANGLE_LINE_7_7
  "         "
  "    x    "
  "    x    "
  "   x x   "
  "   x x   "
  "  x   x  "
  "  x   x  "
  " xxxxxxx "
  "         "
  // RHOMBUS_LINE_7_7
  "         "
  "         "
  "    xxxxx"
  "   x   x "
  "  x   x  "
  " x   x   "
  "xxxx     "
  "         "
  "         "
  // HOURGLASS_LINE_7_7
  "         "
  " xxxxxxx "
  "  x   x  "
  "   x x   "
  "    x    "
  "   x x   "
  "  x   x  "
  " xxxxxxx "
  "         "
  // SATELLITE_LINE_7_7
  "         "
  " x     x "
  "  x x x  "
  "   x x   "
  "  x   x  "
  "   x x   "
  "  x x x  "
  " x     x "
  "         "
  // PINE_TREE_LINE_7_7
  "         "
  "    x    "
  "   x x   "
  "  x   x  "
  " xxxxxxx "
  "    x    "
  "    x    "
  "    x    "
  "         "
  // CAUTION_LINE_7_7
  "         "
  " xxxxxxx "
  "  x   x  "
  "  x   x  "
  "   x x   "
  "   x x   "
  "    x    "
  "    x    "
  "         "
  // SHIP_LINE_7_7
  "         "
  "    x    "
  "    x    "
  "    x    "
  " xxxxxxx "
  "  x   x  "
  "   x x   "
  "    x    "
  "         "
  // CIRCLE_FILLED_7_7
  "         "
  "   xxx   "
  "  xxxxx  "
  " xxxxxxx "
  " xxxxxxx "
  " xxxxxxx "
  "  xxxxx  "
  "   xxx   "
  "         "
  // SQUARE_FILLED_7_7
  "         "
  " xxxxxxx "
  " xxxxxxx "
  " xxxxxxx "
  " xxxxxxx "
  " xxxxxxx "
  " xxxxxxx "
  " xxxxxxx "
  "         "
  // DIAMOND_FILLED_7_7
  "         "
  "    x    "
  "   xxx   "
  "  xxxxx  "
  " xxxxxxx "
  "  xxxxx  "
  "   xxx   "
  "    x    "
  "         "
  // TRIANGLE_FILLED_7_7
  "         "
  "    x    "
  "    x    "
  "   xxx   "
  "   xxx   "
  "  xxxxx  "
  "  xxxxx  "
  " xxxxxxx "
  "         "
  // RHOMBUS_FILLED_7_7
  "         "
  "         "
  "    xxxxx"
  "   xxxxx "
  "  xxxxx  "
  " xxxxx   "
  "xxxxx    "
  "         "
  "         "
  // HOURGLASS_FILLED_7_7
  "         "
  " xxxxxxx "
  "  xxxxx  "
  "   xxx   "
  "    x    "
  "   xxx   "
  "  xxxxx  "
  " xxxxxxx "
  "         "
  // SATELLITE_FILLED_7_7
  "         "
  " x     x "
  "  x x x  "
  "   xxx   "
  "  xxxxx  "
  "   xxx   "
  "  x x x  "
  " x     x "
  "         "
  // PINE_TREE_FILLED_7_7
  "         "
  "    x    "
  "   xxx   "
  "  xxxxx  "
  " xxxxxxx "
  "    x    "
  "    x    "
  "    x    "
  "         "
  // CAUTION_FILLED_7_7
  "         "
  " xxxxxxx "
  "  xxxxx  "
  "  xxxxx  "
  "   xxx   "
  "   xxx   "
  "    x    "
  "    x    "
  "         "
  // SHIP_FILLED_7_7
  "         "
  "    x    "
  "    x    "
  "    x    "
  " xxxxxxx "
  "  xxxxx  "
  "   xxx   "
  "    x    "
  "         "
  // CROSS_9_9
  "x       x"
  " x     x "
  "  x   x  "
  "   x x   "
  "    x    "
  "   x x   "
  "  x   x  "
  " x     x "
  "x       x"
  // PLUS_9_9
  "    x    "
  "    x    "
  "    x    "
  "    x    "
  "xxxxxxxxx"
  "    x    "
  "    x    "
  "    x    "
  "    x    "
  // MINUS_9_9
  "         "
  "         "
  "         "
  "         "
  "xxxxxxxxx"
  "         "
  "         "
  "         "
  "         "
  // SLASH_9_9
  "        x"
  "       x "
  "      x  "
  "     x   "
  "    x    "
  "   x     "
  "  x      "
  " x       "
  "x        "
  // BACKSLASH_9_9
  "x        "
  " x       "
  "  x      "
  "   x     "
  "    x    "
  "     x   "
  "      x  "
  "       x "
  "        x"
  // BAR_9_9,
  "    x    "
  "    x    "
  "    x    "
  "    x    "
  "    x    "
  "    x    "
  "    x    "
  "    x    "
  "    x    "
  // STAR_9_9
  "    x    "
  " x  x  x "
  "  x x x  "
  "   xxx   "
  "xxxxxxxxx"
  "   xxx   "
  "  x x x  "
  " x  x  x "
  "    x    "
  // Y_9_9
  "x       x"
  " x     x "
  "  x   x  "
  "   x x   "
  "    x    "
  "    x    "
  "    x    "
  "    x    "
  "    x    "
  // LIGHTNING_9_9
  "    x    "
  "     x   "
  "      x  "
  "       x "
  "xxxxxxxxx"
  " x       "
  "  x      "
  "   x     "
  "    x    "
  // WELL_9_9
  "    x    "
  "   xxx   "
  "   x x   "
  "   x x   "
  "  x x x  "
  "  xxxxx  "
  "  x x x  "
  " x x x x "
  "xxx   xxx"
  // CIRCLE_LINE_9_9
  "   xxx   "
  " xx   xx "
  " x     x "
  "x       x"
  "x       x"
  "x       x"
  " x     x "
  " xx   xx "
  "   xxx   "
  // SQUARE_LINE_9_9
  "xxxxxxxxx"
  "x       x"
  "x       x"
  "x       x"
  "x       x"
  "x       x"
  "x       x"
  "x       x"
  "xxxxxxxxx"
  // DIAMOND_LINE_9_9
  "    x    "
  "   x x   "
  "  x   x  "
  " x     x "
  "x       x"
  " x     x "
  "  x   x  "
  "   x x   "
  "    x    "
  // TRIANGLE_LINE_9_9
  "    x    "
  "    x    "
  "   x x   "
  "   x x   "
  "  x   x  "
  "  x   x  "
  " x     x "
  " x     x "
  "xxxxxxxxx"
  // RHOMBUS_LINE_9_9
  "         "
  "     xxxx"
  "    x   x"
  "   x   x "
  "  x   x  "
  " x   x   "
  "x   x    "
  "xxxx     "
  "         "
  // HOURGLASS_LINE_9_9
  "xxxxxxxxx"
  " x     x "
  "  x   x  "
  "   x x   "
  "    x    "
  "   x x   "
  "  x   x  "
  " x     x "
  "xxxxxxxxx"
  // SATELLITE_LINE_9_9
  "x       x"
  " x xxx x "
  "  x   x  "
  " x     x "
  " x     x "
  " x     x "
  "  x   x  "
  " x xxx x "
  "x       x"
  // PINE_TREE_LINE_9_9
  "    x    "
  "   x x   "
  "  x   x  "
  " x     x "
  "xxxxxxxxx"
  "    x    "
  "    x    "
  "    x    "
  "    x    "
  // CAUTION_LINE_9_9
  "xxxxxxxxx"
  " x     x "
  " x     x "
  "  x   x  "
  "  x   x  "
  "   x x   "
  "   x x   "
  "    x    "
  "    x    "
  // SHIP_LINE_9_9
  "    x    "
  "    x    "
  "    x    "
  "    x    "
  "xxxxxxxxx"
  " x     x "
  "  x   x  "
  "   x x   "
  "    x    "
  // CIRCLE_FILLED_9_9
  "   xxx   "
  " xxxxxxx "
  " xxxxxxx "
  "xxxxxxxxx"
  "xxxxxxxxx"
  "xxxxxxxxx"
  " xxxxxxx "
  " xxxxxxx "
  "   xxx   "
  // SQUARE_FILLED_9_9
  "xxxxxxxxx"
  "xxxxxxxxx"
  "xxxxxxxxx"
  "xxxxxxxxx"
  "xxxxxxxxx"
  "xxxxxxxxx"
  "xxxxxxxxx"
  "xxxxxxxxx"
  "xxxxxxxxx"
  // DIAMOND_FILLED_9_9
  "    x    "
  "   xxx   "
  "  xxxxx  "
  " xxxxxxx "
  "xxxxxxxxx"
  " xxxxxxx "
  "  xxxxx  "
  "   xxx   "
  "    x    "
  // TRIANGLE_FILLED_9_9
  "    x    "
  "    x    "
  "   xxx   "
  "   xxx   "
  "  xxxxx  "
  "  xxxxx  "
  " xxxxxxx "
  " xxxxxxx "
  "xxxxxxxxx"
  // RHOMBUS_FILLED_9_9
  "         "
  "     xxxx"
  "    xxxxx"
  "   xxxxx "
  "  xxxxx  "
  " xxxxx   "
  "xxxxx    "
  "xxxx     "
  "         "
  // HOURGLASS_FILLED_9_9
  "xxxxxxxxx"
  " xxxxxxx "
  "  xxxxx  "
  "   xxx   "
  "    x    "
  "   xxx   "
  "  xxxxx  "
  " xxxxxxx "
  "xxxxxxxxx"
  // SATELLITE_FILLED_9_9
  "x       x"
  " x xxx x "
  "  xxxxx  "
  " xxxxxxx "
  " xxxxxxx "
  " xxxxxxx "
  "  xxxxx  "
  " x xxx x "
  "x       x"
  // PINE_TREE_FILLED_9_9
  "    x    "
  "   xxx   "
  "  xxxxx  "
  " xxxxxxx "
  "xxxxxxxxx"
  "    x    "
  "    x    "
  "    x    "
  "    x    "
  // CAUTION_FILLED_9_9
  "xxxxxxxxx"
  " xxxxxxx "
  " xxxxxxx "
  "  xxxxx  "
  "  xxxxx  "
  "   xxx   "
  "   xxx   "
  "    x    "
  "    x    "
  // SHIP_FILLED_9_9
  "    x    "
  "    x    "
  "    x    "
  "    x    "
  "xxxxxxxxx"
  " xxxxxxx "
  "  xxxxx  "
  "   xxx   "
  "    x    "
  //"#"
};

static void
convert_bitmaps(void)
{
  int rpos = 0;
  int wpos = 0;
  for (int img = 0; img < SoMarkerSet::NUM_MARKERS; img++) {
    for (int l=8;l>=0;l--) {
      unsigned char v1 = 0;
      unsigned char v2 = 0;
      if (marker_char_bitmaps[(l*9)+rpos  ] == 'x') v1 += 0x80;
      if (marker_char_bitmaps[(l*9)+rpos+1] == 'x') v1 += 0x40;
      if (marker_char_bitmaps[(l*9)+rpos+2] == 'x') v1 += 0x20;
      if (marker_char_bitmaps[(l*9)+rpos+3] == 'x') v1 += 0x10;
      if (marker_char_bitmaps[(l*9)+rpos+4] == 'x') v1 += 0x08;
      if (marker_char_bitmaps[(l*9)+rpos+5] == 'x') v1 += 0x04;
      if (marker_char_bitmaps[(l*9)+rpos+6] == 'x') v1 += 0x02;
      if (marker_char_bitmaps[(l*9)+rpos+7] == 'x') v1 += 0x01;
      if (marker_char_bitmaps[(l*9)+rpos+8] == 'x') v2 += 0x80;
      markerimages[wpos  ] = v1;
      markerimages[wpos+1] = v2;
      markerimages[wpos+2] = 0;
      markerimages[wpos+3] = 0;
      wpos += 4;
    }
    rpos += (9*9);
  }
}

// doc in super
void
SoMarkerSet::GLRender(SoGLRenderAction * action)
{
  // FIXME: the marker bitmaps are toggled off when the leftmost pixel
  // is outside the left border, and ditto for the bottommost pixel
  // versus the bottom border. They should be drawn partly until they
  // are wholly outside the canvas instead. 20011218 mortene.

  SoState * state = action->getState();

  state->push();
  // We just disable lighting and texturing for markers, since we
  // can't see any reason this should ever be enabled.  send an angry
  // email to <pederb@coin3d.org> if you disagree.

  SoLightModelElement::set(state, this, SoLightModelElement::BASE_COLOR);
  SoGLTextureEnabledElement::set(state, this, FALSE);
  SoGLTexture3EnabledElement::set(state, this, FALSE);

  if (this->vertexProperty.getValue()) {
    this->vertexProperty.getValue()->GLRender(action);
  }

  const SoCoordinateElement * tmpcoord;
  const SbVec3f * dummy;
  SbBool needNormals = FALSE;

  SoVertexShape::getVertexData(state, tmpcoord, dummy, needNormals);

  if (!this->shouldGLRender(action)) {
    state->pop();
    return;
  }

  const SoGLCoordinateElement * coords = (SoGLCoordinateElement *)tmpcoord;

  Binding mbind = this->findMaterialBinding(action->getState());

  SoMaterialBundle mb(action);
  mb.sendFirst();

  int32_t idx = this->startIndex.getValue();
  int32_t numpts = this->numPoints.getValue();
  if (numpts < 0) numpts = coords->getNum() - idx;

  int matnr = 0;

  const SbMatrix & mat = SoModelMatrixElement::get(state);
  const SbViewVolume & vv = SoViewVolumeElement::get(state);
  const SbViewportRegion & vp = SoViewportRegionElement::get(state);
  SbVec2s vpsize = vp.getViewportSizePixels();

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(0, vpsize[0], 0, vpsize[1], -1.0f, 1.0f);

  for (int i = 0; i < numpts; i++) {
    int midx = SbMin(i, this->markerIndex.getNum() - 1);
#if COIN_DEBUG
      if (midx < 0 || (this->markerIndex[midx] >= markerlist->getLength())) {
        static SbBool firsterror = TRUE;
        if (firsterror) {
          SoDebugError::postWarning("SoMarkerSet::GLRender",
                                    "markerIndex %d out of bound",
                                    markerIndex[i]);
          firsterror = FALSE;
        }
        // Don't render, jump back to top of for-loop and continue with
        // next index.
        continue;
      }
#endif // COIN_DEBUG
      
    if (mbind == PER_VERTEX) mb.send(matnr++, TRUE);

    if (this->markerIndex[midx]!=NONE) {
      SbVec3f point = coords->get3(idx);
      mat.multVecMatrix(point, point);  // wcs
      vv.projectToScreen(point, point); // normalized screen coordinates
      point[0] = point[0] * float(vpsize[0]); // screen pixel position
      point[1] = point[1] * float(vpsize[1]);
      // change z range from [0,1] to [-1,1]
      point[2] *= 2.0f;
      point[2] -= 1.0f;
      
      so_marker * tmp = &(*markerlist)[ this->markerIndex[midx] ];
      
      // To have the exact center point of the marker drawn at the
      // projected 3D position.  (FIXME: I haven't actually checked that
      // this is what TGS' implementation of the SoMarkerSet node does
      // when rendering, but it seems likely. 20010823 mortene.)
      point[0] = point[0] - (tmp->width - 1) / 2;
      point[1] = point[1] - (tmp->height - 1) / 2;
      
      glPixelStorei(GL_UNPACK_ALIGNMENT, tmp->align);
      glRasterPos3f(point[0], point[1], -point[2]);
      glBitmap(tmp->width, tmp->height, 0, 0, 0, 0, tmp->data);
    }
    idx++;
  }
  // FIXME: this looks wrong, shouldn't we rather reset the alignment
  // value to what it was previously?  20010824 mortene.
  glPixelStorei(GL_UNPACK_ALIGNMENT, 4); // restore default value
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();

  state->pop(); // we pushed, remember
}

// ----------------------------------------------------------------------------------------------------

// Documented in superclass.
void
SoMarkerSet::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  // Overridden to add the number of markers to the number of images
  // in \a action.

  if (!this->shouldPrimitiveCount(action)) return;

  SoState * state = action->getState();

  state->push(); // in case we have a vertexProperty node

  if (this->vertexProperty.getValue()) {
    this->vertexProperty.getValue()->getPrimitiveCount(action);
  }

  const SoCoordinateElement * coord;
  const SbVec3f * dummy;
  SbBool needNormals = FALSE;

  SoVertexShape::getVertexData(state, coord, dummy,
                               needNormals);

  int32_t idx = this->startIndex.getValue();
  int32_t numpts = this->numPoints.getValue();
  if (numpts < 0) numpts = coord->getNum() - idx;

  action->addNumImage(numpts);

  state->pop();
}

/*!
  Returns the number of defined markers.
 */
int
SoMarkerSet::getNumDefinedMarkers(void)
{
  return markerlist->getLength();
}

static void
swap_leftright(unsigned char *data, int width, int height)
{
  // FIXME: sloppy code... 20000906. skei
  unsigned char t;

  int y;
  int linewidth = (width + 7) / 8;
  for (y=0; y<height; y++) {
    for (int x=0; x < (int) floor((double)(linewidth/2)); x++) {
      int tmp = data[y*linewidth+x];
      data[ y*linewidth + x ] = data[ (y*linewidth) + (linewidth-x-1) ];
      data[ (y*linewidth) + (linewidth-x-1) ] = tmp;
    }
  }
  for (y=0; y<height; y++) {
    for (int x=0; x<linewidth; x++) {
      t = 0;
      if ((data[y*linewidth+x] & 128) != 0) t += 1;
      if ((data[y*linewidth+x] &  64) != 0) t += 2;
      if ((data[y*linewidth+x] &  32) != 0) t += 4;
      if ((data[y*linewidth+x] &  16) != 0) t += 8;
      if ((data[y*linewidth+x] &   8) != 0) t += 16;
      if ((data[y*linewidth+x] &   4) != 0) t += 32;
      if ((data[y*linewidth+x] &   2) != 0) t += 64;
      if ((data[y*linewidth+x] &   1) != 0) t += 128;
      data[y*linewidth+x] = t;
    }
  }
}

static void
swap_updown(unsigned char *data, int width, int height)
{
  int linewidth = (width + 7) / 8;
  for (int y = 0; y < (height>>1); y++) {
    for (int x = 0; x < linewidth; x++) {
      int tmp = data[y*linewidth+x];
      data[y*linewidth + x] = data[((height-y-1)*linewidth) + x];
      data[((height-y-1)*linewidth) + x] = tmp;
    }
  }
}

/*!
  Replace the bitmap for the marker at \a markerIndex with the
  representation given by \a size dimensions with the bitmap data at
  \a bytes. \a isLSBFirst and \a isUpToDown indicates how the bitmap
  data is ordered. Does nothing if \a markerIndex is NONE.

  Here's a complete usage example which demonstrates how to set up a
  user-specified marker from a char-map:

  \code
  const int WIDTH = 18;
  const int HEIGHT = 19;
  const int BYTEWIDTH = (WIDTH + 7) / 2;

  const char coin_marker[WIDTH * HEIGHT + 1] = {
    ".+                "
    "+@.+              "
    " .@#.+            "
    " +$@##.+          "
    "  .%@&##.+        "
    "  +$@&&*##.+      "
    "   .%@&&*=##.+    "
    "   +$@&&&&=-##.+  "
    "    .%@&&&&&-;#&+ "
    "    +$@&&&&&&=#.  "
    "     .%@&&&&*#.   "
    "     +$@&&&&#.    "
    "      .%@&@%@#.   "
    "      +$%@%.$@#.  "
    "       .%%. .$@#. "
    "       +$.   .$>#."
    "        +     .$. "
    "               .  "
    "                  " };

  int byteidx = 0;
  unsigned char bitmapbytes[BYTEWIDTH * HEIGHT];
  for (int h = 0; h < HEIGHT; h++) {
    unsigned char bits = 0;
    for (int w = 0; w < WIDTH; w++) {
      if (coin_marker[(h * WIDTH) + w] != ' ') { bits |= (0x80 >> (w % 8)); }
      if ((((w + 1) % 8) == 0) || (w == WIDTH - 1)) {
        bitmapbytes[byteidx++] = bits;
        bits = 0;
      }
    }
  }

  int MYAPP_ARROW_IDX = SoMarkerSet::getNumDefinedMarkers(); // add at end
  SoMarkerSet::addMarker(MYAPP_ARROW_IDX, SbVec2s(WIDTH, HEIGHT),
                         bitmapbytes, FALSE, TRUE);
  \endcode

  This will provide you with an index given by MYAPP_ARROW_IDX which
  can be used in SoMarkerSet::markerIndex to display the new marker.
*/
void
SoMarkerSet::addMarker(int markerIndex, const SbVec2s & size,
                       const unsigned char * bytes, SbBool isLSBFirst,
                       SbBool isUpToDown)
{
  if (markerIndex == NONE) return;

  SbBool appendnew = markerIndex >= markerlist->getLength() ? TRUE : FALSE;
  so_marker tempmarker;
  so_marker * temp = &tempmarker;
  if (appendnew) {
    tempmarker.width  = 0;
    tempmarker.height = 0;
    tempmarker.align  = 0;
    tempmarker.data   = 0;
    tempmarker.deletedata = FALSE;
    while (markerIndex > markerlist->getLength()) markerlist->append(tempmarker);
  }
  else temp = &(*markerlist)[markerIndex];
  temp->width = size[0];
  temp->height = size[1];
  temp->align = 1;

  int datasize = ((size[0] + 7) / 8) * size[1];
  if (temp->deletedata) delete temp->data;
  temp->deletedata = TRUE;
  temp->data = new unsigned char[ datasize ];
  memcpy(temp->data,bytes,datasize);
  // FIXME: the swap_leftright() function seems
  // buggy. Investigate. 20011120 mortene.
  if (isLSBFirst) { swap_leftright(temp->data,size[0],size[1]); }
  if (isUpToDown) { swap_updown(temp->data,size[0],size[1]); }
  if (appendnew) markerlist->append(tempmarker);
}

/*!
  Returns data for marker at \a markerIndex in the \a size, \a bytes and
  \a isLSBFirst parameters.

  \retval FALSE No marker defined for given \a markerIndex, or markerIndex is 
  NONE (not removable).
  \retval TRUE  OK.
*/
SbBool
SoMarkerSet::getMarker(int markerIndex, SbVec2s & size,
                       const unsigned char *& bytes, SbBool & isLSBFirst)
{
  // FIXME: handle isLSBFirst. skei 20000905
  if (markerIndex == NONE ||
      markerIndex >= markerlist->getLength()) return FALSE;
  so_marker * temp = &(*markerlist)[markerIndex];
  size[0] = temp->width;
  size[1] = temp->height;
  bytes = temp->data;
  isLSBFirst = FALSE;
  return TRUE;
}

/*!
  Removes marker at \a markerIndex.

  \retval FALSE No marker defined for given \a markerIndex, or markerIndex is 
  NONE (not removable).
  \retval TRUE  OK.
*/
SbBool
SoMarkerSet::removeMarker(int markerIndex)
{
  if (markerIndex == NONE ||
      markerIndex >= markerlist->getLength()) return FALSE;
  so_marker * tmp = &(*markerlist)[markerIndex];
  if (tmp->deletedata) delete tmp->data;
  markerlist->remove(markerIndex);
  return TRUE;
}

/*!
  Not supported in Coin. Should probably not have been part of the
  public Open Inventor API.
*/
SbBool
SoMarkerSet::isMarkerBitSet(int markerIndex, int bitNumber)
{
  // FIXME: seems simple enough to support.. 20010815 mortene.
  COIN_OBSOLETED();
  return FALSE;
}
