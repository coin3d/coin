/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

/*!
  \class SoClipPlane SoClipPlane.h Inventor/nodes/SoClipPlane.h
  \brief The SoClipPlane class is a node type for specifying clipping planes.
  \ingroup nodes

  A scene graph \e without any SoClipPlane nodes uses six clipping
  planes to define the viewing frustum: top, bottom, left, right, near
  and far. If you want extra clipping planes for "slicing" the visible
  geometry, you can do that by using nodes of this type.  Geometry on
  the back side of the clipping plane is clipped away.

  Note that OpenGL implementations have a fixed maximum number of
  clipping planes available. To find out what this number is, you can
  use the following code:

  \code
      #include <Inventor/elements/SoGLClipPlaneElement.h>
      // ...[snip]...
      int maxplanes = SoGLClipPlaneElement::getMaxGLPlanes();
  \endcode

  Below is a simple, basic scene graph usage example of
  SoClipPlane. It connects an SoClipPlane to an SoCenterballDragger,
  for end-user control over the orientation and position of the
  clipping plane:

  \verbatim
  #Inventor V2.1 ascii
  
  Separator {
     Separator {
        Translation { translation -6 0 0 }
        DEF cbdragger CenterballDragger { }
     }
  
     TransformSeparator {
        Transform {
           rotation 0 0 1 0 = USE cbdragger . rotation
           translation 0 0 0 = USE cbdragger . center
        }
        ClipPlane { }
     }
  
     Sphere { }
  }
  \endverbatim

  Note that SoClipPlane is a state-changing appearance node, and as
  such, it will only assert its effects under the current SoSeparator
  node (as the SoSeparator pops the state stack when traversal returns
  above it), as can be witnessed by loading this simple example file
  into a Coin viewer:

  \verbatim
  #Inventor V2.1 ascii
  
  Separator {
     ClipPlane { }
     Cube { }
  }
  
  Separator {
     Translation { translation -3 0 0 }
     Cube { }
  }
  \endverbatim


  <b>FILE FORMAT/DEFAULTS:</b>
  \code
    ClipPlane {
        plane 1 0 0  0
        on TRUE
    }
  \endcode
*/

// *************************************************************************

#include <Inventor/nodes/SoClipPlane.h>

#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/elements/SoGLClipPlaneElement.h>
#include <Inventor/elements/SoCullElement.h>
#include <Inventor/elements/SoModelMatrixElement.h>

#include "nodes/SoSubNodeP.h"

// *************************************************************************

/*!
  \var SoSFPlane SoClipPlane::plane

  Definition of clipping plane.  Geometry on the back side of the
  clipping plane is clipped away.

  The default clipping plane has it's normal pointing in the <1,0,0>
  direction, and intersects origo. (I.e., everything along the
  negative X axis is clipped.)
*/
/*!
  \var SoSFBool SoClipPlane::on
  Whether clipping plane should be on or off. Defaults to \c TRUE.
*/


// *************************************************************************

SO_NODE_SOURCE(SoClipPlane);

/*!
  Constructor.
*/
SoClipPlane::SoClipPlane(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoClipPlane);

  SO_NODE_ADD_FIELD(plane, (SbPlane(SbVec3f(1.0f, 0.0f, 0.0f), 0.0f)));
  SO_NODE_ADD_FIELD(on, (TRUE));
}

/*!
  Destructor.
*/
SoClipPlane::~SoClipPlane()
{
}

// Doc from superclass.
void
SoClipPlane::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoClipPlane, SO_FROM_INVENTOR_1);

  SO_ENABLE(SoGLRenderAction, SoGLClipPlaneElement);
  SO_ENABLE(SoPickAction, SoClipPlaneElement);
  SO_ENABLE(SoCallbackAction, SoClipPlaneElement);
}

// Doc from superclass.
void
SoClipPlane::doAction(SoAction * action)
{
  if (this->on.isIgnored() || this->on.getValue()) {
    SoClipPlaneElement::add(action->getState(), this, plane.getValue());
  }
}

// Doc from superclass.
void
SoClipPlane::GLRender(SoGLRenderAction * action)
{
  SoClipPlane::doAction(action);
  if (this->on.isIgnored() || this->on.getValue()) {
    SbPlane p(SbVec3f(1.0f, 0.0f, 0.0f), 0.0f);
    if (!this->plane.isIgnored()) p = this->plane.getValue();
    p.transform(SoModelMatrixElement::get(action->getState()));
    SoCullElement::addPlane(action->getState(), p);
  }
}

// Doc from superclass.
void
SoClipPlane::callback(SoCallbackAction * action)
{
  SoClipPlane::doAction(action);
}

// Doc from superclass.
void
SoClipPlane::pick(SoPickAction * action)
{
  SoClipPlane::doAction(action);
}
