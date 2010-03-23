#ifndef COIN_SOVERTEXATTRIBUTE_H
#define COIN_SOVERTEXATTRIBUTE_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#include <Inventor/nodes/SoNode.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/elements/SoVertexAttributeElement.h>
#include <Inventor/fields/SoSFName.h>
#include <Inventor/fields/SoMField.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/tools/SbPimplPtr.h>

class SoMField;
class SoVertexAttributeP;

class COIN_DLL_API SoVertexAttribute : public SoNode {
  typedef SoNode inherited;

public:
  SoVertexAttribute(void);
  static void initClass(void);

  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;

  SoSFName name;
  SoSFName typeName;

  SoMField * getValuesField(void) const;

  virtual void doAction(SoAction * action);
  virtual void GLRender(SoGLRenderAction * action);
  virtual void write(SoWriteAction * action);
  virtual void copyContents(const SoFieldContainer * from, 
                            SbBool copyconnections); 
  virtual void notify(SoNotList * l);

protected:
  virtual ~SoVertexAttribute(void);

  virtual SbBool readInstance(SoInput * in, unsigned short flags);

private:
  static SoType classTypeId;
  static void * createInstance(void);
  virtual const SoFieldData * getFieldData(void) const;

  void initFieldData(void);

  SoVertexAttribute(const SoVertexAttribute & rhs); // N/A
  SoVertexAttribute & operator = (const SoVertexAttribute & rhs); // N/A

  SbPimplPtr<SoVertexAttributeP> pimpl;

}; // SoVertexAttribute

// *************************************************************************

template <int Type>
class SoAnyVertexAttribute : public SoVertexAttribute {
public:

private:
  SoAnyVertexAttribute(void) { }

}; // SoAnyVertexAttribute

// *************************************************************************

#endif // !COIN_SOVERTEXATTRIBUTE_H
