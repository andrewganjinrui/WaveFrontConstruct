// WaveFrontConstruct.h : WaveFrontConstruct Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������
#include "WaveFrontConstructDoc.h"


// CWaveFrontConstructApp:
// �йش����ʵ�֣������ WaveFrontConstruct.cpp
//

class CWaveFrontConstructApp : public CWinApp
{
public:
	CWaveFrontConstructApp();

	CWaveFrontConstructDoc* m_pDoc;


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CWaveFrontConstructApp theApp;