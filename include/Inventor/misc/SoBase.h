#ifndef COIN_SOBASE_H
#define COIN_SOBASE_H

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

#include <Inventor/SoType.h>
#include <Inventor/lists/SoAuditorList.h>
#include <Inventor/C/base/rbptree.h>

class SbString;
class SoBaseList;
class SoInput;
class SoOutput;

class COIN_DLL_API SoBase {

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

  static SbBool connectRoute(const SbName & fromnodename, const SbName & fromfieldname,
                             const SbName & tonodename, const SbName & tofieldname);
  

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
  static SbBool readRoute(SoInput * in);

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
    int32_t referencecount  : 27;
    unsigned int ingraph    :  1;
    unsigned int alive      :  4;
    // The number of bits should sum up to 32, so we don't allocate
    // more than one machine word on a 32-bit platform.
  } objdata;
#endif // DOXYGEN_SKIP_THIS
  
  void doNotify(SoNotList * l, const void * auditor, const SoNotRec::Type type);  
  static void rbptree_notify_cb(void * auditor, void * type, void * closure);
  cc_rbptree auditortree;

  static SbDict * name2obj;
  static SbDict * obj2name;

  static SbString * refwriteprefix;

  static SbBool tracerefs;
  static uint32_t writecounter;
};

#endif // !COIN_SOBASE_H
