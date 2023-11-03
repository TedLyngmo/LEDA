/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  leda_panel.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/leda_window.h>

typedef void (*action_func)(int);

#define MAX_BUT_NUM  64
#define MAX_ITEM_NUM 64 


class LEDA_PANEL : public LEDA_WINDOW {

Window win;

char*  header;

int    XPOS;
int    YPOS;
int    WIDTH;
int    HEIGHT;

int    but_count;
int    item_count;
int    act_str_item;
int    last_ycoord;

char*  button_str[MAX_BUT_NUM];
char*  label_str [MAX_ITEM_NUM];
char*  data_str  [MAX_ITEM_NUM];
int    ycoord[MAX_ITEM_NUM]; 

int    kind   [MAX_ITEM_NUM];
void*  ref    [MAX_ITEM_NUM];
int    dat1   [MAX_ITEM_NUM];   // min (slider), size (choice), 
                                // number of menu items (string_menu_item)
int    dat2   [MAX_ITEM_NUM];   // max (slider)  step (choice)
int    offset [MAX_ITEM_NUM];   // choice item only
char** choices[MAX_ITEM_NUM];   // choice item, string_menu_item)

action_func action[MAX_ITEM_NUM];


int but_layout;

int bg_color;
int item_bg_color;
int d3_box_bg_color;
int shadow_color;
int press_color;


// geometry

int   th;               // text height
int   tw;               // text width
int   xoff;             /* left and right boundary space */
int   yoff;             /* top and bottom boundary space */
int   xoff1;            /* start of items */
int   slider_w;         /* slider item length */
int   slider_h;         /* height of slider items   */
int   string_w;         /* string/int/float item length */
int   string_h;         /* string/int/float item height */
int   ytskip;           /* height of text items   */
int   yskip;            /* height of other items  */
int   choice_h;         /* height of choice items   */
int   choice_w;         /* choice field width */
int   button_h;         /* button height */
int   button_w;         /* button width */
int   bskip;            /* button space */
int   bxoff;            /* left and right button boundary space */

int   but_per_line;



void draw_box_with_shadow(int,int,int,int,int,int);
void draw_d3_box(int x1,int y1,int x2,int y2, int pressed=0);
void draw_string_item(int i, int activ=0, const char* s=0);
void activate_string_item(int);
void panel_text_edit(int i);
void put_text_item(int x, int y, const char* s, int t_len);
void draw_choice_bool_item(int i, int x, int y, int c, int n);
void draw_choice_item(int i);
void draw_bool_item(int i);
void draw_color_item(int i);
void change_color_item(int i, int j);
void draw_slider_item(int i,float x);
void draw_button(const char* s, int x, int y,int pressed);
void draw_menu_button(int x, int y,int pressed);
void item_error();


public:

void set_bg_color(int c) { bg_color = c; }
void buttons_per_line(int n) { but_per_line = n; }

void text_item(const char* s);
void string_item(const char* s, char** x);
void string_menu_item(const char* s, char** x, const char* menu_label,int argc,const char** argv);
void int_item(const char* s, int* x);
void slider_item(const char* s, int* x, int min, int max, action_func F = 0);
void float_item(const char* s, double* x);
void choice_item(const char* text, int* address, int argc, const char** argv, int step, int offset);
void bool_item(const char* text, char* address);
void color_item(const char* text, int* x);
int  button(const char* s);
void button_line(int n, const char** b) ;

void display(int x,int y,int win_x,int win_y,int width,int height);
int  read();
void close();

int open(int x,int y,int win_x,int win_y,int width,int height,int mode=1);

void init(const char*);

 LEDA_PANEL(const char* label="", int bl=0);
 LEDA_PANEL(const char*, int, int);
 LEDA_PANEL(int, int);
~LEDA_PANEL();

static void panel_redraw_func();

};


