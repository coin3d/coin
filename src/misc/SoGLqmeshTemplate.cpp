( const SoGLCoordinateElement * coords,
    const SbVec3f *normals,
    SoMaterialBundle * mb,
    const SoTextureCoordinateBundle * tb,
    SbBool needNormals,
    int rowsize,
    int colsize,
    int start )
{
#if MBINDING==PER_FACE || MBINDING==PER_ROW
  int midx = 0;
#endif
  SbVec3f dummynormal(0.0f, 0.0f, 1.0f);
  const SbVec3f * currnormal = &dummynormal;
  if (normals) currnormal = normals;

#if NBINDING==OVERALL
  if (needNormals) {
    glNormal3fv((const GLfloat *)currnormal);
  }
#endif
  int curridx; // for optimization only

  for (int i = 0; i < colsize-1; i++) {
    int j = 0;
    glBegin(GL_QUAD_STRIP);
#if NBINDING==PER_ROW
    currnormal = normals++;
    glNormal3fv((const GLfloat *)currnormal);
#endif
#if MBINDING==PER_ROW
    mb->send(midx++,TRUE);
#endif
    
    for (j = 0; j < rowsize; j++) {
      curridx = IDX(i,j);
#if NBINDING==PER_VERTEX
      currnormal = &normals[curridx];
      glNormal3fv((const GLfloat *)currnormal);
#endif
#if NBINDING==PER_FACE
      currnormal = normals++;
      glNormal3fv((const GLfloat *)currnormal);
#endif
#if MBINDING==PER_VERTEX
      mb->send(curridx, TRUE);
#endif
      
#if TEXTURES==TRUE
      tb->send(curridx, coords->get3(start + curridx),
               *currnormal);
#endif
      coords->send(start + curridx);
      curridx = IDX(i+1,j);
#if NBINDING==PER_VERTEX
      currnormal = &normals[curridx];
      glNormal3fv((const GLfloat *)currnormal);
#endif
#if MBINDING==PER_VERTEX
      mb->send(curridx, TRUE);
#endif
#if TEXTURES==TRUE
      tb->send(curridx, coords->get3(start + curridx), *currnormal);
#endif
#if MBINDING==PER_FACE
      // FIXME: optimize by moving first 2 vertices in row outside loop
      if (j > 0) mb->send(midx++, TRUE);
#endif
      coords->send(start + curridx);
    }
    glEnd(); // end of strip/row
  }
}
