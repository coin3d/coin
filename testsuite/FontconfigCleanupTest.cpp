#include <Inventor/SoDB.h>
#include "fonts/freetype.h"
#include "glue/freetype.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
static int stage,live,created,destroyed,facecalls;
static bool pathmatches;
static FcPattern *pattern(){++live;++created;return reinterpret_cast<FcPattern *>(std::malloc(1));}
extern "C" int cc_fcglue_available(){return 1;}
extern "C" FcPattern *cc_fcglue_FcNameParse(const unsigned char *){return stage==0?NULL:pattern();}
extern "C" int cc_fcglue_FcPatternAddDouble(FcPattern *,const char *,double){return stage!=1;}
extern "C" int cc_fcglue_FcConfigSubstitute(void *,FcPattern *,FcMatchKind){return stage!=2;}
extern "C" void cc_fcglue_FcDefaultSubstitute(FcPattern *){}
extern "C" FcPattern *cc_fcglue_FcFontMatch(void *,FcPattern *,FcResult *){return stage==3?NULL:pattern();}
extern "C" FcResult cc_fcglue_FcPatternGetString(const FcPattern *,const char *,int,unsigned char **s){
 static unsigned char path[]="fixture-font.ttf";*s=path;return stage==4?FcResultNoMatch:FcResultMatch;
}
extern "C" void cc_fcglue_FcPatternDestroy(FcPattern *p){--live;++destroyed;std::free(p);}
extern "C" void cc_fcglue_FcPatternPrint(const FcPattern *){}
extern "C" FT_Error cc_ftglue_FT_New_Face(void *,const char *path,long,FT_Face *){
 ++facecalls;pathmatches=stage!=5 || std::strcmp(path,"fixture-font.ttf")==0;return 1;
}
int main(){
 SoDB::init();int failures=0;
 for(stage=0;stage<6;++stage){
  live=created=destroyed=facecalls=0;pathmatches=false;
  if(cc_flwft_get_font("Fixture",12)!=NULL || live || created!=destroyed || facecalls!=1 || !pathmatches){
   ++failures;std::fprintf(stderr,"FAIL stage=%d live=%d created=%d destroyed=%d facecalls=%d\n",stage,live,created,destroyed,facecalls);
  }
 }
 SoDB::finish();std::printf("Fontconfig cleanup: 6 stages, %d failures\n",failures);return failures?1:0;
}
