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
  \class SoField SoField.h Inventor/fields/SoField.h
  \brief The SoField class is the top-level abstract base class for fields.
  \ingroup fields

  Fields is the mechanism used throughout Coin for encapsulating basic
  data types to detect changes made to them, and to provide conversion,
  import and export facilities. All public properties in nodes are stored in
  fields, along with the inputs and outputs of engines.

  Forcing data modification to go through a method interface makes it
  possible to automatically detect and react upon changes in the
  user-specified data structures. E.g. the default behavior when changing
  the value of a field in a scene graph node is that there'll automatically
  be a rendering update.


  \sa SoSField, SoMField, SoFieldContainer, SoFieldData
*/

#include <assert.h>

#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>

#include <Inventor/SbName.h>
#include <Inventor/VRMLnodes/SoVRMLInterpOutput.h>
#include <Inventor/VRMLnodes/SoVRMLInterpolator.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/errors/SoReadError.h>
#include <Inventor/fields/SoField.h>
#include <Inventor/fields/SoFieldContainer.h>
#include <Inventor/fields/SoFields.h>
#include <Inventor/lists/SoAuditorList.h>
#include <Inventor/lists/SoFieldList.h>
#include <Inventor/lists/SoVRMLInterpOutputList.h>
#include <Inventor/engines/SoEngine.h>
#include <Inventor/engines/SoEngineOutput.h>
#include <Inventor/lists/SoEngineOutputList.h>
#include <Inventor/SoDB.h>
#include <Inventor/engines/SoFieldConverter.h>
#include <Inventor/actions/SoWriteAction.h>

static const char IGNOREDCHAR = '~';
// FIXME: '\n' not correct on win32/mac? 19980910 mortene.
static const char EOLSTR[] = "\n";

/*
  This class is used to aid in "multiplexing" the pointer member of
  SoField. This is a way to achieve the goal of using minimum storage
  space for SoField classes in the default case (which is important,
  as fields are ubiquitous in Coin). The default case means no
  connections and only a field container given. If any connections are
  made (either "to" or "from"), we allocate an SoConnectStorage and
  move the field container pointer into it, while swapping in the
  SoConnectStorage pointer where the field container pointer used to be.
*/
class SoConnectStorage {
public:
  /* Constructor. */
  SoConnectStorage(SoFieldContainer * container)
    : fcontainer(container), fieldlist(NULL), engineoutlist(NULL),
      interpoutlist(NULL), auditorlist(NULL)
    { }

  /* Destructor. */
  ~SoConnectStorage()
    {
      delete this->fieldlist;
      delete this->engineoutlist;
      delete this->interpoutlist;
      delete this->auditorlist;
    }

  /* Set field's "owner". */
  void setContainer(SoFieldContainer * fc) { this->fcontainer = fc; }
  /* Returns field's "owner". */
  SoFieldContainer * getContainer(void) const { return this->fcontainer; }

  /* Add a new connection from a field. */
  void addConnection(SoField * master)
    {
      if (!this->fieldlist) this->fieldlist = new SoFieldList(1);
      this->fieldlist->append(master);
    }

  /* Remove field connection. */
  void removeConnection(SoField * master)
    {
      this->fieldlist->remove(this->fieldlist->find(master));
    }

  /* Returns number of items in list. */
  int getNumFieldConnections(void) const
    {
      return this->fieldlist ? this->fieldlist->getLength() : 0;
    }

  /* Returns item at given index in list. */
  SoField * getFieldConnection(const int idx) const
    {
      return (*(this->fieldlist))[idx];
    }

  /* Returns reference to field master list. */
  const SoFieldList & getFieldConnections(void) const
    {
      assert(this->fieldlist);
      return *(this->fieldlist);
    }

  /* Add a new connection from an engine output . */
  void addConnection(SoEngineOutput * master)
    {
      if (!this->engineoutlist)
        this->engineoutlist = new SoEngineOutputList(1);
      this->engineoutlist->append(master);
    }

  /* Remove engine output connection. */
  void removeConnection(SoEngineOutput * master)
    {
      this->engineoutlist->remove(this->engineoutlist->find(master));
    }

  /* Returns number of items in list. */
  int getNumEngineConnections(void) const
    {
      return this->engineoutlist ? this->engineoutlist->getLength() : 0;
    }

  /* Returns item at given index in list. */
  SoEngineOutput * getEngineConnection(const int idx) const
    {
      return (*(this->engineoutlist))[idx];
    }

  /* Returns reference to engine master list. */
  const SoEngineOutputList & getEngineConnections(void) const
    {
      assert(this->engineoutlist);
      return *(this->engineoutlist);
    }

  /* Add a new connection from a VRML interpolator output. */
  void addConnection(SoVRMLInterpOutput * master)
    {
      if (!this->interpoutlist)
        this->interpoutlist = new SoVRMLInterpOutputList(1);
      this->interpoutlist->append(master);
    }

  /* Remove interpolator output connection. */
  void removeConnection(SoVRMLInterpOutput * master)
    {
      this->interpoutlist->remove(this->interpoutlist->find(master));
    }

  /* Returns number of items in list. */
  int getNumVRMLInterpConnections(void) const
    {
      return this->interpoutlist ? this->interpoutlist->getLength() : 0;
    }

  /* Returns item at given index in list. */
  SoVRMLInterpOutput * getVRMLInterpConnection(const int idx) const
    {
      return (*(this->interpoutlist))[idx];
    }

  /* Returns reference to interpolator master list. */
  const SoVRMLInterpOutputList & getVRMLInterpConnections(void) const
    {
      assert(this->interpoutlist);
      return *(this->interpoutlist);
    }


  /* Add a new auditor to the field. */
  void addAuditor(void * slave, SoNotRec::Type type)
    {
      if (!this->auditorlist) this->auditorlist = new SoAuditorList;
      this->auditorlist->append(slave, type);
    }

  /* Returns auditor at index idx. */
  void * getAuditor(const int idx, SoNotRec::Type & type) const
    {
      type = this->auditorlist->getType(idx);
      return this->auditorlist->getObject(idx);
    }

  /* Remove auditor from list. */
  void removeAuditor(void * slave, SoNotRec::Type type)
    {
      assert(this->auditorlist);
      this->auditorlist->remove(slave, type);
    }

  /* Remove auditor from list. */
  void removeAuditor(const int idx)
    {
      assert(this->auditorlist);
      assert(idx >= 0 && idx < this->auditorlist->getLength());
      this->auditorlist->remove(idx);
    }

  /* Returns numbers of auditors for field. */
  int getNumAuditors(void) const
    {
      return this->auditorlist ? this->auditorlist->getLength() : 0;
    }

  /* Returns reference to auditor list. */
  SoAuditorList & getAuditors(void) const
    {
      assert(this->auditorlist);
      return *(this->auditorlist);
    }


private:
  /* Pointer to the field's SoFieldContainer "owner" (or NULL if none). */
  SoFieldContainer * fcontainer;

  /* Pointers to lists of connection master fields/engines/interpolators. */
  SoFieldList * fieldlist;
  SoEngineOutputList * engineoutlist;
  SoVRMLInterpOutputList * interpoutlist;

  /* Pointer to the field's list of slaves. */
  SoAuditorList * auditorlist;
};


// *************************************************************************

// Documentation for abstract methods.

/*!
  \fn SoType SoField::getTypeId(void) const

  Returns the type identification instance which uniquely identifies
  the Coin field class the object belongs to.

  \sa getClassTypeId(), SoType
*/
/*!
  \fn SbBool SoField::isSame(const SoField & f) const
  Check for equal type and value(s).
*/
/*!
  \fn void SoField::copyFrom(const SoField & f)
  Copy value(s) from \a f.
*/
/*!
  \fn SbBool SoField::readValue(SoInput * in)
  Read field value(s).
*/
/*!
  \fn void SoField::writeValue(SoOutput * out) const
  Write field value(s).
*/
/*!
  \fn void SoField::convertTo(SoField * dest) const
  Converts our field's value(s) to value(s) acceptable for the \a dest field.
*/


// *************************************************************************

SoType SoField::classTypeId = SoType::badType();

// *************************************************************************

/*!
  This is the base constructor for field classes. It takes care of
  doing the common parts of data initialization in fields.
 */
SoField::SoField(void)
  : donotify(TRUE), container(NULL)
{
  this->statusflags.ignore = FALSE;
  this->statusflags.isdefault = TRUE;
  this->statusflags.extstorage = FALSE;
  this->statusflags.enableconnects = TRUE;
  this->statusflags.needevaluation = FALSE;
  this->statusflags.isevaluating = FALSE;
}

/*!
  Destructor. Disconnects ourself from any connected field, engine or
  VRML interpolator, before we disconnect all auditors on the field.
 */
SoField::~SoField()
{
#if 0 // debug
  SoDebugError::postInfo("SoField::~SoField", "destructing %p", this);
#endif // debug

  // Disconnect ourself from all connections where we are the slave.
  this->disconnect();

  if (this->hasExtendedStorage()) {
    // Disconnect all connections using us as master.
    while (this->storage->getNumAuditors()) {
      SoNotRec::Type type;
      void * obj = this->storage->getAuditor(0, type);

      switch (type) {
      case SoNotRec::FIELD:
        ((SoField *)obj)->disconnect(this);
        break;

      case SoNotRec::CONTAINER:
        this->storage->removeAuditor(0);
        // FIXME: anything else to do here? 19990622 mortene.
        break;

      case SoNotRec::ENGINE:
      case SoNotRec::SENSOR:
      case SoNotRec::INTERP:
        COIN_STUB();
        break;

      case SoNotRec::PARENT:
      default:
        // Not supposed to happen, of course.
        assert(0);
        break;
      }
    }

    delete this->storage;
  }
}

/*!
  Must be called upon initialization of the library to set up
  the type system.
 */
void
SoField::initClass(void)
{
  // Make sure we only initialize once.
  assert(SoField::classTypeId == SoType::badType());
  SoField::classTypeId = SoType::createType(SoType::badType(), "Field");
  SoField::initClasses();
}

/*!
  Sets the flag which indicates whether or not the field should be
  ignored during certain operations.

  The effect of this flag depends on what type of field it is used on,
  and the type of the node which includes the field.

  \sa isIgnored()
 */
void
SoField::setIgnored(SbBool ignore)
{
  if ((this->statusflags.ignore && !ignore) ||
      (!this->statusflags.ignore && ignore)) {
    this->statusflags.ignore = ignore;
    this->touch();
  }
}

/*!
  Returns the ignore flag.

  \sa setIgnored()
 */
SbBool
SoField::isIgnored(void) const
{
  return this->statusflags.ignore;
}

/*!
  Set whether or not this field should be marked as containing a default
  value.

  \sa isDefault()
 */
void
SoField::setDefault(SbBool def)
{
  // FIXME: make private or protected? 19990623 mortene.
  this->statusflags.isdefault = def;
}

/*!
  Check if the field contains its default value. Fields which has their default
  value intact will normally not be included in the output when writing
  scene graphs out to a file, for instance.

  \sa setDefault()
 */
SbBool
SoField::isDefault(void) const
{
  return this->statusflags.isdefault;
}

/*!
  Returns a unique type identifier for the SoField class.

  \sa getTypeId(), SoType
 */
SoType
SoField::getClassTypeId(void)
{
  return SoField::classTypeId;
}

/*!
  Check if the class is of either the same type or a derived type of the one
  given with the \a type parameter.
 */
SbBool
SoField::isOfType(const SoType type) const
{
  return this->getTypeId().isDerivedFrom(type);
}

/*!
  This sets a flag which indicates whether or not the set up connection
  should be considered active. For as long as the "enable connection"
  flag is \a FALSE, no value propagation will be done from any
  connected source field, engine or interpolator into this field.

  If the connection is first disabled and then enabled again, the
  field is marked as dirty, which means that it will again be in
  synchronization with its source field/engine/interpolator.

  \sa isConnectionEnabled()
*/
void
SoField::enableConnection(SbBool flag)
{
  SbBool oldval = this->statusflags.enableconnects;
  this->statusflags.enableconnects = flag;
  if (!oldval && flag) this->setDirty(TRUE);
}

/*!
  Return the current status of the connection enabled flag.

  \sa enableConnection()
 */
SbBool
SoField::isConnectionEnabled(void) const
{
  return this->statusflags.enableconnects;
}

/*!
  Connects this field as a slave to \a master. This means that the value
  of this field will be automatically updated when \a master is changed (as
  long as the connection also is enabled).

  If the field connected \e from is different from the field connected
  \e to, a field converter is inserted. For some combinations of fields
  no such conversion is possible, and we'll return \a FALSE.

  If this field had any master-relationships beforehand, these are all
  broken up if \a append is \a FALSE.

  Call with \a notnotify if you want to avoid the initial notification
  of connected auditors (a.k.a. slaves).

  \sa enableConnection(), isConnectionEnabled(), isConnectedFromField()
  \sa getConnectedField(), appendConnection(SoField *)
*/
SbBool
SoField::connectFrom(SoField * master, SbBool notnotify, SbBool append)
{
  SoType mastertype = master->getTypeId();
  SoType slavetype = this->getTypeId();

  if (mastertype == slavetype) {
    if (!append) this->disconnect();
    this->doConnect(master, !notnotify);
    return TRUE;
  }

  SoFieldConverter * conv=NULL;
  SbBool OK=this->createConverter(mastertype, slavetype, conv);
  if (OK) {
    assert(conv);
    conv->getInput(mastertype)->doConnect(master, !notnotify);
    if (!append) this->disconnect();
    this->doConnect(conv->getOutput(slavetype), !notnotify);
    return TRUE;
  }

  return FALSE;
}

/*!
  Connects this field as a slave to \a master. This means that the value
  of this field will be automatically updated when \a master is changed (as
  long as the connection also is enabled).

  If the field output connected \e from is different from the field connected
  \e to, a field converter is inserted. For some combinations of fields
  no such conversion is possible, and we'll return \a FALSE.

  If this field had any master-relationships beforehand, these are all
  broken up if \a append is \a FALSE.

  Call with \a notnotify if you want to avoid the initial notification
  of connected auditors (a.k.a. slaves).

  \sa enableConnection(), isConnectionEnabled(), isConnectedFromField()
  \sa getConnectedField(), appendConnection(SoVRMLInterpOutput *)
*/
SbBool
SoField::connectFrom(SoVRMLInterpOutput * master,
                     SbBool notnotify, SbBool append)
{
  SoType mastertype = master->getConnectionType();
  SoType slavetype = this->getTypeId();

  if (mastertype == slavetype) {
    if (!append) this->disconnect();
    this->doConnect(master, !notnotify);
    return TRUE;
  }

  SoFieldConverter * conv=NULL;
  SbBool OK=this->createConverter(mastertype, slavetype, conv);
  if (OK) {
    assert(conv);
    conv->getInput(mastertype)->doConnect(master, !notnotify);
    if (!append) this->disconnect();
    this->doConnect(conv->getOutput(slavetype), !notnotify);
    return TRUE;
  }

  return FALSE;
}


/*!
  Connects this field as a slave to \a master. This means that the value
  of this field will be automatically updated when \a master is changed (as
  long as the connection also is enabled).

  If the field output connected \e from is different from the field connected
  \e to, a field converter is inserted. For some combinations of fields
  no such conversion is possible, and we'll return \a FALSE.

  If this field had any master-relationships beforehand, these are all
  broken up if \a append is \a FALSE.

  Call with \a notnotify if you want to avoid the initial notification
  of connected auditors (a.k.a. slaves).

  \sa enableConnection(), isConnectionEnabled(), isConnectedFromField()
  \sa getConnectedField(), appendConnection(SoEngineOutput *)
*/
SbBool
SoField::connectFrom(SoEngineOutput * master,
                     SbBool notnotify, SbBool append)
{
  SoType mastertype = master->getConnectionType();
  SoType slavetype = this->getTypeId();

  if (mastertype == slavetype) {
    if (!append) this->disconnect();
    this->doConnect(master, !notnotify);
    return TRUE;
  }

  SoFieldConverter * conv=NULL;
  SbBool OK=this->createConverter(mastertype, slavetype, conv);
  if (OK) {
    assert(conv);
    conv->getInput(mastertype)->doConnect(master, !notnotify);
    if (!append) this->disconnect();
    this->doConnect(conv->getOutput(slavetype), !notnotify);
    return TRUE;
  }

  return FALSE;
}

/*!
  Disconnect this field as a slave from \a master.
*/
void
SoField::disconnect(SoField * master)
{
#if 0 // debug
  SoDebugError::postInfo("SoField::disconnect",
                         "removing slave %p from master %p", this, master);
#endif // debug
  this->evaluate();
  master->removeAuditor(this, SoNotRec::FIELD);
  this->storage->removeConnection(master);
}

/*!
  Disconnect this field as a slave from \a master.
*/
void
SoField::disconnect(SoVRMLInterpOutput * master)
{
  this->evaluate();
  master->removeConnection(this);
  this->storage->removeConnection(master);
}

/*!
  Disconnect this field as a slave from \a master.
*/
void
SoField::disconnect(SoEngineOutput * master)
{
  this->evaluate();
  master->removeConnection(this);
  this->storage->removeConnection(master);
}

/*!
  Returns number of fields this field is a slave of.

  \sa getConnections()
*/
int
SoField::getNumConnections(void)
{
  return this->hasExtendedStorage() ?
    this->storage->getNumFieldConnections() : 0;
}

/*!
  Returns number of master fields this field is connected to, and places
  references to all of them into \a masterlist.

  \sa getNumConnections()
*/
int
SoField::getConnections(SoFieldList & masterlist)
{
  if (!this->hasExtendedStorage()) return 0;

  int nr = this->storage->getNumFieldConnections();
  if (nr) masterlist = this->storage->getFieldConnections();
  return nr;
}

/*!
  Disconnect all the slave connections this field have.
 */
void
SoField::disconnect(void)
{
  //FIXME: Check for converters?
  //...Not necessary, I think (kintel 19981030)

  // Disconnect us from all master fields.
  while (this->isConnectedFromField())
    this->disconnect(this->storage->getFieldConnection(0));

  // Disconnect us from all master engine outputs.
  while (this->isConnectedFromEngine())
    this->disconnect(this->storage->getEngineConnection(0));

  // Disconnect us from all master VRML interpolator outputs.
  while (this->isConnectedFromVRMLInterp())
    this->disconnect(this->storage->getVRMLInterpConnection(0));

  assert(this->isConnected() == FALSE);
}

/*!
  Returns \a TRUE if we're connected from another field, engine or
  interpolator.

  \sa isConnectedFromField(), isConnectedFromEngine()
  \sa isConnectedFromVRMLInterp(), connectFrom()
 */
SbBool
SoField::isConnected(void) const
{
  return (this->isConnectedFromField() ||
          this->isConnectedFromEngine() ||
          this->isConnectedFromVRMLInterp());
}

/*!
  Returns \a TRUE if we're a slave of at least one field.

  \sa isConnected(), isConnectedFromEngine(), isConnectedFromVRMLInterp()
  \sa connectFrom(SoField *)
 */
SbBool
SoField::isConnectedFromField(void) const
{
  return (this->hasExtendedStorage() &&
          this->storage->getNumFieldConnections());
}

/*!
  Returns \a TRUE if we're a slave of at least one VRML interpolator.

  \sa isConnected(), isConnectedFromEngine(), isConnectedFromField()
  \sa connectFrom(SoVRMLInterpOutput *)
 */
SbBool
SoField::isConnectedFromVRMLInterp(void) const
{
  return (this->hasExtendedStorage() &&
          this->storage->getNumVRMLInterpConnections());
}

/*!
  Returns \a TRUE if we're connected from an engine.

  \sa isConnected(), isConnectedFromVRMLInterp(), isConnectedFromField()
  \sa connectFrom(SoEngineOutput *)
 */
SbBool
SoField::isConnectedFromEngine(void) const
{
  return (this->hasExtendedStorage() &&
          this->storage->getNumEngineConnections());
}

/*!
  Returns \a TRUE if we are connected as a slave to at least one other field.
  \a master will be set to the source field in the last field connection made.

  \sa isConnectedFromField(), connectFrom(SoField *),
  \sa appendConnection(SoField *)
 */
SbBool
SoField::getConnectedField(SoField *& master) const
{
  if (!this->hasExtendedStorage()) return FALSE;
  int nr = this->storage->getNumFieldConnections();
  if (nr) master = this->storage->getFieldConnection(nr - 1);
  return nr ? TRUE : FALSE;
}


/*!
  Returns \a TRUE if we are connected as a slave to at least one VRML
  interpolator. \a master will be set to the source of the last
  interpolator connection made.

  \sa isConnectedFromVRMLInterp(), connectFrom(SoVRMLInterpOutput *)
  \sa appendConnection(SoVRMLInterpOutput *)
 */
SbBool
SoField::getConnectedVRMLInterp(SoVRMLInterpOutput *& master) const
{
  if (!this->hasExtendedStorage()) return FALSE;
  int nr = this->storage->getNumVRMLInterpConnections();
  if (nr) master = this->storage->getVRMLInterpConnection(nr - 1);
  return nr ? TRUE : FALSE;
}
/*!
  Returns \a TRUE if we are connected as a slave to at least one
  engine. \a master will be set to the source of the last
  engine connection made.

  \sa isConnectedFromEngine(), connectFrom(SoEngineOutput *)
  \sa appendConnection(SoEngineOutput *)
 */
SbBool
SoField::getConnectedEngine(SoEngineOutput *& master) const
{
  if (!this->hasExtendedStorage()) return FALSE;
  int nr = this->storage->getNumEngineConnections();
  if (nr) master = this->storage->getEngineConnection(nr - 1);
  return nr ? TRUE : FALSE;
}

/*!
  Appends all the fields which are auditing this field in \a slavelist,
  and returns the number of fieldtype slaves.
 */
int
SoField::getForwardConnections(SoFieldList & slavelist) const
{
  int nr = 0;

  if (this->hasExtendedStorage()) {
    const SoAuditorList & al = this->storage->getAuditors();
    for (int i=0; i < al.getLength(); i++) {
      if (al.getType(i) == SoNotRec::FIELD) {
        slavelist.append((SoField *)al.getObject(i));
        nr++;
      }
    }
  }

  return nr;
}

/*!
  Let the field know in which container it belongs.

  \sa getContainer(), SoFieldContainer
 */
void
SoField::setContainer(SoFieldContainer * cont)
{
  SoFieldContainer * oldcontainer;

  if (!this->hasExtendedStorage()) {
    oldcontainer = this->container;
    this->container = cont;
  }
  else {
    oldcontainer = this->storage->getContainer();
    this->storage->setContainer(cont);
  }

  if (oldcontainer) this->removeAuditor(oldcontainer, SoNotRec::CONTAINER);
  this->addAuditor(cont, SoNotRec::CONTAINER);
}

/*!
  Returns the SoFieldContainer object "owning" this field.

  \sa SoFieldContainer, setContainer()
 */
SoFieldContainer *
SoField::getContainer(void) const
{
  if (!this->hasExtendedStorage())
    return this->container;
  else
    return this->storage->getContainer();
}

/*!
  Set the field's value through the given string. The format of the string
  must adhere to the ASCII format used in Coin data format files.
  Only the value should be specified, though - \e not the name of the field.

  \a FALSE is returned if the stringencoded field value is invalid for the
  field type and can't be parsed in any sensible way.

  \sa get()
 */
SbBool
SoField::set(const char * valueString)
{
  SoInput in;
  in.setBuffer((void *)valueString, strlen(valueString));
  if (!this->readValue(&in)) return FALSE;

  this->valueChanged();
  return TRUE;
}

// Overload SoOutput to make sure no header is written to the buffer.
// NB: this class is only used by the SoField::get() method below.
class DerivedSoOutput : public SoOutput {
  // FIXME: I tried to move this class definition inside the
  // SoField::get() scope, but this resulted in an "unresolved symbol"
  // error at link time when compiling with certain versions of the
  // egcs/gcc compiler (at least with egcs-2.91.66 and gcc
  // 2.95). 19990929 mortene.
public:
  DerivedSoOutput(void) { this->wroteHeader = TRUE; }
  virtual void reset(void) { SoOutput::reset(); this->wroteHeader = TRUE; }
};

/*!
  Returns the field's value as an ASCII string in the export data format
  for Coin (and Open Inventor) files.

  \sa set()
 */
void
SoField::get(SbString & valueString)
{
  DerivedSoOutput out;
  const size_t STARTSIZE = 32;
  void * buffer = malloc(STARTSIZE);

  out.setBuffer(buffer, STARTSIZE, SoField::reallocOutputBuf);
  this->writeValue(&out);

  size_t size;
  out.getBuffer(buffer, size);
  valueString = (char *)buffer;
  free(buffer);
}

/*!
  Mark the field as changed. Touching a field which is part of any component
  (engine or node) in a scene graph will lead to a forced redraw. This is
  useful if you have been doing several updates to the field wrapped in
  a pair of enableNotify() calls to notify the field's auditors that its
  value has changed.

  \sa setValue(), enableNotify()
 */
void
SoField::touch(void)
{
  if (this->container) this->startNotify();
}

/*!
  Trigger a notification sequence.
*/
void
SoField::startNotify(void)
{
  SoNotList list;
  this->notify(&list);
}

/*!
  Notify auditors that this field has changed.
 */
void
SoField::notify(SoNotList * nlist)
{
#if 0 // debug
  SoDebugError::postInfo("SoField::notify", "%p", this);
#endif // debug

  // If we're not the originator of the notification process, we need
  // to be marked dirty, as it means something we're connected to as
  // a slave has changed and our value need to be updated.
  if (nlist->getFirstRec()) this->setDirty(TRUE);

  if (this->isNotifyEnabled()) {
    SoNotRec rec(this->getContainer());
    nlist->append(&rec, this);

    this->notifyAuditors(nlist);
  }
}

/*!
  This method sets whether notification will be propagated on changing the
  value of the field.  The old value of the setting is returned.

  \sa isNotifyEnabled()
*/
SbBool
SoField::enableNotify(SbBool on)
{
  const SbBool old = this->donotify;
  this->donotify = on;
  return old;
}

/*!
  This method returns whether notification of changes to the field value
  are propagated to the auditors.

  \sa enableNotify()
*/
SbBool
SoField::isNotifyEnabled(void) const
{
  return this->donotify;
}

/*!
  Add an auditor to the list. All auditors will be notified whenever this
  field changes its value(s).
 */
void
SoField::addAuditor(void * f, SoNotRec::Type type)
{
  if (!this->hasExtendedStorage()) {
    this->storage = new SoConnectStorage(this->container);
    this->statusflags.extstorage = TRUE;
  }

  this->storage->addAuditor(f, type);
  if (type == SoNotRec::FIELD) this->connectionStatusChanged(+1);
}

/*!
  Remove an auditor from the list.
 */
void
SoField::removeAuditor(void * f, SoNotRec::Type type)
{
#if 0 // debug
  SoDebugError::postInfo("SoField::removeAuditor",
                         "%p removing %p", this, f);
#endif // debug

  assert(this->hasExtendedStorage());
  this->storage->removeAuditor(f, type);
  if (type == SoNotRec::FIELD) this->connectionStatusChanged(-1);
}

/*!
  Checks for equality. Returns \a 0 if the fields are of different type
  or does not equal each other's values.
 */
int
SoField::operator ==(const SoField & f) const
{
  return this->isSame(f);
}

/*!
  Returns \a TRUE if the fields are of different type or has different value.
 */
int
SoField::operator !=(const SoField & f) const
{
  return !this->isSame(f);
}

/*!
  Returns \a TRUE if it is necessary to write the field when dumping a
  scene graph.
 */
SbBool
SoField::shouldWrite(void) const
{
  if (!this->isDefault()) return TRUE;
  if (this->isIgnored()) return TRUE;
  // ..more here?..yes: connections..
  return FALSE;
}

/*!
  Called whenever another slave attaches or detaches itself to us.
  \a numConnections is the difference in number of connections
  made (i.e. if stuff is \e disconnected, \a numConnections will be
  a negative number).
 */
void
SoField::connectionStatusChanged(int /* numConnections */)
{
  // FIXME: not sure if this is correct or not. Looks like it could
  // have unwanted side effects (premature destruction). 19990711
  // mortene.
#if 0
  while (numConnections > 0) {
    this->ref();
    numConnections--;
  }

  while (numConnections < 0) {
    this->unref();
    numConnections++;
  }
#endif // disabled
}

/*!
  FIXME: write doc
 */
SbBool
SoField::isReadOnly(void) const
{
  COIN_STUB();
  return FALSE;
}

/*!
  FIXME: write doc
 */
void
SoField::fixCopy(SbBool /* copyConnections */)
{
  COIN_STUB();
}

/*!
  FIXME: write doc
 */
SbBool
SoField::referencesCopy(void) const
{
  COIN_STUB();
  return FALSE;
}

/*!
  FIXME: write doc
 */
void
SoField::copyConnection(const SoField * /* fromfield */)
{
  COIN_STUB();
}

/*!
  Reads and sets the value of this field from the given SoInput instance.
  Returns \a FALSE if the field value can not be parsed from the input.

  \sa set(), write()
 */
SbBool
SoField::read(SoInput * in, const SbName & name)
{
  this->setDefault(FALSE);
  this->setDirty(FALSE);

  if (!in->isBinary()) {
    // Check for ignored flag first.
    char c;
    if (!in->read(c)) return FALSE;
    if (c == IGNOREDCHAR) {
      this->setIgnored(TRUE);
      return TRUE;
    }
    else {
      in->putBack(c);
    }
  }

  if (!this->readValue(in)) {
    SoReadError::post(in, "Couldn't read value for field \"%s\"",
                      name.getString());
    return FALSE;
  }

  if (in->isBinary()) {
    unsigned int flags;
    if (!in->read(flags)) {
      SoReadError::post(in, "Couldn't read field flags for field \"%s\"",
                        name.getString());
      return FALSE;
    }

    if (flags & 0x01) this->setIgnored(TRUE);
    if (flags & 0x04) this->setDefault(TRUE);
#if COIN_DEBUG
    // FIXME: probably more flags to take care of. 19990711 mortene.
    if (flags & ~(0x01 | 0x04)) {
      SoDebugError::postInfo("SoField::read",
                             "unknown field flags (flags: 0x%x)", flags);
    }
#endif // COIN_DEBUG
  }
  else {
    // Check again for ignored flag.
    char c;
    if (!in->read(c)) return FALSE;
    if (c == IGNOREDCHAR) this->setIgnored(TRUE);
    else in->putBack(c);
  }

  // FIXME: call touch()? Probably not. 19990406 mortene.
  return TRUE;
}

/*!
  Write the value of the field to the given SoOutput instance (which can
  be either a memory buffer or a file, in ASCII or in binary format).

  \sa get(), read(), SoOutput
 */
void
SoField::write(SoOutput * out, const SbName & name) const
{
  if(!this->shouldWrite()) return;

  // ASCII write.
  if (!out->isBinary()) {
    out->indent();
    // Cast to avoid "'s.
    out->write((const char *)name);
    if (!this->isDefault()) {
      out->write(' ');
      this->writeValue(out);
    }
    if (this->isIgnored()) {
      out->write(' ');
      out->write(IGNOREDCHAR);
    }

    if (this->isConnected()) this->writeConnection(out);

    out->write(EOLSTR);
  }
  // Binary write.
  else {
    // Cast to avoid "'s.
    out->write((const char *)name);
    this->writeValue(out);

    unsigned int flags = 0;
    if (this->isIgnored()) flags |= 0x01;
    if (this->isDefault()) flags |= 0x04;
    // FIXME: more flags? 19990629 mortene.
    out->write(flags);

    assert(!this->isConnected() &&
           "FIXME: binary export of connections not implemented yet");
  }
}

/*!
  FIXME: write doc
 */
void
SoField::countWriteRefs(SoOutput * /* out */) const
{
  COIN_STUB();
}

/*!
  Re-evaluates the value of this field any time a getValue() call is made
  and the field is marked dirty. This is done in this way to gain the
  advantages of having lazy evaluation.
 */
void
SoField::evaluate(void) const
{
  // Cast away the const. evaluate() must be const, since we're using
  // evaluate() from getValue().
  SoField * f = (SoField *)this;

  if (f->getDirty() && f->statusflags.isevaluating == FALSE) {
    if (f->isConnected()) {
      f->statusflags.isevaluating = TRUE;
      f->evaluateConnection();
      f->statusflags.isevaluating = FALSE;
    }
    f->setDirty(FALSE);
  }
}

/*!
  Do we need re-evaluation?
*/
SbBool
SoField::getDirty(void)
{
  return this->statusflags.needevaluation;
}

/*!
  Mark field for re-evaluation, but do not trigger it.
*/
void
SoField::setDirty(SbBool dirty)
{
  this->statusflags.needevaluation = dirty ? 1 : 0;
}

/*!
  Connect ourself as slave to another object. This method
  is simply a wrapper around connectFrom().

  \sa connectFrom()
 */
SbBool
SoField::appendConnection(SoEngineOutput * master, SbBool notnotify)
{
  return this->connectFrom(master, notnotify, TRUE);
}

/*!
  Connect ourself as slave to another object. This method
  is simply a wrapper around connectFrom().

  \sa connectFrom()
 */
SbBool
SoField::appendConnection(SoField * master, SbBool notnotify)
{
  return this->connectFrom(master, notnotify, TRUE);
}

/*!
  Connect ourself as slave to another object. This method
  is simply a wrapper around connectFrom().

  \sa connectFrom()
 */
SbBool
SoField::appendConnection(SoVRMLInterpOutput * master, SbBool notnotify)
{
  return this->connectFrom(master, notnotify, TRUE);
}

/*!
  FIXME: write function documentation
*/
SbBool
SoField::createConverter(SoType fromType, SoType toType,
                         SoFieldConverter *& conv)
{
  if (fromType == toType) return TRUE;

  conv = SoDB::createConverter(fromType, toType);
  if (conv) return TRUE;
  else return FALSE;
}

/*!
  \internal
*/
void
SoField::doConnect(SoField * master, SbBool notify)
{
  if (!this->hasExtendedStorage()) {
    this->storage = new SoConnectStorage(this->container);
    this->statusflags.extstorage = TRUE;
  }

  this->storage->addConnection(master);
  master->addAuditor(this, SoNotRec::FIELD);

  if (notify) {
    if (this->isConnectionEnabled()) {
      this->setDirty(TRUE);
      this->setDefault(FALSE);
      this->startNotify();
    }
  }
}

/*!
  FIXME: write function documentation
*/
void
SoField::doConnect(SoEngineOutput * master, SbBool notify)
{
  if (!this->hasExtendedStorage()) {
    this->storage = new SoConnectStorage(this->container);
    this->statusflags.extstorage = TRUE;
  }

  this->storage->addConnection(master);
  master->getContainer()->addAuditor(this, SoNotRec::ENGINE);
  master->addConnection(this);

  if (notify) {
    if (this->isConnectionEnabled()) {
      this->setDirty(TRUE);
      this->setDefault(FALSE);
      this->startNotify();
    }
  }
}

/*!
  FIXME: write function documentation
*/
void
SoField::doConnect(SoVRMLInterpOutput * master, SbBool notify)
{
  if (!this->hasExtendedStorage()) {
    this->storage = new SoConnectStorage(this->container);
    this->statusflags.extstorage = TRUE;
  }

  this->storage->addConnection(master);
  master->getContainer()->addAuditor(this, SoNotRec::INTERP);
  master->addConnection(this);

  if (notify) {
    if (this->isConnectionEnabled()) {
      this->setDirty(TRUE);
      this->setDefault(FALSE);
      this->startNotify();
    }
  }
}

/*!
  FIXME: write function documentation
*/
void *
SoField::reallocOutputBuf(void * buffer, size_t newsize)
{
  return realloc(buffer, newsize);
}

/*!
  FIXME: write function documentation
*/
SbBool
SoField::readConnection(SoInput * /* in */)
{
  COIN_STUB();
  return FALSE;
}

/*!
  Write out information about this field's connection.
*/
void
SoField::writeConnection(SoOutput * out) const
{
  SoField * fieldmaster;
  SoFieldContainer * fieldcont = NULL;
  SoEngineOutput * enginemaster;
  SoEngine * engine = NULL;
  SoVRMLInterpOutput * interpmaster;
  SoVRMLInterpolator * interpolator = NULL;
  SbName name;

  if (this->getConnectedField(fieldmaster)) {
    fieldcont = fieldmaster->getContainer();
    // FIXME: won't this fail with global fields? Check and
    // fix. 19990707 mortene.
    assert(fieldcont);

    // This will trigger if the field container is not in the
    // graph. If this is the case, we just discards the connection
    // (the master's value will be fetched and written, though).
    if (!fieldcont->shouldWrite()) return;

    SbBool ok = fieldcont->getFieldName(fieldmaster, name);
    assert(ok);
  }
  else if (this->getConnectedEngine(enginemaster)) {
    engine = enginemaster->getContainer();
    assert(engine);
    SbBool ok = engine->getOutputName(enginemaster, name);
    assert(ok);
  }
  else if (this->getConnectedVRMLInterp(interpmaster)) {
    interpolator = interpmaster->getContainer();
    assert(interpolator);
    SbBool ok = interpolator->getOutputName(interpmaster, name);
    assert(ok);
  }
  else assert(0);

  out->write(" =");

  if (fieldcont) {
    SoWriteAction wa(out);
    wa.continueToApply((SoNode *)fieldcont);
  }
  else if (engine) {
    COIN_STUB();
  }
  else if (interpolator) {
    COIN_STUB();
  }
  else assert(0);

  out->indent();
  out->write(". ");

  out->write(name.getString());
}

/*!
  \internal

  If we're connected to a field/engine/interpolator, copy the value.
 */
void
SoField::evaluateConnection(void) const
{
  // FIXME: should we evaluate from all masters in turn? 19990623 mortene.

  if (this->isConnectedFromField()) {
    int idx = this->storage->getNumFieldConnections() - 1;
    ((SoField *)(this))->copyFrom(*(this->storage->getFieldConnection(idx)));
  }
  else if (this->isConnectedFromEngine()) {
    int idx = this->storage->getNumEngineConnections() - 1;
    ((SoEngineOutput *)this->storage->getEngineConnection(idx))->getContainer()->startEvaluate();
  }
  else if (this->isConnectedFromVRMLInterp()) {
    COIN_STUB();
  }
  else {
    // Should never happen.
    assert(0);
  }
}

/*!
  \internal

  This method is always called whenever the field's value has been
  changed by direct invocation of setValue() or somesuch. You should
  \e never call this method from anywhere in the code where the field
  value is being set through an evaluation of its connections.

  If \a resetDefault is \a TRUE, the flag marking whether or not the
  field has its default value will be set to \a FALSE.

  The method will also notify any auditors that its value has changed.
 */
void
SoField::valueChanged(SbBool resetDefault)
{
  this->setDirty(FALSE);
  if (resetDefault) this->setDefault(FALSE);
  if (this->container) this->startNotify();
}

/*!
  \internal

  Notify any auditors by marking them dirty - i.e. ready for re-evaluation.
  Auditors include connected fields, sensors, container (node/engine),
  (TODO: more here)
 */
void
SoField::notifyAuditors(SoNotList * list)
{
  if (!this->hasExtendedStorage() && this->container)
    this->container->notify(list);
  else if (this->storage->getNumAuditors())
    this->storage->getAuditors().notify(list);
}

/*!
  FIXME: write function documentation
*/
void
SoField::setFieldType(int /* flagValue */)
{
  COIN_STUB();
}

/*!
  FIXME: write function documentation
*/
int
SoField::getFieldType(void) const
{
  COIN_STUB();
  return 0;
}

/*!
  Initialize all the field classes.
*/
void
SoField::initClasses(void)
{
  SoSField::initClass();
  SoSFBool::initClass();
  SoSFColor::initClass();
  SoSFEngine::initClass();
  SoSFFloat::initClass();
  SoSFShort::initClass();
  SoSFUShort::initClass();
  SoSFInt32::initClass();
  SoSFUInt32::initClass();
  SoSFVec2f::initClass();
  SoSFVec3f::initClass();
  SoSFVec4f::initClass();
  SoSFMatrix::initClass();
  SoSFEnum::initClass();
  SoSFBitMask::initClass();
  SoSFImage::initClass();
  SoSFName::initClass();
  SoSFNode::initClass();
  SoSFPath::initClass();
  SoSFPlane::initClass();
  SoSFRotation::initClass();
  SoSFString::initClass();
  SoSFTime::initClass();
  SoSFTrigger::initClass();
  SoMField::initClass();
  SoMFBool::initClass();
  SoMFColor::initClass();
  SoMFEngine::initClass();
  SoMFEnum::initClass();
  SoMFBitMask::initClass();
  SoMFFloat::initClass();
  SoMFInt32::initClass();
  SoMFMatrix::initClass();
  SoMFName::initClass();
  SoMFNode::initClass();
  SoMFPath::initClass();
  SoMFPlane::initClass();
  SoMFRotation::initClass();
  SoMFShort::initClass();
  SoMFString::initClass();
  SoMFTime::initClass();
  SoMFUInt32::initClass();
  SoMFUShort::initClass();
  SoMFVec2f::initClass();
  SoMFVec3f::initClass();
  SoMFVec4f::initClass();

  // Create these obsoleted types for backwards compatibility. They
  // are typedef'ed to the types which obsoleted them, but this is
  // needed so it will also be possible to use SoType::fromName() with
  // the old names and create instances in that manner.
  //
  // FIXME: SoType::fromName("oldname") == SoType::fromName("newname")
  // will fail, but this can be solved with a hack in
  // SoType::operator==(). Do we _want_ to implement this hack,
  // though? It'd be ugly as hell.  19991109 mortene.

  SoType::createType(SoSField::getClassTypeId(), "SFLong",
                     &SoSFInt32::createInstance);
  SoType::createType(SoSField::getClassTypeId(), "SFULong",
                     &SoSFUInt32::createInstance);
  SoType::createType(SoMField::getClassTypeId(), "MFLong",
                     &SoMFInt32::createInstance);
  SoType::createType(SoMField::getClassTypeId(), "MFULong",
                     &SoMFUInt32::createInstance);
}
