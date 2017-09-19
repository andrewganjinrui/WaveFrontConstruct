// MyWaveFrontView.cpp : 实现文件
//

#include "stdafx.h"
#include "WaveFrontConstruct.h"
#include "MainFrm.h"
#include "MyWaveFrontView.h"


// CMyWaveFrontView

IMPLEMENT_DYNCREATE(CMyWaveFrontView, CListView)

CMyWaveFrontView::CMyWaveFrontView()
{

}

CMyWaveFrontView::~CMyWaveFrontView()
{
}

BEGIN_MESSAGE_MAP(CMyWaveFrontView, CListView)
END_MESSAGE_MAP()


// CMyWaveFrontView 诊断

#ifdef _DEBUG
void CMyWaveFrontView::AssertValid() const
{
	CListView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyWaveFrontView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMyWaveFrontView 消息处理程序

void CMyWaveFrontView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	((CMainFrame*)AfxGetMainWnd())->m_pListView = this;
	InitializeListView();
}

void CMyWaveFrontView::InitializeListView()
{
	CWaveFrontConstructDoc* pDoc = ((CWaveFrontConstructApp*)AfxGetApp())->m_pDoc;

	CListCtrl& theListCtrl = GetListCtrl();
	theListCtrl.SetRedraw(FALSE);

	DWORD dwExStyle = LVS_EX_FULLROWSELECT;
	theListCtrl.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, (WPARAM)dwExStyle, (LPARAM)dwExStyle);
	COLORREF crYellow = RGB(240, 240, 2); //light yellow
	theListCtrl.SetBkColor(crYellow);
	theListCtrl.SetTextBkColor(crYellow);
	
	CString str;
	for (int i = 0; i <= 12; i++)
	{
		str.Format(_T("%d"), i);
		theListCtrl.InsertColumn(i, str);
	}
	for (int k = 0; k < 8; k++)
	{
		str.Format(_T("%d"), k+1);
		theListCtrl.InsertItem(k, str);
		for (int j = 1; j <= 12; j++)
		{
			str.Format(_T("%.17f"), pDoc->m_WaveFront[k][j-1]);
			theListCtrl.SetItemText(k, j, str);
		}
	}
	for(UINT i = 0; i<= 12; i++)
		theListCtrl.SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);
}

BOOL CMyWaveFrontView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此添加专用代码和/或调用基类
	cs.style |= LVS_SHOWSELALWAYS | LVS_REPORT;

	return CListView::PreCreateWindow(cs);
}

void CMyWaveFrontView::DisplayWaveFront()
{
	//The functions GetDocument() and GetParentFrame()->GetActiveDocument() won't always work 
	//here, so we will use the app's doc pointer:
	CWaveFrontConstructDoc* pDoc = ((CWaveFrontConstructApp*)AfxGetApp())->m_pDoc;

	CListCtrl& theListCtrl = GetListCtrl();
	theListCtrl.SetRedraw(FALSE);
	theListCtrl.DeleteAllItems();
	
	CString str;
	for (int k = 0; k < 8; k++)
	{
		str.Format(_T("%d"), k+1);
		theListCtrl.InsertItem(k, str);
		for (int j = 1; j <= 12; j++)
		{
			str.Format(_T("%.16f"), pDoc->m_WaveFront[k][j-1]);
			theListCtrl.SetItemText(k, j, str);
		}
	} 

	theListCtrl.SetRedraw(TRUE);
	theListCtrl.Invalidate();
}
