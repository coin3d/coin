/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2004 by Systems in Motion.  All rights reserved.
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
 *  See <URL:http://www.coin3d.org/> for more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

/*!
  \class SoContextHandler SoContextHandler.h Inventor/misc/SoContextHandler.h
  \brief The SoContextHandler class is for now to be treated as an internal class.
  \ingroup general

  \since Coin 2.0
*/

// FIXME: should be documented and be part of the Doxygen API doc,
// since it's a public class (and possibly useful
// externally). 20030225 mortene.

// *************************************************************************

#include <Inventor/misc/SoContextHandler.h>

#include <Inventor/C/tidbitsp.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/lists/SbList.h>

// *************************************************************************

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

// *************************************************************************

static SbList <socontexthandler_cbitem> * socontexthandler_cblist;

// *************************************************************************

static void
socontexthandler_cleanup(void)
{
#if COIN_DEBUG  
  const int len = socontexthandler_cblist->getLength();
  if (len > 0) {
    SoDebugError::postWarning("socontexthandler_cleanup",
                              "%d context-bound resources not free'd "
                              "before exit.", len);
  }
#endif // COIN_DEBUG  
  delete socontexthandler_cblist;
}

// *************************************************************************

/*!
  This method \e must be called by client code which destructs a
  context, to guarantee that there are no memory leaks upon context
  destruction.

  This will take care of correctly freeing context-bound resources,
  like OpenGL texture objects and display lists.

  Before calling this function, the context \e must be made current.

  Note that if you are using one of the standard GUI-binding libraries
  from Systems in Motion, this is taken care of automatically for
  contexts for canvases set up by SoQt, SoWin, etc.
*/
void
SoContextHandler::destructingContext(uint32_t contextid)
{
  if (socontexthandler_cblist == NULL) { return; }

  // Work on a copy of the list, to allow
  // removeContextDestructionCallback() to be safely invoked while
  // iterating over the list.
  SbList <socontexthandler_cbitem> listcopy = *socontexthandler_cblist;

  for (int i = 0; i < listcopy.getLength(); i++) {
    const socontexthandler_cbitem & item = listcopy[i];
    item.func(contextid, item.closure);
  }
}

// *************************************************************************

void
SoContextHandler::addContextDestructionCallback(ContextDestructionCB * func,
                                                void * closure)
{
  if (socontexthandler_cblist == NULL) {
    socontexthandler_cblist = new SbList <socontexthandler_cbitem>;
    coin_atexit((coin_atexit_f *)socontexthandler_cleanup, 0);
  }

  socontexthandler_cbitem item;
  item.func = func;
  item.closure = closure;
  socontexthandler_cblist->append(item);
}

void
SoContextHandler::removeContextDestructionCallback(ContextDestructionCB * func, void * closure)
{
  assert(socontexthandler_cblist);

  socontexthandler_cbitem item;
  item.func = func;
  item.closure = closure;

  const int idx = socontexthandler_cblist->find(item);
  assert(idx >= 0);
  socontexthandler_cblist->removeFast(idx);
}

// *************************************************************************
