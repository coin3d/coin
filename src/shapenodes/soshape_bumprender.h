#ifndef COIN_SOSHAPE_BUMPRENDER_H
#define COIN_SOSHAPE_BUMPRENDER_H

#include <Inventor/SbVec3f.h>
#include <Inventor/SbVec2f.h>
#include <Inventor/SbVec2s.h>
#include <Inventor/lists/SbList.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/C/glue/gl.h>
#include <Inventor/misc/SbHash.h>

class SoState;
class SoLight;
class SoGLImage;
class SbMatrix;
class SoPrimitiveVertexCache;

// FIXME: inherit from SoCache to avoid regenerating everything every frame

class soshape_bumprender {
public:
  soshape_bumprender(void);
  ~soshape_bumprender();

  void init(SoState * state);
  void renderBump(const SoPrimitiveVertexCache * cache, 
                  SoLight * light, const SbMatrix & toobjectspace);
  void renderNormal(const SoPrimitiveVertexCache * cache);

private:
  SoState * state;
  const cc_glglue * glglue;
  
  void initLight(SoLight * light, const SbMatrix & m);
  void calcTSBCoords(const SoPrimitiveVertexCache * cache, SoLight * light);
  SbVec3f getLightVec(const SbVec3f & v) const;

  SbList <SbVec3f> cubemaplist;
  SbList <SbVec3f> tangentlist;

  SoGLImage * bumpimage;
  SbVec3f lightvec;
  SbBool ispointlight;
};

#endif // COIN_SOSHAPE_BUMPRENDER
