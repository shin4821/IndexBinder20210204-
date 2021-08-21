//FindingForm.c
#include"FindingForm.h"
#include"resource.h"
#include"BusinessCardBinder.h"
#include"IndexBinder.h"
#include"Index.h"
#include<stdio.h>
#include<stdlib.h>
#include<Commctrl.h> // TVM의 헤더파일 추가
#pragma warning(disable:4996)

BOOL CALLBACK FindingFormProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	BOOL ret;
	switch (message) {
	case WM_COMMAND:
		ret = FindingForm_OnCommand(hWnd, wParam, lParam);
		break;

	case WM_CLOSE:
		ret = FindingForm_OnClose(hWnd, wParam, lParam);
		break;

	default:
		ret = FALSE;
		break;
	}
	return ret;
}

BOOL FindingForm_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BOOL ret;
	switch (LOWORD(wParam)) {
	case IDC_BUTTON_FIND:
		ret = FindingForm_OnFindButtonClicked(hWnd, wParam, lParam);
		break;

	case IDC_BUTTON_SELECT:
		ret = FindingForm_OnSelectButtonClicked(hWnd, wParam, lParam);
		break;

	case IDC_BUTTON_FIRST:
		ret = FindingForm_OnFirstButtonClicked(hWnd, wParam, lParam);
		break;

	case IDC_BUTTON_PREVIOUS:
		ret = FindingForm_OnPreviousButtonClicked(hWnd, wParam, lParam);
		break;

	case IDC_BUTTON_NEXT:
		ret = FindingForm_OnNextButtonClicked(hWnd, wParam, lParam);
		break;

	case IDC_BUTTON_LAST:
		ret = FindingForm_OnLastButtonClicked(hWnd, wParam, lParam);
		break;

	default:
		ret = FALSE;
		break;
	}
	return ret;
}

BOOL FindingForm_OnFindButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BusinessCardBinder* businessCardBinder;
	TCHAR name[11];
	Long count;
	Long current;
	BusinessCard* (*indexes);
	HWND businessCardBinderForm;

	//1. 찾기 버튼을 클릭했을 때
	if (HIWORD(wParam) == BN_CLICKED) {
		//1.1 성명을 읽는다.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_NAME), WM_GETTEXT, (WPARAM)11, (LPARAM)name);
		//1.2 명함철 윈도우를 찾는다.
		businessCardBinderForm = FindWindow("#32770", "명함철");
		//1.3 명함철 윈도우의 명함철에서 찾는다.
		businessCardBinder = (BusinessCardBinder*)GetWindowLong(businessCardBinderForm, GWL_USERDATA);
		indexes = (BusinessCard * (*))GetWindowLong(hWnd, GWL_USERDATA);
		if (indexes != NULL) {
			free(indexes);
			indexes = NULL;
		}
		BusinessCardBinder_Find(businessCardBinder, name, &indexes, &count);
		SetWindowLong(hWnd, GWL_USERDATA, (LONG)indexes);
		SetProp(hWnd, "count", (HANDLE)count);
		current = 0;
		SetProp(hWnd, "current", (HANDLE)current);

		//1.4 명함이 있는경우, 찾은 명함들 중 첫번째 명함을 출력한다.
		if (indexes[current] != NULL) {
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONALNAME), WM_SETTEXT, 0,
				(LPARAM)indexes[current]->personal.name);
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONALRANKNAME), WM_SETTEXT, 0,
				(LPARAM)indexes[current]->personal.rankName);
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONALTELEPHONENUMBER), WM_SETTEXT, 0,
				(LPARAM)indexes[current]->personal.telephoneNumber);
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONALEMAILADDRESS), WM_SETTEXT, 0,
				(LPARAM)indexes[current]->personal.emailAddress);
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYNAME), WM_SETTEXT, 0,
				(LPARAM)indexes[current]->company.name);
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYADDRESS), WM_SETTEXT, 0,
				(LPARAM)indexes[current]->company.address);
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYTELEPHONENUMBER), WM_SETTEXT, 0,
				(LPARAM)indexes[current]->company.telephoneNumber);
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYFAXNUMBER), WM_SETTEXT, 0,
				(LPARAM)indexes[current]->company.faxNumber);
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYURL), WM_SETTEXT, 0,
				(LPARAM)indexes[current]->company.url);
		}
		//1.5 명함이 없는 경우 공백 처리한다.
		else {
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONALNAME), WM_SETTEXT, 0, (LPARAM)"");
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONALRANKNAME), WM_SETTEXT, 0, (LPARAM)"");
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONALTELEPHONENUMBER), WM_SETTEXT, 0, (LPARAM)"");
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONALEMAILADDRESS), WM_SETTEXT, 0, (LPARAM)"");
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYNAME), WM_SETTEXT, 0,(LPARAM)"");
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYADDRESS), WM_SETTEXT, 0,(LPARAM)"");
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYTELEPHONENUMBER), WM_SETTEXT, 0, (LPARAM)"");
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYFAXNUMBER), WM_SETTEXT, 0, (LPARAM)"");
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYURL), WM_SETTEXT, 0, (LPARAM)"");
		}
	}
	return TRUE;
}

BOOL FindingForm_OnSelectButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BusinessCardBinder* businessCardBinder;
	HWND businessCardBinderForm;
	BusinessCard* (*indexes);
	Long current;
	BusinessCard* index;
	IndexBinder* indexBinder;
	Index* index_;
	HTREEITEM parent;
	HTREEITEM company;
	HTREEITEM name;
	TCHAR textCompany[32];
	TVITEM item = { 0, };
	Long i;
	Long j = 0;

	//6. 선택 버튼을 클릭했을 때
	if (HIWORD(wParam) == BN_CLICKED) {
		//6.1 명함의 위치를 읽는다.
		indexes = (BusinessCard * (*))GetWindowLong(hWnd, GWL_USERDATA);
		current = (Long)GetProp(hWnd, "current");
		index = indexes[current];

		//6.2 명함철 윈도우를 찾는다.
		businessCardBinderForm = FindWindow("#32770", "명함철");
		businessCardBinder = (BusinessCardBinder*)GetWindowLong(businessCardBinderForm, GWL_USERDATA);

		//6.3 명함철 윈도우의 명함철에서 읽는 위치를 현재 명함으로 이동한다.
		index = Move(businessCardBinder, index);

		//6.4 트리뷰에서 해당 상호를 찾는다.
		parent = SendMessage(GetDlgItem(businessCardBinderForm, IDC_TREE_INDEXES), TVM_GETNEXTITEM,
			(WPARAM)TVGN_ROOT, (LPARAM)NULL);

		company = SendMessage(GetDlgItem(businessCardBinderForm, IDC_TREE_INDEXES), TVM_GETNEXTITEM,
			(WPARAM)TVGN_CHILD, (LPARAM)parent);

		item.mask = TVIF_HANDLE | TVIF_TEXT | TVIF_STATE;
		item.hItem = company;
		item.pszText = textCompany;
		item.cchTextMax = 32;
		SendMessage(GetDlgItem(businessCardBinderForm, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);

		while (strcmp(textCompany, index->company.name) != 0) {
			company = SendMessage(GetDlgItem(businessCardBinderForm, IDC_TREE_INDEXES), TVM_GETNEXTITEM,
				(WPARAM)TVGN_NEXT, (LPARAM)company);
			item.hItem = company;
			item.pszText = textCompany;
			item.cchTextMax = 32;
			SendMessage(GetDlgItem(businessCardBinderForm, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);
		}

		//6.5 트리뷰에서 해당 개인을 찾는다.
		indexBinder = (IndexBinder*)GetProp(businessCardBinderForm, "indexBinder");
		index_ = IndexBinder_Find(indexBinder, index->company.name);
		i = Index_Find(index_, index);
		name = SendMessage(GetDlgItem(businessCardBinderForm, IDC_TREE_INDEXES), TVM_GETNEXTITEM,
			(WPARAM)TVGN_CHILD, (LPARAM)company);
		while (j < i) {
			name = SendMessage(GetDlgItem(businessCardBinderForm, IDC_TREE_INDEXES), TVM_GETNEXTITEM,
				(WPARAM)TVGN_NEXT, (LPARAM)name);
			j++;
		}
		
		//6.6 회사들 상호를 펼친다.
		SendMessage(GetDlgItem(businessCardBinderForm, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND,
			(LPARAM)parent);
		//6.7 해당 상호를 펼친다.
		SendMessage(GetDlgItem(businessCardBinderForm, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND,
			(LPARAM)company);

		//6.8 해당 개인을 선택된 상태로 처리한다.
		SendMessage(GetDlgItem(businessCardBinderForm, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET,
			(LPARAM)name);

		//6.9 명함철 윈도우의 명함에 현재 명함을 출력한다.
		SendMessage(GetDlgItem(businessCardBinderForm, IDC_EDIT_PERSONALNAME), WM_SETTEXT, 0,
			(LPARAM)index->personal.name);
		SendMessage(GetDlgItem(businessCardBinderForm, IDC_EDIT_PERSONALRANKNAME), WM_SETTEXT, 0,
			(LPARAM)index->personal.rankName);
		SendMessage(GetDlgItem(businessCardBinderForm, IDC_EDIT_PERSONALTELEPHONENUMBER), WM_SETTEXT, 0,
			(LPARAM)index->personal.telephoneNumber);
		SendMessage(GetDlgItem(businessCardBinderForm, IDC_EDIT_PERSONALEMAILADDRESS), WM_SETTEXT, 0,
			(LPARAM)index->personal.emailAddress);
		SendMessage(GetDlgItem(businessCardBinderForm, IDC_EDIT_COMPANYNAME), WM_SETTEXT, 0,
			(LPARAM)index->company.name);
		SendMessage(GetDlgItem(businessCardBinderForm, IDC_EDIT_COMPANYADDRESS), WM_SETTEXT, 0,
			(LPARAM)index->company.address);
		SendMessage(GetDlgItem(businessCardBinderForm, IDC_EDIT_COMPANYTELEPHONENUMBER), WM_SETTEXT, 0,
			(LPARAM)index->company.telephoneNumber);
		SendMessage(GetDlgItem(businessCardBinderForm, IDC_EDIT_COMPANYFAXNUMBER), WM_SETTEXT, 0,
			(LPARAM)index->company.faxNumber);
		SendMessage(GetDlgItem(businessCardBinderForm, IDC_EDIT_COMPANYURL), WM_SETTEXT, 0,
			(LPARAM)index->company.url);

		//6.5 찾기 윈도우를 종료한다.
		if (indexes != NULL) {
			free(indexes);
			indexes = NULL;
		}
		RemoveProp(hWnd, "current");
		EndDialog(hWnd, 0);
	}
	return TRUE;
}

BOOL FindingForm_OnFirstButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BusinessCard* (*indexes);
	Long current;

	//2. 처음 버튼을 클릭했을 때
	if (HIWORD(wParam) == BN_CLICKED) {
		//2.1 찾은 명함들 중, 첫번째 명함을 명함에 출력한다.
		indexes = (BusinessCard * (*))GetWindowLong(hWnd, GWL_USERDATA);
		current = 0;
		SetProp(hWnd, "current", (HANDLE)current);

		SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONALNAME), WM_SETTEXT, 0,
			(LPARAM)indexes[current]->personal.name);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONALRANKNAME), WM_SETTEXT, 0,
			(LPARAM)indexes[current]->personal.rankName);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONALTELEPHONENUMBER), WM_SETTEXT, 0,
			(LPARAM)indexes[current]->personal.telephoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONALEMAILADDRESS), WM_SETTEXT, 0,
			(LPARAM)indexes[current]->personal.emailAddress);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYNAME), WM_SETTEXT, 0,
			(LPARAM)indexes[current]->company.name);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYADDRESS), WM_SETTEXT, 0,
			(LPARAM)indexes[current]->company.address);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYTELEPHONENUMBER), WM_SETTEXT, 0,
			(LPARAM)indexes[current]->company.telephoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYFAXNUMBER), WM_SETTEXT, 0,
			(LPARAM)indexes[current]->company.faxNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYURL), WM_SETTEXT, 0,
			(LPARAM)indexes[current]->company.url);
	}
	return TRUE;
}

BOOL FindingForm_OnPreviousButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BusinessCard* (*indexes);
	Long current;

	//3. 이전 버튼을 클릭했을때
	if (HIWORD(wParam) == BN_CLICKED) {
		//3.1 찾는 명함들 중 이전 명함을 명함에 출력한다.
		indexes = (BusinessCard * (*))GetWindowLong(hWnd, GWL_USERDATA);
		current = (Long)GetProp(hWnd, "current");
		current--;
		if (current < 0) {
			current = 0;
		}
		SetProp(hWnd, "current", (HANDLE)current);

		SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONALNAME), WM_SETTEXT, 0,
			(LPARAM)indexes[current]->personal.name);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONALRANKNAME), WM_SETTEXT, 0,
			(LPARAM)indexes[current]->personal.rankName);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONALTELEPHONENUMBER), WM_SETTEXT, 0,
			(LPARAM)indexes[current]->personal.telephoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONALEMAILADDRESS), WM_SETTEXT, 0,
			(LPARAM)indexes[current]->personal.emailAddress);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYNAME), WM_SETTEXT, 0,
			(LPARAM)indexes[current]->company.name);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYADDRESS), WM_SETTEXT, 0,
			(LPARAM)indexes[current]->company.address);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYTELEPHONENUMBER), WM_SETTEXT, 0,
			(LPARAM)indexes[current]->company.telephoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYFAXNUMBER), WM_SETTEXT, 0,
			(LPARAM)indexes[current]->company.faxNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYURL), WM_SETTEXT, 0,
			(LPARAM)indexes[current]->company.url);
	}
	return TRUE;
}

BOOL FindingForm_OnNextButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BusinessCard* (*indexes);
	Long current;
	Long count;

	//4. 다음 버튼을 클릭했을 때
	if (HIWORD(wParam) == BN_CLICKED) {
		//4.1 찾는 명함들 중 다음 명함을 명함에 출력한다.
		indexes = (BusinessCard * (*))GetWindowLong(hWnd, GWL_USERDATA);
		current = GetProp(hWnd, "current");
		count = GetProp(hWnd, "count");
		current++;
		if (current >= count) {
			current = count - 1;
		}
		SetProp(hWnd, "current", (HANDLE)current);

		SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONALNAME), WM_SETTEXT, 0,
			(LPARAM)indexes[current]->personal.name);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONALRANKNAME), WM_SETTEXT, 0,
			(LPARAM)indexes[current]->personal.rankName);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONALTELEPHONENUMBER), WM_SETTEXT, 0,
			(LPARAM)indexes[current]->personal.telephoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONALEMAILADDRESS), WM_SETTEXT, 0,
			(LPARAM)indexes[current]->personal.emailAddress);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYNAME), WM_SETTEXT, 0,
			(LPARAM)indexes[current]->company.name);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYADDRESS), WM_SETTEXT, 0,
			(LPARAM)indexes[current]->company.address);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYTELEPHONENUMBER), WM_SETTEXT, 0,
			(LPARAM)indexes[current]->company.telephoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYFAXNUMBER), WM_SETTEXT, 0,
			(LPARAM)indexes[current]->company.faxNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYURL), WM_SETTEXT, 0,
			(LPARAM)indexes[current]->company.url);
	}
	return TRUE;
}

BOOL FindingForm_OnLastButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BusinessCard* (*indexes);
	Long current;
	Long count;

	//5. 마지막 버튼을 클릭했을 때
	if (HIWORD(wParam) == BN_CLICKED) {
		//5.1 찾은 명함들 중 마지막 명함을 출력한다.
		indexes = (BusinessCard * (*))GetWindowLong(hWnd, GWL_USERDATA);
		count = GetProp(hWnd, "count");
		current = count - 1;
		SetProp(hWnd, "current", (HANDLE)current);

		SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONALNAME), WM_SETTEXT, 0,
			(LPARAM)indexes[current]->personal.name);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONALRANKNAME), WM_SETTEXT, 0,
			(LPARAM)indexes[current]->personal.rankName);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONALTELEPHONENUMBER), WM_SETTEXT, 0,
			(LPARAM)indexes[current]->personal.telephoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONALEMAILADDRESS), WM_SETTEXT, 0,
			(LPARAM)indexes[current]->personal.emailAddress);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYNAME), WM_SETTEXT, 0,
			(LPARAM)indexes[current]->company.name);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYADDRESS), WM_SETTEXT, 0,
			(LPARAM)indexes[current]->company.address);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYTELEPHONENUMBER), WM_SETTEXT, 0,
			(LPARAM)indexes[current]->company.telephoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYFAXNUMBER), WM_SETTEXT, 0,
			(LPARAM)indexes[current]->company.faxNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYURL), WM_SETTEXT, 0,
			(LPARAM)indexes[current]->company.url);
	}
	return TRUE;
}

BOOL FindingForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BusinessCard* (*indexes);

	//7. 닫기 버튼을 클릭했을 때
	  //7.1 찾기 윈도우를 종료한다.
	indexes = (BusinessCard * (*))GetWindowLong(hWnd, GWL_USERDATA);
	if (indexes != NULL) {
		free(indexes);
		indexes = NULL;
	}
	RemoveProp(hWnd, "current");
	RemoveProp(hWnd, "count");
	EndDialog(hWnd, 0);
	return TRUE;

}


