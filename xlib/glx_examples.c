/*
  gcc -o glx_example glx_example.c -lX11 -lGL
  ./glx_example
*/

#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    // 1. Open X display
    Display *display = XOpenDisplay(NULL);
    if (!display) {
	fprintf(stderr, "Cannot open display\n");
	return 1;
    }

    // 2. Choose GLX framebuffer config (visual)
    int screen = DefaultScreen(display);
    static int attribs[] = {
	GLX_RGBA, // Use RGBA mode
	GLX_DOUBLEBUFFER, // Double buffering
	GLX_RED_SIZE, 8, // 8 bits per color channel
	GLX_GREEN_SIZE, 8,
	GLX_BLUE_SIZE, 8,
	GLX_DEPTH_SIZE, 16, // Depth buffer
	None // End of list
    };

    XVisualInfo *visual = glXChooseVisual(display, screen, attribs);
    if (!visual) {
	fprintf(stderr, "No suitable GLX visual found\n");
	XCloseDisplay(display);
	return 1;
    }

    // 3. Create X window
    Window root = RootWindow(display, screen);
    Colormap cmap = XCreateColormap(display, root, visual->visual, AllocNone);
    XSetWindowAttributes swa;
    swa.colormap = cmap;
    swa.event_mask = ExposureMask | KeyPressMask;

    Window window = XCreateWindow(
	display, root, 0, 0, 800, 600, 0,
	visual->depth, InputOutput, visual->visual,
	CWColormap | CWEventMask, &swa
    );

    XMapWindow(display, window);
    XStoreName(display, window, "GLX OpenGL Example");

    // 4. Create GLX context
    GLXContext context = glXCreateContext(display, visual, NULL, True);
    if (!context) {
	fprintf(stderr, "Cannot create GLX context\n");
	XFree(visual);
	XCloseDisplay(display);
	return 1;
    }

    // 5. Make context current
    glXMakeCurrent(display, window, context);

    // 6. Basic OpenGL setup
    glViewport(0, 0, 800, 600);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    // Event loop
    int running = 1;
    while (running) {
	while (XPending(display)) {
	    XEvent event;
	    XNextEvent(display, &event);
	    switch (event.type) {
	    case Expose:
		glClear(GL_COLOR_BUFFER_BIT); // clear screen
		glBegin(GL_TRIANGLES);
		glColor3f(1.0f, 0.0f, 0.0f); glVertex2f(0.0f, 0.5f);
		glColor3f(0.0f, 1.0f, 0.0f); glVertex2f(-0.5f, -0.5f);
		glColor3f(0.0f, 0.0f, 1.0f); glVertex2f(0.5f, -0.5f);
		glEnd();
		glXSwapBuffers(display, window);
		break;
	    case KeyPress:
		running = 0;
		break;
	    }
	}
    }

    // cleanup
    glXMakeCurrent(display, None, NULL);
    glXDestroyContext(display, context);
    XDestroyWindow(display, window);
    XFreeColormap(display, cmap);
    XFree(visual);
    XCloseDisplay(display);

    return 0;
}
