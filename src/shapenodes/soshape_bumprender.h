#ifndef COIN_SOSHAPE_BUMPRENDER_H
#define COIN_SOSHAPE_BUMPRENDER_H

#include <Inventor/SbVec3f.h>
#include <Inventor/SbVec2f.h>
#include <Inventor/SbVec2s.h>
#include <Inventor/lists/SbList.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/C/glue/gl.h>

class SoState;
class SoLight;
class SoGLImage;
class SbMatrix;

// FIXME: inherit from SoCache to avoid regenrating everything every frame

class soshape_bumprender {
public:
  soshape_bumprender(void);
  ~soshape_bumprender();

  void init(SoState * state);
  void addTriangle(const SoPrimitiveVertex * v0,
                   const SoPrimitiveVertex * v1,
                   const SoPrimitiveVertex * v2);

  void renderBump(SoLight * light, const SbMatrix & toobjectspace);
  void renderNormal(void);

private:
  SoState * state;
  const cc_glglue * glglue;

  void initLight(SoLight * light, const SbMatrix & m);
  void calcTSBCoords(SoLight * light);
  SbVec3f getLightVec(const SbVec3f & v) const;

  class Vertex {
  public:
    SbVec3f v;
    SbVec4f tc;
    SbVec2f bc;
    SbVec3f n;
    SbVec3f sTangent;
    SbVec3f tTangent;
  };

  SbList <Vertex> vlist;
  SbList <SbVec3f> cubemaplist;

  SoGLImage * bumpimage;
  SbVec3f lightvec;
  SbBool ispointlight;
};

#endif // COIN_SOSHAPE_BUMPRENDER
