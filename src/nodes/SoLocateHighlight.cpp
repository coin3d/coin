/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoLocateHighlight SoLocateHighlight.h Inventor/nodes/SoLocateHighlight.h
  \brief The SoLocateHighlight class highlights geometry under the cursor.
  \ingroup nodes

  Note: this node is supposed to draw to the front buffer. However, in
  Coin we always draw to the back buffer, forcing a scene redraw
  whenever a highlight state changes.
*/


#include <Inventor/nodes/SoLocateHighlight.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/elements/SoOverrideElement.h>
#include <Inventor/elements/SoLazyElement.h>
#include <Inventor/SoFullPath.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoHandleEventAction.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/misc/SoChildList.h>
#include <Inventor/events/SoLocation2Event.h>
#include <Inventor/SoPickedPoint.h>

/*!
  \enum SoLocateHighlight::Modes
  Enum type for behaviour modes.
*/
/*!
  \var SoLocateHighlight::Modes SoLocateHighlight::AUTO
  Highlight when mouse cursor is over the contents of the node.
*/
/*!
  \var SoLocateHighlight::Modes SoLocateHighlight::ON
  Always highlight.
*/
/*!
  \var SoLocateHighlight::Modes SoLocateHighlight::OFF
  Never highlight.
*/
/*!
  \enum SoLocateHighlight::Styles
  Enum type for highlight styles.
*/
/*!
  \var SoLocateHighlight::Styles SoLocateHighlight::EMISSIVE
  Highlight using emissive color override.
*/
/*!
  \var SoLocateHighlight::Styles SoLocateHighlight::EMISSIVE_DIFFUSE
  Highlight useing emissive and diffuse color override.
*/

/*!
  \var SoSFColor SoLocateHighlight::color
  The color used for highlighting.
*/

/*!
  \var SoSFEnum SoLocateHighlight::style
  The highlight style.
*/
/*!
  \var SoSFEnum SoLocateHighlight::mode
  The highlight mode.
*/

SoFullPath * SoLocateHighlight::currenthighlight = NULL;

#ifndef DOXYGEN_SKIP_THIS

class SoLocateHighlightP {
public:
  SbBool highlighted;
  SoColorPacker colorpacker;
};

#endif // DOXYGEN_SKIP_THIS


#undef THIS
#define THIS this->pimpl

// *************************************************************************

SO_NODE_SOURCE(SoLocateHighlight);

/*!
  Constructor.
*/
SoLocateHighlight::SoLocateHighlight()
{
  THIS = new SoLocateHighlightP;
  SO_NODE_INTERNAL_CONSTRUCTOR(SoLocateHighlight);

  SO_NODE_ADD_FIELD(color, (SbColor(0.3f, 0.3f, 0.3f)));
  SO_NODE_ADD_FIELD(style, (EMISSIVE));
  SO_NODE_ADD_FIELD(mode, (AUTO));

  SO_NODE_DEFINE_ENUM_VALUE(Styles, EMISSIVE);
  SO_NODE_DEFINE_ENUM_VALUE(Styles, EMISSIVE_DIFFUSE);
  SO_NODE_SET_SF_ENUM_TYPE(style, Styles);

  SO_NODE_DEFINE_ENUM_VALUE(Modes, AUTO);
  SO_NODE_DEFINE_ENUM_VALUE(Modes, ON);
  SO_NODE_DEFINE_ENUM_VALUE(Modes, OFF);
  SO_NODE_SET_SF_ENUM_TYPE(mode, Modes);

  THIS->highlighted = FALSE;
}

/*!
  Destructor.
*/
SoLocateHighlight::~SoLocateHighlight()
{
  delete THIS;
}

// doc from parent
void
SoLocateHighlight::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoLocateHighlight, SO_FROM_INVENTOR_1);
}

/*!
  Static method that can be used to turn off the current highlight.
*/
void
SoLocateHighlight::turnOffCurrentHighlight(SoGLRenderAction * action)
{
  SoLocateHighlight::turnoffcurrent(action);
}

// doc from parent
void
SoLocateHighlight::handleEvent(SoHandleEventAction * action)
{
  Modes mymode = (Modes) this->mode.getValue();
  if (mymode == AUTO) {
    const SoEvent * event = action->getEvent();
    if (event->isOfType(SoLocation2Event::getClassTypeId())) {
      const SoPickedPoint * pp = action->getPickedPoint();
      if (pp && pp->getPath()->containsPath(action->getCurPath())) {
        if (!THIS->highlighted) {
          SoLocateHighlight::turnoffcurrent(action);
          SoLocateHighlight::currenthighlight = (SoFullPath*)
            action->getCurPath()->copy();
          SoLocateHighlight::currenthighlight->ref();
          THIS->highlighted = TRUE;
          this->touch(); // force scene redraw
          this->redrawHighlighted(action, TRUE);
        }
      }
      else {
        if (THIS->highlighted) {
          SoLocateHighlight::turnoffcurrent(action);
        }
      }
    }
  }
  inherited::handleEvent(action);
}

// doc from parent
void
SoLocateHighlight::GLRenderBelowPath(SoGLRenderAction * action)
{
  SoState * state = action->getState();
  state->push();
  if (THIS->highlighted || this->mode.getValue() == ON) {
    this->setOverride(action);
  }
  inherited::GLRenderBelowPath(action);
  state->pop();
}

// doc from parent
void
SoLocateHighlight::GLRenderInPath(SoGLRenderAction * action)
{
  SoState * state = action->getState();
  state->push();
  if (THIS->highlighted || this->mode.getValue() == ON) {
    this->setOverride(action);
  }
  inherited::GLRenderInPath(action);
  state->pop();
}

/*!
  Empty method in Coin. Can be used by subclasses to be told
  when status change.
*/
void
SoLocateHighlight::redrawHighlighted(SoAction * /* act */, SbBool /* flag */)
{
}

//
// update override state before rendering
//
void
SoLocateHighlight::setOverride(SoGLRenderAction * action)
{
  SoState * state = action->getState();
  SoLazyElement::setEmissive(state, &this->color.getValue());
  SoOverrideElement::setEmissiveColorOverride(state, this, TRUE);

  Styles mystyle = (Styles) this->style.getValue();
  if (mystyle == SoLocateHighlight::EMISSIVE_DIFFUSE) {
    SoLazyElement::setDiffuse(state, this,
                               1, &this->color.getValue(),
                              &THIS->colorpacker);
    SoOverrideElement::setDiffuseColorOverride(state, this, TRUE);
  }
}

// private convenience method
void
SoLocateHighlight::turnoffcurrent(SoAction * action)
{
  if (SoLocateHighlight::currenthighlight &&
      SoLocateHighlight::currenthighlight->getLength()) {
    SoNode * tail = SoLocateHighlight::currenthighlight->getTail();
    if (tail->isOfType(SoLocateHighlight::getClassTypeId())) {
      ((SoLocateHighlight*)tail)->pimpl->highlighted = FALSE;
      ((SoLocateHighlight*)tail)->touch(); // force scene redraw
      if (action) ((SoLocateHighlight*)tail)->redrawHighlighted(action, FALSE);
    }
  }
  if (SoLocateHighlight::currenthighlight) {
    SoLocateHighlight::currenthighlight->unref();
    SoLocateHighlight::currenthighlight = NULL;
  }
}

#undef THIS
