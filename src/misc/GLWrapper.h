// FIXME: add header. 20011115 mortene.

#ifndef COIN_GLWRAPPER_H
#define COIN_GLWRAPPER_H

/* Under Win32, we need to make sure we use the correct calling method
   by using the APIENTRY define for the function signature types (or
   else we'll get weird stack errors). On other platforms, just define
   APIENTRY empty. */
#ifndef APIENTRY
#define APIENTRY
#endif /* !APIENTRY */

#if HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <Inventor/system/gl.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Our own typedefs for OpenGL functions. These are copied from
   glext.h and prefixed with COIN_ to avoid namespace collisions. */
typedef void (APIENTRY * COIN_PFNGLTEXIMAGE3DPROC) (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
typedef void (APIENTRY * COIN_PFNGLTEXIMAGE3DEXTPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels);

typedef struct {
  /* OpenGL versioning. */
  struct {
    unsigned int major, minor, release;
  } version;

  /* OpenGL calls. Will be NULL if not available, otherwise they
     contain a valid function pointer into the OpenGL library. */
  COIN_PFNGLTEXIMAGE3DPROC glTexImage3D;
  COIN_PFNGLTEXIMAGE3DEXTPROC glTexImage3DEXT;
} GLWrapper_t;

const GLWrapper_t * GLWrapper(int contextid);

/*
  Returns 1 if the OpenGL implementation of the wrapper context is at
  least as "late" as what is given with the input arguments. Otherwise
  returns 0.
 */
int GLWrapper_versionMatchesAtLeast(GLWrapper_t * wrapper,
                                    unsigned int major,
                                    unsigned int minor,
                                    unsigned int release);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !COIN_GLWRAPPER_H */
