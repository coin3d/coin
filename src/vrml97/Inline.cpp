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
  \class SoVRMLInline SoVRMLInline.h Inventor/VRMLnodes/SoVRMLInline.h
  \brief The SoVRMLInline class is used to insert VRML files into a scene.
  \ingroup VRMLnodes

  \WEB3DCOPYRIGHT
  
  \verbatim
  Inline {
    exposedField MFString url        []
    field        SFVec3f  bboxCenter 0 0 0     # (-,)
    field        SFVec3f  bboxSize   -1 -1 -1  # (0,) or -1,-1,-1
  }
  \endverbatim

  The Inline node is a grouping node that reads its children data from
  a location in the World Wide Web. Exactly when its children are read
  and displayed is not defined (e.g. reading the children may be
  delayed until the Inline node's bounding box is visible to the
  viewer). The url field specifies the URL containing the children. An
  Inline node with an empty URL does nothing.  

  Each specified URL shall refer to a valid VRML file that contains a
  list of children nodes, prototypes, and routes at the top level as
  described in 4.6.5, Grouping and children nodes.  

  The results are undefined if the URL refers to a file that is not
  VRML or if the VRML file contains non-children nodes at the top
  level.  

  If multiple URLs are specified, the browser may display a URL of a
  lower preference VRML file while it is obtaining, or if it is unable
  to obtain, the higher preference VRML file. Details on the url field
  and preference order can be found in 4.5, VRML and the World Wide
  Web
  (http://www.web3d.org/technicalinfo/specifications/vrml97/part1/concepts.html#4.5).  

  The results are undefined if the contents of the URL change after it
  has been loaded.  

  The bboxCenter and bboxSize fields specify a bounding box that
  encloses the Inline node's children. This is a hint that may be used
  for optimization purposes. The results are undefined if the
  specified bounding box is smaller than the actual bounding box of
  the children at any time. A default bboxSize value, (-1, -1, -1),
  implies that the bounding box is not specified and if needed shall
  be calculated by the browser. A description of the bboxCenter and
  bboxSize fields is in 4.6.4, Bounding boxes
  (http://www.web3d.org/technicalinfo/specifications/vrml97/part1/concepts.html#4.6.4).  

*/

/*!
  SoSFVec3f SoVRMLInline::bboxCenter
  Center of bounding box.
*/

/*!
  SoSFVec3f SoVRMLInline::bboxSize
  Size of bounding box.
*/

/*!
  SoMFString SoVRMLInline::url
  The VRML file URL.
*/

/*!
  enum SoVRMLInline::BboxVisibility
  Used to enumerate bounding box visibility settings.
*/

/*!
  \var SoVRMLInline::BboxVisibility SoVRMLInline::NEVER
  Never display bounding box.
*/

/*!
  \var SoVRMLInline::BboxVisibility SoVRMLInline::UNTIL_LOADED
  Display bounding box until file is loaded.
*/

/*!
  \var SoVRMLInline::BboxVisibility SoVRMLInline::ALWAYS
  Always display bounding box.
*/

#include <Inventor/VRMLnodes/SoVRMLInline.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/SbColor.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoDB.h>
#include <Inventor/SbBox3f.h>
#include <Inventor/errors/SoReadError.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/misc/SoChildList.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/elements/SoGLDiffuseColorElement.h>
#include <Inventor/elements/SoGLLightModelElement.h>
#include <Inventor/elements/SoGLTextureEnabledElement.h>
#include "../tidbits.h" // coin_atexit()
#include <stdlib.h>


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/system/gl.h>

#ifndef DOXYGEN_SKIP_THIS

class SoVRMLInlineP {
public:
  SbString fullurlname;
  SbBool isrequested;
  SoChildList * children;
  SoFieldSensor * urlsensor;
};

#endif // DOXYGEN_SKIP_THIS

static SoVRMLInline::BboxVisibility
sovrmlinline_bboxvisibility = SoVRMLInline::UNTIL_LOADED;
static SoVRMLInlineFetchURLCB * sovrmlinline_fetchurlcb = NULL;
static void * sovrmlinline_fetchurlcbclosure;

static SbColor * sovrmlinline_bboxcolor = NULL;
static SbBool sovrmlinline_readassofile = TRUE;

static void
sovrmlinline_cleanup_color(void)
{
  delete sovrmlinline_bboxcolor;
  sovrmlinline_bboxcolor = NULL;
}

SO_NODE_SOURCE(SoVRMLInline);

// Doc in parent
void
SoVRMLInline::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoVRMLInline, SO_VRML97_NODE_TYPE);
  sovrmlinline_bboxcolor = new SbColor(0.8f, 0.8f, 0.8f);
  coin_atexit((coin_atexit_f*) sovrmlinline_cleanup_color);
}

#undef THIS
#define THIS this->pimpl

/*!
  Constructor
*/
SoVRMLInline::SoVRMLInline(void)
{
  THIS = new SoVRMLInlineP;
  THIS->isrequested = FALSE;
  THIS->children = new SoChildList(this);

  SO_NODE_INTERNAL_CONSTRUCTOR(SoVRMLInline);

  SO_VRMLNODE_ADD_FIELD(bboxCenter, (0.0f, 0.0f, 0.0f));
  SO_VRMLNODE_ADD_FIELD(bboxSize, (-1.0f, -1.0f, -1.0f));
  SO_VRMLNODE_ADD_EMPTY_EXPOSED_MFIELD(url);

  THIS->urlsensor = new SoFieldSensor(SoVRMLInline::urlFieldModified, this);
  THIS->urlsensor->setPriority(0); // immediate sensor
  THIS->urlsensor->attach(& this->url);
}

/*!
  Destructor.
*/
SoVRMLInline::~SoVRMLInline()
{
  delete THIS->children;
  delete THIS;
}

/*!
  Sets the full (non-relative) URL name.
*/
void
SoVRMLInline::setFullURLName(const SbString & url)
{
  THIS->fullurlname = url;
}

/*!
  Returns the full URL name.
*/
const SbString &
SoVRMLInline::getFullURLName(void)
{
  return THIS->fullurlname;
}

/*!
  Returns a copy of the children.
*/
SoGroup *
SoVRMLInline::copyChildren(void) const
{
  if (THIS->children->getLength() == 0) return NULL;
  assert(THIS->children->getLength() == 1);
  SoNode * rootcopy = (*(THIS->children))[0]->copy();
  assert(rootcopy->isOfType(SoGroup::getClassTypeId()));
  return (SoGroup *)rootcopy;
}

// Doc in parent
SoChildList *
SoVRMLInline::getChildren(void) const
{
  return THIS->children;
}

/*!
  Request URL data.
*/
void
SoVRMLInline::requestURLData(void)
{
  THIS->isrequested = TRUE;
  if (sovrmlinline_fetchurlcb) {
    sovrmlinline_fetchurlcb(THIS->fullurlname,
                            sovrmlinline_fetchurlcbclosure,
                            this);
  }
}

/*!
  Returns TRUE if the URL data has been requested.
*/
SbBool
SoVRMLInline::isURLDataRequested(void) const
{
  return THIS->isrequested;
}

/*!
  Returns TRUE if the data has been loaded.
*/
SbBool
SoVRMLInline::isURLDataHere(void) const
{
  return this->getChildData() != NULL;
}

/*!
  Cancel the URL data request.
*/
void
SoVRMLInline::cancelURLDataRequest(void)
{
  THIS->isrequested = FALSE;
}

/*!
  Sets the child data. Can be used by the URL fetch callback.
*/
void
SoVRMLInline::setChildData(SoNode * urldata)
{
  THIS->isrequested = FALSE;
  THIS->children->truncate(0);
  if (urldata) {
    THIS->children->append(urldata);
  }
}

/*!
  Returns the child data (the scene loaded from the url).
*/
SoNode *
SoVRMLInline::getChildData(void) const
{
  if (THIS->children->getLength()) {
    return (*THIS->children)[0];
  }
  return NULL;
}

/*!
  Sets the callback used to handle URL loading.
*/
void
SoVRMLInline::setFetchURLCallBack(SoVRMLInlineFetchURLCB * f,
                                  void * closure)
{
  sovrmlinline_fetchurlcb = f;
  sovrmlinline_fetchurlcbclosure = closure;
}

/*!
  Sets the bounding box visibility strategy.
*/
void
SoVRMLInline::setBoundingBoxVisibility(BboxVisibility b)
{
  sovrmlinline_bboxvisibility = b;
}

/*!
  Returns the bounding box visibility strategy.
*/
SoVRMLInline::BboxVisibility
SoVRMLInline::getBoundingBoxVisibility(void)
{
  return sovrmlinline_bboxvisibility;
}

/*!
  Sets the color of the bounding box.
*/
void
SoVRMLInline::setBoundingBoxColor(SbColor & color)
{
  sovrmlinline_bboxcolor->setValue(color[0], color[1], color[2]);
}

/*!
  Returns the color of the bounding box.
*/
SbColor &
SoVRMLInline::getBoundingBoxColor(void)
{
  return *sovrmlinline_bboxcolor;
}

/*!
  Sets whether Inline nodes should be treated as a normal Inventor SoFile node.
*/
void
SoVRMLInline::setReadAsSoFile(SbBool enable)
{
  sovrmlinline_readassofile = enable;
}

/*!
  Returns whether Inline nodes is read as SoFile nodes.
*/
SbBool
SoVRMLInline::getReadAsSoFile(void)
{
  return sovrmlinline_readassofile;
}

// Doc in parent
void
SoVRMLInline::doAction(SoAction * action)
{
  int numindices;
  const int * indices;
  if (action->getPathCode(numindices, indices) == SoAction::IN_PATH) {
    THIS->children->traverseInPath(action, numindices, indices);
  }
  else {
    THIS->children->traverse(action);
  }
}

// Doc in parent
void
SoVRMLInline::callback(SoCallbackAction * action)
{
  SoVRMLInline::doAction((SoAction*)action);
}

// Doc in parent
void
SoVRMLInline::GLRender(SoGLRenderAction * action)
{
  BboxVisibility vis = sovrmlinline_bboxvisibility;
  SbVec3f size = this->bboxSize.getValue();
  SoNode * child = this->getChildData();
  if ((size[0] >= 0.0f && size[1] >= 0.0f && size[1] >= 0.0f) &&
      ((vis == ALWAYS) || 
       (vis == UNTIL_LOADED && child == NULL))) {
    SoState * state = action->getState();
    SoGLLightModelElement::forceSend(state, SoLightModelElement::BASE_COLOR);
    SoGLTextureEnabledElement::forceSend(state, FALSE);

    SoGLDiffuseColorElement * diffuse = (SoGLDiffuseColorElement*)
      SoDiffuseColorElement::getInstance(state);
    diffuse->sendOnePacked(sovrmlinline_bboxcolor->getPackedValue());
    
    SbVec3f center = this->bboxCenter.getValue();
    SbVec3f minv = center - size*0.5f;
    SbVec3f maxv = center + size*0.5f;
    
    SbVec3f p[8];
    for (int i = 0; i < 8; i++) {
      p[i][0] = i & 1 ? minv[0] : maxv[0];
      p[i][1] = i & 2 ? minv[1] : maxv[1];
      p[i][2] = i & 4 ? minv[2] : maxv[2];
    }

    glBegin(GL_LINE_LOOP);
    glVertex3fv(p[0].getValue());
    glVertex3fv(p[1].getValue());
    glVertex3fv(p[3].getValue());
    glVertex3fv(p[2].getValue());
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex3fv(p[4].getValue());
    glVertex3fv(p[5].getValue());
    glVertex3fv(p[7].getValue());
    glVertex3fv(p[6].getValue());
    glEnd();

    glBegin(GL_LINES);

    glVertex3fv(p[0].getValue());
    glVertex3fv(p[4].getValue());

    glVertex3fv(p[2].getValue());
    glVertex3fv(p[6].getValue());

    glVertex3fv(p[3].getValue());
    glVertex3fv(p[7].getValue());

    glVertex3fv(p[1].getValue());
    glVertex3fv(p[5].getValue());

    glEnd();
  }
  SoVRMLInline::doAction(action);
}

// Doc in parent
void
SoVRMLInline::getBoundingBox(SoGetBoundingBoxAction * action)
{
  SbVec3f size = this->bboxSize.getValue();
  if (size[0] > 0.0f || size[1] > 0.0f || size[2] > 0) {
    size[0] = SbMax(size[0], 0.0f);
    size[1] = SbMax(size[1], 0.0f);
    size[2] = SbMax(size[2], 0.0f);
    SbVec3f center = this->bboxCenter.getValue();
    size *= 0.5f;
    SbBox3f box(center[0]-size[0],
                center[1]-size[1],
                center[2]-size[2],
                center[0]+size[0],
                center[1]+size[1],
                center[2]+size[2]);
    if (!box.isEmpty()) {
      action->extendBy(box);
      action->setCenter(center, TRUE);
    }
  }
  else {
    SoVRMLInline::doAction((SoAction*)action);
  }
}

// Doc in parent
void
SoVRMLInline::getMatrix(SoGetMatrixAction * action)
{
  SoVRMLInline::doAction((SoAction*)action);
}

// Doc in parent
void
SoVRMLInline::handleEvent(SoHandleEventAction * action)
{
  SoVRMLInline::doAction((SoAction*)action);
}

// Doc in parent
void
SoVRMLInline::search(SoSearchAction * action)
{
  SoNode::search(action);
  if (action->isFound()) return;

  SoVRMLInline::doAction(action);
}

// Doc in parent
void
SoVRMLInline::pick(SoPickAction * action)
{
  SoVRMLInline::doAction((SoAction*)action);
}

// Doc in parent
void
SoVRMLInline::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  SoVRMLInline::doAction((SoAction*)action);
}

/*!
  FIXME: doc
*/
void
SoVRMLInline::addBoundingBoxChild(SbVec3f center,
                                  SbVec3f size)
{
  // FIXME: implement
}

// Doc in parent
SbBool
SoVRMLInline::readInstance(SoInput * in,
                           unsigned short flags)
{
  SbBool ret = TRUE;

  THIS->urlsensor->detach();
  if (sovrmlinline_readassofile) {
    THIS->fullurlname.makeEmpty();
    ret = inherited::readInstance(in, flags);
    ret = ret && this->readLocalFile(in);
  }
  else {
    ret = inherited::readInstance(in, flags);
  }
  THIS->urlsensor->attach(&this->url);

  return ret; 
}

// Doc in parent
void
SoVRMLInline::copyContents(const SoFieldContainer * from,
                           SbBool copyconnections)
{
  THIS->children->truncate(0);
  inherited::copyContents(from, copyconnections);

  SoVRMLInline * inlinenode = (SoVRMLInline *)from;
  THIS->fullurlname = inlinenode->pimpl->fullurlname;
  // the request will go to the original node, not this one.
  THIS->isrequested = FALSE;

  if (inlinenode->pimpl->children->getLength() == 0) return;

  assert(inlinenode->pimpl->children->getLength() == 1);

  SoNode * cp = (SoNode *)
    SoFieldContainer::findCopy((*(inlinenode->pimpl->children))[0],
                               copyconnections);
  THIS->children->append(cp);
}

/*!
  Read the (local) file named in the SoVRMLInline::url field.
*/
SbBool
SoVRMLInline::readLocalFile(SoInput * in)
{
  if (this->url.getNum() == 0) {
    return TRUE;
  }

  SbString filename = this->url[0];

  // If we can't find file, ignore it. Note that this does not match
  // the way Inventor works, which will make the whole read process
  // exit with a failure code.
  if (!in->pushFile(filename.getString())) return TRUE;

  THIS->fullurlname = in->getCurFileName();

  SoSeparator * node = SoDB::readAll(in);
  // Popping the file off the stack again is done implicit in SoInput
  // upon hitting EOF (unless the read fails, see below).

  if (node) {
    THIS->children->truncate(0);
    THIS->children->append((SoNode *)node);
  }
  else {
    // Take care of popping the file off the stack. This is a bit
    // "hack-ish", but its done this way instead of loosening the
    // protection of SoInput::popFile().
    if (in->getCurFileName() == THIS->fullurlname) {
      char dummy;
      while (!in->eof()) in->get(dummy);
    }

    // Note that we handle this differently than Inventor, which lets
    // the whole import fail.
    SoReadError::post(in, "Unable to read Inline file: ``%s''",
                      filename.getString());
  }

  return TRUE;
}

// Callback for the field sensor.
void
SoVRMLInline::urlFieldModified(void * userdata, SoSensor * sensor)
{
  SoVRMLInline * thisp = (SoVRMLInline *)userdata;
  SoInput in;
  thisp->pimpl->fullurlname.makeEmpty();
  if (sovrmlinline_readassofile) {
    (void)thisp->readLocalFile(&in);
  }
}
