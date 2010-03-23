#ifndef COIN_SOSTLFILEKIT_H
#define COIN_SOSTLFILEKIT_H

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

#include <Inventor/SbBasic.h>

#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFEnum.h>

#include <ForeignFiles/SoForeignFileKit.h>

class SbVec3f;
class SoCallbackAction;
class SoPrimitiveVertex;
class SoSTLFileKitP;

class COIN_DLL_API SoSTLFileKit : public SoForeignFileKit {
  typedef SoForeignFileKit inherited;

  SO_KIT_HEADER(SoSTLFileKit);
  SO_KIT_CATALOG_ENTRY_HEADER(shapehints);
  SO_KIT_CATALOG_ENTRY_HEADER(texture);
  SO_KIT_CATALOG_ENTRY_HEADER(normalbinding);
  SO_KIT_CATALOG_ENTRY_HEADER(normals);
  SO_KIT_CATALOG_ENTRY_HEADER(materialbinding);
  SO_KIT_CATALOG_ENTRY_HEADER(material);
  SO_KIT_CATALOG_ENTRY_HEADER(coordinates);
  SO_KIT_CATALOG_ENTRY_HEADER(facets);

public:
  static void initClass(void);
  SoSTLFileKit(void);

  enum Colorization { GREY, MATERIALISE, TNO_VISICAM };

  SoSFString info;
  SoSFBool binary;
  SoSFEnum colorization;

  static SbBool identify(const char * filename);
  virtual SbBool canReadFile(const char * filename = NULL) const;
  virtual SbBool readFile(const char * filename);

  virtual SbBool canWriteFile(const char * filename = NULL) const;
  virtual SbBool writeFile(const char * filename);


  SbBool canReadScene(void) const;
  SbBool readScene(SoNode * scene);
  virtual SoSeparator *convert();

protected:
  virtual ~SoSTLFileKit(void);

  void reset(void);
  SbBool addFacet(const SbVec3f & v1, const SbVec3f & v2, const SbVec3f & v3,
                  const SbVec3f & normal);
  void organizeModel(void);

private:
  SoSTLFileKitP * pimpl;

  static void add_facet_cb(void * closure, SoCallbackAction * action, const SoPrimitiveVertex * v1, const SoPrimitiveVertex * v2, const SoPrimitiveVertex * v3);
  static void put_facet_cb(void * closure, SoCallbackAction * action, const SoPrimitiveVertex * v1, const SoPrimitiveVertex * v2, const SoPrimitiveVertex * v3);

}; // SoSTLFileKit

#endif // !COIN_SOSTLFILEKIT_H
