( const SoGLCoordinateElement * coords,
  const SbVec3f *normals,
  SoMaterialBundle * mb,
  const SoTextureCoordinateBundle * tb,
  int nbind,
  int mbind,
  int doTextures,
  int32_t idx,
  const int32_t *ptr,
  const int32_t *end,
  SbBool needNormals,
  SbBool drawPoints)
{

  SbVec3f dummynormal(0.0f, 0.0f, 1.0f);
  const SbVec3f * currnormal = &dummynormal;
  if (normals) currnormal = normals;
#if NBINDING==OVERALL
  if (needNormals)
    glNormal3fv((const GLfloat *)currnormal);
#endif

#if MBINDING!=OVERALL
  int matnr = 0;
#endif
#if TEXTURES==TRUE
  int texnr = 0;
#endif

#if NBINDING==PER_SEGMENT || MBINDING==PER_SEGMENT

  if (drawPoints) glBegin(GL_POINTS);
  else glBegin(GL_LINES);

  while (ptr < end) {
    int n = *ptr++;
    if (n < 2) {
      idx += n;
      continue;
    }
#if MBINDING==PER_LINE || MBINDING==PER_VERTEX
    mb->send(matnr++, TRUE);
#endif
#if NBINDING==PER_LINE || NBINDING==PER_VERTEX
    currnormal = normals++;
    glNormal3fv((const GLfloat*)currnormal);
#endif
#if TEXTURES==TRUE
    tb->send(texnr++, coords->get3(idx), *currnormal);
#endif
    while (--n) {
#if MBINDING==PER_SEGMENT
      mb->send(matnr++, TRUE);
#endif
#if NBINDING==PER_SEGMENT
      currnormal = normals++;
      glNormal3fv((const GLfloat*)currnormal);
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
      coords->send(idx);
    }
    idx++;
  }
  glEnd();

#else // NBINDING==PER_SEGMENT || MBINDING==PER_SEGMENT

  if (drawPoints) glBegin(GL_POINTS);
  while (ptr < end) {
    int n = *ptr++;
    if (n < 2) {
      idx += n; // FIXME: is this correct?
      continue;
    }
    n -= 2;
    if (!drawPoints) glBegin(GL_LINE_STRIP);
#if NBINDING != OVERALL
    currnormal = normals++;
    glNormal3fv((const GLfloat *)currnormal);
#endif
#if MBINDING != OVERALL
    mb->send(matnr++, TRUE);
#endif
#if TEXTURES==TRUE
    tb->send(texnr++, coords->get3(idx), *currnormal);
#endif
    coords->send(idx++);
    do {
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
    } while (n--);
    if (!drawPoints) glEnd();
  }

#endif // NBINDING==PER_SEGMENT || MBINDING==PER_SEGMENT

  if (drawPoints) glEnd();
}
