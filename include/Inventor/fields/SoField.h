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

#ifndef __SOFIELD_H__
#define __SOFIELD_H__

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


class SoField {

public:
  virtual ~SoField();

  static void initClass(void);
  static void initClasses(void);

  void setIgnored(SbBool ig);
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
  int getNumConnections(void);
  int getForwardConnections(SoFieldList & slavelist) const;
  int getConnections(SoFieldList & masterlist);

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

  SbBool set(const char * valueString);
  void get(SbString & valueString);

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

  virtual void connectionStatusChanged(int numConnections);
  SbBool isReadOnly(void) const;
  virtual SbBool isSame(const SoField & f) const = 0;
  virtual void copyFrom(const SoField & f) = 0;

  virtual void fixCopy(SbBool copyConnections);
  virtual SbBool referencesCopy(void) const;
  void copyConnection(const SoField * fromfield);

  virtual SbBool read(SoInput * in, const SbName & name);
  virtual void write(SoOutput * out, const SbName & name) const;

  virtual void countWriteRefs(SoOutput * out) const;

  void evaluate(void) const;

  void setFieldType(int flagValue);
  int getFieldType(void) const;

  SbBool getDirty(void);
  void setDirty(SbBool dirty);

  virtual void convertTo(SoField * dest) const = 0;

protected:
  SoField(void);

  void valueChanged(SbBool resetDefault = TRUE);
  virtual void evaluateConnection(void) const;
  virtual SbBool readValue(SoInput * in) = 0;
  virtual void writeValue(SoOutput * out) const = 0;
  virtual SbBool readConnection(SoInput * in);
  virtual void writeConnection(SoOutput * out) const;

  static void * reallocOutputBuf(void * buffer, size_t newsize);

private:
  void doConnect(SoField * master, SbBool notify);
  void doConnect(SoVRMLInterpOutput * master, SbBool notify);
  void doConnect(SoEngineOutput * master, SbBool notify);
  SbBool createConverter(SoType fromType, SoType toType,
                         SoFieldConverter *& conv);


  void notifyAuditors(SoNotList * list);

  static SoType classTypeId;

  struct {
    unsigned int isdefault      : 1;
    unsigned int ignore         : 1;
    unsigned int extstorage     : 1;
    unsigned int enableconnects : 1;
    unsigned int needevaluation : 1;
    unsigned int isevaluating   : 1;
  } statusflags;

  union {
    SoFieldContainer * container;
    class SoConnectStorage * storage;
  };

  SbBool hasExtendedStorage(void) const
    { return this->statusflags.extstorage; }

  SbBool donotify;
};

#if !defined(__SOLIB_INTERNAL__)
#include <Inventor/fields/SoSField.h>
#include <Inventor/fields/SoMField.h>
#endif // !__SOLIB_INTERNAL__

#endif // !__SOFIELD_H__
