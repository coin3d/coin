/* FIXME: just a mock-up implementation yet. handegar is working on
   it. 20030901 mortene. */

struct cc_glyph2d {
  /* XXX */
};

typedef struct cc_glyph2d cc_glyph2d;

cc_glyph2d * cc_glyph2d_getglyph(uint32_t character, struct cc_font_specification * spec);

void cc_glyph2d_getadvance(const cc_glyph2d * g, int & x, int & y);
void cc_glyph2d_getkerning(const cc_glyph2d * left, const cc_glyph2d * right, int & x, int & y);

unsigned int cc_glyph2d_getwidth(const cc_glyph2d * g);
unsigned uint8_t * cc_glyph2d_getbitmap(const cc_glyph2d * g);
