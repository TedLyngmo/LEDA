/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  _wcc.c
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany
+  All rights reserved.
+
*******************************************************************************/


/* basic graphic routines for MSDOS
 * implemented using watcom's graphics library (graph.lib)
 */

#include <graph.h>


static int MODE;
static int STYLE;
static int WIDTH;


void init_graphics(int mode, int col)
{
  if (mode == 1) // graphics mode
  { _setvideomode(_MAXRESMODE);
    for(int i=0; i<16; i++)
      set_palette(i,_R_[i],_G_[i],_B_[i]);
    }
  else // text mode
   _setvideomode(_DEFAULTMODE);

  videoconfig vconf;
  _getvideoconfig(&vconf);
  DISP_WIDTH  = vconf.numxpixels;
  DISP_HEIGHT = vconf.numypixels;
  DISP_DEPTH  = vconf.bitsperpixel;
  DISP_MAX_X  = DISP_WIDTH-1;
  DISP_MAX_Y  = DISP_HEIGHT-1;

}


static void set_draw_window(Window w)
{ DosWindow win = win_stack[w];
  int x0 = win->xpos;
  int y0 = win->ypos;
  int x1 = x0 + win->width  - 1;
  int y1 = y0 + win->height - 1;
  _setviewport(x0,y0,x1,y1);
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
  case solid : _setlinestyle(0xFFFF);
               break;
  case dotted: _setlinestyle(0x3333);
               break;
  case dashed: _setlinestyle(0x0F0F);
               break;
  }
  return save;
 }


int set_color(int color)
{ return _setcolor(color); }


int set_mode(int mode)
{ int save = MODE;
  MODE = mode;
  switch(mode) {
   case 0: _setplotaction(_GPSET);
           break;
   case 1: _setplotaction(_GXOR);
           break;
   case 2: _setplotaction(_GXOR);
           break;
   case 3: _setplotaction(_GAND);
           break;
  }
  return save;
}

void set_redraw(Window w, void (*f)())
{ win_stack[w]->redraw = f; }


void line(Window w, int x1, int y1, int x2, int y2)
{ set_draw_window(w);

  _moveto(x1,y1);
  _lineto(x2,y2);

  for(int i = 2; i <= WIDTH; i++)
  { int dx = (x1 > x2) ? x1-x2 : x2-x1;
    int dy = (y1 > y2) ? y1-y2 : y2-y1;
    int d = (i&1) ? i/2 : -i/2;
    if (dx < dy)
      { _moveto(x1+d,y1);
        _lineto(x2+d,y2);
       }
    else
      { _moveto(x1,y1+d);
        _lineto(x2,y2+d);
       }
   }
}


void fill_polygon(Window w, int n, int* xc, int* yc)
{ set_draw_window(w);
  xycoord* points = new xycoord[n];
  for(int i=0; i<n; i++)
  { points[i].xcoord = xc[i];
    points[i].ycoord = yc[i];
   }
  _polygon(_GFILLINTERIOR,n,points);
  delete[] points;
 }


void box(Window w, int x0, int y0, int x1, int y1)
{ set_draw_window(w);
  _rectangle(_GFILLINTERIOR,x0,y0,x1,y1);
 }


void  rectangle(Window w, int x0, int y0, int x1, int y1)
{ set_draw_window(w);
  _rectangle(_GBORDER,x0,y0,x1,y1);
 }

void circle(Window w, int x0,int y0,int r)
{ set_draw_window(w);
  _ellipse(_GBORDER,x0-r,y0-r,x0+r,y0+r);
 }

void fill_circle(Window w, int x0, int y0, int r)
{ set_draw_window(w);
  _ellipse(_GFILLINTERIOR,x0-r,y0-r,x0+r,y0+r);
 }



void put_text(Window w, int x, int y0, const char *text, int opaque)
{
  set_draw_window(w);

  int y1 = y0 + FONT_HEIGHT;

  if (opaque)
  { int save_color = _setcolor(win_stack[w]->bg_col);
    int save_mode = _setplotaction(_GPSET);
    _rectangle(_GFILLINTERIOR,x,y0,x+strlen(text)*FONT_WIDTH-1,y0+FONT_HEIGHT);
    _setplotaction(save_mode);
    _setcolor(save_color);
   }

  const char* stop = text + strlen(text);

  for (const char* p = text; p < stop; p++)
  { unsigned char* q =  FONT + (*p & 127) * FONT_HEIGHT;
    for(int y=y0; y<y1; y++)
    { unsigned char pat = *q++;
      if (pat & 128) _setpixel(x  ,y);
      if (pat &  64) _setpixel(x+1,y);
      if (pat &  32) _setpixel(x+2,y);
      if (pat &  16) _setpixel(x+3,y);
      if (pat &   8) _setpixel(x+4,y);
      if (pat &   4) _setpixel(x+5,y);
      if (pat &   2) _setpixel(x+6,y);
      if (pat &   1) _setpixel(x+7,y);
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
  int save_mode = _setplotaction(_GXOR);
  int save_col  = _setcolor(blue);
  put_text(w,win->width-138,1,s,1);
  _setplotaction(save_mode);
  _setcolor(save_col);
}



void pixel(Window w, int x, int y)
{ set_draw_window(w);
  _setpixel(x,y);
}


void pixels(Window w, int n, int* x, int* y)
{ while(n--) pixel(w,x[n],y[n]); }


void point(Window w, int x, int y)
{ set_draw_window(w);
  _setpixel(x,y);
  _setpixel(x-2,y-2);
  _setpixel(x-1,y-1);
  _setpixel(x+1,y+1);
  _setpixel(x+2,y+2);
  _setpixel(x-2,y+2);
  _setpixel(x-1,y+1);
  _setpixel(x+1,y-1);
  _setpixel(x+2,y-2);
}



void ellipse(Window w, int x0, int y0, int a, int b)
{ set_draw_window(w);
  _ellipse(_GBORDER,x0-a,y0-b,x0+a,y0+b);
}

void fill_ellipse(Window w, int x0, int y0, int a, int b)
{ set_draw_window(w);
  _ellipse(_GFILLINTERIOR,x0-a,y0-b,x0+a,y0+b);
}


void arc(Window w, int x0, int y0, int a, int b, double start, double angle)
{ set_draw_window(w);
  int start_x = x0 + int(cos(start)*1000);
  int start_y = y0 - int(sin(start)*1000);
  int end_x   = x0 + int(cos(start+angle)*1000);
  int end_y   = y0 - int(sin(start+angle)*1000);
  _arc(x0-a,y0-b,x0+a,y0+b,start_x,start_y,end_x,end_y);
 }


void fill_arc(Window,int,int,int,int,double,double)
{ }

void copy_pixrect(Window w, int x1, int y1, int x2, int y2, int x, int y)
{ set_draw_window(w);
  int sz = _imagesize(x1,y1,x2,y2);
  char* image = new char[sz];
  _getimage(x1,y1,x2,y2,image);
  _putimage(x,y,image,_GPSET);
  delete[] image;
 }


void insert_bitmap(Window w, int width, int height, char* data)
{ set_draw_window(w);
  char* p = data;
  for(int y=0; y<height; y++)
    for(int x=0; x<width; x+=8)
    { char pat = *p++;
      if (pat & 0x01) _setpixel(x,   y);
      if (pat & 0x02) _setpixel(x+1, y);
      if (pat & 0x04) _setpixel(x+2, y);
      if (pat & 0x08) _setpixel(x+3, y);
      if (pat & 0x10) _setpixel(x+4, y);
      if (pat & 0x20) _setpixel(x+5, y);
      if (pat & 0x40) _setpixel(x+6, y);
      if (pat & 0x80) _setpixel(x+7, y);
     }
 }



void set_palette(int index, int red, int green, int blue)
{ if (red < 0)     red = _R_[index]; else _R_[index] = red;
  if (green < 0) green = _G_[index]; else _G_[index] = green;
  if (blue < 0)   blue = _B_[index]; else _B_[index] = blue;
  _remappalette(index, (blue<<16) | (green<<8) | red);
}




//------------------------------------------------------------------------------
// pixrects
//------------------------------------------------------------------------------


char* create_pixrect(Window w, int left, int top, int right, int bottom)
{ set_draw_window(w);
  char* bp = new char[_imagesize(left,top,right,bottom)];
  _getimage(left,top,right,bottom,bp);
  return bp;
 }


void insert_pixrect(Window w, int left, int top, char* rect)
{ set_draw_window(w);
  if (left < 0) left = 0;
  if (top < 0) top = 0;
  _putimage(left,top,rect,_GPSET);
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
  _setviewport(0,0,DISP_MAX_X,DISP_MAX_Y);
  set_color(black);

  if (shape == 1)
  { _setpixel(mouse_x-2,mouse_y);
    _setpixel(mouse_x-1,mouse_y);
    _setpixel(mouse_x,mouse_y);
    _setpixel(mouse_x+1,mouse_y);
    _setpixel(mouse_x+3,mouse_y);
    _setpixel(mouse_x,mouse_y-2);
    _setpixel(mouse_x,mouse_y-1);
    _setpixel(mouse_x,mouse_y+1);
    _setpixel(mouse_x,mouse_y+2);
    _ellipse(_GBORDER,mouse_x-6,mouse_y-6,mouse_x+6,mouse_y+6);
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
      p += 13;
     }

    while (p != p_stop)
    { unsigned short pat = *p;
      if (pat & 0x8000) _setpixel(x,   y);
      if (pat & 0x4000) _setpixel(x+1, y);
      if (pat & 0x2000) _setpixel(x+2, y);
      if (pat & 0x1000) _setpixel(x+3, y);
      if (pat & 0x0800) _setpixel(x+4, y);
      if (pat & 0x0400) _setpixel(x+5, y);
      if (pat & 0x0200) _setpixel(x+6, y);
      if (pat & 0x0100) _setpixel(x+7, y);
      if (pat & 0x0080) _setpixel(x+8, y);
      if (pat & 0x0040) _setpixel(x+9, y);
      if (pat & 0x0020) _setpixel(x+10,y);
      if (pat & 0x0010) _setpixel(x+11,y);
      if (pat & 0x0008) _setpixel(x+12,y);
      if (pat & 0x0004) _setpixel(x+13,y);
      if (pat & 0x0002) _setpixel(x+14,y);
      if (pat & 0x0001) _setpixel(x+15,y);
      p+=d;
      y++;
     }
   }
}



