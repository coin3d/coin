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
  \class SoToVRMLAction Inventor/include/SoToVRMLAction.h
  \brief The SoToVRMLAction class builds a new scene graph using only VRML 1.0 nodes.
  \ingroup actions

  FIXME: more class doc, describe the basics of how the conversion is
  done. 20020705 mortene.

  \sa SoToVRML2Action
*/

#include <Inventor/SbName.h>
#include <Inventor/actions/SoToVRMLAction.h>
#include <coindefs.h> // COIN_STUB()


SO_ACTION_SOURCE(SoToVRMLAction);

// *************************************************************************

class SoToVRMLAction {
public:
  SoToVRMLAction(void)
  {
    this->expandsofile = FALSE;
    this->urlname = "";
    this->writetexcoords = FALSE;
    // FIXME: don't know if this is correct default value. 20020705 mortene.
    this->expandtexture2node = FALSE;
    // FIXME: don't know if this is correct default value. 20020705 mortene.
    this->keepunknownnodes = TRUE;
    this->convertinlinenodes = TRUE;
    this->conditionalconversion = FALSE;
    // FIXME: don't know if this is correct default value. 20020705 mortene.
    this->isverbose = FALSE;
  }

  SbBool expandsofile;
  SbString urlname;
  SbBool writetexcoords;
  SbBool expandtexture2node;
  SbBool keepunknownnodes;
  SbBool convertinlinenodes;
  SbBool conditionalconversion;
  SbBool isverbose;
};

#define PRIVATE(p) (p->pimpl)

// *************************************************************************

// Overridden from parent class.
void
SoToVRMLAction::initClass(void)
{
  SO_ACTION_INIT_CLASS(SoToVRMLAction, SoAction);
}


/*!
  Constructor.
*/

SoToVRMLAction::SoToVRMLAction(void)
{
  PRIVATE(this) = new SoToVRMLActionP;
  COIN_STUB();
}

/*!
  The destructor.
*/

SoToVRMLAction::~SoToVRMLAction(void)
{
  delete PRIVATE(this);
}

// Documented in superclass.
void
SoToVRMLAction::beginTraversal(SoNode * /* node */)
{
  COIN_STUB();
}

void
SoToVRMLAction::apply(SoNode * node)
{
  COIN_STUB();
}

void
SoToVRMLAction::apply(SoPath * path)
{
  COIN_STUB();
}

void
SoToVRMLAction::apply(const SoPathList & pathlist, SbBool obeysrules)
{
  COIN_STUB();
}


SoNode *
SoToVRMLAction::getVRMLSceneGraph(void) const
{
  COIN_STUB();
  return NULL;
}

void
SoToVRMLAction::expandSoFile(SbBool flag)
{
  PRIVATE(this)->expandsofile = flag;
}

SbBool
SoToVRMLAction::areSoFileExpanded(void) const
{
  return PRIVATE(this)->expandsofile;
}

void
SoToVRMLAction::setUrlName(const SbString name)
{
  PRIVATE(this)->urlname = name;
}

SbString
SoToVRMLAction::getUrlName(void) const
{
  return PRIVATE(this)->urlname;
}

void
SoToVRMLAction::writeTexCoords(SbBool flag)
{
  PRIVATE(this)->writetexcoords = flag;
}

SbBool
SoToVRMLAction::areTexCoordWritten(void) const
{
  return PRIVATE(this)->writetexcoords;
}

void
SoToVRMLAction::expandTexture2Node(SbBool flag)
{
  PRIVATE(this)->expandtexture2node = flag;
}

SbBool
SoToVRMLAction::areTexture2NodeExpanded(void) const
{
  return PRIVATE(this)->expandtexture2node;
}

void
SoToVRMLAction::keepUnknownNodes(SbBool flag)
{
  PRIVATE(this)->keepunknownnodes = flag;
}

SbBool
SoToVRMLAction::areUnknownNodeKept(void) const
{
  return PRIVATE(this)->keepunknownnodes;
}

void
SoToVRMLAction::convertInlineNodes(SbBool flag)
{
  PRIVATE(this)->convertinlinenodes = flag;
}

SbBool
SoToVRMLAction::doConvertInlineNodes(void) const
{
  return PRIVATE(this)->convertinlinenodes;
}

void
SoToVRMLAction::conditionalConversion(SbBool flag)
{
  PRIVATE(this)->conditionalconversion = flag;
}

SbBool
SoToVRMLAction::doConditionalConversion(void) const
{
  return PRIVATE(this)->conditionalconversion;
}

void
SoToVRMLAction::setVerbosity(SbBool flag)
{
  PRIVATE(this)->isverbose = flag;
}

SbBool
SoToVRMLAction::isVerbose(void) const
{
  return PRIVATE(this)->isverbose;
}
