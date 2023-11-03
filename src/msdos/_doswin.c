/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  _doswin.c
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany
+  All rights reserved.
+
*******************************************************************************/

#include <LEDA/impl/doswin.h>
#include <LEDA/bitmaps/leda_icon.xbm>

//------------------------------------------------------------------------------
// event handling, window manager, etc ...
//------------------------------------------------------------------------------

static char read_kbd()
{
#if defined(__EMX__)
   return _read_kbd(0,0,0);
#else
   return (kbhit()) ? getch() : 0;
#endif
 }


const int root_color = blue2;
const int label_col0 = grey2;
const int label_col1 = grey2;

const int  win_max = 16;
DosWindow  win_stack[win_max];
int        win_top = 0;


#define BORDER_W 5
#define HEADER_W 20

static int display = 0;
static int mouse_installed=0;
static int mouse_two_buttons=0;

static DosWindow root_win = 0;
static DosWindow active_win = 0;

static int pointer_shape = 0;
static int pointer_visible = 0;
static int mouse_x = 320;
static int mouse_y = 240;

static int event_buffer_e = -1;
static int event_buffer_val;
static DosWindow event_buffer_win;

static int last_event_e;
static int last_event_val;
static DosWindow last_event_win;

static int save_mode;
static int save_lw;
static int save_ls;


static void show_pointer()
{ if (!pointer_visible)
     draw_pointer(mouse_x,mouse_y,pointer_shape);
  pointer_visible = 1;
 }

static void hide_pointer()
{ if (pointer_visible)
     draw_pointer(mouse_x,mouse_y,pointer_shape);
  pointer_visible = 0;
 }


static void set_pointer_shape(int shape)
{ if (pointer_shape != shape)
  { hide_pointer();
    pointer_shape = shape;
    show_pointer();
  }
 }


static void put_header(DosWindow win)
{ char str[80];
  int n = (win->width - 40)/text_width("H");
  strncpy(str,win->header,n);
  str[n] = 0;
  put_ctext(root_win->id,win->x0 + win->width/2 + 20, win->y0 + 10 ,str,0);
 }

static void set_header(DosWindow win, const char* s)
{ int save_co = set_color(win->label_col);
  int save_mo = set_mode(0);
  put_header(win);
  if (s != win->header) strcpy(win->header,s);
  set_color(1);
  put_header(win);
  set_color(save_co);
  set_mode(save_mo);
 }

void set_header(Window win, const char* s) { set_header(win_stack[win],s); }


void draw_window(DosWindow win, int clear_win = 1)
{
  int bw  = win->xpos - win->x0 - 2;
  int lw  = win->ypos - win->y0 - 2;
  int bw1 = 2*(bw+1);

  int x0 = win->x0;
  int y0 = win->y0;

  int x1 = win->x1;
  int y1 = win->y1;

  int save_mode = set_mode(0);

  if (clear_win)
  { set_color(win->bg_col);
    box(root_win->id,x0,y0,x1,y1);
   }

  set_color(win->label_col);
  box(root_win->id,x0+bw,y0+bw,x1-bw,y0+lw);
  box(root_win->id,x0,y0+bw1,x0+bw,y1-bw1);
  box(root_win->id,x1-bw,y0+bw1,x1,y1-bw1);
  box(root_win->id,x0+bw1,y1-bw,x1-bw1,y1);
  box(root_win->id,x0+bw1,y0,x1-bw1,y0+bw);
  set_color(1);

  rectangle(root_win->id,x0,y0,x1,y1);

  line(root_win->id,x0+bw,y0+bw,x0+bw1,y0+bw);
  line(root_win->id,x0+bw1,y0+bw,x0+bw1,y0+1);
  line(root_win->id,x0+bw1,y0+1,x1-bw1,y0+1);
  line(root_win->id,x1-bw1,y0+1,x1-bw1,y0+bw);
  line(root_win->id,x1-bw1,y0+bw,x1-bw,y0+bw);
  line(root_win->id,x1-bw,y0+bw,x1-bw,y0+bw1);
  line(root_win->id,x1-bw,y0+bw1,x1-1,y0+bw1);
  line(root_win->id,x1-1,y0+bw1,x1-1,y1-bw1);
  line(root_win->id,x1-1,y1-bw1,x1-bw,y1-bw1);
  line(root_win->id,x1-bw,y1-bw1,x1-bw,y1-bw);
  line(root_win->id,x1-bw,y1-bw,x1-bw1,y1-bw);
  line(root_win->id,x1-bw1,y1-bw,x1-bw1,y1-1);
  line(root_win->id,x1-bw1,y1-1,x0+bw1,y1-1);
  line(root_win->id,x0+bw1,y1-1,x0+bw1,y1-bw);
  line(root_win->id,x0+bw1,y1-bw,x0+bw,y1-bw);
  line(root_win->id,x0+bw,y1-bw,x0+bw,y1-bw1);
  line(root_win->id,x0+bw,y1-bw1,x0+1,y1-bw1);
  line(root_win->id,x0+1,y1-bw1,x0+1,y0+bw1);
  line(root_win->id,x0+1,y0+bw1,x0+bw,y0+bw1);
  line(root_win->id,x0+bw,y0+bw1,x0+bw,y0+bw);

  rectangle(root_win->id, win->xpos-1, win->ypos-1,
                      win->xpos+win->width, win->ypos+win->height);

  // iconize-button
  set_color(0);
  box(root_win->id,x0+13,y0+3,x0+27,y0+15);
  set_color(1);
  rectangle(root_win->id,x0+13,y0+3,x0+27,y0+15);
  line(root_win->id,x0+14,y0+16,x0+28,y0+16);
  line(root_win->id,x0+28,y0+4,x0+28,y0+15);
  line(root_win->id,x0+17,y0+6,x0+23,y0+6);
  line(root_win->id,x0+16,y0+6,x0+20,y0+13);
  line(root_win->id,x0+17,y0+6,x0+20,y0+13);
  line(root_win->id,x0+24,y0+6,x0+20,y0+13);
  line(root_win->id,x0+23,y0+6,x0+20,y0+13);

  set_header(win->id,win->header);

  set_mode(save_mode);

}


Window open_window(int x,int y,int width,int height,int bg,
                   const char* header, const char* label)
{
  if (win_top > 0)  // deactivate top window
  { DosWindow w = win_stack[win_top];
    w->label_col = label_col0;
    draw_window(w,0);
   }

  DosWindow win = new dos_window;
  win_stack[++win_top] = win;
  win->id = win_top;

  strcpy(win->header," ");
  strcpy(win->label,label);

  win->bg_col = bg;
  win->label_col = label_col0;

  width  += 2*BORDER_W;
  height += HEADER_W+BORDER_W;

  win->x0 = x;
  win->y0 = y;
  win->x1 = x+width-1;
  win->y1 = y+height-1;
  win->xpos = x+BORDER_W;
  win->ypos = y+HEADER_W;
  win->width = width - 2*BORDER_W;
  win->height = height - HEADER_W - BORDER_W;

  win->iconized = 0;
  win->save_x0 = 3;
  win->save_y0 = 3 + 150*(win_top-1);
  win->save_x1 = win->save_x0 + 128 + 2*BORDER_W;
  win->save_y1 = win->save_y0 + 128 + HEADER_W + BORDER_W;
  win->save_bg_col = white;
  win->redraw = 0;

  win->image_buf = create_pixrect(root_win->id,win->x0,win->y0,win->x1,win->y1);
  draw_window(win);
  set_header(win->id,header);
  return win->id;
}


void clear_window(Window w)
{ DosWindow win = win_stack[w];
  int save_col  = set_color(win->bg_col);
  int save_mode = set_mode(0);
  box(w,0,0,win->width-1,win->height-1);
  set_color(save_col);
  set_mode(save_mode);

  while (w < win_top)
  { w++;
    DosWindow win = win_stack[w];
    draw_window(win);
    if (win->redraw) (*(win->redraw))();
   }
 }


void close_window(Window w)
{ if (w != win_top) return;  /* can close top window only */
  DosWindow win = win_stack[w];
  insert_pixrect(root_win->id,win->x0,win->y0,win->image_buf);
  delete_pixrect(win->image_buf);
  win_top--;
  delete win;
 }


void set_read_gc()
{ save_mode = set_mode(1);
  save_ls   = set_line_style(0);
  save_lw   = set_line_width(1);
  set_color(1);
  show_pointer();
 }

void reset_gc()
{ hide_pointer();
  set_mode(save_mode);
  set_line_style(save_ls);
  set_line_width(save_lw);
 }



#define UP  72
#define DO  80
#define LE  75
#define RI  77

static int handle_next_event(Window* win, int *val, int *x, int *y)
{
  int  e =  no_event;
  int  i;
  DosWindow w;

  union REGISTERS key_regs;
  union REGISTERS mouse_regs;

  if (event_buffer_e != -1)
  { *val = event_buffer_val;
    w = event_buffer_win;
    *win = w->id;
    *x = mouse_x - w->xpos;
    *y = mouse_y - w->ypos;
    last_event_e = event_buffer_e;
    last_event_val = event_buffer_val;
    last_event_win = event_buffer_win;
    event_buffer_e = -1;
    return last_event_e;
   }

  mouse_regs.x.cx = 8*mouse_x;
  mouse_regs.x.dx = 8*mouse_y;

  char c;

  if ((c = read_kbd()) > 0)
  {
    int step = 16;

    if (c==27)
    { close_display();
      exit(0);
     }

    if (c != 0)   /* ascii char */
       { *val = c;
         e = key_press_event;
        }
    else          /* cursor or function key  */
       {
         c = getch();

         switch(c) {

         case  LE: mouse_regs.x.cx -= step;
                   e = motion_event;
                   break;

         case  RI: mouse_regs.x.cx += step;
                   e = motion_event;
                   break;

         case  UP: mouse_regs.x.dx -= step;
                   e = motion_event;
                   break;

         case  DO: mouse_regs.x.dx += step;
                   e = motion_event;
                   break;

        }

        /* set cursor position */
        mouse_regs.x.ax=04;
        int_86(0x33,&mouse_regs,&mouse_regs);
      }

   } /* kbhit */

 else

  if (mouse_installed)
  {
    int but;

    /* check for button press and release events */

//INT 33 - MS MOUSE - RETURN BUTTON PRESS DATA
//	AX = 0005h
//	BX = button
//	    0000h left
//	    0001h right
//	    0002h middle (Mouse Systems/Logitech mouse)
//Return: AX = 1 if button pressed
//	  BX = number of times specified button has been pressed since last call
//	  CX = column at time specified button was last pressed
//	  DX = row at time specified button was last pressed

//INT 33 - MS MOUSE - RETURN BUTTON RELEASE DATA
//	AX = 0006h
//	BX = button
//	    0000h left
//	    0001h right
//	    0002h middle (Mouse Systems/Logitech mouse)
//Return: AX = 1 if button released
//	  BX = # of times specified button has been released since last call
//	  CX = column at time specified button was last released
//	  DX = row at time specified button was last released

    for(but=0; but < 3; but++)
    {
      mouse_regs.x.ax=5;
      mouse_regs.x.bx=but;
      int_86(0x33,&mouse_regs,&mouse_regs);

      if (mouse_regs.x.bx)
      { e = button_press_event;
        break;
       }

      mouse_regs.x.ax=6;
      mouse_regs.x.bx=but;
      int_86(0x33,&mouse_regs,&mouse_regs);

      if (mouse_regs.x.bx)
      { e = button_release_event;
        break;
       }

     }


    if (but < 3)
    {
      if (but == 0) *val = 1;   // left
      if (but == 1) *val = 3;   // right
      if (but == 2) *val = 2;   // middle

      //INT 16 - GET KEYBOARD SHIFT KEY STATES
      //          AH = 0002h
      //  OUTPUT: AL  bit 0    right shift
      //              bit 1    left shift
      //              bit 2    ctrl
      //              bit 3    alt

      key_regs.h.ah=0x02;
      int_86(0x16,&key_regs,&key_regs);

      // ALT key simulates middle button  (two-button mouse)
      if (key_regs.h.al & 8) *val = 2;       // alt
      if (key_regs.h.al & 3) *val = -*val;   // shift
      if (key_regs.h.al & 4) *val += 3;      // ctrl
     }

   // INT 33 MS-MOUSE GET CURSOR POSITION
   mouse_regs.x.ax=03;
   int_86(0x33,&mouse_regs,&mouse_regs);
  }

  if (mouse_regs.x.cx/8 != mouse_x || mouse_regs.x.dx/8 != mouse_y)
  { if (e==no_event) e = motion_event;
    hide_pointer();
    mouse_y = mouse_regs.x.dx/8;
    mouse_x = mouse_regs.x.cx/8;
    show_pointer();
   }

  for(i = win_top; i >= 0; i--)
  { w = win_stack[i];
    if (mouse_x >= w->x0  && mouse_x <= w->x1 &&
        mouse_y >= w->y0  && mouse_y <= w->y1 )  break;
   }

  *win = w->id;
  *x = mouse_x - w->xpos;
  *y = mouse_y - w->ypos;

  if (e != no_event)
  { last_event_e   = e;
    last_event_val = *val;
    last_event_win = w;
   }

  return e;
 }


void put_back_event()
{ event_buffer_e   = last_event_e;
  event_buffer_val = last_event_val;
  event_buffer_win = last_event_win;
 }

static void change_geometry(DosWindow win, int x0, int y0, int x1, int y1)
{
  if (x0==win->x0 && y0==win->y0 && x1==win->x1 && y1==win->y1) return;

  int w = x1-x0+1;
  int h = y1-y0+1;

  hide_pointer();
  insert_pixrect(root_win->id,win->x0,win->y0,win->image_buf);
  delete_pixrect(win->image_buf);

  win->x0 = x0;
  win->y0 = y0;
  win->x1 = x1;
  win->y1 = y1;
  win->xpos = x0+BORDER_W;
  win->ypos = y0+HEADER_W;
  win->width = w - 2*BORDER_W;
  win->height = h - HEADER_W - BORDER_W;

  win->image_buf = create_pixrect(root_win->id,win->x0,win->y0,win->x1,win->y1);
  draw_window(win);
  show_pointer();
}


static void iconize(DosWindow win)
{ Window w;
  int val,x,y;
  box(root_win->id,win->x0+14,win->y0+4,win->x0+26,win->y0+14);
  while (handle_next_event(&w,&val,&x,&y) != button_release_event);
  box(root_win->id,win->x0+14,win->y0+4,win->x0+26,win->y0+14);

  int x0 = win->save_x0;
  int y0 = win->save_y0;
  int x1 = win->save_x1;
  int y1 = win->save_y1;
  int bg = win->save_bg_col;
  win->save_x0 = win->x0;
  win->save_y0 = win->y0;
  win->save_x1 = win->x1;
  win->save_y1 = win->y1;
  win->save_bg_col = win->bg_col;
  win->bg_col =  bg;
  change_geometry(win,x0,y0,x1,y1);
  win->iconized = 1-win->iconized;

  if (win->iconized)
     insert_bitmap(win->id, leda_icon_width, leda_icon_height,
                   (char*)leda_icon_bits);
 }

static void move_win(DosWindow win, int *x, int *y)
{ int xp0 = win->x0;
  int yp0 = win->y0;
  int xp1 = win->x1;
  int yp1 = win->y1;
  int wi  = xp1-xp0+1;
  int he  = yp1-yp0+1;
  int xb  = win->xpos - xp0;
  int yb  = win->ypos - yp0;
  int xc = mouse_x; /* absolute cursor coordinates */
  int yc = mouse_y;
  int dx = xc-xp0;  /* relative to upper left corner */
  int dy = yc-yp0;

  Window w;
  int e,val;

  rectangle(root_win->id,xp0-1,yp0-1,xp1+1,yp1+1);

  do { e = handle_next_event(&w,&val,x,y);
      if (mouse_x != xc || mouse_y != yc)
       { int rx0 = mouse_x-dx-1;
         int ry0 = mouse_y-dy-1;
         int rx1 = mouse_x-dx+wi;
         int ry1 = mouse_y-dy+he;
         rectangle(root_win->id,rx0,ry0,rx1,ry1);
         rectangle(root_win->id,xc-dx-1,yc-dy-1,xc-dx+wi,yc-dy+he);
         xc = mouse_x;
         yc = mouse_y;
       }
     } while (e != button_release_event);

  xc -= dx;
  yc -= dy;

  rectangle(root_win->id,xc-1,yc-1,xc+wi,yc+he);

  if (win_top > 1)  /* move panel */
  { change_geometry(win,xc,yc,xc+wi-1,yc+he-1);
    if (win->iconized)
       insert_bitmap(win->id, leda_icon_width, leda_icon_height,
                     (char*)leda_icon_bits);
    return;
   }

  if (xc < 0) xc = 0;
  if (yc < 0) yc = 0;
  if (xc + wi > DISP_MAX_X) xc = DISP_MAX_X - wi;
  if (yc + he > DISP_MAX_Y) yc = DISP_MAX_Y - he;

  xc -= xc % 8;
  xc += xp0 % 8;

  hide_pointer();
  copy_pixrect(root_win->id,xp0,yp0+1,xp0+wi,yp0+he-1,xc,yc+1);
  show_pointer();

  set_mode(0);
  set_color(root_color);

  if (xc > xp0)
     box(root_win->id,xp0,yp0,xc-1,yp1);
  else
     box(root_win->id,xc+wi,yp0,xp1,yp1);

  if (yc > yp0)
     box(root_win->id,xp0,yp0,xp1,yc-1);
  else
     box(root_win->id,xp0,yc+he,xp1,yp1);

  set_color(1);
  rectangle(root_win->id,xc,yc,xc+wi-1,yc+he-1);

  set_mode(1);

  win->x0 = xc;
  win->y0 = yc;
  win->x1 = xc+wi-1;
  win->y1 = yc+he-1;
  win->xpos = xc+xb;
  win->ypos = yc+yb;

}


static void resize_win(DosWindow win, int* x, int* y, int pos)
{ int xp0 = win->x0;
  int yp0 = win->y0;
  int xp1 = win->x1;
  int yp1 = win->y1;
  int xb  = win->xpos - xp0;
  int yb  = win->ypos - yp0;

  int xc  = mouse_x; /* absolute cursor coordinates */
  int yc  = mouse_y;

  Window w;
  int e,val,dx,dy;

  rectangle(root_win->id,xp0-1,yp0-1,xp1+1,yp1+1);

  switch(pos) {

   case 0: dx = xp0-xc;          /* upper left */
           dy = yp0-yc;
           break;

   case 1: dx = xp1-xc;          /* upper right */
           dy = yp0-yc;
           break;

   case 2: dx = xp1-xc;          /* lower right */
           dy = yp1-yc;
           break;

   case 3: dx = xp0-xc;          /* lower left */
           dy = yp1-yc;
           break;

   }

  do { e = handle_next_event(&w,&val,x,y);
       if (mouse_x != xc || mouse_y != yc)
        {
          switch(pos) {

          case 0: rectangle(root_win->id,mouse_x+dx-1,mouse_y+dy-1,xp1+1,yp1+1);
                  rectangle(root_win->id,xc+dx-1,yc+dy-1,xp1+1,yp1+1);
                  break;

          case 1: rectangle(root_win->id,xp0-1,mouse_y+dy-1,mouse_x+dx+1,yp1+1);
                  rectangle(root_win->id,xp0-1,yc+dy-1,xc+dx+1,yp1+1);
                  break;

          case 2: rectangle(root_win->id,xp0-1,yp0-1,mouse_x+dx+1,mouse_y+dy+1);
                  rectangle(root_win->id,xp0-1,yp0-1,xc+dx+1,yc+dy+1);
                  break;

          case 3: rectangle(root_win->id,mouse_x+dx-1,yp0-1,xp1+1,mouse_y+dy+1);
                  rectangle(root_win->id,xc+dx-1,yp0-1,xp1+1,yc+dy+1);
                  break;

           }

          xc = mouse_x;
          yc = mouse_y;
        }
     } while (e != button_release_event);

   xc += dx;
   yc += dy;

   switch(pos) {

   case 0: win->x0 = xc;
           win->y0 = yc;
           break;

   case 1: win->x1 = xc;
           win->y0 = yc;
           break;

   case 2: win->x1 = xc;
           win->y1 = yc;
           break;

   case 3: win->x0 = xc;
           win->y1 = yc;
           break;

   }

  win->xpos = win->x0 + xb;
  win->ypos = win->y0 + yb;
  win->width  = win->x1 - win->x0 - 2*xb + 1;
  win->height = win->y1 - win->y0 - xb - yb + 1;

  rectangle(root_win->id,win->x0-1,win->y0-1,win->x1+1,win->y1+1);

  hide_pointer();
  insert_pixrect(root_win->id,xp0,yp0,win->image_buf);
  delete_pixrect(win->image_buf);
  win->image_buf = create_pixrect(root_win->id,win->x0,win->y0,win->x1,win->y1);
  draw_window(win);
  show_pointer();

  *x = win->width;
  *y = win->height;

}


int check_next_event(Window* w, int* val, int* x, int *y, unsigned long* t)
{
  // non-blocking

  // a primitive window manager active while searching for next event

  int cx[4];
  int cy[4];
  int bw = BORDER_W;
  int lw = HEADER_W;

  int e = handle_next_event(w,val,x,y);


  if (*w != active_win->id)
  {  hide_pointer();
     active_win->label_col = label_col0;
     if (active_win->id == win_top) draw_window(active_win,0);
     active_win = win_stack[*w];
     active_win->label_col = label_col1;
     if (active_win->id == win_top) draw_window(active_win,0);
     show_pointer();
   }


  DosWindow win = win_stack[*w];

  int x0 = win->x0;
  int y0 = win->y0;
  int x1 = win->x1;
  int y1 = win->y1;


  cx[0] = x0;
  cy[0] = y0;
  cx[1] = x1-6;
  cy[1] = y0;
  cx[2] = x1-6;
  cy[2] = y1-6;
  cx[3] = x0;
  cy[3] = y1-6;

  if ( (x0 <= mouse_x && mouse_x <= x0+bw)
    || (x1 >= mouse_x && mouse_x >= x1-bw)
    || (y0 <= mouse_y && mouse_y <= y0+lw)
    || (y1 >= mouse_y && mouse_y >= y1-bw))
    {
      // pointer on window boundary: move or resize window

     if (win != win_stack[win_top])  /* only top window can be changed  */
     { set_pointer_shape(0);
       return no_event;
      }

     for(int i=0; i<4; i++)
       if ( cy[i] <= mouse_y && mouse_y <= cy[i]+6 &&
            cx[i] <= mouse_x && mouse_x <= cx[i]+6 )
       { set_pointer_shape(1);
         if (e == button_press_event && ! win->iconized)
         { if (win->iconized)
             e = no_event;
           else
             resize_win(win,x,y,i);
           e = configure_event;
          }
          return e;
        }

    set_pointer_shape(0);

    if (x0+13 <= mouse_x && mouse_x <= x0+27 &&
        y0+3  <= mouse_y && mouse_y <= y0+15 && e == button_press_event)
    { iconize(win);
      return (win->iconized) ? no_event : configure_event;
     }


    if (e== button_press_event)
    { move_win(win,x,y);
      if (win_top==1 || win->iconized)
         return handle_next_event(w,val,x,y);
      else
         return configure_event;
     }

    e = no_event;

  }  // if pointer on window boundary

  set_pointer_shape(0);

  return e;
}


int get_next_event(Window* win, int* val, int* x, int *y, unsigned long* t)
{ // blocking
  int e = no_event;
  while (e == no_event) e = check_next_event(win,val,x,y,t);
  return e;
 }



void open_display()
{ REGISTERS regs;

  if (display) return;

  display = 1;

  init_graphics(1,root_color);

  //INT 33 - MS MOUSE - RESET DRIVER AND READ STATUS
  //    AX = 0000h
  //Return: AX = status
  //	    0000h hardware/driver not installed
  //	    FFFFh hardware/driver installed
  //	    BX = number of buttons
  //	    FFFFh two buttons
  //	    0000h other than two
  //	    0003h Mouse Systems/Logitech mouse
  regs.x.ax=0x00;
  int_86(0x33,&regs,&regs);
  mouse_installed = (regs.x.ax == 0x0ffff);
  mouse_two_buttons = (regs.x.bx == 0x0ffff);

  //INT 33 - MS MOUSE - DEFINE HORIZONTAL CURSOR RANGE = 0..DISP_MAX_X
  regs.x.ax=0x07;
  regs.x.cx=0;
  regs.x.dx=8*DISP_MAX_X;
  int_86(0x33,&regs,&regs);

  //INT 33 - MS MOUSE - DEFINE VERTICAL CURSOR RANGE = 0..DISP_MAX_Y
  regs.x.ax=0x08;
  regs.x.cx=0;
  regs.x.dx=8*DISP_MAX_Y;
  int_86(0x33,&regs,&regs);

  //INT 33 - MS MOUSE - DEFINE MICKEY/PIXEL RATIO = 1
  regs.x.ax=0x0F;
  regs.x.cx=1;
  regs.x.dx=1;
  int_86(0x33,&regs,&regs);

  //INT 33 - MS MOUSE - SET CURSOR POSITION
  mouse_x = DISP_MAX_X/2;
  mouse_y = DISP_MAX_Y/2;
  regs.x.cx = 8*mouse_x;
  regs.x.dx = 8*mouse_y;
  regs.x.ax=04;
  int_86(0x33,&regs,&regs);

  root_win = new dos_window;
  active_win = root_win;
  root_win->id = 0;
  win_stack[0] = root_win;
  win_top = 0;

  root_win->x0 = -1000;
  root_win->y0 = -1000;
  root_win->x1 =  1000;
  root_win->y1 =  1000;
  root_win->xpos = 0;
  root_win->ypos = 0;
  root_win->width  = display_width();
  root_win->height = display_height();
  root_win->bg_col = root_color;

  set_color(root_color);
  box(root_win->id,0,0,DISP_MAX_X,DISP_MAX_Y);
  set_color(black);
}


void close_display()
{ init_graphics(0,0);
  delete root_win;
 }


int  window_width(Window win)  { return win_stack[win]->width; }
int  window_height(Window win) { return win_stack[win]->height; }

void window_position(Window win,int* x,int* y)
{ *x = win_stack[win]->x0;
  *y = win_stack[win]->y0;
 }
