/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  _emx.c
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/



/* basic graphic routines for MSDOS
 * implemented using the emx graphics library (jmgraph.a)
 */

#include <jmgraph.h>
#include <stdio.h>


static int MODE;
static int STYLE;
static int WIDTH;
static int COLOR;

static int win_xmin;  // coordinates of current window
static int win_ymin;
static int win_xmax; 
static int win_ymax;


void init_graphics(int mode, int col)
{
  if (mode == 1) // graphics mode
  { 
     if (!g_mode(G640x480x16))
     { fprintf (stderr, "Cannot switch to graphics mode 640x480x16\n");
       exit(1);
      }

    char pal[48];
    char* p = pal;
    for(int i=0; i<16; i++) 
      { *p++ = _R_[i];
        *p++ = _G_[i];
        *p++ = _B_[i];
       }
      g_vgapal(pal,0,16,1);
    }
  else // text mode
    g_mode(GTEXT);


  DISP_WIDTH  = g_xsize;
  DISP_HEIGHT = g_ysize;
  DISP_DEPTH  = g_colors;
  DISP_MAX_X  = DISP_WIDTH-1;
  DISP_MAX_Y  = DISP_HEIGHT-1;

  MODE  = 0;
  STYLE = 0;
  WIDTH = 1;
  COLOR = 1;

  win_xmin = 0;
  win_ymin = 0;
  win_xmax = DISP_MAX_X;
  win_ymax = DISP_MAX_Y;
  
}


static void set_draw_window(Window w)
{ DosWindow win = win_stack[w];
  win_xmin = win->xpos;
  win_ymin = win->ypos;
  win_xmax = win_xmin + win->width  - 1;
  win_ymax = win_ymin + win->height - 1;
  g_clip(win_xmin,win_ymin,win_xmax,win_ymax);
 }


void flush_display() {}

int  new_color(const char*) { return 1; }

int  text_height(const char*)  { return FONT_HEIGHT; }
int  text_width(const char* s) { return FONT_WIDTH*strlen(s); }

int  load_text_font(const char*) { return 0;}
int  load_bold_font(const char*) { return 0;}
int  load_message_font(const char*) { return 0;}

int  set_font(const char*) { return 0;}
void set_text_font() {}
void set_bold_font() {}
void set_message_font() {}


int set_line_width(int width) 
{ int save = WIDTH;
  WIDTH = width;
  return save; 
 }

int set_line_style(int style) 
{ int save = STYLE;
  STYLE = style;
  switch(style) {
  case solid : //_setlinestyle(0xFFFF);
               break;
  case dotted: //_setlinestyle(0x3333);
               break;
  case dashed: //_setlinestyle(0x0F0F);
               break;
  }
  return save;
 }


int set_color(int col) 
{ int c = COLOR;
  COLOR = col;
  return c; 
 }


int set_mode(int mode)
{ int save = MODE;
  MODE = mode;
  switch(mode) {
   case 0: g_wmode(G_NORM);
           break;
   case 1: g_wmode(G_XOR);
           break;
   case 2: g_wmode(G_OR);
           break;
   case 3: g_wmode(G_AND);
           break;
  }
  return save;
}

void set_redraw(Window w, void (*f)())
{ win_stack[w]->redraw = f; }


void line(Window w, int x1, int y1, int x2, int y2)
{ set_draw_window(w);

  x1 += win_xmin;
  x2 += win_xmin;
  y1 += win_ymin;
  y2 += win_ymin;

  g_line(x1,y1,x2,y2,COLOR);

  for(int i = 2; i <= WIDTH; i++)
  { int dx = (x1 > x2) ? x1-x2 : x2-x1;
    int dy = (y1 > y2) ? y1-y2 : y2-y1;
    int d = (i&1) ? i/2 : -i/2;
    if (dx < dy)
       g_line(x1+d,y1,x2+d,y2,COLOR);
    else
       g_line(x1,y1+d,x2,y2+d,COLOR);
   }
}


void fill_polygon(Window w, int n, int* xc, int* yc)
{ set_draw_window(w);
  for (int i = 0; i<n; i++)
  { xc[i] += win_xmin;
    yc[i] += win_ymin;
   }
  g_polygon(xc,yc,n,COLOR,G_FILL);
 }


void box(Window w, int x0, int y0, int x1, int y1)
{ set_draw_window(w);
  x0 += win_xmin;
  x1 += win_xmin;
  y0 += win_ymin;
  y1 += win_ymin;
  g_box(x0,y0,x1,y1,COLOR,G_FILL);
 }


void  rectangle(Window w, int x0, int y0, int x1, int y1)
{ set_draw_window(w);
  x0 += win_xmin;
  x1 += win_xmin;
  y0 += win_ymin;
  y1 += win_ymin;
  g_box(x0,y0,x1,y1,COLOR,G_OUTLINE);
 }

void circle(Window w, int x0,int y0,int r)
{ set_draw_window(w);
  x0 += win_xmin;
  y0 += win_ymin;
  g_ellipse(x0,y0,r,r,COLOR,G_OUTLINE);
 }

void fill_circle(Window w, int x0, int y0, int r)
{ set_draw_window(w);
  x0 += win_xmin;
  y0 += win_ymin;
  g_ellipse(x0,y0,r,r,COLOR,G_FILL);
 }


void put_text(Window w, int x, int y0, const char *text, int opaque)
{ 
  set_draw_window(w);
  x  += win_xmin;
  y0 += win_ymin;

  int y1 = y0 + FONT_HEIGHT;


  if (opaque)
  { int bg_col = win_stack[w]->bg_col;
    int save = set_mode(0);
    g_box(x,y0,x+strlen(text)*FONT_WIDTH-1,y0+FONT_HEIGHT,bg_col,G_FILL);
    set_mode(save);
   }

  const char* stop = text + strlen(text);

  for (const char* p = text; p < stop; p++)
  { unsigned char* q =  FONT + (*p & 127) * FONT_HEIGHT;
    for(int y=y0; y<y1; y++)
    { unsigned char pat = *q++;
      if (pat & 128) g_set(x  ,y,COLOR);
      if (pat &  64) g_set(x+1,y,COLOR);
      if (pat &  32) g_set(x+2,y,COLOR);
      if (pat &  16) g_set(x+3,y,COLOR);
      if (pat &   8) g_set(x+4,y,COLOR);
      if (pat &   4) g_set(x+5,y,COLOR);
      if (pat &   2) g_set(x+6,y,COLOR);
      if (pat &   1) g_set(x+7,y,COLOR);
     }
    x += 8;
   }
}



void put_text(Window w, int x, int y, const char *text, int l, int opaque)
{ char* str = new char[strlen(text)+1];
  strcpy(str,text);
  str[l] = '\0';
  put_text(w,x,y,text,l,opaque);
  delete[] str;
}

void put_ctext(Window w, int x, int y, const char* str, int opaque)
{ put_text(w,x-(text_width(str)-1)/2, y-(text_height(str)-1)/2, str, opaque); }


void show_coordinates(Window w, const char* s)
{ DosWindow win = win_stack[w];
  set_draw_window(w);
  int save_mode = set_mode(1);
  int save_col = set_color(blue);
  put_text(w,win->width-138,1,s,1); 
  set_color(save_col);
  set_mode(save_mode);
}



void pixel(Window w, int x, int y) 
{ set_draw_window(w);
  x += win_xmin;
  y += win_ymin;
  g_set(x,y,COLOR);
}

void point(Window w, int x, int y) 
{ set_draw_window(w);
  x += win_xmin;
  y += win_ymin;
  g_set(x,y,COLOR);
  g_set(x-2,y-2,COLOR);
  g_set(x-1,y-1,COLOR);
  g_set(x+1,y+1,COLOR);
  g_set(x+2,y+2,COLOR);
  g_set(x-2,y+2,COLOR);
  g_set(x-1,y+1,COLOR);
  g_set(x+1,y-1,COLOR);
  g_set(x+2,y-2,COLOR);
}

void pixels(Window w, int n, int* x, int* y)
{ while(n--) pixel(w,x[n],y[n]); }


void ellipse(Window w, int x0, int y0, int a, int b)
{ set_draw_window(w);
  x0 += win_xmin;
  y0 += win_ymin;
  g_ellipse(x0,y0,a,b,COLOR,G_OUTLINE);
}

void fill_ellipse(Window w, int x0, int y0, int a, int b)
{ set_draw_window(w);
  x0 += win_xmin;
  y0 += win_ymin;
  g_ellipse(x0,y0,a,b,COLOR,G_FILL);
}


void arc(Window w, int x0, int y0, int a, int b, double start, double angle)
{ }


void fill_arc(Window,int,int,int,int,double,double)
{ }

void copy_pixrect(Window win, int x1, int y1, int x2, int y2, int x, int y)
{ set_draw_window(win);
  x1 += win_xmin;
  x2 += win_xmin;
  x  += win_xmin;
  y1 += win_ymin;
  y2 += win_ymin;
  y  += win_ymin;

  int w = x2 - x1 + 1;
  int h = y2 - y1 + 1;
  int sz = g_imagesize(w,h);
  char* image = new char[sz];
  g_getimage(x1,y1,x2,y2,image);
  g_putimage(x,y,x+w-1,y+h-1,image);
  delete[] image;
 }


void insert_bitmap(Window w, int width, int height, char* data)
{ set_draw_window(w);
  char* p = data;
  for(int y=0; y<height; y++)
    for(int x=0; x<width; x+=8)
    { char pat = *p++;
      if (pat & 0x01) g_set(x,   y,black);
      if (pat & 0x02) g_set(x+1, y,black);
      if (pat & 0x04) g_set(x+2, y,black);
      if (pat & 0x08) g_set(x+3, y,black);
      if (pat & 0x10) g_set(x+4, y,black);
      if (pat & 0x20) g_set(x+5, y,black);
      if (pat & 0x40) g_set(x+6, y,black);
      if (pat & 0x80) g_set(x+7, y,black);
     }
 }


void set_palette(int index, int red, int green, int blue)
{ if (red < 0)     red = _R_[index]; else _R_[index] = red;
  if (green < 0) green = _G_[index]; else _G_[index] = green;
  if (blue < 0)   blue = _B_[index]; else _B_[index] = blue;
  char pal[3];
  pal[0] = red;
  pal[1] = green;
  pal[2] = blue;
  g_vgapal(pal,index,1,1);
}




//------------------------------------------------------------------------------
// pixrects
//------------------------------------------------------------------------------


char* create_pixrect(Window win, int left, int top, int right, int bottom)
{ set_draw_window(win);
  left   += win_xmin;
  right  += win_xmin;
  top    += win_ymin;
  bottom += win_ymin;

  left  &= 0xFFF8;
  right |= 7;

  int w = right-left+1;
  int h = bottom-top+1;
  char* bp = new char[g_imagesize(w,h)+8];
  g_getimage(left,top,right,bottom,bp+8);
  *(int*)bp = w;
  *(int*)(bp+4) = h;
  return bp;
 }


void insert_pixrect(Window win, int left, int top, char* rect)
{ set_draw_window(win);

  left   += win_xmin;
  top    += win_ymin;

  left  &= 0xFFF8;

  if (left < 0) left = 0;
  if (top < 0) top = 0;
  int w = *(int*)rect;
  int h = *(int*)(rect+4);
  g_putimage(left,top,left+w-1,top+h-1,rect+8);
 }


void delete_pixrect(char* rect)   { delete rect; }



//------------------------------------------------------------------------------
// mouse cursor
//------------------------------------------------------------------------------


static unsigned short p_mask1[14] = 
{0xc000,0xf000,0x7c00,0x7f00,0x3fc0,0x3fc0,0x1f00,
 0x1f80,0x0dc0,0x0ce0,0x0070,0x0038,0x001c,0x000c};

static unsigned short p_mask2[14] = 
{0x0003,0x000f,0x003e,0x00fe,0x03fc,0x03fc,0x00f8,
 0x01f8,0x03b0,0x0730,0x0e00,0x1c00,0x3800,0x3000};


void draw_pointer(int mouse_x, int mouse_y, int shape)
{
  g_clip(0,0,DISP_MAX_X,DISP_MAX_Y);
  set_color(black);

  if (shape == 1)
  { g_set(mouse_x-2,mouse_y,black);
    g_set(mouse_x-1,mouse_y,black);
    g_set(mouse_x,mouse_y,black);
    g_set(mouse_x+1,mouse_y,black);
    g_set(mouse_x+3,mouse_y,black);
    g_set(mouse_x,mouse_y-2,black);
    g_set(mouse_x,mouse_y-1,black);
    g_set(mouse_x,mouse_y+1,black);
    g_set(mouse_x,mouse_y+2,black);
    g_ellipse(mouse_x,mouse_y,6,6,black,G_OUTLINE);
    return;
   }
  else
  { int x = mouse_x;
    int y = mouse_y;
    unsigned short* p = p_mask1;

    if (x > DISP_MAX_X - 16) 
    { x -= 16;
      p = p_mask2;
     }

    unsigned short* p_stop = p + 14;
    int d  = 1;

    if (y > DISP_MAX_Y - 14) 
    { y -= 14;
      d = -1;
      p_stop = p-1;
      p += 14;
     }
     
    while (p != p_stop)
    { unsigned short pat = *p;
      if (pat & 0x8000) g_set(x,   y,black);
      if (pat & 0x4000) g_set(x+1, y,black);
      if (pat & 0x2000) g_set(x+2, y,black);
      if (pat & 0x1000) g_set(x+3, y,black);
      if (pat & 0x0800) g_set(x+4, y,black);
      if (pat & 0x0400) g_set(x+5, y,black);
      if (pat & 0x0200) g_set(x+6, y,black);
      if (pat & 0x0100) g_set(x+7, y,black);
      if (pat & 0x0080) g_set(x+8, y,black);
      if (pat & 0x0040) g_set(x+9, y,black);
      if (pat & 0x0020) g_set(x+10,y,black);
      if (pat & 0x0010) g_set(x+11,y,black);
      if (pat & 0x0008) g_set(x+12,y,black);
      if (pat & 0x0004) g_set(x+13,y,black);
      if (pat & 0x0002) g_set(x+14,y,black);
      if (pat & 0x0001) g_set(x+15,y,black);
      p+=d;
      y++;
     }
   }
}

