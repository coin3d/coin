#ifndef COIN_SOSHAPE_BUMPRENDER_H
#define COIN_SOSHAPE_BUMPRENDER_H

#include <Inventor/SbVec3f.h>
#include <Inventor/SbVec2f.h>
#include <Inventor/SbVec2s.h>
#include <Inventor/lists/SbList.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/misc/SbHash.h>
#include <Inventor/C/glue/gl.h>
#include <Inventor/SbDict.h>

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

  void calcTangentSpace(const SoPrimitiveVertexCache * cache);
  void renderBump(SoState * state,
                  const SoPrimitiveVertexCache * cache,
                  SoLight * light, const SbMatrix & toobjectspace);
  void renderBumpSpecular(SoState * state,
                          const SoPrimitiveVertexCache * cache,
                          SoLight * light, const SbMatrix & toobjectspace);
  void renderNormal(SoState * state, const SoPrimitiveVertexCache * cache);

private:

  void initLight(SoLight * light, const SbMatrix & m);
  void calcTSBCoords(const SoPrimitiveVertexCache * cache, SoLight * light);
  SbVec3f getLightVec(const SbVec3f & v) const;
  void initPrograms(const cc_glglue * glue, SoState * state);
  void initDiffusePrograms(const cc_glglue * glue, SoState * state);

  void soshape_diffuseprogramdeletion(unsigned long key, void * value);
  void soshape_specularprogramdeletion(unsigned long key, void * value);


  SbList <SbVec3f> cubemaplist;
  SbList <SbVec3f> tangentlist;

  SbVec3f lightvec;
  SbBool ispointlight;

  SbDict * diffuseprogramdict; // Hash table holding the vertex/fragment program ids for each GL context.
  SbDict * specularprogramdict;

  GLuint fragmentprogramid;
  GLuint dirlightvertexprogramid;
  GLuint pointlightvertexprogramid;
  SbBool programsinitialized;

  GLuint normalrenderingvertexprogramid;
  GLuint diffusebumpdirlightvertexprogramid;
  SbBool diffuseprogramsinitialized;

};

#endif // COIN_SOSHAPE_BUMPRENDER
