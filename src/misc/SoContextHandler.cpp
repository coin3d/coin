/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#include <Inventor/misc/SoContextHandler.h>
#include <Inventor/lists/SbList.h>

class socontexthandler_cbitem {
public:
  socontexthandler_cbitem(void) { }
  
  int operator==(const socontexthandler_cbitem & theother) {
    return 
      this->func == theother.func &&
      this->closure == theother.closure;
  }
  
  SoContextHandler::ContextDestructionCB * func;
  void * closure;
};


static SbList <socontexthandler_cbitem> * socontexthandler_cblist;

/*!
  This method \e must be called by client code which destruct a
  context, to guarantee that there are no memory leaks upon context
  destruction (texture objects and display lists must be deleted). 
*/
void
SoContextHandler::destructingContext(uint32_t contextid)
{
  const int n = socontexthandler_cblist ? 
    socontexthandler_cblist->getLength() : 0;
  for (int i = 0; i < n; i++) {
    const socontexthandler_cbitem & item = 
      (*socontexthandler_cblist)[i];
    item.func(contextid, item.closure);
  }
}

void
SoContextHandler::addContextDestructionCallback(ContextDestructionCB * func,
                                                void * closure)
{
  if (socontexthandler_cblist == NULL) {
    socontexthandler_cblist = new SbList <socontexthandler_cbitem>;
  }
  socontexthandler_cbitem item;
  item.func = func;
  item.closure = closure;
  socontexthandler_cblist->append(item);
}

void
SoContextHandler::removeContextDestructionCallback(ContextDestructionCB * func, void * closure)
{
  if (socontexthandler_cblist) {
    socontexthandler_cbitem item;
    item.func = func;
    item.closure = closure;

    int idx = socontexthandler_cblist->find(item);
    if (idx >= 0) {
      socontexthandler_cblist->removeFast(idx);
    }
    else {

    }
  }
}
