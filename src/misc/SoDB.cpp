/**************************************************************************\
 *
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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
  \class SoDB SoDB.h Inventor/SoDB.h
  \brief The SoDB class keeps track of all internal global data for
  the Coin library.
  \ingroup general

  All methods on SoDB are static.

  Make sure you call SoDB::init() (either directly or indirectly
  through SoXt::init()) before you use any of the other Coin
  classes.

  TODO: more doc.
*/

#include <Inventor/SoDB.h>

#include <Inventor/SbName.h>
#include <Inventor/SbTime.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/errors/SoReadError.h>
#include <Inventor/misc/SoBase.h>
#include <Inventor/system/kosher.h>
#include <Inventor/SoPickedPoint.h>

#include <Inventor/nodekits/SoNodeKit.h>

#include <Inventor/fields/SoField.h>
#include <Inventor/fields/SoSFTime.h>
#include <Inventor/events/SoEvent.h>
#include <Inventor/sensors/SoTimerSensor.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/elements/SoElement.h>
#include <Inventor/actions/SoAction.h>
#include <Inventor/engines/SoEngine.h>
#include <Inventor/engines/SoFieldConverter.h>
#include <Inventor/engines/SoConvertAll.h>
#include <Inventor/details/SoDetail.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#include <assert.h>

#ifdef _WIN32
#include <windows.h>
#endif

// *************************************************************************

class SoDB_HeaderInfo {
public:
  SoDB_HeaderInfo(const SbString & hs, const SbBool bin, const float ver,
                  SoDBHeaderCB * pre, SoDBHeaderCB * post, void * ud)
    : headerstring(hs), isBinary(bin), ivVersion(ver),
        preCB(pre), postCB(post), userdata(ud)
    { }

  SbString headerstring;
  SbBool isBinary;
  float ivVersion;
  SoDBHeaderCB * preCB;
  SoDBHeaderCB * postCB;
  void * userdata;
};

// *************************************************************************


#if defined(NEED_TEMPLATE_DEFINITION)
template class SbList<SoDB_HeaderInfo *>;
template class SbList<SbName>;
template class SbList<SoField *>;
#else // ! defined(NEED_TEMPLATE_DEFINITION)
#ifndef DONT_NEED_TEMPLATE_DEFINITION
#error "kosher.h hasn't been included."
#endif // ! DONT_NEED_TEMPLATE_DEFINITION
#endif // ! defined(NEED_TEMPLATE_DEFINITION)

SbList<SoDB_HeaderInfo *> SoDB::headerlist;
SoSensorManager SoDB::sensormanager;
SbTime SoDB::realtimeinterval;
SbList<SbName> SoDB::fieldnamelist;
SbList<SoField *> SoDB::fieldlist;
SoTimerSensor * SoDB::globaltimersensor = NULL;
SbDict SoDB::converters;
int SoDB::notificationcounter = 0;
SbBool SoDB::isinitialized = FALSE;

// *************************************************************************

/*!
  Initialize the Coin system. This needs to be done as the first
  thing before you start using the library, or you'll probably see an
  early crash.
 */
void
SoDB::init(void)
{
  // Sanity check: if anything here breaks,
  // include/Inventor/system/inttypes.h needs fixing. Keep these
  // asserts around.

  assert(sizeof(uint16_t) == 2);
  assert(sizeof(int16_t) == 2);
  assert(sizeof(uint32_t) == 4);
  assert(sizeof(int32_t) == 4);

  // Sanity check: if the int type is less than 32 bits everything
  // probably goes to hell. Remove this check when we are no longer
  // dependent on using native C types where we need to have a
  // particular bitwidth.

  assert(sizeof(int) == 4);

  // Sanity check: if this breaks, the binary format import and export
  // routines will not work correctly. The code should be fixed to use
  // the int16_t type, then we can remove this stoopid check.

  assert(sizeof(short) == 2);

  // Sanity check: if the int type is unequal to the long type, things
  // will probably break. NB! This is really terrible and _must_ be
  // fixed. Remove this check when we are no longer dependent on using
  // native C types where we need to have a particular bitwidth.

  assert(sizeof(int) == sizeof(long));


  // NB! There's a few dependencies in the order of initialization of
  // components below.

  // This obviously needs to be done first.
  SoType::init();

  SoError::initErrors();
  SoInput::init();
  SoBase::initClass();
  SoDetail::initClass();
  // SoPath inherits SoBase, so initialize it after SoBase.
  SoPath::initClass();
  SoFieldContainer::initClass();
  SoField::initClass();
  // Elements must be initialized before actions.
  SoElement::initClass();
  // Actions must be initialized before nodes (because of SO_ENABLE)
  SoAction::initClass();
  SoNode::initClass();
  SoEngine::initClass();
  SoEvent::initClass();


  // Register all valid file format headers.
  SoDB::registerHeader(SbString("#Inventor V2.1 ascii   "), FALSE, 2.1f,
                       NULL, NULL, NULL);
  SoDB::registerHeader(SbString("#Inventor V2.1 binary  "), TRUE, 2.1f,
                       NULL, NULL, NULL);
  SoDB::registerHeader(SbString("#VRML V2.0 utf8"), FALSE, 2.1f,
                       NULL, NULL, NULL);
  SoDB::registerHeader(SbString("#Inventor V2.4 ascii   "), FALSE, 2.0f,
                       NULL, NULL, NULL);
  SoDB::registerHeader(SbString("#Inventor V2.4 binary  "), TRUE, 2.0f,
                       NULL, NULL, NULL);
  SoDB::registerHeader(SbString("#Inventor V2.5 ascii   "), FALSE, 2.0f,
                       NULL, NULL, NULL);
  SoDB::registerHeader(SbString("#Inventor V2.5 binary  "), TRUE, 2.0f,
                       NULL, NULL, NULL);
  SoDB::registerHeader(SbString("#Inventor V2.0 ascii   "), FALSE, 2.0f,
                       NULL, NULL, NULL);
  SoDB::registerHeader(SbString("#Inventor V2.0 binary  "), TRUE, 2.0f,
                       NULL, NULL, NULL);
  SoDB::registerHeader(SbString("#Inventor V1.0 ascii   "), FALSE, 1.0f,
                       NULL, NULL, NULL);
  SoDB::registerHeader(SbString("#Inventor V1.0 binary  "), TRUE, 1.0f,
                       NULL, NULL, NULL);
  SoDB::registerHeader(SbString("#VRML V1.0 ascii   "), FALSE, 2.1f,
                       NULL, NULL, NULL);

  SoDB::realtimeinterval.setValue(1.0/12.0);

  SoDB::createGlobalField("realTime", SoSFTime::getClassTypeId());
  SoDB::globaltimersensor = new SoTimerSensor;
  SoDB::globaltimersensor->setFunction(SoDB::updateRealTimeFieldCB);
  SoDB::globaltimersensor->setInterval(SoDB::realtimeinterval);
  // FIXME: it would be better to not schedule unless something
  // actually attaches itself to the realtime field, or..? Is this at
  // all possible? 19990225 mortene.
  SoDB::globaltimersensor->schedule();

  // Force correct time on first getValue() from "realTime" field.
  SoDB::updateRealTimeFieldCB(NULL, NULL);

  SoDB::isinitialized = TRUE;
}

#if 0 // FIXME: re-code to be run automatically upon exit. 19991106 mortene.
/*!
  \internal

  This will clean up all memory and objects which has been allocated
  as persistent data. This isn't really necessary in user application,
  but is used for debugging purposes -- it makes it easier to find
  memory leaks.

  \sa init()
*/
void
SoDB::clean(void)
{
  // FIXME: attach this to the ANSI C "on exit" hook. Ditto for the
  // clean methods elsewhere. 19991106 mortene.

  delete SoDB::globaltimersensor; SoDB::globaltimersensor = NULL;
}
#endif // re-code

/*!
  Returns a text string containing the name of the library and version
  information.
*/
const char *
SoDB::getVersion(void)
{
  static SbString s;
  s = "SIM Coin ";
  s += COIN_VERSION;
  return s.getString();
}

/*!
  TODO: doc
*/
SbBool
SoDB::read(SoInput * /* in */, SoPath *& /* path */)
{
  COIN_STUB();
  return FALSE;
}

/*!
  Instantiates and reads an object of type SoBase from \a in and returns
  a pointer to it in \a base.

  The reference count of the base object will initially be zero.

  Returns \a FALSE on error.
*/
SbBool
SoDB::read(SoInput * in, SoBase *& base)
{
  if (!in->isValidFile()) return FALSE;
  return SoBase::read(in, base, SoBase::getClassTypeId()) && base;
}

/*!
  Instantiates and reads an object of type SoNode from \a in and returns
  a pointer to it in \a base.

  The reference count of the node will initially be zero.

  Returns \a FALSE on error. Returns \a TRUE with \a rootNode equal to
  \a NULL if we hit end of file instead of a new node specification in
  the file.
 */
SbBool
SoDB::read(SoInput * in, SoNode *& rootNode)
{
  SoBase * baseptr;
  SbBool result = SoDB::read(in, baseptr);
  if (result && baseptr) {
    if (baseptr->isOfType(SoNode::getClassTypeId())) {
      rootNode = (SoNode *)baseptr;
    }
    else {
      baseptr->ref();
      baseptr->unref();
      result = FALSE;
    }
  }
  return result;
}

/*!
  Reads all graphs from \a in and returns them under an SoSeparator node. If
  the file contains only a single graph under an SoSeparator node (which is
  the most common way of constructing and exporting scene graphs), no extra
  SoSeparator node will be made.

  The reference count of the root separator returned from this method will
  be zero. Other nodes in the returned scene graph will have reference counts
  according to the number of parent-child relationsships, as usual.

  Returns \a NULL on any error.
 */
SoSeparator *
SoDB::readAll(SoInput * in)
{
#if 1 // New code, designed to work better with binary files. 19990711 mortene.

  // If this check wasn't present, we'd never return NULL.
  if (!in->isValidFile()) return NULL;

#if COIN_DEBUG // See comments below in next COIN_DEBUG block.
  int stackdepth = in->filestack.getLength();
#endif // COIN_DEBUG

  SoSeparator * root = new SoSeparator;
  root->ref();

  SoNode * topnode = NULL;
  while (SoDB::read(in, topnode) && topnode) {
    root->addChild(topnode);
    topnode = NULL;
  }

  // FIXME: check that we are at EOF. 19990711 mortene.

  SoSeparator * retnode;
  if ((root->getNumChildren() == 1) &&
      (root->getChild(0)->isOfType(SoSeparator::getClassTypeId()))) {
    retnode = (SoSeparator *)root->getChild(0);
    retnode->ref();
    root->unref();
    retnode->unrefNoDelete();
  }
  else {
    retnode = root;
    root->unrefNoDelete();
  }

#if COIN_DEBUG
  // Detect problems with missing pops from the SoInput file stack.
  assert(stackdepth == in->filestack.getLength());
#endif // COIN_DEBUG

  return retnode;

#else // Old code
  if (!in->isValidFile()) return NULL;

  // FIXME: read paths aswell. 19990403 mortene.

  SoSeparator * root = NULL;
  SbBool result = FALSE;

  if (in->isValidFile()) {
    root = new SoSeparator;
    // This doesn't work anymore (moved readChildren() from public to
    // protected in SoGroup). 19991214 mortene.
    result = root->readChildren(in);
  }

  if (result && !in->eof()) {
    if (in->isBinary()) {
      SoReadError::post(in, "Extra characters found in input");
    }
    else {
      char c;
      in->read(c);
      SoReadError::post(in, "Extra characters ('%c') found in input", c);
    }

    // FIXME: return NULL? 19990403 mortene.
  }

  if (!result) {
    if (root) {
      root->ref();
      root->unref();
      root = NULL;
    }
  }
  else {
    if (root->getNumChildren() == 1 &&
        root->getChild(0)->isOfType(SoSeparator::getClassTypeId())) {
      SoSeparator * child = (SoSeparator *)(root->getChild(0));
      child->ref();
      root->ref();
      root->unref();
      child->unrefNoDelete();
      root = child;
    }
  }

  // FIXME: this should really be done implicit in SoInput upon
  // hitting EOF. 19990708 mortene.
  in->popFile();
  return root;
#endif
}

/*!
  Check if \a testString is a valid file format header identifier string.

  \sa getHeaderData(), registerHeader()
 */
SbBool
SoDB::isValidHeader(const char * testString)
{
  SbBool isBinary;
  float ivVersion;
  SoDBHeaderCB * preCB, * postCB;
  void * userdata;

#if COIN_DEBUG
  if (!testString) {
    SoDebugError::postWarning("SoDB::isValidHeader",
                              "Passed a NULL string pointer.");
    return FALSE;
  }
#endif // COIN_DEBUG

  return SoDB::getHeaderData(SbString(testString), isBinary, ivVersion,
                             preCB, postCB, userdata, TRUE);
}

/*!
  Register a header string which should be recognized by SoInput upon
  file import. This is a convenient way for library users to register
  their own VRML or Coin derived file formats.

  Set \a isBinary to TRUE if the file should be read as binary data, and
  set \a ivVersion to indicate which Coin library version is
  needed to read the file.

  Callbacks \a preCB and \a postCB will be called before and after
  importing the custom format.

  If \a headerString can not be accepted as a valid file format header
  for Coin files, \a FALSE will be returned. A valid header \e must
  start with a '#' character, and not be more than 80 characters long.

  \sa getHeaderData()
 */
SbBool
SoDB::registerHeader(const SbString & headerString,
                     SbBool isBinary, float ivVersion,
                     SoDBHeaderCB * preCB, SoDBHeaderCB * postCB,
                     void * userdata)
{
  // Must start with '#'.
  if ((headerString.getLength() == 0) || (headerString.getString()[0] != '#')) {
#if COIN_DEBUG
    SoDebugError::postWarning("SoDB::registerHeader",
                              "File header string must begin with '#', '%s'"
                              " invalid.", headerString.getString());
#endif // COIN_DEBUG
    return FALSE;
  }

  // No more than 80 characters.
  if (headerString.getLength() > 80) {
#if COIN_DEBUG
    SoDebugError::postWarning("SoDB::registerHeader",
                              "File header string must be 80 characters "
                              "or less, so '%s' is invalid.",
                              headerString.getString());
#endif // COIN_DEBUG
    return FALSE;
  }

  SoDB_HeaderInfo * newheader = new SoDB_HeaderInfo(headerString, isBinary,
                                                   ivVersion, preCB, postCB,
                                                   userdata);
  SoDB::headerlist.append(newheader);
  return TRUE;
}

/*!
  Returns the settings for the given \a headerString, if \a headerString
  is a valid header.

  If \a substringOK is \a TRUE, ignore trailing characters in \a headerString
  when checking for validity.

  If no valid header string by this name is found, \a FALSE is returned,
  otherwise \a TRUE will be returned, and the other input arguments will
  be set to their respective values.

  \sa isValidHeader(), registerHeader()
 */
SbBool
SoDB::getHeaderData(const SbString & headerString, SbBool & isBinary,
                    float & ivVersion, SoDBHeaderCB *& preCB,
                    SoDBHeaderCB *& postCB, void *& userdata,
                    SbBool substringOK)
{
  unsigned int hslen = headerString.getLength();
  if (hslen == 0) return FALSE;

  // Disregard whitespace.
  while ((headerString[hslen-1] == ' ') || (headerString[hslen-1] == '\t')) {
    hslen--;
    if (hslen == 0) return FALSE;
  }

  SbString tryheader = headerString.getSubString(0, hslen-1);

  SbBool hit = FALSE;
  for (int i=0; (i < SoDB::getNumHeaders()) && !hit; i++) {
    SoDB_HeaderInfo * hi = SoDB::headerlist[i];
    SbString & s = hi->headerstring;
    unsigned int reglen = s.getLength();
    assert(reglen > 0);
    // Disregard whitespace.
    while ((s[reglen-1] == ' ') || (s[reglen-1] == '\t')) reglen--;
    assert(reglen > 0);

    SbString regheader = s.getSubString(0, reglen-1);

    if (regheader == tryheader)
      hit = TRUE;
    else if (substringOK &&
             (hslen > reglen) &&
             (regheader == tryheader.getSubString(0, reglen - 1)))
      hit = TRUE;

    if (hit) {
      isBinary = hi->isBinary;
      ivVersion = hi->ivVersion;
      preCB = hi->preCB;
      postCB = hi->postCB;
      userdata = hi->userdata;
    }
  }

  return hit;
}

/*!
  Returns number of registered file headers.

  \sa getHeaderString()
 */
int
SoDB::getNumHeaders(void)
{
  return SoDB::headerlist.getLength();
}

/*!
  Returns the identifier header string of index \a i.

  \sa getNumHeaders(), getHeaderData()
 */
SbString
SoDB::getHeaderString(const int i)
{
#if COIN_DEBUG
  if ((i < 0) || (i >= SoDB::headerlist.getLength())) {
    SoDebugError::post("SoDB::getHeaderString", "Index %d out of range.", i);
    return SbString("");
  }
#endif // COIN_DEBUG

  return SoDB::headerlist[i]->headerstring;
}

/*!
  Create a new global field by the given \a type, and identified in
  subsequent accesses to getGlobalField() by \a name. If a global
  field by the name and type already exists, returns a pointer to it.
  If a global field with the same name but a different type exists,
  returns \a NULL.

  \sa getGlobalField(), renameGlobalField()
 */
SoField *
SoDB::createGlobalField(const SbName & name, SoType type)
{
  SoField * f = SoDB::getGlobalField(name);
  if (f) {
    if (f->getTypeId() == type) return f;
    else return NULL;
  }
  else {
#if COIN_DEBUG
    if (!type.canCreateInstance()) {
      SoDebugError::postWarning("SoDB::createGlobalField",
                                "Can't create instance of given type.");
      return NULL;
    }
#endif // COIN_DEBUG
    f = (SoField *)type.createInstance();
    SoDB::fieldlist.append(f);
    SoDB::fieldnamelist.append(name);
    return f;
  }
}

/*!
  If there exist a global field with the given \a name, return a
  pointer to it. If there is no field with this name, return \a NULL.

  \sa createGlobalField(), renameGlobalField()
 */
SoField *
SoDB::getGlobalField(const SbName & name)
{
  int i = SoDB::fieldnamelist.find(name);
  if (i != -1) return SoDB::fieldlist[i];
  return NULL;
}

/*!
  Rename a global field. If \a newName is an empty name, the \a oldName
  field gets deleted. If another global field already goes by the
  name \a newName, that field will get deleted before the rename
  operation.

  \sa getGlobalField(), createGlobalField()
 */
void
SoDB::renameGlobalField(const SbName & oldName, const SbName & newName)
{
  int idx = SoDB::fieldnamelist.find(oldName);
#if COIN_DEBUG
  if (idx == -1) {
    SoDebugError::postWarning("SoDB::renameGlobalField",
                              "Couldn't find global field '%s' to rename.",
                              oldName.getString());
    return;
  }
#endif // COIN_DEBUG

  if (newName == "") { // Remove field.
    delete SoDB::fieldlist[idx];
    SoDB::fieldlist.remove(idx);
    SoDB::fieldnamelist.remove(idx);
  }
  else {
    // Existing entry by same name? If so, remove it.
    int nidx = SoDB::fieldnamelist.find(newName);
    if (nidx != -1) {
      delete SoDB::fieldlist[nidx];
      SoDB::fieldlist.remove(nidx);
      SoDB::fieldnamelist.remove(nidx);
    }

    ((SbName &)(SoDB::fieldnamelist[idx])) = newName;
  }
}

/*!
  \internal

  This is the timer sensor callback which updates the realTime global
  field.
*/
void
SoDB::updateRealTimeFieldCB(void * /* data */, SoSensor * /* sensor */)
{
  SoField * f = SoDB::getGlobalField("realTime");
  if (f && (f->getTypeId() == SoSFTime::getClassTypeId())) {
    ((SoSFTime *)f)->setValue(SbTime::getTimeOfDay());
  }
}

/*!
  Set the time interval between updates to the \a realTime global field.
  Default value is 1/12 s.

  \sa getRealTimeInterval(), getGlobalField()
 */
void
SoDB::setRealTimeInterval(const SbTime & deltaT)
{
#if COIN_DEBUG
  if (deltaT < SbTime(0.0)) {
    SoDebugError::postWarning("SoDB::setRealTimeInterval",
                              "Tried to set negative interval.");
    return;
  }
#endif // COIN_DEBUG

  SbBool isscheduled = SoDB::globaltimersensor->isScheduled();
  if (isscheduled) SoDB::globaltimersensor->unschedule();
  if (deltaT != SbTime(0.0)) {
    SoDB::globaltimersensor->setInterval(deltaT);
    if (isscheduled) SoDB::globaltimersensor->schedule();
  }

  SoDB::realtimeinterval = deltaT;
}

/*!
  Returns the current trigger interval for the global \a realTime SbTime
  field.

  \sa setRealTimeInterval(), getGlobalField()
 */
const SbTime &
SoDB::getRealTimeInterval(void)
{
  return SoDB::realtimeinterval;
}

/*!
  This is just a wrapper for the method in SoSensorManager by the
  same name.

  \sa getDelaySensorTimeout(), SoSensorManager
 */
void
SoDB::setDelaySensorTimeout(const SbTime & t)
{
  SoDB::getSensorManager()->setDelaySensorTimeout(t);
}

/*!
  This is just a wrapper for the method in SoSensorManager by the
  same name.

  \sa setDelaySensorTimeout(), SoSensorManager
 */
const SbTime &
SoDB::getDelaySensorTimeout(void)
{
  return SoDB::getSensorManager()->getDelaySensorTimeout();
}

/*!
  \internal

  Returns a pointer to the global sensor manager. The sensor manager keeps
  track of the sensor queues.
 */
SoSensorManager *
SoDB::getSensorManager(void)
{
  return &(SoDB::sensormanager);
}

/*!
  This is a wrapper around the POSIX \a select() call. It is provided
  so you can do synchronous I/O while Coin continous to handle
  sensor events, rendering, etc. The parameters are the same as for
  \a select(), so check your system documentation on how to use them.
 */
int
SoDB::doSelect(int nfds, fd_set * readfds, fd_set * writefds,
               fd_set * exceptfds, struct timeval * userTimeOut)
{
  // TODO: need to do eventhandling for sensors etc. Check
  // SoSensorManager::doSelect(). Should we just call that method?
  // 19990425 mortene.

#ifdef __BEOS__
  COIN_STUB();
  return 0;
#else // !__BEOS__
  return select(nfds, readfds, writefds, exceptfds, userTimeOut);
#endif // __BEOS__
}

/*!
  TODO: doc
 */
void
SoDB::addConverter(SoType fromType, SoType toType, SoType converterType)
{
  unsigned long val=fromType.getKey() * 65536 + toType.getKey();
  converters.enter(val, (void *)converterType.getKey());
  //FIXME: Check output => warning? kintel
}

/*!
  TODO: doc
 */
SoFieldConverter *
SoDB::createConverter(SoType fromType, SoType toType)
{
  void * type;
  unsigned long val=fromType.getKey() * 65536 + toType.getKey();
  if (converters.find(val, type)) {
    uint16_t key = (uint16_t)((uint32_t)type);
    SoFieldConverter * conv;
    if (SoType::fromKey(key)==SoConvertAll::getClassTypeId())
      conv=new SoConvertAll(fromType, toType);
    else
      conv=(SoFieldConverter *)SoType::fromKey(key).createInstance();
    return conv;
  }
  return NULL;
}

/*!
  TODO: doc
 */
SoVRMLGroup *
SoDB::readAllVRML(SoInput * /* in */)
{
  COIN_STUB();
  return NULL;
}

/*!
  TODO: doc
 */
void
SoDB::createRoute(SoNode * /* fromnode */, const char * /* eventout */,
                  SoNode * /* tonode */, const char * /* eventin */)
{
  COIN_STUB();
}

/*!
  Returns \a TRUE if init() has been called.

  \sa init()
 */
SbBool
SoDB::isInitialized(void)
{
  return SoDB::isinitialized;
}

/*!
  \internal
 */
void
SoDB::startNotify(void)
{
  SoDB::notificationcounter++;
}

/*!
  \internal
 */
SbBool
SoDB::isNotifying(void)
{
  return SoDB::notificationcounter > 0;
}

/*!
  \internal
 */
void
SoDB::endNotify(void)
{
  SoDB::notificationcounter--;
  assert(SoDB::notificationcounter >= 0);

  // FIXME: there's probably something which should be done here when
  // the counter goes down to 0..? 19990530 mortene.
}

/*!
  Turn on or off the realtime sensor. Be careful with turning it
  off (defaut is on), as this may have unwanted side effects which are
  difficult to predict.

  \sa setRealTimeInterval()
*/
void
SoDB::enableRealTimeSensor(SbBool on)
{
  assert(SoDB::isInitialized());

  SbBool isscheduled = SoDB::globaltimersensor->isScheduled();
  if (isscheduled && !on) SoDB::globaltimersensor->unschedule();
  else if (!isscheduled && on) SoDB::globaltimersensor->schedule();
#if COIN_DEBUG
  else SoDebugError::postWarning("SoDB::enableRealTimeSensor",
                                 "realtime sensor already %s",
                                 on ? "on" : "off");
#endif // COIN_DEBUG
}
