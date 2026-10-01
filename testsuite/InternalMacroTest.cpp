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

/**
 * Prove that internal tests can include private implementation headers while
 * retaining the ABI role of ordinary Coin consumers.
 */
#ifndef COIN_INTERNAL
#error InternalMacroTest must have access to private Coin interfaces.
#endif

#ifdef COIN_BUILDING_COIN
#error InternalMacroTest is not part of the Coin library.
#endif

#include "CoinTest.h"
#include "misc/SbHash.h"
#include <type_traits>

static_assert(sizeof(SbHash<int, int>) > 0, "private Coin type is unavailable");

namespace {

class SbHashStatsProbe : public SbHash<unsigned int, int> {
public:
  SbHashStatsProbe(unsigned int sizearg) : SbHash<unsigned int, int>(sizearg) { }

  void stats(int & bucketsUsed, int & buckets, int & elements,
             float & average, int & maximum)
  {
    this->getStats(bucketsUsed, buckets, elements, average, maximum);
  }
};

} // namespace

BOOST_AUTO_TEST_CASE(SbHash_self_assignment_preserves_entries)
{
  SbHash<unsigned int, int> hash(3);
  hash.put(1, 10);
  hash.put(2, 20);

  hash = hash;

  int value = 0;
  BOOST_CHECK_EQUAL(hash.getNumElements(), 2);
  BOOST_REQUIRE(hash.get(1, value));
  BOOST_CHECK_EQUAL(value, 10);
  BOOST_REQUIRE(hash.get(2, value));
  BOOST_CHECK_EQUAL(value, 20);
}

BOOST_AUTO_TEST_CASE(SbHash_mutable_iterator_visits_entries)
{
  SbHash<unsigned int, int> hash(3);
  BOOST_CHECK(hash.begin() == hash.end());

  BOOST_REQUIRE(hash.put(1, 10));
  BOOST_REQUIRE(hash.put(2, 20));
  BOOST_REQUIRE(hash.put(3, 30));

  unsigned int count = 0;
  int sum = 0;
  for (SbHash<unsigned int, int>::iterator it = hash.begin();
       it != hash.end(); ++it) {
    ++count;
    sum += it->obj;
    it->obj += 1;
  }

  BOOST_CHECK_EQUAL(count, 3U);
  BOOST_CHECK_EQUAL(sum, 60);

  int value = 0;
  BOOST_REQUIRE(hash.get(1, value));
  BOOST_CHECK_EQUAL(value, 11);
  BOOST_REQUIRE(hash.get(2, value));
  BOOST_CHECK_EQUAL(value, 21);
  BOOST_REQUIRE(hash.get(3, value));
  BOOST_CHECK_EQUAL(value, 31);
}

BOOST_AUTO_TEST_CASE(SbHash_mutable_iterator_skips_empty_buckets_and_chains)
{
  SbHash<unsigned int, int> hash(11, 100.0f); // Keep collision chains in place.
  BOOST_REQUIRE(hash.put(1, 10));
  BOOST_REQUIRE(hash.put(12, 20));
  BOOST_REQUIRE(hash.put(23, 30));
  BOOST_REQUIRE(hash.put(9, 40));

  unsigned int seen = 0;
  unsigned int count = 0;
  SbHash<unsigned int, int>::iterator it = hash.begin();
  for (; it != hash.end() && count < 5; ++it) {
    const unsigned int bit = 1U << (it->key % 32);
    BOOST_CHECK_EQUAL(seen & bit, 0U);
    seen |= bit;
    it->obj += 1;
    ++count;
  }
  BOOST_CHECK(it == hash.end());
  BOOST_CHECK_EQUAL(count, 4U);

  SbHash<unsigned int, int>::iterator mutable_mid = hash.begin();
  ++mutable_mid; // second entry of the collision chain
  SbHash<unsigned int, int>::const_iterator readonly_mid(mutable_mid);
  BOOST_CHECK(mutable_mid == readonly_mid);
  while (mutable_mid != hash.end()) {
    BOOST_CHECK_EQUAL(mutable_mid->key, readonly_mid->key);
    ++mutable_mid;
    ++readonly_mid;
  }
  BOOST_CHECK(readonly_mid == hash.const_end());
  BOOST_CHECK_EQUAL(seen, (1U << 1) | (1U << 12) |
                          (1U << 23) | (1U << 9));

  int value = 0;
  BOOST_REQUIRE(hash.get(1, value));
  BOOST_CHECK_EQUAL(value, 11);
  BOOST_REQUIRE(hash.get(12, value));
  BOOST_CHECK_EQUAL(value, 21);
  BOOST_REQUIRE(hash.get(23, value));
  BOOST_CHECK_EQUAL(value, 31);
  BOOST_REQUIRE(hash.get(9, value));
  BOOST_CHECK_EQUAL(value, 41);

  const SbHash<unsigned int, int>::const_iterator converted(hash.end());
  BOOST_CHECK(converted == hash.const_end());
  hash.clear();
  BOOST_CHECK(hash.begin() == hash.end());
}

BOOST_AUTO_TEST_CASE(SbHash_iterators_survive_repeated_resizes)
{
  SbHash<unsigned int, int> hash(3);
  for (unsigned int i = 0; i < 200; ++i) {
    const unsigned int key = (i * 37U) % 257U;
    BOOST_REQUIRE(hash.put(key, static_cast<int>(key + 1U)));
  }

  bool seen[257] = {};
  unsigned int count = 0;
  for (SbHash<unsigned int, int>::iterator it = hash.begin();
       it != hash.end(); ++it) {
    BOOST_REQUIRE(it->key < 257U);
    BOOST_CHECK(!seen[it->key]);
    seen[it->key] = true;
    BOOST_CHECK_EQUAL(it->obj, static_cast<int>(it->key + 1U));
    ++count;
  }
  BOOST_CHECK_EQUAL(count, 200U);

  const SbHash<unsigned int, int> & readonly = hash;
  bool seen_const[257] = {};
  count = 0;
  for (SbHash<unsigned int, int>::const_iterator it = readonly.begin();
       it != readonly.end(); ++it) {
    BOOST_REQUIRE(it->key < 257U);
    BOOST_CHECK(seen[it->key]);
    BOOST_CHECK(!seen_const[it->key]);
    seen_const[it->key] = true;
    ++count;
  }
  BOOST_CHECK_EQUAL(count, 200U);
}

BOOST_AUTO_TEST_CASE(SbHash_const_begin_end_are_read_only)
{
  SbHash<unsigned int, int> hash(11, 100.0f);
  BOOST_REQUIRE(hash.put(1, 10));
  const SbHash<unsigned int, int> & readonly = hash;
  static_assert(std::is_same<decltype(readonly.begin()),
                             SbHash<unsigned int, int>::const_iterator>::value,
                "const SbHash must return const_iterator");
  BOOST_CHECK(readonly.begin() != readonly.end());
  BOOST_CHECK_EQUAL(readonly.begin()->obj, 10);
  BOOST_CHECK(readonly.const_begin() == readonly.begin());
  const SbHash<unsigned int, int>::const_iterator first = readonly.begin();
  BOOST_CHECK_EQUAL(first->obj, 10);
  BOOST_CHECK_EQUAL((*first).key, 1U);
  const SbHash<unsigned int, int>::iterator writable = hash.begin();
  writable->obj = 11;
  BOOST_CHECK_EQUAL((*writable).obj, 11);
  BOOST_CHECK(hash.begin() != readonly.end());
  BOOST_CHECK(readonly.end() != hash.begin());
  BOOST_CHECK(hash.end() == readonly.end());
  BOOST_CHECK(readonly.end() == hash.end());
}

BOOST_AUTO_TEST_CASE(SbHash_statistics_are_fractional_and_empty_safe)
{
  SbHashStatsProbe hash(3);
  int bucketsUsed = -1;
  int buckets = -1;
  int elements = -1;
  int maximum = -1;
  float average = -1.0f;

  hash.stats(bucketsUsed, buckets, elements, average, maximum);
  BOOST_CHECK_EQUAL(bucketsUsed, 0);
  BOOST_CHECK_EQUAL(elements, 0);
  BOOST_CHECK_EQUAL(average, 0.0f);

  BOOST_REQUIRE(hash.put(0, 1));
  BOOST_REQUIRE(hash.put(5, 2));
  BOOST_REQUIRE(hash.put(1, 3));
  hash.stats(bucketsUsed, buckets, elements, average, maximum);
  BOOST_CHECK_EQUAL(buckets, 5);
  BOOST_CHECK_EQUAL(bucketsUsed, 2);
  BOOST_CHECK_EQUAL(elements, 3);
  BOOST_CHECK_EQUAL(average, 1.5f);
  BOOST_CHECK_EQUAL(maximum, 2);

  hash.clear();
  hash.stats(bucketsUsed, buckets, elements, average, maximum);
  BOOST_CHECK_EQUAL(buckets, 5);
  BOOST_CHECK_EQUAL(bucketsUsed, 0);
  BOOST_CHECK_EQUAL(elements, 0);
  BOOST_CHECK_EQUAL(average, 0.0f);
  BOOST_CHECK_EQUAL(maximum, 0);
}
