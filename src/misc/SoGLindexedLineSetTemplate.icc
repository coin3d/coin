// skei... 2000.01.10 - phew :-)


( const SoGLCoordinateElement * coords,
  const int32_t *indices,
  int num_vertexindices,
  const SbVec3f *normals,
  const int32_t *normindices,
  SoMaterialBundle *const materials,
  const int32_t *matindices,
  const SoTextureCoordinateBundle * const texcoords,
  const int32_t *texindices,
  const int drawAsPoints)
{

#if MBINDING==PER_VERTEX_INDEXED
  if (matindices == NULL) matindices = indices;
#endif
#if NBINDING==PER_VERTEX_INDEXED
  if (normindices == NULL) normindices = indices;
#endif

#if MBINDING==PER_VERTEX || MBINDING==PER_LINE || MBINDING==PER_SEGMENT
  int matnr = 0;
#endif
#if TEXTURES==TRUE
  int texidx = 0;
#endif
  int32_t i;
  const int32_t *end = indices + num_vertexindices;

  SbVec3f dummynormal(0.0f, 0.0f, 1.0f);
  const SbVec3f *currnormal = &dummynormal;
  if (normals) currnormal = normals;
#if NBINDING==OVERALL
  glNormal3fv((const GLfloat*)currnormal);
#endif

#if MBINDING==PER_SEGMENT || MBINDING==PER_SEGMENT_INDEXED || NBINDING==PER_SEGMENT || NBINDING==PER_SEGMENT_INDEXED

  int previ;

  if (drawAsPoints)
    glBegin(GL_POINTS);
  else
    glBegin(GL_LINES);

  while (indices < end) {
    previ = *indices++;

#if MBINDING==PER_LINE || MBINDING==PER_VERTEX
    materials->send(matnr++, TRUE);
#elif MBINDING==PER_LINE_INDEXED || MBINDING==PER_VERTEX_INDEXED
    materials->send(*matindices++, TRUE);
#endif

#if NBINDING==PER_LINE || NBINDING==PER_VERTEX
    currnormal = normals++;
    glNormal3fv((const GLfloat*) currnormal);
#elif NBINDING==PER_LINE_INDEXED || NBINDING==PER_VERTEX_INDEXED
    currnormal = &normals[*normindices++];
    glNormal3fv((const GLfloat*) currnormal);
#endif
#if TEXTURES==TRUE
    texcoords->send(texindices ? *texindices++ : texidx++,coords->get3(previ), *currnormal);
#endif
    i = *indices++;
    while (i >= 0) {
#if MBINDING==PER_SEGMENT
      materials->send(matnr++, TRUE);
#elif MBINDING==PER_SEGMENT_INDEXED
      materials->send(*matindices++, TRUE);
#endif

#if NBINDING==PER_SEGMENT
      currnormal = normals++;
      glNormal3fv((const GLfloat*) currnormal);
#elif NBINDING==PER_SEGMENT_INDEXED
      currnormal = &normals[*normindices++];
      glNormal3fv((const GLfloat*)currnormal);
#endif
      coords->send(previ);
#if MBINDING==PER_VERTEX
      materials->send(matnr++, TRUE);
#elif MBINDING==PER_VERTEX_INDEXED
      materials->send(*matindices++, TRUE);
#endif
#if NBINDING==PER_VERTEX
      currnormal = normals++;
      glNormal3fv((const GLfloat*)currnormal);
#elif NBINDING==PER_VERTEX_INDEXED
      currnormal = &normals[*normindices++];
      glNormal3fv((const GLfloat*)currnormal);
#endif
#if TEXTURES==TRUE
      texcoords->send(texindices ? *texindices++ : texidx++, coords->get3(i), *currnormal);
#endif
      coords->send(i);
      previ = i;
      i = *indices++;
    }
#if MBINDING==PER_VERTEX_INDEXED
    matindices++;
#endif
#if NBINDING==PER_VERTEX_INDEXED
    normindices++;
#endif
#if TEXTURES==TRUE
    if (texindices) texindices++;
#endif
  }
  glEnd();

#else // no per_segment binding code below

  if (drawAsPoints)
    glBegin(GL_POINTS);

  while (indices < end) {
    if (!drawAsPoints)
      glBegin(GL_LINE_STRIP);

    i = *indices++;
    assert(i >= 0);
#if MBINDING==PER_VERTEX_INDEXED || MBINDING==PER_LINE_INDEXED
    materials->send(*matindices++, TRUE);
#elif MBINDING==PER_VERTEX || MBINDING==PER_LINE
    materials->send(matnr++, TRUE);
#endif
#if NBINDING==PER_VERTEX_INDEXED || NBINDING==PER_LINE_INDEXED
    currnormal = &normals[*normindices++];
    glNormal3fv((const GLfloat*) currnormal);
#elif NBINDING==PER_VERTEX || NBINDING==PER_LINE
    currnormal = normals++;
    glNormal3fv((const GLfloat*) currnormal);
#endif
#if TEXTURES==TRUE
    texcoords->send(texindices ? *texindices++ : texidx++, coords->get3(i), *currnormal);
#endif
    coords->send(i);
    i = *indices++;
    while (i >= 0) {
#if MBINDING==PER_VERTEX
      materials->send(matnr++, TRUE);
#elif MBINDING==PER_VERTEX_INDEXED
      materials->send(*matindices++, TRUE);
#endif
#if NBINDING==PER_VERTEX
      currnormal = normals++;
      glNormal3fv((const GLfloat*) currnormal);
#elif NBINDING==PER_VERTEX_INDEXED
      currnormal = &normals[*normindices++];
      glNormal3fv((const GLfloat*) currnormal);
#endif
#if TEXTURES==TRUE
      texcoords->send(texindices ? *texindices++ : texidx++, coords->get3(i), *currnormal);
#endif
      coords->send(i);
      i = *indices++;
    }
    if (!drawAsPoints)
      glEnd(); // end of line strip
#if MBINDING==PER_VERTEX_INDEXED
    matindices++;
#endif
#if NBINDING==PER_VERTEX_INDEXED
    normindices++;
#endif
#if TEXTURES==TRUE
    if (texindices) texindices++;
#endif
  }
  if (drawAsPoints)
    glEnd();
#endif
}
