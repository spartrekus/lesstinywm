/* Modification of TinyWM 
 which was  written by Nick Welch <mack@incise.org>, 2005.
 */

#include <stdio.h>
#include <stdlib.h>

#include <X11/Xlib.h>

// gives xk_x
#include <X11/keysym.h> 
// gives keycodetokeysym
#include <X11/XKBlib.h> 

#define MAX(a, b) ((a) > (b) ? (a) : (b))

int main()
{
    Display * dpy;
    Window root;
    XWindowAttributes attr;
    XButtonEvent start;

    XEvent ev;

    int winmove = 0;
    long winmove_a1;
    long winmove_a2;
    long winmove_a3;
    long winmove_a4;
    long borpixel = 15;
    int s; 
    Window dwin;
    Window dbor;


    if(!(dpy = XOpenDisplay(0x0))) return 1;
    root = DefaultRootWindow(dpy);

    Colormap screen_colormap; 
    XColor red, brown, blue, yellow , green; 
    Status rc; 
    screen_colormap = DefaultColormap( dpy, DefaultScreen( dpy ) );
    rc = XAllocNamedColor( dpy , screen_colormap, "blue", &blue, &blue );

    //XGrabKey(dpy, XKeysymToKeycode(dpy, XStringToKeysym("F1")), Mod1Mask, root, True, GrabModeAsync, GrabModeAsync);
    //XGrabKey(dpy, XKeysymToKeycode(dpy, XStringToKeysym("X")), Mod4Mask, root, True, GrabModeAsync, GrabModeAsync);
    //XGrabKey(dpy, XKeysymToKeycode(dpy, XStringToKeysym("W")), Mod4Mask, root, True, GrabModeAsync, GrabModeAsync);
    //XGrabKey(dpy, XKeysymToKeycode(dpy, XStringToKeysym("F1")), Mod1Mask, root, True, GrabModeAsync, GrabModeAsync);
    KeyCode keycode; 
    XGrabKey(dpy, keycode , Mod4Mask, root, True, GrabModeAsync, GrabModeAsync);
    XGrabButton(dpy, 1, Mod1Mask, root, True, ButtonPressMask, GrabModeAsync, GrabModeAsync, None, None);
    XGrabButton(dpy, 3, Mod1Mask, root, True, ButtonPressMask, GrabModeAsync, GrabModeAsync, None, None);


    dwin = XCreateSimpleWindow( dpy, RootWindow( dpy, s ), 1, 1, 1, 1, 1 , BlackPixel( dpy , s ) , WhitePixel( dpy , s ) );
    XMapWindow( dpy, dwin );  // trick to make a rectange - to be fixe

    dbor = XCreateSimpleWindow( dpy, RootWindow( dpy, s ), 1, 1, 1, 1, 1 , BlackPixel( dpy , s ) , blue.pixel );
    XMapWindow( dpy, dbor );

    for(;;)
    {
        XNextEvent(dpy, &ev);

        if(ev.type == KeyPress && ev.xkey.subwindow != None)
	{
            // XRaiseWindow(dpy, ev.xkey.subwindow);
            //if( XKeysymToKeycode(dpy, XStringToKeysym("F1")) == XK_x ) system( " xterm &  " );
            //if( ev.xkey.keycode == XK_w ) system( " xterm -bg black -fg green  &  " );
	    int bof = 0;
	    if ( keycode ==  XKeysymToKeycode(dpy, XStringToKeysym("X")))   bof = 1;
	    switch( bof )
	    {
               case 1: 
	         system( " xterm & " );
		 break;
	    }
	}

        else if(ev.type == ButtonPress && ev.xbutton.subwindow != None)
        {
            XGrabPointer(dpy, ev.xbutton.subwindow, True, PointerMotionMask|ButtonReleaseMask, GrabModeAsync, GrabModeAsync, None, None, CurrentTime);
            XGetWindowAttributes(dpy, ev.xbutton.subwindow, &attr);
            start = ev.xbutton;
            XRaiseWindow(dpy, ev.xbutton.subwindow); // here when click 1
        }

        else if(ev.type == MotionNotify)
        {
            int xdiff, ydiff;
            XRaiseWindow(dpy, ev.xmotion.window );

            while(XCheckTypedEvent(dpy, MotionNotify, &ev));
            xdiff = ev.xbutton.x_root - start.x_root;
            ydiff = ev.xbutton.y_root - start.y_root;

            winmove = 1;

	    winmove_a1 = attr.x + (start.button==1 ? xdiff : 0);
	    winmove_a2 = attr.y + (start.button==1 ? ydiff : 0);
	    winmove_a3 = MAX(1, attr.width + (start.button==3 ? xdiff : 0));
	    winmove_a4 = MAX(1, attr.height + (start.button==3 ? ydiff : 0));

	    XMoveResizeWindow(dpy, dwin  , winmove_a1, winmove_a2, winmove_a3, winmove_a4);

           // XMoveResizeWindow(dpy, ev.xmotion.window, attr.x + (start.button==1 ? xdiff : 0), attr.y + (start.button==1 ? ydiff : 0), MAX(1, attr.width + (start.button==3 ? xdiff : 0)), MAX(1, attr.height + (start.button==3 ? ydiff : 0)));
	  /* XDrawRectangle(dpy, dwin , DefaultGC( dpy , s ), 
	     winmove_a1,
	     winmove_a2,
	     winmove_a3 - winmove_a1 , 
	     winmove_a4 - winmove_a2 ); */ 

        }

        else if(ev.type == ButtonRelease)
	{
           XUngrabPointer(dpy, CurrentTime);
           if ( winmove == 1 )
	   {
	      XMoveResizeWindow(dpy, ev.xmotion.window, winmove_a1, winmove_a2, winmove_a3, winmove_a4);
              XRaiseWindow(dpy, ev.xmotion.window );

	      XMoveResizeWindow(dpy, dbor , winmove_a1 , winmove_a2 - 1*borpixel-1,  winmove_a3 , 1 * borpixel );
              XRaiseWindow(dpy, dbor );

              XClearWindow( dpy, dwin );
	      winmove = 0;
	   }
	}

    }
}

