#pragma title("3D Text Control  --  Version 1.0 -- (Text3D.C)")
#pragma subtitle("  3D Text Control Control DLL - Interface Definitions")

#pragma info(noext)

#define INCL_DOS                   /* Include OS/2 DOS Kernal           */
#define INCL_GPI                   /* Include OS/2 PM GPI Interface     */
#define INCL_WIN                   /* Include OS/2 PM Windows Interface */

static char *MODID = "@(#)text3d.c:2.03";

#include <malloc.h>
#include <os2.h>
#include <string.h>

#include <pmcx.h>

#include "text3d.h"

/* This module contains an example installable control that can be used */
/* within applications where additional facilities are provided that    */
/* are not found within the default controls of OS/2 PM.                */
/*                                                                      */
/* For complete details regarding the PM Control Extensions (PMCX)      */
/* consult the User's Guide.                                            */
/*                                                                      */
/* The DLL is created using the following command line invocation:      */
/*                                                                      */
/*     Icc -G3e- -O+ -Rn -W3 -C Text3D.C                                */

/* Filename:   Text3D.C                                                 */

/*  Version:   1.0                                                      */
/*  Created:   1993-12-21                                               */
/*  Revised:   1995-11-16                                               */

/* Routines:   BOOL EXPENTRY Text3DRegister(HAB hAB);                   */
/*             BOOL EXPENTRY Text3DQuery(PUSERINFO pControlInfo);       */
/*             MRESULT EXPENTRY Text3DWndProc(HWND hWnd, ULONG msg,     */
/*                                            MPARAM mp1, MPARAM mp2);  */
/*             MRESULT EXPENTRY Text3DStyles(HWND hWnd, ULONG msg,      */
/*                                           MPARAM mp1, MPARAM mp2);   */


/* Copyright ¸ 1989-1996  Prominare Inc.  All Rights Reserved.          */

/* -------------------------------------------------------------------- */

/************************************************************************/
/************************************************************************/
/*                     DISCLAIMER OF WARRANTIES.                        */
/************************************************************************/
/************************************************************************/
/*     The following [enclosed] code is library code created by         */
/*     Prominare Inc.  This library code is  provided to you solely     */
/*     for the purpose of assisting you in the development of your      */
/*     applications.  The code is provided "AS IS", without             */
/*     warranty of any kind.  Prominare Inc. shall not be liable        */
/*     for any damages arising out of your use of the library code,     */
/*     even if they have been advised of the possibility of such        */
/*     damages.                                                         */
/************************************************************************/
/************************************************************************/

/* --- Control Data Structures ---------------------------------------- */


typedef struct _TEXTFIELD          /* tf */
   {
   POINTL aptl[4];                 /* Entry Field Outline Points        */
   RECTL  rcl;                     /* Frame Rectangle                   */
   LONG   lClrLeftTop;             /* Left and Top Edge Colour          */
   LONG   lClrBottomRight;         /* Right and Bottom Edge Colour      */
   BOOL   fRaised;                 /* Raised Field Flag                 */
   LONG   lClrText;                /* Text Colour                       */
   LONG   lClrBackground;          /* Background Colour                 */
   LONG   lClrBorder;              /* Border Colour                     */
   LONG   cText;                   /* Text Length Holder                */
   PSZ    pszText;                 /* Text Holder                       */
   ULONG  flFormat;                /* Formatting Flags                  */
   ULONG  flStyle;                 /* Style Flags                       */
   BOOL   fDraw;                   /* Draw Flag                         */
   POINTL ptlDraw;                 /* Text Drawing Point                */
   } TEXTFIELD ;

typedef TEXTFIELD *PTEXTFIELD;

/* --- Module Prototype Definitions ----------------------------------- */

static VOID CalcSize(HWND hWnd, PRECTL prcl, PTEXTFIELD ptf);
static VOID SetDefaultColours(HWND hWnd, PTEXTFIELD ptf);
static LONG lGetPresParam(HWND hWnd, ULONG ulID1, ULONG ulID2, LONG lDefault);

#define CLENpszClassname 13
static PSZ pszClassname = "Depressed.3D";

#define DT_MASK (DT_LEFT | DT_CENTER | DT_RIGHT | DT_TOP | DT_VCENTER | DT_BOTTOM | DT_HALFTONE | DT_MNEMONIC | DT_WORDBREAK)

BOOL    EXPENTRY Text3DRegister(HAB hAB);
BOOL    EXPENTRY Text3DQuery(PUSERINFO pControlInfo);
MRESULT EXPENTRY Text3DWndProc(HWND hWnd, ULONG msg, MPARAM mp1, MPARAM mp2);
MRESULT EXPENTRY Text3DStyles(HWND hWnd, ULONG msg, MPARAM mp1, MPARAM mp2);

#pragma subtitle("   3D Text Control DLL - Control Initialization Function")
#pragma page ( )

/* --- Text3DRegister ---------------------------------- [ Public ] --- */
/*                                                                      */
/*     This function is used to register the installable control class  */
/*     with OS/2 Presentation Manager.  The registration must use the   */
/*     USER_CWINDOWWORDS to reserve memory for the control to allow for */
/*     proper usage by Resource Editor and for use by the control       */
/*     dialog and window procedures.  The information for the control   */
/*     containing the style, presentation parameters and control data   */
/*     is pointed to by a pointer that can be referenced by the         */
/*     control's dialog and window procedure as required.  The memory   */
/*     for the structure is allocated and controlled through Resource   */
/*     Editor.   The control can reserve more memory for its use        */
/*     by adding the memory required to that of the USER_CWINDOWWORDS   */
/*     constant.                                                        */
/*                                                                      */
/*     Upon Entry:                                                      */
/*                                                                      */
/*     HAB hAB; = Application Anchor Block Handle                       */
/*                                                                      */
/*     Upon Exit:                                                       */
/*                                                                      */
/*     Text3DRegister =  TRUE : Class Registration Successful           */
/*                    = FALSE : Class Registration Failed               */
/*                                                                      */
/* -------------------------------------------------------------------- */

BOOL EXPENTRY Text3DRegister(HAB hAB)

{
                       /* Register the control class with OS/2          */
                       /* Presentation Manager and return registration  */
                       /* result                                        */

return(WinRegisterClass(hAB, pszClassname, Text3DWndProc, CS_SYNCPAINT | CS_SIZEREDRAW, USER_CWINDOWWORDS));

}
#pragma subtitle("   3D Text Control DLL - Query Control Information Function")
#pragma page ( )

/* --- Text3DQuery ------------------------------------- [ Public ] --- */
/*                                                                      */
/*     This function is used to return to the caller information        */
/*     regarding the installable control and its capabilities.  The     */
/*     function should return a true value otherwise Resource           */
/*     Editor will not register the control as being usable.            */
/*                                                                      */
/*     Upon Entry:                                                      */
/*                                                                      */
/*     PUSERINFO pUserInfo; = User Information Pointer                  */
/*                                                                      */
/*     Upon Exit:                                                       */
/*                                                                      */
/*     Text3DQuery =  TRUE : User Information Being Returned            */
/*                 = FALSE : No User Information Available              */
/*                                                                      */
/* -------------------------------------------------------------------- */

BOOL EXPENTRY Text3DQuery(PUSERINFO pUserInfo)

{
                       /* Complete the User Information structure       */
                       /* passed to the function by Resource Editor  */

                       /* Complete the version and number of control    */
                       /* types.  In Version 1.00 of PMCX, only one     */
                       /* control type is used.                         */
pUserInfo->ulMajor = 3UL;
pUserInfo->ulMinor = 0UL;
                       /* Complete the author and control classname     */

memcpy(pUserInfo->szAuthor,    "Prominare Inc.", 15);
memcpy(pUserInfo->szClassname, pszClassname, CLENpszClassname);
memcpy(pUserInfo->szName,      "Text3D", 7);

                       /* Complete the default size and style of the    */
                       /* first user control type                       */

pUserInfo->utDefined[0].cx           = 25L;
pUserInfo->utDefined[0].cy           = 10L;
pUserInfo->utDefined[0].flStyle      = WS_VISIBLE | SS_TEXT | DT_LEFT | DT_VCENTER;

                       /* Set the maximum amount of text control can    */
                       /* accept including NULL termination byte        */

pUserInfo->utDefined[0].cMaxText     = CCHTEXTMAX;

                       /* Save the style's dialogue ID, type, control   */
                       /* data size and count of style masks            */

pUserInfo->utDefined[0].idDlg        = DLG_CTRLUSER;
pUserInfo->utDefined[0].flOptions    = PMCXOPT_REFRESH;
pUserInfo->utDefined[0].ulType       = UTYPE_PRIVATE;
pUserInfo->utDefined[0].cCtlData     = 0UL;
pUserInfo->utDefined[0].cMasks       = 12UL;
pUserInfo->utDefined[0].flStyleType  = STYLETYPE_BITFLAGS;
pUserInfo->utDefined[0].stMasks[0].flStyleMask = SS_TEXT;
pUserInfo->utDefined[0].stMasks[0].idStyle     = IDS_SS_TEXT;
pUserInfo->utDefined[0].stMasks[1].flStyleMask = DT_LEFT;
pUserInfo->utDefined[0].stMasks[1].idStyle     = IDS_DT_LEFT;
pUserInfo->utDefined[0].stMasks[2].flStyleMask = DT_CENTER;
pUserInfo->utDefined[0].stMasks[2].idStyle     = IDS_DT_CENTER;
pUserInfo->utDefined[0].stMasks[3].flStyleMask = DT_RIGHT;
pUserInfo->utDefined[0].stMasks[3].idStyle     = IDS_DT_RIGHT;
pUserInfo->utDefined[0].stMasks[4].flStyleMask = DT_TOP;
pUserInfo->utDefined[0].stMasks[4].idStyle     = IDS_DT_TOP;
pUserInfo->utDefined[0].stMasks[5].flStyleMask = DT_VCENTER;
pUserInfo->utDefined[0].stMasks[5].idStyle     = IDS_DT_VCENTER;
pUserInfo->utDefined[0].stMasks[6].flStyleMask = DT_BOTTOM;
pUserInfo->utDefined[0].stMasks[6].idStyle     = IDS_DT_BOTTOM;
pUserInfo->utDefined[0].stMasks[7].flStyleMask = DT_HALFTONE;
pUserInfo->utDefined[0].stMasks[7].idStyle     = IDS_DT_HALFTONE;
pUserInfo->utDefined[0].stMasks[8].flStyleMask = DT_MNEMONIC;
pUserInfo->utDefined[0].stMasks[8].idStyle     = IDS_DT_MNEMONIC;
pUserInfo->utDefined[0].stMasks[9].flStyleMask = DT_WORDBREAK;
pUserInfo->utDefined[0].stMasks[9].idStyle     = IDS_DT_WORDBREAK;
pUserInfo->utDefined[0].stMasks[10].flStyleMask = DS_DEPRESSED;
pUserInfo->utDefined[0].stMasks[10].idStyle     = IDS_DS_DEPRESSED;
pUserInfo->utDefined[0].stMasks[11].flStyleMask = DS_RAISED;
pUserInfo->utDefined[0].stMasks[11].idStyle     = IDS_DS_RAISED;

                       /* Save the description of the control           */

memcpy(pUserInfo->utDefined[0].szDescription, "3D Text", 8);

                       /* Return the success flag back to Resource      */
                       /* Editor                                        */
return(TRUE);
}
#pragma subtitle("   3D Text Control DLL - Control Window Procedure")
#pragma page( )

/* --- lGetPresParam ---------------------------------- [ Private } --- */
/*                                                                      */
/*     This function is used to retrieve a presentation parameter       */
/*     that may be present.  If the presentation parameter is not,      */
/*     the default colour passed to the function will be used.          */
/*                                                                      */
/*     Upon Entry:                                                      */
/*                                                                      */
/*     HWND  hWnd;     = Window Handle                                  */
/*     ULONG ulID1;    = Presentation Parameter 1 ID                    */
/*     ULONG ulID2;    = Presentation Parameter 2 ID                    */
/*     LONG  lDefault; = Default Colour                                 */
/*                                                                      */
/*     Upon Exit:                                                       */
/*                                                                      */
/*     lGetPresParam = Colour to Use                                    */
/*                                                                      */
/* -------------------------------------------------------------------- */

LONG lGetPresParam(HWND hWnd, ULONG ulID1, ULONG ulID2, LONG lDefault)

{
HPS   hPS;                         /* Presentation Space Handle         */
LONG  lClr;                        /* Presentation Parameter Colour     */
ULONG ulID;                        /* Presentation Parameter ID         */

if ( WinQueryPresParam(hWnd, ulID1, ulID2, &ulID, 4UL, (PVOID)&lClr,
                       QPF_NOINHERIT | QPF_ID2COLORINDEX | QPF_PURERGBCOLOR) )
   return(lClr);
else
   if ( (lDefault >= SYSCLR_SHADOWHILITEBGND) && (lDefault <= SYSCLR_HELPHILITE) )
       return(WinQuerySysColor(HWND_DESKTOP, lDefault, 0L));
   else
       if ( (lClr = GpiQueryRGBColor(hPS = WinGetPS(hWnd), LCOLOPT_REALIZED, lDefault)) == GPI_ALTERROR )
           {
           WinReleasePS(hPS);
           return(lDefault);
           }
       else
           {
           WinReleasePS(hPS);
           return(lClr);
           }
}
#pragma subtitle("   Shadowed Text DLL - Control Window Sizing Procedure")
#pragma page ( )

/* --- SizeText --------------------------------------- [ Private ] --- */
/*                                                                      */
/*     This function is used to calculate the size and position         */
/*     of the text.                                                     */
/*                                                                      */
/*     Upon Entry:                                                      */
/*                                                                      */
/*     HWND       hWnd; = Control Window Handle                         */
/*     PTEXTFIELD ptf;  = Text Information Pointer                      */
/*                                                                      */
/*     Upon Exit:                                                       */
/*                                                                      */
/*     Nothing                                                          */
/*                                                                      */
/* -------------------------------------------------------------------- */

static VOID SizeText(HWND hWnd, PTEXTFIELD ptf)

{
FONTMETRICS fm;                    /* Font Metrics                      */
HPS         hPS;                   /* Presentation Space Handle         */
POINTL      rgptl[TXTBOX_COUNT];   /* Text Box Point Array              */

GpiQueryFontMetrics(hPS = WinGetPS(hWnd), sizeof(FONTMETRICS), &fm);
GpiQueryTextBox(hPS, (LONG)ptf->cText, ptf->pszText, 5L, rgptl);
WinReleasePS(hPS);

if ( ptf->flStyle & DT_RIGHT )
   ptf->ptlDraw.x = ptf->rcl.xRight - rgptl[TXTBOX_CONCAT].x;
else
   if ( ptf->flStyle & DT_CENTER )
       ptf->ptlDraw.x = (ptf->rcl.xRight - ptf->rcl.xLeft) / 2L - rgptl[TXTBOX_CONCAT].x / 2L + ptf->rcl.xLeft;
   else
       ptf->ptlDraw.x = ptf->rcl.xLeft;

if ( ptf->flStyle & DT_TOP )
   ptf->ptlDraw.y = ptf->rcl.yTop - (rgptl[TXTBOX_TOPRIGHT].y - rgptl[TXTBOX_BOTTOMRIGHT].y) + fm.lMaxDescender;
else
   if ( ptf->flStyle & DT_VCENTER )
       ptf->ptlDraw.y = (ptf->rcl.yTop - ptf->rcl.yBottom) / 2L - (rgptl[TXTBOX_TOPRIGHT].y - rgptl[TXTBOX_BOTTOMRIGHT].y) / 2L +
                        fm.lMaxDescender + ptf->rcl.yBottom;
   else
       ptf->ptlDraw.y = ptf->rcl.yBottom + fm.lMaxDescender;
}
#pragma subtitle("   Shadowed Text DLL - Control Window Sizing Procedure")
#pragma page ( )

/* --- CalcSize --------------------------------------- [ Private ] --- */
/*                                                                      */
/*     This function is used to calculate the sizes and positions       */
/*     of the various elements that are used to make up a shadowed      */
/*     text field.                                                      */
/*                                                                      */
/*     Upon Entry:                                                      */
/*                                                                      */
/*     PRECTL     prcl; = Control Rectangle Pointer                     */
/*     PTEXTFIELD ptf;  = Group Box Information Pointer                 */
/*                                                                      */
/*     Upon Exit:                                                       */
/*                                                                      */
/*     Nothing                                                          */
/*                                                                      */
/* -------------------------------------------------------------------- */

static VOID CalcSize(HWND hWnd, PRECTL prcl, PTEXTFIELD ptf)

{

if ( (prcl->yTop <= prcl->yBottom) || (prcl->xRight <= prcl->xLeft) )
   ptf->fDraw = FALSE;
else
   {
   ptf->fDraw = TRUE;
                       /* Save the rectangle for the control            */
   ptf->rcl = *prcl;
                       /* Adjust the rectangle size for the text which  */
                       /* resides inside the 3D frame                   */
   ++ptf->rcl.xLeft;
   ++ptf->rcl.yBottom;
   ptf->rcl.yTop -= 2L;
   ptf->rcl.xRight -= 2L;

   if ( !(ptf->flStyle & DT_WORDBREAK) && ptf->cText )
       SizeText(hWnd, ptf);

               /*  Text display points                                  */
               /*                                                       */
               /*         1                           2                 */
               /*          +-------------------------+                  */
               /*          |                         |                  */
               /*          |                         |                  */
               /*          |                         |                  */
               /*          |                         |                  */
               /*          +-------------------------+                  */
               /*         0 4                         3                 */

   ptf->aptl[0].x = prcl->xLeft;
   ptf->aptl[0].y = prcl->yTop   - 1L;
   ptf->aptl[1].x = prcl->xRight - 1L;
   ptf->aptl[1].y = prcl->yTop   - 1L;
   ptf->aptl[2].x = prcl->xRight - 1L;
   ptf->aptl[2].y = prcl->yBottom;
   ptf->aptl[3].x = prcl->xLeft;
   ptf->aptl[3].y = prcl->yBottom;

   ptf->flFormat = (ptf->flStyle & DT_MASK) | DT_ERASERECT;
   }
}
#pragma subtitle("   Shadowed Text DLL - Default Colours Procedure")
#pragma page( )

/* --- SetDefaultColours ------------------------------ [ Private ] --- */
/*                                                                      */
/*     This function is used to set the default colours that the        */
/*     image button should use within the internal paint routines.      */
/*     The colour can either be a presentation parameter that has       */
/*     been set or it can be the default colour as defined within       */
/*     control.                                                         */
/*                                                                      */
/*     Upon Entry:                                                      */
/*                                                                      */
/*     HWND       hWnd; = Window Handle                                 */
/*     PTEXTFIELD ptf;  = Text Field Structure Pointer                  */
/*                                                                      */
/*     Upon Exit:                                                       */
/*                                                                      */
/*     Nothing                                                          */
/*                                                                      */
/* -------------------------------------------------------------------- */

static VOID SetDefaultColours(HWND hWnd, PTEXTFIELD ptf)

{
                       /* Set up the colours that will be used within   */
                       /* the painting of the control.                  */

ptf->lClrText       = lGetPresParam(hWnd, PP_FOREGROUNDCOLOR, PP_FOREGROUNDCOLORINDEX, SYSCLR_OUTPUTTEXT);
ptf->lClrBackground = lGetPresParam(hWnd, PP_BACKGROUNDCOLOR, PP_BACKGROUNDCOLORINDEX, SYSCLR_DIALOGBACKGROUND);
ptf->lClrBorder     = lGetPresParam(hWnd, PP_BORDERCOLOR,     PP_BORDERCOLORINDEX,     SYSCLR_BUTTONDARK);
}
#pragma subtitle("   Shadowed Text DLL - 3D Text Window Procedure")
#pragma page ( )

/* --- hpsDrawText ------------------------------------ [ Private ] --- */
/*                                                                      */
/*     This function is used to draw the text and 3D outline.           */
/*                                                                      */
/*     Upon Entry:                                                      */
/*                                                                      */
/*     HPS        hPS; = Window Presentation Space Handle               */
/*     PTEXTFIELD ptf; = Text Field Pointer                             */
/*                                                                      */
/*     Upon Exit:                                                       */
/*                                                                      */
/*     hpsDrawText = Window Presentation Space Handle                   */
/*                                                                      */
/* -------------------------------------------------------------------- */

static HPS hpsDrawText(HPS hPS, PTEXTFIELD ptf)

{
FONTMETRICS fm;                    /* Font Metrics Holder               */
RECTL       rcl;                   /* Rectangle Holder                  */
register LONG i, k, n;             /* String Length Counter             */

                       /* Get the presentation space for the control    */
                       /* and set the colour table to RGB mode          */

GpiCreateLogColorTable(hPS, 0L, LCOLF_RGB, 0L, 0L, (PLONG)NULL);

                       /* Check to see if any text present and if the   */
                       /* case, draw it within the rectangle            */
if ( ptf->cText )
                       /* Check to see if the text is to be broken over */
                       /* more than one line if the length of the text  */
                       /* is greater than the width of the control      */

   if ( ptf->flStyle & DT_WORDBREAK )
       {
                       /* Word break style specified, set the drawing   */
                       /* of the text within a loop such that it can    */
                       /* be drawn on successive lines                  */
       n = ptf->cText;
       GpiQueryFontMetrics(hPS, sizeof(FONTMETRICS), &fm);
       rcl = ptf->rcl;
       i = 0;
       do
           {
           n -= (k = WinDrawText(hPS, n, &ptf->pszText[i], &rcl, ptf->lClrText, ptf->lClrBackground, ptf->flFormat));
           i += k;
           if ( (rcl.yTop -= fm.lMaxBaselineExt) < rcl.yBottom )
               break;
           } while ( n > 0 );
       }
   else
       {
       GpiSetColor(hPS, ptf->lClrText);
       GpiSetBackColor(hPS, ptf->lClrBackground);
       GpiCharStringPosAt(hPS, &ptf->ptlDraw, &ptf->rcl, CHS_OPAQUE | CHS_CLIP, ptf->cText, ptf->pszText, NULL);
       }
else
   WinFillRect(hPS, &ptf->rcl, ptf->lClrBackground);

GpiSetColor(hPS, ptf->lClrLeftTop);

GpiMove(hPS, &ptf->aptl[3]);
GpiPolyLine(hPS, 2L, ptf->aptl);

GpiSetColor(hPS, ptf->lClrBottomRight);
GpiPolyLine(hPS, 2L, &ptf->aptl[2]);

return(hPS);
}
#pragma subtitle("   Shadowed Text DLL - 3D Text Window Procedure")
#pragma page ( )

/* --- Text3DWndProc ---------------------------------- [ Private ] --- */
/*                                                                      */
/*     This function is used to handle the messages sent to the         */
/*     installed control.  The window procedure is designed to          */
/*     allow for multiple instances and to be totally re-entrant.       */
/*                                                                      */
/*     Upon Entry:                                                      */
/*                                                                      */
/*     HWND   hWnd; = Window Handle                                     */
/*     ULONG  msg;  = PM Message                                        */
/*     MPARAM mp1;  = Message Parameter 1                               */
/*     MPARAM mp2;  = Message Parameter 2                               */
/*                                                                      */
/*     Upon Exit:                                                       */
/*                                                                      */
/*     Text3DWndProc = Message Handling Result                          */
/*                                                                      */
/* -------------------------------------------------------------------- */

MRESULT EXPENTRY Text3DWndProc(HWND hWnd, ULONG msg, MPARAM mp1, MPARAM mp2)

{
LONG           lClr;               /* Presentation Parameter Colour     */
PCREATESTRUCT  pcrst;              /* Create Structure Pointer          */
PSWP           pswp;               /* Size/Window Position Pointer      */
PTEXTFIELD     ptf;                /* Text Field Structure Pointer      */
PWNDPARAMS     pwprm;              /* Window Parameters Pointer         */
RECTL          rcl;                /* Rectangle Holder                  */
ULONG          flStyle;            /* Control Style                     */
ULONG          ulID;               /* Presentation Parameter ID         */

switch ( msg )
   {
                       /* Perform control initialization when the       */
                       /* control is created                            */
   case WM_CREATE :
                       /* Save the address of the text string pointer   */
                       /* in the control's reserved memory to allow it  */
                       /* to be referenced as required by the control   */

       WinSetWindowPtr(hWnd, QWW_CDATA, (PVOID)(ptf = (PTEXTFIELD)malloc(sizeof(TEXTFIELD))));
       memset(ptf, 0, sizeof(TEXTFIELD));

                       /* Get the control's creation structure address  */
                       /* to copy the default text of the control to    */
                       /* the memory in the heap                        */

       pcrst = (PCREATESTRUCT)PVOIDFROMMP(mp2);

       if ( pcrst->pszText )
           {
           ptf->pszText = (PSZ)malloc((ULONG)(ptf->cText = (LONG)strlen(pcrst->pszText)) + 1UL);
           memcpy(ptf->pszText, pcrst->pszText, (UINT)ptf->cText);
           }
                       /* Set up the colours that will be used within   */
                       /* the painting of the control.                  */

       SetDefaultColours(hWnd, ptf);

                       /* Check to see if the user provided font that   */
                       /* should override the default font that would   */
                       /* be set                                        */

       if ( !WinQueryPresParam(hWnd, PP_FONTNAMESIZE, 0L, &ulID, 4L, (PVOID)&lClr, QPF_NOINHERIT) )

                       /* System indicates not set since not data was   */
                       /* returned, therefore set default font for the  */
                       /* control                                       */

           WinSetPresParam(hWnd, PP_FONTNAMESIZE, 8L, (PVOID)"10.Helv");

       if ( (ptf->flStyle = pcrst->flStyle) & DS_RAISED )
           {
           ptf->fRaised = TRUE;
           ptf->lClrLeftTop     = RGB_WHITE;
           ptf->lClrBottomRight = ptf->lClrBorder;
           }
       else
           {
           ptf->fRaised = FALSE;
           ptf->lClrLeftTop     = ptf->lClrBorder;
           ptf->lClrBottomRight = RGB_WHITE;
           }

       if ( ((rcl.xRight = pcrst->cx) > 0L) && ((rcl.yTop = pcrst->cy) > 0L) )
           {
           rcl.xLeft = rcl.yBottom = 0L;
           CalcSize(hWnd, &rcl, ptf);
           }
       break;

   case WM_ADJUSTWINDOWPOS :
       pswp = (PSWP)PVOIDFROMMP(mp1);
       if ( (pswp->fl & SWP_SIZE) && (pswp->cx > 0) && (pswp->cy > 0) )

                       /* Get the address of the control info from the  */
                       /* control's reserved memory and save the new    */
                       /* size of the control                           */
           {
           rcl.xLeft = rcl.yBottom = 0L;
           rcl.xRight = pswp->cx;
           rcl.yTop   = pswp->cy;
           CalcSize(hWnd, &rcl, (PTEXTFIELD)WinQueryWindowPtr(hWnd, QWW_CDATA));
           }
       break;

   /*********************************************************************/
   /*  Convert position query                                           */
   /*********************************************************************/

   case WM_QUERYCONVERTPOS :
       return(MRFROMLONG(QCP_NOCONVERT));

                       /* Control size changing, update the rectangle   */
                       /* points for the 3D text control                */
   case WM_SIZE :
       if ( ((LONG)(SHORT)SHORT1FROMMP(mp2) > 0L) && ((LONG)(SHORT)SHORT2FROMMP(mp2) > 0L) )
           {
           rcl.xLeft = rcl.yBottom = 0L;
           rcl.xRight = (LONG)(SHORT)SHORT1FROMMP(mp2);
           rcl.yTop   = (LONG)(SHORT)SHORT2FROMMP(mp2);
           CalcSize(hWnd, &rcl, (PTEXTFIELD)WinQueryWindowPtr(hWnd, QWW_CDATA));
           }
       else
           {
           ptf = (PTEXTFIELD)WinQueryWindowPtr(hWnd, QWW_CDATA);
           ptf->fDraw = FALSE;
           }
       break;
                       /* Process window parameters setting             */

   case WM_SETWINDOWPARAMS :

                       /* Get the address for the windows parameters    */
                       /* structure                                     */

       pwprm = (PWNDPARAMS)PVOIDFROMMP(mp1);

                       /* Check to see if the text for the control is   */
                       /* being set                                     */

       if ( pwprm->fsStatus & WPM_TEXT )
           {
                       /* Text being set, get the address of the text   */
                       /* string stored in the heap                     */

           ptf = (PTEXTFIELD)WinQueryWindowPtr(hWnd, QWW_CDATA);

                       /* Check to see if any text is being set         */

           if ( pwprm->cchText )
               {
               if ( ptf->cText )
                   ptf->pszText = (PSZ)realloc(ptf->pszText, (ULONG)(ptf->cText = (LONG)pwprm->cchText) + 1UL);
               else
                   ptf->pszText = (PSZ)malloc((ULONG)(ptf->cText = (LONG)pwprm->cchText) + 1UL);

                       /* Check to make sure that the text that is to   */
                       /* be set is not greater than the memory         */
                       /* allocated                                     */

               memcpy(ptf->pszText, pwprm->pszText, (UINT)pwprm->cchText);
               ptf->pszText[ptf->cText] = (CHAR)0;
               }
           else
               {
               if ( ptf->cText )
                   free(ptf->pszText);

                       /* No text is being set, clear any existing text */

               ptf->cText = 0L;
               }

           if ( ptf->fDraw )
               {
               if ( !(ptf->flStyle & DT_WORDBREAK) && ptf->cText )
                  SizeText(hWnd, ptf);
               WinReleasePS(hpsDrawText(WinGetPS(hWnd), ptf));
               }
           }
       return(MRFROMLONG(TRUE));

                       /* Process window parameters query               */

   case WM_QUERYWINDOWPARAMS :

                       /* Get the address for the windows parameters    */
                       /* structure                                     */

       pwprm = (PWNDPARAMS)PVOIDFROMMP(mp1);

       if ( (pwprm->fsStatus & (WPM_TEXT | WPM_CCHTEXT)) == (WPM_TEXT | WPM_CCHTEXT) )
           {
                       /* Text being asked for, get the address of the  */
                       /* text string stored in the heap                */

           ptf = (PTEXTFIELD)WinQueryWindowPtr(hWnd, QWW_CDATA);

                       /* Copy the text from the string to the          */
                       /* structure                                     */

           memcpy(pwprm->pszText, ptf->pszText, (UINT)(ptf->cText + 1));
           pwprm->cchText = (ULONG)ptf->cText;
           }
       else
           {
           ptf = (PTEXTFIELD)WinQueryWindowPtr(hWnd, QWW_CDATA);
           if ( pwprm->fsStatus & WPM_TEXT )

                       /* Copy the text from the string to the          */
                       /* structure                                     */

               memcpy(pwprm->pszText, ptf->pszText, (UINT)(ptf->cText + 1));

           if ( pwprm->fsStatus & WPM_CCHTEXT )
               pwprm->cchText = (ULONG)ptf->cText;
           }
                       /* Query type:  get control data length          */

       if ( pwprm->fsStatus & WPM_CBCTLDATA )

                       /* Set the control data length to zero           */

           pwprm->cbCtlData = 0;
       return(MRFROMLONG(TRUE));

                       /* Presentation parameters changed, record the   */
                       /* change internally                             */

   case WM_PRESPARAMCHANGED :

                       /* Get the address of the control info from the  */
                       /* control's reserved memory                     */

       ptf = (PTEXTFIELD)WinQueryWindowPtr(hWnd, QWW_CDATA);

                       /* Get the new presentation parameter colour for */
                       /* the presentation parameter that has changed.  */
                       /* Get the colour as a RGB value so as to be     */
                       /* able to get an exact value and not an         */
                       /* approximation which could happen if the       */
                       /* presentation parameter was set as a RGB but   */
                       /* queried as an index.  When WinQueryPresParam  */
                       /* returns a 0, it indicates that no             */
                       /* presentation parameter set and the default    */
                       /* colours should be used.                       */

       switch ( LONGFROMMP(mp1) )
           {
           case 0 :
               SetDefaultColours(hWnd, ptf);
               break;

           case PP_FOREGROUNDCOLOR :
           case PP_FOREGROUNDCOLORINDEX :
               ptf->lClrText = lGetPresParam(hWnd, PP_FOREGROUNDCOLOR, PP_FOREGROUNDCOLORINDEX, SYSCLR_OUTPUTTEXT);
               break;

           case PP_BACKGROUNDCOLOR :
           case PP_BACKGROUNDCOLORINDEX :
               ptf->lClrBackground = lGetPresParam(hWnd, PP_BACKGROUNDCOLOR, PP_BACKGROUNDCOLORINDEX, SYSCLR_DIALOGBACKGROUND);
               break;

           case PP_BORDERCOLOR :
           case PP_BORDERCOLORINDEX :
               ptf->lClrBorder = lGetPresParam(hWnd, PP_BORDERCOLOR, PP_BORDERCOLORINDEX, SYSCLR_BUTTONDARK);
               if ( ptf->fRaised )
                   ptf->lClrBottomRight = ptf->lClrBorder;
               else
                   ptf->lClrLeftTop     = ptf->lClrBorder;
               break;

           default :
               return(0L);
           }
                   /* Invalidate the button to force to use the     */
                   /* new colours just set or removed               */

       if ( ptf->fDraw )
           WinReleasePS(hpsDrawText(WinGetPS(hWnd), ptf));
       break;
                       /* Mouse being passed over the control, imply    */
                       /* that the control is transparent to the        */
                       /* system                                        */
   case WM_HITTEST :
       return(MRFROMLONG(HT_TRANSPARENT));

                       /* Erase control background                      */

   case WM_ERASEBACKGROUND :
       return(MRFROMLONG(TRUE));

                       /* Paint the Control                             */
   case WM_PAINT :
                       /* Get the address of the text from the          */
                       /* control's reserved memory                     */

       ptf = (PTEXTFIELD)WinQueryWindowPtr(hWnd, QWW_CDATA);

                       /* Before painting, check to see if the style of */
                       /* the control has changed.  When it has changed */
                       /* make sure that the control internal values    */
                       /* are updated to reflect any changes made.      */

       if ( ((flStyle = WinQueryWindowULong(hWnd, QWL_STYLE)) & (DT_MASK | SS_TEXT)) != (ptf->flStyle & (DT_MASK | SS_TEXT)) )
           {
           if ( (ptf->flStyle = flStyle) & DS_RAISED )
               {
               ptf->fRaised = TRUE;
               ptf->lClrLeftTop     = RGB_WHITE;
               ptf->lClrBottomRight = ptf->lClrBorder;
               }
           else
               {
               ptf->fRaised = FALSE;
               ptf->lClrLeftTop     = ptf->lClrBorder;
               ptf->lClrBottomRight = RGB_WHITE;
               }

           WinQueryWindowRect(hWnd, &rcl);
           CalcSize(hWnd, &rcl, ptf);
           }
                       /* Get the presentation space for the control    */
                       /* and set the colour table to RGB mode          */
       if ( ptf->fDraw )
           WinEndPaint(hpsDrawText(WinBeginPaint(hWnd, (HPS)NULL, (PRECTL)NULL), ptf));
       break;
                       /* Control being destroyed                       */
   case WM_DESTROY :
                       /* Release the heap allocated for use by the     */
                       /* control                                       */

       ptf = (PTEXTFIELD)WinQueryWindowPtr(hWnd, QWW_CDATA);
       if ( ptf->cText )
           free(ptf->pszText);
       free(ptf);
       break;
                       /* Default message processing                    */
   default :
       return(WinDefWindowProc(hWnd, msg, mp1, mp2));
   }

return(0L);
}
#pragma subtitle("   3D Text Control DLL - Control Styles Dialogue Procedure")
#pragma page ( )

/* --- Text3DStyles ------------------------------------ [ Public ] --- */
/*                                                                      */
/*     This function is used for the custom control's styles dialogue   */
/*     box procedure.                                                   */
/*                                                                      */
/*     When the dialogue is invoked from Resource Editor, the           */
/*     address of the user style information is contained in message    */
/*     parameter 2.  The dialogue is responsible for saving the         */
/*     address.  The best method to do this is to save the pointer      */
/*     in the dialogue's reserved memory where it can be retrieved as   */
/*     needed.                                                          */
/*                                                                      */
/*     Upon Entry:                                                      */
/*                                                                      */
/*     HWND   hWnd; = Dialog Window Handle                              */
/*     ULONG  msg;  = PM Message                                        */
/*     MPARAM mp1;  = Message Parameter 1                               */
/*     MPARAM mp2;  = Message Parameter 2                               */
/*                                                                      */
/*     Upon Exit:                                                       */
/*                                                                      */
/*     Text3DStyles = Message Handling Result                           */
/*                                                                      */
/* -------------------------------------------------------------------- */

MRESULT EXPENTRY Text3DStyles(HWND hWnd, ULONG msg, MPARAM mp1, MPARAM mp2)

{
PUSERSTYLE   pust;                 /* User Style Pointer                */
SWP          swp;                  /* Screen Window Position Holder     */

switch ( msg )
   {
                       /* Perform dialogue initialization               */
   case WM_INITDLG :
                       /* Save the pointer to user style information    */
                       /* within the dialog's reserved memory           */

       WinSetWindowPtr(hWnd, QWL_USER, (PVOID)mp2);

                       /* Get the pointer to the user style information */

       if ( (pust = (PUSERSTYLE)mp2) != NULL )
           {
                       /* Set the text, ID symbol and value for the     */
                       /* control                                       */

           WinSetDlgItemText(hWnd, EF_TEXT, pust->pszText);
           pust->pfnSetSymbolID(hWnd, IDBX_SYMBOLVALUE, pust);

           if ( pust->flStyle & DS_RAISED )
               WinSendDlgItemMsg(hWnd, RB_RAISED, BM_SETCHECK, MPFROMSHORT(TRUE), 0L);
           else
               WinSendDlgItemMsg(hWnd, RB_DEPRESSED, BM_SETCHECK, MPFROMSHORT(TRUE), 0L);

                       /* Set Auto Scroll check box if selected         */

           if ( pust->flStyle & DT_RIGHT )
               WinSendDlgItemMsg(hWnd, RB_RIGHTALIGNED, BM_SETCHECK, MPFROMSHORT(TRUE), 0L);
           else
               if ( pust->flStyle & DT_CENTER )
                   WinSendDlgItemMsg(hWnd, RB_HORZALIGNED, BM_SETCHECK, MPFROMSHORT(TRUE), 0L);
               else
                   WinSendDlgItemMsg(hWnd, RB_LEFTALIGNED, BM_SETCHECK, MPFROMSHORT(TRUE), 0L);

                       /* Set vertical alignment style radio button     */

           if ( pust->flStyle & DT_BOTTOM )
               WinSendDlgItemMsg(hWnd, RB_BOTALIGNED, BM_SETCHECK, MPFROMSHORT(TRUE), 0L);
           else
               if ( pust->flStyle & DT_VCENTER )
                   WinSendDlgItemMsg(hWnd, RB_VERTCENTERED, BM_SETCHECK, MPFROMSHORT(TRUE), 0L);
               else
                   WinSendDlgItemMsg(hWnd, RB_TOPALIGNED, BM_SETCHECK, MPFROMSHORT(TRUE), 0L);

                       /* Set Word Break check box if style selected    */

           if ( pust->flStyle & DT_WORDBREAK )
               WinSendDlgItemMsg(hWnd, CB_WORDWRAP, BM_SETCHECK, MPFROMSHORT(TRUE), 0L);

                       /* Set Half Tone check box if style selected     */

           if ( pust->flStyle & DT_HALFTONE )
               WinSendDlgItemMsg(hWnd, CB_HALFTONE, BM_SETCHECK, MPFROMSHORT(TRUE), 0L);

                       /* Set Mnemonic check box if style selected      */

           if ( pust->flStyle & DT_MNEMONIC )
               WinSendDlgItemMsg(hWnd, CB_MNEMONIC, BM_SETCHECK, MPFROMSHORT(TRUE), 0L);

           if ( pust->flStyle & WS_VISIBLE )
               WinSendDlgItemMsg(hWnd, CB_VISIBLE, BM_SETCHECK, MPFROMSHORT(TRUE), 0L);

           if ( pust->flStyle & WS_GROUP )
               WinSendDlgItemMsg(hWnd, CB_GROUP, BM_SETCHECK, MPFROMSHORT(TRUE), 0L);

           if ( pust->flStyle & WS_DISABLED )
               WinSendDlgItemMsg(hWnd, CB_DISABLED, BM_SETCHECK, MPFROMSHORT(TRUE), 0L);

           if ( pust->flStyle & WS_TABSTOP )
               WinSendDlgItemMsg(hWnd, CB_TABSTOP, BM_SETCHECK, MPFROMSHORT(TRUE), 0L);
           }
                       /* Centre dialog on the screen                   */

       WinQueryWindowPos(hWnd, (PSWP)&swp);
       WinSetWindowPos(hWnd, HWND_TOP,
                       (WinQuerySysValue(HWND_DESKTOP, SV_CXSCREEN) - swp.cx) / 2L,
                       (WinQuerySysValue(HWND_DESKTOP, SV_CYSCREEN) - swp.cy) / 2L,
                       0L, 0L, SWP_MOVE);
       break;
                       /* Process push button selections                */
   case WM_COMMAND :
       switch ( SHORT1FROMMP(mp1) )
           {
                       /* Presentation push button selected             */

           case DID_FONTCLR :

                       /* Get the pointer to the user style information */

               if ( (pust = PDATAFROMDLG(hWnd)) != NULL )

                       /* Get the address of the look up function from  */
                       /* user style information structure and display  */
                       /* the dialog.  The value selected within the    */
                       /* dialog will be automatically placed within    */
                       /* the required entry fields                     */

                   pust->pfnGetFontClr(hWnd);
               break;
                       /* Enter pushbutton selected get the definitions */
                       /* for the control                               */
           case DID_OK :

                       /* Get the pointer to the user style information */

               if ( (pust = PDATAFROMDLG(hWnd)) != NULL )
                   {
                       /* Get the address of the CUA compliance         */
                       /* function from the user style information      */
                       /* structure.  The function will check the text  */
                       /* for CUA compliance according to index value   */
                       /* selected.  A return value of TRUE from the    */
                       /* compliance function indicates that the text   */
                       /* entered is acceptable.  Conversely, a FALSE   */
                       /* return value indicates that text is non-      */
                       /* compliant.  In this case, the dialog should   */
                       /* not be exited from and the values within the  */
                       /* the entry fields should not be saved.         */

                   if ( !pust->pfnCUACheck(hWnd, EF_TEXT, CUACHK_CAPS) )
                       break;

                       /* Get the address of the symbol validation      */
                       /* function from the user style information      */
                       /* structure.  The function will validate the    */
                       /* symbol and will check for duplications of     */
                       /* values.  A return value of TRUE from the      */
                       /* validation function indicates that the symbol */
                       /* and value are acceptable.  Conversely, a      */
                       /* FALSE return value indicates that symbol or   */
                       /* value was not acceptable.  In this case,      */
                       /* the dialog should not be exited from and the  */
                       /* values within the entry fields should not be  */
                       /* saved.                                        */

                   if ( !pust->pfnGetSymbolID(hWnd, IDBX_SYMBOLVALUE, pust) )
                       break;
                   else
                       {
                       /* Symbol and value validated, get the text of   */
                       /* the control and save within the user style    */
                       /* information structure for use by Resource     */
                       /* Editor                                        */

                       pust->cText = WinQueryDlgItemText(hWnd, EF_TEXT, CCHTEXTMAX, pust->pszText);

                       /* Mask out current edit field styles clearing   */
                       /* selectable styles and save new style          */

                       pust->flStyle = SS_TEXT;

                       /* Get raised/depressed style                    */

                       if ( SHORT1FROMMR(WinSendDlgItemMsg(hWnd, RB_RAISED, BM_QUERYCHECK, 0L, 0L)) )
                           pust->flStyle |= DS_RAISED;
                       else
                           pust->flStyle |= DS_DEPRESSED;

                       /* Get horizontal alignment type                 */

                       switch ( (ULONG)SHORT1FROMMR(WinSendDlgItemMsg(hWnd, RB_LEFTALIGNED, BM_QUERYCHECKINDEX,
                                                                      0L, 0L)) + RB_LEFTALIGNED )
                           {
                           case RB_LEFTALIGNED :
                               pust->flStyle |= DT_LEFT;
                               break;

                           case RB_HORZALIGNED :
                               pust->flStyle |= DT_CENTER;
                               break;

                           case RB_RIGHTALIGNED :
                               pust->flStyle |= DT_RIGHT;
                               break;
                           }

                       /* Get vertical alignment style                  */

                       switch ( (ULONG)SHORT1FROMMR(WinSendDlgItemMsg(hWnd, RB_TOPALIGNED,
                                                                      BM_QUERYCHECKINDEX,
                                                                      0L, 0L)) + RB_TOPALIGNED )
                           {
                           case RB_TOPALIGNED :
                               pust->flStyle |= DT_TOP;
                               break;

                           case RB_VERTCENTERED :
                               pust->flStyle |= DT_VCENTER;
                               break;

                           case RB_BOTALIGNED :
                               pust->flStyle |= DT_BOTTOM;
                               break;
                           }

                       /* Set Half Tone style if selected               */

                       if ( WinSendDlgItemMsg(hWnd, CB_HALFTONE, BM_QUERYCHECK, 0L, 0L) )
                           pust->flStyle |= DT_HALFTONE;

                       /* Set Word Break style if selected              */

                       if ( WinSendDlgItemMsg(hWnd, CB_WORDWRAP, BM_QUERYCHECK, 0L, 0L) )
                           pust->flStyle |= DT_WORDBREAK;

                       /* Set Mnemonic style if selected                */

                       if ( WinSendDlgItemMsg(hWnd, CB_MNEMONIC, BM_QUERYCHECK, 0L, 0L) )
                           pust->flStyle |= DT_MNEMONIC;

                       /* Save completed edit field style in internal   */
                       /* window information                            */

                       if ( WinSendDlgItemMsg(hWnd, CB_VISIBLE, BM_QUERYCHECK, 0L, 0L) )
                           pust->flStyle |= WS_VISIBLE;

                       if ( WinSendDlgItemMsg(hWnd, CB_GROUP, BM_QUERYCHECK, 0L, 0L) )
                           pust->flStyle |= WS_GROUP;

                       if ( WinSendDlgItemMsg(hWnd, CB_DISABLED, BM_QUERYCHECK, 0L, 0L) )
                           pust->flStyle |= WS_DISABLED;

                       if ( WinSendDlgItemMsg(hWnd, CB_TABSTOP, BM_QUERYCHECK, 0L, 0L) )
                           pust->flStyle |= WS_TABSTOP;
                       }
                   }
                       /* Exit the dialogue indicating changes made     */

               WinDismissDlg(hWnd, TRUE);
               break;
                       /* Cancel selected, exit the dialogue without    */
                       /* changing anything                             */

           case DID_CANCEL :
               WinDismissDlg(hWnd, FALSE);
               break;
           }
       break;
                       /* Close received, exit dialog                   */
   case WM_CLOSE :
       WinDismissDlg(hWnd, FALSE);
       break;
                       /* Pass through unhandled messages               */
   default :
       return(WinDefDlgProc(hWnd, msg, mp1, mp2));
   }
return(0L);

}
