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
  \class SoGLCacheElement Inventor/elements/SoGLCacheElement.h
  \brief The SoGLCacheElement class handles OpenGL caches for multiple contexts.
  \ingroup elements
*/

#include <Inventor/elements/SoGLCacheContextElement.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/lists/SbList.h>
#include <Inventor/SbName.h>
#include <Inventor/misc/SoState.h>
#include "../misc/GLWrapper.h"
#include <string.h>
#include <stdlib.h>

#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/system/gl.h>


static int biggest_cache_context_id = 0;

SO_ELEMENT_SOURCE(SoGLCacheContextElement);

typedef struct {
  SbName extname;
  SbList <int> context;
  SbList <SbBool> supported;
} so_glext_info;

typedef struct {
  int context;
  int major;
  int minor;
  int revision;
} so_glversion_info;

typedef struct {
  int context;
  GLWrapper *wrapper;
} so_glwrapper_info;

typedef struct {
  int context;
  int handle;
} so_gltexhandle_info;

static SbList <so_glext_info *> *extsupportlist;
static SbList <so_glversion_info> *glversionlist;
static SbList <so_glwrapper_info> *glwrapperlist;
static SbList <SoGLDisplayList*> *scheduledeletelist;

static void soglcachecontext_cleanup(void)
{
  int i,n = extsupportlist->getLength();
  for (i = 0; i < n; i++) {
    delete (*extsupportlist)[i];
  }
  n = glwrapperlist->getLength();
  for (i = 0; i < n; i++) {
    delete (*glwrapperlist)[i].wrapper;
  }
  delete extsupportlist;
  delete glversionlist;
  delete scheduledeletelist;
  delete glwrapperlist;
}


//
// check if OpenGL extension is supported.
//
static SbBool
extension_supported(const char * extension)
{
  static const GLubyte *extensions = NULL;
  const GLubyte *start;
  GLubyte *where, *terminator;

  /* Extension names should not have spaces. */
  where = (GLubyte *) strchr(extension, ' ');
  if (where || *extension == '\0')
    return FALSE;

  if (!extensions)
    extensions = glGetString(GL_EXTENSIONS);
  start = extensions;
  for (;;) {
    where = (GLubyte *) strstr((const char *)start, extension);
    if (!where)
      break;
    terminator = where + strlen(extension);
    if (where == start || *(where - 1) == ' ') {
      if (*terminator == ' ' || *terminator == '\0') {
        return TRUE;
      }
    }
    start = terminator;
  }
  return FALSE;
}


// doc from parent
void
SoGLCacheContextElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoGLCacheContextElement, inherited);

  extsupportlist = new SbList <so_glext_info *>;
  glversionlist = new SbList <so_glversion_info>;
  scheduledeletelist = new SbList <SoGLDisplayList*>;
  glwrapperlist = new SbList <so_glwrapper_info>;
  atexit(soglcachecontext_cleanup);
}

/*!
  Destructor.
*/
SoGLCacheContextElement::~SoGLCacheContextElement()
{
}

// doc from parent
void
SoGLCacheContextElement::init(SoState * state)
{
  // these values will be set int set(), but initialize them anyway
  this->context = 0;
  this->twopass = FALSE;
  this->remote = FALSE;
  this->autocachebits = 0;
}

// doc from parent
SbBool
SoGLCacheContextElement::matches(const SoElement * elt) const
{
  const SoGLCacheContextElement * elem = (SoGLCacheContextElement*) elt;

  return
    this->context == elem->context &&
    this->twopass == elem->twopass &&
    this->remote == elem->remote;
}

// doc from parent
SoElement *
SoGLCacheContextElement::copyMatchInfo(void) const
{
  SoGLCacheContextElement * elem = (SoGLCacheContextElement*)
    this->getTypeId().createInstance();
  elem->context = this->context;
  elem->twopass = this->twopass;
  elem->remote = this->remote;
  return elem;
}

/*!
  Sets data for context.
*/
void
SoGLCacheContextElement::set(SoState * state, int context,
                             SbBool twopasstransparency,
                             SbBool remoterendering)
{
  SoGLCacheContextElement * elem = (SoGLCacheContextElement *)
    state->getElementNoPush(classStackIndex);
  elem->twopass = twopasstransparency;
  elem->remote = remoterendering;
  elem->autocachebits = 0;
  elem->context = context;
  if (context > biggest_cache_context_id) {
    biggest_cache_context_id = context;
  }

  if (remoterendering) elem->autocachebits = DO_AUTO_CACHE;

  int i = 0;
  while (i < scheduledeletelist->getLength()) {
    SoGLDisplayList * dl = (*scheduledeletelist)[i];
    if (dl->getContext() == context) {
      scheduledeletelist->removeFast(i);
      delete dl;
    }
    else i++;
  }
}

/*!
  Returns context id.
*/
int
SoGLCacheContextElement::get(SoState * state)
{
  const SoGLCacheContextElement * elem = (const SoGLCacheContextElement *)
    SoElement::getConstElement(state, classStackIndex);
  return elem->context;
}

/*!
  Returns an extension id based on the GL extension string.
  The extension id can be used to quickly test for the availability
  of an extension later, using extSupported().
*/
int
SoGLCacheContextElement::getExtID(const char * str)
{
  SbName extname(str);
  int i, n = extsupportlist->getLength();
  for (i = 0; i < n; i++) {
    if ((*extsupportlist)[i]->extname == extname) break;
  }
  if (i == n) { // not found
    so_glext_info * info = new so_glext_info;
    info->extname = extname;
    extsupportlist->append(info);
  }
  return i;
}

/*!
  Returns TRUE if the extension is supported for the current context.
  \a extid must be an id returned from getExtId(). The test result
  is cached so this method is pretty fast and can be used run-time.
*/
SbBool
SoGLCacheContextElement::extSupported(SoState * state, int extid)
{
  assert(extid >= 0 && extid < extsupportlist->getLength());

  so_glext_info * info = (*extsupportlist)[extid];

  int currcontext = SoGLCacheContextElement::get(state);
  int n = info->context.getLength();
  for (int i = 0; i < n; i++) {
    if (info->context[i] == currcontext) return info->supported[i];
  }
  SbBool supported = extension_supported(info->extname.getString());
  info->context.append(currcontext);
  info->supported.append(supported);
  return supported;
}

/*!
  Returns the OpenGL version for the current context. This method
  is an extension versus the Open Inventor API.
*/
void
SoGLCacheContextElement::getOpenGLVersion(SoState *state, 
                                          int &major, int &minor)
{
  int revision;
  SoGLCacheContextElement::getOpenGLVersion(state,major,minor,revision);
}

/*!
  Returns the OpenGL version for the current context. This method
  is an extension versus the Open Inventor API.
*/
void
SoGLCacheContextElement::getOpenGLVersion(SoState *state, 
                                          int &major, 
                                          int &minor, 
                                          int &revision)
{
  int currcontext = SoGLCacheContextElement::get(state);

  int i, n = glversionlist->getLength();
  for (i = 0; i < n; i++) {
    if ((*glversionlist)[i].context == currcontext) {
      major = (*glversionlist)[i].major;
      minor = (*glversionlist)[i].minor;
      revision = (*glversionlist)[i].revision;
      return;
    }
  }

  const char *versionstr = (const char *)glGetString(GL_VERSION);
  int maj,min,rev;
  SoGLCacheContextElement::glVersionStringToNumeric(versionstr, maj, min, rev);

  so_glversion_info info;
  info.context = currcontext;
  info.major = major = maj;
  info.minor = minor = min;
  info.revision = revision = rev;

  glversionlist->append(info);
}

SbBool
SoGLCacheContextElement::openGLVersionMatchesAtLeast(SoState *state,
                                                     int major,
                                                     int minor,
                                                     int revision)
{
  int maj,min,rev;
  SoGLCacheContextElement::getOpenGLVersion(state,maj,min,rev);
  if (maj < major) return false;
  else if (maj > major) return true;
  if (min < minor) return false;
  else if (min > minor) return true;
  if (rev < revision) return false;
  return true;
}

/*!
  Returns a GLWrapper instance that is valid for the context defined by
  /e state. This method is an extension versus the Open Inventor API.
*/
GLWrapper *
SoGLCacheContextElement::getGLWrapper(SoState *state)
{
  int currcontext = SoGLCacheContextElement::get(state);

  int i, n = glwrapperlist->getLength();
  for (i = 0; i < n; i++) {
    if ((*glwrapperlist)[i].context == currcontext) {
      return (*glwrapperlist)[i].wrapper;
    }
  }

  so_glwrapper_info info;
  info.context = currcontext;
  info.wrapper = new GLWrapper;
  glwrapperlist->append(info);
  return info.wrapper;
}

/*!
  Returns if mipmapped textures are fast for the current context.
  In Coin, we just return TRUE for the moment.
*/
SbBool
SoGLCacheContextElement::areMipMapsFast(SoState * state)
{
  return TRUE; // FIXME: how do we test this? pederb 20001003
}

/*!
  Not properly supported yet.
*/
void
SoGLCacheContextElement::shouldAutoCache(SoState * state, int bits)
{
}

/*!
  Not properly supported yet.
*/
void
SoGLCacheContextElement::setAutoCacheBits(SoState * state, int bits)
{
  SoGLCacheContextElement * elem = (SoGLCacheContextElement*)
    state->getElementNoPush(classStackIndex);

  elem->autocachebits = bits;
}

/*!
  Not properly supported yet.
*/
int
SoGLCacheContextElement::resetAutoCacheBits(SoState * state)
{
  SoGLCacheContextElement *elem = (SoGLCacheContextElement *)
    state->getElementNoPush(classStackIndex);
  int ret = elem->autocachebits;
  elem->autocachebits = elem->remote ? DO_AUTO_CACHE : 0;
  return ret;
}

/*!
  Returns TRUE if rendering is remote.
*/
SbBool
SoGLCacheContextElement::getIsRemoteRendering(SoState * state)
{
  const SoGLCacheContextElement *elem = (const SoGLCacheContextElement *)
    state->getConstElement(classStackIndex);
  return elem->remote;
}

//
// internal method used by SoGLDisplayList to delete list as soon as
// the display list context is current again.
//
void
SoGLCacheContextElement::scheduleDelete(SoState * state, class SoGLDisplayList * dl)
{
  if (state && dl->getContext() == SoGLCacheContextElement::get(state)) {
    delete dl;
  }
  else {
    scheduledeletelist->append(dl);
  }
}

/*!
  Returns an unique cache context id. If you render the same scene graph
  into two or different cache contexts, and you've not using display
  list and texture object sharing among contexts, the cache context
  id need to be unique for rendering to work.

  This method is an extension versus the Open Inventor API.

  \sa SoGLRenderAction::setCacheContext()
*/
uint32_t
SoGLCacheContextElement::getUniqueCacheContext(void)
{
  return (uint32_t) ++biggest_cache_context_id;
}

/*!
  \internal

  FIXME: Reinsert Peder's comment about this being ripped from GLUWrapper.
*/
void
SoGLCacheContextElement::glVersionStringToNumeric(const char *versionstr,
                                                  int &major,
                                                  int &minor,
                                                  int &revision)
{
  char buffer[256];
  char *dotptr;

  major = 0;
  minor = 0;
  revision = 0;

  (void)strncpy(buffer, (const char *)versionstr, 255);
  buffer[255] = '\0'; /* strncpy() will not null-terminate if strlen > 255 */
  dotptr = strchr(buffer, '.');
  if (dotptr) {
    char * spaceptr;
    char * start = buffer;
    *dotptr = '\0';
    major = atoi(start);
    start = ++dotptr;

    dotptr = strchr(start, '.');
    spaceptr = strchr(start, ' ');
    if (!dotptr && spaceptr) dotptr = spaceptr;
    if (dotptr && spaceptr && spaceptr < dotptr) dotptr = spaceptr;
    if (dotptr) {
      int terminate = *dotptr == ' ';
      *dotptr = '\0';
      minor = atoi(start);
      if (!terminate) {
        start = ++dotptr;
        dotptr = strchr(start, ' ');
        if (dotptr) *dotptr = '\0';
        revision = atoi(start);
      }
    }
    else {
      minor = atoi(start);
    }
  }
}
