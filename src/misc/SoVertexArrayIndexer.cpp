#include "SoVAIndexer.h"
#include <assert.h>
#include <string.h>

SoVAIndexer::SoVAIndexer(GLenum targetin)
  : target(targetin),
    next(NULL)
{
}

SoVAIndexer::~SoVAIndexer() 
{
  for (int i = 0; i < this->ciarray.getLength(); i++) {
    delete[] this->ciarray[i];
  }
  delete this->next;
}
  
GLenum 
SoVAIndexer::getTarget(void) const
{
  return this->target;
}

void 
SoVAIndexer::setNext(SoVAIndexer * nextin)
{
  this->next = nextin;
}

SoVAIndexer * 
SoVAIndexer::getNext(void) const
{
  return this->next;
}

void 
SoVAIndexer::addTriangle(const int32_t v0,
                         const int32_t v1,
                         const int32_t v2)
{
  assert(this->target == GL_TRIANGLES);
  this->indexarray.append(v0);
  this->indexarray.append(v1);
  this->indexarray.append(v2);
}

void 
SoVAIndexer::addQuad(const int32_t v0,
                     const int32_t v1,
                     const int32_t v2,
                     const int32_t v3)
{
  assert(this->target == GL_QUADS);
  this->indexarray.append(v0);
  this->indexarray.append(v1);
  this->indexarray.append(v2);
}
  
void 
SoVAIndexer::beginTarget(void)
{
  this->targetcounter = 0;
}
 
void 
SoVAIndexer::targetVertex(const int32_t v)
{
  this->targetcounter++;
  this->indexarray.append(v);
}

void 
SoVAIndexer::endTarget(void)
{
  this->countarray.append(this->targetcounter);
}

void 
SoVAIndexer::close(void)
{
  this->indexarray.fit();
  this->countarray.fit();
  this->ciarray.truncate(0);
  const int32_t * ptr = this->indexarray.getArrayPtr();
  for (int i = 0; i < this->countarray.getLength(); i++) {
    this->ciarray.append(ptr);
    ptr += (int) this->countarray[i];
  }
}

void 
SoVAIndexer::render(const cc_glglue * glue)
{
  switch (this->target) {
  case GL_TRIANGLES:
  case GL_QUADS:
    cc_glglue_glDrawElements(glue,
                             this->target,
                             this->indexarray.getLength(),
                             GL_UNSIGNED_INT,
                             (const GLvoid*) this->indexarray.getArrayPtr());

    break;
  default:
    if (cc_glglue_has_multidraw_vertex_arrays(glue)) {
      cc_glglue_glMultiDrawElements(glue,
                                    this->target,
                                    (GLsizei*) this->countarray.getArrayPtr(),
                                    GL_UNSIGNED_INT,
                                    (const GLvoid**) this->ciarray.getArrayPtr(),
                                    this->countarray.getLength());
    }
    else {
      for (int i = 0; i < this->countarray.getLength(); i++) {
        const int32_t * ptr = this->ciarray[i];
        GLsizei cnt = this->countarray[i];
        cc_glglue_glDrawElements(glue,
                                 this->target,
                                 cnt,
                                 GL_UNSIGNED_INT,
                                 (const GLvoid*) ptr);
      }
    }
    break;
  }

  if (this->next) this->next->render(glue);
}
