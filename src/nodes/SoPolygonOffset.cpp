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
  \class SoPolygonOffset SoPolygonOffset.h Inventor/nodes/SoPolygonOffset.h
  \brief The SoPolygonOffset class is a node type for "layering" rendering primitives.
  \ingroup nodes

  A common problem with realtime 3D rendering systems is that rendered
  primitives which are at approximately the same depth with regard to
  the camera viewpoint will appear to flicker. I.e.: from one angle
  one primitive will appear to be closer, while at another angle,
  another primitive will appear closer. When this happens, the
  rendered graphics at that part of the scene will of course look a
  lot less visually pleasing.

  One common situation where this problem often occurs is when you
  attempt to put a wireframe grid as an outline on top of filled
  polygons.

  The cause of the problem described above is that the Z-buffer of any
  render system has a limited resolution, often at 16, 24 or 32
  bits. Because of this, primitives which are close will sometimes get
  the \e same depth value in the Z-buffer, even though they are \a not
  actually at the same depth-coordinate.

  To rectify the flickering problem, this node can be inserted in the
  scene graph at the proper place(s) to explicitly define how
  polygons, lines and/or points should be offset with regard to other
  primitives.

  \since TGS Inventor 2.5
  \since Coin 1.0
*/

#include <Inventor/nodes/SoPolygonOffset.h>
#include <Inventor/nodes/SoSubNodeP.h>

#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLPolygonOffsetElement.h>
#include <Inventor/elements/SoOverrideElement.h>

/*!
  \enum SoPolygonOffset::Style

  Enumeration of the rendering primitives which can be influenced by
  an SoPolygonOffset node.
*/


/*!
  \var SoSFFloat SoPolygonOffset::factor

  Offset multiplication factor.
*/
/*!
  \var SoSFFloat SoPolygonOffset::units

  Absolute offset translation value.
*/
/*!
  \var SoSFBitMask SoPolygonOffset::styles

  The rendering primitive type to be influenced by this node. Defaults
  to SoPolygonOffset::FILLED.
*/
/*!
  \var SoSFBool SoPolygonOffset::on

  Whether the offset is on or off. Default is for SoPolygonOffset::on
  to be \c TRUE.
*/


// *************************************************************************

SO_NODE_SOURCE(SoPolygonOffset);

/*!
  Constructor.
*/
SoPolygonOffset::SoPolygonOffset(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoPolygonOffset);

  SO_NODE_ADD_FIELD(factor, (1.0f));
  SO_NODE_ADD_FIELD(units, (1.0f));
  SO_NODE_ADD_FIELD(styles, (SoPolygonOffset::FILLED));
  SO_NODE_ADD_FIELD(on, (TRUE));

  SO_NODE_DEFINE_ENUM_VALUE(Style, FILLED);
  SO_NODE_DEFINE_ENUM_VALUE(Style, LINES);
  SO_NODE_DEFINE_ENUM_VALUE(Style, POINTS);
  SO_NODE_SET_SF_ENUM_TYPE(styles, Style);
}

/*!
  Destructor.
*/
SoPolygonOffset::~SoPolygonOffset()
{
}

// doc in super
void
SoPolygonOffset::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoPolygonOffset, SO_FROM_INVENTOR_2_5|SO_FROM_COIN_1_0);

  SO_ENABLE(SoCallbackAction, SoPolygonOffsetElement);
  SO_ENABLE(SoGLRenderAction, SoGLPolygonOffsetElement);
}


void
SoPolygonOffset::doAction(SoAction * action)
{
  SoState * state = action->getState();
  
  if (SoOverrideElement::getPolygonOffsetOverride(state)) return;
  
  float factorval, units_val;
  SoPolygonOffsetElement::Style styles_val;
  SbBool offset_on;
  
  factorval = this->factor.getValue();
  units_val = this->units.getValue();
  styles_val = (SoPolygonOffsetElement::Style)this->styles.getValue();
  offset_on = this->on.getValue();
  
  SoPolygonOffsetElement::set(action->getState(),
                              this,
                              factorval,
                              units_val,
                              styles_val,
                              offset_on);
  
  if (this->isOverride()) {
    SoOverrideElement::setPolygonOffsetOverride(state, this, TRUE);
  }
}

void
SoPolygonOffset::callback(SoCallbackAction * action)
{
  SoPolygonOffset::doAction((SoAction *)action);
}

void
SoPolygonOffset::GLRender(SoGLRenderAction * action)
{
  SoPolygonOffset::doAction((SoAction *)action);
}
