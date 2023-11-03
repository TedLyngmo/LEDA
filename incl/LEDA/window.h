/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  window.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/

#ifndef LEDA_WINDOW_H
#define LEDA_WINDOW_H

#include <LEDA/basic.h>
#include <LEDA/plane.h>
#include <LEDA/leda_window.h>
#include <LEDA/leda_panel.h>



/*{\Manpage {window} {} {Graphic Windows}}*/

class window : public LEDA_WINDOW {

/*{\Mdefinition
The data type $window$ provides an interface for the input and
output of basic two-dimensional geometric objects (cf.~section 5.1) using the
X11 window system.  Application programs using data type $window$ have to
be linked with {\em libWx.a} and the X11 library (cf.~section~1.6):

CC $prog.c$ -lP -lG -lL -lWx -lX11 -lm



An instance $W$ of the data type $window$ is an iso-oriented
rectangular window in the two-dimensional plane.
The default representation of $W$ on the screen is a square of maximal possible
edge length positioned in the upper right corner (cf.~creation, variant c)).
The coordinates and scaling of $W$ used for drawing operations are defined
by three double parameters:
$x_0$, the x-coordinate of the left side, $x_1$, the x-coordinate of the right
side, and $y_0$, the y-coordinate of the bottom side.
The y-coordinate of the top side of $W$ is determined by the current size and
shape of the window on the screen, which can be changed interactively.
A graphic window supports operations for drawing points, lines, segments,
arrows, circles, polygons, graphs, \dots and for graphical input of all these
objects using the mouse input device. Most of the drawing operations have an
optional color argument. Possible colors are $black$ (default), $white$,
$blue$, $green$, $red$, $violet$, and $orange$. On monochrome displays all
colors different from $white$ are turned to $black$. There are 6 parameters
used by the drawing operations:

\begin{enumerate}
\item
The {\sl line width} parameter (default value 1 pixel) defines the width of all
kinds of lines (segments, arrows, edges, circles, polygons).

\item
The {\sl line style} parameter defines the style of lines. Possible line
styles are $solid$ (default), $dashed$, and $dotted$.

\item
The {\sl node width} parameter (default value 10 pixels) defines the diameter
of nodes created by the draw\_node and draw\_filled\_node operations.

\item
The {\sl text mode} parameter defines how text is inserted into the window.
Possible values are $transparent$ (default)  and $opaque$.

\item
The {\sl drawing mode} parameter defines the logical operation that is used
for setting pixels in all drawing operations. Possible values are
$src\_mode$ (default) and $xor\_mode$. In $src\_mode$ pixels are set to
the respective color value, in $xor\_mode$ the value is bitwise added to the
current pixel value.

\item
The {\sl redraw function} parameter is used to redraw the entire window
whenever a redrawing is necessary, e.g., if the window shape on the screen
has  been changed. Its type is pointer to a void-function taking no arguments,
i.e., void (*F)();

\end{enumerate} }*/

public:

/*{\Mcreation W }*/ 

enum placement {min = -1, center =-2, max = -3 };


window(float width, float height, float xpos, float ypos, const char* = LEDA::version_string);

/*
window(float xpix, float ypix, float xpos, float ypos);
*/
/*{\Mcreate creates a window $W$ of physical size $xpix \times ypix$ pixels
    with its upper left corner at position ($xpos,ypos$) on the screen.}*/


window(float width, float height, const char* = LEDA::version_string);

/*
window(float xpix, float ypix);
*/
/*{\Mcreate creates a window $W$ of physical size $xpix \times ypix$ pixels
    positioned into the upper right corner of the screen.}*/


window(const char* = LEDA::version_string);

/*
window();
*/
/*{\Mcreate creates a maximal squared window $W$ positioned into the 
    upper right corner of the screen.}*/

/*{\Mtext

\medskip
All three variants initialize the coordinates of $W$ to $x_0 = 0$,
$x_1 = 100$ and $y_0 = 0$. The $init$ operation (see  below) can later
be used to change the window coordinates and scaling.}*/


 window(int);  // just create, do not open

 window(const window& w) : LEDA_WINDOW(w) {}
~window() {}   // ~LEDA_WINDOW does this job

 window& operator=(const window& w) 
 { LEDA_WINDOW::operator=(w); return *this; }
 

/*{\Moperations 3.1 4.4}*/

/*{\Mtext
\medskip
{\bf 3.1 Initialization} }*/

void init(double x0,double x1,double y0) { LEDA_WINDOW::init(x0,x1,y0); }
/*{\Mopl     sets the coordinates of $W$ to $x_0$, $x_1$, and $y_0$.}*/


void set_grid_mode(int d) {LEDA_WINDOW::set_grid_mode(d);}
/*{\Mopl     adds a rectangular grid with integer coordinates and  
             grid distance $d$ to $W$, if $d > 0$. Removes grid from
             $W$, if $d\le 0$.}*/


void init(double x0, double x1, double y0, int d) { LEDA_WINDOW::init(x0,x1,y0,d);}
/*{\Mopl     like init($x_0,x_1,y_0$) followed by set\_grid\_mode($d$).}*/


void clear(color c=BG_color) { LEDA_WINDOW::clear(c); };
/*{\Mopl      clears $W$.}*/

/*{\Mtext
\medskip
{\bf 3.2 Setting parameters} }*/

int set_line_width(int pix){return LEDA_WINDOW::set_line_width(pix);}
/*{\Mopl     sets the line width parameter to $pix$ pixels and
	     returns its previous value.}*/

line_style set_line_style(line_style s)
{return LEDA_WINDOW::set_line_style(s);}
/*{\Mopl     sets the line style parameter to $s$ and returns its
	     previous value.}*/

int set_node_width(int pix) {return LEDA_WINDOW::set_node_width(pix);}
/*{\Mopl     sets the node width parameter to $pix$ pixels and
	     returns its previous value.}*/

text_mode set_text_mode(text_mode m)
{return LEDA_WINDOW::set_text_mode(m);}
/*{\Mopl     sets the text mode parameter to $m$ and returns
	     its previous value.}*/

drawing_mode set_mode(drawing_mode m)
{return LEDA_WINDOW::set_mode(m);}
/*{\Mopl     sets the drawing mode parameter to $m$ and returns 
	     its previous value.}*/

void set_frame_label(string s){ LEDA_WINDOW::set_frame_label(s); }
/*{\Mopl     makes $s$ the window frame label. }*/

void reset_frame_label() { LEDA_WINDOW::reset_frame_label(); }
/*{\Mop      restores the standard LEDA frame label. }*/

void set_redraw(void (*F)()) { LEDA_WINDOW::set_redraw(*F);}
/*{\Mopl     sets the redraw function parameter to $F$.}*/


void set_flush(bool b)     { LEDA_WINDOW::set_flush(b); }
/*{\Mopl      flushes $X11$ output stream after each draw action 
             iff $b = true$.}*/

bool load_text_font(string fn)    { return LEDA_WINDOW::load_text_font(fn); }
/*{\Mopl      loads $X11$ font $fn$ and uses it as text font. Returns true
             on success and false if the font is not available.}*/

bool load_bold_font(string fn)    { return LEDA_WINDOW::load_bold_font(fn); }
/*{\Mopl      loads $X11$ font $fn$ and uses it as bold font. Returns true
             on success and false if the font is not available.}*/

bool load_message_font(string fn) { return LEDA_WINDOW::load_message_font(fn); }
/*{\Mopl      load $X11$ font $fn$ and use it as message font. Returns true
             on success and false if the font is not available.}*/


/*{\Mtext
\medskip
{\bf 3.3 Reading parameters and window coordinates} }*/

int get_line_width() {return LEDA_WINDOW::get_line_width();}
/*{\Mop      returns the current line width.}*/ 

line_style get_line_style() {return LEDA_WINDOW::get_line_style();}
/*{\Mop      returns the current line style.}*/ 

int get_node_width() {return LEDA_WINDOW::get_node_width();}
/*{\Mop      returns the current node width.}*/ 

text_mode get_text_mode() {return LEDA_WINDOW::get_text_mode();}
/*{\Mop      returns the current text mode.}*/ 

drawing_mode get_mode() {return LEDA_WINDOW::get_mode();}
/*{\Mop      returns the current drawing mode.}*/ 

double xmin() {return LEDA_WINDOW::xmin();}
/*{\Mop      returns $x_0$, the minimal x-coordinate of $W$.}*/ 

double ymin() {return LEDA_WINDOW::ymin();}
/*{\Mop      returns $y_0$, the minimal y-coordinate of $W$.}*/ 

double xmax() {return LEDA_WINDOW::xmax();}
/*{\Mop      returns $x_1$, the maximal x-coordinate of $W$.}*/ 

double ymax() {return LEDA_WINDOW::ymax();}
/*{\Mop      returns $y_1$, the maximal y-coordinate of $W$.}*/ 

double scale() {return LEDA_WINDOW::scale();}
/*{\Mop      returns the number of pixels of a unit length 
	     line segment.}*/

operator void*() { return (state==0) ? 0 : this; }



// points
/*{\Mtext
\medskip
{\bf 3.4 Drawing points} 
\setopdims 1.2 4.4
}*/

void draw_point(double x, double y, color c=FG_color);
/*{\Mopl     draws the point $(x,y)$ as a cross of a vertical 
	     and a horizontal segment intersecting at $(x,y)$.}*/

void draw_point(const point& p, color c=FG_color);
/*{\Mopl     draws point ($p$.xcoord(),$p$.ycoord()).}*/

void draw_pix(double x, double y, color c=FG_color);
void draw_pixel(double x, double y, color c=FG_color) { draw_pix(x,y,c); }
/*{\Mopl     sets the color of the pixel at position $(x,y)$ to $c$. }*/

void draw_pix(const point& p, color c=FG_color);
void draw_pixel(const point& p, color c=FG_color) { draw_pix(p,c); }
/*{\Mopl     sets the color of the pixel at position $p$ to $c$. }*/


// drawing segments
/*{\Mtext
\medskip
{\bf 3.5 Drawing line segments}
}*/

void draw_segment(double x1, double y1, double x2, double y2, color c=FG_color);
/*{\Mopl     draws a line segment from $(x_1,y_1)$ to $(x_2,y_2)$.}*/

void draw_segment(const point& p, const point& q, color c=FG_color );
/*{\Mopl     draws a line segment from point $p$ to point $q$.}*/

void draw_segment(const segment& s, color c=FG_color );
/*{\Mopl     draws line segment $s$.}*/

// lines
/*{\Mtext
\medskip
{\bf 3.6 Drawing lines}
}*/

void draw_line(double x1, double y1, double x2, double y2, color c=FG_color );
/*{\Mopl     draws a straight line passing through points $(x_1,y_1)$ and 
	     $(x_2,y_2)$.}*/

void draw_line(const point& p, const point& q, color c=FG_color);
/*{\Mopl     draws a straight line passing through points $p$ and $q$.}*/

void draw_line(const segment& s, color c=FG_color);
/*{\Mopl     draws the line supporting $s$.}*/


void draw_line(const line& l, color c=FG_color);
/*{\Mopl     draws line $l$.}*/

void draw_hline(double y, color c=FG_color );
/*{\Mopl     draws a horizontal line with y-coordinate $y$. }*/

void draw_vline(double x, color c=FG_color );
/*{\Mopl     draws a vertical line with x-coordinate $x$. }*/


// drawing arcs

void draw_arc(double x1, double y1, double x2, double y2, double r, color c=FG_color);
void draw_arc(const segment&, double, color c=FG_color);
void draw_arc(const point& p, const point& q, double r , color c=FG_color);
/*{\Mopl     draws a circular arc with radius r from p to q 
             with the center lying  to the right of the
             directed segment $p\longrightarrow q$.  }*/



// arrows
/*{\Mtext
\medskip
{\bf 3.7 Drawing arrows}
}*/

point draw_arrow_head(const point& q, double dir, color c=FG_color);

void draw_arrow(double x1, double y1, double x2, double y2, color c =FG_color );
/*{\Mopl     draws an arrow pointing from $(x_1,y_1)$ to $(x_2,y_2)$.}*/

void draw_arrow(const point& p, const point& q, color c=FG_color );
/*{\Mopl     draws an arrow pointing from point $p$ to point $q$.}*/

void draw_arrow(const segment& s, color=FG_color );
/*{\Mopl     draws an arrow pointing from $s$.start() to $s$.end().}*/

void draw_arc_arrow(double x1, double y1, double x2, double y2, double r,color c=FG_color);
void draw_arc_arrow(const segment& s, double r, color c=FG_color);
void draw_arc_arrow(const point& p , const point& q, double r, color c=FG_color);
/*{\Mopl      draws a circular arc arrow with radius r pointing from 
             p to q with the center lying  to the right of the
             directed segment $p\longrightarrow q$.  }*/



//circles
/*{\Mtext
\medskip
{\bf 3.8 Drawing circles}
}*/

void draw_circle(double x, double y, double r, color c=FG_color);
/*{\Mopl     draws the circle with center $(x,y)$ and radius $r$.}*/

void draw_circle(const point& p, double r, color c=FG_color);
/*{\Mopl     draws the circle with center $p$ and radius $r$.}*/

void draw_circle(const circle& C, color c=FG_color);
/*{\Mopl     draws circle $C$.}*/

void draw_ellipse(double x, double y, double r1, double r2, color c=FG_color);
/*{\Mopl     draws the ellipse with center $(x,y)$ and radii $r1$ and $r2$.}*/

void draw_ellipse(const point& p, double r1, double r2, color c=FG_color);
/*{\Mopl     draws the ellipse with center $p$ and radii $r1$ and $r2$.}*/



/*{\Mtext
\medskip
{\bf 3.9 Drawing discs}
}*/

void draw_disc(double x, double y, double r, color c=FG_color);
/*{\Mopl     draws a filled circle with center $(x,y)$ and radius $r$.}*/

void draw_disc(const point& p, double r, color c=FG_color);
/*{\Mopl     draws a filled circle with center $p$ and radius $r$.}*/

void draw_disc(const circle& C, color c=FG_color);
/*{\Mopl     draws filled circle $C$.}*/


void draw_filled_ellipse(double x, double y, double r1, double r2, color c=FG_color);
/*{\Mopl  draws a filled ellipse with center $(x,y)$ and radii $r1$ and $r2$.}*/

void draw_filled_ellipse(const point& p, double r1, double r2, color c=FG_color);
/*{\Mopl  draws a filled ellipse with center $p$ and radii $r1$ and $r2$.}*/


//polygons 
/*{\Mtext
\medskip
{\bf 3.10 Drawing polygons }
}*/

void draw_polygon(const list<point>& lp, color c=FG_color );
/*{\Mopl     draws the polygon with vertex sequence $lp$.}*/

void draw_polygon(const polygon& P, color c=FG_color );
/*{\Mopl     draws polygon $P$.}*/

void draw_filled_polygon(const list<point>& lp, color c=FG_color );
/*{\Mopl     draws the filled polygon with vertex sequence $lp$.}*/

void draw_filled_polygon(const polygon& P, color c=FG_color );
/*{\Mopl     draws filled polygon $P$.}*/

void draw_rectangle(double a, double  b, double c, double d, color=FG_color);
void draw_filled_rectangle(double a, double  b, double c, double d, color=FG_color);


// functions
/*{\Mtext
\medskip
{\bf 3.11 Drawing functions}
}*/

void plot_xy(double x0, double x1, draw_func_ptr F, color c=FG_color);
/*{\Mopl     draws function $F$ in range $[x_0,x_1]$, i.e., all points 
	     $(x,y)$ with $y = F(x)$ and $x_0\le x\le x_1$.}*/

void plot_yx(double y0, double y1, draw_func_ptr F, color c=FG_color);
/*{\Mopl     draws function $F$ in range $[y_0,y_1]$, i.e., all points 
	     $(x,y)$ with $x = F(y)$ and $y_0\le y\le y_1$.}*/




// text
/*{\Mtext
\medskip
{\bf 3.12 Drawing text}
}*/

void draw_text(double x, double y, string s, color c=FG_color);
/*{\Mopl     writes string $s$ starting at position $(x,y)$.}*/

void draw_text(const point& p, string s, color c=FG_color);
/*{\Mopl     writes string $s$ starting at position $p$.}*/

void draw_ctext(double x, double y, string s, color c=FG_color);
/*{\Mopl     writes string $s$ centered at position $(x,y)$.}*/

void draw_ctext(const point& p, string s, color c=FG_color);
/*{\Mopl     writes string $s$ centered at position $p$.}*/



// nodes
/*{\Mtext
\medskip
{\bf 3.13 Drawing nodes}
%Nodes are circles of diameter $node\_width$.
}*/

void draw_node(double x0, double y0, color c=FG_color);
/*{\Mopl     draws a node at position $(x_0,y_0)$.}*/

void draw_node(const point& p, color c=FG_color);
/*{\Mopl     draws a node at position $p$.}*/

void draw_filled_node(double x0, double y0, color c=FG_color);
/*{\Mopl     draws a filled node at position $(x_0,y_0)$.}*/

void draw_filled_node(const point& p, color c=FG_color);
/*{\Mopl     draws a filled node at position $p$.}*/

void draw_text_node(double x, double y, string s, color c=BG_color);
/*{\Mopl     draws a node with label $s$ at position $(x,y)$. }*/

void draw_text_node(const point& p, string s, color c=BG_color);
/*{\Mopl     draws a node with label $s$ at position $p$. }*/

void draw_int_node(double x, double y, int i, color c=BG_color);
/*{\Mopl     draws a node with integer label $i$ at position 
	     $(x,y)$. }*/

void draw_int_node(const point& p, int i, color c=BG_color);
/*{\Mopl     draws a node with integer label $i$ at position  $p$. }*/



// edges
/*{\Mtext
\medskip
{\bf 3.14 Drawing edges}
%Edges are straigth line segments or arrows with a clearance of 
%$node\_width/2$ at each end.
}*/

void draw_edge(double x1, double y1, double x2, double y2, color c=FG_color);
/*{\Mopl     draws an edge from $(x_1,y_1)$ to $(x_2,y_2)$.}*/

void draw_edge(const point& p, const point& q, color c=FG_color);
/*{\Mopl     draws an edge from $p$ to $q$.}*/

void draw_edge(const segment& s, color c=FG_color);
/*{\Mopl     draws an edge from $s$.start() to $s$.end().}*/

void draw_edge_arrow(double x1, double y1, double x2, double y2, color c=FG_color);
/*{\Mopl     draws a directed edge from $(x_1,y_1)$ to $(x_2,y_2)$.}*/

void draw_edge_arrow(const point& p, const point& q, color c=FG_color);
/*{\Mopl     draws a directed edge from $p$ to $q$.}*/

void draw_edge_arrow(const segment& s, color c=FG_color);
/*{\Mopl     draws a directed edge from $s$.start() to $s$.end().}*/ 

void draw_arc_edge(double x1, double y1, double x2, double y2, color c=FG_color);
void draw_arc_edge(const segment& s, double r, color c=FG_color);
void draw_arc_edge(const point& p, const point& q, double r, color c=FG_color);
/*{\Mopl     draws a circular edge arc with radius r from p to q 
             with the center lying  to the right of the
             directed segment $p\longrightarrow q$.  }*/

void draw_arc_edge_arrow(double x1, double y1, double x2, double y2, color c=FG_color);
void draw_arc_edge_arrow(const segment& s, double r, color c=FG_color );
void draw_arc_edge_arrow(const point& p, const point& q, double r, color c=FG_color);
/*{\Mopl     draws a circular directed edge arc with radius r from p 
             to q with the center lying  to the right of the
             directed segment $p\longrightarrow q$.  }*/




// mouse input
/*{\Mtext
\medskip
{\bf 3.15 Mouse Input}
}*/


int read_mouse();
/*{\Mop      displays the mouse cursor until a button is pressed.
	     Returns integer 1 for the left, 2 for the middle, and
	     3 for the right button (-1,-2,-3, if the shift key is
	     pressed simultaneously).}*/

int read_mouse(double& x, double& y);
/*{\Mopl     displays the mouse cursor on the screen until a
	     button is pressed. When a button is pressed the
             current position of the cursor is assigned
             to $(x,y)$ and the pressed button is returned.}*/

int read_mouse(point& p);
/*{\Mopl     displays the mouse cursor on the screen until a
	     button is pressed. When a button is pressed the
             current position of the cursor is assigned
             to $p$ and the pressed button is returned.}*/


int read_mouse_seg(double x0, double y0, double& x, double& y);
/*{\Mopl     displays a line segment from $(x_0,y_0)$ to the
	     current cursor position until a mouse button is
	     pressed. When a button is pressed the current
	     position is assigned to $(x,y)$ and the pressed
	     button is returned.}*/

int read_mouse_seg(const point& p, point& q);
/*{\Mopl     displays a line segment from $p$ to the current 
             cursor position until a mouse button is
	     pressed. When a button is pressed the current
	     position is assigned to $q$ and the pressed
	     button is returned.}*/

int read_mouse_rect(double x0, double y0, double& x, double& y);
/*{\Mopl     displays a rectangle with diagonal from $(x_0,y_0)$ 
     	     to the current cursor position until a mouse button 
	     is pressed. When a button is pressed the current 
	     position is assigned to $(x,y)$ and the pressed 
	     button is returned.}*/

int read_mouse_rect(const point& p, point& q);
/*{\Mopl     displays a rectangle with diagonal from $p$ 
     	     to the current cursor position until a mouse button 
	     is pressed. When a button is pressed the current 
	     position is assigned to $q$ and the pressed 
	     button is returned.}*/

int read_mouse_circle(double x0, double y0, double& x, double& y);
/*{\Mopl     displays a circle with center $(x_0,y_0)$ passing 
	     through the current cursor position until a mouse 
	     button is pressed. When a button is pressed the 
	     current position is assigned to $(x,y)$ and the 
	     pressed button is returned.}*/

int read_mouse_circle(const point& p, point& q);
/*{\Mopl     displays a circle with center $p$ passing 
	     through the current cursor position until a mouse 
	     button is pressed. When a button is pressed the 
	     current position is assigned to $q$ and the 
	     pressed button is returned.}*/

int read_mouse_action(mouse_action_func_ptr, double&, double&);
int read_mouse_action(mouse_action_func_ptr, point&);


int get_button();
/*{\Mop     non-blocking read operation, i.e., if a button was pressed 
            its number is returned, otherwise $0$ is returned. }*/

int get_button(double& x, double& y);
/*{\Mopl     if a button was pressed the corresponding position is
            assigned to $(x,y)$ and the button number is returned, 
            otherwise $0$ is returned. }*/

int get_button(point& p);
/*{\Mop     if a button was pressed the corresponding position is
            assigned to $p$ and the button number is returned, 
            otherwise $0$ is returned. }*/

/*{\Mtext
\bigskip
{\bf 3.16 Events}
}*/

/* inherited:
int  read_event(int& val, double& x, double& y);
*/
/*\{\Mop    waits for next event in window W and returns it. 
            Assigns the button or key to $val$ and the position
            in $W$ to  $(x,y)$. Possible events are 
            (cf. <LEDA/impl/x_window.h>): 
            key_press_event, key_release_event,
            button_press_event, button_release_event, 
            configure_event,motion_event, destroy_event. }*/


/* inherited:
unsigned button_press_time();
*/
/*{\Mop     returns $X11$ time-stamp of last button press event. }*/


/* inherited:
unsigned button_release_time();
*/
/*{\Mop     returns $X11$ time-stamp of last button release event. }*/
        
 

/*{\Mtext
\bigskip
{\bf 3.17 Panel Input}
}*/

int     confirm(string s);
/*{\Mop      displays string $s$ and asks for confirmation. 
	     Returns true iff the answer was ``yes''.}*/

void    notice(string s);
void    acknowledge(string s);
/*{\Mopl     displays string $s$ and asks for acknowledgement.}*/


int     read_panel(string h, int n, string*);
/*{\Mopl     displays a panel with header $h$ and an array $S[1..n]$  
	     of $n$ string buttons, returns the index of the selected 
	     button.}*/

int     read_vpanel(string h, int n, string*);
/*{\Mopl     like read\_panel with vertical button layout.}*/

string  read_string(string p);
/*{\Mop      displays a panel with prompt $p$ for string input, 
    	     returns the input.}*/

double  read_real(string p);
/*{\Mop      displays a panel with prompt $p$ for real input 
 	     returns the input.}*/

int     read_int(string p);
/*{\Mop      displays a panel with prompt $p$ for integer input, 
	     returns the input.}*/




// miscellaneous


void message(string s) {LEDA_WINDOW::message(s);};
/*{\Mop      displays message $s$ (each call adds a new line).}*/

void del_message()  { LEDA_WINDOW::del_messages(); };
void del_messages() { LEDA_WINDOW::del_messages(); };
/*{\Mop      deletes the text written by all previous message 
	     operations.}*/

void fill(double x, double y, color c=FG_color);

void copy_rect(double x1, double y1, double x2, double y2, double x, double y);
void move_rect(double x1, double y1, double x2, double y2, double x, double y);
void copy(double x1, double y1, double x2, double y2, int i=0);
void cut(double x1, double y1, double x2, double y2, int i=0);
void paste(int i, double x, double y);
void paste(double x, double y);
void clear_buf(int i=0);





// I/O operators
/*{\Mtext
\bigskip
{\bf 3.18 Input and output operators}

For input and output of basic geometric objects in the plane such as points, 
lines, line segments, circles, and polygons the $<<$ and $>>$ operators can 
be used. Similar to \CC input streams windows have an internal state indicating
whether there is more input to read or not. Its initial value is true and it 
is turned to false if an input sequence is terminated  by clicking the right 
mouse button (similar to ending stream input by the eof character). In 
conditional statements objects of type $window$ are automatically converted 
to boolean by returning this internal state. Thus, they can be used in 
conditional statements in the same way as \CC input streams. For example,
to read a sequence of points terminated by a right button click,  use 
`` {\bf while} ($W\ >>\ p$) $\{\  \dots\ \}$ ''. 
}*/

/*{\Mtext
\medskip
{\bf 3.18.1 Output}
\setopdims 2.5 4.4
}*/

window& draw(const point& p,color c=FG_color)   
{ draw_point(p,c); return *this; }

window& draw(const segment& s,color c=FG_color) 
{ draw_segment(s,c); return *this;}

window& draw(const line& l,color c=FG_color)    
{ draw_line(l,c); return *this; }

window& draw(const circle& C,color c=FG_color)  
{ draw_circle(C,c); return *this;}

window& draw(const polygon& P,color c=FG_color )
{ draw_polygon(P,c); return *this;}

window& operator<<(const point& p)   { return draw(p); }
/*{\Mbinop   like $W$.draw\_point($p$).}*/

window& operator<<(const segment& s) { return draw(s); }
/*{\Mbinop   like $W$.draw\_segment($s$).}*/

window& operator<<(const line& l)    { return draw(l); }
/*{\Mbinop   like $W$.draw\_line($l$).}*/

window& operator<<(const circle& C)  { return draw(C); }
/*{\Mbinop   like $W$.draw\_circle($C$).}*/

window& operator<<(const polygon& P) { return draw(P); }
/*{\Mbinop   like $W$.draw\_polygon($P$).}*/

/*{\Mtext
\bigskip
{\bf 3.18.2 Input}
}*/

window& read(point&);
window& read(segment&);
window& read(line&);
window& read(circle&);
window& read(polygon&);

window& operator>>(point& p);
/*{\Mbinop   reads a point $p$: clicking the left button 
	     assigns the current cursor position to $p$.}*/

window& operator>>(segment& s);
/*{\Mbinop   reads a segment $s$: use the left button to input 
	     the start and end point of $s$.}*/

window& operator>>(line& l);
/*{\Mbinop   reads a line $l$: use the left button to input 
	     two different points on $l$.}*/

window& operator>>(circle& C);
/*{\Mbinop   reads a circle $C$: use the left button to input 
	     the center of $C$ and a point on $C$.}*/

window& operator>>(polygon& P);
/*{\Mbinop   reads a polygon $P$: use the left button to input 
	     the sequence of vertices of $P$, end the sequence 
	     by clicking the middle button.}*/

/*{\Mtext
As long as an input operation has not been completed the last read point can 
be erased by simultaneously pressing the shift key and the left mouse button.
}*/


/*{\Mtext
\bigskip
{\bf 3.19 Non-Member Functions} 
}*/


friend int read_mouse(window*& w, double& x, double& y)
{ return LEDA_WINDOW::read_mouse((LEDA_WINDOW*&)w,x,y); }
/*{\Mfunc   waits for mouse input, assigns a pointer to the 
            corresonding window to $w$ and the position in 
            $*w$ to $(x,y)$ and returns the pressed button. }*/


 
        
/*
friend void put_back_event();
*/
/*{\Mfunc   puts last read event back to the input stream of events. }*/
 

}; // end of class window
 

#include <LEDA/panel.h>

#endif
