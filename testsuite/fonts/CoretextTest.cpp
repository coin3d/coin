#include <Inventor/SoDB.h>
#include <Inventor/C/tidbits.h>
#include <CoreText/CoreText.h>

#include "fonts/freetype.h"
#include "glue/freetype.h"

#include <cstdio>
#include <cstring>

static bool
check_font(const char * requested, CFStringRef expected)
{
  CTFontRef native = CTFontCreateWithName(expected, 16, NULL);
  if (!native) return false;

  CFStringRef postscript = CTFontCopyPostScriptName(native);
  CFRelease(native);
  char name[256];
  const bool converted = postscript &&
    CFStringGetCString(postscript, name, sizeof(name), kCFStringEncodingUTF8);
  if (postscript) CFRelease(postscript);
  if (!converted) return false;

  FT_Face face = static_cast<FT_Face>(cc_flwft_get_font(requested, 16));
  if (!face) return false;

  const char * actual = cc_ftglue_FT_Get_Postscript_Name(face);
  const bool matched = actual && std::strcmp(actual, name) == 0 &&
    cc_flwft_get_glyph(face, 'A') != 0;
  if (!matched) {
    std::fprintf(stderr, "%s: expected %s, got %s (face %ld)\n",
                 requested, name, actual ? actual : "no name", face->face_index);
  }

  cc_flwft_done_font(face);
  return matched;
}

int
main()
{
  SoDB::init();
  coin_setenv("COIN_FORCE_FONTCONFIG_OFF", "1", 1);
  if (!cc_flwft_initialize()) return 77;

  const bool matched = check_font("Helvetica", CFSTR("Helvetica")) &&
    check_font("Helvetica:Bold", CFSTR("Helvetica-Bold"));

  cc_flwft_exit();
  SoDB::finish();
  return matched ? 0 : 1;
}
