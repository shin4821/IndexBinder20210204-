//PuttingOutForm.c
#include"PuttingOutForm.h"
#include"resource.h"
#include"BusinessCardBinder.h"
#include"IndexBinder.h"
#include"Index.h"
#include<Commctrl.h> // TVM의 헤더파일 추가
#include<stdio.h>
#pragma warning(disable:4996)

BOOL CALLBACK PuttingOutFormProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	BOOL ret;

	switch (message) {
	case WM_INITDIALOG:
		ret = PuttingOutForm_OnInitDialog(hWnd, wParam, lParam);
		break;

	case WM_CLOSE:
		ret = PuttingOutForm_OnClose(hWnd, wParam, lParam);
		break;

	default:
		ret = FALSE;
		break;
	}
	return ret;
}

BOOL PuttingOutForm_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BusinessCardBinder* businessCardBinder;
	HWND businessCardBinderForm;
	BusinessCard* index;
	BusinessCard businessCard;
	IndexBinder* indexBinder;
	Index* index_;
	BusinessCard* current;
	HTREEITEM parent;
	HTREEITEM company;
	HTREEITEM name;
	Long i;
	Long j = 0;
	TCHAR textCompany[32];
	TVITEM item = { 0, };

	//1. 뽑기 윈도우가 생성될 때
	  //1.1 명함철 윈도우를 찾는다.
	  businessCardBinderForm = FindWindow("#32770", "명함철");

	  //1.2 명함철 윈도우의 현재 명함의 위치를 찾는다.
	  businessCardBinder = (BusinessCardBinder*)GetWindowLong(businessCardBinderForm, GWL_USERDATA);
	  index = businessCardBinder->current; // 이후 명함철에서 뽑으면 null될것.
	  current = businessCardBinder->current; //이후 Index_Find시 이용할 변수.

	  //1.3 명함철에서 해당 위치의 명함을 뽑는다.
	  businessCard = BusinessCardBinder_PutOut(businessCardBinder, index);

	  //1.4 트리뷰상 해당 상호 핸들을 찾는다.
	  parent = SendMessage(GetDlgItem(businessCardBinderForm, IDC_TREE_INDEXES), TVM_GETNEXTITEM,
		  (WPARAM)TVGN_ROOT, (LPARAM)NULL);
	  company = SendMessage(GetDlgItem(businessCardBinderForm, IDC_TREE_INDEXES), TVM_GETNEXTITEM,
		  (WPARAM)TVGN_CHILD, (LPARAM)parent);

	  item.mask = TVIF_HANDLE | TVIF_TEXT;
	  item.hItem = company;
	  item.pszText = textCompany;
	  item.cchTextMax = 32;
	  SendMessage(GetDlgItem(businessCardBinderForm, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);
	  while (strcmp(textCompany, businessCard.company.name) != 0) {
		  company = SendMessage(GetDlgItem(businessCardBinderForm, IDC_TREE_INDEXES), TVM_GETNEXTITEM,
			  (WPARAM)TVGN_NEXT, (LPARAM)company);
		  item.hItem = company;
		  item.pszText = textCompany;
		  item.cchTextMax = 32;
		  SendMessage(GetDlgItem(businessCardBinderForm, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);
	  }

	  //1.5 트리뷰 상 해당 개인 핸들을 찾는다.
	  indexBinder = (IndexBinder*)GetProp(businessCardBinderForm, "indexBinder");
	  index_ = IndexBinder_Find(indexBinder, businessCard.company.name);
	  i = Index_Find(index_, current);
	  name = SendMessage(GetDlgItem(businessCardBinderForm, IDC_TREE_INDEXES), TVM_GETNEXTITEM,
		  (WPARAM)TVGN_CHILD, (LPARAM)company);
	  while (j < i) {
		  name = SendMessage(GetDlgItem(businessCardBinderForm, IDC_TREE_INDEXES), TVM_GETNEXTITEM,
			  (WPARAM)TVGN_NEXT, (LPARAM)name);
		  j++;
	  }

	  //1.6 개인 핸들을 없앤다.
	  SendMessage(GetDlgItem(businessCardBinderForm, IDC_TREE_INDEXES), TVM_DELETEITEM, 0, (LPARAM)name);

	  //1.7 색인철에서 뽑는다.
	  IndexBinder_PutOut(indexBinder, businessCard.company.name, current);

	  //1.8 색인철에 해당 상호가 여전히 존재하는지 확인한다.
	  index_ = IndexBinder_Find(indexBinder, businessCard.company.name);

	  //1.9 색인철에 해당 상호가 없는 경우, 트리뷰 상 해당 상호 핸들을 없앤다.
	  if (index_ == NULL) {
		  SendMessage(GetDlgItem(businessCardBinderForm, IDC_TREE_INDEXES), TVM_DELETEITEM, 0, (LPARAM)company);
	  }

	  //1.9 뽑은 명함을 뽑기 윈도우 명함에 출력한다.
	  SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONALNAME), WM_SETTEXT, 0, 
		  (LPARAM)businessCard.personal.name);
	  SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONALRANKNAME), WM_SETTEXT, 0,
		  (LPARAM)businessCard.personal.rankName);
	  SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONALTELEPHONENUMBER), WM_SETTEXT, 0,
		  (LPARAM)businessCard.personal.telephoneNumber);
	  SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONALEMAILADDRESS), WM_SETTEXT, 0,
		  (LPARAM)businessCard.personal.emailAddress);
	  SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYNAME), WM_SETTEXT, 0,
		  (LPARAM)businessCard.company.name);
	  SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYADDRESS), WM_SETTEXT, 0,
		  (LPARAM)businessCard.company.address);
	  SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYTELEPHONENUMBER), WM_SETTEXT, 0,
		  (LPARAM)businessCard.company.telephoneNumber);
	  SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYFAXNUMBER), WM_SETTEXT, 0,
		  (LPARAM)businessCard.company.faxNumber);
	  SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYURL), WM_SETTEXT, 0,
		  (LPARAM)businessCard.company.url);

	  //2.0 명함철 윈도우의 명함철에서 현재 명함의 위치를 읽는다.
	  index = businessCardBinder->current;

	  //2.1 현재 명함이 있는 경우, 명함철 윈도우의 명함에 현재 명함을 출력한다.
	  if (index != NULL) {
		  //2.1.1 트리뷰상 해당 상호 핸들을 찾는다.
		  parent = SendMessage(GetDlgItem(businessCardBinderForm, IDC_TREE_INDEXES), TVM_GETNEXTITEM,
			  (WPARAM)TVGN_ROOT, (LPARAM)NULL);
		  company = SendMessage(GetDlgItem(businessCardBinderForm, IDC_TREE_INDEXES), TVM_GETNEXTITEM,
			  (WPARAM)TVGN_CHILD, (LPARAM)parent);

		  item.mask = TVIF_HANDLE | TVIF_TEXT;
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
		 
		  //2.1.2 트리뷰 상 개인 핸들을 찾는다.
		  index_ = IndexBinder_Find(indexBinder, index->company.name);
		  i = Index_Find(index_, index);
		  j = 0;
		  name = SendMessage(GetDlgItem(businessCardBinderForm, IDC_TREE_INDEXES), TVM_GETNEXTITEM,
			  (WPARAM)TVGN_CHILD, (LPARAM)company);
		  while (j < i) {
			  name = SendMessage(GetDlgItem(businessCardBinderForm, IDC_TREE_INDEXES), TVM_GETNEXTITEM,
				  (WPARAM)TVM_GETNEXTITEM, (LPARAM)name);
			  j++;
		  }

		  //2.1.3 회사들 카테고리를 펼친다.
		  SendMessage(GetDlgItem(businessCardBinderForm, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND,
			  (LPARAM)parent);
		  //2.1.4 상호핸들을 펼친다.
		  SendMessage(GetDlgItem(businessCardBinderForm, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND,
			  (LPARAM)company);
		  //2.1.5 트리뷰 상 해당 개인 핸들을 선택된 상태로 처리한다.
		  SendMessage(GetDlgItem(businessCardBinderForm, IDC_TREE_INDEXES), TVM_SELECTITEM,
			  (WPARAM)TVGN_CARET, (LPARAM)name);

		  //2.1.6  명함철 윈도우의 명함에 현재 명함을 출력한다.
		  SendMessage(GetDlgItem(businessCardBinderForm, IDC_EDIT_PERSONALNAME), WM_SETTEXT,
			  0, (LPARAM)index->personal.name);
		  SendMessage(GetDlgItem(businessCardBinderForm, IDC_EDIT_PERSONALRANKNAME), WM_SETTEXT,
			  0, (LPARAM)index->personal.rankName);
		  SendMessage(GetDlgItem(businessCardBinderForm, IDC_EDIT_PERSONALTELEPHONENUMBER), WM_SETTEXT,
			  0, (LPARAM)index->personal.telephoneNumber);
		  SendMessage(GetDlgItem(businessCardBinderForm, IDC_EDIT_PERSONALEMAILADDRESS), WM_SETTEXT,
			  0, (LPARAM)index->personal.emailAddress);
		  SendMessage(GetDlgItem(businessCardBinderForm, IDC_EDIT_COMPANYNAME), WM_SETTEXT,
			  0, (LPARAM)index->company.name);
		  SendMessage(GetDlgItem(businessCardBinderForm, IDC_EDIT_COMPANYADDRESS), WM_SETTEXT,
			  0, (LPARAM)index->company.address);
		  SendMessage(GetDlgItem(businessCardBinderForm, IDC_EDIT_COMPANYTELEPHONENUMBER), WM_SETTEXT,
			  0, (LPARAM)index->company.telephoneNumber);
		  SendMessage(GetDlgItem(businessCardBinderForm, IDC_EDIT_COMPANYFAXNUMBER), WM_SETTEXT,
			  0, (LPARAM)index->company.faxNumber);
		  SendMessage(GetDlgItem(businessCardBinderForm, IDC_EDIT_COMPANYURL), WM_SETTEXT,
			  0, (LPARAM)index->company.url);
	  }
	  //1.7 현재 명함이 없는 경우, 명함철 윈도우의 명함을 공백 처리한다.
	  else {
		  SendMessage(GetDlgItem(businessCardBinderForm, IDC_EDIT_PERSONALNAME), WM_SETTEXT,
			  0, (LPARAM)"");
		  SendMessage(GetDlgItem(businessCardBinderForm, IDC_EDIT_PERSONALRANKNAME), WM_SETTEXT,
			  0, (LPARAM)"");
		  SendMessage(GetDlgItem(businessCardBinderForm, IDC_EDIT_PERSONALTELEPHONENUMBER), WM_SETTEXT,
			  0, (LPARAM)"");
		  SendMessage(GetDlgItem(businessCardBinderForm, IDC_EDIT_PERSONALEMAILADDRESS), WM_SETTEXT,
			  0, (LPARAM)"");
		  SendMessage(GetDlgItem(businessCardBinderForm, IDC_EDIT_COMPANYNAME), WM_SETTEXT,
			  0, (LPARAM)"");
		  SendMessage(GetDlgItem(businessCardBinderForm, IDC_EDIT_COMPANYADDRESS), WM_SETTEXT,
			  0, (LPARAM)"");
		  SendMessage(GetDlgItem(businessCardBinderForm, IDC_EDIT_COMPANYTELEPHONENUMBER), WM_SETTEXT,
			  0, (LPARAM)"");
		  SendMessage(GetDlgItem(businessCardBinderForm, IDC_EDIT_COMPANYFAXNUMBER), WM_SETTEXT,
			  0, (LPARAM)"");
		  SendMessage(GetDlgItem(businessCardBinderForm, IDC_EDIT_COMPANYURL), WM_SETTEXT,
			  0, (LPARAM)"");
	  }
	  return TRUE;
}

BOOL PuttingOutForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BusinessCardBinder* businessCardBinder;
	HWND businessCardBinderForm;
	BusinessCard* index;
	BusinessCard businessCard;
	TCHAR personalName[11];
	TCHAR rankName[16];
	TCHAR personalTelephoneNumber[12];
	TCHAR emailAddress[32];
	TCHAR companyName[32];
	TCHAR address[64];
	TCHAR companyTelephoneNumber[12];
	TCHAR faxNumber[12];
	TCHAR url[64];
	int ret;
	IndexBinder* indexBinder;
	TVINSERTSTRUCT insertStruct = { 0, };
	TVITEM item = { 0, };
	HTREEITEM parent;
	HTREEITEM company;
	HTREEITEM name;
	TCHAR textCompany[32];

	//2. 닫기 버튼을 클릭했을 때
	  //2.1 메세지 박스를 띄운다.
	  ret = MessageBox(hWnd, "끼우시겠습니까?", "확인창", MB_YESNOCANCEL);
	  //2.2 '예'버튼을 클릭했을 때
	  if (ret == IDYES) {
		  //2.2.1 명함을 읽는다.
		  SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONALNAME), WM_GETTEXT, (WPARAM)11, (LPARAM)personalName);
		  SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONALRANKNAME), WM_GETTEXT, (WPARAM)16, 
			  (LPARAM)rankName);
		  SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONALTELEPHONENUMBER), WM_GETTEXT, (WPARAM)12, 
			  (LPARAM)personalTelephoneNumber);
		  SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONALEMAILADDRESS), WM_GETTEXT, (WPARAM)32, 
			  (LPARAM)emailAddress);
		  SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYNAME), WM_GETTEXT, (WPARAM)32, (LPARAM)companyName);
		  SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYADDRESS), WM_GETTEXT, (WPARAM)64, 
			  (LPARAM)address);
		  SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYTELEPHONENUMBER), WM_GETTEXT, (WPARAM)12, 
			  (LPARAM)companyTelephoneNumber);
		  SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYFAXNUMBER), WM_GETTEXT, (WPARAM)12, 
			  (LPARAM)faxNumber);
		  SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYURL), WM_GETTEXT, (WPARAM)64, (LPARAM)url);

		  //2.2.2 명함철 윈도우를 찾는다.
		  businessCardBinderForm = FindWindow("#32770", "명함철");
		  //2.2.3 명함철 윈도우의 명함철에 꽂는다.
		  businessCardBinder = (BusinessCardBinder*)GetWindowLong(businessCardBinderForm, GWL_USERDATA);

		  strcpy(businessCard.personal.name, personalName);
		  strcpy(businessCard.personal.rankName, rankName);
		  strcpy(businessCard.personal.telephoneNumber, personalTelephoneNumber);
		  strcpy(businessCard.personal.emailAddress, emailAddress);
		  strcpy(businessCard.company.name, companyName);
		  strcpy(businessCard.company.address, address);
		  strcpy(businessCard.company.telephoneNumber, companyTelephoneNumber);
		  strcpy(businessCard.company.faxNumber, faxNumber);
		  strcpy(businessCard.company.url, url);
		  index = BusinessCardBinder_Put(businessCardBinder, businessCard);

		  //2.2.4 명함철 윈도우의 색인철에 꽂는다.
		  indexBinder = (IndexBinder*)GetProp(businessCardBinderForm, "indexBinder");
		  IndexBinder_Put(indexBinder, index);

		  //2.2.5 회사들을 찾는다.
		  parent = SendMessage(GetDlgItem(businessCardBinderForm, IDC_TREE_INDEXES), TVM_GETNEXTITEM,
			  (WPARAM)TVGN_ROOT, (LPARAM)NULL);

		  //2.2.6 트리뷰상 해당 상호핸들을 찾는다.
		  company = SendMessage(GetDlgItem(businessCardBinderForm, IDC_TREE_INDEXES), TVM_GETNEXTITEM,
			  (WPARAM)TVGN_CHILD, (LPARAM)parent);
		  item.mask = TVIF_HANDLE | TVIF_TEXT;
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
		  //2.2.7 해당 상호가 있으면 상호 하위에 개인 핸들을 추가한다.
		  if (company != NULL) {
			  insertStruct.hParent = company;
			  insertStruct.hInsertAfter = TVI_LAST;
			  insertStruct.item.mask = TVIF_TEXT;
			  insertStruct.item.pszText = index->personal.name;
			  name = SendMessage(GetDlgItem(businessCardBinderForm, IDC_TREE_INDEXES), TVM_INSERTITEM, 0,
				  (LPARAM)&insertStruct);
		  }
		  //2.2.8 해당 상호가 없으면 
		  else {
			  //2.2.8.1 트리뷰에 상호핸들을 추가한다.
			  insertStruct.hParent = parent;
			  insertStruct.hInsertAfter = TVI_SORT;
			  insertStruct.item.mask = TVIF_TEXT;
			  insertStruct.item.pszText = index->company.name;
			  company = SendMessage(GetDlgItem(businessCardBinderForm, IDC_TREE_INDEXES), TVM_INSERTITEM,
				  0, (LPARAM)&insertStruct);

			  //2.2.8.2 상호핸들 하위에 개인핸들을 추가한다.
			  insertStruct.hParent = company;
			  insertStruct.hInsertAfter = TVI_LAST;
			  insertStruct.item.pszText = index->personal.name;
			  name = SendMessage(GetDlgItem(businessCardBinderForm, IDC_TREE_INDEXES), TVM_INSERTITEM, 0,
				  (LPARAM)&insertStruct);
		  }

		  //2.2.9 회사들을 펼친다.
		  SendMessage(GetDlgItem(businessCardBinderForm, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND,
			  (LPARAM)parent);
		  //2.3.0  해당 상호핸들을 펼친다.
		  SendMessage(GetDlgItem(businessCardBinderForm, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND,
			  (LPARAM)company);

		  //2.3.1 해당 개인핸들을 선택된 상태로 처리한다.
		  SendMessage(GetDlgItem(businessCardBinderForm, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET,
			  (LPARAM)name);


		  //2.3.2 명함철 윈도우의 명함에 현재 명함을 출력한다.
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

		  //2.2.3 뽑기 윈도우를 종료한다.
		  EndDialog(hWnd, 0);
	  }
	  //2.3 '아니오'버튼을 클릭했을때
	  else if(ret == IDNO) {
		  //2.3.1 뽑기 윈도우를 종료한다.
		  EndDialog(hWnd, 0);
	  }
	  return TRUE;
}




