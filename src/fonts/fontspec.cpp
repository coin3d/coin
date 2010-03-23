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

#include "fonts/fontspec.h"

#include <string.h>

#include "glue/freetype.h"

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

  /* handle the previously allowed ':Bold Italic' case for fontconfig */
  /* FIXME: this is an ugly non robust workaround. it would be better
     to agree on an abstract fontname matching schema that is then
     consistently applied upon all font backend
     implementations. 20040929 tamer. */
  if (cc_fcglue_available()) {
    tmpstr = cc_string_get_text(&spec->name);

    if ((tmpptr = strchr(tmpstr, ':'))) {
      char * tmpptrspace;
      if ((tmpptrspace = (char *) strchr(tmpptr, ' '))) {
        *tmpptrspace = ':';
      }
    }

    return;
  }

  /* Check if style is included in the fontname using the
     "family:style" syntax. */
  tmpstr = cc_string_get_text(&spec->name);
  if ((tmpptr = strchr(tmpstr, ':'))) {
    const int pos = (int)(tmpptr - tmpstr);
    const int namelen = cc_string_length(&spec->name);

    int trimstyleend, trimnamestart;
    int trimposstyle = pos + 1;
    int trimposname = pos - 1;

    while (tmpstr[trimposstyle] == ' ') {
      ++trimposstyle;
    }

    while (tmpstr[trimposname] == ' ') {
      --trimposname;
    }

    cc_string_set_text(&spec->style, cc_string_get_text(&spec->name));
    cc_string_remove_substring(&spec->style, 0, trimposstyle - 1);
    cc_string_remove_substring(&spec->name, trimposname + 1, namelen-1);

    trimstyleend = cc_string_length(&spec->style);
    trimposstyle = trimstyleend;
    tmpstr = cc_string_get_text(&spec->style);

    while (tmpstr[trimstyleend-1] == ' ') {
      --trimstyleend;
    }

    if(trimstyleend !=  trimposstyle) {
      cc_string_remove_substring(&spec->style, trimstyleend, cc_string_length(&spec->style) - 1);
    }

    tmpstr = cc_string_get_text(&spec->name);
    trimnamestart = 0;
    while (tmpstr[trimnamestart] == ' ') {
      ++trimnamestart;
    }

    if (trimnamestart != 0) {
      cc_string_remove_substring(&spec->name, 0, trimnamestart-1);
    }

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
