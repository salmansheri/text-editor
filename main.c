#include <GL/glx.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <stdio.h>
Window win;
GLXContext gl_context;
Display *display;
void create_gl_context() {
  int screen_id = DefaultScreen(display);
  int attributes[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};
  XVisualInfo *visual_info = glXChooseVisual(display, screen_id, attributes);
  if (!visual_info) {
    fprintf(stderr, "No Suitable OpenGl Visual found");
    return;
  }
  gl_context = glXCreateContext(display, visual_info, NULL, GL_TRUE);
  if (!gl_context) {
    fprintf(stderr, "OpenGL Context cannot be created ");
  }
}
int main(void) {
  display = XOpenDisplay(NULL);
  win = XCreateSimpleWindow(display, DefaultRootWindow(display), 0, 0, 1280,
                            720, 0, 0, 0);
  XSelectInput(display, win, ExposureMask);
  XMapWindow(display, win);
  Atom atom_delete_window = XInternAtom(display, "WM_DELETE_WINDOW", True);
  XSetWMProtocols(display, win, &atom_delete_window, 1);

  XFlush(display);

  create_gl_context();
  glXMakeCurrent(display, win, gl_context);

  XEvent ev;
  while (1) {
    XNextEvent(display, &ev);
    if (ev.type == ClientMessage) {
      if (ev.xclient.data.l[0] == atom_delete_window) {
        break;
      }
    }
  }
  if (ev.type == Expose) {

    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(1.0f, 0.0f, 0.0f, 0.1f);
    glXSwapBuffers(display, win);
  }
  XDestroyWindow(display, win);
  XCloseDisplay(display);

  return 0;
}
