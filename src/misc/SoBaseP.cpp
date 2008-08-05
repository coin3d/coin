/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2008 by Kongsberg SIM.  All rights reserved.
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

#include "SoBaseP.h"
#include <Inventor/SbName.h>
#include <Inventor/SbString.h>
#include <Inventor/SoInput.h>
#include <Inventor/lists/SbPList.h>
#include <Inventor/lists/SoAuditorList.h>
#include <Inventor/misc/SoBase.h>
#include <Inventor/nodes/SoNode.h>

#include "threads/threadsutilp.h"

// Create a new SoNode-derived instance from the input stream.
SoNode *
SoBaseP::readNode(SoInput * in)
{
  SbName name;
  if (!in->read(name, TRUE)) return NULL;
  SoBase * node = NULL;
  if (!SoBase::readBase(in, name, node)) return NULL;
  assert(node->isOfType(SoNode::getClassTypeId()));
  return (SoNode *) node;
}

// Remove reference from a name to the instance pointer.
void
SoBaseP::removeName2Obj(SoBase * const base, const char * const name)
{
  CC_MUTEX_LOCK(SoBaseP::name2obj_mutex);
  SbPList * l = NULL;
  SbBool found = SoBaseP::name2obj->get(name, l);
  assert(found);

  const int i = l->find(base);
  assert(i >= 0);
  l->remove(i);

  CC_MUTEX_UNLOCK(SoBaseP::name2obj_mutex);
}

// Remove a reference from an instance pointer to its associated name.
void
SoBaseP::removeObj2Name(SoBase * const base, const char * const name)
{
  CC_MUTEX_LOCK(SoBaseP::obj2name_mutex);
  SoBaseP::obj2name->remove(base);
  CC_MUTEX_UNLOCK(SoBaseP::obj2name_mutex);
}

// Used to free the SbPLists in the name<->object dict.
void
SoBaseP::emptyName2ObjHash(const char * const &, SbPList * const & l, void *)
{
  delete l;
}

void * SoBaseP::mutex = NULL;
void * SoBaseP::name2obj_mutex = NULL;
void * SoBaseP::obj2name_mutex = NULL;
void * SoBaseP::auditor_mutex = NULL;
void * SoBaseP::global_mutex = NULL;

SbHash<SoAuditorList *, const SoBase *> * SoBaseP::auditordict = NULL;

// Only a small number of SoBase derived objects will under usual
// conditions have designated names, so we use a couple of static
// dictionary objects to keep track of them. Since we avoid storing a
// pointer for each and every object, we'll cut down on a decent
// amount of memory use this way (SoBase should be kept as slim as
// possible, as any dead weight is brought along in a lot of objects).
SbHash<SbPList *, const char *> * SoBaseP::name2obj = NULL;
SbHash<const char *, const SoBase *> * SoBaseP::obj2name = NULL;

// This is used for debugging purposes: it stores a pointer to all
// SoBase-derived objects that have been allocated and not
// deallocated.
SbBool SoBaseP::trackbaseobjects = FALSE;
void * SoBaseP::allbaseobj_mutex = NULL;
SoBaseSet * SoBaseP::allbaseobj = NULL; // maps from SoBase * to NULL

void
SoBaseP::auditordict_cb(const SoBase * const &, SoAuditorList * const & l, void *)
{
  delete l;
}

void
SoBaseP::cleanup_auditordict(void)
{
  if (SoBaseP::auditordict) {
    SoBaseP::auditordict->apply(SoBaseP::auditordict_cb, NULL);
    delete SoBaseP::auditordict;
    SoBaseP::auditordict = NULL;
  }
}

void
SoBaseP::check_for_leaks(void)
{
#if COIN_DEBUG
  if (SoBaseP::trackbaseobjects) {
    SbList<const SoBase *> keys;
    SoBaseP::allbaseobj->makeKeyList(keys);
    const unsigned int len = keys.getLength();
    if (len > 0) {
      // Use printf()s, in case SoDebugError has been made defunct by
      // previous coin_atexit() work.
      (void)printf("\nSoBase-derived instances not deallocated:\n");

      for (unsigned int i=0; i < len; i++) {
        const SoBase * base = keys[i];
        base->assertAlive();
        const SbName name = base->getName();
        const SoType t = base->getTypeId();
        SbString s;
        s.sprintf("\"%s\"", name.getString());
        (void)printf("\t%p type==(0x%04x, '%s') name=='%s'\n",
                     base, t.getKey(), t.getName().getString(),
                     name == "" ? "no name" : s.getString());
      }
      (void)printf("\n");
    }
  }
#endif // COIN_DEBUG
}

