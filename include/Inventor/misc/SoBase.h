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
 *  LGPL, please contact SIM to aquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#ifndef COIN_SOBASE_H
#define COIN_SOBASE_H

#include <Inventor/SoType.h>
#include <Inventor/lists/SoAuditorList.h>

class SbString;
class SoBaseList;
class SoInput;
class SoOutput;

class COIN_DLL_EXPORT SoBase {

public:
  static void initClass(void);

  void ref(void) const;
  void unref(void) const;
  void unrefNoDelete(void) const;
  int32_t getRefCount(void) const;

  void touch(void);

  virtual SoType getTypeId(void) const = 0;
  SbBool isOfType(SoType type) const;
  static SoType getClassTypeId(void);

  virtual SbName getName(void) const;
  virtual void setName(const SbName & newname);

  static void addName(SoBase * const base, const char * const name);
  static void removeName(SoBase * const base, const char * const name);

  virtual void startNotify(void);
  virtual void notify(SoNotList * l);

  void addAuditor(void * const auditor, const SoNotRec::Type type);
  void removeAuditor(void * const auditor, const SoNotRec::Type type);
  const SoAuditorList & getAuditors(void) const;

  virtual void addWriteReference(SoOutput * out, SbBool isfromfield = FALSE);
  SbBool shouldWrite(void);

  static void incrementCurrentWriteCounter(void);
  static void decrementCurrentWriteCounter(void);

  static SoBase * getNamedBase(const SbName & name, SoType type);
  static int getNamedBases(const SbName & name, SoBaseList & baselist,
                           SoType type);

  static SbBool read(SoInput * in, SoBase *& base, SoType expectedtype);
  static void setInstancePrefix(const SbString & c);

  static void setTraceRefs(SbBool trace);
  static SbBool getTraceRefs(void);


protected:
  // Note: these are bitflags.
  enum BaseFlags { IS_ENGINE = 0x01, IS_GROUP = 0x02 };

  SoBase(void);
  virtual ~SoBase();

  virtual void destroy(void);

  SbBool hasMultipleWriteRefs(void) const;
  SbBool writeHeader(SoOutput * out, SbBool isgroup, SbBool isengine) const;
  void writeFooter(SoOutput * out) const;
  virtual const char * getFileFormatName(void) const;

  virtual SbBool readInstance(SoInput * in, unsigned short flags) = 0;

  static uint32_t getCurrentWriteCounter(void);


private:
  static SbBool readReference(SoInput * in, SoBase *& base);
  static SbBool readBase(SoInput * in, SbName & classname, SoBase *& base);
  static SbBool readBaseInstance(SoInput * in, const SbName & classname,
                                 const SbName & refname, SoBase *& base);

  static SoBase * createInstance(SoInput * in, const SbName & classname);
  static void flushInput(SoInput * in);

  static void cleanClass(void);
  static void freeLists(unsigned long, void * value);

  static SoType classTypeId;

  // FIXME: the ifndef wrapper is a workaround for a bug in Doxygen
  // 1.0.0, where private members in a structure doesn't "inherit" the
  // private status of their "parent". (This has been confirmed to be
  // a bug by Dimitri.) Remove the workaround when a fixed Doxygen
  // appears. 20000124 mortene.
#ifndef DOXYGEN_SKIP_THIS
  struct {
    int16_t referencecount  : 15;
    uint16_t writerefcount  : 15;
    unsigned int multirefs  :  1;
    unsigned int ingraph    :  1;
  } objdata;
#endif // DOXYGEN_SKIP_THIS

  // Don't convert this to a pointer reference, as practically
  // speaking all SoBase derived objects have auditors -- so the list
  // will be allocated anyway, and we won't save any space (on the
  // contrary, we'll use a few extra bytes for the pointer on each
  // instance).
  //
  // But we should look into the possibility of slimming down
  // SoAuditorList.
  SoAuditorList auditors;

  static SbDict * name2obj;
  static SbDict * obj2name;

  static SbString * refwriteprefix;

  static SbBool tracerefs;
  static uint32_t writecounter;
};

#endif // !COIN_SOBASE_H
