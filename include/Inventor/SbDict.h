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

#ifndef __SBDICT_H__
#define __SBDICT_H__

#include <Inventor/SbBasic.h>

class SbPList;
class SbDictEntry;


class SbDict {
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

private:
  int tablesize;
  SbDictEntry ** buckets;
  SbDictEntry *& findEntry(const unsigned long key) const;
  static void addEntryToPLists(const unsigned long key, 
			       void * const value, 
			       void * const data);
  static void copyval(unsigned long key, void * value, void * data);
};

#endif // !__SBDICT_H__
