/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

/*!
  \class SoGLDisplayList Inventor/elements/SoGLCacheContextElement.h
  \brief The SoGLDisplayList class stores and manages OpenGL display lists.
  \ingroup elements

  The TEXTURE_OBJECT type is not directly supported in Coin. We handle
  textures differently in a more flexible class called SoGLImage,
  which also stores some information about the texture used when
  rendering. Old code which use this element should not stop
  working though. The texture object extension will just not be used,
  and the texture will be stored in a display list instead.
*/

// where this class is declared
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/caches/SoGLRenderCache.h>
#include <Inventor/elements/SoCacheElement.h>

#include <assert.h>
#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H
#if HAVE_WINDOWS_H
#include <windows.h>
#endif // HAVE_WINDOWS_H
#include <GL/gl.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

/*!
  Constructor.
*/
SoGLDisplayList::SoGLDisplayList(SoState * state, Type type, int allocnum,
                                 SbBool mipmaptexobj)
  : type(type),
    numalloc(allocnum),
    context(SoGLCacheContextElement::get(state)),
    refcount(0),
    mipmap(mipmaptexobj)
{
  if (type == TEXTURE_OBJECT) {
    // it is only possible to create one texture object at a time
    assert(allocnum == 1);
#if GL_VERSION_1_1
    // use temporary variable, in case GLuint is typedef'ed to
    // something other than unsigned int
    GLuint tmpindex; 
    glGenTextures(1, &tmpindex);
    this->firstindex = (unsigned int )tmpindex;
#elif GL_EXT_texture_object
    static int sogldl_texobj_ext = -1;
    if (sogldl_texobj_ext == -1) {
      sogldl_texobj_ext =
        SoGLCacheContextElement::getExtID("GL_EXT_texture_object");
    }
    if (SoGLCacheContextElement::extSupported(state, sogldl_texobj_ext)) {
      GLuint tmpindex;
      glGenTexturesEXT(1, &tmpindex);
      this->firstindex = (unsigned int) tmpindex;
    }
    else {
      // fall back to display list
      this->type = DISPLAY_LIST;
      this->firstindex = (unsigned int) glGenLists(allocnum);
    }
#else // GL_EXT_texture_object
    this->firstindex = (unsigned int) glGenLists(allocnum);
#endif // ! GL_EXT_texture_object
  }
  else {
    this->firstindex = (unsigned int) glGenLists(allocnum);
  }
}

// private destructor. Use ref()/unref()
SoGLDisplayList::~SoGLDisplayList()
{
  if (this->type == DISPLAY_LIST) {
    glDeleteLists((GLuint) this->firstindex, this->numalloc);
  }
  else {
    // use temporary variable in case GLUint != unsigned int. It is
    // only possible to create one texture objects at a time, so it's
    // safe just to copy and delete the first index.
    GLuint tmpindex = (GLuint) this->firstindex;
#if GL_VERSION_1_1
    glDeleteTextures(1, &tmpindex);
#elif GL_EXT_texture_object
    glDeleteTexturesEXT(1, &tmpindex);
#endif // GL_EXT_texture_object
  }
}

/*!
  Increase reference count for this display list/texture object.
*/
void
SoGLDisplayList::ref(void)
{
  this->refcount++;
}

/*!
  Decrease reference count for this instance. When reference count
  reaches 0, the instence is deleted.
*/
void
SoGLDisplayList::unref(SoState * state)
{
  assert(this->refcount > 0);
  if (--this->refcount == 0) {
    // Let SoGLCacheContext delete this instance the next time context is current.
    SoGLCacheContextElement::scheduleDelete(state, this);
  }
}

/*!
  Open this display list/texture object.
*/
void
SoGLDisplayList::open(SoState * state, int index)
{
  if (type == DISPLAY_LIST) {
    glNewList((GLuint) (this->firstindex+index), GL_COMPILE_AND_EXECUTE);
  }
  else {
    assert(index == 0);
#if GL_VERSION_1_1
    glBindTexture(GL_TEXTURE_2D, (GLuint) this->firstindex);
#elif GL_EXT_texture_object
    glBindTextureEXT(GL_TEXTURE_2D, (GLuint) this->firstindex);
#endif // GL_EXT_texture_object
  }
}

/*!
  Close this display list/texture object.
*/
void
SoGLDisplayList::close(SoState * state)
{
  if (this->type == DISPLAY_LIST) {
    glEndList();
  }
}

/*!
  Execute this display list/texture object.
*/
void
SoGLDisplayList::call(SoState * state, int index)
{
  if (this->type == DISPLAY_LIST) {
    glCallList((GLuint) (this->firstindex + index));
  }
  else {
    assert(index == 0);
#if GL_VERSION_1_1
    glBindTexture(GL_TEXTURE_2D, (GLuint) this->firstindex);
#elif GL_EXT_texture_object
    glBindTextureEXT(GL_TEXTURE_2D, (GLuint) this->firstindex+index);
#endif // GL_EXT_texture_object
  }
  this->addDependency(state);
}

/*!
  Create a dependency on the displat list.
*/
void
SoGLDisplayList::addDependency(SoState * state)
{
  if (state->isCacheOpen()) {
    SoGLRenderCache * cache = (SoGLRenderCache*)
      SoCacheElement::getCurrentCache(state);
    if (cache) cache->addNestedCache(this);
  }
}

/*!
  Returns whether the texture object stored in this instance
  was created with mipmap data. This method is an extension
  versus the Open Inventor API.
*/
SbBool
SoGLDisplayList::isMipMapTextureObject(void) const
{
  return this->mipmap;
}

/*!
  Return type. Display list or texture object.
*/
SoGLDisplayList::Type
SoGLDisplayList::getType(void) const
{
  return this->type;
}

/*!
  Return number of display lists/texture objects allocated.
*/
int
SoGLDisplayList::getNumAllocated(void) const
{
  return this->numalloc;
}

/*!
  Return first GL index for this display list.
*/
unsigned int
SoGLDisplayList::getFirstIndex(void) const
{
  return this->firstindex;
}

/*!
  Return an id for the current context.
*/
int
SoGLDisplayList::getContext(void) const
{
  return this->context;
}
