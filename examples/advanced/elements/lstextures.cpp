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

#include <Inventor/SoDB.h>
#include <Inventor/SoInput.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTexture2.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/misc/SoState.h>
#include <assert.h>
#include <stdio.h>

#include "texturefilenameelement.h"

SoCallbackAction::Response
pre_tex2_cb(void * data, SoCallbackAction * action, const SoNode * node)
{
  fprintf(stdout, "tex2 callback\n");
  SoTextureFilenameElement::set(action->getState(), (SoNode *)node,
                                ((SoTexture2 *)node)->filename.getValue());
  return SoCallbackAction::CONTINUE;
}

SoCallbackAction::Response
pre_cube_cb(void * data, SoCallbackAction * action, const SoNode * node)
{
  const SbString & filename = SoTextureFilenameElement::get(action->getState());
  fprintf(stdout, "Cube texture: %s\n", filename.getString());
  return SoCallbackAction::CONTINUE;
}

int
main(int argc, char **argv)
{
  assert(argc == 2);

  SoDB::init();

  SoTextureFilenameElement::initClass();
  SO_ENABLE(SoCallbackAction, SoTextureFilenameElement);

  SoInput input;
  assert(input.openFile(argv[1]));

  SoSeparator * root = SoDB::readAll(&input);
  if (root) {
    SoCallbackAction cbaction;
    cbaction.addPreCallback(SoTexture2::getClassTypeId(),
                            pre_tex2_cb, (void *) 4);
    cbaction.addPreCallback(SoCube::getClassTypeId(),
			    pre_cube_cb, (void *) 3);
    cbaction.apply(root);
  }
  return 0;
}
