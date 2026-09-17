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

#include <Inventor/SbString.h>

#include "CoinTest.h"
#include "misc/SbHash.h"

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

class SbHashIndexProbe : public SbHash<unsigned int, int> {
public:
  SbHashIndexProbe(unsigned int sizearg) : SbHash<unsigned int, int>(sizearg) { }

  unsigned int bucketIndex(unsigned int key) const
  {
    return this->getIndex(key);
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
}

BOOST_AUTO_TEST_CASE(SbHash_preserves_legacy_bucket_mapping)
{
  static_assert(noexcept(SbHashFunc(0U)),
                "built-in SbHash functions must be non-throwing");
  SbHashIndexProbe hash(257);
  BOOST_CHECK_EQUAL(hash.bucketIndex(0U), 0U);
  BOOST_CHECK_EQUAL(hash.bucketIndex(1U), 1U);
  BOOST_CHECK_EQUAL(hash.bucketIndex(255U), 255U);
  BOOST_CHECK_EQUAL(hash.bucketIndex(258U), 1U);
}

BOOST_AUTO_TEST_CASE(SbHash_hashes_c_strings_without_an_SbString_temporary)
{
  const char * text = "SbHash";
  const SbString string(text);

  static_assert(noexcept(SbHashFunc(text)),
                "C-string hashing must satisfy the SbHash noexcept contract");
  BOOST_CHECK_EQUAL(SbHashFunc(static_cast<const char *>(NULL)), 0U);
  BOOST_CHECK_EQUAL(SbHashFunc(text), SbHashFunc(string));
}
