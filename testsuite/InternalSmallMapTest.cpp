/**************************************************************************\
 * Copyright (c) 2026 FreeCAD contributors
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
\**************************************************************************/

#include "misc/SbSmallMap.h"
#include "CoinTest.h"

BOOST_AUTO_TEST_CASE(SbSmallMap_inline_and_spill_storage)
{
  SbSmallMap<unsigned int, void *> map;
  int values[6] = { 0, 1, 2, 3, 4, 5 };

  for (unsigned int i = 0; i < 4; ++i) {
    BOOST_REQUIRE(map.put(i, &values[i]));
  }
  BOOST_CHECK_EQUAL(map.getNumElements(), 4U);

  BOOST_CHECK(!map.put(2U, &values[5]));
  void * value = NULL;
  BOOST_REQUIRE(map.get(2U, value));
  BOOST_CHECK_EQUAL(value, static_cast<void *>(&values[5]));

  // The fifth distinct key exercises SbList's transition from its four-entry
  // inline buffer to dynamically allocated contiguous storage.
  BOOST_REQUIRE(map.put(4U, &values[4]));
  BOOST_REQUIRE(map.put(5U, &values[5]));
  for (unsigned int i = 0; i < 6; ++i) {
    BOOST_REQUIRE(map.get(i, value));
  }

  BOOST_CHECK_EQUAL(map.erase(1U), static_cast<size_t>(1));
  BOOST_CHECK_EQUAL(map.erase(1U), static_cast<size_t>(0));
  BOOST_CHECK_EQUAL(map.getNumElements(), 5U);
  BOOST_CHECK(!map.get(1U, value));
  BOOST_REQUIRE(map.get(5U, value));
  BOOST_CHECK_EQUAL(value, static_cast<void *>(&values[5]));

  // Copies made after spilling must own independent heap storage.
  SbSmallMap<unsigned int, void *> copy(map);
  map.clear();
  BOOST_CHECK_EQUAL(map.getNumElements(), 0U);
  BOOST_CHECK_EQUAL(copy.getNumElements(), 5U);
  BOOST_REQUIRE(copy.get(5U, value));
  BOOST_CHECK_EQUAL(value, static_cast<void *>(&values[5]));

  SbSmallMap<unsigned int, void *> assigned;
  assigned = copy;
  copy.erase(5U);
  BOOST_REQUIRE(assigned.get(5U, value));
  BOOST_CHECK_EQUAL(value, static_cast<void *>(&values[5]));

  unsigned int visited = 0;
  for (SbSmallMap<unsigned int, void *>::const_iterator it = assigned.const_begin();
       it != assigned.const_end(); ++it) {
    BOOST_CHECK(it->obj != NULL);
    ++visited;
  }
  BOOST_CHECK_EQUAL(visited, 5U);

  SbList<unsigned int> keys;
  assigned.makeKeyList(keys);
  BOOST_CHECK_EQUAL(keys.getLength(), 5);

  assigned.clear();
  BOOST_CHECK_EQUAL(assigned.getNumElements(), 0U);
  BOOST_CHECK(!assigned.get(0U, value));
}

BOOST_AUTO_TEST_CASE(SbSmallMap_subscript_and_find)
{
  SbSmallMap<int, int> map;
  map[7] = 42;

  SbSmallMap<int, int>::const_iterator found = map.find(7);
  BOOST_REQUIRE(found != map.const_end());
  BOOST_CHECK_EQUAL(found->key, 7);
  BOOST_CHECK_EQUAL(found->obj, 42);
  BOOST_CHECK(map.find(8) == map.const_end());
}
