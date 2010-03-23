#ifndef COIN_FONTSPEC_H
#define COIN_FONTSPEC_H

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

#ifndef COIN_INTERNAL
#error this is a private header file
#endif /* ! COIN_INTERNAL */

/* ********************************************************************** */

#include <Inventor/C/base/string.h>

#ifdef __cplusplus
extern "C" {
#endif

  typedef struct {
    float size;
    cc_string name;  
    cc_string style; /* 'Bold', 'Italic' or 'Bold Italic' */

    float complexity;  /* detail level for vector glyphs

                       FIXME: does this variable really belong here?
                       Is this really an attribute of a /font/? And is
                       it used when asking for a font from the
                       underlying font API? (I.e. FreeType or Win32.)
                       20050519 mortene. */

  } cc_font_specification;

  void cc_fontspec_construct(cc_font_specification * spec,
                             const char * name_style,
                             float size, float complexity);

  void cc_fontspec_copy(const cc_font_specification * from,
                        cc_font_specification * to);

  void cc_fontspec_clean(cc_font_specification * spec);

#ifdef __cplusplus
}
#endif

#endif /* !COIN_FONTSPEC_H */
