
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include <Inventor/C/glue/fontlib_wrapper.h>
#include <Inventor/C/glue/flwfreetype.h>
#include <ft2build.h>
// FT build macros don't work for MSVC dsp builds. preng 2003-03-11
// #include FT_FREETYPE_H
// #include FT_GLYPH_H
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>

#ifdef __cplusplus
extern "C" {
#endif

FT_Library  library;

int
flwftInitialize()
{
  int error;
  error = FT_Init_FreeType( &library );
  if ( error ) {
    fprintf(stderr,"flwftInitialize error %d\n", error);
  }
  return error;
}

void
flwftExit()
{
  FT_Done_FreeType(library);
}

FLWfont 
flwftGetFont(const char * fontname)
{
  int error;
  FT_Face face;
  error = FT_New_Face( library,
                       fontname,
                       0,
                       &face );
  if ( error ) {
    // fprintf(stderr,"flwftGetFont error %d for font file %s\n", error, fontname);
    return NULL;
  }
  return face;
}

int
flwftGetFontName(FLWfont font, char * buffer, int bufsize)
{
  FT_Face face;
  if (font) {
    face = (FT_Face)font;
    if (bufsize > strlen(face->family_name) + strlen(face->style_name) + 1) {
      sprintf(buffer, "%s %s", face->family_name, face->style_name);
      return 0;
    } else if (bufsize > strlen(face->family_name)) {
      sprintf(buffer, face->family_name);
      return 0;
    }
  }
  return -1;
}

int
flwftGetFontStyle(FLWfont font, char * buffer, int bufsize)
{
  FT_Face face;
  if (font) {
    face = (FT_Face)font;
    if (bufsize > strlen(face->style_name)) {
      strcpy(buffer, face->style_name);
      return 0;
    }
  }
  return -1;
}

void 
flwftDoneFont(FLWfont font)
{
  int error;
  FT_Face face;
  if (font) {
    face = (FT_Face)font;
    error = FT_Done_Face(face);
    if ( error ) {
      fprintf(stderr,"flwftDoneFont error %d\n", error);
    }
  }
}

int
flwftGetNumCharmaps(FLWfont font)
{
  if (font)
    return ((FT_Face)font)->num_charmaps;
  else
    return 0;
}

int
flwftGetCharmapName(FLWfont font, int charmap, char * buffer, int bufsize)
{
  FT_Face face;
  char * name = "unknown";
  if (font) {
    face = (FT_Face)font;
    if (charmap < face->num_charmaps) {
      switch (face->charmaps[charmap]->encoding) {
      case ft_encoding_symbol: 
        name = "symbol"; break;
      case ft_encoding_unicode:	       
        name = "unicode"; break;
      case ft_encoding_latin_1:	       
        name = "latin_1"; break;
      case ft_encoding_latin_2:	       
        name = "latin_2"; break;
      case ft_encoding_sjis:	       
        name = "sjis"; break;
      case ft_encoding_gb2312:  
        name = "gb2312"; break;
      case ft_encoding_big5:	       
        name = "big5"; break;
      case ft_encoding_wansung:	       
        name = "wansung"; break;
      case ft_encoding_johab:	       
        name = "johab"; break;
      case ft_encoding_adobe_standard: 
        name = "adobe_standard"; break;
      case ft_encoding_adobe_expert:   
        name = "adobe_expert"; break;
      case ft_encoding_adobe_custom:   
        name = "adobe_custom"; break;
      case ft_encoding_apple_roman:    
        name = "apple_roman"; break;
      }
      if (strlen(name) < bufsize) {
        strcpy(buffer, name);
        return 0;
      } else
        return -2;
    }
  }
  return -1;
}

int 
flwftSetCharmap(FLWfont font, int charmap)
{
  int error;
  FT_Face face;
  if (font) {
    face = (FT_Face)font;
    if ( charmap < face->num_charmaps) {
      error = FT_Select_Charmap(face, face->charmaps[charmap]->encoding);
      if (error)
        fprintf(stderr,"flwftSetCharmap ERROR: set charmap %d returned error %d\n", charmap, error);
      return error;
    }
  }
  return -1;
}

int
flwftSetCharSize(FLWfont font, int width, int height)
{
  int error;
  FT_Face face;
  if (font) {
    face = (FT_Face)font;
    error = FT_Set_Char_Size(face, width << 6, height << 6, 72, 72);
    if (error)
      fprintf(stderr,"flwftSetCharSize ERROR %d\n", error);
    return error;
  } else
    return -1;
}

int
flwftSetFontRotation(FLWfont font, float angle)
{
  FT_Matrix matrix;
  FT_Face face;
  if (font) {
    face = (FT_Face)font;
    matrix.xx = (FT_Fixed)( cos(angle)*0x10000);
    matrix.xy = (FT_Fixed)(-sin(angle)*0x10000);
    matrix.yx = (FT_Fixed)( sin(angle)*0x10000);
    matrix.yy = (FT_Fixed)( cos(angle)*0x10000);
    FT_Set_Transform(face, &matrix, 0);
    return 0;
  } else
    return -1;
}

FLWglyph
flwftGetGlyph(FLWfont font, unsigned int charidx)
{
  FT_Face face;
  if (font) {
    face = (FT_Face)font;
    return FT_Get_Char_Index(face, charidx);
  } else
    return 0;

}

int
flwftGetAdvance(FLWfont font, FLWglyph glyph, float *x, float *y)
{
  int error;
  FT_Face face;
  int tmp;
  if (font) {
    face = (FT_Face)font;
    error = FT_Load_Glyph(face, glyph, FT_LOAD_DEFAULT);
    if (error) {
      fprintf(stderr,"flwftGetAdvance ERROR %d\n", error);
      return error;
    }
    tmp = face->glyph->advance.x;
    x[0] = tmp / 64.0;
    tmp = face->glyph->advance.y;
    y[0] = tmp / 64.0;
    return 0;
  } else
    return -1;
}

int
flwftGetKerning(FLWfont font, FLWglyph glyph1, FLWglyph glyph2, float *x, float *y)
{
  int error;
  FT_Vector kerning;
  FT_Face face;
  if (font) {
    face = (FT_Face)font;
    if ( FT_HAS_KERNING(face) ) {
      error = FT_Get_Kerning(face, glyph1, glyph2, ft_kerning_default, &kerning);
      if (error) {
        fprintf(stderr,"flwftGetKerning ERROR %d\n", error);
        return error;
      }
      *x = kerning.x / 64.0;
      *y = kerning.y / 64.0;
      return 0;
    } else {
      *x = 0.0;
      *y = 0.0;
      return 0;
    }
  } else
    return -1;
}

void 
flwftDoneGlyph(FLWfont font, FLWglyph glyph)
{
  // No action, since an FLWglyph is just an index.
}

FLWbitmap *
flwftGetBitmap(FLWfont font, FLWglyph glyph)
{
  int error;
  FLWbitmap * bm;
  FT_Face face;
  FT_Glyph g;
  // FT_Matrix matrix;
  // FT_Vector vec;
  FT_BitmapGlyph tfbmg;
  FT_Bitmap * tfbm;
  // FT_BBox bb;
  // float angle;
  if (font) {
    face = (FT_Face)font;
    error = FT_Load_Glyph(face, glyph, FT_LOAD_DEFAULT);
    if (error) {
      fprintf(stderr,"flwftGetBitmap ERROR in FT_Load_Glyph %d\n", error);
      return NULL;
    }
    error = FT_Get_Glyph(face->glyph, &g);
    if (error) {
      fprintf(stderr,"flwftGetBitmap ERROR in FT_Get_Glyph %d\n", error);
      return NULL;
    }
    /*
      angle = 3.14159;
      matrix.xx = (FT_Fixed)( cos(angle)*0x10000);
      matrix.xy = (FT_Fixed)(-sin(angle)*0x10000);
      matrix.yx = (FT_Fixed)( sin(angle)*0x10000);
      matrix.yy = (FT_Fixed)( cos(angle)*0x10000);
      vec.x = 14;
      vec.y = 14;
      error = FT_Glyph_Transform(g, &matrix, &vec);
      if (error) {
      fprintf(stderr,"flwftGetBitmap ERROR in FT_Glyph_Transform %d\n", error);
      return NULL;
      }
      FT_Glyph_Get_CBox(g, ft_glyph_bbox_pixels, &bb );
      printf("flwftGetBitmap: bb = %ld %ld - %ld %ld\n", bb.xMin, bb.yMin, bb.xMax, bb.yMax);
    */
    error = FT_Glyph_To_Bitmap(&g, ft_render_mode_mono, 0, 1);
    if (error) {
      fprintf(stderr,"flwftGetBitmap ERROR in FT_Glyph_To_Bitmap %d\n", error);
      return NULL;
    }
    tfbmg = (FT_BitmapGlyph)g;
    tfbm = &tfbmg->bitmap;
    bm = (FLWbitmap *)malloc(sizeof(FLWbitmap));
    if (!bm) {
      fprintf(stderr,"flwftGetBitmap ERROR in malloc %d bytes\n", sizeof(FLWbitmap));
      return NULL;
    }
    bm->buffer = (unsigned char *)malloc(tfbm->rows * tfbm->pitch);
    if (!bm->buffer) {
      fprintf(stderr,"flwftGetBitmap ERROR in malloc %d bytes\n", tfbm->rows * tfbm->pitch);
      return NULL;
    }
    bm->bearingX = tfbmg->left;
    bm->bearingY = tfbmg->top;
    bm->rows = tfbm->rows;
    bm->width = tfbm->width;
    bm->pitch = tfbm->pitch;
    // bm->advanceX = face->glyph->advance.x >> 6;
    // bm->advanceY = face->glyph->advance.y >> 6;
    memcpy(bm->buffer, tfbm->buffer, tfbm->rows * tfbm->pitch);
    FT_Done_Glyph(g);
    return bm;
  } else
    return NULL;
}

int 
flwftGetOutline(FLWfont font, FLWglyph glyph)
{
  // FIXME: implement.
  fprintf(stderr,"flwftGetOutline has not been implemented yet.\n");
  return 0;
}

#ifdef __cplusplus
}
#endif

