/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 2001 by Systems in Motion. All rights reserved.
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
  \class SoVRMLAnchor SoVRMLAnchor.h Inventor/VRMLnodes/SoVRMLAnchor.h
  \brief The SoVRMLAnchor class is used for linking to other URL resources.
  \ingroup VRMLnodes

  \WEB3DCOPYRIGHT

  \verbatim
  Anchor {
    eventIn      MFNode   addChildren
    eventIn      MFNode   removeChildren
    exposedField MFNode   children        []
    exposedField SFString description     ""
    exposedField MFString parameter       []
    exposedField MFString url             []
    field        SFVec3f  bboxCenter      0 0 0     # (-inf, inf)
    field        SFVec3f  bboxSize        -1 -1 -1  # (0, inf) or -1,-1,-1
  }

  \endverbatim

  The Anchor grouping node retrieves the content of a URL when the
  user activates (e.g., clicks) some geometry contained within the
  Anchor node's children. If the URL points to a valid VRML file, that
  world replaces the world of which the Anchor node is a part (except
  when the parameter field, described below, alters this
  behaviour). If non-VRML data is retrieved, the browser shall
  determine how to handle that data; typically, it will be passed to
  an appropriate non-VRML browser.  Exactly how a user activates
  geometry contained by the Anchor node depends on the pointing device
  and is determined by the VRML browser. Typically, clicking with the
  pointing device will result in the new scene replacing the current
  scene. An Anchor node with an empty url does nothing when its
  children are chosen. A description of how multiple Anchors and
  pointing-device sensors are resolved on activation is contained in
  4.6.7, Sensor nodes
  (http://www.web3d.org/technicalinfo/specifications/vrml97/part1/concepts.html#4.6.7).

  More details on the children, addChildren, and removeChildren fields
  and eventIns can be found in 4.6.5, Grouping and children nodes
  (http://www.web3d.org/technicalinfo/specifications/vrml97/part1/concepts.html#4.6.5).

  The description field in the Anchor node specifies a textual
  description of the Anchor node. This may be used by browser-specific
  user interfaces that wish to present users with more detailed
  information about the Anchor.  The parameter exposed field may be
  used to supply any additional information to be interpreted by the
  browser. Each string shall consist of "keyword=value" pairs. For
  example, some browsers allow the specification of a 'target' for a
  link to display a link in another part of an HTML document. The
  parameter field is then:

  \verbatim
  Anchor {
    parameter [ "target=name_of_frame" ]
    ...
  }
  \endverbatim

  An Anchor node may be used to bind the initial Viewpoint node in a
  world by specifying a URL ending with "#ViewpointName" where
  "ViewpointName" is the name of a viewpoint defined in the VRML
  file. For example:

  \verbatim
  Anchor {
    url "http://www.school.edu/vrml/someScene.wrl#OverView"
    children  Shape { geometry Box {} }
  }
  \endverbatim

  specifies an anchor that loads the VRML file "someScene.wrl" and
  binds the initial user view to the Viewpoint node named "OverView"
  when the Anchor node's geometry (Box) is activated. If the named
  Viewpoint node is not found in the VRML file, the VRML file is
  loaded using the default Viewpoint node binding stack rules (see
  VRMLViewpoint).  If the url field is specified in the form
  "#ViewpointName" (i.e. no file name), the Viewpoint node with the
  given name ("ViewpointName") in the Anchor's run-time name scope(s)
  shall be bound (set_bind TRUE).  The results are undefined if there
  are multiple Viewpoints with the same name in the Anchor's run-time
  name scope(s). The results are undefined if the Anchor node is not
  part of any run-time name scope or is part of more than one run-time
  name scope. See 4.4.6, Run-time name scope, for a description of
  run-time name scopes. See VRMLViewpoint, for the Viewpoint
  transition rules that specify how browsers shall interpret the
  transition from the old Viewpoint node to the new one. For example:

  \verbatim

  Anchor {
    url "#Doorway"
    children Shape {
      geometry Sphere {}
    }
  }
  \endverbatim

  binds the viewer to the viewpoint defined by the "Doorway" viewpoint
  in the current world when the sphere is activated. In this case, if
  the Viewpoint is not found, no action occurs on activation.  More
  details on the url field are contained in 4.5, VRML and the World
  Wide Web
  (http://www.web3d.org/technicalinfo/specifications/vrml97/part1/concepts.html#4.5).
  The bboxCenter and bboxSize fields specify a bounding box
  that encloses the Anchor's children. This is a hint that may be used
  for optimization purposes. The results are undefined if the
  specified bounding box is smaller than the actual bounding box of
  the children at any time.  The default bboxSize value, (-1, -1, -1),
  implies that the bounding box is not specified and if needed shall
  be calculated by the browser. More details on the bboxCenter and
  bboxSize fields can be found in 4.6.4, Bounding boxes
  (http://www.web3d.org/technicalinfo/specifications/vrml97/part1/concepts.html#4.6.4).

*/

/*!
  \var SoVRMLAnchor::url

  The URL string.
*/

/*!
  \var SoVRMLAnchor::description

  The textual description of the URL.
*/

/*!
  \var SoVRMLAnchor::parameter

  May be used to supply additional information to the browser.

  Each string should be pairs of \e keyword = \e value.
*/

/*!
  \var SoVRMLAnchor::bboxCenter
  Children bounding box hint center. Default value is (0, 0, 0).
*/

/*!
  \var SoVRMLAnchor::bboxSize
  Children bounding box size hint. Default value is (-1, -1, -1).
*/

#include <Inventor/VRMLnodes/SoVRMLAnchor.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/actions/SoHandleEventAction.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <stdlib.h>

SO_NODE_SOURCE(SoVRMLAnchor);

// doc in parent
void
SoVRMLAnchor::initClass(void) // static
{
  SO_NODE_INTERNAL_INIT_CLASS(SoVRMLAnchor, SO_VRML97_NODE_TYPE);
}


/*!
  Default constructor.
*/
SoVRMLAnchor::SoVRMLAnchor(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoVRMLAnchor);

  SO_VRMLNODE_ADD_EMPTY_EXPOSED_MFIELD(url);
  SO_VRMLNODE_ADD_EXPOSED_FIELD(description, (""));
  SO_VRMLNODE_ADD_EMPTY_EXPOSED_MFIELD(parameter);

  SO_VRMLNODE_ADD_FIELD(bboxCenter, (0.0f, 0.0f, 0.0f));
  SO_VRMLNODE_ADD_FIELD(bboxSize, (-1.0f, -1.0f, -1.0f));
}

/*!
  Destructor.
*/
SoVRMLAnchor::~SoVRMLAnchor()
{
}

/*!
  Sets the callback that will be called when the node is selected.
*/
void
SoVRMLAnchor::setFetchURLCallBack(SoVRMLAnchorCB *, void * closure)
{
  // FIXME: store callback, and call it when a pick is detected.
}

// doc in parent
void
SoVRMLAnchor::handleEvent(SoHandleEventAction * action)
{
  SoState * state = action->getState();
  state->push();
  // FIXME: detect picks
  inherited::handleEvent(action);
  state->pop();
}

// doc in parent
void
SoVRMLAnchor::write(SoWriteAction * action)
{
  SoState * state = action->getState();
  state->push();
  // do not call inherited::write() or SoGroup::write()
  SoNode::write(action);
  state->pop();
}
