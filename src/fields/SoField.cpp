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
  \class SoField SoField.h Inventor/fields/SoField.h
  \brief The SoField class is the top-level abstract base class for fields.
  \ingroup fields

  Fields is the mechanism used throughout Coin for encapsulating basic
  data types to detect changes made to them, and to provide
  conversion, import and export facilities. All public properties in
  nodes are stored in fields, and so are the inputs and outputs of
  engines.

  Forcing data modification to go through a public function interface
  while hiding the data members makes it possible to automatically
  detect and react upon changes in the user-specified data
  structures. E.g. the default behavior when changing the value of a
  field in a scene graph node is that there'll automatically be a
  rendering update.

  Note: there are some field classes which has been obsoleted from the
  Open Inventor API. They are: SoSFLong, SoSFULong, SoMFLong and
  SoMFULong. You should use these classes instead (respectively):
  SoSFInt32, SoSFUInt32, SoMFInt32 and SoMFUInt32.


  \sa SoFieldContainer, SoFieldData
*/


// Metadon doc:
/*¡
  Some code for various handling of connections to/from VRML
  interpolators is missing. Do a "grep" for FIXME and COIN_STUB to
  find and fix these cases when implementing the VRML support.
*/

#include <assert.h>
#include <coindefs.h> // COIN_STUB()

#include <Inventor/SoDB.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/VRMLnodes/SoVRMLInterpOutput.h>
#include <Inventor/VRMLnodes/SoVRMLInterpolator.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/engines/SoConvertAll.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/errors/SoReadError.h>
#include <Inventor/fields/SoFields.h>
#include <Inventor/lists/SoEngineList.h>
#include <Inventor/lists/SoEngineOutputList.h>
#include <Inventor/lists/SoVRMLInterpOutputList.h>
#include <Inventor/sensors/SoDataSensor.h>

static const char IGNOREDCHAR = '~';
static const char CONNECTIONCHAR = '=';

/*
  This class is used to aid in "multiplexing" the pointer member of
  SoField. This is a way to achieve the goal of using minimum storage
  space for SoField classes in the default case (which is important,
  as fields are ubiquitous in Coin). The default case means no
  connections and only a field container given. If any connections are
  made (either "to" or "from"), we allocate an SoConnectStorage and
  move the field container pointer into it, while swapping in the
  SoConnectStorage pointer where the field container pointer used to
  be.
*/
class SoConnectStorage {
public:
  SoConnectStorage(SoFieldContainer * c)
    : container(c)
    { }

#if COIN_DEBUG
  // Check that everything has been emptied.
  ~SoConnectStorage()
  {
    SbPList keys, values;
    maptoconverter.makePList(keys, values);
    assert(keys.getLength() == 0);
    assert(values.getLength() == 0);

    assert(masterfields.getLength() == 0);
    assert(masterengineouts.getLength() == 0);
    assert(masterinterpouts.getLength() == 0);

    assert(slaves.getLength() == 0);
    assert(auditors.getLength() == 0);
  }
#endif // COIN_DEBUG

  // The container this field is part of.
  SoFieldContainer * container;

  // List of masters we're connected to as a slave. Use maptoconverter
  // dict to find SoFieldConverter engine in the connection (if any).
  SoFieldList masterfields;
  SoEngineOutputList masterengineouts;
  SoVRMLInterpOutputList masterinterpouts;
  // Fields which are slaves to us. Use maptoconverter dict to find
  // SoFieldConverter engine in the connection (if any).
  SoFieldList slaves;
  // Direct auditors of us.
  SoAuditorList auditors;
  // Dictionary of void* -> SoFieldConverter* mappings.
  SbDict maptoconverter;


  // Convenience functions for adding, removing and finding SbDict
  // mappings.

  void addConverter(const void * item, SoFieldConverter * converter)
  {
    // "item" can be SoField* or SoEngineOutput*.
    this->maptoconverter.enter((const unsigned long)item, converter);
  }

  void removeConverter(const void * item)
  {
    SbBool ok = this->maptoconverter.remove((const unsigned long)item);
    assert(ok);
  }

  SoFieldConverter * findConverter(const void * item)
  {
    void * val;
    if (!this->maptoconverter.find((const unsigned long)item, val))
      return NULL;
    return (SoFieldConverter *)val;
  }
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
  Copy value(s) from \a f into this field.
*/
/*!
  \fn SbBool SoField::readValue(SoInput * in)
  Read field value(s).
*/
/*!
  \fn void SoField::writeValue(SoOutput * out) const
  Write field value(s).
*/


// *************************************************************************

SoType SoField::classTypeId = SoType::badType();

// *************************************************************************

/*!
  This is the base constructor for field classes. It takes care of
  doing the common parts of data initialization in fields.
*/
SoField::SoField(void)
  : container(NULL)
{
  this->statusflags.donotify = TRUE;

  this->statusflags.ignore = FALSE;
  this->statusflags.isdefault = TRUE;
  this->statusflags.extstorage = FALSE;
  this->statusflags.enableconnects = TRUE;
  this->statusflags.needevaluation = FALSE;
  this->statusflags.isevaluating = FALSE;
  this->statusflags.type = 0;
  this->statusflags.readonly = 0;
}

/*!
  Destructor. Disconnects ourself from any connected field, engine or
  VRML interpolator, before we disconnect all auditors on the field.
*/
SoField::~SoField()
{
#if COIN_DEBUG && 0 // debug
  SoDebugError::postInfo("SoField::~SoField", "destructing %p", this);
#endif // debug

  // Disconnect ourself from all connections where this field is the
  // slave.
  this->disconnect();

  if (this->hasExtendedStorage()) {

    // Disconnect slave fields using us as a master.
    while (this->storage->slaves.getLength()) {
      this->storage->slaves[0]->disconnect(this);
    }

    // Disconnect other auditors.
    while (this->storage->auditors.getLength()) {
      SoNotRec::Type type = this->storage->auditors.getType(0);
      void * obj = this->storage->auditors.getObject(0);

      switch (type) {
      case SoNotRec::ENGINE:
        ((SoEngineOutput *)obj)->removeConnection(this);
        break;

      case SoNotRec::INTERP:
        ((SoVRMLInterpOutput *)obj)->removeConnection(this);
        break;

      case SoNotRec::CONTAINER:
        assert(FALSE && "Container should not be in auditorlist");
        break;

      case SoNotRec::SENSOR:
        ((SoDataSensor *)obj)->dyingReference();
        break;

      case SoNotRec::FIELD:
        assert(FALSE); // should not happen, as slave fields are removed first.
        break;

      default:
        assert(FALSE); // no other allowed types.
        break;
      }
    }

    delete this->storage;
  }

#if COIN_DEBUG && 0 // debug
  SoDebugError::postInfo("SoField::~SoField", "%p done", this);
#endif // debug
}

/*!
  Must be called upon initialization of the library to set up the type
  system.
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
  Set whether or not this field should be marked as containing a
  default value.

  \sa isDefault()
*/
void
SoField::setDefault(SbBool def)
{
  this->statusflags.isdefault = def;
}

/*!
  Check if the field contains its default value. Fields which has
  their default value intact will normally not be included in the
  output when writing scene graphs out to a file, for instance.

  \sa setDefault()
*/
SbBool
SoField::isDefault(void) const
{
  return this->statusflags.isdefault;
}

/*!
  Returns a unique type identifier for this field class.

  \sa getTypeId(), SoType
*/
SoType
SoField::getClassTypeId(void)
{
  return SoField::classTypeId;
}

/*!
  Check if this instance is of a derived type or is the same type as
  the one given with the \a type parameter.
*/
SbBool
SoField::isOfType(const SoType type) const
{
  return this->getTypeId().isDerivedFrom(type);
}

/*!
  This sets a \a flag value which indicates whether or not the set up
  connection should be considered active. For as long as the "enable
  connection" flag is \c FALSE, no value propagation will be done from
  any connected source field, engine or interpolator into this field.

  If the connection is first disabled and then enabled again, the
  field will automatically be synchronized with any master field,
  engine or interpolator.

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
  Connects this field as a slave to \a master. This means that the
  value of this field will be automatically updated when \a master is
  changed (as long as the connection also is enabled).

  If the field connected \e from has a different type from the field
  connected \e to, a field converter is inserted. For some
  combinations of fields no such conversion is possible, and we'll
  return \c FALSE.

  If this field had any connections to master fields beforehand, these
  are all broken up if \a append is \c FALSE.

  Call with \a notnotify if you want to avoid the initial notification
  of connected auditors (a.k.a. \e slaves).

  \sa enableConnection(), isConnectionEnabled(), isConnectedFromField()
  \sa getConnectedField(), appendConnection(SoField *)
*/
SbBool
SoField::connectFrom(SoField * master, SbBool notnotify, SbBool append)
{
  // Initialize.  /////////////////////////////////////////////////

  this->extendStorageIfNecessary();
  master->extendStorageIfNecessary();

  SoType mastertype = master->getTypeId();
  SoType thistype = this->getTypeId();
  SoType containertype = this->getContainer()->getTypeId();
  SbBool containerisconverter =
    containertype.isDerivedFrom(SoFieldConverter::getClassTypeId());


  // Set up all links.  ///////////////////////////////////////////

  if (mastertype == thistype) { // Can do direct field-to-field link.
    if (!append) this->disconnect();
    // Set up the auditor link from the master to the slave field.
    // (Note that the ``this'' slave field can also be an input field
    // of an SoFieldConverter instance.)
    master->addAuditor(this, SoNotRec::FIELD);
  }
  else { // Needs an SoFieldConverter between the fields.
    SoFieldConverter * conv = this->createConverter(mastertype);
    if (!conv) return FALSE;

    if (!append) this->disconnect();

    // Link up the input SoField of the SoFieldConverter to the master
    // field by recursively calling connectFrom().
    SoField * converterinput = conv->getInput(mastertype);
    converterinput->connectFrom(master, notnotify);

    // Connect from the SoFieldConverter output to the slave field.
    SoEngineOutput * converteroutput = conv->getOutput(thistype);
    converteroutput->addConnection(this);

    // Remember the connection from the slave field to the
    // SoFieldConverter by setting up a dict entry.
    this->storage->addConverter(master, conv);
  }

  // Common bookkeeping.
  this->storage->masterfields.append(master); // slave -> master link
  if (!containerisconverter)
    master->storage->slaves.append(this); // master -> slave link


  // Notification.  ///////////////////////////////////////////////

  if ((notnotify == FALSE) && this->isConnectionEnabled()) {
    this->setDirty(TRUE);
    this->setDefault(FALSE);
    this->startNotify();
  }

  return TRUE;
}

/*!
  Connects this field as a slave to \a master. This means that the value
  of this field will be automatically updated when \a master is changed (as
  long as the connection also is enabled).

  If the field output connected \e from is of a different type from
  the field connected \e to, a field converter is inserted. For some
  combinations of fields no such conversion is possible, and we'll
  return \c FALSE.

  If this field had any master-relationships beforehand, these are all
  broken up if \a append is \c FALSE.

  Call with \a notnotify if you want to avoid the initial notification
  of connected auditors (a.k.a. \e slaves).

  \sa enableConnection(), isConnectionEnabled(), isConnectedFromField()
  \sa getConnectedField(), appendConnection(SoEngineOutput *)
*/
SbBool
SoField::connectFrom(SoEngineOutput * master, SbBool notnotify, SbBool append)
{
  // Initialize.  /////////////////////////////////////////////////

  this->extendStorageIfNecessary();

  SoType mastertype = master->getConnectionType();
  SoType thistype = this->getTypeId();


  // Set up all links.  ///////////////////////////////////////////

  if (mastertype == thistype) { // Can do direct field-to-engineout link.
    if (!append) this->disconnect();
    // Set up the auditor link from the master engineout to the slave
    // field.  (Note that the ``this'' slave field can also be an
    // input field of an SoFieldConverter instance.)

    // This is enough, the container SoEngine will automatically pick
    // up on it.
    master->addConnection(this);
  }
  else { // Needs an SoFieldConverter between this field and the SoEngineOutput
    SoFieldConverter * conv = this->createConverter(mastertype);
    if (!conv) return FALSE;

    if (!append) this->disconnect();

    // Link up the input SoField of the SoFieldConverter to the master
    // SoEngineOutput by recursively calling connectFrom().
    SoField * converterinput = conv->getInput(mastertype);
    converterinput->connectFrom(master, notnotify);

    // Connect from the SoFieldConverter output to the slave field.
    SoEngineOutput * converteroutput = conv->getOutput(thistype);
    converteroutput->addConnection(this);

    // Remember the connection from the slave field to the
    // SoFieldConverter by setting up a dict entry.
    this->storage->addConverter(master, conv);
  }

  // Common bookkeeping.
  this->storage->masterengineouts.append(master); // slave -> master link


  // Notification.  ///////////////////////////////////////////////

  if ((notnotify == FALSE) && this->isConnectionEnabled()) {
    this->setDirty(TRUE);
    this->setDefault(FALSE);
    this->startNotify();
  }

  return TRUE;
}


/*!
  Connects this field as a slave to \a master. This means that the value
  of this field will be automatically updated when \a master is changed (as
  long as the connection also is enabled).

  If the field output connected \e from is of a different type from
  the field connected \e to, a field converter is inserted. For some
  combinations of fields no such conversion is possible, and we'll
  return \c FALSE.

  If this field had any master-relationships beforehand, these are all
  broken up if \a append is \c FALSE.

  Call with \a notnotify if you want to avoid the initial notification
  of connected auditors (a.k.a. \e slaves).

  \sa enableConnection(), isConnectionEnabled(), isConnectedFromField()
  \sa getConnectedField(), appendConnection(SoVRMLInterpOutput *)
*/
SbBool
SoField::connectFrom(SoVRMLInterpOutput * master,
                     SbBool notnotify, SbBool append)
{
  // I believe the code will be more or less identical with
  // SoField::disconnect(SoEngineOutput *). 20000411 mortene.
  COIN_STUB(); // FIXME
  return FALSE;
}


/*!
  Disconnect this field as a slave from \a master.
*/
void
SoField::disconnect(SoField * master)
{
#if COIN_DEBUG && 0 // debug
  SoDebugError::postInfo("SoField::disconnect",
                         "removing slave field %p from master field %p",
                         this, master);
#endif // debug

  this->evaluate();

  SoType containertype = this->getContainer()->getTypeId();
  SbBool containerisconverter =
    containertype.isDerivedFrom(SoFieldConverter::getClassTypeId());


  // Decouple links. ///////////////////////////////////////////////////

  // Remove bookkeeping material.
  if (!containerisconverter) master->storage->slaves.removeItem(this);
  this->storage->masterfields.removeItem(master);

  SoFieldConverter * converter = this->storage->findConverter(master);
  if (converter) { // There's a converter engine between the fields.
//      SoField * converterinput = converter->getInput(master->getTypeId());
    SoField * converterinput = converter->getInput(SoType::badType()); // FIXME: type
    converterinput->disconnect(master);

    SoEngineOutput * converteroutput = converter->getOutput(this->getTypeId());
    converteroutput->removeConnection(this);

    this->storage->removeConverter(master);
  }
  else { // No converter, just a direct link.
    master->removeAuditor(this, SoNotRec::FIELD);
  }
}

/*!
  Disconnect this field as a slave from \a master.
*/
void
SoField::disconnect(SoEngineOutput * master)
{
#if COIN_DEBUG && 0 // debug
  SoDebugError::postInfo("SoField::disconnect",
                         "removing slave field %p from master engineout %p",
                         this, master);
#endif // debug

  this->evaluate();


  // Decouple links. ///////////////////////////////////////////////////

  // Remove bookkeeping material.
  this->storage->masterengineouts.removeItem(master);

  SoFieldConverter * converter = this->storage->findConverter(master);
  if (converter) { // There's a converter engine between the fields.
    SoField * converterinput = converter->getInput(master->getConnectionType());
    converterinput->disconnect(master);

    SoEngineOutput * converteroutput = converter->getOutput(this->getTypeId());
    converteroutput->removeConnection(this);

    this->storage->removeConverter(master);
  }
  else { // No converter, just a direct link.
    master->removeConnection(this);
  }
}

/*!
  Disconnect this field as a slave from \a master.
*/
void
SoField::disconnect(SoVRMLInterpOutput * master)
{
  // I believe the code will be more or less identical with
  // SoField::disconnect(SoEngineOutput *). 20000411 mortene.
  COIN_STUB(); // FIXME
}

/*!
  Returns number of fields this field is a slave of.

  \sa getConnections()
*/
int
SoField::getNumConnections(void) const
{
  return this->hasExtendedStorage() ?
    this->storage->masterfields.getLength() : 0;
}

/*!
  Returns number of masters this field is connected to, and places
  pointers to all of them into \a masterlist.

  Note that we replace the contents of \a masterlist, i.e. we're \e
  not appending new data.

  \sa getNumConnections()
*/
int
SoField::getConnections(SoFieldList & masterlist) const
{
  if (!this->hasExtendedStorage()) return 0;

  masterlist = this->storage->masterfields;
  return masterlist.getLength();
}

/*!
  Disconnect all connections from this field as a slave to master
  fields, engine outputs or VRML interpolator outputs.
*/
void
SoField::disconnect(void)
{
  // Disconnect us from all master fields.
  while (this->isConnectedFromField())
    this->disconnect(this->storage->masterfields[0]);

  // Disconnect us from all master engine outputs.
  while (this->isConnectedFromEngine())
    this->disconnect(this->storage->masterengineouts[0]);

  // Disconnect us from all master VRML interpolator outputs.
  while (this->isConnectedFromVRMLInterp())
    this->disconnect(this->storage->masterinterpouts[0]);

  assert(this->isConnected() == FALSE);
}

/*!
  Returns \c TRUE if we're connected from another field, engine or
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
  Returns \c TRUE if we're a slave of at least one field.

  \sa isConnected(), isConnectedFromEngine(), isConnectedFromVRMLInterp()
  \sa connectFrom(SoField *)
*/
SbBool
SoField::isConnectedFromField(void) const
{
  return (this->hasExtendedStorage() &&
          this->storage->masterfields.getLength() > 0);
}

/*!
  Returns \c TRUE if we're a slave of at least one VRML interpolator.

  \sa isConnected(), isConnectedFromEngine(), isConnectedFromField()
  \sa connectFrom(SoVRMLInterpOutput *)
*/
SbBool
SoField::isConnectedFromVRMLInterp(void) const
{
  return (this->hasExtendedStorage() &&
          this->storage->masterinterpouts.getLength() > 0);
}

/*!
  Returns \c TRUE if we're connected from an engine.

  \sa isConnected(), isConnectedFromVRMLInterp(), isConnectedFromField()
  \sa connectFrom(SoEngineOutput *)
*/
SbBool
SoField::isConnectedFromEngine(void) const
{
  return (this->hasExtendedStorage() &&
          this->storage->masterengineouts.getLength() > 0);
}

// Simplify by collecting common code for SoField::getConnected*() methods.
#define SOFIELD_GETCONNECTED(_masterlist_) \
  if (!this->hasExtendedStorage()) return FALSE; \
  int nrmasters = this->storage->_masterlist_.getLength(); \
  if (nrmasters < 1) return FALSE; \
  master = this->storage->_masterlist_[nrmasters - 1]; \
  return TRUE

/*!
  Returns \c TRUE if we are connected as a slave to at least one other
  field.  \a master will be set to the source field in the last field
  connection made.

  \sa isConnectedFromField(), connectFrom(SoField *),
  \sa appendConnection(SoField *)
*/
SbBool
SoField::getConnectedField(SoField *& master) const
{
  SOFIELD_GETCONNECTED(masterfields);
}


/*!
  Returns \c TRUE if we are connected as a slave to at least one VRML
  interpolator. \a master will be set to the source of the last
  interpolator connection made.

  \sa isConnectedFromVRMLInterp(), connectFrom(SoVRMLInterpOutput *)
  \sa appendConnection(SoVRMLInterpOutput *)
*/
SbBool
SoField::getConnectedVRMLInterp(SoVRMLInterpOutput *& master) const
{
  SOFIELD_GETCONNECTED(masterinterpouts);
}

/*!
  Returns \c TRUE if we are connected as a slave to at least one
  engine. \a master will be set to the source of the last engine
  connection made.

  \sa isConnectedFromEngine(), connectFrom(SoEngineOutput *)
  \sa appendConnection(SoEngineOutput *)
*/
SbBool
SoField::getConnectedEngine(SoEngineOutput *& master) const
{
  SOFIELD_GETCONNECTED(masterengineouts);
}

#undef SOFIELD_GETCONNECTED

/*!
  Appends all the fields which are auditing this field in \a
  slavelist, and returns the number of fields which are our slaves.
*/
int
SoField::getForwardConnections(SoFieldList & slavelist) const
{
  if (!this->hasExtendedStorage()) return 0;

  int nr = 0;

  for (int i=0; i < this->storage->slaves.getLength(); i++) {
    slavelist.append(this->storage->slaves[i]);
    nr++;
  }

  return nr;
}

/*!
  Let the field know to which container it belongs.

  \sa getContainer(), SoFieldContainer
*/
void
SoField::setContainer(SoFieldContainer * cont)
{
  if (!this->hasExtendedStorage()) this->container = cont;
  else this->storage->container = cont;

  // The field should have been set to its default value before it is
  // added to the container.
  //
  // This might seem strange, but it looks like it is necessary to do
  // it this way to be compatible with Open Inventor.
  this->setDefault(TRUE);
}

/*!
  Returns the SoFieldContainer object "owning" this field.

  \sa SoFieldContainer, setContainer()
*/
SoFieldContainer *
SoField::getContainer(void) const
{
  if (!this->hasExtendedStorage()) return this->container;
  else return this->storage->container;
}

/*!
  Set the field's value through the given \a valuestring. The format
  of the string must adhere to the ASCII format used in Coin data
  format files.

  Only the value should be specified - \e not the name of the field.

  \c FALSE is returned if the field value is invalid for the field
  type and can't be parsed in any sensible way.

  \sa get()
*/
SbBool
SoField::set(const char * valuestring)
{
  // Note that it is not necessary to set a header identification line
  // for this to work.
  SoInput in;
  in.setBuffer((void *)valuestring, strlen(valuestring));
  if (!this->readValue(&in)) return FALSE;

  this->valueChanged();
  return TRUE;
}

/*!
  Returns the field's value as an ASCII string in the export data
  format for Inventor files.

  \sa set()
*/
void
SoField::get(SbString & valuestring)
{
  // Note: this code has an almost verbatim copy in SoMField::get1(),
  // so remember to update both places if any fixes are done.

  SoOutput out;
  // FIXME: wouldn't it be better to empty the headerstring like this
  // than doing the stripping later on? 20000312 mortene.
//    out.setHeaderString("");
  const size_t STARTSIZE = 32;
  void * buffer = malloc(STARTSIZE);

  out.setBuffer(buffer, STARTSIZE, realloc);
  this->writeValue(&out);

  size_t size;
  out.getBuffer(buffer, size);
  // Strip off header.
  char * start = strstr((char *)buffer, "\n\n");
  assert(start != NULL);
  start += 2;
  valuestring = start;
  free(buffer);
}

/*!
  Mark the field as changed. Touching a field which is part of any
  component (engine or node) in a scene graph will lead to a forced
  redraw. This is useful if you have been doing several updates to the
  field wrapped in a pair of enableNotify() calls to notify the
  field's auditors that its value has changed.

  \sa setValue(), enableNotify()
*/
void
SoField::touch(void)
{
  if (this->container) this->startNotify();
}

/*!
  Trigger a notification sequence.

  At the end of a notification sequence, all "immediate" sensors
  (i.e. sensors set up with a zero priority) are triggered.
*/
void
SoField::startNotify(void)
{
  SoNotList l;
#if COIN_DEBUG && 0 // debug
  SoDebugError::postInfo("SoField::startNotify", "field %p (%s), list %p",
                         this, this->getTypeId().getName().getString(), &l);
#endif // debug

  this->notify(&l);

  // Process zero-priority sensors after notification has been done.
  SoSensorManager * sm = SoDB::getSensorManager();
  if (sm->isDelaySensorPending()) sm->processImmediateQueue();

#if COIN_DEBUG && 0 // debug
  SoDebugError::postInfo("SoField::startNotify", "DONE\n\n");
#endif // debug
}

/*!
  Notify auditors that this field has changed.
*/
void
SoField::notify(SoNotList * nlist)
{
#if COIN_DEBUG && 0 // debug
  SoDebugError::postInfo("SoField::notify", "%p (%s (%s '%s')) -- start",
                         this,
                         this->getTypeId().getName().getString(),
                         this->getContainer() ? this->getContainer()->getTypeId().getName().getString() : "*none*",
                         this->getContainer() ? this->getContainer()->getName().getString() : "*none*");
#endif // debug

  // If we're not the originator of the notification process, we need
  // to be marked dirty, as it means something we're connected to as a
  // slave has changed and our value needs to be updated.
  if (nlist->getFirstRec()) this->setDirty(TRUE);

  if (this->isNotifyEnabled()) {
    SoNotRec rec(this->getContainer());
    nlist->append(&rec, this);
    nlist->setLastType(SoNotRec::CONTAINER); // FIXME: Not sure about this. 20000304 mortene.

#if COIN_DEBUG && 0 // debug
    SoDebugError::postInfo("SoField::notify",
                           "field %p, list %p", this, nlist);
#endif // debug
    if (this->getContainer()) this->getContainer()->notify(nlist);
    this->notifyAuditors(nlist);
  }

#if COIN_DEBUG && 0 // debug
  SoDebugError::postInfo("SoField::notify", "%p (%s (%s '%s')) -- done",
                         this,
                         this->getTypeId().getName().getString(),
                         this->getContainer() ? this->getContainer()->getTypeId().getName().getString() : "*none*",
                         this->getContainer() ? this->getContainer()->getName().getString() : "*none*");
#endif // debug
}

/*!
  This method sets whether notification will be propagated on changing
  the value of the field.  The old value of the setting is returned.

  \sa isNotifyEnabled()
*/
SbBool
SoField::enableNotify(SbBool on)
{
  const SbBool old = this->statusflags.donotify;
  this->statusflags.donotify = on;
  return old;
}

/*!
  This method returns whether notification of changes to the field
  value are propagated to the auditors.

  \sa enableNotify()
*/
SbBool
SoField::isNotifyEnabled(void) const
{
  return this->statusflags.donotify;
}

// Makes an extended storage block on first connection.
void
SoField::extendStorageIfNecessary(void)
{
  if (!this->hasExtendedStorage()) {
    this->storage = new SoConnectStorage(this->container);
    this->statusflags.extstorage = TRUE;
  }
}

/*!
  Add an auditor to the list. All auditors will be notified whenever
  this field changes its value(s).
*/
void
SoField::addAuditor(void * f, SoNotRec::Type type)
{
  this->extendStorageIfNecessary();
  this->storage->auditors.append(f, type);
  this->connectionStatusChanged(+1);
}

/*!
  Remove an auditor from the list.
*/
void
SoField::removeAuditor(void * f, SoNotRec::Type type)
{
#if COIN_DEBUG && 0 // debug
  SoDebugError::postInfo("SoField::removeAuditor",
                         "%p removing %p", this, f);
#endif // debug

  assert(this->hasExtendedStorage());
  this->storage->auditors.remove(f, type);
  this->connectionStatusChanged(-1);
}

/*!
  Checks for equality. Returns \c 0 if the fields are of different
  type or the field's value(s) are not equal.
*/
int
SoField::operator ==(const SoField & f) const
{
  return this->isSame(f);
}

/*!
  Returns \c TRUE if the fields are of different type or has different
  value.
*/
int
SoField::operator !=(const SoField & f) const
{
  return !this->isSame(f);
}

/*!
  Returns \c TRUE if it is necessary to write the field when dumping a
  scene graph.
*/
SbBool
SoField::shouldWrite(void) const
{
  if (!this->isDefault()) return TRUE;
  if (this->isIgnored()) return TRUE;
  return FALSE;
}

/*!
  Called whenever another slave attaches or detaches itself to us.  \a
  numconnections is the difference in number of connections made
  (i.e. if stuff is \e disconnected, \a numconnections will be a
  negative number).

  The default method is empty. Overload in subclasses if you want do
  something special on connections/deconnections.
*/
void
SoField::connectionStatusChanged(int numconnections)
{
}

/*!
  Returns \c TRUE if this field should not be written into at the
  moment the method is called.
*/
SbBool
SoField::isReadOnly(void) const
{
  return this->statusflags.readonly;
}

/*!
  This method is internally called after copyFrom() during scene graph
  copies, and should do the operations necessary for fixing up the
  field instance after it has gotten a new value.

  The default method in the SoField superclass does nothing.

  The application programmer should normally not need to consider this
  method, unless he constructs a complex field type which contains new
  references to container instances (i.e. nodes or
  engines). Overloading this method is then necessary to update the
  reference pointers, as they could have been duplicated during the
  copy operation.
*/
void
SoField::fixCopy(SbBool copyconnections)
{
}

/*!
  Returns \c TRUE if this field has references to any containers in
  the scene graph which are also duplicated during the copy operation.

  Note that this method \e only is valid to call during copy
  operations.

  See also the note about the relevance of the fixCopy() method for
  application programmers, as it is applicable on this method aswell.
*/
SbBool
SoField::referencesCopy(void) const
{
  SoFieldList masters;
  int nr = this->getConnections(masters);

  for (int i=0; i < nr; i++) {
    SoFieldContainer * fc = masters[i]->getContainer();
    if (SoFieldContainer::checkCopy(fc)) return TRUE;
  }

  return FALSE;
}

/*!
  If \a fromfield contains a connection to another field, make this
  field also use the same connection.
*/
void
SoField::copyConnection(const SoField * fromfield)
{
  // Consider most common case first.
  if (!fromfield->isConnected()) return;

  // FIXME: copy _all_ connections (in preparation for VRML2 support)?
  // 20000116 mortene.

#define COPYCONNECT(_fieldtype_, _getfunc_) \
  _fieldtype_ * master; \
  (void)fromfield->_getfunc_(master); \
  SoFieldContainer * masterfc = master->getContainer(); \
  int ptroffset = (char *)master - (char *)masterfc; \
  SoFieldContainer * copyfc = masterfc->copyThroughConnection(); \
  _fieldtype_ * copyfield = (_fieldtype_ *)((char *)copyfc + ptroffset); \
  (void)this->connectFrom(copyfield)


  // Connections already in place will be automatically removed, as
  // the append argument to connectFrom() is default FALSE.

  if (fromfield->isConnectedFromField()) {
    COPYCONNECT(SoField, getConnectedField);
  }
  else if (fromfield->isConnectedFromEngine()) {
    COPYCONNECT(SoEngineOutput, getConnectedEngine);
  }
  else if (fromfield->isConnectedFromVRMLInterp()) {
    COPYCONNECT(SoVRMLInterpOutput, getConnectedVRMLInterp);
  }

#undef COPYCONNECT
}

/*!
  Reads and sets the value of this field from the given SoInput
  instance.  Returns \c FALSE if the field value can not be parsed
  from the input.

  This field has the \a name given as the second argument.

  \sa set(), write()
*/
SbBool
SoField::read(SoInput * in, const SbName & name)
{
  // This macro is convenient for reading with error detection.
#define READ_VAL(val) \
  if (!in->read(val)) { \
    SoReadError::post(in, "Premature end of file"); \
    return FALSE; \
  }


  this->setDefault(FALSE);
  this->setDirty(FALSE);

  if (!in->isBinary()) { // ASCII file format.
    char c;
    // Check for the ignored flag first, as it is valid to let the
    // field data be just the ignored flag and nothing else.
    READ_VAL(c);
    if (c == IGNOREDCHAR) this->setIgnored(TRUE);
    else {
      in->putBack(c);

      // Read field value(s).
      if (!this->readValue(in)) {
        SoReadError::post(in, "Couldn't read value for field \"%s\"",
                          name.getString());
        return FALSE;
      }

      if (!in->eof()) {  // Can happen for memory buffers with SoField::set().
        // Check again for ignored flag.
        READ_VAL(c);
        if (c == IGNOREDCHAR) this->setIgnored(TRUE);
        else in->putBack(c);
      }
    }

    if (!in->eof()) {  // Can happen for memory buffers with SoField::set().
      // Check if there's a field-to-field connection here.
      READ_VAL(c);
      if (c == CONNECTIONCHAR) { if (!this->readConnection(in)) return FALSE; }
      else in->putBack(c);
    }
  }
  else { // Binary file format.
    // Read field value(s).
    if (!this->readValue(in)) {
      SoReadError::post(in, "Couldn't read value for field \"%s\"",
                        name.getString());
      return FALSE;
    }

    // Check for the "ignored", "connection" and "default" flags.
    unsigned int flags;
    READ_VAL(flags);

    if (flags & SoField::IGNORED) this->setIgnored(TRUE);
    if (flags & SoField::CONNECTED) { if (!this->readConnection(in)) return FALSE; }
    if (flags & SoField::DEFAULT) this->setDefault(TRUE);
#if COIN_DEBUG
    if (flags & ~SoField::ALLFILEFLAGS) {
      SoDebugError::postWarning("SoField::read",
                                "unknown field flags (0x%x) -- ",
                                "please report to coin-bugs@sim.no", flags);
    }
#endif // COIN_DEBUG
  }

#undef READ_VAL

  return TRUE;
}

/*!
  Write the value of the field to the given SoOutput instance (which
  can be either a memory buffer or a file, in ASCII or in binary
  format).

  \sa get(), read(), SoOutput
*/
void
SoField::write(SoOutput * out, const SbName & name) const
{
  if (out->getStage() == SoOutput::COUNT_REFS) {
    // Handle first stage of write operations.
    this->countWriteRefs(out);
    return;
  }

  // Ok, we've passed the first write stage and is _really_ writing.


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

    SbName dummy;
    SoFieldContainer * fc = this->resolveWriteConnection(dummy);
    if (fc && fc->shouldWrite()) this->writeConnection(out);

    out->write('\n');
  }
  // Binary write.
  else {
    // Cast to avoid "'s.
    out->write((const char *)name);
    this->writeValue(out);

    unsigned int flags = 0;
    if (this->isIgnored()) flags |= SoField::IGNORED;
    SbName dummy;
    SoFieldContainer * fc = this->resolveWriteConnection(dummy);
    if (fc && fc->shouldWrite()) flags |= SoField::CONNECTED;
    if (this->isDefault()) flags |= SoField::DEFAULT;

    out->write(flags);

    if (fc && fc->shouldWrite()) this->writeConnection(out);
  }
}

/*!
  This method is called during the first pass of write operations, to
  count the number of write references to this field and to "forward"
  the reference counting operation to the field containers we're
  connected to.
*/
void
SoField::countWriteRefs(SoOutput * out) const
{
  // FIXME: call the "writereference" method of _all_ connected
  // fields, engine outputs (?) and VRML interpolators? 20000130 mortene.
  SbName dummy;
  SoFieldContainer * fc = this->resolveWriteConnection(dummy);
  if (fc) fc->addWriteReference(out, TRUE);
}

/*!
  Re-evaluates the value of this field any time a getValue() call is
  made and the field is marked dirty. This is done in this way to gain
  the advantages of having lazy evaluation.
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
  Connect ourself as slave to another object, while still keeping the
  other connections currently in place.

  \sa connectFrom()
*/
SbBool
SoField::appendConnection(SoEngineOutput * master, SbBool notnotify)
{
  return this->connectFrom(master, notnotify, TRUE);
}

/*!
  Connect ourself as slave to another object, while still keeping the
  other connections currently in place.

  \sa connectFrom()
*/
SbBool
SoField::appendConnection(SoField * master, SbBool notnotify)
{
  return this->connectFrom(master, notnotify, TRUE);
}

/*!
  Connect ourself as slave to another object, while still keeping the
  other connections currently in place.

  \sa connectFrom()
*/
SbBool
SoField::appendConnection(SoVRMLInterpOutput * master, SbBool notnotify)
{
  return this->connectFrom(master, notnotify, TRUE);
}

// Make a converter from value(s) of the given field type and the
// value(s) of this type. Returns NULL if no value conversion between
// types is possible.
SoFieldConverter *
SoField::createConverter(SoType from) const
{
  SoType thistype = this->getTypeId();
  assert(from != thistype);
  SoType convtype = SoDB::getConverter(from, thistype);
  if (convtype == SoType::badType()) {
#if COIN_DEBUG // COIN_DEBUG
    SoDebugError::postWarning("SoField::createConverter",
                              "no converter for %s to %s",
                              from.getName().getString(),
                              thistype.getName().getString());
#endif // COIN_DEBUG
    return NULL;
  }

  // FIXME: doesn't we leak the SoFieldConverter instance? 20000409 mortene.

  // FIXME: is it really wise to treat the SoConvertAll field
  // converters as a special case? 20000217 mortene.
  if (convtype == SoConvertAll::getClassTypeId())
    return new SoConvertAll(from, this->getTypeId());

  return (SoFieldConverter *)convtype.createInstance();
}


/*!
  Read the fieldcontainer and master field id of a field-to-field
  connection.
*/
SbBool
SoField::readConnection(SoInput * in)
{
  // Read the fieldcontainer instance containing the master field
  // we're connected to.
  SoBase * bp;
  if (!SoBase::read(in, bp, SoFieldContainer::getClassTypeId())) return FALSE;
  if (!bp) {
    SoReadError::post(in, "couldn't read field-to-field connection");
    return FALSE;
  }

  SoFieldContainer * fc = (SoFieldContainer *)bp;

  // Scan past the '.' character for ASCII format.
  if (!in->isBinary()) {
    char c;
    if (!in->read(c)) {
      SoReadError::post(in, "premature EOF");
      return FALSE;
    }
    if (c != '.') {
      SoReadError::post(in, "expected '.', got '%c'", c);
      return FALSE;
    }
  }

  // Read name of master field.
  SbName mastername;
  if (!in->read(mastername)) {
    SoReadError::post(in, "premature EOF");
    return FALSE;
  }


  // Find container for master field.
  // FIXME: does this also work for connections to engines and VRML
  // interpolators? 20000129 mortene.
  SoField * masterfield = fc->getField(mastername);
  if (!masterfield) {
    SoReadError::post(in, "no field ``%s'' in ``%s''",
                      mastername.getString(),
                      fc->getTypeId().getName().getString());
    return FALSE;
  }

  // Finally, make connection.
  if (!this->connectFrom(masterfield)) {
    SoReadError::post(in, "couldn't connect to ``%s''",
                      mastername.getString());
  }

  return TRUE;
}

/*!
  Write out information about this field's connection.
*/
void
SoField::writeConnection(SoOutput * out) const
{
  SbName mastername;
  SoFieldContainer * fc = this->resolveWriteConnection(mastername);
  assert(fc);

  if (!out->isBinary()) {
    out->write(' ');
    out->write(CONNECTIONCHAR);
  }

  if (fc->isOfType(SoNode::getClassTypeId())) {
    SoWriteAction wa(out);
    wa.continueToApply((SoNode *)fc);
  }
  else {
    // Note: for this to work, classes inheriting SoFieldContainer
    // which are _not_ also inheriting from SoNode must call
    // SoBase::writeHeader() and SoBase::writeFooter().
    fc->writeInstance(out);
    // FIXME: does this work for engines? 20000131 mortene.
  }

  if (!out->isBinary()) {
    out->indent();
    out->write(". ");
  }

  out->write(mastername.getString());
}

// Check if this field should write a connection upon export. Returns
// a pointer to the fieldcontainer with the master field we're
// connected to (or NULL if none, or if the master field's container
// is not within the scenegraph). If the return value is non-NULL, the
// name of the master field is copied to the mastername argument.
SoFieldContainer *
SoField::resolveWriteConnection(SbName & mastername) const
{
  if (!this->isConnected()) return NULL;

  SoFieldContainer * fc = NULL;
  SoField * fieldmaster;
  SoEngineOutput * enginemaster;
  SoVRMLInterpOutput * interpmaster;

  if (this->getConnectedField(fieldmaster)) {
    fc = fieldmaster->getContainer();
    assert(fc);
    SbBool ok = fc->getFieldName(fieldmaster, mastername);
    assert(ok);
  }
  else if (this->getConnectedEngine(enginemaster)) {
    fc = enginemaster->getContainer();
    assert(fc);
    // FIXME: couldn't we use getFieldName()? 20000129 mortene.
    SbBool ok = ((SoEngine *)fc)->getOutputName(enginemaster, mastername);
    assert(ok);
  }
  else if (this->getConnectedVRMLInterp(interpmaster)) {
    fc = interpmaster->getContainer();
    assert(fc);
    // FIXME: couldn't we use getFieldName()? 20000129 mortene.
    SbBool ok = ((SoVRMLInterpolator *)fc)->getOutputName(interpmaster, mastername);
    assert(ok);
  }
  else assert(FALSE);

  return fc;
}


/*!
  If we're connected to a field/engine/interpolator, copy the value
  from the master source.
*/
void
SoField::evaluateConnection(void) const
{
  // FIXME: should we evaluate from all masters in turn? 19990623 mortene.

  if (this->isConnectedFromField()) {
    int idx = this->storage->masterfields.getLength() - 1;
    SoField * master = this->storage->masterfields[idx];
    SoFieldConverter * converter = this->storage->findConverter(master);
    if (converter) converter->evaluateWrapper();
    else ((SoField *)this)->copyFrom(*master); // cast away const and copy
  }
  else if (this->isConnectedFromEngine()) {
    int idx = this->storage->masterengineouts.getLength() - 1;
    SoEngineOutput * master = this->storage->masterengineouts[idx];
    SoFieldConverter * converter = this->storage->findConverter(master);
    if (converter) converter->evaluateWrapper();
    else master->getContainer()->evaluateWrapper();
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
  This method is always called whenever the field's value has been
  changed by direct invocation of setValue() or some such. You should
  \e never call this method from anywhere in the code where the field
  value is being set through an evaluation of its connections.

  If \a resetdefault is \c TRUE, the flag marking whether or not the
  field has its default value will be set to \c FALSE.

  The method will also notify any auditors that the field's value has
  changed.
*/
void
SoField::valueChanged(SbBool resetdefault)
{
  if (this->statusflags.readonly) return;
  this->statusflags.readonly = 1;

  this->setDirty(FALSE);
  if (resetdefault) this->setDefault(FALSE);
  if (this->container) this->startNotify();

  this->statusflags.readonly = 0;
}

// Notify any auditors by marking them dirty - i.e. ready for
// re-evaluation.  Auditors include connected fields, sensors,
// containers (nodes/engines), ...
void
SoField::notifyAuditors(SoNotList * l)
{
#if COIN_DEBUG && 0 // debug
  SoDebugError::postInfo("SoField::notifyAuditors",
                         "field %p, list %p", this, l);
#endif // debug
  if (this->hasExtendedStorage() && this->storage->auditors.getLength())
    this->storage->auditors.notify(l);
}

/*!
  Set type of this field.

  The possible values for \a type is: 0 for ordinary fields, 1 for
  VRML2 eventIn fields, 2 for VRML2 eventOut fields, 3 for some other
  type of eventIn field, 4 for internal fields, 5 for VRML2
  exposedField fields.

  Note: this method is only used for VRML2 support, and is therefore
  not "active" yet.
*/
void
SoField::setFieldType(int type)
{
  this->statusflags.type = type;
}

/*!
  Return the type of this field.

  \sa setFieldType()
*/
int
SoField::getFieldType(void) const
{
  return this->statusflags.type;
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
