( const SoGLCoordinateElement * coords,
    const SbVec3f *normals,
    SoMaterialBundle * mb,
    const SoTextureCoordinateBundle * tb,
    int nbind,         // skei toto: might be removed, not used... ?
    int mbind,         // --"--
    int doTextures,    // --"--
    int32_t idx,
    const int32_t *ptr,
    const int32_t *end,
    SbBool needNormals)
{
#if MBINDING!=OVERALL
  int matnr = 0;
#endif
#if TEXTURES==TRUE
  int texnr = 0;
#endif
  int n;

  SbVec3f dummynormal(0.0f, 0.0f, 1.0f);
  const SbVec3f * currnormal = &dummynormal;
  if (normals) currnormal = normals;
#if NBINDING==OVERALL
  if (needNormals) glNormal3fv((const GLfloat *)currnormal);
#endif

  while (ptr < end) {
    n = *ptr++ - 2;
    assert(n > 0);

    glBegin(GL_TRIANGLE_STRIP);

#if NBINDING==PER_VERTEX || NBINDING==PER_STRIP
      currnormal = normals++;
      glNormal3fv((const GLfloat *)currnormal);
#endif
#if MBINDING==PER_STRIP || MBINDING==PER_VERTEX
      mb->send(matnr++, TRUE);
#endif
#if TEXTURES==TRUE
      tb->send(texnr++, coords->get3(idx), *currnormal);
#endif
    coords->send(idx++);

#if NBINDING==PER_VERTEX
      currnormal = normals++;
      glNormal3fv((const GLfloat *)currnormal);
#endif
#if MBINDING==PER_VERTEX
      mb->send(matnr++, TRUE);
#endif
#if TEXTURES==TRUE
      tb->send(texnr++, coords->get3(idx), *currnormal);
#endif
    coords->send(idx++);

    while (n--) {
#if NBINDING==PER_FACE || NBINDING==PER_VERTEX
        currnormal = normals++;
        glNormal3fv((const GLfloat *)currnormal);
#endif
#if MBINDING==PER_FACE || MBINDING==PER_VERTEX
        mb->send(matnr++, TRUE);
#endif
#if TEXTURES==TRUE
        tb->send(texnr++, coords->get3(idx), *currnormal);
#endif
      coords->send(idx++);
    }
    glEnd();
  }
}
