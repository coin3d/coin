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

#ifndef __SOBASE_H__
#define __SOBASE_H__

#include <Inventor/SoType.h>
#include <Inventor/lists/SoAuditorList.h>

class SbString;
class SoBaseList;
class SoInput;
class SoOutput;

class SoBase {

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
  virtual void notify(SoNotList * list);

  void addAuditor(void * const auditor, const SoNotRec::Type type);
  void removeAuditor(void * const auditor, const SoNotRec::Type type);
  const SoAuditorList & getAuditors(void) const;

  virtual void addWriteReference(SoOutput * out, SbBool isFromField = FALSE);
  SbBool shouldWrite(void);

  static void incrementCurrentWriteCounter(void);
  static void decrementCurrentWriteCounter(void);

  static SoBase * getNamedBase(const SbName & name, SoType type);
  static int getNamedBases(const SbName & name, SoBaseList & baselist,
			   SoType type);

  static SbBool read(SoInput * in, SoBase *& base, SoType expectedType);
  static void setInstancePrefix(const SbString & c);

  static void setTraceRefs(SbBool bTrace);
  static SbBool getTraceRefs(void);


protected:
  // Bitflags.
  enum BaseFlags { IS_ENGINE = 0x01, IS_GROUP = 0x02 };

  SoBase(void);
  virtual ~SoBase();

  virtual void destroy(void);

  SbBool hasMultipleWriteRefs(void) const;
  SbBool writeHeader(SoOutput * out, SbBool isGroup, SbBool isEngine) const;
  void writeFooter(SoOutput * out) const;
  virtual const char * getFileFormatName(void) const;

  virtual SbBool readInstance(SoInput * in, unsigned short flags) = 0;

  static uint32_t getCurrentWriteCounter(void);


private:
  static SoType classTypeId;

  struct {
    int16_t referencecount  : 15;
    uint16_t writerefcount  : 15;
    unsigned int multirefs  :  1;
    unsigned int ingraph    :  1;
  } objdata;

  // Don't convert this to a pointer reference, as practically
  // speaking all SoBase derived objects have auditors -- so the list
  // will be allocated anyway, and we won't save any space (on the
  // contrary, we'll use a few extra bytes for the pointer on each
  // instance).
  //
  // But we should look into the possibility of slimming down
  // SoAuditorList.
  SoAuditorList auditors;
  
  static SbDict * name2obj_dict;
  static SbDict * obj2name_dict;

  static SbString refwriteprefix;

  static SbBool readReference(SoInput * in, SoBase *& base);
  static SbBool readBase(SoInput * in, SbName & className, SoBase *& base);
  static SbBool readBaseInstance(SoInput * in, const SbName & className,
				 const SbName & refName, SoBase *& base);

  static SoBase * createInstance(SoInput * in, const SbName & className);
  static void flushInput(SoInput * in);

  static void freeLists(unsigned long, void * value);

  friend class SoSFNode; // For access to writeHeader() and writeFooter().
};

#endif // !__SOBASE_H__
