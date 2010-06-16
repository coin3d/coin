#ifndef COIN_SOBASEP_H
#define COIN_SOBASEP_H

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
typedef SbHash<const SoBase *, void *> SoBaseSet;

class SoBase::PImpl {
public:
  static const char OPEN_BRACE;
  static const char CLOSE_BRACE;

  static const char END_OF_LINE[];
  static const char DEF_KEYWORD[];
  static const char USE_KEYWORD[];
  static const char NULL_KEYWORD[];
  static const char ROUTE_KEYWORD[];
  static const char PROTO_KEYWORD[];
  static const char EXTERNPROTO_KEYWORD[];

  static void * mutex;
  static void * name2obj_mutex;
  static void * obj2name_mutex;
  static void * auditor_mutex;
  static void * global_mutex;

  static SbHash<const SoBase *, SoAuditorList *> * auditordict;
  static SbHash<const char *, SbPList *> * name2obj;
  static SbHash<const SoBase *, const char *> * obj2name;

  static SbBool trackbaseobjects;
  static void * allbaseobj_mutex;
  static SoBaseSet * allbaseobj; // maps from SoBase * to NULL

  static SbString * refwriteprefix;
  static SbBool tracerefs;
  static uint32_t writecounter;

  static void cleanup_auditordict(void);

  static void removeName2Obj(SoBase * const base, const char * const name);
  static void removeObj2Name(SoBase * const base, const char * const name);

  static void check_for_leaks(void);

  static SbBool readReference(SoInput * in, SoBase *& base);
  static SbBool readBase(SoInput * in, SbName & classname, SoBase *& base);
  static SbBool readBaseInstance(SoInput * in, const SbName & classname,
                                 const SbName & refname, SoBase *& base);

  static SoBase * createInstance(SoInput * in, const SbName & classname);
  static void flushInput(SoInput * in);

  static void rbptree_notify_cb(void * auditor, void * type, void * closure);

  static SoNode * readNode(SoInput * in);

  // only needed for the callback from cc_rbptree_traverse
  struct NotifyData {
    int cnt;
    int total;
    SoNotList * list;
    SoBase * thisp;
  };

}; // SoBase::PImpl

#endif // !COIN_SOBASEP_H
