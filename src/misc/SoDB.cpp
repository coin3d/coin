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

/* *********************************************************************** */
/* Misc global Doxygen documentation stuff. ****************************** */

/*!
  \mainpage

  <a href="http://www.coin3d.org">Coin</a> is an OpenGL based,
  retained mode 3D graphics rendering library. It is implemented in
  C++ and publicly released with the source code open for your
  perusal. The application programmer's interface (API) is fully
  compatible with SGI's Open Inventor, the \e de \e facto standard 3D
  graphics API for complex visualization applications.

  For an excellently written, detailed, tutorial-style introduction to
  the Open Inventor API used by the Coin library, we highly recommend
  the book «The Inventor Mentor» (subtitle: «Programming
  Object-Oriented 3D Graphics with Open Inventor»), ISBN
  0-201-62495-8. It walks the Coin application programmer through all
  the principles applied in the API, richly illustrated and with
  numerous, well documented code examples.

  (The «The Inventor Mentor» is getting a bit old, but don't let that
  put you off. We heartily recommend this book as it covers all the
  principles, aswell as close to 100% of the API, in an excellent
  manner.)


  Systems in Motion is working on providing libraries for interfacing
  Coin with a wide range of windowing systems and GUI toolkits.
  <ol>
  <li><a href="http://doc.coin3d.org/SoXt">SoXt</a> is for interfacing with Xt/Motif on X Windows.</li>
  <li><a href="http://doc.coin3d.org/SoQt">SoQt</a> is for interfacing with Trolltech's cross-platform Qt toolkit
      (UNIX, Windows, Mac OS X).</li>
  <li><a href="http://doc.coin3d.org/SoWin">SoWin</a> is for interfacing with the Win32 API on Microsoft Windows
      platforms.</li>
  <li><a href="http://doc.coin3d.org/SoGtk">SoGtk</a> is for interfacing with GTK+.</li>
  </ol>

  See <http://www.coin3d.org/> for more information about Coin and the
  GUI toolkit libraries.

  <b>IMPORTANT NOTE: the online documentation for the Coin library is
  still a work-in-progress.</b> Although most classes have been
  documented properly, there are still a few "holes" in the
  documentation for certain classes. If you happen upon an
  undocumented or poorly documented class and / or class method which
  you find hard to understand, please give us a notice so we can
  rectify the situation.
*/

/*! \defgroup actions Actions */
/*! \defgroup base Base classes */
/*! \defgroup bundles Bundles (mostly for internal use) */
/*! \defgroup caches Cache storage (mostly for internal use) */
/*! \defgroup details Detail classes */
/*! \defgroup draggers Draggers */
/*! \defgroup elements Elements (mostly for internal use) */
/*! \defgroup engines Engine classes */
/*! \defgroup errors Error handling */
/*! \defgroup events Event classes */
/*! \defgroup fields Field classes */
/*! \defgroup general General classes */
/*! \defgroup manips Manipulators */
/*! \defgroup nodekits Node Kits */
/*! \defgroup nodes Nodes */
/*! \defgroup projectors Projector classes (mostly for internal use) */
/*! \defgroup sensors Sensor classes */
/*! \defgroup VRMLnodes VRML97 classes */

/*!
  \class SbBool SbBasic.h Inventor/SbBasic.h
  \brief SbBool is a compiler portable boolean type.
  \ingroup base

  SbBool is meant to be a "compiler portable" way of defining a
  boolean type, since there are older compilers out there which don't
  support the ISO-standard C++ \c bool keyword.

  SbBool is not really a class, just a \c typedef.
*/


/* *********************************************************************** */

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
#include <Inventor/misc/SoProto.h>
#include <Inventor/misc/SoProtoInstance.h>
#include <../tidbits.h> // coin_atexit()
#include <coindefs.h> // COIN_STUB()
#include <stdlib.h>

#include <Inventor/errors/SoDebugError.h>

#include <assert.h>
#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#if HAVE_WINDOWS_H
#include <windows.h>
#endif // HAVE_WINDOWS_H
#if HAVE_UNISTD_H
#include <unistd.h> // fd_set (?)
#endif // HAVE_UNISTD_H

#ifdef HAVE_VRML97
#include <Inventor/VRMLnodes/SoVRML.h>
#endif // HAVE_VRML97

#ifdef HAVE_THREADS
#include <Inventor/C/threads/threadp.h>
#endif // HAVE_THREADS

#ifdef COIN_THREADSAFE
#include <Inventor/threads/SbStorage.h>
static SbStorage * sodb_notificationcounter_storage = NULL;
#endif // COIN_THREADSAFE

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
SbBool SoDB::isinitialized = FALSE;
int SoDB::notificationcounter = 0;

#ifdef COIN_THREADSAFE
#define SODB_NOTIFICATIONCOUNTER *(((int*) sodb_notificationcounter_storage->get()))
#else // COIN_THREADSAFE
#define SODB_NOTIFICATIONCOUNTER SoDB::notificationcounter
#endif // ! COIN_THREADSAFE

#ifdef COIN_THREADSAFE
// callback from SbStorage that initializes the counter to 0
static void 
sodb_clear_counter(void * ptr)
{
  int * intptr = (int*) ptr;
  *intptr = 0;
}
#endif // COIN_THREADSAFE

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

  // Sanity check: if anything here breaks, either
  // include/Inventor/system/inttypes.h.in or the bitwidth define
  // configure tests need fixing. Keep these tests around.

  assert(sizeof(uint8_t) == 1);
  assert(sizeof(int8_t) == 1);
  assert(sizeof(uint16_t) == 2);
  assert(sizeof(int16_t) == 2);
  assert(sizeof(uint32_t) == 4);
  assert(sizeof(int32_t) == 4);
#ifdef HAVE_UINT64_T
  assert(sizeof(uint64_t) == 8);
#endif // HAVE_UINT64_T
#ifdef HAVE_INT64_T
  assert(sizeof(int64_t) == 8);
#endif // HAVE_INT64_T


  // Sanity check: if the int type is not equal to 32 bits everything
  // probably goes to hell. FIXME: remove this check when we are no
  // longer dependent on using native C types where we need to have a
  // particular bitwidth.

#if 0
  // TMP OBSOLETED assert for 1.0 release. We should be 99% ok, I can
  // only think of possibilities for problems in the binary .iv import
  // and export code. 20010308 mortene.
  assert(sizeof(int) == 4);
#else
  if (sizeof(int) != 4) {
    SoDebugError::postWarning("SoDB::init",
                              "sizeof(int) != 4 "
                              "(Coin not tested on this platform)");
  }
#endif

  // Sanity check: if the unsigned long type is less than the pointer
  // size (which could in theory happen on 64-bits machines), SbDict
  // usage will fail all over the place where we cast pointers to
  // SbDict keys. FIXME: remove this check when we are no longer
  // dependent on using native C types where we need to have a
  // particular bitwidth. 20020225 mortene.
  assert(sizeof(unsigned long) >= sizeof(void *));

  // Sanity check: if this breaks, the binary format import and export
  // routines will not work correctly. FIXME: the code should be fixed
  // to use the int16_t type, then we can remove this stoopid check.

#if 0
  // TMP OBSOLETED assert for 1.0 release. We should be 99% ok, I can
  // only think of possibilities for problems in the binary .iv import
  // and export code. 20010308 mortene.
  assert(sizeof(short) == 2);
#else
  if (sizeof(short) != 2) {
    SoDebugError::postWarning("SoDB::init",
                              "sizeof(short) != 2 "
                              "(Coin not tested on this platform)");
  }
#endif


  // Sanity check: if the int type is unequal to the long type, things
  // will probably break. FIXME: this is really terrible and _must_ be
  // fixed. Remove this check when we are no longer dependent on using
  // native C types where we need to have a particular bitwidth.

  // TMP OBSOLETED assert for 1.0 release. We should be 99% ok, I can
  // only think of possibilities for problems in the binary .iv import
  // and export code. 20010308 mortene.

#if 0
  assert(sizeof(int) == sizeof(long));
#else
  if (sizeof(int) != sizeof(long)) {
    SoDebugError::postWarning("SoDB::init",
                              "sizeof(int) != sizeof(long) "
                              "(Coin not tested on this platform)");
  }
#endif

#ifdef HAVE_THREADS
  // initialize thread system first
  cc_thread_init();
#ifdef COIN_THREADSAFE
  sodb_notificationcounter_storage = new SbStorage(sizeof(int), sodb_clear_counter, NULL);
#endif // COIN_THREADSAFE
#endif // HAVE_THREADS

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

  SoProto::initClass();
  SoProtoInstance::initClass();

#ifdef HAVE_VRML97
  so_vrml_init();
#endif // HAVE_VRML97

  // Register all valid file format headers.
  SoDB::registerHeader(SbString("#Inventor V2.1 ascii   "), FALSE, 2.1f,
                       NULL, NULL, NULL);
  SoDB::registerHeader(SbString("#Inventor V2.1 binary  "), TRUE, 2.1f,
                       NULL, NULL, NULL);

  SoDB::registerHeader(SbString("#VRML V2.0 utf8"), FALSE, 2.1f,
                       NULL, NULL, NULL);

  // FIXME: there are nodes in TGS' later Inventor versions that we do
  // not support, so it's not really correct to register 2.4 and 2.5
  // headers.  20010925 mortene.
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

  // FIXME: this is erroneous, we don't _really_ support v1.x Inventor
  // files.  Should perhaps just remove the following lines, but do it
  // as part of a larger renovation on the file-format support /
  // versioning stuff.  20010925 mortene.
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
  coin_atexit((coin_atexit_f *)SoDB::clean);
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
  
#ifdef COIN_THREADSAFE
  delete sodb_notificationcounter_storage;
#endif // COIN_THREADSAFE
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
    coin_atexit((coin_atexit_f *)cleanup_func);
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
  Reads all graphs from \a in and returns them under an SoSeparator
  node. If the file contains only a single graph under an SoSeparator
  node (which is the most common way of constructing and exporting
  scene graphs), no \e extra SoSeparator root node will be made, but
  the returned root node will be the top-most node from the file.

  The reference count of the root separator returned from this method
  will be zero. Other nodes in the returned scene graph will have
  reference counts according to the number of parent-child
  relationships, as usual.

  The common layout for how to load, work with and then finally
  destruct and return memory resources of scenegraphs usually goes
  like this:

  \code
  // [snip]
  SoSeparator * root = SoDB::readAll(&input); 
  if (!root) { exit(1); }

  // root-node returned from SoDB::readAll() has initial zero
  // ref-count, so reference it before we start using it to
  // avoid premature destruction.
  root->ref();

  // [do your thing here -- rendering of the scene, whatever]

  // Bring ref-count of root-node back to zero to cause the
  // destruction of the scene.
  root->unref();
  // (Upon self-destructing, the root-node will also de-reference
  // it's children nodes, so they also self-destruct, and so on
  // recursively down the scenegraph hierarchy until the complete
  // scenegraph has self-destructed and thereby returned all
  // memory resources it was using up.)
  \endcode

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
      return NULL;
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

  Of particular interest is the \e realTime global field set up by the
  library on initialization. This field is used as a source field to
  all the autonomous animation objects within the library, like for
  instance the SoTimeCounter engine or the SoRotor node. So if you
  want to control the speed of "action" within a (partly) animating
  scene, grab the global field named "realTime", call
  SoDB::enableRealTimeSensor(FALSE), and control the realTime field
  yourself.

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

  The void* arguments must be valid pointers to fd_set
  structures. We've changed this from the original SGI Inventor API to
  avoid messing up the header file with system-specific includes.
*/
int
SoDB::doSelect(int nfds, void * readfds, void * writefds,
               void * exceptfds, struct timeval * usertimeout)
{
  // FIXME: need to do eventhandling for sensors etc. Check
  // SoSensorManager::doSelect(). Should we just call that method?
  // 19990425 mortene.

  // FIXME: use configure for these checks. 20000130 mortene.
#ifdef __BEOS__
  COIN_STUB();
  return 0;
#else // !__BEOS__
  fd_set * rds = (fd_set *)readfds;
  fd_set * wds = (fd_set *)writefds;
  fd_set * eds = (fd_set *)exceptfds;
  // FIXME: the winsock select() call is most likely incorrectly used
  // here.  What should be done (on Win32) is for sockets to get
  // waitable event handles by using WSAEventSelect, and for
  // filehandles to get the handle (_get_osfhandle(fd)), and then use
  // MsgWaitForMultipleObjectsEx() for waiting.  For fds that aren't
  // sockets or filehandles; complain.
  //
  // Considering sockets at all should be determined on whether
  // LoadLibrary finds the wsock32.dll or not.
  //
  // Someone experienced in Win32-programming ought to check this out
  // and do proper testing (write a test-case) - I just picked up this
  // info from this discussion:
  // http://archive.develooper.com/perl-loop@perl.org/msg00398.html
  // 
  // 20011002 larsa.

  // FIXME update: I think the SoDB::doSelect() and
  // SoSensorManager::doSelect() calls are pretty lousy design anyway,
  // so perhaps we should just obsolete them.  The application
  // programmer should rather set up a timer himself and periodically
  // call into the sensor processing queues of Coin, like what is done
  // by the So* libraries. 20011115 mortene.
  return select(nfds, rds, wds, eds, usertimeout);
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
  uint32_t val = (((uint32_t)from.getKey()) << 16) + to.getKey();
  // FIXME: ugly, need a better dict/hash class. 20000216 mortene.
  void * key;
  if (!SoDB::converters->find(val, key)) return SoType::badType();
  // the extra intermediate "long" cast is needed by 64-bits IRIX CC
  return SoType::fromKey((uint16_t)((uint32_t)((long)key)));
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
  \COININTERNAL
 */
void
SoDB::startNotify(void)
{
  int & counter = SODB_NOTIFICATIONCOUNTER;
  counter++;
}

/*!
  \COININTERNAL
 */
SbBool
SoDB::isNotifying(void)
{
  int & counter = SODB_NOTIFICATIONCOUNTER;
  return counter > 0;
}

/*!
  \COININTERNAL
 */
void
SoDB::endNotify(void)
{
  int & counter = SODB_NOTIFICATIONCOUNTER;
  counter--;
  assert(counter >= 0);
  if (counter == 0) {
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
  intervals. See the class documentation of SoOffscreenRenderer for
  further information.

  \sa setRealTimeInterval(), getGlobalField()
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
