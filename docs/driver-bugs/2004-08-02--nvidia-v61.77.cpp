/*
  The simple example below causes a WinXP machine with NVIDIA 61.77 to
  crash within the driver. The example code just compiles a GL display
  list with a few triangles, then invokes this list. This causes a
  crash.

  Not yet reported to NVIDIA, asking for confirmation from other
  systems, on the nvnews.net forums, aswell as from HitecO (who
  originally found the bug).

  System information:

  * Driver version 61.77 (also found same problem with 61.76 and
    56.64).

  * GeForce FX 5900

  * Win XP Service Pack 1.

  * Display settings: any resolution, any color depth.

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
  const float v[3][3] = { { 0, -1, 0 }, { 1, -1, 0 }, { 1, 0, 0 } };

  for (int j=0; j < 3; j++) {
    glNormal3f(0, 0, 1);
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
    glEnable(GL_LIGHTING);

    // *********************************************************************

    GLuint gllist = glGenLists(1);
    // Change to GL_COMPILE, and the crashes go away:
    glNewList(gllist,GL_COMPILE_AND_EXECUTE);

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
    glDeleteLists(gllist, 1);
#endif
    printf("gllist==%d\n", gllist);
  }
}

// *************************************************************************

int
main(int argc, char ** argv)
{
  srand(argc > 1 ? atoi(argv[1]) : 0);

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

  glutInitWindowSize(892, 658);
  (void)glutCreateWindow("hepp");
  glutDisplayFunc(expose_cb);

  glutMainLoop();

  return 0;
}

// *************************************************************************
