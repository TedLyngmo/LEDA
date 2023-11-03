/*******************************************************************************
+
+  LEDA-R  3.2.2
+
+  _os2.c
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, Germany     
+  All rights reserved.
+ 
*******************************************************************************/


//-----------------------------------------------------------------------------
// basic graphics routines for OS/2  (PM)
//
// s.n. (May & April 1995)
//-----------------------------------------------------------------------------

#include <LEDA/impl/x_basic.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define INCL_WIN
#define INCL_GPI
#include <os2.h>

#define LEDA_ID         1
#define TEXT_FONT_ID    1
#define BOLD_FONT_ID    2

#define IDM_FILE        1
#define IDM_EXIT        2
#define IDM_FONT        3


typedef void (*redraw_fct)();

struct OS2_Window {
HWND hwndFrame;
HWND hwnd;
HPS  hps_mem;
HPS  hps_scr;
int  bg_col;
int  xmax;
int  ymax;
int  refresh;
int  iconized;
redraw_fct redraw;
};


#define YCOORD(w,ypix)  (wlist[w].ymax - ypix)


// global data

static HAB hab;
static HMQ hmq;


static FATTRS TextFattrs;
static FATTRS BoldFattrs;
static FATTRS MesgFattrs;

static FONTMETRICS FontMetrics;

struct event {
int win;
int kind;
int val;
int x;
int y;
unsigned long t;
};

static event cur_event;

#define  MAX_WIN 32

static OS2_Window wlist[MAX_WIN+1];
static Window wcount = 0;

static LONG   rgb_table[32];

static ULONG* color_table;
static ULONG  color_table_src[16];
static ULONG  color_table_xor[16];

static long mode_table[4];
static long lstyle_table[4];

static int GCOLOR;
static int MODE;
static int LSTYLE;
static int LWIDTH;


static int shift_key_down = 0;
static int ctrl_key_down = 0;
static int alt_key_down = 0;


/* display */

static void message(char* s)
{ WinAlarm( HWND_DESKTOP, WA_ERROR );
  WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, (PSZ) s, 
                 (PSZ)"LEDA WINDOW MESSAGE", 256, MB_OK);
} 

static void message(char* s, int i)
{ WinAlarm( HWND_DESKTOP, WA_ERROR );
  char msg[256];
  sprintf(msg,s,i);
  WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, (PSZ) msg, 
                (PSZ)"LEDA WINDOW MESSAGE", 256, MB_OK);
} 

static void message(char* s, int i, int j)
{ WinAlarm( HWND_DESKTOP, WA_ERROR );
  char msg[256];
  sprintf(msg,s,i,j);
  WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, (PSZ) msg, 
                (PSZ)"LEDA WINDOW MESSAGE", 256, MB_OK);
} 


static void init_palette(HWND hwnd,HPS hps)
{

#define LONGFromRGB(R,G,B) (LONG)(((LONG)R<<16)+((LONG)G<<8)+(LONG)B)

             // wi bl  re  gr  bl  ye  vi  or  cy  br  pi  gr  bl  g1  g2  g3
unsigned char 
      R[16] = {255, 0,255, 32,  0,255,160,255,  0,192,255,  0,  0,212,180,116};
unsigned char 
      G[16] = {255, 0,  0,255,  0,255,  0,160,192,112,  0,160,128,212,180,116};
unsigned char 
      B[16] = {255, 0,  0,  0,213,  0,208,  0,192, 56,255,100,255,212,180,116};

  for (int i=0; i < 16; i++) 
  { rgb_table[2*i] = i;
    rgb_table[2*i+1] = LONGFromRGB(R[i],G[i],B[i]);
   }

  if (!GpiCreateLogColorTable(hps,LCOL_PURECOLOR,LCOLF_INDRGB,0,32,rgb_table))
     message("cannot create color table");

/*
   ULONG   cclr;
   int i;
   for (i=0; i < 16; i++) 
      rgb_table[i] = LONGFromRGB(R[i],G[i],B[i]);

   HPAL hPal = GpiCreatePalette( hab,
   			         (ULONG) LCOL_PURECOLOR,
			         (ULONG) LCOLF_CONSECRGB,
			         32,
			         rgb_table );

   if( hPal == NULLHANDLE || hPal == GPI_ERROR)
      message("GpiCreatePalette Error");
   else 
      if (GpiSelectPalette(hps, hPal ) == PAL_ERROR)
        message("GpiSelectPalette Error");
      else 
        if (WinRealizePalette(hwnd,hps,&cclr) == PAL_ERROR)
          message("WinRealizePalette Error");
*/

}


void open_display(void)
{ hab = WinInitialize (0);
  hmq = WinCreateMsgQueue (hab, 0);
  int i;
  for(i=0; i < 16; i++) color_table_src[i] = color_table_xor[i] = i;

  color_table_xor[white] = black;
  color_table_xor[black] = white;
  color_table_xor[red]   = cyan;
  color_table_xor[green] = violet;
  color_table_xor[blue]  = yellow;
  color_table_xor[yellow]= blue;
  color_table_xor[violet]= green;
  color_table_xor[orange]= blue2;
  color_table_xor[blue2] = orange;
  color_table_xor[pink]  = green;

  color_table = color_table_src;

  GCOLOR = black;

  mode_table[src_mode] = FM_OVERPAINT;
  mode_table[or_mode]  = FM_OR;
  mode_table[xor_mode] = FM_XOR;
  mode_table[and_mode] = FM_AND;
  MODE = src_mode;

  lstyle_table[dotted] = LINETYPE_DOT;
  lstyle_table[dashed] = LINETYPE_LONGDASH;
  lstyle_table[solid]  = LINETYPE_SOLID;
  LSTYLE = solid;
  LWIDTH = 1;

  HPS hps = WinGetPS(HWND_DESKTOP);

  TextFattrs.usRecordLength  = sizeof(FATTRS);
  TextFattrs.fsSelection     = 0;
  TextFattrs.lMatch          = 0L;
  TextFattrs.idRegistry      = 0;
  TextFattrs.lMaxBaselineExt = 14L;
  TextFattrs.lAveCharWidth   = 6L;
  TextFattrs.fsType          = 0;
  strcpy(TextFattrs.szFacename,"System VIO");

  GpiCreateLogFont(hps,(PSTR8)NULL,TEXT_FONT_ID,&TextFattrs);
  GpiSetCharSet(hps,TEXT_FONT_ID);
  GpiQueryFontMetrics(hps,sizeof(FONTMETRICS),(PFONTMETRICS)&FontMetrics);

  BoldFattrs.usRecordLength  = sizeof(FATTRS);
  BoldFattrs.fsSelection     = 0;
  BoldFattrs.lMatch          = 0L;
  BoldFattrs.idRegistry      = 0;
  BoldFattrs.lMaxBaselineExt = 14L;
  BoldFattrs.lAveCharWidth   = 8L;
  BoldFattrs.fsType          = 0;
  strcpy(BoldFattrs.szFacename,"System VIO");

  WinReleasePS(hps);
}


void close_display(void)
{ WinDestroyMsgQueue (hmq);
  WinTerminate (hab);
 }


void flush_display(void)
{ //QMSG qmsg;
  //if (WinPeekMsg(hab,&qmsg,0L,0,0,PM_REMOVE)) WinDispatchMsg (hab, &qmsg);
 }


static void refresh_win(Window w, int x0=0, int y0=0, int x1=0, int y1=0)
{ 
  if (wlist[w].refresh) 
  { if (x0 == 0 && x1 == 0) x1 = wlist[w].xmax;
    if (y0 == 0 && y1 == 0) y1 = wlist[w].ymax;
    if (x0 > x1) { int tmp = x0; x0 = x1; x1 = tmp; }
    if (y0 > y1) { int tmp = y0; y0 = y1; y1 = tmp; }
    int d = LWIDTH;
    POINTL aptl[4];
    aptl[0].x = x0-d;
    aptl[0].y = y0-d;
    aptl[1].x = x1+d;
    aptl[1].y = y1+d;
    aptl[2].x = x0-d;
    aptl[2].y = y0-d;
    aptl[3].x = x1+d;
    aptl[3].y = y1+d;
    GpiBitBlt(wlist[w].hps_scr,wlist[w].hps_mem,4,aptl,ROP_SRCCOPY,BBO_IGNORE);
  }
}



void start_batch(Window w) {  wlist[w].refresh = 0; }

void end_batch(Window w) 
{  wlist[w].refresh = 1; 
   refresh_win(w);
}
   

int  display_width(void)
{ return WinQuerySysValue(HWND_DESKTOP, SV_CXSCREEN); }

int  display_height(void)
{ return WinQuerySysValue(HWND_DESKTOP, SV_CYSCREEN); }

/*
int  mouse_buttons(void)
{ return WinQuerySysValue(HWND_DESKTOP, SV_CMOUSEBUTTONS); }
*/


int  display_depth(void) { return 16; }


/* windows */

static void font_dialog(HWND hwnd)
{ 
  FONTDLG FontDlg;
  char szFamilyname[FACESIZE];
  szFamilyname[0] = 0;
  memset((PCH)&FontDlg,0,sizeof(FONTDLG));
  FontDlg.cbSize         = sizeof(FONTDLG);
  FontDlg.hpsScreen      = WinGetPS(hwnd);
  FontDlg.pszFamilyname  = (PSZ)szFamilyname;
  FontDlg.usFamilyBufLen = FACESIZE;
  FontDlg.pszPreview     = (PSZ)"Sample Text";
  FontDlg.fl             = FNTS_RESETBUTTON | FNTS_CENTER |
                           FNTS_INITFROMFATTRS | FNTS_BITMAPONLY;
  FontDlg.flStyle        = FATTR_SEL_ITALIC;
  FontDlg.clrFore        = CLR_BLACK;
  FontDlg.clrBack        = CLR_PALEGRAY;
  FontDlg.fAttrs         = TextFattrs;
  FontDlg.pszTitle       = (PSZ)"FONT SELECTION";
  
  WinFontDlg(HWND_DESKTOP,hwnd,(PFONTDLG)&FontDlg);
  if (FontDlg.lReturn == DID_OK)
  {  char msg[256];
     TextFattrs = FontDlg.fAttrs;
     sprintf(msg,"Facename = %s  MaxBaseLinExt = %d CharWidth = %d", 
             TextFattrs.szFacename,
             TextFattrs.lMaxBaselineExt,
             TextFattrs.lAveCharWidth);
     message(msg);
     GpiCreateLogFont(FontDlg.hpsScreen,(PSTR8)NULL,TEXT_FONT_ID,&TextFattrs);
     GpiSetCharSet(FontDlg.hpsScreen,TEXT_FONT_ID);
     GpiQueryFontMetrics(FontDlg.hpsScreen,sizeof(FONTMETRICS),(PFONTMETRICS)&FontMetrics);
   }

  WinReleasePS(FontDlg.hpsScreen);
}


static MRESULT EXPENTRY ClientWndProc (HWND hwnd, ULONG msg,
                                       MPARAM mp1, MPARAM mp2)
{
  for(int w = 1; w<=wcount; w++)
     if (wlist[w].hwnd == hwnd) break;

  cur_event.win = w;

  switch (msg) {

  case WM_SIZE: 
        { RECTL rcl;
          WinQueryWindowRect(hwnd, &rcl);
          wlist[w].xmax = rcl.xRight;
          wlist[w].ymax = rcl.yTop;
          if (rcl.xRight != 0  && !wlist[w].iconized) 
          { cur_event.kind = configure_event;
            cur_event.x = rcl.xRight;
            cur_event.y = rcl.yTop;
            if (wlist[w].redraw) clear_window(w);
           }
           wlist[w].iconized = (rcl.xRight == 0);

/*
          // have to adjust bitmap (how is it done ?)
          HPS hps = wlist[w].hps_scr;
          BITMAPINFOHEADER2 bmp = {16, 0, 0, 4, 1}; 
          bmp.cx = cur_event.x;
          bmp.cy = cur_event.y;
          HBITMAP hbm = GpiCreateBitmap(hps, &bmp, 0L, NULL, NULL);
          if (hbm == 0) message("cannot create bitmap");
          HBITMAP hbm_prev = GpiSetBitmap(hps, hbm);
          GpiDeleteBitmap(hbm_prev);
*/
          break;
         }

  case WM_PAINT: 
        { HPS hps = WinBeginPaint(hwnd,wlist[w].hps_scr,0); 
          refresh_win(w);
          WinEndPaint(hps);
          return ((MRESULT)0);
         }

  case WM_QUIT: 
  case WM_CLOSE: 
         cur_event.kind = destroy_event;
         close_display();
         exit(0);

  case WM_BUTTON1DBLCLK: 
  case WM_BUTTON1DOWN: 
         cur_event.kind = button_press_event;
         cur_event.val = 1;
         cur_event.x = SHORT1FROMMP(mp1);
         cur_event.y = YCOORD(w,SHORT2FROMMP(mp1));
         if (shift_key_down) cur_event.val = -cur_event.val;
         if (ctrl_key_down)  cur_event.val += 3;
         break;

  case WM_BUTTON2DBLCLK: 
  case WM_BUTTON2DOWN: 
         cur_event.kind = button_press_event;
         cur_event.val = 3;
         cur_event.x = SHORT1FROMMP(mp1);
         cur_event.y = YCOORD(w,SHORT2FROMMP(mp1));
         if (shift_key_down) cur_event.val = -cur_event.val;
         if (ctrl_key_down)  cur_event.val += 3;
         break;

/*
  case WM_BUTTON3DOWN: 
*/

  case WM_BUTTON1UP: 
         cur_event.kind = button_release_event;
         cur_event.val = 1;
         cur_event.x = SHORT1FROMMP(mp1);
         cur_event.y = YCOORD(w,SHORT2FROMMP(mp1));
         if (shift_key_down) cur_event.val = -cur_event.val;
         if (ctrl_key_down)  cur_event.val += 3;
         break;

  case WM_BUTTON2UP: 
         cur_event.kind = button_release_event;
         cur_event.val = 3;
         cur_event.x = SHORT1FROMMP(mp1);
         cur_event.y = YCOORD(w,SHORT2FROMMP(mp1));
         if (shift_key_down) cur_event.val = -cur_event.val;
         if (ctrl_key_down)  cur_event.val += 3;
         break;

/*
  case WM_BUTTON3UP: 
*/

  case WM_MOUSEMOVE: 
         cur_event.kind = motion_event;
         cur_event.x = SHORT1FROMMP(mp1);
         cur_event.y = YCOORD(w,SHORT2FROMMP(mp1));
         break;

  case WM_CHAR: 
        { USHORT fsKeyFlags = (USHORT) SHORT1FROMMP(mp1);
          if (fsKeyFlags & KC_KEYUP) 
             { //key up
               shift_key_down = 0;
               ctrl_key_down = 0;
               alt_key_down = 0;
              }
          else // key down
             { if (fsKeyFlags & KC_SHIFT) shift_key_down = 1;
               if (fsKeyFlags & KC_CTRL)  ctrl_key_down = 1;
               if (fsKeyFlags & KC_ALT)   alt_key_down = 1;
               if (fsKeyFlags & KC_CHAR )
               { cur_event.val = SHORT1FROMMP(mp2);
                 cur_event.kind = key_press_event;
                }
              }
           break;
         }


    case WM_ERASEBACKGROUND:
          return (MRFROMLONG(1L));
     

    case WM_COMMAND:
      switch (SHORT1FROMMP (mp1))
      { 
        case IDM_FONT:
           font_dialog(hwnd);
           return ((MRESULT)0);

        case IDM_EXIT:
           WinSendMsg (hwnd, WM_CLOSE, NULL, NULL);
           return ((MRESULT)0);
        }

    }

   return WinDefWindowProc (hwnd, msg, mp1, mp2);


}



Window open_window(int x,int y,int width,int height,int bg_col, 
                   const char* header, const char* label)
{
  ULONG flFrameFlags;
  HWND hwndFrame;
  HWND hwnd;

  if (wcount == MAX_WIN) 
      message("Maximal number of windows (%d) exceeded",MAX_WIN);

  WinRegisterClass (hab,(PSZ)"LEDA-WINDOW", ClientWndProc, CS_SAVEBITS, 0L);

  //WinRegisterClass (hab,(PSZ)"LEDA-WINDOW", ClientWndProc, 0, 0L);

  flFrameFlags = (FCF_TITLEBAR      | 
                  FCF_SYSMENU       |
                  FCF_SIZEBORDER    | 
                  FCF_MINMAX        |
                  FCF_ACCELTABLE    |
//                FCF_MENU          | 
//                FCF_SHELLPOSITION | 
                  FCF_ICON          | 
                  FCF_TASKLIST);

  FRAMECDATA FrameData;
  FrameData.cb = sizeof(FRAMECDATA);
  FrameData.flCreateFlags = flFrameFlags;
  FrameData.hmodResources = 0;
  FrameData.idResources = LEDA_ID;

  hwndFrame = WinCreateWindow (HWND_DESKTOP, WC_FRAME, (PSZ)header,
                               0L, 0, 0, 0, 0, 0, HWND_TOP, LEDA_ID,
                               (PVOID)(PFRAMECDATA)&FrameData, NULL);


  hwnd = WinCreateWindow(hwndFrame,(PSZ)"LEDA-WINDOW", NULL, 
                               0L, 0, 0, 0, 0, hwndFrame, HWND_TOP, FID_CLIENT,
                               NULL, NULL);

  WinShowWindow(hwndFrame,TRUE);

  //height += WinQuerySysValue(HWND_DESKTOP, SV_CYMENU); 

  height += WinQuerySysValue(HWND_DESKTOP, SV_CYTITLEBAR); 
  height += WinQuerySysValue(HWND_DESKTOP, SV_CYSIZEBORDER);
  width  += 2*WinQuerySysValue(HWND_DESKTOP, SV_CXSIZEBORDER);

  y = WinQuerySysValue(HWND_DESKTOP, SV_CYSCREEN) - height - y; 

  WinSetWindowPos(hwndFrame, 0L, x, y, width, height, SWP_MOVE | SWP_SIZE );


  // create screen presentation space

  SIZEL sizel = {0,0};

  HDC hdc = WinOpenWindowDC(hwnd);
  HPS hps_scr = GpiCreatePS(hab,hdc,&sizel,PU_PELS | GPIF_DEFAULT | 
                                              GPIT_NORMAL | GPIA_ASSOC);

  if (hps_scr == 0) message("cannot create client hps");

 
 // create memory device context and presentation space, 

  DEVOPENSTRUC dop = {NULL,(PSZ)"DISPLAY",NULL,NULL,NULL,NULL,NULL,NULL,NULL};

  HDC hdc_mem = DevOpenDC(hab, OD_MEMORY, (PSZ)"*", 5L,(PDEVOPENDATA)&dop, 
                                                                NULLHANDLE);

  HPS hps_mem = GpiCreatePS(hab,hdc_mem,&sizel, GPIA_ASSOC | GPIT_NORMAL | PU_PELS);
  if (hps_mem == 0) message("cannot create memory hps");

  BITMAPINFOHEADER2 bmp = {16, 0, 0, 4, 1}; 
  bmp.cx =  width;
  bmp.cy =  height;
  HBITMAP hbm = GpiCreateBitmap(hps_mem, &bmp, 0L, NULL, NULL);
  if (hbm == 0) message("cannot create bitmap");
  GpiSetBitmap(hps_mem, hbm);
 

  // create and set font

  if (GpiCreateLogFont(hps_mem,(PSTR8)NULL,TEXT_FONT_ID,&TextFattrs) == FALSE)
     message("cannot create font");

  GpiSetCharSet(hps_mem,TEXT_FONT_ID);
  GpiQueryFontMetrics(hps_mem,sizeof(FONTMETRICS),(PFONTMETRICS)&FontMetrics);


  QMSG qmsg;
  while (WinPeekMsg(hab,&qmsg,0L,0,0,PM_REMOVE)) WinDispatchMsg (hab, &qmsg);

  // initialize color table

  init_palette(hwnd,hps_mem);


  // set default values for drawing parameters

  GpiSetMix(hps_mem,mode_table[MODE]);
  GpiSetColor(hps_mem,color_table[GCOLOR]);
  GpiSetBackColor(hps_mem,color_table[bg_col]);
  GpiSetLineType(hps_mem,lstyle_table[LSTYLE]);


  // get window dimensions

  SWP swp;
  WinQueryWindowPos(hwnd, &swp);

  wcount++;
  wlist[wcount].hwndFrame = hwndFrame;
  wlist[wcount].hwnd = hwnd;
  wlist[wcount].hps_mem = hps_mem;
  wlist[wcount].hps_scr = hps_scr;
  wlist[wcount].bg_col = bg_col;
  wlist[wcount].refresh = 1;
  wlist[wcount].iconized = 0;
  wlist[wcount].redraw = 0;
  wlist[wcount].xmax = swp.cx-1;
  wlist[wcount].ymax = swp.cy-1;

  WinSetActiveWindow(HWND_DESKTOP,hwndFrame);

  clear_window(wcount);
  return wcount;
}




void close_window(Window win) 
{ GpiAssociate(wlist[win].hps_mem, NULLHANDLE); 
  GpiDestroyPS(wlist[win].hps_mem); 
  GpiAssociate(wlist[win].hps_scr, NULLHANDLE); 
  GpiDestroyPS(wlist[win].hps_scr); 
  WinDestroyWindow(wlist[win].hwndFrame); 
 }

void set_header(Window win, const char *s)
{ WinSetWindowText(wlist[win].hwndFrame, (PSZ)s);
  WinInvalidateRect(WinWindowFromID(wlist[win].hwndFrame, FID_TITLEBAR),
                                                     (PRECTL)NULL, FALSE);
 }


void set_redraw(Window w, void (*f)()) 
{ wlist[w].redraw = f; }


int window_width(Window win)
{ SWP swp;
  WinQueryWindowPos(wlist[win].hwnd, &swp);
  return swp.cx;
 }

int window_height(Window win)
{ SWP swp;
  WinQueryWindowPos(wlist[win].hwnd, &swp);
  return swp.cy;
 }


void window_position(Window win, int* x, int* y)
{ SWP swp;
  WinQueryWindowPos(wlist[win].hwnd, &swp);
  *x = swp.x;
  *y = swp.y;
 }


void clear_window(Window win)
{ // dispatch all pending messages first
  QMSG qmsg;
  while (WinPeekMsg(hab,&qmsg,0L,0,0,PM_REMOVE)) WinDispatchMsg (hab, &qmsg);
  RECTL rcl;
  WinQueryWindowRect(wlist[win].hwnd, &rcl);
  int save_mode = set_mode(src_mode);
  WinFillRect(wlist[win].hps_mem,&rcl,color_table[wlist[win].bg_col]);
  set_mode(save_mode);
  refresh_win(win);
}




/* drawing */

void line(Window win, int x1, int y1, int x2, int y2)
{ POINTL pos;
  HPS hps = wlist[win].hps_mem;
  y1 = YCOORD(win,y1);
  y2 = YCOORD(win,y2);
  GpiBeginPath(hps,1L);
  pos.x = x1;
  pos.y = y1;
  GpiMove(hps,&pos);
  pos.x = x2;
  pos.y = y2;
  GpiLine(hps,&pos);
  GpiEndPath(hps);
  GpiStrokePath(hps,1L,0L);
  refresh_win(win,x1,y1,x2,y2);
 }

void rectangle(Window win, int x1, int y1, int x2, int y2)
{ POINTL pos;
  HPS hps = wlist[win].hps_mem;
  y1 = YCOORD(win,y1);
  y2 = YCOORD(win,y2);
  pos.x = x1;
  pos.y = y1;
  GpiMove(hps,&pos);
  pos.x = x2;
  pos.y = y2;
  GpiBox(hps,DRO_OUTLINE,&pos,0,0);
  refresh_win(win,x1,y1,x2,y2);
 }

void box(Window win, int x1, int y1, int x2, int y2)
{ POINTL pos;
  HPS hps = wlist[win].hps_mem;
  y1 = YCOORD(win,y1);
  y2 = YCOORD(win,y2);
  pos.x = x1;
  pos.y = y1;
  GpiMove(hps,&pos);
  pos.x = x2;
  pos.y = y2;
  GpiBox(hps,DRO_OUTLINEFILL,&pos,0,0);
  refresh_win(win,x1,y1,x2,y2);
 }

void circle(Window win, int x, int y, int r)
{ HPS hps = wlist[win].hps_mem;
  ARCPARAMS  arcparams; 
  POINTL     center;
  y = YCOORD(win,y);
  arcparams.lP = r;
  arcparams.lQ = r;
  arcparams.lR = 0;
  arcparams.lS = 0;
  GpiSetArcParams(hps, (PARCPARAMS)&arcparams);
  center.x = x;
  center.y = y;
  GpiBeginPath(hps,1L);
  GpiMove (hps,&center);
  GpiFullArc(hps,DRO_OUTLINE,MAKEFIXED(1,0));
  GpiEndPath(hps);
  GpiStrokePath(hps,1L,0L);
  int d= int(0.7 * r + 1);
  refresh_win(win,x-r,y-d,x-d,y+d);
  refresh_win(win,x+d,y-d,x+r,y+d);
  refresh_win(win,x-d,y-r,x+d,y-d);
  refresh_win(win,x-d,y+d,x+d,y+r);
}


void fill_circle(Window win, int x, int y, int r)
{ HPS hps = wlist[win].hps_mem;
  ARCPARAMS  arcparams; 
  POINTL     center;
  y = YCOORD(win,y);
  arcparams.lP = r;
  arcparams.lQ = r;
  arcparams.lR = 0;
  arcparams.lS = 0;
  GpiSetArcParams(hps, (PARCPARAMS)&arcparams);
  center.x = x;
  center.y = y;
  GpiMove (hps,&center);
  GpiFullArc(hps,DRO_OUTLINEFILL,MAKEFIXED(1,0));
  refresh_win(win,x-r,y-r,x+r,y+r);
}


void pixel(Window win, int x, int y)
{ line(win,x,y,x,y); }

void pixels(Window win, int n, int* x, int* y)
{ int save = wlist[win].refresh;
  wlist[win].refresh = 0;
  while (n--) pixel(win,x[n],y[n]);
  wlist[win].refresh = 1;
  refresh_win(win);
}

void point(Window win, int x, int y)
{ HPS hps = wlist[win].hps_mem;
  y = YCOORD(win,y);
  POINTL pos;
  pos.x = x-2;
  pos.y = y-2;
  GpiMove(hps,&pos);
  pos.x = x+2;
  pos.y = y+2;
  GpiLine(hps,&pos);
  pos.x = x-2;
  pos.y = y+2;
  GpiMove(hps,&pos);
  pos.x = x-1;
  pos.y = y+1;
  GpiLine(hps,&pos);
  pos.x = x+1;
  pos.y = y-1;
  GpiMove(hps,&pos);
  pos.x = x+2;
  pos.y = y-2;
  GpiLine(hps,&pos);
  refresh_win(win,x-2,y-2,x+2,y+2);
 }


void arc(Window win,int mx,int my,int r1,int r2,double start,double angle)
{ HPS hps = wlist[win].hps_mem;
  POINTL p[3];
  p[0].x = mx + int(r1*cos(start));
  p[0].y = YCOORD(win,my + int(r1*sin(start)));
  p[1].x = mx + int(r1*cos(start+angle/2));
  p[1].y = YCOORD(win,my + int(r1*sin(start+angle/2)));
  p[2].x = mx + int(r1*cos(start+angle));
  p[2].y = YCOORD(win,my + int(r1*sin(start+angle)));
  GpiBeginPath(hps,1L);
  GpiMove(hps,p);
  GpiPointArc(hps,p+1);
  GpiEndPath(hps);
  GpiStrokePath(hps,1L,0L);
  refresh_win(win,mx-r1,YCOORD(win,my-r2),mx+r1,YCOORD(win,my+r2));
}


void fill_arc(Window win,int x0,int y0,int r1,int r2,double start,double angle)
{}


void ellipse(Window win, int x, int y, int r1, int r2)
{ HPS hps = wlist[win].hps_mem;
  ARCPARAMS  arcparams; 
  POINTL     center;
  y = YCOORD(win,y);
  arcparams.lP = r1;
  arcparams.lQ = r2;
  arcparams.lR = 0;
  arcparams.lS = 0;
  GpiSetArcParams(hps, (PARCPARAMS)&arcparams);
  center.x = x;
  center.y = y;
  GpiMove (hps,&center);
  GpiFullArc(hps,DRO_OUTLINE,MAKEFIXED(1,0));
  refresh_win(win,x-r1,y-r2,x+r1,y+r2);
}


void fill_ellipse(Window win, int x, int y, int r1, int r2)
{ HPS hps = wlist[win].hps_mem;
  ARCPARAMS  arcparams; 
  POINTL     center;
  y = YCOORD(win,y);
  arcparams.lP = r1;
  arcparams.lQ = r2;
  arcparams.lR = 0;
  arcparams.lS = 0;
  GpiSetArcParams(hps, (PARCPARAMS)&arcparams);
  center.x = x;
  center.y = y;
  GpiMove (hps,&center);
  GpiFullArc(hps,DRO_OUTLINEFILL,MAKEFIXED(1,0));
  refresh_win(win,x-r1,y-r2,x+r1,y+r2);
}


void fill_polygon(Window win, int n, int* xcoord, int* ycoord)
{ HPS hps = wlist[win].hps_mem;
  POINTL*  p = new POINTL[n];
  for(int i=0; i < n; i++) 
  { p[i].x = xcoord[i];
    p[i].y = YCOORD(win,ycoord[i]);
   }
  GpiMove(hps,p);
  GpiBeginArea(hps,BA_BOUNDARY | BA_ALTERNATE);
  GpiPolyLine(hps,n,p);
  GpiEndArea(hps);

  int xmin = 10000;
  int ymin = 10000;
  int xmax = 0;
  int ymax = 0;

  for(i=0; i < n; i++) 
  { if (xmin > p[i].x) xmin = p[i].x;
    if (xmax < p[i].x) xmax = p[i].x;
    if (ymin > p[i].y) ymin = p[i].y;
    if (ymax < p[i].y) ymax = p[i].y;
   }
  refresh_win(win,xmin,ymin,xmax,ymax);
 }


void put_text(Window win, int x, int y, const char* s, int l, int opaque)
{ int h = text_height(s);
  int w = text_width(s);

  if (opaque)
  { int save_col = set_color(white);
    int save_mode = set_mode(src_mode);
    box(win,x,y,x+w,y+h);
    set_color(save_col);
    set_mode(save_mode);
   }

  y = YCOORD(win,y);

  POINTL pos;
  pos.x = x;
  pos.y = y - FontMetrics.lMaxAscender;
  int len = strlen(s);
  if (len > 512) len = 512;
  if (len > l) len = l;
  GpiCharStringAt(wlist[win].hps_mem,&pos,len, (PSZ)s);
  refresh_win(win,x,y,x+w,y-h);
 }
  
void put_text(Window win, int x, int y, const char* s, int opaque)
{ put_text(win, x, y, s, 512, opaque); }


void put_ctext(Window win, int x, int y, const char* s, int opaque)
{ int h = text_height(s);
  int w = text_width(s);
  put_text(win,x-w/2,y-h/2,s,opaque);
}


char* create_pixrect(Window win, int x1, int y1, int x2, int y2)
{ return 0; }

void insert_pixrect(Window win, int x, int y, char* rect)
{}

void delete_pixrect(char* rect)
{}

void copy_pixrect(Window win, int x1, int y1, int x2, int y2, int x, int y)
{}

void insert_bitmap(Window win, int width, int height, char* data)
{}

void show_coordinates(Window win, const char* s)
{ int col = set_color(black);
  int mode = set_mode(src_mode);
  put_text(win,window_width(win)-120,0,s,1); 
  set_mode(mode);
  set_color(col);
 }


/* fonts and text */

int  load_text_font(const char* font_name) { return 0; }
int  load_bold_font(const char* font_name) { return 0; }
int  load_message_font(const char* font_name) { return 0; }

int  set_font(const char* fname) { return 0; }

void set_text_font(void)
{ for(int win=1; win <=wcount; win++)
  { HPS hps = wlist[win].hps_mem;
    GpiCreateLogFont(hps,(PSTR8)NULL,TEXT_FONT_ID,&TextFattrs);
    GpiSetCharSet(hps,TEXT_FONT_ID);
    GpiQueryFontMetrics(hps,sizeof(FONTMETRICS),(PFONTMETRICS)&FontMetrics);
   }
}

void set_bold_font(void)
{ for(int win=1; win <=wcount; win++)
  { HPS hps = wlist[win].hps_mem;
    GpiCreateLogFont(hps,(PSTR8)NULL,TEXT_FONT_ID,&TextFattrs);
    GpiSetCharSet(hps,TEXT_FONT_ID);
    GpiQueryFontMetrics(hps,sizeof(FONTMETRICS),(PFONTMETRICS)&FontMetrics);
   }
}

void set_message_font(void)
{ for(int win=1; win <=wcount; win++)
  { HPS hps = wlist[win].hps_mem;
    GpiCreateLogFont(hps,(PSTR8)NULL,BOLD_FONT_ID,&BoldFattrs);
    GpiSetCharSet(hps,BOLD_FONT_ID);
    GpiQueryFontMetrics(hps,sizeof(FONTMETRICS),(PFONTMETRICS)&FontMetrics);
   }
}


int text_width(const char* s)
{ return  strlen(s) * FontMetrics.lAveCharWidth; }

int text_height(const char* s) 
{ return  FontMetrics.lMaxBaselineExt; }


/* drawing parameters */

int set_color(int col) 
{ int c = GCOLOR;
  for(int win=1; win <=wcount; win++)
  GpiSetColor(wlist[win].hps_mem,color_table[col]); 
  GCOLOR = col;
  return c;
 }

int set_mode(int mod) 
{ int m = MODE;
  for(int win=1; win <=wcount; win++)
     GpiSetMix(wlist[win].hps_mem,mode_table[mod]); 

  if (mod == xor_mode) // switch to complementary colors
  { 
    color_table = color_table_xor;
   }
  else
    color_table = color_table_src;

  MODE = mod;

  return m;
 }

int set_line_width(int w) 
{ int lw = LWIDTH;
  for(int win=1; win <=wcount; win++)
     GpiSetLineWidthGeom(wlist[win].hps_mem,w); 
  LWIDTH = w;
  return lw;
 }

int set_line_style(int s)
{ int ls = LSTYLE;
  for(int win=1; win <=wcount; win++)
  GpiSetLineType(wlist[win].hps_mem,lstyle_table[s]);
  LSTYLE = s;
  return ls;
 }

void set_read_gc()
{ for(int win=1; win <=wcount; win++)
  { HPS hps = wlist[win].hps_mem;
    GpiSetMix(hps,FM_XOR);
    GpiSetColor(hps,color_table[black]);
    GpiSetLineType(hps,LINETYPE_SOLID);
    GpiSetLineWidthGeom(hps,1);
   }
 }

void reset_gc()
{ for(int win=1; win <=wcount; win++)
  { HPS hps = wlist[win].hps_mem;
    GpiSetMix(hps,mode_table[MODE]);
    GpiSetColor(hps,color_table[black]);
    GpiSetLineType(hps,lstyle_table[LSTYLE]);
    GpiSetLineWidthGeom(hps,LWIDTH);
  }
}




/* colors */

void set_palette(int i, int r, int g, int b)
{ unsigned char R = (unsigned char)r;
  unsigned char G = (unsigned char)g;
  unsigned char B = (unsigned char)b;
  rgb_table[2*i+1] = LONGFromRGB(R,G,B);
  for(int win=1; win<=wcount; win++)
    GpiCreateLogColorTable(wlist[win].hps_mem,LCOL_PURECOLOR,
                                              LCOLF_INDRGB,0L,32L,rgb_table);
}

int  new_color(const char*) { return black; }


/* events */

static QMSG qmsg;
static int putback = 0;
static event old_event;


int  get_next_event(Window* win, int* val, int* x, int* y, unsigned long *t)
{ 
  if (putback) 
   { cur_event = old_event;
     putback = 0;
    }
  else
    { WinGetMsg (hab, &qmsg, 0L, 0, 0);
      cur_event.kind = -1;
      WinDispatchMsg (hab, &qmsg);
      cur_event.t = qmsg.time;
      if (cur_event.kind != -1) old_event = cur_event;
     } 

  *win = cur_event.win;
  *val = cur_event.val;
  *x   = cur_event.x;
  *y   = cur_event.y;
  *t   = cur_event.t;
  return cur_event.kind;
 }


int  check_next_event(Window* win, int* val, int* x, int* y, unsigned long *t)
{ 
  if (putback || WinPeekMsg(hab, &qmsg, 0L, 0, 0, PM_NOREMOVE))
     return get_next_event(win,val,x,y,t); 
  else
     return no_event;
 }

void put_back_event(void) {  putback = 1; }

