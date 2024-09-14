#include <X11/X.h>
#include <X11/Xlib.h>
#include <stdio.h>
Window win;
int main(void) {
  Display *display = XOpenDisplay(NULL);
  win = XCreateSimpleWindow(display, DefaultRootWindow(display), 0, 0, 1280,
                            720, 0, 0, 0);
  XMapWindow(display, win);
  Atom atom_delete_window = XInternAtom(display, "WM_DELETE_WINDOW", True);
  XSetWMProtocols(display, win, &atom_delete_window, 1);

  XFlush(display);

  XEvent ev;
  while (1) {
    XNextEvent(display, &ev);
    if (ev.type == ClientMessage) {
      if (ev.xclient.data.l[0] == atom_delete_window) {
        break;
      }
    }
  }
  XDestroyWindow(display, win);
  XCloseDisplay(display);

  return 0;
}
