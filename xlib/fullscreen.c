#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    Display *display;
    Window window;
    XEvent event;
    int screen;

    display = XOpenDisplay(NULL);
    if (display == NULL) {
	fprintf(stderr, "Cannot open display\n");
	exit(1);
    }

    screen = DefaultScreen(display);

    window = XCreateSimpleWindow(display, RootWindow(display, screen),
				 0, 0,
				 DisplayWidth(display, screen),
				 DisplayHeight(display, screen),
				 0,
				 BlackPixel(display, screen),
				 WhitePixel(display, screen));

    XSelectInput(display, window, ExposureMask | KeyPressMask);

    XSetWindowAttributes attributes;
    attributes.override_redirect = True;
    XChangeWindowAttributes(display, window, CWOverrideRedirect, &attributes);

    XMapWindow(display, window);

    while (1) {
	XNextEvent(display, &event);
	if (event.type == KeyPress) {
            // Get the keysym of the pressed key
            KeySym keysym = XLookupKeysym(&event.xkey, 0);
            if (keysym == XK_q) {  // Exit on 'q' key
                break;
            }
        }

    }

    XDestroyWindow(display, window);
    XCloseDisplay(display);

    return 0;
}
