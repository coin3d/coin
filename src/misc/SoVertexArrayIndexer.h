#ifndef COIN_VAINDEXER_H
#define COIN_VAINDEXER_H

#include <Inventor/lists/SbList.h>
#include <Inventor/system/gl.h>
#include <Inventor/C/glue/gl.h>
#include <stdlib.h>

class SoVAIndexer {
public:
  SoVAIndexer(GLenum target);
  ~SoVAIndexer();
  
  void setNext(SoVAIndexer * next);
  SoVAIndexer * getNext(void) const;
  GLenum getTarget(void) const;
  
  void addTriangle(const int32_t v0,
                   const int32_t v1,
                   const int32_t v2);

  void addQuad(const int32_t v0,
               const int32_t v1,
               const int32_t v2,
               const int32_t v3);
  
  void beginTarget(void);
  void targetVertex(const int32_t v);
  void endTarget(void);

  void close(void);
  void render(const cc_glglue * glue);

  // for debugging
  int getNumVertices(void) {
    return this->indexarray.getLength();
  }

private:
  GLenum target;
  SoVAIndexer * next;

  int targetcounter;
  SbList <GLsizei> countarray;
  SbList <const int32_t *> ciarray;
  SbList <int32_t> indexarray;
};


#endif // COIN_VAINDEXER_H
