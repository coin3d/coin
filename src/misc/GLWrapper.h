#ifndef _glwrapper_h_
#define _glwrapper_h_

//  Under Win32, we need to make sure we use the correct calling method
//  by using the APIENTRY define for the function signature types (or
//  else we'll get weird stack errors). On other platforms, just define
//  APIENTRY empty.
#ifndef APIENTRY
#define APIENTRY
#endif

#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/system/gl.h>

// Our own typedefs for OpenGL functions. These are copied from glext.h
// and prefixed with COIN_ to avoid namespace collisions.
typedef void (APIENTRY * COIN_PFNGLTEXIMAGE3DPROC) (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
typedef void (APIENTRY * COIN_PFNGLTEXIMAGE3DEXTPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels);

class GLWrapper
{
public:
  GLWrapper();
  ~GLWrapper();

  // GL calls which might be used.
  COIN_PFNGLTEXIMAGE3DPROC glTexImage3D;
  COIN_PFNGLTEXIMAGE3DEXTPROC glTexImage3DEXT;
};

#endif // _glwrapper_h_
