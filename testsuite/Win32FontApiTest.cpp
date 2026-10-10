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

// Issue #136: exercise the retained GDI name-query contract on real Windows.
#include "glue/win32api.h"
#include <algorithm>
#include <cstdio>
#include <vector>

int main()
{
  HDC dc = CreateCompatibleDC(NULL);
  if (!dc) return 1;
  int failures = 0;
  const int fonts[] = { DEFAULT_GUI_FONT, ANSI_FIXED_FONT };
  for (int font : fonts) {
    HGDIOBJ previous = SelectObject(dc, GetStockObject(font));
    if (!previous || previous == HGDI_ERROR) {
      ++failures;
      continue;
    }
    const int size = cc_win32()->GetTextFace(dc, 0, NULL);
    const int nativeSize = GetTextFace(dc, 0, NULL);
    if (size <= 0 || size != nativeSize) {
      ++failures;
    }
    else {
      const TCHAR guard = static_cast<TCHAR>(0x7f);
      std::vector<TCHAR> wrapped(size + 1, guard), native(size + 1, guard);
      const int copied = cc_win32()->GetTextFace(dc, size, wrapped.data());
      const int nativeCopied = GetTextFace(dc, size, native.data());
      const bool terminated = std::find(wrapped.begin(), wrapped.begin() + size,
                                        static_cast<TCHAR>(0)) != wrapped.begin() + size;
      if (copied <= 0 || copied != nativeCopied || !terminated ||
          wrapped[size] != guard || native[size] != guard || wrapped != native) {
        ++failures;
      }
      if (cc_win32()->GetTextFace(dc, 0, NULL) != size) ++failures;
    }
    if (!SelectObject(dc, previous)) ++failures;
  }
  if (!DeleteDC(dc)) ++failures;
  if (failures) std::fprintf(stderr, "Win32 font name query failures: %d\n", failures);
  return failures ? 1 : 0;
}
