//PuttingForm.h
#ifndef _PUTTINGFORM_H
#define _PUTTINGFORM_H
#include<windows.h>

BOOL CALLBACK PuttingFormProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL PuttingForm_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL PuttingForm_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL PuttingForm_OnCompanyNameKillFocus(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL PuttingForm_OnPutButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam);
BOOL PuttingForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam);

#endif//_PUTTINGFORM_H