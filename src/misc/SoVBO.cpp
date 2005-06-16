/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2005 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org/> for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

/*!
  \class SoVBO
  \brief The SoVBO class is used to handle OpenGL vertex buffer objects.

  It wraps the buffer handling, taking care of multi-context handling
  and allocation/deallocation of buffers. FIXME: more doc.
  
*/

#include "SoVBO.h"
#include <Inventor/misc/SoContextHandler.h>
#include <Inventor/elements/SoGLCacheContextElement.h>

/*!
  Constructor
*/
SoVBO::SoVBO(const GLenum target, const GLenum usage)
  : target(target),
    usage(usage),
    data(NULL),
    datasize(0),
    vbohash(5)
{
  SoContextHandler::addContextDestructionCallback(context_destruction_cb, this);
}

/*!
  Destructor
*/
SoVBO::~SoVBO()
{
  SoContextHandler::removeContextDestructionCallback(context_destruction_cb, this);
  // schedule delete for all allocated GL resources
  this->vbohash.apply(vbo_schedule, NULL);
}

/*!
  Sets the buffer data.
*/
void 
SoVBO::setData(const GLvoid * data, intptr_t size)
{
  // schedule delete for all allocated GL resources
  this->vbohash.apply(vbo_schedule, NULL);
  // clear hash table
  this->vbohash.clear();

  this->data = data;
  this->datasize = size;
}

/*!
  Binds the buffer for the context \a contextid.
*/
void 
SoVBO::bindBuffer(uint32_t contextid)
{
  const cc_glglue * glue = cc_glglue_instance((int) contextid);

  GLuint buffer;
  if (!this->vbohash.get(contextid, buffer)) {
    // need to create a new buffer for this context
    cc_glglue_glGenBuffers(glue, 1, &buffer);
    cc_glglue_glBindBuffer(glue, GL_ARRAY_BUFFER, buffer);
    cc_glglue_glBufferData(glue, GL_ARRAY_BUFFER,
                           this->datasize,
                           this->data,
                           GL_STATIC_DRAW);
    this->vbohash.put(contextid, buffer);
  }
  else {
    // buffer already exists, bind it
    cc_glglue_glBindBuffer(glue, GL_ARRAY_BUFFER, buffer);
  }
}

//
// Callback from SbHash
//
void 
SoVBO::vbo_schedule(const uint32_t & key,
                    const GLuint & value,
                    void * closure)
{
  void * ptr = (void*) ((uintptr_t) value);
  SoGLCacheContextElement::scheduleDeleteCallback(key, vbo_delete, ptr);
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
