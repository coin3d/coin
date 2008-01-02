/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2008 by Kongsberg SIM.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg SIM about acquiring
 *  a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg SIM, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

/*!
  \class SoAnnoText3Property SoAnnoText3Property.h Inventor/nodes/SoAnnoText3Property.h
  \brief The SoAnnoText3Property class is ... FIXME: doc
  \ingroup nodes

  FIXME: doc

  <b>FILE FORMAT/DEFAULTS:</b>
  \code
    AnnoText3Property {
        renderPrintType RENDER3D_PRINT_VECTOR
        fontSizeHint FIT_VECTOR_TEXT
        isCharOrientedRasterPrint TRUE
    }
  \endcode

  \since Coin 2.4
  \since TGS Inventor v2.6
*/

// *************************************************************************

#include <Inventor/nodes/SoAnnoText3Property.h>

#include <Inventor/errors/SoDebugError.h>

#include "nodes/SoSubNodeP.h"

// *************************************************************************

#ifndef COIN_BETA_VERSION
#error This must either be implemented properly or taken out before the next official release.
#endif // COIN_BETA_VERSION

// *************************************************************************

SO_NODE_SOURCE(SoAnnoText3Property);

// *************************************************************************

SoAnnoText3Property::SoAnnoText3Property(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoAnnoText3Property);

  SO_NODE_DEFINE_ENUM_VALUE(RenderPrintType, RENDER3D_PRINT_VECTOR);
  SO_NODE_DEFINE_ENUM_VALUE(RenderPrintType, RENDER3D_PRINT_RASTER);
  SO_NODE_DEFINE_ENUM_VALUE(RenderPrintType, RENDER2D_PRINT_RASTER);
  SO_NODE_SET_SF_ENUM_TYPE(renderPrintType, RenderPrintType);

  SO_NODE_ADD_FIELD(renderPrintType, (RENDER3D_PRINT_VECTOR));

  SO_NODE_DEFINE_ENUM_VALUE(FontSizeHint, ANNOTATION);
  SO_NODE_DEFINE_ENUM_VALUE(FontSizeHint, FIT_VECTOR_TEXT);
  SO_NODE_SET_SF_ENUM_TYPE(fontSizeHint, FontSizeHint);

  SO_NODE_ADD_FIELD(fontSizeHint, (FIT_VECTOR_TEXT));

  SO_NODE_ADD_FIELD(isCharOrientedRasterPrint, (TRUE));


  static SbBool first = TRUE;
  if (first) {
    SoDebugError::postWarning("SoAnnoText3Property::SoAnnoText3Property",
                              "Node not yet implemented.");
    first = FALSE;
  }
}

SoAnnoText3Property::~SoAnnoText3Property()
{
}

// Doc from superclass.
void
SoAnnoText3Property::initClass(void)
{
  // Note: I couldn't find a way to figure out whether this node was
  // added by TGS for Inventor 2.5 or 2.6, so I set it up for the most
  // conservative choice.  -mortene.
  SO_NODE_INTERNAL_INIT_CLASS(SoAnnoText3Property, SO_FROM_INVENTOR_2_6|SO_FROM_COIN_2_4);
}

// *************************************************************************

void
SoAnnoText3Property::doAction(SoAction * action)
{
  // FIXME: implement
}

void
SoAnnoText3Property::GLRender(SoGLRenderAction * action)
{
  // FIXME: implement
}

void
SoAnnoText3Property::callback(SoCallbackAction * action)
{
  // FIXME: implement
}

void
SoAnnoText3Property::getBoundingBox(SoGetBoundingBoxAction * action)
{
  // FIXME: implement
}

void
SoAnnoText3Property::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  // FIXME: implement
}

void
SoAnnoText3Property::pick(SoPickAction * action)
{
  // FIXME: implement
}

// *************************************************************************
