#ifndef FLW_FREETYPE_H
#define FLW_FREETYPE_H

#ifdef __cplusplus
extern "C" {
#endif
  
  /*
    Here is the glue between the FLW and the FreeType font lib.

    See http://www.freetype.org for more information about
    the FreeType font library.
  */
  
#include <Inventor/C/glue/fontlib_wrapper.h>
  
  int
  flwftInitialize();
  
  void
  flwftExit();
  
  FLWfont 
  flwftGetFont(const char * fontname);
  
  int
  flwftGetFontName(FLWfont font, char * buffer, int bufsize);

  int
  flwftGetFontStyle(FLWfont font, char * buffer, int bufsize);

  void 
  flwftDoneFont(FLWfont font);
  
  int
  flwftGetNumCharmaps(FLWfont font);
  
  int
  flwftGetCharmapName(FLWfont font, int charmap, char * buffer, int bufsize);
  
  int 
  flwftSetCharmap(FLWfont font, int charmap);
  
  int
  flwftSetCharSize(FLWfont font, int width, int height);
  
  int
  flwftSetFontRotation(FLWfont font, float angle);
  
  FLWglyph
  flwftGetGlyph(FLWfont font, unsigned int charidx);
  
  int
  flwftGetAdvance(FLWfont font, FLWglyph glyph, float *x, float *y);
  
  int
  flwftGetKerning(FLWfont font, FLWglyph glyph1, FLWglyph glyph2, float *x, float *y);
  
  void 
  flwftDoneGlyph(FLWfont font, FLWglyph glyph);
  
  FLWbitmap *
  flwftGetBitmap(FLWfont font, FLWglyph glyph);
  
  int 
  flwftGetOutline(FLWfont font, FLWglyph glyph);
  
#ifdef __cplusplus
}
#endif

#endif  // FLW_FREETYPE_H
