/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#ifndef COIN_SBDICT_H
#define COIN_SBDICT_H

#include <Inventor/SbBasic.h>
#include <stddef.h>

class SbPList;
class SbDictEntry;


class COIN_DLL_API SbDict {
public:
  SbDict(const int entries = 251);
  SbDict(const SbDict & from);
  ~SbDict();

  SbDict & operator=(const SbDict & from);

  void applyToAll(void (* rtn)(unsigned long key, void * value)) const;
  void applyToAll(void (* rtn)(unsigned long key, void * value, void * data),
                  void * data) const;
  void clear(void);

  SbBool enter(const unsigned long key, void * const value);
  SbBool find(const unsigned long key, void *& value) const;
  void makePList(SbPList & keys, SbPList & values);
  SbBool remove(const unsigned long key);

  void setHashingFunction(unsigned long (*func)(const unsigned long key));

private:
  unsigned long (*hashfunc)(const unsigned long key);
  int tablesize;
  SbDictEntry ** buckets;
  SbDictEntry *findEntry(const unsigned long key,
                         const unsigned long bucketnum,
                         SbDictEntry **prev = NULL) const;
  static void copyval(unsigned long key, void * value, void * data);

};

#endif // !COIN_SBDICT_H
