/**************************************************************************\
 * 
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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

#if NBINDING==PER_VERTEX_INDEXED || NBINDING==PER_TRIANGLE_INDEXED || NBINDING==PER_STRIP_INDEXED
#define NINDEX TRUE
#else
#define NINDEX FALSE
#endif

#if MBINDING==PER_VERTEX_INDEXED || MBINDING==PER_TRIANGLE_INDEXED || MBINDING==PER_STRIP_INDEXED
#define MINDEX TRUE
#else
#define MINDEX FALSE
#endif
(
   const SoGLCoordinateElement * const vertexlist,
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
#if NINDEX==FALSE
  if (normalindices);
#endif
#if MINDEX==FALSE
  if (matindices);
#endif
#if TEXTURES==FALSE
  if (texcoords && texindices);
#else
  int texidx = 0;
#endif
  const int32_t *viptr = vertexindices;
  const int32_t *viendptr = viptr + numindices;
  int32_t v1, v2, v3;
  SbVec3f dummynormal(0.0f, 0.0f, 1.0f);
  const SbVec3f *currnormal = &dummynormal;
  if (normals) currnormal = normals;

#if MBINDING==PER_TRIANGLE || MBINDING==PER_STRIP || MBINDING==PER_VERTEX
  int matnr = 0;
#endif

#if NBINDING==OVERALL
  if (normals);
#endif
#if MBINDING==OVERALL
  if (materials);
#endif
  
  while ( viptr < viendptr ) {
    v1 = *viptr++;
    v2 = *viptr++;
    v3 = *viptr++;
    assert(v1 >= 0 && v2 >= 0 && v3 >= 0);

    glBegin(GL_TRIANGLE_STRIP); // move to right before glArrayElement()?
    
    /* vertex 1 *********************************************************/
#if MBINDING==PER_VERTEX || MBINDING==PER_TRIANGLE || MBINDING==PER_STRIP
    materials->send(matnr++, TRUE);
#elif MBINDING==PER_VERTEX_INDEXED || MBINDING==PER_STRIP_INDEXED || MBINDING==PER_TRIANGLE_INDEXED
    materials->send(*matindices++, TRUE);
#endif
#if NBINDING==PER_VERTEX || NBINDING==PER_TRIANGLE || NBINDING==PER_STRIP
    currnormal = normals++;
    glNormal3fv((const GLfloat*)currnormal);
#elif NBINDING==PER_VERTEX_INDEXED || NBINDING==PER_TRIANGLE_INDEXED || NBINDING==PER_STRIP_INDEXED
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
    
    v1 = *viptr++;
    while (v1 >= 0) {
#if MBINDING==PER_VERTEX || MBINDING==PER_TRIANGLE
      materials->send(matnr++, TRUE);
#elif MBINDING==PER_VERTEX_INDEXED || MBINDING==PER_TRIANGLE_INDEXED
      materials->send(*matindices++, TRUE);
#endif
#if NBINDING==PER_VERTEX || NBINDING==PER_TRIANGLE
      currnormal = normals++;
      glNormal3fv((const GLfloat*)currnormal);
#elif NBINDING==PER_VERTEX_INDEXED || NBINDING==PER_TRIANGLE_INDEXED
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
    glEnd(); // end of tristrip

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
}

#undef MINDEX
#undef NINDEX
