/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  _xbasic.c
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/impl/doswin.h>
#include <LEDA/bitmaps/font8.16>


// RGB values    wi bl  re  gr  bl  ye  vi  or  cy  br  pi  gr  bl  g1  g2  g3
char _R_[16] = { 63, 0, 63,  0,  0, 56, 40, 63,  0, 48, 63,  0, 32, 53, 46, 24};
char _G_[16] = { 63, 0,  0, 48,  0, 56,  0, 40, 48, 28,  0, 40, 32, 53, 46, 24};
char _B_[16] = { 63, 0,  0,  0, 63,  0, 52,  0, 48, 14, 63, 32, 48, 53, 46, 24};


int DISP_WIDTH = 640;
int DISP_HEIGHT= 480;
int DISP_DEPTH = 4;

int DISP_MAX_X = DISP_WIDTH - 1;
int DISP_MAX_Y = DISP_HEIGHT- 1;

int  display_width() { return DISP_WIDTH; }
int  display_height(){ return DISP_HEIGHT; }
int  display_depth() { return DISP_DEPTH; }

void start_batch(Window) {}
void end_batch(Window) {}

#if defined(__WATCOMC__)
#include "_wcc.c"
#elif defined(__EMX__)
#include "_emx.c"
#else
#include "_vga.c"
#endif

