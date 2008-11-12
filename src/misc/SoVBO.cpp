/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2008 by Kongsberg SIM.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg SIM about acquiring
 *  a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg SIM, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

/*!
  \class SoVBO
  \brief The SoVBO class is used to handle OpenGL vertex buffer objects.

  It wraps the buffer handling, taking care of multi-context handling
  and allocation/deallocation of buffers. FIXME: more doc.

*/

#include "misc/SoVBO.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <Inventor/misc/SoContextHandler.h>
#include <Inventor/misc/SoGLDriverDatabase.h>
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/elements/SoShapeStyleElement.h>
#include <Inventor/C/tidbits.h>
#include <Inventor/SbVec3f.h>
#include <Inventor/errors/SoDebugError.h>

#include "misc/SoVertexArrayIndexer.h"
#include "threads/threadsutilp.h"
#include "glue/glp.h"
#include "tidbitsp.h"
#include "share/gl/CoinGLPerformance.h"

static int vbo_vertex_count_min_limit = -1;
static int vbo_vertex_count_max_limit = -1;
static int vbo_render_as_vertex_arrays = -1;
static int vbo_enabled = -1;
static int vbo_debug = -1;

static SbHash <SbBool, uint32_t> * vbo_isfast_hash;
//
// Callback from SbHash
//
/*
void
SoVBO::vbo_schedule(const uint32_t & key,
                    const GLuint & value,
                    void * closure)
{
  void * ptr = (void*) ((uintptr_t) value);
  SoGLCacheContextElement::scheduleDeleteCallback(key, vbo_delete, ptr);
}
*/

/*!
  Constructor
*/
SoVBO::SoVBO(const GLenum target, const GLenum usage)
  : target(target),
    usage(usage),
    data(NULL),
    datasize(0),
    dataid(0),
    didalloc(FALSE),
    vbohash(5)
{
  SoContextHandler::addContextDestructionCallback(context_destruction_cb, this);
}


//
// Callback from SoGLCacheContextElement
//
void
SoVBO::vbo_delete(void * closure, uint32_t contextid)
{
  const cc_glglue * glue = cc_glglue_instance((int) contextid);
  GLuint id = (GLuint) ((uintptr_t) closure);
  cc_glglue_glDeleteBuffers(glue, 1, &id);
}

struct vbo_schedule :
  public SbHash <GLuint, uint32_t>::ApplyFunctor<void *>
{
  void operator()(uint32_t & key,
                GLuint & value,
                void * closure
                )
  {
    void * ptr = (void*) ((uintptr_t) value);
    SoGLCacheContextElement::scheduleDeleteCallback(key, SoVBO::vbo_delete, ptr);
  }
};

/*!
  Destructor
*/
SoVBO::~SoVBO()
{
  SoContextHandler::removeContextDestructionCallback(context_destruction_cb, this);
  // schedule delete for all allocated GL resources
  vbo_schedule functor;
  this->vbohash.apply<void *>(functor, NULL);
  if (this->didalloc) {
    char * ptr = (char*) this->data;
    delete[] ptr;
  }
}

// atexit cleanup function
static void vbo_atexit_cleanup(void)
{
  delete vbo_isfast_hash;
  vbo_isfast_hash = NULL;
  vbo_vertex_count_min_limit = -1;
  vbo_vertex_count_max_limit = -1;
  vbo_render_as_vertex_arrays = -1;
  vbo_enabled = -1;
}

void
SoVBO::init(void)
{
  coin_glglue_add_instance_created_callback(context_created, NULL);

  vbo_isfast_hash = new SbHash <SbBool, uint32_t> (3);
  coin_atexit(vbo_atexit_cleanup, CC_ATEXIT_NORMAL);

  // use COIN_VBO_MAX_LIMIT to set the largest VBO we create
  if (vbo_vertex_count_max_limit < 0) {
    const char * env = coin_getenv("COIN_VBO_MAX_LIMIT");
    if (env) {
      vbo_vertex_count_max_limit = atoi(env);
    }
    else {
      vbo_vertex_count_max_limit = 256000;
    }
  }

  // use COIN_VBO_MIN_LIMIT to set the smallest VBO we create
  if (vbo_vertex_count_min_limit < 0) {
    const char * env = coin_getenv("COIN_VBO_MIN_LIMIT");
    if (env) {
      vbo_vertex_count_min_limit = atoi(env);
    }
    else {
      vbo_vertex_count_min_limit = 40;
    }
  }

  // use COIN_VERTEX_ARRAYS to globally disable vertex array rendering
  if (vbo_render_as_vertex_arrays < 0) {
    const char * env = coin_getenv("COIN_VERTEX_ARRAYS");
    if (env) {
      vbo_render_as_vertex_arrays = atoi(env);
    }
    else {
      vbo_render_as_vertex_arrays = 1;
    }
  }

  // use COIN_VBO to globally disable VBOs when doing vertex array rendering
  if (vbo_enabled < 0) {
    const char * env = coin_getenv("COIN_VBO");
    if (env) {
      vbo_enabled = atoi(env);
    }
    else {
      vbo_enabled = 1;
    }
  }
  if (vbo_debug < 0) {
    const char * env = coin_getenv("COIN_DEBUG_VBO");
    if (env) {
      vbo_debug = atoi(env);
    }
    else {
      vbo_debug = 0;
    }
  }
}

/*!
  Used to allocate buffer data. The user is responsible for filling in
  the correct type of data in the buffer before the buffer is used.

  \sa setBufferData()
*/
void *
SoVBO::allocBufferData(intptr_t size, uint32_t dataid)
{
  // schedule delete for all allocated GL resources
  vbo_schedule functor;
  this->vbohash.apply<void *>(functor, NULL);
  // clear hash table
  this->vbohash.clear();

  if (this->didalloc && this->datasize == size) {
    return (void*)this->data;
  }
  if (this->didalloc) {
    char * ptr = (char*) this->data;
    delete[] ptr;
  }

  char * ptr = new char[size];
  this->didalloc = TRUE;
  this->data = (const GLvoid*) ptr;
  this->datasize = size;
  this->dataid = dataid;
  return (void*) this->data;
}

/*!
  Sets the buffer data. \a dataid is a unique id used to identify
  the buffer data. In Coin it's possible to use the node id
  (SoNode::getNodeId()) to test if a buffer is valid for a node.
*/
void
SoVBO::setBufferData(const GLvoid * data, intptr_t size, uint32_t dataid)
{
  // schedule delete for all allocated GL resources
  vbo_schedule functor;
  this->vbohash.apply<void *>(functor, NULL);
  // clear hash table
  this->vbohash.clear();

  // clean up old buffer (if any)
  if (this->didalloc) {
    char * ptr = (char*) this->data;
    delete[] ptr;
  }

  this->data = data;
  this->datasize = size;
  this->dataid = dataid;
  this->didalloc = FALSE;
}

/*!
  Returns the buffer data id.

  \sa setBufferData()
*/
uint32_t
SoVBO::getBufferDataId(void) const
{
  return this->dataid;
}

/*!
  Returns the data pointer and size.
*/
void
SoVBO::getBufferData(const GLvoid *& data, intptr_t & size)
{
  data = this->data;
  size = this->datasize;
}


/*!
  Binds the buffer for the context \a contextid.
*/
void
SoVBO::bindBuffer(uint32_t contextid)
{
  if ((this->data == NULL) ||
      (this->datasize == 0)) {
    assert(0 && "no data in buffer");
    return;
  }

  const cc_glglue * glue = cc_glglue_instance((int) contextid);

  GLuint buffer;
  if (!this->vbohash.get(contextid, buffer)) {
    // need to create a new buffer for this context
    cc_glglue_glGenBuffers(glue, 1, &buffer);
    cc_glglue_glBindBuffer(glue, this->target, buffer);
    cc_glglue_glBufferData(glue, this->target,
                           this->datasize,
                           this->data,
                           this->usage);
    this->vbohash.put(contextid, buffer);
  }
  else {
    // buffer already exists, bind it
    cc_glglue_glBindBuffer(glue, this->target, buffer);
  }

#if COIN_DEBUG
  if (vbo_debug) {
    if (this->target == GL_ELEMENT_ARRAY_BUFFER) {
      SoDebugError::postInfo("SoVBO::bindBuffer",
                             "Rendering using VBO. Index array size: %d",
                             this->datasize / sizeof(int32_t));
    }
    else {
      SoDebugError::postInfo("SoVBO::bindBuffer",
                             "Setting up buffer for rendering. Datasize: %d",
                             this->datasize);
    }
  }
#endif // COIN_DEBUG
}



//
// Callback from SoContextHandler
//
void
SoVBO::context_destruction_cb(uint32_t context, void * userdata)
{
  GLuint buffer;
  SoVBO * thisp = (SoVBO*) userdata;

  if (thisp->vbohash.get(context, buffer)) {
    const cc_glglue * glue = cc_glglue_instance((int) context);
    cc_glglue_glDeleteBuffers(glue, 1, &buffer);
    thisp->vbohash.remove(context);
  }
}


/*!
  Sets the global limits on the number of vertex data in a node before
  vertex buffer objects are considered to be used for rendering.
*/
void
SoVBO::setVertexCountLimits(const int minlimit, const int maxlimit)
{
  vbo_vertex_count_min_limit = minlimit;
  vbo_vertex_count_max_limit = maxlimit;
}

/*!
  Returns the vertex VBO minimum limit.

  \sa setVertexCountLimits()
 */
int
SoVBO::getVertexCountMinLimit(void)
{
  return vbo_vertex_count_min_limit;
}

/*!
  Returns the vertex VBO maximum limit.

  \sa setVertexCountLimits()
 */
int
SoVBO::getVertexCountMaxLimit(void)
{
  return vbo_vertex_count_max_limit;
}

SbBool
SoVBO::shouldCreateVBO(SoState * state, const uint32_t contextid, const int numdata)
{
  if (!vbo_enabled || !vbo_render_as_vertex_arrays) return FALSE;
  int minv = SoVBO::getVertexCountMinLimit();
  int maxv = SoVBO::getVertexCountMaxLimit();
  return
    (numdata >= minv) &&
    (numdata <= maxv) &&
    SoVBO::isVBOFast(contextid) &&
    !(SoShapeStyleElement::get(state)->getFlags() & SoShapeStyleElement::SHADOWMAP);

}

SbBool
SoVBO::shouldRenderAsVertexArrays(SoState * state,
                                  const uint32_t contextid,
                                  const int numdata)
{
  // FIXME: consider also using results from the performance tests

  // don't render as vertex arrays if there are very few elements to
  // be rendered. The VA setup overhead would make it slower than just
  // doing plain immediate mode rendering.
  return (numdata >= vbo_vertex_count_min_limit) && vbo_render_as_vertex_arrays;
}

SbBool
SoVBO::isVBOFast(const uint32_t contextid)
{
  SbBool result = TRUE;
  assert(vbo_isfast_hash != NULL);
  (void) vbo_isfast_hash->get(contextid, result);
  return result;
}

//
// callback from glglue (when a new glglue instance is created)
//
void
SoVBO::context_created(const uint32_t contextid, void * closure)
{
  SoVBO::testGLPerformance(contextid);
}

/* **********************************************************************************************/

typedef struct {
  SoVBO * vbo;
  SbVec3f * vertexarray;
  int size;
  SoVertexArrayIndexer * indexer;
  uint32_t contextid;
} vbo_performance_test_data;

// how many times the geometry is rendered in the performance test callbacks
#define PERF_NUM_LOOP 4

// rendering loop used to test vertex array and VBO rendering speed
static void
vbo_performance_test(const cc_glglue * glue,
                     const vbo_performance_test_data * data,
                     const SbBool do_vbo)
{
  if (do_vbo) { data->vbo->bindBuffer(data->contextid); }

  cc_glglue_glVertexPointer(glue, 3, GL_FLOAT, 0,
                            do_vbo ? NULL : data->vertexarray);

  cc_glglue_glEnableClientState(glue, GL_VERTEX_ARRAY);

  for (int i = 0; i < PERF_NUM_LOOP; i++) {
    data->indexer->render(glue, do_vbo, data->contextid);
  }
  cc_glglue_glDisableClientState(glue, GL_VERTEX_ARRAY);

  if (do_vbo) { cc_glglue_glBindBuffer(glue, GL_ARRAY_BUFFER, 0); }
}

// callback to test VBO rendering speed
static void
vbo_performance_test_vbo(const cc_glglue * glue, void * closure)
{
  vbo_performance_test_data * data = (vbo_performance_test_data *) closure;
  vbo_performance_test(glue, data, TRUE);
}

// callback to test vertex array rendering speed
static void
vbo_performance_test_va(const cc_glglue * glue, void * closure)
{
  vbo_performance_test_data * data = (vbo_performance_test_data *) closure;
  vbo_performance_test(glue, data, FALSE);
}

// callback to test immediate mode rendering speed
static void
vbo_performance_test_immediate(const cc_glglue * glue, void * closure)
{
  vbo_performance_test_data * data = (vbo_performance_test_data *) closure;

  int x, y;
  int size = data->size;
  SbVec3f * vertexarray = data->vertexarray;

  for (int i = 0; i < PERF_NUM_LOOP; i++) {
#define IDX(ix, iy) ((iy)*size+ix)
    glBegin(GL_TRIANGLES);
    for (y = 0; y < size-1; y++) {
      for (x = 0; x < size-1; x++) {
        glVertex3fv((const GLfloat*) &vertexarray[IDX(x,y)]);
        glVertex3fv((const GLfloat*)&vertexarray[IDX(x+1,y)]);
        glVertex3fv((const GLfloat*)&vertexarray[IDX(x+1,y+1)]);

        glVertex3fv((const GLfloat*)&vertexarray[IDX(x,y)]);
        glVertex3fv((const GLfloat*)&vertexarray[IDX(x+1,y+1)]);
        glVertex3fv((const GLfloat*)&vertexarray[IDX(x,y+1)]);
      }
    }
    glEnd();
#undef IDX
  }
}

#undef PERF_NUM_LOOP

//
// test OpenGL performance for a context.
//
void
SoVBO::testGLPerformance(const uint32_t contextid)
{
  SbBool isfast = FALSE;
  // did we alreay test this for this context?
  assert(vbo_isfast_hash != NULL);
  if (vbo_isfast_hash->get(contextid, isfast)) return;

#if 1 // disabled for now. Our test seems to be buggy
  vbo_isfast_hash->put(contextid, TRUE);
  return;
#else // disabled
  const cc_glglue * glue = cc_glglue_instance(contextid);
  if (SoGLDriverDatabase::isSupported(glue, SO_GL_VERTEX_BUFFER_OBJECT)) {
    // create a regular grid with 128x128 points to test the
    // performance. This size was chosen since it's fairly quick to
    // render on most gfx cards (important to avoid that the
    // application freezes up for too long while running this test).
    const int size = 128;
    const int half = size / 2;
    SbVec3f * vertexarray = new SbVec3f[size*size];
    SoVertexArrayIndexer * idx = new SoVertexArrayIndexer;

    int x, y;

    for (y = 0; y < size; y++) {
      for (x = 0; x < size; x++) {
        vertexarray[y*size+x].setValue(float(x-half)/float(size)*0.1f,
                                       float(y-half)/float(size)*0.1f, 4.0f);
      }
    }
#define IDX(ix, iy) ((iy)*size+ix)
    for (y = 0; y < size-1; y++) {
      for (x = 0; x < size-1; x++) {
        idx->addTriangle(IDX(x,y), IDX(x+1,y), IDX(x+1, y+1));
        idx->addTriangle(IDX(x,y), IDX(x+1,y+1), IDX(x, y+1));
      }
    }
#undef IDX
    idx->close();
    SoVBO * vbo = new SoVBO();
    vbo->setBufferData(vertexarray, size*size*sizeof(SbVec3f), 0);
    vbo_performance_test_data data;
    data.vbo = vbo;
    data.vertexarray = vertexarray;
    data.indexer = idx;
    data.contextid = contextid;
    data.size = size;
    // bind buffer first to create VBO
    vbo->bindBuffer(contextid);
    // unset VBO buffer before rendering
    cc_glglue_glBindBuffer(glue, GL_ARRAY_BUFFER, 0);

    CC_PERF_RENDER_CB * rendercbs[] = {
      vbo_performance_test_immediate,
      vbo_performance_test_va,
      vbo_performance_test_vbo
    };

    double averagerendertime[3];
    cc_perf_gl_timer(glue,
                     sizeof(rendercbs) / sizeof(rendercbs[0]),
                     rendercbs,
                     averagerendertime,
                     NULL, NULL, 10, SbTime(0.2),
                     &data);

#if defined(COIN_DEBUG) && 0
    SoDebugError::postInfo("SoVBO::testGLPerformance",
                           "render times: %g %g %g",
                           averagerendertime[0],
                           averagerendertime[1],
                           averagerendertime[2]);
#endif

    delete vbo;
    delete idx;
    delete[] vertexarray;

    // VBO is considered to be fast if it's 1.5 times faster than vertex
    // array and immediate mode rendering.
    if ((1.5f * averagerendertime[2] < averagerendertime[0]) &&
        (1.5f * averagerendertime[2] < averagerendertime[1])) isfast = TRUE;

  }
  vbo_isfast_hash->put(contextid, isfast);
#endif // !disabled
}
