//BusinessCardBinderForm.h
#ifndef _BUSINESSCARDBINDERFORM_H
#define _BUSINESSCARDBINDERFORM_H
#include<windows.h>

BOOL CALLBACK BusinessCardBinderFormProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL BusinessCardBinderForm_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL BusinessCardBinderForm_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL BusinessCardBinderForm_OnPutButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL BusinessCardBinderForm_OnFindButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL BusinessCardBinderForm_OnPutOutButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL BusinessCardBinderForm_OnArrangeButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL BusinessCardBinderForm_OnFirstButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL BusinessCardBinderForm_OnPreviousButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL BusinessCardBinderForm_OnNextButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL BusinessCardBinderForm_OnLastButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL BusinessCardBinderForm_OnNotify(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL BusinessCardBinderForm_OnTreeViewItemButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL BusinessCardBinderForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam);

#endif//_BUSINESSCARDBINDERFORM_H
