// Coordinates are consumed at submission; vertex data must survive EndPolygon.
#include <Inventor/SoDB.h>
#include "glue/GLUWrapper.h"
#include <cstdio>
#include <cmath>
static int count,sum;
static void APIENTRY vertex(void *data){++count;sum+=*static_cast<int *>(data);}
static void APIENTRY begin(GLenum){}
static void APIENTRY error(GLenum){count=-100;}
int main(){
 SoDB::init();const GLUWrapper_t *w=GLUWrapper();
 if(!w->available || !w->gluNewTess || !w->gluTessVertex || !w->gluTessCallback || !w->gluTessBeginPolygon || !w->gluTessBeginContour || !w->gluTessEndContour || !w->gluTessEndPolygon || !w->gluDeleteTess)return 77;
 auto *t=w->gluNewTess();if(!t)return 77;
 w->gluTessCallback(t,GLU_TESS_VERTEX,reinterpret_cast<gluTessCallback_cb_t>(vertex));
 w->gluTessCallback(t,GLU_TESS_BEGIN,reinterpret_cast<gluTessCallback_cb_t>(begin));
 w->gluTessCallback(t,GLU_TESS_ERROR,reinterpret_cast<gluTessCallback_cb_t>(error));
 int indices[]={1,2,4};double input[][3]={{0,0,0},{10,0,0},{0,10,0}};double coords[3];
 w->gluTessBeginPolygon(t,NULL);w->gluTessBeginContour(t);
 for(int i=0;i<3;++i){for(int j=0;j<3;++j)coords[j]=input[i][j];w->gluTessVertex(t,coords,&indices[i]);for(double &v:coords)v=std::nan("");}
 w->gluTessEndContour(t);w->gluTessEndPolygon(t);w->gluDeleteTess(t);SoDB::finish();
 bool ok=count==3 && sum==7;std::printf("GLU coordinate reuse: vertices %d, index sum %d\n",count,sum);return ok?0:1;
}
