#pragma once
#include "WaveFrontConstructDoc.h"


// CInfoView 窗体视图

class CInfoView : public CFormView
{
	DECLARE_DYNCREATE(CInfoView)

protected:
	CInfoView();           // 动态创建所使用的受保护的构造函数
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()


public:
	virtual void OnInitialUpdate();
};


