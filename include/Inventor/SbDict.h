/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
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

#ifndef __SBDICT_H__
#define __SBDICT_H__

#include <Inventor/SbBasic.h>
#include <stddef.h>

class SbPList;
class SbDictEntry;


class SbDict {
public:
  // The "entries" argument of the second constructor listed below has
  // a default value under Open Inventor, but that doesn't work with
  // the SGI MIPS CC v6.2 compiler (and I also have vague memories of
  // a bug report for the Sun C++ compiler on this exact case).
  //
  // What happens is that dictionary definitions like "SbDict mydict;"
  // tries to call the implicit default no-argument constructor, and
  // _not_ the constructor taking an integer argument with its default
  // value. This spells disaster, as none of the object variables will
  // be initialized.
  //
  // So to work around this compiler bug and still keep API
  // compatibility with Open Inventor, I've added an explicit default
  // constructor.
  //
  // mortene.

  SbDict(void);
  SbDict(const int entries);
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
  void commonConstructor(const int entries);
  unsigned long (*hashfunc)(const unsigned long key);
  int tablesize;
  SbDictEntry ** buckets;
  SbDictEntry *findEntry(const unsigned long key,
                         const unsigned long bucketnum,
                         SbDictEntry **prev = NULL) const;
  static void copyval(unsigned long key, void * value, void * data);

};

#endif // !__SBDICT_H__
