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
  Collects functionality for glyph-handling common for 2D and 3D
  glyphs.
*/

/* ********************************************************************** */

#include "glyph.h"

#include <cassert>

#include <Inventor/C/base/list.h>

#include "fontlib_wrapper.h"
#include "coindefs.h"

#ifndef COIN_WORKAROUND_NO_USING_STD_FUNCS
using std::free;
#endif // !COIN_WORKAROUND_NO_USING_STD_FUNCS

/* ********************************************************************** */

void 
cc_glyph_unref(cc_dict * dict, cc_glyph * glyph, cc_glyph_finalize * f)
{
  cc_list * glyphlist;
  int ret;
  void * tmp;
  int i;

  glyph->refcount--;

  assert(glyph->refcount >= 0);
  if (glyph->refcount > 0) { return; }

  /* external finalizing: */
  if (f) { (*f)(glyph); }

  /* handling of common data: */

  ret = cc_dict_get(dict, (uintptr_t)glyph->character, &tmp);
  assert(ret);
  glyphlist = (cc_list *)tmp;
    
  for (i = 0; i < cc_list_get_length(glyphlist); i++) {
    if (glyph == (cc_glyph *)cc_list_get(glyphlist, i)) break;
  }    
  assert(i < cc_list_get_length(glyphlist));

  cc_list_remove_fast(glyphlist, i);
  if (cc_list_get_length(glyphlist) == 0) {
    (void)cc_dict_remove(dict, (uintptr_t)glyph->character);
    cc_list_destruct(glyphlist);
  }

  cc_fontspec_clean(glyph->fontspec);
  free(glyph->fontspec);

  cc_flw_done_glyph(glyph->fontidx, glyph->glyphidx);
  cc_flw_unref_font(glyph->fontidx);

  free(glyph);
}

/* ********************************************************************** */
