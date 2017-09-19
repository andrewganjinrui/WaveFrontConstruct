// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "WaveFrontConstruct.h"

#include "MainFrm.h"
#include "InfoView.h"
#include "3DplayView.h"
#include "WaveFrontConstructView.h"
#include "MyWaveFrontView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_BEAMOPTLOCATE, &CMainFrame::OnBeamoptlocate)
	ON_COMMAND(ID_WFCONSTRUCT, &CMainFrame::OnWfconstruct)
	ON_COMMAND(ID_WFDISPLAY, &CMainFrame::OnWfdisplay)
	ON_COMMAND(ID_WAVEFRONTVIEW, &CMainFrame::OnWavefrontview)
	ON_COMMAND(ID_ZERNIKEVIEW, &CMainFrame::OnZernikeview)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
	m_pListView = NULL;
	m_bListView = false;
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}

	// TODO: 如果不需要可停靠工具栏，则删除这三行
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return TRUE;
}


// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame 消息处理程序




BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: 在此添加专用代码和/或调用基类
	//把文档划分为左右两个客户区
	if(!m_wndSplitter.CreateStatic(this, 1, 2, WS_CHILD|WS_VISIBLE))
	{
		TRACE("Failed to CreateStaticSplitter\n"); 
		return FALSE;
	}
	//在左边客户区新建一个命令视图窗口
	if(!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CWaveFrontConstructView), CSize(800,600), pContext))
	{
		TRACE("Failed to create command view pane\n"); 
		return FALSE;
	}
	//第二次分割上下两个窗口
	m_wndSplitter2.CreateStatic(&m_wndSplitter, 2, 1, WS_CHILD|WS_VISIBLE, m_wndSplitter.IdFromRowCol(0, 1));
	m_wndSplitter2.CreateView( 0, 0, RUNTIME_CLASS(CInfoView), CSize(0,0), pContext);
	m_wndSplitter2.CreateView( 1, 0, RUNTIME_CLASS(C3DplayView), CSize(0,0), pContext);
	m_wndSplitter2.SetRowInfo( 0, 200, 50 );

	return TRUE; //重要
}

void CMainFrame::OnBeamoptlocate()
{
	// TODO: 在此添加命令处理程序代码
	CWaveFrontConstructView *pView  = (CWaveFrontConstructView *)m_wndSplitter.GetPane(0,0);
	pView->OnLocate();
}

void CMainFrame::OnWfconstruct()
{
	// TODO: 在此添加命令处理程序代码
	CWaveFrontConstructView *pView  = (CWaveFrontConstructView *)m_wndSplitter.GetPane(0,0);
	pView->OnWavefrontconstruct();
}

void CMainFrame::OnWfdisplay()
{
	// TODO: 在此添加命令处理程序代码
	CWaveFrontConstructView *pViewWaveFront  = (CWaveFrontConstructView *)m_wndSplitter.GetPane(0,0);
	pViewWaveFront->DrawGrid();
}

void CMainFrame::OnWavefrontview()
{
	// TODO: 在此添加命令处理程序代码
	m_bListView = true;

	CRuntimeClass* pViewClass = RUNTIME_CLASS(CMyWaveFrontView);
	CWnd* pActiveView = m_wndSplitter2.GetPane(0,0);

	if(pActiveView->IsKindOf(pViewClass))
		return ;

	CWaveFrontConstructDoc* pDoc = ((CWaveFrontConstructApp*)AfxGetApp())->m_pDoc;
	if (pDoc == NULL)
	{
		return;
	}

	BOOL bSaveAutoDelete = pDoc->m_bAutoDelete;
	pDoc->m_bAutoDelete = FALSE;

	CRect rcView;
	pActiveView->GetWindowRect( &rcView );

	pActiveView->DestroyWindow();

	pDoc->m_bAutoDelete = bSaveAutoDelete;

	CCreateContext context;
	context.m_pNewDocTemplate = NULL;
	context.m_pLastView = NULL;
	context.m_pCurrentFrame = NULL;
	context.m_pNewViewClass = pViewClass;
	context.m_pCurrentDoc = pDoc;
 	m_wndSplitter2.CreateView(0, 0, pViewClass, CSize(rcView.Width(), rcView.Height()), &context);
	
	CMyWaveFrontView* pNewView = (CMyWaveFrontView*)m_wndSplitter2.GetPane(0,0);
	pNewView->OnInitialUpdate();
	pNewView->DisplayWaveFront();
}

void CMainFrame::OnZernikeview()
{
	// TODO: 在此添加命令处理程序代码
	m_bListView = false;

	CRuntimeClass* pViewClass = RUNTIME_CLASS(CInfoView);
	CWnd* pActiveView = m_wndSplitter2.GetPane(0,0);

	if(pActiveView->IsKindOf(pViewClass))
		return ;

	CWaveFrontConstructDoc* pDoc = ((CWaveFrontConstructApp*)AfxGetApp())->m_pDoc;

	BOOL bSaveAutoDelete = pDoc->m_bAutoDelete;
	pDoc->m_bAutoDelete = FALSE;

	CRect rcView;
	pActiveView->GetWindowRect( &rcView );

	pActiveView->DestroyWindow();

	pDoc->m_bAutoDelete = bSaveAutoDelete;

	CCreateContext context;
	context.m_pNewDocTemplate = NULL;
	context.m_pLastView = NULL;
	context.m_pCurrentFrame = NULL;
	context.m_pNewViewClass = pViewClass;
	context.m_pCurrentDoc = pDoc;
	m_wndSplitter2.CreateView(0, 0, pViewClass, CSize(rcView.Width(), rcView.Height()), &context);
	
	CInfoView* pNewView = (CInfoView*)m_wndSplitter2.GetPane(0,0);
	pNewView->SetInfoZernike();
}
