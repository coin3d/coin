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
#include <time.h>
#include <limits.h>

#include <windows.h>
#include <winbase.h>

#include <GL/glut.h>

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
  for (int loop = 0; loop < 2048; loop++) {
    printf("loop==%d\n", loop);

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

    GLuint textures[1];
    glGenTextures(1,textures);

    static GLvoid * pixels = NULL;
    if (!pixels) {
      pixels = malloc(256*256*4);
      (void)memset(pixels, 0x55, 256*256*4);
    }

    glBindTexture(GL_TEXTURE_2D,textures[0]);
    glTexImage2D(GL_TEXTURE_2D,0,0x0003,256,256,
		 0,GL_RGB,GL_UNSIGNED_BYTE,pixels);

    // *********************************************************************

    GLuint gllist = glGenLists(1);
    glNewList(gllist,GL_COMPILE_AND_EXECUTE);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,textures[0]);

    glColor4f(0, 1, 0, 1);

    glBegin(GL_TRIANGLES);
    const int nr = ((float)rand() / RAND_MAX) * 32;
    printf("nr==%d\n", nr);
    for (i=0; i < nr; i++) { send_triangle(); }
    glColor4f(1, 0, 0, 1);
    send_triangle();
    glEnd();

    glEndList();

    // *********************************************************************

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glCallList(gllist);

    glutSwapBuffers();

#if 0 // enable this, and the crashes go away
    glDeleteTextures(2, textures);
    glDeleteLists(gllist, 1);
#endif
    printf("textures[0]==%d, gllist==%d\n", textures[0], gllist);
  }
}

// *************************************************************************

int
main(int argc, char ** argv)
{
  srand(atoi(argv[1]));

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

  glutInitWindowSize(892, 658);
  (void)glutCreateWindow("hepp");
  glutDisplayFunc(expose_cb);

  glutMainLoop();

  return 0;
}

// *************************************************************************
