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

#ifndef __SOFIELDCONTAINER_H__
#define __SOFIELDCONTAINER_H__

#include <Inventor/misc/SoBase.h>
#include <Inventor/fields/SoFieldData.h>

class SbString;
class SoFieldList;
class SoOutput;

class SoFieldContainer : public SoBase {
  typedef SoBase inherited;

public:
  static void initClass(void);
  static SoType getClassTypeId(void);

  void setToDefaults(void);
  SbBool hasDefaultValues(void) const;

  SbBool fieldsAreEqual(const SoFieldContainer * const container) const;
  void copyFieldValues(const SoFieldContainer * const container,
		       SbBool copyConnections = FALSE);

  SbBool set(const char * const fieldDataString);
  void get(SbString & fieldDataString);

  virtual int getFields(SoFieldList & list) const;
  virtual int getAllFields(SoFieldList & list) const;
  virtual SoField * getField(const SbName & fieldName) const;
  virtual SoField * getEventIn(const SbName & fieldName) const;
  virtual SoField * getEventOut(const SbName & fieldName) const;
  SbBool getFieldName(const SoField * const field, SbName & fieldName) const;

  SbBool enableNotify(const SbBool flag);
  SbBool isNotifyEnabled(void) const;

  SbBool set(const char * const fieldDataString, SoInput * const dictIn);
  void get(SbString & fieldDataString, SoOutput * const dictOut);

  virtual void notify(SoNotList * list);

  virtual SbBool validateNewFieldValue(SoField * pField, void * newValue);

  virtual void addWriteReference(SoOutput * out, SbBool isFromField = FALSE);
  virtual void writeInstance(SoOutput * out);

  SbBool getIsBuiltIn(void) const;
  virtual const SoFieldData * getFieldData(void) const;

  virtual void copyContents(const SoFieldContainer * fromFC,
			    SbBool copyConnections);
  virtual SoFieldContainer * copyThroughConnection(void) const;

  static void initCopyDict(void);
  static void addCopy(const SoFieldContainer * const orig,
		      const SoFieldContainer * const copy);
  static SoFieldContainer * checkCopy(const SoFieldContainer * const orig);
  static SoFieldContainer * findCopy(const SoFieldContainer * const orig,
				     const SbBool copyConnections);
  static void copyDone(void);

  // FIXME: hide this. 19990610 mortene.
  SoFieldData fieldData;

protected:
  SoFieldContainer(void);
  ~SoFieldContainer(void);

  virtual SbBool readInstance(SoInput * in, unsigned short flags);
  SbBool isBuiltIn;

private:
  static SoType classTypeId;
  SbBool donotify;
};

#endif // !__SOFIELDCONTAINER_H__
