/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  x_basic.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/

#ifndef LEDA_X_BASIC_H
#define LEDA_X_BASIC_H

// basic graphics operations used in the LEDA window & panel classes

#include <LEDA/impl/x_window.h>

/* display */

void open_display(void);
void close_display(void);
void flush_display(void);
int  display_width(void);
int  display_height(void);
int  display_depth(void);


/* windows */

Window open_window(int x,int y,int width,int height,int bg_col, 
                   const char* header, const char* label);
void close_window(Window win);
void clear_window(Window win);
int  window_height(Window win);
int  window_width(Window win);
void window_position(Window win, int* x, int* y);
void set_header(Window win, const char *s);
void set_redraw(Window,void (*f)());

void start_batch(Window);
void end_batch(Window);


/* drawing */

void pixel(Window win, int x, int y);
void pixels(Window win, int n, int* x, int* y);
void point(Window win, int x, int y);
void line(Window win, int x1, int y1, int x2, int y2);
void rectangle(Window win, int x1, int y1, int x2, int y2);
void box(Window win, int x1, int y1, int x2, int y2);
void arc(Window win,int x0,int y0,int r1,int r2,double start,double angle);
void circle(Window win, int x0, int y0, int r);
void ellipse(Window win, int x0, int y0, int r1, int r2);
void fill_arc(Window win,int x0,int y0,int r1,int r2,double start,double angle);
void fill_circle(Window win, int x0, int y0, int r);
void fill_ellipse(Window win, int x0, int y0, int r1, int r2);
void fill_polygon(Window win, int n, int* xcoord, int* ycoord);
void put_text(Window win, int x, int y, const char* s, int opaque);
void put_text(Window win, int x, int y, const char* s, int l, int opaque);
void put_ctext(Window win, int x, int y, const char* s, int opaque);

char* create_pixrect(Window win, int x1, int y1, int x2, int y2);
void insert_pixrect(Window win, int x, int y, char* rect);
void delete_pixrect(char* rect);
void copy_pixrect(Window win, int x1, int y1, int x2, int y2, int x, int y);

void insert_bitmap(Window win, int width, int height, char* data);
void show_coordinates(Window win, const char* s);


/* fonts and text */

int  load_text_font(const char* font_name);
int  load_bold_font(const char* font_name);
int  load_message_font(const char* font_name);
int  set_font(const char* fname);
void set_text_font(void);
void set_bold_font(void);
void set_message_font(void);
int text_width(const char* s);
int text_height(const char* s);


/* drawing parameters */

int  set_color(int col);
int  set_mode(int m);
int  set_line_width(int w);
int  set_line_style(int s);
void set_read_gc(void);
void reset_gc(void);


/* colors */

void set_palette(int,int,int,int);
int  new_color(const char*);


/* events */

int  get_next_event(Window* win, int* val, int* x, int* y, unsigned long *t);
int  check_next_event(Window* win, int* val, int* x, int* y, unsigned long *t);
void put_back_event(void);


#endif
