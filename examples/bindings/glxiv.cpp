// *************************************************************************

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/gl.h>
#include <GL/glx.h>

// *************************************************************************

typedef struct {
  Display * display;
  Window window;
  GLXContext context;
} WindowData;

// *************************************************************************

static void
draw_scene(WindowData * win)
{
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glBegin(GL_POLYGON);
  glColor3f(1.0, 0.0, 0.0);
  glVertex3f(0.0, 0.0, 0.0);

  glColor3f(0.0, 1.0, 0.0);
  glVertex3f(0.5, 0.0, 0.0);

  glColor3f(0.0, 0.0, 1.0);
  glVertex3f(0.5, 0.5, 0.0);
  glEnd();

  glXSwapBuffers(win->display, win->window);
}

static void
make_window(WindowData * win,
	    int x, int y, unsigned int width, unsigned int height)
{
  int attrib[] = {
    GLX_RGBA,
    GLX_RED_SIZE, 1,
    GLX_GREEN_SIZE, 1,
    GLX_BLUE_SIZE, 1,
    GLX_DOUBLEBUFFER,
    None
  };


  const int scrnum = DefaultScreen(win->dpy);
  const Window rootwindow = RootWindow(win->dpy, scrnum);

  XVisualInfo * visinfo = glXChooseVisual(win->dpy, scrnum, attrib);
  if (!visinfo) {
    (void)fprintf(stderr, "Error: couldn't get an RGB, double-buffered visual.\n");
    exit(1);
  }

  XSetWindowAttributes attr;
  attr.background_pixel = 0;
  attr.border_pixel = 0;
  attr.colormap = XCreateColormap(win->dpy, rootwindow, visinfo->visual, AllocNone);
  attr.event_mask = StructureNotifyMask | ExposureMask | KeyPressMask;
  unsigned long mask = CWBackPixel | CWBorderPixel | CWColormap | CWEventMask;

  win->window = XCreateWindow(win->dpy, rootwindow, 0, 0, width, height,
			      0, visinfo->depth, InputOutput,
			      visinfo->visual, mask, &attr);
  
  {
    XSizeHints sizehints;
    sizehints.x = x;
    sizehints.y = y;
    sizehints.width  = width;
    sizehints.height = height;
    sizehints.flags = USSize | USPosition;
    XSetNormalHints(win->dpy, win->window, &sizehints);
    const char * name = "Coin in GLX";
    XSetStandardProperties(win->dpy, win->window, name, name,
                           None, (char **)NULL, 0, &sizehints);
  }

  win->context = glXCreateContext(win->dpy, visinfo, NULL, True);
  if (!win->context) {
    (void)fprintf(stderr, "Error: glXCreateContext() failed.\n");
    exit(1);
  }

  XFree(visinfo);
}

// *************************************************************************

static void
event_loop(WindowData *win)
{
  while (1) {
    static long mask = StructureNotifyMask | ExposureMask | KeyPressMask;
    XEvent event;
    while (XCheckWindowEvent(win->display, win->window, mask, &event)) {
      if (event.xany.window == win->window) {
        switch (event.type) {

        case Expose:
          draw_scene(win);
          break;

        case ConfigureNotify:
          glViewport(0, 0,
                     event.xconfigure.width, event.xconfigure.height);
          break;

        case KeyPress:
          {
            char buffer[10];
            int r;
            r = XLookupString(&event.xkey, buffer, sizeof(buffer),
                              NULL, NULL);
            if (buffer[0] == 27) {
              /* escape */
              return;
            }
          }
	  break;
        }
      }
    }
  }
}

// *************************************************************************

int
main(int argc, char *argv[])
{
  Display * dpy = XOpenDisplay(NULL);
  if (!dpy) {
    (void)fprintf(stderr, "Error: couldn't open default display.\n");
    exit(1);
  }

  WindowData win = { dpy, NULL, NULL };
  make_window(&win, 100, 100, 400, 400);

  XMapWindow(win.dpy, win.window);
  glXMakeCurrent(win.dpy, win.window, win.context);

  event_loop(&win);

  glXDestroyContext(win.dpy, win.context);
  XDestroyWindow(win.dpy, win.window);
  XCloseDisplay(win.dpy);

  return 0;
}

// *************************************************************************
