( const SoGLCoordinateElement * coords,
    const SbVec3f *normals,
    SoMaterialBundle * mb,
    const SoTextureCoordinateBundle * tb,
    int nbind,        // skei todo: remove this, not used..
    int mbind,        // --"--
    int doTextures,   // --"--
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
  int mode = GL_POLYGON;
  int newmode;
  int n;

  SbVec3f dummynormal(0.0f, 0.0f, 1.0f);
  const SbVec3f * currnormal = &dummynormal;
  if (normals) currnormal = normals;
#if NBINDING==OVERALL
  if (needNormals)  glNormal3fv((const GLfloat *)currnormal);
#endif

  while (ptr < end) {

    n = *ptr++;
    if (n == 3) newmode = GL_TRIANGLES;
    else if (n == 4) newmode = GL_QUADS;
    else newmode = GL_POLYGON;
    if (newmode != mode) {
      if (mode != GL_POLYGON) glEnd();
      mode = newmode;
      glBegin((GLenum) mode);
    }
    else if (mode == GL_POLYGON) glBegin(GL_POLYGON);

#if NBINDING!=OVERALL
    currnormal = normals++;
    glNormal3fv((const GLfloat *)currnormal);
#endif
#if MBINDING!=OVERALL
    mb->send(matnr++, TRUE);
#endif
#if TEXTURES==TRUE
    tb->send(texnr++, coords->get3(idx), *currnormal);
#endif
    coords->send(idx++);
    while (--n) {
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
    }
    if (mode == GL_POLYGON) glEnd();
  }
  if (mode != GL_POLYGON) glEnd();
}
