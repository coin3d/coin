/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2004 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org/> for more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#else /* No config.h? Hmm. Assume the freetype library is available for linking. */
#define FREETYPEGLUE_ASSUME_FREETYPE 1
#endif /* !HAVE_CONFIG_H */

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stddef.h>

#include <sys/stat.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif /* HAVE_UNISTD_H */
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif /* HAVE_SYS_TYPES_H */

#ifdef HAVE_FREETYPE /* In case we're _not_ doing runtime linking. */
#define FREETYPEGLUE_ASSUME_FREETYPE 1
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#endif /* FREETYPEGLUE_ASSUME_FREETYPE */

#include <Inventor/C/basic.h>
#include <Inventor/C/threads/threadsutilp.h>
#include <Inventor/C/glue/dl.h>
#include <Inventor/C/tidbitsp.h>
#include <Inventor/C/tidbits.h>
#include <Inventor/C/errors/debugerror.h>
#include <Inventor/C/glue/freetype.h>

typedef FT_Error (*cc_ftglue_FT_Init_FreeType_t)(FT_Library * library);
typedef void (*cc_ftglue_FT_Library_Version_t)(void * library, int * major, int * minor, int * patch);
typedef void (*cc_ftglue_FT_Done_FreeType_t)(void * library);
typedef FT_Error (*cc_ftglue_FT_New_Face_t)(void * library, const char * filepathname, long faceindex, FT_Face * face);
typedef FT_Error (*cc_ftglue_FT_Done_Face_t)(void * face);
typedef FT_Error (*cc_ftglue_FT_Select_Charmap_t)(FT_Face face, int encoding);
typedef FT_Error (*cc_ftglue_FT_Set_Char_Size_t)(FT_Face face, long width, long height, unsigned int hres, unsigned int vres);
typedef void (*cc_ftglue_FT_Set_Transform_t)(FT_Face face, FT_Matrix * matrix, FT_Vector * delta);
typedef FT_Error (*cc_ftglue_FT_Get_Char_Index_t)(FT_Face face, unsigned long charidx);
typedef FT_Error (*cc_ftglue_FT_Load_Glyph_t)(FT_Face face, unsigned int glyph, int32_t loadflags);
typedef FT_Error (*cc_ftglue_FT_Get_Kerning_t)(FT_Face face, unsigned int left, unsigned int right, unsigned int kernmode, FT_Vector * akerning);
typedef FT_Error (*cc_ftglue_FT_Get_Glyph_t)(void * glyphslot, FT_Glyph * glyph);
typedef FT_Error (*cc_ftglue_FT_Glyph_To_Bitmap_t)(FT_Glyph * glyph, int rendermode, FT_Vector * origin, int destroy);
typedef void (*cc_ftglue_FT_Done_Glyph_t)(FT_Glyph glyph);
typedef FT_Error (*cc_ftglue_FT_Outline_Decompose_t)(FT_Outline * outline, const FT_Outline_Funcs * func_interface, void * user);


typedef struct {
  int available;
  cc_ftglue_FT_Init_FreeType_t FT_Init_FreeType;
  cc_ftglue_FT_Library_Version_t FT_Library_Version;
  cc_ftglue_FT_Done_FreeType_t FT_Done_FreeType;
  cc_ftglue_FT_New_Face_t FT_New_Face;
  cc_ftglue_FT_Done_Face_t  FT_Done_Face;
  cc_ftglue_FT_Select_Charmap_t FT_Select_Charmap; 
  cc_ftglue_FT_Set_Char_Size_t FT_Set_Char_Size;
  cc_ftglue_FT_Set_Transform_t FT_Set_Transform;
  cc_ftglue_FT_Get_Char_Index_t FT_Get_Char_Index;
  cc_ftglue_FT_Load_Glyph_t FT_Load_Glyph;
  cc_ftglue_FT_Get_Kerning_t FT_Get_Kerning;
  cc_ftglue_FT_Get_Glyph_t FT_Get_Glyph; 
  cc_ftglue_FT_Glyph_To_Bitmap_t FT_Glyph_To_Bitmap; 
  cc_ftglue_FT_Done_Glyph_t FT_Done_Glyph;
  cc_ftglue_FT_Outline_Decompose_t FT_Outline_Decompose;
} cc_ftglue_t;

static cc_ftglue_t * freetype_instance = NULL;
static cc_libhandle freetype_libhandle = NULL;
static int freetype_failed_to_load = 0;

/* Cleans up at exit. */
static void
ftglue_cleanup(void)
{
#ifdef FREETYPE_RUNTIME_LINKING
  if (freetype_libhandle) { cc_dl_close(freetype_libhandle); }
#endif /* FREETYPE_RUNTIME_LINKING */
  assert(freetype_instance);
  free(freetype_instance);
}

static const cc_ftglue_t *
ftglue_init(void)
{
  CC_SYNC_BEGIN(ftglue_init);

  if (!freetype_instance && !freetype_failed_to_load) {
    /* First invocation, do initializations. */
    cc_ftglue_t * fi = (cc_ftglue_t *)malloc(sizeof(cc_ftglue_t));
    (void)coin_atexit((coin_atexit_f *)ftglue_cleanup, 0);

    /* The common case is that FreeType is either available from the
       linking process or we're successfully going to link it in. */
    fi->available = 1;

#ifdef FREETYPE_RUNTIME_LINKING
    {
      int idx;
      /* FIXME: should we get the system shared library name from an
         Autoconf check? 20000930 mortene. */
      const char * possiblelibnames[] = {
        NULL, /* is set below */
        "freetype", "libfreetype", "libfreetype.so",
        "libfreetype.dylib",
        NULL
      };
      possiblelibnames[0] = coin_getenv("COIN_FREETYPE2_LIBNAME");
      idx = possiblelibnames[0] ? 0 : 1;
      while (!freetype_libhandle && possiblelibnames[idx]) {
        freetype_libhandle = cc_dl_open(possiblelibnames[idx]);
        idx++;
      }

      if (!freetype_libhandle) {
        fi->available = 0;
        freetype_failed_to_load = 1;
      }
    }
    /* Define FTGLUE_REGISTER_FUNC macro. Casting the type is
       necessary for this file to be compatible with C++ compilers. */
#define FTGLUE_REGISTER_FUNC(_funcsig_, _funcname_) \
    do { \
      fi->_funcname_ = (_funcsig_)cc_dl_sym(freetype_libhandle, SO__QUOTE(_funcname_)); \
      if (fi->_funcname_ == NULL) fi->available = 0; \
    } while (0)

#elif defined(FREETYPEGLUE_ASSUME_FREETYPE) /* !FREETYPE_RUNTIME_LINKING */

    /* Define FTGLUE_REGISTER_FUNC macro. */
#define FTGLUE_REGISTER_FUNC(_funcsig_, _funcname_) \
    fi->_funcname_ = (_funcsig_)_funcname_

#else /* !FREETYPEGLUE_ASSUME_FREETYPE */
    fi->available = 0;
    /* Define FTGLUE_REGISTER_FUNC macro. */
#define FTGLUE_REGISTER_FUNC(_funcsig_, _funcname_) \
    fi->_funcname_ = NULL

#endif /* !FREETYPEGLUE_ASSUME_FREETYPE */

    FTGLUE_REGISTER_FUNC(cc_ftglue_FT_Init_FreeType_t, FT_Init_FreeType);
    FTGLUE_REGISTER_FUNC(cc_ftglue_FT_Library_Version_t, FT_Library_Version);
    FTGLUE_REGISTER_FUNC(cc_ftglue_FT_Done_FreeType_t, FT_Done_FreeType);

    if (fi->available && (!fi->FT_Init_FreeType ||
                          !fi->FT_Library_Version ||
                          !fi->FT_Done_FreeType)) {
      /* something is seriously wrong */
      cc_debugerror_post("freetype glue",
                         "Loaded freetype DLL ok, but couldn't resolve some symbols.");
      fi->available = 0;
      freetype_failed_to_load = 1;
      freetype_instance = fi;
    }
    else {
      FTGLUE_REGISTER_FUNC(cc_ftglue_FT_New_Face_t, FT_New_Face);
      FTGLUE_REGISTER_FUNC(cc_ftglue_FT_Done_Face_t, FT_Done_Face);
      FTGLUE_REGISTER_FUNC(cc_ftglue_FT_Select_Charmap_t, FT_Select_Charmap); 
      FTGLUE_REGISTER_FUNC(cc_ftglue_FT_Set_Char_Size_t, FT_Set_Char_Size);
      FTGLUE_REGISTER_FUNC(cc_ftglue_FT_Set_Transform_t, FT_Set_Transform);
      FTGLUE_REGISTER_FUNC(cc_ftglue_FT_Get_Char_Index_t, FT_Get_Char_Index);
      FTGLUE_REGISTER_FUNC(cc_ftglue_FT_Load_Glyph_t, FT_Load_Glyph);
      FTGLUE_REGISTER_FUNC(cc_ftglue_FT_Get_Kerning_t, FT_Get_Kerning);
      FTGLUE_REGISTER_FUNC(cc_ftglue_FT_Get_Glyph_t, FT_Get_Glyph); 
      FTGLUE_REGISTER_FUNC(cc_ftglue_FT_Glyph_To_Bitmap_t, FT_Glyph_To_Bitmap); 
      FTGLUE_REGISTER_FUNC(cc_ftglue_FT_Done_Glyph_t, FT_Done_Glyph);
      FTGLUE_REGISTER_FUNC(cc_ftglue_FT_Outline_Decompose_t, FT_Outline_Decompose);

      /* Do this late, so we can detect recursive calls to this function. */
      freetype_instance = fi;

      if (!fi->available) {
        cc_debugerror_post("ftglue_init",
                           "Failed to initialize freetype glue.");
      }
    }
  }
  CC_SYNC_END(ftglue_init);
  return freetype_instance;
}


FT_Error 
cc_ftglue_FT_Init_FreeType(FT_Library * library)
{
  assert(freetype_instance && freetype_instance->available);
  return freetype_instance->FT_Init_FreeType(library);
}

void 
cc_ftglue_FT_Library_Version(void * library, int * major, int * minor, int * patch)
{
  assert(freetype_instance && freetype_instance->available);
  freetype_instance->FT_Library_Version(library, major, minor, patch);
}

void 
cc_ftglue_FT_Done_FreeType(void * library)
{
  assert(freetype_instance && freetype_instance->available);
  freetype_instance->FT_Done_FreeType(library);
}

FT_Error 
cc_ftglue_FT_New_Face(void * library, const char * filepathname, long faceindex, FT_Face * face)
{
  assert(freetype_instance && freetype_instance->available);
  return freetype_instance->FT_New_Face(library, filepathname, faceindex, face);
}

FT_Error 
cc_ftglue_FT_Done_Face(void * face)
{
  assert(freetype_instance && freetype_instance->available);
  return freetype_instance->FT_Done_Face(face);
}

FT_Error 
cc_ftglue_FT_Select_Charmap(FT_Face face, int encoding)
{
  assert(freetype_instance && freetype_instance->available);
  return freetype_instance->FT_Select_Charmap(face, encoding);
}

FT_Error 
cc_ftglue_FT_Set_Char_Size(FT_Face face, long width, long height, unsigned int hres, unsigned int vres)
{
  assert(freetype_instance && freetype_instance->available);
  return freetype_instance->FT_Set_Char_Size(face, width, height, hres, vres);
}

void 
cc_ftglue_FT_Set_Transform(FT_Face face, FT_Matrix * matrix, FT_Vector * delta)
{
  assert(freetype_instance && freetype_instance->available);
  freetype_instance->FT_Set_Transform(face, matrix, delta);
}

FT_Error 
cc_ftglue_FT_Get_Char_Index(FT_Face face, unsigned long charidx)
{
  assert(freetype_instance && freetype_instance->available);
  return freetype_instance->FT_Get_Char_Index(face, charidx);
}

FT_Error 
cc_ftglue_FT_Load_Glyph(FT_Face face, unsigned int glyph, int32_t loadflags)
{
  assert(freetype_instance && freetype_instance->available);
  return freetype_instance->FT_Load_Glyph(face, glyph, loadflags);
}

FT_Error 
cc_ftglue_FT_Get_Kerning(FT_Face face, unsigned int left, unsigned int right, unsigned int kernmode, FT_Vector * akerning)
{
  assert(freetype_instance && freetype_instance->available);
  return freetype_instance->FT_Get_Kerning(face, left, right, kernmode, akerning);
}

FT_Error 
cc_ftglue_FT_Get_Glyph(void * glyphslot, FT_Glyph * glyph)
{
  assert(freetype_instance && freetype_instance->available);
  return freetype_instance->FT_Get_Glyph(glyphslot, glyph);
}

FT_Error 
cc_ftglue_FT_Glyph_To_Bitmap(FT_Glyph * glyph, int rendermode, FT_Vector * origin, int destroy)
{
  assert(freetype_instance && freetype_instance->available);
  return freetype_instance->FT_Glyph_To_Bitmap(glyph, rendermode, origin, destroy);
}

void 
cc_ftglue_FT_Done_Glyph(FT_Glyph glyph)
{
  assert(freetype_instance && freetype_instance->available);
  freetype_instance->FT_Done_Glyph(glyph);
}

int
cc_ftglue_available(void)
{
  ftglue_init();
  return freetype_instance && freetype_instance->available;
}

FT_Error 
cc_ftglue_FT_Outline_Decompose(FT_Outline * outline, const FT_Outline_Funcs * func_interface, void * user)
{
  assert(freetype_instance && freetype_instance->available);
  return freetype_instance->FT_Outline_Decompose(outline, func_interface, user);
}

