
#ifndef COIN_FONTSPEC_H
#define COIN_FONTSPEC_H

#include <Inventor/C/base/string.h>

typedef struct cc_font_specification {
  float size;
  cc_string * name;
  cc_string * style;
} cc_font_specification;


#endif
