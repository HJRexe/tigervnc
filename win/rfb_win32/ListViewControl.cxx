// ListViewControl.cxx: implementation of the ListViewControl class.
//
//////////////////////////////////////////////////////////////////////
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "ListViewControl.h"
#include "commctrl.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
using namespace rfb;
using namespace rfb::win32;

ListViewControl::ListViewControl()
{
}

bool ListViewControl::IsSelectedLVItem(DWORD idListView,
                                      HWND hDlg, int numberItem)
{	
  return (ListView_GetItemState(GetDlgItem(hDlg, idListView),
    numberItem, LVIS_SELECTED) == LVIS_SELECTED);
}

void ListViewControl::SelectLVItem(DWORD idListView, HWND hDlg, int numberItem)
{
  ListView_SetItemState(GetDlgItem(hDlg, idListView),
    numberItem, LVIS_SELECTED, LVIS_SELECTED);
}

BOOL ListViewControl::InitLVColumns(DWORD idListView, HWND hDlg, int width, int columns,
                                    char * title[], DWORD mask, DWORD LVStyle, DWORD format)
{
  HWND hList = GetDlgItem(hDlg, idListView);
  (void)ListView_SetExtendedListViewStyle(hList, LVStyle);
  WCHAR szText[256];
  LVCOLUMNW lvc;
  int iCol;

  lvc.mask = mask;

  for (iCol = 0; iCol < columns; iCol++) {
    lvc.iSubItem = iCol;
    lvc.pszText = szText;
    lvc.cx = width;
    lvc.fmt = format;

    // UTF-8 narrow header -> wide string for the Unicode list-view message.
    MultiByteToWideChar(CP_UTF8, 0, title[iCol], -1, szText, 256);
    szText[255] = 0;
    if ((int)SendMessageW(hList, LVM_INSERTCOLUMNW,
                          iCol, (LPARAM)&lvc) == -1)
      return FALSE;
  }
  return TRUE;
}

BOOL ListViewControl::InsertLVItem(DWORD idListView, HWND hDlg, int number,  char * texts[],
                                   int columns)
{
  int i;
  HWND hList = GetDlgItem(hDlg, idListView);
  WCHAR wText[256];
  LVITEMW lvI;
  lvI.mask = LVIF_TEXT| LVIF_STATE;
  lvI.state = 0;
  lvI.stateMask = 0;
  lvI.iItem = number;
  lvI.iSubItem = 0;

  // UTF-8 narrow row text -> wide string for the Unicode list-view message.
  MultiByteToWideChar(CP_UTF8, 0, texts[0], -1, wText, 256);
  wText[255] = 0;
  lvI.pszText = wText;

  if ((int)SendMessageW(hList, LVM_INSERTITEMW,
                        0, (LPARAM)&lvI) == -1)
    return FALSE;

  for (i =1; i < columns; i++) {
    SetLVItemText(
      idListView, hDlg,
      number, i, texts[i]);
  }
  return TRUE;
}

void ListViewControl::SetLVItemText(DWORD idListView, HWND hDlg, int numberItem,
                                    int namberColumn, char * text)
{
  HWND hList = GetDlgItem(hDlg, idListView);
  WCHAR wText[256];
  // UTF-8 narrow sub-item text -> wide string for the Unicode list-view message.
  MultiByteToWideChar(CP_UTF8, 0, text, -1, wText, 256);
  wText[255] = 0;
  LVITEMW lvItem;
  lvItem.mask = LVIF_TEXT;
  lvItem.iItem = numberItem;
  lvItem.iSubItem = namberColumn;
  lvItem.pszText = wText;
  SendMessageW(hList, LVM_SETITEMTEXTW,
               numberItem, (LPARAM)&lvItem);
}

void ListViewControl::GetLVItemText(DWORD idListView, HWND hDlg, int numberItem,
                                    int namberColumn, char * text)
{
	 ListView_GetItemText(GetDlgItem(hDlg, idListView), numberItem,
							namberColumn, text, 256);
}

void ListViewControl::DeleteLVItem(DWORD idListView, HWND hDlg, int number)
{
  (void)ListView_DeleteItem(GetDlgItem(hDlg, idListView), number);
}

void ListViewControl::DeleteAllLVItem(DWORD idListView, HWND hDlg)
{
  (void)ListView_DeleteAllItems(GetDlgItem(hDlg, idListView));
}

ListViewControl::~ListViewControl()
{
}
