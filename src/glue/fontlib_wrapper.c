
// FIXME: Get rid of (typecast)0 hack. preng 20030225

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <Inventor/C/glue/fontlib_wrapper.h>

#ifdef HAVE_FREETYPE
#include <Inventor/C/glue/flwfreetype.h>
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

struct glyphstruct {
  FLWglyph glyph;
  FLWbitmap * bitmap;
  int defaultglyph;
};

struct fontstruct {
  FLWfont font;
  char * fontname;
  char * requestname;
  struct glyphstruct * glyphs;
  int glyphcnt;
  int glyphmax;
  int sizex, sizey;
  int defaultfont;
};

static struct fontstruct **fonts;
static int fontcnt;
static int fontmax = 0;

// The default bitmap font, as a fallback. Data at end of this file.
unsigned char coin_default2dfont[][12];
int coin_default2dfont_isolatin1_mapping[];

#define NOGLYPH -1

/*
  BEGIN Internal functions
*/

FLWbitmap *
get_default_bitmap(unsigned int character)
{
  FLWbitmap * bm;
  if (character < 256) {
    bm = (FLWbitmap *)malloc(sizeof(FLWbitmap));
    if (bm) {
      bm->buffer = (unsigned char *)coin_default2dfont + 12 * coin_default2dfont_isolatin1_mapping[character];
      bm->bearingX = 0;
      bm->bearingY = 12;
      bm->rows = 12;
      bm->width = 8;
      bm->pitch = 1;
      return bm;
    }
  }
  return (FLWbitmap *)0;
}

void
glyphstruct_init(struct glyphstruct * gs)
{
  gs->glyph = (FLWglyph)NOGLYPH;
  gs->bitmap = (FLWbitmap *)0;
  gs->defaultglyph = 0;
}

struct fontstruct *
fontstruct_new(FLWfont font)
{
  int i;
  struct fontstruct * fs;
  fs = (struct fontstruct *)malloc(sizeof(struct fontstruct));
  if (!fs)
    return (struct fontstruct *)0;
  fs->font = font;
  fs->fontname = (char *)0;
  fs->requestname = (char *)0;
  fs->glyphcnt = 0;
  fs->glyphmax = 0;
  fs->sizex = 0;
  fs->sizey = 0;
  fs->defaultfont = 0;
  fs->glyphs = (struct glyphstruct *)malloc(10*sizeof(struct glyphstruct));
  if (fs->glyphs)
    fs->glyphmax = 10;
  for (i=0; i<fs->glyphmax; i++)
    glyphstruct_init(&fs->glyphs[i]);
  return fs;
}

int
fontstruct_set_fontname(struct fontstruct * fs, const char * newname)
{
  if (fs) {
    if (fs->fontname)
      free(fs->fontname);
    fs->fontname = (char *)malloc(strlen(newname) + 1);
    if (fs->fontname) {
      strcpy(fs->fontname, newname);
      return 0;
    }
  }
  return -1;
}

int
fontstruct_set_requestname(struct fontstruct * fs, const char * newname)
{
  if (fs) {
    if (fs->requestname)
      free(fs->requestname);
    fs->requestname = (char *)malloc(strlen(newname) + 1);
    if (fs->requestname) {
      strcpy(fs->requestname, newname);
      return 0;
    }
  }
  return -1;
}

int
fontstruct_set_size(struct fontstruct * fs, const int x, const int y)
{
  if (fs) {
    fs->sizex = x;
    fs->sizey = y;
    return 0;
  }
  return -1;
}

int
fontstruct_expand()
{
  int i;
  struct fontstruct **newfonts;
  if (fonts) {
    newfonts = (struct fontstruct **)malloc(sizeof(struct fontstruct*) * 2 * fontmax);
    if (newfonts) {
      for (i=0; i<fontmax; i++)
        newfonts[i] = fonts[i];
      for (i=fontmax; i<fontmax*2; i++)
        newfonts[i] = (struct fontstruct *)0;
      free(fonts);
      fonts = newfonts;
      fontmax *= 2;
      return 0;
    }
  }
  return -1;
}

int
fontstruct_insert(struct fontstruct * fs)
{
  int i;
  if (fs && fonts) {
    // Check for (and reuse) empty slots
    for (i=0; i<fontmax; i++)
      if (!fonts[i]) {
        fonts[i] = fs;
        if (i>=fontcnt)
          fontcnt = i+1;
        return i;
      }
    // Expand fonts array and use next slot
    if (fontstruct_expand() == 0) {
      fonts[fontcnt++] = fs;
      return fontcnt-1;
    }
  }
  return -1;
}

void
fontstruct_rmglyph(struct fontstruct * fs, int glyph)
{
  if (fs && glyph<fs->glyphcnt && fs->glyphs[glyph].glyph != NOGLYPH) {
    fs->glyphs[glyph].glyph = NOGLYPH;
    flwDoneBitmap(fs->glyphs[glyph].bitmap);
    fs->glyphs[glyph].bitmap = (FLWbitmap *)0;
  }
}

void
fontstruct_rmfont(int font)
{
  int i;
  if (font<fontcnt && fonts[font]) {
    if (fonts[font]->fontname)
      free(fonts[font]->fontname);
    if (fonts[font]->requestname)
      free(fonts[font]->requestname);
    for (i=0; i<fonts[font]->glyphcnt; i++) {
      fontstruct_rmglyph(fonts[font], i);
    }
    free( fonts[font]->glyphs );
    free( fonts[font] );
    fonts[font] = (struct fontstruct *)0;
  }
}

void
fontstruct_cleanup()
{
  int i;
  if (fonts) {
    for (i=0; i<fontcnt; i++)
      fontstruct_rmfont(i);
    free(fonts);
  }
}

void
fontstruct_dumpfont(struct fontstruct * fs)
{
  if (fs) {
    printf("  Font %s / %s :\n", fs->fontname, fs->requestname);
    printf("    glyphcnt %d\n", fs->glyphcnt);
    printf("    %s\n", fs->defaultfont ? "default font" : "regular font");
    printf("    size (%d, %d)\n", fs->sizex, fs->sizey);
  }
}

void
fontstruct_dump()
{
  int i;
  if (fonts) {
    printf("Fontlib cache: %d fonts.\n", fontcnt);
    for (i=0; i<fontcnt; i++)
      fontstruct_dumpfont(fonts[i]);
  }
}

int
fontstruct_expand_glyphs(struct fontstruct * fs)
{
  int i;
  struct glyphstruct * newglyphs;
  if (fs && fs->glyphs) {
    newglyphs = (struct glyphstruct *)malloc(sizeof(struct glyphstruct) * 2 * fs->glyphmax);
    if (newglyphs) {
      for (i=0; i<fs->glyphmax; i++)
        newglyphs[i] = fs->glyphs[i];
      for (i=fs->glyphmax; i<fs->glyphmax*2; i++)
        glyphstruct_init(&newglyphs[i]);
      free(fs->glyphs);
      fs->glyphs = newglyphs;
      fs->glyphmax *= 2;
      return 0;
    }
  }
  return -1;
}

int
fontstruct_insert_glyph(int font, FLWglyph glyph, int defaultglyph)
{
  int i;
  struct fontstruct * fs;
  if (font<fontcnt && fonts && fonts[font]) {
    fs = fonts[font];
    // Check for (and reuse) empty slots
    for (i=0; i<fs->glyphmax; i++)
      if (fs->glyphs[i].glyph == NOGLYPH) {
        fs->glyphs[i].glyph = glyph;
        if (i>=fs->glyphcnt)
          fs->glyphcnt = i+1;
        fs->glyphs[i].defaultglyph = defaultglyph;
        return i;
      }
    // Expand glyphs array and use next slot
    if (fontstruct_expand_glyphs(fs) == 0) {
      fs->glyphs[fs->glyphcnt].glyph = glyph;
      fs->glyphs[fs->glyphcnt].defaultglyph = defaultglyph;
      return fs->glyphcnt++;
    }
  }
  return -1;
}


/*
  END Internal functions
*/


int
flwInitialize()
{
  int i, result;
  char dummy[80];
  if (fontmax > 0)
    return -1;
  fontcnt = 0;
  fontmax = 0;
  fonts = (struct fontstruct**)malloc(10*sizeof(struct fontstruct *));
  if (!fonts)
    return -1;
  fontmax = 10;
  for (i=0; i<fontmax; i++)
    fonts[i] = (struct fontstruct *)0;
#ifdef HAVE_FREETYPE
  result = flwftInitialize();
#else
  result = 0;
#endif
  flwCreateFont("defaultFont", dummy, 80, 12, 12);
  return result;
}

void
flwExit()
{
#ifdef HAVE_FREETYPE
  flwftExit();
#endif
  fontstruct_cleanup();
}

/*
  Create one font. fontname is the name & style _requested_, outname will hold
  the name of the font actually created - these will typically be different.
  If a font has already been created for this fontname, the function will not
  create a duplicate, but simply return that font. If lookup, the method will
  look up whether the font already exists, and fail (return -1) if it doesn't.
*/
int
flwCreateFont(const char * fontname, char * outname, const int outnamelen, const int sizex, const int sizey)
{
  struct fontstruct * fs;
  char newname[300];
  FLWfont font;
  int i;

  // Don't create font if one has already been created for this requestname and size
  for (i=0; i<fontcnt; i++)
    if (fonts[i]->sizex == sizex && fonts[i]->sizey == sizey && !strcmp(fontname, fonts[i]->requestname)) {
      if ((int) strlen(fonts[i]->fontname) < outnamelen)
        sprintf(outname, fonts[i]->fontname);
      return i;
    }
#ifdef HAVE_FREETYPE
  font = flwftGetFont(fontname);
#else
  font = (FLWfont)0;
#endif
  if (font) {
    fs = fontstruct_new(font);
    if (!fs)
      return -1;
    fontstruct_set_requestname(fs, fontname);
    fontstruct_set_size(fs, sizex, sizey);
#ifdef HAVE_FREETYPE
    flwftGetFontName(font, newname, 300);
    flwftSetCharSize(font, sizex, sizey);
#else
    sprintf(newname, "unknown");
#endif
    fontstruct_set_fontname(fs, newname);
    if ((int) strlen(newname) < outnamelen)
      sprintf(outname, newname);
    return fontstruct_insert(fs);
  } else {
    // Return default font.
    fs = fontstruct_new((FLWfont)0);
    if (!fs)
      return -1;
    fontstruct_set_requestname(fs, fontname);
    fontstruct_set_size(fs, 0, 0);
    fontstruct_set_fontname(fs, "defaultFont");
    if ((int) strlen(fs->fontname) < outnamelen)
      sprintf(outname, fs->fontname);
    fs->defaultfont = 1;
    return fontstruct_insert(fs);
  }
}

int
flwGetFont(const char * fontname, const int sizex, const int sizey)
{
  struct fontstruct * fs;
  FLWfont font;
  int i;
  for (i=0; i<fontcnt; i++)
    if ((fonts[i]->defaultfont ||
         (fonts[i]->sizex == sizex && fonts[i]->sizey == sizey)) &&
        !strcmp(fontname, fonts[i]->fontname))
      return i;
  return -1;
}

void
flwDoneFont(int font)
{
  if (font >= 0 && font < fontcnt && fonts[font]) {
#ifdef HAVE_FREETYPE
    flwftDoneFont(fonts[font]->font);
#endif
  }
  fontstruct_rmfont(font);
}

int
flwGetNumCharmaps(int font)
{
  if (font >= 0 && font < fontcnt && fonts[font]) {
#ifdef HAVE_FREETYPE
    return flwftGetNumCharmaps(fonts[font]->font);
#endif
  }
  return 0;
}

int
flwGetCharmapName(int font, int charmap, char * buffer, int bufsize)
{
  if (font >= 0 && font < fontcnt && fonts[font]) {
#ifdef HAVE_FREETYPE
    return flwftGetCharmapName(fonts[font]->font, charmap, buffer, bufsize);
#endif
  }
  return -1;
}


int
flwGetFontName(int font, char * buffer, int bufsize)
{
  if (font >= 0 && fonts[font]) {
    if ((int)strlen(fonts[font]->fontname) < bufsize) {
      memcpy(buffer, fonts[font]->fontname, strlen(fonts[font]->fontname)+1);
      return 0;
    }
  }
  return -1;
}

int
flwSetCharmap(int font, int charmap)
{
  if (font >= 0 && font < fontcnt && fonts[font]) {
#ifdef HAVE_FREETYPE
    return flwftSetCharmap(fonts[font]->font, charmap);
#endif
  }
  return -1;
}

int
flwSetCharSize(int font, int width, int height)
{
  if (font >= 0 && font < fontcnt && fonts[font]) {
    fonts[font]->sizex = width;
    fonts[font]->sizey = height;
#ifdef HAVE_FREETYPE
    return flwftSetCharSize(fonts[font]->font, width, height);
#endif
  }
  return -1;
}

int
flwSetFontRotation(int font, float angle)
{
  if (font >= 0 && font < fontcnt && fonts[font]) {
#ifdef HAVE_FREETYPE
    return flwftSetFontRotation(fonts[font]->font, angle);
#endif
  }
  return -1;
}

int
flwGetGlyph(int font, int charidx)
{
  FLWglyph glyph;
  int idx;
  if (font >= 0 && font < fontcnt && fonts[font]) {
#ifdef HAVE_FREETYPE
    glyph = flwftGetGlyph(fonts[font]->font, charidx);
#else
    glyph = (FLWglyph)0;
#endif
    if (glyph)
      return fontstruct_insert_glyph(font, glyph, 0);
    else {
      // Create glyph from default font, mark as default.
      return fontstruct_insert_glyph(font, charidx, 1);
    }
  }
  return -1;
}

int
flwGetAdvance(int font, int glyph, float *x, float *y)
{
  struct fontstruct * fs;
  if (font >= 0 && font < fontcnt && fonts[font]) {
    fs = fonts[font];
    if (fs->defaultfont || fs->glyphs[glyph].defaultglyph) {
      *x = 8;
      *y = 0;
      return 0;
    } else if (glyph<fs->glyphcnt && fs->glyphs[glyph].glyph != NOGLYPH) {
#ifdef HAVE_FREETYPE
      return flwftGetAdvance(fs->font, fs->glyphs[glyph].glyph, x, y);
#endif
    }
  }
  return -1;
}

int
flwGetKerning(int font, int glyph1, int glyph2, float *x, float *y)
{
  struct fontstruct * fs;
  if (font >= 0 && font < fontcnt && fonts[font]) {
    fs = fonts[font];
    if (fs->defaultfont || fs->glyphs[glyph1].defaultglyph || fs->glyphs[glyph2].defaultglyph) {
      *x = 0;
      *y = 0;
      return 0;
    } else if (glyph1<fs->glyphcnt && fs->glyphs[glyph1].glyph != NOGLYPH)
      if (glyph2<fs->glyphcnt && fs->glyphs[glyph2].glyph != NOGLYPH) {
#ifdef HAVE_FREETYPE
        return flwftGetKerning(fs->font, fs->glyphs[glyph1].glyph, fs->glyphs[glyph2].glyph, x, y);
#endif
      }
  }
  return -1;
}

void
flwDoneGlyph(int font, int glyph)
{
  struct fontstruct * fs;
  if (font >= 0 && font < fontcnt && fonts[font]) {
    fs = fonts[font];
    if (glyph<fs->glyphcnt && fs->glyphs[glyph].glyph != NOGLYPH) {
#ifdef HAVE_FREETYPE
      flwftDoneGlyph(fs->font, fs->glyphs[glyph].glyph);
#endif
      fontstruct_rmglyph(fs, glyph);
    }
  }
}

FLWbitmap *
flwGetBitmap(int font, int glyph)
{
  unsigned char * buf;
  struct fontstruct * fs;
  FLWbitmap * bm;
  int i, defaultglyph;
  defaultglyph = 0;
  bm = (FLWbitmap *)0;
  if (font >= 0 && font < fontcnt && fonts[font]) {
    fs = fonts[font];
    if (glyph<fs->glyphcnt && fs->glyphs[glyph].glyph != NOGLYPH) {
      if (fs->glyphs[glyph].bitmap)
        return fs->glyphs[glyph].bitmap;
#ifdef HAVE_FREETYPE
      if (!fs->defaultfont && !fs->glyphs[glyph].defaultglyph) {
        bm = flwftGetBitmap(fs->font, fs->glyphs[glyph].glyph);
      };
#endif
      if (!bm) {
        // glyph handle == char value in default font. &255 to avoid index out of range.
        bm = get_default_bitmap(fs->glyphs[glyph].glyph & 0xff);
        defaultglyph = 1;
      }
      if (bm) {
        if (!defaultglyph) {
          buf = (unsigned char *)malloc(bm->pitch * bm->rows);
          if (!buf) {
            flwDoneBitmap(bm);
            return (FLWbitmap *)0;
          }
          // Copy & reverse buffer to OpenGL "up" direction.
          for (i=0; i<(int)bm->rows; i++)
            memcpy(buf + i*bm->pitch, bm->buffer + (bm->rows-i-1)*bm->pitch, bm->pitch);
          free(bm->buffer);
          bm->buffer = buf;
        }
      }
      fonts[font]->glyphs[glyph].bitmap = bm;
      return bm;
    }
  }
  return (FLWbitmap *)0;
}

int
flwGetOutline(int font, int glyph)
{
  fprintf(stderr,"flwGetOutline has not been implemented yet.\n");
  return -1;
}

void
flwDoneBitmap(FLWbitmap * bitmap)
{
  if( bitmap) {
    if (bitmap->buffer)
      free(bitmap->buffer);
    free(bitmap);
  }
}



/*
  Default font data below. Ripped from src/misc/default2dfont.cpp
*/


unsigned char coin_default2dfont[][12] = {
  {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 }, //
  {  0,  0, 12, 12,  0,  8, 12, 12, 12, 12, 12,  0 }, // !
  {  0,  0,  0,  0,  0,  0,  0,  0,  0, 20, 20, 20 }, // "
  {  0,  0, 18, 18, 18, 63, 18, 18, 63, 18, 18,  0 }, // #
  {  0,  8, 28, 42, 10, 10, 12, 24, 40, 42, 28,  8 }, // $
  {  0,  0,  6, 73, 41, 22,  8, 52, 74, 73, 48,  0 }, // %
  {  0,  0, 29, 34, 34, 37, 25, 12, 18, 18, 12,  0 }, // &
  {  0,  0,  0,  0,  0,  0,  0,  0,  0, 24, 12, 12 }, // '
  {  0,  6,  8,  8, 16, 16, 16, 16, 16,  8,  8,  6 }, // (
  {  0, 48,  8,  8,  4,  4,  4,  4,  4,  8,  8, 48 }, // )
  {  0,  0,  0,  0,  0,  0,  8, 42, 20, 42,  8,  0 }, // *
  {  0,  0,  0,  8,  8,  8,127,  8,  8,  8,  0,  0 }, // +
  {  0, 24, 12, 12,  0,  0,  0,  0,  0,  0,  0,  0 }, // ,
  {  0,  0,  0,  0,  0,  0,127,  0,  0,  0,  0,  0 }, // -
  {  0,  0, 24, 24,  0,  0,  0,  0,  0,  0,  0,  0 }, // .
  {  0, 32, 32, 16, 16,  8,  8,  8,  4,  4,  2,  2 }, // /
  {  0,  0, 28, 34, 34, 34, 34, 34, 34, 34, 28,  0 }, // 0
  {  0,  0,  8,  8,  8,  8,  8,  8, 40, 24,  8,  0 }, // 1
  {  0,  0, 62, 32, 16,  8,  4,  2,  2, 34, 28,  0 }, // 2
  {  0,  0, 28, 34,  2,  2, 12,  2,  2, 34, 28,  0 }, // 3
  {  0,  0,  4,  4,  4,126, 68, 36, 20, 12,  4,  0 }, // 4
  {  0,  0, 28, 34,  2,  2,  2, 60, 32, 32, 62,  0 }, // 5
  {  0,  0, 28, 34, 34, 34, 60, 32, 32, 34, 28,  0 }, // 6
  {  0,  0, 16, 16, 16,  8,  8,  4,  2,  2, 62,  0 }, // 7
  {  0,  0, 28, 34, 34, 34, 28, 34, 34, 34, 28,  0 }, // 8
  {  0,  0, 28, 34,  2,  2, 30, 34, 34, 34, 28,  0 }, // 9
  {  0,  0, 24, 24,  0,  0,  0, 24, 24,  0,  0,  0 }, // :
  {  0, 48, 24, 24,  0,  0,  0, 24, 24,  0,  0,  0 }, // ;
  {  0,  0,  0,  2,  4,  8, 16,  8,  4,  2,  0,  0 }, // <
  {  0,  0,  0,  0,  0,127,  0,127,  0,  0,  0,  0 }, // =
  {  0,  0,  0, 16,  8,  4,  2,  4,  8, 16,  0,  0 }, // >
  {  0,  0, 16, 16,  0, 16, 28,  2,  2,  2, 60,  0 }, // ?
  {  0,  0, 28, 32, 73, 86, 82, 82, 78, 34, 28,  0 }, // @
  {  0,  0, 33, 33, 33, 63, 18, 18, 18, 12, 12,  0 }, // A
  {  0,  0, 60, 34, 34, 34, 60, 34, 34, 34, 60,  0 }, // B
  {  0,  0, 14, 16, 32, 32, 32, 32, 32, 18, 14,  0 }, // C
  {  0,  0, 56, 36, 34, 34, 34, 34, 34, 36, 56,  0 }, // D
  {  0,  0, 62, 32, 32, 32, 60, 32, 32, 32, 62,  0 }, // E
  {  0,  0, 16, 16, 16, 16, 30, 16, 16, 16, 30,  0 }, // F
  {  0,  0, 14, 18, 34, 34, 32, 32, 32, 18, 14,  0 }, // G
  {  0,  0, 34, 34, 34, 34, 62, 34, 34, 34, 34,  0 }, // H
  {  0,  0, 62,  8,  8,  8,  8,  8,  8,  8, 62,  0 }, // I
  {  0,  0,112,  8,  8,  8,  8,  8,  8,  8, 62,  0 }, // J
  {  0,  0, 33, 33, 34, 36, 56, 40, 36, 34, 33,  0 }, // K
  {  0,  0, 30, 16, 16, 16, 16, 16, 16, 16, 16,  0 }, // L
  {  0,  0, 33, 33, 33, 45, 45, 45, 51, 51, 33,  0 }, // M
  {  0,  0, 34, 34, 38, 38, 42, 42, 50, 50, 34,  0 }, // N
  {  0,  0, 12, 18, 33, 33, 33, 33, 33, 18, 12,  0 }, // O
  {  0,  0, 32, 32, 32, 60, 34, 34, 34, 34, 60,  0 }, // P
  {  3,  6, 12, 18, 33, 33, 33, 33, 33, 18, 12,  0 }, // Q
  {  0,  0, 34, 34, 34, 36, 60, 34, 34, 34, 60,  0 }, // R
  {  0,  0, 60,  2,  2,  6, 28, 48, 32, 32, 30,  0 }, // S
  {  0,  0,  8,  8,  8,  8,  8,  8,  8,  8,127,  0 }, // T
  {  0,  0, 28, 34, 34, 34, 34, 34, 34, 34, 34,  0 }, // U
  {  0,  0, 12, 12, 18, 18, 18, 33, 33, 33, 33,  0 }, // V
  {  0,  0, 34, 34, 34, 54, 85, 73, 73, 73, 65,  0 }, // W
  {  0,  0, 34, 34, 20, 20,  8, 20, 20, 34, 34,  0 }, // X
  {  0,  0,  8,  8,  8,  8, 20, 20, 34, 34, 34,  0 }, // Y
  {  0,  0, 62, 32, 16, 16,  8,  4,  4,  2, 62,  0 }, // Z
  {  0, 14,  8,  8,  8,  8,  8,  8,  8,  8,  8, 14 }, // [
  {  0,  2,  2,  4,  4,  8,  8,  8, 16, 16, 32, 32 }, // [backslash]
  {  0, 56,  8,  8,  8,  8,  8,  8,  8,  8,  8, 56 }, // ]
  {  0,  0,  0,  0,  0, 34, 34, 20, 20,  8,  8,  0 }, // ^
  {  0,127,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 }, // _
  {  0,  0,  0,  0,  0,  0,  0,  0,  0, 24, 24, 12 }, // `
  {  0,  0, 29, 34, 34, 30,  2, 34, 28,  0,  0,  0 }, // a
  {  0,  0, 60, 34, 34, 34, 34, 50, 44, 32, 32, 32 }, // b
  {  0,  0, 14, 16, 32, 32, 32, 16, 14,  0,  0,  0 }, // c
  {  0,  0, 26, 38, 34, 34, 34, 34, 30,  2,  2,  2 }, // d
  {  0,  0, 28, 34, 32, 62, 34, 34, 28,  0,  0,  0 }, // e
  {  0,  0, 16, 16, 16, 16, 16, 16, 62, 16, 16, 14 }, // f
  { 28,  2,  2, 26, 38, 34, 34, 34, 30,  0,  0,  0 }, // g
  {  0,  0, 34, 34, 34, 34, 34, 50, 44, 32, 32, 32 }, // h
  {  0,  0,  8,  8,  8,  8,  8,  8, 56,  0,  8,  8 }, // i
  { 56,  4,  4,  4,  4,  4,  4,  4, 60,  0,  4,  4 }, // j
  {  0,  0, 33, 34, 36, 56, 40, 36, 34, 32, 32, 32 }, // k
  {  0,  0,  8,  8,  8,  8,  8,  8,  8,  8,  8, 56 }, // l
  {  0,  0, 73, 73, 73, 73, 73,109, 82,  0,  0,  0 }, // m
  {  0,  0, 34, 34, 34, 34, 34, 50, 44,  0,  0,  0 }, // n
  {  0,  0, 28, 34, 34, 34, 34, 34, 28,  0,  0,  0 }, // o
  { 32, 32, 60, 34, 34, 34, 34, 50, 44,  0,  0,  0 }, // p
  {  2,  2, 26, 38, 34, 34, 34, 34, 30,  0,  0,  0 }, // q
  {  0,  0, 16, 16, 16, 16, 16, 24, 22,  0,  0,  0 }, // r
  {  0,  0, 60,  2,  2, 28, 32, 32, 30,  0,  0,  0 }, // s
  {  0,  0, 14, 16, 16, 16, 16, 16, 62, 16, 16,  0 }, // t
  {  0,  0, 26, 38, 34, 34, 34, 34, 34,  0,  0,  0 }, // u
  {  0,  0,  8,  8, 20, 20, 34, 34, 34,  0,  0,  0 }, // v
  {  0,  0, 34, 34, 34, 85, 73, 73, 65,  0,  0,  0 }, // w
  {  0,  0, 34, 34, 20,  8, 20, 34, 34,  0,  0,  0 }, // x
  { 48, 16,  8,  8, 20, 20, 34, 34, 34,  0,  0,  0 }, // y
  {  0,  0, 62, 32, 16,  8,  4,  2, 62,  0,  0,  0 }, // z
  {  0,  6,  8,  8,  8,  4, 24,  4,  8,  8,  8,  6 }, // {
  {  0,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8 }, // |
  {  0, 48,  8,  8,  8, 16, 12, 16,  8,  8,  8, 48 }, // }
  {  0,  0,  0,  0,  0,  0, 78, 57,  0,  0,  0,  0 },  // ~
  // iso-latin-1 norwegian letters
  {  0,  0, 59, 76, 72, 62,  9, 73, 54,  0,  0,  0 }, // ae
  {  0,  0, 92, 34, 50, 42, 42, 38, 29,  0,  0,  0 }, // oe
  {  0,  0, 29, 34, 34, 30,  2, 34, 28,  8, 20,  8 }, // aa
  {  0,  0, 79, 72, 72, 72,127, 72, 72, 72, 63,  0 }, // AE
  {  0,  0, 44, 18, 41, 41, 41, 37, 37, 18, 13,  0 }, // OE
  {  0,  0, 33, 33, 33, 63, 18, 18, 12, 12, 18, 12 }  // AA
};

// map from iso-latin1 to font data array index
int coin_default2dfont_isolatin1_mapping[] = {
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   1,   2,   3,   4,   5,   6,   7,
    8,   9,  10,  11,  12,  13,  14,  15,
   16,  17,  18,  19,  20,  21,  22,  23,
   24,  25,  26,  27,  28,  29,  30,  31,
   32,  33,  34,  35,  36,  37,  38,  39,
   40,  41,  42,  43,  44,  45,  46,  47,
   48,  49,  50,  51,  52,  53,  54,  55,
   56,  57,  58,  59,  60,  61,  62,  63,
   64,  65,  66,  67,  68,  69,  70,  71,
   72,  73,  74,  75,  76,  77,  78,  79,
   80,  81,  82,  83,  84,  85,  86,  87,
   88,  89,  90,  91,  92,  93,  94,   0,
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0, 100,  98,   0,
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,
   99,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,  97,  95,   0,
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,
   96,   0,   0,   0,   0,   0,   0,   0
};



#ifdef __cplusplus
}
#endif
