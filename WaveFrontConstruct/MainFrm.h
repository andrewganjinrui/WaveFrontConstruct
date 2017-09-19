// MainFrm.h : CMainFrame ��Ľӿ�
//


#pragma once
#include "MyWaveFrontView.h"

class CMainFrame : public CFrameWnd 
{
	
protected: // �������л�����
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// ����
public:
	CSplitterWnd m_wndSplitter;
	CSplitterWnd m_wndSplitter2;

	CMyWaveFrontView* m_pListView;
	bool m_bListView;

// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // �ؼ���Ƕ���Ա
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
public:
	afx_msg void OnBeamoptlocate();
	afx_msg void OnWfconstruct();
	afx_msg void OnWfdisplay();
	afx_msg void OnWavefrontview();
	afx_msg void OnZernikeview();
};


