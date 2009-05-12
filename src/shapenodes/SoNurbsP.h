#ifndef COIN_SOSHAPE_SONURBSP_H
#define COIN_SOSHAPE_SONURBSP_H
#include "glue/glp.h"

#include <Inventor/SoPrimitiveVertex.h>

class SoAction;

template<class Master>
class SoNurbsP
{
 public:
  //
  // used only for GLU callbacks
  //
  struct coin_nurbs_cbdata {
    SoAction * action;
    SoPrimitiveVertex vertex;
    Master * thisp;
  };

  static void APIENTRY tessBegin(int , void * data);
  static void APIENTRY tessTexCoord(float * texcoord, void * data);
  static void APIENTRY tessNormal(float * normal, void * data);
  static void APIENTRY tessVertex(float * vertex, void * data);
  static void APIENTRY tessEnd(void * data);

};

template<class Master>
void APIENTRY
SoNurbsP<Master>::tessTexCoord(float * texcoord, void * data)
{
  coin_nurbs_cbdata * cbdata = static_cast<coin_nurbs_cbdata *>(data);
  cbdata->vertex.setTextureCoords(SbVec4f(texcoord[0], texcoord[1], texcoord[2], texcoord[3]));
}

template<class Master>
void APIENTRY
SoNurbsP<Master>::tessNormal(float * normal, void * data)
{
  coin_nurbs_cbdata * cbdata = static_cast<coin_nurbs_cbdata *>(data);
  cbdata->vertex.setNormal(SbVec3f(normal[0], normal[1], normal[2]));
}

template<class Master>
void APIENTRY
SoNurbsP<Master>::tessVertex(float * vertex, void * data)
{
  coin_nurbs_cbdata * cbdata = static_cast<coin_nurbs_cbdata *>(data);
  //The GLU API is not perfectly clear on wether the vertex always
  //contains 4 datapoints. But for every function where it is
  //documented, the documentation says 3D homogeneous coordinates are
  //used.
  cbdata->vertex.setPoint(SbVec3f(vertex[0], vertex[1], vertex[2])/vertex[3]);
  cbdata->thisp->shapeVertex(&cbdata->vertex);
}

template<class Master>
void APIENTRY
SoNurbsP<Master>::tessEnd(void * data)
{
  coin_nurbs_cbdata * cbdata = static_cast<coin_nurbs_cbdata *>(data);
  cbdata->thisp->endShape();
}

//This function has specializations in implementation files, so be careful about reuse
template<class Master>
void APIENTRY
SoNurbsP<Master>::tessBegin(int type, void * data)
{
  coin_nurbs_cbdata * cbdata = static_cast<coin_nurbs_cbdata *>(data);
  typename Master::TriangleShape shapetype;
  switch (type) {
  case GL_LINES:
    shapetype = SoShape::LINES;
    break;
  case GL_LINE_STRIP:
    shapetype = SoShape::LINE_STRIP;
    break;
  case GL_LINE_LOOP:
    shapetype = SoShape::LINE_STRIP; // will not be closed...
    // FIXME: implement this functionality. 20010909 mortene.
    // Update 20060130 kyrah: According to gluNurbsCallback(3), the
    // only valid arguments we can expect here are GL_LINES or
    // GL_LINE_STRIP, so this should really be an assert. (See also
    // FIXME from 20010909 below - yes, it should be an assert.)

    // FIXME: un-commenting the following line exposes a _weird_ HP-UX
    // aCC bug -- should investigate closer.  (Detected with aCC
    // version B3910B A.03.25).  If possible, try to re-write the
    // COIN_STUB() function with a work-around for the aCC bug.
    // 20010917 mortene.

    //COIN_STUB();

#if COIN_DEBUG
    SoDebugError::postWarning("SoNurbsCurveP::tessBegin",
                              "LINE_LOOP is not supported yet");
#endif // COIN_DEBUG
    break;
  case GL_POINTS:
    shapetype = SoShape::POINTS;
    break;
  default:
    shapetype = SoShape::POLYGON; // illegal value
    // FIXME: should this be an assert, or does it represent
    // something which is out of our control, like a possible future
    // feature of the GLU tessellator?  20010909 mortene.
#if COIN_DEBUG && 1 // debug
    SoDebugError::postWarning("SoNurbsCurveP::tessBegin",
                              "unsupported GL enum: 0x%x", type);
#endif // debug
    break;
  }
  if (shapetype != SoShape::POINTS) {
    cbdata->thisp->beginShape(cbdata->action, shapetype, NULL);
  }
}


#endif //COIN_SOSHAPE_SONURBSP_H
