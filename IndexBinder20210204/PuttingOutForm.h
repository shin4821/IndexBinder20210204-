//PuttingOutForm.h
#ifndef _PUTTINGOUTFORM_H
#define _PUTTINGOUTFORM_H
#include<windows.h>

BOOL CALLBACK PuttingOutFormProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL PuttingOutForm_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL PuttingOutForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam);

#endif//_PUTTINGOUTFORM_H
