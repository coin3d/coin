/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
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
  \mainpage

  Coin is an OpenGL based, retained mode 3D graphics rendering
  library. It is implemented in C++ and publicly released with the
  source code open for your perusal. The application programmer's
  interface (API) is based on (and will eventually be fully compatible
  with) the API of Open Inventor, the \e de \e facto standard 3D
  graphics library for complex visualization applications.

  Systems in Motion is working on providing libraries for interfacing
  Coin with a wide range of windowing systems and GUI toolkits. The
  library for interfacing against X11 through Xt/Motif is nearing
  completion, and the SoQt library for interfacing to X11 or MSWin
  systems through the Qt toolkit is also fully usable. Other libraries
  in development includes SoWin (interfacing Coin directly with the
  Win32 API), SoGtk (gkt+) and SoBe (BeOS).

  See <http://www.coin3d.org> for more information about Coin and the
  SoXt, SoQt and other GUI glue libraries.
 */

/*!
  \class SoDB SoDB.h Inventor/SoDB.h
  \brief The SoDB class keeps track of internal global data.
  \ingroup general

  This class collects various methods for initializing, setting and
  accessing common global data from the Coin library.

  All methods on SoDB are static.

  Make sure you call SoDB::init() (either directly or indirectly
  through the init() method of the GUI glue library) before you use
  any of the other Coin classes.
*/

#include <Inventor/SoDB.h>

#include <Inventor/SoInput.h>
#include <Inventor/actions/SoAction.h>
#include <Inventor/details/SoDetail.h>
#include <Inventor/elements/SoElement.h>
#include <Inventor/engines/SoEngine.h>
#include <Inventor/errors/SoReadError.h>
#include <Inventor/events/SoEvent.h>
#include <Inventor/fields/SoGlobalField.h>
#include <Inventor/fields/SoSFTime.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/sensors/SoTimerSensor.h>
#include <coindefs.h> // COIN_STUB()
#include <stdlib.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#include <assert.h>
#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#if HAVE_WINDOWS_H
#include <windows.h>
#endif // HAVE_WINDOWS_H

static SbString * coin_versionstring = NULL;

// atexit callback
static void cleanup_func(void)
{
  delete coin_versionstring;
}

// *************************************************************************

class SoDB_HeaderInfo {
public:
  SoDB_HeaderInfo(const SbString & hs, const SbBool bin, const float ver,
                  SoDBHeaderCB * pre, SoDBHeaderCB * post, void * ud)
    : headerstring(hs), isbinary(bin), ivversion(ver),
        preload_cb(pre), postload_cb(post), userdata(ud)
    { }

  SbString headerstring;
  SbBool isbinary;
  float ivversion;
  SoDBHeaderCB * preload_cb, * postload_cb;
  void * userdata;
};

// *************************************************************************


// OBSOLETED: this code was only active for GCC 2.7.x, and I don't
// think we support that old compiler version anyhow. Do look into if
// this is what the old SGI MIPSpro CC compiler for IRIX6.2 needs to
// stop spitting out all those linker warnings, though. 20000208 mortene.
#if 0 // obsoleted
// #if defined(NEED_TEMPLATE_DEFINITION)
template class SbList<SoDB_HeaderInfo *>;
template class SbList<SbName>;
template class SbList<SoField *>;
// [...]
#endif // obsoleted


SbList<SoDB_HeaderInfo *> * SoDB::headerlist = NULL;
SoSensorManager * SoDB::sensormanager = NULL;
SbTime * SoDB::realtimeinterval = NULL;
SoTimerSensor * SoDB::globaltimersensor = NULL;
SbDict * SoDB::converters = NULL;
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
  if (SoDB::isInitialized()) return;

  // Sanity check: if anything here breaks,
  // include/Inventor/system/inttypes.h needs fixing. Keep these
  // asserts around.

  assert(sizeof(uint16_t) == 2);
  assert(sizeof(int16_t) == 2);
  assert(sizeof(uint32_t) == 4);
  assert(sizeof(int32_t) == 4);

  // Sanity check: if the int type is not equal to 32 bits everything
  // probably goes to hell. FIXME: remove this check when we are no
  // longer dependent on using native C types where we need to have a
  // particular bitwidth.

  // TMP OBSOLETED for 1.0 release. We should be 99% ok, I can only
  // think of possibilities for problems in the binary .iv import and
  // export code. 20010308 mortene.

//    assert(sizeof(int) == 4);


  // Sanity check: if this breaks, the binary format import and export
  // routines will not work correctly. FIXME: the code should be fixed
  // to use the int16_t type, then we can remove this stoopid check.

  // TMP OBSOLETED for 1.0 release. We should be 99% ok, I can only
  // think of possibilities for problems in the binary .iv import and
  // export code. 20010308 mortene.

//    assert(sizeof(short) == 2);


  // Sanity check: if the int type is unequal to the long type, things
  // will probably break. FIXME: this is really terrible and _must_ be
  // fixed. Remove this check when we are no longer dependent on using
  // native C types where we need to have a particular bitwidth.

  // TMP OBSOLETED for 1.0 release. We should be 99% ok, I can only
  // think of possibilities for problems in the binary .iv import and
  // export code. 20010308 mortene.

//    assert(sizeof(int) == sizeof(long));


  // Allocate our static members.
  SoDB::headerlist = new SbList<SoDB_HeaderInfo *>;
  SoDB::sensormanager = new SoSensorManager;
  SoDB::realtimeinterval = new SbTime;
  SoDB::converters = new SbDict;



  // NB! There are dependencies in the order of initialization of
  // components below.

  // This obviously needs to be done first.
  SoType::init();

  SoError::initClasses();
  SoInput::init();
  SoBase::initClass();
  SoDetail::initClass();
  // SoPath inherits SoBase, so initialize it after SoBase.
  SoPath::initClass();
  SoFieldContainer::initClass();
  SoGlobalField::initClass(); // depends on SoFieldContainer init
  SoField::initClass();
  // Elements must be initialized before actions.
  SoElement::initClass();
  // Actions must be initialized before nodes (because of SO_ENABLE)
  SoAction::initClass();
  SoNode::initClass();
  SoEngine::initClass();
  SoEvent::initClass();
  SoSensor::initClass();

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

  SoDB::realtimeinterval->setValue(1.0/12.0);

  SoDB::createGlobalField("realTime", SoSFTime::getClassTypeId());

  SoDB::globaltimersensor = new SoTimerSensor;
  SoDB::globaltimersensor->setFunction(SoDB::updateRealTimeFieldCB);
  SoDB::globaltimersensor->setInterval(*SoDB::realtimeinterval);
  // FIXME: it would be better to not schedule unless something
  // actually attaches itself to the realtime field, or does this muck
  // up the code too much? 19990225 mortene.
  SoDB::globaltimersensor->schedule();

  // Force correct time on first getValue() from "realTime" field.
  SoDB::updateRealTimeFieldCB(NULL, NULL);

  SoDB::isinitialized = TRUE;

#if COIN_DEBUG
  // Debugging for memory leaks will be easier if we can clean up the
  // resource usage. This needs to be done last in init(), so we get
  // called _before_ clean() methods in other classes.
  (void)atexit(SoDB::clean);
#endif // COIN_DEBUG
}

// This will free all resources which has been allocated by the SoDB
// class. This isn't really necessary in user applications (the
// underlying operating system will take care of making the resources
// available again), but it is useful for debugging purposes -- it
// makes it easier to find memory leaks.
void
SoDB::clean(void)
{
#if COIN_DEBUG
  // Avoid having the SoSensorManager instance trigging the callback
  // into the So@Gui@ class -- not only have it possible "died", but
  // the whole GUI toolkit could have died until we come here.
  //
  // (This has already proven itself to be a source of problems with
  // the SoQt library, which wets its pants on the
  // SoDB::globaltimersensor destruction under MSWindows if we don't
  // first nullify the callback function pointer.)
  SoDB::sensormanager->setChangedCallback(NULL, NULL);

  delete SoDB::globaltimersensor;

  delete SoDB::converters;
  delete SoDB::realtimeinterval;
  delete SoDB::sensormanager;

  for (int i = 0; i < SoDB::headerlist->getLength(); i++)
    delete (*SoDB::headerlist)[i];
  delete SoDB::headerlist;
#endif // COIN_DEBUG
}

/*!
  Returns a text string containing the name of the library and version
  information.
*/
const char *
SoDB::getVersion(void)
{
  if (coin_versionstring == NULL) {
    coin_versionstring = new SbString;
    atexit(cleanup_func);
  }
  *coin_versionstring = "SIM Coin ";
  *coin_versionstring += COIN_VERSION;
  return coin_versionstring->getString();
}

/*!
  Instantiates and reads an SoPath object from \a in and returns a
  pointer to it in \a path.

  The reference count of the SoPath object will initially be zero.

  Returns \c FALSE on error. Returns \c TRUE with \a path equal to \a
  NULL if we hit end of file instead of a new path specification in
  the file.
*/
SbBool
SoDB::read(SoInput * in, SoPath *& path)
{
  path = NULL;
  SoBase * baseptr;
  if (!SoDB::read(in, baseptr)) return FALSE;
  if (!baseptr) return TRUE; // eof

  if (!baseptr->isOfType(SoPath::getClassTypeId())) {
    SoReadError::post(in, "'%s' not derived from SoPath",
                      baseptr->getTypeId().getName().getString());
    baseptr->ref();
    baseptr->unref();
    return FALSE;
  }

  path = (SoPath *)baseptr;
  return TRUE;
}

/*!
  Instantiates and reads an object of type SoBase from \a in and
  returns a pointer to it in \a base. \a base will be \c NULL on
  return if we hit end of file.

  The reference count of the base object will initially be zero.

  Returns \c FALSE on error.
*/
SbBool
SoDB::read(SoInput * in, SoBase *& base)
{
  // Header is only required when reading from a stream, if reading from
  // memory no header is required
  if (!in->isValidFile()) return FALSE;
  return SoBase::read(in, base, SoBase::getClassTypeId());
}

/*!
  Instantiates and reads an object of type SoNode from \a in and returns
  a pointer to it in \a rootnode.

  The reference count of the node will initially be zero.

  Returns \c FALSE on error. Returns \c TRUE with \a rootnode equal to
  \c NULL if we hit end of file instead of a new node specification in
  the file.
 */
SbBool
SoDB::read(SoInput * in, SoNode *& rootnode)
{
  rootnode = NULL;
  SoBase * baseptr;
  if (!SoDB::read(in, baseptr)) return FALSE;
  if (!baseptr) return TRUE; // eof

  if (!baseptr->isOfType(SoNode::getClassTypeId())) {
    SoReadError::post(in, "'%s' not derived from SoNode",
                      baseptr->getTypeId().getName().getString());
    baseptr->ref();
    baseptr->unref();
    return FALSE;
  }

  rootnode = (SoNode *)baseptr;
  return TRUE;
}

/*!
  Reads all graphs from \a in and returns them under an SoSeparator node. If
  the file contains only a single graph under an SoSeparator node (which is
  the most common way of constructing and exporting scene graphs), no extra
  SoSeparator node will be made.

  The reference count of the root separator returned from this method will
  be zero. Other nodes in the returned scene graph will have reference counts
  according to the number of parent-child relationsships, as usual.

  Returns \c NULL on any error.
 */
SoSeparator *
SoDB::readAll(SoInput * in)
{
  if (!in->isValidFile()) {
    SoReadError::post(in, "Not a valid Inventor file.");
    return NULL;
  }

#if COIN_DEBUG // See comments below in next COIN_DEBUG block.
  int stackdepth = in->filestack.getLength();
#endif // COIN_DEBUG

  SoSeparator * root = new SoSeparator;
  root->ref();

  SoNode * topnode;
  do {
    if (!SoDB::read(in, topnode)) {
      root->unref();
      return FALSE;
    }
    if (topnode) root->addChild(topnode);
  } while (topnode && in->skipWhiteSpace());

  SoSeparator * retnode;
  if ((root->getNumChildren() == 1) &&
      (root->getChild(0)->isOfType(SoSeparator::getClassTypeId()))) {
    retnode = (SoSeparator *)root->getChild(0);
    retnode->ref();
    root->unref();
  }
  else {
    retnode = root;
  }

#if COIN_DEBUG
  // Detect problems with missing pops from the SoInput file stack.
  assert(stackdepth == in->filestack.getLength());
#endif // COIN_DEBUG
  if (in->filestack.getLength() == 1) in->popFile(); // force a header post callback

  assert(retnode->getRefCount() == 1);
  retnode->unrefNoDelete(); // return with a zero refcount
  return retnode;
}

/*!
  Check if \a testString is a valid file format header identifier string.

  \sa getHeaderData(), registerHeader()
 */
SbBool
SoDB::isValidHeader(const char * teststring)
{
  SbBool isbinary;
  float ivversion;
  SoDBHeaderCB * preload_cb, * postload_cb;
  void * userdata;

#if COIN_DEBUG
  if (!teststring) {
    SoDebugError::postWarning("SoDB::isValidHeader",
                              "Passed a NULL string pointer.");
    return FALSE;
  }
#endif // COIN_DEBUG

  return SoDB::getHeaderData(SbString(teststring), isbinary, ivversion,
                             preload_cb, postload_cb, userdata, TRUE);
}

/*!
  Register a header string which should be recognized by SoInput upon
  file import. This is a convenient way for library users to register
  their own VRML or Coin derived file formats.

  Set \a isbinary to \c TRUE if the file should be read as binary
  data, and set \a ivversion to indicate which Coin library version is
  needed to read the file.

  Callbacks \a precallback and \a postcallback will be called before
  and after importing the custom format.

  If \a headerstring can not be accepted as a valid file format header
  for Coin files, \c FALSE will be returned. A valid header \e must
  start with a '#' character, and not be more than 80 characters long.

  \sa getHeaderData()
*/
SbBool
SoDB::registerHeader(const SbString & headerstring,
                     SbBool isbinary, float ivversion,
                     SoDBHeaderCB * precallback, SoDBHeaderCB * postcallback,
                     void * userdata)
{
  // Must start with '#'.
  if ((headerstring.getLength() == 0) ||
      (headerstring.getString()[0] != '#')) {
#if COIN_DEBUG
    SoDebugError::postWarning("SoDB::registerHeader",
                              "File header string must begin with '#', '%s'"
                              " invalid.", headerstring.getString());
#endif // COIN_DEBUG
    return FALSE;
  }

  // No more than 80 characters.
  if (headerstring.getLength() > 80) {
#if COIN_DEBUG
    SoDebugError::postWarning("SoDB::registerHeader",
                              "File header string must be 80 characters "
                              "or less, so '%s' is invalid.",
                              headerstring.getString());
#endif // COIN_DEBUG
    return FALSE;
  }

  SoDB_HeaderInfo * newheader =
    new SoDB_HeaderInfo(headerstring, isbinary, ivversion,
                        precallback, postcallback, userdata);
  SoDB::headerlist->append(newheader);
  return TRUE;
}

/*!
  Returns the settings for the given \a headerstring, if \a headerstring
  is a valid header.

  If \a substringok is \c TRUE, ignore trailing characters in \a headerstring
  when checking for validity.

  If no valid header string by this name is found, \c FALSE is returned,
  otherwise \c TRUE will be returned, and the other input arguments will
  be set to their respective values.

  \sa isValidHeader(), registerHeader()
 */
SbBool
SoDB::getHeaderData(const SbString & headerstring, SbBool & isbinary,
                    float & ivversion, SoDBHeaderCB *& precallback,
                    SoDBHeaderCB *& postcallback, void *& userdata,
                    SbBool substringok)
{
  unsigned int hslen = headerstring.getLength();
  if (hslen == 0) return FALSE;

  // Disregard whitespace.
  while ((headerstring[hslen-1] == ' ') || (headerstring[hslen-1] == '\t')) {
    hslen--;
    if (hslen == 0) return FALSE;
  }

  SbString tryheader = headerstring.getSubString(0, hslen-1);

  SbBool hit = FALSE;
  for (int i=0; (i < SoDB::getNumHeaders()) && !hit; i++) {
    SoDB_HeaderInfo * hi = (*SoDB::headerlist)[i];
    SbString & s = hi->headerstring;
    unsigned int reglen = s.getLength();
    assert(reglen > 0);
    // Disregard whitespace.
    while ((s[reglen-1] == ' ') || (s[reglen-1] == '\t')) reglen--;
    assert(reglen > 0);

    SbString regheader = s.getSubString(0, reglen-1);

    if (regheader == tryheader)
      hit = TRUE;
    else if (substringok && (hslen > reglen) &&
             (regheader == tryheader.getSubString(0, reglen - 1)))
      hit = TRUE;

    if (hit) {
      isbinary = hi->isbinary;
      ivversion = hi->ivversion;
      precallback = hi->preload_cb;
      postcallback = hi->postload_cb;
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
  return SoDB::headerlist->getLength();
}

/*!
  Returns the identifier header string of index \a i.

  \sa getNumHeaders(), getHeaderData()
 */
SbString
SoDB::getHeaderString(const int i)
{
#if COIN_DEBUG
  if ((i < 0) || (i >= SoDB::headerlist->getLength())) {
    SoDebugError::post("SoDB::getHeaderString", "Index %d out of range.", i);
    return SbString("");
  }
#endif // COIN_DEBUG

  return (*SoDB::headerlist)[i]->headerstring;
}

/*!
  Create a new global field by the given \a type, and identified in
  subsequent accesses to getGlobalField() by \a name. If a global
  field by the name and type already exists, returns a pointer to it.
  If a global field with the same name but a different type exists,
  returns \c NULL.

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

#if COIN_DEBUG
  if (!type.canCreateInstance()) {
    SoDebugError::postWarning("SoDB::createGlobalField",
                              "Can't create instance of field type ``%s''.",
                              type.getName().getString());
    return NULL;
  }
#endif // COIN_DEBUG

  // (Deallocation of the field happens in the SoGlobalField
  // destructor.)
  SoField * newfield = (SoField *)type.createInstance();
  (void) new SoGlobalField(name, newfield);
  return newfield;
}

/*!
  If there exist a global field with the given \a name, return a
  pointer to it. If there is no field with this name, return \c NULL.

  \sa createGlobalField(), renameGlobalField()
*/
SoField *
SoDB::getGlobalField(const SbName & name)
{
  SoGlobalField * gf = SoGlobalField::getGlobalFieldContainer(name);
  return gf ? gf->getGlobalField() : NULL;
}

/*!
  Rename a global field. If \a to is an empty name, the \a from field
  gets deleted. If another global field already goes by the name \a
  to, that field will get deleted before the rename operation.

  \sa getGlobalField(), createGlobalField()
*/
void
SoDB::renameGlobalField(const SbName & from, const SbName & to)
{
  SoGlobalField * gf = SoGlobalField::getGlobalFieldContainer(from);

#if COIN_DEBUG
  if (gf == NULL) {
    SoDebugError::postWarning("SoDB::renameGlobalField",
                              "Couldn't find global field '%s' to rename.",
                              from.getString());
    return;
  }
#endif // COIN_DEBUG

  if (to == "") { // Remove field.
    SoGlobalField::removeGlobalFieldContainer(gf);
  }
  else {
    // Existing entry by same name? If so, remove it.
    SoGlobalField * old = SoGlobalField::getGlobalFieldContainer(to);
    if (old) SoGlobalField::removeGlobalFieldContainer(old);

    gf->setName(to);
  }
}

// This is the timer sensor callback which updates the realTime global
// field.
void
SoDB::updateRealTimeFieldCB(void * /* data */, SoSensor * /* sensor */)
{
  SoField * f = SoDB::getGlobalField("realTime");
  if (f && (f->getTypeId() == SoSFTime::getClassTypeId())) {
    ((SoSFTime *)f)->setValue(SbTime::getTimeOfDay());
  }
}

/*!
  Set the time interval between updates for the \c realTime global field
  to \a interval. Default value is 1/12 s.

  \sa getRealTimeInterval(), getGlobalField()
 */
void
SoDB::setRealTimeInterval(const SbTime & interval)
{
#if COIN_DEBUG
  if (interval < SbTime(0.0)) {
    SoDebugError::postWarning("SoDB::setRealTimeInterval",
                              "Tried to set negative interval.");
    return;
  }
#endif // COIN_DEBUG

  SbBool isscheduled = SoDB::globaltimersensor->isScheduled();
  if (isscheduled) SoDB::globaltimersensor->unschedule();
  if (interval != SbTime(0.0)) {
    SoDB::globaltimersensor->setInterval(interval);
    if (isscheduled) SoDB::globaltimersensor->schedule();
  }

  (*SoDB::realtimeinterval) = interval;
}

/*!
  Returns the current trigger interval for the global \a realTime SbTime
  field.

  \sa setRealTimeInterval(), getGlobalField()
 */
const SbTime &
SoDB::getRealTimeInterval(void)
{
  return *SoDB::realtimeinterval;
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
  Returns a pointer to the global sensor manager. The sensor manager keeps
  track of the sensor queues.
 */
SoSensorManager *
SoDB::getSensorManager(void)
{
  return SoDB::sensormanager;
}

/*!
  This is a wrapper around the POSIX \c select() call. It is provided
  so you can do synchronous I/O while Coin continous to handle sensor
  events, rendering, etc. The parameters are the same as for \c
  select(), so check your system documentation on how to use them.
 */
int
SoDB::doSelect(int nfds, fd_set * readfds, fd_set * writefds,
               fd_set * exceptfds, struct timeval * usertimeout)
{
  // FIXME: need to do eventhandling for sensors etc. Check
  // SoSensorManager::doSelect(). Should we just call that method?
  // 19990425 mortene.

  // FIXME: use configure for these checks. 20000130 mortene.
#ifdef __BEOS__
  COIN_STUB();
  return 0;
#else // !__BEOS__
  return select(nfds, readfds, writefds, exceptfds, usertimeout);
#endif // __BEOS__
}

/*!
  Notify SoDB that there exists a way to convert data from the \a from
  SoField type to the \a to SoField type, by connecting them with an
  instance of the \a converter SoFieldConverter type.

  By doing this, SoDB::getConverter() will later be able to
  automatically return the type of the correct conversion class when
  requested.

  Coin internally provides conversion between most field types, so
  application programmers should usually not need to use this
  function. The exception is if you are writing your own field type
  classes, and want to be able to connect them to the internal field
  types (or other extensions field types).

  \sa createConverter(), SoFieldConverter
 */
void
SoDB::addConverter(SoType from, SoType to, SoType converter)
{
  uint32_t val = (((uint32_t)from.getKey()) << 16) + to.getKey();
  SbBool nonexist = SoDB::converters->enter(val, (void *)converter.getKey());
  if (!nonexist) {
#if COIN_DEBUG
    SoDebugError::postWarning("SoDB::addConverter",
                              "Conversion from ``%s'' to ``%s'' is already "
                              "handled by instances of ``%s''",
                              from.getName().getString(),
                              to.getName().getString(),
                              converter.getName().getString());
#endif // COIN_DEBUG
  }
}

/*!
  Return the type of an SoFieldConverter class which is able to
  convert data between fields of type \a from to the data field(s) of
  field type \a to.

  If no conversion between the given field types is possible, returns
  SoType::badType().

  \sa addConverter()
 */
SoType
SoDB::getConverter(SoType from, SoType to)
{
  void * key;
  uint32_t val = (((uint32_t)from.getKey()) << 16) + to.getKey();
  // FIXME: ugly, need a better dict/hash class. 20000216 mortene.
  if (!SoDB::converters->find(val, key)) return SoType::badType();
  return SoType::fromKey((uint16_t)((uint32_t)key));
}

/*!
  Returns \c TRUE if init() has been called.

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
  if (SoDB::notificationcounter == 0) {
    // Process zero-priority sensors after notification has been done.
    SoSensorManager * sm = SoDB::getSensorManager();
    if (sm->isDelaySensorPending()) sm->processImmediateQueue();
  }
}

/*!
  Turn on or off the realtime sensor.

  The most common use for turning the realtime sensor off is to
  control the realTime global field from the user application. This is
  for instance handy when you want to take screen snapshots at fixed
  intervals.

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
