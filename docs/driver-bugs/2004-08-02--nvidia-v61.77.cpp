/*
  The simple example below causes a WinXP machine with NVIDIA 61.77 to
  crash within the driver. The example code just compiles a GL display
  list with a few textures and a few triangles, then invokes this
  list. This causes a crash.

  Not yet reported to NVIDIA, asking for confirmation from other
  systems, on the nvnews.net forums, aswell as from HitecO (who
  originally found the bug).

  System information:

  * Driver version 61.77 (also found same problem with 61.76 and
    56.64).

  * GeForce FX 5900

  * Win XP Service Pack 1.

  * Display settings: 1280x1024, 32 bit.  Notes: this specific example
    does *not* crash when color quality is set to 16 bit, but an
    extended example I have will still crash. Changing the display
    resolution does not make any difference, it seems.

  <mortene@sim.no>.
*/

// *************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <windows.h>
#include <winbase.h>

#include <GL/glut.h>

#ifndef GL_TEXTURE_3D
#define GL_TEXTURE_3D 0x806F
#endif

static int winid;

static float
normrand(void)
{
  return (float)rand() / RAND_MAX;
}

static void
send_triangle(void)
{
  const float t[3][2] = { { 0, 0 }, { 1, 0 }, { 1, 1 } };
  const float v[3][3] = { { 0, -1, 0 }, { 1, -1, 0 }, { 1, 0, 0 } };

  for (int j=0; j < 3; j++) {
    glNormal3f(0, 0, 1);
    glTexCoord2f(t[j][0], t[j][1]);
    glVertex3f(v[j][0], v[j][1], v[j][2]);
  }
}

static void
expose_cb(void)
{
  int i;

  glEnable(GL_DEPTH_TEST);
  glDrawBuffer(GL_BACK);
  glClearColor(0.000000,0.000000,0.000000,0.000000);

  glColorMaterial(GL_FRONT_AND_BACK,GL_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_NORMALIZE);
  glViewport(0,0,892,658);
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  glEnable(GL_LIGHT0);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_LIGHTING);

  GLuint textures[2];
  glGenTextures(2,textures);

  glBindTexture(GL_TEXTURE_2D,textures[0]);

  static GLvoid * pixels = NULL;
  if (!pixels) {
    pixels = malloc(256*256*4);
    (void)memset(pixels, 0x55, 256*256*4);
  }
  glTexImage2D(GL_TEXTURE_2D,0,0x0003,2,2,0,GL_RGB,GL_UNSIGNED_BYTE,pixels);
  glTexImage2D(GL_TEXTURE_2D,1,0x0003,1,1,0,GL_RGB,GL_UNSIGNED_BYTE,pixels);

  glBindTexture(GL_TEXTURE_2D,textures[0]);
  glDisable(GL_ALPHA_TEST);

  glBegin(GL_TRIANGLES);
  glColor4ub(255,255,0,255);
  send_triangle();
  glEnd();

  glBindTexture(GL_TEXTURE_2D,textures[1]);

  int dim = 256;
  int level = 0;
  while (dim > 0) {
    glTexImage2D(GL_TEXTURE_2D,level++,0x0003,dim,dim,
		 0,GL_RGB,GL_UNSIGNED_BYTE,pixels);
    dim >>= 1;
  }

  // *************************************************************************

  GLuint gllist = glGenLists(1);
  glNewList(gllist,GL_COMPILE_AND_EXECUTE);

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D,textures[1]);

  glBegin(GL_TRIANGLES);
  send_triangle();
  glEnd();

  glBindTexture(GL_TEXTURE_2D,textures[1]);
  glBegin(GL_TRIANGLES);
  send_triangle();
  glEnd();

  glColor4ub(0,0,255,255);
  glBindTexture(GL_TEXTURE_2D,textures[1]);

  glBegin(GL_TRIANGLES);
  glColor4ub(255,255,0,255);
  send_triangle();
  glEnd();

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D,textures[1]);

  glColor4ub(255,0,255,255);

  glBegin(GL_TRIANGLES);
  for (i=0; i < 16; i++) { send_triangle(); }
  glColor4f(normrand(), normrand(), normrand(), 1);
  for (i=0; i < 2; i++) { send_triangle(); }
  glEnd();

  glEndList();

  // *************************************************************************

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

  glCallList(gllist);

  glutSwapBuffers();
}

// *************************************************************************

int
main(int argc, char ** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

  glutInitWindowSize(892, 658);
  winid = glutCreateWindow("hepp");
  glutDisplayFunc(expose_cb);

  glutMainLoop();

  return 0;
}

// *************************************************************************
