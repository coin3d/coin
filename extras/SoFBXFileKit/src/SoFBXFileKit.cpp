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

#include "SoFBXFileKit.h"
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/C/tidbits.h>

#include <fbxsdk.h>
#include "FBXFileReader.h"

namespace {

class SoFBXFileKitP {
public:
  SoFBXFileKitP(SoFBXFileKit * pub)
    :api(pub) {
  }
  ~SoFBXFileKitP(void) {
  }

  static void cleanup(void) {
    SoFBXFileKitP::sdkManager->Destroy();
    SoFBXFileKitP::sdkManager = NULL;
  }

public:
  static KFbxSdkManager * sdkManager;
  SoFBXFileKit * const api;
}; // SoFBXFileKitP

KFbxSdkManager * SoFBXFileKitP::sdkManager = NULL;

} // anonymous namespace

/*!
  \class SoFBXFileKit SoFBXFileKit.h
  \brief SoFBXFileKit is a class for importing FBX and certain other files into Coin.

  You can use this class to read and convert FBX files into Coin scene graphs.
  Files of the following file formats can also be read but are at the moment only partially supported: 3DS, OBJ, COLLADA (.dae).

  The FBX support is provided through the FBX SDK from Autodesk.

  \relates foreignfileformats
  \COIN_CLASS_EXTENSION
*/

#define PRIVATE(obj) ((obj)->pimpl)

SO_KIT_SOURCE(SoFBXFileKit)

/*!
  Initializes class and registers file identification functions.
*/
void
SoFBXFileKit::initClass(void)
{
  SO_KIT_INIT_CLASS(SoFBXFileKit, SoForeignFileKit, SoForeignFileKit);

  SoFBXFileKitP::sdkManager = KFbxSdkManager::Create();

  // Initialize FBX SDK and register its supported extensions
  KFbxIOPluginRegistry *reg = SoFBXFileKitP::sdkManager->GetIOPluginRegistry();
  int numReaders = reg->GetReaderFormatCount();
  for (int i=0;i<numReaders;i++) {
    SoForeignFileKit::registerFileExtension(SoFBXFileKit::getClassTypeId(),
                                            reg->GetReaderFormatExtension(i), 
                                            SoFBXFileKit::identify);
  }

  cc_coin_atexit((coin_atexit_f*)SoFBXFileKitP::cleanup);
}

/*!
  Returns wether or not \a filename is identified as an FBX-supported file.
*/
SbBool
SoFBXFileKit::identify(const char * filename)
{
  assert(filename);
  // FIXME: Implement
  return TRUE;
}

/*!
  Constructor.
*/
SoFBXFileKit::SoFBXFileKit(void)
{
  PRIVATE(this) = new SoFBXFileKitP(this);

  SO_KIT_CONSTRUCTOR(SoFBXFileKit);

  SO_KIT_INIT_INSTANCE();
}

/*!
  Destructor.
*/
SoFBXFileKit::~SoFBXFileKit(void)
{
  delete PRIVATE(this);
  PRIVATE(this) = NULL;
}

// doc in inherited class
SbBool
SoFBXFileKit::canReadFile(const char * filename) const
{
  if (!filename) return TRUE; // we can read FBX files, in general
  return SoFBXFileKit::identify(filename);
}

/*!
  Reads in an FBX file.

  Returns FALSE if \a filename could not be opened or parsed
  correctly.

  \sa canReadFile, canWriteScene, writeScene
*/
SbBool
SoFBXFileKit::readFile(const char * filename)
{
  assert(filename);

  FBXFileReader reader(SoFBXFileKitP::sdkManager);
  SoSeparator *newroot = reader.read(filename);
  if (newroot) this->setAnyPart("topSeparator", newroot);

  return (newroot != NULL);
}

/*!
  Converts (if necessary) the FBX model into a native scene graph and returns the root.
*/
SoSeparator *
SoFBXFileKit::convert()
{
  return SO_GET_ANY_PART(this, "topSeparator", SoSeparator);
}

/*!
  We cannot write any of the foreign file formats supported by the FBX SDK.

  \sa writeFile
*/
SbBool
SoFBXFileKit::canWriteFile(const char * filename) const
{
  return FALSE;
}

/*!
  \sa canWriteFile
*/
SbBool
SoFBXFileKit::writeFile(const char * filename)
{
  return FALSE;
}

#undef PRIVATE
