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
  \class SoTextureCoordinate2 SoTextureCoordinate2.h Inventor/nodes/SoTextureCoordinate2.h
  \brief The SoTextureCoordinate2 class contains a set of coordinates for the mapping of 2D textures.
  \ingroup nodes

  When encountering nodes of this type during traversal, the
  coordinates it contains will be put on the state stack. Some shape
  nodes (for instance SoIndexedFaceSet, among many others) can then
  use these coordinates for explicit, detailed control of how textures
  are mapped to it's surfaces.

  (If texturemapping is used without any SoTextureCoordinate2 nodes in
  the scenegraph leading up to a shape node, all shape types have
  default fallbacks. So SoTextureCoordinate2 nodes are only necessary
  to use if you are not satisfied with the default mapping.)

  Note that an SoTextureCoordinate2 node will \e replace the
  coordinates already present in the state (if any).

  Here's a very simple example (in Inventor scenegraph file format --
  mapping it to sourcecode is straightforward) that shows how to set
  up two quadratic polygons, one mapped 1:1 to the texture, the other
  using only the upper left quarter of the texture:

\code

Separator {
   Texture2 {
      image 6 8 3
      0x00ff0000 0x00ff0000 0x000000ff 0x000000ff 0x00ff00ff 0x00ff00ff
      0x00ff0000 0x00ff0000 0x000000ff 0x000000ff 0x00ff00ff 0x00ff00ff
      0x00ff0000 0x00ff0000 0x000000ff 0x000000ff 0x00ff00ff 0x00ff00ff
      0x0000ff00 0x0000ff00 0x0000ffff 0x0000ffff 0x0000ff00 0x0000ff00
      0x0000ff00 0x0000ff00 0x0000ffff 0x0000ffff 0x0000ff00 0x0000ff00
      0x00ffff00 0x00ffff00 0x000000ff 0x000000ff 0x00ffffff 0x00ffffff
      0x00ffff00 0x00ffff00 0x000000ff 0x000000ff 0x00ffffff 0x00ffffff
      0x00ffff00 0x00ffff00 0x000000ff 0x000000ff 0x00ffffff 0x00ffffff
   }

   Coordinate3 { point [ -1 -1 0, 1 -1 0, 1 1 0, -1 1 0 ] }

   # "1:1 mapping" to actual texture appearance. (Note that Y goes
   # from bottom to top, versus the common way of specifying bitmap
   # data from top to bottom.)
   TextureCoordinate2 { point [ 0 1, 1 1, 1 0, 0 0 ] }

   IndexedFaceSet {
      coordIndex [ 0, 1, 2, 3, -1 ]
      textureCoordIndex [ 0, 1, 2, 3, -1 ]
   }

   Translation { translation +4 0 0 }

   # Top left corner.
   TextureCoordinate2 { point [ 0 0.5, 0.5 0.5, 0.5 0, 0 0 ] }

   IndexedFaceSet {
      coordIndex [ 0, 1, 2, 3, -1 ]
      textureCoordIndex [ 0, 1, 2, 3, -1 ]
   }
}

\endcode

  \sa SoTextureCoordinateFunction, SoTextureCoordinateBinding
*/

#include <Inventor/nodes/SoTextureCoordinate2.h>
#include <Inventor/nodes/SoSubNodeP.h>


#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLTextureCoordinateElement.h>
#include <Inventor/actions/SoCallbackAction.h>

/*!
  \var SoMFVec2f SoTextureCoordinate2::point

  The set of 2D texture coordinates. Default value of field is an
  empty set.

  Texture coordinates should be specified on normalized coordinates,
  ie in the range [0, 1]. Origo is defined to be in the top-most,
  left-most corner (so note that Y coordinates is "flipped" versus the
  ordinary way of specifying data from bottom to top in bitmaps.)
*/

// *************************************************************************

SO_NODE_SOURCE(SoTextureCoordinate2);

/*!
  Constructor.
*/
SoTextureCoordinate2::SoTextureCoordinate2(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoTextureCoordinate2);
  SO_NODE_ADD_FIELD(point, (NULL));
}

/*!
  Destructor.
*/
SoTextureCoordinate2::~SoTextureCoordinate2()
{
}

// Documented in superclass.
void
SoTextureCoordinate2::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoTextureCoordinate2, SO_FROM_INVENTOR_2_1);

  SO_ENABLE(SoGLRenderAction, SoGLTextureCoordinateElement);

  SO_ENABLE(SoCallbackAction, SoTextureCoordinateElement);
}

// Documented in superclass.
void
SoTextureCoordinate2::doAction(SoAction * action)
{
  SoTextureCoordinateElement::set2(action->getState(), this,
                                   point.getNum(),
                                   point.getValues(0));
}

// Documented in superclass.
void
SoTextureCoordinate2::GLRender(SoGLRenderAction * action)
{
  SoGLTextureCoordinateElement::setTexGen(action->getState(), this, NULL);
  SoTextureCoordinate2::doAction((SoAction *)action);
}

// Documented in superclass.
void
SoTextureCoordinate2::callback(SoCallbackAction * action)
{
  SoTextureCoordinate2::doAction((SoAction *)action);
}

// Documented in superclass.
void
SoTextureCoordinate2::pick(SoPickAction * action)
{
  SoTextureCoordinate2::doAction((SoAction *)action);
}
