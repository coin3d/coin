/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2005 by Systems in Motion.  All rights reserved.
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
 *  See <URL:http://www.coin3d.org/> for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

/*!
  \class SoPattern SoPattern.h Inventor/nodes/SoPattern.h
  \brief The SoPattern class is a node type for specifying clipping planes.
  \ingroup nodes

  FIXME: class doc

  <b>FILE FORMAT/DEFAULTS:</b>
  \code
    Pattern {
        category ""
        name ""
    }
  \endcode

  \since Coin 2.4
  \since TGS Inventor 2.6
*/

// *************************************************************************

#include <Inventor/nodes/SoPattern.h>

#include <string.h>

#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/nodes/SoSubNodeP.h>

// *************************************************************************

#ifndef COIN_BETA_VERSION
#error This must either be implemented properly or taken out before the next official release.
#endif // COIN_BETA_VERSION

// *************************************************************************

SO_NODE_SOURCE(SoPattern);

// *************************************************************************

SoPattern::SoPattern(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoPattern);

  SO_NODE_ADD_FIELD(category, (""));
  SO_NODE_ADD_FIELD(name, (""));
}

SoPattern::~SoPattern()
{
}

// Doc from superclass.
void
SoPattern::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoPattern, SO_FROM_INVENTOR_2_6|SO_FROM_COIN_2_4);

  // FIXME: implement SoPatternElement and SoGLPatternElement. 20040818 mortene.
//   SO_ENABLE(SoGLRenderAction, SoPatternElement);
//   SO_ENABLE(SoCallbackAction, SoPatternElement);
}

// *************************************************************************

// Doc from superclass.
void
SoPattern::doAction(SoAction * action)
{
  static SbBool first = TRUE;
  if (first) {
    SoDebugError::postWarning("SoPattern::doAction",
                              "SoPattern not yet implemented, "
                              "patterns will not have any effect.");
    first = FALSE;
  }
}

// Doc from superclass.
void
SoPattern::GLRender(SoGLRenderAction * action)
{
  SoPattern::doAction(action);
}

// Doc from superclass.
void
SoPattern::callback(SoCallbackAction * action)
{
  SoPattern::doAction(action);
}

// *************************************************************************

/*!
  FIXME: doc
*/
SbBool
SoPattern::loadPatterns(const char * filename,
                        int numfilters, const SbString * filternames,
                        SoPattern::FilterType filtertype)
{
  // FIXME: implement. 20040818 mortene.
  return FALSE;
}

/*!
  FIXME: doc
*/
void
SoPattern::addPattern(const char * category, const char * name,
                      const unsigned char * bytes)
{
  // FIXME: implement. 20040818 mortene.
}

/*!
  FIXME: doc
*/
void
SoPattern::getPattern(const char * category, const char * name,
                      unsigned char *& bytes)
{
  // FIXME: implement. 20040818 mortene.

  static unsigned char dummy[128];
  (void)memset(dummy, 0x00, 128);

  bytes = dummy;
}

/*!
  FIXME: doc
*/
void
SoPattern::getPatternNames(int & number, SbString *& names,
                           const char * category)
{
  // FIXME: implement. 20040818 mortene.

  number = 0;
  names = NULL;
}

// *************************************************************************
