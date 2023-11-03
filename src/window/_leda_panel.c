/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  _leda_panel.c
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/


// defines the LEDA_PANEL operations declared in <LEDA/leda_panel.h>
// using the basic graphics routines from <LEDA/impl/x_basic.h>

#include <LEDA/leda_panel.h>
#include <LEDA/impl/x_basic.h>

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

enum { Text_Item, 
       String_Item, 
       String_Menu_Item, 
       Int_Item, 
       Slider_Item, 
       Float_Item, 
       Button_Item, 
       Choice_Item,
       Bool_Item,
       Color_Item };


static char* duplicate_string(const char* p)
{ char* q = new char[strlen(p)+1];
  if (q==0) 
  { fprintf(stderr,"duplicate_string: out of memory");
    abort();
   }
  strcpy(q,p);
  return q;
}

static char* make_string(int x)
{ char str[256];
  sprintf(str,"%d",x);
  return duplicate_string(str);
 }

static char* make_string(double x)
{ char str[256];
  sprintf(str,"%f",x);
  return duplicate_string(str);
 }


void LEDA_PANEL::panel_redraw_func() 
{ if (active_window)
  { LEDA_PANEL* p = (LEDA_PANEL*)active_window; 
    p->open(p->XPOS, p->YPOS, 0, 0, 0, 0, 0); 
   }
 }


void LEDA_PANEL::init(const char* s)
{ open_display();
  header = duplicate_string(s);
  XPOS = -1;
  YPOS = -1;
  but_layout = 0;
  item_count = 0;
  but_count = 0;
  win = 0;

  item_bg_color  = white;
  bg_color       = grey1;
  press_color    = grey2;
  shadow_color   = grey3;


  tw = text_width("H");
  th = text_height("H");
  ytskip   = th + 2;
  yskip    = th + 11;
  bskip    = th;
  yoff     = th / 2;
  xoff     = tw + 2;
  last_ycoord = yoff;

  slider_h = th - 1;
  string_h = th + 2;
  button_h = th + 4;
  choice_h = th + 5;

  slider_w  = 200; // minimal slider length
  string_w  = 200; // minimal string/int/float item length
  choice_w  = 35;  // minimal choice field width
  button_w  = 40;  // minimal button width

  but_per_line = 0;  
 }


LEDA_PANEL::LEDA_PANEL(const char* s, int bl)
{ WIDTH  = -1;
  HEIGHT = -1;
  init(s);
  but_per_line = (bl == 1) ? 1 : 0;
}

LEDA_PANEL::LEDA_PANEL(const char* s, int width, int height)
{ WIDTH  = width;
  HEIGHT = height;
  init(s);
 }

LEDA_PANEL::LEDA_PANEL(int width, int height)
{ WIDTH  = width;
  HEIGHT = height;
  init("");
 }

void LEDA_PANEL::item_error()
{ fprintf(stderr,"Sorry, too many items."); 
  exit(1);
 }

void LEDA_PANEL::text_item(const char* s)
{ if (item_count >= MAX_ITEM_NUM) item_error();
  kind[item_count] = Text_Item;
  label_str[item_count] = duplicate_string(s);
  data_str[item_count] = 0;
  ycoord[item_count] = last_ycoord;
  last_ycoord += ytskip;
  item_count++;
}


void LEDA_PANEL::string_item(const char* s, char** x)
{ if (item_count >= MAX_ITEM_NUM) item_error();
  label_str[item_count] = duplicate_string(s);
  data_str[item_count] = duplicate_string(*x);
  ref[item_count] = x;
  kind[item_count] = String_Item;
  ycoord[item_count] = last_ycoord;
  last_ycoord += yskip;
  item_count++;
 }


void LEDA_PANEL::string_menu_item(const char* s, char** x, const char* ,
                                  int argc,const char** argv)
{ 
  if (item_count >= MAX_ITEM_NUM) item_error();
  label_str[item_count] = duplicate_string(s);
  data_str[item_count] = duplicate_string(*x);
  ref[item_count] = x;
  kind[item_count] = String_Menu_Item;
  dat1[item_count] = argc;
  choices[item_count] = new char*[argc];
  for(int i = 0; i < argc; i++) 
     choices[item_count][i] = duplicate_string(argv[i]);
  ycoord[item_count] = last_ycoord;
  last_ycoord += yskip;
  item_count++;
 }


void LEDA_PANEL::int_item(const char* s, int* x)
{ if (item_count >= MAX_ITEM_NUM) item_error();
  label_str[item_count] = duplicate_string(s);
  data_str[item_count] = make_string(*x);
  ref[item_count] = x;
  kind[item_count] = Int_Item;
  ycoord[item_count] = last_ycoord;
  last_ycoord += yskip;
  item_count++;
 }

void LEDA_PANEL::float_item(const char* s, double* x)
{ if (item_count >= MAX_ITEM_NUM) item_error();
  label_str[item_count] = duplicate_string(s);
  data_str[item_count] = make_string(*x);
  ref[item_count] = x;
  kind[item_count] = Float_Item;
  ycoord[item_count] = last_ycoord;
  last_ycoord += yskip;
  item_count++;
 }

void  LEDA_PANEL::slider_item(const char* s, int* x, int min, int max, action_func F)
{ if (item_count >= MAX_ITEM_NUM) item_error();
  label_str[item_count] = duplicate_string(s);
  data_str[item_count] = make_string(*x);
  ref[item_count] = x;
  dat1[item_count] = min;
  dat2[item_count] = max;
  kind[item_count] = Slider_Item;
  ycoord[item_count] = last_ycoord;
  action[item_count] = F;
  last_ycoord += yskip;
  item_count++;
 }

void LEDA_PANEL::choice_item(const char* s, int* address, int argc,
                             const char** argv, int step, int off)
{ 
  if (item_count >= MAX_ITEM_NUM) item_error();
  label_str[item_count] = duplicate_string(s);
  data_str[item_count] = 0;
  kind[item_count] = Choice_Item;
  ref[item_count] = address;
  dat1[item_count] = argc;
  dat2[item_count] = step;
  offset[item_count] = off;
  choices[item_count] = new char*[argc];
  for(int i=0; i<argc; i++) choices[item_count][i] = duplicate_string(argv[i]);
  ycoord[item_count] = last_ycoord;
  last_ycoord += yskip;
  item_count++;
 }


void LEDA_PANEL::bool_item(const char* s, char* address)
{ if (item_count >= MAX_ITEM_NUM) item_error();
  label_str[item_count] = duplicate_string(s);
  data_str[item_count] = 0;
  kind[item_count] = Bool_Item;
  ref[item_count] = address;
  dat1[item_count] = 2;
  dat2[item_count] = 1;
  offset[item_count] = 0;
  choices[item_count] = new char*[2];
  choices[item_count][0] = duplicate_string("off");
  choices[item_count][1] = duplicate_string("on");
  ycoord[item_count] = last_ycoord;
  last_ycoord += yskip;
  item_count++;
 }


void LEDA_PANEL::color_item(const char* s, int* x)
{ if (item_count >= MAX_ITEM_NUM) item_error();
  label_str[item_count] = duplicate_string(s);
  data_str[item_count] = 0;
  ref[item_count] = x;
  if (display_depth() > 1)
     kind[item_count] = Color_Item;
  else 
   { // choice item [white][black]
     kind[item_count] = Choice_Item;
     dat1[item_count] = 2;
     dat2[item_count] = 1;
     offset[item_count] = 0;
     choices[item_count] = new char*[2];
     choices[item_count][0] = duplicate_string("white");
     choices[item_count][1] = duplicate_string("black");
    }
  ycoord[item_count] = last_ycoord;
  last_ycoord += yskip;
  item_count++;
 }



int LEDA_PANEL::button(const char* s)
{ if (but_count == MAX_BUT_NUM) return -1;
  // space before first line of buttons
  if (but_count==0 && item_count>0)  text_item(""); 
  button_str[but_count] = duplicate_string(s);
  return but_count++;
 }


void  LEDA_PANEL::button_line(int n, const char** b) 
{ for(int i=0; i<n; i++) button(b[i]); }


void LEDA_PANEL::draw_box_with_shadow(int x1,int y1,int x2,int y2,int c,int w)
{ // below and right of box(x1,y1,x2,y2)
  ::set_color(shadow_color);
  box(win,x1+3,y1+3,x2+w,y2+w);
  ::set_color(c);
  box(win,x1,y1,x2,y2);
  ::set_color(black);
  rectangle(win,x1,y1,x2,y2);
 }


void LEDA_PANEL::draw_d3_box(int x1,int y1,int x2,int y2, int pressed)
{
  ::set_color(bg_color);
  box(win,x1,y1,x2,y2);

   //if (pressed) { x1++; y1++; x2++; y2++; }
   if (pressed) { x1++; y1++; }

   int c1 = (pressed) ? shadow_color : white;
   int c2 = (pressed) ? white        : shadow_color;
   int c3 = (pressed) ? press_color  : bg_color;


   ::set_color(c3);
   box(win,x1,y1,x2,y2);

   ::set_color(c1);
   line(win,x1,y1,x2,y1);
   line(win,x1,y1,x1,y2);

   ::set_color(c2);
   line(win,x1+1,y2,x2,y2);
   line(win,x2,y1+1,x2,y2);

  ::set_color(black);
}



void LEDA_PANEL::draw_string_item(int i,int active, const char* s)
{ 
  int  yt = ycoord[i] + (yskip-th)/2;
  put_text(win,xoff,yt,label_str[i],0);

  int x1 = xoff1;
  int x2 = x1 + string_w;
  int y1 = yt - (string_h-th)/2 - 1;
  int y2 = yt + string_h + (string_h-th)/2;
  
  if (s == 0)  s = data_str[i];

  int max_c = string_w/tw - 1;
  int d = strlen(s) - max_c;

  if (active && d > 0) s += d;

  if (bg_color == white)
    { ::set_color(bg_color);
      box(win,x1,y1,x2,y2);
      ::set_color(black);
      rectangle(win,x1,y1,x2,y2);
     }
  else
    draw_d3_box(x1,y1,x2,y2,active);

  // write text into box
  ::set_color(black);
  put_text(win,x1+4,yt,s,max_c,0);

  if (active) put_text(win,x1+4+strlen(s)*tw,yt,"|",0);
}




void LEDA_PANEL::activate_string_item(int n)
{ if (n == act_str_item) return;
  if (act_str_item > -1) 
    draw_string_item(act_str_item);
  draw_string_item(n,1);
  act_str_item = n;
 }
   

void LEDA_PANEL::panel_text_edit(int n)
{ 
  int h  = yskip-8;
  int y  = ycoord[n] + 4;
  int yt = y + (h - th)/2 + 1;
  int max_c = string_w/tw - 1;

  char  str[128];
  strcpy(str,data_str[n]);
  int j = strlen(str);

  for(;;)
  { int  k,val;
    char c = 13;

    ::set_read_gc();

    while(1)
    { Window w;
      int xc,yc;
      unsigned long t;
      k = get_next_event(&w,&val,&xc,&yc,&t);
      if (w != win) continue;
      if (k == key_press_event || k == button_press_event) break;
     }

     reset_gc();

     if (k == key_press_event) c = val;
     if (c == 13) 
     { put_back_event();
       break;
      }

     if (isprint(c))
     { str[j++]=c;
       str[j] = '\0';
      }

     if(c==8 && j>0) str[--j] = '\0';

     draw_string_item(n,1,str);
   }

  delete data_str[n];
  data_str[n] = duplicate_string(str);
}


void LEDA_PANEL::draw_choice_bool_item(int i, int x, int y, int c, int n)
{
  ::set_color(bg_color);
  box(win,x,y,x+n*choice_w,y+choice_h);

  int j;

  if (bg_color != white)
     for(j=0; j<n; j++)
     { draw_d3_box(x,y,x+choice_w-1,y+choice_h,j==c);
       ::set_color(black);
       put_ctext(win,x+choice_w/2,y+choice_h/2,choices[i][j],0);
       x += choice_w;
      }
  else
    for(j=0; j<n; j++)
    { ::set_color(item_bg_color);
      box(win,x+1,y+1,x+choice_w-1,y+choice_h-1);
      ::set_color(black);
      put_ctext(win,x+choice_w/2,y+choice_h/2,choices[i][j],0);
      if (bg_color == item_bg_color) rectangle(win,x,y,x+choice_w,y+choice_h);
      if(j == c) 
      { rectangle(win,x,y,x+choice_w,y+choice_h);
        rectangle(win,x+1,y+1,x+choice_w-1,y+choice_h-1);
       }
      x += choice_w;
     }

}



void LEDA_PANEL::draw_choice_item(int i)
{ 
  int yt = ycoord[i] + (yskip-th)/2;
  put_text(win,xoff,yt,label_str[i],0);

  int y = yt - (choice_h-th)/2 - 1;
  int c = (*(int*)ref[i]-offset[i])/dat2[i];

  draw_choice_bool_item(i,xoff1,y,c,dat1[i]);
}


#if !(defined(__GNUC__) && __GNUC_MINOR__ > 5)
typedef char bool;
#endif


void LEDA_PANEL::draw_bool_item(int i)
{ 
  int yt = ycoord[i] + (yskip-th)/2 + 1;
  put_text(win,xoff,yt,label_str[i],0);

  int y = yt - (choice_h-th)/2;
  int c = (*(bool*)ref[i]-offset[i])/dat2[i];

  draw_choice_bool_item(i,xoff1,y,c,dat1[i]);
}


void LEDA_PANEL::change_color_item(int i, int j)
{ int yt = ycoord[i] + (yskip-th)/2 + 1;
  int y = yt - (choice_h-th)/2 - 1;
  int old = *(int*)ref[i];

  *(int*)ref[i] = j;

  char col_str[8];

  int x = xoff1 + j*choice_h;
  sprintf(col_str,"%d",j);
  press_color = j;
  draw_d3_box(x,y,x+choice_h-1,y+choice_h-1,1);
  ::set_color((j==black) ? white : black);
  put_ctext(win,x+choice_h/2,y+choice_h/2,col_str,0);

  j = old;
  x = xoff1 + j*choice_h;
  sprintf(col_str,"%d",j);
  press_color = grey2;
  draw_d3_box(x,y,x+choice_h-1,y+choice_h-1,0);
  ::set_color((j==bg_color) ? black : j);
  put_ctext(win,x+choice_h/2,y+choice_h/2,col_str,0);

  ::set_color(black);
}


void LEDA_PANEL::draw_color_item(int i)
{ 
  int yt = ycoord[i] + (yskip-th)/2 + 1;
  int x = xoff1;
  int y = yt - (choice_h-th)/2 - 1;

  put_text(win,xoff,yt,label_str[i],0);

  char c = *(int*)ref[i];


  ::set_color(bg_color);
  box(win,x,y,x+16*choice_h,y+choice_h);

  for(int j=0; j < 16; j++)
  { char col_str[8];
    sprintf(col_str,"%d",j);
    press_color = j;
    draw_d3_box(x,y,x+choice_h-1,y+choice_h-1,j==c);
    if (j != c)
      ::set_color((j==bg_color) ? black : j);
    else
      ::set_color((j==black) ? white : black);
    put_ctext(win,x+choice_h/2,y+choice_h/2,col_str,0);
    x += choice_h;
   }
  press_color = grey2;
  ::set_color(black);

  //end_group(win);
}



void LEDA_PANEL::draw_slider_item(int i, float x)
{ 
  int x0 = xoff1;
  int x1 = x0 + slider_w;
  int y0 = ycoord[i] + (yskip-slider_h)/2 + 1;
  int y1 = y0 + slider_h;
  int yt = ycoord[i] + (yskip-th)/2 + 1;

  if (x == 0) // first time
  { float d = float(slider_w)/(dat2[i]-dat1[i]);
    x = xoff1 + d * (*(int*)ref[i]-dat1[i]);
    put_text(win,xoff,yt,label_str[i],0);
   }

  float mi = dat1[i];
  float ma = dat2[i];

  if (x < x0) x = x0;
  if (x > x1) x = x1;

  int val = int(mi + (ma-mi)*(x-x0)/slider_w + 0.5);

  if (action[i]) action[i](val);

  *(int*)ref[i] = val;

  // text
  char text[16];
  sprintf(text,"%4d",val);
  int t_w = text_width(text) + 3;
  int t_h = text_height(text);
  ::set_color(bg_color);
  box(win,x0-t_w,y0,x0,y0+t_h);
  ::set_color(black);
  put_text(win,x0-t_w,yt,text,0);

  // slider
  if (bg_color == white)
    { ::set_color(black);
      box(win,x0,y0,int(x+0.5),y1);
      ::set_color(white);
      box(win,int(x+0.5),y0,x1,y1);
      ::set_color(black);
      rectangle(win,x0,y0,x1,y1);
     }
  else
    { draw_d3_box(int(x+0.5),y0,x1,y1,1);
      draw_d3_box(x0,y0,int(x+0.5)+1,y1);
     }
  ::set_color(black);

}


void LEDA_PANEL::draw_button(const char* s, int x, int y, int pressed)
{ 
   int x1 = x+1;
   int y1 = y+4;
   int x2 = x1 + button_w;
   int y2 = y1 + button_h + 1;
   int yt = y1 + button_h/2;
   int xt = x1 + button_w/2;

   if (bg_color == white)
     { draw_box_with_shadow(x1,y1,x2,y2,white,3);
       if (pressed)
       { box(win,x1+1,y1+1,x2-1,y2-1);
         ::set_color(white);
        }
       put_ctext(win,xt,yt,s,0);
      }
   else
     { draw_d3_box(x1,y1,x2,y2,pressed);
       ::set_color(black);
       put_ctext(win,xt,yt,s,0);
      }
   ::set_color(black);
   flush_display();
}





void LEDA_PANEL::draw_menu_button(int x, int y, int pressed)
{ 
  int x1 = x;
  int y1 = y+4;
  int x2 = x1+string_h;
  int y2 = y1+string_h;

  ::set_color(bg_color);
  box(win,x1,y1,x2,y2);

  if (bg_color == item_bg_color)
  { ::set_color(black);
    rectangle(win,x1,y1,x2,y2);
  }

  if (pressed) 
  { ::set_color(black);
    box(win,x1+1,y1+1,x2-1,y2-1);
   }

  y1 += 3;
  y2 -= 3;
  x1 += 4;

  ::set_color(shadow_color);
  line(win,x1,y1,x1,y2-1);
  x1++;
  y1++;
  y2--;

  ::set_color(item_bg_color);
  while (y1 <= y2)
  { ::set_color(item_bg_color);
    line(win,x1,y1,x1,y2);
    x1++;
    line(win,x1,y1,x1,y2);
    ::set_color(shadow_color);
    pixel(win,x1-1,y1);
    pixel(win,x1,  y1);
    ::set_color(white);
    pixel(win,x1-1,y2);
    pixel(win,x1,  y1);
    x1++;
    y1++;
    y2--;
   }

  flush_display();
}


static int read_panel_at(const char* header, int n, char** but,int x, int y)
{ LEDA_PANEL p(header);
  for(int i=0; i<n; i++) p.button(but[i]);
  return p.open(x,y,0,0,0,0,3);
 }


int  LEDA_PANEL::open(int xpos, int ypos, int win_x, int win_y, 
                      int win_width, int win_height, int mode)
{


  // mode = 0:  just display panel window  on screen
  // mode = 1:  display, read (blocking), and close 
  // mode = 2:  read
  // mode = 3:  display, read (non-blocking) , and close

  if (display_depth() == 1)
  { bg_color = white;   
    shadow_color = black;   
    item_bg_color = white;   
   }

repaint:

  slider_w  = 200; // minimal slider length
  string_w  = 200; // minimal string/int/float item length
  choice_w  = 30;  // minimal choice field width
  button_w  = 40;  // minimal button width
  xoff1 = 0;       // start of items
  bxoff = 0;       // left and right button boundary space

  int width = (win) ? ::window_width(win) : 100;
  int height; 

  int   bw1;              /* button_w + bskip   */
  int   but_lines;
  int   but,w,i,j;

  unsigned long t;

  int user_buttons = but_count;
  int save_lw = ::set_line_width(1);
  int save_ls = ::set_line_style(0);
  int save_mo = ::set_mode(0);

  if (but_count==0)
  { button("CONTINUE");
    button("QUIT");
   }


  height = 2*yoff;

  for(i=0;i<item_count; i++)
    if (kind[i] != Text_Item)
       { height += yskip;
         if ((w = text_width(label_str[i])+50) > xoff1) xoff1 = w;
 
         if (kind[i] == Choice_Item || kind[i] == Bool_Item)
         { int j;
           for(j=0; j<dat1[i];j++)
           if ((w = text_width(choices[i][j])+10) > choice_w) choice_w = w;
          }
       }
    else
       height += ytskip;


  // adjust width of buttons

  for(i=0; i < but_count; i++)
     if ((w = text_width(button_str[i])+10) > button_w) button_w = w;

  bw1 = button_w + bskip;




  // adjust panel width

  if ((w = text_width(header)) > width) width = w;

  if ((w = but_per_line*bw1+2*xoff) > width) width = w;

  for(i=0; i<item_count; i++)
    switch (kind[i])
    {
      case  Text_Item:
                  if ((w = 2*xoff + text_width(label_str[i])) > width) width=w;
                  break;

      case Choice_Item:
                 if ((w = xoff1+choice_w*dat1[i]+yskip+xoff) > width) width = w;
                 break;

      case Color_Item:
                 if ((w = xoff1+16*choice_h+yskip+xoff) > width) width = w;
                 break;

      case Slider_Item:
                 if ((w = xoff1+slider_w+yskip+xoff) > width) width = w;
                 break;

      case String_Menu_Item:
                 if ((w = xoff1+string_w+2*yskip+xoff) > width) width = w;
                 break;


      default:   if ((w = xoff1+string_w+yskip+xoff) > width) width = w;
                 break;
     }

  // adjust width of slider & string boxes

  w = width-xoff1-2*yskip;
  if (w > slider_w) slider_w = w;
  if (w > string_w) string_w = w;


  // center buttons

  if (but_per_line == 0)
  { but_per_line = (width-2*xoff+bskip)/bw1;
    if (but_per_line == 0)
    { but_per_line = 1;
      width = button_w + 2*xoff;
     }
   }
    

  but_lines = but_count/but_per_line;

  if (but_count % but_per_line) but_lines++;

  if (but_lines == 1)
     bxoff = (width - but_count * bw1 + bskip)/2;
  else
     bxoff = (width - but_per_line * bw1 + bskip)/2;


  height += but_lines * yskip;
  height += yoff;

  for (i=0; i< but_lines; i++)
     kind[item_count+i] = Button_Item;


  if (WIDTH  == -1)  WIDTH  = width;
  if (HEIGHT == -1)  HEIGHT = height;


  if(XPOS == -1)
    if (xpos == -1)
      if (win_width == 0)
         { /* center panel window on the screen */
           XPOS = (display_width() - WIDTH)/2;
           YPOS = (display_height() - HEIGHT)/2;
          }
      else
         { /* center panel window on draw window */
           XPOS = win_x + (win_width - WIDTH)/2;
           YPOS = win_y + (win_height- HEIGHT)/2;
          }
    else /* use supplied coordinates */
      { XPOS = xpos;
        YPOS = ypos;
       }


  if (win==0) 
  { LEDA_WINDOW::open(WIDTH,HEIGHT,XPOS,YPOS,header,bg_color);
    set_show_coordinates(0);
    set_redraw(panel_redraw_func);
    win = draw_win;
   }

  //clear_window(win);

  act_str_item = -1;

  ::set_color(1);

  int y = yoff;

  for(i=0;i<item_count; i++)
  {
    switch (kind[i]) {

    case Text_Item:
        { put_text(win,xoff,ycoord[i],label_str[i],0);
          break;
         }

    case Choice_Item:
        { draw_choice_item(i);
          break;
         }

    case Bool_Item:
        { draw_bool_item(i);
          break;
         }

    case Color_Item:
        { draw_color_item(i);
          break;
         }

    case Slider_Item:
        { draw_slider_item(i,0);
          break;
         }

    case Int_Item:
        { draw_string_item(i);
          if (act_str_item == -1) activate_string_item(i);
          break;
         }
 
    case Float_Item:
        { draw_string_item(i);
          if (act_str_item == -1) activate_string_item(i);
          break;
         }
  
    case String_Item:
        { draw_string_item(i);
          if (act_str_item == -1) activate_string_item(i);
          break;
         }

    case String_Menu_Item:
        { draw_string_item(i);
          draw_menu_button(xoff1+string_w+6,ycoord[i],0);
          if (act_str_item == -1) activate_string_item(i);
          break;
         }
   }

   y += ((kind[i] == Text_Item) ?  ytskip : yskip);

  }


  // place buttons

  //int y = ycoord[item_count-1];
  y -= yskip;
  int x = bxoff;

  for(i=0; i < but_count; i++)
  { if (i % but_per_line == 0)
    { y += yskip;
      x = bxoff;
     }
    draw_button(button_str[i],x,y,0);
    x += bw1;
  }

  but = -1;

  if (mode == 0) goto end;


  while(but == -1)
  { int b;
    i = -1;

    while (i < 0 || i >= item_count+but_lines)
    { int k;
      ::set_read_gc();
      while (1)
      { Window w;
        k = get_next_event(&w,&b,&x,&y,&t);
        if (w == win)
        { if (k == button_press_event || k == key_press_event) break;
          if (k == configure_event) 
          { reset_gc();
            goto repaint;
           }
         }
        else
          if (mode == 3 && k==button_press_event) 
          { put_back_event();
            reset_gc();
            goto end;
           }
       }
      reset_gc();

      if (k==button_press_event)
        { for(i=0,j=yoff;i<item_count+but_lines && j<y; i++)
            if (kind[i] == Text_Item)
               j += ytskip;
            else
               j += yskip;
  
           if (j >=y) i--;
         }
       else  /* key pressed */
       if (act_str_item > -1)
       { if (b == 13) // return key pressed
         { j = act_str_item;
           for(;;)
           { j = (j + 1) % item_count;
             k =  kind[j]; 
             if (k==String_Item || k==String_Menu_Item || 
                 k==Int_Item || k == Float_Item) break;
             }
           activate_string_item(j);
         }
         else
         { x = xoff1;
           i = act_str_item;
           for(k=0,j=yoff; k <= i; k++)
            if (kind[k] == Text_Item)
               j += ytskip;
            else
               j += yskip;
           put_back_event();
          }
       }
     }

    y  = j-yskip;

    switch (kind[i]) {

    case Text_Item: break;

    case Slider_Item:
    { Window w = win;
      int xx = 0;
      if (x < xoff1 || x > xoff1+slider_w) break;
      ::set_read_gc();
      while(w == win)
      { if (xx != x)
        { xx = x;
          reset_gc();
          draw_slider_item(i,xx);
          ::set_read_gc();
         }
        if (get_next_event(&w,&b,&x,&j,&t) == button_release_event) break;
       }
      reset_gc();
      break;
     }

    case Int_Item:
    { activate_string_item(i);
      panel_text_edit(i);
      int x = atoi(data_str[i]);
      delete data_str[i];
      data_str[i] = make_string(x);
      *(int*)ref[i] = x;
      break;
     }

    case Float_Item:
    { activate_string_item(i);
      panel_text_edit(i);
      double x = (double)atof(data_str[i]);
      delete data_str[i];
      data_str[i] = make_string(x);
      *(double*)ref[i] = x;
      break;
     }

    case String_Item:
    { activate_string_item(i);
      panel_text_edit(i);
      delete *(char**)ref[i];
      *(char**)ref[i] = duplicate_string(data_str[i]);
      break;
     }

    case String_Menu_Item:
    { activate_string_item(i);
      if (x < xoff1+string_w+10)
         { panel_text_edit(i);
           delete *(char**)ref[i];
           *(char**)ref[i] = duplicate_string(data_str[i]);
          }
      else
         { Window w;
           draw_menu_button(xoff1+string_w+6,y,1);
           while (get_next_event(&w,&b,&x,&j,&t) != button_release_event);
           draw_menu_button(xoff1+string_w+6,y,0);
           int sel = read_panel_at(label_str[i],dat1[i],choices[i],
                                   XPOS+xoff1+string_w+16, YPOS+y+24);
           if (sel > -1)
           { delete *(char**)ref[i];
             *(char**)ref[i] = duplicate_string((choices[i])[sel]);
             data_str[i] = duplicate_string((choices[i])[sel]);
             draw_string_item(i,1);
            }
          }
      break;
    }


   case Choice_Item:
   { j = (x-xoff1)/choice_w;
     if (j >= 0 && j<dat1[i])
     { *(int*)ref[i] = offset[i] + j * dat2[i];
       draw_choice_item(i);
      }
     break;
    }

   case Bool_Item:
   { j = (x-xoff1)/choice_w;
     if (j >= 0 && j<dat1[i])
     { *(char*)ref[i] = offset[i] + j * dat2[i];
       draw_bool_item(i);
      }
     break;
    }

   case Color_Item:
   { j = (x-xoff1)/choice_h;
     if (0 <= j && j <= 15) change_color_item(i,j);
     break;
    }

   case Button_Item:
   { j = (x-bxoff)/bw1;
     i = (i-item_count)*but_per_line +j;
     if (x > bxoff && j < but_per_line && i < but_count)
     { Window w;
       draw_button(button_str[i],bxoff+j*bw1,y,1);
       while (get_next_event(&w,&b,&b,&b,&t) != button_release_event);
       but = i;
       //if (mode == 2) draw_button(button_str[i],bxoff+j*bw1,y,1);
      }
     break;
    }

   }

   if (mode == 2) break; // no button click neccessary

 } // while but == -1


 if (mode == 2) // redisplay  (mode = 0)
     open(XPOS,YPOS,0,0,0,0,0);

end:
/*
  window_position(win,&(XPOS),&(YPOS));
*/

  ::set_line_width(save_lw);
  ::set_line_style(save_ls);
  ::set_mode(save_mo);

  if (user_buttons == 0)
  { but_count = 0;
    item_count--;
    if (but == 1)   /* quit button pressed */
    { close_display();
      exit(0);
     }
   }

  if (mode == 1 || mode == 3) close();

  return but;
}


void LEDA_PANEL::display(int xpos, int ypos, int win_x, int win_y, int win_width, int win_height)
{ open(xpos, ypos, win_x, win_y, win_width, win_height, 0); }

/*
int  LEDA_PANEL::read(int xpos, int ypos, int win_x, int win_y, int win_width, int win_height)
{ return open(xpos, ypos, win_x, win_y, win_width, win_height, 2); }
*/

int  LEDA_PANEL::read() { return open(XPOS,YPOS,0,0,0,0,2); }



void  LEDA_PANEL::close() 
{ LEDA_WINDOW::close();
  win = 0;
 }



LEDA_PANEL::~LEDA_PANEL() 
{ delete header; 
  for(int i = 0; i<item_count; i++)
  { if (label_str[i]) delete label_str[i];
    if (data_str[i]) delete data_str[i];
    if (kind[i] == String_Menu_Item || 
        kind[i] == Choice_Item || 
        kind[i] == Bool_Item    )
      { for(int j = 0; j < dat1[i]; j++) delete choices[i][j];
        delete choices[i];
       }
   }
  for(int j = 0; j<but_count; j++) delete button_str[j];
}


