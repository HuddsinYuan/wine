/*
 * Treeview control
 *
 * Copyright 1998 Eric Kohl
 *
 * NOTES
 *   This is just a dummy control. An author is needed! Any volunteers?
 *   I will only improve this control once in a while.
 *     Eric <ekohl@abo.rhein-zeitung.de>
 *
 * TODO:
 *   - All messages.
 *   - All notifications.
 */

#include "windows.h"
#include "commctrl.h"
#include "treeview.h"
#include "heap.h"
#include "win.h"
#include "debug.h"


#define TREEVIEW_GetInfoPtr(wndPtr) ((TREEVIEW_INFO *)wndPtr->wExtra[0])



static LRESULT
TREEVIEW_SetImageList (WND *wndPtr, WPARAM32 wParam, LPARAM lParam)
{
    TREEVIEW_INFO *infoPtr = TREEVIEW_GetInfoPtr(wndPtr);
    HIMAGELIST himlTemp;

    if ((INT32)wParam == TVSIL_NORMAL) {
	himlTemp = infoPtr->himlNormal;
	infoPtr->himlNormal = (HIMAGELIST)lParam;
    }
    else if ((INT32)wParam == TVSIL_STATE) {
	himlTemp = infoPtr->himlState;
	infoPtr->himlState = (HIMAGELIST)lParam;
    }
    else
	return NULL;

    return (LRESULT)himlTemp;
}



static LRESULT
TREEVIEW_Create (WND *wndPtr, WPARAM32 wParam, LPARAM lParam)
{
    TREEVIEW_INFO *infoPtr;

    /* allocate memory for info structure */
    infoPtr = (TREEVIEW_INFO *)HeapAlloc (GetProcessHeap (), HEAP_ZERO_MEMORY,
                                   sizeof(TREEVIEW_INFO));
    wndPtr->wExtra[0] = (DWORD)infoPtr;

    if (infoPtr == NULL) {
	ERR (treeview, "could not allocate info memory!\n");
	return 0;
    }

    if ((TREEVIEW_INFO*)wndPtr->wExtra[0] != infoPtr) {
	ERR (treeview, "pointer assignment error!\n");
	return 0;
    }

    /* set default settings */
    infoPtr->clrBk = GetSysColor32 (COLOR_WINDOW);
    infoPtr->clrText = GetSysColor32 (COLOR_BTNTEXT);
    infoPtr->himlNormal = NULL;
    infoPtr->himlState = NULL;


    return 0;
}


static LRESULT
TREEVIEW_Destroy (WND *wndPtr, WPARAM32 wParam, LPARAM lParam)
{
    TREEVIEW_INFO *infoPtr = TREEVIEW_GetInfoPtr(wndPtr);




    /* free tree view info data */
    HeapFree (GetProcessHeap (), 0, infoPtr);

    return 0;
}


static LRESULT
TREEVIEW_EraseBackground (WND *wndPtr, WPARAM32 wParam, LPARAM lParam)
{
    TREEVIEW_INFO *infoPtr = TREEVIEW_GetInfoPtr(wndPtr);
    HBRUSH32 hBrush = CreateSolidBrush32 (infoPtr->clrBk);
    RECT32 rect;

    GetClientRect32 (wndPtr->hwndSelf, &rect);
    FillRect32 ((HDC32)wParam, &rect, hBrush);
    DeleteObject32 (hBrush);
    return TRUE;
}



LRESULT WINAPI
TreeviewWindowProc (HWND32 hwnd, UINT32 uMsg, WPARAM32 wParam, LPARAM lParam)
{
    WND *wndPtr = WIN_FindWndPtr(hwnd);

    switch (uMsg)
    {


//	case TVM_GETIMAGELIST:
//	    return TREEVIEW_GetImageList (wndPtr, wParam, lParam);



	case TVM_SETIMAGELIST:
	    return TREEVIEW_SetImageList (wndPtr, wParam, lParam);



	case WM_CREATE:
	    return TREEVIEW_Create (wndPtr, wParam, lParam);

	case WM_DESTROY:
	    return TREEVIEW_Destroy (wndPtr, wParam, lParam);

//	case EM_ENABLE:

	case WM_ERASEBKGND:
	    return TREEVIEW_EraseBackground (wndPtr, wParam, lParam);

	case WM_GETDLGCODE:
	    return DLGC_WANTARROWS | DLGC_WANTCHARS;


//	case WM_PAINT:
//	    return TREEVIEW_Paint (wndPtr, wParam);

//	case WM_SETFONT:

//	case WM_TIMER:

//	case WM_VSCROLL:

	default:
	    if (uMsg >= WM_USER)
		ERR (treeview, "unknown msg %04x wp=%08x lp=%08lx\n",
		     uMsg, wParam, lParam);
	    return DefWindowProc32A (hwnd, uMsg, wParam, lParam);
    }
    return 0;
}


void
TREEVIEW_Register (void)
{
    WNDCLASS32A wndClass;

    if (GlobalFindAtom32A (WC_TREEVIEW32A)) return;

    ZeroMemory (&wndClass, sizeof(WNDCLASS32A));
    wndClass.style         = CS_GLOBALCLASS | CS_DBLCLKS;
    wndClass.lpfnWndProc   = (WNDPROC32)TreeviewWindowProc;
    wndClass.cbClsExtra    = 0;
    wndClass.cbWndExtra    = sizeof(TREEVIEW_INFO *);
    wndClass.hCursor       = LoadCursor32A (0, IDC_ARROW32A);
    wndClass.hbrBackground = 0;
    wndClass.lpszClassName = WC_TREEVIEW32A;
 
    RegisterClass32A (&wndClass);
}

