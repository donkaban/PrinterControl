#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>

#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <vector>
#include <stdlib.h>

extern std::vector<std::string> get_ip();


static Display * display = NULL;
static Window    window;
static Window    root;    
static int       screen;
static Visual  * visual = NULL;
static GC gc;  
static bool done = false;

#define FATAL(msg) {std::cout << "\nERROR : " << msg << std::endl; exit(-1);}

Bool waitMapping(Display *, XEvent *e, char *arg )
{
    return (e->type == MapNotify) && (e->xmap.window == (Window)arg);
}

void printText(uint x, uint y,const std::string & msg)
{
    XDrawString (display, window, gc, x,y,msg.c_str(), msg.size());
}


class button
{
    typedef void (*func)(); 
public:
    button(uint x, uint y, uint dx, uint dy, const std::string & label, const func & f):
        f(f),
        x1(x),
        x2(x+dx),
        y1(y),
        y2(y + dy)
    {
        XDrawRectangle(display,window,gc,x, y, dx, dy);
        printText(x+25 ,y+dy/2+7, label);
    }

    void check(uint x, uint y)
    {
        if(x > x1 && x < x2 && y > y1 && y < y2)
        {    
            XFillRectangle(display, window, gc, x1, y1, x2-x1, y2-y1);
            f();
        }
    }
private:
    func f;
    uint x1,x2,y1,y2;

};

static std::vector<button> buttons;

void b1() {system("./reboot_script.sh");done=true;}
void b2() {system("./reset_script.sh");done=true;}
void b3() {done = true;}


void init(uint w, uint h)
{
  
    XEvent event;
    display = XOpenDisplay(NULL);
    if(!display) FATAL("can't open X11 display");
    
    root   = XDefaultRootWindow(display);
    screen = XDefaultScreen(display);
    visual = XDefaultVisual(display, screen);
    window = XCreateSimpleWindow(display,root,0,0,w,h,0,0,0);
    XSetWindowAttributes attr; 
    std::memset(&attr,0,sizeof(attr));
    attr.event_mask = StructureNotifyMask|ButtonPressMask|ButtonReleaseMask|ButtonMotionMask;
    attr.override_redirect  = True;
    attr.background_pixel   = 0x99999999;
    XWithdrawWindow(display,window, screen);  
    XChangeWindowAttributes(display,window,CWBackPixel|CWOverrideRedirect|CWSaveUnder|CWEventMask|CWBorderPixel, &attr);
    gc = XCreateGC(display, window,0,NULL);
    XMapWindow(display,window);
    XIfEvent(display, &event, waitMapping, (char*)window );
    XSetInputFocus(display,window, RevertToNone, CurrentTime);
    XFontStruct *fontInfo;
    if((fontInfo =  XLoadQueryFont(display, "fixed" )) == NULL)
        FATAL("font not found")
    XSetFillStyle(display, gc, FillOpaqueStippled);
    XSetFont(display, gc, fontInfo->fid);
    XDrawLine(display,window,gc,20, 50, 575, 50);
    XDrawLine(display,window,gc,20, 425, 575, 425);
  
    printText(20,48,"SYNQERA PLATFORM CONTROL");
    std::vector<std::string> ips = get_ip();
    for(uint i = 0; i< ips.size(); i++)
        printText(20,100 + i *20, ips[i]);

    buttons.push_back(button(600,50,  150, 75, "REBOOT", b1));
    buttons.push_back(button(600,150, 150, 75, "RESET", b2));
    buttons.push_back(button(600,350, 150, 75, "QUIT", b3));
  
    XFlush(display);
}

void close()
{
    XDestroyWindow(display,window);
    if(visual) delete visual;
}

void poll()
{
        XEvent evt;
        while(XPending(display) > 0 )
        { 
            XNextEvent(display, &evt);
            switch (evt.type)
            {
                case ButtonPress:
                {
                    XButtonEvent * event = (XButtonEvent *) &evt;
                    for(uint i = 0; i < buttons.size(); i++) 
                        buttons[i].check(event->x, event->y);
                    break;
                }
                default:
                    break;
            }
        }
    }



int main()
{
    init(800,480);
    while(!done)
        poll();
    close();
    return 0;
}
