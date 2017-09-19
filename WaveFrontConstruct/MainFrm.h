// MainFrm.h : CMainFrame 类的接口
//


#pragma once
#include "MyWaveFrontView.h"

class CMainFrame : public CFrameWnd 
{
	
protected: // 仅从序列化创建
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 属性
public:
	CSplitterWnd m_wndSplitter;
	CSplitterWnd m_wndSplitter2;

	CMyWaveFrontView* m_pListView;
	bool m_bListView;

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// 生成的消息映射函数
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


