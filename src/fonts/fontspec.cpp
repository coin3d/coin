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

#include "fonts/fontspec.h"

#include <cstring>
#include <string>
#include <cctype>

#include "glue/freetype.h"

void
cc_fontspec_construct(cc_font_specification * spec,
                      const char * name_style, float size, float complexity)
{
  spec->size = size;
  spec->complexity = complexity;
  cc_string_construct(&spec->name);
  cc_string_construct(&spec->style);

  const std::string text = name_style ? name_style : "";
  const auto trim = [](const std::string & value) {
    size_t first = 0, last = value.size();
    while (first < last && std::isspace(static_cast<unsigned char>(value[first]))) ++first;
    while (last > first && std::isspace(static_cast<unsigned char>(value[last-1]))) --last;
    return value.substr(first, last-first);
  };
  const std::string cleaned = trim(text);
  const size_t firstcolon = cleaned.find(':');
  const size_t lastseparator = cleaned.find_last_of("/\\");
  const size_t colon = lastseparator == std::string::npos ?
    firstcolon : cleaned.find(':', lastseparator + 1);
  const bool explicitpattern = firstcolon != std::string::npos &&
    cleaned.find('=', firstcolon + 1) != std::string::npos;
  // Preserve Fontconfig property patterns and colons in directory names.
  if (colon == std::string::npos || explicitpattern) {
    cc_string_set_text(&spec->name, cleaned.c_str());
  }
  else {
    const std::string family = trim(cleaned.substr(0, colon));
    const std::string style = trim(cleaned.substr(colon+1));
    cc_string_set_text(&spec->name, family.c_str());
    cc_string_set_text(&spec->style, style.c_str());
  }
}

void
cc_fontspec_copy(const cc_font_specification * from,
                 cc_font_specification * to)
{
  to->size = from->size;
  to->complexity = from->complexity;

  cc_string_construct(&to->name);
  cc_string_set_string(&to->name, &from->name);
  cc_string_construct(&to->style);
  cc_string_set_string(&to->style, &from->style);
}

void
cc_fontspec_clean(cc_font_specification * spec)
{
  cc_string_clean(&spec->name);
  cc_string_clean(&spec->style);
}
