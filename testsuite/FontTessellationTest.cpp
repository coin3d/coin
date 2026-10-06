// Faults enter through the native adapters; production tessellation and rollback run.
#include <Inventor/SoDB.h>
#include "fonts/freetype.h"
#include "fonts/common.h"
#include "fonts/fontspec.h"
#include "glue/freetype.h"
#include "glue/GLUWrapper.h"
#include <vector>
#include <limits>
#include <cmath>
#include <cstdio>
#include <cstdlib>
struct coin_GLUtessellator { std::vector<void *> vertices; };
static coin_GLUtessellator tess;
static gluTessCallback_cb_t beginfn,vertexfn,errorfn;
static int stage,done,deleted,vertices;
static FT_OutlineGlyphRec fixture;
static coin_GLUtessellator *APIENTRY newtess(){tess.vertices.clear();vertices=0;return stage==1?NULL:&tess;}
static void APIENTRY callback(coin_GLUtessellator *,GLenum kind,gluTessCallback_cb_t fn){
 if(kind==GLU_TESS_BEGIN)beginfn=fn; if(kind==GLU_TESS_VERTEX)vertexfn=fn; if(kind==GLU_TESS_ERROR)errorfn=fn;
}
static void APIENTRY nop(coin_GLUtessellator *){}
static void APIENTRY polygon(coin_GLUtessellator *,void *){}
static void APIENTRY normal(coin_GLUtessellator *,double,double,double){}
static void APIENTRY vertex(coin_GLUtessellator *,double coords[3],void *data){
 if(!std::isfinite(coords[0]) || !std::isfinite(coords[1]))std::abort();
 ++vertices;tess.vertices.push_back(data);
}
static void APIENTRY endpoly(coin_GLUtessellator *){
 if(stage==3){reinterpret_cast<void (*)(GLenum)>(errorfn)(100151);return;}
 reinterpret_cast<void (*)(GLenum)>(beginfn)(GL_TRIANGLES);
 for(size_t i=0;i+2<tess.vertices.size();i+=3)
  for(int j=0;j<3;++j)reinterpret_cast<void (*)(void *)>(vertexfn)(tess.vertices[i+j]);
}
static void APIENTRY destroy(coin_GLUtessellator *){++deleted;}
extern "C" const GLUWrapper_t *GLUWrapper(){
 static GLUWrapper_t w={};w.available=1;w.gluNewTess=newtess;w.gluTessCallback=callback;
 w.gluTessBeginPolygon=polygon;w.gluTessEndPolygon=endpoly;w.gluTessBeginContour=nop;
 w.gluTessEndContour=nop;w.gluTessNormal=stage==4?NULL:normal;w.gluTessVertex=vertex;w.gluDeleteTess=destroy;return &w;
}
extern "C" FT_Error cc_ftglue_FT_Set_Char_Size(FT_Face,long,long,unsigned,unsigned){return 0;}
extern "C" FT_Error cc_ftglue_FT_Load_Glyph(FT_Face,unsigned,int32_t){return 0;}
extern "C" FT_Error cc_ftglue_FT_Get_Glyph(void *, FT_Glyph * g)
{
  fixture.root.format = static_cast<decltype(fixture.root.format)>(stage == 5 ? 0 : 0x6f75746c);
  *g = reinterpret_cast<FT_Glyph>(&fixture);
  return 0;
}
extern "C" void cc_ftglue_FT_Done_Glyph(FT_Glyph){++done;}
extern "C" FT_Error cc_ftglue_FT_Outline_Decompose(FT_Outline *,const FT_Outline_Funcs *f,void *u){
 FT_Vector a={0,0},b={640,0},c={640,640},d={0,640};
 if(f->move_to(&a,u)||f->line_to(&b,u))return 1;
 if(stage==2)return 1;
 if(f->conic_to(&c,&d,u)||f->cubic_to(&c,&b,&a,u))return 1;
 return 0;
}
static int checks,failures;
static void check(const char *label,bool ok){++checks;if(!ok){++failures;std::fprintf(stderr,"FAIL %s stage %d\n",label,stage);}}
static void release(cc_font_vector_glyph *g){if(g){std::free(g->vertices);std::free(g->faceindices);std::free(g->edgeindices);std::free(g);}}
int main(){
 SoDB::init();FT_FaceRec_ face={};FT_GlyphSlotRec_ slot={};face.glyph=&slot;
 float values[]={-1,0,0.001f,0.5f,1,1000000,std::numeric_limits<float>::infinity(),std::numeric_limits<float>::quiet_NaN()};
 for(float value:values){stage=0;auto *g=cc_flwft_get_vector_glyph(&face,1,value);
  check("bounded finite tessellation",g && vertices>=3 && vertices<=33);release(g);
  cc_font_specification spec;cc_fontspec_construct(&spec,"defaultFont",12,value);
  check("finite cache complexity",std::isfinite(spec.complexity)&&spec.complexity>=0&&spec.complexity<=1);cc_fontspec_clean(&spec);
 }
 for(stage=1;stage<=5;++stage){const int fault=stage;done=deleted=0;
  auto *g=cc_flwft_get_vector_glyph(&face,1,.5f);check("failure rejected",!g);release(g);
  check("glyph released",done==(stage==4?0:1));
  check("tessellator released",deleted==((stage==2||stage==3)?1:0));
  stage=0;g=cc_flwft_get_vector_glyph(&face,1,.5f);check("next glyph recovers",g&&vertices==17);release(g);stage=fault;
 }
 SoDB::finish();std::printf("Tessellation: %d checks, %d failures\n",checks,failures);return failures?1:0;
}
