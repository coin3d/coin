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
  \class SoWWWAnchor SoWWWAnchor.h Inventor/nodes/SoWWWAnchor.h
  \brief The SoWWWAnchor class adds URL callbacks to the highlighted geometry.
  \ingroup nodes

  In addition to highlighting geometry under the cursor, the application
  programmer can set callbacks. It is possible to set one callback for
  picking, the fetch callback, and one callback for highlighting.

  \since Inventor 2.1
*/


#include <Inventor/nodes/SoWWWAnchor.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/events/SoMouseButtonEvent.h>
#include <Inventor/actions/SoHandleEventAction.h>
#include <Inventor/SoPickedPoint.h>
#include <coindefs.h>


/*!
  \enum SoWWWAnchor::Mapping
  Enum that says how a picked node's position should be mapped to the URL.
*/
/*!
  \var SoWWWAnchor::Mapping SoWWWAnchor::NONE
  The position of the picked node is not mapped to the URL.
*/
/*!
  \var SoWWWAnchor::Mapping SoWWWAnchor::POINT
  The position of the picked node is mapped to the URL as object space
  coordinates, adding a parameter string to the end of the URL. To 
  assure that the URL works with all browsers, the coordinates are
  divided by commas sent as the hex representation.

  If a model by the name of sim.wrl resided at www.coin3d.org and the
  picked point had the coordinates [1.5, 10, 6.77], the resulting URL
  would be "http://www.coin3d.org/sim.wrl?1.5%2c10%2c6.77".
*/


/*!
  \var SoSFString SoWWWAnchor::name
  The name of the URL which the anchor points to.
*/
/*!
  \var SoSFString SoWWWAnchor::description
  The description of the URL.
*/
/*!
  \var SoSFEnum SoWWWAnchor::map
  Enum describing how a node's position should be mapped to the URL.
*/

// static members
SoWWWAnchorCB * SoWWWAnchor::fetchfunc;
void * SoWWWAnchor::fetchdata;
SoWWWAnchorCB * SoWWWAnchor::highlightfunc;
void * SoWWWAnchor::highlightdata;

// *************************************************************************

#ifndef DOXYGEN_SKIP_THIS

class SoWWWAnchorP {
 public:
  SoWWWAnchorP(SoWWWAnchor * owner) {
    this->owner = owner;
    this->fullname = "";
    this->name = "";
  }
  SoWWWAnchor * owner;
  SbString fullname;
  SbString name;
};

#endif // DOXYGEN_SKIP_THIS

SO_NODE_SOURCE(SoWWWAnchor);

#undef THIS
#define THIS this->pimpl

/*!
  Constructor.
*/
SoWWWAnchor::SoWWWAnchor()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoWWWAnchor);

  THIS = new SoWWWAnchorP(this);

  SO_NODE_ADD_FIELD(name, ("<Undefined URL>"));
  SO_NODE_ADD_FIELD(description, (""));
  SO_NODE_ADD_FIELD(map, (NONE));

  SO_NODE_DEFINE_ENUM_VALUE(Map, NONE);
  SO_NODE_DEFINE_ENUM_VALUE(Map, POINT);
  SO_NODE_SET_SF_ENUM_TYPE(map, Map);
}

/*!
  Destructor.
*/
SoWWWAnchor::~SoWWWAnchor()
{
  delete THIS;
}

// doc in super
void
SoWWWAnchor::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoWWWAnchor, SO_FROM_INVENTOR_2_1|SoNode::VRML1);
}


/*!
  Sets the full URL to \a url. If this is set, this URL will be used in
  callbacks instead of the URL set in SoWWWAnchor::name.

  \sa SoWWWAnchor::getFullURLName()
 */
void
SoWWWAnchor::setFullURLName(const SbString & url)
{
  THIS->fullname = url;
}

/*!
  Returns the full URL if its set by SoWWWAnchor::setFullURLName(). Otherwise
  the contents of SoWWWAnchor::name is returned.

  \sa SoWWWAnchor::setFullURLName()
 */
const SbString &
SoWWWAnchor::getFullURLName(void)
{
  if (THIS->fullname.getLength() > 0) {
    return THIS->fullname;
  } 
  
  this->name.get(THIS->name);
  return THIS->name;
}

// doc from parent
void
SoWWWAnchor::handleEvent(SoHandleEventAction * action)
{
  const SoEvent * event = action->getEvent();
  if (event->isOfType(SoMouseButtonEvent::getClassTypeId()) &&
      SoWWWAnchor::fetchfunc) {
    const SoMouseButtonEvent * mbevent = (SoMouseButtonEvent*)event;
    if (SoMouseButtonEvent::isButtonPressEvent(mbevent, 
                                               SoMouseButtonEvent::BUTTON1)) {
      SbString s = THIS->fullname;
      if (s.getLength() == 0) {
        this->name.get(s);
      }
      if (this->map.getValue() == POINT) {
        const SoPickedPoint * pp = action->getPickedPoint();
        const SbVec3f point = pp->getObjectPoint(NULL);
        SbString temp;
        temp.sprintf("?%g%%2c%g%%2c%g", point[0], point[1], point[2]);
        s.operator+=(temp);
      }
      
      fetchfunc(s, fetchdata, this);
    }
  }
  inherited::handleEvent(action);
}

/*!
  Sets the callback function \a f that is called when a SoWWWAnchor node is
  clicked on. This callback can among other things be used to provide a 
  browser with the URL of this node.

  The callback will be called with the URL, \a userData and a pointer to
  this node as arguments.
 */
void
SoWWWAnchor::setFetchURLCallBack(SoWWWAnchorCB * f, void * userData)
{
  SoWWWAnchor::fetchfunc = f;
  SoWWWAnchor::fetchdata = userData;
}

/*!
  Sets the callback function \a f that is called when a SoWWWAnchor node
  is highlighted. This callback can among other things be used to provide
  the user with a visual clue on which URL the node points to, for example
  by showing the URL as a string.

  The callback will be called with the URL, \a userData and a pointer to
  this node as arguments.
 */
void
SoWWWAnchor::setHighlightURLCallBack(SoWWWAnchorCB * f, void * userData)
{
  SoWWWAnchor::highlightfunc = f;
  SoWWWAnchor::highlightdata = userData;
}

/*!
  Reimplemented from SoLocateHighlight.

  Calls the highlight callback. 
 */
void
SoWWWAnchor::redrawHighlighted(SoAction * act, SbBool isNowHighlighting)
{
  inherited::redrawHighlighted(act, isNowHighlighting);
  SbString s = THIS->fullname;
  if (s.getLength() == 0) {
    this->name.get(s);
  }
  if (SoWWWAnchor::highlightfunc) {
    SoWWWAnchor::highlightfunc(s, SoWWWAnchor::highlightdata, this);  
  }
}

