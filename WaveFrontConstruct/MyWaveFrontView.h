#pragma once


// CMyWaveFrontView 视图

class CMyWaveFrontView : public CListView
{
	DECLARE_DYNCREATE(CMyWaveFrontView)

protected:
	CMyWaveFrontView();           // 动态创建所使用的受保护的构造函数
	virtual ~CMyWaveFrontView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	void InitializeListView();
	void DisplayWaveFront();
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};


