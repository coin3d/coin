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

#include "fonts/coretext.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#ifdef __APPLE__
#include <CoreText/CoreText.h>
#include <cstring>
#include <vector>

#include "glue/freetype.h"

static SbBool
copy_utf8(CFStringRef source, cc_string * target)
{
  if (!source) return FALSE;

  const CFIndex size = CFStringGetMaximumSizeForEncoding(
    CFStringGetLength(source), kCFStringEncodingUTF8);
  if (size < 0) return FALSE;

  std::vector<char> buffer(size + 1);
  if (!CFStringGetCString(source, &buffer[0], buffer.size(), kCFStringEncodingUTF8)) {
    return FALSE;
  }

  cc_string_set_text(target, &buffer[0]);
  return TRUE;
}

static CTFontRef
create_font(const char * fontname, unsigned int pixelsize)
{
  CFStringRef name = CFStringCreateWithCString(NULL, fontname, kCFStringEncodingUTF8);
  if (!name) return NULL;

  const CFRange colon = CFStringFind(name, CFSTR(":"), 0);
  if (colon.location == kCFNotFound) {
    CTFontRef font = CTFontCreateWithName(name, pixelsize, NULL);
    CFRelease(name);
    return font;
  }

  // Coin uses family:style; CoreText expects separate descriptor attributes.
  CFStringRef family = CFStringCreateWithSubstring(NULL, name,
    CFRangeMake(0, colon.location));
  CFStringRef style = CFStringCreateWithSubstring(NULL, name,
    CFRangeMake(colon.location + 1, CFStringGetLength(name) - colon.location - 1));
  CFRelease(name);
  if (!family || !style) {
    if (family) CFRelease(family);
    if (style) CFRelease(style);
    return NULL;
  }

  const void * keys[] = { kCTFontFamilyNameAttribute, kCTFontStyleNameAttribute };
  const void * values[] = { family, style };
  CFDictionaryRef attributes = CFDictionaryCreate(NULL, keys, values, 2,
    &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
  CFRelease(family);
  CFRelease(style);
  if (!attributes) return NULL;

  CTFontDescriptorRef descriptor = CTFontDescriptorCreateWithAttributes(attributes);
  CFRelease(attributes);
  if (!descriptor) return NULL;

  CTFontRef font = CTFontCreateWithFontDescriptor(descriptor, pixelsize, NULL);
  CFRelease(descriptor);
  return font;
}

SbBool
cc_flwct_find_file(const char * fontname, unsigned int pixelsize,
                   void * library, cc_string * filename, long * faceindex)
{
  CTFontRef font = create_font(fontname, pixelsize);
  if (!font) return FALSE;

  CFTypeRef location = CTFontCopyAttribute(font, kCTFontURLAttribute);
  CFStringRef postscript = CTFontCopyPostScriptName(font);
  CFRelease(font);
  if (!location || CFGetTypeID(location) != CFURLGetTypeID() || !postscript) {
    if (location) CFRelease(location);
    if (postscript) CFRelease(postscript);
    return FALSE;
  }

  CFStringRef path = CFURLCopyFileSystemPath((CFURLRef)location, kCFURLPOSIXPathStyle);
  CFRelease(location);
  cc_string name;
  cc_string_construct(&name);
  const SbBool converted = copy_utf8(path, filename) && copy_utf8(postscript, &name);
  if (path) CFRelease(path);
  CFRelease(postscript);
  if (!converted) {
    cc_string_clean(&name);
    return FALSE;
  }

  // Descriptor order need not match a collection's FreeType face indices.
  SbBool found = FALSE;
  long count = 1;
  for (long index = 0; index < count; ++index) {
    FT_Face face;
    if (cc_ftglue_FT_New_Face(library, cc_string_get_text(filename), index, &face)) break;

    count = face->num_faces;
    const char * candidate = cc_ftglue_FT_Get_Postscript_Name(face);
    found = candidate && std::strcmp(candidate, cc_string_get_text(&name)) == 0;
    cc_ftglue_FT_Done_Face(face);
    if (!found) continue;

    *faceindex = index;
    break;
  }

  cc_string_clean(&name);
  return found;
}
#endif /* __APPLE__ */
