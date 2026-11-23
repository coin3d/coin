/**************************************************************************\
* Copyright (c) Kongsberg Oil & Gas Technologies AS
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are
* met:
*
* 1. Redistributions of source code must retain the above copyright notice,
*    this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the distribution.
* 3. Neither the name of the copyright holder nor the names of its
*    contributors may be used to endorse or promote products derived from
*    this software without specific prior written permission.
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

#include "CoinTest.h"

#include <Inventor/SbImage.h>
#include <Inventor/SbVec3f.h>
#include <Inventor/engines/SoHeightMapToNormalMap.h>

#include <algorithm>
#include <cstddef>
#include <vector>

namespace {

typedef std::vector<unsigned char> Bytes;

void
checkEqual(const Bytes & actual, const Bytes & expected)
{
  BOOST_REQUIRE_EQUAL(actual.size(), expected.size());
  for (std::size_t index = 0; index < actual.size(); ++index) {
    BOOST_CHECK_EQUAL(static_cast<int>(actual[index]),
                      static_cast<int>(expected[index]));
  }
}

unsigned char
heightAt(const Bytes & source, const int width, const int height,
         const int components, int x, int y)
{
  x = (x + width) % width;
  y = (y + height) % height;
  return source[(static_cast<std::size_t>(y) * width + x) * components];
}

Bytes
referenceNormalMap(const Bytes & source, const int width, const int height,
                   const int components)
{
  Bytes expected(static_cast<std::size_t>(width) * height * 3);
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      const int dx =
        heightAt(source, width, height, components, x - 1, y - 1) +
        2 * heightAt(source, width, height, components, x - 1, y) +
        heightAt(source, width, height, components, x - 1, y + 1) -
        heightAt(source, width, height, components, x + 1, y - 1) -
        2 * heightAt(source, width, height, components, x + 1, y) -
        heightAt(source, width, height, components, x + 1, y + 1);
      const int dy =
        heightAt(source, width, height, components, x - 1, y - 1) +
        2 * heightAt(source, width, height, components, x, y - 1) +
        heightAt(source, width, height, components, x + 1, y - 1) -
        heightAt(source, width, height, components, x - 1, y + 1) -
        2 * heightAt(source, width, height, components, x, y + 1) -
        heightAt(source, width, height, components, x + 1, y + 1);

      SbVec3f normal(static_cast<float>(dx) / 255.0f,
                     static_cast<float>(-dy) / 255.0f, 1.0f);
      normal.normalize();

      const std::size_t destination =
        (static_cast<std::size_t>(y) * width + x) * 3;
      for (int component = 0; component < 3; ++component) {
        expected[destination + component] = static_cast<unsigned char>(
          std::min(255.0f, (normal[component] + 1.0f) * 128.0f));
      }
    }
  }
  return expected;
}

Bytes
convert(const Bytes & source, const int width, const int height,
        const int components)
{
  SbImage destination;
  SoHeightMapToNormalMap::convert(&source[0], SbVec2s(width, height),
                                  components, destination);

  SbVec2s resultSize;
  int resultComponents = 0;
  const unsigned char * result =
    destination.getValue(resultSize, resultComponents);
  BOOST_REQUIRE_EQUAL(resultSize[0], width);
  BOOST_REQUIRE_EQUAL(resultSize[1], height);
  BOOST_REQUIRE_EQUAL(resultComponents, 3);
  BOOST_REQUIRE(result != NULL);
  return Bytes(result, result + static_cast<std::size_t>(width) * height * 3);
}

void
checkReference(const Bytes & source, const int width, const int height,
               const int components)
{
  const Bytes actual = convert(source, width, height, components);
  const Bytes expected = referenceNormalMap(source, width, height, components);
  checkEqual(actual, expected);
}

Bytes
interleaved(const Bytes & heights, const int components)
{
  Bytes result(heights.size() * components);
  for (std::size_t pixel = 0; pixel < heights.size(); ++pixel) {
    result[pixel * components] = heights[pixel];
    for (int component = 1; component < components; ++component) {
      result[pixel * components + component] = static_cast<unsigned char>(
        (pixel * 67 + component * 43 + 19) & 0xff);
    }
  }
  return result;
}

} // namespace

BOOST_AUTO_TEST_CASE(heightmap_constant_levels_are_exactly_flat)
{
  const int dimensions[][2] = {
    { 1, 1 }, { 1, 7 }, { 9, 1 }, { 2, 5 }, { 7, 3 }
  };
  const unsigned char flat[] = { 128, 128, 255 };

  for (int components = 1; components <= 4; ++components) {
    for (std::size_t shape = 0;
         shape < sizeof(dimensions) / sizeof(dimensions[0]); ++shape) {
      const int width = dimensions[shape][0];
      const int height = dimensions[shape][1];
      for (int level = 0; level <= 255; ++level) {
        Bytes source(static_cast<std::size_t>(width) * height * components,
                     static_cast<unsigned char>(level));
        const Bytes actual = convert(source, width, height, components);
        for (std::size_t pixel = 0; pixel < actual.size() / 3; ++pixel) {
          for (int component = 0; component < 3; ++component) {
            BOOST_CHECK_EQUAL(static_cast<int>(actual[pixel * 3 + component]),
                              static_cast<int>(flat[component]));
          }
        }
      }
    }
  }
}

BOOST_AUTO_TEST_CASE(heightmap_sobel_wrap_signs_normalization_and_quantization)
{
  const unsigned char values[] = {
      3,  17,  91, 211,  44,
    250, 101,   7,  64, 139,
     32,  53, 199,  12, 177
  };
  const Bytes heights(values, values + sizeof(values));
  for (int components = 1; components <= 4; ++components) {
    checkReference(interleaved(heights, components), 5, 3, components);
  }
}

BOOST_AUTO_TEST_CASE(heightmap_degenerate_dimensions_keep_periodic_sobel)
{
  const unsigned char rowValues[] = { 0, 1, 64, 127, 200, 254, 255 };
  const Bytes row(rowValues, rowValues + sizeof(rowValues));
  const unsigned char columnValues[] = { 255, 2, 87, 132, 203, 17 };
  const Bytes column(columnValues, columnValues + sizeof(columnValues));

  for (int components = 1; components <= 4; ++components) {
    checkReference(interleaved(row, components), 7, 1, components);
    checkReference(interleaved(column, components), 1, 6, components);
  }
}

BOOST_AUTO_TEST_CASE(heightmap_only_first_component_is_height)
{
  const unsigned char values[] = {
    0, 19, 80, 255,
    7, 61, 92, 173,
    4, 44, 201, 121
  };
  const Bytes heights(values, values + sizeof(values));
  const Bytes single = convert(heights, 4, 3, 1);

  for (int components = 2; components <= 4; ++components) {
    const Bytes multi = convert(interleaved(heights, components),
                                4, 3, components);
    checkEqual(single, multi);
  }
}

BOOST_AUTO_TEST_CASE(heightmap_is_invariant_under_representable_offsets)
{
  const unsigned char values[] = {
     0,  2,  5,  7,  4,
     3,  8,  1,  6,  9,
    11, 10,  4, 12,  2,
     8,  3, 13,  7,  6
  };
  const Bytes base(values, values + sizeof(values));
  const Bytes expected = convert(base, 5, 4, 1);

  const int offsets[] = { 1, 17, 63, 127, 200, 242 };
  for (std::size_t index = 0;
       index < sizeof(offsets) / sizeof(offsets[0]); ++index) {
    Bytes shifted(base);
    for (std::size_t pixel = 0; pixel < shifted.size(); ++pixel) {
      shifted[pixel] = static_cast<unsigned char>(shifted[pixel] + offsets[index]);
    }
    const Bytes actual = convert(shifted, 5, 4, 1);
    checkEqual(actual, expected);
  }
}
