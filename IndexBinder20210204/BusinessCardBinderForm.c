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
#include<Commctrl.h> // TVM의 헤더파일 추가
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

	//1. 윈도우가 생성될때
	  //1.1 명함철을 만든다.(create)
	businessCardBinder = (BusinessCardBinder*)malloc(sizeof(BusinessCardBinder));
	BusinessCardBinder_Create(businessCardBinder);
	SetWindowLong(hWnd, GWL_USERDATA, (LONG)businessCardBinder);

	//1.2 색인철을 만든다.(create)
	indexBinder = (IndexBinder*)malloc(sizeof(IndexBinder));
	BinaryTree_Create(indexBinder);
	SetProp(hWnd, "indexBinder", indexBinder);

	//1.6 SysTreeView32에  회사들 카테고리를 만든다.
	insertStruct.hParent = TVI_ROOT;
	insertStruct.hInsertAfter = TVI_FIRST;
	insertStruct.item.mask = TVIF_TEXT;
	insertStruct.item.pszText = "회사들";
	parent = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_INSERTITEM, 0, (LPARAM)&insertStruct);

	//1.3 명함철에 적재한다.(Load)
	count=Load(businessCardBinder);
	if (count > 0){
		
		//1.4 명함의 처음부터 끝까지 반복한다.
		index = BusinessCardBinder_First(businessCardBinder);
		while (index != previous) {
			//1.6.1 색인철에 끼운다.(put)
			IndexBinder_Put(indexBinder, index);
			previous = index;
			index = BusinessCardBinder_Next(businessCardBinder);
		}
		//1.5 색인리스트를 만든다.(MakeList)
		MakeList(indexBinder, &indexes, &count);

		//1.7 색인의 개수만큼 반복한다.
		i = 0;
		while (i < count) {
			//1.7.1 회사들 하위에 상호명을 추가한다.
			insertStruct.hParent = parent;
			insertStruct.hInsertAfter = TVI_LAST;
			insertStruct.item.pszText = indexes[i].companyName;
			company = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_INSERTITEM, 0, (LPARAM)&insertStruct);

			//1.7.2 명함의 개수만큼 반복한다.
			j = 0;
			while (j < indexes[i].businessCards.length) {
				//1.7.2.1 상호명 하위에 개인성명을 추가한다.
				index = Index_GetAt(indexes+i, j);
				insertStruct.hParent = company;
				insertStruct.hInsertAfter = TVI_LAST;
				insertStruct.item.pszText = index->personal.name;
				SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_INSERTITEM, 0, (LPARAM)&insertStruct);
				j++;
			}
			i++;
		}
		//1.8 명함철의 현재 위치를 첫번째 위치로 이동시킨다.
		index = BusinessCardBinder_First(businessCardBinder);
		if (index != NULL) {
			//1.8.1 명함이 있으면
			//1.8.1.1  색인철에서 명함의 상호명으로 색인을 찾는다.(색인핸들을 구한다.)
			company = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD,
				(LPARAM)parent); //첫번째 상호핸들
			item.mask = TVIF_TEXT | TVIF_HANDLE;
			item.hItem = company;
			item.pszText = textCompany; //첫번째 상호핸들을 buffer에 넣어준다.
			item.cchTextMax = 32;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);

			while (strcmp(index->company.name, textCompany) != 0) { // 상호핸들과 현재 명함위치의 상호가 같은지 확인.
				company = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT,
					(LPARAM)company); // 그 다음 상호핸들 찾기.
				item.hItem = company;
				item.pszText = textCompany;//상호핸들을 buffer에 넣어준다.
				item.cchTextMax = 32;
				SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);
		    }

			//1.8.1.2 개인성명 핸들을 구한다.
			  // 색인 위치를 구한다.
			  index_ = IndexBinder_Find(indexBinder, index->company.name);

			  // 개인성명 배열위치를 구한다.
			  i = Index_Find(index_, index);

			  // 개인성명 핸들을 구한다.
			  person = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD,
				  (LPARAM)company);
			  j = 0;
			  while (j < i) {
				  person = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT,
					  (LPARAM)person);
				  j++;
			  }

			 
			//1.8.1.3 트리뷰컨트롤에서 명함을 선택한다.

			//회사들(parent) expand해주기.
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)parent);
			//회사(company) expand해주기.
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)company);
			//해당 번째의 개인성명을 선택된 상태로 처리한다.
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET, (LPARAM)person);


			//1.8.1.4. 명함에 현재 위치의 명함을 출력한다.
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
		//2.2 색인 핸들을 구했으면, indexes 배열 내 색인 위치를 구한다.
		i = 0;
		while (strcmp(indexes[i].companyName, companyName) != 0) {
			i++;
		}
		//2.3 개인성명 배열 위치를 구한다.
		j = 0;
		index = Index_GetAt(indexes + i, j);
		while (strcmp(index->personal.name, name) == 0) {
			j++;
			index = Index_GetAt(indexes + i, j);
		}
#endif
	}

	//2.8 indexes를 할당 해제한다.
	if (indexes != NULL) {
		free(indexes);
		indexes = NULL;
	}

	return TRUE;
}

BOOL BusinessCardBinderForm_OnPutButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	//2. 꽂기 버튼을 클릭했을때
	if (HIWORD(wParam) == BN_CLICKED) {
		//2.1 꽂기 윈도우를 출력한다.
		DialogBox((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_PUTTINGFORM), NULL,
			PuttingFormProc);
	}
	return TRUE;
}

BOOL BusinessCardBinderForm_OnFindButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	//3. 찾기 버튼을 클릭했을때
	if (HIWORD(wParam) == BN_CLICKED) {
		//3.1 찾기 윈도우를 출력한다.
		DialogBox((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_FINDINGFORM), NULL,
			FindingFormProc);
	}
	return TRUE;

}

BOOL BusinessCardBinderForm_OnPutOutButtonClicked(HWND hWnd, WPARAM wParam, LPARAM lParam) {
	//4. 뽑기 버튼을 클릭했을때
	if (HIWORD(wParam) == BN_CLICKED) {
		//4.1 뽑기 윈도우를 출력한다.
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

	//5. 정리하기 버튼을 클릭했을 때
	if (HIWORD(wParam) == BN_CLICKED) {
		//5.1 색인철에서 정리한다.
		
		indexBinder = (IndexBinder*)GetProp(hWnd, "indexBinder");
		IndexBinder_Arrange(indexBinder);

		//5.2 트리뷰컨트롤의 항목들을 초기화한다.
		parent = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_ROOT,
			(LPARAM)NULL);
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_DELETEITEM, (WPARAM)0, (LPARAM)parent);

		//5.3 색인리스트를 구한다.
		MakeList(indexBinder, &indexes, &count);

		//5.4 트리뷰컨트롤에 회사들 항목을 추가한다.
		insertStruct.hParent = TVI_ROOT;
		insertStruct.hInsertAfter = TVI_FIRST;
		insertStruct.item.mask = TVIF_TEXT;
		insertStruct.item.pszText = "회사들";
		parent = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_INSERTITEM, 0, (LPARAM)&insertStruct);

		//5.5 색인의 개수만큼 반복한다.
		while (i < count) {
			//5.5.1 회사들 하위에 상호명을 추가한다.
			insertStruct.hParent = parent;
			insertStruct.hInsertAfter = TVI_LAST;
			insertStruct.item.pszText = indexes[i].companyName;
			company = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_INSERTITEM, 0, (LPARAM)&insertStruct);

			//5.5.2 명함의 개수만큼 반복한다.
			j = 0;
			while (j < indexes[i].length) {
				//5.5.2.1 상호명 하위에 개인성명을 추가한다.
				insertStruct.hParent = company;
				insertStruct.hInsertAfter = TVI_LAST;
				index = Index_GetAt(indexes + i, j);
				insertStruct.item.pszText = index->personal.name;
				SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_INSERTITEM, 0, (LPARAM)&insertStruct);
				j++;
			}
			i++;
		}

		//5.6 명함의 현재위치를 첫번째 명함위치로 옮긴다.
		businessCardBinder = (BusinessCardBinder*)GetWindowLong(hWnd, GWL_USERDATA);
		index =  BusinessCardBinder_First(businessCardBinder);

		//5.7 트리뷰 상 현재 위치의 상호핸들을 찾는다.
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

		//5.8 개인성명 핸들을 구한다.
		index_ = IndexBinder_Find(indexBinder, index->company.name);
		i = Index_Find(index_, index);

		name = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD,
			(LPARAM)company);
		while (j < i) {
			name = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT,
				(LPARAM)name);
			j++;
		}

		//5.7 회사들을 펼친다.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND,
			(LPARAM)parent);
		//5.8  해당 상호핸들을 펼친다.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND,
			(LPARAM)company);

		//5.9 해당 개인핸들을 선택된 상태로 처리한다.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET,
			(LPARAM)name);

		//6.0 명함철의 현재 위치의 명함을 출력한다.
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

	//2. 트리뷰컨트롤 상의 성명핸들 또는 상호핸들을 더블클릭했을때
	if (((LPNMHDR)lParam)->code == NM_DBLCLK) {
		//2.1 클릭된 핸들이 성명핸들인지 상호핸들인지 확인한다.
		handle= SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CARET,
			(LPARAM)NULL); // 클릭한 핸들

		parentHandle = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_PARENT,
			(LPARAM)handle); // 클릭한 핸들의 상위 핸들

		parent = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_ROOT,
			(LPARAM)NULL); //루트 핸들

		//2.2 클릭횐 핸들이 개인핸들일 경우,
		if (parentHandle != parent) {
			//2.2.1 클릭된 개인성명 핸들을 구한다.
			name = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CARET,
				(LPARAM)NULL);

			//2.2.2 개인성명 부모핸들(상호핸들)을 구한다.
			company = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_PARENT,
				(LPARAM)name);

			//2.2.3 상호핸들의 text를 구한다.
			item.mask = TVIF_HANDLE | TVIF_TEXT;
			item.hItem = company;
			item.pszText = textCompany;
			item.cchTextMax = 32;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);


			//2.2.4 해당 상호핸들에서 해당 개인핸들이 몇번째인지 확인한다.(첨자 확인)
			testName = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD,
				(LPARAM)company);

			while (testName != NULL && testName != name) {
				testName = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT,
					(LPARAM)testName);
				i++;
			}

			//2.2.5 색인철에서 상호 위치를 구한다.
			indexBinder = (IndexBinder*)GetProp(hWnd, "indexBinder");
			index_ = IndexBinder_Find(indexBinder, textCompany);

			//2.2.6 해당 상호 내 명함의 위치를 찾는다.
			index = Index_GetAt(index_, i);

			//2.2.7 해당 위치의 명함을 현재 명함으로 설정한다.
			businessCardBinder = (BusinessCardBinder*)GetWindowLong(hWnd, GWL_USERDATA);
			index = Move(businessCardBinder, index);


			//2.2.8 현재 명함을 명함에 출력한다.
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

	//6. 처음 버튼을 클릭했을 때
	if (HIWORD(wParam) == BN_CLICKED) {
		//6.1 색인철에서 색인을 갖고온다.
		indexBinder = (IndexBinder*)GetProp(hWnd, "indexBinder");

		//6.2 명함철에서 처음 명함을 현재 위치로 갖다둔다.
		businessCardBinder = (BusinessCardBinder*)GetWindowLong(hWnd, GWL_USERDATA);
		index = BusinessCardBinder_First(businessCardBinder);

		//6.3 현재 명함의 상호로 색인 핸들을 찾는다.
		parent = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_ROOT,
			(LPARAM)NULL); //뿌리 노드를 찾는다.
		company = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD,
			(LPARAM)parent);//첫번째 상호를 찾는다.

		item.mask = TVIF_TEXT | TVIF_HANDLE;
		item.hItem = company;
		item.pszText = textCompany; //버퍼에 상호를 넣어준다.
		item.cchTextMax = 32;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);

		while (strcmp(textCompany, index->company.name) != 0) {//버퍼와 상호를 비교한다.
			company = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT,
				(LPARAM)company);
			item.hItem = company;
			item.pszText = textCompany;
			item.cchTextMax = 32;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);
		}
		//6.4 명함핸들을 찾는다.
		  //6.4.1 색인 위치를 구한다.
		index_ = IndexBinder_Find(indexBinder, index->company.name);
		//6.4.2 배열에서 개인의 위치를 구한다.
		i = Index_Find(index_, index);
		//6.4.3 개인핸들을 찾는다.
		name = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD,
			(LPARAM)company);
		while (j < i) {
			name = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT,
				(LPARAM)name);
			j++;
		}

		//6.5 회시들과 상호를 expand한다.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)parent);
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)company);

		//6.6 해당 개인핸들을 선택된 상태로 처리한다.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET, (LPARAM)name);

		//6.7 명함에 현재 위치의 명함을 출력한다.
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

	//7. 이전 버튼을 클릭했을 때
	if (HIWORD(wParam) == BN_CLICKED) {
		//7.1 색인철에서 색인을 갖고온다.
		indexBinder = (IndexBinder*)GetProp(hWnd, "indexBinder");

		//7.2 명함철에서 이전 명함을 현재 위치로 갖다둔다.
		businessCardBinder = (BusinessCardBinder*)GetWindowLong(hWnd, GWL_USERDATA);
		index = BusinessCardBinder_Previous(businessCardBinder);

		//7.3 현재 명함의 상호로 색인 핸들을 찾는다.
		parent = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_ROOT,
			(LPARAM)NULL); //뿌리 노드를 찾는다.
		company = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD,
			(LPARAM)parent);//첫번째 상호를 찾는다.

		item.mask = TVIF_TEXT | TVIF_HANDLE;
		item.hItem = company;
		item.pszText = textCompany; //버퍼에 상호를 넣어준다.
		item.cchTextMax = 32;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);

		while (strcmp(textCompany, index->company.name) != 0) {//버퍼와 상호를 비교한다.
			company = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT,
				(LPARAM)company);
			item.hItem = company;
			item.pszText = textCompany;
			item.cchTextMax = 32;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);
		}
		//7.4 명함핸들을 찾는다.
		  //7.4.1 색인 위치를 구한다.
		index_ = IndexBinder_Find(indexBinder, index->company.name);
		//7.4.2 배열에서 개인의 위치를 구한다.
		i = Index_Find(index_, index);
		//7.4.3 개인핸들을 찾는다.
		name = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD,
			(LPARAM)company);
		while (j < i) {
			name = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT,
				(LPARAM)name);
			j++;
		}

		//7.5 회시들과 상호를 expand한다.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)parent);
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)company);

		//7.6 해당 개인핸들을 선택된 상태로 처리한다.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET, (LPARAM)name);

		//7.7 명함에 현재 위치의 명함을 출력한다.
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

	//8. 다음 버튼을 클릭했을 때
	if (HIWORD(wParam) == BN_CLICKED) {
		//8.1 색인철에서 색인을 갖고온다.
		indexBinder = (IndexBinder*)GetProp(hWnd, "indexBinder");

		//8.2 명함철에서 다음 명함을 현재 위치로 갖다둔다.
		businessCardBinder = (BusinessCardBinder*)GetWindowLong(hWnd, GWL_USERDATA);
		index = BusinessCardBinder_Next(businessCardBinder);

		//8.3 현재 명함의 상호로 색인 핸들을 찾는다.
		parent = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_ROOT,
			(LPARAM)NULL); //뿌리 노드를 찾는다.
		company = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD,
			(LPARAM)parent);//첫번째 상호를 찾는다.
		
		item.mask = TVIF_TEXT | TVIF_HANDLE;
		item.hItem = company;
		item.pszText = textCompany; //버퍼에 상호를 넣어준다.
		item.cchTextMax = 32;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);

		while (strcmp(textCompany, index->company.name) != 0) {//버퍼와 상호를 비교한다.
			company = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT,
				(LPARAM)company);
			item.hItem = company;
			item.pszText = textCompany;
			item.cchTextMax = 32;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);
		}
		//8.4 명함핸들을 찾는다.
		  //8.4.1 색인 위치를 구한다.
		  index_ = IndexBinder_Find(indexBinder, index->company.name);
		  //8.4.2 배열에서 개인의 위치를 구한다.
		  i = Index_Find(index_, index);
		  //8.4.3 개인핸들을 찾는다.
		  name = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD,
			  (LPARAM)company);
		  while (j < i) {
			  name = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT,
				  (LPARAM)name);
			  j++;
		  }

        //8.5 회시들과 상호를 expand한다.
		  SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)parent);
		  SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)company);


		//8.6 해당 개인핸들을 선택된 상태로 처리한다.
		  SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET, (LPARAM)name);


		//8.7 명함에 현재 위치의 명함을 출력한다.
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

	//9. 다음 버튼을 클릭했을 때
	if (HIWORD(wParam) == BN_CLICKED) {
		//9.1 색인철에서 색인을 갖고온다.
		indexBinder = (IndexBinder*)GetProp(hWnd, "indexBinder");

		//9.2 명함철에서 다음 명함을 현재 위치로 갖다둔다.
		businessCardBinder = (BusinessCardBinder*)GetWindowLong(hWnd, GWL_USERDATA);
		index = BusinessCardBinder_Last(businessCardBinder);

		//9.3 현재 명함의 상호로 색인 핸들을 찾는다.
		parent = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_ROOT,
			(LPARAM)NULL); //뿌리 노드를 찾는다.
		company = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD,
			(LPARAM)parent);//첫번째 상호를 찾는다.

		item.mask = TVIF_TEXT | TVIF_HANDLE;
		item.hItem = company;
		item.pszText = textCompany; //버퍼에 상호를 넣어준다.
		item.cchTextMax = 32;
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);

		while (strcmp(textCompany, index->company.name) != 0) {//버퍼와 상호를 비교한다.
			company = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT,
				(LPARAM)company);
			item.hItem = company;
			item.pszText = textCompany;
			item.cchTextMax = 32;
			SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETITEM, 0, (LPARAM)&item);
		}
		//9.4 명함핸들을 찾는다.
		  //9.4.1 색인 위치를 구한다.
		index_ = IndexBinder_Find(indexBinder, index->company.name);
		//9.4.2 배열에서 개인의 위치를 구한다.
		i = Index_Find(index_, index);
		//9.4.3 개인핸들을 찾는다.
		name = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_CHILD,
			(LPARAM)company);
		while (j < i) {
			name = SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_GETNEXTITEM, (WPARAM)TVGN_NEXT,
				(LPARAM)name);
			j++;
		}

		//9.5 회시들과 상호를 expand한다.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)parent);
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_EXPAND, (WPARAM)TVE_EXPAND, (LPARAM)company);

		//9.6 해당 개인핸들을 선택된 상태로 처리한다.
		SendMessage(GetDlgItem(hWnd, IDC_TREE_INDEXES), TVM_SELECTITEM, (WPARAM)TVGN_CARET, (LPARAM)name);

		//9.7 명함에 현재 위치의 명함을 출력한다.
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

	//10. 닫기 버튼을 클릭했을 때
	  //10.1 저장한다.(save)
	businessCardBinder = (BusinessCardBinder*)GetWindowLong(hWnd, GWL_USERDATA);
	indexBinder = (IndexBinder*)GetProp(hWnd, "indexBinder");

	if (businessCardBinder != NULL) {

		Save(businessCardBinder);

		//10.2 명함철을 없앤다.
		BusinessCardBinder_Destroy(businessCardBinder);
		free(businessCardBinder);
		businessCardBinder = NULL;
	}

	if (indexBinder != NULL) {
		//10.3 색인철을 없앤다
		IndexBinder_Destroy(indexBinder);
		free(indexBinder);
		indexBinder = NULL;
		RemoveProp(hWnd, "indexBinder");
	}

	//10.4 윈도우를 종료한다.
	EndDialog(hWnd, 0);
	return TRUE;
}











