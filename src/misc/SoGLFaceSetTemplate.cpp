/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

#if NBINDING==PER_VERTEX_INDEXED
#define NINDEX TRUE
#elif NBINDING==PER_FACE_INDEXED
#define NINDEX TRUE
#else
#define NINDEX FALSE
#endif

#if MBINDING==PER_VERTEX_INDEXED
#define MINDEX TRUE
#elif MBINDING==PER_FACE_INDEXED
#define MINDEX TRUE
#else
#define MINDEX FALSE
#endif
(const SoGLCoordinateElement * const vertexlist,
 const int32_t *vertexindices,
 int numindices,
 const SbVec3f *normals,
 const int32_t *normalindices,
 SoMaterialBundle *materials,
 const int32_t *matindices,
 const SoTextureCoordinateBundle * const texcoords,
 const int32_t *texindices)
{
  // to avoid compiler warnings
#if NINDEX==FALSE && !defined(_WIN32)
  if (normalindices);
#endif
#if MINDEX==FALSE && !defined(_WIN32)
  if (matindices);
#endif
#if TEXTURES==FALSE && !defined(_WIN32)
  if (texcoords && texindices);
#else
  int texidx = 0;
#endif

  int mode = GL_POLYGON; // ...to save a test
  int newmode;
  const int32_t *viptr = vertexindices;
  const int32_t *viendptr = viptr + numindices;
  int32_t v1, v2, v3, v4, v5 = 0; // v5 init unnecessary, but kills a compiler warning.
  SbVec3f dummynormal(0,0,1);

#if NBINDING==PER_VERTEX || NBINDING==PER_FACE || NBINDING==PER_VERTEX_INDEXED || NBINDING==PER_FACE_INDEXED || TEXTURES==TRUE
  const SbVec3f *currnormal = &dummynormal;
  if (normals) currnormal = normals;
#endif // currnormal wrapper

#if MBINDING==PER_FACE || MBINDING==PER_VERTEX
  int matnr = 0;
#endif

#if NBINDING==OVERALL && !defined(_WIN32)
  if (normals);
#endif
#if MBINDING==OVERALL && !defined(_WIN32)
  if (materials);
#endif

  while (viptr < viendptr) {
    v1 = *viptr++;
    v2 = *viptr++;
    v3 = *viptr++;
#if 0
    if (v1 < 0 || v2 < 0 || v3 < 0) return;
#else
    assert(v1 >= 0 && v2 >= 0 && v3 >= 0);
#endif
    v4 = *viptr++;
    if (v4  < 0) newmode = GL_TRIANGLES;
    else {
      v5 = *viptr++;
      if (v5 < 0) newmode = GL_QUADS;
      else newmode = GL_POLYGON;
    }
    if (newmode != mode) {
      if (mode != GL_POLYGON) glEnd();
      mode = newmode;
      glBegin((GLenum) mode);
    }
    else if (mode == GL_POLYGON) glBegin(GL_POLYGON);

    /* vertex 1 *********************************************************/
#if MBINDING==PER_VERTEX || MBINDING==PER_FACE
    materials->send(matnr++, TRUE);
#elif MBINDING==PER_VERTEX_INDEXED || MBINDING==PER_FACE_INDEXED
    materials->send(*matindices++, TRUE);
#endif
#if NBINDING==PER_VERTEX || NBINDING==PER_FACE
    currnormal = normals++;
    glNormal3fv((const GLfloat*)currnormal);
#elif NBINDING==PER_VERTEX_INDEXED || NBINDING==PER_FACE_INDEXED
    currnormal = &normals[*normalindices++];
    glNormal3fv((const GLfloat*)currnormal);
#endif
#if TEXTURES==TRUE
    texcoords->send(texindices ? *texindices++ : texidx++,
                    vertexlist->get3(v1),
                    *currnormal);
#endif
    vertexlist->send(v1);

    /* vertex 2 *********************************************************/
#if MBINDING==PER_VERTEX
    materials->send(matnr++, TRUE);
#elif MBINDING==PER_VERTEX_INDEXED
    materials->send(*matindices++, TRUE);
#endif
#if NBINDING==PER_VERTEX
    currnormal = normals++;
    glNormal3fv((const GLfloat*)currnormal);
#elif NBINDING==PER_VERTEX_INDEXED
    currnormal = &normals[*normalindices++];
    glNormal3fv((const GLfloat*)currnormal);
#endif
#if TEXTURES==TRUE
    texcoords->send(texindices ? *texindices++ : texidx++,
                    vertexlist->get3(v2),
                    *currnormal);
#endif
    vertexlist->send(v2);

    /* vertex 3 *********************************************************/
#if MBINDING==PER_VERTEX
    materials->send(matnr++, TRUE);
#elif MBINDING==PER_VERTEX_INDEXED
    materials->send(*matindices++, TRUE);
#endif
#if NBINDING==PER_VERTEX
    currnormal = normals++;
    glNormal3fv((const GLfloat*)currnormal);
#elif NBINDING==PER_VERTEX_INDEXED
    currnormal = &normals[*normalindices++];
    glNormal3fv((const GLfloat*)currnormal);
#endif
#if TEXTURES==TRUE
    texcoords->send(texindices ? *texindices++ : texidx++,
                    vertexlist->get3(v3),
                    *currnormal);
#endif
    vertexlist->send(v3);

    if (mode != GL_TRIANGLES) {
      /* vertex 4 (quad or polygon)**************************************/
#if MBINDING==PER_VERTEX
      materials->send(matnr++, TRUE);
#elif MBINDING==PER_VERTEX_INDEXED
      materials->send(*matindices++, TRUE);
#endif
#if NBINDING==PER_VERTEX
      currnormal = normals++;
      glNormal3fv((const GLfloat*)currnormal);
#elif NBINDING==PER_VERTEX_INDEXED
      currnormal = &normals[*normalindices++];
      glNormal3fv((const GLfloat*)currnormal);
#endif
#if TEXTURES==TRUE
      texcoords->send(texindices ? *texindices++ : texidx++,
                      vertexlist->get3(v4),
                      *currnormal);
#endif
      vertexlist->send(v4);
      if (mode == GL_POLYGON) {
      /* vertex 5 (polygon) ********************************************/
#if MBINDING==PER_VERTEX
        materials->send(matnr++, TRUE);
#elif MBINDING==PER_VERTEX_INDEXED
        materials->send(*matindices++, TRUE);
#endif
#if NBINDING==PER_VERTEX
        currnormal = normals++;
        glNormal3fv((const GLfloat*)currnormal);
#elif NBINDING==PER_VERTEX_INDEXED
        currnormal = &normals[*normalindices++];
        glNormal3fv((const GLfloat*)currnormal);
#endif
#if TEXTURES==TRUE
        texcoords->send(texindices ? *texindices++ : texidx++,
                        vertexlist->get3(v5),
                        *currnormal);
#endif
        vertexlist->send(v5);
        v1 = *viptr++;
        while (v1 >= 0) {
      /* vertex 6-n (polygon) *****************************************/
#if MBINDING==PER_VERTEX
          materials->send(matnr++, TRUE);
#elif MBINDING==PER_VERTEX_INDEXED
          materials->send(*matindices++, TRUE);
#endif
#if NBINDING==PER_VERTEX
          currnormal = normals++;
          glNormal3fv((const GLfloat*)currnormal);
#elif NBINDING==PER_VERTEX_INDEXED
          currnormal = &normals[*normalindices++];
          glNormal3fv((const GLfloat*)currnormal);
#endif
#if TEXTURES==TRUE
          texcoords->send(texindices ? *texindices++ : texidx++,
                          vertexlist->get3(v1),
                          *currnormal);
#endif
          vertexlist->send(v1);
          v1 = *viptr++;
        }
        glEnd(); /* draw polygon */
      }
    }
#if MBINDING==PER_VERTEX_INDEXED
    matindices++;
#endif
#if NBINDING==PER_VERTEX_INDEXED
    normalindices++;
#endif
#if TEXTURES==TRUE
    if (texindices) texindices++;
#endif
  }
  // check if triangle or quad
  if (mode != GL_POLYGON) glEnd();
}
#undef MINDEX
#undef NINDEX
