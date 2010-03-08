#ifndef COIN_GLYPH_H
#define COIN_GLYPH_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

/*
  The interface below collects functionality for glyph-handling common
  for 2D and 3D glyphs.
*/

/* ********************************************************************** */

#ifndef COIN_INTERNAL
#error this is a private header file
#endif /* ! COIN_INTERNAL */

/* ********************************************************************** */

#include "base/dict.h"
#include "fonts/fontspec.h"

/* ********************************************************************** */

#ifdef __cplusplus
extern "C" {
#endif

#if 0 /* hack for emacs indentation */
}
#endif

struct cc_glyph {
  int refcount;

  int glyphidx;
  uint32_t character;

  int fontidx;    
  cc_font_specification * fontspec;
};

typedef struct cc_glyph cc_glyph;

/* ********************************************************************** */

typedef void cc_glyph_finalize(cc_glyph *);

void cc_glyph_unref(cc_dict * d, cc_glyph * g, cc_glyph_finalize * f);

/* ********************************************************************** */

#ifdef __cplusplus
}
#endif

#endif /* !COIN_GLYPH_H */
