#ifndef COIN_GLUE_GL_CORE_H
#define COIN_GLUE_GL_CORE_H

#ifndef COIN_INTERNAL
#error this is a private header file
#endif

#ifndef COIN_BUILDING_COIN
#error this header is only available while building Coin
#endif

#if defined(__gl_h_) || defined(__gl_glext_h_) || \
    defined(GL_VERSION_ES_CM_1_0) || defined(GL_ES_VERSION_2_0)
#error incompatible OpenGL headers were included before glue/gl-core.h
#endif

#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES 1
#endif

#if defined(__APPLE__)
#include <OpenGL/gl3.h>
#else
#if defined(_WIN32)
#include <windows.h>
#endif
#include <GL/glcorearb.h>
#endif

#endif /* ! COIN_GLUE_GL_CORE_H */
