/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
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

#ifndef COIN_SOFIELD_H
#define COIN_SOFIELD_H

#include <Inventor/SoType.h>
#include <Inventor/misc/SoNotification.h>

class SbString;
class SoEngineOutput;
class SoFieldContainer;
class SoFieldConverter;
class SoFieldList;
class SoInput;
class SoOutput;
class SoVRMLInterpOutput;


class COIN_DLL_EXPORT SoField {

public:
  virtual ~SoField();

  static void initClass(void);
  static void initClasses(void);

  void setIgnored(SbBool ignore);
  SbBool isIgnored(void) const;

  void setDefault(SbBool def);
  SbBool isDefault(void) const;

  virtual SoType getTypeId(void) const = 0;

  static SoType getClassTypeId(void);
  SbBool isOfType(const SoType type) const;

  void enableConnection(SbBool flag);
  SbBool isConnectionEnabled(void) const;

  // Field<-Engine connection stuff.
  SbBool connectFrom(SoEngineOutput * master,
                     SbBool notnotify = FALSE, SbBool append = FALSE);
  SbBool appendConnection(SoEngineOutput * master, SbBool notnotify = FALSE);
  void disconnect(SoEngineOutput * engineoutput);
  SbBool isConnectedFromEngine(void) const;
  SbBool getConnectedEngine(SoEngineOutput *& master) const;

  // Field<->Field connection stuff.
  SbBool connectFrom(SoField * master,
                     SbBool notnotify = FALSE, SbBool append = FALSE);
  SbBool appendConnection(SoField * master, SbBool notnotify = FALSE);
  void disconnect(SoField * field);
  SbBool isConnectedFromField(void) const;
  SbBool getConnectedField(SoField *& master) const;
  int getNumConnections(void) const;
  int getForwardConnections(SoFieldList & slavelist) const;
  int getConnections(SoFieldList & masterlist) const;

  // Field<-Interpolator connection stuff.
  SbBool connectFrom(SoVRMLInterpOutput * master,
                     SbBool notnotify = FALSE, SbBool append = FALSE);
  SbBool appendConnection(SoVRMLInterpOutput * master,
                          SbBool notnotify = FALSE);
  void disconnect(SoVRMLInterpOutput * interpoutput);
  SbBool isConnectedFromVRMLInterp(void) const;
  SbBool getConnectedVRMLInterp(SoVRMLInterpOutput *& master) const;

  void disconnect(void);
  SbBool isConnected(void) const;

  void setContainer(SoFieldContainer * cont);
  SoFieldContainer * getContainer(void) const;

  SbBool set(const char * valuestring);
  void get(SbString & valuestring);

  SbBool shouldWrite(void) const;

  virtual void touch(void);
  virtual void startNotify(void);
  virtual void notify(SoNotList * nlist);
  SbBool enableNotify(SbBool on);
  SbBool isNotifyEnabled(void) const;

  void addAuditor(void * f, SoNotRec::Type type);
  void removeAuditor(void * f, SoNotRec::Type type);

  int operator ==(const SoField & f) const;
  int operator !=(const SoField & f) const;

  virtual void connectionStatusChanged(int numconnections);
  SbBool isReadOnly(void) const;
  virtual SbBool isSame(const SoField & f) const = 0;
  virtual void copyFrom(const SoField & f) = 0;

  virtual void fixCopy(SbBool copyconnections);
  virtual SbBool referencesCopy(void) const;
  void copyConnection(const SoField * fromfield);

  virtual SbBool read(SoInput * in, const SbName & name);
  virtual void write(SoOutput * out, const SbName & name) const;

  virtual void countWriteRefs(SoOutput * out) const;

  void evaluate(void) const;

  void setFieldType(int type);
  int getFieldType(void) const;

  SbBool getDirty(void) const;
  void setDirty(SbBool dirty);

protected:
  SoField(void);

  void valueChanged(SbBool resetdefault = TRUE);
  virtual void evaluateConnection(void) const;
  virtual SbBool readValue(SoInput * in) = 0;
  virtual void writeValue(SoOutput * out) const = 0;
  virtual SbBool readConnection(SoInput * in);
  virtual void writeConnection(SoOutput * out) const;

private:
  void extendStorageIfNecessary(void);
  SoFieldConverter * createConverter(SoType from) const;
  SoFieldContainer * resolveWriteConnection(SbName & mastername) const;

  void notifyAuditors(SoNotList * l);

  static SoType classTypeId;

  // These are bit flags.
  enum FileFormatFlags {
    IGNORED = 0x01,
    CONNECTED = 0x02,
    DEFAULT = 0x04,
    ALLFILEFLAGS = IGNORED|CONNECTED|DEFAULT
  };

  // FIXME: the ifndef wrapper is a workaround for a bug in Doxygen
  // 1.0.0, where private members in a structure doesn't "inherit" the
  // private status of their "parent". (This has been confirmed to be
  // a bug by Dimitri.) Remove the workaround when a fixed Doxygen
  // appears. 20000201 mortene.
#ifndef DOXYGEN_SKIP_THIS
  struct {
    unsigned int isdefault      : 1;
    unsigned int ignore         : 1;
    unsigned int extstorage     : 1;
    unsigned int enableconnects : 1;
    unsigned int needevaluation : 1;
    unsigned int isevaluating   : 1;
    unsigned int type           : 3; // Needs to hold values in range [0-5]
    unsigned int readonly       : 1;
    unsigned int donotify       : 1;
  } statusflags;

  union {
    SoFieldContainer * container;
    class SoConnectStorage * storage;
  };
#endif // DOXYGEN_SKIP_THIS

  SbBool hasExtendedStorage(void) const
    { return this->statusflags.extstorage; }
};


#ifndef COIN_INTERNAL
// Added to be Inventor compliant.
#include <Inventor/fields/SoSField.h>
#include <Inventor/fields/SoMField.h>
#endif // !COIN_INTERNAL

#endif // !COIN_SOFIELD_H
