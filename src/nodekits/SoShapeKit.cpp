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
  \class SoShapeKit SoShapeKit.h Inventor/nodekits/SoShapeKit.h
  \brief The SoShapeKit class provides templates to insert what is usually needed for shape nodes.
  \ingroup nodekits

  \NODEKIT_PRE_DIAGRAM

  \verbatim
  CLASS SoShapeKit
  -->"this"
        "callbackList"
        "topSeparator"
           "pickStyle"
           "appearance"
           "units"
           "transform"
           "texture2Transform"
  -->      "materialBinding"
  -->      "normalBinding"
  -->      "textureCoordinateBinding"
  -->      "shapeHints"
  -->      "coordinate3"
  -->      "coordinate4"
  -->      "normal"
  -->      "textureCoordinate2"
  -->      "profileCoordinate2"
  -->      "profileCoordinate3"
  -->      "profileList"
           "childList"
  -->      "textureCoordinateFunction"
  -->      "localTransform"
  -->      "shapeSeparator"
  -->         "shape"
  \endverbatim

  \NODEKIT_POST_DIAGRAM


  \NODEKIT_PRE_TABLE

  \verbatim
  CLASS SoShapeKit
  PVT   "this",  SoShapeKit  --- 
        "callbackList",  SoNodeKitListPart [ SoCallback, SoEventCallback ] 
  PVT   "topSeparator",  SoSeparator  --- 
        "pickStyle",  SoPickStyle  --- 
        "appearance",  SoAppearanceKit  --- 
        "units",  SoUnits  --- 
        "transform",  SoTransform  --- 
        "texture2Transform",  SoTexture2Transform  --- 
        "materialBinding",  SoMaterialBinding  --- 
        "normalBinding",  SoNormalBinding  --- 
        "textureCoordinateBinding",  SoTextureCoordinateBinding  --- 
        "shapeHints",  SoShapeHints  --- 
        "coordinate3",  SoCoordinate3  --- 
        "coordinate4",  SoCoordinate4  --- 
        "normal",  SoNormal  --- 
        "textureCoordinate2",  SoTextureCoordinate2  --- 
        "profileCoordinate2",  SoProfileCoordinate2  --- 
        "profileCoordinate3",  SoProfileCoordinate3  --- 
        "profileList",  SoNodeKitListPart [ SoProfile ] 
        "childList",  SoNodeKitListPart [ SoShapeKit, SoSeparatorKit ] 
        "textureCoordinateFunction",  SoTextureCoordinateFunction  --- , (default type = SoTextureCoordinateDefault)
        "localTransform",  SoTransform  --- 
  PVT   "shapeSeparator",  SoSeparator  --- 
        "shape",  SoShape  --- , (default type = SoCube)
  \endverbatim

  \NODEKIT_POST_TABLE
*/

#include <Inventor/nodekits/SoShapeKit.h>
#include <Inventor/nodekits/SoSubKitP.h>
#include <Inventor/nodekits/SoNodeKitListPart.h>
#include <Inventor/nodes/SoMaterialBinding.h>
#include <Inventor/nodes/SoNormalBinding.h>
#include <Inventor/nodes/SoTextureCoordinateBinding.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoCoordinate4.h>
#include <Inventor/nodes/SoNormal.h>
#include <Inventor/nodes/SoTextureCoordinate2.h>
#include <Inventor/nodes/SoProfileCoordinate2.h>
#include <Inventor/nodes/SoProfileCoordinate3.h>
#include <Inventor/nodes/SoTextureCoordinateDefault.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoProfile.h>


SO_KIT_SOURCE(SoShapeKit);


/*!
  Constructor.
*/
SoShapeKit::SoShapeKit(void)
{
  SO_KIT_INTERNAL_CONSTRUCTOR(SoShapeKit);

  // Note: we must use "" instead of , , to humour MS VisualC++ 6.

  SO_KIT_ADD_CATALOG_ENTRY(materialBinding, SoMaterialBinding, TRUE, topSeparator, normalBinding, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(normalBinding, SoNormalBinding, TRUE, topSeparator, textureCoordinateBinding, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(textureCoordinateBinding, SoTextureCoordinateBinding, TRUE, topSeparator, shapeHints, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(shapeHints, SoShapeHints, TRUE, topSeparator, coordinate3, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(coordinate3, SoCoordinate3, TRUE, topSeparator, coordinate4, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(coordinate4, SoCoordinate4, TRUE, topSeparator, normal, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(normal, SoNormal, TRUE, topSeparator, textureCoordinate2, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(textureCoordinate2, SoTextureCoordinate2, TRUE, topSeparator, profileCoordinate2, TRUE);
  SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY(textureCoordinateFunction, SoTextureCoordinateFunction, SoTextureCoordinateDefault, TRUE, topSeparator, localTransform, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(profileCoordinate2, SoProfileCoordinate2, TRUE, topSeparator, profileCoordinate3, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(profileCoordinate3, SoProfileCoordinate3, TRUE, topSeparator, profileList, TRUE);
  SO_KIT_ADD_CATALOG_LIST_ENTRY(profileList, SoGroup, TRUE, topSeparator, childList, SoProfile, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(localTransform, SoTransform, TRUE, topSeparator, shapeSeparator, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(shapeSeparator, SoSeparator, TRUE, topSeparator, "", FALSE);
  SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY(shape, SoShape, SoCube, FALSE, shapeSeparator, "", TRUE);

  SO_KIT_INIT_INSTANCE();
}

/*!
  Destructor.
*/
SoShapeKit::~SoShapeKit()
{
}

// Documented in superclass.
void
SoShapeKit::initClass(void)
{
  SO_KIT_INTERNAL_INIT_CLASS(SoShapeKit, SO_FROM_INVENTOR_1);
}

// Documented in superclass.
void
SoShapeKit::setDefaultOnNonWritingFields(void)
{
  this->shapeSeparator.setDefault(TRUE);
  inherited::setDefaultOnNonWritingFields();
}
