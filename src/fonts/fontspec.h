
#ifndef COIN_FONTSPEC_H
#define COIN_FONTSPEC_H

#include <Inventor/C/base/string.h>

typedef struct cc_font_specification {
  float size;
  cc_string * name;  
  cc_string * style; /* 'Bold', 'Italic' or 'Bold Italic' */
  float complexity;  /* detail level for vector glyphs */
} cc_font_specification;


#endif
