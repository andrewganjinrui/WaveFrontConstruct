// WaveFrontConstructDoc.cpp : CWaveFrontConstructDoc ���ʵ��
//

#include "stdafx.h"
#include "WaveFrontConstruct.h"

#include "WaveFrontConstructDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWaveFrontConstructDoc

IMPLEMENT_DYNCREATE(CWaveFrontConstructDoc, CDocument)

BEGIN_MESSAGE_MAP(CWaveFrontConstructDoc, CDocument)
END_MESSAGE_MAP()


// CWaveFrontConstructDoc ����/����

CWaveFrontConstructDoc::CWaveFrontConstructDoc()
{
	// TODO: �ڴ����һ���Թ������
	m_pImage = NULL;
	m_SizeDoc = CSize(100, 100);
	m_ParaDocVect.clear();

	for (int i = 0; i < 15; i++)
	{
		m_Zernike[i] = 0;
	}

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 12; j ++)
		{
			m_GridX[i][j] = 0;
			m_GridY[i][j] = 0;
		}
	}
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			m_WaveFront[i][j] = 0.0;
		}
	}
}

CWaveFrontConstructDoc::~CWaveFrontConstructDoc()
{
	FreeImage();
}

BOOL CWaveFrontConstructDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CWaveFrontConstructDoc ���л�

void CWaveFrontConstructDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// CWaveFrontConstructDoc ���

#ifdef _DEBUG
void CWaveFrontConstructDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CWaveFrontConstructDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CWaveFrontConstructDoc ����

BOOL CWaveFrontConstructDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  �ڴ������ר�õĴ�������
	((CWaveFrontConstructApp*)AfxGetApp())->m_pDoc = this;

	IplImage* pImage = cvLoadImage(lpszPathName);
	cvFlip(pImage);
	UpdateImage(pImage);

	return TRUE;
}

void CWaveFrontConstructDoc::UpdateImage(IplImage* pImage)
{
	FreeImage();
	m_pImage = pImage;

	int lHeight = pImage->height;
	int lWidth = pImage->width;
	m_SizeDoc = CSize(lWidth, lHeight);
}

void CWaveFrontConstructDoc::SetZernikeData(CvMat* Mat, ParameterVect& ParaDocVect)
{
	for (int i = 0; i < 15; i++)
	{
		m_Zernike[i] = cvmGet(Mat, i, 0);
	}
	m_ParaDocVect = ParaDocVect;

//	SaveRequiredData();
}

void CWaveFrontConstructDoc::UpdateWaveFront()
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			m_WaveFront[i][j] = Get_Z(m_GridX[i][j], m_GridY[i][j]);
		}
	}
}

void CWaveFrontConstructDoc::SaveRequiredData()
{
	CStdioFile file;
	CFileException ex;
	if (!file.Open(_T("D:\\Data.txt"), CFile::modeWrite|CFile::modeCreate, &ex))
	{
		file.Close();
	}

	CString str;
	str.Format(_T("Zernikeϵ����15�ף�:\n\n"));
	file.WriteString(str);
	for (int i = 0; i < 15; i++)
	{
		str.Format(_T("%d�ף�%.16f\n"), i, m_Zernike[i]);
		file.WriteString(str);
	}

	double x, y, z;
	str.Format(_T("\n��ǰ����:\n\n"));
	file.WriteString(str);
	for (int i = 0; i < m_ParaDocVect.size(); i++)
	{
		x = m_ParaDocVect[i].CenterOpt.x;
		y = m_ParaDocVect[i].CenterOpt.y;
		z = Get_Z(x, y);
		str.Format(_T("�ӿ׾�%d:\n���ģ�(%.16f, %.16f)\n��ǰ��%.16f\n"), i, x, y, z);
		file.WriteString(str);
	}
	
	 file.Close();
}

float CWaveFrontConstructDoc::Get_Z(float x, float y)
{
	float z = 0.0;
	z = 2*x*y*m_Zernike[3] +
		(-1 + 2*x*x + 2*y*y) * m_Zernike[4] + (y*y-x*x) * m_Zernike[5] + (-pow(x,3) + 3*x*y*y) * m_Zernike[6] +
		(-2*x+3*x*x*x+3*x*y*y) * m_Zernike[7] + (-2*y+3*x*x*y+3*y*y*y) * m_Zernike[8] +
		(pow(y,3)-3*x*x*y) * m_Zernike[9] + (-4*y*pow(x,3)+4*x*pow(y,3)) * m_Zernike[10] +
		(-6*x*y+8*y*pow(x,3)+8*x*pow(y,3))* m_Zernike[11] + 
		(1-6*x*x-6*y*y+6*pow(x,4)+6*pow(y,4)+12*x*x*y*y) * m_Zernike[12] +
		(3*x*x-3*y*y-4*pow(x,4)+4*pow(y,4)) * m_Zernike[13] + (pow(x,4)-6*x*x*y*y+pow(y,4)) * m_Zernike[14];
	return z;
}
