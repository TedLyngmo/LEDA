/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  panel.h
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany
+  All rights reserved.
+
*******************************************************************************/

#ifndef LEDA_PANEL_H
#define LEDA_PANEL_H

#include <LEDA/window.h>

//------------------------------------------------------------------------------
//   PANELS
//------------------------------------------------------------------------------

/*{\Manpage {panel} {} {Panels}}*/

class panel : public LEDA_PANEL {

/*{\Mdefinition
Panels are windows used for displaying text messages and updating the
values of variables. A panel $P$ consists of a set of panel items and a set
of buttons.  A variable of
a certain type (int, bool, string, double, color) is associated with each item (except for text items). It can be
manipulated through the item and a string label.}*/

public:

/*{\Mcreation P }*/

 panel() {}
/*{\Mcreate creates an empty panel $P$.}*/

 panel(string s) : LEDA_PANEL(s) {}
/*{\Mcreate creates an empty panel $P$ with header $s$.}*/

 panel(string s, int w, int h) : LEDA_PANEL(s,w,h) {}
/*{\Mcreate creates an empty panel $P$ of width $w$ and
            height $h$ with header $s$.}*/

~panel() {}


/*{\Moperations 1.2 4.8 }*/

void set_bg_color(color bg_col);
/*{\Mop   sets the panel background color to $bg\_col$.}*/

void buttons_per_line(int n) { LEDA_PANEL::buttons_per_line(n); }
/*{\Mop  defines the maximal number $n$ of buttons per line. }*/

void label(string s);
/*{\Mop   sets the panel label to $s$.}*/


void text_item(string s);
/*{\Mop      adds a text\_item $s$ to $P$.}*/

void bool_item(string s, bool& x);
/*{\Mopl     adds a boolean item with label $s$ and variable $x$ to $P$.}*/

void real_item(string s, double& x);
/*{\Mopl     adds a real item with label $s$ and variable $x$ to $P$.}*/

void double_item(string s, double& x);

void color_item(string s, color& x);
/*{\Mopl     adds a color item with label $s$ and variable $x$ to $P$.}*/

void lstyle_item(string s, line_style& x);
/*{\Mopl     adds a line style item with label $s$ and variable $x$ to $P$.}*/

void int_item(string s, int& x);
/*{\Mopl     adds an integer item with label $s$ and variable $x$ to $P$.}*/

void int_item(string s, int& x, int l, int h, int step);
/*{\Mopl     adds an integer choice item with label $s$, variable $x$,
	     range $l$,\dots, $h$, and step size $step$ to $P$.}*/

void int_item(string s, int& x, int l, int h, void (*F)(int));
void int_item(string s, int& x, int l, int h);
/*{\Mopl     adds an integer slider item with label $s$, variable $x$, and
	     range $l$,\dots,$h$ to $P$.}*/

void string_item(string s, string& x);
/*{\Mopl     adds a string item with label $s$ and variable $x$ to $P$.}*/

void string_item(string s, string& x, list<string>& L);
/*{\Mopl     adds a string item with label $s$, variable $x$, and menu $L$
	     to $P$.}*/

void choice_item(string s, int& x, list<string>& L);
/*{\Mopl     adds an integer item with label $s$, variable $x$, and choices
	     from $L$ to $P$.}*/

void choice_item(string s, int& x, string s1, string s2);
void choice_item(string s, int& x, string s1, string s2, string s3);
void choice_item(string s, int& x, string s1, string s2, string s3, string s4);
void choice_item(string s, int& x, string s1, string s2, string s3, string s4, string s5);
/*
void choice_item(string s, int& x, string s1,...,string sk);
*/
/*{\Mopl     adds an integer item with label $s$, variable $x$, and choices
	     $s_1$, \dots, $s_k$ to $P$ ($k \le 4$).}*/


int button(string s);
/*{\Mop      adds a button with label $s$ to $P$ and returns its number.}*/

void new_button_line();
/*{\Mop      starts a new line of buttons.}*/

void new_button_line(list<string>&);


// display panel window on screen

void display();
/*{\Mop      displays \var\ centered on screen. }*/

void display(int x, int y);
/*{\Mop      displays \var\  with left upper corner at $(x,y)$. }*/

void display(window& W);
/*{\Mop      displays \var\  centered over window $W$. }*/

void display(window& W, int x, int y);
/*{\Mopl      displays \var\  with left upper corner at position $(x,y)$
             of window $W$. }*/


// read panel

int  read() { return LEDA_PANEL::read(); }
/*{\Mop      waits for a button selection in $P$. Returns the number of
             the selected button.}*/

// open = display + read

int  open();
/*{\Mop      $P$.display() $+$  $P$.read().}*/

int  open(int x, int y);
/*{\Mop      $P$.display$(x,y)$ $+$  $P$.read().}*/

int  open(window& W);
/*{\Mop      $P$.display$(W)$ $+$  $P$.read().}*/

int  open(window& W, int x, int y);
/*{\Mopl      $P$.display$(W,x,y)$ $+$  $P$.read().}*/

};

#endif
