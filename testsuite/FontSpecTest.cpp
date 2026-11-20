#include "fonts/fontspec.h"
#include <cstdio>
#include <cstring>
struct Case {const char *input,*family,*style;};
int main() {
 const Case cases[]={{"Arial:Bold","Arial","Bold"},{":Bold","","Bold"},{"Arial:","Arial",""},
 {"   :   ","",""},{"  Arial  :  Bold Italic  ","Arial","Bold Italic"},{"","",""},
 {" \t\n ","",""},{"Arial","Arial",""},{"Arial:style=Bold:slant=italic","Arial:style=Bold:slant=italic",""},
 {"Arial:Bold:Italic","Arial","Bold:Italic"},{"C:\\Fonts\\a.ttf","C:\\Fonts\\a.ttf",""},
 {"/tmp/collection:local/font.ttf","/tmp/collection:local/font.ttf",""},
 {"/tmp/collection:local/font.ttf:Bold","/tmp/collection:local/font.ttf","Bold"},
 {nullptr,"",""},{"\xc3\xa9:Bold","\xc3\xa9","Bold"}};
 int failures=0;
 for(const auto &test:cases) {
  cc_font_specification spec;cc_fontspec_construct(&spec,test.input,12.5f,0.7f);
  if(std::strcmp(cc_string_get_text(&spec.name),test.family) || std::strcmp(cc_string_get_text(&spec.style),test.style)
     || spec.size!=12.5f || spec.complexity!=0.7f) {++failures;std::fprintf(stderr,"FAIL parser '%s': family='%s' style='%s'\n",test.input?test.input:"NULL",cc_string_get_text(&spec.name),cc_string_get_text(&spec.style));}
  cc_fontspec_clean(&spec);
 }
 // Exercise every empty/whitespace endpoint around the separator.
 for(int left=0;left<32;++left)for(int right=0;right<32;++right) {
  char input[66];std::memset(input,' ',left+right+1);input[left]=':';input[left+right+1]=0;
  cc_font_specification spec;cc_fontspec_construct(&spec,input,12,0.5f);
  if(cc_string_length(&spec.name)||cc_string_length(&spec.style))++failures;
  cc_fontspec_clean(&spec);
 }
 std::printf("Font parser: 1039 cases, %d failures\n",failures);return failures?1:0;
}
