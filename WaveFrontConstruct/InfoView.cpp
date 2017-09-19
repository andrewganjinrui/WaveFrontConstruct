// InfoView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WaveFrontConstruct.h"
#include "InfoView.h"


// CInfoView

IMPLEMENT_DYNCREATE(CInfoView, CFormView)

CInfoView::CInfoView()
	: CFormView(CInfoView::IDD)
{

}

CInfoView::~CInfoView()
{
}

void CInfoView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CInfoView, CFormView)
END_MESSAGE_MAP()


// CInfoView ���

#ifdef _DEBUG
void CInfoView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CInfoView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CInfoView ��Ϣ�������

CWaveFrontConstructDoc* CInfoView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWaveFrontConstructDoc)));
	return (CWaveFrontConstructDoc*)m_pDocument;
}

void CInfoView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: �ڴ����ר�ô����/����û���
	CString str;
	str.Format(_T("%.7f"), 0.0);
	for (int i = 1000; i <= 1014; i++)
	{
		SetDlgItemText(i, str);
	}
	
}

void CInfoView::SetInfoZernike()
{
	CWaveFrontConstructDoc* pDoc = GetDocument();
	double* Data = pDoc->m_Zernike;
	
	CString str;
	for (int i = 0; i < 15; i++)
	{
		str.Format(_T("%.16f"), Data[i]);
		SetDlgItemText(i+1000, str);
	}
}
