/*******************************************************************************
+
+  LEDA-R  3.2.3
+
+  _vga.c
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany
+  All rights reserved.
+
*******************************************************************************/


#include <LEDA/impl/doswin.h>

/*
 * basic graphics routines in VGA mode 640x480x16
 * (part of this code is based on "VGAlib" by Tommy Frandsen)
 */

#if defined(__EMX__)
#include <sys/hw.h>
#define port_out(value,port) _outp32(port,value)
#define port_in(port)  _inp32(port)
#else
#if defined(__TURBOC__) || defined(__GNUG__)
#define port_out(value,port) outportb(port,value)
#define port_in(port)  inportb(port)
#else
#define port_out(value,port) outp(port,value)
#define port_in(port)  inp(port)
#endif
#endif


#if defined (__ZTC__) || defined(__TURBOC__)
typedef unsigned char far* VIDEO_PTR;
#else
typedef unsigned char* VIDEO_PTR;
#endif



static VIDEO_PTR  VIDEO=0;
static int LINE_BYTES = 80;

#define FONT_SIZE  0x2000

/* VGA index register ports */
#define CRT_IC  0x3D4   /* CRT Controller Index - color emulation */
#define CRT_IM  0x3B4   /* CRT Controller Index - mono emulation */
#define ATT_IW  0x3C0   /* Attribute Controller Index & Data Write Register */
#define GRA_I   0x3CE   /* Graphics Controller Index */
#define SEQ_I   0x3C4   /* Sequencer Index */
#define PEL_IW  0x3C8   /* PEL Write Index */
#define PEL_IR  0x3C7   /* PEL Read Index */

/* VGA data register ports */
#define CRT_DC  0x3D5   /* CRT Controller Data Register - color emulation */
#define CRT_DM  0x3B5   /* CRT Controller Data Register - mono emulation */
#define ATT_R   0x3C1   /* Attribute Controller Data Read Register */
#define GRA_D   0x3CF   /* Graphics Controller Data Register */
#define SEQ_D   0x3C5   /* Sequencer Data Register */
#define MIS_R   0x3CC   /* Misc Output Read Register */
#define MIS_W   0x3C2   /* Misc Output Write Register */
#define IS1_RC  0x3DA   /* Input Status Register 1 - color emulation */
#define IS1_RM  0x3BA   /* Input Status Register 1 - mono emulation */
#define PEL_D   0x3C9   /* PEL Data Register */

/* VGA indexes max counts */
#define CRT_C   24      /* 24 CRT Controller Registers */
#define ATT_C   21      /* 21 Attribute Controller Registers */
#define GRA_C   9       /* 9  Graphics Controller Registers */
#define SEQ_C   5       /* 5  Sequencer Registers */
#define MIS_C   1       /* 1  Misc Output Register */

/* VGA registers saving indexes */
#define CRT     0               /* CRT Controller Registers start */
#define ATT     CRT+CRT_C       /* Attribute Controller Registers start */
#define GRA     ATT+ATT_C       /* Graphics Controller Registers start */
#define SEQ     GRA+GRA_C       /* Sequencer Registers */
#define MIS     SEQ+SEQ_C       /* General Registers */
#define END     MIS+MIS_C       /* last */


/* variables used to shift between monchrome and color emulation */
static int CRT_I;		/* current CRT index register address */
static int CRT_D;		/* current CRT data register address */
static int IS1_R;		/* current input status register address */
static int color_text;		/* true if color text emulation */


/* BIOS mode 12h - 640x480x16 */
static char g640x480x16_regs[60] = {
  0x5F,0x4F,0x50,0x82,0x54,0x80,0x0B,0x3E,0x00,0x40,0x00,0x00,
  0x00,0x00,0x00,0x00,0xEA,0x8C,0xDF,0x28,0x00,0xE7,0x04,0xE3,
  0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,
  0x0C,0x0D,0x0E,0x0F,0x01,0x00,0x0F,0x00,0x00,
  0x00,0x0F,0x00,0x20,0x00,0x00,0x05,0x0F,0xFF,
  0x03,0x01,0x0F,0x00,0x06,
  0xE3
};


static char text_regs[60];   /* VGA registers for saved text mode */

/* saved text mode palette values */
static char text_red[256];
static char text_green[256];
static char text_blue[256];


static int initialized = 0;

static char font_buf1[FONT_SIZE];  /* saved font data - plane 2 */
static char font_buf2[FONT_SIZE];  /* saved font data - plane 3 */


static void set_regs(char regs[])
{
    int i;

    /* disable video */
    port_in(IS1_R);
    port_out(0x00, ATT_IW);

    /* update misc output register */
    port_out(regs[MIS], MIS_W);

    /* synchronous reset on */
    port_out(0x00,SEQ_I);
    port_out(0x01,SEQ_D);

    /* write sequencer registers */
    for (i = 1; i < SEQ_C; i++) {
	port_out(i, SEQ_I);
	port_out(regs[SEQ+i], SEQ_D);
    }

    /* synchronous reset off */
    port_out(0x00, SEQ_I);
    port_out(0x03, SEQ_D);

    /* deprotect CRT registers 0-7 */
    port_out(0x11, CRT_I);
    port_out(port_in(CRT_D)&0x7F, CRT_D);

    /* write CRT registers */
    for (i = 0; i < CRT_C; i++) {
	port_out(i, CRT_I);
	port_out(regs[CRT+i], CRT_D);
    }

    /* write graphics controller registers */
    for (i = 0; i < GRA_C; i++) {
	port_out(i, GRA_I);
	port_out(regs[GRA+i], GRA_D);
    }

    /* write attribute controller registers */
    for (i = 0; i < ATT_C; i++) {
	port_in(IS1_R);   /* reset flip-flop */
	port_out(i, ATT_IW);
	port_out(regs[ATT+i],ATT_IW);
    }
}

static void vga_initialize()
{
    int  i, j;
    int mem_fd = -1;  /* /dev/mem file descriptor		     */

    if (initialized) return;

    initialized = 1;


#if defined(__EMX__)
    _portaccess(0x300,0x3FF);
    VIDEO = (VIDEO_PTR)_memaccess(0xa0000,0xaffff,1);
#elif defined(__GNUC__)
    VIDEO = (VIDEO_PTR)0xd0000000;
#elif defined(__ZTC__)
    VIDEO = (VIDEO_PTR)_x386_mk_protected_ptr(0xa0000);
#elif defined(__WATCOMC__)
    VIDEO = (VIDEO_PTR)0xa0000;
#else
    VIDEO = (VIDEO_PTR)MK_FP(0xa000,0);
#endif


    /* color or monochrome text emulation? */
    color_text = port_in(MIS_R)&0x01;

    /* chose registers for color/monochrome emulation */
    if (color_text) {
	CRT_I = CRT_IC;
	CRT_D = CRT_DC;
	IS1_R = IS1_RC;
    } else {
	CRT_I = CRT_IM;
	CRT_D = CRT_DM;
	IS1_R = IS1_RM;
    }

    /* disable video */
    port_in(IS1_R);
    port_out(0x00, ATT_IW);

    /* save text mode palette - first select palette index 0 */
    port_out(0, PEL_IR);

    /* read RGB components - index is autoincremented */
    for(i = 0; i < 256; i++) {
	for(j = 0; j < 10; j++) ;   /* delay (minimum 240ns) */
	text_red[i] = port_in(PEL_D);
	for(j = 0; j < 10; j++) ;   /* delay (minimum 240ns) */
	text_green[i] = port_in(PEL_D);
	for(j = 0; j < 10; j++) ;   /* delay (minimum 240ns) */
	text_blue[i] = port_in(PEL_D);
    }

    /* save text mode VGA registers */
    for (i = 0; i < CRT_C; i++) {
	 port_out(i, CRT_I);
	 text_regs[CRT+i] = port_in(CRT_D);
    }
    for (i = 0; i < ATT_C; i++) {
      	 port_in(IS1_R);
         port_out(i, ATT_IW);
         text_regs[ATT+i] = port_in(ATT_R);
    }
    for (i = 0; i < GRA_C; i++) {
       	 port_out(i, GRA_I);
       	 text_regs[GRA+i] = port_in(GRA_D);
    }
    for (i = 0; i < SEQ_C; i++) {
       	 port_out(i, SEQ_I);
       	 text_regs[SEQ+i] = port_in(SEQ_D);
    }
    text_regs[MIS] = port_in(MIS_R);

    /* shift to color emulation */
    CRT_I = CRT_IC;
    CRT_D = CRT_DC;
    IS1_R = IS1_RC;
    port_out(port_in(MIS_R)|0x01, MIS_W);

    /* save font data - first select a 16 color graphics mode */
    set_regs(g640x480x16_regs);

    /* save font data in plane 2 */
    port_out(0x04, GRA_I);
    port_out(0x02, GRA_D);
    for(i = 0; i < FONT_SIZE; i++) font_buf1[i] = VIDEO[i];

    /* save font data in plane 3 */
    port_out(0x04, GRA_I);
    port_out(0x03, GRA_D);
    for(i = 0; i < FONT_SIZE; i++) font_buf2[i] = VIDEO[i];
}


void set_palette(int index, int red, int green, int blue)
{
    volatile int i;

    if (red < 0)     red = _R_[index]; else _R_[index] = red;
    if (green < 0) green = _G_[index]; else _G_[index] = green;
    if (blue < 0)   blue = _B_[index]; else _B_[index] = blue;


    /* select palette register */
    port_out(index, PEL_IW);

    /* write RGB components */
    for(i = 0; i < 10; i++) ;   /* delay (minimum 240ns) */
    port_out(red, PEL_D);
    for(i = 0; i < 10; i++) ;   /* delay (minimum 240ns) */
    port_out(green, PEL_D);
    for(i = 0; i < 10; i++) ;   /* delay (minimum 240ns) */
    port_out(blue, PEL_D);
}


void get_palette(int index, int *red, int *green, int *blue)
{
    int i;

    /* select palette register */
    port_out(index, PEL_IR);

    /* read RGB components */
    for(i = 0; i < 10; i++) ;   /* delay (minimum 240ns) */
    *red = (int) port_in(PEL_D);
    for(i = 0; i < 10; i++) ;   /* delay (minimum 240ns) */
    *green = (int) port_in(PEL_D);
    for(i = 0; i < 10; i++) ;   /* delay (minimum 240ns) */
    *blue = (int) port_in(PEL_D);
}

static void vga_clear(int c)
{
  register VIDEO_PTR p;
  register VIDEO_PTR last;

  /* set color c */
  port_out(c, GRA_I );
  port_out(0, GRA_D );

  /* set mode 0 */
  port_out(0x03, GRA_I );
  port_out(0, GRA_D );

  /* write to all bits */
  port_out(0x08, GRA_I );
  port_out(0xFF, GRA_D );

  last  = VIDEO + DISP_HEIGHT*LINE_BYTES;

  for(p = VIDEO; p < last; p++)  *p = 0;

}


void init_graphics(int mode, int root_col)  // mode = 0: Text, 1: 640x480x16
{
    int i;

    vga_initialize();

    if (mode == 0)  // TEXT
      {
        vga_clear(0);

        /* restore font data - first select a 16 color graphics mode */
        set_regs(g640x480x16_regs);

	/* disable Set/Reset Register */
    	port_out(0x01, GRA_I );
    	port_out(0x00, GRA_D );

        /* restore font data in plane 2 - necessary for all VGA's */
    	port_out(0x02, SEQ_I );
    	port_out(0x04, SEQ_D );
        for(i = 0; i < FONT_SIZE; i++) VIDEO[i] = font_buf1[i];

        /* restore font data in plane 3 - necessary for Trident VGA's */
    	port_out(0x02, SEQ_I );
    	port_out(0x08, SEQ_D );
        for(i = 0; i < FONT_SIZE; i++) VIDEO[i] = font_buf2[i];

        /* change register adresses if monochrome text mode */
        if (!color_text) {
            CRT_I = CRT_IM;
            CRT_D = CRT_DM;
            IS1_R = IS1_RM;
            port_out(port_in(MIS_R)&0xFE, MIS_W);
        }

	/* restore text mode VGA registers */
    	set_regs(text_regs);

        /* restore saved palette */
        for(i = 0; i < 256; i++)
            set_palette(i, text_red[i], text_green[i], text_blue[i]);

        DISP_WIDTH = 80;
        DISP_MAX_X = 79;
        DISP_HEIGHT= 25;
        DISP_MAX_Y = 24;

      }
    else // graphics mode
      {
        /* shift to color emulation */
        CRT_I = CRT_IC;
        CRT_D = CRT_DC;
        IS1_R = IS1_RC;
        port_out(port_in(MIS_R)|0x01, MIS_W);
        set_regs(g640x480x16_regs);

        /* set default palette */
        for(i = 0; i < 16; i++)
          set_palette(i, _R_[i], _G_[i], _B_[i]);

        vga_clear(root_col);

        LINE_BYTES = 80;
        DISP_WIDTH = 640;
        DISP_MAX_X = 639;
        DISP_HEIGHT= 480;
        DISP_MAX_Y = 479;
        DISP_DEPTH = 4;
      }

    /* enable video */
    port_in(IS1_R);
    port_out(0x20, ATT_IW);

    if (mode == 0)  // TEXT
    { union REGISTERS regs;
      regs.h.ah=0x00;
      regs.h.al=0x02;
      int_86(0x10,&regs,&regs);
    }

}




//------------------------------------------------------------------------------
//  basic graphic routines of libWx  declared in <LEDA/impl/x_basic.h>)
//  iplemented for MSDOS using the VGA 640x480x16 color graphics mode
//------------------------------------------------------------------------------


#if !defined(M_PI)
#define	M_PI   3.14159265358979323846
#endif

static VIDEO_PTR video_buf;
static int COLOR = 1;
static int MODE  = 0;
static int LINEWIDTH = 1;
static int LINESTYLE = 0;

void flush_display() {}

int  new_color(const char*) { return 1; }

int  text_height(const char*)  { return FONT_HEIGHT; }
int  text_width(const char* s) { return FONT_WIDTH*strlen(s); }

int load_text_font(const char*) { return 0;}
int load_bold_font(const char*) { return 0;}
int load_message_font(const char*) { return 0;}

int  set_font(const char*) { return 0;}
void set_text_font() {}
void set_bold_font() {}
void set_message_font() {}

int set_line_width(int width)
{ int save = LINEWIDTH;
  LINEWIDTH = width;
  return save;
}

int set_line_style(int style)
{ int save = LINESTYLE;
  LINESTYLE = style;
  return save;
}

int set_color(int color)
{  int save = COLOR;
   COLOR = color;
   port_out(0x00, GRA_I );
   port_out(color, GRA_D );
   return save;
 }

int set_mode(int mode)
{ int save = MODE;

  MODE = mode;

  if (mode==1)         mode = 3; /* xor */
  else if (mode==2)    mode = 2; /* or  */
     else if (mode==3) mode = 1; /* and */
                  else mode = 0; /* src */

  port_out(0x03, GRA_I );
  port_out(mode<<3, GRA_D );
  return save;
}

void set_redraw(Window w, void (*f)())
{ win_stack[w]->redraw = f; }




static void put_pixel(DosWindow win, int x, int y)
{ VIDEO_PTR p;

  if (x < 0 || x >= win->width || y < 0 || y >= win->height) return;

  x += win->xpos;
  y += win->ypos;

  if (x < 0 || x > DISP_MAX_X || y < 0 || y > DISP_MAX_Y) return;

  if (VIDEO==0)   /* write into (monochrome) buffer */
     video_buf[y*LINE_BYTES+(x>>3)] |= (0x80 >> (x & 7));
  else
  { p = VIDEO + y*LINE_BYTES + (x>>3);
    port_out(8, GRA_I);
    port_out((0x80 >> (x & 7)), GRA_D);
    *p = *p;
   }
}

static void vline(DosWindow win,int x, int y0, int y1)
{ register VIDEO_PTR p;
  register VIDEO_PTR last;

  if (y0 > y1)
  { int y = y0;
    y0 = y1;
    y1 = y;
   }

  // clip into window
  if (x < 0 || x >= win->width || y1 < 0 || y0 >= win->height) return;
  if (y0 < 0) y0 = 0;
  if (y1 >= win->height) y1 = win->height-1;

  x  += win->xpos;
  y0 += win->ypos;
  y1 += win->ypos;

  // clip into screen
  if (x < 0 || x > DISP_MAX_X || y1 < 0 || y0 > DISP_MAX_Y) return;
  if (y0 < 0) y0 = 0;
  if (y1 > DISP_MAX_Y) y1 = DISP_MAX_Y;

  port_out(8, GRA_I);
  port_out(128 >> (x&7), GRA_D);
  last  = VIDEO + LINE_BYTES*y1 + (x>>3);
  for(p = VIDEO + LINE_BYTES*y0 + (x>>3); p <= last; p+=LINE_BYTES)  *p = *p;

}


static void hline(DosWindow win, int x0, int x1, int y)
{ register VIDEO_PTR p;
  register VIDEO_PTR first;
  register VIDEO_PTR last;
  char byte;

  if (x0 > x1)
  { int x = x0;
    x0 = x1;
    x1 = x;
   }

  // clip into window
  if (y < 0 || y >= win->height || x1 < 0 || x0 >= win->width) return;
  if (x0 < 0) x0 = 0;
  if (x1 >= win->width) x1 = win->width-1;

  x0 += win->xpos;
  x1 += win->xpos;
  y  += win->ypos;

  // clip into screen
  if (y < 0 || y > DISP_MAX_Y || x1 < 0 || x0 > DISP_MAX_X) return;
  if (x0 < 0) x0 = 0;
  if (x1 > DISP_MAX_X) x1 = DISP_MAX_X;

  first = VIDEO + LINE_BYTES*y + (x0>>3);
  last  = VIDEO + LINE_BYTES*y + (x1>>3);

  port_out(8, GRA_I);

  if (first == last)
  { byte  = 0xFF>>(x0&7);
    byte &= 0xFF<<((~x1)&7);
    port_out(byte, GRA_D);
    *first = *first;
    return;
   }

  port_out(0xFF>>(x0&7), GRA_D);
  *first = *first;

  port_out(0xFF<<((~x1)&7), GRA_D);
  *last = *last;

  port_out(0xFF, GRA_D);
  for(p=first+1; p<last; p++) *p = *p;

 }


static void Draw_Line(DosWindow win, int x1, int y1, int x2, int y2)
{
  register int sy = 1;
  register int dx = x2 - x1;
  register int dy = y2 - y1;
  register int c;
  int i;

  if (dx < 0)
  { int i = x1;
    x1 = x2;
    x2 = i;
    dx = -dx;
    i = y1;
    y1 = y2;
    y2 = i;
    dy = -dy;
   }
  if (dy < 0)
  { dy = -dy;
    sy = -1;
   }

  if (dx > dy)
  { c = dx / 2;
    put_pixel(win,x1,y1);
    for (i=1; i<=LINEWIDTH/2; i++)
    { put_pixel(win,x1,y1+i);
      put_pixel(win,x1,y1-i);
     }
    while(x1 != x2)
    { x1++;
      c += dy;
      if (c >= dx)
      { c -= dx;
        y1 += sy;
       }
      put_pixel(win,x1,y1);
      for (i=1; i<=LINEWIDTH/2; i++)
      { put_pixel(win,x1,y1+i);
        put_pixel(win,x1,y1-i);
       }
    }
  }
  else
  { c = dy / 2;
    put_pixel(win,x1,y1);
    for (i=1; i<=LINEWIDTH/2; i++)
    { put_pixel(win,x1+i,y1);
      put_pixel(win,x1-i,y1);
     }
    while(y1 != y2)
    { y1 += sy;
      c += dx;
      if (c >= dy)
      { c -= dy;
        x1++;
       }
      put_pixel(win,x1,y1);
      for (i=1; i<=LINEWIDTH/2; i++)
      { put_pixel(win,x1+i,y1);
        put_pixel(win,x1-i,y1);
       }
    }
  }
}

void line(Window w, int x1, int y1, int x2, int y2)
{ int i;

  DosWindow win = win_stack[w];

  if (x1 == x2)
  { vline(win,x1,y1,y2);
    for (i=1; i<=LINEWIDTH/2; i++)
    { vline(win,x1-i,y1,y2);
      vline(win,x1+i,y1,y2);
     }
    return;
   }

  if (y1 == y2)
  { hline(win,x1,x2,y1);
    for (i=1; i<=LINEWIDTH/2; i++)
    { hline(win,x1,x2,y1-i);
      hline(win,x1,x2,y1+i);
     }
    return;
   }

  Draw_Line(win,x1,y1,x2,y2);
}



static int get_pixel(int x, int y)
{
  register int bit = 0x80 >> (x&7);
  register VIDEO_PTR byte = VIDEO + LINE_BYTES*y+(x>>3);
  register int c;

  if (x < 0 || x > DISP_MAX_X || y < 0 || y > DISP_MAX_Y) return 0;

  /* set read mode 1 */
  port_out(5, GRA_I);
  port_out(8, GRA_D);

  for(c=0; c<16; c++)
  { port_out(2, GRA_I);
    port_out(c, GRA_D);
    if (*byte & bit)  break;
   }

  return c;
}


#define FILLPUT(pos,byte)\
{ p = pos;\
  if ((*p & (byte)) == 0)\
  { *p |= (byte);\
    POS[top] = p; \
    BYTE[top] = byte; \
    top = (top+1) % 512; } else;\
}

static  VIDEO_PTR      POS[512];
static  unsigned char  BYTE[512];

static void fill_bits(VIDEO_PTR pos,unsigned char byte)
{ register int bot = 0;
  register int top = 0;
  register VIDEO_PTR p;

  FILLPUT(pos,byte)

  while (top != bot)
  { pos  = POS[bot];
    byte = BYTE[bot];

    bot = (bot+1) % 512;

    if (byte == 128)
       FILLPUT(pos-1,1)
    else
       FILLPUT(pos,byte<<1)

    if (byte == 1)
       FILLPUT(pos+1,128)
    else
       FILLPUT(pos,byte>>1)

    FILLPUT(pos-LINE_BYTES,byte)
    FILLPUT(pos+LINE_BYTES,byte)
  }
}

static  VIDEO_PTR  POS1[512];
static  int pos_top = 0;

void fill_bytes(VIDEO_PTR pos, int d)
{ VIDEO_PTR p = pos+d;

  unsigned char c = *p;
  unsigned char pat = 0xFF;

  if (c == 0)
  { *p = 0xFF;
    fill_bytes(p,  1);
    fill_bytes(p, -1);
    fill_bytes(p, LINE_BYTES);
    fill_bytes(p,-LINE_BYTES);
    return;
   }

  if (d == -1)
  { while((c&1) == 0)
    { pat <<= 1;
      c >>= 1;
     }
    *p |= ~pat;
   }

  if (d == 1)
  { while((c&128) == 0)
    { pat >>= 1;
      c <<= 1;
     }
    *p |= ~pat;
   }

  if((c&8) == 0 && (d == LINE_BYTES || d == -LINE_BYTES))   POS1[pos_top++] = p;

 }


void fill_polygon(Window w, int n, int* xcoord, int* ycoord)
{
  DosWindow win = win_stack[w];

  register VIDEO_PTR p;
  register VIDEO_PTR q;
  register VIDEO_PTR first;
  register VIDEO_PTR first1;
  register VIDEO_PTR last;

  int i,m1,m2;
  int minxi = 0;
  int maxxi = 0;
  int minyi = 0;
  int maxyi = 0;
  int minx,maxx,miny,maxy,x,y;

  VIDEO_PTR video_save = VIDEO;

  VIDEO = 0;

  video_buf = (VIDEO_PTR)malloc(LINE_BYTES*480);


  for(i=1;i<n;i++)
  { minxi = (xcoord[i] < xcoord[minxi]) ? i : minxi;
    minyi = (ycoord[i] < ycoord[minyi]) ? i : minyi;
    maxxi = (xcoord[i] > xcoord[maxxi]) ? i : maxxi;
    maxyi = (ycoord[i] > ycoord[maxyi]) ? i : maxyi;
   }

  minx = (xcoord[minxi] + win->xpos)/8;
  maxx = (xcoord[maxxi] + win->xpos)/8;
  miny = ycoord[minyi] + win->ypos;
  maxy = ycoord[maxyi] + win->ypos;

  m1 =  (minxi == 0)   ?  n-1 : minxi-1;
  m2 =  (minxi == n-1) ?  0   : minxi+1;

  for(i=miny; i<=maxy; i++)
  { last = video_buf+LINE_BYTES*i + maxx;
    for(p=video_buf+LINE_BYTES*i+minx; p<=last; p++) *p= 0;
   }

  for(i=0; i<n-1; i++)
      Draw_Line(win,xcoord[i],ycoord[i],xcoord[i+1],ycoord[i+1]);

  Draw_Line(win,xcoord[0],ycoord[0],xcoord[n-1],ycoord[n-1]);

  x = (xcoord[m1] + xcoord[m2] + xcoord[minxi])/3 + win->xpos;
  y = (ycoord[m1] + ycoord[m2] + ycoord[minxi])/3 + win->ypos;

  pos_top = 0;
  fill_bytes(video_buf + LINE_BYTES*y + x/8,0);

  while (pos_top--) fill_bits(POS1[pos_top],8);

  fill_bits(video_buf+LINE_BYTES*y+x/8,128>>(x%8));

  VIDEO = video_save;

  first  = video_buf+LINE_BYTES*miny + minx;
  first1 = VIDEO+LINE_BYTES*miny + minx;
  last   = video_buf+LINE_BYTES*miny + maxx;

  port_out(8, GRA_I);

  while(miny <= maxy)
  { for(p=first, q = first1; p<=last; p++, q++)
    { port_out(*p, GRA_D);
      *q = *q;
     }
    first  += LINE_BYTES;
    first1 += LINE_BYTES;
    last   += LINE_BYTES;
    miny++;
   }

  free((char*)video_buf);
}


void box(Window w, int x0, int y0, int x1, int y1)
{ DosWindow win = win_stack[w];
  if (y0 > y1)
  { int y = y0;
    y0 = y1;
    y1 = y;
   }
  while(y0<=y1) hline(win,x0,x1,y0++);
 }


void  rectangle(Window w, int x0, int y0, int x1, int y1)
{
  int left  = x0;
  int right = x1;
  int top   = y0;
  int bottom= y1;

  if (x0 > x1)
  { left  = x1;
    right = x0;
   }

  if (y0 > y1)
  { top  = y1;
    bottom = y0;
   }

  line(w,left, top,     right,top);
  line(w,left, bottom,  right,bottom);
  line(w,left, bottom-1,left, top+1);
  line(w,right,bottom-1,right,top+1);
}


void circle(Window w, int x0,int y0,int r0)
{ DosWindow win = win_stack[w];
  int r;

  for (r = r0-LINEWIDTH/2; r <= r0+LINEWIDTH/2; r++)
  { int y = r;
    int x = 0;
    int e = 3-2*y;

    put_pixel(win,x0,y0+r);
    put_pixel(win,x0,y0-r);
    put_pixel(win,x0+r,y0);
    put_pixel(win,x0-r,y0);

    for (x=1;x<y;)
      { put_pixel(win,x0+x,y0+y);
        put_pixel(win,x0+x,y0-y);
        put_pixel(win,x0-x,y0+y);
        put_pixel(win,x0-x,y0-y);
        put_pixel(win,x0+y,y0+x);
        put_pixel(win,x0+y,y0-x);
        put_pixel(win,x0-y,y0+x);
        put_pixel(win,x0-y,y0-x);
        x++;
        if (e>=0) { y--; e = e - 4*y; }
        e = e + 4*x + 2;
       }

    if (x == y)
    { put_pixel(win,x0+x,y0+y);
      put_pixel(win,x0+x,y0-y);
      put_pixel(win,x0-x,y0+y);
      put_pixel(win,x0-x,y0-y);
     }
  }
}


void fill_circle(Window w, int x0, int y0, int r)
{ DosWindow win = win_stack[w];
  int y = 1;
  int x = r;
  int e = 3-2*r;

  hline(win,x0-x,x0+x,y0);

  while (y<=x)
  { hline(win,x0-x,x0+x,y0+y);
    hline(win,x0-x,x0+x,y0-y);

    if (y<x && e>=0)
    { hline(win,x0-y,x0+y,y0+x);
      hline(win,x0-y,x0+y,y0-x);
      x--;
      e = e - 4*x;
     }
    y++;
    e = e + 4*y + 2;
   }
}



static void Put_Text(DosWindow win, int x, int y, const char *str, int bg_col)
{ /* bgcol = -1 : transparent */
  register unsigned char *fp1;
  register unsigned char *fp2;
  register unsigned char c;

  register VIDEO_PTR start;
  register VIDEO_PTR stop;
  register VIDEO_PTR q;

  char text[128];

  int  len = strlen(str);
  int xmin = win->xpos;
  int ymin = win->ypos;
  int xmax = xmin + win->width  - 1;
  int ymax = ymin + win->height - 1;

  if (xmin < 0) xmin = 0;
  if (ymin < 0) ymin = 0;
  if (xmax > DISP_MAX_X) xmax = DISP_MAX_X;
  if (ymax > DISP_MAX_Y) ymax = DISP_MAX_Y;


  if (bg_col >= 0)  // clear background
  { int save_color = set_color(bg_col);
    int save_mode = set_mode(0);
    box(win->id,x,y,x+text_width(str)-1,y+FONT_HEIGHT-1);
    set_mode(save_mode);
    set_color(save_color);
   }

  x += win->xpos;
  y += win->ypos;

  if (y < ymin || y > ymax || x > xmax) return;  // string not visible

  if (x < xmin)
  { int pre = (xmin - x)/FONT_WIDTH + 1;
    if (pre >= len) return;
    str += pre;
    len -= pre;
    x = xmin;
  }

  int maxlen = (xmax-x)/FONT_WIDTH;
  if (len > maxlen) len = maxlen;
  if (len > 0) strncpy(text,str,len);
  text[len] = 0;

  int dy = ymax - y;
  if (dy > FONT_HEIGHT) dy = FONT_HEIGHT;


  fp1 = FONT + FONT_HEIGHT * ' ';
  fp2 = FONT + FONT_HEIGHT * (text[0] & 127);

  start = VIDEO + LINE_BYTES*y + x/8;
  stop = start + LINE_BYTES*dy;

  x &= 7;

  port_out(8, GRA_I);

  for(int i=0;i<len; i++)
  { for (q = start; q < stop; q+=LINE_BYTES, fp1++,fp2++)
    { c = ((*fp2)>>x) | ((*fp1)<<(8-x));
      port_out(c, GRA_D);
      *q = *q;
     }
    fp1 = FONT + FONT_HEIGHT * (text[i] & 127);
    fp2 = FONT + FONT_HEIGHT * (text[i+1] & 127);
    start++;
    stop++;
   }

  if (x > 0)
    for (q = start; q < stop; q+=LINE_BYTES, fp1++,fp2++)
    { c = (*fp1)<<(8-x);
      port_out(c, GRA_D);
      *q = *q;
     }
}


void put_text(Window w, int x, int y, const char *text, int opaque)
{ DosWindow win = win_stack[w];
  Put_Text(win,x,y,text,opaque ? win->bg_col : -1);
}

void put_text(Window w, int x, int y, const char *text, int l, int opaque)
{ DosWindow win = win_stack[w];
  //char* str = new char[l+1];
  //strncpy(str,text,l);
  char* str = new char[strlen(text)+1];
  strcpy(str,text);
  str[l] = '\0';
  Put_Text(win,x,y,str,opaque ? win->bg_col : -1);
  delete[] str;
}

void put_ctext(Window w, int x, int y, const char* str, int opaque)
{ put_text(w,x-(text_width(str)-1)/2, y-(text_height(str)-1)/2, str, opaque); }


void show_coordinates(Window w, const char* s)
{ DosWindow win = win_stack[w];
  int save_mode = set_mode(0);
  int save_col  = set_color(4);
  put_text(w,win->width-138,1,s,1);
  set_mode(save_mode);
  set_color(save_col);
}



void pixel(Window w, int x, int y) { put_pixel(win_stack[w],x,y);}


void pixels(Window w, int n, int* x, int* y)
{ while(n--) put_pixel(win_stack[w],x[n],y[n]); }


void point(Window w, int x, int y)
{ DosWindow win = win_stack[w];
  put_pixel(win,x,y);
  put_pixel(win,x-2,y-2);
  put_pixel(win,x-1,y-1);
  put_pixel(win,x+1,y+1);
  put_pixel(win,x+2,y+2);
  put_pixel(win,x-2,y+2);
  put_pixel(win,x-1,y+1);
  put_pixel(win,x+1,y-1);
  put_pixel(win,x+2,y-2);
}



#define put_arc_pixel(X,Y,x,y,top) { X[top] = x; Y[top] = y; top++; }

void arc(Window w, int x0, int y0, int r1, int r2, double start, double angle)
{ DosWindow win = win_stack[w];
  int* X = new int[10*r1];
  int* Y = new int[10*r2];
  int r;

 x0 += win->xpos;
 y0 += win->ypos;

 if (angle < 0)
 { start += angle;
   angle *= -1;
  }

 if (angle > 2*M_PI) angle = 2*M_PI;

 while (start < 0) start += 2*M_PI;

 for (r = r1-LINEWIDTH/2; r <= r1+LINEWIDTH/2; r++)
 { int y = r;
   int x = 0;
   int e = 3-2*y;
   int top = 0;
   int high;
   int high1;
   int s,l;
   float L;
   int i;

   while (x < y)
   { put_arc_pixel(X,Y,x,y,top);
     x++;
     if (e>=0) { y--; e -= 4*y; }
     e += 4*x + 2;
    }

   high = top-1;

   if (x==y) put_arc_pixel(X,Y,x,y,top);

   high1 = top;

   for(i = 0;    i < high1; i++) put_arc_pixel(X,Y, Y[i],-X[i],top);
   for(i = high; i > 0;     i--) put_arc_pixel(X,Y, X[i],-Y[i],top);
   for(i = 0;    i < high1; i++) put_arc_pixel(X,Y,-X[i],-Y[i],top);
   for(i = high; i > 0;     i--) put_arc_pixel(X,Y,-Y[i],-X[i],top);
   for(i = 0;    i < high1; i++) put_arc_pixel(X,Y,-Y[i], X[i],top);
   for(i = high; i > 0;     i--) put_arc_pixel(X,Y,-X[i], Y[i],top);
   for(i = 0;    i < high1; i++) put_arc_pixel(X,Y, X[i], Y[i],top);
   for(i = high; i > 0;     i--) put_arc_pixel(X,Y, Y[i], X[i],top);

   L = (top - high1)/(2*M_PI);
   s = high1 - 1 + (int)(start*L);
   l = s + (int)(angle*L);

   if (l >= top)
   { for(i=s; i < top; i++) put_pixel(win,x0+X[i],y0+Y[i]);
     s = high - 1;
     l = s + l - top;
    }
   for(i=s; i < l; i++) put_pixel(win,x0+X[i],y0+Y[i]);
  }

  delete X;
  delete Y;
}


static void ellipse_point(DosWindow win, int x0, int y0, int x, int y)
{ put_pixel(win,x0+x,y0+y);
  put_pixel(win,x0-x,y0+y);
  put_pixel(win,x0+x,y0-y);
  put_pixel(win,x0-x,y0-y);
 }


void ellipse(Window w, int x0, int y0, int a, int b)
{
  /* Foley, van Dam, Feiner, Huges: Computer Graphics, page 90 */

  DosWindow win = win_stack[w];

  double d1,d2;
  int x,y;
  int a_2 = a*a;
  int b_2 = b*b;

  put_pixel(win,x0,y0-b);
  put_pixel(win,x0,y0+b);
  put_pixel(win,x0-a,y0);
  put_pixel(win,x0+a,y0);

  x = 0;
  y = b;

  d1 = b*b + a*a*(0.25 - b);

  while (a_2*(y - 0.5) > b_2*(x+1))
  { if (d1 < 0)
      d1 += b_2*(2*x + 3);
    else
    { d1 += b_2*(2*x + 3) + a_2*(2 - 2*y);
      y--;
     }
    x++;
    ellipse_point(win,x0,y0,x,y);
  }

  d2 = b_2*(x+0.5)*(x+0.5) + a_2*(y - 1)*(y - 1) - a_2*b_2;

  while (y > 1)
  { if (d2 < 0)
     { d2 += b_2*(2*x+2)+a_2*(3-2*y);
       x++;
      }
    else
       d2 += a*a*(3-2*y);

    y--;

    ellipse_point(win,x0,y0,x,y);
   }
}


void fill_ellipse(Window w, int x0, int y0, int a, int b)
{ DosWindow win = win_stack[w];
  double d1,d2;
  int x,y;
  int a_2 = a*a;
  int b_2 = b*b;

  x = 0;
  y = b;

  d1 = b*b + a*a*(0.25 - b);

  while (a_2*(y - 0.5) > b_2*(x+1))
  { if (d1 < 0)
      d1 += b_2*(2*x + 3);
    else
    { d1 += b_2*(2*x + 3) + a_2*(2 - 2*y);
      hline(win,x0-x,x0+x,y0+y);
      hline(win,x0-x,x0+x,y0-y);
      y--;
     }
    x++;
  }
  hline(win,x0-x,x0+x,y0+y);
  hline(win,x0-x,x0+x,y0-y);

  d2 = b_2*(x+0.5)*(x+0.5) + a_2*(y - 1)*(y - 1) - a_2*b_2;

  while (y > 1)
  { if (d2 < 0)
     { d2 += b_2*(2*x+2)+a_2*(3-2*y);
       x++;
      }
    else
       d2 += a*a*(3-2*y);

    y--;

    hline(win,x0-x,x0+x,y0+y);
    hline(win,x0-x,x0+x,y0-y);
   }

  hline(win,x0-x,x0+x,y0);

}


void fill_arc(Window,int,int,int,int,double,double)
{ }



void copy_pixrect(Window w,int left,int top,int right,int bottom,int x,int y)
{
  DosWindow win = win_stack[w];
  int width = right-left+1;
  int height = bottom-top+1;

  left += win->xpos;
  top  += win->ypos;
  x    += win->xpos;
  y    += win->ypos;

  register VIDEO_PTR first1;
  register VIDEO_PTR first;
  register VIDEO_PTR last1;
  register VIDEO_PTR last;
  register VIDEO_PTR p;
  register VIDEO_PTR q;
  int i;


  if(x < 0)
  { left += x;
    width -= x;
    x = 0;
   }

  if(y < 0)
  { top += y;
    height -= y;
    y = 0;
   }

  if(x+width > DISP_MAX_X) width = DISP_MAX_X - x;

  if(y+height > DISP_MAX_Y) height = DISP_MAX_Y - y;

  /* set write mode 1 */
  port_out(5, GRA_I);
  port_out(1, GRA_D);

  if (y <= top)
  { first1 = VIDEO + LINE_BYTES*y + x/8;
    last1  = VIDEO + LINE_BYTES*y + (x+width-1)/8;
    first  = VIDEO + LINE_BYTES*top + left/8;
    last   = VIDEO + LINE_BYTES*top + (left+width-1)/8;
   }
  else
  { first1 = VIDEO + LINE_BYTES*(y+height-1) + x/8;
    last1  = VIDEO + LINE_BYTES*(y+height-1) + (x+width-1)/8;
    first  = VIDEO + LINE_BYTES*(top+height-1) + left/8;
    last   = VIDEO + LINE_BYTES*(top+height-1) + (left+width-1)/8;
   }

  for(i=0; i<height; i++)
  {
    if (x <= left)
      for(q=first, p=first1; q<=last; q++,p++) *p = *q;
    else
      for(q=last, p=last1; q>=first; q--,p--) *p = *q;

    if (y <= top)
    { first1 += LINE_BYTES;
      first  += LINE_BYTES;
      last1  += LINE_BYTES;
      last   += LINE_BYTES;
     }
    else
    { first1 -= LINE_BYTES;
      first  -= LINE_BYTES;
      last1  -= LINE_BYTES;
      last   -= LINE_BYTES;
     }
   }

  /* set write mode 0 */
  port_out(5, GRA_I);
  port_out(0, GRA_D);

 }


static char rev_byte(char c)
{ char c1 = 0x00;
   for(int i=0; i<8; i++)
   { c1 <<= 1;
     if (c&1) c1 |= 1;
     c >>= 1;
    }
  return c1;
}


void insert_bitmap(Window w, int width, int height, char* data)
{
  register VIDEO_PTR first;
  register VIDEO_PTR last;
  register VIDEO_PTR q;

  DosWindow win = win_stack[w];

  int x = win->x0/8 + 1;
  int y = win->ypos;

  int wi = (width > win->width) ? win->width : width;
  int he = (height > win->height) ? win->height : height;

  first  = VIDEO + LINE_BYTES*y + x;
  last   = VIDEO + LINE_BYTES*y + x + wi/8 - 1;

  if (width % 8)
     width  = 1+ width/8;
  else
     width  = width/8;

  port_out(8, GRA_I);

  for(int i=0; i<he; i++)
  { char* p = data + i*width;
    for(q=first; q<=last; q++)
    { port_out(rev_byte(*p++), GRA_D);
      *q = *q;
     }
    first += LINE_BYTES;
    last  += LINE_BYTES;
   }
 }



//------------------------------------------------------------------------------
// pixrects
//------------------------------------------------------------------------------


struct pixrect
{ VIDEO_PTR plane[4];
  int width;
  int height;
  int bwidth;  // bytes
};

char* create_pixrect(Window w, int left, int top, int right, int bottom)
{
  DosWindow win = win_stack[w];
  int xmax = win->xpos + win->width - 1;
  int ymax = win->ypos + win->height - 1;

  register int y;
  register VIDEO_PTR p;
  register VIDEO_PTR q;
  register VIDEO_PTR first;
  register VIDEO_PTR last;


  left   += win->xpos;
  right  += win->xpos;
  top    += win->ypos;
  bottom += win->ypos;


  if (right  > xmax) right = xmax;
  if (bottom > ymax) bottom = ymax;

  if(left < 0) left = 0;
  if(top  < 0) top = 0;
  if(right  > DISP_MAX_X) right = DISP_MAX_X;
  if(bottom > DISP_MAX_Y) bottom = DISP_MAX_Y;


  pixrect* bp = new pixrect;

  bp->width    = right-left+1;
  bp->height   = bottom-top+1;
  bp->bwidth   = right/8 - left/8 + 1;
  bp->plane[0] = (VIDEO_PTR)malloc(bp->bwidth * bp->height);
  bp->plane[1] = (VIDEO_PTR)malloc(bp->bwidth * bp->height);
  bp->plane[2] = (VIDEO_PTR)malloc(bp->bwidth * bp->height);
  bp->plane[3] = (VIDEO_PTR)malloc(bp->bwidth * bp->height);


  /* set read mode 0 */
  port_out(5, GRA_I);
  port_out(0, GRA_D);

  /* read planes 0 to 3 */
  for(int i=0; i<4; i++)
  { p = bp->plane[i];

    port_out(4, GRA_I);
    port_out(i, GRA_D);

    first = VIDEO + LINE_BYTES*top + left/8;
    last  = VIDEO + LINE_BYTES*top + right/8;

    for(y=top; y<=bottom; y++)
    { for(q=first; q<=last; q++) *p++ = *q;
      first  += LINE_BYTES;
      last   += LINE_BYTES;
     }
   }
  return (char*)bp;
}


void insert_pixrect(Window w, int left, int top, char* rect)
{ register int y;
  register VIDEO_PTR p;
  register VIDEO_PTR q;
  register VIDEO_PTR pfirst;
  register VIDEO_PTR first;
  register VIDEO_PTR last;

  pixrect* bp = (pixrect*)rect;

  DosWindow win = win_stack[w];
  int xmax = win->xpos + win->width - 1;
  int ymax = win->ypos + win->height - 1;

  left   += win->xpos;
  top    += win->ypos;

  if(left < 0) left = 0;
  if(top  < 0) top = 0;

  int right  = left + bp->width - 1;
  int bottom = top + bp->height - 1;

  if (right  > xmax) right = xmax;
  if (bottom > xmax) bottom = ymax;

  int save_mode = set_mode(0);
  int save_col  = set_color(0);

  // clean area

  port_out(8, GRA_I);
  port_out(0xFF, GRA_D);
  first  = VIDEO + LINE_BYTES*top + left/8;
  last   = VIDEO + LINE_BYTES*top + right/8;
  for(y=top; y<=bottom; y++)
  { for(q=first; q<=last; q++) *q = *q;
    first  += LINE_BYTES;
    last   += LINE_BYTES;
   }


  // fill in plane data

  set_mode(2); /* or */

  for(int i = 0; i < 4; i++)
  {
    set_color(1<<i);

    pfirst = bp->plane[i];
    first  = VIDEO + LINE_BYTES*top + left/8;
    last   = VIDEO + LINE_BYTES*top + right/8;

    port_out(8, GRA_I);

    for(y=top; y<=bottom; y++)
    { for(q=first,p=pfirst; q<=last; q++,p++)
      { port_out(*p, GRA_D);
        *q = *q;
       }
      pfirst += bp->bwidth;
      first  += LINE_BYTES;
      last   += LINE_BYTES;
     }
   }

  set_mode(save_mode);
  set_color(save_col);
}


void delete_pixrect(char* rect)
{ pixrect* bp =(pixrect*)rect;
  for(int i=0; i<4; i++) free((char*)(bp->plane[i]));
  delete bp;
 }



//------------------------------------------------------------------------------
// mouse cursor
//------------------------------------------------------------------------------

static unsigned char pointer_mask[2][2][14] = {
{{0xc0,0xf0,0x7c,0x7f,0x3f,0x3f,0x1f,0x1f,0x0d,0x0c,0x00,0x00,0x00,0x00},
 {0x00,0x00,0x00,0x00,0xc0,0xc0,0x00,0x80,0xc0,0xe0,0x70,0x38,0x1c,0x0c}},

{{0x00,0x00,0x00,0x00,0x03,0x03,0x00,0x01,0x03,0x07,0x0e,0x1c,0x38,0x30},
 {0x03,0x0f,0x3e,0xfe,0xfc,0xfc,0xf8,0xf8,0xb0,0x30,0x00,0x00,0x00,0x00} }
};


static void pointer_pixel(int x, int y)
{ if (x < 0 || x > DISP_MAX_X || y < 0 || y > DISP_MAX_Y) return;
  VIDEO_PTR p = VIDEO + y*LINE_BYTES + (x>>3);
  port_out(8, GRA_I);
  port_out((0x80 >> (x & 7)), GRA_D);
  *p = *p;
}

void draw_pointer(int mouse_x, int mouse_y, int shape)
{
  register int x1 = mouse_x % 8;
  register int x2 = 8-x1;
  register int x = mouse_x/8;
  VIDEO_PTR start = VIDEO + LINE_BYTES*mouse_y + x;
  register VIDEO_PTR q;
  register unsigned char c;

  set_color(black);

  if (shape == 1)
  { pointer_pixel(mouse_x-3,mouse_y);
    pointer_pixel(mouse_x-2,mouse_y);
    pointer_pixel(mouse_x-1,mouse_y);
    pointer_pixel(mouse_x,mouse_y);
    pointer_pixel(mouse_x+1,mouse_y);
    pointer_pixel(mouse_x+2,mouse_y);
    pointer_pixel(mouse_x+3,mouse_y);
    pointer_pixel(mouse_x,mouse_y-3);
    pointer_pixel(mouse_x,mouse_y-2);
    pointer_pixel(mouse_x,mouse_y-1);
    pointer_pixel(mouse_x,mouse_y+1);
    pointer_pixel(mouse_x,mouse_y+2);
    pointer_pixel(mouse_x,mouse_y+3);
    // circle(mouse_x,mouse_y,3);
    return;
   }


  int a = 0;
  int b = 14;
  int d = 1;
  int k = 0;

  if (mouse_y + 15 > DISP_MAX_Y)
  { a = 13;
    b = -1;
    d = -1;
    start -= 14*LINE_BYTES;
   }

  if (x > 77)
  { start -= 2;
    k = 1;
   }

  port_out(8, GRA_I);

  for(int i=a; i!=b; i+=d)
  { q = start;
    c = (pointer_mask[k][0][i]>>x1);
    port_out(c, GRA_D);
    *q = *q;
    ++q;
    c = ((pointer_mask[k][1][i]>>x1) | (pointer_mask[k][0][i]<<x2));
    port_out(c, GRA_D);
    *q = *q;

    if (x2)
    { ++q;
      c = (pointer_mask[k][1][i]<<x2);
      port_out(c, GRA_D);
      *q = *q;
     }
    start+=LINE_BYTES;
   }
}

