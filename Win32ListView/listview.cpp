#define _CRT_SECURE_NO_WARNINGS
#undef UNICODE
#include "framework.h"
#include "resource.h"
#include <commctrl.h>
#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "comctl32.lib")
#include <stdio.h>

//==============Global Vatriabls===================
static HWND hList = NULL;  // List View identifier
LVCOLUMN LvCol; // Make Coluom struct for ListView
LVITEM LvItem;  // ListView Item struct
LV_DISPINFO lvd;
int iSelect = 0;
int index = 0;
int flag = 0;
HWND hEdit;
bool escKey = 0;
char tempstr[100] = "";
TCHAR tchar;
MSG msg;
LPTSTR bgimgpath = NULL;
//===================================================

//======================Handles================================================//
HINSTANCE hInst; // main function handler
#define WIN32_LEAN_AND_MEAN // this will assume smaller exe

LRESULT ProcessCustomDraw(LPARAM lParam)
{
	LPNMLVCUSTOMDRAW lplvcd = (LPNMLVCUSTOMDRAW)lParam;

	switch (lplvcd->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT: //Before the paint cycle begins
		//request notifications for individual listview items
		return CDRF_NOTIFYITEMDRAW;

	case CDDS_ITEMPREPAINT: //Before an item is drawn
	{
		return CDRF_NOTIFYSUBITEMDRAW;
	}
	break;

	case CDDS_SUBITEM | CDDS_ITEMPREPAINT: //Before a subitem is drawn
	{
		switch (lplvcd->iSubItem)
		{
		case 0:
		{
			lplvcd->clrText = RGB(255, 255, 255);
			lplvcd->clrTextBk = RGB(240, 55, 23);
			return CDRF_NEWFONT;
		}
		break;

		case 1:
		{
			lplvcd->clrText = RGB(255, 255, 0);
			lplvcd->clrTextBk = RGB(0, 0, 0);
			return CDRF_NEWFONT;
		}
		break;

		case 2:
		{
			lplvcd->clrText = RGB(20, 26, 158);
			lplvcd->clrTextBk = RGB(200, 200, 10);
			return CDRF_NEWFONT;
		}
		break;

		case 3:
		{
			lplvcd->clrText = RGB(12, 15, 46);
			lplvcd->clrTextBk = RGB(200, 200, 200);
			return CDRF_NEWFONT;
		}
		break;

		case 4:
		{
			lplvcd->clrText = RGB(120, 0, 128);
			lplvcd->clrTextBk = RGB(20, 200, 200);
			return CDRF_NEWFONT;
		}
		break;

		case 5:
		{
			lplvcd->clrText = RGB(255, 255, 255);
			lplvcd->clrTextBk = RGB(0, 0, 150);
			return CDRF_NEWFONT;
		}
		break;

		}

	}
	}
	return CDRF_DODEFAULT;
}

//================================About dialog window=============================//
BOOL CALLBACK DialogProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{

		// This Window Message will close the dialog  //
	   //============================================//

	case WM_CLOSE:
	{
		PostQuitMessage(0);
		EndDialog(hWnd, 0); // kill dialog
	}
	break;

	case WM_NOTIFY:
	{
		switch (LOWORD(wParam))
		{
		case IDC_LIST:
			LPNMLISTVIEW pnm = (LPNMLISTVIEW)lParam;

			if (pnm->hdr.hwndFrom == hList && pnm->hdr.code == NM_CUSTOMDRAW)
			{
				SetWindowLong(hWnd, DWL_MSGRESULT, (LONG)ProcessCustomDraw(lParam));
				return TRUE;
			}

			if (((LPNMHDR)lParam)->code == NM_DBLCLK)
			{
				char Text[255] = { 0 };
				char Temp[255] = { 0 };
				char Temp1[255] = { 0 };
				int iSlected = 0;
				int j = 0;

				iSlected = SendMessage(hList, LVM_GETNEXTITEM, -1, LVNI_FOCUSED);

				if (iSlected == -1)
				{
					MessageBox(hWnd, "No Items in ListView", "Error", MB_OK | MB_ICONINFORMATION);
					break;
				}

				memset(&LvItem, 0, sizeof(LvItem));
				LvItem.mask = LVIF_TEXT;
				LvItem.iSubItem = 0;
				LvItem.pszText = Text;
				LvItem.cchTextMax = 256;
				LvItem.iItem = iSlected;

				SendMessage(hList, LVM_GETITEMTEXT, iSlected, (LPARAM)&LvItem);

				sprintf(Temp1, Text);

				for (j = 1; j <= 5; j++)
				{
					LvItem.iSubItem = j;
					SendMessage(hList, LVM_GETITEMTEXT, iSlected, (LPARAM)&LvItem);
					sprintf(Temp, " %s", Text);
					lstrcat(Temp1, Temp);
				}

				MessageBox(hWnd, Temp1, "test", MB_OK);

			}
			if (((LPNMHDR)lParam)->code == NM_CLICK)
			{
				iSelect = SendMessage(hList, LVM_GETNEXTITEM, -1, LVNI_FOCUSED);

				if (iSelect == -1)
				{
					break;
				}
				index = iSelect;
				flag = 1;
			}

			if (((LPNMHDR)lParam)->code == LVN_BEGINLABELEDIT)
			{
				//Editing=1;
				hEdit = ListView_GetEditControl(hList);
				GetWindowText(hEdit, tempstr, sizeof(tempstr));
			}

			if (((LPNMHDR)lParam)->code == LVN_ENDLABELEDIT)
			{
				int iIndex;
				char text[255] = "";

				tchar = (TCHAR)msg.wParam;
				if (tchar == 0x1b)
					escKey = 1;

				iIndex = SendMessage(hList, LVM_GETNEXTITEM, -1, LVNI_FOCUSED);
				if (iIndex == -1)
					break;

				LvItem.iSubItem = 0;

				if (escKey == 0)
				{
					LvItem.pszText = text;
					GetWindowText(hEdit, text, sizeof(text));
					SendMessage(hList, LVM_SETITEMTEXT, (WPARAM)iIndex, (LPARAM)&LvItem);
				}
				else {
					LvItem.pszText = tempstr;
					SendMessage(hList, LVM_SETITEMTEXT, (WPARAM)iIndex, (LPARAM)&LvItem);
					escKey = 0;
				}
				//Editing=0;
			}
			break;
		}
	}

	case WM_PAINT:
	{
		return 0;
	}
	break;

	// This Window Message is the heart of the dialog  //
	//================================================//
	case WM_INITDIALOG:
	{
		int i;
		char Temp[255];
		LVBKIMAGE plvbki = { 0 };
		InitCommonControls();
		hList = GetDlgItem(hWnd, IDC_LIST); // get the ID of the ListView				 

		memset(&plvbki, 0, sizeof(plvbki));
		plvbki.ulFlags = LVBKIF_SOURCE_URL;
		plvbki.pszImage = bgimgpath;
		plvbki.xOffsetPercent = 40;
		plvbki.yOffsetPercent = 15;

		SendMessage(hList, LVM_SETTEXTBKCOLOR, 0, (LPARAM)CLR_NONE);

		// set either a background image (need *absolute* path)
		// https://docs.microsoft.com/en-us/windows/win32/controls/lvm-setbkimage
		//SendMessage(hList,LVM_SETBKIMAGE,0,(LPARAM)(LPLVBKIMAGE)&plvbki);

		// or set a background color 
		// https://docs.microsoft.com/en-us/windows/win32/controls/lvm-setbkcolor
		// https://docs.microsoft.com/en-us/windows/win32/api/commctrl/nf-commctrl-listview_setbkcolor
		ListView_SetBkColor(hList, 0x330055); // 0xBBGGRR

		SendMessage(hList, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT); // Set style

		SendMessageA(hWnd, WM_SETICON, (WPARAM)1, (LPARAM)LoadIconA(hInst, MAKEINTRESOURCE(IDI_ICON1)));

		// Here we put the info on the Coulom headers
		// this is not data, only name of each header we like
		memset(&LvCol, 0, sizeof(LvCol)); // Reset Coluom
		LvCol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM; // Type of mask
		LvCol.cx = 0x28;                                // width between each coloum
		LvCol.pszText = _strdup("Item");                     // First Header (memleak)
		LvCol.cx = 0x42;

		// Inserting Couloms as much as we want
		SendMessage(hList, LVM_INSERTCOLUMN, 0, (LPARAM)&LvCol); // Insert/Show the coloum
		LvCol.pszText = _strdup("Sub Item1");                          // Next coloum (memleak)
		SendMessage(hList, LVM_INSERTCOLUMN, 1, (LPARAM)&LvCol); // ...
		LvCol.pszText = _strdup("Sub Item2");                       //(memleak)
		SendMessage(hList, LVM_INSERTCOLUMN, 2, (LPARAM)&LvCol); //
		LvCol.pszText = _strdup("Sub Item3");                              //(memleak)
		SendMessage(hList, LVM_INSERTCOLUMN, 3, (LPARAM)&LvCol); //
		LvCol.pszText = _strdup("Sub Item4");                            //(memleak)
		SendMessage(hList, LVM_INSERTCOLUMN, 4, (LPARAM)&LvCol); //
		LvCol.pszText = _strdup("Sub Item5");                      //(memleak)
		SendMessage(hList, LVM_INSERTCOLUMN, 5, (LPARAM)&LvCol); // ...same as above

		memset(&LvItem, 0, sizeof(LvItem)); // Reset Item Struct

		//  Setting properties Of Items:

		LvItem.mask = LVIF_TEXT;   // Text Style
		LvItem.cchTextMax = 256; // Max size of test

		LvItem.iItem = 0;          // choose item  
		LvItem.iSubItem = 0;       // Put in first coluom
		LvItem.pszText = _strdup("Item 0"); // Text to display (can be from a char variable) (Items) memleak

		SendMessage(hList, LVM_INSERTITEM, 0, (LPARAM)&LvItem); // Send to the Listview

		for (i = 1; i <= 5; i++) // Add SubItems in a loop
		{
			LvItem.iSubItem = i;
			sprintf(Temp, "SubItem %d", i);
			LvItem.pszText = Temp;
			SendMessage(hList, LVM_SETITEM, 0, (LPARAM)&LvItem); // Enter text to SubItems
		}

		// lets add a new Item:
		LvItem.iItem = 1;            // choose item  
		LvItem.iSubItem = 0;         // Put in first coluom
		LvItem.pszText = _strdup("Item 1");   // Text to display (can be from a char variable) (Items) memleak
		SendMessage(hList, LVM_INSERTITEM, 0, (LPARAM)&LvItem); // Send to the Listview

		for (i = 1; i <= 5; i++) // Add SubItems in a loop
		{
			LvItem.iSubItem = i;
			sprintf(Temp, "SubItem %d", i);
			LvItem.pszText = Temp;
			SendMessage(hList, LVM_SETITEM, 0, (LPARAM)&LvItem); // Enter etxt to SubItems
		}

		//ListView_SetItemState(hList,0,LVIS_SELECTED	,LVIF_STATE);
		ShowWindow(hWnd, SW_NORMAL);
		UpdateWindow(hWnd);

		while (TRUE)
		{

			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				/*
				if(msg.message==WM_CHAR)
				{
					tchar = (TCHAR)msg.wParam;
					if(tchar == 0x1b)
						escKey=1;

				}
				*/
				if (msg.message == WM_QUIT)// killing while looking for a message
				{
					break;
				}


				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}
	break;

	// This Window Message will control the dialog  //
   //==============================================//
	case WM_COMMAND:
	{
		switch (LOWORD(wParam)) // what we press on?
		{

		case IDC_ADDITEM:
		{
			int iItem;
			char ItemText[100];

			iItem = SendMessage(hList, LVM_GETITEMCOUNT, 0, 0);

			GetDlgItemText(hWnd, IDC_ADD, ItemText, 100);

			if ((lstrlen(ItemText)) == 0)
			{
				MessageBox(hWnd, "Please Write Some Text", "Error", MB_OK | MB_ICONINFORMATION);
				break;
			}

			LvItem.iItem = iItem;            // choose item  
			LvItem.iSubItem = 0;         // Put in first coluom
			LvItem.pszText = ItemText;   // Text to display (can be from a char variable) (Items)
			SendMessage(hList, LVM_INSERTITEM, 0, (LPARAM)&LvItem); // Send to the Listview
		}
		break;

		case IDC_ADDSUBITEM:
		{
			int Item, i;
			char SubItemText[100];

			Item = SendMessage(hList, LVM_GETITEMCOUNT, 0, 0);

			GetDlgItemText(hWnd, IDC_ADDSUB, SubItemText, 100);

			if ((lstrlen(SubItemText)) == 0)
			{
				MessageBox(hWnd, "Please Write Some Text", "Error", MB_OK | MB_ICONINFORMATION);
				break;
			}

			LvItem.iItem = Item - 1;            // choose item  

			for (i = 1; i <= 5; i++)
			{
				LvItem.pszText = SubItemText;   // Text to display (can be from a char variable) (Items)
				LvItem.iSubItem = i;         // Put in first coluom
				SendMessage(hList, LVM_SETITEM, 0, (LPARAM)&LvItem);
			}
		}
		break;

		case IDC_BOTH:
		{
			int itemIndex, j;
			char iSubItemText[100] = "";
			char iItemText[100] = "";

			itemIndex = SendMessage(hList, LVM_GETITEMCOUNT, 0, 0);

			GetDlgItemText(hWnd, IDC_ADD, iItemText, 100);
			GetDlgItemText(hWnd, IDC_ADDSUB, iSubItemText, 100);

			if ((lstrlen(iSubItemText) && lstrlen(iItemText)) == 0)
			{
				MessageBox(hWnd, "Please Write Some Text", "Error", MB_OK | MB_ICONINFORMATION);
				break;
			}

			LvItem.iItem = itemIndex;            // choose item  
			LvItem.iSubItem = 0;
			LvItem.pszText = iItemText;
			SendMessage(hList, LVM_INSERTITEM, 0, (LPARAM)&LvItem);

			for (j = 1; j <= 5; j++)
			{
				LvItem.pszText = iSubItemText;   // Text to display (can be from a char variable) (Items)
				LvItem.iSubItem = j;         // Put in first coluom
				SendMessage(hList, LVM_SETITEM, 0, (LPARAM)&LvItem);
			}
		}
		break;

		case IDC_DELALL:
			SendMessage(hList, LVM_DELETEALLITEMS, 0, 0);
			break;

		case IDC_DELSELITEM:
			if (flag)
				SendMessage(hList, LVM_DELETEITEM, iSelect, 0);
			flag = 0;
			break;

		case IDC_RENAME:
		{
			if (index == -1)
			{
				MessageBox(hWnd, "Nothing to rename", "error", MB_OK);
			}
			else {
				//Editing=1;
				SendMessage(hList, LVM_EDITLABEL, (WPARAM)index, (LPARAM)0);
			}
		}
		break;

		case IDC_SELECT_ALL:
		{
			ListView_SetItemState(hList, -1, 0, LVIS_SELECTED); // deselect all
			SendMessage(hList, LVM_ENSUREVISIBLE, (WPARAM)-1, FALSE); // Send to the Listview                        
			ListView_SetItemState(hList, -1, LVIS_SELECTED, LVIS_SELECTED);
		}
		break;

		case IDC_LAST_ITEM:
		{
			int items;
			items = SendMessage(hList, LVM_GETITEMCOUNT, (WPARAM)0, (LPARAM)0);
			items--;
			ListView_SetItemState(hList, -1, 0, LVIS_SELECTED); // deselect all
			SendMessage(hList, LVM_ENSUREVISIBLE, (WPARAM)items, FALSE); // Send to the Listview
			ListView_SetItemState(hList, items, LVIS_SELECTED, LVIS_SELECTED);
			ListView_SetItemState(hList, items, LVIS_FOCUSED, LVIS_FOCUSED);

		}
		break;

		}
	}
	break;

	default:
	{
		return FALSE;
	}
	}

	return TRUE;
}

//===========================MAIN FUNCTION-WIN32 STARTING POINT========================================//
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	// add this code if win2000/nt/xp doesn't
	// load ur winmain (experimental only)
	// also add comctl32.lib

	OleInitialize(NULL);

	// calculate absolute path to the background picture, we assume that the current working
	// directory is the project's root (which should be the case when hitting F5 from within Visual Studio)
	// (http://www.luigibianchi.com/windows.htm)
	DWORD result = GetCurrentDirectory(0, NULL);
	bgimgpath = (LPTSTR)malloc(result + 6);
	GetCurrentDirectory(result, bgimgpath);
	memcpy(bgimgpath + result - 1, "\\a.jpg\0", 7);
	OutputDebugString(bgimgpath);

	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwICC = ICC_LISTVIEW_CLASSES;
	InitCtrls.dwSize = sizeof(INITCOMMONCONTROLSEX);
	BOOL bRet = InitCommonControlsEx(&InitCtrls);

	hInst = hInstance;

	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDC_DIALOG), NULL, (DLGPROC)DialogProc, 0);
	return 0;
}
//======================================================================================================//