#ifndef FLW_H
#define FLW_H

#ifdef __cplusplus
extern "C" {
#endif
  
  /*
    FLW is a Font Library Wrapper designed to allow any
    number of underlying font libraries to be used through
    the same API. Functions and datatypes are modelled on the
    FreeType font library, which is also the only library
    supported in this first version of FLW. 

    Which underlying font library to use is determined at
    compile time.

    See http://www.freetype.org for more information about
    the FreeType font library.
    
    Search the net for "libFL" for an example of a font 
    library wrapper between SGI OIV and FreeType.
  */

  
  typedef void * FLWfont;
  typedef int FLWglyph;
  
  typedef struct FLWbitmap {
    int bearingX;     // left side of bitmap relative to pen
    int bearingY;     // top of bitmap relative to pen
    unsigned int rows;         // height of bitmap
    unsigned int width;        // width of bitmap
    unsigned int pitch;        // number of bytes occupied by each row (rows are padded to nearest byte)
    int advanceX;     // where to position pen for next glyph
    int advanceY;
    unsigned char * buffer;    // bitmap data
  } FLWbitmap;
  
  int
  flwInitialize();
  
  void
  flwExit();
  
  int 
  flwCreateFont(const char * fontname, char * outname, const int outnamelen, const int sizex, const int sizey);
  
  int 
  flwGetFont(const char * fontname, const int sizex, const int ysizey);
  
  void 
  flwDoneFont(int font);
  
  int
  flwGetNumCharmaps(int font);
  
  int
  flwGetCharmapName(int font, int charmap, char * buffer, int bufsize);
  
  int 
  flwSetCharmap(int font, int charmap);
  
  int
  flwSetCharSize(int font, int width, int height);
  
  int
  flwGetFontName(int font, char * buffer, int bufsize);
  
  int
  flwSetFontRotation(int font, float angle);
  
  int
  flwGetGlyph(int font, int charidx);
  
  int
  flwGetAdvance(int font, int glyph, float *x, float *y);
  
  int
  flwGetKerning(int font, int glyph1, int glyph2, float *x, float *y);
  
  void 
  flwDoneGlyph(int font, int glyph);
  
  FLWbitmap *
  flwGetBitmap(int font, int glyph);
  
  void
  flwDoneBitmap(FLWbitmap * bitmap);
  
  int 
  flwGetOutline(int font, int glyph);
  
  
#ifdef __cplusplus
}
#endif
  
#endif  // FLW_H
