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

#include <cstdint>
#include <map>
#include <new>

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

BOOST_AUTO_TEST_CASE(SbSmallMap_spill_failure_preserves_contents_and_recovers)
{
  SbSmallMap<int, int> map;
  for (int key = 0; key < 4; ++key) {
    BOOST_REQUIRE(map.put(key, key * 10));
  }

  SbSmallMap<int, int>::failNextAllocationForTesting();
  bool threw = false;
  try {
    map.put(4, 40);
  }
  catch (const std::bad_alloc &) {
    threw = true;
  }
  BOOST_REQUIRE(threw);

  BOOST_CHECK_EQUAL(map.getNumElements(), 4U);
  for (int key = 0; key < 4; ++key) {
    int value = -1;
    BOOST_REQUIRE(map.get(key, value));
    BOOST_CHECK_EQUAL(value, key * 10);
  }
  int missing = -1;
  BOOST_CHECK(!map.get(4, missing));

  BOOST_REQUIRE(map.put(4, 40));
  BOOST_CHECK_EQUAL(map.getNumElements(), 5U);
  BOOST_REQUIRE(map.get(4, missing));
  BOOST_CHECK_EQUAL(missing, 40);
}

BOOST_AUTO_TEST_CASE(SbSmallMap_inline_copy_clear_and_reuse)
{
  SbSmallMap<int, int> original;
  original.put(-1, 10);
  original.put(0, 20);
  original.put(1, 30);
  original.put(2, 40);

  SbSmallMap<int, int> copy(original);
  original[-1] = 99;
  original.erase(0);

  int value = 0;
  BOOST_REQUIRE(copy.get(-1, value));
  BOOST_CHECK_EQUAL(value, 10);
  BOOST_REQUIRE(copy.get(0, value));
  BOOST_CHECK_EQUAL(value, 20);

  SbSmallMap<int, int> assigned;
  assigned.put(100, 100);
  assigned = copy;
  SbSmallMap<int, int> * same = &assigned;
  assigned = *same;
  BOOST_CHECK_EQUAL(assigned.getNumElements(), 4U);
  BOOST_REQUIRE(assigned.get(2, value));
  BOOST_CHECK_EQUAL(value, 40);

  assigned.clear();
  BOOST_CHECK(assigned.const_begin() == assigned.const_end());
  BOOST_CHECK_EQUAL(assigned.erase(1), static_cast<size_t>(0));
  BOOST_REQUIRE(assigned.put(7, 70));
  BOOST_REQUIRE(assigned.get(7, value));
  BOOST_CHECK_EQUAL(value, 70);
}

BOOST_AUTO_TEST_CASE(SbSmallMap_erase_swap_preserves_entries)
{
  SbSmallMap<int, int> map;
  for (int key = 0; key < 7; ++key) {
    BOOST_REQUIRE(map.put(key, key * 10));
  }

  // Exercise removal from the beginning, middle and end. SbList::removeFast()
  // fills a hole with the last entry, so iteration order is deliberately not
  // part of the contract.
  BOOST_CHECK_EQUAL(map.erase(0), static_cast<size_t>(1));
  BOOST_CHECK_EQUAL(map.erase(3), static_cast<size_t>(1));
  BOOST_CHECK_EQUAL(map.erase(6), static_cast<size_t>(1));
  BOOST_CHECK_EQUAL(map.getNumElements(), 4U);

  for (int key = 0; key < 7; ++key) {
    int value = -1;
    const SbBool found = map.get(key, value);
    if (key == 0 || key == 3 || key == 6) {
      BOOST_CHECK(!found);
    }
    else {
      BOOST_REQUIRE(found);
      BOOST_CHECK_EQUAL(value, key * 10);
    }
  }
}

BOOST_AUTO_TEST_CASE(SbSmallMap_differential_operation_sequence)
{
  SbSmallMap<int, int> map;
  std::map<int, int> reference;
  unsigned int state = 0x6d2b79f5U;

  for (int step = 0; step < 2000; ++step) {
    state = state * 1664525U + 1013904223U;
    const int key = static_cast<int>((state >> 16) % 17U) - 8;
    const int value = step * 3 + 1;

    switch (state % 5U) {
    case 0: {
      const bool inserted = reference.find(key) == reference.end();
      reference[key] = value;
      BOOST_CHECK_EQUAL(static_cast<bool>(map.put(key, value)), inserted);
      break;
    }
    case 1:
      BOOST_CHECK_EQUAL(map.erase(key), reference.erase(key));
      break;
    case 2:
      map[key] += 1;
      reference[key] += 1;
      break;
    case 3: {
      int actual = -1;
      const std::map<int, int>::const_iterator expected = reference.find(key);
      BOOST_CHECK_EQUAL(static_cast<bool>(map.get(key, actual)),
                        expected != reference.end());
      if (expected != reference.end()) BOOST_CHECK_EQUAL(actual, expected->second);
      break;
    }
    default: {
      const SbSmallMap<int, int> copy(map);
      BOOST_CHECK_EQUAL(copy.getNumElements(), map.getNumElements());
      break;
    }
    }

    BOOST_CHECK_EQUAL(map.getNumElements(),
                      static_cast<unsigned int>(reference.size()));
    for (int candidate = -8; candidate <= 8; ++candidate) {
      int actual = -1;
      const std::map<int, int>::const_iterator expected = reference.find(candidate);
      BOOST_CHECK_EQUAL(static_cast<bool>(map.get(candidate, actual)),
                        expected != reference.end());
      if (expected != reference.end()) BOOST_CHECK_EQUAL(actual, expected->second);
    }
  }
}

BOOST_AUTO_TEST_CASE(SbSmallMap_bump_specular_context_registry_pattern)
{
  struct ProgramRecord {
    unsigned int dirlight;
    unsigned int pointlight;
    unsigned int fragment;
  };

  SbSmallMap<uint32_t, ProgramRecord> specular;

  for (uint32_t context = 0; context < 2; ++context) {
    const ProgramRecord specularprogram = {
      context + 10U, context + 20U, context + 30U
    };
    BOOST_REQUIRE(specular.put(context, specularprogram));
  }

  for (uint32_t context = 0; context < 2; ++context) {
    ProgramRecord specularprogram = { 0, 0, 0 };
    BOOST_REQUIRE(specular.get(context, specularprogram));
    BOOST_CHECK_EQUAL(specularprogram.dirlight, context + 10U);
    BOOST_CHECK_EQUAL(specularprogram.pointlight, context + 20U);
    BOOST_CHECK_EQUAL(specularprogram.fragment, context + 30U);
  }

  const ProgramRecord replacement = { 70U, 80U, 90U };
  BOOST_CHECK(!specular.put(1U, replacement));
  ProgramRecord updated = { 0, 0, 0 };
  BOOST_REQUIRE(specular.get(1U, updated));
  BOOST_CHECK_EQUAL(updated.dirlight, 70U);
  BOOST_CHECK_EQUAL(updated.pointlight, 80U);
  BOOST_CHECK_EQUAL(updated.fragment, 90U);
  BOOST_CHECK_EQUAL(specular.getNumElements(), 2U);
}
