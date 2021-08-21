//PuttingForm.c
#include"PuttingForm.h"
#include"resource.h"
#include"BusinessCardBinder.h"
#include"IndexBinder.h"
#include"Index.h"
#include<stdio.h>
#include<stdlib.h>
#include<Commctrl.h> // TVM의 헤더파일 추가
#pragma warning(disable:4996)

BOOL CALLBACK PuttingFormProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

	BOOL ret;
	switch (message) {
	case WM_INITDIALOG:
		ret = PuttingForm_OnInitDialog(hWnd, wParam, lParam);
		break;

	case WM_COMMAND:
		ret = PuttingForm_OnCommand(hWnd, wParam, lParam);
		break;

	case WM_CLOSE:
		ret = PuttingForm_OnClose(hWnd, wParam, lParam);
		break;

	default:
		ret = FALSE;
		break;
	}
	return ret;

}

BOOL PuttingForm_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BOOL ret;
	switch (LOWORD(wParam)) {
	case IDC_EDIT_COMPANYNAME:
		ret = PuttingForm_OnCompanyNameKillFocus(hWnd, wParam, lParam);
		break;

	case IDC_BUTTON_PUT:
		ret = PuttingForm_OnPutButtonClicked(hWnd, wParam, lParam);
		break;

	default:
		ret = FALSE;
		break;
	}
	return ret;
}

BOOL PuttingForm_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	int i;
	TCHAR personalRankName[][16] = { "인턴", "사원", "주임", "대리", "과장", "차장", "부장", "이사", "상무",
	"전무", "부사장", "사장" };
	TCHAR personalPortalSites[][32] = { "naver.com", "gmail.net","daum.net" };

	//1. 꽂기 윈도우가 생성될 때
	  //1.1 직급 칸의 콤보박스에 직급들을 추가한다.
	for (i = 0; i<sizeof(personalRankName) / sizeof(personalRankName[0]); i++) {
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONALRANKNAME), CB_ADDSTRING, 0,
			(LPARAM)personalRankName[i]);
	  }
	//1.2 이메일 주소칸의 콤보박스에 이메일 주소들을 추가한다.
	for (i = 0; i<sizeof(personalPortalSites) / sizeof(personalPortalSites[0]); i++) {
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONALPORTALSITE), CB_ADDSTRING, 0,
			(LPARAM)personalPortalSites[i]);
	}
	return TRUE;
}


BOOL PuttingForm_OnCompanyNameKillFocus(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BusinessCardBinder* businessCardBinder;
	TCHAR companyName[32];
	BusinessCard* index;
	HWND businessCardBinderForm;
	IndexBinder* indexBinder;
	Index* index_;

	//2. 회사 명칭에 대하여 포커스를 잃을때
	if (HIWORD(wParam) == EN_KILLFOCUS) {
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYNAME), WM_GETTEXT, (WPARAM)32, (LPARAM)companyName);
		//2.1 명함철 윈도우를 찾는다.
		businessCardBinderForm = FindWindow("#32770", "명함철");

		//2.2 명함철 윈도우의 색인철에서 해당 상호를 찾는다.
		indexBinder = (IndexBinder*)GetProp(businessCardBinderForm, "indexBinder");
		index_ = IndexBinder_Find(indexBinder, companyName);
		//2.3 같은 회사 상호가 있을 경우, 해당 회사의 주소, 전화번호,팩스번호, url을 명함에 출력한다.
		if (index_ != NULL) {
			index = Index_GetAt(index_, 0);
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYADDRESS), WM_SETTEXT, 0,
				(LPARAM)index->company.address);
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYTELEPHONENUMBER), WM_SETTEXT, 0,
				(LPARAM)index->company.telephoneNumber);
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYFAXNUMBER), WM_SETTEXT, 0,
				(LPARAM)index->company.faxNumber);
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYURL), WM_SETTEXT, 0,
				(LPARAM)index->company.url);
		}
	}
	return TRUE;
}

BOOL PuttingForm_OnPutButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BusinessCardBinder* businessCardBinder;
	TCHAR personalName[11];
	TCHAR personalRankName[16];
	TCHAR personalTelephoneNumber[12];
	TCHAR personalEmailAddress[32];
	TCHAR personalTotalEmailAddress[32];
	TCHAR personalPortalSite[32];
	TCHAR companyName[32];
	TCHAR companyAddress[64];
	TCHAR companyTelephoneNumber[12];
	TCHAR companyFaxNumber[12];
	TCHAR companyUrl[64];
	BusinessCard* index;
	BusinessCard businessCard;
	HWND businessCardBinderForm;
	IndexBinder* indexBinder;
	Index* index_;
	TVINSERTSTRUCT insertStruct = { 0, };
	TVITEM item = { 0, };
	Long i = 0;
	Long j = 0;
	Long count;
	HTREEITEM parent;
	HTREEITEM company;
	HTREEITEM name;
	TCHAR textCompany[32];


	//3. 꽂기 버튼을 클릭했을때
	if (HIWORD(wParam) == BN_CLICKED) {
		//3.1 명함을 읽는다.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONALNAME), WM_GETTEXT, (WPARAM)11, (LPARAM)personalName);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONALRANKNAME), WM_GETTEXT, (WPARAM)16, 
			(LPARAM)personalRankName);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONALTELEPHONENUMBER), WM_GETTEXT, (WPARAM)12, 
			(LPARAM)personalTelephoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONALEMAILADDRESS), WM_GETTEXT, (WPARAM)32, 
			(LPARAM)personalEmailAddress);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONALPORTALSITE), WM_GETTEXT, (WPARAM)32, 
			(LPARAM)personalPortalSite);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYNAME), WM_GETTEXT, (WPARAM)32, (LPARAM)companyName);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYADDRESS), WM_GETTEXT, (WPARAM)64, 
			(LPARAM)companyAddress);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYTELEPHONENUMBER), WM_GETTEXT, (WPARAM)12, 
			(LPARAM)companyTelephoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYFAXNUMBER), WM_GETTEXT, (WPARAM)12, 
			(LPARAM)companyFaxNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYURL), WM_GETTEXT, (WPARAM)64, (LPARAM)companyUrl);

		//3.2 명함철 윈도우를 찾는다.
		businessCardBinderForm = FindWindow("#32770", "명함철");

		//3.3 명함철 윈도우의 명함철에 꽂는다.
		strcpy(businessCard.personal.name, personalName);
		strcpy(businessCard.personal.rankName, personalRankName);
		strcpy(businessCard.personal.telephoneNumber, personalTelephoneNumber);
		sprintf(personalTotalEmailAddress, "%s@%s", personalEmailAddress, personalPortalSite);
		strcpy(businessCard.personal.emailAddress, personalTotalEmailAddress);
		strcpy(businessCard.company.name, companyName);
		strcpy(businessCard.company.address, companyAddress);
		strcpy(businessCard.company.telephoneNumber, companyTelephoneNumber);
		strcpy(businessCard.company.faxNumber, companyFaxNumber);
		strcpy(businessCard.company.url, companyUrl);

		businessCardBinder = (BusinessCardBinder*)GetWindowLong(businessCardBinderForm, GWL_USERDATA);
		index = BusinessCardBinder_Put(businessCardBinder, businessCard);

		//3.4 명함철 윈도우의 명함에 현재 명함을 출력한다.
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


		//3.5 명함철 윈도우의 색인철에 꽂는다.
		indexBinder = (IndexBinder*)GetProp(businessCardBinderForm, "indexBinder");
		IndexBinder_Put(indexBinder, index);

		//3.6 회사들을 찾는다.
		parent = SendMessage(GetDlgItem(businessCardBinderForm, IDC_TREE_INDEXES), TVM_GETNEXTITEM,
				(WPARAM)TVGN_ROOT, (LPARAM)NULL);

		//3.7 트리뷰상 해당 상호핸들을 찾는다.
		company = SendMessage(GetDlgItem(businessCardBinderForm, IDC_TREE_INDEXES), TVM_GETNEXTITEM,
			(WPARAM)TVGN_CHILD, (LPARAM)parent);
		//3.8 트리뷰에서 상호를 찾는다.
		item.mask = TVIF_HANDLE | TVIF_TEXT | TVIF_STATE;
		item.hItem = company;
		item.pszText = textCompany;
		item.cchTextMax = 32;
		SendMessage(GetDlgItem(businessCardBinderForm, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);

		while (company != NULL && strcmp(textCompany, index->company.name) != 0) {
			company = SendMessage(GetDlgItem(businessCardBinderForm, IDC_TREE_INDEXES), TVM_GETNEXTITEM,
			         (WPARAM)TVGN_NEXT, (LPARAM)company);
			item.hItem = company;
			item.pszText = textCompany;
			item.cchTextMax = 32;
			SendMessage(GetDlgItem(businessCardBinderForm, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);
		}
		//3.9 해당 상호가 있으면 상호 하위에 개인 핸들을 추가한다.
		if (company != NULL) {
			insertStruct.hParent = company;
			insertStruct.hInsertAfter = TVI_LAST;
			insertStruct.item.mask = TVIF_TEXT;
			insertStruct.item.pszText = index->personal.name;
			name = SendMessage(GetDlgItem(businessCardBinderForm, IDC_TREE_INDEXES), TVM_INSERTITEM, 0,
					(LPARAM)&insertStruct);
		}
		else {//4.0 해당 상호가 없으면
			//4.1 트리뷰에 상호핸들을 추가한다.
			insertStruct.hParent = parent;
			insertStruct.hInsertAfter = TVI_SORT;
			insertStruct.item.mask = TVIF_TEXT;
			insertStruct.item.pszText = index->company.name;
			company = SendMessage(GetDlgItem(businessCardBinderForm, IDC_TREE_INDEXES), TVM_INSERTITEM,
				0, (LPARAM)&insertStruct);

			//4.2 상호핸들 하위에 개인핸들을 추가한다.
			insertStruct.hParent = company;
			insertStruct.hInsertAfter = TVI_LAST;
			insertStruct.item.pszText = index->personal.name;
			name = SendMessage(GetDlgItem(businessCardBinderForm, IDC_TREE_INDEXES), TVM_INSERTITEM, 0,
				(LPARAM)&insertStruct);
		}

		//4.3 회사들을 펼친다.
		SendMessage(GetDlgItem(businessCardBinderForm, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND,
			(LPARAM)parent);
		//4.4  해당 상호핸들을 펼친다.
		SendMessage(GetDlgItem(businessCardBinderForm, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND,
			(LPARAM)company);


		//4.5 해당 개인핸들을 선택된 상태로 처리한다.
		SendMessage(GetDlgItem(businessCardBinderForm, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET,
			(LPARAM)name);


		//4.2 꽂기 윈도우를 종료한다.
		EndDialog(hWnd, 0);
	}
	return TRUE;
}


BOOL PuttingForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	//4. 닫기 버튼을 클릭했을 때
	  //4.1 꽂기 윈도우를 종료한다.
	EndDialog(hWnd, 0);
	return TRUE;
}

