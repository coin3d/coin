#ifndef COIN_SOBASEP_H
#define COIN_SOBASEP_H

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

#include "misc/SbHash.h"

class SoBase;
class SoNode;
class SoAuditorList;
class SbPList;
class SoInput;

#ifndef COIN_INTERNAL
#error this is a private header file
#endif /* !COIN_INTERNAL */


// FIXME: should implement and use a proper set-abstraction
// datatype. 20050524 mortene.
typedef SbHash<void *, const SoBase *> SoBaseSet;

class SoBaseP {
public:
  static SoNode * readNode(SoInput * in);

  static void * mutex;
  static void * name2obj_mutex;
  static void * obj2name_mutex;
  static void * auditor_mutex;
  static void * global_mutex;

  static SbHash<SoAuditorList *, const SoBase *> * auditordict;
  static SbHash<SbPList *, const char *> * name2obj;
  static SbHash<const char *, const SoBase *> * obj2name;

  static void auditordict_cb(const SoBase * const & key, SoAuditorList * const & value, void * closure);
  static void cleanup_auditordict(void);

  static void removeName2Obj(SoBase * const base, const char * const name);
  static void removeObj2Name(SoBase * const base, const char * const name);

  static void emptyName2ObjHash(const char * const & n, SbPList * const & l, void * closure);

  static void check_for_leaks(void);
  static SbBool trackbaseobjects;
  static void * allbaseobj_mutex;
  static SoBaseSet * allbaseobj; // maps from SoBase * to NULL
};

#endif // !COIN_SOBASEP_H
