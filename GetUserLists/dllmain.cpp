// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include "resource.h"
#include <Windows.h>
#include "GetUserLists.h"
#include "commctrl.h"
#include <TlHelp32.h>
INT_PTR CALLBACK DialogProc(
	_In_ HWND   hwndDlg,
	_In_ UINT   uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
);
void InitListContrl(HWND List);
VOID ThreadProcess(HMODULE hModule);
HWND gUserListView = NULL;
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProcess, hModule, NULL, 0);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

VOID ThreadProcess(HMODULE hModule)
{
	DialogBox(hModule, MAKEINTRESOURCE(MAIN), NULL, &DialogProc);
}

INT_PTR CALLBACK DialogProc(
	_In_ HWND   hwndDlg,
	_In_ UINT   uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
)
{
	HWND hListView = NULL;
	switch (uMsg)
	{
	case WM_INITDIALOG:
		hListView = GetDlgItem(hwndDlg, USER_LISTS);
		gUserListView = hListView;
		InitListContrl(hListView);
		break;
	case WM_CLOSE:
		EndDialog(hwndDlg,1);
		break;
	case WM_COMMAND:
		if (wParam == HOOK_USER_LISTS) {
			MessageBox(NULL,"开始hook","标题",MB_OK);
			HookWechatQrcode(hwndDlg, gUserListView,0x420F34);
		}
		break;
	default:
		break;
	}
	return FALSE;
}


void InitListContrl(HWND List)
{
	LVCOLUMN pcol = { 0 };
	LPCSTR titleBuffer[] = { "wxid","微信账号","微信昵称" };
	pcol.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	pcol.fmt = LVCFMT_LEFT;
	pcol.cx = 120;
	for (int i = 0; i < 3; i++) {
		//创建list成员信息
		pcol.pszText = (LPSTR)titleBuffer[i];
		ListView_InsertColumn(List, i, &pcol);
	}
}