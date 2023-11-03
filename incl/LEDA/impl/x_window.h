/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  x_window.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/

#ifndef LEDA_X_WINDOW_H
#define LEDA_X_WINDOW_H

// windows, events, colors ...


typedef unsigned long Window;


enum { 
  key_press_event, 
  key_release_event, 
  button_press_event, 
  button_release_event,
  configure_event, 
  motion_event, 
  destroy_event, 
  no_event 
};


enum {
  white  =  0,
  black  =  1,
  red    =  2,
  green  =  3,
  blue   =  4,
  yellow =  5,
  violet =  6,
  orange =  7,
  cyan   =  8,
  brown  =  9,
  pink   = 10,
  green2 = 11,
  blue2  = 12,
  grey1  = 13,
  grey2  = 14,
  grey3  = 15 
};

enum line_style   {solid, dashed, dotted};
enum text_mode    {transparent, opaque};
enum drawing_mode {src_mode, xor_mode, or_mode, and_mode};

#endif
