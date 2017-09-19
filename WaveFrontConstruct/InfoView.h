#pragma once
#include "WaveFrontConstructDoc.h"


// CInfoView ������ͼ

class CInfoView : public CFormView
{
	DECLARE_DYNCREATE(CInfoView)

protected:
	CInfoView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CInfoView();

public:
	enum { IDD = IDD_DIALOG1 };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

public:
	CWaveFrontConstructDoc* GetDocument() const;
	void SetInfoZernike();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()


public:
	virtual void OnInitialUpdate();
};


