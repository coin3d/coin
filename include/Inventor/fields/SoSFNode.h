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

#ifndef __SOSFNODE_H__
#define __SOSFNODE_H__

#include <Inventor/fields/SoSField.h>

class SoNode;


class SoSFNode : public SoSField {
  typedef SoSField inherited;

//$ BEGIN TEMPLATE SField(SoSFNode, SoNode *, SoNode *)
private:
  static SoType classTypeId;

public:
  static void * createInstance(void);
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;

  static void initClass(void);

  virtual void copyFrom(const SoField & field);
  const SoSFNode & operator = (const SoSFNode & field);
  virtual SbBool isSame(const SoField & field) const;
public:
  SoSFNode(void);
  virtual ~SoSFNode(void);
private:
  virtual SbBool readValue(SoInput * in);
  virtual void writeValue(SoOutput * out) const;
public:
  /*! Returns this field's value. */
  SoNode * getValue(void) const
    { this->evaluate(); return this->value; }
  void setValue(SoNode * newvalue);
  /*! Copy value from \a newvalue into this field. */
  SoNode * operator = (SoNode * newvalue)
    { this->setValue(newvalue); return this->value; }

  int operator == (const SoSFNode & field) const;
  /*! Returns \a TRUE if this field is not equal to \a field. */
  int operator != (const SoSFNode & field) const
    { return ! operator == (field); }

protected:
  /*! The value contained in the field. */
  SoNode * value;
//$ END TEMPLATE SField

public:
  virtual void fixCopy(SbBool copyConnections);
  virtual SbBool referencesCopy(void) const;

private:
  virtual void convertTo(SoField * dest) const;

  friend class SoMFNode; // For readValue() & writeValue().
};

#endif // !__SOSFNODE_H__
