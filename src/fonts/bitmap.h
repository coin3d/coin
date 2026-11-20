#ifndef COIN_FONTS_BITMAP_H
#define COIN_FONTS_BITMAP_H

/**************************************************************************\
 * Copyright (c) Kongsberg Oil & Gas Technologies AS
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

#ifndef COIN_INTERNAL
#error this is a private header file
#endif

#include "fonts/common.h"
#include <climits>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <limits>

// Consumers use int pixel dimensions, including the padded monochrome width.
inline bool
coin_font_bitmap_size(unsigned int rows, unsigned int width,
                      unsigned int pitch, SbBool mono, size_t & bytes)
{
  bytes = 0;
  if (rows > INT_MAX || width > INT_MAX ||
      pitch > unsigned(INT_MAX) / (mono ? 8u : 1u)) return false;
  if (rows == 0 || width == 0) return true;
  const size_t required = mono ? (size_t(width) + 7) / 8 : size_t(width);
  if (pitch < required || size_t(rows) >
      size_t(std::numeric_limits<ptrdiff_t>::max()) / pitch) return false;
  bytes = size_t(rows) * pitch;
  return true;
}

// Source is the FreeType buffer base: negative pitch stores the bottom row
// first. Move to the top row before stepping by the signed pitch.
// Normalize to a positive-pitch, top-down buffer owned by the caller.
inline bool
coin_font_bitmap_copy(unsigned int rows, unsigned int width, int pitch,
                      const unsigned char * source, SbBool mono,
                      unsigned char * & buffer, unsigned int & stride,
                      void * (*allocate)(size_t) = std::malloc)
{
  const uint64_t magnitude = pitch < 0 ? uint64_t(-int64_t(pitch)) : uint64_t(pitch);
  if (magnitude > UINT_MAX) return false;
  size_t bytes;
  if (!coin_font_bitmap_size(rows, width, unsigned(magnitude), mono, bytes)) return false;
  unsigned char * copy = NULL;
  if (bytes) {
    if (!source) return false;
    copy = static_cast<unsigned char *>(allocate(bytes));
    if (!copy) return false;
    const unsigned char * top = pitch < 0 ?
      source + size_t(rows - 1) * size_t(magnitude) : source;
    for (unsigned int y = 0; y < rows; ++y) {
      std::memcpy(copy + size_t(y) * size_t(magnitude),
                  top + ptrdiff_t(y) * ptrdiff_t(pitch), size_t(magnitude));
    }
  }
  buffer = copy;
  stride = unsigned(magnitude);
  return true;
}

// Reverse the owned buffer in place, avoiding a second allocation for OpenGL.
inline bool
coin_font_bitmap_flip(cc_font_bitmap & bitmap)
{
  size_t bytes;
  if (!coin_font_bitmap_size(bitmap.rows, bitmap.width, bitmap.pitch, bitmap.mono, bytes)) return false;
  if (!bytes) return true;
  if (!bitmap.buffer) return false;
  for (unsigned int y = 0; y < bitmap.rows / 2; ++y) {
    unsigned char * a = bitmap.buffer + size_t(y) * bitmap.pitch;
    unsigned char * b = bitmap.buffer + size_t(bitmap.rows - y - 1) * bitmap.pitch;
    for (unsigned int x = 0; x < bitmap.pitch; ++x) {
      const unsigned char value = a[x]; a[x] = b[x]; b[x] = value;
    }
  }
  return true;
}

#endif // COIN_FONTS_BITMAP_H
