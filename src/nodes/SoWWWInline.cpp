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
  \class SoWWWInline SoWWWInline.h Inventor/nodes/SoWWWInline.h
  \brief The SoWWWInline class is a node used to include data from an URL.
  \ingroup nodes

  If the URL is not a local file, the application is responsible for
  supplying a callback to a function which will fetch the data of the
  URL.

  As long as no data has been imported, the scenegraph representation
  of the node will be that of a bounding box enclosing the geometry we
  expect to fetch from the URL.  The application is naturally also
  responsible for specifying the expected dimensions of the geometry.

  \since Inventor 2.1
*/

// FIXME: as far as I can tell, SoWWWInline does not automatically
// trigger a (re-)load when the "SoWWWInline::name" field
// changes. Shouldn't it? Test what SGI/TGS Inventor does and mimic
// it's behaviour. 20020522 mortene.

#include <Inventor/nodes/SoWWWInline.h>
#include <Inventor/nodes/SoSubNodeP.h>

#include <../tidbits.h> // coin_atexit()
#include <coindefs.h> // COIN_OBSOLETED()
#include <Inventor/SbColor.h>

#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLTextureEnabledElement.h>
#include <Inventor/elements/SoGLTexture3EnabledElement.h>
#include <Inventor/elements/SoLightModelElement.h>
#include <Inventor/elements/SoDiffuseColorElement.h>
#include <Inventor/bundles/SoMaterialBundle.h>
#include <Inventor/misc/SoChildList.h>
#include <Inventor/SbColor.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoDB.h>
#include <Inventor/errors/SoReadError.h>
#include <Inventor/errors/SoDebugError.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <Inventor/system/gl.h>

/*!
  \enum SoWWWInline::BboxVisibility
  Used to enumerate bbox rendering strategies.
*/
/*!
  \var SoWWWInline::BboxVisibility SoWWWInline::NEVER
  Never render bounding box.
*/
/*!
  \var SoWWWInline::BboxVisibility SoWWWInline::UNTIL_LOADED
  Render bounding box until children are loaded.
*/
/*!
  \var SoWWWInline::BboxVisibility SoWWWInline::ALWAYS
  Always render bounding box, event when children are loaded.
*/

/*!
  \var SoSFString SoWWWInline::name
  Name of file/URL where children should be read.
*/

/*!
  \var SoSFVec3f SoWWWInline::bboxCenter
  Center of bounding box.
*/
/*!
  \var SoSFVec3f SoWWWInline::bboxSize
  Size of bounding box.
*/
/*!
  \var SoSFNode SoWWWInline::alternateRep
  Alternate representation. Used when children can't be read from name.
*/

// static members
SoWWWInlineFetchURLCB * SoWWWInline::fetchurlcb;
void * SoWWWInline::fetchurlcbdata;
SbColor * SoWWWInline::bboxcolor;
SoWWWInline::BboxVisibility SoWWWInline::bboxvisibility = SoWWWInline::UNTIL_LOADED;
// FIXME: shouldn't this have an explicit init value? (Check what TGS
// Inventor is using.) 20010816 mortene.
SbBool SoWWWInline::readassofile;

void
SoWWWInline::cleanup(void)
{
  delete SoWWWInline::bboxcolor;
}

// *************************************************************************

#ifndef DOXYGEN_SKIP_THIS

class SoWWWInlineP {
 public:
  SoWWWInlineP(SoWWWInline * owner) {
    this->owner = owner;
  }
  SoWWWInline * owner;
  SoChildList * children;
  SbBool readNamedFile(SoInput * in);
  SbBool readChildren(SoInput * in = NULL);
  SbString fullname;
  SbBool didrequest;

  static const char UNDEFINED_FILE[];
};

const char SoWWWInlineP::UNDEFINED_FILE[] = "<Undefined file>";

#endif // DOXYGEN_SKIP_THIS

SO_NODE_SOURCE(SoWWWInline);


#undef THIS
#define THIS this->pimpl

/*!
  Constructor.
*/
SoWWWInline::SoWWWInline()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoWWWInline);

  THIS = new SoWWWInlineP(this);
  THIS->children = new SoChildList(this);
  THIS->didrequest = FALSE;

  SO_NODE_ADD_FIELD(name, (SoWWWInlineP::UNDEFINED_FILE));
  SO_NODE_ADD_FIELD(bboxCenter, (0.0f, 0.0f, 0.0f));
  SO_NODE_ADD_FIELD(bboxSize, (0.0f, 0.0f, 0.0f));
  SO_NODE_ADD_FIELD(alternateRep, (NULL));

  // Instantiated dynamically to avoid problems on platforms with
  // systemloaders that hate static constructors in C++ libraries.
  if (SoWWWInline::bboxcolor == NULL) {
    SoWWWInline::bboxcolor = new SbColor(0.8f, 0.8f, 0.8f);
    coin_atexit((coin_atexit_f *)SoWWWInline::cleanup);
  }
}

/*!
  Destructor.
*/
SoWWWInline::~SoWWWInline()
{
  delete THIS->children;
  delete THIS;
}

// doc in super
void
SoWWWInline::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoWWWInline, SO_FROM_INVENTOR_2_1|SoNode::VRML1);
}

/*!
  If the SoWWWInline::name field specifies a relative URL, use this
  method to name the complete URL.
*/
void
SoWWWInline::setFullURLName(const SbString & url)
{
  THIS->fullname = url;
}

/*!
  If a full URL has been set with the SoWWWInline::setFullURLName()
  method, return it.  If not, returns the value of the
  SoWWWInline::name field.
*/
const SbString &
SoWWWInline::getFullURLName(void)
{
  return THIS->fullname.getLength() ? THIS->fullname : this->name.getValue();
}

/*!
  Returns a subgraph with a deep copy of the children of this node.
*/
SoGroup *
SoWWWInline::copyChildren(void) const
{
  SoChildList * children = this->getChildren();

  if (children->getLength() == 0) return NULL;
  assert(children->getLength() == 1);
  SoNode * rootcopy = (*children)[0]->copy();
  assert(rootcopy->isOfType(SoGroup::getClassTypeId()));
  return (SoGroup *) rootcopy;
}

/*!
  Start requesting URL data. This might trigger a callback to
  the callback set in SoWWWInline::setFetchURLCallBack().
*/
void
SoWWWInline::requestURLData(void)
{
  if (!THIS->didrequest) {
    THIS->didrequest = TRUE;
    SoInput in;
    (void) THIS->readChildren(&in);
  }
}

/*!
  Returns \c TRUE if SoWWWInline::requestURLData() has been called
  without being canceled by SoWWWInline::cancelURLData().
*/
SbBool
SoWWWInline::isURLDataRequested(void) const
{
  return THIS->didrequest;
}

/*!
  Return \c TRUE if the current child data has been read from file an
  URL.
*/
SbBool
SoWWWInline::isURLDataHere(void) const
{
  SoChildList * children = this->getChildren();
  if (children->getLength() == 0 ||
      (*children)[0] == this->alternateRep.getValue()) return FALSE;
  return FALSE;
}

/*!
  Can be used to signal that URL loading has been canceled.  You
  should use this method if you intend to request URL data more than
  once.
*/
void
SoWWWInline::cancelURLDataRequest(void)
{
  THIS->didrequest = FALSE;
}

/*!
  Manually set up the subgraph for this node.
*/
void
SoWWWInline::setChildData(SoNode * urldata)
{
  THIS->children->truncate(0);
  THIS->children->append(urldata);
}

/*!
  Returns the child data for this node. This can be data read from a
  file, from an URL, from the contents of SoWWWInline::alternateRep or
  that was manually set with SoWWWInline::setChildData().
*/
SoNode *
SoWWWInline::getChildData(void) const
{
  if (THIS->children->getLength()) { return (*THIS->children)[0]; }
  return NULL;
}

/*!
  Sets the URL fetch callback. This will be used in
  SoWWWInline::readInstance() or when the user calls
  SoWWWInline::requestURLData().
*/
void
SoWWWInline::setFetchURLCallBack(SoWWWInlineFetchURLCB * f,
                                 void * userdata)
{
  SoWWWInline::fetchurlcb = f;
  SoWWWInline::fetchurlcbdata = userdata;
}

/*!
  Sets the bounding box visibility strategy.
*/
void
SoWWWInline::setBoundingBoxVisibility(BboxVisibility b)
{
  SoWWWInline::bboxvisibility = b;
}

/*!
  Returns the bounding box visibility.
*/
SoWWWInline::BboxVisibility
SoWWWInline::getBoundingBoxVisibility(void)
{
  return SoWWWInline::bboxvisibility;
}

/*!
  Sets the bounding box color.
*/
void
SoWWWInline::setBoundingBoxColor(SbColor & c)
{
  *SoWWWInline::bboxcolor = c;
}

/*!
  Returns the bounding box color.
*/
const SbColor &
SoWWWInline::getBoundingBoxColor(void)
{
  return *SoWWWInline::bboxcolor;
}

/*!
  Sets whether children should be read from a local file, in the same
  manner as SoFile children are read.

  If this is set to \c TRUE, the URL must point to a file on the local
  file system, as can be accessed by the standard C library fopen()
  call.
*/
void
SoWWWInline::setReadAsSoFile(SbBool onoff)
{
  SoWWWInline::readassofile = onoff;
}

/*!
  Returns if children should be read from local files.

  \sa setReadAsSoFile()
*/
SbBool
SoWWWInline::getReadAsSoFile(void)
{
  return SoWWWInline::readassofile;
}


// Documented in superclass.  Overridden to render children and/or
// bounding box.
void
SoWWWInline::GLRender(SoGLRenderAction * action)
{
  if (this->getChildData()) {
    SoWWWInline::doAction(action);
    if (SoWWWInline::bboxvisibility == UNTIL_LOADED) return;
  }
  if (SoWWWInline::bboxvisibility == NEVER) return;

  SoState * state = action->getState();
  state->push();

  SoDiffuseColorElement::set(state, this, 1, SoWWWInline::bboxcolor);

  // disable lighting
  SoLightModelElement::set(state, this, SoLightModelElement::BASE_COLOR);
  // disable texture mapping
  SoGLTextureEnabledElement::set(state, this, FALSE);
  SoGLTexture3EnabledElement::set(state, this, FALSE);

  // update GL state
  state->lazyEvaluate();

  SoMaterialBundle mb(action);
  mb.sendFirst(); // set current color

  float cx, cy, cz;
  this->bboxCenter.getValue().getValue(cx, cy, cz);

  float x0, y0, z0;
  this->bboxSize.getValue().getValue(x0, y0, z0);
  x0 = -x0/2.0f + cx;
  y0 = -y0/2.0f + cy;
  z0 = -z0/2.0f + cz;
  float x1, y1, z1;
  this->bboxSize.getValue().getValue(x1, y1, z1);
  x1 = x1/2.0f + cx;
  y1 = y1/2.0f + cy;
  z1 = z1/2.0f + cz;

  glBegin(GL_LINE_LOOP);
  glVertex3f(x0, y0, z0);
  glVertex3f(x1, y0, z0);
  glVertex3f(x1, y1, z0);
  glVertex3f(x0, y1, z0);
  glEnd();
  glBegin(GL_LINE_LOOP);
  glVertex3f(x0, y0, z1);
  glVertex3f(x1, y0, z1);
  glVertex3f(x1, y1, z1);
  glVertex3f(x0, y1, z1);
  glEnd();

  glBegin(GL_LINES);
  glVertex3f(x0, y0, z0);
  glVertex3f(x0, y0, z1);
  glVertex3f(x0, y1, z0);
  glVertex3f(x0, y1, z1);
  glVertex3f(x1, y0, z0);
  glVertex3f(x1, y0, z1);
  glVertex3f(x1, y1, z0);
  glVertex3f(x1, y1, z1);
  glEnd();

  state->pop(); // restore state
}

// doc in super
void
SoWWWInline::getBoundingBox(SoGetBoundingBoxAction * action)
{
  SbVec3f halfsize = bboxSize.getValue()/2.0f;
  SbVec3f center = bboxCenter.getValue();

  action->extendBy(SbBox3f(-halfsize[0] + center[0],
                           -halfsize[1] + center[1],
                           -halfsize[2] + center[2],
                           halfsize[0] + center[0],
                           halfsize[1] + center[1],
                           halfsize[2] + center[2]));

  assert(! action->isCenterSet());
  action->setCenter(center, TRUE);
}

/*!
  Returns the child list with the child data for this node.
*/
SoChildList *
SoWWWInline::getChildren(void) const
{
  return THIS->children;
}

// doc in super
void
SoWWWInline::doAction(SoAction * action)
{
  int numindices;
  const int * indices;
  if (action->getPathCode(numindices, indices) == SoAction::IN_PATH) {
    this->getChildren()->traverseInPath(action, numindices, indices);
  }
  else {
    this->getChildren()->traverse((SoAction *)action);
  }
}

/*!
  This method should probably have been private in OIV. It is
  obsoleted in Coin. Let us know if you need it.
*/
void
SoWWWInline::doActionOnKidsOrBox(SoAction * action)
{
  COIN_OBSOLETED();
}

// doc in super
void
SoWWWInline::callback(SoCallbackAction * action)
{
  SoWWWInline::doAction((SoAction *)action);
}

// doc in super
void
SoWWWInline::getMatrix(SoGetMatrixAction * action)
{
  SoWWWInline::doAction((SoAction *)action);
}

// doc in super
void
SoWWWInline::handleEvent(SoHandleEventAction * action)
{
  SoWWWInline::doAction((SoAction *)action);
}

// doc in super
void
SoWWWInline::search(SoSearchAction * action)
{
  // maybe search subgraph???
  inherited::search(action);
}

// doc in super
void
SoWWWInline::pick(SoPickAction * action)
{
  SoWWWInline::doAction((SoAction *)action);
}

// doc in super
void
SoWWWInline::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  SoWWWInline::doAction((SoAction *)action);
}

/*!
  Convenience method that extends the current bounding box to
  include the box specified by \a center and \a size.
*/
void
SoWWWInline::addBoundingBoxChild(SbVec3f center, SbVec3f size)
{
  SbVec3f orgsize = this->bboxSize.getValue();
  SbVec3f orgcenter = this->bboxCenter.getValue();

  orgsize *= 0.5f;
  SbBox3f bbox(orgcenter-orgsize, orgcenter+orgsize);

  size *= 0.5f;
  SbBox3f newbox(center-size, center+size);

  bbox.extendBy(newbox);
  this->bboxCenter = bbox.getCenter();
  bbox.getSize(size[0], size[1], size[2]);
  this->bboxSize = size;
}

// Documented in superclass. Overridden to fetch/read child data.
SbBool
SoWWWInline::readInstance(SoInput * in, unsigned short flags)
{
  SbBool ret = inherited::readInstance(in, flags);
  if (ret) {
    ret = THIS->readChildren(in);
  }
  return ret;
}

// Documented in superclass. Overridden to copy children.
void
SoWWWInline::copyContents(const SoFieldContainer * fromfc,
                          SbBool copyconnections)
{
  this->getChildren()->truncate(0);
  inherited::copyContents(fromfc, copyconnections);

  SoWWWInline * inlinenode = (SoWWWInline *) fromfc;

  if (inlinenode->getChildren()->getLength() == 0) return;

  assert(inlinenode->getChildren()->getLength() == 1);

  SoNode * cp = (SoNode *)
    SoFieldContainer::findCopy((*(inlinenode->getChildren()))[0], copyconnections);
  this->getChildren()->append(cp);
}

#undef THIS

#ifndef DOXYGEN_SKIP_THIS

// Read the file named in the name field. Based on SoFile::readNamedFile
SbBool
SoWWWInlineP::readNamedFile(SoInput * in)
{
  // If we can't find file, ignore it. Note that this does not match
  // the way Inventor works, which will make the whole read process
  // exit with a failure code.

  SbString name = this->owner->getFullURLName();

  if (!in->pushFile(name.getString())) return TRUE;

  SoSeparator * node = SoDB::readAll(in);
  // Popping the file off the stack again is done implicit in SoInput
  // upon hitting EOF (unless the read fails, see below).

  if (node) {
    this->children->truncate(0);
    this->children->append((SoNode *)node);
  }
  else {
    // Take care of popping the file off the stack. This is a bit
    // "hack-ish", but its done this way instead of loosening the
    // protection of SoInput::popFile().
    if (in->getCurFileName() == name) {
      char dummy;
      while (!in->eof()) in->get(dummy);
    }

    // Note that we handle this differently than Inventor, which lets
    // the whole import fail.
    SoReadError::post(in, "Unable to read subfile: ``%s''",
                      name.getString());
  }
  return TRUE;
}

// read children, either using the URL callback or by reading from
// local file directly.
SbBool
SoWWWInlineP::readChildren(SoInput * in)
{
  if (in && SoWWWInline::readassofile &&
      this->owner->name.getValue() != SoWWWInlineP::UNDEFINED_FILE) {
    if (!this->readNamedFile(in)) {
      if (this->owner->alternateRep.getValue()) {
#if COIN_DEBUG
        SoDebugError::postInfo("SoWWWInline::readInstance",
                               "Using alternate representation");
#endif // COIN_INSTANCE
        this->owner->setChildData(this->owner->alternateRep.getValue());
      }
    }
  }
  else if (!SoWWWInline::readassofile) {
    if (SoWWWInline::fetchurlcb) {
      SoWWWInline::fetchurlcb(this->owner->getFullURLName(),
                              SoWWWInline::fetchurlcbdata,
                              this->owner);
    }
  }
  return TRUE; // always return TRUE
}

#endif // DOXYGEN_SKIP_THIS
