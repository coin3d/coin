/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
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
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
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

#include <Inventor/caches/SoGLRenderCache.h>
#include <Inventor/elements/SoCacheElement.h>
#include <Inventor/elements/SoGLTexture3EnabledElement.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/C/glue/gl.h>

#include <string.h>
#include <assert.h>

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
  // Check for known buggy OpenGL driver.
  const char * versionstr = (const char *)glGetString(GL_VERSION);
  if (strcmp(versionstr, "1.3.1 NVIDIA 28.02") == 0) {
    // (From NVidia's changelog, it looks like the problem we've been
    // seeing with the 28.02 driver and displaylists *might* have been
    // fixed for the next version (28.80)).

    // Here's an Inventor file which can be used to reproduce the bug:
    // -----8<----- [snip] ----------8<----- [snip] ----------8<------
    // #Inventor V2.1 ascii
    //
    // # This dead simple scenegraph causes the examinerviewer to go blank
    // # on a Linux box with GeForce2 card and version 1.3.1 28.02 of the
    // # NVidia OpenGL drivers. The problem is gone for version 1.3.1 29.60
    // # of the drivers, so this seems _very_ much like a driver bug with
    // # OpenGL displaylists.
    // #
    // # The bug is also present for SGI Inventor.
    // #
    // # <mortene@sim.no>.
    //
    // Separator {
    //    renderCaching ON
    //    ShapeHints {
    //       vertexOrdering COUNTERCLOCKWISE
    //       faceType UNKNOWN_FACE_TYPE
    //    }
    //    Cube { }
    // }
    // -----8<----- [snip] ----------8<----- [snip] ----------8<------
    

    // FIXME: should be more robust, and rather just disable the use
    // of GL displaylists (but still issuing an
    // SoDebugError::postWarning()). This should be straightforward to
    // do when the FIXME below on better handling of the case where we
    // are not able to allocate displaylist indices is taken care
    // of. 20020911 mortene.

    static SbBool first = TRUE;
    if (first) {
      SoDebugError::post("SoGLDisplayList::SoGLDisplayList",
                         "This OpenGL driver ('%s') is known to contain serious "
                         "bugs in GL displaylist handling, and we strongly urge "
                         "you to upgrade! As long as you are using this driver, "
                         "GL rendering is likely to cause all sorts of nasty "
                         "problems.",
                         versionstr);
      first = FALSE;
    }
  }


  // Reserve displaylist IDs.

  if (this->type == TEXTURE_OBJECT) {
    assert(allocnum == 1 && "it is only possible to create one texture object at a time");
    const cc_glglue * glw = cc_glglue_instance(this->context);
    if (cc_glglue_has_texture_objects(glw)) {
      // use temporary variable, in case GLuint is typedef'ed to
      // something other than unsigned int
      GLuint tmpindex;
      cc_glglue_glGenTextures(glw, 1, &tmpindex);
      this->firstindex = (unsigned int )tmpindex;
    }
    else { // Fall back to display list
      this->type = DISPLAY_LIST;
      this->firstindex = (unsigned int) glGenLists(allocnum);
      if (this->firstindex == 0) {
        SoDebugError::post("SoGLDisplayList::SoGLDisplayList",
                           "could not reserve %d displaylist%s", allocnum, allocnum==1 ? "" : "s");
      }
    }
  }
  else {
    this->firstindex = (unsigned int) glGenLists(allocnum);
    if (this->firstindex == 0) {
      SoDebugError::post("SoGLDisplayList::SoGLDisplayList",
                         "could not reserve %d displaylist%s", allocnum, allocnum==1 ? "" : "s");
      // FIXME: be more robust in handling this -- the rendering will
      // gradually go bonkers after we hit this problem. 20020619 mortene.
    }
#if COIN_DEBUG && 0 // debug
    SoDebugError::postInfo("SoGLDisplayList::SoGLDisplayList",
                           "firstindex==%d", this->firstindex);
#endif // debug
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
    const cc_glglue * glw = cc_glglue_instance(this->context);
    if (cc_glglue_has_texture_objects(glw)) {
      cc_glglue_glDeleteTextures(glw, 1, &tmpindex);
    }
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
    this->bindTexture(state);
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
    this->bindTexture(state);
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

/*!
  \COININTERNAL

  \since 2001-11-27
*/
void
SoGLDisplayList::bindTexture(SoState *state)
{
  const cc_glglue * glw = cc_glglue_instance(this->context);
  if (cc_glglue_has_texture_objects(glw)) {
    if (SoGLTexture3EnabledElement::get(state)) {
      // FIXME: shouldn't this rather be an assert()? 20020918 mortene.
      if (cc_glglue_has_3d_textures(glw)) {
        cc_glglue_glBindTexture(glw, GL_TEXTURE_3D, (GLuint)this->firstindex);
      }
    }
    else {
      cc_glglue_glBindTexture(glw, GL_TEXTURE_2D, (GLuint)this->firstindex);
    }
  }
}
