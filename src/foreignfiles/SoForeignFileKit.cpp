/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2009 by Kongsberg SIM.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg SIM about acquiring
 *  a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg SIM, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#ifdef HAVE_NODEKITS

#include <ForeignFiles/SoForeignFileKit.h>

#include <assert.h>
#include <stdlib.h>

#include <Inventor/errors/SoDebugError.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/SoType.h>
#include <Inventor/SbName.h>
#include <Inventor/SoInput.h>
#include <Inventor/C/tidbits.h>

#include <ForeignFiles/SoSTLFileKit.h>

#include "nodekits/SoSubKitP.h"
#include "misc/SbHash.h"
#include "tidbitsp.h"

/*!
  \page foreignfilesupport Foreign File Format Support

  This is a page describing foreign file format support in Coin.

  Formats supported so far:
  - STL / Stereolithography files (.stl)

  The SoSTLFileKit server as a first example on how foreign file
  format support can be implemented through the SoForeignFileKit
  interface nodekit.  For STL, the file format is so trivial that it
  maps directly into a few Open Inventor nodes. For other file
  formats, the direct mapping might not be possible or desirable
  (maybe important information will get lost in such a conversion),
  and the implementor might choose to implement a set of internal
  custom nodes for doing the rendering (and applying the other
  actions), and choose to first do pure Open Inventor organization
  when the node kit is asked to write its contents as a scene graph.

  \sa SoForeignFileKit, SoSTLFileKit
*/

/*!
  \class SoForeignFileKit ForeignFiles/SoForeignFileKit.h
  \brief Abstract base class for foreign file format support in Coin.

  Abstract base class for foreign file format support in Coin.

  \relates foreignfilesupport
  \COIN_CLASS_EXTENSION
  \since Coin 3.0
*/

/*
  TODO / UNRESOLVED ISSUES:
  
  - file format sub-formats (e.g. stl: stl ascii, stl binary, stl binary colored)
  - specify/implement format specification/syntax for writeScene()
  - improve file type handler registering

*/

class SoForeignFileKitP {
public:
  SoForeignFileKitP(SoForeignFileKit * api) { }

  static SbHash<SoType, const char *> * fileexts;

};

SbHash<SoType, const char *> * SoForeignFileKitP::fileexts = NULL;

SO_KIT_ABSTRACT_SOURCE(SoForeignFileKit);

static void
foreignfilekit_cleanup(void)
{
  delete SoForeignFileKitP::fileexts;
  SoForeignFileKitP::fileexts = NULL;
}

void
SoForeignFileKit::initClass(void)
{
  assert(SoForeignFileKit::classTypeId == SoType::badType());
  // Make sure parent class has been initialized.
  assert(inherited::getClassTypeId() != SoType::badType());

  SO_KIT_INIT_ABSTRACT_CLASS(SoForeignFileKit, SoBaseKit, SoBaseKit);
  // set up support dictionary
  SoForeignFileKitP::fileexts = new SbHash<SoType, const char *>(11);
  coin_atexit((coin_atexit_f*)foreignfilekit_cleanup, CC_ATEXIT_NORMAL);

  SoForeignFileKit::initClasses();
}

void
SoForeignFileKit::initClasses(void)
{
  SoSTLFileKit::initClass();
  // SoDFXFileKit::initClass()
}


#define PRIVATE(obj) ((obj)->pimpl)


SoForeignFileKit::SoForeignFileKit(void)
{
  PRIVATE(this) = NULL;
  // PRIVATE(this) = new SoForeignFileKitP(this);

  SO_KIT_INTERNAL_CONSTRUCTOR(SoForeignFileKit);

  SO_KIT_ADD_FIELD(filename, (""));

  SO_KIT_ADD_CATALOG_ENTRY(topSeparator, SoSeparator, TRUE, this, \x0, FALSE);

  SO_KIT_INIT_INSTANCE();
}

SoForeignFileKit::~SoForeignFileKit(void)
{
  // delete PRIVATE(this);
}

SbBool
SoForeignFileKit::registerFileExtension(SoType handler, SbName extension, SoForeignFileIdentifyFunc * identify)
{
  assert(SoForeignFileKitP::fileexts != NULL);
  assert(handler.canCreateInstance());

  if (extension.getString()[0] == '.') {
#if COIN_DEBUG
    SoDebugError::post("SoForeignFileKit::registerFileExtension",
                       "Do not include the extension separator '.' "
                       "when registering file extension.");
#endif // COIN_DEBUG
  }

  if (SoForeignFileKitP::fileexts->put(extension.getString(), handler)) {
    return TRUE;
  }
  return FALSE;
}

SbBool
SoForeignFileKit::isFileSupported(SoInput * in, SbBool exhaust)
{
  assert(in);
  if (in->getCurFileName() == NULL || in->getNumBytesRead() > 0) {
    // can only read proper files, from the beginning
    return FALSE;
  }
  return SoForeignFileKit::isFileSupported(in->getCurFileName(), exhaust);
}

SbBool
SoForeignFileKit::isFileSupported(const char * filename, SbBool exhaust)
{
  assert(SoForeignFileKitP::fileexts != NULL);

  const char * extptr = strrchr(filename, '.');
  if (extptr) {
    extptr++;
    SbName ext(SbString(extptr).lower());
    SoType handler = SoType::badType();
    if (SoForeignFileKitP::fileexts->get(ext.getString(), handler)) {
      SoForeignFileKit * foreignfile =
        (SoForeignFileKit *) handler.createInstance();
      foreignfile->ref();
      SbBool canread = foreignfile->canReadFile(filename);
      foreignfile->unref();
      return canread;
    }
  }
  if (exhaust) {
    // SoForeignFileKitP::fileexts->apply()
  }
  return FALSE;
}

SoForeignFileKit *
SoForeignFileKit::createForeignFileKit(SoInput * in, SbBool exhaust)
{
  assert(in);
  if (in->getCurFileName() == NULL || in->getNumBytesRead() > 0) {
    // can only read proper files, from the beginning
    return FALSE;
  }
  SoForeignFileKit * kit =
    SoForeignFileKit::createForeignFileKit(in->getCurFileName(), exhaust);
  if (kit) {
    if (!in->eof()) {
      // Places the stream at the end of the current file on the
      // stack. This is a "hack-ish", but its done this way
      // instead of loosening the protection of SoInput::popFile().
      char dummy;
      while (!in->eof() && in->get(dummy));
      assert(in->eof());
    }
  }
  return kit;
}

SoForeignFileKit *
SoForeignFileKit::createForeignFileKit(const char * filename, SbBool exhaust)
{
  assert(SoForeignFileKitP::fileexts != NULL);

  const char * extptr = strrchr(filename, '.');
  if (extptr) {
    extptr++;
    SbName ext(SbString(extptr).lower());
    SoType handler = SoType::badType();
    if (SoForeignFileKitP::fileexts->get(ext.getString(), handler)) {
      SoForeignFileKit * foreignfile =
        (SoForeignFileKit *) handler.createInstance();
      foreignfile->ref();
      SbBool canread = foreignfile->canReadFile(filename);
      SbBool ok = FALSE;
      if (canread) {
        ok = foreignfile->readFile(filename);
      }
      if (canread && ok) {
        foreignfile->unrefNoDelete();
        return foreignfile;
      } else {
        foreignfile->unref();
        return NULL;
      }
    }
  }

  return NULL;
}

// *************************************************************************
// dummy implementations for unsupported functionality

SbBool
SoForeignFileKit::canReadFile(const char * filename) const
{
  return FALSE;
}

SbBool
SoForeignFileKit::readFile(const char * filename)
{
  return FALSE;
}

SbBool
SoForeignFileKit::canReadScene(void) const
{
  return FALSE;
}

SbBool
SoForeignFileKit::readScene(SoNode * scene)
{
  return FALSE;
}

SbBool
SoForeignFileKit::canWriteFile(const char * filename) const
{
  return FALSE;
}

SbBool
SoForeignFileKit::writeFile(const char * filename)
{
  return FALSE;
}

SbBool
SoForeignFileKit::canWriteScene(const char * format) const
{
  return FALSE;
}

SbBool
SoForeignFileKit::writeScene(SoNode *& root, const char * format)
{
  return FALSE;
}

// *************************************************************************

#undef PRIVATE
#endif // HAVE_NODEKITS
