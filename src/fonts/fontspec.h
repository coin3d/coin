
#ifndef COIN_FONTSPEC_H
#define COIN_FONTSPEC_H

#include <Inventor/C/base/string.h>

typedef struct cc_font_specification {
  float angle;
  float size;
  cc_string * name;
  cc_string * family;
  cc_string * style;
} cc_font_specification;


#endif
