/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  doswin.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany
+  All rights reserved.
+
*******************************************************************************/


#ifndef LEDA_DOSWIN_H
#define LEDA_DOSWIN_H

#include <LEDA/impl/x_basic.h>

#include <stdlib.h>
#include <string.h>
#include <math.h>


#if defined(__TURBOC__)
extern "C" unsigned char inportb(int port_id);
extern "C" void outportb(int portid, unsigned char value);
#define _PORT_DEFS
#endif

#include <dos.h>
#include <conio.h>

#if defined(__EMX__)
union  REGISTERS { struct  _wordregs x; struct  _byteregs h; };
#else
union  REGISTERS { struct  WORDREGS x; struct  BYTEREGS h; };
#endif

#if defined(__WATCOMC__)
inline void int_86(int i, REGISTERS* p, REGISTERS* q)
{ int386(i,(REGS*)p, (REGS*)q); }
#else
#if defined(__EMX__)
inline void int_86(int i, REGISTERS* p, REGISTERS* q)
{ _int86(i,(REGS*)p, (REGS*)q); }
#else
inline void int_86(int i, REGISTERS* p, REGISTERS* q)
{ int86(i,(REGS*)p, (REGS*)q); }
#endif
#endif



struct dos_window
{
  char* image_buf;
  int   width;
  int   height;
  int   xpos;
  int   ypos;
  int   x0;
  int   y0;
  int   x1;
  int   y1;
  int   bg_col;
  int   label_col;
  char  header[128];
  char  label[128];
  char  iconized;
  int   save_xpos;
  int   save_ypos;
  int   save_x0;
  int   save_y0;
  int   save_x1;
  int   save_y1;
  int   save_bg_col;
  int   id;
  void  (*redraw)();
};

typedef dos_window* DosWindow;


extern DosWindow  win_stack[16];
extern int win_top;

void init_graphics(int mode, int root_col);
void draw_pointer(int x,int y,int shape);

extern int DISP_WIDTH;
extern int DISP_HEIGHT;
extern int DISP_MAX_X;
extern int DISP_MAX_Y;

#endif
