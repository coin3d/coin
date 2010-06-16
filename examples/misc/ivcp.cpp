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

/*
 * Simple example that demonstrates how to read in and write out an
 * Open Inventor model, essentially copying it.  This process has no use
 * per se, but can be used to sanity-check the import/export-related
 * parts of Coin, and can be used as a starting point for utilities doing
 * scene graph rewrites.
 *
 * Build the example using this command:
 *
 *   coin-config --build ivcp ivcp.cpp
 *
 */

#include <cassert>
#include <cstdio>

#include <Inventor/SoDB.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/SoPath.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodekits/SoNodeKit.h>
#include <Inventor/SoInteraction.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/annex/ForeignFiles/SoForeignFileKit.h>

enum FileType { INVENTOR, VRML1, VRML2 };

int
main(int argc, char ** argv)
{
  fprintf(stderr, "ivcp v0.1\n");

  SoDB::init();
  SoNodeKit::init();
  SoInteraction::init();

  if (argc != 3 ) {
    fprintf(stdout, "Usage: %s infile outfile\n", argv[0]);
    return 0;
  }

  SoInput * in = new SoInput;
  if (!in->openFile(argv[1])) {
    fprintf(stderr, "error: could not open file '%s'\n", argv[1]);
    delete in;
    SoDB::cleanup();
    return -1;
  }

  SoNode * scene = SoDB::readAll(in);

  if (!scene) {
    fprintf(stderr, "error: could not read file '%s'\n", argv[1]);
    delete in;
    SoDB::cleanup();
    return -1;
  }
  FileType inputFileType;
  if (in->isFileVRML1())
    inputFileType = VRML1;
  else if (in->isFileVRML2())
    inputFileType = VRML2;
  else
    inputFileType = INVENTOR;

  delete in;
  scene->ref();

  SoNode * firstChild = static_cast<SoSeparator*>(scene)->getNumChildren()?
    static_cast<SoSeparator*>(scene)->getChild(0)
    :NULL;

  if (firstChild && firstChild->isOfType(SoForeignFileKit::getClassTypeId())) {
    SoForeignFileKit * kit = (SoForeignFileKit *) firstChild;
    if (kit->canWriteScene() ) {
      SoNode * subscene = NULL;
      kit->writeScene(subscene);
      if (!subscene ) {
        return -1;
      }
      subscene->ref();
      scene->unref();
      scene = subscene;
    }
  }

  SoOutput * out = new SoOutput;
  if (!out->openFile(argv[2])) {
    fprintf(stderr, "error: could not open file '%s' for writing\n");
    scene->unref();
    delete out;
    SoDB::cleanup();
    return -1;
  }
  switch (inputFileType) {
  case VRML1:
    out->setHeaderString("#VRML V1.0 ascii");
    break;
  case VRML2:
    out->setHeaderString("#VRML V2.0 utf8");
  }

  SoWriteAction wa(out);
  wa.apply(scene);

  out->closeFile();
  delete out;

  scene->unref();

  // with actions on the stack, cleanup can't be called...
  // SoDB::cleanup();
  return 0;
}
