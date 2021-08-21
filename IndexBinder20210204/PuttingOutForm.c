//PuttingOutForm.c
#include"PuttingOutForm.h"
#include"resource.h"
#include"BusinessCardBinder.h"
#include"IndexBinder.h"
#include"Index.h"
#include<Commctrl.h> // TVM�� ������� �߰�
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

	//1. �̱� �����찡 ������ ��
	  //1.1 ����ö �����츦 ã�´�.
	  businessCardBinderForm = FindWindow("#32770", "����ö");

	  //1.2 ����ö �������� ���� ������ ��ġ�� ã�´�.
	  businessCardBinder = (BusinessCardBinder*)GetWindowLong(businessCardBinderForm, GWL_USERDATA);
	  index = businessCardBinder->current; // ���� ����ö���� ������ null�ɰ�.
	  current = businessCardBinder->current; //���� Index_Find�� �̿��� ����.

	  //1.3 ����ö���� �ش� ��ġ�� ������ �̴´�.
	  businessCard = BusinessCardBinder_PutOut(businessCardBinder, index);

	  //1.4 Ʈ����� �ش� ��ȣ �ڵ��� ã�´�.
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

	  //1.5 Ʈ���� �� �ش� ���� �ڵ��� ã�´�.
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

	  //1.6 ���� �ڵ��� ���ش�.
	  SendMessage(GetDlgItem(businessCardBinderForm, IDC_TREE_INDEXES), TVM_DELETEITEM, 0, (LPARAM)name);

	  //1.7 ����ö���� �̴´�.
	  IndexBinder_PutOut(indexBinder, businessCard.company.name, current);

	  //1.8 ����ö�� �ش� ��ȣ�� ������ �����ϴ��� Ȯ���Ѵ�.
	  index_ = IndexBinder_Find(indexBinder, businessCard.company.name);

	  //1.9 ����ö�� �ش� ��ȣ�� ���� ���, Ʈ���� �� �ش� ��ȣ �ڵ��� ���ش�.
	  if (index_ == NULL) {
		  SendMessage(GetDlgItem(businessCardBinderForm, IDC_TREE_INDEXES), TVM_DELETEITEM, 0, (LPARAM)company);
	  }

	  //1.9 ���� ������ �̱� ������ ���Կ� ����Ѵ�.
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

	  //2.0 ����ö �������� ����ö���� ���� ������ ��ġ�� �д´�.
	  index = businessCardBinder->current;

	  //2.1 ���� ������ �ִ� ���, ����ö �������� ���Կ� ���� ������ ����Ѵ�.
	  if (index != NULL) {
		  //2.1.1 Ʈ����� �ش� ��ȣ �ڵ��� ã�´�.
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
		 
		  //2.1.2 Ʈ���� �� ���� �ڵ��� ã�´�.
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

		  //2.1.3 ȸ��� ī�װ��� ��ģ��.
		  SendMessage(GetDlgItem(businessCardBinderForm, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND,
			  (LPARAM)parent);
		  //2.1.4 ��ȣ�ڵ��� ��ģ��.
		  SendMessage(GetDlgItem(businessCardBinderForm, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND,
			  (LPARAM)company);
		  //2.1.5 Ʈ���� �� �ش� ���� �ڵ��� ���õ� ���·� ó���Ѵ�.
		  SendMessage(GetDlgItem(businessCardBinderForm, IDC_TREE_INDEXES), TVM_SELECTITEM,
			  (WPARAM)TVGN_CARET, (LPARAM)name);

		  //2.1.6  ����ö �������� ���Կ� ���� ������ ����Ѵ�.
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
	  //1.7 ���� ������ ���� ���, ����ö �������� ������ ���� ó���Ѵ�.
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

	//2. �ݱ� ��ư�� Ŭ������ ��
	  //2.1 �޼��� �ڽ��� ����.
	  ret = MessageBox(hWnd, "����ðڽ��ϱ�?", "Ȯ��â", MB_YESNOCANCEL);
	  //2.2 '��'��ư�� Ŭ������ ��
	  if (ret == IDYES) {
		  //2.2.1 ������ �д´�.
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

		  //2.2.2 ����ö �����츦 ã�´�.
		  businessCardBinderForm = FindWindow("#32770", "����ö");
		  //2.2.3 ����ö �������� ����ö�� �ȴ´�.
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

		  //2.2.4 ����ö �������� ����ö�� �ȴ´�.
		  indexBinder = (IndexBinder*)GetProp(businessCardBinderForm, "indexBinder");
		  IndexBinder_Put(indexBinder, index);

		  //2.2.5 ȸ����� ã�´�.
		  parent = SendMessage(GetDlgItem(businessCardBinderForm, IDC_TREE_INDEXES), TVM_GETNEXTITEM,
			  (WPARAM)TVGN_ROOT, (LPARAM)NULL);

		  //2.2.6 Ʈ����� �ش� ��ȣ�ڵ��� ã�´�.
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
		  //2.2.7 �ش� ��ȣ�� ������ ��ȣ ������ ���� �ڵ��� �߰��Ѵ�.
		  if (company != NULL) {
			  insertStruct.hParent = company;
			  insertStruct.hInsertAfter = TVI_LAST;
			  insertStruct.item.mask = TVIF_TEXT;
			  insertStruct.item.pszText = index->personal.name;
			  name = SendMessage(GetDlgItem(businessCardBinderForm, IDC_TREE_INDEXES), TVM_INSERTITEM, 0,
				  (LPARAM)&insertStruct);
		  }
		  //2.2.8 �ش� ��ȣ�� ������ 
		  else {
			  //2.2.8.1 Ʈ���信 ��ȣ�ڵ��� �߰��Ѵ�.
			  insertStruct.hParent = parent;
			  insertStruct.hInsertAfter = TVI_SORT;
			  insertStruct.item.mask = TVIF_TEXT;
			  insertStruct.item.pszText = index->company.name;
			  company = SendMessage(GetDlgItem(businessCardBinderForm, IDC_TREE_INDEXES), TVM_INSERTITEM,
				  0, (LPARAM)&insertStruct);

			  //2.2.8.2 ��ȣ�ڵ� ������ �����ڵ��� �߰��Ѵ�.
			  insertStruct.hParent = company;
			  insertStruct.hInsertAfter = TVI_LAST;
			  insertStruct.item.pszText = index->personal.name;
			  name = SendMessage(GetDlgItem(businessCardBinderForm, IDC_TREE_INDEXES), TVM_INSERTITEM, 0,
				  (LPARAM)&insertStruct);
		  }

		  //2.2.9 ȸ����� ��ģ��.
		  SendMessage(GetDlgItem(businessCardBinderForm, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND,
			  (LPARAM)parent);
		  //2.3.0  �ش� ��ȣ�ڵ��� ��ģ��.
		  SendMessage(GetDlgItem(businessCardBinderForm, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND,
			  (LPARAM)company);

		  //2.3.1 �ش� �����ڵ��� ���õ� ���·� ó���Ѵ�.
		  SendMessage(GetDlgItem(businessCardBinderForm, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET,
			  (LPARAM)name);


		  //2.3.2 ����ö �������� ���Կ� ���� ������ ����Ѵ�.
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

		  //2.2.3 �̱� �����츦 �����Ѵ�.
		  EndDialog(hWnd, 0);
	  }
	  //2.3 '�ƴϿ�'��ư�� Ŭ��������
	  else if(ret == IDNO) {
		  //2.3.1 �̱� �����츦 �����Ѵ�.
		  EndDialog(hWnd, 0);
	  }
	  return TRUE;
}




