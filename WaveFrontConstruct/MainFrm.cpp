// MainFrm.cpp : CMainFrame ���ʵ��
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
	ID_SEPARATOR,           // ״̬��ָʾ��
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
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
		TRACE0("δ�ܴ���������\n");
		return -1;      // δ�ܴ���
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}

	// TODO: �������Ҫ��ͣ������������ɾ��������
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return TRUE;
}


// CMainFrame ���

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


// CMainFrame ��Ϣ�������




BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: �ڴ����ר�ô����/����û���
	//���ĵ�����Ϊ���������ͻ���
	if(!m_wndSplitter.CreateStatic(this, 1, 2, WS_CHILD|WS_VISIBLE))
	{
		TRACE("Failed to CreateStaticSplitter\n"); 
		return FALSE;
	}
	//����߿ͻ����½�һ��������ͼ����
	if(!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CWaveFrontConstructView), CSize(800,600), pContext))
	{
		TRACE("Failed to create command view pane\n"); 
		return FALSE;
	}
	//�ڶ��ηָ�������������
	m_wndSplitter2.CreateStatic(&m_wndSplitter, 2, 1, WS_CHILD|WS_VISIBLE, m_wndSplitter.IdFromRowCol(0, 1));
	m_wndSplitter2.CreateView( 0, 0, RUNTIME_CLASS(CInfoView), CSize(0,0), pContext);
	m_wndSplitter2.CreateView( 1, 0, RUNTIME_CLASS(C3DplayView), CSize(0,0), pContext);
	m_wndSplitter2.SetRowInfo( 0, 200, 50 );

	return TRUE; //��Ҫ
}

void CMainFrame::OnBeamoptlocate()
{
	// TODO: �ڴ���������������
	CWaveFrontConstructView *pView  = (CWaveFrontConstructView *)m_wndSplitter.GetPane(0,0);
	pView->OnLocate();
}

void CMainFrame::OnWfconstruct()
{
	// TODO: �ڴ���������������
	CWaveFrontConstructView *pView  = (CWaveFrontConstructView *)m_wndSplitter.GetPane(0,0);
	pView->OnWavefrontconstruct();
}

void CMainFrame::OnWfdisplay()
{
	// TODO: �ڴ���������������
	CWaveFrontConstructView *pViewWaveFront  = (CWaveFrontConstructView *)m_wndSplitter.GetPane(0,0);
	pViewWaveFront->DrawGrid();
}

void CMainFrame::OnWavefrontview()
{
	// TODO: �ڴ���������������
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
	// TODO: �ڴ���������������
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
