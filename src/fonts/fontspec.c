/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
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
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#include "fontspec.h"

#include <string.h>


void
cc_fontspec_construct(cc_font_specification * spec,
		      const char * name_style, float size, float complexity)
{
  const char * tmpstr, * tmpptr;

  spec->size = size;
  spec->complexity = complexity;

  cc_string_construct(&spec->name);
  cc_string_set_text(&spec->name, name_style);

  cc_string_construct(&spec->style);

  /* Check if style is included in the fontname using the
     "family:style" syntax. */
  tmpstr = cc_string_get_text(&spec->name);
  tmpptr = strchr(tmpstr, ':');
  if (tmpptr != NULL) {
    const int pos = (int)(tmpptr - tmpstr);
    const int namelen = cc_string_length(&spec->name);

    cc_string_set_text(&spec->style, cc_string_get_text(&spec->name));
    cc_string_remove_substring(&spec->style, 0, pos);
    cc_string_remove_substring(&spec->name, pos, namelen - 1);
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
