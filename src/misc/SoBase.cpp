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
  \class SoBase SoBase.h Inventor/misc/SoBase.h
  \brief The SoBase class is the top-level superclass for a number
  of class-hierarchies.
  \ingroup general

  SoBase provides the basic interfaces and methods for doing reference
  counting, type identification and import/export. All classes in Coin
  which uses these mechanisms are descendent from this class.

*/

/*¡
  There's a lot of methods in SoBase used to implement VRML support
  which are missing.

  One more thing missing: detect cases where we should instantiate
  SoUnknownEngine in addition to the SoUnknownNode cases.
*/

#include <Inventor/misc/SoBase.h>
#include <Inventor/SbName.h>
#include <Inventor/SbString.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/errors/SoReadError.h>
#include <Inventor/lists/SoBaseList.h>
#include <Inventor/nodes/SoUnknownNode.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#include <assert.h>

/*!
  \fn SoType SoBase::getTypeId(void) const
  Returns the actual type id of an object derived from a class inheriting
  SoBase. Needs to be overloaded in \e all subclasses.
*/
/*!
  \fn SbBool SoBase::readInstance(SoInput * in, unsigned short flags)

  Reads definition of SoBase instance from input stream \a in.
*/
/*!
  \enum SoBase::BaseFlags
  \internal
*/



// Strings and character tokens used in parsing.
static const char OPEN_BRACE = '{';
static const char CLOSE_BRACE = '}';
static const char END_OF_LINE[] = "\n";
static const char DEF_KEYWORD[] = "DEF";
static const char USE_KEYWORD[] = "USE";
static const char NULL_KEYWORD[] = "NULL";


// Only a small number of SoBase derived objects will under usual
// conditions have designated names, so we use a couple of static
// SbDict objects to keep track of them. Since we avoid storing a
// pointer for each and every object, we'll cut down on a decent
// amount of memory use this way (SoBase should be kept as slim as
// possible, as any dead weight is brought along in a lot of objects).
SbDict * SoBase::name2obj_dict; // maps from char * to SbPList(SoBase)
SbDict * SoBase::obj2name_dict; // maps from SoBase * to char *

SbString SoBase::refwriteprefix = "+";

SoType SoBase::classTypeId = SoType::badType();

SbBool SoBase::tracerefs = FALSE;
uint32_t SoBase::writecounter = 0;

/*!
  Constructor. The initial reference count will be set to zero.
 */
SoBase::SoBase(void)
{
  this->objdata.referencecount = 0;
  this->objdata.writerefcount = 0;
  this->objdata.multirefs = FALSE;
  this->objdata.ingraph = FALSE;
}

/*!
  Destructor. There should not be any normal circumstance where you need
  to explicitly delete an object derived from the SoBase class, as the
  reference counting should take care of deallocating unused objects.

  \sa unref(), unrefNoDelete()
 */
SoBase::~SoBase()
{
#if 0 // debug
  SoDebugError::postInfo("SoBase::~SoBase", "starting destructor: %p", this);
#endif // debug

  SbName n = this->getName();
  if (n.getLength()) this->removeName(this, n.getString());
}

/*!
  Commits suicide. Called when the reference count goes to zero.
 */
void
SoBase::destroy(void)
{
#if COIN_DEBUG
  if (SoBase::tracerefs) {
    SoDebugError::postInfo("SoBase::destroy",
                           "%p ('%s')",
                           this, this->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
  delete this;
}

/*!
  This need to be called before we start instantiating and using objects
  derived from SoBase. SoDB::init() will call this method automatically.
 */
void
SoBase::initClass(void)
{
  (void)atexit(SoBase::cleanClass);

  // Avoid multiple attempts at initialization.
  assert(SoBase::classTypeId == SoType::badType());

  SoBase::classTypeId = SoType::createType(SoType::badType(), "Base");

  SoBase::name2obj_dict = new SbDict;
  SoBase::obj2name_dict = new SbDict;
}

/*
  Clean up all commonly allocated resources before applcation exit. Only
  for debugging purposes.
 */
void
SoBase::cleanClass(void)
{
#if COIN_DEBUG
  assert(SoBase::name2obj_dict);
  assert(SoBase::obj2name_dict);

  // Delete the SbPLists in the dictionaries.
  SoBase::name2obj_dict->applyToAll(SoBase::freeLists);

  delete SoBase::name2obj_dict; SoBase::name2obj_dict = NULL;
  delete SoBase::obj2name_dict; SoBase::obj2name_dict = NULL;
#endif // COIN_DEBUG
}


/*!
  Increase the reference count of the object. This might be necessary
  to do explicitly from user code for certain situations (chiefly to
  avoid premature deletion), but is usually called from other instances
  within the Coin library when objects are made dependent on each other.

  \sa unref(), unrefNoDelete()
 */
void
SoBase::ref(void) const
{
  // Cast away constness.
  SoBase * base = (SoBase *)this;
  base->objdata.referencecount++;

#if COIN_DEBUG
  if (SoBase::tracerefs) {
    SoDebugError::postInfo("SoBase::ref",
                           "%p ('%s') - referencecount: %d",
                           this, this->getTypeId().getName().getString(),
                           this->objdata.referencecount);
  }
#endif // COIN_DEBUG
}

/*!
  Decrease the reference count of an object. If the reference count
  reaches zero, the object will delete itself. Be careful when
  explicitly calling this method, but beware that you usually need to
  match user level calls to ref() with calls to either unref() or
  unrefNoDelete() to avoid memory leaks.

  \sa ref(), unrefNoDelete()
*/
void
SoBase::unref(void) const
{
  // Cast away constness.
  SoBase * base = (SoBase *)this;
  base->objdata.referencecount--;
#if COIN_DEBUG
  if (SoBase::tracerefs) {
    SoDebugError::postInfo("SoBase::unref",
                           "%p ('%s') - referencecount: %d",
                           this, this->getTypeId().getName().getString(),
                           this->objdata.referencecount);
  }
  if (this->objdata.referencecount < 0)
    SoDebugError::postWarning("SoBase::unref", "ref count less than zero");
#endif // COIN_DEBUG
  if (this->objdata.referencecount == 0) base->destroy();
}

/*!
  Decrease reference count, but do \e not delete ourself if the count
  reaches zero.

  \sa unref()
 */
void
SoBase::unrefNoDelete(void) const
{
  // Cast away constness.
  SoBase * base = (SoBase *)this;
  base->objdata.referencecount--;
#if COIN_DEBUG
  if (SoBase::tracerefs) {
    SoDebugError::postInfo("SoBase::unrefNoDelete",
                           "%p ('%s') - referencecount: %d",
                           this, this->getTypeId().getName().getString(),
                           this->objdata.referencecount);
  }
#endif // COIN_DEBUG
}

/*!
  Returns number of objects referring to this object. NB: this method
  should not be called during ordinary use, it should only be necessary
  to use for debugging purposes.
 */
int32_t
SoBase::getRefCount(void) const
{
  return this->objdata.referencecount;
}

/*!
  Force an update, in the sense that all objects connected to this will
  have to re-check the values of their inter-dependent data.
 */
void
SoBase::touch(void)
{
  this->startNotify();
}

/*!
  Returns \a TRUE if the type of this object is either of the same
  type or inherited from \a type.
 */
SbBool
SoBase::isOfType(SoType type) const
{
  return this->getTypeId().isDerivedFrom(type);
}

/*!
  Returns the type identifier for the SoBase class.
 */
SoType
SoBase::getClassTypeId(void)
{
  return SoBase::classTypeId;
}

/*!
  Returns name of object. If no name has been given to this object,
  the returned SbName will contain the empty string.
 */
SbName
SoBase::getName(void) const
{
  // If this assert fails, SoBase::initClass() has probably not been
  // called.
  assert(SoBase::obj2name_dict);

  void * value;
  SbBool found = SoBase::obj2name_dict->find((unsigned long)this, value);
  return found ? SbName((const char *)value) : SbName();
}

/*!
  Set the name of this object.

  Characters that are invalid to use as parts of names for SoBase
  derived objects will be automatically replaced by underscore
  characters. If the name \e starts with an invalid character, the new
  name will be \e preceded by an underscore character.

  \sa getName(), SbName::isBaseNameStartChar(), SbName::isBaseNameChar()
*/
void
SoBase::setName(const SbName & newname)
{
  // remove old name first
  SbName oldName = this->getName();
  if (oldName.getLength()) this->removeName(this, oldName.getString());

  // check for bad characters
  const char * str = newname.getString();
  SbBool isBad = FALSE;

  isBad = (newname.getLength() > 0) && !SbName::isBaseNameStartChar(str[0]);

  int i;
  for (i = 1; i < newname.getLength() && !isBad; i++) {
    isBad = !SbName::isIdentChar(str[i]);
  }

  if (isBad) {
    // replace bad characters
    SbString goodstring;

    if (!SbName::isBaseNameStartChar(str[0])) goodstring += '_';

    for (i = 0; i < newname.getLength(); i++) {
      goodstring += SbName::isBaseNameChar(str[i]) ? str[i] : '_';
    }

#if COIN_DEBUG
    SoDebugError::postWarning("SoBase::setName", "Bad characters in "
                              "name '%s'. Replacing with name '%s'",
                              str, goodstring.getString());
#endif // COIN_DEBUG

    this->addName(this, SbName(goodstring.getString()));
  }
  else {
    this->addName(this, newname.getString());
  }
}

/*!
  Adds a name<->object mapping to the global dictionary.
 */
void
SoBase::addName(SoBase * const b, const char * const name)
{
  SbPList * l;
  void * t;
  if (!SoBase::name2obj_dict->find((unsigned long)name, t)) {
    // name not used before, create new list
    l = new SbPList;
    SoBase::name2obj_dict->enter((unsigned long)name, l);
  }
  else {
    // name is used before, find pointer to old list
    l = (SbPList *)t;
  }
  // append this to the list
  l->append(b);

  // set name of object. SbDict::enter() will overwrite old name
  SoBase::obj2name_dict->enter((unsigned long)b, (void *)name);
}

/*!
  Removes a name<->object mapping from the global dictionary.
*/
void
SoBase::removeName(SoBase * const b, const char * const name)
{
  void * t;
  SbBool found = SoBase::name2obj_dict->find((unsigned long)name, t);
  assert(found);

  SbPList * list = (SbPList *) t;
  int i = list->find(b);
  assert(i >= 0);
  list->remove(i);

  SoBase::obj2name_dict->remove((unsigned long)b);
}

/*!
  This is the method which starts the notification sequence
  after changes.
*/
void
SoBase::startNotify(void)
{
}

/*!
  Notifies all auditors for this instance when changes are made.
*/
void
SoBase::notify(SoNotList * list)
{
  this->auditors.notify(list);
}

/*!
  Add an auditor to notify whenever the object changes in any significant
  way.

  \sa removeAuditor()
 */
void
SoBase::addAuditor(void * const auditor, const SoNotRec::Type type)
{
  this->auditors.append(auditor, type);
}

/*!
  Remove an auditor from the list. \a auditor will then no longer be
  notified whenever any updates are made to this object.

  \sa addAuditor()
*/
void
SoBase::removeAuditor(void * const auditor, const SoNotRec::Type type)
{
  this->auditors.remove(auditor, type);
}

/*!
  Returns list of objects auditing this object.

  \sa addAuditor(), removeAuditor()
*/
const SoAuditorList &
SoBase::getAuditors(void) const
{
  return this->auditors;
}

/*!
  This method is used during the first write pass of a write action to
  count the number of references to this object in the scene graph.
*/
void
SoBase::addWriteReference(SoOutput * out, SbBool isFromField)
{
  assert(out->getStage() == SoOutput::COUNT_REFS);

  this->objdata.writerefcount++;
  if (this->objdata.writerefcount > 1) this->objdata.multirefs = TRUE;
  if (!isFromField) this->objdata.ingraph = TRUE;
}

/*!
  Returns \a TRUE if this object should be written out during a write action.
  Will return \a FALSE if no references to this object has been made in the
  scene graph.

  Note that connections from the fields of fieldcontainer objects is not
  alone a valid reason for writing out the object -- there must also be at
  least one reference directly from another SoBase (like a parent/child
  relationship, for instance).

  This method will return a valid result only during the second pass of
  write actions.
*/
SbBool
SoBase::shouldWrite(void)
{
  return (this->objdata.ingraph == TRUE);
}

/*!
  \internal
  Don't know what this is good for.
*/
void
SoBase::incrementCurrentWriteCounter(void)
{
  SoBase::writecounter++;
}

/*!
  \internal
  Don't know what this is good for.
 */
void
SoBase::decrementCurrentWriteCounter(void)
{
  SoBase::writecounter--;
}

/*!
  Returns the object of \a type, or derived from \ type, registered
  under \a name. If several has been registered under the same name
  with the same type, returns the first.

  If no object of a valid type or subtype has been registered with the
  given name, returns \a NULL.
*/
SoBase *
SoBase::getNamedBase(const SbName & name, SoType type)
{
  void * t;
  if (SoBase::name2obj_dict->find((unsigned long)((const char *)name), t)) {
    SbPList * l = (SbPList *)t;
    if (l->getLength()) {
      SoBase * b = (SoBase *)((*l)[0]);
      if (b->isOfType(type)) return b;
    }
  }

  return NULL;
}

/*!
  Returns the number of objects of type \a type, or derived from \a type,
  registered under \a name.

  All matches will also be appended to \a baselist.
 */
int
SoBase::getNamedBases(const SbName & name, SoBaseList & baselist, SoType type)
{
  int matches = 0;

  void * t;
  if (SoBase::name2obj_dict->find((unsigned long)((const char *)name), t)) {
    SbPList * l = (SbPList *)t;
    for (int i=0; i < l->getLength(); i++) {
      SoBase * b = (SoBase *)((*l)[i]);
      if (b->isOfType(type)) {
        baselist.append(b);
        matches++;
      }
    }
  }

  return matches;
}

/*!

  Read next SoBase derived instance from the \a in stream, check that
  it is derived from \a expectedType and place a pointer to the newly
  allocated instance in \a base.

  \c FALSE is returned on read errors, mismatch with the \a
  expectedType, or if there are attempts at referencing (through the
  \c USE keyword) unknown instances.

  If we return \c TRUE with \a base equal to \c NULL, there was either
  end-of-file or a NULL keyword for the next \a base instance in the
  file. Use SoInput::eof() after calling this method to detect
  end-of-file conditions.

  If \c TRUE is returned and \a base is non-NULL upon return,
  the instance was allocated and initialized according the what
  was read from the \a in stream.

*/
SbBool
SoBase::read(SoInput * in, SoBase *& base, SoType expectedType)
{
  assert(expectedType != SoType::badType());
  base = NULL;

  SbName name;
  SbBool result = in->read(name, TRUE);
  if (!result) {
#if COIN_DEBUG && 0 // debug
    SoDebugError::postInfo("SoBase::read", "hit EOF");
#endif // debug
    return TRUE; // EOF, return TRUE with base==NULL
  }

#if COIN_DEBUG && 0 // debug
  SoDebugError::postInfo("SoBase::read", "name: '%s'", name.getString());
#endif // debug

  if (name == NULL_KEYWORD) return TRUE; // happens with So[S|M]FNode field values
  else if (name == USE_KEYWORD) result = SoBase::readReference(in, base);
  else result = SoBase::readBase(in, name, base);

  // Check type correctness.
  if (result) {
    assert(base);

    SoType type = base->getTypeId();
    assert(type != SoType::badType());

    if (!type.isDerivedFrom(expectedType)) {
#if COIN_DEBUG
      SoDebugError::postInfo("SoBase::read",
                             "type '%s' is not derived from '%s'",
                             type.getName().getString(),
                             expectedType.getName().getString());
#endif // COIN_DEBUG
      result = FALSE;
    }
  }

  if (!result && base && (name != USE_KEYWORD)) {
    base->ref();
    base->unref();
    base = NULL;
  }

  return result;
}

/*!
  Referenced instances of SoBase are written as "DEF NamePrefixNumber" when
  exported. "Name" is the name of the base instance from setName(), "Prefix"
  is common for all objects and can be set by this method (default is "+"),
  and "Number" is a unique id which is necessary if multiple objects have
  the same name.

  If you want the prefix to be something else than "+", use this method.
 */
void
SoBase::setInstancePrefix(const SbString & c)
{
  SoBase::refwriteprefix = c;
}

/*!
  Set to \c TRUE to activate debugging of reference counting, which
  could aid in finding hard to track down problems with accesses to
  freed memory or memory leaks. Note: this will produce lots of
  debug information in any "normal" running system, so use sensibly.

  The reference tracing functionality will be disabled in "release
  versions" of the Coin library.
 */
void
SoBase::setTraceRefs(SbBool bTrace)
{
  SoBase::tracerefs = TRUE;
}

/*!
  Return the status of the reference tracing flag.

  \sa setTraceRefs()
 */
SbBool
SoBase::getTraceRefs(void)
{
  return SoBase::tracerefs;
}

/*!
  Returns \a TRUE if this object will be written more than once upon
  export. Note that the result from this method is only valid during the
  second pass of a write action (and partly during the COUNT_REFS pass).
 */
SbBool
SoBase::hasMultipleWriteRefs(void) const
{
  return this->objdata.multirefs;
}

/*!
  Write out the header of any SoBase derived object. The header consists
  of the \c DEF keyword and the object name (if the object has a name,
  otherwise these will be skipped), the class name and a left bracket.

  Alternatively, the object representation may be made up of just the
  \c USE keyword plus the object name, if this is the second or subsequent
  reference written to the file.

  If the object has been completed just by writing the header (which will
  be the case if we're writing multiple references of an object),
  we return \a TRUE, otherwise \a FALSE.

  If we return \a FALSE (i.e. there's more to write), we will
  increment the indentation level.

  \sa writeFooter(), SoOutput::indent()
 */
SbBool
SoBase::writeHeader(SoOutput * out, SbBool isGroup, SbBool isEngine) const
{
  if (!out->isBinary()) {
    out->write(END_OF_LINE);
    out->indent();
  }

  SbName name = this->getName();
  int refid = out->findReference(this);
  SbBool firstwrite = refid == -1;
  SbBool multiref = this->hasMultipleWriteRefs();

  if (multiref && firstwrite) refid = out->addReference(this);

  if (!firstwrite) {
    out->write(USE_KEYWORD);
    if (!out->isBinary()) out->write(' ');
    SbString s = name.getString();
    s += SoBase::refwriteprefix.getString();
    s.addIntString(refid);
    out->write(s.getString());
  }
  else {
    if (name.getLength() || multiref) {
      out->write(DEF_KEYWORD);
      if (!out->isBinary()) out->write(' ');

      SbString s = name.getString();
      if (multiref) {
        s += SoBase::refwriteprefix.getString();
        s.addIntString(refid);
      }
      out->write(s.getString());
      if (!out->isBinary()) out->write(' ');
    }

    out->write(this->getFileFormatName());
    if (out->isBinary()) {
      uint32_t flags = 0x0;
      if (isGroup) flags |= SoBase::IS_GROUP;
      if (isEngine) flags |= SoBase::IS_ENGINE;
      out->write(flags);
    }
    else {
      out->write(" {");
      out->write(END_OF_LINE);
      out->incrementIndent();
    }
  }

  SoBase * thisp = (SoBase *)this;
  thisp->objdata.writerefcount--;
  if (this->objdata.writerefcount == 0) {
    // Make ready for next inital write action pass.
    thisp->objdata.ingraph = FALSE;
    thisp->objdata.multirefs = FALSE;
  }

  // Don't need to write out the rest if we are writing anything but
  // the first instance.
  return (firstwrite == FALSE);
}

/*!
  This method will terminate the block representing an SoBase derived
  object.
 */
void
SoBase::writeFooter(SoOutput * out) const
{
  if (!out->isBinary()) {
    out->decrementIndent();
    out->indent();
    out->write('}');
  }
}

/*!
  Returns the class name this object should be written under.  Default
  string returned is the name of the class from the type system.

  User extensions nodes and engines override this method to return the
  name of the extension (instead of "UnknownNode" or "UnknownEngine").
 */
const char *
SoBase::getFileFormatName(void) const
{
  return this->getTypeId().getName().getString();
}

/*!
  \internal
  Don't know what this is good for.
 */
uint32_t
SoBase::getCurrentWriteCounter(void)
{
  return SoBase::writecounter;
}

/*
  \internal
  Used to free the SbPLists in the name<->object dict.
*/
void
SoBase::freeLists(unsigned long, void * value)
{
  delete (SbPList *)value;
}

/*!
  \internal
 */
SbBool
SoBase::readReference(SoInput * in, SoBase *& base)
{
  SbName refname;
  if (!in->read(refname, FALSE)) {
    SoReadError::post(in, "Premature end of file after \"%s\"", USE_KEYWORD);
    return FALSE;
  }
  else if ((base = in->findReference(refname)) == NULL) {
    SoReadError::post(in, "Unknown reference \"%s\"", refname.getString());
    return FALSE;
  }

  return TRUE;
}

/*!
  \internal
 */
SbBool
SoBase::readBase(SoInput * in, SbName & className, SoBase *& base)
{
#if COIN_DEBUG && 0 // debug
  SoDebugError::postInfo("SoBase::readBase", "className: '%s'",
                         className.getString());
#endif // debug

  SbBool ret = TRUE, flush = FALSE;
  base = NULL;

  SbName refName;

  if (className == DEF_KEYWORD) {
    if (!in->read(refName, FALSE) || !in->read(className, TRUE)) {
      SoReadError::post(in, "Premature end of file after %s", DEF_KEYWORD);
      ret = FALSE;
    }

    if (!refName) {
      SoReadError::post(in, "No name given after %s", DEF_KEYWORD);
      ret = FALSE;
    }

    if (!className) {
      SoReadError::post(in, "Invalid definition of %s", refName.getString());
      ret = FALSE;
    }
  }

  if (ret) {
    SbBool gotchar = FALSE; // Unnecessary, but kills a compiler warning.
    char c;
    if (!in->isBinary() && (!(gotchar = in->read(c)) || c != OPEN_BRACE)) {
      if (gotchar)
        SoReadError::post(in, "Expected '%c'; got '%c'", OPEN_BRACE, c);
      else
        SoReadError::post(in, "Expected '%c'; got EOF", OPEN_BRACE);
      ret = FALSE;
    }
    else {
      ret = SoBase::readBaseInstance(in, className, refName, base);

      if (!in->isBinary()) {
        if (!ret) {
          flush = TRUE;
        }
        else if (!(gotchar = in->read(c)) || c != CLOSE_BRACE) {
          if (gotchar)
            SoReadError::post(in, "Expected '%c'; got '%c'", CLOSE_BRACE, c);
          else
            SoReadError::post(in, "Expected '%c'; got EOF", CLOSE_BRACE);
          ret = FALSE;
        }
      }
    }
  }

  if (!ret && flush) SoBase::flushInput(in);
  return ret;
}

/*!
  \internal
 */
SbBool
SoBase::readBaseInstance(SoInput * in, const SbName & className,
                         const SbName & refName, SoBase *& base)
{
  SbBool retval = TRUE;

  if ((base = SoBase::createInstance(in, className))) {
    if (!(!refName)) {
      // Set up new entry in reference hash -- with full name.
      in->addReference(refName, base);

      // Remove reference counter suffix, if any (i.e. "goldsphere+2"
      // becomes "goldsphere").
      SbString instancename = refName.getString();
      const char * strp = instancename.getString();
      const char * occ = strstr(strp, SoBase::refwriteprefix.getString());

      if (occ != strp) { // They will be equal if the name is only a refcount.
        if (occ) instancename = instancename.getSubString(0, occ - strp - 1);
        // Set name identifier for newly created SoBase instance.
        base->setName(instancename);
      }
    }

    unsigned short flags = 0;
    if (in->isBinary())
      retval = in->read(flags);
    else
      flags =
        (base->isOfType(SoType::fromName("SoGroup")) ? IS_GROUP : 0x0) |
        (base->isOfType(SoType::fromName("SoEngine")) ? IS_ENGINE : 0x0);

    if (retval) retval = base->readInstance(in, flags);

    if (!retval) {
      if (!(!refName)) in->removeReference(refName);
      base->ref();
      base->unref();
      base = NULL;
    }
  }
  else {
    retval = FALSE;
  }

  return retval;
}

/*!
  \internal
 */
SoBase *
SoBase::createInstance(SoInput * in, const SbName & className)
{
  SoType type = SoType::fromName(className);

  SoBase * instance = NULL;

  if (type == SoType::badType()) {

    // FIXME: I think this code is plain wrong. There are flags to
    // check to see if we have an SoUnknownEngine on our hands (or a
    // group-type SoUnknownNode). 20000102 mortene.
#if 0
    SbString unknownString;
    if (!in->read(unknownString) || unknownString != "fields") {
      // FIXME: check for unknown engine class aswell? 19991229 mortene.
      SoReadError::post(in, "Unknown class \"%s\"", className.getString());
      return NULL;
    }
#endif
    // Default to SoUnknownNode for now..
    SoUnknownNode * unknownnode = new SoUnknownNode;
    unknownnode->setNodeClassName(className);
    instance = unknownnode;
  }
  else if (!type.canCreateInstance()) {
    SoReadError::post(in, "Class \"%s\" is abstract", className.getString());
  }
  else {
    instance = (SoBase *)type.createInstance();
  }

  return instance;
}

/*!
  \internal
 */
void
SoBase::flushInput(SoInput * in)
{
  assert(!in->isBinary());

  int nestLevel = 1;
  char c;

  while (nestLevel > 0 && in->read(c)) {
    if (c == CLOSE_BRACE) nestLevel--;
    else if (c == OPEN_BRACE) nestLevel++;
  }
}
