/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

/*!
  \class SoInteractionKit SoInteractionKit.h Inventor/nodekits/SoInteractionKit.h
  \brief The SoInteractionKit class is the base class for all draggers.
  \ingroup nodekits

  Ihis nodekits makes it possible to set surrogate paths for
  parts. Instead of creating new geometry for the dragger, it is
  possible to specify an existing path in your scene to be used for
  interaction. All picks on this path will be handled by the dragger.
*/

#include <Inventor/nodekits/SoInteractionKit.h>
#include <Inventor/misc/SoBasic.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/SoDB.h>
#include <Inventor/SoInput.h>
#include <Inventor/fields/SoSFNode.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/nodes/SoText2.h>

#include <stdlib.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

SbList <SoNode*> * SoInteractionKit::defaultdraggerparts = NULL;

//
// atexit callback. To delete default dragger parts files.
//
void
SoInteractionKit::clean(void)
{
  int n = SoInteractionKit::defaultdraggerparts->getLength();
  for (int i = 0; i < n; i++) {
    (*SoInteractionKit::defaultdraggerparts)[i]->unref();
  }

  delete SoInteractionKit::defaultdraggerparts;
}

void
SoInteractionKit::sensorCB(void *data, SoSensor *)
{
  SoInteractionKit *thisp = (SoInteractionKit*) data;
  SoSeparator *sep = (SoSeparator*)thisp->topSeparator.getValue();
  if (thisp->oldTopSeparator != sep) {
    thisp->connectSeparator(thisp->oldTopSeparator, FALSE);
    thisp->connectSeparator(sep, TRUE);
    thisp->oldTopSeparator = sep;
  }
}

SO_KIT_SOURCE(SoInteractionKit);


/*!
  Constructor.
*/
SoInteractionKit::SoInteractionKit(void)
{
  SO_KIT_INTERNAL_CONSTRUCTOR(SoInteractionKit);

  SO_NODE_ADD_FIELD(renderCaching, (SoInteractionKit::AUTO));
  SO_NODE_ADD_FIELD(boundingBoxCaching, (SoInteractionKit::AUTO));
  SO_NODE_ADD_FIELD(renderCulling, (SoInteractionKit::AUTO));
  SO_NODE_ADD_FIELD(pickCulling, (SoInteractionKit::AUTO));

  SO_NODE_DEFINE_ENUM_VALUE(CacheEnabled, ON);
  SO_NODE_DEFINE_ENUM_VALUE(CacheEnabled, OFF);
  SO_NODE_DEFINE_ENUM_VALUE(CacheEnabled, AUTO);

  SO_NODE_SET_SF_ENUM_TYPE(renderCaching, CacheEnabled);
  SO_NODE_SET_SF_ENUM_TYPE(boundingBoxCaching, CacheEnabled);
  SO_NODE_SET_SF_ENUM_TYPE(renderCulling, CacheEnabled);
  SO_NODE_SET_SF_ENUM_TYPE(pickCulling, CacheEnabled);


  // Note: we must use "" instead of , , to humour MS VisualC++ 6.

  SO_KIT_ADD_CATALOG_ENTRY(topSeparator, SoSeparator, TRUE, this, "", FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(geomSeparator, SoSeparator, TRUE, topSeparator, "", FALSE);

  SO_KIT_INIT_INSTANCE();

  this->oldTopSeparator = NULL;
  this->topSeparatorSensor = new SoFieldSensor(SoInteractionKit::sensorCB, this);
  this->topSeparatorSensor->setPriority(0);

  // setup
  SoInteractionKit::sensorCB(this, this->topSeparatorSensor);

  // FIXME: investigate whether setUpConnections should be called here,
  // or if that always will be handled by the subclass dragger.
  // pederb, 2000-01-13
}

/*!
  Destructor.
*/
SoInteractionKit::~SoInteractionKit()
{
  this->connectSeparator(this->oldTopSeparator, FALSE); // disconnect fields
  delete this->topSeparatorSensor;
}

/*!
  Does initialization common for all objects of the
  SoInteractionKit class. This includes setting up the
  type system, among other things.
*/
void
SoInteractionKit::initClass(void)
{
  SoInteractionKit::defaultdraggerparts = new SbList <SoNode*>;

  SO_KIT_INTERNAL_INIT_CLASS(SoInteractionKit);
}

/*!
  Sets a part in the kit as a surrogate path. The \a partname part is set
  to NULL, and the surrogate path is remembered. Following picks on the
  surrogate path will be regarded as a pick on \a partname.
*/
SbBool
SoInteractionKit::setPartAsPath(const SbName &partname,
                                SoPath *path)
{
  return this->setAnySurrogatePath(partname, path, TRUE, TRUE);
}

/*!
  Sets the value of \a partname to \a node, and sets the part's field to
  default (field/part is not written). If \a onlyifdefault is \e TRUE,
  \a partname is only set if it is already in the default state.
*/
SbBool
SoInteractionKit::setPartAsDefault(const SbName &partname,
                                   SoNode *node,
                                   SbBool onlyifdefault)
{
  return this->setAnyPartAsDefault(partname, node, FALSE, onlyifdefault);
}

/*!
  Find node in the global dictionary, and set as default.
  \sa setPartAsDefault()
*/
SbBool
SoInteractionKit::setPartAsDefault(const SbName &partname,
                                   const SbName &nodename,
                                   SbBool onlyifdefault)
{
  return this->setAnyPartAsDefault(partname, nodename, FALSE, onlyifdefault);
}


/*!
  Checks if \a path is contained within any of the surrogate paths
  in any interaction kits from this node down. Returns information
  about the owner and the surrogate path if found, and \a fillargs is \e TRUE.
*/
SbBool
SoInteractionKit::isPathSurrogateInMySubgraph(const SoPath *path,
                                              SoPath *&pathToOwner,
                                              SbName  &surrogatename,
                                              SoPath *&surrogatepath,
                                              SbBool fillargs)
{
  int idx = this->findSurrogateInPath(path);
  if (idx >= 0) {
    if (fillargs) {
      // FIXME: make sure this path is unref'ed in dragger, pederb 2000-01-13
      pathToOwner = new SoPath(this); // a very short path
      pathToOwner->ref();
      surrogatename = this->surrogateNames[idx];
      surrogatepath = this->surrogatePaths[idx];
    }
    return TRUE;
  }
  else {
    SoSearchAction sa;
    sa.setType(SoInteractionKit::getClassTypeId());
    sa.setFind(SoSearchAction::ALL);
    sa.setSearchingAll(TRUE);
    sa.apply(this);
    SoPathList &list = sa.getPaths();
    for (int i = 0; i < list.getLength(); i++) {
      SoInteractionKit *kit = (SoInteractionKit*)list[i]->getTail();
      assert(kit->isOfType(SoInteractionKit::getClassTypeId()));
      int idx = kit->findSurrogateInPath(path);
      if (idx >= 0) {
        if (fillargs) {
          pathToOwner = list[i]->copy();
          pathToOwner->ref();
          surrogatename = kit->surrogateNames[idx];
          surrogatepath = kit->surrogatePaths[idx];
        }
        return TRUE;
      }
    }
  }
  return FALSE;
}

/*!
  \overload
*/
SbBool
SoInteractionKit::isPathSurrogateInMySubgraph(const SoPath *path)
{
  SoPath *dummypath, *dummypath2;
  SbName dummyname;

  return this->isPathSurrogateInMySubgraph(path, dummypath,
                                           dummyname, dummypath2, FALSE);
}

/*!
  Convenience method that sets the switch value for a switch node.
  Checks if node != 0, and only sets the switch value if value
  has changed.
*/
void
SoInteractionKit::setSwitchValue(SoNode *node, const int newVal)
{
  if (node == NULL) return;
  assert(node->isOfType(SoSwitch::getClassTypeId()));
  SoSwitch *mySwitch = (SoSwitch*)node;
  if (mySwitch->whichChild.getValue() != newVal) {
    mySwitch->whichChild = newVal;
  }
}

/*!
  Overloaded to copy the surrogate lists.
 */
void
SoInteractionKit::copyContents(const SoFieldContainer *fromFC,
                               SbBool copyConnections)
{
  inherited::copyContents(fromFC, copyConnections);

  assert(fromFC->isOfType(SoInteractionKit::getClassTypeId()));
  SoInteractionKit *kit = (SoInteractionKit*) fromFC;

  this->surrogateNames.truncate(0);
  this->surrogateNames.truncate(0);

  int i, n = kit->surrogateNames.getLength();
  for (i = 0; i < n; i++) {
    this->surrogateNames.append(kit->surrogateNames[i]);
    this->surrogateNames.append(kit->surrogateNames[i]);
  }

  if (copyConnections) {
    // FIXME: copy connections also (if that applies to this node?)
    // pederb, 2000-01-13
    COIN_STUB();
  }
}

/*!
  Overloaded to check topSeperator and fields after reading.
*/
SbBool
SoInteractionKit::readInstance(SoInput *in, unsigned short flags)
{
  SbBool ret = inherited::readInstance(in, flags); // will handle fields
  if (ret) {
    // update connections, if necessary
    SoInteractionKit::sensorCB(this, NULL);
  }
  return ret;
}

/*!
  Reads default parts for a dragger. \a fileName is the user-changeable
  resource file (most often an ascii iv file), while \a defaultBuffer and
  defBufSize contains the compiled-in default parts (binary iv).

  This method is called from dragger constructors.
*/
void
SoInteractionKit::readDefaultParts(const char *fileName,
                                   const char defaultBuffer[],
                                   int defBufSize)
{
  SbString fullname;
  char *dir = getenv("SO_DRAGGER_DIR");
  if (dir == NULL) {
#if COIN_DEBUG && 1 // debug
    SoDebugError::postInfo("SoInteractionKit::readDefaultParts",
                           "\n\n\nDraggers are under development. You must set the "
                           "SO_DRAGGER_DIR environment\nvariable to the draggerDefaults "
                           "directory to test the draggers!\n\n");
#endif // debug
    return;
  }
  fullname = dir;
  
#ifdef _WIN32  
  if (fillname[fullname.getLength()-1] != '\\') fullname += "\\";
#else // ! WIN32
  if (fullname[fullname.getLength()-1] != '/') fullname += "/";
#endif // !WIN32
  
  fullname += fileName;
  
  SoInput input;
  if (!fileName || !input.openFile(fullname.getString())) {
    input.setBuffer((void*)defaultBuffer, defBufSize);
  }
  SoNode *node = (SoNode*)SoDB::readAll(&input);
  if (node == NULL) {
#if COIN_DEBUG && 1 // debug
    SoDebugError::postInfo("SoInteractionKit::readDefaultParts",
                           "error reading dragger defaults: %s", fullname.getString());
#endif // debug
  }
  else {
    node->ref(); // this node is unref'ed at exit
    SoInteractionKit::defaultdraggerparts->append(node);
    if (SoInteractionKit::defaultdraggerparts->getLength() == 1) {
      atexit(SoInteractionKit::clean);
    }
  }
}

/*!
  Protected version of setPartAsDefault(), to make it possible to set non-leaf
  and private parts (if anypart is \e TRUE).
  \sa setPartAsDefault()
*/
SbBool
SoInteractionKit::setAnyPartAsDefault(const SbName &partname,
                                      SoNode *node,
                                      SbBool anypart,
                                      SbBool onlyifdefault)
{
  SoBaseKit *kit = this;
  int partNum;
  SbBool isList;
  int listIdx;
  if (SoBaseKit::findPart(SbString(partname.getString()), kit, partNum,
                          isList, listIdx, TRUE)) {
    SoSFNode *field = kit->fieldList[partNum];
    // FIXME: default check not working properly. pederb, 2000-01-21
    if (1 || (!onlyifdefault || field->isDefault())) {
      kit->setPart(partNum, node);
      field->setDefault(TRUE);
    }
    else {
#if COIN_DEBUG && 1 // debug
      SoDebugError::postInfo("SoInteractionKit::setAnyPartAsDefault",
                             "no permission to set");
#endif // debug
    }
  }
#if COIN_DEBUG && 1 // debug
  else {
    SoDebugError::postInfo("SoInteractionKit::setAnyPartAsDefault",
                           "part %s not found", partname.getString());
  }
#endif // debug

  return FALSE;
}

/*!
  Protected version of setPartAsDefault(), to make it possible to set non-leaf
  and private parts (if anypart is \e TRUE).
  \sa setPartAsDefault()
*/
SbBool
SoInteractionKit::setAnyPartAsDefault(const SbName &partname,
                                      const SbName &nodename,
                                      SbBool anypart,
                                      SbBool onlyifdefault)
{
  SoNode *node = (SoNode*)
    SoBase::getNamedBase(nodename, SoNode::getClassTypeId());
  if (node) {
    return this->setAnyPartAsDefault(partname, node, anypart, onlyifdefault);
  }
#if COIN_DEBUG && 1 // debug
  else {
    SoDebugError::postInfo("SoInteractionKit::setAnyPartAsDefault",
                           "nodename %s not found", nodename.getString());

    // FIXME: temporary code, pederb 2000-01-21
    node = new SoText2();
    ((SoText2*)node)->string = "Default dragger part not found";
    return this->setAnyPartAsDefault(partname, node, anypart, onlyifdefault);
  }
#endif // debug
  return FALSE;
}

/*!
  Protected version of setPartAsPath(), to make it possible to set non-leaf
  and private parts.
  \sa setPartAsPath()
*/
SbBool
SoInteractionKit::setAnySurrogatePath(const SbName &partname,
                                      SoPath *path,
                                      SbBool leafcheck,
                                      SbBool publiccheck)
{
  SoBaseKit *kit = this;
  int partNum;
  SbBool isList;
  int listIdx;
  if (SoBaseKit::findPart(SbString(partname.getString()), kit, partNum,
                          isList, listIdx, TRUE)) {
    assert(kit->isOfType(SoInteractionKit::getClassTypeId()));
    const SoNodekitCatalog *catalog = kit->getNodekitCatalog();
    if (leafcheck && !catalog->isLeaf(partNum)) {
#if COIN_DEBUG && 1 // debug
      SoDebugError::postInfo("SoInteractionKit::setAnySurrogatePath",
                             "part %s is not leaf", partname.getString());
#endif // debug
      return FALSE;
    }
    if (publiccheck && !catalog->isPublic(partNum)) {
#if COIN_DEBUG && 1 // debug
      SoDebugError::postInfo("SoInteractionKit::setAnySurrogatePath",
                             "part %s is not public", partname.getString());
#endif // debug
      return FALSE;
    }
    int parentIdx = catalog->getParentPartNumber(partNum);
    SoNode *parent = this->fieldList[parentIdx]->getValue();
    if (parent->isOfType(SoSwitch::getClassTypeId())) {
      SoNode *node = this->fieldList[partNum]->getValue();
      SoType type = node->getTypeId();
      if (type == SoGroup::getClassTypeId() ||
          type == SoSeparator::getClassTypeId()) {
        // replace with empty group to keep switch numbering
        kit->setPart(partNum, (SoNode*)type.createInstance());
      }
      else { // set to NULL and update switch numbering
        SoSwitch *sw = (SoSwitch*)parent;
        int whichChild = sw->whichChild.getValue();
        int partIdx = sw->findChild(node);
        if (partIdx == whichChild) {
          sw->whichChild.setValue(SO_SWITCH_NONE);
        }
        else if (partIdx < whichChild) {
          sw->whichChild.setValue(whichChild-1);
        }
        kit->setPart(partNum, NULL);
      }
    }
    else {
      // set part to NULL
      kit->setPart(partNum, NULL);
    }
    // add the path
    ((SoInteractionKit*)kit)->addSurrogatePath(path, catalog->getName(partNum));
    return TRUE;
  }
#if COIN_DEBUG && 1 // debug
  else {
    SoDebugError::postInfo("SoInteractionKit::setAnyPartAsDefault",
                           "part %s not found", partname.getString());
  }
#endif // debug
  return FALSE;
}

/*!
  FIXME: doc
*/
SbBool
SoInteractionKit::setUpConnections(SbBool onoff, SbBool doitalways)
{
  if (!doitalways && this->connectionsSetUp == onoff)
    return onoff;

  SoSeparator *sep = (SoSeparator*)this->topSeparator.getValue();

  if (onoff) {
    inherited::setUpConnections(onoff, doitalways);
    this->connectSeparator(sep, TRUE);
  }
  else {
    this->connectSeparator(sep, FALSE);
    inherited::setUpConnections(onoff, doitalways);
  }
  this->connectionsSetUp = onoff;
  return !onoff;
}

/*!
  Overloaded to detect when part changes value. If a substitute
  path for that part exists, it must be cleared.
*/
SbBool
SoInteractionKit::setPart(const int partNum, SoNode *node)
{
  this->removeSurrogatePath(this->getNodekitCatalog()->getName(partNum));
  return inherited::setPart(partNum, node);
}

/*!
  FIXME: doc
*/
void
SoInteractionKit::setDefaultOnNonWritingFields(void)
{
  this->topSeparator.setDefault(TRUE);
  this->geomSeparator.setDefault(TRUE);

  if (!this->renderCaching.isConnected() &&
      this->renderCaching.getValue() == SoInteractionKit::AUTO)
    this->renderCaching.setDefault(TRUE);
  if (!this->boundingBoxCaching.isConnected() &&
      this->boundingBoxCaching.getValue() == SoInteractionKit::AUTO)
    this->boundingBoxCaching.setDefault(TRUE);
  if (!this->pickCulling.isConnected() &&
      this->pickCulling.getValue() == SoInteractionKit::AUTO)
    this->pickCulling.setDefault(TRUE);
  if (!this->renderCulling.isConnected() &&
      this->renderCulling.getValue() == SoInteractionKit::AUTO)
    this->renderCulling.setDefault(TRUE);

  const SoNodekitCatalog *catalog = this->getNodekitCatalog();

  for (int i = 0; i < this->numCatalogEntries; i++) {
    if (!catalog->isLeaf(i)) {
      SoNode *node = this->fieldList[i]->getValue();
      if (node && node->getTypeId() == SoSwitch::getClassTypeId()) {
        this->fieldList[i]->setDefault(TRUE);
      }
    }
  }

  inherited::setDefaultOnNonWritingFields();
}

//
// checks if partname is in surrogate list. Returns index, -1 if not found.
//
int
SoInteractionKit::findSurrogateIndex(const SbName &partname) const
{
  int i, n = this->surrogateNames.getLength();
  for (i = 0; i < n; i++) {
    if (this->surrogateNames[i] == partname) return i;
  }
  return -1;
}

//
// removes surrogate path with name 'partname'
//
void
SoInteractionKit::removeSurrogatePath(const SbName &partname)
{
  int idx = this->findSurrogateIndex(partname);
  if (idx >= 0) this->removeSurrogatePath(idx);
}

//
// removes a specified surrogate path
//
void
SoInteractionKit::removeSurrogatePath(const int idx)
{
  assert(idx >= 0 && idx < this->surrogateNames.getLength());
  this->surrogateNames.remove(idx);
  this->surrogatePaths.remove(idx);
}

//
// return index of surrogate path that is contained within path,
// or -1 if none found.
//
int
SoInteractionKit::findSurrogateInPath(const SoPath *path)
{
  int n = this->surrogatePaths.getLength();
  for (int i = 0; i < n; i++) {
    if (path->containsPath(this->surrogatePaths[i])) return i;
  }
  return -1;
}

//
// adds or replaces a surrogate path
//
void
SoInteractionKit::addSurrogatePath(SoPath *path, const SbName &name)
{
  int idx = this->findSurrogateIndex(name);
  if (idx >= 0) {
    this->surrogatePaths.remove(idx);
    this->surrogateNames.remove(idx);
  }
  this->surrogatePaths.append(path);
  this->surrogateNames.append(name);
}

//
// private method for connecting/disconnecting to topSeparator
//
void
SoInteractionKit::connectSeparator(SoSeparator *sep, const SbBool onOff)
{
  if (sep == NULL) return;
  if (onOff) {
    sep->renderCaching.connectFrom(&this->renderCaching);
    sep->boundingBoxCaching.connectFrom(&this->renderCaching);
    sep->pickCulling.connectFrom(&this->renderCaching);
    sep->renderCulling.connectFrom(&this->renderCaching);
  }
  else {
    sep->renderCaching.disconnect();
    sep->boundingBoxCaching.disconnect();
    sep->pickCulling.disconnect();
    sep->renderCulling.disconnect();
  }
}
