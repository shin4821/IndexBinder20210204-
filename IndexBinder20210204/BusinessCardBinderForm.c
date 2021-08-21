//BusinessCardBinderForm.c
#include"BusinessCardBinderForm.h"
#include"BusinessCardBinder.h"
#include"IndexBinder.h"
#include"Index.h"
#include"PuttingForm.h"
#include"FindingForm.h"
#include"PuttingOutForm.h"
#include"resource.h"
#include<stdio.h>
#include<stdlib.h>
#include<Commctrl.h> // TVM�� ������� �߰�
#pragma warning(disable:4996)


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevlnstance, LPSTR lpCmdLine, int nCmdShow) {
	int response;
	response = DialogBox(hInstance, MAKEINTRESOURCE(IDD_BUSINESSCARDBINDERFORM), NULL,
		BusinessCardBinderFormProc);
	return response;
}

BOOL CALLBACK BusinessCardBinderFormProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	BOOL ret;

	switch (message) {
	case WM_INITDIALOG:
		ret = BusinessCardBinderForm_OnInitDialog(hWnd, wParam, lParam);
		break;


	case WM_COMMAND:
		ret = BusinessCardBinderForm_OnCommand(hWnd, wParam, lParam);
		break;


	case WM_NOTIFY:
		ret = BusinessCardBinderForm_OnNotify(hWnd, wParam, lParam);
		break;



	case WM_CLOSE:
		ret = BusinessCardBinderForm_OnClose(hWnd, wParam, lParam);
		break;

	default:
		ret = FALSE;
		break;
	}
	return ret;
}

BOOL BusinessCardBinderForm_OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BOOL ret;
	switch (LOWORD(wParam)) {
	case IDC_BUTTON_PUT:
			ret = BusinessCardBinderForm_OnPutButtonClicked(hWnd, wParam, lParam);
			break;

	case IDC_BUTTON_FIND:
		ret = BusinessCardBinderForm_OnFindButtonClicked(hWnd, wParam, lParam);
		break;

	case IDC_BUTTON_PUTOUT:
		ret = BusinessCardBinderForm_OnPutOutButtonClicked(hWnd, wParam, lParam);
		break;


	case IDC_BUTTON_ARRANGE:
		ret = BusinessCardBinderForm_OnArrangeButtonClicked(hWnd, wParam, lParam);
		break;


	case IDC_BUTTON_FIRST:
		ret = BusinessCardBinderForm_OnFirstButtonClicked(hWnd, wParam, lParam);
		break;

	case IDC_BUTTON_PREVIOUS:
		ret = BusinessCardBinderForm_OnPreviousButtonClicked(hWnd, wParam, lParam);
		break;

	case IDC_BUTTON_NEXT:
		ret = BusinessCardBinderForm_OnNextButtonClicked(hWnd, wParam, lParam);
		break;

	case IDC_BUTTON_LAST:
		ret = BusinessCardBinderForm_OnLastButtonClicked(hWnd, wParam, lParam);
		break;

		default:
			ret = FALSE;
			break;
	}
	return ret;
}


BOOL BusinessCardBinderForm_OnNotify(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BOOL ret;
	switch (wParam) {
	case IDC_TREE_INDEXES:
		ret = BusinessCardBinderForm_OnTreeViewItemButtonClicked(hWnd, wParam, lParam);
		break;
	}
	return ret;
}



BOOL BusinessCardBinderForm_OnInitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam) {

	BusinessCardBinder* businessCardBinder;
	BusinessCard* index;
	IndexBinder* indexBinder;
	Index* index_;
	BusinessCard* previous = NULL;
	Long i;
	Long j = 0;
	Long count;
	TCHAR textCompany[32];
	TCHAR textPerson[11];
	HTREEITEM person;
	HTREEITEM parent;
	HTREEITEM company;
	Index(*indexes) = NULL;
	TVINSERTSTRUCT insertStruct = { 0, };
	TVITEM item = { 0, };

	//1. �����찡 �����ɶ�
	  //1.1 ����ö�� �����.(create)
	businessCardBinder = (BusinessCardBinder*)malloc(sizeof(BusinessCardBinder));
	BusinessCardBinder_Create(businessCardBinder);
	SetWindowLong(hWnd, GWL_USERDATA, (LONG)businessCardBinder);

	//1.2 ����ö�� �����.(create)
	indexBinder = (IndexBinder*)malloc(sizeof(IndexBinder));
	BinaryTree_Create(indexBinder);
	SetProp(hWnd, "indexBinder", indexBinder);

	//1.6 SysTreeView32��  ȸ��� ī�װ��� �����.
	insertStruct.hParent = TVI_ROOT;
	insertStruct.hInsertAfter = TVI_FIRST;
	insertStruct.item.mask = TVIF_TEXT;
	insertStruct.item.pszText = "ȸ���";
	parent = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_INSERTITEM, 0, (LPARAM)&insertStruct);

	//1.3 ����ö�� �����Ѵ�.(Load)
	count=Load(businessCardBinder);
	if (count > 0){
		
		//1.4 ������ ó������ ������ �ݺ��Ѵ�.
		index = BusinessCardBinder_First(businessCardBinder);
		while (index != previous) {
			//1.6.1 ����ö�� �����.(put)
			IndexBinder_Put(indexBinder, index);
			previous = index;
			index = BusinessCardBinder_Next(businessCardBinder);
		}
		//1.5 ���θ���Ʈ�� �����.(MakeList)
		MakeList(indexBinder, &indexes, &count);

		//1.7 ������ ������ŭ �ݺ��Ѵ�.
		i = 0;
		while (i < count) {
			//1.7.1 ȸ��� ������ ��ȣ���� �߰��Ѵ�.
			insertStruct.hParent = parent;
			insertStruct.hInsertAfter = TVI_LAST;
			insertStruct.item.pszText = indexes[i].companyName;
			company = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_INSERTITEM, 0, (LPARAM)&insertStruct);

			//1.7.2 ������ ������ŭ �ݺ��Ѵ�.
			j = 0;
			while (j < indexes[i].businessCards.length) {
				//1.7.2.1 ��ȣ�� ������ ���μ����� �߰��Ѵ�.
				index = Index_GetAt(indexes+i, j);
				insertStruct.hParent = company;
				insertStruct.hInsertAfter = TVI_LAST;
				insertStruct.item.pszText = index->personal.name;
				SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_INSERTITEM, 0, (LPARAM)&insertStruct);
				j++;
			}
			i++;
		}
		//1.8 ����ö�� ���� ��ġ�� ù��° ��ġ�� �̵���Ų��.
		index = BusinessCardBinder_First(businessCardBinder);
		if (index != NULL) {
			//1.8.1 ������ ������
			//1.8.1.1  ����ö���� ������ ��ȣ������ ������ ã�´�.(�����ڵ��� ���Ѵ�.)
			company = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD,
				(LPARAM)parent); //ù��° ��ȣ�ڵ�
			item.mask = TVIF_TEXT | TVIF_HANDLE;
			item.hItem = company;
			item.pszText = textCompany; //ù��° ��ȣ�ڵ��� buffer�� �־��ش�.
			item.cchTextMax = 32;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);

			while (strcmp(index->company.name, textCompany) != 0) { // ��ȣ�ڵ�� ���� ������ġ�� ��ȣ�� ������ Ȯ��.
				company = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT,
					(LPARAM)company); // �� ���� ��ȣ�ڵ� ã��.
				item.hItem = company;
				item.pszText = textCompany;//��ȣ�ڵ��� buffer�� �־��ش�.
				item.cchTextMax = 32;
				SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);
		    }

			//1.8.1.2 ���μ��� �ڵ��� ���Ѵ�.
			  // ���� ��ġ�� ���Ѵ�.
			  index_ = IndexBinder_Find(indexBinder, index->company.name);

			  // ���μ��� �迭��ġ�� ���Ѵ�.
			  i = Index_Find(index_, index);

			  // ���μ��� �ڵ��� ���Ѵ�.
			  person = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD,
				  (LPARAM)company);
			  j = 0;
			  while (j < i) {
				  person = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT,
					  (LPARAM)person);
				  j++;
			  }

			 
			//1.8.1.3 Ʈ������Ʈ�ѿ��� ������ �����Ѵ�.

			//ȸ���(parent) expand���ֱ�.
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)parent);
			//ȸ��(company) expand���ֱ�.
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)company);
			//�ش� ��°�� ���μ����� ���õ� ���·� ó���Ѵ�.
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET, (LPARAM)person);


			//1.8.1.4. ���Կ� ���� ��ġ�� ������ ����Ѵ�.
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONALNAME), WM_SETTEXT, 0, index->personal.name);
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONALRANKNAME), WM_SETTEXT, 0, index->personal.rankName);
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONALTELEPHONENUMBER), WM_SETTEXT, 0,
				index->personal.telephoneNumber);
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONALEMAILADDRESS), WM_SETTEXT, 0,
				index->personal.emailAddress);
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYNAME), WM_SETTEXT, 0, index->company.name);
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYADDRESS), WM_SETTEXT, 0, index->company.address);
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYTELEPHONENUMBER), WM_SETTEXT, 0,
				index->company.telephoneNumber);
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYFAXNUMBER), WM_SETTEXT, 0, index->company.faxNumber);
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYURL), WM_SETTEXT, 0, index->company.url);
		}

#if 0
		//2.2 ���� �ڵ��� ��������, indexes �迭 �� ���� ��ġ�� ���Ѵ�.
		i = 0;
		while (strcmp(indexes[i].companyName, companyName) != 0) {
			i++;
		}
		//2.3 ���μ��� �迭 ��ġ�� ���Ѵ�.
		j = 0;
		index = Index_GetAt(indexes + i, j);
		while (strcmp(index->personal.name, name) == 0) {
			j++;
			index = Index_GetAt(indexes + i, j);
		}
#endif
	}

	//2.8 indexes�� �Ҵ� �����Ѵ�.
	if (indexes != NULL) {
		free(indexes);
		indexes = NULL;
	}

	return TRUE;
}

BOOL BusinessCardBinderForm_OnPutButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	//2. �ȱ� ��ư�� Ŭ��������
	if (HIWORD(wParam) == BN_CLICKED) {
		//2.1 �ȱ� �����츦 ����Ѵ�.
		DialogBox((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_PUTTINGFORM), NULL,
			PuttingFormProc);
	}
	return TRUE;
}

BOOL BusinessCardBinderForm_OnFindButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	//3. ã�� ��ư�� Ŭ��������
	if (HIWORD(wParam) == BN_CLICKED) {
		//3.1 ã�� �����츦 ����Ѵ�.
		DialogBox((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_FINDINGFORM), NULL,
			FindingFormProc);
	}
	return TRUE;

}

BOOL BusinessCardBinderForm_OnPutOutButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	//4. �̱� ��ư�� Ŭ��������
	if (HIWORD(wParam) == BN_CLICKED) {
		//4.1 �̱� �����츦 ����Ѵ�.
		DialogBox((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_PUTTINGOUTFORM), NULL,
			PuttingOutFormProc);
	}
	return TRUE;
}

BOOL BusinessCardBinderForm_OnArrangeButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BusinessCardBinder* businessCardBinder;
	BusinessCard* index;
	IndexBinder* indexBinder;
	Index* index_;
	HTREEITEM parent;
	HTREEITEM company;
	HTREEITEM name;
	Long count;
	Index(*indexes);
	Long i = 0;
	TVINSERTSTRUCT insertStruct = { 0, };
	TVITEM item = { 0, };
	Long j=0;
	TCHAR textCompany[32];

	//5. �����ϱ� ��ư�� Ŭ������ ��
	if (HIWORD(wParam) == BN_CLICKED) {
		//5.1 ����ö���� �����Ѵ�.
		
		indexBinder = (IndexBinder*)GetProp(hWnd, "indexBinder");
		IndexBinder_Arrange(indexBinder);

		//5.2 Ʈ������Ʈ���� �׸���� �ʱ�ȭ�Ѵ�.
		parent = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_ROOT,
			(LPARAM)NULL);
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_DELETEITEM, (WPARAM)0, (LPARAM)parent);

		//5.3 ���θ���Ʈ�� ���Ѵ�.
		MakeList(indexBinder, &indexes, &count);

		//5.4 Ʈ������Ʈ�ѿ� ȸ��� �׸��� �߰��Ѵ�.
		insertStruct.hParent = TVI_ROOT;
		insertStruct.hInsertAfter = TVI_FIRST;
		insertStruct.item.mask = TVIF_TEXT;
		insertStruct.item.pszText = "ȸ���";
		parent = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_INSERTITEM, 0, (LPARAM)&insertStruct);

		//5.5 ������ ������ŭ �ݺ��Ѵ�.
		while (i < count) {
			//5.5.1 ȸ��� ������ ��ȣ���� �߰��Ѵ�.
			insertStruct.hParent = parent;
			insertStruct.hInsertAfter = TVI_LAST;
			insertStruct.item.pszText = indexes[i].companyName;
			company = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_INSERTITEM, 0, (LPARAM)&insertStruct);

			//5.5.2 ������ ������ŭ �ݺ��Ѵ�.
			j = 0;
			while (j < indexes[i].length) {
				//5.5.2.1 ��ȣ�� ������ ���μ����� �߰��Ѵ�.
				insertStruct.hParent = company;
				insertStruct.hInsertAfter = TVI_LAST;
				index = Index_GetAt(indexes + i, j);
				insertStruct.item.pszText = index->personal.name;
				SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_INSERTITEM, 0, (LPARAM)&insertStruct);
				j++;
			}
			i++;
		}

		//5.6 ������ ������ġ�� ù��° ������ġ�� �ű��.
		businessCardBinder = (BusinessCardBinder*)GetWindowLong(hWnd, GWL_USERDATA);
		index =  BusinessCardBinder_First(businessCardBinder);

		//5.7 Ʈ���� �� ���� ��ġ�� ��ȣ�ڵ��� ã�´�.
		parent = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_ROOT,
			(LPARAM)NULL);

		company = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD,
			(LPARAM)parent); 

		item.mask = TVIF_TEXT | TVIF_HANDLE;
		item.hItem = company;
		item.pszText = textCompany; 
		item.cchTextMax = 32;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);

		while (strcmp(index->company.name, textCompany) != 0) { 
			company = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT,
				(LPARAM)company);
			item.hItem = company;
			item.pszText = textCompany;
			item.cchTextMax = 32;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);
		}

		//5.8 ���μ��� �ڵ��� ���Ѵ�.
		index_ = IndexBinder_Find(indexBinder, index->company.name);
		i = Index_Find(index_, index);

		name = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD,
			(LPARAM)company);
		while (j < i) {
			name = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT,
				(LPARAM)name);
			j++;
		}

		//5.7 ȸ����� ��ģ��.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND,
			(LPARAM)parent);
		//5.8  �ش� ��ȣ�ڵ��� ��ģ��.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND,
			(LPARAM)company);

		//5.9 �ش� �����ڵ��� ���õ� ���·� ó���Ѵ�.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET,
			(LPARAM)name);

		//6.0 ����ö�� ���� ��ġ�� ������ ����Ѵ�.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONALNAME), WM_SETTEXT, 0, (LPARAM)index->personal.name);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONALRANKNAME), WM_SETTEXT, 0, 
			(LPARAM)index->personal.rankName);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONALTELEPHONENUMBER), WM_SETTEXT, 0, 
			(LPARAM)index->personal.telephoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONALEMAILADDRESS), WM_SETTEXT, 0, 
			(LPARAM)index->personal.emailAddress);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYNAME), WM_SETTEXT, 0, (LPARAM)index->company.name);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYADDRESS), WM_SETTEXT, 0, (LPARAM)index->company.address);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYTELEPHONENUMBER), WM_SETTEXT, 0, 
			(LPARAM)index->company.telephoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYFAXNUMBER), WM_SETTEXT, 0, 
			(LPARAM)index->company.faxNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYURL), WM_SETTEXT, 0, (LPARAM)index->company.url);
	}
	return TRUE;
}

BOOL BusinessCardBinderForm_OnTreeViewItemButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BusinessCardBinder* businessCardBinder;
	BusinessCard* index;
	IndexBinder* indexBinder;
	Index* index_;
	Long i=0;
	TVITEM item = { 0, };
	HTREEITEM name;
	HTREEITEM testName;
	HTREEITEM company;
	HTREEITEM handle;
	HTREEITEM parentHandle;
	HTREEITEM parent;
	TCHAR textName[11];
	TCHAR textCompany[32];

	//2. Ʈ������Ʈ�� ���� �����ڵ� �Ǵ� ��ȣ�ڵ��� ����Ŭ��������
	if (((LPNMHDR)lParam)->code == NM_DBLCLK) {
		//2.1 Ŭ���� �ڵ��� �����ڵ����� ��ȣ�ڵ����� Ȯ���Ѵ�.
		handle= SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CARET,
			(LPARAM)NULL); // Ŭ���� �ڵ�

		parentHandle = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_PARENT,
			(LPARAM)handle); // Ŭ���� �ڵ��� ���� �ڵ�

		parent = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_ROOT,
			(LPARAM)NULL); //��Ʈ �ڵ�

		//2.2 Ŭ��Ⱥ �ڵ��� �����ڵ��� ���,
		if (parentHandle != parent) {
			//2.2.1 Ŭ���� ���μ��� �ڵ��� ���Ѵ�.
			name = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CARET,
				(LPARAM)NULL);

			//2.2.2 ���μ��� �θ��ڵ�(��ȣ�ڵ�)�� ���Ѵ�.
			company = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_PARENT,
				(LPARAM)name);

			//2.2.3 ��ȣ�ڵ��� text�� ���Ѵ�.
			item.mask = TVIF_HANDLE | TVIF_TEXT;
			item.hItem = company;
			item.pszText = textCompany;
			item.cchTextMax = 32;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);


			//2.2.4 �ش� ��ȣ�ڵ鿡�� �ش� �����ڵ��� ���°���� Ȯ���Ѵ�.(÷�� Ȯ��)
			testName = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD,
				(LPARAM)company);

			while (testName != NULL && testName != name) {
				testName = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT,
					(LPARAM)testName);
				i++;
			}

			//2.2.5 ����ö���� ��ȣ ��ġ�� ���Ѵ�.
			indexBinder = (IndexBinder*)GetProp(hWnd, "indexBinder");
			index_ = IndexBinder_Find(indexBinder, textCompany);

			//2.2.6 �ش� ��ȣ �� ������ ��ġ�� ã�´�.
			index = Index_GetAt(index_, i);

			//2.2.7 �ش� ��ġ�� ������ ���� �������� �����Ѵ�.
			businessCardBinder = (BusinessCardBinder*)GetWindowLong(hWnd, GWL_USERDATA);
			index = Move(businessCardBinder, index);


			//2.2.8 ���� ������ ���Կ� ����Ѵ�.
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONALNAME), WM_SETTEXT, 0, index->personal.name);
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONALRANKNAME), WM_SETTEXT, 0, index->personal.rankName);
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONALTELEPHONENUMBER), WM_SETTEXT, 0,
				index->personal.telephoneNumber);
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONALEMAILADDRESS), WM_SETTEXT, 0,
				index->personal.emailAddress);
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYNAME), WM_SETTEXT, 0, index->company.name);
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYADDRESS), WM_SETTEXT, 0, index->company.address);
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYTELEPHONENUMBER), WM_SETTEXT, 0,
				index->company.telephoneNumber);
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYFAXNUMBER), WM_SETTEXT, 0, index->company.faxNumber);
			SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYURL), WM_SETTEXT, 0, index->company.url);
		}
	}
	return TRUE;
}


BOOL BusinessCardBinderForm_OnFirstButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BusinessCardBinder* businessCardBinder;
	BusinessCard* index;
	IndexBinder* indexBinder;
	Index* index_;
	HTREEITEM parent;
	HTREEITEM company;
	HTREEITEM name;
	TCHAR textCompany[32];
	Long i;
	Long j = 0;
	TVITEM item = { 0, };

	//6. ó�� ��ư�� Ŭ������ ��
	if (HIWORD(wParam) == BN_CLICKED) {
		//6.1 ����ö���� ������ ����´�.
		indexBinder = (IndexBinder*)GetProp(hWnd, "indexBinder");

		//6.2 ����ö���� ó�� ������ ���� ��ġ�� ���ٵд�.
		businessCardBinder = (BusinessCardBinder*)GetWindowLong(hWnd, GWL_USERDATA);
		index = BusinessCardBinder_First(businessCardBinder);

		//6.3 ���� ������ ��ȣ�� ���� �ڵ��� ã�´�.
		parent = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_ROOT,
			(LPARAM)NULL); //�Ѹ� ��带 ã�´�.
		company = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD,
			(LPARAM)parent);//ù��° ��ȣ�� ã�´�.

		item.mask = TVIF_TEXT | TVIF_HANDLE;
		item.hItem = company;
		item.pszText = textCompany; //���ۿ� ��ȣ�� �־��ش�.
		item.cchTextMax = 32;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);

		while (strcmp(textCompany, index->company.name) != 0) {//���ۿ� ��ȣ�� ���Ѵ�.
			company = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT,
				(LPARAM)company);
			item.hItem = company;
			item.pszText = textCompany;
			item.cchTextMax = 32;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);
		}
		//6.4 �����ڵ��� ã�´�.
		  //6.4.1 ���� ��ġ�� ���Ѵ�.
		index_ = IndexBinder_Find(indexBinder, index->company.name);
		//6.4.2 �迭���� ������ ��ġ�� ���Ѵ�.
		i = Index_Find(index_, index);
		//6.4.3 �����ڵ��� ã�´�.
		name = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD,
			(LPARAM)company);
		while (j < i) {
			name = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT,
				(LPARAM)name);
			j++;
		}

		//6.5 ȸ�õ�� ��ȣ�� expand�Ѵ�.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)parent);
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)company);

		//6.6 �ش� �����ڵ��� ���õ� ���·� ó���Ѵ�.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET, (LPARAM)name);

		//6.7 ���Կ� ���� ��ġ�� ������ ����Ѵ�.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONALNAME), WM_SETTEXT, 0, (LPARAM)index->personal.name);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONALRANKNAME), WM_SETTEXT, 0,
			(LPARAM)index->personal.rankName);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONALTELEPHONENUMBER), WM_SETTEXT, 0,
			(LPARAM)index->personal.telephoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONALEMAILADDRESS), WM_SETTEXT, 0,
			(LPARAM)index->personal.emailAddress);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYNAME), WM_SETTEXT, 0, (LPARAM)index->company.name);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYADDRESS), WM_SETTEXT, 0,
			(LPARAM)index->company.address);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYTELEPHONENUMBER), WM_SETTEXT, 0,
			(LPARAM)index->company.telephoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYFAXNUMBER), WM_SETTEXT, 0,
			(LPARAM)index->company.faxNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYURL), WM_SETTEXT, 0, (LPARAM)index->company.url);
	}
	return TRUE;
}

BOOL BusinessCardBinderForm_OnPreviousButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BusinessCardBinder* businessCardBinder;
	BusinessCard* index;
	IndexBinder* indexBinder;
	Index* index_;
	HTREEITEM parent;
	HTREEITEM company;
	HTREEITEM name;
	TCHAR textCompany[32];
	Long i;
	Long j = 0;
	TVITEM item = { 0, };

	//7. ���� ��ư�� Ŭ������ ��
	if (HIWORD(wParam) == BN_CLICKED) {
		//7.1 ����ö���� ������ ����´�.
		indexBinder = (IndexBinder*)GetProp(hWnd, "indexBinder");

		//7.2 ����ö���� ���� ������ ���� ��ġ�� ���ٵд�.
		businessCardBinder = (BusinessCardBinder*)GetWindowLong(hWnd, GWL_USERDATA);
		index = BusinessCardBinder_Previous(businessCardBinder);

		//7.3 ���� ������ ��ȣ�� ���� �ڵ��� ã�´�.
		parent = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_ROOT,
			(LPARAM)NULL); //�Ѹ� ��带 ã�´�.
		company = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD,
			(LPARAM)parent);//ù��° ��ȣ�� ã�´�.

		item.mask = TVIF_TEXT | TVIF_HANDLE;
		item.hItem = company;
		item.pszText = textCompany; //���ۿ� ��ȣ�� �־��ش�.
		item.cchTextMax = 32;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);

		while (strcmp(textCompany, index->company.name) != 0) {//���ۿ� ��ȣ�� ���Ѵ�.
			company = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT,
				(LPARAM)company);
			item.hItem = company;
			item.pszText = textCompany;
			item.cchTextMax = 32;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);
		}
		//7.4 �����ڵ��� ã�´�.
		  //7.4.1 ���� ��ġ�� ���Ѵ�.
		index_ = IndexBinder_Find(indexBinder, index->company.name);
		//7.4.2 �迭���� ������ ��ġ�� ���Ѵ�.
		i = Index_Find(index_, index);
		//7.4.3 �����ڵ��� ã�´�.
		name = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD,
			(LPARAM)company);
		while (j < i) {
			name = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT,
				(LPARAM)name);
			j++;
		}

		//7.5 ȸ�õ�� ��ȣ�� expand�Ѵ�.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)parent);
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)company);

		//7.6 �ش� �����ڵ��� ���õ� ���·� ó���Ѵ�.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET, (LPARAM)name);

		//7.7 ���Կ� ���� ��ġ�� ������ ����Ѵ�.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONALNAME), WM_SETTEXT, 0, (LPARAM)index->personal.name);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONALRANKNAME), WM_SETTEXT, 0,
			(LPARAM)index->personal.rankName);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONALTELEPHONENUMBER), WM_SETTEXT, 0,
			(LPARAM)index->personal.telephoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONALEMAILADDRESS), WM_SETTEXT, 0,
			(LPARAM)index->personal.emailAddress);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYNAME), WM_SETTEXT, 0, (LPARAM)index->company.name);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYADDRESS), WM_SETTEXT, 0,
			(LPARAM)index->company.address);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYTELEPHONENUMBER), WM_SETTEXT, 0,
			(LPARAM)index->company.telephoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYFAXNUMBER), WM_SETTEXT, 0,
			(LPARAM)index->company.faxNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYURL), WM_SETTEXT, 0, (LPARAM)index->company.url);
	}
	return TRUE;
}

BOOL BusinessCardBinderForm_OnNextButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BusinessCardBinder* businessCardBinder;
	BusinessCard* index;
	IndexBinder* indexBinder;
	Index* index_;
	HTREEITEM parent;
	HTREEITEM company;
	HTREEITEM name;
	TCHAR textCompany[32];
	Long i;
	Long j = 0;
	TVITEM item= { 0, };

	//8. ���� ��ư�� Ŭ������ ��
	if (HIWORD(wParam) == BN_CLICKED) {
		//8.1 ����ö���� ������ ����´�.
		indexBinder = (IndexBinder*)GetProp(hWnd, "indexBinder");

		//8.2 ����ö���� ���� ������ ���� ��ġ�� ���ٵд�.
		businessCardBinder = (BusinessCardBinder*)GetWindowLong(hWnd, GWL_USERDATA);
		index = BusinessCardBinder_Next(businessCardBinder);

		//8.3 ���� ������ ��ȣ�� ���� �ڵ��� ã�´�.
		parent = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_ROOT,
			(LPARAM)NULL); //�Ѹ� ��带 ã�´�.
		company = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD,
			(LPARAM)parent);//ù��° ��ȣ�� ã�´�.
		
		item.mask = TVIF_TEXT | TVIF_HANDLE;
		item.hItem = company;
		item.pszText = textCompany; //���ۿ� ��ȣ�� �־��ش�.
		item.cchTextMax = 32;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);

		while (strcmp(textCompany, index->company.name) != 0) {//���ۿ� ��ȣ�� ���Ѵ�.
			company = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT,
				(LPARAM)company);
			item.hItem = company;
			item.pszText = textCompany;
			item.cchTextMax = 32;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);
		}
		//8.4 �����ڵ��� ã�´�.
		  //8.4.1 ���� ��ġ�� ���Ѵ�.
		  index_ = IndexBinder_Find(indexBinder, index->company.name);
		  //8.4.2 �迭���� ������ ��ġ�� ���Ѵ�.
		  i = Index_Find(index_, index);
		  //8.4.3 �����ڵ��� ã�´�.
		  name = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD,
			  (LPARAM)company);
		  while (j < i) {
			  name = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT,
				  (LPARAM)name);
			  j++;
		  }

        //8.5 ȸ�õ�� ��ȣ�� expand�Ѵ�.
		  SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)parent);
		  SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)company);


		//8.6 �ش� �����ڵ��� ���õ� ���·� ó���Ѵ�.
		  SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET, (LPARAM)name);


		//8.7 ���Կ� ���� ��ġ�� ������ ����Ѵ�.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONALNAME), WM_SETTEXT, 0, (LPARAM)index->personal.name);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONALRANKNAME), WM_SETTEXT, 0,
			(LPARAM)index->personal.rankName);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONALTELEPHONENUMBER), WM_SETTEXT, 0,
			(LPARAM)index->personal.telephoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONALEMAILADDRESS), WM_SETTEXT, 0,
			(LPARAM)index->personal.emailAddress);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYNAME), WM_SETTEXT, 0, (LPARAM)index->company.name);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYADDRESS), WM_SETTEXT, 0,
			(LPARAM)index->company.address);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYTELEPHONENUMBER), WM_SETTEXT, 0,
			(LPARAM)index->company.telephoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYFAXNUMBER), WM_SETTEXT, 0,
			(LPARAM)index->company.faxNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYURL), WM_SETTEXT, 0, (LPARAM)index->company.url);
	}
	return TRUE;
}

BOOL BusinessCardBinderForm_OnLastButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BusinessCardBinder* businessCardBinder;
	BusinessCard* index;
	IndexBinder* indexBinder;
	Index* index_;
	HTREEITEM parent;
	HTREEITEM company;
	HTREEITEM name;
	TCHAR textCompany[32];
	Long i;
	Long j = 0;
	TVITEM item = { 0, };

	//9. ���� ��ư�� Ŭ������ ��
	if (HIWORD(wParam) == BN_CLICKED) {
		//9.1 ����ö���� ������ ����´�.
		indexBinder = (IndexBinder*)GetProp(hWnd, "indexBinder");

		//9.2 ����ö���� ���� ������ ���� ��ġ�� ���ٵд�.
		businessCardBinder = (BusinessCardBinder*)GetWindowLong(hWnd, GWL_USERDATA);
		index = BusinessCardBinder_Last(businessCardBinder);

		//9.3 ���� ������ ��ȣ�� ���� �ڵ��� ã�´�.
		parent = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_ROOT,
			(LPARAM)NULL); //�Ѹ� ��带 ã�´�.
		company = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD,
			(LPARAM)parent);//ù��° ��ȣ�� ã�´�.

		item.mask = TVIF_TEXT | TVIF_HANDLE;
		item.hItem = company;
		item.pszText = textCompany; //���ۿ� ��ȣ�� �־��ش�.
		item.cchTextMax = 32;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);

		while (strcmp(textCompany, index->company.name) != 0) {//���ۿ� ��ȣ�� ���Ѵ�.
			company = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT,
				(LPARAM)company);
			item.hItem = company;
			item.pszText = textCompany;
			item.cchTextMax = 32;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);
		}
		//9.4 �����ڵ��� ã�´�.
		  //9.4.1 ���� ��ġ�� ���Ѵ�.
		index_ = IndexBinder_Find(indexBinder, index->company.name);
		//9.4.2 �迭���� ������ ��ġ�� ���Ѵ�.
		i = Index_Find(index_, index);
		//9.4.3 �����ڵ��� ã�´�.
		name = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD,
			(LPARAM)company);
		while (j < i) {
			name = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT,
				(LPARAM)name);
			j++;
		}

		//9.5 ȸ�õ�� ��ȣ�� expand�Ѵ�.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)parent);
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)company);

		//9.6 �ش� �����ڵ��� ���õ� ���·� ó���Ѵ�.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET, (LPARAM)name);

		//9.7 ���Կ� ���� ��ġ�� ������ ����Ѵ�.
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONALNAME), WM_SETTEXT, 0, (LPARAM)index->personal.name);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONALRANKNAME), WM_SETTEXT, 0,
			(LPARAM)index->personal.rankName);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONALTELEPHONENUMBER), WM_SETTEXT, 0,
			(LPARAM)index->personal.telephoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_PERSONALEMAILADDRESS), WM_SETTEXT, 0,
			(LPARAM)index->personal.emailAddress);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYNAME), WM_SETTEXT, 0, (LPARAM)index->company.name);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYADDRESS), WM_SETTEXT, 0,
			(LPARAM)index->company.address);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYTELEPHONENUMBER), WM_SETTEXT, 0,
			(LPARAM)index->company.telephoneNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYFAXNUMBER), WM_SETTEXT, 0,
			(LPARAM)index->company.faxNumber);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT_COMPANYURL), WM_SETTEXT, 0, (LPARAM)index->company.url);
	}
	return TRUE;
}


BOOL BusinessCardBinderForm_OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	BusinessCardBinder* businessCardBinder;
	IndexBinder* indexBinder;

	//10. �ݱ� ��ư�� Ŭ������ ��
	  //10.1 �����Ѵ�.(save)
	businessCardBinder = (BusinessCardBinder*)GetWindowLong(hWnd, GWL_USERDATA);
	indexBinder = (IndexBinder*)GetProp(hWnd, "indexBinder");

	if (businessCardBinder != NULL) {

		Save(businessCardBinder);

		//10.2 ����ö�� ���ش�.
		BusinessCardBinder_Destroy(businessCardBinder);
		free(businessCardBinder);
		businessCardBinder = NULL;
	}

	if (indexBinder != NULL) {
		//10.3 ����ö�� ���ش�
		IndexBinder_Destroy(indexBinder);
		free(indexBinder);
		indexBinder = NULL;
		RemoveProp(hWnd, "indexBinder");
	}

	//10.4 �����츦 �����Ѵ�.
	EndDialog(hWnd, 0);
	return TRUE;
}











